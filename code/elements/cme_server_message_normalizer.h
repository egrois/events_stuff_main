#ifndef ELEMENTS_CME_SERVER_MESSAGE_NORMALIZER_H
#define ELEMENTS_CME_SERVER_MESSAGE_NORMALIZER_H

#include "sbe/cmeMdpV6/AdminHeartbeat12.hpp"
#include "sbe/cmeMdpV6/AdminLogin15.hpp"
#include "sbe/cmeMdpV6/AdminLogout16.hpp"
#include "sbe/cmeMdpV6/AggressorSide.hpp"
#include "sbe/cmeMdpV6/ChannelReset4.hpp"
#include "sbe/cmeMdpV6/DecimalQty.hpp"
#include "sbe/cmeMdpV6/EventType.hpp"
#include "sbe/cmeMdpV6/FLOAT.hpp"
#include "sbe/cmeMdpV6/GroupSize.hpp"
#include "sbe/cmeMdpV6/GroupSize8Byte.hpp"
#include "sbe/cmeMdpV6/HaltReason.hpp"
#include "sbe/cmeMdpV6/InstAttribValue.hpp"
#include "sbe/cmeMdpV6/LegSide.hpp"
#include "sbe/cmeMdpV6/MDEntryType.hpp"
#include "sbe/cmeMdpV6/MDEntryTypeBook.hpp"
#include "sbe/cmeMdpV6/MDEntryTypeDailyStatistics.hpp"
#include "sbe/cmeMdpV6/MDEntryTypeStatistics.hpp"
#include "sbe/cmeMdpV6/MDIncrementalRefreshBook32.hpp"
#include "sbe/cmeMdpV6/MDIncrementalRefreshDailyStatistics33.hpp"
#include "sbe/cmeMdpV6/MDIncrementalRefreshLimitsBanding34.hpp"
#include "sbe/cmeMdpV6/MDIncrementalRefreshSessionStatistics35.hpp"
#include "sbe/cmeMdpV6/MDIncrementalRefreshTrade36.hpp"
#include "sbe/cmeMdpV6/MDIncrementalRefreshTradeSummary42.hpp"
#include "sbe/cmeMdpV6/MDIncrementalRefreshVolume37.hpp"
#include "sbe/cmeMdpV6/MDInstrumentDefinitionFuture27.hpp"
#include "sbe/cmeMdpV6/MDInstrumentDefinitionOption41.hpp"
#include "sbe/cmeMdpV6/MDInstrumentDefinitionSpread29.hpp"
#include "sbe/cmeMdpV6/MDUpdateAction.hpp"
#include "sbe/cmeMdpV6/MatchEventIndicator.hpp"
#include "sbe/cmeMdpV6/MaturityMonthYear.hpp"
#include "sbe/cmeMdpV6/MessageHeader.hpp"
#include "sbe/cmeMdpV6/OpenCloseSettlFlag.hpp"
#include "sbe/cmeMdpV6/PRICE.hpp"
#include "sbe/cmeMdpV6/PRICENULL.hpp"
#include "sbe/cmeMdpV6/PutOrCall.hpp"
#include "sbe/cmeMdpV6/QuoteRequest39.hpp"
#include "sbe/cmeMdpV6/SecurityStatus30.hpp"
#include "sbe/cmeMdpV6/SecurityTradingEvent.hpp"
#include "sbe/cmeMdpV6/SecurityTradingStatus.hpp"
#include "sbe/cmeMdpV6/SecurityUpdateAction.hpp"
#include "sbe/cmeMdpV6/SettlPriceType.hpp"

#include <md.h>
#include <ClientManager.h>
#include <macros.h>

namespace elements
{
template <typename Handler>
class cme_server_message_normalizer
{
public:
    cme_server_message_normalizer(Handler *h) : _handler(*h){}
    ~cme_server_message_normalizer(){}

    void convert(IncrementalRefreshPtr & p)
    {
        cmeMdp::Message *msg = (cmeMdp::Message*) &(p->metadata);
        switch (msg->_id)
        {
            case cmeMdp::SECURITYSTATUS_ID:
                //std::cout << "GG=30" << std::endl;
                convert(&(p->securityStatus30));
                break;
            case cmeMdp::MDINCREMENTALREFRESHBOOK_ID:
                //std::cout << "GG=32" << std::endl;
                convert(&(p->mDIncrementalRefreshBook32));
                break;
            case cmeMdp::MDINCREMENTALREFRESHDAILYSTATISTICS_ID:
                //std::cout << "GG=33" << std::endl;
                convert_dummy(&(p->mDIncrementalRefreshDailyStatistics33));
                break;
            case cmeMdp::MDINCREMENTALREFRESHLIMITSBANDING_ID:
                //std::cout << "GG=34" << std::endl;
                convert_dummy(&(p->mDIncrementalRefreshLimitsBanding34));
                break;
            case cmeMdp::MDINCREMENTALREFRESHSESSIONSTATISTICS_ID:
                //std::cout << "GG=35" << std::endl;
                convert_dummy(&(p->mDIncrementalRefreshSessionStatistics35));
                break;
            case cmeMdp::MDINCREMENTALREFRESHTRADE_ID:
                //std::cout << "GG=36" << std::endl;
                convert_dummy(&(p->mDIncrementalRefreshTrade36));
                break;
            case cmeMdp::MDINCREMENTALREFRESHVOLUME_ID:
                //std::cout << "GG=37" << std::endl;
                convert_dummy(&(p->mDIncrementalRefreshVolume37));
                break;
            case cmeMdp::MDINCREMENTALREFRESHTRADESUMMARY_ID:
                //std::cout << "GG=42" << std::endl;
                convert(&(p->mDIncrementalRefreshTradeSummary42));
                break;
            default:
                return;
        }
    }

    void convert(cmeMdp::SecurityStatus30 *p)
    {
        auto &msg = *p;
        _security_status.reset();
        _security_status.MsgSeqNum = (int64_t)_sequence_number; 
        _security_status.SendingTime = from_epoch_nanos(msg._sendingTime); 
        _security_status.ReceivedTime = from_epoch_nanos(msg._recvTime); 
        _security_status.has_MsgSeqNum = true;
        _security_status.has_SendingTime = true;
        _security_status.has_ReceivedTime = true;

        std::stringstream ss;
        ss << "0x" << std::hex << (int)msg._MatchEventIndicator;
        _security_status.MatchEventIndicator = ss.str();
        _security_status.has_MatchEventIndicator = true;
        
        switch(msg._HaltReason)
        {
        case cmeMdpV6::HaltReason::Value::GroupSchedule:
            _security_status.HaltReasonInt = "GroupSchedule";
            break; 
        case cmeMdpV6::HaltReason::Value::SurveillanceIntervention:
            _security_status.HaltReasonInt = "SurveillanceIntervention";
            break; 
        case cmeMdpV6::HaltReason::Value::MarketEvent:
            _security_status.HaltReasonInt = "MarketEvent";
            break; 
        case cmeMdpV6::HaltReason::Value::InstrumentActivation:
            _security_status.HaltReasonInt = "InstrumentActivation";
            break; 
        case cmeMdpV6::HaltReason::Value::InstrumentExpiration:
            _security_status.HaltReasonInt = "InstrumentExpiration";
            break; 
        case cmeMdpV6::HaltReason::Value::Unknown:
            _security_status.HaltReasonInt = "Unknown";
            break; 
        case cmeMdpV6::HaltReason::Value::RecoveryInProcess:
            _security_status.HaltReasonInt = "RecoveryInProcess";
            break;
        case cmeMdpV6::HaltReason::Value::NULL_VALUE:
            _security_status.HaltReasonInt = "NULL";
            break; 
        default:
            _security_status.HaltReasonInt = "NULL";
        };
        _security_status.has_HaltReasonInt = true;

        switch(msg._SecurityTradingEvent)
        {
        case cmeMdpV6::SecurityTradingEvent::Value::NoEvent:
            _security_status.SecurityTradingEvent = "NoEvent";
            break;
        case cmeMdpV6::SecurityTradingEvent::NoCancel:
            _security_status.SecurityTradingEvent = "NoCancel";
            break;
        case cmeMdpV6::SecurityTradingEvent::ResetStatistics:
            _security_status.SecurityTradingEvent = "ResetStatistics";
            break;
        case cmeMdpV6::SecurityTradingEvent::ImpliedMatchingON:
            _security_status.SecurityTradingEvent = "ImpliedMatchingON";
            break;
        case cmeMdpV6::SecurityTradingEvent::ImpliedMatchingOFF:
            _security_status.SecurityTradingEvent = "ImpliedMatchingOFF";
            break;
        case cmeMdpV6::SecurityTradingEvent::NULL_VALUE:
            _security_status.SecurityTradingEvent = "NULL";
            break;
        default:
            _security_status.SecurityTradingEvent = "NULL";
        };   
        _security_status.has_SecurityTradingEvent = true;
        
        if(msg._SecurityID != 0 )
        {
            _security_status.SecurityID = msg._SecurityID;
            _security_status.has_SecurityID = true;
        }       

        switch(msg._SecurityTradingStatus)
        {
        case cmeMdpV6::SecurityTradingStatus::Value::TradingHalt:
            _security_status.SecurityTradingStatus = "TradingHalt";
            break;
        case cmeMdpV6::SecurityTradingStatus::Value::Close:
            _security_status.SecurityTradingStatus = "Close";
            break;
        case cmeMdpV6::SecurityTradingStatus::Value::NewPriceIndication:
            _security_status.SecurityTradingStatus = "NewPriceIndication";
            break;
        case cmeMdpV6::SecurityTradingStatus::Value::ReadyToTrade:
            _security_status.SecurityTradingStatus = "ReadyToTrade";
            break;
        case cmeMdpV6::SecurityTradingStatus::Value::NotAvailableForTrading:
            _security_status.SecurityTradingStatus = "NotAvailableForTrading";
            break;
        case cmeMdpV6::SecurityTradingStatus::Value::UnknownorInvalid:
            _security_status.SecurityTradingStatus = "UnknownorInvalid";
            break;
        case cmeMdpV6::SecurityTradingStatus::Value::PreOpen:
            _security_status.SecurityTradingStatus = "PreOpen";
            break;
        case cmeMdpV6::SecurityTradingStatus::Value::PreCross:
            _security_status.SecurityTradingStatus = "PreCross";
            break;
        case cmeMdpV6::SecurityTradingStatus::Value::Cross:
            _security_status.SecurityTradingStatus = "Cross";
            break;
        case cmeMdpV6::SecurityTradingStatus::Value::PostClose:
            _security_status.SecurityTradingStatus = "PostClose";
            break;
        case cmeMdpV6::SecurityTradingStatus::Value::NoChange:
            _security_status.SecurityTradingStatus = "NoChange";
            break;
        case cmeMdpV6::SecurityTradingStatus::Value::NULL_VALUE:
            _security_status.SecurityTradingStatus = "NULL";
            break;
        default:
            _security_status.SecurityTradingStatus = "NULL";
        }
        _security_status.has_SecurityTradingStatus = true;

        _handler.on_security_status_raw(_security_status);
    }

    void convert(cmeMdp::MDIncrementalRefreshBook32 *p)
    {
        auto &msg = *p;
        _incremental_refresh.MsgSeqNum = (int64_t)_sequence_number; 
        _incremental_refresh.SendingTime = from_epoch_nanos(msg._sendingTime); 
        _incremental_refresh.ReceivedTime = from_epoch_nanos(msg._recvTime); 
        auto entries = msg.NoMDEntries;
        _incremental_refresh.MDEntries.resize(entries._groupCount);
        _incremental_refresh.OrderIDEntries.resize(0);
        int i = 0;
        while( i < entries._groupCount )
        {
            cme::MDEntry *e = &_incremental_refresh.MDEntries[i];
            e->reset();

            switch(entries._MDUpdateAction[i])
            {
            case cmeMdpV6::MDUpdateAction::Value::New:
                e->MDUpdateAction = cme::MDUpdateActionEnum::NEW;        
                break;
            case cmeMdpV6::MDUpdateAction::Value::Change:
                e->MDUpdateAction = cme::MDUpdateActionEnum::CHANGE;        
                break;
            case cmeMdpV6::MDUpdateAction::Value::Delete:
                e->MDUpdateAction = cme::MDUpdateActionEnum::DELETE;        
                break;
            case cmeMdpV6::MDUpdateAction::Value::Overlay:
                e->MDUpdateAction = cme::MDUpdateActionEnum::OVERLAY;        
                break;
            case cmeMdpV6::MDUpdateAction::Value::DeleteThru:
            case cmeMdpV6::MDUpdateAction::Value::DeleteFrom:
            case cmeMdpV6::MDUpdateAction::Value::NULL_VALUE:
            default:
                 throw std::runtime_error("Unhandled update action"); 
            };
            e->has_MDUpdateAction = true;
       
            e->MDPriceLevel = (int32_t)entries._MDPriceLevel[i];
            e->has_MDPriceLevel = true;

            switch(entries._MDEntryType[i])
            {
            case cmeMdpV6::MDEntryTypeBook::Value::Bid:
                e->MDEntryType = cme::MDEntryTypeEnum::BID;
                e->QuoteCondition = cme::QuoteConditionEnum::NORMAL;
                break;
            case cmeMdpV6::MDEntryTypeBook::Value::Offer:
                e->MDEntryType = cme::MDEntryTypeEnum::OFFER;
                e->QuoteCondition = cme::QuoteConditionEnum::NORMAL;
                break;
            case cmeMdpV6::MDEntryTypeBook::Value::ImpliedBid:
                e->MDEntryType = cme::MDEntryTypeEnum::BID;
                e->QuoteCondition = cme::QuoteConditionEnum::IMPLIED;
                break;
            case cmeMdpV6::MDEntryTypeBook::Value::ImpliedOffer:
                e->MDEntryType = cme::MDEntryTypeEnum::OFFER;
                e->QuoteCondition = cme::QuoteConditionEnum::IMPLIED;
                break;
            case cmeMdpV6::MDEntryTypeBook::Value::BookReset:
                throw std::runtime_error("Unhandled entry type BookReset"); 
            case cmeMdpV6::MDEntryTypeBook::Value::NULL_VALUE:
                throw std::runtime_error("Unhandled entry type NULL_VALUE"); 
            default:    
                throw std::runtime_error("Unhandled entry type ???"); 
            };
            e->has_MDEntryType = true;
            e->has_QuoteCondition = true;

            e->SecurityID = entries._SecurityID[i];
            e->has_SecurityID = true;
            
            e->RptSeq = (int32_t)entries._RptSeq[i];
            e->has_RptSeq = true;

            e->MDEntryPx = entries._MDEntryPx[i]; 
            e->has_MDEntryPx = true;

            if(entries._NumberOfOrders[i] != 0 && entries._NumberOfOrders[i] < 1000000 )
            {
                e->NumberOfOrders =  (int32_t)entries._NumberOfOrders[i];
                e->has_NumberOfOrders = true;
            }
            else
            {
                e->NumberOfOrders = 0;
                e->has_NumberOfOrders = true;
            }

            e->MDEntrySize = (int32_t)entries._MDEntrySize[i];
            e->has_MDEntrySize = true;

            ++i;
        }
        _handler.on_incremental_refresh_raw(_incremental_refresh);
    }

    void convert(cmeMdp::MDIncrementalRefreshTradeSummary42 *p)
    {
        auto &msg = *p;
        
        // std::cout << "11" << std::endl;       
 
        _incremental_refresh.MsgSeqNum = (int64_t)_sequence_number; 
        _incremental_refresh.SendingTime = from_epoch_nanos(msg._sendingTime); 
        _incremental_refresh.ReceivedTime = from_epoch_nanos(msg._recvTime); 
        
        auto entries = msg.NoMDEntries;
        _incremental_refresh.MDEntries.resize(entries._groupCount);
        int i = 0;
        while( i < entries._groupCount )
        {
            cme::MDEntry *e = &_incremental_refresh.MDEntries[i];
            e->reset();
            
            e->MDEntryType = cme::MDEntryTypeEnum::TRADE;
            e->has_MDEntryType = true;
            
            switch(entries._MDUpdateAction[i])
            {
            case cmeMdpV6::MDUpdateAction::Value::New:
                e->MDUpdateAction = cme::MDUpdateActionEnum::NEW;        
                break;
            case cmeMdpV6::MDUpdateAction::Value::Change:
                e->MDUpdateAction = cme::MDUpdateActionEnum::CHANGE;        
                break;
            case cmeMdpV6::MDUpdateAction::Value::Delete:
                e->MDUpdateAction = cme::MDUpdateActionEnum::DELETE;        
                break;
            case cmeMdpV6::MDUpdateAction::Value::Overlay:
                e->MDUpdateAction = cme::MDUpdateActionEnum::OVERLAY;        
                break;
            case cmeMdpV6::MDUpdateAction::Value::DeleteThru:
            case cmeMdpV6::MDUpdateAction::Value::DeleteFrom:
            case cmeMdpV6::MDUpdateAction::Value::NULL_VALUE:
            default:
                throw std::runtime_error("Unhandled update action"); 
            };

            e->has_MDUpdateAction = true;

            // std::cout << "22" << std::endl;       
            
            e->SecurityID = entries._SecurityID[i];
            e->has_SecurityID = true;
            
            e->RptSeq = (int32_t)entries._RptSeq[i];
            e->has_RptSeq = true;
            
            e->MDEntryPx = entries._MDEntryPx[i]; 
            e->has_MDEntryPx = true;
          
            if (entries._NumberOfOrders[i] > 0 && entries._NumberOfOrders[i] < 1000000 )
            {           
                e->NumberOfOrders =  (int32_t)entries._NumberOfOrders[i];
                e->has_NumberOfOrders = true;            
            }
            else
            {
                e->NumberOfOrders = 0;
                e->has_NumberOfOrders = false;            
            }           
 
            e->MDEntrySize = (int32_t)entries._MDEntrySize[i];
            e->has_MDEntrySize = true;
           
            switch(entries._AggressorSide[i])
            {
            case cmeMdpV6::AggressorSide::Value::Buy:
                e->AggressorSide = cme::AggressorSideEnum::BUY; 
                e->has_AggressorSide = true;
                break;
            case cmeMdpV6::AggressorSide::Value::Sell:
                e->AggressorSide = cme::AggressorSideEnum::SELL; 
                e->has_AggressorSide = true;
                break;
            case cmeMdpV6::AggressorSide::Value::NoAggressor:
                e->AggressorSide = cme::AggressorSideEnum::NONE; 
                e->has_AggressorSide = true;
                break;
            case cmeMdpV6::AggressorSide::Value::NULL_VALUE:
            default:
                e->AggressorSide = cme::AggressorSideEnum::NONE; 
                e->has_AggressorSide = true;
                break;
            };
            ++i;
        }

        // std::cout << "33" << std::endl;       
        
        auto entries2 = msg.NoOrderIDEntries;
        _incremental_refresh.OrderIDEntries.resize(entries2._groupCount);

        // std::cout << "group_count" << entries2._groupCount << std::endl; 

        i = 0;
        while( i < entries2._groupCount)
        {
            cme::OrderIDEntry *e = &_incremental_refresh.OrderIDEntries[i];
            e->reset();

            e->OrderID = entries2._OrderID[i];
            e->has_OrderID = true;
            
            e->LastQty = entries2._LastQty[i];
            e->has_LastQty = true;
            
            ++i;
        }

        // std::cout << "44" << std::endl;       
        _handler.on_incremental_refresh_raw(_incremental_refresh);   
    }
   
    template <typename T>
    void convert_dummy(T *p)
    {
        auto &msg = *p;
        
        _incremental_refresh.MsgSeqNum = (int64_t)_sequence_number; 
        _incremental_refresh.SendingTime = from_epoch_nanos(msg._sendingTime); 
        _incremental_refresh.ReceivedTime = from_epoch_nanos(msg._recvTime); 
        
        auto entries = msg.NoMDEntries;
        _incremental_refresh.MDEntries.resize(entries._groupCount);
        int i = 0;
        while( i < entries._groupCount )
        {
            cme::MDEntry *e = &_incremental_refresh.MDEntries[i];
            e->reset();
            
            e->SecurityID = entries._SecurityID[i];
            e->has_SecurityID = true;
            
            e->RptSeq = (int32_t)entries._RptSeq[i];
            e->has_RptSeq = true;
            
            e->MDEntryType = cme::MDEntryTypeEnum::DUMMY_MDP3;
            e->has_MDEntryType = true;
            
            ++i;
        }
        _incremental_refresh.OrderIDEntries.resize(0);
        _handler.on_incremental_refresh_raw(_incremental_refresh);
    }

private:

    cme::IncrementalRefresh     _incremental_refresh;
    cme::SecurityStatus         _security_status;
    cme::SecurityDefinition     _secdef;
    Handler &                   _handler;
    int64_t                     _sequence_number = -1;
};

}
#endif 
