#ifndef ELEMENTS_EVENT_TRADER_FAIR_CALCULATOR_H
#define ELEMENTS_EVENT_TRADER_FAIR_CALCULATOR_H

#include <event_trader_utils.h>
#include <eco_event.h>
#include <params.pb.h>

#include <etfc/etfc_employment_situation.h>
#include <etfc/etfc_baker_hughes.h>

namespace elements
{

class etfc_factory
{
public:

    static event_trader_fair_calculator * get(params::EventTrader const & par_event_trader)
    {
        auto event_id = par_event_trader.event_id();
        event_id = event_id.substr(0, event_id.length() - 9);

        if(event_id == "Payrolls")
        {
            return new etfc_employment_situation( par_event_trader ); 
        }
        else if (event_id == "BakerHughes")
        {
            return new etfc_baker_hughes( par_event_trader ); 
        }
        else
        {
            std::cout << "No fair calc for " << event_id << std::endl;
            assert(false);
        }
    }
};

}

#endif // ELEMENTS_EVENT_TRADER_FAIR_CALCULATOR_H 
