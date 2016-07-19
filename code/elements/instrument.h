#ifndef ELEMENTS_INSTRUMENT_H
#define ELEMENTS_INSTRUMENT_H

#include <md.h>
#include <price_level_book.h>
#include <order_stacker.h>
#include <auto_stacker.h>

namespace elements
{

class instrument
{
public:
    instrument( cme::SecurityDefinition sd, bool apply_trades, bool maintain_implied, double dollars_per_tick, int32_t parameter_id, 
                int32_t pos_offset, double cash_offset ):
        _secdef(sd),
        _symbol(sd.SecurityDesc),
        _apply_trades(apply_trades),
        _maintain_implied(maintain_implied), 
        _book(sd, _apply_trades, false),
        _implied_book(sd, false, true),
        _dollars_per_tick(dollars_per_tick),
        _parameter_id(parameter_id),
        _cash(cash_offset),
        _pos(pos_offset)
    {
        _dollars_per_unit = ((double)_dollars_per_tick  * gpm::anti_mult_double) / (double) _secdef.MinPriceIncrement;
    }

    std::string const & symbol() { return _symbol; }
    cme::SecurityDefinition const & secdef() { return _secdef; }

    price_level_book const & book() { return _book; }
    price_level_book const & implied_book() { return _implied_book; } 
    
    price_level_book  & unsafe_book() { return _book; }
    price_level_book  & unsafe_implied_book() { return _implied_book; } 

    bool apply_trades() { return _apply_trades; }
    bool maintain_implied() { return _maintain_implied; } 
    double dollars_per_tick() { return _dollars_per_tick; } 
    double dollars_per_unit() { return _dollars_per_unit; } 

    void on_fill( int32_t size, int64_t price ) 
    {
        _pos += size;
        _cash -= (size * price * dollars_per_unit()); 
        _vol += abs(size);
        _ltp = price;
    }

    double pnl()
    {
        if(!book().has_top())
        {
            return NAN;
        }
        else
        {
            int64_t price = (_pos >= 0) ? book().top_price(BookSide::BID) : book().top_price(BookSide::ASK); 
            return _cash + (double)_pos * (double)price * dollars_per_unit();
        }
    }

    int32_t pos()
    {
        return _pos;
    }
    
    int32_t vol()
    {
        return _vol;
    }

    int64_t ltp()
    {
        return _ltp;
    }

    double cash()
    {
        return _cash;
    }

    void set_order_stacker( order_stacker * stacker ){ _stacker = stacker; }
    order_stacker *  get_order_stacker(){ return _stacker; }
    
    void set_auto_stacker( auto_stacker * stacker ){ _auto_stacker = stacker; }
    auto_stacker *  get_auto_stacker(){ return _auto_stacker; }

    int32_t parameter_id() { return _parameter_id; }

    void set_void_pointer(void *p){ _void_pointer = p; }
    void *get_void_pointer() { return _void_pointer; } 

private:

    cme::SecurityDefinition _secdef;
    std::string _symbol;
    bool _apply_trades;
    bool _maintain_implied;
    price_level_book _book;
    price_level_book _implied_book;
    double _dollars_per_tick;
    double _dollars_per_unit;
    int32_t _pos = 0;
    double _cash = 0; 
    int32_t _vol = 0;
    int64_t _ltp = 0;

    order_stacker * _stacker = nullptr;
    auto_stacker * _auto_stacker = nullptr;
    int32_t _parameter_id = -1;

    void *_void_pointer = nullptr;
};

}

#endif // ELEMENTS_INSTRUMENT_H
