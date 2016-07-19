#ifndef ELEMENTS_STRATEGY_h
#define ELEMENTS_STRATEGY_H

#include <eloop.h>
#include <instrument.h>
#include <md.h>
#include <md_translate.h>
#include <cme_fill_simulator.h>
#include <order_interface.h>
#include <log_messages.h>
#include <order_entry_manager.h>
#include <algorithm>

namespace elements
{

template <typename ParamMsg>
class strategy : public oi::order_handler  
{
public:

    strategy(){}
    virtual ~strategy(){}

    typedef strategy<ParamMsg>                              StrategyType;
    typedef eloop<ParamMsg,StrategyType>                    EventLoopType;
    typedef parameters_manager<ParamMsg,StrategyType>       ParametersManagerType;
    typedef timers_manager <StrategyType,EventLoopType>     TimersManagerType;
    typedef feed_manager <StrategyType>                     FeedManagerType;
 
    // USER methods
    virtual void on_start_of_day() {}

    virtual void on_parameters_update(ParamMsg& msg) {}

    virtual bool validate_parameters(ParamMsg &msg) { return true; }
    virtual void on_timer(timestamp & t, int32_t timer_id) = 0; 
    virtual void on_incremental_refresh(cme::IncrementalRefresh &msg) {}
    virtual void on_security_definition(cme::SecurityDefinition &msg) {}
    virtual void on_security_status(cme::SecurityStatus &msg) {}
    virtual void on_order(timestamp ts, oi::order_event const & event){}
    virtual void on_terminate() { std::cout << "TERMINATED" << std::endl; }
    virtual void on_bb_event(eco_event const &event) {} 
    
    virtual int64_t submit_order(oi::outgoing_order &order, bool isManual = false) 
    {
        if(!trading_enabled() && order.action != oi::OrderActionType::CANCEL )
            return -1;

        if(!trading_enabled(order.sec_id) && order.action != oi::OrderActionType::CANCEL )
            return -1;

        return get_order_manager()->submit(order, isManual); 
    }
    
    // Accessors
    EventLoopType                   * get_event_loop()        { return _event_loop; }
    ParametersManagerType           * get_params_manager()    { return _params_manager; }
    TimersManagerType               * get_timers_manager()    { return _timers_manager; }
    FeedManagerType                 * get_feed_manager()      { return _feed_manager; }
    std::vector<instrument *>       & get_instruments()       { return _event_loop->instruments(); }
    std::map<int32_t,int32_t>       * get_instruments_map()   { return _event_loop->instruments_map(); }
    std::map<std::string,int32_t>   * get_symbol_map()        { return _event_loop->symbol_map(); }
    
    instrument                      * get_instrument_by_id(int32_t id)
    {
        auto im = get_instruments_map();
        auto i = get_instruments();
        auto it = im->find(id);
        if(it == im->end())
            return nullptr;
        else
            return i[it->second];
    }  

    int32_t get_instrument_index(int32_t id)
    {
        return get_instrument_by_id(id)->parameter_id();
    }

    oi::order_manager_interface * get_order_manager()     { return _order_manager; }   
    
    // Adapters
    std::string get_date() 
    {
        return _event_loop->get_date();
    }

    uint32_t register_timer(timestamp start) 
    { 
        return _timers_manager->register_timer(start); 
    }
    
    uint32_t register_timer(timestamp start, timestamp stop, milliseconds interval) 
    { 
        return _timers_manager->register_timer(start, stop, interval);
    }
   
    timestamp now() 
    {
        return _event_loop->now();
    }
 
    template <typename T>
    void log(std::string prefix, T msg)
    {
        _event_loop->log(prefix, msg);
    }

    oob_logger<log_message> * get_oob_logger(){ return _event_loop->get_oob_logger(); }

    // CALL this when done
    void terminate() { _event_loop->terminate(); }
    
    // Do not look

    void on_before_eloop_initialize(){}
    void on_after_eloop_initialize()
    {
        timespec ts = to_timespec(now()); 
        ts.tv_nsec = 0;
        auto start = from_timespec(ts);
        auto stop  = make_time(get_date(), "23:59:00.000");
        auto params =  get_params_manager()->get_parameters().strategy(); 
        _status_timer_id = register_timer(start, stop, seconds(params.status_interval()));
 
        on_start_of_day();
    }
    
    template <typename Hdr > void operator()(Hdr & hdr, elements::SecurityDefinition &msg)
    {
        assert(translate( &hdr, &msg, &_secdef));
        on_security_definition(_secdef);
    }
    
    template <typename Hdr > void operator()(Hdr & hdr, elements::MarketDataIncrementalRefresh &msg)
    {
        assert(translate( &hdr, &msg, &_incremental_refresh));
        assert(now()==_incremental_refresh.ReceivedTime);
        on_incremental_refresh_raw(_incremental_refresh);
    }

    template <typename Hdr > void operator()(Hdr & hdr, elements::SecurityStatus &msg)
    {
        assert(translate( &hdr, &msg, &_security_status));
        on_security_status(_security_status);
    }

    template <typename Hdr, typename Msg > void operator()(Hdr & hdr, Msg &msg){ }

    void on_incremental_refresh_raw(elements::cme::IncrementalRefresh &msg) 
    {
        // std::cout << msg << std::endl;
        auto instruments = get_instruments();
        for(auto ins: instruments)
        {
            ins->unsafe_book().on_incremental_refresh(msg);
            if(ins->maintain_implied())
                ins->unsafe_implied_book().on_incremental_refresh(msg);
        }
        if(_order_manager != nullptr) _order_manager->on_market_changed(); 
        on_incremental_refresh(msg); 
    }
    
    void on_security_definition_raw(elements::cme::SecurityDefinition &msg) 
    {
        on_security_definition(msg);
    }
    
    void on_security_status_raw(elements::cme::SecurityStatus &msg) 
    {
        on_security_status(msg);
    }
    
    void on_timer_raw(timestamp & t, int32_t timer_id) 
    {
        if(timer_id != _status_timer_id)
        {
            on_timer(t,timer_id);
            return;
        }     

        auto  instruments = get_instruments();
        for(auto  ins: instruments)
        {
            if(!ins->book().has_top())
                continue;
            
            if(get_params_manager()->get_parameters().strategy().enable_oob_log())
            {
                _log_msg.type = MSG_TYPE::MSG_PNL_STATUS;
                _log_msg.uber.a_pnl_status.epoch = to_epoch_nanos(t);
                strcpy( _log_msg.uber.a_pnl_status.sym, ins->symbol().c_str());

                int64_t bid = ins->book().top_price(BookSide::BID);
                int64_t ask = ins->book().top_price(BookSide::ASK);

                _log_msg.uber.a_pnl_status.pnl = ins->pnl();
                _log_msg.uber.a_pnl_status.bid = (double)bid * gpm::amd;
                _log_msg.uber.a_pnl_status.ask = (double)ask * gpm::amd;
                _log_msg.uber.a_pnl_status.bsz = ins->book().size_at_price(bid, BookSide::BID);
                _log_msg.uber.a_pnl_status.asz = ins->book().size_at_price(ask, BookSide::ASK);
                _log_msg.uber.a_pnl_status.pos = ins->pos();
                _log_msg.uber.a_pnl_status.vol = ins->vol();
                _log_msg.uber.a_pnl_status.mp = ins->book().microprice() * gpm::amd;
                _log_msg.uber.a_pnl_status.cash = ins->cash();
                get_oob_logger()->log(_log_msg);

                auto ordman = get_order_manager();
                if(ordman != nullptr)
                {
                    auto obp  = ordman->get_order_book(ins->secdef().SecurityID); 
                    _log_msg.type = MSG_TYPE::MSG_ORDER_BOOK_ENTRY;
                
                    _log_msg.uber.a_order_book_entry.progress = EVENT_PROGRESS::MIDDLE;
                    
                    strcpy( _log_msg.uber.a_order_book_entry.sym, ins->symbol().c_str());
                    _log_msg.uber.a_order_book_entry.side = ORDER_SIDE::SELL;
                    for(auto level: obp->ask.levels)
                    {
                        _log_msg.uber.a_order_book_entry.price = level.first * gpm::amd;
                        for(auto order : level.second.orders)
                        {
                            _log_msg.uber.a_order_book_entry.size = order.data->original.size;
                            _log_msg.uber.a_order_book_entry.id = order.order_id;
                            get_oob_logger()->log(_log_msg);
                        } 
                    }

                    _log_msg.uber.a_order_book_entry.side = ORDER_SIDE::BUY;
                    for(auto &level: obp->bid.levels)
                    {
                        _log_msg.uber.a_order_book_entry.price = level.first * gpm::amd;
                        for(auto &order : level.second.orders)
                        {
                            _log_msg.uber.a_order_book_entry.size = order.data->original.size;
                            _log_msg.uber.a_order_book_entry.id = order.order_id;
                            get_oob_logger()->log(_log_msg);
                        } 
                    }
                }

                int64_t mpi = ins->book().min_price_increment();
                _log_msg.type = MSG_TYPE::MSG_MD_BOOK;
                _log_msg.uber.a_md_book.epoch = to_epoch_nanos(t);
                strcpy( _log_msg.uber.a_md_book.sym, ins->symbol().c_str());
                _log_msg.uber.a_md_book.top_bid_price = (double)bid * gpm::amd;
                _log_msg.uber.a_md_book.top_ask_price = (double)ask * gpm::amd;
                _log_msg.uber.a_md_book.mpi = (double)mpi * gpm::amd;

                _log_msg.uber.a_md_book.a0 = ins->book().size_at_price (ask + mpi * 0, BookSide::ASK);
                _log_msg.uber.a_md_book.a1 = ins->book().size_at_price (ask + mpi * 1, BookSide::ASK);
                _log_msg.uber.a_md_book.a2 = ins->book().size_at_price (ask + mpi * 2, BookSide::ASK);
                _log_msg.uber.a_md_book.a3 = ins->book().size_at_price (ask + mpi * 3, BookSide::ASK);
                _log_msg.uber.a_md_book.a4 = ins->book().size_at_price (ask + mpi * 4, BookSide::ASK);
                _log_msg.uber.a_md_book.a5 = ins->book().size_at_price (ask + mpi * 5, BookSide::ASK);
                _log_msg.uber.a_md_book.a6 = ins->book().size_at_price (ask + mpi * 6, BookSide::ASK);
                _log_msg.uber.a_md_book.a7 = ins->book().size_at_price (ask + mpi * 7, BookSide::ASK);
                _log_msg.uber.a_md_book.a8 = ins->book().size_at_price (ask + mpi * 8, BookSide::ASK);
                _log_msg.uber.a_md_book.a9 = ins->book().size_at_price (ask + mpi * 9, BookSide::ASK);
                
                _log_msg.uber.a_md_book.b0 = ins->book().size_at_price (bid - mpi * 0, BookSide::BID);
                _log_msg.uber.a_md_book.b1 = ins->book().size_at_price (bid - mpi * 1, BookSide::BID);
                _log_msg.uber.a_md_book.b2 = ins->book().size_at_price (bid - mpi * 2, BookSide::BID);
                _log_msg.uber.a_md_book.b3 = ins->book().size_at_price (bid - mpi * 3, BookSide::BID);
                _log_msg.uber.a_md_book.b4 = ins->book().size_at_price (bid - mpi * 4, BookSide::BID);
                _log_msg.uber.a_md_book.b5 = ins->book().size_at_price (bid - mpi * 5, BookSide::BID);
                _log_msg.uber.a_md_book.b6 = ins->book().size_at_price (bid - mpi * 6, BookSide::BID);
                _log_msg.uber.a_md_book.b7 = ins->book().size_at_price (bid - mpi * 7, BookSide::BID);
                _log_msg.uber.a_md_book.b8 = ins->book().size_at_price (bid - mpi * 8, BookSide::BID);
                _log_msg.uber.a_md_book.b9 = ins->book().size_at_price (bid - mpi * 9, BookSide::BID);
                _log_msg.uber.a_md_book.pos = ins->pos();
                _log_msg.uber.a_md_book.last_trade_price = (double)ins->ltp() * gpm::amd;

                get_oob_logger()->log(_log_msg);
            }
        }
    }

    virtual void on_order_event(timestamp ts, oi::order_event const & event)
    {
        instrument *ins = get_instrument_by_id(event.original->sec_id);

        if(get_params_manager()->get_parameters().strategy().enable_oob_log())
        {
            _log_msg.type = MSG_TYPE::MSG_ORDER_EVENT;
            _log_msg.uber.a_order_event.epoch = to_epoch_nanos(ts);
            strcpy( _log_msg.uber.a_order_event.sym, ins->symbol().c_str());

            switch(event.event_type)
            {
            case oi::OrderEventType::NEW_SUB:
                _log_msg.uber.a_order_event.type = ORDER_EVENT::NEW_SUB;
                break;
            case oi::OrderEventType::CANCEL_SUB:
                _log_msg.uber.a_order_event.type = ORDER_EVENT::CANCEL_SUB;
                break;
            case oi::OrderEventType::MODIFY_SUB:
                _log_msg.uber.a_order_event.type = ORDER_EVENT::MODIFY_SUB;
                break;
            case oi::OrderEventType::NEW_ACK:
                _log_msg.uber.a_order_event.type = ORDER_EVENT::NEW_ACK;
                break;
            case oi::OrderEventType::CANCEL_ACK:
                _log_msg.uber.a_order_event.type = ORDER_EVENT::CANCEL_ACK;
                break;
            case oi::OrderEventType::MODIFY_ACK:
                _log_msg.uber.a_order_event.type = ORDER_EVENT::MODIFY_ACK;
                break;
            case oi::OrderEventType::FILL:
                _log_msg.uber.a_order_event.type = ORDER_EVENT::FILL;
                break;
            case oi::OrderEventType::PARTIAL_FILL:
                _log_msg.uber.a_order_event.type = ORDER_EVENT::PARTIAL_FILL;
                break;
            case oi::OrderEventType::EXPIRED:
                _log_msg.uber.a_order_event.type = ORDER_EVENT::EXPIRED;
                break;
            case oi::OrderEventType::NEW_REJECT:
                _log_msg.uber.a_order_event.type = ORDER_EVENT::NEW_REJECT;
                break;
            case oi::OrderEventType::CANCEL_REJECT: 
                _log_msg.uber.a_order_event.type = ORDER_EVENT::CANCEL_REJECT;
                break;
            case oi::OrderEventType::MODIFY_REJECT:
                _log_msg.uber.a_order_event.type = ORDER_EVENT::MODIFY_REJECT;
                break;
            };
            
            switch(event.original->action)
            {
            case oi::OrderActionType::NEW:
                _log_msg.uber.a_order_event.action = ORDER_ACTION::NEW;
                break;
            case oi::OrderActionType::MODIFY:
                _log_msg.uber.a_order_event.action = ORDER_ACTION::MODIFY;
                break;
            case oi::OrderActionType::CANCEL:
                _log_msg.uber.a_order_event.action = ORDER_ACTION::CANCEL;
               break;
            };

            _log_msg.uber.a_order_event.order_id = event.original->order_id;
            _log_msg.uber.a_order_event.sec_id = event.original->sec_id;
            _log_msg.uber.a_order_event.side = (event.original->side == oi::SideType::BUY) 
                ? ORDER_SIDE::BUY : ORDER_SIDE::SELL;
            _log_msg.uber.a_order_event.price = (double)event.original->price * gpm::amd;
            _log_msg.uber.a_order_event.size = event.original->size;
            _log_msg.uber.a_order_event.tif = (event.original->tif == oi::TimeInForceType::DAY) 
                ? ORDER_TIF::DAY : ORDER_TIF::FILL_OR_KILL;
            _log_msg.uber.a_order_event.last_qty = event.updated->LastQty;
            _log_msg.uber.a_order_event.leaves_qty = event.updated->LeavesQty;
            _log_msg.uber.a_order_event.last_px =(double) event.updated->LastPx * gpm::amd;
            get_oob_logger()->log(_log_msg);
        }       
 
        switch(event.event_type) 
        {
        case oi::OrderEventType::FILL:
        case oi::OrderEventType::PARTIAL_FILL:
            if(ins != nullptr)
            {
                int32_t sign = (event.original->side ==oi::SideType::BUY )? 1 : -1;
                int32_t size = event.updated->LastQty * sign;
                int64_t price = event.updated->LastPx;
                assert(size != 0); 
                ins->on_fill( size, price); 
            }
            break;
        default:
            break;        
        } 

        on_order(ts, event);
    }

    void on_order_entry(oem_ticket & ticket)
    {
        auto om = get_order_manager();
        oi::outgoing_order order;
   
        switch(ticket.action)
        {
        case OEM_NEW:
            order.action = oi::OrderActionType::NEW;
            break;
        case OEM_MODIFY:
            order.action = oi::OrderActionType::MODIFY;
            break;
        case OEM_CANCEL:
            order.action = oi::OrderActionType::CANCEL;
            break;
        default:
            std::cout << "unknown order action" << std::endl;
            return;
        };

        order.order_id = ticket.order_id;
        
        auto symbol_map = get_symbol_map();
        auto it = symbol_map->find(ticket.symbol);
        if(it == symbol_map->end())  
        {
            std::cout << "trying to trade unknown symbol " << ticket.symbol << std::endl;
            return;
        }

        order.sec_id = it->second;
        if(ticket.is_bid)
            order.side = oi::SideType::BUY;
        else
            order.side = oi::SideType::SELL;
        
        order.price = ticket.price;
        order.size = ticket.size;
        order.order_type = oi::OrderType::LIMIT; 
        if(ticket.is_ioc)
            order.tif = oi::TimeInForceType::FILL_OR_KILL;
        else
            order.tif = oi::TimeInForceType::DAY;        
        order.ts = now();

        submit_order(order, true);
    }

    bool trading_enabled()
    {
        if(get_params_manager()->get_parameters().strategy().trading_enabled())
            return true;
        else
            return false;
    }
    
    bool trading_enabled(int32_t sec_id)
    {
        int32_t idx = get_instrument_index(sec_id);
        if(idx < 0)
            return false;
        
        if(get_params_manager()->get_parameters().strategy().instrument(idx).trading_enabled())
            return true;
        else
            return false;
    }
   
    void cancel_orders()
    {
        auto ordman = get_order_manager();
        if(ordman == nullptr)
            return;

        auto  instruments = get_instruments();
        for(auto ins: instruments)
        {
            int32_t sec_id = ins->secdef().SecurityID;
            std::set<int64_t> orders_to_cancel;

            auto bk  = ordman->get_order_book(sec_id); 

            for( auto  & level : bk->ask.levels )
                for(auto & order : level.second.orders)
                    orders_to_cancel.insert(order.data->original.order_id);
       
            for( auto  & level : bk->bid.levels )
                for(auto & order : level.second.orders)
                    orders_to_cancel.insert(order.data->original.order_id);

            for(auto id : orders_to_cancel)
            {
                oi::outgoing_order o;
                o.action = oi::OrderActionType::CANCEL;
                o.sec_id = sec_id;
                o.order_id = id;
                submit_order(o);
            }
        }
    }

    void cancel_orders(int32_t sec_id)
    {
        auto ordman = get_order_manager();
        if(ordman == nullptr)
            return;

        std::set<int64_t> orders_to_cancel;
        
        auto bk  = ordman->get_order_book(sec_id); 
        for( auto  & level : bk->ask.levels )
            for(auto & order : level.second.orders)
                orders_to_cancel.insert(order.data->original.order_id);
       
        for( auto  & level : bk->bid.levels )
            for(auto & order : level.second.orders)
                orders_to_cancel.insert(order.data->original.order_id);

        for(auto id : orders_to_cancel)
        {
            oi::outgoing_order o;
            o.action = oi::OrderActionType::CANCEL;
            o.sec_id = sec_id;
            o.order_id = id;
            submit_order(o);
        }
    }
    
    void on_parameters_update_raw(ParamMsg& msg) 
    {
        auto &par = get_params_manager()->get_parameters().strategy();     
 
        if(!par.trading_enabled())
        {
            cancel_orders(); 
        }
        else
        {
            int i = 0;
            auto  instruments = get_instruments();
            for(auto ins: instruments)
            {
                int32_t sec_id = ins->secdef().SecurityID;
                if(!par.instrument(ins->parameter_id()).trading_enabled())
                    cancel_orders(sec_id);
                ++i;
            }
        }

        on_parameters_update(msg); 
    }

    std::set<int64_t> open_orders(int32_t sec_id)
    {
        std::set<int64_t> orders;
        auto ordman = get_order_manager();
        if(ordman == nullptr)
            return orders;

        auto bk  = ordman->get_order_book(sec_id); 
        for( auto  & level : bk->ask.levels )
            for(auto & order : level.second.orders)
                orders.insert(order.data->original.order_id);
        for( auto  & level : bk->bid.levels )
            for(auto & order : level.second.orders)
                orders.insert(order.data->original.order_id);
        return orders;
    }

    std::set<int64_t> open_orders()
    {
        std::set<int64_t> orders;
        auto  instruments = get_instruments();
        for(auto ins: instruments)
        {
            auto o = open_orders(ins->secdef().SecurityID);
            orders.insert(o.begin(), o.end());
        }
        return orders;
    }


    bool check_order_sanity()
    {
        auto set1 = open_orders();
        auto set2 = get_order_manager()->open_orders();

        //std::set<int64_t> diff1;
        //std::set<int64_t> diff2;

        //std::set_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), diff1 );
        //std::set_difference(set2.begin(), set2.end(), set1.begin(), set1.end(), diff2 );

        //for(auto id: diff1)
        //    std::cout << "order diff1: " << id << std::endl;
        //for(auto id: diff2)
        //    std::cout << "order diff2: " << id << std::endl;

        // if(diff1.size() == 0 && diff2.size() == 0)
        if(set1.size() == set2.size() )
            return true;
        else
            return false;
    }
 
    void set_event_loop(EventLoopType *p) { _event_loop = p; }
    void set_parameters_manager(ParametersManagerType *p) { _params_manager = p; }
    void set_timers_manager(TimersManagerType *p) { _timers_manager = p; }
    void set_feed_manager(FeedManagerType *p) { _feed_manager = p; }
    void set_order_manager(oi::order_manager_interface *p) { _order_manager = p; }


protected:

    EventLoopType                   *_event_loop = nullptr;
    ParametersManagerType           *_params_manager = nullptr;
    TimersManagerType               *_timers_manager = nullptr;
    FeedManagerType                 *_feed_manager = nullptr;
    oi::order_manager_interface     *_order_manager = nullptr; 
    cme::IncrementalRefresh         _incremental_refresh;
    cme::SecurityStatus             _security_status;
    cme::SecurityDefinition         _secdef;

    int32_t                         _status_timer_id;
    log_message                     _log_msg;
};

}

#endif // ELEMENTS_STRATEGY_H
