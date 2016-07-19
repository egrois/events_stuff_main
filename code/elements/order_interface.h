#ifndef ELEMENTS_ORDER_INTERFACE_H
#define ELEMENTS_ORDER_INTERFACE_H

#include <instrument.h>
#include <vector>
#include <order_book.h>

namespace elements
{

class instrument;

namespace oi
{

enum OrderActionType
{
    NEW = 1,
    CANCEL = 2,
    MODIFY = 3
};

enum OrderType : char
{
    MARKET = '1',
    LIMIT = '2'
};

enum SideType : char
{
    BUY = '1',
    SELL = '2'    
};

enum TimeInForceType : char
{
    DAY = '0',
    FILL_OR_KILL = '3'
};

enum OrderError
{
    OK = 0
};

enum OrderEventType
{
    NEW_SUB = 0,
    CANCEL_SUB = 1,
    MODIFY_SUB = 2,
    NEW_ACK = 3,
    CANCEL_ACK = 4,
    MODIFY_ACK = 5,
    FILL = 6,
    PARTIAL_FILL = 7,
    EXPIRED = 8,
    NEW_REJECT = 9,
    CANCEL_REJECT = 10, 
    MODIFY_REJECT = 11,
    SYSTEM_REJECT = 12
};

struct outgoing_order
{
    outgoing_order() { order_id = sec_id = price = size = 0; }

    OrderActionType     action;
    int64_t             order_id;
    int32_t             sec_id;
    SideType            side;
    int64_t             price;
    int32_t             size;
    OrderType           order_type;
    TimeInForceType     tif;
    OrderError          error; 
    timestamp           ts;
    int64_t             owner = 0;
};

std::ostream & operator << (std::ostream & os, oi::outgoing_order const & msg) 
{
    os << " action=";
    switch(msg.action)
    {
    case OrderActionType::NEW:
        os << "NEW";
        break;
    case OrderActionType::CANCEL:
        os << "CANCEL";
        break;
    case OrderActionType::MODIFY:
        os << "MODIFY";
        break;
    default:
        os << "UNK";
        break;
    };

    os  << " order_id=" << msg.order_id;
    os  << " sec_id=" << msg.sec_id;

    os  <<" side=";
    switch(msg.side)
    {
    case SideType::BUY:
        os << "BUY";
        break;
    case SideType::SELL:
        os << "SELL";
        break;
    default:
        os << "UNK";
        break;
    };

    os  << " price=" << msg.price;
    os  << " size=" << msg.size;
    
    os << " order_type=";
    switch(msg.order_type)
    {
    case OrderType::MARKET:
        os << "MARKET";
        break;
    case OrderType::LIMIT:
        os << "LIMIT";
        break;
    default:
        os << "UNK";
        break;
    };

    os << " tif=";
    switch(msg.tif)
    {
    case TimeInForceType::DAY:
        os <<"DAY";
        break;
    case TimeInForceType::FILL_OR_KILL:
        os <<"FILL_OR_KILL";
        break;
    default:
        os << "UNK";
        break;
    };
    
    os   << " ts=" << str(msg.ts);
    os   << " owner=" << msg.owner;

    return os;
}

struct incoming_order
{
    incoming_order(){ LastQty = LeavesQty = LastPx = 0; }

    int32_t LastQty;
    int32_t LeavesQty;
    int64_t LastPx;
};

std::ostream & operator << (std::ostream & os, oi::incoming_order const & msg)
{
    os  << " LastQty=" << msg.LastQty 
        << " LeavesQty=" << msg.LeavesQty 
        << " LastPx=" << msg.LastPx;
    return os;
}

struct order_event
{
    OrderEventType      event_type;
    outgoing_order      *original;
    incoming_order      *updated;   
};

struct order_bag
{
    oi::outgoing_order original;
    oi::incoming_order updated;
};

std::ostream & operator << (std::ostream & os, oi::order_event const & msg)
{
    switch(msg.event_type)
    {
    case OrderEventType::NEW_SUB:
        os << "NEW_SUB";
        break;
    case OrderEventType::CANCEL_SUB:
        os << "CANCEL_SUB";
        break;
    case OrderEventType::MODIFY_SUB:
        os << "MODIFY_SUB";
        break;
    case OrderEventType::NEW_ACK:
        os << "NEW_ACK";
        break;
    case OrderEventType::CANCEL_ACK:
        os << "CANCEL_ACK";
        break;
    case OrderEventType::MODIFY_ACK:
        os  << "MODIFY_ACK";
        break;
    case OrderEventType::FILL:
        os  << "FILL";
        break;
    case OrderEventType::PARTIAL_FILL:
        os  << "PARTIAL_FILL";
        break;
    case OrderEventType::EXPIRED:
        os  << "EXPIRED";
        break;
    case OrderEventType::NEW_REJECT:
        os  << "NEW_REJECT";
        break;
    case OrderEventType::CANCEL_REJECT: 
        os  << "CANCEL_REJECT";
        break;
    case OrderEventType::MODIFY_REJECT:
        os  << "MODIFY_REJECT";
        break;
    default:
        assert(false);
    };
    os << (*msg.original) << " " << (*msg.updated) << std::endl;
    return os;
}

class order_handler
{
public:
    virtual void on_order_event(timestamp ts, order_event const & event) = 0;
    virtual timestamp now() = 0;
    virtual int64_t submit_order(oi::outgoing_order & order, bool isManual = false) = 0;
};

class order_manager_interface
{
public:

    typedef ob::order_book<oi::order_bag *, instrument *> order_book_type;

    order_manager_interface(){}
    virtual ~order_manager_interface(){}

    void install_handler(order_handler * handler) { _handler = handler; } 
    void set_instruments(std::vector<instrument*> *p) { _instruments = p; } 
    
    virtual int64_t submit(outgoing_order & order, bool isManual = false) = 0;
    virtual void on_market_changed() = 0;

    virtual order_book_type * get_order_book( int32_t sec_id ) = 0; 

    virtual std::set<int64_t> open_orders() { return std::set<int64_t>(); }

protected:
    order_handler               * _handler = nullptr;
    std::vector<instrument*>    * _instruments = nullptr;
};

}
}

#endif //ELEMENTS_ORDER_INTERFACE_H 
