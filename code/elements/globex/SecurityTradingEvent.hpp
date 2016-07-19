/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_SECURITYTRADINGEVENT_HPP_
#define _MKTDATA_SECURITYTRADINGEVENT_HPP_

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

class SecurityTradingEvent
{
public:

    enum Value 
    {
        NoEvent = (sbe_uint8_t)0,
        NoCancel = (sbe_uint8_t)1,
        ResetStatistics = (sbe_uint8_t)4,
        ImpliedMatchingON = (sbe_uint8_t)5,
        ImpliedMatchingOFF = (sbe_uint8_t)6,
        NULL_VALUE = (sbe_uint8_t)255
    };

    static SecurityTradingEvent::Value get(const sbe_uint8_t value)
    {
        switch (value)
        {
            case 0: return NoEvent;
            case 1: return NoCancel;
            case 4: return ResetStatistics;
            case 5: return ImpliedMatchingON;
            case 6: return ImpliedMatchingOFF;
            case 255: return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum SecurityTradingEvent [E103]");
    }
};
}
#endif
