#ifndef ELEMENTS_OOB_LOGGER_H
#define ELEMENTS_OOB_LOGGER_H

#include "threaded_runner.h"
#include "circular_fifo.h"
#include <fstream>

namespace elements
{

template <typename Msg>
class oob_logger : public threaded_runner 
{
public:
    oob_logger(std::string file_name, int size, int affinity)
        :   threaded_runner(affinity),
            _queue(size)
            
    {
        _os.open(file_name, std::ofstream::out | std::ofstream::app);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        run();
    }
   
    ~oob_logger()
    {
        stop();
        _os.close();
    }

    void log(const Msg &msg)
    {
        _queue.push(msg);
    }

private:

    void _process()
    {
        int i = 0;
        while( !_queue.empty() )
        {
            ++i;
            _queue.pop(_msg);
            _os << _msg;
        }
        if(i > 1000) _os << "DANGER: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::nanoseconds(100));
    } 

    Msg _msg;
    circular_fifo <Msg> _queue; 
    std::ofstream _os;
};

}

#endif // OOB_LOGGER_H
