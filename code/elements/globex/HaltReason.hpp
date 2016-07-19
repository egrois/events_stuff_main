/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_HALTREASON_HPP_
#define _MKTDATA_HALTREASON_HPP_

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

class HaltReason
{
public:

    enum Value 
    {
        GroupSchedule = (sbe_uint8_t)0,
        SurveillanceIntervention = (sbe_uint8_t)1,
        MarketEvent = (sbe_uint8_t)2,
        InstrumentActivation = (sbe_uint8_t)3,
        InstrumentExpiration = (sbe_uint8_t)4,
        Unknown = (sbe_uint8_t)5,
        RecoveryInProcess = (sbe_uint8_t)6,
        NULL_VALUE = (sbe_uint8_t)255
    };

    static HaltReason::Value get(const sbe_uint8_t value)
    {
        switch (value)
        {
            case 0: return GroupSchedule;
            case 1: return SurveillanceIntervention;
            case 2: return MarketEvent;
            case 3: return InstrumentActivation;
            case 4: return InstrumentExpiration;
            case 5: return Unknown;
            case 6: return RecoveryInProcess;
            case 255: return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum HaltReason [E103]");
    }
};
}
#endif
