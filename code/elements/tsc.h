#ifndef ELEMENTS_TSC_H
#define ELEMENTS_TSC_H

#include <stdint.h>
#include <time.h>
#include <iostream>

/*
 * $ cat /proc/cpuinfo | grep -i tsc
 * flags we need  : ... tsc  rdtscp constant_tsc nonstop_tsc ...
 */

namespace elements
{
class TSC
{
public:

    TSC()
    {
        //struct timespec t1, t2;
        //clock_gettime(CLOCK_MONOTONIC, &t1);
        //auto begin = (*this)();
        //uint64_t i;
        //for (i = 0; i < 10000000; i++);
        //auto end = (*this)();
        //clock_gettime(CLOCK_MONOTONIC, &t2);
        //double diff = (double)(end - begin);
        //double diff2 = (t2.tv_sec - t1.tv_sec) * 1.0e9 + (double)(t2.tv_nsec - t1.tv_nsec); 
        //_nanos_per_tick = diff2 / diff; 
        //std::cout << "CALIBRATED TSC: " << _nanos_per_tick << std::endl;
    }

    uint64_t operator()()
    {
        unsigned int hi, lo;
        __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
        return ((uint64_t)hi << 32) | lo;
    }

    double nanos(uint64_t t1, uint64_t t2)
    {
        if ( t1 > t2 )
            return 0;
        else
            return ( t2 - t1 ) * _nanos_per_tick;
    }

    double nanos_per_tick(){ return _nanos_per_tick;  }

private:
    double _nanos_per_tick = 0.385; 

};
}

#endif // ELEMENT_TSC_H 
