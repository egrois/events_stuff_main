#ifndef ELEMENTS_PRICE_LEVEL_BOOK_H
#define ELEMENTS_PRICE_LEVEL_BOOK_H

#include <easy_time.h>
#include <md.h>
#include <global_price_multiplier.h>

namespace elements
{

enum BookSide
{
    BID= 0,
    ASK = 1
};

struct price_level
{
    price_level() : size(0),num_orders(0), time(timestamp()){}

    int32_t size;
    int32_t num_orders;
    timestamp time;
};

class price_level_book
{
public:

    price_level_book(
        cme::SecurityDefinition const & secdef, 
        bool apply_trades, 
        bool is_implied,
        double variance = 0.25) 
            : _apply_trades(apply_trades), 
              _is_implied(is_implied) 
    {
        _security_id = secdef.SecurityID;     
        
        _min_price_increment = (int64_t)(secdef.MinPriceIncrement * gpm::mult_double + 1.0e-10); 

        // TODO
        double sd_ref_price = 0.0;
        if(secdef.has_TradingReferencePrice)
        {
            sd_ref_price = secdef.TradingReferencePrice;
        }
        else if(secdef.has_LowLimitPrice && secdef.has_HighLimitPrice)
        {
            sd_ref_price = 0.5 * (secdef.HighLimitPrice + secdef.LowLimitPrice);
        }
        else if (secdef.SecurityDesc[0] == 'G' && secdef.SecurityDesc[1] == 'E')
        {
           sd_ref_price = 10000.0; 
        }
        else
        {   
            std::cout << "cannot establish reference price " << std::endl;
            std::cout << secdef << std::endl;
            assert(false);
        }

        double epsilon = (sd_ref_price >= 0) ? 1.0e-10 : -1.0e-10;
        int64_t ref = (int64_t)(sd_ref_price * gpm::mult_double + epsilon); 
    
        int64_t levels_away = (int64_t)((ref * variance) / _min_price_increment);

        int64_t hi = ref + levels_away * _min_price_increment;
        int64_t lo = ref - levels_away * _min_price_increment;

        _ref_price = lo;
        _reserve_levels = (int64_t)((hi - lo) / _min_price_increment) + 1;        

        _bid.resize(_reserve_levels);
        _ask.resize(_reserve_levels);       
        _has_top_bid = false;
        _has_top_ask = false;
    }
    
    ~price_level_book(){}

    bool has_top() const 
    {
        if(_has_top_bid && _has_top_ask)
            return true;
        else
            return false;
    }

    int64_t top_price( BookSide side) const
    {
        if(side == BookSide::BID)
        {
            assert(_has_top_bid == true);
            return _top_bid;
        } 
        else
        {
            assert(_has_top_ask == true);
            return _top_ask;
        }    
    }
   
    int32_t size_at_price(int64_t price, BookSide side) const
    {
        if(side == BookSide::BID)
            return _bid[idx(price)].size;
        else
            return _ask[idx(price)].size;
    }  
    
    int32_t num_orders_at_price(int64_t price, BookSide side) const
    {
        if(side == BookSide::BID)
            return _bid[idx(price)].num_orders;
        else
            return _ask[idx(price)].num_orders;
    }
    
    timestamp time_at_price(int64_t price, BookSide side) const
    {
        if(side == BookSide::BID)
            return _bid[idx(price)].time;
        else
            return _ask[idx(price)].time;
    }
    
    int32_t idx(int64_t price) const
    {
        if(price % _min_price_increment != 0)
        {
            std::cout << *_recent << std::endl;
            std::cout << " price not on grid" << std::endl;
            std::cout   << "secid= " << _security_id << " price=" << price << " minincr=" 
                        << _min_price_increment << std::endl;
            throw std::runtime_error("");
        }

        int32_t index = (price - _ref_price) / _min_price_increment; 

        if(index < 0)
        {
            std::cout << *_recent << std::endl;
            std::cout << " index < 0" << std::endl;
            std::cout   << "secid= " << _security_id << " price=" << price << " minincr=" 
                        << _min_price_increment << std::endl;
            throw std::runtime_error("");
        }
        
        if(index >= _reserve_levels)
        {
            std::cout   << *_recent << std::endl; 
            std::cout   << "price=" << price 
                        << " ref_price=" << _ref_price
                        << " min_incr=" << _min_price_increment
                        << " Secid=" << _security_id 
                        << " index exceeds reserve levels index=" << index 
                        << " reserved=" << _reserve_levels << std::endl;
            throw std::runtime_error("");
        }
        return index;     
    }
    
    double midquote() const
    { 
        assert(has_top() == true);
        return ((double)_top_bid + (double)_top_ask) * 0.5; 
    }

    double spread() const
    {
        assert(has_top() == true);
        return (double)_top_ask - (double)_top_bid; 
    }

    double microprice() const
    { 
        assert(has_top() == true);
        double b = (double)size_at_price(_top_bid, BookSide::BID);
        double a = (double)size_at_price(_top_ask, BookSide::ASK);
        return ((double)_top_bid * a + (double)_top_ask * b) / (double)(b + a);
    }
      
    double microprice2() const 
    { 
        assert(has_top() == true);
        double b = (double)num_orders_at_price(_top_bid, BookSide::BID);
        double a = (double)num_orders_at_price(_top_ask, BookSide::ASK);
        return (double)(_top_bid * a + _top_ask * b) / (double)(b + a);
    }  
       
    double imbalance() const
    {
        assert(has_top() == true);
        double a = 0;
        double b = 0;
        for(int i = 0; i < 5; ++i)
        {
            a += (double)size_at_price(_top_ask + i * _min_price_increment, BookSide::ASK);
            b += (double)size_at_price(_top_bid - i * _min_price_increment, BookSide::BID);
        }
        return (b - a)/(b + a);
    }
    
    double imbalance2() const
    {
        assert(has_top() == true);
        double a = 0;
        double b = 0;
        for(int i = 0; i < 5; ++i)
        {
            a += (double)num_orders_at_price(_top_ask + i * _min_price_increment, BookSide::ASK);
            b += (double)num_orders_at_price(_top_bid - i * _min_price_increment, BookSide::BID);
        }
        return (b - a)/(b + a);
    }
    
    double imbalance3() const
    {
        assert(has_top() == true);
        double a = 0;
        double b = 0;
        for(int i = 0; i < 5; ++i)
        {
            a +=    (double)size_at_price(_top_ask + i * _min_price_increment, BookSide::ASK) * 
                    (double)num_orders_at_price(_top_ask + i * _min_price_increment, BookSide::ASK);
            b +=    (double)num_orders_at_price(_top_bid - i * _min_price_increment, BookSide::BID) *
                    (double)size_at_price(_top_bid - i * _min_price_increment, BookSide::BID);
        }
        return (b - a)/(b + a);
    }
 
    void on_incremental_refresh(cme::IncrementalRefresh const & msg)
    {
        _recent = (cme::IncrementalRefresh const * )&msg;
        for( auto rec : msg.MDEntries )
        {
            if(!rec.has_SecurityID || rec.SecurityID != _security_id )
                continue;

            assert(rec.has_RptSeq);
            assert(rec.has_MDEntryType); 

            if( ( _seq >= 0 ) && (rec.RptSeq - _seq) > 1)
            {
                std::cout << "WARN: SEQ_GAP id=" << rec.SecurityID << " " << _seq << " " << rec.RptSeq << std::endl;
            }

            if(rec.RptSeq > _seq)
                _seq = rec.RptSeq;

            if( !_is_implied && rec.QuoteCondition == cme::QuoteConditionEnum::IMPLIED)
                continue;

            if(_is_implied  && rec.QuoteCondition != cme::QuoteConditionEnum::IMPLIED)
                continue; 

            if(rec.MDEntryType == cme::MDEntryTypeEnum::DUMMY_MDP3)
                continue;

            try { idx(rec.MDEntryPx); } catch(...) { continue; }

            switch(rec.MDEntryType)
            {
                case cme::MDEntryTypeEnum::BID:
                    assert(rec.has_MDUpdateAction == true);
                    if(rec.MDUpdateAction == cme::MDUpdateActionEnum::NEW)
                    {
                        auto & pp = _bid[idx(rec.MDEntryPx)];                       
                        pp.size = rec.MDEntrySize; 
                        pp.num_orders = rec.NumberOfOrders;
                        pp.time = msg.ReceivedTime; 
                
                        if(rec.MDPriceLevel == 1)
                        {
                            _top_bid = rec.MDEntryPx;

                            if(has_top() && _top_bid >= _top_ask)
                            {
                                _top_ask = _top_bid + _min_price_increment;
                            }
                            _has_top_bid = true;
                        }
                    }
                    else if(rec.MDUpdateAction == cme::MDUpdateActionEnum::CHANGE) 
                    {
                        auto & pp = _bid[idx(rec.MDEntryPx)];                       
                        pp.size = rec.MDEntrySize; 
                        pp.num_orders = rec.NumberOfOrders;
                        pp.time = msg.ReceivedTime; 
                        
                        if(rec.MDPriceLevel == 1)
                        {
                            _top_bid = rec.MDEntryPx;
                            _has_top_bid = true;
                        }
                    }
                    else if(rec.MDUpdateAction == cme::MDUpdateActionEnum::DELETE)
                    {
                        if(rec.MDPriceLevel == 1)
                        {
                            auto & pp = _bid[idx(rec.MDEntryPx)];                       
                            pp.size = 0; 
                            pp.num_orders = 0;
                            pp.time = msg.ReceivedTime; 
                            
                            if( _has_top_bid )
                            {
                                if( _top_bid >= rec.MDEntryPx )
                                {
                                    _top_bid = rec.MDEntryPx - _min_price_increment;
                                    _has_top_bid = false; 
                                    for(int i = 0; i < 10; ++i) // TODO
                                    {
                                        if(size_at_price(_top_bid, BID) <= 0 )
                                        { 
                                            _top_bid -= _min_price_increment;
                                        }
                                        else
                                        {
                                            _has_top_bid = true;
                                            break;
                                        }   
                                    }  
                                }                 
                            }                         
                        }
                    }
                    break; 
                case cme::MDEntryTypeEnum::OFFER:
                    assert(rec.has_MDUpdateAction == true);
                    if(rec.MDUpdateAction == cme::MDUpdateActionEnum::NEW)
                    {
                        auto & pp = _ask[idx(rec.MDEntryPx)];                       
                        pp.size = rec.MDEntrySize; 
                        pp.num_orders = rec.NumberOfOrders;
                        pp.time = msg.ReceivedTime; 
                
                        if(rec.MDPriceLevel == 1)
                        {
                            _top_ask = rec.MDEntryPx;

                            if(has_top() && _top_ask <= _top_bid)
                            {
                                _top_bid = _top_ask - _min_price_increment;
                            }
                            _has_top_ask = true;
                        }
                    }
                    else if(rec.MDUpdateAction == cme::MDUpdateActionEnum::CHANGE) 
                    {
                        auto & pp = _ask[idx(rec.MDEntryPx)];                       
                        pp.size = rec.MDEntrySize; 
                        pp.num_orders = rec.NumberOfOrders;
                        pp.time = msg.ReceivedTime; 
                        
                        if(rec.MDPriceLevel == 1)
                        {
                            _top_ask = rec.MDEntryPx;
                            _has_top_ask = true;
                        }
                    }
                    else if(rec.MDUpdateAction == cme::MDUpdateActionEnum::DELETE)
                    {
                        if(rec.MDPriceLevel == 1)
                        {
                            auto & pp = _ask[idx(rec.MDEntryPx)];                       
                            pp.size = 0; 
                            pp.num_orders = 0;
                            pp.time = msg.ReceivedTime; 
                            if( _has_top_ask )
                            {
                                if( _top_ask >= rec.MDEntryPx )
                                {
                                    _top_ask = rec.MDEntryPx + _min_price_increment;
                                    _has_top_ask = false; 
                                    for(int i = 0; i < 10; ++i) // TODO
                                    {
                                        if(size_at_price(_top_ask, ASK) <= 0 )
                                        { 
                                            _top_ask += _min_price_increment;
                                        }
                                        else
                                        {
                                            _has_top_ask = true;
                                            break;
                                        }   
                                    }  
                                }                 
                            }                         
                        }
                    }
                    break; 
                case cme::MDEntryTypeEnum::TRADE:
                    
                    if(!_apply_trades || !has_top() || rec.has_TradeCondition )
                        break;
                    
                    if(rec.MDEntryPx == _top_ask)
                    {
                        auto & pp = _ask[idx(rec.MDEntryPx)];                       
                        pp.size -= rec.MDEntrySize;
                        pp.num_orders -= 1;
                        if(pp.size <= 0)
                        {
                            _top_ask += _min_price_increment;
                        }
                        else if( pp.num_orders <= 0)
                        {
                            pp.num_orders = 1;
                        }
                    }
                    else if (rec.MDEntryPx == _top_bid)
                    {
                        auto & pp = _ask[idx(rec.MDEntryPx)];                       
                        pp.size -= rec.MDEntrySize;
                        pp.num_orders -= 1;
                        if(pp.size <= 0)
                        {
                            _top_bid -= _min_price_increment;
                        }
                        else if(pp.num_orders <= 0)
                        {
                            pp.num_orders = 1;
                        }
                    }
                    break; 
                default:
                    break;
            }
            // print_top(std::cout);
            // std::cout << std::endl;
        } 
    }

    std::ostream & print_top(std::ostream & os) const 
    {
        if(!has_top()) return os;
        
        int64_t b = _top_bid; 
        int64_t a = _top_ask;

        //std::cout << _top_bid << " " << _top_ask << std::endl;

        int32_t b_size = size_at_price(b, BookSide::BID);
        int32_t a_size = size_at_price(a, BookSide::ASK);

        int32_t b_num = num_orders_at_price(b, BookSide::BID);
        int32_t a_num = num_orders_at_price(a, BookSide::ASK);

        timestamp b_time = time_at_price(b,BookSide::BID);
        timestamp a_time = time_at_price(a,BookSide::ASK);

        os  << "BID("
            << str(b_time) << "," 
            << std::setw(10) << b << ","
            << std::setw(3)<< b_size << ","
            << std::setw(3)<< b_num 
            << ") ASK("
            << str(a_time) << ","
            << std::setw(10) << a << ","
            << std::setw(3) << a_size << ","
            << std::setw(3)<< a_num
            << ")";      
        return os;
    }

    std::ostream & print(std::ostream & os, int num_levels) const
    {
        if(!has_top()) return os;
        
        BookSide side = BookSide::BID;
        
        for(int i = 0; i < num_levels; ++i)
        {
            int64_t price = top_price(side) - i * _min_price_increment;
            os << "BID "
               << price << " " 
               << size_at_price(price, side) << " "
               << num_orders_at_price(price, side) << " "
               << str(time_at_price(price, side)) 
               << std::endl;
        }

        side = BookSide::ASK;
        for(int i = 0; i < num_levels; ++i)
        {
            int64_t price = top_price(side) + i * _min_price_increment;
            os << "ASK "
               << price << " " 
               << size_at_price(price, side) << " "
               << num_orders_at_price(price, side) << " "
               << str(time_at_price(price, side)) 
               << std::endl;
        }
        return os;
    }
       
    int64_t min_price_increment() const { return _min_price_increment; }

private:
    
    bool _has_top_bid = false;
    bool _has_top_ask = false;
    
    int32_t _security_id;
    bool _apply_trades;
    int64_t _min_price_increment;
    int64_t _ref_price; 
    int32_t _reserve_levels;
    bool _is_implied;

    int64_t _top_bid = 0;
    int64_t _top_ask = 0;

    std::vector<price_level>   _bid;
    std::vector<price_level>   _ask;

    int32_t _seq = -1;

    cme::IncrementalRefresh const *  _recent; 
};

};

#endif // ELEMENTS_PRICE_LEVEL_BOOK_H
