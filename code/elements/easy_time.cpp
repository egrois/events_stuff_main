#include "easy_time.h"
#include <sstream>
#include <iomanip>
#include <time.h>

namespace elements
{

std::string str(const timestamp& tp)
{
    using std::chrono::microseconds;
    using std::chrono::seconds;
    using std::chrono::duration_cast;
    
    std::time_t t = clock::to_time_t(tp);
    struct tm * ptm;
    ptm = localtime(&t);

    auto duration = tp.time_since_epoch();
    auto secs = duration_cast<seconds>(duration);
    auto micros = duration_cast<microseconds>(duration - secs);

    std::stringstream os;
    os << std::setfill('0') << std::setw(2) << ptm->tm_hour << ":"
       << std::setfill('0') << std::setw(2) << ptm->tm_min << ":"
       << std::setfill('0') << std::setw(2) << ptm->tm_sec << "."
       << std::setw(6) << micros.count();
    return os.str();
}

std::string str_date(const timestamp& tp)
{
    using std::chrono::microseconds;
    using std::chrono::seconds;
    using std::chrono::duration_cast;
    
    std::time_t t = clock::to_time_t(tp);
    struct tm * ptm;
    ptm = localtime(&t);

    auto duration = tp.time_since_epoch();
    auto secs = duration_cast<seconds>(duration);
    auto micros = duration_cast<microseconds>(duration - secs);

    std::stringstream os;
    os << std::setfill('0') << std::setw(4) << ptm->tm_year + 1900
       << std::setfill('0') << std::setw(2) << ptm->tm_mon + 1
       << std::setfill('0') << std::setw(2) << ptm->tm_mday ;
    return os.str();
}
std::string str_date2(const timestamp& tp)
{
    using std::chrono::microseconds;
    using std::chrono::seconds;
    using std::chrono::duration_cast;
    
    std::time_t t = clock::to_time_t(tp);
    struct tm * ptm;
    ptm = localtime(&t);

    auto duration = tp.time_since_epoch();
    auto secs = duration_cast<seconds>(duration);
    auto micros = duration_cast<microseconds>(duration - secs);

    std::stringstream os;
    os << std::setfill('0') << std::setw(4) << ptm->tm_year + 1900 << "-"
       << std::setfill('0') << std::setw(2) << ptm->tm_mon + 1 << "-"
       << std::setfill('0') << std::setw(2) << ptm->tm_mday ;
    return os.str();
}

timeval  to_timeval(const timestamp & tp)
{
    using std::chrono::microseconds;
    using std::chrono::seconds;
    using std::chrono::duration_cast;
    struct timeval tv;
    auto duration = tp.time_since_epoch();
    auto secs = duration_cast<seconds>(duration);
    auto micros = duration_cast<microseconds>(duration - secs);
    tv.tv_sec = secs.count();
    tv.tv_usec = micros.count();
    return tv; 
}

timestamp from_timeval(const timeval & tv)
{
    using std::chrono::microseconds;
    using std::chrono::seconds;
    return timestamp(seconds(tv.tv_sec) + microseconds(tv.tv_usec));
}

timespec to_timespec(const timestamp &tp)
{  
    using std::chrono::nanoseconds;
    using std::chrono::seconds;
    using std::chrono::duration_cast;
    struct timespec ts;
    auto duration = tp.time_since_epoch();
    auto secs = duration_cast<seconds>(duration);
    auto nanos = duration_cast<nanoseconds>(duration - secs);
    ts.tv_sec = secs.count();
    ts.tv_nsec = nanos.count();
    return ts; 
}

timestamp from_timespec(const timespec &ts)
{
    using std::chrono::nanoseconds;
    using std::chrono::seconds;
    return timestamp(seconds(ts.tv_sec) + nanoseconds(ts.tv_nsec));
}

timespec to_timespec(const elements::nanoseconds &nano)
{
    using std::chrono::nanoseconds;
    using std::chrono::seconds;
    using std::chrono::duration_cast;
    struct timespec ts;
    auto sec = std::chrono::duration_cast<seconds>( nano );
    ts.tv_sec = sec.count(); 
    ts.tv_nsec = std::chrono::duration_cast<nanoseconds>( nano - sec ).count();
    return ts;
}

tm to_tm(const timestamp& tp)
{
    tm _tm;
    return _tm;
}

timestamp from_tm(const tm& t)
{
    timestamp ts;
    return ts;
}

time_t to_time_t(const timestamp& tp)
{
    //todo
    time_t tt;
    return tt;
}

timestamp from_time_t(const time_t& t)
{
    //todo
    timestamp tp;
    return tp;
}

date get_date(const timestamp& tp)
{
    //todo
    date dt;
    return dt;
}

seconds seconds_since_midnigth(const timestamp& tp)
{
    //todo
    seconds s(0);
    return s;
}

uint64_t to_epoch_nanos( const timestamp & tp)
{
    return std::chrono::duration_cast<nanoseconds>(tp.time_since_epoch()).count();
}

timestamp from_epoch_nanos( uint64_t t)
{
    auto epoch = std::chrono::duration_cast<clock::duration>(nanoseconds(t) );
    return timestamp(std::chrono::duration_cast<clock::duration>(nanoseconds(t))); 
}

timestamp from_fix_time(char const * buf)
{
    tm tt;
    int micro = 0;
    char const * b = buf;

    tt.tm_year =   (*b++ - 48) * 1000;
    tt.tm_year +=  (*b++ - 48) * 100;
    tt.tm_year +=  (*b++ - 48) * 10;
    tt.tm_year +=  (*b++ - 48) ;
    tt.tm_mon  =   (*b++ - 48) * 10;
    tt.tm_mon  +=  (*b++ - 48) ;
    tt.tm_mday =   (*b++ - 48) * 10;
    tt.tm_mday +=  (*b++ - 48) ;
    tt.tm_hour =   (*b++ - 48) * 10 ;
    tt.tm_hour +=  (*b++ - 48) ;
    tt.tm_min  =   (*b++ - 48) * 10;
    tt.tm_min  +=  (*b++ - 48) ;
    tt.tm_sec  =   (*b++ - 48) * 10 ;
    tt.tm_sec  +=  (*b++ - 48) ;
    micro      =   (*b++ - 48) * 100000 ;
    micro      +=  (*b++ - 48) * 10000 ;
    micro      +=  (*b++ - 48) * 1000 ;
    //micro      +=  (*b++ - 48) * 100 ;
    //micro      +=  (*b++ - 48) * 10 ;
    //micro      +=  (*b++ - 48) ;

    tt.tm_isdst = -1;
    tt.tm_year -= 1900;
    tt.tm_mon -= 1;
    std::time_t a = timegm(&tt);
    timestamp t = std::chrono::system_clock::from_time_t(a);
    t += microseconds(micro);
    return t;
}

timestamp make_time(std::string date, std::string time)
{
    std::tm tm;

    tm.tm_isdst = -1;

    std::string frac = "";
    size_t pos = time.find(".");
    if(pos != std::string::npos)
    {
        frac = time.substr(pos + 1);
        time = time.substr(0, pos);
        assert((frac.length() == 3) || (frac.length() == 6) || frac.length() == 9);
    }
    
    std::string dt = date + " " + time;
    assert(strptime( dt.c_str(), "%Y%m%d %H:%M:%S", &tm) != NULL);
    timestamp t = std::chrono::system_clock::from_time_t(std::mktime(&tm));

    if(frac.length() == 0)
        return t;
    else if(frac.length() == 3)
        return t + milliseconds(atoi(frac.c_str()));
    else if(frac.length() == 6)
        return t + microseconds(atoi(frac.c_str()));
    else
        return t + nanoseconds(atoi(frac.c_str()));
}

}
