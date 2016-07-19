/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_SNAPSHOTFULLREFRESH38_HPP_
#define _MKTDATA_SNAPSHOTFULLREFRESH38_HPP_

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

class SnapshotFullRefresh38
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

    SnapshotFullRefresh38(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    SnapshotFullRefresh38(char *buffer, const int bufferLength, const int actingBlockLength, const int actingVersion)
    {
        reset(buffer, 0, bufferLength, actingBlockLength, actingVersion);
    }

    SnapshotFullRefresh38(const SnapshotFullRefresh38& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
    }

#if __cplusplus >= 201103L
    SnapshotFullRefresh38(SnapshotFullRefresh38&& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
    }

    SnapshotFullRefresh38& operator=(SnapshotFullRefresh38&& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
        return *this;
    }

#endif

    SnapshotFullRefresh38& operator=(const SnapshotFullRefresh38& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
        return *this;
    }

    static const sbe_uint16_t sbeBlockLength(void)
    {
        return (sbe_uint16_t)59;
    }

    static const sbe_uint16_t sbeTemplateId(void)
    {
        return (sbe_uint16_t)38;
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
        return "W";
    }

    sbe_uint64_t offset(void) const
    {
        return offset_;
    }

    SnapshotFullRefresh38 &wrapForEncode(char *buffer, const int offset, const int bufferLength)
    {
        reset(buffer, offset, bufferLength, sbeBlockLength(), sbeSchemaVersion());
        return *this;
    }

    SnapshotFullRefresh38 &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion, const int bufferLength)
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

    static const int lastMsgSeqNumProcessedId(void)
    {
        return 369;
    }

    static const int lastMsgSeqNumProcessedSinceVersion(void)
    {
         return 0;
    }

    bool lastMsgSeqNumProcessedInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *LastMsgSeqNumProcessedMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "SeqNum";
        }

        return "";
    }

    static const sbe_uint32_t lastMsgSeqNumProcessedNullValue()
    {
        return SBE_NULLVALUE_UINT32;
    }

    static const sbe_uint32_t lastMsgSeqNumProcessedMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t lastMsgSeqNumProcessedMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t lastMsgSeqNumProcessed(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 0)));
    }

    SnapshotFullRefresh38 &lastMsgSeqNumProcessed(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 0)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
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
        return SBE_NULLVALUE_UINT32;
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
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 4)));
    }

    SnapshotFullRefresh38 &totNumReports(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 4)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
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
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_int32_t *)(buffer_ + offset_ + 8)));
    }

    SnapshotFullRefresh38 &securityID(const sbe_int32_t value)
    {
        *((sbe_int32_t *)(buffer_ + offset_ + 8)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const int rptSeqId(void)
    {
        return 83;
    }

    static const int rptSeqSinceVersion(void)
    {
         return 0;
    }

    bool rptSeqInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *RptSeqMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "SeqNum";
        }

        return "";
    }

    static const sbe_uint32_t rptSeqNullValue()
    {
        return SBE_NULLVALUE_UINT32;
    }

    static const sbe_uint32_t rptSeqMinValue()
    {
        return 0;
    }

    static const sbe_uint32_t rptSeqMaxValue()
    {
        return 4294967293;
    }

    sbe_uint32_t rptSeq(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 12)));
    }

    SnapshotFullRefresh38 &rptSeq(const sbe_uint32_t value)
    {
        *((sbe_uint32_t *)(buffer_ + offset_ + 12)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
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
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_uint64_t *)(buffer_ + offset_ + 16)));
    }

    SnapshotFullRefresh38 &transactTime(const sbe_uint64_t value)
    {
        *((sbe_uint64_t *)(buffer_ + offset_ + 16)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
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
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_uint64_t *)(buffer_ + offset_ + 24)));
    }

    SnapshotFullRefresh38 &lastUpdateTime(const sbe_uint64_t value)
    {
        *((sbe_uint64_t *)(buffer_ + offset_ + 24)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
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
        return SBE_LITTLE_ENDIAN_ENCODE_16(*((sbe_uint16_t *)(buffer_ + offset_ + 32)));
    }

    SnapshotFullRefresh38 &tradeDate(const sbe_uint16_t value)
    {
        *((sbe_uint16_t *)(buffer_ + offset_ + 32)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
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
        return SecurityTradingStatus::get((*((sbe_uint8_t *)(buffer_ + offset_ + 34))));
    }

    SnapshotFullRefresh38 &mDSecurityTradingStatus(const SecurityTradingStatus::Value value)
    {
        *((sbe_uint8_t *)(buffer_ + offset_ + 34)) = (value);
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
        highLimitPrice_.wrap(buffer_, offset_ + 35, actingVersion_, bufferLength_);
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
        lowLimitPrice_.wrap(buffer_, offset_ + 43, actingVersion_, bufferLength_);
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
        maxPriceVariation_.wrap(buffer_, offset_ + 51, actingVersion_, bufferLength_);
        return maxPriceVariation_;
    }

    class NoMDEntries
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
            dimensions_.blockLength((sbe_uint16_t)22);
            dimensions_.numInGroup((sbe_uint8_t)count);
            index_ = -1;
            count_ = count;
            blockLength_ = 22;
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
            return 22;
        }

        inline int count(void) const
        {
            return count_;
        }

        inline bool hasNext(void) const
        {
            return index_ + 1 < count_;
        }

        inline NoMDEntries &next(void)
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
        inline void forEach(std::function<void(NoMDEntries&)> func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #endif


        static const int mDEntryPxId(void)
        {
            return 270;
        }

        static const int mDEntryPxSinceVersion(void)
        {
             return 0;
        }

        bool mDEntryPxInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *MDEntryPxMetaAttribute(const MetaAttribute::Attribute metaAttribute)
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
        PRICENULL mDEntryPx_;

public:

        PRICENULL &mDEntryPx(void)
        {
            mDEntryPx_.wrap(buffer_, offset_ + 0, actingVersion_, bufferLength_);
            return mDEntryPx_;
        }

        static const int mDEntrySizeId(void)
        {
            return 271;
        }

        static const int mDEntrySizeSinceVersion(void)
        {
             return 0;
        }

        bool mDEntrySizeInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *MDEntrySizeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "Qty";
            }

            return "";
        }

        static const sbe_int32_t mDEntrySizeNullValue()
        {
            return 2147483647;
        }

        static const sbe_int32_t mDEntrySizeMinValue()
        {
            return -2147483647;
        }

        static const sbe_int32_t mDEntrySizeMaxValue()
        {
            return 2147483647;
        }

        sbe_int32_t mDEntrySize(void) const
        {
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_int32_t *)(buffer_ + offset_ + 8)));
        }

        NoMDEntries &mDEntrySize(const sbe_int32_t value)
        {
            *((sbe_int32_t *)(buffer_ + offset_ + 8)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static const int numberOfOrdersId(void)
        {
            return 346;
        }

        static const int numberOfOrdersSinceVersion(void)
        {
             return 0;
        }

        bool numberOfOrdersInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *NumberOfOrdersMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        static const sbe_int32_t numberOfOrdersNullValue()
        {
            return 2147483647;
        }

        static const sbe_int32_t numberOfOrdersMinValue()
        {
            return -2147483647;
        }

        static const sbe_int32_t numberOfOrdersMaxValue()
        {
            return 2147483647;
        }

        sbe_int32_t numberOfOrders(void) const
        {
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_int32_t *)(buffer_ + offset_ + 12)));
        }

        NoMDEntries &numberOfOrders(const sbe_int32_t value)
        {
            *((sbe_int32_t *)(buffer_ + offset_ + 12)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static const int mDPriceLevelId(void)
        {
            return 1023;
        }

        static const int mDPriceLevelSinceVersion(void)
        {
             return 0;
        }

        bool mDPriceLevelInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *MDPriceLevelMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        static const sbe_int8_t mDPriceLevelNullValue()
        {
            return (sbe_int8_t)127;
        }

        static const sbe_int8_t mDPriceLevelMinValue()
        {
            return (sbe_int8_t)-127;
        }

        static const sbe_int8_t mDPriceLevelMaxValue()
        {
            return (sbe_int8_t)127;
        }

        sbe_int8_t mDPriceLevel(void) const
        {
            return (*((sbe_int8_t *)(buffer_ + offset_ + 16)));
        }

        NoMDEntries &mDPriceLevel(const sbe_int8_t value)
        {
            *((sbe_int8_t *)(buffer_ + offset_ + 16)) = (value);
            return *this;
        }

        static const int tradingReferenceDateId(void)
        {
            return 5796;
        }

        static const int tradingReferenceDateSinceVersion(void)
        {
             return 0;
        }

        bool tradingReferenceDateInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *TradingReferenceDateMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "LocalMktDate";
            }

            return "";
        }

        static const sbe_uint16_t tradingReferenceDateNullValue()
        {
            return (sbe_uint16_t)65535;
        }

        static const sbe_uint16_t tradingReferenceDateMinValue()
        {
            return (sbe_uint16_t)0;
        }

        static const sbe_uint16_t tradingReferenceDateMaxValue()
        {
            return (sbe_uint16_t)65534;
        }

        sbe_uint16_t tradingReferenceDate(void) const
        {
            return SBE_LITTLE_ENDIAN_ENCODE_16(*((sbe_uint16_t *)(buffer_ + offset_ + 17)));
        }

        NoMDEntries &tradingReferenceDate(const sbe_uint16_t value)
        {
            *((sbe_uint16_t *)(buffer_ + offset_ + 17)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
            return *this;
        }

        static const int openCloseSettlFlagId(void)
        {
            return 286;
        }

        static const int openCloseSettlFlagSinceVersion(void)
        {
             return 0;
        }

        bool openCloseSettlFlagInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *OpenCloseSettlFlagMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        OpenCloseSettlFlag::Value openCloseSettlFlag(void) const
        {
            return OpenCloseSettlFlag::get((*((sbe_uint8_t *)(buffer_ + offset_ + 19))));
        }

        NoMDEntries &openCloseSettlFlag(const OpenCloseSettlFlag::Value value)
        {
            *((sbe_uint8_t *)(buffer_ + offset_ + 19)) = (value);
            return *this;
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
            settlPriceType_.wrap(buffer_, offset_ + 20, actingVersion_, bufferLength_);
            return settlPriceType_;
        }

        static const int mDEntryTypeId(void)
        {
            return 269;
        }

        static const int mDEntryTypeSinceVersion(void)
        {
             return 0;
        }

        bool mDEntryTypeInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *MDEntryTypeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "char";
            }

            return "";
        }

        MDEntryType::Value mDEntryType(void) const
        {
            return MDEntryType::get((*((sbe_char_t *)(buffer_ + offset_ + 21))));
        }

        NoMDEntries &mDEntryType(const MDEntryType::Value value)
        {
            *((sbe_char_t *)(buffer_ + offset_ + 21)) = (value);
            return *this;
        }
    };

private:
    NoMDEntries noMDEntries_;

public:

    static const int NoMDEntriesId(void)
    {
        return 268;
    }


    inline NoMDEntries &noMDEntries(void)
    {
        noMDEntries_.wrapForDecode(buffer_, positionPtr_, actingVersion_, bufferLength_);
        return noMDEntries_;
    }

    NoMDEntries &noMDEntriesCount(const int count)
    {
        noMDEntries_.wrapForEncode(buffer_, count, positionPtr_, actingVersion_, bufferLength_);
        return noMDEntries_;
    }
};
}
#endif
