#ifndef ELEMENTS_SBE_CME_PARSER_H
#define ELEMENTS_SBE_CME_PARSER_H

#include <iostream>
#include <string>
#include <md.h>

#include "globex/AdminHeartbeat12.hpp"
#include "globex/AdminLogin15.hpp"
#include "globex/AdminLogout16.hpp"
#include "globex/AggressorSide.hpp"
#include "globex/ChannelReset4.hpp"
#include "globex/DecimalQty.hpp"
#include "globex/EventType.hpp"
#include "globex/FLOAT.hpp"
#include "globex/GroupSize.hpp"
#include "globex/GroupSize8Byte.hpp"
#include "globex/HaltReason.hpp"
#include "globex/InstAttribValue.hpp"
#include "globex/LegSide.hpp"
#include "globex/MDEntryType.hpp"
#include "globex/MDEntryTypeBook.hpp"
#include "globex/MDEntryTypeDailyStatistics.hpp"
#include "globex/MDEntryTypeStatistics.hpp"
#include "globex/MDIncrementalRefreshBook32.hpp"
#include "globex/MDIncrementalRefreshDailyStatistics33.hpp"
#include "globex/MDIncrementalRefreshLimitsBanding34.hpp"
#include "globex/MDIncrementalRefreshSessionStatistics35.hpp"
#include "globex/MDIncrementalRefreshTrade36.hpp"
#include "globex/MDIncrementalRefreshTradeSummary42.hpp"
#include "globex/MDIncrementalRefreshVolume37.hpp"
#include "globex/MDInstrumentDefinitionFuture27.hpp"
#include "globex/MDInstrumentDefinitionOption41.hpp"
#include "globex/MDInstrumentDefinitionSpread29.hpp"
#include "globex/MDUpdateAction.hpp"
#include "globex/MatchEventIndicator.hpp"
#include "globex/MaturityMonthYear.hpp"
#include "globex/MessageHeader.hpp"
#include "globex/OpenCloseSettlFlag.hpp"
#include "globex/PRICE.hpp"
#include "globex/PRICENULL.hpp"
#include "globex/PutOrCall.hpp"
#include "globex/QuoteRequest39.hpp"
#include "globex/SecurityStatus30.hpp"
#include "globex/SecurityTradingEvent.hpp"
#include "globex/SecurityTradingStatus.hpp"
#include "globex/SecurityUpdateAction.hpp"
#include "globex/SettlPriceType.hpp"
#include "globex/SnapshotFullRefresh38.hpp"

#include <global_price_multiplier.h>
#include <iomanip>

namespace elements
{

struct BinaryPacketHeader
{
    uint32_t msgSeqNum;
    uint64_t sendingTime;       
} __attribute__ ((packed)); 

struct BinaryMessageHeader
{
    uint16_t msgSize;
    uint16_t blockLength;
    uint16_t templateID;
    uint16_t schemaID;
    uint16_t version;
} __attribute__ ((packed)); 


template <typename Handler>
class cme_sbe_parser
{
public:

    cme_sbe_parser(Handler &handler) : _handler(handler)
    {
    }
    
    void on_start_of_packet( BinaryPacketHeader const & hdr)
    {
        if(  _sequence_number > 0 && (int64_t)hdr.msgSeqNum - _sequence_number > 1 )
            std::cout << "MSG_GAP: " << _sequence_number << " " << hdr.msgSeqNum << std::endl;
        if(_sequence_number <  hdr.msgSeqNum)
            _sequence_number = (int64_t)hdr.msgSeqNum;
        //else
        //{
        //    std::cout << "OUT_OF_SEQUENCE " << std::endl;
        //}
    }
 
    void handle(globex::MDIncrementalRefreshBook32 & msg)
    {
        //TODO std::string                     TradeDate;
        _incremental_refresh.MsgSeqNum = (int64_t)_sequence_number; 
        _incremental_refresh.SendingTime = from_epoch_nanos(msg.transactTime()); 
        _incremental_refresh.ReceivedTime = _rcvd_time;
        auto entries = msg.noMDEntries();
        _incremental_refresh.MDEntries.resize(entries.count());
        _incremental_refresh.OrderIDEntries.resize(0);
        int i = 0;
        while(entries.hasNext() && i < entries.count())
        {
            entries.next();
            cme::MDEntry *e = &_incremental_refresh.MDEntries[i];
            e->reset();

            switch(entries.mDUpdateAction())
            {
            case globex::MDUpdateAction::Value::New:
                e->MDUpdateAction = cme::MDUpdateActionEnum::NEW;        
                break;
            case globex::MDUpdateAction::Value::Change:
                e->MDUpdateAction = cme::MDUpdateActionEnum::CHANGE;        
                break;
            case globex::MDUpdateAction::Value::Delete:
                e->MDUpdateAction = cme::MDUpdateActionEnum::DELETE;        
                break;
            case globex::MDUpdateAction::Value::Overlay:
                e->MDUpdateAction = cme::MDUpdateActionEnum::OVERLAY;        
                break;
            case globex::MDUpdateAction::Value::DeleteThru:
            case globex::MDUpdateAction::Value::DeleteFrom:
            case globex::MDUpdateAction::Value::NULL_VALUE:
            default:
                 throw std::runtime_error("Unhandled update action"); 
            };
            e->has_MDUpdateAction = true;
       
            e->MDPriceLevel = (int32_t)entries.mDPriceLevel();
            e->has_MDPriceLevel = true;

            switch(entries.mDEntryType())
            {
            case globex::MDEntryTypeBook::Value::Bid:
                e->MDEntryType = cme::MDEntryTypeEnum::BID;
                e->QuoteCondition = cme::QuoteConditionEnum::NORMAL;
                break;
            case globex::MDEntryTypeBook::Value::Offer:
                e->MDEntryType = cme::MDEntryTypeEnum::OFFER;
                e->QuoteCondition = cme::QuoteConditionEnum::NORMAL;
                break;
            case globex::MDEntryTypeBook::Value::ImpliedBid:
                e->MDEntryType = cme::MDEntryTypeEnum::BID;
                e->QuoteCondition = cme::QuoteConditionEnum::IMPLIED;
                break;
            case globex::MDEntryTypeBook::Value::ImpliedOffer:
                e->MDEntryType = cme::MDEntryTypeEnum::OFFER;
                e->QuoteCondition = cme::QuoteConditionEnum::IMPLIED;
                break;
            case globex::MDEntryTypeBook::Value::BookReset:
            case globex::MDEntryTypeBook::Value::NULL_VALUE:
            default:    
                throw std::runtime_error("Unhandled entry type"); 
            };
            e->has_MDEntryType = true;
            e->has_QuoteCondition = true;

            e->SecurityID = entries.securityID();
            e->has_SecurityID = true;
            
            e->RptSeq = (int32_t)entries.rptSeq();
            e->has_RptSeq = true;

            auto px = entries.mDEntryPx();
            assert(px.exponent() == -7); 
            e->MDEntryPx = px.mantissa(); // assume exponent -7 
            e->has_MDEntryPx = true;

            if(entries.numberOfOrders() != entries.numberOfOrdersNullValue())
            {
                e->NumberOfOrders =  (int32_t)entries.numberOfOrders();
                e->has_NumberOfOrders = true;
            }
            else
            {
                e->NumberOfOrders = 0;
                e->has_NumberOfOrders = true;
            }

            e->MDEntrySize = (int32_t)entries.mDEntrySize();
            e->has_MDEntrySize = true;

            ++i;
        }

        _handler.on_incremental_refresh_raw(_incremental_refresh);   
    }

    void handle(globex::MDIncrementalRefreshTradeSummary42 &msg)
    {
        //TODO std::string                     TradeDate;
        _incremental_refresh.MsgSeqNum = (int64_t)_sequence_number; 
        _incremental_refresh.SendingTime = from_epoch_nanos(msg.transactTime()); 
        _incremental_refresh.ReceivedTime = _rcvd_time;
        auto entries = msg.noMDEntries();
        _incremental_refresh.MDEntries.resize(entries.count());
    
        int i = 0;
        while(entries.hasNext() && i < entries.count())
        {
            entries.next();
            cme::MDEntry *e = &_incremental_refresh.MDEntries[i];
            e->reset();
            
            e->MDEntryType = cme::MDEntryTypeEnum::TRADE;
            e->has_MDEntryType = true;
            
            switch(entries.mDUpdateAction())
            {
            case globex::MDUpdateAction::Value::New:
                e->MDUpdateAction = cme::MDUpdateActionEnum::NEW;        
                break;
            case globex::MDUpdateAction::Value::Change:
                e->MDUpdateAction = cme::MDUpdateActionEnum::CHANGE;        
                break;
            case globex::MDUpdateAction::Value::Delete:
                e->MDUpdateAction = cme::MDUpdateActionEnum::DELETE;        
                break;
            case globex::MDUpdateAction::Value::Overlay:
                e->MDUpdateAction = cme::MDUpdateActionEnum::OVERLAY;        
                break;
            case globex::MDUpdateAction::Value::DeleteThru:
            case globex::MDUpdateAction::Value::DeleteFrom:
            case globex::MDUpdateAction::Value::NULL_VALUE:
            default:
                throw std::runtime_error("Unhandled update action"); 
            };

            e->has_MDUpdateAction = true;

            e->SecurityID = entries.securityID();
            e->has_SecurityID = true;
            
            e->RptSeq = (int32_t)entries.rptSeq();
            e->has_RptSeq = true;
            
            auto px = entries.mDEntryPx();
            assert(px.exponent() == -7); 
            e->MDEntryPx = px.mantissa(); // assume exponent -7 
            e->has_MDEntryPx = true;
           
            e->NumberOfOrders =  (int32_t)entries.numberOfOrders();
            e->has_NumberOfOrders = true;            
            
            e->MDEntrySize = (int32_t)entries.mDEntrySize();
            e->has_MDEntrySize = true;
           
            switch(entries.aggressorSide())
            {
            case globex::AggressorSide::Value::Buy:
                e->AggressorSide = cme::AggressorSideEnum::BUY; 
                e->has_AggressorSide = true;
                break;
            case globex::AggressorSide::Value::Sell:
                e->AggressorSide = cme::AggressorSideEnum::SELL; 
                e->has_AggressorSide = true;
                break;
            case globex::AggressorSide::Value::NoAggressor:
                e->AggressorSide = cme::AggressorSideEnum::NONE; 
                e->has_AggressorSide = true;
                break;
            case globex::AggressorSide::Value::NULL_VALUE:
            default:
                e->AggressorSide = cme::AggressorSideEnum::NONE; 
                e->has_AggressorSide = true;
                break;
            };
            ++i;
        }
        auto entries2 = msg.noOrderIDEntries();
        _incremental_refresh.OrderIDEntries.resize(entries2.count());
    
        i = 0;
        while(entries2.hasNext() && i < entries2.count())
        {
            entries2.next();
            cme::OrderIDEntry *e = &_incremental_refresh.OrderIDEntries[i];
            e->reset();

            e->OrderID = entries2.orderID();
            e->has_OrderID = true;
            
            e->LastQty = entries2.lastQty();
            e->has_LastQty = true;
            
            ++i;
        }

        _handler.on_incremental_refresh_raw(_incremental_refresh);   
    }

    void handle(globex::SecurityStatus30 &msg)
    {
        _security_status.reset();
        _security_status.MsgSeqNum = (int64_t)_sequence_number; 
        _security_status.has_MsgSeqNum = true;
        
        _security_status.SendingTime = from_epoch_nanos(msg.transactTime()); 
        _security_status.has_SendingTime = true;

        _security_status.ReceivedTime = _rcvd_time;
        _security_status.has_ReceivedTime = true;

        if(msg.securityGroup()[0] != msg.securityGroupNullValue())
        {
            _security_status.SecurityGroup = msg.securityGroup();
            _security_status.has_SecurityGroup= true;
        }

        if(msg.asset()[0] != msg.assetNullValue())
        {
            _security_status.Asset = msg.asset();
            _security_status.has_Asset = true;
        }

        uint8_t mei = 0;
        mei = msg.matchEventIndicator().LastTradeMsg() ? ( mei | 0x1 ) : mei;
        mei = msg.matchEventIndicator().LastVolumeMsg() ? ( mei | 0x2 ) : mei;
        mei = msg.matchEventIndicator().LastQuoteMsg() ? ( mei | 0x4) : mei;
        mei = msg.matchEventIndicator().LastStatsMsg() ? ( mei | 0x8 ) : mei;
        mei = msg.matchEventIndicator().LastImpliedMsg() ? ( mei | 0x10 ) : mei;
        mei = msg.matchEventIndicator().RecoveryMsg() ? ( mei | 0x20 ) : mei;
        mei = msg.matchEventIndicator().Reserved() ? ( mei | 0x40) : mei;
        mei = msg.matchEventIndicator().EndOfEvent() ? ( mei | 0x80 ) : mei;

        std::stringstream ss;
        ss << "0x" << std::hex << (int)mei;
        _security_status.MatchEventIndicator = ss.str();
        _security_status.has_MatchEventIndicator = true;
        
        switch(msg.haltReason())
        {
        case globex::HaltReason::Value::GroupSchedule:
            _security_status.HaltReasonInt = "GroupSchedule";
            break; 
        case globex::HaltReason::Value::SurveillanceIntervention:
            _security_status.HaltReasonInt = "SurveillanceIntervention";
            break; 
        case globex::HaltReason::Value::MarketEvent:
            _security_status.HaltReasonInt = "MarketEvent";
            break; 
        case globex::HaltReason::Value::InstrumentActivation:
            _security_status.HaltReasonInt = "InstrumentActivation";
            break; 
        case globex::HaltReason::Value::InstrumentExpiration:
            _security_status.HaltReasonInt = "InstrumentExpiration";
            break; 
        case globex::HaltReason::Value::Unknown:
            _security_status.HaltReasonInt = "Unknown";
            break; 
        case globex::HaltReason::Value::RecoveryInProcess:
            _security_status.HaltReasonInt = "RecoveryInProcess";
            break;
        case globex::HaltReason::Value::NULL_VALUE:
            _security_status.HaltReasonInt = "NULL";
            break; 
        default:
            _security_status.HaltReasonInt = "NULL";
        };
        _security_status.has_HaltReasonInt = true;

        switch(msg.securityTradingEvent())
        {
        case globex::SecurityTradingEvent::Value::NoEvent:
            _security_status.SecurityTradingEvent = "NoEvent";
            break;
        case globex::SecurityTradingEvent::NoCancel:
            _security_status.SecurityTradingEvent = "NoCancel";
            break;
        case globex::SecurityTradingEvent::ResetStatistics:
            _security_status.SecurityTradingEvent = "ResetStatistics";
            break;
        case globex::SecurityTradingEvent::ImpliedMatchingON:
            _security_status.SecurityTradingEvent = "ImpliedMatchingON";
            break;
        case globex::SecurityTradingEvent::ImpliedMatchingOFF:
            _security_status.SecurityTradingEvent = "ImpliedMatchingOFF";
            break;
        case globex::SecurityTradingEvent::NULL_VALUE:
            _security_status.SecurityTradingEvent = "NULL";
            break;
        default:
            _security_status.SecurityTradingEvent = "NULL";
        };   
        _security_status.has_SecurityTradingEvent = true;
        
        if(msg.securityID() != msg.securityIDNullValue())
        {
            _security_status.SecurityID = msg.securityID();
            _security_status.has_SecurityID = true;
        }       

        switch(msg.securityTradingStatus())
        {
        case globex::SecurityTradingStatus::Value::TradingHalt:
            _security_status.SecurityTradingStatus = "TradingHalt";
            break;
        case globex::SecurityTradingStatus::Value::Close:
            _security_status.SecurityTradingStatus = "Close";
            break;
        case globex::SecurityTradingStatus::Value::NewPriceIndication:
            _security_status.SecurityTradingStatus = "NewPriceIndication";
            break;
        case globex::SecurityTradingStatus::Value::ReadyToTrade:
            _security_status.SecurityTradingStatus = "ReadyToTrade";
            break;
        case globex::SecurityTradingStatus::Value::NotAvailableForTrading:
            _security_status.SecurityTradingStatus = "NotAvailableForTrading";
            break;
        case globex::SecurityTradingStatus::Value::UnknownorInvalid:
            _security_status.SecurityTradingStatus = "UnknownorInvalid";
            break;
        case globex::SecurityTradingStatus::Value::PreOpen:
            _security_status.SecurityTradingStatus = "PreOpen";
            break;
        case globex::SecurityTradingStatus::Value::PreCross:
            _security_status.SecurityTradingStatus = "PreCross";
            break;
        case globex::SecurityTradingStatus::Value::Cross:
            _security_status.SecurityTradingStatus = "Cross";
            break;
        case globex::SecurityTradingStatus::Value::PostClose:
            _security_status.SecurityTradingStatus = "PostClose";
            break;
        case globex::SecurityTradingStatus::Value::NoChange:
            _security_status.SecurityTradingStatus = "NoChange";
            break;
        case globex::SecurityTradingStatus::Value::NULL_VALUE:
            _security_status.SecurityTradingStatus = "NULL";
            break;
        default:
            _security_status.SecurityTradingStatus = "NULL";
        }
        _security_status.has_SecurityTradingStatus = true;

        _handler.on_security_status_raw(_security_status);
    }
 
    void handle(globex::MDInstrumentDefinitionFuture27 &msg)
    {
        _secdef.reset();

        if(msg.highLimitPrice().mantissa() != msg.highLimitPrice().mantissaNullValue())
        { 
            _secdef.HighLimitPrice = (double)msg.highLimitPrice().mantissa() * gpm::anti_mult_double;
            _secdef.has_HighLimitPrice = true;
        }

        if(msg.lowLimitPrice().mantissa() != msg.lowLimitPrice().mantissaNullValue())
        { 
            _secdef.LowLimitPrice = (double)msg.lowLimitPrice().mantissa() * gpm::anti_mult_double;
            _secdef.has_LowLimitPrice = true;
        }

        if(msg.tradingReferencePrice().mantissa() != msg.tradingReferencePrice().mantissaNullValue())
        { 
            _secdef.TradingReferencePrice = (double)msg.tradingReferencePrice().mantissa() * gpm::anti_mult_double;
            _secdef.has_TradingReferencePrice = true;
        }

        _secdef.Symbol = msg.symbol();
        _secdef.has_Symbol = true;

        _secdef.SecurityDesc = msg.symbol();
        _secdef.has_SecurityDesc = true;

        _secdef.SecurityID = msg.securityID();
        _secdef.has_SecurityID = true;

        _secdef.MinPriceIncrement = (double)msg.minPriceIncrement().mantissa() * gpm::anti_mult_double;
        _secdef.has_MinPriceIncrement = true;

        _handler.on_security_definition_raw(_secdef);
    }

    template <typename M>
    void do_dummy_incremental_refresh( M & msg)
    {
        _incremental_refresh.MsgSeqNum = (int64_t)_sequence_number; 
        _incremental_refresh.SendingTime = from_epoch_nanos(msg.transactTime()); 
        _incremental_refresh.ReceivedTime = _rcvd_time;
        auto entries = msg.noMDEntries();
        _incremental_refresh.MDEntries.resize(entries.count());
        int i = 0;
        while(entries.hasNext() && i < entries.count())
        {
            entries.next();
            cme::MDEntry *e = &_incremental_refresh.MDEntries[i];
            e->reset();
            
            e->SecurityID = entries.securityID();
            e->has_SecurityID = true;
            
            e->RptSeq = (int32_t)entries.rptSeq();
            e->has_RptSeq = true;
            
            e->MDEntryType = cme::MDEntryTypeEnum::DUMMY_MDP3;
            e->has_MDEntryType = true;
            ++i;
        }
        
        _incremental_refresh.OrderIDEntries.resize(0);

        _handler.on_incremental_refresh_raw(_incremental_refresh);
    }
 
    void handle(globex::MDIncrementalRefreshDailyStatistics33& msg) 
    {
        do_dummy_incremental_refresh(msg);
    }
 
    void handle(globex::MDIncrementalRefreshLimitsBanding34& msg) 
    {
        do_dummy_incremental_refresh(msg);
    }

    void handle(globex::MDIncrementalRefreshSessionStatistics35& msg)
    {
        do_dummy_incremental_refresh(msg);
    }

    void handle(globex::MDIncrementalRefreshTrade36& msg)
    {
        do_dummy_incremental_refresh(msg);
    }
    
    void handle(globex::MDIncrementalRefreshVolume37& msg)
    {
        do_dummy_incremental_refresh(msg);
    }
 
    void parse(char const *buf, int len, timestamp ts, int offset)
    {
        timespec t = to_timespec(ts); 
        parse(buf,len,t,offset);
    }    

    void parse(char const *buf, int len, timespec &ts, int offset)
    {
        _rcvd_time = from_timespec(ts); 
 
        char *p = (char*)buf;
        
        p += offset;

        struct BinaryPacketHeader * bph = (BinaryPacketHeader *)p;
        
        on_start_of_packet(*bph);
        
        p += sizeof(BinaryPacketHeader);
 
        while ( len - (int)((char*)p - (char*)buf) > 2)
        {
            uint16_t &sz = *((uint16_t *)p);
            p += 2;             
            
            _hdr.wrap(p, 0, 1, 8);
            
            p += _hdr.size();

            switch(_hdr.templateId())
            {
            case 32:
                _md32.wrapForDecode(p, 0, _hdr.blockLength(), _hdr.version(), sz - _hdr.size());
                handle(_md32);
                break;
            case 42:
                _md42.wrapForDecode(p, 0, _hdr.blockLength(), _hdr.version(),  sz - _hdr.size());
                handle(_md42);
                break;;
            case 30:
                _md30.wrapForDecode(p, 0, _hdr.blockLength(), _hdr.version(),  sz - _hdr.size());
                handle(_md30);
                break;
            case 27:
                _md27.wrapForDecode(p, 0, _hdr.blockLength(), _hdr.version(),  sz - _hdr.size());
                handle(_md27);
                break;
            case 33:
                _md33.wrapForDecode(p, 0, _hdr.blockLength(), _hdr.version(),  sz - _hdr.size());
                handle(_md33);
                break;
            case 34:
                _md34.wrapForDecode(p, 0, _hdr.blockLength(), _hdr.version(),  sz - _hdr.size());
                handle(_md34);
                break;
            case 35:
                _md35.wrapForDecode(p, 0, _hdr.blockLength(), _hdr.version(),  sz - _hdr.size());
                handle(_md35);
                break;
            case 36:
                _md36.wrapForDecode(p, 0, _hdr.blockLength(), _hdr.version(),  sz - _hdr.size());
                handle(_md36);
                break;
            case 37:
                _md37.wrapForDecode(p, 0, _hdr.blockLength(), _hdr.version(),  sz - _hdr.size());
                handle(_md37);
                break;
            default:
                // std::cout << "UNKNOWN template id " << _hdr.templateId()  << " of len=" << len << std::endl; 
                break;
            };
            p += sz - _hdr.size() - 2;
        }
        
        assert(len - (int)((char*)p - (char*)buf) == 0);
    } 

private:

    globex::MessageHeader                      _hdr;
    globex::MDIncrementalRefreshBook32         _md32; 
    globex::MDIncrementalRefreshTradeSummary42 _md42;
    globex::SecurityStatus30                   _md30;
    globex::MDInstrumentDefinitionFuture27     _md27;

    globex::MDIncrementalRefreshLimitsBanding34         _md34;   
    globex::MDIncrementalRefreshSessionStatistics35     _md35;
    globex::MDIncrementalRefreshDailyStatistics33       _md33;
    globex::MDIncrementalRefreshTrade36                 _md36;
    globex::MDIncrementalRefreshVolume37                _md37;

    Handler&                _handler;

    cme::IncrementalRefresh _incremental_refresh;
    cme::SecurityStatus     _security_status;
    cme::SecurityDefinition _secdef;

    int64_t                 _sequence_number = -1;
    uint64_t                _send_time = 0;
    timestamp               _rcvd_time;
};

}


#endif //ELEMENTS_SBE_CME_PARSER_H

