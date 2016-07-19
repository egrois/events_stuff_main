#include "strategy.h" 
#include "params.pb.h"

using namespace elements;

class REPLACE : public strategy<params::Params>
{
public:
    REPLACE(){}
    virtual ~REPLACE(){}

    virtual void on_bb_event(eco_event const &event) 
    {
    }
    
    virtual void on_start_of_day()
    {
    }

    virtual void on_incremental_refresh(cme::IncrementalRefresh &msg) 
    {
    }
    
    virtual void on_security_status(cme::SecurityStatus &msg) 
    {
    }
 
    virtual void on_timer(timestamp & t, int32_t timer_id)
    {
    }

    virtual void on_order(timestamp ts, oi::order_event const & event)
    {
    }

    virtual void on_parameters_update(params::Params & msg) 
    {
    }
};

int main( int argc, char ** argv)
{
    std::string  par = (argc > 1) ? argv[1] : "strategy.param";
    REPLACE handler;
    REPLACE::EventLoopType e;
    e.initialize(par, handler);
    e.run();
    return 0;
}
