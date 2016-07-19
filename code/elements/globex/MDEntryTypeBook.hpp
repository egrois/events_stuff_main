/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_MDENTRYTYPEBOOK_HPP_
#define _MKTDATA_MDENTRYTYPEBOOK_HPP_

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

class MDEntryTypeBook
{
public:

    enum Value 
    {
        Bid = (sbe_char_t)48,
        Offer = (sbe_char_t)49,
        ImpliedBid = (sbe_char_t)69,
        ImpliedOffer = (sbe_char_t)70,
        BookReset = (sbe_char_t)74,
        NULL_VALUE = (sbe_char_t)0
    };

    static MDEntryTypeBook::Value get(const sbe_char_t value)
    {
        switch (value)
        {
            case 48: return Bid;
            case 49: return Offer;
            case 69: return ImpliedBid;
            case 70: return ImpliedOffer;
            case 74: return BookReset;
            case 0: return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum MDEntryTypeBook [E103]");
    }
};
}
#endif
