/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_SECURITYTRADINGSTATUS_HPP_
#define _MKTDATA_SECURITYTRADINGSTATUS_HPP_

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

class SecurityTradingStatus
{
public:

    enum Value 
    {
        TradingHalt = (sbe_uint8_t)2,
        Close = (sbe_uint8_t)4,
        NewPriceIndication = (sbe_uint8_t)15,
        ReadyToTrade = (sbe_uint8_t)17,
        NotAvailableForTrading = (sbe_uint8_t)18,
        UnknownorInvalid = (sbe_uint8_t)20,
        PreOpen = (sbe_uint8_t)21,
        PreCross = (sbe_uint8_t)24,
        Cross = (sbe_uint8_t)25,
        PostClose = (sbe_uint8_t)26,
        NoChange = (sbe_uint8_t)103,
        NULL_VALUE = (sbe_uint8_t)255
    };

    static SecurityTradingStatus::Value get(const sbe_uint8_t value)
    {
        switch (value)
        {
            case 2: return TradingHalt;
            case 4: return Close;
            case 15: return NewPriceIndication;
            case 17: return ReadyToTrade;
            case 18: return NotAvailableForTrading;
            case 20: return UnknownorInvalid;
            case 21: return PreOpen;
            case 24: return PreCross;
            case 25: return Cross;
            case 26: return PostClose;
            case 103: return NoChange;
            case 255: return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum SecurityTradingStatus [E103]");
    }
};
}
#endif
