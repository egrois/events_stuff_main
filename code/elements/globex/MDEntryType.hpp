/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_MDENTRYTYPE_HPP_
#define _MKTDATA_MDENTRYTYPE_HPP_

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

class MDEntryType
{
public:

    enum Value 
    {
        Bid = (sbe_char_t)48,
        Offer = (sbe_char_t)49,
        Trade = (sbe_char_t)50,
        OpeningPrice = (sbe_char_t)52,
        SettlementPrice = (sbe_char_t)54,
        TradingSessionHighPrice = (sbe_char_t)55,
        TradingSessionLowPrice = (sbe_char_t)56,
        TradeVolume = (sbe_char_t)66,
        OpenInterest = (sbe_char_t)67,
        ImpliedBid = (sbe_char_t)69,
        ImpliedOffer = (sbe_char_t)70,
        EmptyBook = (sbe_char_t)74,
        SessionHighBid = (sbe_char_t)78,
        SessionLowOffer = (sbe_char_t)79,
        FixingPrice = (sbe_char_t)87,
        ElectronicVolume = (sbe_char_t)101,
        ThresholdLimitsandPriceBandVariation = (sbe_char_t)103,
        NULL_VALUE = (sbe_char_t)0
    };

    static MDEntryType::Value get(const sbe_char_t value)
    {
        switch (value)
        {
            case 48: return Bid;
            case 49: return Offer;
            case 50: return Trade;
            case 52: return OpeningPrice;
            case 54: return SettlementPrice;
            case 55: return TradingSessionHighPrice;
            case 56: return TradingSessionLowPrice;
            case 66: return TradeVolume;
            case 67: return OpenInterest;
            case 69: return ImpliedBid;
            case 70: return ImpliedOffer;
            case 74: return EmptyBook;
            case 78: return SessionHighBid;
            case 79: return SessionLowOffer;
            case 87: return FixingPrice;
            case 101: return ElectronicVolume;
            case 103: return ThresholdLimitsandPriceBandVariation;
            case 0: return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum MDEntryType [E103]");
    }
};
}
#endif
