#ifndef ELEMENTS_AUTO_STACKER_H
#define ELEMENTS_AUTO_STACKER_H

namespace elements
{

#include <order_interface.h>
#include <easy_time.h>
#include <stacker_utility.h>

struct auto_stacker_params
{
    int32_t         num_levels         = 0;
    int32_t         num_visible_levels = 0;
    int32_t         max_order_size     = 0;
    int32_t         skip_mask          = 1;  
    int64_t         reference_price    = 0;
    int64_t         initial_edge       = 10;
    int64_t         ask_replenish_edge     = 10; 
    bool            ask_replenish          = false;
    int32_t         ask_replenish_size     = 1;     
    int64_t         bid_replenish_edge     = 10; 
    bool            bid_replenish          = false;
    int32_t         bid_replenish_size     = 1;     

    utility        *bid_utility        = nullptr;           
    utility        *ask_utility        = nullptr;
};

std::ostream & operator << (std::ostream & os, auto_stacker_params const & p)
{
    //os << "STACKER PARAMS: "    << " num_levels=" << p.num_levels
    //                            << " max_order_size=" << p.max_order_size
    //                            << " top_bid=" << p.top_bid
    //                            << " top_ask=" << p.top_ask
    //                            << " skip_mask=" << p.skip_mask;
    return os;
} 

class auto_stacker
{
    typedef std::map<int64_t, ob::price_level<oi::order_bag *> >::iterator level_it_type;

public:

    auto_stacker(  int64_t sec_id, 
                    oi::order_handler *order_handler, 
                    oi::order_manager_interface::order_book_type *book, 
                    int64_t mpi) 
        :   _sec_id(sec_id), 
            _order_handler(order_handler),
            _book(book),
            _mpi(mpi)
    {
    }

    virtual ~auto_stacker() 
    {
        delete _params.bid_utility;
        delete _params.ask_utility;
    }


    bool set_params(auto_stacker_params const * params) 
    {
        _params = *params;
        return true;
    }

    void set_ask_replenish_edge(int64_t edge) { _params.ask_replenish_edge = edge; }
    void set_ask_replenish(bool is_replenish) { _params.ask_replenish = is_replenish;  }
    void set_ask_replenish_size(int32_t sz) { _params.ask_replenish_size = sz;  }     
    
    void set_bid_replenish_edge(int64_t edge) { _params.bid_replenish_edge = edge; }
    void set_bid_replenish(bool is_replenish) { _params.bid_replenish = is_replenish;  }
    void set_bid_replenish_size(int32_t sz) { _params.bid_replenish_size = sz;  }     
   
    void set_num_visible_levels(int32_t num) { _params.num_visible_levels = num;  }     
 
    auto_stacker_params & get_params(){return _params;}
    
    void enable() 
    { 
        std::cout << "enabling auto stacker sec_id=" << _sec_id << std::endl;

        if(validate_params() && _enabled == false)
        {
            place_orders();
            _enabled = true;
        }
    }

    void disable()
    {
        std::cout << "disabling auto stacker sec_id=" << _sec_id << std::endl;
        cancel_orders(); 
        _enabled = false;
    }

    bool enabled() { return _enabled; }

    bool validate_params()
    {
        if(_params.skip_mask < 0.5 || _params.skip_mask > 100 )
        {
            std::cout << "invalid skip_mask" << std::endl;
            return false;
        }
        return true;
    }

    void place_orders()
    {
        place_bid_orders();
        place_ask_orders();
    }

    void place_bid_orders()
    {
        oi::SideType side = oi::SideType::BUY;        
        clear_marks(side); 
        int64_t min_price = _params.reference_price - _params.initial_edge * _mpi;
        int64_t max_price = min_price - _mpi * std::min( _params.num_levels - 1, _params.num_visible_levels );

        int marked = 0;
        
        for(int64_t price = min_price; price >= max_price; price -= _mpi )
        {
            if( (price / _mpi) % _params.skip_mask == 0 )
            {
                mark_level_to_create( side, price );
                ++marked;
            }
            if( marked > 20 )
                break;
        }
        execute_marks( side );
    }

    void place_ask_orders()
    {
        oi::SideType side = oi::SideType::SELL;        
        clear_marks(side); 
        int64_t min_price = _params.reference_price + _params.initial_edge * _mpi;
        int64_t max_price = min_price + _mpi * std::min( _params.num_levels - 1, _params.num_visible_levels );
        
        int marked = 0;

        for(int64_t price = min_price; price <= max_price; price +=_mpi )
        {
            if( (price / _mpi) % _params.skip_mask == 0 )
            {
                mark_level_to_create( side, price );
                ++marked;
            }
            if( marked > 20 )
                break;
        }
        execute_marks( side );
    }

    void cancel_orders()
    {
        std::set<int64_t> orders_to_cancel;
        
        for( auto  & level : _book->ask.levels )
        {
            for(auto & order : level.second.orders)
            {
                if( (int64_t)order.owner == (int64_t)this ) 
                {
                    orders_to_cancel.insert(order.data->original.order_id);
                }
            }
        }
       
        for( auto  & level : _book->bid.levels )
        {
            for(auto & order : level.second.orders)
            {
                if( (int64_t *)order.owner == (int64_t *)this ) 
                {
                    orders_to_cancel.insert(order.data->original.order_id);
                }
            }
        }

        for(auto id : orders_to_cancel)
        {
            oi::outgoing_order o;
            o.action = oi::OrderActionType::CANCEL;
            o.sec_id = _sec_id;
            o.order_id = id;
            _order_handler->submit_order(o);
        }
    }

    void refresh( int64_t top_bid_price, int64_t top_ask_price)
    {
        int64_t min_bid, max_bid, min_ask, max_ask;
        get_order_ranges(min_bid, max_bid, min_ask, max_ask);
        
        if(min_bid == std::numeric_limits<int64_t>::max() )
        {
            std::cout << "FORCED PLACING BID ORDERS" << std::endl;
            // place_bid_orders();           
            return; 
        }
        
        if(min_ask == std::numeric_limits<int64_t>::max() )
        {
            std::cout << "FORCED PLACING ASK ORDERS" << std::endl;
            //  place_ask_orders();           
            return; 
        }

        int32_t num_bid_levels = (min_bid - max_bid) /_mpi + 1;
        int32_t bid_levels_to_add = _params.num_visible_levels - num_bid_levels; 
        oi::SideType side = oi::SideType::BUY;        
        clear_marks(side); 

        if(bid_levels_to_add > 0)
        {
            int32_t added = 0;
            for(int64_t price = max_bid - _mpi; added < bid_levels_to_add; price -= _mpi, ++added )
            {
                if (price < _params.reference_price - _mpi * _params.num_levels)
                    break;
 
                if( (price / _mpi) % _params.skip_mask == 0 )
                { 
                    mark_level_to_create( side, price );
                }
            }
        }
        execute_marks( side );

        int32_t num_ask_levels = (max_ask - min_ask) /_mpi + 1;
        int32_t ask_levels_to_add = _params.num_visible_levels - num_ask_levels; 
        side = oi::SideType::SELL;        
        clear_marks(side); 
        if(ask_levels_to_add > 0)
        {
            int32_t added = 0;
            for(int64_t price = max_ask + _mpi; added < ask_levels_to_add; price += _mpi, ++added )
            {  
                if (price > _params.reference_price + _mpi * _params.num_levels)
                    break;
 
                if( (price / _mpi) % _params.skip_mask == 0 )
                {
                    mark_level_to_create( side, price );
                }
            }
        }
        execute_marks( side );


        if (_params.bid_replenish)
        { 
            side = oi::SideType::BUY;        
            clear_marks(side); 
            for(int64_t price = min_bid + _mpi; price  <= top_bid_price - _mpi *_params.bid_replenish_edge; price +=_mpi)
            {
                if( (price / _mpi) % _params.skip_mask == 0 )
                {
                    mark_level_to_create( side, price, _params.bid_replenish_size );
                }
            }    
            execute_marks( side );
        }

        if (_params.ask_replenish)
        { 
            side = oi::SideType::SELL;        
            clear_marks(side); 
            for(int64_t price = min_ask - _mpi; price  >= top_ask_price +_mpi * _params.ask_replenish_edge; price -=_mpi)
            {
                if( (price / _mpi) % _params.skip_mask == 0 )
                {
                    mark_level_to_create( side, price, _params.ask_replenish_size );
                }
            } 
            execute_marks( side );
        }
    }

    void clear_marks( oi::SideType side )
    {
        if(side == oi::SideType::SELL)
            _ask_marks.resize(0);
        else
            _bid_marks.resize(0);
    }

    void execute_marks( oi::SideType side )
    {
        if(side == oi::SideType::SELL)
            for(auto mark : _ask_marks)
                _order_handler->submit_order( mark );
        else
            for(auto mark : _bid_marks)
                _order_handler->submit_order( mark );
        clear_marks(side);
    }
   
    int32_t get_size( int32_t orig_size, oi::SideType side, int64_t price)
    {
        if(side == oi::SideType::SELL)
        {
            return (*_params.ask_utility)(orig_size,  ( price - _params.reference_price + _params.initial_edge) / _mpi );
        }
        else
        {
            return (*_params.bid_utility)(orig_size,  (_params.reference_price -_params.initial_edge - price) / _mpi );
        }
    }
 
    void mark_level_to_create(oi::SideType side, int64_t price, int32_t forced_size = -1)
    {
        int32_t size = forced_size;
        if(size < 0) 
        {
            size = get_size( 0, side, price );
        }
        while( size > 0 ) new_works(side, size, price);
    }
    
    void new_works( oi::SideType side, int32_t &size, int64_t price)
    {
        int32_t osize = std::min(_params.max_order_size, size );
        size -= osize;                   
      
        oi::outgoing_order * o = nullptr;
 
        if(side == oi::SideType::SELL)
        {
            _ask_marks.resize(_ask_marks.size() + 1);
            o = &_ask_marks[_ask_marks.size() - 1];
        }
        else
        {
             _bid_marks.resize(_bid_marks.size() + 1);
            o = &_bid_marks[_bid_marks.size() - 1];
        }
    
        prepare_order(*o);
        o->action = oi::OrderActionType::NEW;
        o->side = side; 
        o->price = price;
        o->size = osize; 
    }
    
    void prepare_order(oi::outgoing_order & o)
    {
        o.sec_id =_sec_id; 
        o.order_type = oi::OrderType::LIMIT; 
        o.tif = oi::TimeInForceType::DAY; 
        o.owner = (int64_t)this;
    }

    bool get_order_ranges(int64_t &min_bid, int64_t &max_bid, int64_t &min_ask, int64_t &max_ask )
    {
        int32_t num_bid_orders = 0;
        int32_t num_ask_orders = 0;

        min_bid = std::numeric_limits<int64_t>::max();
        min_ask = std::numeric_limits<int64_t>::max();
        max_bid = std::numeric_limits<int64_t>::min();
        max_ask = std::numeric_limits<int64_t>::min();
        
        for( auto  & level : _book->ask.levels )
        {
            for(auto & order : level.second.orders)
            {
                if( (int64_t)order.owner == (int64_t)this ) 
                {
                    ++num_ask_orders;
                    if(level.first <= min_ask)
                        min_ask = level.first;
                    if(level.first >= max_ask)
                        max_ask = level.first;
                }
            }
        }
        
        for( auto  & level : _book->bid.levels )
        {
            for(auto & order : level.second.orders)
            {
                if( (int64_t)order.owner == (int64_t)this ) 
                {
                    ++num_bid_orders;
                    if(level.first <= min_bid)
                        min_bid = level.first;
                    if(level.first >= max_bid)
                        max_bid = level.first;
                }
            }
        }

       // if(num_ask_orders == 0  || num_bid_orders == 0)
       //     return false;

        if(num_bid_orders != 0)
            std::swap(min_bid, max_bid);

        // std::cout << "order_ranges: " << "min_bid=" << min_bid << " min_ask=" << min_ask << " max_bid=" << max_bid << " max_ask=" << max_ask << std::endl;

        //assert(min_bid >= max_bid);
        //assert(min_ask <= max_ask);
        //assert(min_bid < min_ask);

        return true; 
    }

private:

    oi::order_handler *_order_handler; 
    oi::order_manager_interface::order_book_type *_book = nullptr; 
    auto_stacker_params _params;
    int64_t _sec_id;

    bool _enabled = false;

    int64_t _mpi = 0;

    std::vector<oi::outgoing_order> _ask_marks;
    std::vector<oi::outgoing_order> _bid_marks;
};

}

#endif
