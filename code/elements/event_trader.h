#ifndef ELEMENTS_EVENT_TRADER_H
#define ELEMENTS_EVENT_TRADER_H

#include <symbol_state.h>
#include <bb_manager.h>
#include <order_interface.h>
#include <params.pb.h>
#include <event_trader_utils.h>
#include <event_trader_fair_calculator.h>
#include <order_interface.h>

namespace elements
{

class event_trader
{

public:

    event_trader(
        params::EventTrader const & par_event_trader,
        params::Strategy const & par_strategy,
        oi::order_handler *order_handler, 
        std::vector<symbol_state *> s,
        std::string date) : _date(date), _order_handler(order_handler)
    {
   
        _fair_calculator = etfc_factory::get( par_event_trader ); 
 
        _event_id = par_event_trader.event_id();
        _event_trader_id = par_event_trader.event_trader_id();
        
        std::cout << "Initializing event trader event_id="  << _event_id 
                  << " event_trader_id=" << _event_trader_id << std::endl;       

        // INSTRUMENT 
        int num = par_event_trader.event_trader_instrument_size();
        for(int i = 0; i < num; ++i)
        {
            bool found = false;
            auto ins = par_event_trader.event_trader_instrument(i);
            for(auto state: s)
            {
                if(state->get_instrument()->symbol() == ins.ticker())  
                {
                    event_trader_instrument_state etis;
                    etis.ticker = ins.ticker();
                    etis.beta = ins.beta();
                    etis.max_position  = ins.max_position(); 
                    etis.penetration = ins.penetration();
                    etis.stop_loss = ins.stop_loss();
                    etis.stop_gain = ins.stop_gain();
                    etis.state = state;
                    found = true;
                    _states.push_back(etis);
                }      
            }

            if( found == false)
            {
                std::cout << "Cannot find event_trader instrument in instrument list " 
                        << ins.ticker() << std::endl;
                assert(false);
            }
        }

        // EVENT
        bool found = false; 
        num = par_strategy.eco_event_size();
        for(int i = 0; i < num; ++i)
        {
            auto ev = par_strategy.eco_event(i); 
            // std::cout << "COMPARE: " <<  ev.eco_event_id() << " " <<  _event_id << std::endl;
            
            if(ev.eco_event_id() == _event_id)
            {
                _event_time = make_time( _date, ev.time());

                _components.clear();
                
                int num_comp = ev.eco_component_size();
                for(int j = 0; j < num_comp; ++j)
                {
                    auto co = ev.eco_component(j);
                    _components.insert(co.bb_name()); 
                }

                // std::cout << "FOUND" << std::endl;
                found = true;
            }
        }

        if(found == false)
        {
            std::cout << "Cannot find event  " << _event_id << std::endl;
            assert(false);            
        }

        // VARIABLES
        num = par_event_trader.event_trader_variable_size();
        for(int i = 0; i < num; ++i)
        {
            event_trader_variable etv;

            auto var = par_event_trader.event_trader_variable(i);
            auto name = var.bb_name_match();
            if(_components.find(name) == _components.end())
            {
                std::cout << "Event Trader Uses non-configured event component: " << name << std::endl;
                assert(false);
            }
            
            if(var.has_survey_median())
                etv.survey_median = var.survey_median();    
 
            if(var.has_survey_low())
                etv.survey_low = var.survey_low();    
            
            if(var.has_survey_high())
                etv.survey_high = var.survey_high();    
            
            if(var.has_survey_average())
                etv.survey_average = var.survey_average();    
            
            if(var.has_last())
                etv.last = var.last();    
        
            _variables[name] = etv;
        }

        std::cout   << "Configured event_trader - ";
        print();

        // TODO prit variables
    }

    virtual ~event_trader(){}

    void print()
    {
        std::cout   << "Configured event_trader - "
                    << " _event_id=" << _event_id
                    << " _event_trader_id=" << _event_trader_id
                    << " _event_time=" << str(_event_time)
                    << " _date=" << _date;

        std::cout << " _components(";
        for(auto c:_components)
        {
            std::cout << c << ",";
        }
        std::cout << ") " << std::endl;

        std::cout << " _variables: ";
        for(auto v: _variables)
        {
            std::cout << "(" << v.first << ":" 
                << " survey_median=" << v.second.survey_median << ","
                << " survey_low=" << v.second.survey_low << ","
                << " survey_high=" << v.second.survey_high << ","
                << " survey_average=" << v.second.survey_average << ","
                << " last=" << v.second.last << ") ";
        }
        std::cout << std::endl;
    }

    void send_initial_orders()
    {
        _fair_calculator->fair(_states, _ev); 
        
        for(auto s:_states)
        {

            auto * ins = s.state->get_instrument();

            double price = s.state->get_snapshots()[0].microprice;
            double ret = price - s.ref_price;
            double ps = 1.0e-7;
            double mpi = (double) (ins->book().min_price_increment());
            double ret_ticks =  ret / mpi; 

            std::cout   << "SYMBOL_REACTION " << s.ticker << " "
                        << " mp="           << price * ps
                        << " ref="          << s.ref_price * ps
                        << " ret="          << ret * ps
                        << " ret_ticks="    << ret_ticks 
                        << " fair="         << s.fair * ps
                        << " react="        << s.dollar_reaction 
                        << " react_ticks="  << (s.fair - s.ref_price) / mpi 
                        << std::endl; 

            ins->book().print(std::cout, 10);
            std::cout << std::endl;

            oi::outgoing_order o;
            o.sec_id                = ins->secdef().SecurityID;
            o.order_type            = oi::OrderType::LIMIT; 
            o.tif                   = oi::TimeInForceType::FILL_OR_KILL; 
            o.owner                 = (int64_t)this;
            o.action                = oi::OrderActionType::NEW;
            o.size                  = s.max_position; 

            if(ret_ticks > 0)
            {
                o.side                  = oi::SideType::BUY; 
                o.price                 = ins->book().top_price(BookSide::ASK) + 
                                            s.penetration * ins->book().min_price_increment() ;
            }
            else
            {
                o.side                  = oi::SideType::SELL; 
                o.price                 = ins->book().top_price(BookSide::BID) - 
                                            s.penetration * ins->book().min_price_increment() ;
            }
            
            _order_handler->submit_order(o, false);
        }
    }

    void check_on_orders()
    {
        for(auto s:_states)
        {
            auto * ins = s.state->get_instrument();
            auto pnl = ins->pnl();
            auto pos = ins->pos();

            if(pos == 0)
                continue;

            double pnl_per_contract = pnl / (double)abs(pos);
            double pnl_ticks_per_contract = pnl_per_contract / (double)ins->dollars_per_tick();

            std::cout   << "CHECK_ON_ORDERS " << s.ticker
                        << " ppc=" << pnl_per_contract 
                        << " ptpc=" << pnl_ticks_per_contract 
                        << " pnl=" << pnl
                        << " pos=" << pos 
                        << std::endl;

            if( (pnl_ticks_per_contract >= s.stop_gain) || pnl_ticks_per_contract <= s.stop_loss)
            {
                oi::outgoing_order o;
                o.sec_id                = ins->secdef().SecurityID;
                o.order_type            = oi::OrderType::LIMIT; 
                o.tif                   = oi::TimeInForceType::FILL_OR_KILL; 
                o.owner                 = (int64_t)this;
                o.action                = oi::OrderActionType::NEW;
                o.size                  = abs(pos); 

                if(pos < 0 )
                {
                    o.side                  = oi::SideType::BUY; 
                    o.price                 = ins->book().top_price(BookSide::ASK);  
                }
                else
                {
                    o.side                  = oi::SideType::SELL; 
                    o.price                 = ins->book().top_price(BookSide::BID); 
                }
                _order_handler->submit_order(o, false);
            }
        }
    }

    virtual void on_timer(timestamp & t) 
    {
        if(!_has_event_happen)
            return;

        ++_timers_since_event; 

        if(_timers_since_event < 100)
        {
            for(auto s:_states)
            {
                std::cout << str(t) << "BOOK " << s.ticker  << std::endl;
                s.state->get_instrument()->book().print(std::cout, 10);
                std::cout << std::endl;
            }
        }

        if(_timers_since_event == 5)
            send_initial_orders();

        if(_timers_since_event > 10)
            check_on_orders();

    }

    virtual void on_bb_event(eco_event const &event)
    {
       //  std::cout << "Event Message received by event trader" << std::endl;

        if(event.event_id != _event_id)
            return;

        if(!event.is_all_sent)
            return;

        _has_event_happen = true;

        std::cout << str(event.ts) << " Event happen" << std::endl;

        _ev = event;

        for(auto &s:_states)
        {
            s.ref_price = s.state->get_snapshots()[5].microprice;
        } 
    } 

private:

    bool _has_event_happen = false;
    int _timers_since_event = 0;
    std::string _event_id = "";
    std::string _event_trader_id = "";
    timestamp   _event_time;
    std::string _date;
    std::set<std::string> _components;
    std::map<std::string, event_trader_variable> _variables;
    std::vector<event_trader_instrument_state> _states;
    eco_event _ev;
    event_trader_fair_calculator *_fair_calculator;
    oi::order_handler *_order_handler;
};

}

#endif // ELEMENTS_EVENT_TRADER_H
