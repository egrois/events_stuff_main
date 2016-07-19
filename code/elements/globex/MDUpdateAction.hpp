/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_MDUPDATEACTION_HPP_
#define _MKTDATA_MDUPDATEACTION_HPP_

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

class MDUpdateAction
{
public:

    enum Value 
    {
        New = (sbe_uint8_t)0,
        Change = (sbe_uint8_t)1,
        Delete = (sbe_uint8_t)2,
        DeleteThru = (sbe_uint8_t)3,
        DeleteFrom = (sbe_uint8_t)4,
        Overlay = (sbe_uint8_t)5,
        NULL_VALUE = (sbe_uint8_t)255
    };

    static MDUpdateAction::Value get(const sbe_uint8_t value)
    {
        switch (value)
        {
            case 0: return New;
            case 1: return Change;
            case 2: return Delete;
            case 3: return DeleteThru;
            case 4: return DeleteFrom;
            case 5: return Overlay;
            case 255: return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum MDUpdateAction [E103]");
    }
};
}
#endif
