#ifndef ELEMENTS_CME_ASCII_SECDEF_PARSER_H
#define ELEMENTS_CME_ASCII_SECDEF_PARSER_H

#include <fix_tokenizer.h>


namespace elements
{
struct AsciiSecDefMsg
{
    char * MatchEventIndicator;
    char * DecayQuantity;
    char * UnitOfMeasureQty;
    char * SecurityIDSource;
    char * SecurityGroup;
    char * HighLimitPrice;
    char * SubFraction;
    char * Asset;
    char * UnitOfMeasure;
    char * SecurityUpdateAction;
    char * CFICode;
    char * ApplID;
    char * NoInstAttrib;
    char * Symbol;
    char * OriginalContractSize;
    char * LowLimitPrice;
    char * MinPriceIncrementAmount;
    char * DecayStartDate;
    char * SecurityID;
    char * UnderlyingProduct;
    char * NoLotTypeRules;
    char * DisplayFactor;
    char * MinPriceIncrement;
    char * SettlCurrency;
    char * MaxPriceVariation;
    char * UserDefinedInstrument;
    char * PriceDisplayFormat;
    char * NoEvents;
    char * MaturityMonthYear;
    char * NoMDFeedTypes;
    char * OpenInterestQty;
    char * MaxTradeVol;
    char * ContractMultiplierUnit;
    char * MatchAlgorithm;
    char * ContractMultiplier;
    char * MinTradeVol;
    char * LastUpdateTime;
    char * SettlPriceType;
    char * MarketSegmentID;
    char * SecurityExchange;
    char * SecurityType;
    char * Currency;
    char * MainFraction;
    char * TradingReferencePrice;
    char * ClearedVolume;

    bool has_MatchEventIndicator;
    bool has_DecayQuantity;
    bool has_UnitOfMeasureQty;
    bool has_SecurityIDSource;
    bool has_SecurityGroup;
    bool has_HighLimitPrice;
    bool has_SubFraction;
    bool has_Asset;
    bool has_UnitOfMeasure;
    bool has_SecurityUpdateAction;
    bool has_CFICode;
    bool has_ApplID;
    bool has_NoInstAttrib;
    bool has_Symbol;
    bool has_OriginalContractSize;
    bool has_LowLimitPrice;
    bool has_MinPriceIncrementAmount;
    bool has_DecayStartDate;
    bool has_SecurityID;
    bool has_UnderlyingProduct;
    bool has_NoLotTypeRules;
    bool has_DisplayFactor;
    bool has_MinPriceIncrement;
    bool has_SettlCurrency;
    bool has_MaxPriceVariation;
    bool has_UserDefinedInstrument;
    bool has_PriceDisplayFormat;
    bool has_NoEvents;
    bool has_MaturityMonthYear;
    bool has_NoMDFeedTypes;
    bool has_OpenInterestQty;
    bool has_MaxTradeVol;
    bool has_ContractMultiplierUnit;
    bool has_MatchAlgorithm;
    bool has_ContractMultiplier;
    bool has_MinTradeVol;
    bool has_LastUpdateTime;
    bool has_SettlPriceType;
    bool has_MarketSegmentID;
    bool has_SecurityExchange;
    bool has_SecurityType;
    bool has_Currency;
    bool has_MainFraction;
    bool has_TradingReferencePrice;
    bool has_ClearedVolume;


    void reset()
    {
        has_MatchEventIndicator = false;
        has_DecayQuantity = false;
        has_UnitOfMeasureQty = false;
        has_SecurityIDSource = false;
        has_SecurityGroup = false;
        has_HighLimitPrice = false;
        has_SubFraction = false;
        has_Asset = false;
        has_UnitOfMeasure = false;
        has_SecurityUpdateAction = false;
        has_CFICode = false;
        has_ApplID = false;
        has_NoInstAttrib = false;
        has_Symbol = false;
        has_OriginalContractSize = false;
        has_LowLimitPrice = false;
        has_MinPriceIncrementAmount = false;
        has_DecayStartDate = false;
        has_SecurityID = false;
        has_UnderlyingProduct = false;
        has_NoLotTypeRules = false;
        has_DisplayFactor = false;
        has_MinPriceIncrement = false;
        has_SettlCurrency = false;
        has_MaxPriceVariation = false;
        has_UserDefinedInstrument = false;
        has_PriceDisplayFormat = false;
        has_NoEvents = false;
        has_MaturityMonthYear = false;
        has_NoMDFeedTypes = false;
        has_OpenInterestQty = false;
        has_MaxTradeVol = false;
        has_ContractMultiplierUnit = false;
        has_MatchAlgorithm = false;
        has_ContractMultiplier = false;
        has_MinTradeVol = false;
        has_LastUpdateTime = false;
        has_SettlPriceType = false;
        has_MarketSegmentID = false;
        has_SecurityExchange = false;
        has_SecurityType = false;
        has_Currency = false;
        has_MainFraction = false;
        has_TradingReferencePrice = false;
        has_ClearedVolume = false;
    }

    void on_msg_field( int key, char * value )
    {

        switch(key)
        {
        case 5799:
            MatchEventIndicator = value;
            has_MatchEventIndicator = true;
            break;
        case 5818:
            DecayQuantity = value;
            has_DecayQuantity = true;
            break;
        case 1147:
            UnitOfMeasureQty = value;
            has_UnitOfMeasureQty = true;
            break;
        case 22:
            SecurityIDSource = value;
            has_SecurityIDSource = true;
            break;
        case 1151:
            SecurityGroup = value;
            has_SecurityGroup = true;
            break;
        case 1149:
            HighLimitPrice = value;
            has_HighLimitPrice = true;
            break;
        case 37703:
            SubFraction = value;
            has_SubFraction = true;
            break;
        case 6937:
            Asset = value;
            has_Asset = true;
            break;
        case 996:
            UnitOfMeasure = value;
            has_UnitOfMeasure = true;
            break;
        case 980:
            SecurityUpdateAction = value;
            has_SecurityUpdateAction = true;
            break;
        case 461:
            CFICode = value;
            has_CFICode = true;
            break;
        case 1180:
            ApplID = value;
            has_ApplID = true;
            break;
        case 870:
            NoInstAttrib = value;
            has_NoInstAttrib = true;
            break;
        case 55:
            Symbol = value;
            has_Symbol = true;
            break;
        case 5849:
            OriginalContractSize = value;
            has_OriginalContractSize = true;
            break;
        case 1148:
            LowLimitPrice = value;
            has_LowLimitPrice = true;
            break;
        case 1146:
            MinPriceIncrementAmount = value;
            has_MinPriceIncrementAmount = true;
            break;
        case 5819:
            DecayStartDate = value;
            has_DecayStartDate = true;
            break;
        case 48:
            SecurityID = value;
            has_SecurityID = true;
            break;
        case 462:
            UnderlyingProduct = value;
            has_UnderlyingProduct = true;
            break;
        case 1234:
            NoLotTypeRules = value;
            has_NoLotTypeRules = true;
            break;
        case 9787:
            DisplayFactor = value;
            has_DisplayFactor = true;
            break;
        case 969:
            MinPriceIncrement = value;
            has_MinPriceIncrement = true;
            break;
        case 120:
            SettlCurrency = value;
            has_SettlCurrency = true;
            break;
        case 1143:
            MaxPriceVariation = value;
            has_MaxPriceVariation = true;
            break;
        case 9779:
            UserDefinedInstrument = value;
            has_UserDefinedInstrument = true;
            break;
        case 9800:
            PriceDisplayFormat = value;
            has_PriceDisplayFormat = true;
            break;
        case 864:
            NoEvents = value;
            has_NoEvents = true;
            break;
        case 200:
            MaturityMonthYear = value;
            has_MaturityMonthYear = true;
            break;
        case 1141:
            NoMDFeedTypes = value;
            has_NoMDFeedTypes = true;
            break;
        case 5792:
            OpenInterestQty = value;
            has_OpenInterestQty = true;
            break;
        case 1140:
            MaxTradeVol = value;
            has_MaxTradeVol = true;
            break;
        case 1435:
            ContractMultiplierUnit = value;
            has_ContractMultiplierUnit = true;
            break;
        case 1142:
            MatchAlgorithm = value;
            has_MatchAlgorithm = true;
            break;
        case 231:
            ContractMultiplier = value;
            has_ContractMultiplier = true;
            break;
        case 562:
            MinTradeVol = value;
            has_MinTradeVol = true;
            break;
        case 779:
            LastUpdateTime = value;
            has_LastUpdateTime = true;
            break;
        case 731:
            SettlPriceType = value;
            has_SettlPriceType = true;
            break;
        case 1300:
            MarketSegmentID = value;
            has_MarketSegmentID = true;
            break;
        case 207:
            SecurityExchange = value;
            has_SecurityExchange = true;
            break;
        case 167:
            SecurityType = value;
            has_SecurityType = true;
            break;
        case 15:
            Currency = value;
            has_Currency = true;
            break;
        case 37702:
            MainFraction = value;
            has_MainFraction = true;
            break;
        case 1150:
            TradingReferencePrice = value;
            has_TradingReferencePrice = true;
            break;
        case 5791:
            ClearedVolume = value;
            has_ClearedVolume = true;
            break;
        default:
            return;
        };
    }
};

std::ostream& operator<<(std::ostream& os, const AsciiSecDefMsg& msg)
{
    if(msg.has_MatchEventIndicator == true) os << " MatchEventIndicator=" << msg.MatchEventIndicator;
    if(msg.has_DecayQuantity == true) os << " DecayQuantity=" << msg.DecayQuantity;
    if(msg.has_UnitOfMeasureQty == true) os << " UnitOfMeasureQty=" << msg.UnitOfMeasureQty;
    if(msg.has_SecurityIDSource == true) os << " SecurityIDSource=" << msg.SecurityIDSource;
    if(msg.has_SecurityGroup == true) os << " SecurityGroup=" << msg.SecurityGroup;
    if(msg.has_HighLimitPrice == true) os << " HighLimitPrice=" << msg.HighLimitPrice;
    if(msg.has_SubFraction == true) os << " SubFraction=" << msg.SubFraction;
    if(msg.has_Asset == true) os << " Asset=" << msg.Asset;
    if(msg.has_UnitOfMeasure == true) os << " UnitOfMeasure=" << msg.UnitOfMeasure;
    if(msg.has_SecurityUpdateAction == true) os << " SecurityUpdateAction=" << msg.SecurityUpdateAction;
    if(msg.has_CFICode == true) os << " CFICode=" << msg.CFICode;
    if(msg.has_ApplID == true) os << " ApplID=" << msg.ApplID;
    if(msg.has_NoInstAttrib == true) os << " NoInstAttrib=" << msg.NoInstAttrib;
    if(msg.has_Symbol == true) os << " Symbol=" << msg.Symbol;
    if(msg.has_OriginalContractSize == true) os << " OriginalContractSize=" << msg.OriginalContractSize;
    if(msg.has_LowLimitPrice == true) os << " LowLimitPrice=" << msg.LowLimitPrice;
    if(msg.has_MinPriceIncrementAmount == true) os << " MinPriceIncrementAmount=" << msg.MinPriceIncrementAmount;
    if(msg.has_DecayStartDate == true) os << " DecayStartDate=" << msg.DecayStartDate;
    if(msg.has_SecurityID == true) os << " SecurityID=" << msg.SecurityID;
    if(msg.has_UnderlyingProduct == true) os << " UnderlyingProduct=" << msg.UnderlyingProduct;
    if(msg.has_NoLotTypeRules == true) os << " NoLotTypeRules=" << msg.NoLotTypeRules;
    if(msg.has_DisplayFactor == true) os << " DisplayFactor=" << msg.DisplayFactor;
    if(msg.has_MinPriceIncrement == true) os << " MinPriceIncrement=" << msg.MinPriceIncrement;
    if(msg.has_SettlCurrency == true) os << " SettlCurrency=" << msg.SettlCurrency;
    if(msg.has_MaxPriceVariation == true) os << " MaxPriceVariation=" << msg.MaxPriceVariation;
    if(msg.has_UserDefinedInstrument == true) os << " UserDefinedInstrument=" << msg.UserDefinedInstrument;
    if(msg.has_PriceDisplayFormat == true) os << " PriceDisplayFormat=" << msg.PriceDisplayFormat;
    if(msg.has_NoEvents == true) os << " NoEvents=" << msg.NoEvents;
    if(msg.has_MaturityMonthYear == true) os << " MaturityMonthYear=" << msg.MaturityMonthYear;
    if(msg.has_NoMDFeedTypes == true) os << " NoMDFeedTypes=" << msg.NoMDFeedTypes;
    if(msg.has_OpenInterestQty == true) os << " OpenInterestQty=" << msg.OpenInterestQty;
    if(msg.has_MaxTradeVol == true) os << " MaxTradeVol=" << msg.MaxTradeVol;
    if(msg.has_ContractMultiplierUnit == true) os << " ContractMultiplierUnit=" << msg.ContractMultiplierUnit;
    if(msg.has_MatchAlgorithm == true) os << " MatchAlgorithm=" << msg.MatchAlgorithm;
    if(msg.has_ContractMultiplier == true) os << " ContractMultiplier=" << msg.ContractMultiplier;
    if(msg.has_MinTradeVol == true) os << " MinTradeVol=" << msg.MinTradeVol;
    if(msg.has_LastUpdateTime == true) os << " LastUpdateTime=" << msg.LastUpdateTime;
    if(msg.has_SettlPriceType == true) os << " SettlPriceType=" << msg.SettlPriceType;
    if(msg.has_MarketSegmentID == true) os << " MarketSegmentID=" << msg.MarketSegmentID;
    if(msg.has_SecurityExchange == true) os << " SecurityExchange=" << msg.SecurityExchange;
    if(msg.has_SecurityType == true) os << " SecurityType=" << msg.SecurityType;
    if(msg.has_Currency == true) os << " Currency=" << msg.Currency;
    if(msg.has_MainFraction == true) os << " MainFraction=" << msg.MainFraction;
    if(msg.has_TradingReferencePrice == true) os << " TradingReferencePrice=" << msg.TradingReferencePrice;
    if(msg.has_ClearedVolume == true) os << " ClearedVolume=" << msg.ClearedVolume;
    return os;
}

template <typename Handler>
class cme_ascii_secdef_parser
{

public:

    cme_ascii_secdef_parser( Handler *h): _h(h)
    {
        _tok =  new fix_tokenizer<cme_ascii_secdef_parser<Handler> >(this);
    }

    cme_ascii_secdef_parser()
    {
        delete _tok;
    }

    bool parse(char * s)
    {
        return _tok->parse(s);
    }

    void on_msg_start()
    {
        msg.reset();
    }

    void on_msg_field( int key, char * value )
    {
        msg.on_msg_field(key, value);
    }

    void on_msg_end()
    {
        (*_h)(msg);
    }

private:

    Handler *_h;
    fix_tokenizer<cme_ascii_secdef_parser<Handler> > * _tok;
    AsciiSecDefMsg msg;
};

}

#endif //ELEMENTS_SECDEF_PARSER_H
