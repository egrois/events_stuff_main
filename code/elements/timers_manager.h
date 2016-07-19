#ifndef ELEMENTS_TIMERS_MANAGER_H
#define ELEMENTS_TIMERS_MANAGER_H

#include <easy_time.h>
#include <set>
#include <tsc.h>

namespace elements
{
    struct timer
    {
        uint32_t id;
        bool is_oneshot;
        timestamp start;
        timestamp stop;
        milliseconds interval;
        timestamp next_time;
        uint32_t count = 0;

        bool compute_next_time(const timestamp &t)
        {
            if(count == 0)
            {
                next_time = start;
                return true;
            }
            
            if(is_oneshot)
            {
                return false;
            }
            
            next_time += interval;
            if(next_time > stop)
                return false;
            else
                return true;
        }
    };

    struct timer_record
    {
        timer_record():p(nullptr) {}
        timer_record(timer *t) : p(t) {}
        timer * p;
        bool operator < (const timer_record & a) const { return (p->next_time < a.p->next_time);}
        timestamp & next_time(){ return p->next_time; }
    };

template <typename Handler, typename TimeSource>
class timers_manager
{
public:

    timers_manager()
    {
        _timers = new timer[_max_timers];        
    }

    ~timers_manager()
    {
        delete [] _timers;
    }

    Handler * get_handler() { return _handler; }
    TimeSource * get_time_source() { return _time_source; } 

    bool init(Handler * handler, TimeSource * time_source)
    {
        _handler = handler;
        _time_source = time_source;
        return true;
    }

    uint32_t register_timer(timestamp start, timestamp stop, milliseconds interval)
    {
        auto ts = _time_source->now();
        timer *t = &_timers[_next_id];
        t->id = _next_id;
        t->is_oneshot = false;
        t->start = start;
        t->stop = stop;
        t->interval = interval;
        t->count = 0;
        t->compute_next_time(ts);
        ++_next_id;
        _records.insert(timer_record(t));     
        return t->id;
    }

    uint32_t register_timer(timestamp start)
    {
        auto ts = _time_source->now();
        timer *t = &_timers[_next_id];
        t->id = _next_id;
        t->is_oneshot = false;
        t->start = start;
        t->count = 0;
        t->compute_next_time(ts);
        ++_next_id;
        _records.insert(timer_record(t));     
        return t->id;
    }
    
    void cancel_timer(uint32_t timer_id)
    {
        _it = _records.begin();
        while(_it != _records.end())
        {
            if(_it->p->id == timer_id)
            {
                _records.erase(_it);
                return true;
            }
        }
        return false;
    }
    
    void poll()
    {
        uint64_t cycles = _tsc();
        if( _tsc.nanos(_last_checked, cycles) > _update_interval_nanos || cycles < _last_checked )
        {
            _last_checked = cycles;
            check_updates();
        }
    }

    void check_updates()
    {
        auto ts = _time_source->next();
        _it = _records.begin();

        while(_it != _records.end() && _it->p->next_time < ts)
        {
            _time_source->set_now(_it->p->next_time);
            _handler->on_timer_raw(_it->p->next_time, _it->p->id);
            timer * t = _it->p;
            _records.erase(_it);
            ++t->count;
            if(t->compute_next_time(ts)) 
                _records.insert(timer_record(t));
            _it = _records.begin();
        }
    }

private:

    std::multiset<timer_record> _records;
    Handler *_handler;
    TimeSource *_time_source;    
    uint32_t _next_id = 1;
    uint32_t _max_timers = 1000;
    timer *_timers;
    std::multiset<timer_record>::iterator _it;
    TSC _tsc;
    uint64_t _last_checked = 0;
    double _update_interval_nanos = 1000;
};

}

#endif // ELEMENTS_TIMERS_MANAGER_H
