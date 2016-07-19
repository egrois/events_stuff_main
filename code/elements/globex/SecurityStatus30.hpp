/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_SECURITYSTATUS30_HPP_
#define _MKTDATA_SECURITYSTATUS30_HPP_

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

class SecurityStatus30
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

    SecurityStatus30(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    SecurityStatus30(char *buffer, const int bufferLength, const int actingBlockLength, const int actingVersion)
    {
        reset(buffer, 0, bufferLength, actingBlockLength, actingVersion);
    }

    SecurityStatus30(const SecurityStatus30& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
    }

#if __cplusplus >= 201103L
    SecurityStatus30(SecurityStatus30&& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
    }

    SecurityStatus30& operator=(SecurityStatus30&& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
        return *this;
    }

#endif

    SecurityStatus30& operator=(const SecurityStatus30& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
        return *this;
    }

    static const sbe_uint16_t sbeBlockLength(void)
    {
        return (sbe_uint16_t)30;
    }

    static const sbe_uint16_t sbeTemplateId(void)
    {
        return (sbe_uint16_t)30;
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
        return "f";
    }

    sbe_uint64_t offset(void) const
    {
        return offset_;
    }

    SecurityStatus30 &wrapForEncode(char *buffer, const int offset, const int bufferLength)
    {
        reset(buffer, offset, bufferLength, sbeBlockLength(), sbeSchemaVersion());
        return *this;
    }

    SecurityStatus30 &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion, const int bufferLength)
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

    static const int transactTimeId(void)
    {
        return 60;
    }

    static const int transactTimeSinceVersion(void)
    {
         return 0;
    }

    bool transactTimeInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *TransactTimeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "UTCTimestamp";
        }

        return "";
    }

    static const sbe_uint64_t transactTimeNullValue()
    {
        return SBE_NULLVALUE_UINT64;
    }

    static const sbe_uint64_t transactTimeMinValue()
    {
        return 0x0L;
    }

    static const sbe_uint64_t transactTimeMaxValue()
    {
        return 0xfffffffffffffffeL;
    }

    sbe_uint64_t transactTime(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_uint64_t *)(buffer_ + offset_ + 0)));
    }

    SecurityStatus30 &transactTime(const sbe_uint64_t value)
    {
        *((sbe_uint64_t *)(buffer_ + offset_ + 0)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const int securityGroupId(void)
    {
        return 1151;
    }

    static const int securityGroupSinceVersion(void)
    {
         return 0;
    }

    bool securityGroupInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SecurityGroupMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "String";
        }

        return "";
    }

    static const sbe_char_t securityGroupNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t securityGroupMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t securityGroupMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int securityGroupLength(void)
    {
        return 6;
    }

    const char *securityGroup(void) const
    {
        return (buffer_ + offset_ + 8);
    }

    sbe_char_t securityGroup(const int index) const
    {
        if (index < 0 || index >= 6)
        {
            throw std::runtime_error("index out of range for securityGroup [E104]");
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 8 + (index * 1))));
    }

    void securityGroup(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 6)
        {
            throw std::runtime_error("index out of range for securityGroup [E105]");
        }

        *((sbe_char_t *)(buffer_ + offset_ + 8 + (index * 1))) = (value);
    }

    int getSecurityGroup(char *dst, const int length) const
    {
        if (length > 6)
        {
             throw std::runtime_error("length too large for getSecurityGroup [E106]");
        }

        ::memcpy(dst, buffer_ + offset_ + 8, length);
        return length;
    }

    SecurityStatus30 &putSecurityGroup(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 8, src, 6);
        return *this;
    }

    static const int assetId(void)
    {
        return 6937;
    }

    static const int assetSinceVersion(void)
    {
         return 0;
    }

    bool assetInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *AssetMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "String";
        }

        return "";
    }

    static const sbe_char_t assetNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t assetMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t assetMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int assetLength(void)
    {
        return 6;
    }

    const char *asset(void) const
    {
        return (buffer_ + offset_ + 14);
    }

    sbe_char_t asset(const int index) const
    {
        if (index < 0 || index >= 6)
        {
            throw std::runtime_error("index out of range for asset [E104]");
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 14 + (index * 1))));
    }

    void asset(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 6)
        {
            throw std::runtime_error("index out of range for asset [E105]");
        }

        *((sbe_char_t *)(buffer_ + offset_ + 14 + (index * 1))) = (value);
    }

    int getAsset(char *dst, const int length) const
    {
        if (length > 6)
        {
             throw std::runtime_error("length too large for getAsset [E106]");
        }

        ::memcpy(dst, buffer_ + offset_ + 14, length);
        return length;
    }

    SecurityStatus30 &putAsset(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 14, src, 6);
        return *this;
    }

    static const int securityIDId(void)
    {
        return 48;
    }

    static const int securityIDSinceVersion(void)
    {
         return 0;
    }

    bool securityIDInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SecurityIDMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "int";
        }

        return "";
    }

    static const sbe_int32_t securityIDNullValue()
    {
        return 2147483647;
    }

    static const sbe_int32_t securityIDMinValue()
    {
        return -2147483647;
    }

    static const sbe_int32_t securityIDMaxValue()
    {
        return 2147483647;
    }

    sbe_int32_t securityID(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_int32_t *)(buffer_ + offset_ + 20)));
    }

    SecurityStatus30 &securityID(const sbe_int32_t value)
    {
        *((sbe_int32_t *)(buffer_ + offset_ + 20)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int tradeDateId(void)
    {
        return 75;
    }

    static const int tradeDateSinceVersion(void)
    {
         return 0;
    }

    bool tradeDateInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *TradeDateMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "LocalMktDate";
        }

        return "";
    }

    static const sbe_uint16_t tradeDateNullValue()
    {
        return (sbe_uint16_t)65535;
    }

    static const sbe_uint16_t tradeDateMinValue()
    {
        return (sbe_uint16_t)0;
    }

    static const sbe_uint16_t tradeDateMaxValue()
    {
        return (sbe_uint16_t)65534;
    }

    sbe_uint16_t tradeDate(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_16(*((sbe_uint16_t *)(buffer_ + offset_ + 24)));
    }

    SecurityStatus30 &tradeDate(const sbe_uint16_t value)
    {
        *((sbe_uint16_t *)(buffer_ + offset_ + 24)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
        return *this;
    }

    static const int matchEventIndicatorId(void)
    {
        return 5799;
    }

    static const int matchEventIndicatorSinceVersion(void)
    {
         return 0;
    }

    bool matchEventIndicatorInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *MatchEventIndicatorMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "MultipleCharValue";
        }

        return "";
    }

private:
    MatchEventIndicator matchEventIndicator_;

public:

    MatchEventIndicator &matchEventIndicator()
    {
        matchEventIndicator_.wrap(buffer_, offset_ + 26, actingVersion_, bufferLength_);
        return matchEventIndicator_;
    }

    static const int securityTradingStatusId(void)
    {
        return 326;
    }

    static const int securityTradingStatusSinceVersion(void)
    {
         return 0;
    }

    bool securityTradingStatusInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SecurityTradingStatusMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "int";
        }

        return "";
    }

    SecurityTradingStatus::Value securityTradingStatus(void) const
    {
        return SecurityTradingStatus::get((*((sbe_uint8_t *)(buffer_ + offset_ + 27))));
    }

    SecurityStatus30 &securityTradingStatus(const SecurityTradingStatus::Value value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 27)) = (value);
        return *this;
    }

    static const int haltReasonId(void)
    {
        return 327;
    }

    static const int haltReasonSinceVersion(void)
    {
         return 0;
    }

    bool haltReasonInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *HaltReasonMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "int";
        }

        return "";
    }

    HaltReason::Value haltReason(void) const
    {
        return HaltReason::get((*((sbe_uint8_t *)(buffer_ + offset_ + 28))));
    }

    SecurityStatus30 &haltReason(const HaltReason::Value value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 28)) = (value);
        return *this;
    }

    static const int securityTradingEventId(void)
    {
        return 1174;
    }

    static const int securityTradingEventSinceVersion(void)
    {
         return 0;
    }

    bool securityTradingEventInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SecurityTradingEventMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "int";
        }

        return "";
    }

    SecurityTradingEvent::Value securityTradingEvent(void) const
    {
        return SecurityTradingEvent::get((*((sbe_uint8_t *)(buffer_ + offset_ + 29))));
    }

    SecurityStatus30 &securityTradingEvent(const SecurityTradingEvent::Value value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 29)) = (value);
        return *this;
    }
};
}
#endif
