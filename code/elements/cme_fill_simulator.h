#ifndef ELEMENTS_CME_FILL_SIMULATOR_H
#define ELEMENTS_CME_FILL_SIMULATOR_H

#include <order_interface.h> 
#include <feed.h>
#include <iostream>
#include <easy_time.h>
#include <order_book.h>

namespace elements
{

class cme_fill_simulator :  public elements::oi::order_manager_interface,
                            public elements::feed 
{
public:

    cme_fill_simulator(){}
    cme_fill_simulator(int max_orders) : _max_orders(max_orders) {}
     
    virtual ~cme_fill_simulator()
    {
    }

    bool init()
    {
        _storage.reserve(_max_orders);
        for(auto ins : *_instruments)       
            _orders[ins->secdef().SecurityID] = order_book_type(ins);
    }

    bool set_delays(int d1_us, int d2_us ) 
    { 
        _d1 = microseconds(d1_us);
        _d2 = microseconds(d2_us); 
    }

    void set_realtime() { _is_realtime = true; }

    virtual int64_t submit(oi::outgoing_order & order, bool isManual)
    {
        if(!verify(order)) 
        {
            return -1;
        }

        order.ts = _handler->now() + _d1;

        if(order.action == oi::OrderActionType::NEW)
        {
            oi::order_bag bag;

            assert(_storage.size() < _max_orders);

            order.order_id = _storage.size();

            bag.original = order;

            _event.original = &bag.original;
            _event.updated = &bag.updated;
            _storage.push_back(std::move(bag));
        }
        else
        {
            if( order.order_id >= _storage.size() || order.order_id < 0)
            {
                std::cout << "bad order id " << order.order_id << std::endl;
                return -1;
            }
            
            _event.original = & order; 
            _event.updated = & _storage[order.order_id].updated; 
        }       

        if(order.action == oi::OrderActionType::NEW)
            _event.event_type = oi::OrderEventType::NEW_SUB;
        else if(order.action == oi::OrderActionType::MODIFY)
            _event.event_type = oi::OrderEventType::MODIFY_SUB;
        else if(order.action == oi::OrderActionType::CANCEL)
            _event.event_type = oi::OrderEventType::CANCEL_SUB;
        else
        {
            std::cout << "Uknown order action" << std::endl;
            assert(false);
        }

        _event.updated->LastPx = 0;
        _event.updated->LeavesQty = 0;
        _event.updated->LastQty = 0;
        _handler->on_order_event(_handler->now(), _event);
        _lobby.push_back(order);
        return order.order_id;
    }
    
    virtual bool peek_time(timestamp &ts)
    {
        if(_lobby.size() == 0 )
            return false;
        ts = _lobby.front().ts;
        return true;
    }
    
    virtual bool poll()
    {
        if(_lobby.size() == 0 )
            return false;
        match(_lobby.front());
        _lobby.pop_front(); 
        return true;
    }

    virtual void on_market_changed()
    {
        for(auto & entry : _orders)
        {
            auto & obook = entry.second;
            auto & book = entry.second.market->book();
    
            // BID 
            bool exhausted = false;
            bool level_erased = false;
            _bid_mask.clear();

            for(auto level = obook.bid.levels.rbegin(); ( (level !=  obook.bid.levels.rend()) && (!exhausted)); )
            {
                for(auto order = level->second.orders.begin(); ((order != level->second.orders.end()) && (!exhausted)); )
                {  
                    auto & ord = order->data->original;
                    std::vector<std::pair<int64_t, int32_t> > filled;
                    int remaining = cross(book, ord.size, ord.price, ord.side, filled);
                    
                    assert(remaining >= 0);
                 
                    if(filled.size() == 0)
                    {
                        exhausted = true;
                        continue;
                    }
                   
                    _event.original = &order->data->original;
                    _event.updated = &order->data->updated;

                    if(remaining > 0)
                    {
                        int32_t rem_sz = ord.size;
                        for(auto entry : filled)
                        {
                            _event.event_type = oi::OrderEventType::PARTIAL_FILL;
                            _event.updated->LastQty = entry.second;
                            rem_sz -= entry.second;
                            _event.updated->LeavesQty = rem_sz;
                            //_event.updated->LastPx = entry.first; 
                            _event.updated->LastPx = ord.price; 
                            _handler->on_order_event(_handler->now(), _event);
                        }
                        
                        assert(rem_sz == remaining);
                        ord.size = remaining;
                        
                        exhausted = true;
                        continue;
                    }

                    if(filled.size() == 1) // remaining = 0 
                    {
                        _event.event_type = oi::OrderEventType::FILL;
                        _event.updated->LastQty = filled[0].second;
                        _event.updated->LeavesQty = 0;
                        //_event.updated->LastPx = filled[0].first;
                        _event.updated->LastPx = ord.price; 
                        _handler->on_order_event(_handler->now(), _event);
                    }
                    else if( filled.size() > 1 )
                    {
                        int32_t rem_sz = ord.size;
                        for(auto entry : filled)
                        {
                            _event.event_type = oi::OrderEventType::PARTIAL_FILL;
                            _event.updated->LastQty = entry.second;
                            rem_sz -= entry.second;
                            _event.updated->LeavesQty = rem_sz;
                            //_event.updated->LastPx = entry.first;
                            _event.updated->LastPx = ord.price; 
                            _handler->on_order_event(_handler->now(), _event);
                        }
                        assert(rem_sz == remaining);
                        ord.size = remaining;
                    }
                    
                    if(level->second.orders.size() == 1)
                    {
                        order = level->second.orders.erase(order);
                        obook.bid.levels.erase(--(level.base()));
                        
                        level_erased = true;
                        break;
                    }
                    else
                    {
                        order = level->second.orders.erase(order);
                        continue;
                    }
                }

                if(!level_erased)
                    ++level;
            }

            // ASK 
            exhausted = false;
            level_erased = false;
            _ask_mask.clear();
            for(auto level = obook.ask.levels.begin(); ( (level !=  obook.ask.levels.end()) && (!exhausted)); )
            {
                for(auto order = level->second.orders.begin(); ((order != level->second.orders.end()) && (!exhausted)); )
                { 
                    auto & ord = order->data->original;
                    std::vector<std::pair<int64_t, int32_t> > filled;
                    int remaining = cross(book, ord.size, ord.price, ord.side, filled);
                    
                    assert(remaining >= 0);
                 
                    if(filled.size() == 0)
                    {
                        exhausted = true;
                        continue;
                    }
                    
                    _event.original = &order->data->original;
                    _event.updated = &order->data->updated;

                    if(remaining > 0)
                    {
                        int32_t rem_sz = ord.size;
                        for(auto entry : filled)
                        {
                            _event.event_type = oi::OrderEventType::PARTIAL_FILL;
                            _event.updated->LastQty = entry.second;
                            rem_sz -= entry.second;
                            _event.updated->LeavesQty = rem_sz;
                            //_event.updated->LastPx = entry.first;
                            _event.updated->LastPx = ord.price; 
                            _handler->on_order_event(_handler->now(), _event);
                        }
                        
                        assert(rem_sz == remaining);
                        ord.size = remaining;
                        
                        exhausted = true;
                        continue;
                    }

                    if(filled.size() == 1) 
                    {
                        _event.event_type = oi::OrderEventType::FILL;
                        _event.updated->LastQty = filled[0].second;
                        _event.updated->LeavesQty = 0;
                        _event.updated->LastPx = filled[0].first;
                        //_event.updated->LastPx = ord.price; 
                        _handler->on_order_event(_handler->now(), _event);
                    }
                    else if( filled.size() > 1 )
                    {
                        int32_t rem_sz = ord.size;
                        for(auto entry : filled)
                        {
                            _event.event_type = oi::OrderEventType::PARTIAL_FILL;
                            _event.updated->LastQty = entry.second;
                            rem_sz -= entry.second;
                            _event.updated->LeavesQty = rem_sz;
                            //_event.updated->LastPx = entry.first;
                            _event.updated->LastPx = ord.price; 
                            _handler->on_order_event(_handler->now(), _event);
                        }
                        assert(rem_sz == remaining);
                        ord.size = remaining;
                    }
                    
                    if(level->second.orders.size() == 1)
                    {
                        order = level->second.orders.erase(order);
                        level =  obook.ask.levels.erase(level);
                        level_erased = true;
                        break;
                    }
                    else
                    {
                        order = level->second.orders.erase(order);
                        continue;
                    }
                }

                if(!level_erased)
                    ++level;
            }
        }
    }

    void match(oi::outgoing_order & o)
    {
        switch(o.action)
        {
        case oi::OrderActionType::NEW:
            match_new(o);
            break;
        case oi::OrderActionType::CANCEL:
            match_cancel(o);
            break;
        case oi::OrderActionType::MODIFY:
            match_modify(o);
            break;
        }
    }

    void match_new(oi::outgoing_order & o)
    {
        auto it = _orders.find(o.sec_id);
        assert(it != _orders.end());
        assert(o.order_id < _storage.size());
        assert(o.sec_id == it->second.market->secdef().SecurityID);

        auto & book = it->second.market->book();
        assert(book.has_top());
        assert(o.order_type == oi::OrderType::LIMIT); 

        std::vector<std::pair<int64_t, int32_t> > filled;
        clear_masks();
        int remaining = cross(book, o.size, o.price, o.side, filled);
        
        auto &bag = _storage[o.order_id];
        _event.original = &bag.original;
        _event.updated = &bag.updated;

        if(remaining == 0 && filled.size() == 1) 
        {
            _event.event_type = oi::OrderEventType::FILL;
            _event.updated->LastQty = filled[0].second;
            _event.updated->LeavesQty = 0;
            _event.updated->LastPx = filled[0].first;
            _handler->on_order_event(_handler->now(), _event);
            return;
        }

        int32_t rem_sz = o.size;
        for(auto entry : filled)
        {
            _event.event_type = oi::OrderEventType::PARTIAL_FILL;
            _event.updated->LastQty = entry.second;
            rem_sz -= entry.second;
            _event.updated->LeavesQty = rem_sz;
            _event.updated->LastPx = entry.first;
            _handler->on_order_event(_handler->now(), _event);
        }
        assert(rem_sz == remaining);
        o.size = remaining;
 
        if(o.tif == oi::TimeInForceType::FILL_OR_KILL)
        {
            if(remaining > 0)
            {
                _event.event_type = oi::OrderEventType::EXPIRED;
                _handler->on_order_event(_handler->now(), _event); 
            }
        }
        else if(o.tif == oi::TimeInForceType::DAY)
        {
            if(remaining > 0) 
            {
                bag.original.size = remaining;
                it->second.add_order(  
                    bag.original.order_id,
                    (bag.original.side == oi::SideType::BUY),
                    bag.original.price,
                    bag.original.ts,
                    &bag,
                    bag.original.owner); 
                _event.event_type = oi::OrderEventType::NEW_ACK;
                _event.updated->LastPx = 0;
                _event.updated->LeavesQty = 0;
                _event.updated->LastQty = 0;
                _handler->on_order_event(_handler->now(), _event); 
            }
        }
        else
        {
            assert(false);
        } 
    }

    void match_cancel(oi::outgoing_order & o)
    {
        auto it = _orders.find(o.sec_id);
        assert(it != _orders.end());
        assert(o.order_id < _storage.size());
        
        auto &bag = _storage[o.order_id];
        _event.original = &bag.original;
        _event.updated = &bag.updated;
        _event.updated->LastPx = 0;
        _event.updated->LeavesQty = 0;
        _event.updated->LastQty = 0;
       
        if(it->second.remove(o.order_id))
        {
            _event.event_type = oi::OrderEventType::CANCEL_ACK;
            _handler->on_order_event(_handler->now(), _event); 
        }
        else
        {
            _event.event_type = oi::OrderEventType::CANCEL_REJECT;
            _handler->on_order_event(_handler->now(), _event); 
        } 
    }

    void match_modify(oi::outgoing_order & o)
    {
        auto it = _orders.find(o.sec_id);
        assert(it != _orders.end());
        assert(o.order_id < _storage.size());
        
        auto &bag = _storage[o.order_id];
        _event.original = &bag.original;
        
        _event.updated = &bag.updated;
        _event.updated->LastPx = 0;
        _event.updated->LeavesQty = 0;
        _event.updated->LastQty = 0;
 
        if((o.side == bag.original.side) && (bag.original.sec_id == o.sec_id) && (it->second.remove(o.order_id)) )
        {
            if(o.size <= bag.original.size && o.price == bag.original.price)
                o.ts = bag.original.ts;
            bag.original = o;

            auto & book = it->second.market->book();
            
            std::vector<std::pair<int64_t, int32_t> > filled;
            clear_masks();
            int remaining = cross(book, o.size, o.price, o.side, filled);
            
            if(remaining == 0 && filled.size() == 1) 
            {
                _event.event_type = oi::OrderEventType::FILL;
                _event.updated->LastQty = filled[0].second;
                _event.updated->LeavesQty = 0;
                _event.updated->LastPx = filled[0].first;
                _handler->on_order_event(_handler->now(),_event);
                return;
            }

            if(remaining > 0) 
            {
                bag.original.size = remaining;
                it->second.add_order(  
                    bag.original.order_id,
                    (bag.original.side == oi::SideType::BUY),
                    bag.original.price,
                    bag.original.ts,
                    &bag,
                    bag.original.owner ); 

                _event.event_type = oi::OrderEventType::MODIFY_ACK;
                _event.updated->LastPx = 0;
                _event.updated->LeavesQty = 0;
                _event.updated->LastQty = 0;
                _handler->on_order_event(_handler->now(),_event); 
            }
        }
        else
        {
            _event.event_type = oi::OrderEventType::MODIFY_REJECT;
            _event.original = &o;
            _event.updated->LastPx = 0;
            _event.updated->LeavesQty = 0;
            _event.updated->LastQty = 0;
            _handler->on_order_event(_handler->now(), _event); 
        }
    }
    
    bool verify( oi::outgoing_order & order)
    {
        instrument *p = nullptr;
        if(order.action == oi::OrderActionType::NEW)
        {
            for(auto ins: *_instruments)
            {
                if(ins->secdef().SecurityID == order.sec_id)
                    p = &(*ins);
            }

            if( p == nullptr )
            {
                std::cout << "order for unknown intrument" << std::endl;
                return false;
            }
        }
        else if( order.action == oi::OrderActionType::MODIFY)
        {
            if(order.order_id >= _storage.size())
            {
                std::cout << "unknown order id" << std::endl;
                return false;
            } 
            _storage[order.order_id].original.sec_id;
            for(auto ins: *_instruments)
            {
                if(ins->secdef().SecurityID == _storage[order.order_id].original.sec_id)
                    p = &(*ins);
            }

            if( p == nullptr )
            {
                std::cout << "order for unknown intrument" << std::endl;
                return false;
            }
        }
        else
        {
            return true;
        }

        if( order.price % p->book().min_price_increment() != 0)
        {
            std::cout << " fs price not on grid" << std::endl;
            return false;
        }

        return true;
    }

    int cross(  price_level_book const & book, 
                int32_t size,
                int64_t price, 
                oi::SideType side,
                std::vector<std::pair<int64_t, int32_t> > & fills,
                int32_t max_deep = 10)
    {
        fills.clear();
        
        BookSide bside; 
        int sign;
        std::map<int64_t, int32_t> *mask;
        if(side == oi::SideType::BUY)
        { 
            bside = BookSide::ASK;
            sign =  1;
            mask = &_ask_mask;
        } 
        else if(side == oi::SideType::SELL)
        { 
            bside = BookSide::BID;
            sign = -1;
            mask = &_bid_mask;
        }
        else return 0;

        if( !book.has_top() )
        {
            return 0;
        }

        int32_t size_remaining = size;
        int64_t book_price = book.top_price(bside);
        int64_t incr = sign * book.min_price_increment();           
        
        int num_iter = 0; 
        while(      size_remaining > 0 
                &&  (book_price - price) * sign <= 0 
                &&  num_iter < max_deep )
        {
            int32_t size = book.size_at_price(book_price, bside);
            if(mask->find(book_price) != mask->end())
                size += (*mask)[book_price];

            if(size > 0)
            {
                int32_t size_traded = std::min(size_remaining, size);
                fills.push_back(std::make_pair(book_price, size_traded));
                size_remaining -= size_traded;
                if((*mask).find(book_price) == mask->end())
                    (*mask)[book_price] = -size_traded;
                else
                    (*mask)[book_price] -= size_traded;                
            }
            ++num_iter;
            book_price += incr;
        }
        return size_remaining;
    }

    void clear_masks()
    {
        _bid_mask.clear();
        _ask_mask.clear();
    }

    virtual order_book_type * get_order_book( int32_t sec_id ) 
    {
        auto it = _orders.find(sec_id);
        if(it == _orders.end())
            return nullptr;
        return &it->second;
    }

private:
    
    microseconds _d1;
    microseconds _d2;
    int _max_orders = 100000;

    std::unordered_map<int32_t, order_book_type >   _orders;
    std::list<oi::outgoing_order>                   _lobby;
    std::vector<oi::order_bag>                      _storage; 
    std::map<int64_t, int32_t>                      _bid_mask,_ask_mask;
    oi::order_event _event;

    bool _is_realtime = false;
};
}

#endif // ELEMENTS_CME_FILL_SIMULATOR_H
