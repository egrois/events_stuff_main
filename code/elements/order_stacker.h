#ifndef ELEMENTS_ORDER_STACKER_H
#define ELEMENTS_ORDER_STACKER_H

#include <order_interface.h>
#include <easy_time.h>
#include <stacker_utility.h>

namespace elements
{

struct order_stacker_params
{
    int32_t         num_levels         = 0;
    int32_t         max_order_size     = 0;
    int64_t         top_bid            = 0;
    int64_t         top_ask            = 0;
    int32_t         skip_mask          = 1;  
    utility        *bid_utility        = nullptr;           
    utility        *ask_utility        = nullptr;

    bool compare(order_stacker_params & s)
    {
        if(s.num_levels != num_levels)
            return false;

        if(s.max_order_size != max_order_size)
            return false;

        if(s.skip_mask != skip_mask)
            return false;

        if ( bid_utility == nullptr || ask_utility == nullptr || s.bid_utility == nullptr || s.ask_utility == nullptr)
            return false;

        if( !bid_utility->compare(*s.bid_utility) || !ask_utility->compare(*s.ask_utility) )
            return false;

        return true;
    }
};


std::ostream & operator << (std::ostream & os, order_stacker_params const & p)
{
    os << "STACKER PARAMS: "    << " num_levels=" << p.num_levels
                                << " max_order_size=" << p.max_order_size
                                << " top_bid=" << p.top_bid
                                << " top_ask=" << p.top_ask
                                << " skip_mask=" << p.skip_mask;
    return os;
} 

class order_stacker // : public oi::order_handler
{
    typedef std::map<int64_t, ob::price_level<oi::order_bag *> >::iterator level_it_type;

public:

    order_stacker(  int64_t sec_id, 
                    oi::order_handler *order_handler, 
                    oi::order_manager_interface::order_book_type *book, 
                    int64_t mpi) 
        :   _sec_id(sec_id), 
            _order_handler(order_handler),
            _book(book),
            _mpi(mpi)
    {
    }

    virtual ~order_stacker() 
    {
        delete _params.bid_utility;
        delete _params.ask_utility;
    }

    bool set_params(order_stacker_params const * params) 
    {
        if(params->top_bid >=params->top_ask )
        {
            std::cout << "bid larger than ask " << std::endl;
            return false;
        }
        _params = *params;
        return true;
    }
    
    void set_num_levels(int32_t m) { _params.num_levels = m; }
    void set_max_order_size(int32_t m) { _params.max_order_size = m; }
    void set_skip_mask(int32_t m) { _params.skip_mask = m; } 
    void set_bid_utility( utility *u)  { _params.bid_utility = u; }
    void set_ask_utility( utility *u)  { _params.ask_utility = u; }
    bool set_top(int64_t bid, int64_t ask)
    {
        if(bid >= ask )
        {
            std::cout << "bid larger than ask 2" << std::endl;
            return false;
        }
        _params.top_bid = bid;
        _params.top_ask = ask;
        return true;
    }  

    order_stacker_params & get_params(){return _params;}

    int64_t get_top_ask() { return _params.top_ask; }
    int64_t get_top_bid() { return _params.top_bid; }

    void enable() 
    { 
        _enabled = true;
        std::cout << "enabling stacker sec_id=" << _sec_id << std::endl;
        refresh();
    }
    void disable()
    {
        _enabled = false;
        std::cout << "disabling stacker sec_id=" << _sec_id << std::endl;
        refresh();
    }

    bool enabled() { return _enabled; }


    bool has_bid_fade(){ return _has_bid_fade; }
    bool has_ask_fade(){ return _has_ask_fade; }

    void set_bid_fade(int64_t val) { _bid_fade = val; _has_bid_fade = true; }
    void set_ask_fade(int64_t val) { _ask_fade = val; _has_ask_fade = true; }
    
    void clear_bid_fade() { _has_bid_fade = true; }
    void clear_ask_fade() { _has_ask_fade = true; }

    void refresh()
    {
        clear_marks(oi::SideType::SELL);
        clear_marks(oi::SideType::BUY);
        if( _enabled )
        {
            if(!validate_params())
            {
                std::cout << "order_stacker parameters are invalid " << std::endl;
                return;
            }

            update_bid();
            update_ask();
        }
        else
        {
            std::cout <<"cancelling all orders" << std::endl;
            cancel_all_orders();     
        }
    }

    bool validate_params()
    {
        if(_params.skip_mask < 0.5 || _params.skip_mask > 100 )
        {
            std::cout << "invalid skip_mask" << std::endl;
            return false;
        }
        return true;
    }

private:

    void update_ask()
    {
        oi::SideType side = oi::SideType::SELL;        
    
        clear_marks(side); 
    
        std::list <level_it_type> levels_to_remove;
    
        int64_t min_price = _params.top_ask;
        int64_t max_price = _params.top_ask + _mpi * ( _params.num_levels - 1 );
    
        for(auto it = _book->ask.levels.begin(); it != _book->ask.levels.end(); ++it )
        {
            if( it->first < min_price || it->first > max_price || ((it->first / _mpi) % _params.skip_mask != 0 ) ) 
                levels_to_remove.push_back(it);
        }
        auto it = _book->ask.levels.begin();
        while( it !=  _book->ask.levels.end() && it->first < min_price ) ++it;

        for(int64_t price = min_price; price <= max_price; price +=_mpi )
        {
            if(it == _book->ask.levels.end())
            {
                if(levels_to_remove.size() == 0 )
                {
                    if( (price / _mpi) % _params.skip_mask == 0 )
                        mark_level_to_create( side, price );
                }
                else
                {
                    if( (price / _mpi) % _params.skip_mask == 0 )
                    {
                        mark_level_to_move( side, *levels_to_remove.begin(), price );
                        levels_to_remove.pop_front();
                    }
                }
            } 
            else if(price < it->first)
            {
                if( (price / _mpi) % _params.skip_mask == 0 )
                    mark_level_to_create( side, price );
            }
            else if( price == it->first)
            {
                int32_t orig_size = size_at_level(it);
                int32_t size = get_size( orig_size, side, price );
                if(orig_size != size)
                {
                    if( (price / _mpi) % _params.skip_mask == 0 )
                        mark_level_to_move( side, it, price );
                }
                ++it; 
            }
            else
                assert(false);
        }

        for(auto &it : levels_to_remove)
        {
            mark_level_to_delete( side, it );
        }

        execute_marks( side );
    }

    void update_bid()
    {
        oi::SideType side = oi::SideType::BUY;        
    
        clear_marks(side); 
    
        std::list <level_it_type> levels_to_remove;
    
        int64_t min_price = _params.top_bid;
        int64_t max_price = _params.top_bid - _mpi * ( _params.num_levels - 1 );
    
        for(auto it = _book->bid.levels.begin(); it != _book->bid.levels.end(); ++it )
        {    
            if( it->first > min_price || it->first < max_price || ((it->first / _mpi) % _params.skip_mask != 0) ) 
                levels_to_remove.push_back(it);
        }   
 
        auto it = _book->bid.levels.rbegin();
        while( it !=  _book->bid.levels.rend() && it->first > min_price ) ++it;

        for(int64_t price = min_price; price >= max_price; price -= _mpi )
        {
            if(it == _book->bid.levels.rend())
            {
                if(levels_to_remove.size() == 0 )
                {
                    if( (price / _mpi) % _params.skip_mask == 0 )
                        mark_level_to_create( side, price );
                }
                else
                {
                    if( (price / _mpi) % _params.skip_mask == 0 )
                    {
                        mark_level_to_move( side, *levels_to_remove.begin(), price );
                        levels_to_remove.pop_front();
                    }
                }
            } 
            else if(price > it->first)
            {
                if( (price / _mpi) % _params.skip_mask == 0 )
                    mark_level_to_create( side, price );
            }
            else if( price == it->first)
            {
                int32_t orig_size = size_at_level(it);
                int32_t size = get_size(orig_size, side, price );
                if(orig_size != size)
                {
                    if( (price / _mpi) % _params.skip_mask == 0 )
                        mark_level_to_move( side, it, price );
                }
                ++it; 
            }
            else
            {
                assert(false);
            }
        }

        for(auto &it : levels_to_remove)
        {
            mark_level_to_delete( side, it );
        }

        execute_marks( side );
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
    
    void delete_works(oi::SideType side, int64_t order_id)
    {
        oi::outgoing_order * o = &_ask_marks[_ask_marks.size()];
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
        o->action = oi::OrderActionType::CANCEL;
        o->order_id = order_id; 
    }
    
    void modify_works( oi::SideType side, int32_t &size, int64_t price, int64_t order_id)
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
        o->action = oi::OrderActionType::MODIFY;
        o->side = side; 
        o->price = price;
        o->size = osize; 
        o->order_id = order_id; 
    }

    int32_t get_size( int32_t orig_size, oi::SideType side, int64_t price)
    {
        if(side == oi::SideType::SELL)
        {
            assert( price - _params.top_ask >= 0 );
            return (*_params.ask_utility)(orig_size,  ( price - _params.top_ask ) / _mpi );
        }
        else
        {
            assert( _params.top_bid - price >= 0 );
            return (*_params.bid_utility)(orig_size,  (_params.top_bid - price) / _mpi );
        }
    }
 
    void mark_level_to_create(oi::SideType side, int64_t price)
    {
        int32_t size = get_size( 0, side, price );
        while( size > 0 ) new_works(side, size, price);
    }
    
    template <typename IteratorType>
    void mark_level_to_move(oi::SideType side, IteratorType it, int64_t price )
    {
        auto order_it = it->second.orders.begin();
        int32_t orig_size = size_at_level(it);
        int32_t size = get_size( orig_size, side, price );

        while( size > 0 )
        {
            if(order_it == it->second.orders.end())
                new_works(side, size, price);
            else 
            {
                if(order_it->owner == (int64_t)this)
                    modify_works( side, size, price, order_it->order_id );
                ++order_it;
            }
        }
        
        while( order_it != it->second.orders.end() )
        {
            delete_works(side, order_it->order_id );
            ++order_it;
        }
    }
    
    template <typename IteratorType>
    void mark_level_to_delete(oi::SideType side, IteratorType it )
    {
        for(auto &oo : it->second.orders)
        {
            if( oo.owner != (int64_t)this )
                continue;
            delete_works(side, oo.order_id);
        }
    }

    template <typename IteratorType>
    int32_t size_at_level(IteratorType it)
    {
        int32_t size = 0;
        for(auto &oo : it->second.orders)
        {
            if( oo.owner != (int64_t)this )
                continue;
            size += oo.data->original.size;
        }
        return size;
    }    

    void cancel_all_orders()
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

    void prepare_order(oi::outgoing_order & o)
    {
        o.sec_id =_sec_id; 
        o.order_type = oi::OrderType::LIMIT; 
        o.tif = oi::TimeInForceType::DAY; 
        o.owner = (int64_t)this;
    }

private:

    oi::order_handler *_order_handler; 
    oi::order_manager_interface::order_book_type *_book = nullptr; 
    order_stacker_params _params;
    int64_t _sec_id;

    bool _enabled = false;

    bool _has_bid_fade = false;
    bool _has_ask_fade = false;
    
    int64_t _bid_fade = 0;
    int64_t _ask_fade = 0;

    int64_t _mpi = 0;

    std::vector<oi::outgoing_order> _ask_marks;
    std::vector<oi::outgoing_order> _bid_marks;
};

}

#endif // ELEMENTS_ORDER_STACKER_H
