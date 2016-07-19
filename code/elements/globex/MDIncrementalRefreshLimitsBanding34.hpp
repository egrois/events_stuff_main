/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_MDINCREMENTALREFRESHLIMITSBANDING34_HPP_
#define _MKTDATA_MDINCREMENTALREFRESHLIMITSBANDING34_HPP_

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

class MDIncrementalRefreshLimitsBanding34
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

    MDIncrementalRefreshLimitsBanding34(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    MDIncrementalRefreshLimitsBanding34(char *buffer, const int bufferLength, const int actingBlockLength, const int actingVersion)
    {
        reset(buffer, 0, bufferLength, actingBlockLength, actingVersion);
    }

    MDIncrementalRefreshLimitsBanding34(const MDIncrementalRefreshLimitsBanding34& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
    }

#if __cplusplus >= 201103L
    MDIncrementalRefreshLimitsBanding34(MDIncrementalRefreshLimitsBanding34&& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
    }

    MDIncrementalRefreshLimitsBanding34& operator=(MDIncrementalRefreshLimitsBanding34&& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
        return *this;
    }

#endif

    MDIncrementalRefreshLimitsBanding34& operator=(const MDIncrementalRefreshLimitsBanding34& codec)
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
        return (sbe_uint16_t)34;
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

    MDIncrementalRefreshLimitsBanding34 &wrapForEncode(char *buffer, const int offset, const int bufferLength)
    {
        reset(buffer, offset, bufferLength, sbeBlockLength(), sbeSchemaVersion());
        return *this;
    }

    MDIncrementalRefreshLimitsBanding34 &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion, const int bufferLength)
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

    MDIncrementalRefreshLimitsBanding34 &transactTime(const sbe_uint64_t value)
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
            highLimitPrice_.wrap(buffer_, offset_ + 0, actingVersion_, bufferLength_);
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
            lowLimitPrice_.wrap(buffer_, offset_ + 8, actingVersion_, bufferLength_);
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
            maxPriceVariation_.wrap(buffer_, offset_ + 16, actingVersion_, bufferLength_);
            return maxPriceVariation_;
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
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_int32_t *)(buffer_ + offset_ + 24)));
        }

        NoMDEntries &securityID(const sbe_int32_t value)
        {
            *((sbe_int32_t *)(buffer_ + offset_ + 24)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
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
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_uint32_t *)(buffer_ + offset_ + 28)));
        }

        NoMDEntries &rptSeq(const sbe_uint32_t value)
        {
            *((sbe_uint32_t *)(buffer_ + offset_ + 28)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
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

        static const sbe_int8_t mDUpdateActionNullValue()
        {
            return SBE_NULLVALUE_INT8;
        }

        static const sbe_int8_t mDUpdateActionMinValue()
        {
            return (sbe_int8_t)-127;
        }

        static const sbe_int8_t mDUpdateActionMaxValue()
        {
            return (sbe_int8_t)127;
        }

        sbe_int8_t mDUpdateAction(void) const
        {
            return (sbe_int8_t)0;
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
            static sbe_uint8_t mDEntryTypeValues[] = {103};

            return (const char *)mDEntryTypeValues;
        }

        sbe_char_t mDEntryType(const int index) const
        {
            static sbe_uint8_t mDEntryTypeValues[] = {103};

            return mDEntryTypeValues[index];
        }

        int getMDEntryType(char *dst, const int length) const
        {
            static sbe_uint8_t mDEntryTypeValues[] = {103};
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
};
}
#endif
