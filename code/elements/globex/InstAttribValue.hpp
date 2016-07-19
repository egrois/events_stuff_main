/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_INSTATTRIBVALUE_HPP_
#define _MKTDATA_INSTATTRIBVALUE_HPP_

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

class InstAttribValue
{
private:
    char *buffer_;
    int offset_;
    int actingVersion_;

    inline void reset(char *buffer, const int offset, const int bufferLength, const int actingVersion)
    {
        if (SBE_BOUNDS_CHECK_EXPECT((offset > (bufferLength - 4)), false))
        {
            throw std::runtime_error("buffer too short for flyweight [E107]");
        }
        buffer_ = buffer;
        offset_ = offset;
        actingVersion_ = actingVersion;
    }

public:
    InstAttribValue(void) : buffer_(NULL), offset_(0) {}

    InstAttribValue(char *buffer, const int bufferLength, const int actingVersion)
    {
        reset(buffer, 0, bufferLength, actingVersion);
    }

    InstAttribValue(const InstAttribValue& codec) :
        buffer_(codec.buffer_), offset_(codec.offset_), actingVersion_(codec.actingVersion_) {}

#if __cplusplus >= 201103L
    InstAttribValue(InstAttribValue&& codec) :
        buffer_(codec.buffer_), offset_(codec.offset_), actingVersion_(codec.actingVersion_) {}

    InstAttribValue& operator=(InstAttribValue&& codec)
    {
        buffer_ = codec.buffer_;
        offset_ = codec.offset_;
        actingVersion_ = codec.actingVersion_;
        return *this;
    }

#endif

    InstAttribValue& operator=(const InstAttribValue& codec)
    {
        buffer_ = codec.buffer_;
        offset_ = codec.offset_;
        actingVersion_ = codec.actingVersion_;
        return *this;
    }

    InstAttribValue &wrap(char *buffer, const int offset, const int actingVersion, const int bufferLength)
    {
        reset(buffer, offset, bufferLength, actingVersion);
        return *this;
    }

    static const int size(void)
    {
        return 4;
    }


    InstAttribValue &clear(void)
    {
        *((sbe_uint32_t *)(buffer_ + offset_)) = 0;
        return *this;
    }


    bool ElectronicMatchEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 0)) ? true : false;
    }

    InstAttribValue &ElectronicMatchEligible(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 0)) : (bits & ~(0x1L << 0));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool OrderCrossEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 1)) ? true : false;
    }

    InstAttribValue &OrderCrossEligible(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 1)) : (bits & ~(0x1L << 1));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool BlockTradeEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 2)) ? true : false;
    }

    InstAttribValue &BlockTradeEligible(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 2)) : (bits & ~(0x1L << 2));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool EFPEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 3)) ? true : false;
    }

    InstAttribValue &EFPEligible(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 3)) : (bits & ~(0x1L << 3));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool EBFEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 4)) ? true : false;
    }

    InstAttribValue &EBFEligible(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 4)) : (bits & ~(0x1L << 4));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool EFSEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 5)) ? true : false;
    }

    InstAttribValue &EFSEligible(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 5)) : (bits & ~(0x1L << 5));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool EFREligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 6)) ? true : false;
    }

    InstAttribValue &EFREligible(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 6)) : (bits & ~(0x1L << 6));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool OTCEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 7)) ? true : false;
    }

    InstAttribValue &OTCEligible(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 7)) : (bits & ~(0x1L << 7));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool iLinkIndicativeMassQuotingEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 8)) ? true : false;
    }

    InstAttribValue &iLinkIndicativeMassQuotingEligible(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 8)) : (bits & ~(0x1L << 8));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool NegativeStrikeEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 9)) ? true : false;
    }

    InstAttribValue &NegativeStrikeEligible(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 9)) : (bits & ~(0x1L << 9));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool NegativePriceOutrightEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 10)) ? true : false;
    }

    InstAttribValue &NegativePriceOutrightEligible(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 10)) : (bits & ~(0x1L << 10));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool IsFractional(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 11)) ? true : false;
    }

    InstAttribValue &IsFractional(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 11)) : (bits & ~(0x1L << 11));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool VolatilityQuotedOption(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 12)) ? true : false;
    }

    InstAttribValue &VolatilityQuotedOption(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 12)) : (bits & ~(0x1L << 12));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool RFQCrossEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 13)) ? true : false;
    }

    InstAttribValue &RFQCrossEligible(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 13)) : (bits & ~(0x1L << 13));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool ZeroPriceOutrightEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 14)) ? true : false;
    }

    InstAttribValue &ZeroPriceOutrightEligible(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 14)) : (bits & ~(0x1L << 14));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool DecayingProductEligibility(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 15)) ? true : false;
    }

    InstAttribValue &DecayingProductEligibility(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 15)) : (bits & ~(0x1L << 15));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool VariableProductEligibility(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 16)) ? true : false;
    }

    InstAttribValue &VariableProductEligibility(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 16)) : (bits & ~(0x1L << 16));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool DailyProductEligibility(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 17)) ? true : false;
    }

    InstAttribValue &DailyProductEligibility(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 17)) : (bits & ~(0x1L << 17));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool GTOrdersEligibility(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 18)) ? true : false;
    }

    InstAttribValue &GTOrdersEligibility(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 18)) : (bits & ~(0x1L << 18));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool ImpliedMatchingEligibility(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_))) & (0x1L << 19)) ? true : false;
    }

    InstAttribValue &ImpliedMatchingEligibility(const bool value)
    {
        sbe_uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 19)) : (bits & ~(0x1L << 19));
        *((sbe_uint32_t *)(buffer_ + offset_)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }
};
}
#endif
