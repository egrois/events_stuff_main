#ifndef ELEMENTS_ECO_EVENT_H
#define ELEMENTS_ECO_EVENT_H

#include <string>
#include <vector>
#include <easy_time.h>

namespace elements
{
struct eco_component 
{
    std::string bb_name;
    std::string bb_field;
    bool critical;
    double out_value = 0.0;
    bool out_is_ready = false;
    timestamp out_ts;
};

std::ostream & operator << (std::ostream & os, eco_component const & comp )
{
    os     << "    bb_name=" << comp.bb_name
                << " bb_field=" << comp.bb_field
                << " critical=" << comp.critical
                << " out_value=" << comp.out_value
                << " out_is_ready=" << comp.out_is_ready
                << " out_ts=" << str(comp.out_ts);
    return os;
}

struct eco_event
{
    timestamp ts; 
    timestamp last_ts; 
    std::string event_id;
    std::vector<eco_component> components;

    bool is_armed               = false;
    bool is_critical_sent       = false;
    bool is_all_sent            = false;

    bool is_critical_ready()
    {
        for(auto comp: components)
        {
            //std::cout << "is_critical_ready " << comp << std::endl;
            if(comp.critical && !comp.out_is_ready)
                return false;
        }
        return true;
    }

    bool is_all_ready()
    {
        for(auto comp: components)
        {
            //std::cout  << "is_all_ready " << comp << std::endl;
            if(!comp.out_is_ready)
                return false;
        }
        return true;
    }

    bool operator < (eco_event const & a) { return ts < a.ts; }

};

std::ostream & operator << (std::ostream & os, eco_event const & ev )
{
    os  << " ts=" << str(ev.ts)
        << " last_ts=" << str(ev.last_ts)
        << " event_id=" << ev.event_id
        << " is_armed=" << ev.is_armed
        << " is_critical_sent=" << ev.is_critical_sent
        << " is_all_sent=" << ev.is_all_sent << std::endl;
    for(auto & comp: ev.components)
        os << comp << std::endl;
    return os;
} 

}
#endif //ELEMENTS_ECO_EVENT_H

