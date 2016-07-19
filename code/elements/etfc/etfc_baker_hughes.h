#ifndef ELEMENTS_ETFC_BAKER_HUGHES_H
#define ELEMENTS_ETFC_BAKER_HUGHES_H

#include <event_trader_utils.h>
#include <eco_event.h>
#include <params.pb.h>

namespace elements
{

class etfc_baker_hughes : public event_trader_fair_calculator
{
public:
    etfc_baker_hughes(params::EventTrader const & par_event_trader)
    {
        int num = par_event_trader.event_trader_variable_size();         
        for(int i = 0; i < num; ++i)
        {
            auto var = par_event_trader.event_trader_variable(i);  
            if(var.bb_name_match() != "BAKENAM Index")
                continue;
            _prev = var.last();
        }

        std::cout << "_prev=" << _prev << std::endl;
    }

    virtual ~etfc_baker_hughes(){}

    virtual void fair( std::vector<event_trader_instrument_state> & states, eco_event & ev)
    {
        for(auto comp: ev.components)        
        {
            if(comp.bb_name == "BAKENAM Index")
            {
                if( comp.bb_field == "LAST_PRICE")
                    _actual = comp.out_value;                
            }
        }
       

        // TODO: not working 
        double factor = 10.0 * (_actual - _prev);
        for( auto s : states )
        {
            s.dollar_reaction = ( ( s.ref_price * factor ) / 10000.0 ) * s.state->get_instrument()->dollars_per_unit();
            s.fair = s.ref_price + ( s.dollar_reaction / s.state->get_instrument()->dollars_per_unit());
        }
    }

private:

    double _prev = NAN;
    double _actual = NAN;
};

}

#endif
