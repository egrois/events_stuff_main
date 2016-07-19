#ifndef ELEMENTS_ETFC_EMPLOYMENT_SITUATION_H
#define ELEMENTS_ETFC_EMPLOYMENT_SITUATION_H

#include <event_trader_utils.h>
#include <eco_event.h>
#include <params.pb.h>

namespace elements
{

class etfc_employment_situation : public event_trader_fair_calculator
{
public:
    etfc_employment_situation(params::EventTrader const & par_event_trader)
    {
        int num = par_event_trader.event_trader_variable_size();         
        for(int i = 0; i < num; ++i)
        {
            auto var = par_event_trader.event_trader_variable(i);  
            if(var.bb_name_match() != "NFP TCH Index")
                continue;
            _tch_expected = var.survey_median();
            _tch_prev = var.last();
        }

        std::cout << "_tch_expected=" << _tch_expected << " _tch_prev=" << _tch_prev << std::endl;
    }

    virtual ~etfc_employment_situation(){}

    virtual void fair( std::vector<event_trader_instrument_state> & states, eco_event & ev)
    {
        for(auto comp: ev.components)        
        {
            if(comp.bb_name == "NFP TCH Index")
            {
                if( comp.bb_field == "LAST_PRICE")
                    _tch_actual = comp.out_value;                
                else if(comp.bb_field == "ASK")
                    _tch_rev = comp.out_value;
            }
        }

        double tch_surp = _tch_actual - _tch_expected;
        double tch_rev_surp = _tch_rev - _tch_prev;

        std::cout   << "EVENT "
                    << " tch_act=" << _tch_actual
                    << " tch_exp=" << _tch_expected
                    << " tch_rev=" << _tch_rev
                    << " tch_pre=" << _tch_prev
                    << " tch_sur=" << tch_surp 
                    << " tch_rsu=" << tch_rev_surp << std::endl; 

        // ZN return in basis points 
        double factor = -0.4244 * tch_surp - 0.1753 * tch_rev_surp;

        double zn_beta = NAN;
        double zn_dollar_reaction = NAN;


        for( auto s : states )
        {
            if(s.state->get_instrument()->symbol().substr(0,2) == "ZN")
            {
                zn_beta = s.beta;
                zn_dollar_reaction = ( ( s.ref_price * factor ) / 10000.0 ) * 
                                        s.state->get_instrument()->dollars_per_unit();
                //std::cout   << "ZN "
                //            << " ref_price=" << s.ref_price * 1.0e-7
                //            << " factor=" << factor
                //            << " dollars_per_unit=" <<  s.state->get_instrument()->dollars_per_unit() << std::endl;
            }
        }
        
        for( auto & s : states )
        {
            double beta = s.beta / zn_beta;
            s.dollar_reaction = beta * factor;
            s.fair = s.ref_price + s.dollar_reaction / s.state->get_instrument()->dollars_per_unit();

            std::cout << s.state->get_instrument()->symbol() << " " 
                    << " beta=" << beta
                    << " ref_price=" <<  s.ref_price * 1.0e-7
                    << " dollar_reaction=" << s.dollar_reaction
                    << " dollarts_per_unit=" << s.state->get_instrument()->dollars_per_unit() << std::endl;
        }
    }

private:
    double _tch_expected = NAN;
    double _tch_prev = NAN;
    double _tch_actual = NAN;
    double _tch_rev = NAN;
};

}

#endif
