/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_MDINCREMENTALREFRESHTRADESUMMARY42_HPP_
#define _MKTDATA_MDINCREMENTALREFRESHTRADESUMMARY42_HPP_

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

class MDIncrementalRefreshTradeSummary42
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

    MDIncrementalRefreshTradeSummary42(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    MDIncrementalRefreshTradeSummary42(char *buffer, const int bufferLength, const int actingBlockLength, const int actingVersion)
    {
        reset(buffer, 0, bufferLength, actingBlockLength, actingVersion);
    }

    MDIncrementalRefreshTradeSummary42(const MDIncrementalRefreshTradeSummary42& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
    }

#if __cplusplus >= 201103L
    MDIncrementalRefreshTradeSummary42(MDIncrementalRefreshTradeSummary42&& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
    }

    MDIncrementalRefreshTradeSummary42& operator=(MDIncrementalRefreshTradeSummary42&& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
        return *this;
    }

#endif

    MDIncrementalRefreshTradeSummary42& operator=(const MDIncrementalRefreshTradeSummary42& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
        return *this;
    }

    static const sbe_uint16_t sbeBlockLength(void)
    {
        return (sbe_uint16_t)11;
    }

    static const sbe_uint16_t sbeTemplateId(void)
    {
        return (sbe_uint16_t)42;
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
        return "X";
    }

    sbe_uint64_t offset(void) const
    {
        return offset_;
    }

    MDIncrementalRefreshTradeSummary42 &wrapForEncode(char *buffer, const int offset, const int bufferLength)
    {
        reset(buffer, offset, bufferLength, sbeBlockLength(), sbeSchemaVersion());
        return *this;
    }

    MDIncrementalRefreshTradeSummary42 &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion, const int bufferLength)
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

    MDIncrementalRefreshTradeSummary42 &transactTime(const sbe_uint64_t value)
    {
        *((sbe_uint64_t *)(buffer_ + offset_ + 0)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
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
        matchEventIndicator_.wrap(buffer_, offset_ + 8, actingVersion_, bufferLength_);
        return matchEventIndicator_;
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
            dimensions_.blockLength((sbe_uint16_t)32);
            dimensions_.numInGroup((sbe_uint8_t)count);
            index_ = -1;
            count_ = count;
            blockLength_ = 32;
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
            return 32;
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
        PRICE mDEntryPx_;

public:

        PRICE &mDEntryPx(void)
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
            return SBE_NULLVALUE_INT32;
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
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_int32_t *)(buffer_ + offset_ + 12)));
        }

        NoMDEntries &securityID(const sbe_int32_t value)
        {
            *((sbe_int32_t *)(buffer_ + offset_ + 12)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
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
                case MetaAttribute::SEMANTIC_TYPE: return "int";
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
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 16)));
        }

        NoMDEntries &rptSeq(const sbe_uint32_t value)
        {
            *((sbe_uint32_t *)(buffer_ + offset_ + 16)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
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
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_int32_t *)(buffer_ + offset_ + 20)));
        }

        NoMDEntries &numberOfOrders(const sbe_int32_t value)
        {
            *((sbe_int32_t *)(buffer_ + offset_ + 20)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static const int aggressorSideId(void)
        {
            return 5797;
        }

        static const int aggressorSideSinceVersion(void)
        {
             return 0;
        }

        bool aggressorSideInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *AggressorSideMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        AggressorSide::Value aggressorSide(void) const
        {
            return AggressorSide::get((*((sbe_uint8_t *)(buffer_ + offset_ + 24))));
        }

        NoMDEntries &aggressorSide(const AggressorSide::Value value)
        {
            *((sbe_uint8_t *)(buffer_ + offset_ + 24)) = (value);
            return *this;
        }

        static const int mDUpdateActionId(void)
        {
            return 279;
        }

        static const int mDUpdateActionSinceVersion(void)
        {
             return 0;
        }

        bool mDUpdateActionInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *MDUpdateActionMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        MDUpdateAction::Value mDUpdateAction(void) const
        {
            return MDUpdateAction::get((*((sbe_uint8_t *)(buffer_ + offset_ + 25))));
        }

        NoMDEntries &mDUpdateAction(const MDUpdateAction::Value value)
        {
            *((sbe_uint8_t *)(buffer_ + offset_ + 25)) = (value);
            return *this;
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

        static const sbe_char_t mDEntryTypeNullValue()
        {
            return (sbe_char_t)0;
        }

        static const sbe_char_t mDEntryTypeMinValue()
        {
            return (sbe_char_t)32;
        }

        static const sbe_char_t mDEntryTypeMaxValue()
        {
            return (sbe_char_t)126;
        }

        static const int mDEntryTypeLength(void)
        {
            return 1;
        }

        const char *mDEntryType(void) const
        {
            static sbe_uint8_t mDEntryTypeValues[] = {50};

            return (const char *)mDEntryTypeValues;
        }

        sbe_char_t mDEntryType(const int index) const
        {
            static sbe_uint8_t mDEntryTypeValues[] = {50};

            return mDEntryTypeValues[index];
        }

        int getMDEntryType(char *dst, const int length) const
        {
            static sbe_uint8_t mDEntryTypeValues[] = {50};
            int bytesToCopy = (length < (int)sizeof(mDEntryTypeValues)) ? length : (int)sizeof(mDEntryTypeValues);

            ::memcpy(dst, mDEntryTypeValues, bytesToCopy);
            return bytesToCopy;
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

    class NoOrderIDEntries
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
        GroupSize8Byte dimensions_;

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
            *positionPtr_ = *positionPtr_ + 8;
        }

        inline void wrapForEncode(char *buffer, const int count,     int *pos, const int actingVersion, const int bufferLength)
        {
            buffer_ = buffer;
            bufferLength_ = bufferLength;
            dimensions_.wrap(buffer_, *pos, actingVersion, bufferLength);
            dimensions_.blockLength((sbe_uint16_t)16);
            dimensions_.numInGroup((sbe_uint8_t)count);
            index_ = -1;
            count_ = count;
            blockLength_ = 16;
            actingVersion_ = actingVersion;
            positionPtr_ = pos;
            *positionPtr_ = *positionPtr_ + 8;
        }

        static const int sbeHeaderSize()
        {
            return 8;
        }

        static const int sbeBlockLength()
        {
            return 16;
        }

        inline int count(void) const
        {
            return count_;
        }

        inline bool hasNext(void) const
        {
            return index_ + 1 < count_;
        }

        inline NoOrderIDEntries &next(void)
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
        inline void forEach(std::function<void(NoOrderIDEntries&)> func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #endif


        static const int orderIDId(void)
        {
            return 37;
        }

        static const int orderIDSinceVersion(void)
        {
             return 0;
        }

        bool orderIDInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *OrderIDMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        static const sbe_uint64_t orderIDNullValue()
        {
            return SBE_NULLVALUE_UINT64;
        }

        static const sbe_uint64_t orderIDMinValue()
        {
            return 0x0L;
        }

        static const sbe_uint64_t orderIDMaxValue()
        {
            return 0xfffffffffffffffeL;
        }

        sbe_uint64_t orderID(void) const
        {
            return SBE_LITTLE_ENDIAN_ENCODE_64(*((sbe_uint64_t *)(buffer_ + offset_ + 0)));
        }

        NoOrderIDEntries &orderID(const sbe_uint64_t value)
        {
            *((sbe_uint64_t *)(buffer_ + offset_ + 0)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
            return *this;
        }

        static const int lastQtyId(void)
        {
            return 32;
        }

        static const int lastQtySinceVersion(void)
        {
             return 0;
        }

        bool lastQtyInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *LastQtyMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "Qty";
            }

            return "";
        }

        static const sbe_int32_t lastQtyNullValue()
        {
            return SBE_NULLVALUE_INT32;
        }

        static const sbe_int32_t lastQtyMinValue()
        {
            return -2147483647;
        }

        static const sbe_int32_t lastQtyMaxValue()
        {
            return 2147483647;
        }

        sbe_int32_t lastQty(void) const
        {
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_int32_t *)(buffer_ + offset_ + 8)));
        }

        NoOrderIDEntries &lastQty(const sbe_int32_t value)
        {
            *((sbe_int32_t *)(buffer_ + offset_ + 8)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
            return *this;
        }
    };

private:
    NoOrderIDEntries noOrderIDEntries_;

public:

    static const int NoOrderIDEntriesId(void)
    {
        return 37705;
    }


    inline NoOrderIDEntries &noOrderIDEntries(void)
    {
        noOrderIDEntries_.wrapForDecode(buffer_, positionPtr_, actingVersion_, bufferLength_);
        return noOrderIDEntries_;
    }

    NoOrderIDEntries &noOrderIDEntriesCount(const int count)
    {
        noOrderIDEntries_.wrapForEncode(buffer_, count, positionPtr_, actingVersion_, bufferLength_);
        return noOrderIDEntries_;
    }
};
}
#endif
