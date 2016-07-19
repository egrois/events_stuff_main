#ifndef ELEMENTS_THREADED_RUNNER_H
#define ELEMENTS_THREADED_RUNNER_H

#include <thread>

namespace elements
{
class threaded_runner
{
public:
    threaded_runner() {}
    threaded_runner( int affinity ) { _affinity = affinity; }
    virtual ~threaded_runner() {}

    void run()
    {
        _is_running = true; 
        using namespace std;
        _thread = new std::thread( bind( &elements::threaded_runner::_run, this ) );
    }

    void stop()
    {
        _is_running = false;
        _thread->join();
        delete _thread;
    }

protected:
    bool _is_running = false;
    std::thread * _thread;
    int _affinity = -1;
    
    void _run()
    {
        if(_affinity >= 0 )
        {
            cpu_set_t mask;
            CPU_ZERO(&mask);
            CPU_SET((unsigned int)_affinity, &mask);  
            sched_setaffinity(0,sizeof(mask), &mask);
        } 

        while(true)
        {
            _process();
            if(_is_running == false)
                break;
        }
    }

    virtual void _process() {   }
};

}

#endif //ELEMENTS_THREADED_RUNNER_H
