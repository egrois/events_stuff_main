#ifndef ELEMENTS_EASY_TIME_H
#define ELEMENTS_EASY_TIME_H

#include <chrono>
#include <sys/time.h>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

namespace elements
{
    typedef std::chrono::system_clock::time_point timestamp;
    typedef std::chrono::system_clock clock;

    typedef std::chrono::nanoseconds nanoseconds; 
    typedef std::chrono::microseconds microseconds;
    typedef std::chrono::milliseconds milliseconds;
    typedef std::chrono::seconds seconds;
    typedef std::chrono::minutes minutes;
    typedef std::chrono::hours hours;

    typedef boost::gregorian::date date;

    std::string str(const timestamp& tp);
    std::string str_date(const timestamp& tp);
    std::string str_date2(const timestamp& tp);

    timestamp make_time(std::string date, std::string time);

    timeval to_timeval(const timestamp& tp);
    timestamp from_timeval(const timeval& tv);

    timespec to_timespec(const timestamp& tp);
    timestamp from_timespec(const timespec& ts);

    timespec to_timespec(const elements::nanoseconds &nano);
    
    tm to_tm(const timestamp& tp);
    timestamp from_tm(const tm& t);

    time_t to_time_t(const timestamp& tp);
    timestamp from_time_t(const time_t& t);

    date get_date(const timestamp& tp);
    seconds seconds_since_midnigth(const timestamp& tp);

    uint64_t to_epoch_nanos( const timestamp & tp);
    timestamp from_epoch_nanos( uint64_t t);

    timestamp from_fix_time(char const *buf);
}

#endif // ELEMENTS_EASY_TIME_H
