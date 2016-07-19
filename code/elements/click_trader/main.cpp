#include "strategy.h" 
#include "params.pb.h"
#include <log_messages.h>
#include <order_stacker.h>
#include <easy_time.h>
#include <symbol_state.h>
#include <event_trader.h>


using namespace elements;

class my_strategy : public strategy<params::Params>
{
public:
    my_strategy(){}
    virtual ~my_strategy(){}

    virtual void on_bb_event(eco_event const &event) 
    {
        for(auto event_trader: _event_traders)
        {
            event_trader->on_bb_event(event);    
        }   
    }

    void init_event_traders()
    {
        auto &params = get_params_manager()->get_parameters().strategy();
        for(int i = 0; i < params.event_trader_size(); ++i)
            _event_traders.push_back( new event_trader( params.event_trader(i), params, this, _states, get_date()) );
    }
    
    virtual void on_start_of_day()
    {
        auto instruments = get_instruments();
        for( auto ins : instruments )
        {
            auto * state = new symbol_state(ins);
            _states.push_back(state);
            ins->set_void_pointer(state);
            _states2[ins->secdef().SecurityID] = _states.back();
        }

        _stacker_poll_interval = milliseconds(1000);
        auto start = now() + _stacker_poll_interval;
        auto stop  = make_time(get_date(), "23:59:00.000");
        _stacker_timer_id = register_timer(start, stop, _stacker_poll_interval); 
        assert(_stacker_timer_id >= 0);
        
        _sanity_poll_interval = seconds(60);
        _sanity_timer_id = register_timer(start, stop, _sanity_poll_interval); 
        assert(_sanity_timer_id >= 0);

        _snapshot_poll_interval = milliseconds(100);
        
        auto nanos = to_epoch_nanos(start);
        std::cout  << "start: "<< str(start)  << " " << nanos << std::endl;
        nanos = ( nanos / 100000000  + 1) * 100000000;
        auto start_on_boundary = from_epoch_nanos(nanos);
        std::cout  << "start_on_boundary: "<< str(start_on_boundary) << " " << nanos << std::endl;

        _snapshot_timer_id = register_timer(start_on_boundary, stop, _snapshot_poll_interval); 
        assert(_snapshot_timer_id >= 0);

        init_event_traders();
    }

    virtual void on_incremental_refresh(cme::IncrementalRefresh &msg) 
    {
        for(auto state: _states)
            state->on_market_data(now());
 
        for(auto entry: msg.MDEntries)
        {
            if(entry.MDEntryType != cme::MDEntryTypeEnum::TRADE)
                continue;

            if( entry.has_TradeCondition)
                continue;

            auto it = _states2.find( entry.SecurityID );
            if(it != _states2.end())
                it->second->on_trade(entry, now());
        }
    }

    virtual void on_security_status(cme::SecurityStatus &msg) 
    {
    }
 
    virtual void on_timer(timestamp & t, int32_t timer_id)
    {
        if(timer_id == _stacker_timer_id)
        {
            auto &params = get_params_manager()->get_parameters().strategy();
            if(!params.trading_enabled())
                return;

            auto instruments = get_instruments();
            for( auto ins : instruments )
            {
                int32_t sec_id = ins->secdef().SecurityID;
                auto par = params.instrument(ins->parameter_id()); 
                if(!par.trading_enabled())
                    continue;
                
                auto stacker = ins->get_order_stacker();
                if( stacker != nullptr && stacker->enabled() &&  ins->book().has_top())
                {
                    auto edge = par.stacker_edge() * ins->book().min_price_increment();

                    stacker->set_top(
                        ins->book().top_price(BookSide::BID) -  edge,
                        ins->book().top_price(BookSide::ASK) +  edge);
                    auto ts = now();
                    if(now() - _stacker_updates[sec_id] > seconds(par.stacker_update_interval()) )
                    {
                         stacker->refresh();
                         _stacker_updates[sec_id] = ts;
                    }
                }
                
                auto a_stacker = ins->get_auto_stacker();
                if( a_stacker != nullptr && a_stacker->enabled() &&  ins->book().has_top())
                {
                    auto ts = now();
                    if(now() - _auto_stacker_updates[sec_id] > seconds(par.auto_stacker_update_interval()) )
                    {
                        a_stacker->refresh( ins->book().top_price(BookSide::BID),ins->book().top_price(BookSide::ASK));
                        _auto_stacker_updates[sec_id] = ts;
                    }
                }
            }
        }
        else if(timer_id == _sanity_timer_id)
        {
            std::cout << "HEARTBIT: "<< str(now())<< std::endl;

            if(!check_order_sanity())
                std::cout << "INSANE ORDERS" << std::endl;
        }
        else if(timer_id == _snapshot_timer_id)
        {
            for (auto state: _states)
                state->on_timer(t);
            
            for(auto state: _states)
            {
                auto snapshots =  state->get_snapshots();
            }

            for(auto event_trader: _event_traders)
            {
                event_trader->on_timer(t);
            }
        }
    }

    virtual void on_order(timestamp ts, oi::order_event const & event)
    {
    }

    template <typename T1, typename T2>
    utility * get_utility(T1& stacker_par, T2& side_par)
    {
        if(side_par.ut_type() == "constant")
        {
            return new constant_utility( stacker_par.num_levels(), (int)side_par.ut_param1());
        }
        else if(side_par.ut_type() == "linear")
        {
            return new linear_utility(
                        stacker_par.num_levels(), 
                        side_par.ut_param1(),
                        side_par.ut_param2(),
                        side_par.ut_tolerance());
        }
        else if( side_par.ut_type() == "exponential")
        {
            return new exponential_utility(
                        stacker_par.num_levels(), 
                        side_par.ut_param1(),
                        side_par.ut_param2(),
                        side_par.ut_tolerance());
        }
        else if( side_par.ut_type() == "quadratic")
        {
            return new quadratic_utility(
                        stacker_par.num_levels(), 
                        side_par.ut_param1(),
                        side_par.ut_param2(),
                        side_par.ut_tolerance());
        }
        else
        {
            return nullptr;
        }
    }

    virtual void on_parameters_update(params::Params & msg) 
    {
        auto &params = get_params_manager()->get_parameters().strategy();
        auto instruments = get_instruments();
        for( auto ins : instruments )
        {
            int32_t sec_id = ins->secdef().SecurityID;
            auto par = params.instrument(ins->parameter_id()); 
            order_stacker *  stacker = ins->get_order_stacker();

            if( params.trading_enabled() &&  par.trading_enabled() && par.enable_stacker())
            {
                auto & stacker_params =  par.stacker_config( par.stacker_config_id() );
                if( stacker == nullptr )
                {
                    stacker = new order_stacker( sec_id,this,get_order_manager()->get_order_book(sec_id), ins->book().min_price_increment() );
                    ins->set_order_stacker(stacker);
                }

                order_stacker_params sp;
                sp.num_levels = stacker_params.num_levels();
                sp.max_order_size = stacker_params.max_order_size();
                sp.skip_mask = stacker_params.skip_mask();
                sp.bid_utility = get_utility(stacker_params, stacker_params.bid_config());
                sp.ask_utility = get_utility(stacker_params, stacker_params.ask_config());
                    
                auto edge = par.stacker_edge() * ins->book().min_price_increment();
                sp.top_bid = ins->book().top_price(BookSide::BID) - edge;
                sp.top_ask = ins->book().top_price(BookSide::ASK) + edge;

                if(!stacker->enabled() || !stacker->get_params().compare(sp))
                {
                    stacker->set_params(&sp);
                    std::cout << sp << std::endl;
                    stacker->enable();
                    _stacker_updates[sec_id] = now();
                }
            }
            else if(   stacker == nullptr 
                    || !params.trading_enabled() 
                    || !par.trading_enabled()
                    || !par.enable_stacker() )
            {
                if( stacker != nullptr )
                {
                    stacker->disable();
                    delete stacker;
                }
                ins->set_order_stacker(nullptr);
            }

            if(!par.has_enable_auto_stacker())
                continue;

            auto_stacker *  a_stacker = ins->get_auto_stacker();

            if( params.trading_enabled() &&  par.trading_enabled() && par.enable_auto_stacker())
            {
                auto & a_stacker_params =  par.auto_stacker_config();
                if( a_stacker == nullptr )
                {
                    a_stacker = new auto_stacker( sec_id, this, 
                        get_order_manager()->get_order_book(sec_id), 
                        ins->book().min_price_increment() );
                    ins->set_auto_stacker(a_stacker);
                }

                auto_stacker_params sp;
                sp.num_levels = a_stacker_params.num_levels();
                sp.num_visible_levels = a_stacker_params.num_visible_levels();
                sp.max_order_size = a_stacker_params.max_order_size();
                sp.skip_mask = a_stacker_params.skip_mask();
                sp.initial_edge = a_stacker_params.initial_edge();
                sp.ask_replenish_edge = a_stacker_params.ask_replenish_edge(); 
                sp.ask_replenish = a_stacker_params.ask_replenish();
                sp.ask_replenish_size = a_stacker_params.ask_replenish_size();     
                sp.bid_replenish_edge = a_stacker_params.bid_replenish_edge(); 
                sp.bid_replenish = a_stacker_params.bid_replenish();
                sp.bid_replenish_size = a_stacker_params.bid_replenish_size();     
                sp.bid_utility = get_utility(a_stacker_params, a_stacker_params.bid_config());
                sp.ask_utility = get_utility(a_stacker_params, a_stacker_params.ask_config());
                sp.reference_price = ins->book().top_price(BookSide::BID);

                if(!a_stacker->enabled())
                {
                    a_stacker->set_params(&sp);
                    a_stacker->enable();
                    _auto_stacker_updates[sec_id] = now();
                }
                else
                {
                    a_stacker->set_ask_replenish_edge(sp.ask_replenish_edge); 
                    a_stacker->set_ask_replenish(sp.ask_replenish);
                    a_stacker->set_ask_replenish_size(sp.ask_replenish_size);     
                    a_stacker->set_bid_replenish_edge(sp.bid_replenish_edge); 
                    a_stacker->set_bid_replenish(sp.bid_replenish);
                    a_stacker->set_bid_replenish_size(sp.bid_replenish_size);     
                    a_stacker->set_num_visible_levels(sp.num_visible_levels);     
                }
            }
            else if(   a_stacker == nullptr 
                    || !params.trading_enabled() 
                    || !par.trading_enabled()
                    || !par.enable_auto_stacker() )
            {
                if( a_stacker != nullptr )
                {
                    a_stacker->disable();
                    delete a_stacker;
                }
                ins->set_auto_stacker(nullptr);
            }
        }
    }

private:

    log_message _log_msg;
    timestamp _stop;

    milliseconds _stacker_poll_interval;
    milliseconds _sanity_poll_interval;
    milliseconds _snapshot_poll_interval;

    int32_t _stacker_timer_id = -1;
    int32_t _sanity_timer_id = -1;
    int32_t _snapshot_timer_id = -1;

    std::map<int32_t, timestamp> _stacker_updates; 
    std::map<int32_t, timestamp> _auto_stacker_updates; 
    std::vector<symbol_state *> _states;
    std::map<int32_t, symbol_state * > _states2; 

    std::vector<event_trader *> _event_traders;
};

int main( int argc, char ** argv)
{
    std::string  par = (argc > 1) ? argv[1] : "strategy.param";
    my_strategy handler;
    my_strategy::EventLoopType e;
    e.initialize(par, handler);
    e.run();
    return 0;
}
