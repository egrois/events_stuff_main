/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_GROUPSIZE8BYTE_HPP_
#define _MKTDATA_GROUPSIZE8BYTE_HPP_

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

class GroupSize8Byte
{
private:
    char *buffer_;
    int offset_;
    int actingVersion_;

    inline void reset(char *buffer, const int offset, const int bufferLength, const int actingVersion)
    {
        if (SBE_BOUNDS_CHECK_EXPECT((offset > (bufferLength - 8)), false))
        {
            throw std::runtime_error("buffer too short for flyweight [E107]");
        }
        buffer_ = buffer;
        offset_ = offset;
        actingVersion_ = actingVersion;
    }

public:
    GroupSize8Byte(void) : buffer_(NULL), offset_(0) {}

    GroupSize8Byte(char *buffer, const int bufferLength, const int actingVersion)
    {
        reset(buffer, 0, bufferLength, actingVersion);
    }

    GroupSize8Byte(const GroupSize8Byte& codec) :
        buffer_(codec.buffer_), offset_(codec.offset_), actingVersion_(codec.actingVersion_) {}

#if __cplusplus >= 201103L
    GroupSize8Byte(GroupSize8Byte&& codec) :
        buffer_(codec.buffer_), offset_(codec.offset_), actingVersion_(codec.actingVersion_) {}

    GroupSize8Byte& operator=(GroupSize8Byte&& codec)
    {
        buffer_ = codec.buffer_;
        offset_ = codec.offset_;
        actingVersion_ = codec.actingVersion_;
        return *this;
    }

#endif

    GroupSize8Byte& operator=(const GroupSize8Byte& codec)
    {
        buffer_ = codec.buffer_;
        offset_ = codec.offset_;
        actingVersion_ = codec.actingVersion_;
        return *this;
    }

    GroupSize8Byte &wrap(char *buffer, const int offset, const int actingVersion, const int bufferLength)
    {
        reset(buffer, offset, bufferLength, actingVersion);
        return *this;
    }

    static const int size(void)
    {
        return 8;
    }


    static const sbe_uint16_t blockLengthNullValue()
    {
        return SBE_NULLVALUE_UINT16;
    }

    static const sbe_uint16_t blockLengthMinValue()
    {
        return (sbe_uint16_t)0;
    }

    static const sbe_uint16_t blockLengthMaxValue()
    {
        return (sbe_uint16_t)65534;
    }

    sbe_uint16_t blockLength(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_16(*((sbe_uint16_t *)(buffer_ + offset_ + 0)));
    }

    GroupSize8Byte &blockLength(const sbe_uint16_t value)
    {
        *((sbe_uint16_t *)(buffer_ + offset_ + 0)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
        return *this;
    }

    static const sbe_uint8_t numInGroupNullValue()
    {
        return SBE_NULLVALUE_UINT8;
    }

    static const sbe_uint8_t numInGroupMinValue()
    {
        return (sbe_uint8_t)0;
    }

    static const sbe_uint8_t numInGroupMaxValue()
    {
        return (sbe_uint8_t)254;
    }

    sbe_uint8_t numInGroup(void) const
    {
        return (*((sbe_uint8_t *)(buffer_ + offset_ + 7)));
    }

    GroupSize8Byte &numInGroup(const sbe_uint8_t value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 7)) = (value);
        return *this;
    }
};
}
#endif
