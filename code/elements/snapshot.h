#ifndef ELEMENTS_SNAPSHOT_H
#define ELEMENTS_SNAPSHOT_H

#include <md.h>
#include <cmath>
#include <math.h>
#include <sstream>
#include <limits>

namespace elements
{

std::ostream & double_out(std::ostream & s, double a)
{
    s << std::setprecision(std::numeric_limits<double>::digits10 + 1) << a;
    return s;
}

struct snapshot
{
    double abs_vol      = 0; 
    double buy_vol      = 0;
    double sell_vol     = 0;
    double high         = NAN;
    double low          = NAN; 
    double open         = NAN;
    double close        = NAN;

    std::list<std::string> saved;

    void reset()
    {
        abs_vol      = 0; 
        buy_vol      = 0;
        sell_vol     = 0;
        high         = NAN;
        low          = NAN; 
        open         = close;
        close        = NAN;
    }

    void save_and_reset( 

        std::string const &symbol, 
        timestamp time, 
        std::string const & addon)

    {
        std::stringstream ss;

        ss << symbol << ",";
        ss << str_date2(time) << " " << str(time)  << ",";
        ss << abs_vol  << ",";
        ss << buy_vol  << ",";
        ss << sell_vol << ",";

        double_out(ss, high  * 1e-7 ) << ",";
        double_out(ss, low   * 1e-7 ) << ",";
        double_out(ss, open  * 1e-7 ) << ",";
        double_out(ss, close * 1e-7 ) << ",";

        ss << addon;

        saved.push_back(ss.str());

        reset();
    }

    void on_trade( cme::MDEntry const & r)
    {
        if(r.AggressorSide == cme::AggressorSideEnum::BUY)
        {
            buy_vol += r.MDEntrySize;
            abs_vol += r.MDEntrySize;
        }
        else if(r.AggressorSide == cme::AggressorSideEnum::SELL)
        {
            sell_vol += r.MDEntrySize;
            abs_vol += r.MDEntrySize;
        }
        else return;
        
        if(isnan(high) || (high  < r.MDEntryPx))
            high = r.MDEntryPx;

        if(isnan(low) || (low > r.MDEntryPx))
            low = r.MDEntryPx;

        close = r.MDEntryPx;
    }
};

}
#endif // ELEMENTS_SNAPSHOT_H
