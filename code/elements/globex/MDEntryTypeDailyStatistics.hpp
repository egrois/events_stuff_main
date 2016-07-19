/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_MDENTRYTYPEDAILYSTATISTICS_HPP_
#define _MKTDATA_MDENTRYTYPEDAILYSTATISTICS_HPP_

#if defined(SBE_HAVE_CMATH)
/* cmath needed for std::numeric_limits<double>::quiet_NaN() */
#  include <cmath>
#  define SBE_FLOAT_NAN std::numeric_limits<float>::quiet_NaN()
#  define SBE_DOUBLE_NAN std::numeric_limits<double>::quiet_NaN()
#else
/* math.h needed for NAN */
#  include <math.h>
#  define SBE_FLOAT_NAN NAN
#  define SBE_DOUBLE_NAN NAN
#endif

#include <sbe/sbe.hpp>

using namespace sbe;

namespace globex {

class MDEntryTypeDailyStatistics
{
public:

    enum Value 
    {
        SettlementPrice = (sbe_char_t)54,
        ClearedVolume = (sbe_char_t)66,
        OpenInterest = (sbe_char_t)67,
        FixingPrice = (sbe_char_t)87,
        NULL_VALUE = (sbe_char_t)0
    };

    static MDEntryTypeDailyStatistics::Value get(const sbe_char_t value)
    {
        switch (value)
        {
            case 54: return SettlementPrice;
            case 66: return ClearedVolume;
            case 67: return OpenInterest;
            case 87: return FixingPrice;
            case 0: return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum MDEntryTypeDailyStatistics [E103]");
    }
};
}
#endif
