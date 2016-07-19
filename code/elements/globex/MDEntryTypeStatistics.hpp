/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_MDENTRYTYPESTATISTICS_HPP_
#define _MKTDATA_MDENTRYTYPESTATISTICS_HPP_

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

class MDEntryTypeStatistics
{
public:

    enum Value 
    {
        OpenPrice = (sbe_char_t)52,
        HighTrade = (sbe_char_t)55,
        LowTrade = (sbe_char_t)56,
        HighestBid = (sbe_char_t)78,
        LowestOffer = (sbe_char_t)79,
        NULL_VALUE = (sbe_char_t)0
    };

    static MDEntryTypeStatistics::Value get(const sbe_char_t value)
    {
        switch (value)
        {
            case 52: return OpenPrice;
            case 55: return HighTrade;
            case 56: return LowTrade;
            case 78: return HighestBid;
            case 79: return LowestOffer;
            case 0: return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum MDEntryTypeStatistics [E103]");
    }
};
}
#endif
