/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_MDINSTRUMENTDEFINITIONSPREAD29_HPP_
#define _MKTDATA_MDINSTRUMENTDEFINITIONSPREAD29_HPP_

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

class MDInstrumentDefinitionSpread29
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

    MDInstrumentDefinitionSpread29(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    MDInstrumentDefinitionSpread29(char *buffer, const int bufferLength, const int actingBlockLength, const int actingVersion)
    {
        reset(buffer, 0, bufferLength, actingBlockLength, actingVersion);
    }

    MDInstrumentDefinitionSpread29(const MDInstrumentDefinitionSpread29& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
    }

#if __cplusplus >= 201103L
    MDInstrumentDefinitionSpread29(MDInstrumentDefinitionSpread29&& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
    }

    MDInstrumentDefinitionSpread29& operator=(MDInstrumentDefinitionSpread29&& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
        return *this;
    }

#endif

    MDInstrumentDefinitionSpread29& operator=(const MDInstrumentDefinitionSpread29& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
        return *this;
    }

    static const sbe_uint16_t sbeBlockLength(void)
    {
        return (sbe_uint16_t)193;
    }

    static const sbe_uint16_t sbeTemplateId(void)
    {
        return (sbe_uint16_t)29;
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
        return "d";
    }

    sbe_uint64_t offset(void) const
    {
        return offset_;
    }

    MDInstrumentDefinitionSpread29 &wrapForEncode(char *buffer, const int offset, const int bufferLength)
    {
        reset(buffer, offset, bufferLength, sbeBlockLength(), sbeSchemaVersion());
        return *this;
    }

    MDInstrumentDefinitionSpread29 &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion, const int bufferLength)
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
        matchEventIndicator_.wrap(buffer_, offset_ + 0, actingVersion_, bufferLength_);
        return matchEventIndicator_;
    }

    static const int totNumReportsId(void)
    {
        return 911;
    }

    static const int totNumReportsSinceVersion(void)
    {
         return 0;
    }

    bool totNumReportsInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *TotNumReportsMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "int";
        }

        return "";
    }

    static const sbe_uint32_t totNumReportsNullValue()
    {
        return 4294967295;
    }

    static const sbe_uint32_t totNumReportsMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t totNumReportsMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t totNumReports(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 1)));
    }

    MDInstrumentDefinitionSpread29 &totNumReports(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 1)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int securityUpdateActionId(void)
    {
        return 980;
    }

    static const int securityUpdateActionSinceVersion(void)
    {
         return 0;
    }

    bool securityUpdateActionInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SecurityUpdateActionMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "char";
        }

        return "";
    }

    SecurityUpdateAction::Value securityUpdateAction(void) const
    {
        return SecurityUpdateAction::get((*((sbe_char_t *)(buffer_ + offset_ + 5))));
    }

    MDInstrumentDefinitionSpread29 &securityUpdateAction(const SecurityUpdateAction::Value value)
    {
        *((sbe_char_t *)(buffer_ + offset_ + 5)) = (value);
        return *this;
    }

    static const int lastUpdateTimeId(void)
    {
        return 779;
    }

    static const int lastUpdateTimeSinceVersion(void)
    {
         return 0;
    }

    bool lastUpdateTimeInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *LastUpdateTimeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "UTCTimestamp";
        }

        return "";
    }

    static const sbe_uint64_t lastUpdateTimeNullValue()
    {
        return SBE_NULLVALUE_UINT64;
    }

    static const sbe_uint64_t lastUpdateTimeMinValue()
    {
        return 0x0L;
    }

    static const sbe_uint64_t lastUpdateTimeMaxValue()
    {
        return 0xfffffffffffffffeL;
    }

    sbe_uint64_t lastUpdateTime(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_uint64_t *)(buffer_ + offset_ + 6)));
    }

    MDInstrumentDefinitionSpread29 &lastUpdateTime(const sbe_uint64_t value)
    {
        *((sbe_uint64_t *)(buffer_ + offset_ + 6)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const int mDSecurityTradingStatusId(void)
    {
        return 1682;
    }

    static const int mDSecurityTradingStatusSinceVersion(void)
    {
         return 0;
    }

    bool mDSecurityTradingStatusInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *MDSecurityTradingStatusMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "int";
        }

        return "";
    }

    SecurityTradingStatus::Value mDSecurityTradingStatus(void) const
    {
        return SecurityTradingStatus::get((*((sbe_uint8_t *)(buffer_ + offset_ + 14))));
    }

    MDInstrumentDefinitionSpread29 &mDSecurityTradingStatus(const SecurityTradingStatus::Value value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 14)) = (value);
        return *this;
    }

    static const int applIDId(void)
    {
        return 1180;
    }

    static const int applIDSinceVersion(void)
    {
         return 0;
    }

    bool applIDInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *ApplIDMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "int";
        }

        return "";
    }

    static const sbe_int16_t applIDNullValue()
    {
        return SBE_NULLVALUE_INT16;
    }

    static const sbe_int16_t applIDMinValue()
    {
        return (sbe_int16_t)-32767;
    }

    static const sbe_int16_t applIDMaxValue()
    {
        return (sbe_int16_t)32767;
    }

    sbe_int16_t applID(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_16(*((sbe_int16_t *)(buffer_ + offset_ + 15)));
    }

    MDInstrumentDefinitionSpread29 &applID(const sbe_int16_t value)
    {
        *((sbe_int16_t *)(buffer_ + offset_ + 15)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
        return *this;
    }

    static const int marketSegmentIDId(void)
    {
        return 1300;
    }

    static const int marketSegmentIDSinceVersion(void)
    {
         return 0;
    }

    bool marketSegmentIDInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *MarketSegmentIDMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "int";
        }

        return "";
    }

    static const sbe_uint8_t marketSegmentIDNullValue()
    {
        return SBE_NULLVALUE_UINT8;
    }

    static const sbe_uint8_t marketSegmentIDMinValue()
    {
        return (sbe_uint8_t)0;
    }

    static const sbe_uint8_t marketSegmentIDMaxValue()
    {
        return (sbe_uint8_t)254;
    }

    sbe_uint8_t marketSegmentID(void) const
    {
        return (*((sbe_uint8_t *)(buffer_ + offset_ + 17)));
    }

    MDInstrumentDefinitionSpread29 &marketSegmentID(const sbe_uint8_t value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 17)) = (value);
        return *this;
    }

    static const int underlyingProductId(void)
    {
        return 462;
    }

    static const int underlyingProductSinceVersion(void)
    {
         return 0;
    }

    bool underlyingProductInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *UnderlyingProductMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "int";
        }

        return "";
    }

    static const sbe_uint8_t underlyingProductNullValue()
    {
        return (sbe_uint8_t)255;
    }

    static const sbe_uint8_t underlyingProductMinValue()
    {
        return (sbe_uint8_t)0;
    }

    static const sbe_uint8_t underlyingProductMaxValue()
    {
        return (sbe_uint8_t)254;
    }

    sbe_uint8_t underlyingProduct(void) const
    {
        return (*((sbe_uint8_t *)(buffer_ + offset_ + 18)));
    }

    MDInstrumentDefinitionSpread29 &underlyingProduct(const sbe_uint8_t value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 18)) = (value);
        return *this;
    }

    static const int securityExchangeId(void)
    {
        return 207;
    }

    static const int securityExchangeSinceVersion(void)
    {
         return 0;
    }

    bool securityExchangeInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SecurityExchangeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "Exchange";
        }

        return "";
    }

    static const sbe_char_t securityExchangeNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t securityExchangeMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t securityExchangeMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int securityExchangeLength(void)
    {
        return 4;
    }

    const char *securityExchange(void) const
    {
        return (buffer_ + offset_ + 19);
    }

    sbe_char_t securityExchange(const int index) const
    {
        if (index < 0 || index >= 4)
        {
            throw std::runtime_error("index out of range for securityExchange [E104]");
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 19 + (index * 1))));
    }

    void securityExchange(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 4)
        {
            throw std::runtime_error("index out of range for securityExchange [E105]");
        }

        *((sbe_char_t *)(buffer_ + offset_ + 19 + (index * 1))) = (value);
    }

    int getSecurityExchange(char *dst, const int length) const
    {
        if (length > 4)
        {
             throw std::runtime_error("length too large for getSecurityExchange [E106]");
        }

        ::memcpy(dst, buffer_ + offset_ + 19, length);
        return length;
    }

    MDInstrumentDefinitionSpread29 &putSecurityExchange(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 19, src, 4);
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
        return (buffer_ + offset_ + 23);
    }

    sbe_char_t securityGroup(const int index) const
    {
        if (index < 0 || index >= 6)
        {
            throw std::runtime_error("index out of range for securityGroup [E104]");
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 23 + (index * 1))));
    }

    void securityGroup(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 6)
        {
            throw std::runtime_error("index out of range for securityGroup [E105]");
        }

        *((sbe_char_t *)(buffer_ + offset_ + 23 + (index * 1))) = (value);
    }

    int getSecurityGroup(char *dst, const int length) const
    {
        if (length > 6)
        {
             throw std::runtime_error("length too large for getSecurityGroup [E106]");
        }

        ::memcpy(dst, buffer_ + offset_ + 23, length);
        return length;
    }

    MDInstrumentDefinitionSpread29 &putSecurityGroup(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 23, src, 6);
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
        return (buffer_ + offset_ + 29);
    }

    sbe_char_t asset(const int index) const
    {
        if (index < 0 || index >= 6)
        {
            throw std::runtime_error("index out of range for asset [E104]");
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 29 + (index * 1))));
    }

    void asset(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 6)
        {
            throw std::runtime_error("index out of range for asset [E105]");
        }

        *((sbe_char_t *)(buffer_ + offset_ + 29 + (index * 1))) = (value);
    }

    int getAsset(char *dst, const int length) const
    {
        if (length > 6)
        {
             throw std::runtime_error("length too large for getAsset [E106]");
        }

        ::memcpy(dst, buffer_ + offset_ + 29, length);
        return length;
    }

    MDInstrumentDefinitionSpread29 &putAsset(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 29, src, 6);
        return *this;
    }

    static const int symbolId(void)
    {
        return 55;
    }

    static const int symbolSinceVersion(void)
    {
         return 0;
    }

    bool symbolInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SymbolMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "String";
        }

        return "";
    }

    static const sbe_char_t symbolNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t symbolMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t symbolMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int symbolLength(void)
    {
        return 20;
    }

    const char *symbol(void) const
    {
        return (buffer_ + offset_ + 35);
    }

    sbe_char_t symbol(const int index) const
    {
        if (index < 0 || index >= 20)
        {
            throw std::runtime_error("index out of range for symbol [E104]");
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 35 + (index * 1))));
    }

    void symbol(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 20)
        {
            throw std::runtime_error("index out of range for symbol [E105]");
        }

        *((sbe_char_t *)(buffer_ + offset_ + 35 + (index * 1))) = (value);
    }

    int getSymbol(char *dst, const int length) const
    {
        if (length > 20)
        {
             throw std::runtime_error("length too large for getSymbol [E106]");
        }

        ::memcpy(dst, buffer_ + offset_ + 35, length);
        return length;
    }

    MDInstrumentDefinitionSpread29 &putSymbol(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 35, src, 20);
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
        return SBE_NULLVALUE_INT32;
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
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_int32_t *)(buffer_ + offset_ + 55)));
    }

    MDInstrumentDefinitionSpread29 &securityID(const sbe_int32_t value)
    {
        *((sbe_int32_t *)(buffer_ + offset_ + 55)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int securityIDSourceId(void)
    {
        return 22;
    }

    static const int securityIDSourceSinceVersion(void)
    {
         return 0;
    }

    bool securityIDSourceInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SecurityIDSourceMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "char";
        }

        return "";
    }

    static const sbe_char_t securityIDSourceNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t securityIDSourceMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t securityIDSourceMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int securityIDSourceLength(void)
    {
        return 1;
    }

    const char *securityIDSource(void) const
    {
        static sbe_uint8_t securityIDSourceValues[] = {56};

        return (const char *)securityIDSourceValues;
    }

    sbe_char_t securityIDSource(const int index) const
    {
        static sbe_uint8_t securityIDSourceValues[] = {56};

        return securityIDSourceValues[index];
    }

    int getSecurityIDSource(char *dst, const int length) const
    {
        static sbe_uint8_t securityIDSourceValues[] = {56};
        int bytesToCopy = (length < (int)sizeof(securityIDSourceValues)) ? length : (int)sizeof(securityIDSourceValues);

        ::memcpy(dst, securityIDSourceValues, bytesToCopy);
        return bytesToCopy;
    }

    static const int securityTypeId(void)
    {
        return 167;
    }

    static const int securityTypeSinceVersion(void)
    {
         return 0;
    }

    bool securityTypeInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SecurityTypeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "String";
        }

        return "";
    }

    static const sbe_char_t securityTypeNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t securityTypeMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t securityTypeMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int securityTypeLength(void)
    {
        return 6;
    }

    const char *securityType(void) const
    {
        return (buffer_ + offset_ + 59);
    }

    sbe_char_t securityType(const int index) const
    {
        if (index < 0 || index >= 6)
        {
            throw std::runtime_error("index out of range for securityType [E104]");
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 59 + (index * 1))));
    }

    void securityType(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 6)
        {
            throw std::runtime_error("index out of range for securityType [E105]");
        }

        *((sbe_char_t *)(buffer_ + offset_ + 59 + (index * 1))) = (value);
    }

    int getSecurityType(char *dst, const int length) const
    {
        if (length > 6)
        {
             throw std::runtime_error("length too large for getSecurityType [E106]");
        }

        ::memcpy(dst, buffer_ + offset_ + 59, length);
        return length;
    }

    MDInstrumentDefinitionSpread29 &putSecurityType(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 59, src, 6);
        return *this;
    }

    static const int cFICodeId(void)
    {
        return 461;
    }

    static const int cFICodeSinceVersion(void)
    {
         return 0;
    }

    bool cFICodeInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *CFICodeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "String";
        }

        return "";
    }

    static const sbe_char_t cFICodeNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t cFICodeMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t cFICodeMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int cFICodeLength(void)
    {
        return 6;
    }

    const char *cFICode(void) const
    {
        return (buffer_ + offset_ + 65);
    }

    sbe_char_t cFICode(const int index) const
    {
        if (index < 0 || index >= 6)
        {
            throw std::runtime_error("index out of range for cFICode [E104]");
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 65 + (index * 1))));
    }

    void cFICode(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 6)
        {
            throw std::runtime_error("index out of range for cFICode [E105]");
        }

        *((sbe_char_t *)(buffer_ + offset_ + 65 + (index * 1))) = (value);
    }

    int getCFICode(char *dst, const int length) const
    {
        if (length > 6)
        {
             throw std::runtime_error("length too large for getCFICode [E106]");
        }

        ::memcpy(dst, buffer_ + offset_ + 65, length);
        return length;
    }

    MDInstrumentDefinitionSpread29 &putCFICode(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 65, src, 6);
        return *this;
    }

    static const int maturityMonthYearId(void)
    {
        return 200;
    }

    static const int maturityMonthYearSinceVersion(void)
    {
         return 0;
    }

    bool maturityMonthYearInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *MaturityMonthYearMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "MonthYear";
        }

        return "";
    }

private:
    MaturityMonthYear maturityMonthYear_;

public:

    MaturityMonthYear &maturityMonthYear(void)
    {
        maturityMonthYear_.wrap(buffer_, offset_ + 71, actingVersion_, bufferLength_);
        return maturityMonthYear_;
    }

    static const int currencyId(void)
    {
        return 15;
    }

    static const int currencySinceVersion(void)
    {
         return 0;
    }

    bool currencyInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *CurrencyMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "Currency";
        }

        return "";
    }

    static const sbe_char_t currencyNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t currencyMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t currencyMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int currencyLength(void)
    {
        return 3;
    }

    const char *currency(void) const
    {
        return (buffer_ + offset_ + 76);
    }

    sbe_char_t currency(const int index) const
    {
        if (index < 0 || index >= 3)
        {
            throw std::runtime_error("index out of range for currency [E104]");
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 76 + (index * 1))));
    }

    void currency(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 3)
        {
            throw std::runtime_error("index out of range for currency [E105]");
        }

        *((sbe_char_t *)(buffer_ + offset_ + 76 + (index * 1))) = (value);
    }

    int getCurrency(char *dst, const int length) const
    {
        if (length > 3)
        {
             throw std::runtime_error("length too large for getCurrency [E106]");
        }

        ::memcpy(dst, buffer_ + offset_ + 76, length);
        return length;
    }

    MDInstrumentDefinitionSpread29 &putCurrency(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 76, src, 3);
        return *this;
    }

    static const int securitySubTypeId(void)
    {
        return 762;
    }

    static const int securitySubTypeSinceVersion(void)
    {
         return 0;
    }

    bool securitySubTypeInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SecuritySubTypeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "String";
        }

        return "";
    }

    static const sbe_char_t securitySubTypeNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t securitySubTypeMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t securitySubTypeMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int securitySubTypeLength(void)
    {
        return 5;
    }

    const char *securitySubType(void) const
    {
        return (buffer_ + offset_ + 79);
    }

    sbe_char_t securitySubType(const int index) const
    {
        if (index < 0 || index >= 5)
        {
            throw std::runtime_error("index out of range for securitySubType [E104]");
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 79 + (index * 1))));
    }

    void securitySubType(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 5)
        {
            throw std::runtime_error("index out of range for securitySubType [E105]");
        }

        *((sbe_char_t *)(buffer_ + offset_ + 79 + (index * 1))) = (value);
    }

    int getSecuritySubType(char *dst, const int length) const
    {
        if (length > 5)
        {
             throw std::runtime_error("length too large for getSecuritySubType [E106]");
        }

        ::memcpy(dst, buffer_ + offset_ + 79, length);
        return length;
    }

    MDInstrumentDefinitionSpread29 &putSecuritySubType(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 79, src, 5);
        return *this;
    }

    static const int userDefinedInstrumentId(void)
    {
        return 9779;
    }

    static const int userDefinedInstrumentSinceVersion(void)
    {
         return 0;
    }

    bool userDefinedInstrumentInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *UserDefinedInstrumentMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "char";
        }

        return "";
    }

    static const sbe_char_t userDefinedInstrumentNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t userDefinedInstrumentMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t userDefinedInstrumentMaxValue()
    {
        return (sbe_char_t)126;
    }

    sbe_char_t userDefinedInstrument(void) const
    {
        return (*((sbe_char_t *)(buffer_ + offset_ + 84)));
    }

    MDInstrumentDefinitionSpread29 &userDefinedInstrument(const sbe_char_t value)
    {
        *((sbe_char_t *)(buffer_ + offset_ + 84)) = (value);
        return *this;
    }

    static const int matchAlgorithmId(void)
    {
        return 1142;
    }

    static const int matchAlgorithmSinceVersion(void)
    {
         return 0;
    }

    bool matchAlgorithmInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *MatchAlgorithmMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "char";
        }

        return "";
    }

    static const sbe_char_t matchAlgorithmNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t matchAlgorithmMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t matchAlgorithmMaxValue()
    {
        return (sbe_char_t)126;
    }

    sbe_char_t matchAlgorithm(void) const
    {
        return (*((sbe_char_t *)(buffer_ + offset_ + 85)));
    }

    MDInstrumentDefinitionSpread29 &matchAlgorithm(const sbe_char_t value)
    {
        *((sbe_char_t *)(buffer_ + offset_ + 85)) = (value);
        return *this;
    }

    static const int minTradeVolId(void)
    {
        return 562;
    }

    static const int minTradeVolSinceVersion(void)
    {
         return 0;
    }

    bool minTradeVolInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *MinTradeVolMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "Qty";
        }

        return "";
    }

    static const sbe_uint32_t minTradeVolNullValue()
    {
        return SBE_NULLVALUE_UINT32;
    }

    static const sbe_uint32_t minTradeVolMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t minTradeVolMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t minTradeVol(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 86)));
    }

    MDInstrumentDefinitionSpread29 &minTradeVol(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 86)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int maxTradeVolId(void)
    {
        return 1140;
    }

    static const int maxTradeVolSinceVersion(void)
    {
         return 0;
    }

    bool maxTradeVolInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *MaxTradeVolMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "Qty";
        }

        return "";
    }

    static const sbe_uint32_t maxTradeVolNullValue()
    {
        return SBE_NULLVALUE_UINT32;
    }

    static const sbe_uint32_t maxTradeVolMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t maxTradeVolMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t maxTradeVol(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 90)));
    }

    MDInstrumentDefinitionSpread29 &maxTradeVol(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 90)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int minPriceIncrementId(void)
    {
        return 969;
    }

    static const int minPriceIncrementSinceVersion(void)
    {
         return 0;
    }

    bool minPriceIncrementInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *MinPriceIncrementMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "Price";
        }

        return "";
    }

private:
    PRICE minPriceIncrement_;

public:

    PRICE &minPriceIncrement(void)
    {
        minPriceIncrement_.wrap(buffer_, offset_ + 94, actingVersion_, bufferLength_);
        return minPriceIncrement_;
    }

    static const int displayFactorId(void)
    {
        return 9787;
    }

    static const int displayFactorSinceVersion(void)
    {
         return 0;
    }

    bool displayFactorInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *DisplayFactorMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "float";
        }

        return "";
    }

private:
    FLOAT displayFactor_;

public:

    FLOAT &displayFactor(void)
    {
        displayFactor_.wrap(buffer_, offset_ + 102, actingVersion_, bufferLength_);
        return displayFactor_;
    }

    static const int priceDisplayFormatId(void)
    {
        return 9800;
    }

    static const int priceDisplayFormatSinceVersion(void)
    {
         return 0;
    }

    bool priceDisplayFormatInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *PriceDisplayFormatMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "int";
        }

        return "";
    }

    static const sbe_uint8_t priceDisplayFormatNullValue()
    {
        return (sbe_uint8_t)255;
    }

    static const sbe_uint8_t priceDisplayFormatMinValue()
    {
        return (sbe_uint8_t)0;
    }

    static const sbe_uint8_t priceDisplayFormatMaxValue()
    {
        return (sbe_uint8_t)254;
    }

    sbe_uint8_t priceDisplayFormat(void) const
    {
        return (*((sbe_uint8_t *)(buffer_ + offset_ + 110)));
    }

    MDInstrumentDefinitionSpread29 &priceDisplayFormat(const sbe_uint8_t value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 110)) = (value);
        return *this;
    }

    static const int priceRatioId(void)
    {
        return 5770;
    }

    static const int priceRatioSinceVersion(void)
    {
         return 0;
    }

    bool priceRatioInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *PriceRatioMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "Price";
        }

        return "";
    }

private:
    PRICENULL priceRatio_;

public:

    PRICENULL &priceRatio(void)
    {
        priceRatio_.wrap(buffer_, offset_ + 111, actingVersion_, bufferLength_);
        return priceRatio_;
    }

    static const int tickRuleId(void)
    {
        return 6350;
    }

    static const int tickRuleSinceVersion(void)
    {
         return 0;
    }

    bool tickRuleInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *TickRuleMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "int";
        }

        return "";
    }

    static const sbe_int8_t tickRuleNullValue()
    {
        return (sbe_int8_t)127;
    }

    static const sbe_int8_t tickRuleMinValue()
    {
        return (sbe_int8_t)-127;
    }

    static const sbe_int8_t tickRuleMaxValue()
    {
        return (sbe_int8_t)127;
    }

    sbe_int8_t tickRule(void) const
    {
        return (*((sbe_int8_t *)(buffer_ + offset_ + 119)));
    }

    MDInstrumentDefinitionSpread29 &tickRule(const sbe_int8_t value)
    {
        *((sbe_int8_t *)(buffer_ + offset_ + 119)) = (value);
        return *this;
    }

    static const int unitOfMeasureId(void)
    {
        return 996;
    }

    static const int unitOfMeasureSinceVersion(void)
    {
         return 0;
    }

    bool unitOfMeasureInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *UnitOfMeasureMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "String";
        }

        return "";
    }

    static const sbe_char_t unitOfMeasureNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t unitOfMeasureMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t unitOfMeasureMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int unitOfMeasureLength(void)
    {
        return 30;
    }

    const char *unitOfMeasure(void) const
    {
        return (buffer_ + offset_ + 120);
    }

    sbe_char_t unitOfMeasure(const int index) const
    {
        if (index < 0 || index >= 30)
        {
            throw std::runtime_error("index out of range for unitOfMeasure [E104]");
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 120 + (index * 1))));
    }

    void unitOfMeasure(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 30)
        {
            throw std::runtime_error("index out of range for unitOfMeasure [E105]");
        }

        *((sbe_char_t *)(buffer_ + offset_ + 120 + (index * 1))) = (value);
    }

    int getUnitOfMeasure(char *dst, const int length) const
    {
        if (length > 30)
        {
             throw std::runtime_error("length too large for getUnitOfMeasure [E106]");
        }

        ::memcpy(dst, buffer_ + offset_ + 120, length);
        return length;
    }

    MDInstrumentDefinitionSpread29 &putUnitOfMeasure(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 120, src, 30);
        return *this;
    }

    static const int tradingReferencePriceId(void)
    {
        return 1150;
    }

    static const int tradingReferencePriceSinceVersion(void)
    {
         return 0;
    }

    bool tradingReferencePriceInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *TradingReferencePriceMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "Price";
        }

        return "";
    }

private:
    PRICENULL tradingReferencePrice_;

public:

    PRICENULL &tradingReferencePrice(void)
    {
        tradingReferencePrice_.wrap(buffer_, offset_ + 150, actingVersion_, bufferLength_);
        return tradingReferencePrice_;
    }

    static const int settlPriceTypeId(void)
    {
        return 731;
    }

    static const int settlPriceTypeSinceVersion(void)
    {
         return 0;
    }

    bool settlPriceTypeInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SettlPriceTypeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
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
    SettlPriceType settlPriceType_;

public:

    SettlPriceType &settlPriceType()
    {
        settlPriceType_.wrap(buffer_, offset_ + 158, actingVersion_, bufferLength_);
        return settlPriceType_;
    }

    static const int openInterestQtyId(void)
    {
        return 5792;
    }

    static const int openInterestQtySinceVersion(void)
    {
         return 0;
    }

    bool openInterestQtyInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *OpenInterestQtyMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "Qty";
        }

        return "";
    }

    static const sbe_int32_t openInterestQtyNullValue()
    {
        return 2147483647;
    }

    static const sbe_int32_t openInterestQtyMinValue()
    {
        return -2147483647;
    }

    static const sbe_int32_t openInterestQtyMaxValue()
    {
        return 2147483647;
    }

    sbe_int32_t openInterestQty(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_int32_t *)(buffer_ + offset_ + 159)));
    }

    MDInstrumentDefinitionSpread29 &openInterestQty(const sbe_int32_t value)
    {
        *((sbe_int32_t *)(buffer_ + offset_ + 159)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int clearedVolumeId(void)
    {
        return 5791;
    }

    static const int clearedVolumeSinceVersion(void)
    {
         return 0;
    }

    bool clearedVolumeInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *ClearedVolumeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "Qty";
        }

        return "";
    }

    static const sbe_int32_t clearedVolumeNullValue()
    {
        return 2147483647;
    }

    static const sbe_int32_t clearedVolumeMinValue()
    {
        return -2147483647;
    }

    static const sbe_int32_t clearedVolumeMaxValue()
    {
        return 2147483647;
    }

    sbe_int32_t clearedVolume(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_int32_t *)(buffer_ + offset_ + 163)));
    }

    MDInstrumentDefinitionSpread29 &clearedVolume(const sbe_int32_t value)
    {
        *((sbe_int32_t *)(buffer_ + offset_ + 163)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int highLimitPriceId(void)
    {
        return 1149;
    }

    static const int highLimitPriceSinceVersion(void)
    {
         return 0;
    }

    bool highLimitPriceInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *HighLimitPriceMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "Price";
        }

        return "";
    }

private:
    PRICENULL highLimitPrice_;

public:

    PRICENULL &highLimitPrice(void)
    {
        highLimitPrice_.wrap(buffer_, offset_ + 167, actingVersion_, bufferLength_);
        return highLimitPrice_;
    }

    static const int lowLimitPriceId(void)
    {
        return 1148;
    }

    static const int lowLimitPriceSinceVersion(void)
    {
         return 0;
    }

    bool lowLimitPriceInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *LowLimitPriceMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "Price";
        }

        return "";
    }

private:
    PRICENULL lowLimitPrice_;

public:

    PRICENULL &lowLimitPrice(void)
    {
        lowLimitPrice_.wrap(buffer_, offset_ + 175, actingVersion_, bufferLength_);
        return lowLimitPrice_;
    }

    static const int maxPriceVariationId(void)
    {
        return 1143;
    }

    static const int maxPriceVariationSinceVersion(void)
    {
         return 0;
    }

    bool maxPriceVariationInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *MaxPriceVariationMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "Price";
        }

        return "";
    }

private:
    PRICENULL maxPriceVariation_;

public:

    PRICENULL &maxPriceVariation(void)
    {
        maxPriceVariation_.wrap(buffer_, offset_ + 183, actingVersion_, bufferLength_);
        return maxPriceVariation_;
    }

    static const int mainFractionId(void)
    {
        return 37702;
    }

    static const int mainFractionSinceVersion(void)
    {
         return 0;
    }

    bool mainFractionInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *MainFractionMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "int";
        }

        return "";
    }

    static const sbe_uint8_t mainFractionNullValue()
    {
        return (sbe_uint8_t)255;
    }

    static const sbe_uint8_t mainFractionMinValue()
    {
        return (sbe_uint8_t)0;
    }

    static const sbe_uint8_t mainFractionMaxValue()
    {
        return (sbe_uint8_t)254;
    }

    sbe_uint8_t mainFraction(void) const
    {
        return (*((sbe_uint8_t *)(buffer_ + offset_ + 191)));
    }

    MDInstrumentDefinitionSpread29 &mainFraction(const sbe_uint8_t value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 191)) = (value);
        return *this;
    }

    static const int subFractionId(void)
    {
        return 37703;
    }

    static const int subFractionSinceVersion(void)
    {
         return 0;
    }

    bool subFractionInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *SubFractionMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "int";
        }

        return "";
    }

    static const sbe_uint8_t subFractionNullValue()
    {
        return (sbe_uint8_t)255;
    }

    static const sbe_uint8_t subFractionMinValue()
    {
        return (sbe_uint8_t)0;
    }

    static const sbe_uint8_t subFractionMaxValue()
    {
        return (sbe_uint8_t)254;
    }

    sbe_uint8_t subFraction(void) const
    {
        return (*((sbe_uint8_t *)(buffer_ + offset_ + 192)));
    }

    MDInstrumentDefinitionSpread29 &subFraction(const sbe_uint8_t value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 192)) = (value);
        return *this;
    }

    class NoEvents
    {
    private:
        char *buffer_;
        int bufferLength_;
        int *positionPtr_;
        int blockLength_;
        int count_;
        int index_;
        int offset_;
        int actingVersion_;
        GroupSize dimensions_;

    public:

        inline void wrapForDecode(char *buffer, int *pos, const int actingVersion,     const int bufferLength)
        {
            buffer_ = buffer;
            bufferLength_ = bufferLength;
            dimensions_.wrap(buffer_, *pos, actingVersion, bufferLength);
            blockLength_ = dimensions_.blockLength();
            count_ = dimensions_.numInGroup();
            index_ = -1;
            actingVersion_ = actingVersion;
            positionPtr_ = pos;
            *positionPtr_ = *positionPtr_ + 3;
        }

        inline void wrapForEncode(char *buffer, const int count,     int *pos, const int actingVersion, const int bufferLength)
        {
            buffer_ = buffer;
            bufferLength_ = bufferLength;
            dimensions_.wrap(buffer_, *pos, actingVersion, bufferLength);
            dimensions_.blockLength((sbe_uint16_t)9);
            dimensions_.numInGroup((sbe_uint8_t)count);
            index_ = -1;
            count_ = count;
            blockLength_ = 9;
            actingVersion_ = actingVersion;
            positionPtr_ = pos;
            *positionPtr_ = *positionPtr_ + 3;
        }

        static const int sbeHeaderSize()
        {
            return 3;
        }

        static const int sbeBlockLength()
        {
            return 9;
        }

        inline int count(void) const
        {
            return count_;
        }

        inline bool hasNext(void) const
        {
            return index_ + 1 < count_;
        }

        inline NoEvents &next(void)
        {
            offset_ = *positionPtr_;
            if (SBE_BOUNDS_CHECK_EXPECT(( (offset_ + blockLength_) > bufferLength_ ), false))
            {
                throw std::runtime_error("buffer too short to support next group index [E108]");
            }
            *positionPtr_ = offset_ + blockLength_;
            ++index_;

            return *this;
        }

    #if __cplusplus < 201103L
        template<class Func>
        inline void forEach(Func& func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #else
        inline void forEach(std::function<void(NoEvents&)> func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #endif


        static const int eventTypeId(void)
        {
            return 865;
        }

        static const int eventTypeSinceVersion(void)
        {
             return 0;
        }

        bool eventTypeInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *EventTypeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        EventType::Value eventType(void) const
        {
            return EventType::get((*((sbe_uint8_t *)(buffer_ + offset_ + 0))));
        }

        NoEvents &eventType(const EventType::Value value)
        {
            *((sbe_uint8_t *)(buffer_ + offset_ + 0)) = (value);
            return *this;
        }

        static const int eventTimeId(void)
        {
            return 1145;
        }

        static const int eventTimeSinceVersion(void)
        {
             return 0;
        }

        bool eventTimeInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *EventTimeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "UTCTimestamp";
            }

            return "";
        }

        static const sbe_uint64_t eventTimeNullValue()
        {
            return SBE_NULLVALUE_UINT64;
        }

        static const sbe_uint64_t eventTimeMinValue()
        {
            return 0x0L;
        }

        static const sbe_uint64_t eventTimeMaxValue()
        {
            return 0xfffffffffffffffeL;
        }

        sbe_uint64_t eventTime(void) const
        {
            return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_uint64_t *)(buffer_ + offset_ + 1)));
        }

        NoEvents &eventTime(const sbe_uint64_t value)
        {
            *((sbe_uint64_t *)(buffer_ + offset_ + 1)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
            return *this;
        }
    };

private:
    NoEvents noEvents_;

public:

    static const int NoEventsId(void)
    {
        return 864;
    }


    inline NoEvents &noEvents(void)
    {
        noEvents_.wrapForDecode(buffer_, positionPtr_, actingVersion_, bufferLength_);
        return noEvents_;
    }

    NoEvents &noEventsCount(const int count)
    {
        noEvents_.wrapForEncode(buffer_, count, positionPtr_, actingVersion_, bufferLength_);
        return noEvents_;
    }

    class NoMDFeedTypes
    {
    private:
        char *buffer_;
        int bufferLength_;
        int *positionPtr_;
        int blockLength_;
        int count_;
        int index_;
        int offset_;
        int actingVersion_;
        GroupSize dimensions_;

    public:

        inline void wrapForDecode(char *buffer, int *pos, const int actingVersion,     const int bufferLength)
        {
            buffer_ = buffer;
            bufferLength_ = bufferLength;
            dimensions_.wrap(buffer_, *pos, actingVersion, bufferLength);
            blockLength_ = dimensions_.blockLength();
            count_ = dimensions_.numInGroup();
            index_ = -1;
            actingVersion_ = actingVersion;
            positionPtr_ = pos;
            *positionPtr_ = *positionPtr_ + 3;
        }

        inline void wrapForEncode(char *buffer, const int count,     int *pos, const int actingVersion, const int bufferLength)
        {
            buffer_ = buffer;
            bufferLength_ = bufferLength;
            dimensions_.wrap(buffer_, *pos, actingVersion, bufferLength);
            dimensions_.blockLength((sbe_uint16_t)4);
            dimensions_.numInGroup((sbe_uint8_t)count);
            index_ = -1;
            count_ = count;
            blockLength_ = 4;
            actingVersion_ = actingVersion;
            positionPtr_ = pos;
            *positionPtr_ = *positionPtr_ + 3;
        }

        static const int sbeHeaderSize()
        {
            return 3;
        }

        static const int sbeBlockLength()
        {
            return 4;
        }

        inline int count(void) const
        {
            return count_;
        }

        inline bool hasNext(void) const
        {
            return index_ + 1 < count_;
        }

        inline NoMDFeedTypes &next(void)
        {
            offset_ = *positionPtr_;
            if (SBE_BOUNDS_CHECK_EXPECT(( (offset_ + blockLength_) > bufferLength_ ), false))
            {
                throw std::runtime_error("buffer too short to support next group index [E108]");
            }
            *positionPtr_ = offset_ + blockLength_;
            ++index_;

            return *this;
        }

    #if __cplusplus < 201103L
        template<class Func>
        inline void forEach(Func& func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #else
        inline void forEach(std::function<void(NoMDFeedTypes&)> func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #endif


        static const int mDFeedTypeId(void)
        {
            return 1022;
        }

        static const int mDFeedTypeSinceVersion(void)
        {
             return 0;
        }

        bool mDFeedTypeInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *MDFeedTypeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "String";
            }

            return "";
        }

        static const sbe_char_t mDFeedTypeNullValue()
        {
            return (sbe_char_t)0;
        }

        static const sbe_char_t mDFeedTypeMinValue()
        {
            return (sbe_char_t)32;
        }

        static const sbe_char_t mDFeedTypeMaxValue()
        {
            return (sbe_char_t)126;
        }

        static const int mDFeedTypeLength(void)
        {
            return 3;
        }

        const char *mDFeedType(void) const
        {
            return (buffer_ + offset_ + 0);
        }

        sbe_char_t mDFeedType(const int index) const
        {
            if (index < 0 || index >= 3)
            {
                throw std::runtime_error("index out of range for mDFeedType [E104]");
            }

            return (*((sbe_char_t *)(buffer_ + offset_ + 0 + (index * 1))));
        }

        void mDFeedType(const int index, const sbe_char_t value)
        {
            if (index < 0 || index >= 3)
            {
                throw std::runtime_error("index out of range for mDFeedType [E105]");
            }

            *((sbe_char_t *)(buffer_ + offset_ + 0 + (index * 1))) = (value);
        }

        int getMDFeedType(char *dst, const int length) const
        {
            if (length > 3)
            {
                 throw std::runtime_error("length too large for getMDFeedType [E106]");
            }

            ::memcpy(dst, buffer_ + offset_ + 0, length);
            return length;
        }

        NoMDFeedTypes &putMDFeedType(const char *src)
        {
            ::memcpy(buffer_ + offset_ + 0, src, 3);
            return *this;
        }

        static const int marketDepthId(void)
        {
            return 264;
        }

        static const int marketDepthSinceVersion(void)
        {
             return 0;
        }

        bool marketDepthInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *MarketDepthMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        static const sbe_int8_t marketDepthNullValue()
        {
            return SBE_NULLVALUE_INT8;
        }

        static const sbe_int8_t marketDepthMinValue()
        {
            return (sbe_int8_t)-127;
        }

        static const sbe_int8_t marketDepthMaxValue()
        {
            return (sbe_int8_t)127;
        }

        sbe_int8_t marketDepth(void) const
        {
            return (*((sbe_int8_t *)(buffer_ + offset_ + 3)));
        }

        NoMDFeedTypes &marketDepth(const sbe_int8_t value)
        {
            *((sbe_int8_t *)(buffer_ + offset_ + 3)) = (value);
            return *this;
        }
    };

private:
    NoMDFeedTypes noMDFeedTypes_;

public:

    static const int NoMDFeedTypesId(void)
    {
        return 1141;
    }


    inline NoMDFeedTypes &noMDFeedTypes(void)
    {
        noMDFeedTypes_.wrapForDecode(buffer_, positionPtr_, actingVersion_, bufferLength_);
        return noMDFeedTypes_;
    }

    NoMDFeedTypes &noMDFeedTypesCount(const int count)
    {
        noMDFeedTypes_.wrapForEncode(buffer_, count, positionPtr_, actingVersion_, bufferLength_);
        return noMDFeedTypes_;
    }

    class NoInstAttrib
    {
    private:
        char *buffer_;
        int bufferLength_;
        int *positionPtr_;
        int blockLength_;
        int count_;
        int index_;
        int offset_;
        int actingVersion_;
        GroupSize dimensions_;

    public:

        inline void wrapForDecode(char *buffer, int *pos, const int actingVersion,     const int bufferLength)
        {
            buffer_ = buffer;
            bufferLength_ = bufferLength;
            dimensions_.wrap(buffer_, *pos, actingVersion, bufferLength);
            blockLength_ = dimensions_.blockLength();
            count_ = dimensions_.numInGroup();
            index_ = -1;
            actingVersion_ = actingVersion;
            positionPtr_ = pos;
            *positionPtr_ = *positionPtr_ + 3;
        }

        inline void wrapForEncode(char *buffer, const int count,     int *pos, const int actingVersion, const int bufferLength)
        {
            buffer_ = buffer;
            bufferLength_ = bufferLength;
            dimensions_.wrap(buffer_, *pos, actingVersion, bufferLength);
            dimensions_.blockLength((sbe_uint16_t)4);
            dimensions_.numInGroup((sbe_uint8_t)count);
            index_ = -1;
            count_ = count;
            blockLength_ = 4;
            actingVersion_ = actingVersion;
            positionPtr_ = pos;
            *positionPtr_ = *positionPtr_ + 3;
        }

        static const int sbeHeaderSize()
        {
            return 3;
        }

        static const int sbeBlockLength()
        {
            return 4;
        }

        inline int count(void) const
        {
            return count_;
        }

        inline bool hasNext(void) const
        {
            return index_ + 1 < count_;
        }

        inline NoInstAttrib &next(void)
        {
            offset_ = *positionPtr_;
            if (SBE_BOUNDS_CHECK_EXPECT(( (offset_ + blockLength_) > bufferLength_ ), false))
            {
                throw std::runtime_error("buffer too short to support next group index [E108]");
            }
            *positionPtr_ = offset_ + blockLength_;
            ++index_;

            return *this;
        }

    #if __cplusplus < 201103L
        template<class Func>
        inline void forEach(Func& func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #else
        inline void forEach(std::function<void(NoInstAttrib&)> func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #endif


        static const int instAttribTypeId(void)
        {
            return 871;
        }

        static const int instAttribTypeSinceVersion(void)
        {
             return 0;
        }

        bool instAttribTypeInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *InstAttribTypeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        static const sbe_int8_t instAttribTypeNullValue()
        {
            return SBE_NULLVALUE_INT8;
        }

        static const sbe_int8_t instAttribTypeMinValue()
        {
            return (sbe_int8_t)-127;
        }

        static const sbe_int8_t instAttribTypeMaxValue()
        {
            return (sbe_int8_t)127;
        }

        sbe_int8_t instAttribType(void) const
        {
            return (sbe_int8_t)24;
        }

        static const int instAttribValueId(void)
        {
            return 872;
        }

        static const int instAttribValueSinceVersion(void)
        {
             return 0;
        }

        bool instAttribValueInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *InstAttribValueMetaAttribute(const MetaAttribute::Attribute metaAttribute)
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
        InstAttribValue instAttribValue_;

    public:

        InstAttribValue &instAttribValue()
        {
            instAttribValue_.wrap(buffer_, offset_ + 0, actingVersion_, bufferLength_);
            return instAttribValue_;
        }
    };

private:
    NoInstAttrib noInstAttrib_;

public:

    static const int NoInstAttribId(void)
    {
        return 870;
    }


    inline NoInstAttrib &noInstAttrib(void)
    {
        noInstAttrib_.wrapForDecode(buffer_, positionPtr_, actingVersion_, bufferLength_);
        return noInstAttrib_;
    }

    NoInstAttrib &noInstAttribCount(const int count)
    {
        noInstAttrib_.wrapForEncode(buffer_, count, positionPtr_, actingVersion_, bufferLength_);
        return noInstAttrib_;
    }

    class NoLotTypeRules
    {
    private:
        char *buffer_;
        int bufferLength_;
        int *positionPtr_;
        int blockLength_;
        int count_;
        int index_;
        int offset_;
        int actingVersion_;
        GroupSize dimensions_;

    public:

        inline void wrapForDecode(char *buffer, int *pos, const int actingVersion,     const int bufferLength)
        {
            buffer_ = buffer;
            bufferLength_ = bufferLength;
            dimensions_.wrap(buffer_, *pos, actingVersion, bufferLength);
            blockLength_ = dimensions_.blockLength();
            count_ = dimensions_.numInGroup();
            index_ = -1;
            actingVersion_ = actingVersion;
            positionPtr_ = pos;
            *positionPtr_ = *positionPtr_ + 3;
        }

        inline void wrapForEncode(char *buffer, const int count,     int *pos, const int actingVersion, const int bufferLength)
        {
            buffer_ = buffer;
            bufferLength_ = bufferLength;
            dimensions_.wrap(buffer_, *pos, actingVersion, bufferLength);
            dimensions_.blockLength((sbe_uint16_t)5);
            dimensions_.numInGroup((sbe_uint8_t)count);
            index_ = -1;
            count_ = count;
            blockLength_ = 5;
            actingVersion_ = actingVersion;
            positionPtr_ = pos;
            *positionPtr_ = *positionPtr_ + 3;
        }

        static const int sbeHeaderSize()
        {
            return 3;
        }

        static const int sbeBlockLength()
        {
            return 5;
        }

        inline int count(void) const
        {
            return count_;
        }

        inline bool hasNext(void) const
        {
            return index_ + 1 < count_;
        }

        inline NoLotTypeRules &next(void)
        {
            offset_ = *positionPtr_;
            if (SBE_BOUNDS_CHECK_EXPECT(( (offset_ + blockLength_) > bufferLength_ ), false))
            {
                throw std::runtime_error("buffer too short to support next group index [E108]");
            }
            *positionPtr_ = offset_ + blockLength_;
            ++index_;

            return *this;
        }

    #if __cplusplus < 201103L
        template<class Func>
        inline void forEach(Func& func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #else
        inline void forEach(std::function<void(NoLotTypeRules&)> func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #endif


        static const int lotTypeId(void)
        {
            return 1093;
        }

        static const int lotTypeSinceVersion(void)
        {
             return 0;
        }

        bool lotTypeInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *LotTypeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        static const sbe_int8_t lotTypeNullValue()
        {
            return SBE_NULLVALUE_INT8;
        }

        static const sbe_int8_t lotTypeMinValue()
        {
            return (sbe_int8_t)-127;
        }

        static const sbe_int8_t lotTypeMaxValue()
        {
            return (sbe_int8_t)127;
        }

        sbe_int8_t lotType(void) const
        {
            return (*((sbe_int8_t *)(buffer_ + offset_ + 0)));
        }

        NoLotTypeRules &lotType(const sbe_int8_t value)
        {
            *((sbe_int8_t *)(buffer_ + offset_ + 0)) = (value);
            return *this;
        }

        static const int minLotSizeId(void)
        {
            return 1231;
        }

        static const int minLotSizeSinceVersion(void)
        {
             return 0;
        }

        bool minLotSizeInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *MinLotSizeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "Qty";
            }

            return "";
        }

private:
        DecimalQty minLotSize_;

public:

        DecimalQty &minLotSize(void)
        {
            minLotSize_.wrap(buffer_, offset_ + 1, actingVersion_, bufferLength_);
            return minLotSize_;
        }
    };

private:
    NoLotTypeRules noLotTypeRules_;

public:

    static const int NoLotTypeRulesId(void)
    {
        return 1234;
    }


    inline NoLotTypeRules &noLotTypeRules(void)
    {
        noLotTypeRules_.wrapForDecode(buffer_, positionPtr_, actingVersion_, bufferLength_);
        return noLotTypeRules_;
    }

    NoLotTypeRules &noLotTypeRulesCount(const int count)
    {
        noLotTypeRules_.wrapForEncode(buffer_, count, positionPtr_, actingVersion_, bufferLength_);
        return noLotTypeRules_;
    }

    class NoLegs
    {
    private:
        char *buffer_;
        int bufferLength_;
        int *positionPtr_;
        int blockLength_;
        int count_;
        int index_;
        int offset_;
        int actingVersion_;
        GroupSize dimensions_;

    public:

        inline void wrapForDecode(char *buffer, int *pos, const int actingVersion,     const int bufferLength)
        {
            buffer_ = buffer;
            bufferLength_ = bufferLength;
            dimensions_.wrap(buffer_, *pos, actingVersion, bufferLength);
            blockLength_ = dimensions_.blockLength();
            count_ = dimensions_.numInGroup();
            index_ = -1;
            actingVersion_ = actingVersion;
            positionPtr_ = pos;
            *positionPtr_ = *positionPtr_ + 3;
        }

        inline void wrapForEncode(char *buffer, const int count,     int *pos, const int actingVersion, const int bufferLength)
        {
            buffer_ = buffer;
            bufferLength_ = bufferLength;
            dimensions_.wrap(buffer_, *pos, actingVersion, bufferLength);
            dimensions_.blockLength((sbe_uint16_t)18);
            dimensions_.numInGroup((sbe_uint8_t)count);
            index_ = -1;
            count_ = count;
            blockLength_ = 18;
            actingVersion_ = actingVersion;
            positionPtr_ = pos;
            *positionPtr_ = *positionPtr_ + 3;
        }

        static const int sbeHeaderSize()
        {
            return 3;
        }

        static const int sbeBlockLength()
        {
            return 18;
        }

        inline int count(void) const
        {
            return count_;
        }

        inline bool hasNext(void) const
        {
            return index_ + 1 < count_;
        }

        inline NoLegs &next(void)
        {
            offset_ = *positionPtr_;
            if (SBE_BOUNDS_CHECK_EXPECT(( (offset_ + blockLength_) > bufferLength_ ), false))
            {
                throw std::runtime_error("buffer too short to support next group index [E108]");
            }
            *positionPtr_ = offset_ + blockLength_;
            ++index_;

            return *this;
        }

    #if __cplusplus < 201103L
        template<class Func>
        inline void forEach(Func& func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #else
        inline void forEach(std::function<void(NoLegs&)> func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #endif


        static const int legSecurityIDId(void)
        {
            return 602;
        }

        static const int legSecurityIDSinceVersion(void)
        {
             return 0;
        }

        bool legSecurityIDInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *LegSecurityIDMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        static const sbe_int32_t legSecurityIDNullValue()
        {
            return SBE_NULLVALUE_INT32;
        }

        static const sbe_int32_t legSecurityIDMinValue()
        {
            return -2147483647;
        }

        static const sbe_int32_t legSecurityIDMaxValue()
        {
            return 2147483647;
        }

        sbe_int32_t legSecurityID(void) const
        {
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_int32_t *)(buffer_ + offset_ + 0)));
        }

        NoLegs &legSecurityID(const sbe_int32_t value)
        {
            *((sbe_int32_t *)(buffer_ + offset_ + 0)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static const int legSecurityIDSourceId(void)
        {
            return 603;
        }

        static const int legSecurityIDSourceSinceVersion(void)
        {
             return 0;
        }

        bool legSecurityIDSourceInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *LegSecurityIDSourceMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "char";
            }

            return "";
        }

        static const sbe_char_t legSecurityIDSourceNullValue()
        {
            return (sbe_char_t)0;
        }

        static const sbe_char_t legSecurityIDSourceMinValue()
        {
            return (sbe_char_t)32;
        }

        static const sbe_char_t legSecurityIDSourceMaxValue()
        {
            return (sbe_char_t)126;
        }

        static const int legSecurityIDSourceLength(void)
        {
            return 1;
        }

        const char *legSecurityIDSource(void) const
        {
            static sbe_uint8_t legSecurityIDSourceValues[] = {56};

            return (const char *)legSecurityIDSourceValues;
        }

        sbe_char_t legSecurityIDSource(const int index) const
        {
            static sbe_uint8_t legSecurityIDSourceValues[] = {56};

            return legSecurityIDSourceValues[index];
        }

        int getLegSecurityIDSource(char *dst, const int length) const
        {
            static sbe_uint8_t legSecurityIDSourceValues[] = {56};
            int bytesToCopy = (length < (int)sizeof(legSecurityIDSourceValues)) ? length : (int)sizeof(legSecurityIDSourceValues);

            ::memcpy(dst, legSecurityIDSourceValues, bytesToCopy);
            return bytesToCopy;
        }

        static const int legSideId(void)
        {
            return 624;
        }

        static const int legSideSinceVersion(void)
        {
             return 0;
        }

        bool legSideInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *LegSideMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        LegSide::Value legSide(void) const
        {
            return LegSide::get((*((sbe_uint8_t *)(buffer_ + offset_ + 4))));
        }

        NoLegs &legSide(const LegSide::Value value)
        {
            *((sbe_uint8_t *)(buffer_ + offset_ + 4)) = (value);
            return *this;
        }

        static const int legRatioQtyId(void)
        {
            return 623;
        }

        static const int legRatioQtySinceVersion(void)
        {
             return 0;
        }

        bool legRatioQtyInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *LegRatioQtyMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "Qty";
            }

            return "";
        }

        static const sbe_int8_t legRatioQtyNullValue()
        {
            return SBE_NULLVALUE_INT8;
        }

        static const sbe_int8_t legRatioQtyMinValue()
        {
            return (sbe_int8_t)-127;
        }

        static const sbe_int8_t legRatioQtyMaxValue()
        {
            return (sbe_int8_t)127;
        }

        sbe_int8_t legRatioQty(void) const
        {
            return (*((sbe_int8_t *)(buffer_ + offset_ + 5)));
        }

        NoLegs &legRatioQty(const sbe_int8_t value)
        {
            *((sbe_int8_t *)(buffer_ + offset_ + 5)) = (value);
            return *this;
        }

        static const int legPriceId(void)
        {
            return 566;
        }

        static const int legPriceSinceVersion(void)
        {
             return 0;
        }

        bool legPriceInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *LegPriceMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "Price";
            }

            return "";
        }

private:
        PRICENULL legPrice_;

public:

        PRICENULL &legPrice(void)
        {
            legPrice_.wrap(buffer_, offset_ + 6, actingVersion_, bufferLength_);
            return legPrice_;
        }

        static const int legOptionDeltaId(void)
        {
            return 1017;
        }

        static const int legOptionDeltaSinceVersion(void)
        {
             return 0;
        }

        bool legOptionDeltaInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *LegOptionDeltaMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "Qty";
            }

            return "";
        }

private:
        DecimalQty legOptionDelta_;

public:

        DecimalQty &legOptionDelta(void)
        {
            legOptionDelta_.wrap(buffer_, offset_ + 14, actingVersion_, bufferLength_);
            return legOptionDelta_;
        }
    };

private:
    NoLegs noLegs_;

public:

    static const int NoLegsId(void)
    {
        return 555;
    }


    inline NoLegs &noLegs(void)
    {
        noLegs_.wrapForDecode(buffer_, positionPtr_, actingVersion_, bufferLength_);
        return noLegs_;
    }

    NoLegs &noLegsCount(const int count)
    {
        noLegs_.wrapForEncode(buffer_, count, positionPtr_, actingVersion_, bufferLength_);
        return noLegs_;
    }
};
}
#endif
