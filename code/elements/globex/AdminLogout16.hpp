/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_ADMINLOGOUT16_HPP_
#define _MKTDATA_ADMINLOGOUT16_HPP_

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

#include <globex/MDEntryTypeBook.hpp>
#include <globex/OpenCloseSettlFlag.hpp>
#include <globex/SecurityTradingStatus.hpp>
#include <globex/LegSide.hpp>
#include <globex/MatchEventIndicator.hpp>
#include <globex/MaturityMonthYear.hpp>
#include <globex/FLOAT.hpp>
#include <globex/MDEntryTypeDailyStatistics.hpp>
#include <globex/PRICE.hpp>
#include <globex/PutOrCall.hpp>
#include <globex/EventType.hpp>
#include <globex/SecurityUpdateAction.hpp>
#include <globex/SecurityTradingEvent.hpp>
#include <globex/DecimalQty.hpp>
#include <globex/MDEntryTypeStatistics.hpp>
#include <globex/MDUpdateAction.hpp>
#include <globex/InstAttribValue.hpp>
#include <globex/AggressorSide.hpp>
#include <globex/GroupSize8Byte.hpp>
#include <globex/HaltReason.hpp>
#include <globex/GroupSize.hpp>
#include <globex/SettlPriceType.hpp>
#include <globex/MDEntryType.hpp>
#include <globex/PRICENULL.hpp>

using namespace sbe;

namespace globex {

class AdminLogout16
{
private:
    char *buffer_;
    int bufferLength_;
    int *positionPtr_;
    int offset_;
    int position_;
    int actingBlockLength_;
    int actingVersion_;

    inline void reset(char *buffer, const int offset, const int bufferLength, const int actingBlockLength, const int actingVersion)
    {
        buffer_ = buffer;
        offset_ = offset;
        bufferLength_ = bufferLength;
        actingBlockLength_ = actingBlockLength;
        actingVersion_ = actingVersion;
        positionPtr_ = &position_;
        position(offset + actingBlockLength_);
    }

public:

    AdminLogout16(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    AdminLogout16(char *buffer, const int bufferLength, const int actingBlockLength, const int actingVersion)
    {
        reset(buffer, 0, bufferLength, actingBlockLength, actingVersion);
    }

    AdminLogout16(const AdminLogout16& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
    }

#if __cplusplus >= 201103L
    AdminLogout16(AdminLogout16&& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
    }

    AdminLogout16& operator=(AdminLogout16&& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
        return *this;
    }

#endif

    AdminLogout16& operator=(const AdminLogout16& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
        return *this;
    }

    static const sbe_uint16_t sbeBlockLength(void)
    {
        return (sbe_uint16_t)180;
    }

    static const sbe_uint16_t sbeTemplateId(void)
    {
        return (sbe_uint16_t)16;
    }

    static const sbe_uint16_t sbeSchemaId(void)
    {
        return (sbe_uint16_t)1;
    }

    static const sbe_uint16_t sbeSchemaVersion(void)
    {
        return (sbe_uint16_t)5;
    }

    static const char *sbeSemanticType(void)
    {
        return "5";
    }

    sbe_uint64_t offset(void) const
    {
        return offset_;
    }

    AdminLogout16 &wrapForEncode(char *buffer, const int offset, const int bufferLength)
    {
        reset(buffer, offset, bufferLength, sbeBlockLength(), sbeSchemaVersion());
        return *this;
    }

    AdminLogout16 &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion, const int bufferLength)
    {
        reset(buffer, offset, bufferLength, actingBlockLength, actingVersion);
        return *this;
    }

    sbe_uint64_t position(void) const
    {
        return position_;
    }

    void position(const int position)
    {
        if (SBE_BOUNDS_CHECK_EXPECT((position > bufferLength_), false))
        {
            throw std::runtime_error("buffer too short [E100]");
        }
        position_ = position;
    }

    int size(void) const
    {
        return position() - offset_;
    }

    char *buffer(void)
    {
        return buffer_;
    }

    int actingVersion(void) const
    {
        return actingVersion_;
    }

    static const int textId(void)
    {
        return 58;
    }

    static const int textSinceVersion(void)
    {
         return 0;
    }

    bool textInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *TextMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "String";
        }

        return "";
    }

    static const sbe_char_t textNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t textMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t textMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int textLength(void)
    {
        return 180;
    }

    const char *text(void) const
    {
        return (buffer_ + offset_ + 0);
    }

    sbe_char_t text(const int index) const
    {
        if (index < 0 || index >= 180)
        {
            throw std::runtime_error("index out of range for text [E104]");
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 0 + (index * 1))));
    }

    void text(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 180)
        {
            throw std::runtime_error("index out of range for text [E105]");
        }

        *((sbe_char_t *)(buffer_ + offset_ + 0 + (index * 1))) = (value);
    }

    int getText(char *dst, const int length) const
    {
        if (length > 180)
        {
             throw std::runtime_error("length too large for getText [E106]");
        }

        ::memcpy(dst, buffer_ + offset_ + 0, length);
        return length;
    }

    AdminLogout16 &putText(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 0, src, 180);
        return *this;
    }
};
}
#endif
