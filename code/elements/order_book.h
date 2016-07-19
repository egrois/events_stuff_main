#ifndef ELEMENTS_ORDER_BOOK_H
#define ELEMENTS_ORDER_BOOK_H

#include <easy_time.h>
#include <set>

namespace elements
{
namespace ob
{
template <typename ORDER_DATA>
struct order
{
    timestamp ts;
    int64_t order_id;
    ORDER_DATA data;
    int64_t owner;
    bool operator<(order const & o) const { return ts < o.ts;}
};

template<typename ORDER_DATA>
struct price_level
{
    std::set<order<ORDER_DATA> > orders;
};

template<typename ORDER_DATA>
struct side
{
    bool is_bid;
    std::map<int64_t, price_level<ORDER_DATA> > levels;
};

template<typename ORDER_DATA, typename MARKET>
struct order_book
{
    order_book(){}
    order_book(MARKET mkt){ market = mkt; }
    MARKET market; 
    side<ORDER_DATA> bid, ask;

    bool add_order( int64_t order_id,
                    bool is_bid,
                    int64_t price, 
                    timestamp ts,
                    ORDER_DATA data,
                    int64_t owner_tag)
    {
        order<ORDER_DATA> ord;
        ord.ts = ts;
        ord.order_id = order_id;
        ord.data = data;
        ord.owner = owner_tag;
        
        if(is_bid)
        {
            if(bid.levels.find(price) == bid.levels.end())
            {
                price_level<ORDER_DATA> pl;
                pl.orders.insert(ord);
                bid.levels[price] = pl;
            }
            else
            {
                bid.levels[price].orders.insert(ord);
            }
        }
        else
        {
            if(ask.levels.find(price) == ask.levels.end())
            {
                price_level<ORDER_DATA> pl;
                pl.orders.insert(ord);
                ask.levels[price] = pl;
            }
            else
                ask.levels[price].orders.insert(ord);
        }    
        return true;
    }

    bool remove(int64_t order_id)
    {
        for(auto pl = bid.levels.begin(); pl != bid.levels.end(); ++pl)
        {
            for(auto ord = pl->second.orders.begin(); ord != pl->second.orders.end(); ++ord)
            {
                if(ord->order_id == order_id)
                {
                    if(pl->second.orders.size() == 1 )
                        bid.levels.erase(pl);
                    else
                        pl->second.orders.erase(ord);
                    return true;
                }
            }
        }

        for(auto pl = ask.levels.begin(); pl != ask.levels.end(); ++pl)
        {
            for(auto ord = pl->second.orders.begin(); ord != pl->second.orders.end(); ++ord)
            {
                if(ord->order_id == order_id)
                {
                    if(pl->second.orders.size() == 1 )
                        ask.levels.erase(pl);
                    else
                        pl->second.orders.erase(ord);
                    return true;
                }
            }
        }
        return false; 
    }
    
    std::ostream & print(std::ostream & os) const
    {
        os  << "num_bid_levels: " << bid.levels.size()
            << " num_ask_levels: " << ask.levels.size();
        return os;
    }
};

}
}

#endif //ELEMENTS_ORDER_BOOK_H
