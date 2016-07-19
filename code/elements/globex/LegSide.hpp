/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_LEGSIDE_HPP_
#define _MKTDATA_LEGSIDE_HPP_

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

class LegSide
{
public:

    enum Value 
    {
        BuySide = (sbe_uint8_t)1,
        SellSide = (sbe_uint8_t)2,
        NULL_VALUE = (sbe_uint8_t)255
    };

    static LegSide::Value get(const sbe_uint8_t value)
    {
        switch (value)
        {
            case 1: return BuySide;
            case 2: return SellSide;
            case 255: return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum LegSide [E103]");
    }
};
}
#endif
