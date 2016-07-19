/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_MATURITYMONTHYEAR_HPP_
#define _MKTDATA_MATURITYMONTHYEAR_HPP_

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

class MaturityMonthYear
{
private:
    char *buffer_;
    int offset_;
    int actingVersion_;

    inline void reset(char *buffer, const int offset, const int bufferLength, const int actingVersion)
    {
        if (SBE_BOUNDS_CHECK_EXPECT((offset > (bufferLength - 5)), false))
        {
            throw std::runtime_error("buffer too short for flyweight [E107]");
        }
        buffer_ = buffer;
        offset_ = offset;
        actingVersion_ = actingVersion;
    }

public:
    MaturityMonthYear(void) : buffer_(NULL), offset_(0) {}

    MaturityMonthYear(char *buffer, const int bufferLength, const int actingVersion)
    {
        reset(buffer, 0, bufferLength, actingVersion);
    }

    MaturityMonthYear(const MaturityMonthYear& codec) :
        buffer_(codec.buffer_), offset_(codec.offset_), actingVersion_(codec.actingVersion_) {}

#if __cplusplus >= 201103L
    MaturityMonthYear(MaturityMonthYear&& codec) :
        buffer_(codec.buffer_), offset_(codec.offset_), actingVersion_(codec.actingVersion_) {}

    MaturityMonthYear& operator=(MaturityMonthYear&& codec)
    {
        buffer_ = codec.buffer_;
        offset_ = codec.offset_;
        actingVersion_ = codec.actingVersion_;
        return *this;
    }

#endif

    MaturityMonthYear& operator=(const MaturityMonthYear& codec)
    {
        buffer_ = codec.buffer_;
        offset_ = codec.offset_;
        actingVersion_ = codec.actingVersion_;
        return *this;
    }

    MaturityMonthYear &wrap(char *buffer, const int offset, const int actingVersion, const int bufferLength)
    {
        reset(buffer, offset, bufferLength, actingVersion);
        return *this;
    }

    static const int size(void)
    {
        return 5;
    }


    static const sbe_uint16_t yearNullValue()
    {
        return (sbe_uint16_t)65535;
    }

    static const sbe_uint16_t yearMinValue()
    {
        return (sbe_uint16_t)0;
    }

    static const sbe_uint16_t yearMaxValue()
    {
        return (sbe_uint16_t)65534;
    }

    sbe_uint16_t year(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_16(*((sbe_uint16_t *)(buffer_ + offset_ + 0)));
    }

    MaturityMonthYear &year(const sbe_uint16_t value)
    {
        *((sbe_uint16_t *)(buffer_ + offset_ + 0)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
        return *this;
    }

    static const sbe_uint8_t monthNullValue()
    {
        return (sbe_uint8_t)255;
    }

    static const sbe_uint8_t monthMinValue()
    {
        return (sbe_uint8_t)0;
    }

    static const sbe_uint8_t monthMaxValue()
    {
        return (sbe_uint8_t)254;
    }

    sbe_uint8_t month(void) const
    {
        return (*((sbe_uint8_t *)(buffer_ + offset_ + 2)));
    }

    MaturityMonthYear &month(const sbe_uint8_t value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 2)) = (value);
        return *this;
    }

    static const sbe_uint8_t dayNullValue()
    {
        return (sbe_uint8_t)255;
    }

    static const sbe_uint8_t dayMinValue()
    {
        return (sbe_uint8_t)0;
    }

    static const sbe_uint8_t dayMaxValue()
    {
        return (sbe_uint8_t)254;
    }

    sbe_uint8_t day(void) const
    {
        return (*((sbe_uint8_t *)(buffer_ + offset_ + 3)));
    }

    MaturityMonthYear &day(const sbe_uint8_t value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 3)) = (value);
        return *this;
    }

    static const sbe_uint8_t weekNullValue()
    {
        return (sbe_uint8_t)255;
    }

    static const sbe_uint8_t weekMinValue()
    {
        return (sbe_uint8_t)0;
    }

    static const sbe_uint8_t weekMaxValue()
    {
        return (sbe_uint8_t)254;
    }

    sbe_uint8_t week(void) const
    {
        return (*((sbe_uint8_t *)(buffer_ + offset_ + 4)));
    }

    MaturityMonthYear &week(const sbe_uint8_t value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 4)) = (value);
        return *this;
    }
};
}
#endif
