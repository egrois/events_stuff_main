/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_SETTLPRICETYPE_HPP_
#define _MKTDATA_SETTLPRICETYPE_HPP_

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

class SettlPriceType
{
private:
    char *buffer_;
    int offset_;
    int actingVersion_;

    inline void reset(char *buffer, const int offset, const int bufferLength, const int actingVersion)
    {
        if (SBE_BOUNDS_CHECK_EXPECT((offset > (bufferLength - 1)), false))
        {
            throw std::runtime_error("buffer too short for flyweight [E107]");
        }
        buffer_ = buffer;
        offset_ = offset;
        actingVersion_ = actingVersion;
    }

public:
    SettlPriceType(void) : buffer_(NULL), offset_(0) {}

    SettlPriceType(char *buffer, const int bufferLength, const int actingVersion)
    {
        reset(buffer, 0, bufferLength, actingVersion);
    }

    SettlPriceType(const SettlPriceType& codec) :
        buffer_(codec.buffer_), offset_(codec.offset_), actingVersion_(codec.actingVersion_) {}

#if __cplusplus >= 201103L
    SettlPriceType(SettlPriceType&& codec) :
        buffer_(codec.buffer_), offset_(codec.offset_), actingVersion_(codec.actingVersion_) {}

    SettlPriceType& operator=(SettlPriceType&& codec)
    {
        buffer_ = codec.buffer_;
        offset_ = codec.offset_;
        actingVersion_ = codec.actingVersion_;
        return *this;
    }

#endif

    SettlPriceType& operator=(const SettlPriceType& codec)
    {
        buffer_ = codec.buffer_;
        offset_ = codec.offset_;
        actingVersion_ = codec.actingVersion_;
        return *this;
    }

    SettlPriceType &wrap(char *buffer, const int offset, const int actingVersion, const int bufferLength)
    {
        reset(buffer, offset, bufferLength, actingVersion);
        return *this;
    }

    static const int size(void)
    {
        return 1;
    }


    SettlPriceType &clear(void)
    {
        *((sbe_uint8_t *)(buffer_ + offset_)) = 0;
        return *this;
    }


    bool Final(void) const
    {
        return ((*((sbe_uint8_t *)(buffer_ + offset_))) & (0x1L << 0)) ? true : false;
    }

    SettlPriceType &Final(const bool value)
    {
        sbe_uint8_t bits = (*((sbe_uint8_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 0)) : (bits & ~(0x1L << 0));
        *((sbe_uint8_t *)(buffer_ + offset_)) = (bits);
        return *this;
    }

    bool Actual(void) const
    {
        return ((*((sbe_uint8_t *)(buffer_ + offset_))) & (0x1L << 1)) ? true : false;
    }

    SettlPriceType &Actual(const bool value)
    {
        sbe_uint8_t bits = (*((sbe_uint8_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 1)) : (bits & ~(0x1L << 1));
        *((sbe_uint8_t *)(buffer_ + offset_)) = (bits);
        return *this;
    }

    bool Rounded(void) const
    {
        return ((*((sbe_uint8_t *)(buffer_ + offset_))) & (0x1L << 2)) ? true : false;
    }

    SettlPriceType &Rounded(const bool value)
    {
        sbe_uint8_t bits = (*((sbe_uint8_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 2)) : (bits & ~(0x1L << 2));
        *((sbe_uint8_t *)(buffer_ + offset_)) = (bits);
        return *this;
    }

    bool Intraday(void) const
    {
        return ((*((sbe_uint8_t *)(buffer_ + offset_))) & (0x1L << 3)) ? true : false;
    }

    SettlPriceType &Intraday(const bool value)
    {
        sbe_uint8_t bits = (*((sbe_uint8_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 3)) : (bits & ~(0x1L << 3));
        *((sbe_uint8_t *)(buffer_ + offset_)) = (bits);
        return *this;
    }

    bool ReservedBits(void) const
    {
        return ((*((sbe_uint8_t *)(buffer_ + offset_))) & (0x1L << 4)) ? true : false;
    }

    SettlPriceType &ReservedBits(const bool value)
    {
        sbe_uint8_t bits = (*((sbe_uint8_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 4)) : (bits & ~(0x1L << 4));
        *((sbe_uint8_t *)(buffer_ + offset_)) = (bits);
        return *this;
    }

    bool NullValue(void) const
    {
        return ((*((sbe_uint8_t *)(buffer_ + offset_))) & (0x1L << 7)) ? true : false;
    }

    SettlPriceType &NullValue(const bool value)
    {
        sbe_uint8_t bits = (*((sbe_uint8_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 7)) : (bits & ~(0x1L << 7));
        *((sbe_uint8_t *)(buffer_ + offset_)) = (bits);
        return *this;
    }
};
}
#endif
