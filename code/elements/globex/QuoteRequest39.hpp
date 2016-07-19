/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_QUOTEREQUEST39_HPP_
#define _MKTDATA_QUOTEREQUEST39_HPP_

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

class QuoteRequest39
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

    QuoteRequest39(void) : buffer_(NULL), bufferLength_(0), offset_(0) {}

    QuoteRequest39(char *buffer, const int bufferLength, const int actingBlockLength, const int actingVersion)
    {
        reset(buffer, 0, bufferLength, actingBlockLength, actingVersion);
    }

    QuoteRequest39(const QuoteRequest39& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
    }

#if __cplusplus >= 201103L
    QuoteRequest39(QuoteRequest39&& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
    }

    QuoteRequest39& operator=(QuoteRequest39&& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
        return *this;
    }

#endif

    QuoteRequest39& operator=(const QuoteRequest39& codec)
    {
        reset(codec.buffer_, codec.offset_, codec.bufferLength_, codec.actingBlockLength_, codec.actingVersion_);
        return *this;
    }

    static const sbe_uint16_t sbeBlockLength(void)
    {
        return (sbe_uint16_t)35;
    }

    static const sbe_uint16_t sbeTemplateId(void)
    {
        return (sbe_uint16_t)39;
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
        return "R";
    }

    sbe_uint64_t offset(void) const
    {
        return offset_;
    }

    QuoteRequest39 &wrapForEncode(char *buffer, const int offset, const int bufferLength)
    {
        reset(buffer, offset, bufferLength, sbeBlockLength(), sbeSchemaVersion());
        return *this;
    }

    QuoteRequest39 &wrapForDecode(char *buffer, const int offset, const int actingBlockLength, const int actingVersion, const int bufferLength)
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

    QuoteRequest39 &transactTime(const sbe_uint64_t value)
    {
        *((sbe_uint64_t *)(buffer_ + offset_ + 0)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const int quoteReqIDId(void)
    {
        return 131;
    }

    static const int quoteReqIDSinceVersion(void)
    {
         return 0;
    }

    bool quoteReqIDInActingVersion(void)
    {
        return (actingVersion_ >= 0) ? true : false;
    }


    static const char *QuoteReqIDMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "String";
        }

        return "";
    }

    static const sbe_char_t quoteReqIDNullValue()
    {
        return (sbe_char_t)0;
    }

    static const sbe_char_t quoteReqIDMinValue()
    {
        return (sbe_char_t)32;
    }

    static const sbe_char_t quoteReqIDMaxValue()
    {
        return (sbe_char_t)126;
    }

    static const int quoteReqIDLength(void)
    {
        return 23;
    }

    const char *quoteReqID(void) const
    {
        return (buffer_ + offset_ + 8);
    }

    sbe_char_t quoteReqID(const int index) const
    {
        if (index < 0 || index >= 23)
        {
            throw std::runtime_error("index out of range for quoteReqID [E104]");
        }

        return (*((sbe_char_t *)(buffer_ + offset_ + 8 + (index * 1))));
    }

    void quoteReqID(const int index, const sbe_char_t value)
    {
        if (index < 0 || index >= 23)
        {
            throw std::runtime_error("index out of range for quoteReqID [E105]");
        }

        *((sbe_char_t *)(buffer_ + offset_ + 8 + (index * 1))) = (value);
    }

    int getQuoteReqID(char *dst, const int length) const
    {
        if (length > 23)
        {
             throw std::runtime_error("length too large for getQuoteReqID [E106]");
        }

        ::memcpy(dst, buffer_ + offset_ + 8, length);
        return length;
    }

    QuoteRequest39 &putQuoteReqID(const char *src)
    {
        ::memcpy(buffer_ + offset_ + 8, src, 23);
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
        matchEventIndicator_.wrap(buffer_, offset_ + 31, actingVersion_, bufferLength_);
        return matchEventIndicator_;
    }

    class NoRelatedSym
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

        inline NoRelatedSym &next(void)
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
        inline void forEach(std::function<void(NoRelatedSym&)> func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #endif


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
            return (buffer_ + offset_ + 0);
        }

        sbe_char_t symbol(const int index) const
        {
            if (index < 0 || index >= 20)
            {
                throw std::runtime_error("index out of range for symbol [E104]");
            }

            return (*((sbe_char_t *)(buffer_ + offset_ + 0 + (index * 1))));
        }

        void symbol(const int index, const sbe_char_t value)
        {
            if (index < 0 || index >= 20)
            {
                throw std::runtime_error("index out of range for symbol [E105]");
            }

            *((sbe_char_t *)(buffer_ + offset_ + 0 + (index * 1))) = (value);
        }

        int getSymbol(char *dst, const int length) const
        {
            if (length > 20)
            {
                 throw std::runtime_error("length too large for getSymbol [E106]");
            }

            ::memcpy(dst, buffer_ + offset_ + 0, length);
            return length;
        }

        NoRelatedSym &putSymbol(const char *src)
        {
            ::memcpy(buffer_ + offset_ + 0, src, 20);
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
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_int32_t *)(buffer_ + offset_ + 20)));
        }

        NoRelatedSym &securityID(const sbe_int32_t value)
        {
            *((sbe_int32_t *)(buffer_ + offset_ + 20)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static const int orderQtyId(void)
        {
            return 38;
        }

        static const int orderQtySinceVersion(void)
        {
             return 0;
        }

        bool orderQtyInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *OrderQtyMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "Qty";
            }

            return "";
        }

        static const sbe_int32_t orderQtyNullValue()
        {
            return 2147483647;
        }

        static const sbe_int32_t orderQtyMinValue()
        {
            return -2147483647;
        }

        static const sbe_int32_t orderQtyMaxValue()
        {
            return 2147483647;
        }

        sbe_int32_t orderQty(void) const
        {
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((sbe_int32_t *)(buffer_ + offset_ + 24)));
        }

        NoRelatedSym &orderQty(const sbe_int32_t value)
        {
            *((sbe_int32_t *)(buffer_ + offset_ + 24)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static const int quoteTypeId(void)
        {
            return 537;
        }

        static const int quoteTypeSinceVersion(void)
        {
             return 0;
        }

        bool quoteTypeInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *QuoteTypeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        static const sbe_int8_t quoteTypeNullValue()
        {
            return SBE_NULLVALUE_INT8;
        }

        static const sbe_int8_t quoteTypeMinValue()
        {
            return (sbe_int8_t)-127;
        }

        static const sbe_int8_t quoteTypeMaxValue()
        {
            return (sbe_int8_t)127;
        }

        sbe_int8_t quoteType(void) const
        {
            return (*((sbe_int8_t *)(buffer_ + offset_ + 28)));
        }

        NoRelatedSym &quoteType(const sbe_int8_t value)
        {
            *((sbe_int8_t *)(buffer_ + offset_ + 28)) = (value);
            return *this;
        }

        static const int sideId(void)
        {
            return 54;
        }

        static const int sideSinceVersion(void)
        {
             return 0;
        }

        bool sideInActingVersion(void)
        {
            return (actingVersion_ >= 0) ? true : false;
        }


        static const char *SideMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        static const sbe_int8_t sideNullValue()
        {
            return (sbe_int8_t)127;
        }

        static const sbe_int8_t sideMinValue()
        {
            return (sbe_int8_t)-127;
        }

        static const sbe_int8_t sideMaxValue()
        {
            return (sbe_int8_t)127;
        }

        sbe_int8_t side(void) const
        {
            return (*((sbe_int8_t *)(buffer_ + offset_ + 29)));
        }

        NoRelatedSym &side(const sbe_int8_t value)
        {
            *((sbe_int8_t *)(buffer_ + offset_ + 29)) = (value);
            return *this;
        }
    };

private:
    NoRelatedSym noRelatedSym_;

public:

    static const int NoRelatedSymId(void)
    {
        return 146;
    }


    inline NoRelatedSym &noRelatedSym(void)
    {
        noRelatedSym_.wrapForDecode(buffer_, positionPtr_, actingVersion_, bufferLength_);
        return noRelatedSym_;
    }

    NoRelatedSym &noRelatedSymCount(const int count)
    {
        noRelatedSym_.wrapForEncode(buffer_, count, positionPtr_, actingVersion_, bufferLength_);
        return noRelatedSym_;
    }
};
}
#endif
