#ifndef ELEMENTS_INSTRUMENT_STATE_H
#define ELEMENTS_INSTRUMENT_STATE_H

#include <easy_time.h>
#include <boost/circular_buffer.hpp>

using namespace elements;

struct snapshot
{
    snapshot() { reset(); }

    double abs_vol;
    double buy_vol;
    double sell_vol;
    double high;
    double low; 
    double open;
    double close;
    
    double top_bid_price;
    double bid_size[5];
    double top_ask_price;
    double ask_size[5];
    double microprice;

    void reset()
    {
        abs_vol  = 0;
        buy_vol = 0;
        sell_vol = 0;
        high = NAN;
        low = NAN; 
        open = NAN;
        close = NAN;
    }

    void on_trade( cme::MDEntry const & r)
    {
        if(r.AggressorSide == cme::AggressorSideEnum::BUY)
        {
            buy_vol += r.MDEntrySize;
            abs_vol += r.MDEntrySize;
        }
        else if(r.AggressorSide == cme::AggressorSideEnum::SELL)
        {
            sell_vol += r.MDEntrySize;
            abs_vol += r.MDEntrySize;
        }
        else return;
        
        if(isnan(high) || (high  < r.MDEntryPx))
            high = r.MDEntryPx;

        if(isnan(low) || (low > r.MDEntryPx))
            low = r.MDEntryPx;

        close = r.MDEntryPx;
    }
};


std::ostream & operator << (std::ostream & os, snapshot const & s)
{
    os <<  " abs_vol=" << s.abs_vol;
    os <<  " buy_vol=" << s.buy_vol;
    os <<  " sell_vol=" << s.sell_vol;
    os <<  " high=" << s.high * 1.0e-7;
    os <<  " low=" << s.low * 1.0e-7; 
    os <<  " open=" << s.open * 1.0e-7;
    os <<  " close=" << s.close * 1.0e-7;
    os <<  " microprice=" << s.microprice * 1.0e-7;

    return os;
}

class symbol_state
{
public:

    symbol_state(instrument * ins) : _instrument(ins)
    {
        _book = &_instrument->book();
        _mpi = _book->min_price_increment(); 
    }

    ~symbol_state(){}

    instrument * get_instrument() { return _instrument; }

    void on_market_data(timestamp ts) 
    {
    }

    void on_trade(cme::MDEntry &r, timestamp ts) 
    {
        _current.on_trade(r);
    }

    void on_timer(timestamp & t)
    {
        if(_book->has_top())
        {
            _current.top_bid_price = _book->top_price(BookSide::BID);
            _current.top_ask_price = _book->top_price(BookSide::ASK);
            for(int i = 0; i < 5; ++i)
            {
                _current.bid_size[i] = _book->size_at_price(_current.top_bid_price - i * _mpi, BookSide::BID);
                _current.ask_size[i] = _book->size_at_price(_current.top_ask_price + i * _mpi, BookSide::ASK);
            }

            _current.microprice = _book->microprice();
        }
        else
        {
            _current.top_bid_price = NAN; 
            _current.top_ask_price = NAN;
            for(int i = 0; i < 5; ++i)
            {
                _current.bid_size[i] =  NAN;
                _current.ask_size[i] =  NAN;
            }
            _current.microprice = NAN;
        }

        _snapshots.push_front(_current);
        _current.reset();
    }

    
    boost::circular_buffer<snapshot> & get_snapshots() { return _snapshots;}

private:

    snapshot _current;
    instrument *_instrument = nullptr;
    price_level_book       const     *_book; 
    uint64_t                          _mpi;
    boost::circular_buffer<snapshot> _snapshots{1000};
};

#endif // ELEMENTS_INSTRUMENT_STATE_H
