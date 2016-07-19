#ifndef ELEMENTS_MD_TRANSLATE_H
#define ELEMENTS_MD_TRANSLATE_H

#include <md.h>
#include <ascii_fix_parser.h>
#include <cme_ascii_secdef_parser.h>
#include <easy_time.h>
#include <stdlib.h>
#include <limits>
#include <global_price_multiplier.h>

namespace elements
{

    bool str_to_int64(int64_t &val, char const *str)
    {
        char *temp;
        errno = 0;
        val = strtol(str, &temp, 10);
        if(temp == str || *temp != '\0' 
            || ((val == std::numeric_limits<int64_t>::min() || val == std::numeric_limits<int64_t>::max()) && errno == ERANGE))
            return false;
        else
            return true;
    }

    bool str_to_int32(int32_t &val, char const *str)
    {
        char *temp;
        errno = 0;
        int64_t val2 = 0;
        val2 = strtol(str, &temp, 10);
        if(temp == str || *temp != '\0' 
            || ((val2 == std::numeric_limits<int64_t>::min() || val2 == std::numeric_limits<int64_t>::max()) && errno == ERANGE))
            return false;
        else
        {
            val = (int32_t)(val2);
            return true;
        }
    }

    int64_t price_to_int(char const *  str, int64_t mult)
    {
        double a = strtod(str, NULL);
        a = a + 1.0e-10;
        a *= (double)mult;
        return (int64_t)a;
    }

    bool translate( Header const * header,
                    MarketDataIncrementalRefresh const * in, 
                    cme::IncrementalRefresh *out)
    {
        assert(str_to_int64(out->MsgSeqNum,header->MsgSeqNum));
        out->SendingTime = from_fix_time(header->SendingTime); 
        out->ReceivedTime = out->SendingTime; 
        out->TradeDate = in->TradeDate;
        out->MDEntries.resize(in->MDIncGrp.size());
    
        for(int i = 0; i < in->MDIncGrp.size(); ++i)
        {
            cme::MDEntry *e = &out->MDEntries[i];
            MDIncGrp_Type const *s = &in->MDIncGrp[i];

            e->reset();
            
            if(s->has_MDUpdateAction)
            {
                e->MDUpdateAction = (cme::MDUpdateActionEnum)s->MDUpdateAction[0];
                e->has_MDUpdateAction = true;
            }

            if(s->has_MDPriceLevel)
            {
               assert(str_to_int32(e->MDPriceLevel, s->MDPriceLevel)); 
               e->has_MDPriceLevel = true; 
            }

            if(s->has_MDEntryType)
            {
                e->MDEntryType = (cme::MDEntryTypeEnum)s->MDEntryType[0];
                e->has_MDEntryType = true;
            }

            if(s->has_SecurityIDSource)
            {
                e->SecurityIDSource =  s->SecurityIDSource[0]; 
                e->has_SecurityIDSource = true; 
            }

            if(s->has_SecurityID)
            {
                assert(str_to_int32(e->SecurityID, s->SecurityID));
                e->has_SecurityID = true;
            }

            if(s->has_RptSeq)
            {
                assert(str_to_int32(e->RptSeq,s->RptSeq));
                e->has_RptSeq = true;
            }

            if(s->has_QuoteCondition)
            {
                e->QuoteCondition = (cme::QuoteConditionEnum)s->QuoteCondition[0];
                e->has_QuoteCondition = true;
            }

            if(s->has_MDEntryPx)
            {
                e->MDEntryPx = price_to_int(s->MDEntryPx, gpm::mult_int); 
                e->has_MDEntryPx = true;
            }

            if(s->has_NumberOfOrders)
            {
                str_to_int32(e->NumberOfOrders,s->NumberOfOrders);
                e->has_NumberOfOrders = false;
            }
 
            if(s->has_MDEntryTime)
            {
                char temp_time[25];
                strcpy(temp_time, in->TradeDate);
                strcat(temp_time, s->MDEntryTime);
                e->MDEntryTime = from_fix_time(temp_time);
                e->has_MDEntryTime =true;
            }

            if(s->has_MDEntrySize)
            {
                assert(str_to_int32(e->MDEntrySize,s->MDEntrySize));
                e->has_MDEntrySize = true;
            }

            if(s->has_TradeCondition)
            {
                e->TradeCondition = (cme::TradeConditionEnum)s->TradeCondition[0];
                e->has_TradeCondition = true;
            }

            if(s->has_MDQuoteType)
            {
                if(strlen(s->MDQuoteType) == 0)
                {
                    e->MDQuoteType = cme::QuoteTypeEnum::NONE;
                }
                else
                {
                    e->MDQuoteType = (cme::QuoteTypeEnum)s->MDQuoteType[0];
                } 
                e->has_MDQuoteType = true;
            }

            if(s->has_AggressorSide)
            {
                if(strlen(s->AggressorSide) == 0)
                {
                    e->AggressorSide = cme::AggressorSideEnum::NONE;
                }
                else
                {
                    e->AggressorSide = (cme::AggressorSideEnum)s->AggressorSide[0];
                }                
                e->has_AggressorSide = true;
            }

            if(s->has_MatchEventIndicator)
            {
                if(strlen(s->MatchEventIndicator) == 0)
                {
                    e->MatchEventIndicator = cme::MatchEventIndicatorEnum::NONE;
                }
                else
                {
                    e->MatchEventIndicator = (cme::MatchEventIndicatorEnum)s->MatchEventIndicator[0];
                }
                e->has_MatchEventIndicator = true;
            }

            if(s->has_TradeID)
            {
                assert(str_to_int64(e->TradeID, s->TradeID));
                e->has_TradeID = true;
            }

            if(s->has_SecurityDesc)
            {
                strcpy(e->SecurityDesc,s->SecurityDesc);
                e->has_SecurityDesc = true;
            }
        }
        return true; 
    }

    bool translate( Header const * header,
                    SecurityDefinition const * in, 
                    cme::SecurityDefinition *out,
                    bool ignore_header=false)
    {
        if(!ignore_header)
        {
            assert(str_to_int64(out->MsgSeqNum,header->MsgSeqNum));
            out->SendingTime = from_fix_time(header->SendingTime); 
        }

        if(in->has_TotNumReports) { out->has_TotNumReports = true; out->TotNumReports = in->TotNumReports; };
        if(in->has_TradingReferencePrice) { out->has_TradingReferencePrice = true; out->TradingReferencePrice = strtod(in->TradingReferencePrice,NULL); };
        if(in->has_SettlPriceType) { out->has_SettlPriceType = true; out->SettlPriceType = in->SettlPriceType; };
        if(in->has_HighLimitPrice) { out->has_HighLimitPrice = true; out->HighLimitPrice = strtod(in->HighLimitPrice, NULL); };
        if(in->has_LowLimitPrice) { out->has_LowLimitPrice = true; out->LowLimitPrice = strtod(in->LowLimitPrice,NULL); };
        if(in->has_SecurityGroup) { out->has_SecurityGroup = true; out->SecurityGroup = in->SecurityGroup; };
        if(in->has_Symbol) { out->has_Symbol = true; out->Symbol = in->Symbol; };
        if(in->has_ExpirationCycle) { out->has_ExpirationCycle = true; out->ExpirationCycle = in->ExpirationCycle; };
        if(in->has_SecurityDesc) { out->has_SecurityDesc = true; out->SecurityDesc = in->SecurityDesc; };
        if(in->has_SecurityID) { assert(str_to_int32(out->SecurityID, in->SecurityID)); out->has_SecurityID = true; }
        if(in->has_SecurityIDSource) { out->has_SecurityIDSource = true; out->SecurityIDSource = in->SecurityIDSource; };
        if(in->has_CFICode) { out->has_CFICode = true; out->CFICode = in->CFICode; };
        if(in->has_UnderlyingProduct) { out->has_UnderlyingProduct = true; out->UnderlyingProduct = in->UnderlyingProduct; };
        if(in->has_SecurityExchange) { out->has_SecurityExchange = true; out->SecurityExchange = in->SecurityExchange; };
        if(in->has_PricingModel) { out->has_PricingModel = true; out->PricingModel = in->PricingModel; };
        if(in->has_MinCabPrice) { out->has_MinCabPrice = true; out->MinCabPrice = in->MinCabPrice; };
        if(in->has_UnitOfMeasureQty) { out->has_UnitOfMeasureQty = true; out->UnitOfMeasureQty = in->UnitOfMeasureQty; };
        if(in->has_StrikePrice) { out->has_StrikePrice = true; out->StrikePrice = in->StrikePrice; };
        if(in->has_StrikeCurrency) { out->has_StrikeCurrency = true; out->StrikeCurrency = in->StrikeCurrency; };
        if(in->has_MinTradeVol) { out->has_MinTradeVol = true; out->MinTradeVol = in->MinTradeVol; };
        if(in->has_MaxTradeVol) { out->has_MaxTradeVol = true; out->MaxTradeVol = in->MaxTradeVol; };
        if(in->has_Currency) { out->has_Currency = true; out->Currency = in->Currency; };
        if(in->has_SettlCurrency) { out->has_SettlCurrency = true; out->SettlCurrency = in->SettlCurrency; };
        if(in->has_MatchAlgorithm) { out->has_MatchAlgorithm = true; out->MatchAlgorithm = in->MatchAlgorithm; };
        if(in->has_SecuritySubType) { out->has_SecuritySubType = true; out->SecuritySubType = in->SecuritySubType; };
        if(in->has_MaxPriceVariation) { out->has_MaxPriceVariation = true; out->MaxPriceVariation = in->MaxPriceVariation; };
        if(in->has_ImpliedMarketIndicator) { out->has_ImpliedMarketIndicator = true; out->ImpliedMarketIndicator = in->ImpliedMarketIndicator; };
        if(in->has_MaturityMonthYear) { out->has_MaturityMonthYear = true; out->MaturityMonthYear = in->MaturityMonthYear; };
        if(in->has_MinPriceIncrement) { out->has_MinPriceIncrement = true; out->MinPriceIncrement = strtod(in->MinPriceIncrement, NULL); };
        if(in->has_MinPriceIncrementAmount) { out->has_MinPriceIncrementAmount = true; out->MinPriceIncrementAmount = strtod(in->MinPriceIncrementAmount, NULL); };
        if(in->has_LastUpdateTime) { out->has_LastUpdateTime = true; out->LastUpdateTime = in->LastUpdateTime; };
        if(in->has_SecurityUpdateAction) { out->has_SecurityUpdateAction = true; out->SecurityUpdateAction = in->SecurityUpdateAction; };
        if(in->has_DisplayFactor) { out->has_DisplayFactor = true; out->DisplayFactor = in->DisplayFactor; };
        if(in->has_ApplID) { out->has_ApplID = true; out->ApplID = in->ApplID; };
        if(in->has_MarketSegmentID) { out->has_MarketSegmentID = true; out->MarketSegmentID = in->MarketSegmentID; };
        if(in->has_TradingReferenceDate) { out->has_TradingReferenceDate = true; out->TradingReferenceDate = in->TradingReferenceDate; };
        if(in->has_UserDefinedInstrument) { out->has_UserDefinedInstrument = true; out->UserDefinedInstrument = in->UserDefinedInstrument; };
        if(in->has_PriceRatio) { out->has_PriceRatio = true; out->PriceRatio = in->PriceRatio; };
        if(in->has_ContractMultiplierUnit) { out->has_ContractMultiplierUnit = true; out->ContractMultiplierUnit = in->ContractMultiplierUnit; };
        if(in->has_FlowScheduleType) { out->has_FlowScheduleType = true; out->FlowScheduleType = in->FlowScheduleType; };
        if(in->has_ContractMultiplier) { out->has_ContractMultiplier = true; out->ContractMultiplier = in->ContractMultiplier; };
        if(in->has_UnitOfMeasure) { out->has_UnitOfMeasure = true; out->UnitOfMeasure = in->UnitOfMeasure; };
        if(in->has_DecayQty) { out->has_DecayQty = true; out->DecayQty = in->DecayQty; };
        if(in->has_DecayStartDate) { out->has_DecayStartDate = true; out->DecayStartDate = in->DecayStartDate; };
        if(in->has_OriginalContractSize) { out->has_OriginalContractSize = true; out->OriginalContractSize = in->OriginalContractSize; };
        if(in->has_ClearedVolume) { out->has_ClearedVolume = true; out->ClearedVolume = in->ClearedVolume; };
        if(in->has_OpenInterestQty) { out->has_OpenInterestQty = true; out->OpenInterestQty = in->OpenInterestQty; };
 
        return true;
    }

    bool translate( Header const * header,
                    SecurityStatus const * in, 
                    cme::SecurityStatus *out)
    {
        if(in->has_SecurityID) { assert(str_to_int32(out->SecurityID, in->SecurityID)); out->has_SecurityID = true; }
        if(in->has_SecurityIDSource) { out->has_SecurityIDSource = true; out->SecurityIDSource = in->SecurityIDSource; };
        if(in->has_TradeDate) { out->has_TradeDate = true; out->TradeDate = in->TradeDate; };
        if(in->has_HighPx) { out->has_HighPx = true; out->HighPx = in->HighPx; };
        if(in->has_LowPx) { out->has_LowPx = true; out->LowPx = in->LowPx; };
        if(in->has_Symbol) { out->has_Symbol = true; out->Symbol = in->Symbol; };
        if(in->has_SecurityTradingStatus) { out->has_SecurityTradingStatus = true; out->SecurityTradingStatus = in->SecurityTradingStatus; };
        if(in->has_HaltReasonInt) { out->has_HaltReasonInt = true; out->HaltReasonInt = in->HaltReasonInt; };
        if(in->has_SecurityTradingEvent) { out->has_SecurityTradingEvent = true; out->SecurityTradingEvent = in->SecurityTradingEvent; };
        if(in->has_SecurityDesc) { out->has_SecurityDesc = true; out->SecurityDesc = in->SecurityDesc; };
        return true;
    }
    
    bool translate(AsciiSecDefMsg const * in, cme::SecurityDefinition *out)
    {
        // if(in->has_MatchEventIndicator) { out->MatchEventIndicator = in->MatchEventIndicator; out->has_MatchEventIndicator = true; }
        // if(in->has_DecayQuantity) { out->DecayQuantity = in->DecayQuantity; out->has_DecayQuantity = true; }
        if(in->has_UnitOfMeasureQty) { out->UnitOfMeasureQty = in->UnitOfMeasureQty; out->has_UnitOfMeasureQty = true; }
        if(in->has_SecurityIDSource) { out->SecurityIDSource = in->SecurityIDSource; out->has_SecurityIDSource = true; }
        if(in->has_SecurityGroup) { out->SecurityGroup = in->SecurityGroup; out->has_SecurityGroup = true; }
        if(in->has_HighLimitPrice) { out->HighLimitPrice = strtod(in->HighLimitPrice,NULL); out->has_HighLimitPrice = true; }
        //if(in->has_SubFraction) { out->SubFraction = in->SubFraction; out->has_SubFraction = true; }
        //if(in->has_Asset) { out->Asset = in->Asset; out->has_Asset = true; }
        if(in->has_UnitOfMeasure) { out->UnitOfMeasure = in->UnitOfMeasure; out->has_UnitOfMeasure = true; }
        if(in->has_SecurityUpdateAction) { out->SecurityUpdateAction = in->SecurityUpdateAction; out->has_SecurityUpdateAction = true; }
        if(in->has_CFICode) { out->CFICode = in->CFICode; out->has_CFICode = true; }
        if(in->has_ApplID) { out->ApplID = in->ApplID; out->has_ApplID = true; }
        //if(in->has_NoInstAttrib) { out->NoInstAttrib = in->NoInstAttrib; out->has_NoInstAttrib = true; }
        if(in->has_Symbol) { out->Symbol = in->Symbol; out->has_Symbol = true; out->Symbol = in->Symbol; }
        if(in->has_Symbol) { out->SecurityDesc = in->Symbol; out->has_SecurityDesc = true; out->SecurityDesc = in->Symbol; }
        if(in->has_OriginalContractSize) { out->OriginalContractSize = in->OriginalContractSize; out->has_OriginalContractSize = true; }
        if(in->has_LowLimitPrice) { out->LowLimitPrice = strtod(in->LowLimitPrice,NULL); out->has_LowLimitPrice = true; }
        if(in->has_DecayStartDate) { out->DecayStartDate = in->DecayStartDate; out->has_DecayStartDate = true; }
        if(in->has_SecurityID) { assert(str_to_int32(out->SecurityID, in->SecurityID)); out->has_SecurityID = true; }
        if(in->has_UnderlyingProduct) { out->UnderlyingProduct = in->UnderlyingProduct; out->has_UnderlyingProduct = true; }
        // if(in->has_NoLotTypeRules) { out->NoLotTypeRules = in->NoLotTypeRules; out->has_NoLotTypeRules = true; }
        if(in->has_DisplayFactor) { out->DisplayFactor = in->DisplayFactor; out->has_DisplayFactor = true; }
        if(in->has_MinPriceIncrement) { out->has_MinPriceIncrement = true; out->MinPriceIncrement = strtod(in->MinPriceIncrement, NULL); };
        if(in->has_MinPriceIncrementAmount) { out->has_MinPriceIncrementAmount = true; out->MinPriceIncrementAmount = strtod(in->MinPriceIncrementAmount, NULL); };
        if(in->has_SettlCurrency) { out->SettlCurrency = in->SettlCurrency; out->has_SettlCurrency = true; }
        if(in->has_MaxPriceVariation) { out->MaxPriceVariation = in->MaxPriceVariation; out->has_MaxPriceVariation = true; }
        if(in->has_UserDefinedInstrument) { out->UserDefinedInstrument = in->UserDefinedInstrument; out->has_UserDefinedInstrument = true; }
        //if(in->has_PriceDisplayFormat) { out->PriceDisplayFormat = in->PriceDisplayFormat; out->has_PriceDisplayFormat = true; }
        //if(in->has_NoEvents) { out->NoEvents = in->NoEvents; out->has_NoEvents = true; }
        if(in->has_MaturityMonthYear) { out->MaturityMonthYear = in->MaturityMonthYear; out->has_MaturityMonthYear = true; }
        //if(in->has_NoMDFeedTypes) { out->NoMDFeedTypes = in->NoMDFeedTypes; out->has_NoMDFeedTypes = true; }
        if(in->has_OpenInterestQty) { out->OpenInterestQty = in->OpenInterestQty; out->has_OpenInterestQty = true; }
        if(in->has_MaxTradeVol) { out->MaxTradeVol = in->MaxTradeVol; out->has_MaxTradeVol = true; }
        if(in->has_ContractMultiplierUnit) { out->ContractMultiplierUnit = in->ContractMultiplierUnit; out->has_ContractMultiplierUnit = true; }
        if(in->has_MatchAlgorithm) { out->MatchAlgorithm = in->MatchAlgorithm; out->has_MatchAlgorithm = true; }
        if(in->has_ContractMultiplier) { out->ContractMultiplier = in->ContractMultiplier; out->has_ContractMultiplier = true; }
        if(in->has_MinTradeVol) { out->MinTradeVol = in->MinTradeVol; out->has_MinTradeVol = true; }
        if(in->has_LastUpdateTime) { out->LastUpdateTime = in->LastUpdateTime; out->has_LastUpdateTime = true; }
        if(in->has_SettlPriceType) { out->SettlPriceType = in->SettlPriceType; out->has_SettlPriceType = true; }
        if(in->has_MarketSegmentID) { out->MarketSegmentID = in->MarketSegmentID; out->has_MarketSegmentID = true; }
        if(in->has_SecurityExchange) { out->SecurityExchange = in->SecurityExchange; out->has_SecurityExchange = true; }
        // if(in->has_SecurityType) { out->SecurityType = in->SecurityType; out->has_SecurityType = true; }
        if(in->has_Currency) { out->Currency = in->Currency; out->has_Currency = true; }
        //if(in->has_MainFraction) { out->MainFraction = in->MainFraction; out->has_MainFraction = true; }
        if(in->has_TradingReferencePrice) { out->has_TradingReferencePrice = true; out->TradingReferencePrice = strtod(in->TradingReferencePrice,NULL); };
        if(in->has_ClearedVolume) { out->ClearedVolume = in->ClearedVolume; out->has_ClearedVolume = true; }
        return true;
    }
}

#endif
