#ifndef ELEMENTS_EVENT_TRADER_UTILS_H
#define ELEMENTS_EVENT_TRADER_UTILS_H

#include <symbol_state.h>
#include <string>

namespace elements
{

struct event_trader_instrument_state
{
    std::string ticker;
    double beta;
    int32_t max_position = 0;
    int32_t penetration = 0;
    int32_t stop_loss = 0;
    int32_t stop_gain =0; 

    symbol_state *state = nullptr;
    
    double ref_price = NAN;
    double fair = NAN;
    double dollar_reaction = NAN;
};

struct event_trader_variable
{
    double survey_median = NAN;
    double survey_low = NAN;
    double survey_high = NAN;
    double survey_average = NAN;
    double last = NAN; 
};

class event_trader_fair_calculator
{
public:
    virtual ~event_trader_fair_calculator(){}
    virtual void fair( std::vector<event_trader_instrument_state> & states, eco_event & ev) = 0;
};

}

#endif //ELEMENTS_EVENT_TRADER_UTILS_H
