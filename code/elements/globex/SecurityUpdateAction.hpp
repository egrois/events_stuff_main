/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_SECURITYUPDATEACTION_HPP_
#define _MKTDATA_SECURITYUPDATEACTION_HPP_

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

class SecurityUpdateAction
{
public:

    enum Value 
    {
        Add = (sbe_char_t)65,
        Delete = (sbe_char_t)68,
        Modify = (sbe_char_t)77,
        NULL_VALUE = (sbe_char_t)0
    };

    static SecurityUpdateAction::Value get(const sbe_char_t value)
    {
        switch (value)
        {
            case 65: return Add;
            case 68: return Delete;
            case 77: return Modify;
            case 0: return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum SecurityUpdateAction [E103]");
    }
};
}
#endif
