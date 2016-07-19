#ifndef CME_ORDER_MANAGER_H
#define CME_ORDER_MANAGER_H

#include <order_interface.h>
#include <TradingManager.h>
#include <order_book.h>

namespace elements
{

class cme_order_manager : public elements::oi::order_manager_interface
{
public:
   
     cme_order_manager(
        oi::order_handler *handler,
        std::vector<instrument*> *instruments,
        std::string config_file, 
        oob_logger<log_message> * oob
        ) :
            _tm(new greyjoy::TradingManager(config_file, true)),
            _mIter(_tm->getEventMessages()),
            _sIter(_tm->getSystemEventMessages()),
            _oob_logger(oob)
    {
        std::cout << "Entering constructor" << std::endl;
        _handler = handler;
        _instruments = instruments;       
        _storage.reserve(_max_orders);
        for(auto ins : *_instruments)       
            _orders[ins->secdef().SecurityID] = order_book_type(ins);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        std::cout << "flushing ... " << std::endl;
        flush();
        std::cout << "flushing ... " << std::endl;
        flush();
        std::cout << "finished flushing ... " << std::endl;
    }

    virtual ~cme_order_manager()
    {
        delete _tm;
    }
    
    virtual int64_t submit(oi::outgoing_order & order, bool isManual)
    {
        int64_t ret = -1;
        
        greyjoy::Side::Enum side;
        greyjoy::TimeInForce::Enum tif;
        oi::order_bag *bag;

        auto it = _orders.find(order.sec_id);
        if(it == _orders.end())
        {
            std::cout << "submit: no book for instrument id " << order.sec_id << std::endl;
            return -1;
        }

        order.ts = _handler->now();

        uint64_t t0,t1;

        switch(order.action)
        {
        case oi::OrderActionType::NEW:

            side = (order.side == oi::SideType::BUY) ? greyjoy::Side::BUY : greyjoy::Side::SELL;
           
            tif = greyjoy::TimeInForce::DAY;
            if(order.tif == oi::TimeInForceType::FILL_OR_KILL)
                tif = greyjoy::TimeInForce::FILL_OR_KILL;
            else if( order.tif == oi::TimeInForceType::DAY)
                tif = greyjoy::TimeInForce::DAY;
            else
            {
                std::cout << "Order type not supported" << std::endl;
                assert(false);
            }
 
            if( !isManual)
                ret = _tm->sendNewOrder(order.sec_id, 
                                        side, 
                                        greyjoy::OrderType::LIMIT,
                                        order.price, 
                                        order.size,
                                        order.size,
                                        tif,
                                        0,
                                        greyjoy::SessionSendType::Enum::ROUND_ROBIN );
            else     
                ret = _tm->sendManualNewOrder(
                                        order.sec_id, 
                                        side,
                                        greyjoy::OrderType::LIMIT,
                                        order.price,
                                        order.size,
                                        order.size,
                                        tif,
                                        0,
                                        "EGROIS",
                                        greyjoy::SessionSendType::Enum::ROUND_ROBIN );
          
            if(_id_to_index.find(ret) != _id_to_index.end() )
            {
                std::cout << "Duplicate order id " << std::endl;
                return -1;
            }
            
            _id_to_index[ret]   = _storage.size(); 
            _storage.resize(_storage.size() + 1);
            bag = &_storage[_storage.size() - 1];
 
            order.order_id = ret;
            bag->original = order;

            _event.original     = &(bag->original);
            _event.updated      = &(bag->updated);
            _event.event_type   = oi::OrderEventType::NEW_SUB;


           
            t0 = _tsc(); 
            it->second.add_order(   bag->original.order_id,
                                    (bag->original.side == oi::SideType::BUY),
                                    bag->original.price,
                                    bag->original.ts,
                                    bag,
                                    bag->original.owner ); 
            t1 = _tsc();
            // std::cout << "sending new " << order.order_id  << std::endl;
            // std::cout << order << std::endl;
            break;

        case oi::OrderActionType::MODIFY:
           
            t0 = _tsc(); 

            if(!isManual)
                ret =  _tm->sendReplaceOrder( order.order_id, order.price, order.size);
            else
                ret =  _tm->sendManualReplaceOrder( order.order_id, order.price, order.size, "EGROIS");

            t1 = _tsc();
             //greyjoy::SessionSendType::Enum::ROUND_ROBIN );
            // std::cout << "sending modify " << order.order_id << std::endl;
            // std::cout << order << std::endl;
            break;

        case oi::OrderActionType::CANCEL:
            t0 = _tsc();  
            if(!isManual)
	            ret = _tm->sendCancelOrder(order.order_id);
            else
	            ret = _tm->sendManualCancelOrder(order.order_id, "EGROIS");
                
            t1 = _tsc();  
            // std::cout << "sending cancel " << order.order_id << std::endl;
            // std::cout << order << std::endl;
            break;
        };

        
        auto epoch = to_epoch_nanos(order.ts);
        _log_msg.type = MSG_TYPE::MSG_LATENCY;
        _log_msg.uber.a_latency.epoch = epoch;
        _log_msg.uber.a_latency.nanos = _tsc.nanos(t0,t1);
        _log_msg.uber.a_latency.type = LATENCY_TYPE::LT_CME_OMS2;
        _oob_logger->log(_log_msg);

        return ret;
    } 
    
    virtual void on_market_changed()
    {
    }

    virtual order_book_type * get_order_book( int32_t sec_id )
    { 
        auto it = _orders.find(sec_id);
        if(it == _orders.end())
            return nullptr;
        return &it->second;
    }

    void poll()
    {
        //std::cout << "before_pause " << str(clock::now()) << std::endl;
        _sIter.pause();
        //std::cout << "after_pause " << str(clock::now()) << std::endl;
        //int i = 0;
        while( ( _s_msg = _sIter.nextMessage() ) != nullptr)
        {
            //if (i > 1000)
            //{
            //    std::cout << "exceed 1000 a" << std::endl;
            //    break;
            //}

            greyjoy::SystemEventStruct p = (*_s_msg);
            // std::cout << " System Event = " << p.toString() << std::endl;
            process_system_event(p);
            //++i;
        }
        //std::cout << "before_resume " << str(clock::now()) << std::endl;
        _sIter.resume();
       // std::cout << "after_resume " << str(clock::now()) << std::endl;

        _mIter.pause();
        //std::cout << "after_pause2 " << str(clock::now()) << std::endl;

        //i = 0;
        while( ( _msg = _mIter.nextMessage() ) != nullptr)
        {
            //if (i > 1000)
            //{
            //    std::cout << "exceed 1000 b" << std::endl;
            //    break;
            //}
            greyjoy::VenueEvent p = (*_msg);
            // std::cout << " Venue Event = " << p->toString() << std::endl;
            process_venue_event(&p);
           // ++i;
        }
        //std::cout << "before_resume2 " << str(clock::now()) << std::endl;
        _mIter.resume();
        //std::cout << "after_resume2 " << str(clock::now()) << std::endl;
    } 

    void flush()
    {
        _sIter.pause(); 
        while( ( _s_msg = _sIter.nextMessage() ) != nullptr ) {}
        _sIter.resume();
        
        _mIter.pause();
        while( ( _msg = _mIter.nextMessage() ) != nullptr ) {}
        _mIter.resume();
    }

    void process_system_event(greyjoy::SystemEventStruct &p)
    {
        
        std::cout   << str(clock::now()) << " " << p.toString() << std::endl;

        switch(p._sessionStatus)
        {
		case greyjoy::VenueSessionStatus::INIT_NOT_SET:
            std::cout << "venue_session_status=INIT_NOT_SET" << std::endl;  
            break;
		case greyjoy::VenueSessionStatus::DISCONNECTED:
            std::cout   << "VenueSessionStatus = DISCONNECTED at " << str(_handler->now()) << " " << str(clock::now()) 
                        << " " << p.toString() << std::endl;
            assert(false);
            break;
		case greyjoy::VenueSessionStatus::LOGIN_IN_PROGESS:
            std::cout << str(_handler->now()) << " VenueSessionStatus = LOGIN_IN_PROGESS" << std::endl;  
            break;
		case greyjoy::VenueSessionStatus::LOGOUT_IN_PROGRESS:
            std::cout  << str(_handler->now())<< " VenueSessionStatus = LOGOUT_IN_PROGRESS" << std::endl;  
            break;
		case greyjoy::VenueSessionStatus::ACTIVE:
            std::cout  << str(_handler->now())<< " VenueSessionStatus = ACTIVE" << std::endl;  
            break;
		case greyjoy::VenueSessionStatus::RECONNECTING:
            std::cout  << str(_handler->now())<< " VenueSessionStatus = RECONNECTING" << std::endl;  
            break;
        default:
            std::cout  << str(_handler->now())<< " VenueSessionStatus = DEFAULT" << std::endl;  
            break;
        };

        switch(p._systemRiskErrorCode)
        {
		case greyjoy::SystemRiskErrorCode::INIT_NOT_SET:
            std::cout << str(_handler->now())<< " SystemRiskErrorCode = INIT_NOT_SET" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::MESSAGE_THROTTLE_LIMIT_BREACHED:
            std::cout << str(_handler->now())<< " SystemRiskErrorCode = MESSAGE_THROTTLE_LIMIT_BREACHED" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::EXECUTION_THROTTLE_LIMIT_BREACHED:
            std::cout << str(_handler->now())<< " SystemRiskErrorCode = EXECUTION_THROTTLE_LIMIT_BREACHED" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::PORTFOLIO_GMV_BREACHED:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = PORTFOLIO_GMV_BREACHED" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::INTRADAY_POSITION_BREACHED:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = INTRADAY_POSITION_BREACHED" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::INTRADAY_OPEN_ORDER_LIMIT_BREACHED:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = INTRADAY_OPEN_ORDER_LIMIT_BREACHED" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::SINGLE_ORDER_QUANTITY_BREACHED:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = SINGLE_ORDER_QUANTITY_BREACHED" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::SINGLE_ORDER_VALUE_BREACHED:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = SINGLE_ORDER_VALUE_BREACHED" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::LOW_PRICE_COLLAR_LIMIT_BREACHED:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = LOW_PRICE_COLLAR_LIMIT_BREACHED" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::HIGH_PRICE_COLLAR_LIMIT_BREACHED:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = HIGH_PRICE_COLLAR_LIMIT_BREACHED" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::NO_LIMITS_FOUND_FOR_PRODUCT:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = NO_LIMITS_FOUND_FOR_PRODUCT" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::INSTRUMENT_GROUP_MESSAGE_THROTTLE_LIMIT_BREACHED:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = INSTRUMENT_GROUP_MESSAGE_THROTTLE_LIMIT_BREACHED" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::INSTRUMENT_GROUP_EXECUTION_THROTTLE_LIMIT_BREACHED:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = INSTRUMENT_GROUP_EXECUTION_THROTTLE_LIMIT_BREACHED" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::SESSION_REJECT:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = SESSION_REJECT" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::GMV_BREACHED:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = GMV_BREACHED" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::POSITION_LIMIT_BREACHED:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = POSITION_LIMIT_BREACHED" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::OPEN_ORDER_POSITION_LIMIT_BREACHED:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = OPEN_ORDER_POSITION_LIMIT_BREACHED" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::LIQUID_ONLY_VIOLATION:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = LIQUID_ONLY_VIOLATION" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::POSITION_SERVER_CONNECTION_ERROR:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = POSITION_SERVER_CONNECTION_ERROR" << std::endl;
            break;
		case greyjoy::SystemRiskErrorCode::IN_FLIGHT_REPLACE_REJECT:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = IN_FLIGHT_REPLACE_REJECT" << std::endl;
            break;
        default:
            std::cout << str(_handler->now())<< "SystemRiskErrorCode = DEFAULT" << std::endl;  
            break;
        };
    }

    void process_venue_event(greyjoy::VenueEvent *p)
    {
        std::cout <<  p->toString()      << std::endl;

        int64_t order_id = ( p->_OrigClOrdID > 0 ) ? p->_OrigClOrdID : p->_ClOrdID;
        auto order_index =  _id_to_index.find(order_id);
        if( order_index == _id_to_index.end())
        {
            std::cout << str(_handler->now())  << " " << str(clock::now()) << " process_venue(): Unknown order_id=" << order_id << std::endl;
            return; 
        }   
 
        auto bag = &_storage[order_index->second];
        _event.original = &bag->original;
        _event.updated = &bag->updated;

        int64_t sec_id = _event.original->sec_id;
        auto book_it = _orders.find( _event.original->sec_id);
       
        if(book_it == _orders.end())
        {
            std::cout << "process_venue(): Unknown instrumentid=" << _event.original->sec_id << std::endl;
            return;
        } 
 
        switch(p->_status)
        {

		case greyjoy::OrderStatus::INIT_NOT_SET:
            std::cout << "order_status=INIT_NOT_SET " << std::endl; 
            break;

		case greyjoy::OrderStatus::NEW:
            std::cout << "NEW" << std::endl; 
            _event.event_type = oi::OrderEventType::NEW_ACK;
            _event.updated->LastPx = 0;
            _event.updated->LeavesQty = 0;
            _event.updated->LastQty = 0;
            _handler->on_order_event(_handler->now(), _event); 
            break;

		case greyjoy::OrderStatus::PARTIALLY_FILLED:
            std::cout << "PARTIALLY_FILLED" << std::endl; 
            _event.event_type = oi::OrderEventType::PARTIAL_FILL;
            _event.updated->LastPx = p->_price;
            _event.updated->LeavesQty = p->_leavesQty;
            _event.updated->LastQty = p->_qty;
            if(_event.updated->LeavesQty <= 0 )
                book_it->second.remove(order_id); 
            else
                _event.original->size = p->_leavesQty;
            _handler->on_order_event(_handler->now(), _event);
            break;

		case greyjoy::OrderStatus::FILLED:
            std::cout << "FILLED" << std::endl; 
            _event.event_type = oi::OrderEventType::FILL;
            _event.updated->LastPx = p->_price;
            _event.updated->LeavesQty = p->_leavesQty;
            _event.updated->LastQty = p->_qty;
            if(_event.updated->LeavesQty <= 0 )
                book_it->second.remove(order_id); 
            else
                _event.original->size = p->_leavesQty;
            _handler->on_order_event(_handler->now(), _event);
            break;


		case greyjoy::OrderStatus::DONE_FOR_DAY:
            std::cout << "DONE_FOR_DAY" << std::endl; 
            break;
		
        case greyjoy::OrderStatus::CANCELLED:
            std::cout << "CANCEL" << std::endl; 
            _event.event_type = oi::OrderEventType::CANCEL_ACK;
            book_it->second.remove(order_id); 
            _handler->on_order_event(_handler->now(), _event);
            break;

		case greyjoy::OrderStatus::REPLACED:
            std::cout << "REPLACED" << std::endl; 
            _event.event_type = oi::OrderEventType::MODIFY_ACK;
            if(p->_price == _event.original->price && p->_qty < _event.original->size)
            {
                _event.original->size = p->_qty;
            }
            else
            {
                book_it->second.remove(order_id); 
                _event.original->price = p->_price;
                _event.original->size = p->_qty;            
                _event.original->ts = _handler->now();
                
                book_it->second.add_order(   
                    order_id,
                    (_event.original->side == oi::SideType::BUY),
                    _event.original->price,
                    bag->original.ts,
                    bag,
                    bag->original.owner ); 
            }
            _handler->on_order_event(_handler->now(), _event);
            break;

		case greyjoy::OrderStatus::PENDING_CANCEL:
            std::cout << "PENDING_CANCEL" << std::endl; 
            break;

		case greyjoy::OrderStatus::STOPPED:
            std::cout << "STOPPED" << std::endl; 
            break;

		case greyjoy::OrderStatus::REJECTED:
            std::cout << "REJECTED" << std::endl; 
            _event.event_type = oi::OrderEventType::CANCEL_REJECT;
            book_it->second.remove(order_id); 
            _handler->on_order_event(_handler->now(), _event);
            break;

		case greyjoy::OrderStatus::REPLACE_REJECTED:
            std::cout << "REPLACE_REJECTED" << std::endl; 
            _event.event_type = oi::OrderEventType::MODIFY_REJECT;
            if(p->_ordRejReason == 2045 || p->_ordRejReason == 2046)
                book_it->second.remove(order_id); 
            _handler->on_order_event(_handler->now(), _event);
            break;

		case greyjoy::OrderStatus::CANCEL_REJECTED:
            std::cout << "CANCEL REJECTED" << std::endl; 
            _event.event_type = oi::OrderEventType::CANCEL_REJECT;
            if(p->_ordRejReason == 2045 || p->_ordRejReason == 2046)
                book_it->second.remove(order_id); 
            _handler->on_order_event(_handler->now(), _event);
            break;

		case greyjoy::OrderStatus::SUSPENDED:
            std::cout << "SUSPENDED" << std::endl; 
            break;

		case greyjoy::OrderStatus::PENDING_NEW:
            std::cout << "PENDING_NEW" << std::endl; 
            break;

		case greyjoy::OrderStatus::CALCULATED:
            std::cout << "CALCULATED" << std::endl; 
            break;

		case greyjoy::OrderStatus::EXPIRED:
            std::cout << "EXPIRED" << std::endl; 
            _event.event_type = oi::OrderEventType::EXPIRED;
            book_it->second.remove(order_id); 
            _handler->on_order_event(_handler->now(), _event);
            break;
	
    	case greyjoy::OrderStatus::ACCEPTED_FOR_BIDDING:
            std::cout << "ACCEPTED_FOR_BIDDING" << std::endl; 
            break;
		case greyjoy::OrderStatus::PENDING_REPLACE:
            std::cout << "PENDING_REPLACE" << std::endl; 
            break;
		case greyjoy::OrderStatus::SENT_ACK_NEW:
            std::cout << "SENT_ACK_NEW" << std::endl; 
            break;
		case greyjoy::OrderStatus::SENT_ACK_CANCEL:
            std::cout << "SENT_ACK_CANCEL" << std::endl; 
            break;
		case greyjoy::OrderStatus::SENT_ACK_REPLACE:
            std::cout << "SENT_ACK_REPLACE" << std::endl; 
            break;
	
    	case greyjoy::OrderStatus::SYSTEM_REJECTED:
            std::cout << "RISK_REJECTED" << std::endl; 
            _event.event_type = oi::OrderEventType::CANCEL_REJECT;
            book_it->second.remove(order_id); 
            _handler->on_order_event(_handler->now(), _event);
            break;
	
    	case greyjoy::OrderStatus::SESSION_INACTIVE_REJECT:
            _event.event_type = oi::OrderEventType::SYSTEM_REJECT;
            book_it->second.remove(order_id); 
            _handler->on_order_event(_handler->now(), _event);
            std::cout << "SESSION_INACTIVE_REJECT" << std::endl; 
            break;
		case greyjoy::OrderStatus::RECV_ACK_NEW:
            std::cout << "RECV_ACK_NEW" << std::endl; 
            break;
		case greyjoy::OrderStatus::RECV_ACK_CANCEL:
            std::cout << "RECV_ACK_CANCEL" << std::endl; 
            break;
		case greyjoy::OrderStatus::RECV_ACK_REPLACE:
            std::cout << "RECV_ACK_REPLACE" << std::endl; 
            break;
        default:
            break;
        };
    }

    virtual std::set<int64_t> open_orders() 
    {
        std::set<int64_t> ids;
        bolton::MessageIterator<greyjoy::FlatOrderPtr> & ooIter = _tm->getOpenOrders();
        greyjoy::FlatOrderPtr floopy = nullptr;
        while ((floopy = ooIter.nextMessage()) != nullptr)
        {
            const greyjoy::Order f = (*floopy);
            ids.insert(f.getOrderId());
            std::cout << "OMS ORDER ID: " << f.getOrderId() << std::endl;
        }
        return ids; 
    }
    
private:

    greyjoy::TradingManager                                                                    * _tm    = nullptr;
   // bolton::MessageIterator<greyjoy::VenueEventPtr>                                            & _mIter;
    bolton::MessageIterator<greyjoy::VenueEvent>                                            & _mIter;
    // boost::interprocess::offset_ptr<boost::interprocess::offset_ptr<greyjoy::VenueEvent> >       _msg   = nullptr;
    boost::interprocess::offset_ptr<greyjoy::VenueEvent >       _msg   = nullptr;
    bolton::MessageIterator<greyjoy::SystemEventStruct>                                        & _sIter;
    boost::interprocess::offset_ptr<greyjoy::SystemEventStruct>                                  _s_msg = nullptr;

    std::unordered_map<int32_t, order_book_type >   _orders;
    std::vector<oi::order_bag>                      _storage; 
    std::unordered_map<int64_t, int32_t>            _id_to_index;
    oi::order_event                                 _event;

    int _max_orders = 500000;
    TSC                                             _tsc; 
    oob_logger<log_message> *                       _oob_logger = nullptr;
    log_message                                     _log_msg;
};

}

#endif // CME_ORDER_MANAGER_H
