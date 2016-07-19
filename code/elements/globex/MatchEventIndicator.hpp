/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_MATCHEVENTINDICATOR_HPP_
#define _MKTDATA_MATCHEVENTINDICATOR_HPP_

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

class MatchEventIndicator
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
    MatchEventIndicator(void) : buffer_(NULL), offset_(0) {}

    MatchEventIndicator(char *buffer, const int bufferLength, const int actingVersion)
    {
        reset(buffer, 0, bufferLength, actingVersion);
    }

    MatchEventIndicator(const MatchEventIndicator& codec) :
        buffer_(codec.buffer_), offset_(codec.offset_), actingVersion_(codec.actingVersion_) {}

#if __cplusplus >= 201103L
    MatchEventIndicator(MatchEventIndicator&& codec) :
        buffer_(codec.buffer_), offset_(codec.offset_), actingVersion_(codec.actingVersion_) {}

    MatchEventIndicator& operator=(MatchEventIndicator&& codec)
    {
        buffer_ = codec.buffer_;
        offset_ = codec.offset_;
        actingVersion_ = codec.actingVersion_;
        return *this;
    }

#endif

    MatchEventIndicator& operator=(const MatchEventIndicator& codec)
    {
        buffer_ = codec.buffer_;
        offset_ = codec.offset_;
        actingVersion_ = codec.actingVersion_;
        return *this;
    }

    MatchEventIndicator &wrap(char *buffer, const int offset, const int actingVersion, const int bufferLength)
    {
        reset(buffer, offset, bufferLength, actingVersion);
        return *this;
    }

    static const int size(void)
    {
        return 1;
    }


    MatchEventIndicator &clear(void)
    {
        *((sbe_uint8_t *)(buffer_ + offset_)) = 0;
        return *this;
    }


    bool LastTradeMsg(void) const
    {
        return ((*((sbe_uint8_t *)(buffer_ + offset_))) & (0x1L << 0)) ? true : false;
    }

    MatchEventIndicator &LastTradeMsg(const bool value)
    {
        sbe_uint8_t bits = (*((sbe_uint8_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 0)) : (bits & ~(0x1L << 0));
        *((sbe_uint8_t *)(buffer_ + offset_)) = (bits);
        return *this;
    }

    bool LastVolumeMsg(void) const
    {
        return ((*((sbe_uint8_t *)(buffer_ + offset_))) & (0x1L << 1)) ? true : false;
    }

    MatchEventIndicator &LastVolumeMsg(const bool value)
    {
        sbe_uint8_t bits = (*((sbe_uint8_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 1)) : (bits & ~(0x1L << 1));
        *((sbe_uint8_t *)(buffer_ + offset_)) = (bits);
        return *this;
    }

    bool LastQuoteMsg(void) const
    {
        return ((*((sbe_uint8_t *)(buffer_ + offset_))) & (0x1L << 2)) ? true : false;
    }

    MatchEventIndicator &LastQuoteMsg(const bool value)
    {
        sbe_uint8_t bits = (*((sbe_uint8_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 2)) : (bits & ~(0x1L << 2));
        *((sbe_uint8_t *)(buffer_ + offset_)) = (bits);
        return *this;
    }

    bool LastStatsMsg(void) const
    {
        return ((*((sbe_uint8_t *)(buffer_ + offset_))) & (0x1L << 3)) ? true : false;
    }

    MatchEventIndicator &LastStatsMsg(const bool value)
    {
        sbe_uint8_t bits = (*((sbe_uint8_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 3)) : (bits & ~(0x1L << 3));
        *((sbe_uint8_t *)(buffer_ + offset_)) = (bits);
        return *this;
    }

    bool LastImpliedMsg(void) const
    {
        return ((*((sbe_uint8_t *)(buffer_ + offset_))) & (0x1L << 4)) ? true : false;
    }

    MatchEventIndicator &LastImpliedMsg(const bool value)
    {
        sbe_uint8_t bits = (*((sbe_uint8_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 4)) : (bits & ~(0x1L << 4));
        *((sbe_uint8_t *)(buffer_ + offset_)) = (bits);
        return *this;
    }

    bool RecoveryMsg(void) const
    {
        return ((*((sbe_uint8_t *)(buffer_ + offset_))) & (0x1L << 5)) ? true : false;
    }

    MatchEventIndicator &RecoveryMsg(const bool value)
    {
        sbe_uint8_t bits = (*((sbe_uint8_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 5)) : (bits & ~(0x1L << 5));
        *((sbe_uint8_t *)(buffer_ + offset_)) = (bits);
        return *this;
    }

    bool Reserved(void) const
    {
        return ((*((sbe_uint8_t *)(buffer_ + offset_))) & (0x1L << 6)) ? true : false;
    }

    MatchEventIndicator &Reserved(const bool value)
    {
        sbe_uint8_t bits = (*((sbe_uint8_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 6)) : (bits & ~(0x1L << 6));
        *((sbe_uint8_t *)(buffer_ + offset_)) = (bits);
        return *this;
    }

    bool EndOfEvent(void) const
    {
        return ((*((sbe_uint8_t *)(buffer_ + offset_))) & (0x1L << 7)) ? true : false;
    }

    MatchEventIndicator &EndOfEvent(const bool value)
    {
        sbe_uint8_t bits = (*((sbe_uint8_t *)(buffer_ + offset_)));
        bits = value ? (bits | (0x1L << 7)) : (bits & ~(0x1L << 7));
        *((sbe_uint8_t *)(buffer_ + offset_)) = (bits);
        return *this;
    }
};
}
#endif
