/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_DECIMALQTY_HPP_
#define _MKTDATA_DECIMALQTY_HPP_

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

class DecimalQty
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
    DecimalQty(void) : buffer_(NULL), offset_(0) {}

    DecimalQty(char *buffer, const int bufferLength, const int actingVersion)
    {
        reset(buffer, 0, bufferLength, actingVersion);
    }

    DecimalQty(const DecimalQty& codec) :
        buffer_(codec.buffer_), offset_(codec.offset_), actingVersion_(codec.actingVersion_) {}

#if __cplusplus >= 201103L
    DecimalQty(DecimalQty&& codec) :
        buffer_(codec.buffer_), offset_(codec.offset_), actingVersion_(codec.actingVersion_) {}

    DecimalQty& operator=(DecimalQty&& codec)
    {
        buffer_ = codec.buffer_;
        offset_ = codec.offset_;
        actingVersion_ = codec.actingVersion_;
        return *this;
    }

#endif

    DecimalQty& operator=(const DecimalQty& codec)
    {
        buffer_ = codec.buffer_;
        offset_ = codec.offset_;
        actingVersion_ = codec.actingVersion_;
        return *this;
    }

    DecimalQty &wrap(char *buffer, const int offset, const int actingVersion, const int bufferLength)
    {
        reset(buffer, offset, bufferLength, actingVersion);
        return *this;
    }

    static const int size(void)
    {
        return 4;
    }


    static const sbe_int32_t mantissaNullValue()
    {
        return 2147483647;
    }

    static const sbe_int32_t mantissaMinValue()
    {
        return -2147483647;
    }

    static const sbe_int32_t mantissaMaxValue()
    {
        return 2147483647;
    }

    sbe_int32_t mantissa(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_int32_t *)(buffer_ + offset_ + 0)));
    }

    DecimalQty &mantissa(const sbe_int32_t value)
    {
        *((sbe_int32_t *)(buffer_ + offset_ + 0)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const sbe_int8_t exponentNullValue()
    {
        return SBE_NULLVALUE_INT8;
    }

    static const sbe_int8_t exponentMinValue()
    {
        return (sbe_int8_t)-127;
    }

    static const sbe_int8_t exponentMaxValue()
    {
        return (sbe_int8_t)127;
    }

    sbe_int8_t exponent(void) const
    {
        return (sbe_int8_t)-4;
    }
};
}
#endif
