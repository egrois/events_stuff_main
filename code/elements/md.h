#ifndef ELEMENTS_CME_MARKET_DATA_H
#define ELEMENTS_CME_MARKET_DATA_H

#include <easy_time.h>
#include <iosfwd>

namespace elements
{
namespace cme
{

enum class MDUpdateActionEnum : char
{
    NEW                             = '0',
    CHANGE                          = '1',
    DELETE                          = '2',
    OVERLAY                         = '5'
};

std::ostream & operator << (std::ostream & os, cme::MDUpdateActionEnum const & msg) 
{
    switch (msg)
    {
        case MDUpdateActionEnum::NEW:
            os << "NEW";
            break;
        case MDUpdateActionEnum::CHANGE:
            os << "CHANGE";
            break;
        case MDUpdateActionEnum::DELETE:
            os << "DELETE";
            break;
        case MDUpdateActionEnum::OVERLAY:
            os << "OVERLAY";
            break;
        default:
            os << "?";
    }
    return os;
}


enum class MDEntryTypeEnum : char
{
    BID                             = '0',
    OFFER                           = '1',
    TRADE                           = '2',
    OPENING_PRICE                   = '4',
    SETTLEMENT_PRICE                = '6',
    TRADING_SESSION_HIGH_PRICE      = '7',
    TRADING_SESSION_LOW_PRICE       = '8',
    TRADE_VOLUME                    = 'B',
    OPEN_INTEREST                   = 'C',
    SIMULATED_SELL                  = 'E',
    SIMULATED_BUY                   = 'F',
    EMPTY_THE_BOOK                  = 'J',
    SESSION_HIGH_BID                = 'N',
    SESSION_LOW_OFFER               = 'O',
    FIXING_PRICE                    = 'W',
    CASH_NOTE                       = 'X',
    DUMMY_MDP3                      = 'Z'
};

std::ostream & operator << (std::ostream & os, cme::MDEntryTypeEnum const & msg) 
{
    switch(msg)
    {
        case MDEntryTypeEnum::BID: os << "BID"; break;
        case MDEntryTypeEnum::OFFER: os << "OFFER"; break;
        case MDEntryTypeEnum::TRADE: os << "TRADE"; break;
        case MDEntryTypeEnum::OPENING_PRICE: os << "OPENING_PRICE"; break;
        case MDEntryTypeEnum::SETTLEMENT_PRICE: os << "SETTLEMENT_PRICE"; break;
        case MDEntryTypeEnum::TRADING_SESSION_HIGH_PRICE: os << "TRADING_SESSION_HIGH_PRICE"; break;
        case MDEntryTypeEnum::TRADING_SESSION_LOW_PRICE: os << "TRADING_SESSION_LOW_PRICE"; break;
        case MDEntryTypeEnum::TRADE_VOLUME: os << "TRADE_VOLUME"; break;
        case MDEntryTypeEnum::OPEN_INTEREST: os << "OPEN_INTEREST"; break;
        case MDEntryTypeEnum::SIMULATED_SELL: os << "SIMULATED_SELL"; break;
        case MDEntryTypeEnum::SIMULATED_BUY: os << "SIMULATED_BUY"; break;
        case MDEntryTypeEnum::EMPTY_THE_BOOK: os << "EMPTY_THE_BOOK"; break;
        case MDEntryTypeEnum::SESSION_HIGH_BID: os << "SESSION_HIGH_BID"; break;
        case MDEntryTypeEnum::SESSION_LOW_OFFER: os << "SESSION_LOW_OFFER:"; break;
        case MDEntryTypeEnum::FIXING_PRICE: os << "FIXING_PRICE"; break;
        case MDEntryTypeEnum::CASH_NOTE: os << "CASH_NOTE"; break;
        case MDEntryTypeEnum::DUMMY_MDP3: os << "DUMMY_MDP3"; break;
        default: os << "?";    
    }
    return os;
}

enum class QuoteConditionEnum : char
{
    NORMAL                          = ' ',
    IMPLIED                         = 'K',
    EXCHANGE_BEST                   = 'C'
};
std::ostream & operator << (std::ostream & os, cme::QuoteConditionEnum const & msg) 
{
    switch(msg)
    {
        case QuoteConditionEnum::NORMAL: os << "NORMAL"; break;
        case QuoteConditionEnum::IMPLIED: os << "IMPLIED"; break;
        case QuoteConditionEnum::EXCHANGE_BEST: os << "EXCHANGE_BEST"; break;
        default: os << "?";    
    }
    return os;
}

enum class TradingSessionIDEnum : char
{
    PRE_OPENING                     = '0',
    OPENING                         = '1',
    CONTINIOUS                      = '2'
};
std::ostream & operator << (std::ostream & os, cme::TradingSessionIDEnum const & msg) 
{
    switch(msg)
    {
        case TradingSessionIDEnum::PRE_OPENING: os << "PRE_OPENING"; break;
        case TradingSessionIDEnum::OPENING: os << "OPENING"; break;
        case TradingSessionIDEnum::CONTINIOUS: os << "CONTINIOUS"; break;
        default: os << "?";    
    }
    return os;
}

enum class TickDirectionEnum : char
{
    PLUS_TICK                       = '0',
    MINUS_TICK                      = '2'
};
std::ostream & operator << (std::ostream & os, cme::TickDirectionEnum const & msg) 
{
    switch(msg)
    {
        case TickDirectionEnum::PLUS_TICK: os << "PLUS_TICK"; break;
        case TickDirectionEnum::MINUS_TICK: os << "MINUS_TICK"; break;
        default: os << "?";    
    }
    return os;
}

enum class TradeConditionEnum : char
{
    OPENING_TRADE                   = 'E',
    CALCULATED                      = '1'
};
std::ostream & operator << (std::ostream & os, cme::TradeConditionEnum const & msg) 
{
    switch(msg)
    {
        case TradeConditionEnum::OPENING_TRADE: os << "OPENING_TRADE"; break;
        case TradeConditionEnum::CALCULATED: os << "CALCULATED"; break;
        default: os << "NORMAL";    
    }
    return os;
}

enum class QuoteTypeEnum : char
{
    NONE                            = ' ',
    TRADABLE                        = '1'
};
std::ostream & operator << (std::ostream & os, cme::QuoteTypeEnum const & msg) 
{
    switch(msg)
    {
        case QuoteTypeEnum::NONE: os << "NONE"; break;
        case QuoteTypeEnum::TRADABLE: os << "TRADABLE"; break;
        default: os << "?";    
    }
    return os;
}

enum class AggressorSideEnum : char
{
    NONE                            = ' ',
    BUY                             = '1',
    SELL                            = '2'
};
std::ostream & operator << (std::ostream & os, cme::AggressorSideEnum const & msg) 
{
    switch(msg)
    {
        case AggressorSideEnum::NONE: os << "NONE"; break;
        case AggressorSideEnum::BUY: os << "BUY"; break;
        case AggressorSideEnum::SELL: os << "SELL"; break;
        default: os << "?";    
    }
    return os;
}

enum class MatchEventIndicatorEnum : char
{
    NONE                            = ' ',
    MID_EVENT                       = '0',
    BEGIN                           = '1',
    END                             = '2',
    BEGIN_AND_END                   = '3'
};
std::ostream & operator << (std::ostream & os, cme::MatchEventIndicatorEnum const & msg) 
{
    switch(msg)
    {
        case MatchEventIndicatorEnum::NONE: os << "NONE"; break;
        case MatchEventIndicatorEnum::MID_EVENT: os << "MID_EVENT"; break;
        case MatchEventIndicatorEnum::BEGIN: os << "BEGIN"; break;
        case MatchEventIndicatorEnum::END: os << "END"; break;
        case MatchEventIndicatorEnum::BEGIN_AND_END: os << "BEGIN_AND_END"; break;
        default: os << "?";    
    }
    return os;
}

struct MDEntry
{
    MDUpdateActionEnum              MDUpdateAction;
    int32_t                         MDPriceLevel;
    MDEntryTypeEnum                 MDEntryType;
    char                            SecurityIDSource;
    int32_t                         SecurityID;
    int32_t                         RptSeq;
    QuoteConditionEnum              QuoteCondition;
    int64_t                         MDEntryPx;
    int32_t                         NumberOfOrders;
    timestamp                       MDEntryTime;
    int32_t                         MDEntrySize;
    // TradingSessionIDEnum            TradingSessionID;
    // int64_t                         NetChgPrevDay;
    // TickDirectionEnum               TickDirection;
    // int32_t                         OpenCloseSettlFlag;
    // std::string                     SettlDate;
    TradeConditionEnum              TradeCondition;
    // int32_t                         TradeVolume;
    QuoteTypeEnum                   MDQuoteType;
    // std::string                     FixingBracket;
    AggressorSideEnum               AggressorSide;
    MatchEventIndicatorEnum         MatchEventIndicator;
    int64_t                         TradeID;
    char                            SecurityDesc[50];

    bool has_MDUpdateAction;
    bool has_MDPriceLevel;
    bool has_MDEntryType;
    bool has_SecurityIDSource;
    bool has_SecurityID;
    bool has_RptSeq;
    bool has_QuoteCondition;
    bool has_MDEntryPx;
    bool has_NumberOfOrders;
    bool has_MDEntryTime;
    bool has_MDEntrySize;
    bool has_TradingSessionID;
    bool has_NetChgPrevDay;
    bool has_TickDirection;
    bool has_OpenCloseSettlFlag;
    bool has_SettlDate;
    bool has_TradeCondition;
    bool has_TradeVolume;
    bool has_MDQuoteType;
    bool has_FixingBracket;
    bool has_AggressorSide;
    bool has_MatchEventIndicator;
    bool has_TradeID;
    bool has_SecurityDesc;

    void reset()
    {
        has_MDUpdateAction = false;
        has_MDPriceLevel = false;
        has_MDEntryType = false;
        has_SecurityIDSource = false;
        has_SecurityID = false;
        has_RptSeq = false;
        has_QuoteCondition = false;
        has_MDEntryPx = false;
        has_NumberOfOrders = false;
        has_MDEntryTime = false;
        has_MDEntrySize = false;
        has_TradingSessionID = false;
        has_NetChgPrevDay = false;
        has_TickDirection = false;
        has_OpenCloseSettlFlag = false;
        has_SettlDate = false;
        has_TradeCondition = false;
        has_TradeVolume = false;
        has_MDQuoteType = false;
        has_FixingBracket = false;
        has_AggressorSide = false;
        has_MatchEventIndicator = false;
        has_TradeID = false;
    }
};

struct OrderIDEntry
{
    uint64_t OrderID;
    int32_t  LastQty;

    bool has_OrderID = false;
    bool has_LastQty = false;

    void reset()
    {
        has_OrderID = false;
        has_LastQty = false; 
    }
};


struct IncrementalRefresh
{
    int64_t                         MsgSeqNum;
    timestamp                       SendingTime;
    timestamp                       ReceivedTime;
    std::string                     TradeDate;

    bool has_MsgSeqNum;
    bool has_SendingTime;
    bool has_ReceivedTime;
    bool has_TradeDate;

    void reset()
    {
        has_MsgSeqNum = false;
        has_SendingTime = false;
        has_ReceivedTime = false;
        has_TradeDate = false;
    }
    std::vector<MDEntry>            MDEntries;
    std::vector<OrderIDEntry >      OrderIDEntries; 

};

std::ostream & operator << (std::ostream & os, cme::IncrementalRefresh const & msg) 
{
    if(msg.has_MsgSeqNum) os << "MsgSeqNum=" << msg.MsgSeqNum;
    if(msg.has_SendingTime) os << " SendingTime=" << str(msg.SendingTime);
    if(msg.has_ReceivedTime) os << " ReceivedTime=" << str(msg.ReceivedTime);
    os << std::endl; 

    int i = 0;
    for(auto entry: msg.MDEntries )
    {
        os << " entry==" << i;
        if(entry.has_MDUpdateAction) os << " MDUpdateAction=" << entry.MDUpdateAction;
        if(entry.has_MDPriceLevel) os << " MDPriceLevel=" << entry.MDPriceLevel;
        if(entry.has_MDEntryType) os << " MDEntryType=" << entry.MDEntryType;
        if(entry.has_SecurityIDSource) os << " SecurityIDSource=" << entry.SecurityIDSource;
        if(entry.has_SecurityID) os << " SecurityID=" << entry.SecurityID;
        if(entry.has_RptSeq) os << " RptSeq=" << entry.RptSeq;
        if(entry.has_QuoteCondition) os << " QuoteCondition=" << entry.QuoteCondition;
        if(entry.has_MDEntryPx) os << " MDEntryPx=" << entry.MDEntryPx;
        if(entry.has_NumberOfOrders) os << " NumberOfOrders=" << entry.NumberOfOrders;
        if(entry.has_MDEntryTime) os << " MDEntryTime=" << str(entry.MDEntryTime);
        if(entry.has_MDEntrySize) os << " MDEntrySize=" << entry.MDEntrySize;
        if(entry.has_TradeCondition) os << " TradeCondition=" << entry.TradeCondition;
        if(entry.has_MDQuoteType) os << " MDQuoteType;=" << entry.MDQuoteType;
        if(entry.has_AggressorSide) os << " AggressorSide=" << entry.AggressorSide;
        if(entry.has_MatchEventIndicator) os << " MatchEventIndicator=" << entry.MatchEventIndicator;
        if(entry.has_TradeID) os << " TradeID=" << entry.TradeID;
        if(entry.has_SecurityDesc) os << " SecurityDesc=" << entry.SecurityDesc; 
        os << std::endl;
        ++i;
    }

    i = 0;
    for(auto entry: msg.OrderIDEntries )
    {
        os << "oid_entry=" << i;
        if(entry.has_OrderID) os << " OrderID=" << entry.OrderID;
        if(entry.has_LastQty) os << " LastQty=" << entry.LastQty;
        os << std::endl;
        ++i;
    }
    return os;
}

struct SecurityStatus
{
    int64_t                         MsgSeqNum;
    timestamp                       SendingTime;
    timestamp                       ReceivedTime;
    int32_t                         SecurityID;
    std::string                     SecurityIDSource;
    std::string                     TradeDate;
    std::string                     HighPx;
    std::string                     LowPx;
    std::string                     Symbol;
    std::string                     SecurityTradingStatus;
    std::string                     HaltReasonInt;
    std::string                     SecurityTradingEvent;
    std::string                     SecurityDesc;
    std::string                     SecurityGroup;
    std::string                     Asset;
    std::string                     MatchEventIndicator;


    bool has_MsgSeqNum;
    bool has_SendingTime;
    bool has_ReceivedTime;
    bool has_SecurityID;
    bool has_SecurityIDSource;
    bool has_TradeDate;
    bool has_HighPx;
    bool has_LowPx;
    bool has_Symbol;
    bool has_SecurityTradingStatus;
    bool has_HaltReasonInt;
    bool has_SecurityTradingEvent;
    bool has_SecurityDesc;
    bool has_SecurityGroup;
    bool has_Asset;
    bool has_MatchEventIndicator;

    void reset()
    {
        has_MsgSeqNum = false;
        has_SendingTime = false;
        has_ReceivedTime = false;
        has_SecurityID = false;
        has_SecurityIDSource = false;
        has_TradeDate = false;
        has_HighPx = false;
        has_LowPx = false;
        has_Symbol = false;
        has_SecurityTradingStatus = false;
        has_HaltReasonInt = false;
        has_SecurityTradingEvent = false;
        has_SecurityDesc = false;
        has_SecurityGroup = false;
        has_Asset = false;
        has_MatchEventIndicator = false;
    }
};
std::ostream & operator << (std::ostream & os, cme::SecurityStatus const & msg) 
{
    if(msg.has_MsgSeqNum) os <<" MsgSeqNum=" << msg.MsgSeqNum;
    if(msg.has_SendingTime) os << " SendingTime=" << str(msg.SendingTime);
    if(msg.has_ReceivedTime) os << " ReceivedTime=" << str(msg.ReceivedTime);
    if(msg.has_SecurityID) os << " SecurityID="<< msg.SecurityID;
    if(msg.has_SecurityIDSource) os << " SecurityIDSource=" << msg.SecurityIDSource;
    if(msg.has_TradeDate) os << " TradeDate)=" <<  msg.TradeDate;
    if(msg.has_HighPx) os << " HighPx=" << msg.HighPx;
    if(msg.has_LowPx) os <<  " LowPx=" << msg.LowPx;
    if(msg.has_Symbol) os << " Symbol="<< msg.Symbol;
    if(msg.has_SecurityTradingStatus) os << " SecurityTradingStatus=" << msg.SecurityTradingStatus;
    if(msg.has_HaltReasonInt) os << " HaltReasonInt="<< msg.HaltReasonInt;
    if(msg.has_SecurityTradingEvent) os << " SecurityTradingEvent="<< msg.SecurityTradingEvent;
    if(msg.has_SecurityDesc) os << " SecurityDesc="<< msg.SecurityDesc;
    if(msg.has_SecurityGroup) os << " SecurityGroup=" << msg.SecurityGroup;
    if(msg.has_Asset) os << " Asset=" << msg.Asset;
    if(msg.has_MatchEventIndicator) os <<" MatchEventIndicator=" << msg.MatchEventIndicator ;
    return os;
}


struct SecurityDefinition
{
    int64_t                         MsgSeqNum;
    timestamp                       SendingTime;
    timestamp                       ReceivedTime;
    std::string                     TotNumReports;
    double                          TradingReferencePrice;
    std::string                     SettlPriceType;
    double                          HighLimitPrice;
    double                          LowLimitPrice;
    std::string                     SecurityGroup;
    std::string                     Symbol;
    std::string                     ExpirationCycle;
    std::string                     SecurityDesc;
    int32_t                         SecurityID;
    std::string                     SecurityIDSource;
    std::string                     CFICode;
    std::string                     UnderlyingProduct;
    std::string                     SecurityExchange;
    std::string                     PricingModel;
    std::string                     MinCabPrice;
    std::string                     UnitOfMeasureQty;
    std::string                     StrikePrice;
    std::string                     StrikeCurrency;
    std::string                     MinTradeVol;
    std::string                     MaxTradeVol;
    std::string                     Currency;
    std::string                     SettlCurrency;
    std::string                     MatchAlgorithm;
    std::string                     SecuritySubType;
    std::string                     MaxPriceVariation;
    std::string                     ImpliedMarketIndicator;
    std::string                     MaturityMonthYear;
    double                          MinPriceIncrement;
    double                          MinPriceIncrementAmount;
    std::string                     LastUpdateTime;
    std::string                     SecurityUpdateAction;
    std::string                     DisplayFactor;
    std::string                     ApplID;
    std::string                     MarketSegmentID;
    std::string                     TradingReferenceDate;
    std::string                     UserDefinedInstrument;
    std::string                     PriceRatio;
    std::string                     ContractMultiplierUnit;
    std::string                     FlowScheduleType;
    std::string                     ContractMultiplier;
    std::string                     UnitOfMeasure;
    std::string                     DecayQty;
    std::string                     DecayStartDate;
    std::string                     OriginalContractSize;
    std::string                     ClearedVolume;
    std::string                     OpenInterestQty;

    bool has_MsgSeqNum;
    bool has_SendingTime;
    bool has_ReceivedTime;
    bool has_TotNumReports;
    bool has_TradingReferencePrice;
    bool has_SettlPriceType;
    bool has_HighLimitPrice;
    bool has_LowLimitPrice;
    bool has_SecurityGroup;
    bool has_Symbol;
    bool has_ExpirationCycle;
    bool has_SecurityDesc;
    bool has_SecurityID;
    bool has_SecurityIDSource;
    bool has_CFICode;
    bool has_UnderlyingProduct;
    bool has_SecurityExchange;
    bool has_PricingModel;
    bool has_MinCabPrice;
    bool has_UnitOfMeasureQty;
    bool has_StrikePrice;
    bool has_StrikeCurrency;
    bool has_MinTradeVol;
    bool has_MaxTradeVol;
    bool has_Currency;
    bool has_SettlCurrency;
    bool has_MatchAlgorithm;
    bool has_SecuritySubType;
    bool has_MaxPriceVariation;
    bool has_ImpliedMarketIndicator;
    bool has_MaturityMonthYear;
    bool has_MinPriceIncrement;
    bool has_MinPriceIncrementAmount;
    bool has_LastUpdateTime;
    bool has_SecurityUpdateAction;
    bool has_DisplayFactor;
    bool has_ApplID;
    bool has_MarketSegmentID;
    bool has_TradingReferenceDate;
    bool has_UserDefinedInstrument;
    bool has_PriceRatio;
    bool has_ContractMultiplierUnit;
    bool has_FlowScheduleType;
    bool has_ContractMultiplier;
    bool has_UnitOfMeasure;
    bool has_DecayQty;
    bool has_DecayStartDate;
    bool has_OriginalContractSize;
    bool has_ClearedVolume;
    bool has_OpenInterestQty;
   
    void reset() 
    {
        has_MsgSeqNum = false;
        has_SendingTime = false;
        has_ReceivedTime = false;
        has_TotNumReports = false;
        has_TradingReferencePrice = false;
        has_SettlPriceType = false;
        has_HighLimitPrice = false;
        has_LowLimitPrice = false;
        has_SecurityGroup = false;
        has_Symbol = false;
        has_ExpirationCycle = false;
        has_SecurityDesc = false;
        has_SecurityID = false;
        has_SecurityIDSource = false;
        has_CFICode = false;
        has_UnderlyingProduct = false;
        has_SecurityExchange = false;
        has_PricingModel = false;
        has_MinCabPrice = false;
        has_UnitOfMeasureQty = false;
        has_StrikePrice = false;
        has_StrikeCurrency = false;
        has_MinTradeVol = false;
        has_MaxTradeVol = false;
        has_Currency = false;
        has_SettlCurrency = false;
        has_MatchAlgorithm = false;
        has_SecuritySubType = false;
        has_MaxPriceVariation = false;
        has_ImpliedMarketIndicator = false;
        has_MaturityMonthYear = false;
        has_MinPriceIncrement = false;
        has_MinPriceIncrementAmount = false;
        has_LastUpdateTime = false;
        has_SecurityUpdateAction = false;
        has_DisplayFactor = false;
        has_ApplID = false;
        has_MarketSegmentID = false;
        has_TradingReferenceDate = false;
        has_UserDefinedInstrument = false;
        has_PriceRatio = false;
        has_ContractMultiplierUnit = false;
        has_FlowScheduleType = false;
        has_ContractMultiplier = false;
        has_UnitOfMeasure = false;
        has_DecayQty = false;
        has_DecayStartDate = false;
        has_OriginalContractSize = false;
        has_ClearedVolume = false;
        has_OpenInterestQty = false;
    }

};

std::ostream & operator << (std::ostream & os, cme::SecurityDefinition const & msg) 
{
    if(msg.has_MsgSeqNum) os <<" MsgSeqNum=" << msg.MsgSeqNum;
    if(msg.has_SendingTime) os << " SendingTime=" << str(msg.SendingTime);
    if(msg.has_ReceivedTime) os << " ReceivedTime=" << str(msg.ReceivedTime);
    if(msg.has_TotNumReports) os << " TotNumReports=" << msg.TotNumReports;
    if(msg.has_TradingReferencePrice) os << " TradingReferencePrice=" << msg.TradingReferencePrice;
    if(msg.has_SettlPriceType) os << " SettlPriceType=" << msg.SettlPriceType;
    if(msg.has_HighLimitPrice) os << " HighLimitPrice=" << msg.HighLimitPrice;
    if(msg.has_LowLimitPrice) os << " LowLimitPrice=" << msg.LowLimitPrice;
    if(msg.has_SecurityGroup) os << " SecurityGroup=" << msg.SecurityGroup;
    if(msg.has_Symbol) os << " Symbol=" << msg.Symbol;
    if(msg.has_ExpirationCycle) os << " ExpirationCycle=" << msg.ExpirationCycle;
    if(msg.has_SecurityDesc) os << " SecurityDesc=" << msg.SecurityDesc;
    if(msg.has_SecurityID) os << " SecurityID=" << msg.SecurityID;
    if(msg.has_SecurityIDSource) os << " SecurityIDSource=" << msg.SecurityIDSource;
    if(msg.has_CFICode) os << " CFICode=" << msg.CFICode;
    if(msg.has_UnderlyingProduct) os << " UnderlyingProduct=" << msg.UnderlyingProduct;
    if(msg.has_SecurityExchange) os << " SecurityExchange=" << msg.SecurityExchange;
    if(msg.has_PricingModel) os << " PricingModel=" << msg.PricingModel;
    if(msg.has_MinCabPrice) os << " MinCabPrice=" << msg.MinCabPrice;
    if(msg.has_UnitOfMeasureQty) os << " UnitOfMeasureQty=" << msg.UnitOfMeasureQty;
    if(msg.has_StrikePrice) os << " StrikePrice=" << msg.StrikePrice;
    if(msg.has_StrikeCurrency) os << " StrikeCurrency=" << msg.StrikeCurrency;
    if(msg.has_MinTradeVol) os << " MinTradeVol=" << msg.MinTradeVol;
    if(msg.has_MaxTradeVol) os << " MaxTradeVol=" << msg.MaxTradeVol;
    if(msg.has_Currency) os << " Currency=" << msg.Currency;
    if(msg.has_SettlCurrency) os << " SettlCurrency=" << msg.SettlCurrency;
    if(msg.has_MatchAlgorithm) os << " MatchAlgorithm=" << msg.MatchAlgorithm;
    if(msg.has_SecuritySubType) os << " SecuritySubType=" << msg.SecuritySubType;
    if(msg.has_MaxPriceVariation) os << " MaxPriceVariation=" << msg.MaxPriceVariation;
    if(msg.has_ImpliedMarketIndicator) os << " ImpliedMarketIndicator=" << msg.ImpliedMarketIndicator;
    if(msg.has_MaturityMonthYear) os << " MaturityMonthYear=" << msg.MaturityMonthYear;
    if(msg.has_MinPriceIncrement) os << " MinPriceIncrement=" << msg.MinPriceIncrement;
    if(msg.has_MinPriceIncrementAmount) os << " MinPriceIncrementAmount=" << msg.MinPriceIncrementAmount;
    if(msg.has_LastUpdateTime) os << " LastUpdateTime=" << msg.LastUpdateTime;
    if(msg.has_SecurityUpdateAction) os << " SecurityUpdateAction=" << msg.SecurityUpdateAction;
    if(msg.has_DisplayFactor) os << " DisplayFactor=" << msg.DisplayFactor;
    if(msg.has_ApplID) os << " ApplID=" << msg.ApplID;
    if(msg.has_MarketSegmentID) os << " MarketSegmentID=" << msg.MarketSegmentID;
    if(msg.has_TradingReferenceDate) os << " TradingReferenceDate=" << msg.TradingReferenceDate;
    if(msg.has_UserDefinedInstrument) os << " UserDefinedInstrument=" << msg.UserDefinedInstrument;
    if(msg.has_PriceRatio) os << " PriceRatio=" << msg.PriceRatio;
    if(msg.has_ContractMultiplierUnit) os << " ContractMultiplierUnit=" << msg.ContractMultiplierUnit;
    if(msg.has_FlowScheduleType) os << " FlowScheduleType=" << msg.FlowScheduleType;
    if(msg.has_ContractMultiplier) os << " ContractMultiplier=" << msg.ContractMultiplier;
    if(msg.has_UnitOfMeasure) os << " UnitOfMeasure=" << msg.UnitOfMeasure;
    if(msg.has_DecayQty) os << " DecayQty=" << msg.DecayQty;
    if(msg.has_DecayStartDate) os << " DecayStartDate=" << msg.DecayStartDate;
    if(msg.has_OriginalContractSize) os << " OriginalContractSize=" << msg.OriginalContractSize;
    if(msg.has_ClearedVolume) os << " ClearedVolume=" << msg.ClearedVolume;
    if(msg.has_OpenInterestQty) os << " OpenInterestQty=" << msg.OpenInterestQty;
    os << std::endl;
    return os;
}

}}

#endif // ELEMENTS_CME_MARKET_DATA_H
