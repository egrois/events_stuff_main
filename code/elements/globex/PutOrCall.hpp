/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_PUTORCALL_HPP_
#define _MKTDATA_PUTORCALL_HPP_

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

class PutOrCall
{
public:

    enum Value 
    {
        Put = (sbe_uint8_t)0,
        Call = (sbe_uint8_t)1,
        NULL_VALUE = (sbe_uint8_t)255
    };

    static PutOrCall::Value get(const sbe_uint8_t value)
    {
        switch (value)
        {
            case 0: return Put;
            case 1: return Call;
            case 255: return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum PutOrCall [E103]");
    }
};
}
#endif
