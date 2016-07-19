#ifndef ELEMENTS_FIX_PARSER_H
#define ELEMENTS_FIX_PARSER_H

#include <assert.h>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <cstring>
#include <fix_tokenizer.h>
#include <iosfwd>

namespace elements
{

int MsgTypeToInt(std::string &s)
{
    assert(s.length() < 3);
    if( s.length() == 1 )
        return int(s[0]);
    else if ( s.length() == 2 )
        return 1000 * int(s[0]) + int(s[1]);
}

int MsgTypeToInt(const char * s)
{
    int len = strlen(s);
    assert( len < 3);
    if( len == 1 )
        return int(s[0]);
    else if ( len == 2 )
        return 1000 * int(s[0]) + int(s[1]);
}

enum FIELD_TYPE
{
    SIMPLE_FIELD = 1,
    UNKNOWN_FIELD = 2,
    GROUP_COUNT_FIELD =3,
    DUPLICATE_FIELD = 4,
    COMPONENT_FIELD = 5
};

struct msg_base
{

    FIELD_TYPE on_msg_field(int key, char * value)
    {
        if(!is_possible_field(key))
        {
            //std::cout << "UNKNOWN "  << key<< std::endl;
            return FIELD_TYPE::UNKNOWN_FIELD;
        }

        if(is_simple_field(key))
        {
            if(set_field(key, value))
            {
                //std::cout << "SIMPLE "<< key << std::endl;
                return FIELD_TYPE::SIMPLE_FIELD;
            }
            else
            {
                //std::cout << "DUPLICATE " << key << std::endl;
                return FIELD_TYPE::DUPLICATE_FIELD;
            }
        }


        if(is_group_count_field(key))
        {
            //std::cout << "GROUP_COUNT " << key << std::endl;
            prepare_group(key, value);
            return FIELD_TYPE::GROUP_COUNT_FIELD;
        }

        //std::cout << "COMPONENT "<< key << std::endl;
        prepare_component(key);
        return FIELD_TYPE::COMPONENT_FIELD;
    }

    bool is_possible_field( int key )
    {
        auto a = possible_fields();
        if(a.find(key) != a.end())
            return true;
        else
            return false;
    }

    bool is_simple_field( int key )
    {
        auto a = simple_fields();
        if(a.find(key) != a.end())
            return true;
        else
            return false;
    }

    virtual int get_component_id(int field_id ) = 0;
    virtual int get_group_component_id(int field_id ) = 0;
    virtual bool set_field(int key, char *value) = 0;
    virtual std::set<int> & possible_fields() = 0;
    virtual std::set<int> & simple_fields() = 0;
    virtual bool is_group_count_field(int) = 0;

    virtual void prepare_group(int key, char * value) = 0;
    virtual void prepare_component(int key) = 0;
    virtual bool advance_group() = 0;
    virtual void reset() = 0;

    msg_base * get_group() {
        return group;
    }
    msg_base * get_component() {
        return component;
    }

    bool is_grp = false;
    int grp_index = 0;
    int num_in_grp;
    int grp_field_id = -1;

    msg_base * group = nullptr;
    msg_base * component = nullptr;
};

enum FLD
{
    Account = 1,
    AdvId = 2,
    AdvRefID = 3,
    AdvSide = 4,
    AdvTransType = 5,
    AvgPx = 6,
    BeginSeqNo = 7,
    BeginString = 8,
    BodyLength = 9,
    CheckSum = 10,
    ClOrdID = 11,
    Commission = 12,
    CommType = 13,
    CumQty = 14,
    Currency = 15,
    EndSeqNo = 16,
    ExecID = 17,
    ExecInst = 18,
    ExecRefID = 19,
    HandlInst = 21,
    SecurityIDSource = 22,
    IOIID = 23,
    IOIQltyInd = 25,
    IOIRefID = 26,
    IOIQty = 27,
    IOITransType = 28,
    LastCapacity = 29,
    LastMkt = 30,
    LastPx = 31,
    LastQty = 32,
    NoLinesOfText = 33,
    MsgSeqNum = 34,
    MsgType = 35,
    NewSeqNo = 36,
    OrderID = 37,
    OrderQty = 38,
    OrdStatus = 39,
    OrdType = 40,
    OrigClOrdID = 41,
    OrigTime = 42,
    PossDupFlag = 43,
    Price = 44,
    RefSeqNum = 45,
    SecurityID = 48,
    SenderCompID = 49,
    SenderSubID = 50,
    SendingTime = 52,
    Quantity = 53,
    Side = 54,
    Symbol = 55,
    TargetCompID = 56,
    TargetSubID = 57,
    Text = 58,
    TimeInForce = 59,
    TransactTime = 60,
    Urgency = 61,
    ValidUntilTime = 62,
    SettlType = 63,
    SettlDate = 64,
    SymbolSfx = 65,
    ListID = 66,
    ListSeqNo = 67,
    TotNoOrders = 68,
    ListExecInst = 69,
    AllocID = 70,
    AllocTransType = 71,
    RefAllocID = 72,
    NoOrders = 73,
    AvgPxPrecision = 74,
    TradeDate = 75,
    PositionEffect = 77,
    NoAllocs = 78,
    AllocAccount = 79,
    AllocQty = 80,
    ProcessCode = 81,
    NoRpts = 82,
    RptSeq = 83,
    CxlQty = 84,
    NoDlvyInst = 85,
    AllocStatus = 87,
    AllocRejCode = 88,
    Signature = 89,
    SecureDataLen = 90,
    SecureData = 91,
    SignatureLength = 93,
    EmailType = 94,
    RawDataLength = 95,
    RawData = 96,
    PossResend = 97,
    EncryptMethod = 98,
    StopPx = 99,
    ExDestination = 100,
    CxlRejReason = 102,
    OrdRejReason = 103,
    IOIQualifier = 104,
    Issuer = 106,
    SecurityDesc = 107,
    HeartBtInt = 108,
    MinQty = 110,
    MaxFloor = 111,
    TestReqID = 112,
    ReportToExch = 113,
    LocateReqd = 114,
    OnBehalfOfCompID = 115,
    OnBehalfOfSubID = 116,
    QuoteID = 117,
    NetMoney = 118,
    SettlCurrAmt = 119,
    SettlCurrency = 120,
    ForexReq = 121,
    OrigSendingTime = 122,
    GapFillFlag = 123,
    NoExecs = 124,
    ExpireTime = 126,
    DKReason = 127,
    DeliverToCompID = 128,
    DeliverToSubID = 129,
    IOINaturalFlag = 130,
    QuoteReqID = 131,
    BidPx = 132,
    OfferPx = 133,
    BidSize = 134,
    OfferSize = 135,
    NoMiscFees = 136,
    MiscFeeAmt = 137,
    MiscFeeCurr = 138,
    MiscFeeType = 139,
    PrevClosePx = 140,
    ResetSeqNumFlag = 141,
    SenderLocationID = 142,
    TargetLocationID = 143,
    OnBehalfOfLocationID = 144,
    DeliverToLocationID = 145,
    NoRelatedSym = 146,
    Subject = 147,
    Headline = 148,
    URLLink = 149,
    ExecType = 150,
    LeavesQty = 151,
    CashOrderQty = 152,
    AllocAvgPx = 153,
    AllocNetMoney = 154,
    SettlCurrFxRate = 155,
    SettlCurrFxRateCalc = 156,
    NumDaysInterest = 157,
    AccruedInterestRate = 158,
    AccruedInterestAmt = 159,
    SettlInstMode = 160,
    AllocText = 161,
    SettlInstID = 162,
    SettlInstTransType = 163,
    EmailThreadID = 164,
    SettlInstSource = 165,
    SecurityType = 167,
    EffectiveTime = 168,
    StandInstDbType = 169,
    StandInstDbName = 170,
    StandInstDbID = 171,
    SettlDeliveryType = 172,
    BidSpotRate = 188,
    BidForwardPoints = 189,
    OfferSpotRate = 190,
    OfferForwardPoints = 191,
    OrderQty2 = 192,
    SettlDate2 = 193,
    LastSpotRate = 194,
    LastForwardPoints = 195,
    AllocLinkID = 196,
    AllocLinkType = 197,
    SecondaryOrderID = 198,
    NoIOIQualifiers = 199,
    MaturityMonthYear = 200,
    PutOrCall = 201,
    StrikePrice = 202,
    CoveredOrUncovered = 203,
    OptAttribute = 206,
    SecurityExchange = 207,
    NotifyBrokerOfCredit = 208,
    AllocHandlInst = 209,
    MaxShow = 210,
    PegOffsetValue = 211,
    XmlDataLen = 212,
    XmlData = 213,
    SettlInstRefID = 214,
    NoRoutingIDs = 215,
    RoutingType = 216,
    RoutingID = 217,
    Spread = 218,
    BenchmarkCurveCurrency = 220,
    BenchmarkCurveName = 221,
    BenchmarkCurvePoint = 222,
    CouponRate = 223,
    CouponPaymentDate = 224,
    IssueDate = 225,
    RepurchaseTerm = 226,
    RepurchaseRate = 227,
    Factor = 228,
    TradeOriginationDate = 229,
    ExDate = 230,
    ContractMultiplier = 231,
    NoStipulations = 232,
    StipulationType = 233,
    StipulationValue = 234,
    YieldType = 235,
    Yield = 236,
    TotalTakedown = 237,
    Concession = 238,
    RepoCollateralSecurityType = 239,
    RedemptionDate = 240,
    UnderlyingCouponPaymentDate = 241,
    UnderlyingIssueDate = 242,
    UnderlyingRepoCollateralSecurityType = 243,
    UnderlyingRepurchaseTerm = 244,
    UnderlyingRepurchaseRate = 245,
    UnderlyingFactor = 246,
    UnderlyingRedemptionDate = 247,
    LegCouponPaymentDate = 248,
    LegIssueDate = 249,
    LegRepoCollateralSecurityType = 250,
    LegRepurchaseTerm = 251,
    LegRepurchaseRate = 252,
    LegFactor = 253,
    LegRedemptionDate = 254,
    CreditRating = 255,
    UnderlyingCreditRating = 256,
    LegCreditRating = 257,
    TradedFlatSwitch = 258,
    BasisFeatureDate = 259,
    BasisFeaturePrice = 260,
    MDReqID = 262,
    SubscriptionRequestType = 263,
    MarketDepth = 264,
    MDUpdateType = 265,
    AggregatedBook = 266,
    NoMDEntryTypes = 267,
    NoMDEntries = 268,
    MDEntryType = 269,
    MDEntryPx = 270,
    MDEntrySize = 271,
    MDEntryDate = 272,
    MDEntryTime = 273,
    TickDirection = 274,
    MDMkt = 275,
    QuoteCondition = 276,
    TradeCondition = 277,
    MDEntryID = 278,
    MDUpdateAction = 279,
    MDEntryRefID = 280,
    MDReqRejReason = 281,
    MDEntryOriginator = 282,
    LocationID = 283,
    DeskID = 284,
    DeleteReason = 285,
    OpenCloseSettlFlag = 286,
    SellerDays = 287,
    MDEntryBuyer = 288,
    MDEntrySeller = 289,
    MDEntryPositionNo = 290,
    FinancialStatus = 291,
    CorporateAction = 292,
    DefBidSize = 293,
    DefOfferSize = 294,
    NoQuoteEntries = 295,
    NoQuoteSets = 296,
    QuoteStatus = 297,
    QuoteCancelType = 298,
    QuoteEntryID = 299,
    QuoteRejectReason = 300,
    QuoteResponseLevel = 301,
    QuoteSetID = 302,
    QuoteRequestType = 303,
    TotNoQuoteEntries = 304,
    UnderlyingSecurityIDSource = 305,
    UnderlyingIssuer = 306,
    UnderlyingSecurityDesc = 307,
    UnderlyingSecurityExchange = 308,
    UnderlyingSecurityID = 309,
    UnderlyingSecurityType = 310,
    UnderlyingSymbol = 311,
    UnderlyingSymbolSfx = 312,
    UnderlyingMaturityMonthYear = 313,
    UnderlyingPutOrCall = 315,
    UnderlyingStrikePrice = 316,
    UnderlyingOptAttribute = 317,
    UnderlyingCurrency = 318,
    SecurityReqID = 320,
    SecurityRequestType = 321,
    SecurityResponseID = 322,
    SecurityResponseType = 323,
    SecurityStatusReqID = 324,
    UnsolicitedIndicator = 325,
    SecurityTradingStatus = 326,
    HaltReasonInt = 327,
    InViewOfCommon = 328,
    DueToRelated = 329,
    BuyVolume = 330,
    SellVolume = 331,
    HighPx = 332,
    LowPx = 333,
    Adjustment = 334,
    TradSesReqID = 335,
    TradingSessionID = 336,
    ContraTrader = 337,
    TradSesMethod = 338,
    TradSesMode = 339,
    TradSesStatus = 340,
    TradSesStartTime = 341,
    TradSesOpenTime = 342,
    TradSesPreCloseTime = 343,
    TradSesCloseTime = 344,
    TradSesEndTime = 345,
    NumberOfOrders = 346,
    MessageEncoding = 347,
    EncodedIssuerLen = 348,
    EncodedIssuer = 349,
    EncodedSecurityDescLen = 350,
    EncodedSecurityDesc = 351,
    EncodedListExecInstLen = 352,
    EncodedListExecInst = 353,
    EncodedTextLen = 354,
    EncodedText = 355,
    EncodedSubjectLen = 356,
    EncodedSubject = 357,
    EncodedHeadlineLen = 358,
    EncodedHeadline = 359,
    EncodedAllocTextLen = 360,
    EncodedAllocText = 361,
    EncodedUnderlyingIssuerLen = 362,
    EncodedUnderlyingIssuer = 363,
    EncodedUnderlyingSecurityDescLen = 364,
    EncodedUnderlyingSecurityDesc = 365,
    AllocPrice = 366,
    QuoteSetValidUntilTime = 367,
    QuoteEntryRejectReason = 368,
    LastMsgSeqNumProcessed = 369,
    RefTagID = 371,
    RefMsgType = 372,
    SessionRejectReason = 373,
    BidRequestTransType = 374,
    ContraBroker = 375,
    ComplianceID = 376,
    SolicitedFlag = 377,
    ExecRestatementReason = 378,
    BusinessRejectRefID = 379,
    BusinessRejectReason = 380,
    GrossTradeAmt = 381,
    NoContraBrokers = 382,
    MaxMessageSize = 383,
    NoMsgTypes = 384,
    MsgDirection = 385,
    NoTradingSessions = 386,
    TotalVolumeTraded = 387,
    DiscretionInst = 388,
    DiscretionOffsetValue = 389,
    BidID = 390,
    ClientBidID = 391,
    ListName = 392,
    TotNoRelatedSym = 393,
    BidType = 394,
    NumTickets = 395,
    SideValue1 = 396,
    SideValue2 = 397,
    NoBidDescriptors = 398,
    BidDescriptorType = 399,
    BidDescriptor = 400,
    SideValueInd = 401,
    LiquidityPctLow = 402,
    LiquidityPctHigh = 403,
    LiquidityValue = 404,
    EFPTrackingError = 405,
    FairValue = 406,
    OutsideIndexPct = 407,
    ValueOfFutures = 408,
    LiquidityIndType = 409,
    WtAverageLiquidity = 410,
    ExchangeForPhysical = 411,
    OutMainCntryUIndex = 412,
    CrossPercent = 413,
    ProgRptReqs = 414,
    ProgPeriodInterval = 415,
    IncTaxInd = 416,
    NumBidders = 417,
    BidTradeType = 418,
    BasisPxType = 419,
    NoBidComponents = 420,
    Country = 421,
    TotNoStrikes = 422,
    PriceType = 423,
    DayOrderQty = 424,
    DayCumQty = 425,
    DayAvgPx = 426,
    GTBookingInst = 427,
    NoStrikes = 428,
    ListStatusType = 429,
    NetGrossInd = 430,
    ListOrderStatus = 431,
    ExpireDate = 432,
    ListExecInstType = 433,
    CxlRejResponseTo = 434,
    UnderlyingCouponRate = 435,
    UnderlyingContractMultiplier = 436,
    ContraTradeQty = 437,
    ContraTradeTime = 438,
    LiquidityNumSecurities = 441,
    MultiLegReportingType = 442,
    StrikeTime = 443,
    ListStatusText = 444,
    EncodedListStatusTextLen = 445,
    EncodedListStatusText = 446,
    PartyIDSource = 447,
    PartyID = 448,
    NetChgPrevDay = 451,
    PartyRole = 452,
    NoPartyIDs = 453,
    NoSecurityAltID = 454,
    SecurityAltID = 455,
    SecurityAltIDSource = 456,
    NoUnderlyingSecurityAltID = 457,
    UnderlyingSecurityAltID = 458,
    UnderlyingSecurityAltIDSource = 459,
    Product = 460,
    CFICode = 461,
    UnderlyingProduct = 462,
    UnderlyingCFICode = 463,
    TestMessageIndicator = 464,
    BookingRefID = 466,
    IndividualAllocID = 467,
    RoundingDirection = 468,
    RoundingModulus = 469,
    CountryOfIssue = 470,
    StateOrProvinceOfIssue = 471,
    LocaleOfIssue = 472,
    NoRegistDtls = 473,
    MailingDtls = 474,
    InvestorCountryOfResidence = 475,
    PaymentRef = 476,
    DistribPaymentMethod = 477,
    CashDistribCurr = 478,
    CommCurrency = 479,
    CancellationRights = 480,
    MoneyLaunderingStatus = 481,
    MailingInst = 482,
    TransBkdTime = 483,
    ExecPriceType = 484,
    ExecPriceAdjustment = 485,
    DateOfBirth = 486,
    TradeReportTransType = 487,
    CardHolderName = 488,
    CardNumber = 489,
    CardExpDate = 490,
    CardIssNum = 491,
    PaymentMethod = 492,
    RegistAcctType = 493,
    Designation = 494,
    TaxAdvantageType = 495,
    RegistRejReasonText = 496,
    FundRenewWaiv = 497,
    CashDistribAgentName = 498,
    CashDistribAgentCode = 499,
    CashDistribAgentAcctNumber = 500,
    CashDistribPayRef = 501,
    CashDistribAgentAcctName = 502,
    CardStartDate = 503,
    PaymentDate = 504,
    PaymentRemitterID = 505,
    RegistStatus = 506,
    RegistRejReasonCode = 507,
    RegistRefID = 508,
    RegistDtls = 509,
    NoDistribInsts = 510,
    RegistEmail = 511,
    DistribPercentage = 512,
    RegistID = 513,
    RegistTransType = 514,
    ExecValuationPoint = 515,
    OrderPercent = 516,
    OwnershipType = 517,
    NoContAmts = 518,
    ContAmtType = 519,
    ContAmtValue = 520,
    ContAmtCurr = 521,
    OwnerType = 522,
    PartySubID = 523,
    NestedPartyID = 524,
    NestedPartyIDSource = 525,
    SecondaryClOrdID = 526,
    SecondaryExecID = 527,
    OrderCapacity = 528,
    OrderRestrictions = 529,
    MassCancelRequestType = 530,
    MassCancelResponse = 531,
    MassCancelRejectReason = 532,
    TotalAffectedOrders = 533,
    NoAffectedOrders = 534,
    AffectedOrderID = 535,
    AffectedSecondaryOrderID = 536,
    QuoteType = 537,
    NestedPartyRole = 538,
    NoNestedPartyIDs = 539,
    TotalAccruedInterestAmt = 540,
    MaturityDate = 541,
    UnderlyingMaturityDate = 542,
    InstrRegistry = 543,
    CashMargin = 544,
    NestedPartySubID = 545,
    Scope = 546,
    MDImplicitDelete = 547,
    CrossID = 548,
    CrossType = 549,
    CrossPrioritization = 550,
    OrigCrossID = 551,
    NoSides = 552,
    Username = 553,
    Password = 554,
    NoLegs = 555,
    LegCurrency = 556,
    TotNoSecurityTypes = 557,
    NoSecurityTypes = 558,
    SecurityListRequestType = 559,
    SecurityRequestResult = 560,
    RoundLot = 561,
    MinTradeVol = 562,
    MultiLegRptTypeReq = 563,
    LegPositionEffect = 564,
    LegCoveredOrUncovered = 565,
    LegPrice = 566,
    TradSesStatusRejReason = 567,
    TradeRequestID = 568,
    TradeRequestType = 569,
    PreviouslyReported = 570,
    TradeReportID = 571,
    TradeReportRefID = 572,
    MatchStatus = 573,
    MatchType = 574,
    OddLot = 575,
    NoClearingInstructions = 576,
    ClearingInstruction = 577,
    TradeInputSource = 578,
    TradeInputDevice = 579,
    NoDates = 580,
    AccountType = 581,
    CustOrderCapacity = 582,
    ClOrdLinkID = 583,
    MassStatusReqID = 584,
    MassStatusReqType = 585,
    OrigOrdModTime = 586,
    LegSettlType = 587,
    LegSettlDate = 588,
    DayBookingInst = 589,
    BookingUnit = 590,
    PreallocMethod = 591,
    UnderlyingCountryOfIssue = 592,
    UnderlyingStateOrProvinceOfIssue = 593,
    UnderlyingLocaleOfIssue = 594,
    UnderlyingInstrRegistry = 595,
    LegCountryOfIssue = 596,
    LegStateOrProvinceOfIssue = 597,
    LegLocaleOfIssue = 598,
    LegInstrRegistry = 599,
    LegSymbol = 600,
    LegSymbolSfx = 601,
    LegSecurityID = 602,
    LegSecurityIDSource = 603,
    NoLegSecurityAltID = 604,
    LegSecurityAltID = 605,
    LegSecurityAltIDSource = 606,
    LegProduct = 607,
    LegCFICode = 608,
    LegSecurityType = 609,
    LegMaturityMonthYear = 610,
    LegMaturityDate = 611,
    LegStrikePrice = 612,
    LegOptAttribute = 613,
    LegContractMultiplier = 614,
    LegCouponRate = 615,
    LegSecurityExchange = 616,
    LegIssuer = 617,
    EncodedLegIssuerLen = 618,
    EncodedLegIssuer = 619,
    LegSecurityDesc = 620,
    EncodedLegSecurityDescLen = 621,
    EncodedLegSecurityDesc = 622,
    LegRatioQty = 623,
    LegSide = 624,
    TradingSessionSubID = 625,
    AllocType = 626,
    NoHops = 627,
    HopCompID = 628,
    HopSendingTime = 629,
    HopRefID = 630,
    MidPx = 631,
    BidYield = 632,
    MidYield = 633,
    OfferYield = 634,
    ClearingFeeIndicator = 635,
    WorkingIndicator = 636,
    LegLastPx = 637,
    PriorityIndicator = 638,
    PriceImprovement = 639,
    Price2 = 640,
    LastForwardPoints2 = 641,
    BidForwardPoints2 = 642,
    OfferForwardPoints2 = 643,
    RFQReqID = 644,
    MktBidPx = 645,
    MktOfferPx = 646,
    MinBidSize = 647,
    MinOfferSize = 648,
    QuoteStatusReqID = 649,
    LegalConfirm = 650,
    UnderlyingLastPx = 651,
    UnderlyingLastQty = 652,
    LegRefID = 654,
    ContraLegRefID = 655,
    SettlCurrBidFxRate = 656,
    SettlCurrOfferFxRate = 657,
    QuoteRequestRejectReason = 658,
    SideComplianceID = 659,
    AcctIDSource = 660,
    AllocAcctIDSource = 661,
    BenchmarkPrice = 662,
    BenchmarkPriceType = 663,
    ConfirmID = 664,
    ConfirmStatus = 665,
    ConfirmTransType = 666,
    ContractSettlMonth = 667,
    DeliveryForm = 668,
    LastParPx = 669,
    NoLegAllocs = 670,
    LegAllocAccount = 671,
    LegIndividualAllocID = 672,
    LegAllocQty = 673,
    LegAllocAcctIDSource = 674,
    LegSettlCurrency = 675,
    LegBenchmarkCurveCurrency = 676,
    LegBenchmarkCurveName = 677,
    LegBenchmarkCurvePoint = 678,
    LegBenchmarkPrice = 679,
    LegBenchmarkPriceType = 680,
    LegBidPx = 681,
    LegIOIQty = 682,
    NoLegStipulations = 683,
    LegOfferPx = 684,
    LegOrderQty = 685,
    LegPriceType = 686,
    LegQty = 687,
    LegStipulationType = 688,
    LegStipulationValue = 689,
    LegSwapType = 690,
    Pool = 691,
    QuotePriceType = 692,
    QuoteRespID = 693,
    QuoteRespType = 694,
    QuoteQualifier = 695,
    YieldRedemptionDate = 696,
    YieldRedemptionPrice = 697,
    YieldRedemptionPriceType = 698,
    BenchmarkSecurityID = 699,
    ReversalIndicator = 700,
    YieldCalcDate = 701,
    NoPositions = 702,
    PosType = 703,
    LongQty = 704,
    ShortQty = 705,
    PosQtyStatus = 706,
    PosAmtType = 707,
    PosAmt = 708,
    PosTransType = 709,
    PosReqID = 710,
    NoUnderlyings = 711,
    PosMaintAction = 712,
    OrigPosReqRefID = 713,
    PosMaintRptRefID = 714,
    ClearingBusinessDate = 715,
    SettlSessID = 716,
    SettlSessSubID = 717,
    AdjustmentType = 718,
    ContraryInstructionIndicator = 719,
    PriorSpreadIndicator = 720,
    PosMaintRptID = 721,
    PosMaintStatus = 722,
    PosMaintResult = 723,
    PosReqType = 724,
    ResponseTransportType = 725,
    ResponseDestination = 726,
    TotalNumPosReports = 727,
    PosReqResult = 728,
    PosReqStatus = 729,
    SettlPrice = 730,
    SettlPriceType = 731,
    UnderlyingSettlPrice = 732,
    UnderlyingSettlPriceType = 733,
    PriorSettlPrice = 734,
    NoQuoteQualifiers = 735,
    AllocSettlCurrency = 736,
    AllocSettlCurrAmt = 737,
    InterestAtMaturity = 738,
    LegDatedDate = 739,
    LegPool = 740,
    AllocInterestAtMaturity = 741,
    AllocAccruedInterestAmt = 742,
    DeliveryDate = 743,
    AssignmentMethod = 744,
    AssignmentUnit = 745,
    OpenInterest = 746,
    ExerciseMethod = 747,
    TotNumTradeReports = 748,
    TradeRequestResult = 749,
    TradeRequestStatus = 750,
    TradeReportRejectReason = 751,
    SideMultiLegReportingType = 752,
    NoPosAmt = 753,
    AutoAcceptIndicator = 754,
    AllocReportID = 755,
    NoNested2PartyIDs = 756,
    Nested2PartyID = 757,
    Nested2PartyIDSource = 758,
    Nested2PartyRole = 759,
    Nested2PartySubID = 760,
    BenchmarkSecurityIDSource = 761,
    SecuritySubType = 762,
    UnderlyingSecuritySubType = 763,
    LegSecuritySubType = 764,
    AllowableOneSidednessPct = 765,
    AllowableOneSidednessValue = 766,
    AllowableOneSidednessCurr = 767,
    NoTrdRegTimestamps = 768,
    TrdRegTimestamp = 769,
    TrdRegTimestampType = 770,
    TrdRegTimestampOrigin = 771,
    ConfirmRefID = 772,
    ConfirmType = 773,
    ConfirmRejReason = 774,
    BookingType = 775,
    IndividualAllocRejCode = 776,
    SettlInstMsgID = 777,
    NoSettlInst = 778,
    LastUpdateTime = 779,
    AllocSettlInstType = 780,
    NoSettlPartyIDs = 781,
    SettlPartyID = 782,
    SettlPartyIDSource = 783,
    SettlPartyRole = 784,
    SettlPartySubID = 785,
    SettlPartySubIDType = 786,
    DlvyInstType = 787,
    TerminationType = 788,
    NextExpectedMsgSeqNum = 789,
    OrdStatusReqID = 790,
    SettlInstReqID = 791,
    SettlInstReqRejCode = 792,
    SecondaryAllocID = 793,
    AllocReportType = 794,
    AllocReportRefID = 795,
    AllocCancReplaceReason = 796,
    CopyMsgIndicator = 797,
    AllocAccountType = 798,
    OrderAvgPx = 799,
    OrderBookingQty = 800,
    NoSettlPartySubIDs = 801,
    NoPartySubIDs = 802,
    PartySubIDType = 803,
    NoNestedPartySubIDs = 804,
    NestedPartySubIDType = 805,
    NoNested2PartySubIDs = 806,
    Nested2PartySubIDType = 807,
    AllocIntermedReqType = 808,
    UnderlyingPx = 810,
    PriceDelta = 811,
    ApplQueueMax = 812,
    ApplQueueDepth = 813,
    ApplQueueResolution = 814,
    ApplQueueAction = 815,
    NoAltMDSource = 816,
    AltMDSourceID = 817,
    SecondaryTradeReportID = 818,
    AvgPxIndicator = 819,
    TradeLinkID = 820,
    OrderInputDevice = 821,
    UnderlyingTradingSessionID = 822,
    UnderlyingTradingSessionSubID = 823,
    TradeLegRefID = 824,
    ExchangeRule = 825,
    TradeAllocIndicator = 826,
    ExpirationCycle = 827,
    TrdType = 828,
    TrdSubType = 829,
    TransferReason = 830,
    TotNumAssignmentReports = 832,
    AsgnRptID = 833,
    ThresholdAmount = 834,
    PegMoveType = 835,
    PegOffsetType = 836,
    PegLimitType = 837,
    PegRoundDirection = 838,
    PeggedPrice = 839,
    PegScope = 840,
    DiscretionMoveType = 841,
    DiscretionOffsetType = 842,
    DiscretionLimitType = 843,
    DiscretionRoundDirection = 844,
    DiscretionPrice = 845,
    DiscretionScope = 846,
    TargetStrategy = 847,
    TargetStrategyParameters = 848,
    ParticipationRate = 849,
    TargetStrategyPerformance = 850,
    LastLiquidityInd = 851,
    PublishTrdIndicator = 852,
    ShortSaleReason = 853,
    QtyType = 854,
    SecondaryTrdType = 855,
    TradeReportType = 856,
    AllocNoOrdersType = 857,
    SharedCommission = 858,
    ConfirmReqID = 859,
    AvgParPx = 860,
    ReportedPx = 861,
    NoCapacities = 862,
    OrderCapacityQty = 863,
    NoEvents = 864,
    EventType = 865,
    EventDate = 866,
    EventPx = 867,
    EventText = 868,
    PctAtRisk = 869,
    NoInstrAttrib = 870,
    InstrAttribType = 871,
    InstrAttribValue = 872,
    DatedDate = 873,
    InterestAccrualDate = 874,
    CPProgram = 875,
    CPRegType = 876,
    UnderlyingCPProgram = 877,
    UnderlyingCPRegType = 878,
    UnderlyingQty = 879,
    TrdMatchID = 880,
    SecondaryTradeReportRefID = 881,
    UnderlyingDirtyPrice = 882,
    UnderlyingEndPrice = 883,
    UnderlyingStartValue = 884,
    UnderlyingCurrentValue = 885,
    UnderlyingEndValue = 886,
    NoUnderlyingStips = 887,
    UnderlyingStipType = 888,
    UnderlyingStipValue = 889,
    MaturityNetMoney = 890,
    MiscFeeBasis = 891,
    TotNoAllocs = 892,
    LastFragment = 893,
    CollReqID = 894,
    CollAsgnReason = 895,
    CollInquiryQualifier = 896,
    NoTrades = 897,
    MarginRatio = 898,
    MarginExcess = 899,
    TotalNetValue = 900,
    CashOutstanding = 901,
    CollAsgnID = 902,
    CollAsgnTransType = 903,
    CollRespID = 904,
    CollAsgnRespType = 905,
    CollAsgnRejectReason = 906,
    CollAsgnRefID = 907,
    CollRptID = 908,
    CollInquiryID = 909,
    CollStatus = 910,
    TotNumReports = 911,
    LastRptRequested = 912,
    AgreementDesc = 913,
    AgreementID = 914,
    AgreementDate = 915,
    StartDate = 916,
    EndDate = 917,
    AgreementCurrency = 918,
    DeliveryType = 919,
    EndAccruedInterestAmt = 920,
    StartCash = 921,
    EndCash = 922,
    UserRequestID = 923,
    UserRequestType = 924,
    NewPassword = 925,
    UserStatus = 926,
    UserStatusText = 927,
    StatusValue = 928,
    StatusText = 929,
    RefCompID = 930,
    RefSubID = 931,
    NetworkResponseID = 932,
    NetworkRequestID = 933,
    LastNetworkResponseID = 934,
    NetworkRequestType = 935,
    NoCompIDs = 936,
    NetworkStatusResponseType = 937,
    NoCollInquiryQualifier = 938,
    TrdRptStatus = 939,
    AffirmStatus = 940,
    UnderlyingStrikeCurrency = 941,
    LegStrikeCurrency = 942,
    TimeBracket = 943,
    CollAction = 944,
    CollInquiryStatus = 945,
    CollInquiryResult = 946,
    StrikeCurrency = 947,
    NoNested3PartyIDs = 948,
    Nested3PartyID = 949,
    Nested3PartyIDSource = 950,
    Nested3PartyRole = 951,
    NoNested3PartySubIDs = 952,
    Nested3PartySubID = 953,
    Nested3PartySubIDType = 954,
    LegContractSettlMonth = 955,
    LegInterestAccrualDate = 956,
    NoStrategyParameters = 957,
    StrategyParameterName = 958,
    StrategyParameterType = 959,
    StrategyParameterValue = 960,
    HostCrossID = 961,
    SideTimeInForce = 962,
    MDReportID = 963,
    SecurityReportID = 964,
    SecurityStatusField = 965,
    SettleOnOpenFlag = 966,
    StrikeMultiplier = 967,
    StrikeValue = 968,
    MinPriceIncrement = 969,
    PositionLimit = 970,
    NTPositionLimit = 971,
    UnderlyingAllocationPercent = 972,
    UnderlyingCashAmount = 973,
    UnderlyingCashType = 974,
    UnderlyingSettlementType = 975,
    QuantityDate = 976,
    ContIntRptID = 977,
    LateIndicator = 978,
    InputSource = 979,
    SecurityUpdateAction = 980,
    NoExpiration = 981,
    ExpirationQtyType = 982,
    ExpQty = 983,
    NoUnderlyingAmounts = 984,
    UnderlyingPayAmount = 985,
    UnderlyingCollectAmount = 986,
    UnderlyingSettlementDate = 987,
    UnderlyingSettlementStatus = 988,
    SecondaryIndividualAllocID = 989,
    LegReportID = 990,
    RndPx = 991,
    IndividualAllocType = 992,
    AllocCustomerCapacity = 993,
    TierCode = 994,
    UnitOfMeasure = 996,
    TimeUnit = 997,
    UnderlyingUnitOfMeasure = 998,
    LegUnitOfMeasure = 999,
    UnderlyingTimeUnit = 1000,
    LegTimeUnit = 1001,
    AllocMethod = 1002,
    TradeID = 1003,
    SideTradeReportID = 1005,
    SideFillStationCd = 1006,
    SideReasonCd = 1007,
    SideTrdSubTyp = 1008,
    SideLastQty = 1009,
    MessageEventSource = 1011,
    SideTrdRegTimestamp = 1012,
    SideTrdRegTimestampType = 1013,
    SideTrdRegTimestampSrc = 1014,
    AsOfIndicator = 1015,
    NoSideTrdRegTS = 1016,
    LegOptionRatio = 1017,
    NoInstrumentParties = 1018,
    InstrumentPartyID = 1019,
    TradeVolume = 1020,
    MDBookType = 1021,
    MDFeedType = 1022,
    MDPriceLevel = 1023,
    MDOriginType = 1024,
    FirstPx = 1025,
    MDEntrySpotRate = 1026,
    MDEntryForwardPoints = 1027,
    ManualOrderIndicator = 1028,
    CustDirectedOrder = 1029,
    ReceivedDeptID = 1030,
    CustOrderHandlingInst = 1031,
    OrderHandlingInstSource = 1032,
    DeskType = 1033,
    DeskTypeSource = 1034,
    DeskOrderHandlingInst = 1035,
    ExecAckStatus = 1036,
    UnderlyingDeliveryAmount = 1037,
    UnderlyingCapValue = 1038,
    UnderlyingSettlMethod = 1039,
    SecondaryTradeID = 1040,
    FirmTradeID = 1041,
    SecondaryFirmTradeID = 1042,
    CollApplType = 1043,
    UnderlyingAdjustedQuantity = 1044,
    UnderlyingFXRate = 1045,
    UnderlyingFXRateCalc = 1046,
    AllocPositionEffect = 1047,
    DealingCapacity = 1048,
    InstrmtAssignmentMethod = 1049,
    InstrumentPartyIDSource = 1050,
    InstrumentPartyRole = 1051,
    NoInstrumentPartySubIDs = 1052,
    InstrumentPartySubID = 1053,
    InstrumentPartySubIDType = 1054,
    PositionCurrency = 1055,
    CalculatedCcyLastQty = 1056,
    AggressorIndicator = 1057,
    NoUndlyInstrumentParties = 1058,
    UnderlyingInstrumentPartyID = 1059,
    UnderlyingInstrumentPartyIDSource = 1060,
    UnderlyingInstrumentPartyRole = 1061,
    NoUndlyInstrumentPartySubIDs = 1062,
    UnderlyingInstrumentPartySubID = 1063,
    UnderlyingInstrumentPartySubIDType = 1064,
    BidSwapPoints = 1065,
    OfferSwapPoints = 1066,
    LegBidForwardPoints = 1067,
    LegOfferForwardPoints = 1068,
    SwapPoints = 1069,
    MDQuoteType = 1070,
    LastSwapPoints = 1071,
    SideGrossTradeAmt = 1072,
    LegLastForwardPoints = 1073,
    LegCalculatedCcyLastQty = 1074,
    LegGrossTradeAmt = 1075,
    MaturityTime = 1079,
    RefOrderID = 1080,
    RefOrderIDSource = 1081,
    SecondaryDisplayQty = 1082,
    DisplayWhen = 1083,
    DisplayMethod = 1084,
    DisplayLowQty = 1085,
    DisplayHighQty = 1086,
    DisplayMinIncr = 1087,
    RefreshQty = 1088,
    MatchIncrement = 1089,
    MaxPriceLevels = 1090,
    PreTradeAnonymity = 1091,
    PriceProtectionScope = 1092,
    LotType = 1093,
    PegPriceType = 1094,
    PeggedRefPrice = 1095,
    PegSecurityIDSource = 1096,
    PegSecurityID = 1097,
    PegSymbol = 1098,
    PegSecurityDesc = 1099,
    TriggerType = 1100,
    TriggerAction = 1101,
    TriggerPrice = 1102,
    TriggerSymbol = 1103,
    TriggerSecurityID = 1104,
    TriggerSecurityIDSource = 1105,
    TriggerSecurityDesc = 1106,
    TriggerPriceType = 1107,
    TriggerPriceTypeScope = 1108,
    TriggerPriceDirection = 1109,
    TriggerNewPrice = 1110,
    TriggerOrderType = 1111,
    TriggerNewQty = 1112,
    TriggerTradingSessionID = 1113,
    TriggerTradingSessionSubID = 1114,
    OrderCategory = 1115,
    NoRootPartyIDs = 1116,
    RootPartyID = 1117,
    RootPartyIDSource = 1118,
    RootPartyRole = 1119,
    NoRootPartySubIDs = 1120,
    RootPartySubID = 1121,
    RootPartySubIDType = 1122,
    TradeHandlingInstr = 1123,
    OrigTradeHandlingInstr = 1124,
    OrigTradeDate = 1125,
    OrigTradeID = 1126,
    OrigSecondaryTradeID = 1127,
    ApplVerID = 1128,
    CstmApplVerID = 1129,
    RefApplVerID = 1130,
    RefCstmApplVerID = 1131,
    TZTransactTime = 1132,
    ExDestinationIDSource = 1133,
    ReportedPxDiff = 1134,
    RptSys = 1135,
    AllocClearingFeeIndicator = 1136,
    DefaultApplVerID = 1137,
    DisplayQty = 1138,
    ExchangeSpecialInstructions = 1139,
    MaxTradeVol = 1140,
    NoMDFeedTypes = 1141,
    MatchAlgorithm = 1142,
    MaxPriceVariation = 1143,
    ImpliedMarketIndicator = 1144,
    EventTime = 1145,
    MinPriceIncrementAmount = 1146,
    UnitOfMeasureQty = 1147,
    LowLimitPrice = 1148,
    HighLimitPrice = 1149,
    TradingReferencePrice = 1150,
    SecurityGroup = 1151,
    LegNumber = 1152,
    SettlementCycleNo = 1153,
    SideCurrency = 1154,
    SideSettlCurrency = 1155,
    ApplExtID = 1156,
    CcyAmt = 1157,
    NoSettlDetails = 1158,
    SettlObligMode = 1159,
    SettlObligMsgID = 1160,
    SettlObligID = 1161,
    SettlObligTransType = 1162,
    SettlObligRefID = 1163,
    SettlObligSource = 1164,
    NoSettlOblig = 1165,
    QuoteMsgID = 1166,
    QuoteEntryStatus = 1167,
    TotNoCxldQuotes = 1168,
    TotNoAccQuotes = 1169,
    TotNoRejQuotes = 1170,
    PrivateQuote = 1171,
    RespondentType = 1172,
    MDSubBookType = 1173,
    SecurityTradingEvent = 1174,
    NoStatsIndicators = 1175,
    StatsType = 1176,
    NoOfSecSizes = 1177,
    MDSecSizeType = 1178,
    MDSecSize = 1179,
    ApplID = 1180,
    ApplSeqNum = 1181,
    ApplBegSeqNum = 1182,
    ApplEndSeqNum = 1183,
    SecurityXMLLen = 1184,
    SecurityXML2 = 1185,
    SecurityXMLSchema = 1186,
    RefreshIndicator = 1187,
    Volatility = 1188,
    TimeToExpiration = 1189,
    RiskFreeRate = 1190,
    PriceUnitOfMeasure = 1191,
    PriceUnitOfMeasureQty = 1192,
    SettlMethod = 1193,
    ExerciseStyle = 1194,
    OptPayoutAmount = 1195,
    PriceQuoteMethod = 1196,
    ValuationMethod = 1197,
    ListMethod = 1198,
    CapPrice = 1199,
    FloorPrice = 1200,
    NoStrikeRules = 1201,
    StartStrikePxRange = 1202,
    EndStrikePxRange = 1203,
    StrikeIncrement = 1204,
    NoTickRules = 1205,
    StartTickPriceRange = 1206,
    EndTickPriceRange = 1207,
    TickIncrement = 1208,
    TickRuleType = 1209,
    NestedInstrAttribType = 1210,
    NestedInstrAttribValue = 1211,
    LegMaturityTime = 1212,
    UnderlyingMaturityTime = 1213,
    DerivativeSymbol = 1214,
    DerivativeSymbolSfx = 1215,
    DerivativeSecurityID = 1216,
    DerivativeSecurityIDSource = 1217,
    NoDerivativeSecurityAltID = 1218,
    DerivativeSecurityAltID = 1219,
    DerivativeSecurityAltIDSource = 1220,
    SecondaryLowLimitPrice = 1221,
    MaturityRuleID = 1222,
    StrikeRuleID = 1223,
    LegUnitOfMeasureQty = 1224,
    DerivativeOptPayAmount = 1225,
    EndMaturityMonthYear = 1226,
    ProductComplex = 1227,
    DerivativeProductComplex = 1228,
    MaturityMonthYearIncrement = 1229,
    SecondaryHighLimitPrice = 1230,
    MinLotSize = 1231,
    NoExecInstRules = 1232,
    NoLotTypeRules = 1234,
    NoMatchRules = 1235,
    NoMaturityRules = 1236,
    NoOrdTypeRules = 1237,
    NoTimeInForceRules = 1239,
    SecondaryTradingReferencePrice = 1240,
    StartMaturityMonthYear = 1241,
    FlexProductEligibilityIndicator = 1242,
    DerivFlexProductEligibilityIndicator = 1243,
    FlexibleIndicator = 1244,
    TradingCurrency = 1245,
    DerivativeProduct = 1246,
    DerivativeSecurityGroup = 1247,
    DerivativeCFICode = 1248,
    DerivativeSecurityType = 1249,
    DerivativeSecuritySubType = 1250,
    DerivativeMaturityMonthYear = 1251,
    DerivativeMaturityDate = 1252,
    DerivativeMaturityTime = 1253,
    DerivativeSettleOnOpenFlag = 1254,
    DerivativeInstrmtAssignmentMethod = 1255,
    DerivativeSecurityStatus = 1256,
    DerivativeInstrRegistry = 1257,
    DerivativeCountryOfIssue = 1258,
    DerivativeStateOrProvinceOfIssue = 1259,
    DerivativeLocaleOfIssue = 1260,
    DerivativeStrikePrice = 1261,
    DerivativeStrikeCurrency = 1262,
    DerivativeStrikeMultiplier = 1263,
    DerivativeStrikeValue = 1264,
    DerivativeOptAttribute = 1265,
    DerivativeContractMultiplier = 1266,
    DerivativeMinPriceIncrement = 1267,
    DerivativeMinPriceIncrementAmount = 1268,
    DerivativeUnitOfMeasure = 1269,
    DerivativeUnitOfMeasureQty = 1270,
    DerivativeTimeUnit = 1271,
    DerivativeSecurityExchange = 1272,
    DerivativePositionLimit = 1273,
    DerivativeNTPositionLimit = 1274,
    DerivativeIssuer = 1275,
    DerivativeIssueDate = 1276,
    DerivativeEncodedIssuerLen = 1277,
    DerivativeEncodedIssuer = 1278,
    DerivativeSecurityDesc = 1279,
    DerivativeEncodedSecurityDescLen = 1280,
    DerivativeEncodedSecurityDesc = 1281,
    DerivativeSecurityXMLLen = 1282,
    DerivativeSecurityXML2 = 1283,
    DerivativeSecurityXMLSchema = 1284,
    DerivativeContractSettlMonth = 1285,
    NoDerivativeEvents = 1286,
    DerivativeEventType = 1287,
    DerivativeEventDate = 1288,
    DerivativeEventTime = 1289,
    DerivativeEventPx = 1290,
    DerivativeEventText = 1291,
    NoDerivativeInstrumentParties = 1292,
    DerivativeInstrumentPartyID = 1293,
    DerivativeInstrumentPartyIDSource = 1294,
    DerivativeInstrumentPartyRole = 1295,
    NoDerivativeInstrumentPartySubIDs = 1296,
    DerivativeInstrumentPartySubID = 1297,
    DerivativeInstrumentPartySubIDType = 1298,
    DerivativeExerciseStyle = 1299,
    MarketSegmentID = 1300,
    MarketID = 1301,
    MaturityMonthYearIncrementUnits = 1302,
    MaturityMonthYearFormat = 1303,
    StrikeExerciseStyle = 1304,
    SecondaryPriceLimitType = 1305,
    PriceLimitType = 1306,
    ExecInstValue = 1308,
    NoTradingSessionRules = 1309,
    NoMarketSegments = 1310,
    NoDerivativeInstrAttrib = 1311,
    NoNestedInstrAttrib = 1312,
    DerivativeInstrAttribType = 1313,
    DerivativeInstrAttribValue = 1314,
    DerivativePriceUnitOfMeasure = 1315,
    DerivativePriceUnitOfMeasureQty = 1316,
    DerivativeSettlMethod = 1317,
    DerivativePriceQuoteMethod = 1318,
    DerivativeValuationMethod = 1319,
    DerivativeListMethod = 1320,
    DerivativeCapPrice = 1321,
    DerivativeFloorPrice = 1322,
    DerivativePutOrCall = 1323,
    ListUpdateAction = 1324,
    ParentMktSegmID = 1325,
    TradingSessionDesc = 1326,
    TradSesUpdateAction = 1327,
    RejectText = 1328,
    FeeMultiplier = 1329,
    UnderlyingLegSymbol = 1330,
    UnderlyingLegSymbolSfx = 1331,
    UnderlyingLegSecurityID = 1332,
    UnderlyingLegSecurityIDSource = 1333,
    NoUnderlyingLegSecurityAltID = 1334,
    UnderlyingLegSecurityAltID = 1335,
    UnderlyingLegSecurityAltIDSource = 1336,
    UnderlyingLegSecurityType = 1337,
    UnderlyingLegSecuritySubType = 1338,
    UnderlyingLegMaturityMonthYear = 1339,
    UnderlyingLegStrikePrice = 1340,
    UnderlyingLegSecurityExchange = 1341,
    NoOfLegUnderlyings = 1342,
    UnderlyingLegPutOrCall = 1343,
    UnderlyingLegCFICode = 1344,
    UnderlyingLegMaturityDate = 1345,
    ApplReqID = 1346,
    ApplReqType = 1347,
    ApplResponseType = 1348,
    ApplTotalMessageCount = 1349,
    ApplLastSeqNum = 1350,
    NoApplIDs = 1351,
    ApplResendFlag = 1352,
    ApplResponseID = 1353,
    ApplResponseError = 1354,
    RefApplID = 1355,
    ApplReportID = 1356,
    RefApplLastSeqNum = 1357,
    LegPutOrCall = 1358,
    TotNoFills = 1361,
    NoFills = 1362,
    FillExecID = 1363,
    FillPx = 1364,
    FillQty = 1365,
    LegAllocID = 1366,
    LegAllocSettlCurrency = 1367,
    TradSesEvent = 1368,
    MassActionReportID = 1369,
    NoNotAffectedOrders = 1370,
    NotAffectedOrderID = 1371,
    NotAffOrigClOrdID = 1372,
    MassActionType = 1373,
    MassActionScope = 1374,
    MassActionResponse = 1375,
    MassActionRejectReason = 1376,
    MultilegModel = 1377,
    MultilegPriceMethod = 1378,
    LegVolatility = 1379,
    DividendYield = 1380,
    LegDividendYield = 1381,
    CurrencyRatio = 1382,
    LegCurrencyRatio = 1383,
    LegExecInst = 1384,
    ContingencyType = 1385,
    ListRejectReason = 1386,
    NoTrdRepIndicators = 1387,
    TrdRepPartyRole = 1388,
    TrdRepIndicator = 1389,
    TradePublishIndicator = 1390,
    UnderlyingLegOptAttribute = 1391,
    UnderlyingLegSecurityDesc = 1392,
    MarketReqID = 1393,
    MarketReportID = 1394,
    MarketUpdateAction = 1395,
    MarketSegmentDesc = 1396,
    EncodedMktSegmDescLen = 1397,
    EncodedMktSegmDesc = 1398,
    ApplNewSeqNum = 1399,
    EncryptedPasswordMethod = 1400,
    EncryptedPasswordLen = 1401,
    EncryptedPassword = 1402,
    EncryptedNewPasswordLen = 1403,
    EncryptedNewPassword = 1404,
    UnderlyingLegMaturityTime = 1405,
    RefApplExtID = 1406,
    DefaultApplExtID = 1407,
    DefaultCstmApplVerID = 1408,
    SessionStatus = 1409,
    DefaultVerIndicator = 1410,
    Nested4PartySubIDType = 1411,
    Nested4PartySubID = 1412,
    NoNested4PartySubIDs = 1413,
    NoNested4PartyIDs = 1414,
    Nested4PartyID = 1415,
    Nested4PartyIDSource = 1416,
    Nested4PartyRole = 1417,
    LegLastQty = 1418,
    UnderlyingExerciseStyle = 1419,
    LegExerciseStyle = 1420,
    LegPriceUnitOfMeasure = 1421,
    LegPriceUnitOfMeasureQty = 1422,
    UnderlyingUnitOfMeasureQty = 1423,
    UnderlyingPriceUnitOfMeasure = 1424,
    UnderlyingPriceUnitOfMeasureQty = 1425,
    ApplReportType = 1426,
    SideExecID = 1427,
    OrderDelay = 1428,
    OrderDelayUnit = 1429,
    VenueType = 1430,
    RefOrdIDReason = 1431,
    OrigCustOrderCapacity = 1432,
    RefApplReqID = 1433,
    ModelType = 1434,
    ContractMultiplierUnit = 1435,
    LegContractMultiplierUnit = 1436,
    UnderlyingContractMultiplierUnit = 1437,
    DerivativeContractMultiplierUnit = 1438,
    FlowScheduleType = 1439,
    LegFlowScheduleType = 1440,
    UnderlyingFlowScheduleType = 1441,
    DerivativeFlowScheduleType = 1442,
    FillLiquidityInd = 1443,
    SideLiquidityInd = 1444,
    NoRateSources = 1445,
    RateSource2 = 1446,
    RateSourceType = 1447,
    ReferencePage = 1448,
    RestructuringType = 1449,
    Seniority = 1450,
    NotionalPercentageOutstanding = 1451,
    OriginalNotionalPercentageOutstanding = 1452,
    UnderlyingRestructuringType = 1453,
    UnderlyingSeniority = 1454,
    UnderlyingNotionalPercentageOutstanding = 1455,
    UnderlyingOriginalNotionalPercentageOutstanding = 1456,
    AttachmentPoint = 1457,
    DetachmentPoint = 1458,
    UnderlyingAttachmentPoint = 1459,
    UnderlyingDetachmentPoint = 1460,
    NoTargetPartyIDs = 1461,
    TargetPartyID = 1462,
    TargetPartyIDSource = 1463,
    TargetPartyRole = 1464,
    SecurityListID = 1465,
    SecurityListRefID = 1466,
    SecurityListDesc = 1467,
    EncodedSecurityListDescLen = 1468,
    EncodedSecurityListDesc = 1469,
    SecurityListType = 1470,
    SecurityListTypeSource = 1471,
    NewsID = 1472,
    NewsCategory = 1473,
    LanguageCode = 1474,
    NoNewsRefIDs = 1475,
    NewsRefID = 1476,
    NewsRefType = 1477,
    StrikePriceDeterminationMethod = 1478,
    StrikePriceBoundaryMethod = 1479,
    StrikePriceBoundaryPrecision = 1480,
    UnderlyingPriceDeterminationMethod = 1481,
    OptPayoutType = 1482,
    NoComplexEvents = 1483,
    ComplexEventType = 1484,
    ComplexOptPayoutAmount = 1485,
    ComplexEventPrice = 1486,
    ComplexEventPriceBoundaryMethod = 1487,
    ComplexEventPriceBoundaryPrecision = 1488,
    ComplexEventPriceTimeType = 1489,
    ComplexEventCondition = 1490,
    NoComplexEventDates = 1491,
    ComplexEventStartDate = 1492,
    ComplexEventEndDate = 1493,
    NoComplexEventTimes = 1494,
    ComplexEventStartTime = 1495,
    ComplexEventEndTime = 1496,
    StreamAsgnReqID = 1497,
    StreamAsgnReqType = 1498,
    NoAsgnReqs = 1499,
    MDStreamID = 1500,
    StreamAsgnRptID = 1501,
    StreamAsgnRejReason = 1502,
    StreamAsgnAckType = 1503,
    RelSymTransactTime = 1504,
    StreamAsgnType = 1617,
    TradingReferenceDate = 5796,
    LegSecurityGroup = 5795,
    AggressorSide = 5797,
    MatchEventIndicator = 5799,
    Asset = 6937,
    UserDefinedInstrument = 9779,
    MDSecurityTradingStatus = 1682,
    MinCabPrice = 9850,
    PriceRatio = 5770,
    DisplayFactor = 9787,
    TickRule = 6350,
    PriceDisplayFormat = 9800,
    PricingModel = 9853,
    DecayQty = 5818,
    DecayStartDate = 5819,
    OriginalContractSize = 5849,
    ClearedVolume = 5791,
    OpenInterestQty = 5792,
    FixingBracket = 5790
};
enum MSG
{
    XXMarketDataIncrementalRefresh = 88,
    XXSecurityDefinition = 100,
    XXSecurityStatus = 102
};

enum COMP
{
    XXAttrbGrp = 0,
    XXInstrmtLegGrp = 1,
    XXLotTypeRules = 2,
    XXEvntGrp = 3,
    XXMDIncGrp = 4,
    XXSecAltIDGrp = 5,
    XXMarketDataFeedTypes = 6,
    XXUndInstrmtGrp = 7
};


struct AttrbGrp_Type : public msg_base
{
    char *                                   InstrAttribType;
    char *                                   InstrAttribValue;
    bool has_InstrAttribType;
    bool has_InstrAttribValue;
    virtual void reset()
    {
        has_InstrAttribType = false;
        has_InstrAttribValue = false;
    }

    std::set<int> poss_fields = {871,872};
    std::set<int> sim_fields = {871,872};
    std::map<int,int> field_to_comp = {};
    std::map<int,int> field_to_group_comp = {};
    std::set<int> group_count_fields = {};
    virtual std::set<int> & possible_fields() {
        return poss_fields;
    }
    virtual std::set<int> & simple_fields() {
        return sim_fields;
    }
    virtual int get_component_id( int field_id ) {
        assert( field_to_comp.find(field_id) != field_to_comp.end());
        return field_to_comp[field_id];
    }
    virtual int get_group_component_id( int field_id ) {
        assert(field_to_group_comp.find(field_id) != field_to_group_comp.end());
        return field_to_group_comp[field_id];
    }
    virtual bool is_group_count_field( int field_id ) {
        return (group_count_fields.find(field_id) != group_count_fields.end());
    }
    virtual void prepare_group(int key, char * value)
    {
        grp_field_id = key;
        int group_count = atoi(value);
        assert(group_count > 0);
        assert(group_count < 100);
        switch(grp_field_id)
        {
        default:
            assert(false);
        }
    }
    virtual bool  advance_group()
    {
        if(grp_field_id < 0) return false;
        switch(grp_field_id)
        {
        default:
            assert(false);
            return true;
        }
    }
    virtual void prepare_component(int key)
    {
        switch(key)
        {
        default:
            assert(false);
        }
    }
    virtual bool set_field(int key, char * value)
    {
        switch(key)
        {
        case 871:
            if(has_InstrAttribType == true) return false;
            InstrAttribType = value;
            has_InstrAttribType = true;
            return true;
        case 872:
            if(has_InstrAttribValue == true) return false;
            InstrAttribValue = value;
            has_InstrAttribValue = true;
            return true;
        default:
            assert(false);
            return false;
        }
    };

};

std::ostream& operator<<(std::ostream& os, const AttrbGrp_Type& msg)
{
    if(msg.has_InstrAttribType) std::cout << " InstrAttribType=" << msg.InstrAttribType;
    if(msg.has_InstrAttribValue) std::cout << " InstrAttribValue=" << msg.InstrAttribValue;
    return os;
}

struct InstrmtLegGrp_Type : public msg_base
{
    char *                                   LegSymbol;
    char *                                   LegRatioQty;
    char *                                   LegSecurityID;
    char *                                   LegSecurityDesc;
    char *                                   LegSecurityIDSource;
    char *                                   LegSide;
    char *                                   LegSecurityGroup;
    char *                                   LegCFICode;
    char *                                   LegSecuritySubType;
    char *                                   LegCurrency;
    char *                                   LegMaturityMonthYear;
    char *                                   LegStrikePrice;
    char *                                   LegStrikeCurrency;
    char *                                   LegSecurityExchange;
    char *                                   LegPrice;
    char *                                   LegOptionRatio;
    bool has_LegSymbol;
    bool has_LegRatioQty;
    bool has_LegSecurityID;
    bool has_LegSecurityDesc;
    bool has_LegSecurityIDSource;
    bool has_LegSide;
    bool has_LegSecurityGroup;
    bool has_LegCFICode;
    bool has_LegSecuritySubType;
    bool has_LegCurrency;
    bool has_LegMaturityMonthYear;
    bool has_LegStrikePrice;
    bool has_LegStrikeCurrency;
    bool has_LegSecurityExchange;
    bool has_LegPrice;
    bool has_LegOptionRatio;
    virtual void reset()
    {
        has_LegSymbol = false;
        has_LegRatioQty = false;
        has_LegSecurityID = false;
        has_LegSecurityDesc = false;
        has_LegSecurityIDSource = false;
        has_LegSide = false;
        has_LegSecurityGroup = false;
        has_LegCFICode = false;
        has_LegSecuritySubType = false;
        has_LegCurrency = false;
        has_LegMaturityMonthYear = false;
        has_LegStrikePrice = false;
        has_LegStrikeCurrency = false;
        has_LegSecurityExchange = false;
        has_LegPrice = false;
        has_LegOptionRatio = false;
    }

    std::set<int> poss_fields = {556,566,600,602,603,608,610,612,616,620,623,624,764,942,1017,5795};
    std::set<int> sim_fields = {556,566,600,602,603,608,610,612,616,620,623,624,764,942,1017,5795};
    std::map<int,int> field_to_comp = {};
    std::map<int,int> field_to_group_comp = {};
    std::set<int> group_count_fields = {};
    virtual std::set<int> & possible_fields() {
        return poss_fields;
    }
    virtual std::set<int> & simple_fields() {
        return sim_fields;
    }
    virtual int get_component_id( int field_id ) {
        assert( field_to_comp.find(field_id) != field_to_comp.end());
        return field_to_comp[field_id];
    }
    virtual int get_group_component_id( int field_id ) {
        assert(field_to_group_comp.find(field_id) != field_to_group_comp.end());
        return field_to_group_comp[field_id];
    }
    virtual bool is_group_count_field( int field_id ) {
        return (group_count_fields.find(field_id) != group_count_fields.end());
    }
    virtual void prepare_group(int key, char * value)
    {
        grp_field_id = key;
        int group_count = atoi(value);
        assert(group_count > 0);
        assert(group_count < 100);
        switch(grp_field_id)
        {
        default:
            assert(false);
        }
    }
    virtual bool  advance_group()
    {
        if(grp_field_id < 0) return false;
        switch(grp_field_id)
        {
        default:
            assert(false);
            return true;
        }
    }
    virtual void prepare_component(int key)
    {
        switch(key)
        {
        default:
            assert(false);
        }
    }
    virtual bool set_field(int key, char * value)
    {
        switch(key)
        {
        case 556:
            if(has_LegCurrency == true) return false;
            LegCurrency = value;
            has_LegCurrency = true;
            return true;
        case 566:
            if(has_LegPrice == true) return false;
            LegPrice = value;
            has_LegPrice = true;
            return true;
        case 600:
            if(has_LegSymbol == true) return false;
            LegSymbol = value;
            has_LegSymbol = true;
            return true;
        case 602:
            if(has_LegSecurityID == true) return false;
            LegSecurityID = value;
            has_LegSecurityID = true;
            return true;
        case 603:
            if(has_LegSecurityIDSource == true) return false;
            LegSecurityIDSource = value;
            has_LegSecurityIDSource = true;
            return true;
        case 608:
            if(has_LegCFICode == true) return false;
            LegCFICode = value;
            has_LegCFICode = true;
            return true;
        case 610:
            if(has_LegMaturityMonthYear == true) return false;
            LegMaturityMonthYear = value;
            has_LegMaturityMonthYear = true;
            return true;
        case 612:
            if(has_LegStrikePrice == true) return false;
            LegStrikePrice = value;
            has_LegStrikePrice = true;
            return true;
        case 616:
            if(has_LegSecurityExchange == true) return false;
            LegSecurityExchange = value;
            has_LegSecurityExchange = true;
            return true;
        case 620:
            if(has_LegSecurityDesc == true) return false;
            LegSecurityDesc = value;
            has_LegSecurityDesc = true;
            return true;
        case 623:
            if(has_LegRatioQty == true) return false;
            LegRatioQty = value;
            has_LegRatioQty = true;
            return true;
        case 624:
            if(has_LegSide == true) return false;
            LegSide = value;
            has_LegSide = true;
            return true;
        case 764:
            if(has_LegSecuritySubType == true) return false;
            LegSecuritySubType = value;
            has_LegSecuritySubType = true;
            return true;
        case 942:
            if(has_LegStrikeCurrency == true) return false;
            LegStrikeCurrency = value;
            has_LegStrikeCurrency = true;
            return true;
        case 1017:
            if(has_LegOptionRatio == true) return false;
            LegOptionRatio = value;
            has_LegOptionRatio = true;
            return true;
        case 5795:
            if(has_LegSecurityGroup == true) return false;
            LegSecurityGroup = value;
            has_LegSecurityGroup = true;
            return true;
        default:
            assert(false);
            return false;
        }
    };

};

std::ostream& operator<<(std::ostream& os, const InstrmtLegGrp_Type& msg)
{
    if(msg.has_LegSymbol) std::cout << " LegSymbol=" << msg.LegSymbol;
    if(msg.has_LegRatioQty) std::cout << " LegRatioQty=" << msg.LegRatioQty;
    if(msg.has_LegSecurityID) std::cout << " LegSecurityID=" << msg.LegSecurityID;
    if(msg.has_LegSecurityDesc) std::cout << " LegSecurityDesc=" << msg.LegSecurityDesc;
    if(msg.has_LegSecurityIDSource) std::cout << " LegSecurityIDSource=" << msg.LegSecurityIDSource;
    if(msg.has_LegSide) std::cout << " LegSide=" << msg.LegSide;
    if(msg.has_LegSecurityGroup) std::cout << " LegSecurityGroup=" << msg.LegSecurityGroup;
    if(msg.has_LegCFICode) std::cout << " LegCFICode=" << msg.LegCFICode;
    if(msg.has_LegSecuritySubType) std::cout << " LegSecuritySubType=" << msg.LegSecuritySubType;
    if(msg.has_LegCurrency) std::cout << " LegCurrency=" << msg.LegCurrency;
    if(msg.has_LegMaturityMonthYear) std::cout << " LegMaturityMonthYear=" << msg.LegMaturityMonthYear;
    if(msg.has_LegStrikePrice) std::cout << " LegStrikePrice=" << msg.LegStrikePrice;
    if(msg.has_LegStrikeCurrency) std::cout << " LegStrikeCurrency=" << msg.LegStrikeCurrency;
    if(msg.has_LegSecurityExchange) std::cout << " LegSecurityExchange=" << msg.LegSecurityExchange;
    if(msg.has_LegPrice) std::cout << " LegPrice=" << msg.LegPrice;
    if(msg.has_LegOptionRatio) std::cout << " LegOptionRatio=" << msg.LegOptionRatio;
    return os;
}

struct LotTypeRules_Type : public msg_base
{
    char *                                   LotType;
    char *                                   MinLotSize;
    bool has_LotType;
    bool has_MinLotSize;
    virtual void reset()
    {
        has_LotType = false;
        has_MinLotSize = false;
    }

    std::set<int> poss_fields = {1093,1231};
    std::set<int> sim_fields = {1093,1231};
    std::map<int,int> field_to_comp = {};
    std::map<int,int> field_to_group_comp = {};
    std::set<int> group_count_fields = {};
    virtual std::set<int> & possible_fields() {
        return poss_fields;
    }
    virtual std::set<int> & simple_fields() {
        return sim_fields;
    }
    virtual int get_component_id( int field_id ) {
        assert( field_to_comp.find(field_id) != field_to_comp.end());
        return field_to_comp[field_id];
    }
    virtual int get_group_component_id( int field_id ) {
        assert(field_to_group_comp.find(field_id) != field_to_group_comp.end());
        return field_to_group_comp[field_id];
    }
    virtual bool is_group_count_field( int field_id ) {
        return (group_count_fields.find(field_id) != group_count_fields.end());
    }
    virtual void prepare_group(int key, char * value)
    {
        grp_field_id = key;
        int group_count = atoi(value);
        assert(group_count > 0);
        assert(group_count < 100);
        switch(grp_field_id)
        {
        default:
            assert(false);
        }
    }
    virtual bool  advance_group()
    {
        if(grp_field_id < 0) return false;
        switch(grp_field_id)
        {
        default:
            assert(false);
            return true;
        }
    }
    virtual void prepare_component(int key)
    {
        switch(key)
        {
        default:
            assert(false);
        }
    }
    virtual bool set_field(int key, char * value)
    {
        switch(key)
        {
        case 1093:
            if(has_LotType == true) return false;
            LotType = value;
            has_LotType = true;
            return true;
        case 1231:
            if(has_MinLotSize == true) return false;
            MinLotSize = value;
            has_MinLotSize = true;
            return true;
        default:
            assert(false);
            return false;
        }
    };

};

std::ostream& operator<<(std::ostream& os, const LotTypeRules_Type& msg)
{
    if(msg.has_LotType) std::cout << " LotType=" << msg.LotType;
    if(msg.has_MinLotSize) std::cout << " MinLotSize=" << msg.MinLotSize;
    return os;
}

struct EvntGrp_Type : public msg_base
{
    char *                                   EventType;
    char *                                   EventDate;
    char *                                   EventTime;
    char *                                   EventPx;
    char *                                   EventText;
    bool has_EventType;
    bool has_EventDate;
    bool has_EventTime;
    bool has_EventPx;
    bool has_EventText;
    virtual void reset()
    {
        has_EventType = false;
        has_EventDate = false;
        has_EventTime = false;
        has_EventPx = false;
        has_EventText = false;
    }

    std::set<int> poss_fields = {865,866,867,868,1145};
    std::set<int> sim_fields = {865,866,867,868,1145};
    std::map<int,int> field_to_comp = {};
    std::map<int,int> field_to_group_comp = {};
    std::set<int> group_count_fields = {};
    virtual std::set<int> & possible_fields() {
        return poss_fields;
    }
    virtual std::set<int> & simple_fields() {
        return sim_fields;
    }
    virtual int get_component_id( int field_id ) {
        assert( field_to_comp.find(field_id) != field_to_comp.end());
        return field_to_comp[field_id];
    }
    virtual int get_group_component_id( int field_id ) {
        assert(field_to_group_comp.find(field_id) != field_to_group_comp.end());
        return field_to_group_comp[field_id];
    }
    virtual bool is_group_count_field( int field_id ) {
        return (group_count_fields.find(field_id) != group_count_fields.end());
    }
    virtual void prepare_group(int key, char * value)
    {
        grp_field_id = key;
        int group_count = atoi(value);
        assert(group_count > 0);
        assert(group_count < 100);
        switch(grp_field_id)
        {
        default:
            assert(false);
        }
    }
    virtual bool  advance_group()
    {
        if(grp_field_id < 0) return false;
        switch(grp_field_id)
        {
        default:
            assert(false);
            return true;
        }
    }
    virtual void prepare_component(int key)
    {
        switch(key)
        {
        default:
            assert(false);
        }
    }
    virtual bool set_field(int key, char * value)
    {
        switch(key)
        {
        case 865:
            if(has_EventType == true) return false;
            EventType = value;
            has_EventType = true;
            return true;
        case 866:
            if(has_EventDate == true) return false;
            EventDate = value;
            has_EventDate = true;
            return true;
        case 867:
            if(has_EventPx == true) return false;
            EventPx = value;
            has_EventPx = true;
            return true;
        case 868:
            if(has_EventText == true) return false;
            EventText = value;
            has_EventText = true;
            return true;
        case 1145:
            if(has_EventTime == true) return false;
            EventTime = value;
            has_EventTime = true;
            return true;
        default:
            assert(false);
            return false;
        }
    };

};

std::ostream& operator<<(std::ostream& os, const EvntGrp_Type& msg)
{
    if(msg.has_EventType) std::cout << " EventType=" << msg.EventType;
    if(msg.has_EventDate) std::cout << " EventDate=" << msg.EventDate;
    if(msg.has_EventTime) std::cout << " EventTime=" << msg.EventTime;
    if(msg.has_EventPx) std::cout << " EventPx=" << msg.EventPx;
    if(msg.has_EventText) std::cout << " EventText=" << msg.EventText;
    return os;
}

struct MDIncGrp_Type : public msg_base
{
    char *                                   MDUpdateAction;
    char *                                   MDPriceLevel;
    char *                                   MDEntryType;
    char *                                   SecurityIDSource;
    char *                                   SecurityID;
    char *                                   RptSeq;
    char *                                   QuoteCondition;
    char *                                   MDEntryPx;
    char *                                   NumberOfOrders;
    char *                                   MDEntryTime;
    char *                                   MDEntrySize;
    char *                                   TradingSessionID;
    char *                                   NetChgPrevDay;
    char *                                   TickDirection;
    char *                                   OpenCloseSettlFlag;
    char *                                   SettlDate;
    char *                                   TradeCondition;
    char *                                   TradeVolume;
    char *                                   MDQuoteType;
    char *                                   FixingBracket;
    char *                                   AggressorSide;
    char *                                   MatchEventIndicator;
    char *                                   TradeID;
    char *                                   SecurityDesc;
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
    virtual void reset()
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
        has_SecurityDesc = false;
    }

    std::set<int> poss_fields = {22,48,64,83,107,269,270,271,273,274,276,277,279,286,336,346,451,1003,1020,1023,1070,5790,5797,5799};
    std::set<int> sim_fields = {22,48,64,83,107,269,270,271,273,274,276,277,279,286,336,346,451,1003,1020,1023,1070,5790,5797,5799};
    std::map<int,int> field_to_comp = {};
    std::map<int,int> field_to_group_comp = {};
    std::set<int> group_count_fields = {};
    virtual std::set<int> & possible_fields() {
        return poss_fields;
    }
    virtual std::set<int> & simple_fields() {
        return sim_fields;
    }
    virtual int get_component_id( int field_id ) {
        assert( field_to_comp.find(field_id) != field_to_comp.end());
        return field_to_comp[field_id];
    }
    virtual int get_group_component_id( int field_id ) {
        assert(field_to_group_comp.find(field_id) != field_to_group_comp.end());
        return field_to_group_comp[field_id];
    }
    virtual bool is_group_count_field( int field_id ) {
        return (group_count_fields.find(field_id) != group_count_fields.end());
    }
    virtual void prepare_group(int key, char * value)
    {
        grp_field_id = key;
        int group_count = atoi(value);
        assert(group_count > 0);
        assert(group_count < 100);
        switch(grp_field_id)
        {
        default:
            assert(false);
        }
    }
    virtual bool  advance_group()
    {
        if(grp_field_id < 0) return false;
        switch(grp_field_id)
        {
        default:
            assert(false);
            return true;
        }
    }
    virtual void prepare_component(int key)
    {
        switch(key)
        {
        default:
            assert(false);
        }
    }
    virtual bool set_field(int key, char * value)
    {
        switch(key)
        {
        case 22:
            if(has_SecurityIDSource == true) return false;
            SecurityIDSource = value;
            has_SecurityIDSource = true;
            return true;
        case 48:
            if(has_SecurityID == true) return false;
            SecurityID = value;
            has_SecurityID = true;
            return true;
        case 64:
            if(has_SettlDate == true) return false;
            SettlDate = value;
            has_SettlDate = true;
            return true;
        case 83:
            if(has_RptSeq == true) return false;
            RptSeq = value;
            has_RptSeq = true;
            return true;
        case 107:
            if(has_SecurityDesc == true) return false;
            SecurityDesc = value;
            has_SecurityDesc = true;
            return true;
        case 269:
            if(has_MDEntryType == true) return false;
            MDEntryType = value;
            has_MDEntryType = true;
            return true;
        case 270:
            if(has_MDEntryPx == true) return false;
            MDEntryPx = value;
            has_MDEntryPx = true;
            return true;
        case 271:
            if(has_MDEntrySize == true) return false;
            MDEntrySize = value;
            has_MDEntrySize = true;
            return true;
        case 273:
            if(has_MDEntryTime == true) return false;
            MDEntryTime = value;
            has_MDEntryTime = true;
            return true;
        case 274:
            if(has_TickDirection == true) return false;
            TickDirection = value;
            has_TickDirection = true;
            return true;
        case 276:
            if(has_QuoteCondition == true) return false;
            QuoteCondition = value;
            has_QuoteCondition = true;
            return true;
        case 277:
            if(has_TradeCondition == true) return false;
            TradeCondition = value;
            has_TradeCondition = true;
            return true;
        case 279:
            if(has_MDUpdateAction == true) return false;
            MDUpdateAction = value;
            has_MDUpdateAction = true;
            return true;
        case 286:
            if(has_OpenCloseSettlFlag == true) return false;
            OpenCloseSettlFlag = value;
            has_OpenCloseSettlFlag = true;
            return true;
        case 336:
            if(has_TradingSessionID == true) return false;
            TradingSessionID = value;
            has_TradingSessionID = true;
            return true;
        case 346:
            if(has_NumberOfOrders == true) return false;
            NumberOfOrders = value;
            has_NumberOfOrders = true;
            return true;
        case 451:
            if(has_NetChgPrevDay == true) return false;
            NetChgPrevDay = value;
            has_NetChgPrevDay = true;
            return true;
        case 1003:
            if(has_TradeID == true) return false;
            TradeID = value;
            has_TradeID = true;
            return true;
        case 1020:
            if(has_TradeVolume == true) return false;
            TradeVolume = value;
            has_TradeVolume = true;
            return true;
        case 1023:
            if(has_MDPriceLevel == true) return false;
            MDPriceLevel = value;
            has_MDPriceLevel = true;
            return true;
        case 1070:
            if(has_MDQuoteType == true) return false;
            MDQuoteType = value;
            has_MDQuoteType = true;
            return true;
        case 5790:
            if(has_FixingBracket == true) return false;
            FixingBracket = value;
            has_FixingBracket = true;
            return true;
        case 5797:
            if(has_AggressorSide == true) return false;
            AggressorSide = value;
            has_AggressorSide = true;
            return true;
        case 5799:
            if(has_MatchEventIndicator == true) return false;
            MatchEventIndicator = value;
            has_MatchEventIndicator = true;
            return true;
        default:
            assert(false);
            return false;
        }
    };

};

std::ostream& operator<<(std::ostream& os, const MDIncGrp_Type& msg)
{
    if(msg.has_MDUpdateAction) std::cout << " MDUpdateAction=" << msg.MDUpdateAction;
    if(msg.has_MDPriceLevel) std::cout << " MDPriceLevel=" << msg.MDPriceLevel;
    if(msg.has_MDEntryType) std::cout << " MDEntryType=" << msg.MDEntryType;
    if(msg.has_SecurityIDSource) std::cout << " SecurityIDSource=" << msg.SecurityIDSource;
    if(msg.has_SecurityID) std::cout << " SecurityID=" << msg.SecurityID;
    if(msg.has_RptSeq) std::cout << " RptSeq=" << msg.RptSeq;
    if(msg.has_QuoteCondition) std::cout << " QuoteCondition=" << msg.QuoteCondition;
    if(msg.has_MDEntryPx) std::cout << " MDEntryPx=" << msg.MDEntryPx;
    if(msg.has_NumberOfOrders) std::cout << " NumberOfOrders=" << msg.NumberOfOrders;
    if(msg.has_MDEntryTime) std::cout << " MDEntryTime=" << msg.MDEntryTime;
    if(msg.has_MDEntrySize) std::cout << " MDEntrySize=" << msg.MDEntrySize;
    if(msg.has_TradingSessionID) std::cout << " TradingSessionID=" << msg.TradingSessionID;
    if(msg.has_NetChgPrevDay) std::cout << " NetChgPrevDay=" << msg.NetChgPrevDay;
    if(msg.has_TickDirection) std::cout << " TickDirection=" << msg.TickDirection;
    if(msg.has_OpenCloseSettlFlag) std::cout << " OpenCloseSettlFlag=" << msg.OpenCloseSettlFlag;
    if(msg.has_SettlDate) std::cout << " SettlDate=" << msg.SettlDate;
    if(msg.has_TradeCondition) std::cout << " TradeCondition=" << msg.TradeCondition;
    if(msg.has_TradeVolume) std::cout << " TradeVolume=" << msg.TradeVolume;
    if(msg.has_MDQuoteType) std::cout << " MDQuoteType=" << msg.MDQuoteType;
    if(msg.has_FixingBracket) std::cout << " FixingBracket=" << msg.FixingBracket;
    if(msg.has_AggressorSide) std::cout << " AggressorSide=" << msg.AggressorSide;
    if(msg.has_MatchEventIndicator) std::cout << " MatchEventIndicator=" << msg.MatchEventIndicator;
    if(msg.has_TradeID) std::cout << " TradeID=" << msg.TradeID;
    if(msg.has_SecurityDesc) std::cout << " SecurityDesc=" << msg.SecurityDesc;
    return os;
}

struct SecAltIDGrp_Type : public msg_base
{
    char *                                   SecurityAltID;
    char *                                   SecurityAltIDSource;
    bool has_SecurityAltID;
    bool has_SecurityAltIDSource;
    virtual void reset()
    {
        has_SecurityAltID = false;
        has_SecurityAltIDSource = false;
    }

    std::set<int> poss_fields = {455,456};
    std::set<int> sim_fields = {455,456};
    std::map<int,int> field_to_comp = {};
    std::map<int,int> field_to_group_comp = {};
    std::set<int> group_count_fields = {};
    virtual std::set<int> & possible_fields() {
        return poss_fields;
    }
    virtual std::set<int> & simple_fields() {
        return sim_fields;
    }
    virtual int get_component_id( int field_id ) {
        assert( field_to_comp.find(field_id) != field_to_comp.end());
        return field_to_comp[field_id];
    }
    virtual int get_group_component_id( int field_id ) {
        assert(field_to_group_comp.find(field_id) != field_to_group_comp.end());
        return field_to_group_comp[field_id];
    }
    virtual bool is_group_count_field( int field_id ) {
        return (group_count_fields.find(field_id) != group_count_fields.end());
    }
    virtual void prepare_group(int key, char * value)
    {
        grp_field_id = key;
        int group_count = atoi(value);
        assert(group_count > 0);
        assert(group_count < 100);
        switch(grp_field_id)
        {
        default:
            assert(false);
        }
    }
    virtual bool  advance_group()
    {
        if(grp_field_id < 0) return false;
        switch(grp_field_id)
        {
        default:
            assert(false);
            return true;
        }
    }
    virtual void prepare_component(int key)
    {
        switch(key)
        {
        default:
            assert(false);
        }
    }
    virtual bool set_field(int key, char * value)
    {
        switch(key)
        {
        case 455:
            if(has_SecurityAltID == true) return false;
            SecurityAltID = value;
            has_SecurityAltID = true;
            return true;
        case 456:
            if(has_SecurityAltIDSource == true) return false;
            SecurityAltIDSource = value;
            has_SecurityAltIDSource = true;
            return true;
        default:
            assert(false);
            return false;
        }
    };

};

std::ostream& operator<<(std::ostream& os, const SecAltIDGrp_Type& msg)
{
    if(msg.has_SecurityAltID) std::cout << " SecurityAltID=" << msg.SecurityAltID;
    if(msg.has_SecurityAltIDSource) std::cout << " SecurityAltIDSource=" << msg.SecurityAltIDSource;
    return os;
}

struct MarketDataFeedTypes_Type : public msg_base
{
    char *                                   MDFeedType;
    char *                                   MarketDepth;
    char *                                   MDBookType;
    bool has_MDFeedType;
    bool has_MarketDepth;
    bool has_MDBookType;
    virtual void reset()
    {
        has_MDFeedType = false;
        has_MarketDepth = false;
        has_MDBookType = false;
    }

    std::set<int> poss_fields = {264,1021,1022};
    std::set<int> sim_fields = {264,1021,1022};
    std::map<int,int> field_to_comp = {};
    std::map<int,int> field_to_group_comp = {};
    std::set<int> group_count_fields = {};
    virtual std::set<int> & possible_fields() {
        return poss_fields;
    }
    virtual std::set<int> & simple_fields() {
        return sim_fields;
    }
    virtual int get_component_id( int field_id ) {
        assert( field_to_comp.find(field_id) != field_to_comp.end());
        return field_to_comp[field_id];
    }
    virtual int get_group_component_id( int field_id ) {
        assert(field_to_group_comp.find(field_id) != field_to_group_comp.end());
        return field_to_group_comp[field_id];
    }
    virtual bool is_group_count_field( int field_id ) {
        return (group_count_fields.find(field_id) != group_count_fields.end());
    }
    virtual void prepare_group(int key, char * value)
    {
        grp_field_id = key;
        int group_count = atoi(value);
        assert(group_count > 0);
        assert(group_count < 100);
        switch(grp_field_id)
        {
        default:
            assert(false);
        }
    }
    virtual bool  advance_group()
    {
        if(grp_field_id < 0) return false;
        switch(grp_field_id)
        {
        default:
            assert(false);
            return true;
        }
    }
    virtual void prepare_component(int key)
    {
        switch(key)
        {
        default:
            assert(false);
        }
    }
    virtual bool set_field(int key, char * value)
    {
        switch(key)
        {
        case 264:
            if(has_MarketDepth == true) return false;
            MarketDepth = value;
            has_MarketDepth = true;
            return true;
        case 1021:
            if(has_MDBookType == true) return false;
            MDBookType = value;
            has_MDBookType = true;
            return true;
        case 1022:
            if(has_MDFeedType == true) return false;
            MDFeedType = value;
            has_MDFeedType = true;
            return true;
        default:
            assert(false);
            return false;
        }
    };

};

std::ostream& operator<<(std::ostream& os, const MarketDataFeedTypes_Type& msg)
{
    if(msg.has_MDFeedType) std::cout << " MDFeedType=" << msg.MDFeedType;
    if(msg.has_MarketDepth) std::cout << " MarketDepth=" << msg.MarketDepth;
    if(msg.has_MDBookType) std::cout << " MDBookType=" << msg.MDBookType;
    return os;
}

struct UndInstrmtGrp_Type : public msg_base
{
    char *                                   UnderlyingSymbol;
    char *                                   UnderlyingSecurityID;
    char *                                   UnderlyingSecurityIDSource;
    bool has_UnderlyingSymbol;
    bool has_UnderlyingSecurityID;
    bool has_UnderlyingSecurityIDSource;
    virtual void reset()
    {
        has_UnderlyingSymbol = false;
        has_UnderlyingSecurityID = false;
        has_UnderlyingSecurityIDSource = false;
    }

    std::set<int> poss_fields = {305,309,311};
    std::set<int> sim_fields = {305,309,311};
    std::map<int,int> field_to_comp = {};
    std::map<int,int> field_to_group_comp = {};
    std::set<int> group_count_fields = {};
    virtual std::set<int> & possible_fields() {
        return poss_fields;
    }
    virtual std::set<int> & simple_fields() {
        return sim_fields;
    }
    virtual int get_component_id( int field_id ) {
        assert( field_to_comp.find(field_id) != field_to_comp.end());
        return field_to_comp[field_id];
    }
    virtual int get_group_component_id( int field_id ) {
        assert(field_to_group_comp.find(field_id) != field_to_group_comp.end());
        return field_to_group_comp[field_id];
    }
    virtual bool is_group_count_field( int field_id ) {
        return (group_count_fields.find(field_id) != group_count_fields.end());
    }
    virtual void prepare_group(int key, char * value)
    {
        grp_field_id = key;
        int group_count = atoi(value);
        assert(group_count > 0);
        assert(group_count < 100);
        switch(grp_field_id)
        {
        default:
            assert(false);
        }
    }
    virtual bool  advance_group()
    {
        if(grp_field_id < 0) return false;
        switch(grp_field_id)
        {
        default:
            assert(false);
            return true;
        }
    }
    virtual void prepare_component(int key)
    {
        switch(key)
        {
        default:
            assert(false);
        }
    }
    virtual bool set_field(int key, char * value)
    {
        switch(key)
        {
        case 305:
            if(has_UnderlyingSecurityIDSource == true) return false;
            UnderlyingSecurityIDSource = value;
            has_UnderlyingSecurityIDSource = true;
            return true;
        case 309:
            if(has_UnderlyingSecurityID == true) return false;
            UnderlyingSecurityID = value;
            has_UnderlyingSecurityID = true;
            return true;
        case 311:
            if(has_UnderlyingSymbol == true) return false;
            UnderlyingSymbol = value;
            has_UnderlyingSymbol = true;
            return true;
        default:
            assert(false);
            return false;
        }
    };

};

std::ostream& operator<<(std::ostream& os, const UndInstrmtGrp_Type& msg)
{
    if(msg.has_UnderlyingSymbol) std::cout << " UnderlyingSymbol=" << msg.UnderlyingSymbol;
    if(msg.has_UnderlyingSecurityID) std::cout << " UnderlyingSecurityID=" << msg.UnderlyingSecurityID;
    if(msg.has_UnderlyingSecurityIDSource) std::cout << " UnderlyingSecurityIDSource=" << msg.UnderlyingSecurityIDSource;
    return os;
}

struct Header : public msg_base
{
    char *                                   ApplVerID;
    char *                                   BeginString;
    char *                                   BodyLength;
    char *                                   MsgType;
    char *                                   SenderCompID;
    char *                                   TargetCompID;
    char *                                   OnBehalfOfCompID;
    char *                                   DeliverToCompID;
    char *                                   SecureDataLen;
    char *                                   SecureData;
    char *                                   MsgSeqNum;
    char *                                   SenderSubID;
    char *                                   SenderLocationID;
    char *                                   TargetSubID;
    char *                                   TargetLocationID;
    char *                                   OnBehalfOfSubID;
    char *                                   OnBehalfOfLocationID;
    char *                                   DeliverToSubID;
    char *                                   DeliverToLocationID;
    char *                                   PossDupFlag;
    char *                                   PossResend;
    char *                                   SendingTime;
    char *                                   OrigSendingTime;
    char *                                   XmlDataLen;
    char *                                   XmlData;
    char *                                   MessageEncoding;
    char *                                   LastMsgSeqNumProcessed;
    bool has_ApplVerID;
    bool has_BeginString;
    bool has_BodyLength;
    bool has_MsgType;
    bool has_SenderCompID;
    bool has_TargetCompID;
    bool has_OnBehalfOfCompID;
    bool has_DeliverToCompID;
    bool has_SecureDataLen;
    bool has_SecureData;
    bool has_MsgSeqNum;
    bool has_SenderSubID;
    bool has_SenderLocationID;
    bool has_TargetSubID;
    bool has_TargetLocationID;
    bool has_OnBehalfOfSubID;
    bool has_OnBehalfOfLocationID;
    bool has_DeliverToSubID;
    bool has_DeliverToLocationID;
    bool has_PossDupFlag;
    bool has_PossResend;
    bool has_SendingTime;
    bool has_OrigSendingTime;
    bool has_XmlDataLen;
    bool has_XmlData;
    bool has_MessageEncoding;
    bool has_LastMsgSeqNumProcessed;
    virtual void reset()
    {
        has_ApplVerID = false;
        has_BeginString = false;
        has_BodyLength = false;
        has_MsgType = false;
        has_SenderCompID = false;
        has_TargetCompID = false;
        has_OnBehalfOfCompID = false;
        has_DeliverToCompID = false;
        has_SecureDataLen = false;
        has_SecureData = false;
        has_MsgSeqNum = false;
        has_SenderSubID = false;
        has_SenderLocationID = false;
        has_TargetSubID = false;
        has_TargetLocationID = false;
        has_OnBehalfOfSubID = false;
        has_OnBehalfOfLocationID = false;
        has_DeliverToSubID = false;
        has_DeliverToLocationID = false;
        has_PossDupFlag = false;
        has_PossResend = false;
        has_SendingTime = false;
        has_OrigSendingTime = false;
        has_XmlDataLen = false;
        has_XmlData = false;
        has_MessageEncoding = false;
        has_LastMsgSeqNumProcessed = false;
    }

    std::set<int> poss_fields = {8,9,34,35,43,49,50,52,56,57,90,91,97,115,116,122,128,129,142,143,144,145,212,213,347,369,1128};
    std::set<int> sim_fields = {8,9,34,35,43,49,50,52,56,57,90,91,97,115,116,122,128,129,142,143,144,145,212,213,347,369,1128};
    std::map<int,int> field_to_comp = {};
    std::map<int,int> field_to_group_comp = {};
    std::set<int> group_count_fields = {};
    virtual std::set<int> & possible_fields() {
        return poss_fields;
    }
    virtual std::set<int> & simple_fields() {
        return sim_fields;
    }
    virtual int get_component_id( int field_id ) {
        assert( field_to_comp.find(field_id) != field_to_comp.end());
        return field_to_comp[field_id];
    }
    virtual int get_group_component_id( int field_id ) {
        assert(field_to_group_comp.find(field_id) != field_to_group_comp.end());
        return field_to_group_comp[field_id];
    }
    virtual bool is_group_count_field( int field_id ) {
        return (group_count_fields.find(field_id) != group_count_fields.end());
    }
    virtual void prepare_group(int key, char * value)
    {
        grp_field_id = key;
        int group_count = atoi(value);
        assert(group_count > 0);
        assert(group_count < 100);
        switch(grp_field_id)
        {
        default:
            assert(false);
        }
    }
    virtual bool  advance_group()
    {
        if(grp_field_id < 0) return false;
        switch(grp_field_id)
        {
        default:
            assert(false);
            return true;
        }
    }
    virtual void prepare_component(int key)
    {
        switch(key)
        {
        default:
            assert(false);
        }
    }
    virtual bool set_field(int key, char * value)
    {
        switch(key)
        {
        case 8:
            if(has_BeginString == true) return false;
            BeginString = value;
            has_BeginString = true;
            return true;
        case 9:
            if(has_BodyLength == true) return false;
            BodyLength = value;
            has_BodyLength = true;
            return true;
        case 34:
            if(has_MsgSeqNum == true) return false;
            MsgSeqNum = value;
            has_MsgSeqNum = true;
            return true;
        case 35:
            if(has_MsgType == true) return false;
            MsgType = value;
            has_MsgType = true;
            return true;
        case 43:
            if(has_PossDupFlag == true) return false;
            PossDupFlag = value;
            has_PossDupFlag = true;
            return true;
        case 49:
            if(has_SenderCompID == true) return false;
            SenderCompID = value;
            has_SenderCompID = true;
            return true;
        case 50:
            if(has_SenderSubID == true) return false;
            SenderSubID = value;
            has_SenderSubID = true;
            return true;
        case 52:
            if(has_SendingTime == true) return false;
            SendingTime = value;
            has_SendingTime = true;
            return true;
        case 56:
            if(has_TargetCompID == true) return false;
            TargetCompID = value;
            has_TargetCompID = true;
            return true;
        case 57:
            if(has_TargetSubID == true) return false;
            TargetSubID = value;
            has_TargetSubID = true;
            return true;
        case 90:
            if(has_SecureDataLen == true) return false;
            SecureDataLen = value;
            has_SecureDataLen = true;
            return true;
        case 91:
            if(has_SecureData == true) return false;
            SecureData = value;
            has_SecureData = true;
            return true;
        case 97:
            if(has_PossResend == true) return false;
            PossResend = value;
            has_PossResend = true;
            return true;
        case 115:
            if(has_OnBehalfOfCompID == true) return false;
            OnBehalfOfCompID = value;
            has_OnBehalfOfCompID = true;
            return true;
        case 116:
            if(has_OnBehalfOfSubID == true) return false;
            OnBehalfOfSubID = value;
            has_OnBehalfOfSubID = true;
            return true;
        case 122:
            if(has_OrigSendingTime == true) return false;
            OrigSendingTime = value;
            has_OrigSendingTime = true;
            return true;
        case 128:
            if(has_DeliverToCompID == true) return false;
            DeliverToCompID = value;
            has_DeliverToCompID = true;
            return true;
        case 129:
            if(has_DeliverToSubID == true) return false;
            DeliverToSubID = value;
            has_DeliverToSubID = true;
            return true;
        case 142:
            if(has_SenderLocationID == true) return false;
            SenderLocationID = value;
            has_SenderLocationID = true;
            return true;
        case 143:
            if(has_TargetLocationID == true) return false;
            TargetLocationID = value;
            has_TargetLocationID = true;
            return true;
        case 144:
            if(has_OnBehalfOfLocationID == true) return false;
            OnBehalfOfLocationID = value;
            has_OnBehalfOfLocationID = true;
            return true;
        case 145:
            if(has_DeliverToLocationID == true) return false;
            DeliverToLocationID = value;
            has_DeliverToLocationID = true;
            return true;
        case 212:
            if(has_XmlDataLen == true) return false;
            XmlDataLen = value;
            has_XmlDataLen = true;
            return true;
        case 213:
            if(has_XmlData == true) return false;
            XmlData = value;
            has_XmlData = true;
            return true;
        case 347:
            if(has_MessageEncoding == true) return false;
            MessageEncoding = value;
            has_MessageEncoding = true;
            return true;
        case 369:
            if(has_LastMsgSeqNumProcessed == true) return false;
            LastMsgSeqNumProcessed = value;
            has_LastMsgSeqNumProcessed = true;
            return true;
        case 1128:
            if(has_ApplVerID == true) return false;
            ApplVerID = value;
            has_ApplVerID = true;
            return true;
        default:
            assert(false);
            return false;
        }
    };

};

std::ostream& operator<<(std::ostream& os, const Header& msg)
{
    if(msg.has_ApplVerID) std::cout << " ApplVerID=" << msg.ApplVerID;
    if(msg.has_BeginString) std::cout << " BeginString=" << msg.BeginString;
    if(msg.has_BodyLength) std::cout << " BodyLength=" << msg.BodyLength;
    if(msg.has_MsgType) std::cout << " MsgType=" << msg.MsgType;
    if(msg.has_SenderCompID) std::cout << " SenderCompID=" << msg.SenderCompID;
    if(msg.has_TargetCompID) std::cout << " TargetCompID=" << msg.TargetCompID;
    if(msg.has_OnBehalfOfCompID) std::cout << " OnBehalfOfCompID=" << msg.OnBehalfOfCompID;
    if(msg.has_DeliverToCompID) std::cout << " DeliverToCompID=" << msg.DeliverToCompID;
    if(msg.has_SecureDataLen) std::cout << " SecureDataLen=" << msg.SecureDataLen;
    if(msg.has_SecureData) std::cout << " SecureData=" << msg.SecureData;
    if(msg.has_MsgSeqNum) std::cout << " MsgSeqNum=" << msg.MsgSeqNum;
    if(msg.has_SenderSubID) std::cout << " SenderSubID=" << msg.SenderSubID;
    if(msg.has_SenderLocationID) std::cout << " SenderLocationID=" << msg.SenderLocationID;
    if(msg.has_TargetSubID) std::cout << " TargetSubID=" << msg.TargetSubID;
    if(msg.has_TargetLocationID) std::cout << " TargetLocationID=" << msg.TargetLocationID;
    if(msg.has_OnBehalfOfSubID) std::cout << " OnBehalfOfSubID=" << msg.OnBehalfOfSubID;
    if(msg.has_OnBehalfOfLocationID) std::cout << " OnBehalfOfLocationID=" << msg.OnBehalfOfLocationID;
    if(msg.has_DeliverToSubID) std::cout << " DeliverToSubID=" << msg.DeliverToSubID;
    if(msg.has_DeliverToLocationID) std::cout << " DeliverToLocationID=" << msg.DeliverToLocationID;
    if(msg.has_PossDupFlag) std::cout << " PossDupFlag=" << msg.PossDupFlag;
    if(msg.has_PossResend) std::cout << " PossResend=" << msg.PossResend;
    if(msg.has_SendingTime) std::cout << " SendingTime=" << msg.SendingTime;
    if(msg.has_OrigSendingTime) std::cout << " OrigSendingTime=" << msg.OrigSendingTime;
    if(msg.has_XmlDataLen) std::cout << " XmlDataLen=" << msg.XmlDataLen;
    if(msg.has_XmlData) std::cout << " XmlData=" << msg.XmlData;
    if(msg.has_MessageEncoding) std::cout << " MessageEncoding=" << msg.MessageEncoding;
    if(msg.has_LastMsgSeqNumProcessed) std::cout << " LastMsgSeqNumProcessed=" << msg.LastMsgSeqNumProcessed;
    return os;
}

struct Trailer : public msg_base
{
    char *                                   SignatureLength;
    char *                                   Signature;
    char *                                   CheckSum;
    bool has_SignatureLength;
    bool has_Signature;
    bool has_CheckSum;
    virtual void reset()
    {
        has_SignatureLength = false;
        has_Signature = false;
        has_CheckSum = false;
    }

    std::set<int> poss_fields = {10,89,93};
    std::set<int> sim_fields = {10,89,93};
    std::map<int,int> field_to_comp = {};
    std::map<int,int> field_to_group_comp = {};
    std::set<int> group_count_fields = {};
    virtual std::set<int> & possible_fields() {
        return poss_fields;
    }
    virtual std::set<int> & simple_fields() {
        return sim_fields;
    }
    virtual int get_component_id( int field_id ) {
        assert( field_to_comp.find(field_id) != field_to_comp.end());
        return field_to_comp[field_id];
    }
    virtual int get_group_component_id( int field_id ) {
        assert(field_to_group_comp.find(field_id) != field_to_group_comp.end());
        return field_to_group_comp[field_id];
    }
    virtual bool is_group_count_field( int field_id ) {
        return (group_count_fields.find(field_id) != group_count_fields.end());
    }
    virtual void prepare_group(int key, char * value)
    {
        grp_field_id = key;
        int group_count = atoi(value);
        assert(group_count > 0);
        assert(group_count < 100);
        switch(grp_field_id)
        {
        default:
            assert(false);
        }
    }
    virtual bool  advance_group()
    {
        if(grp_field_id < 0) return false;
        switch(grp_field_id)
        {
        default:
            assert(false);
            return true;
        }
    }
    virtual void prepare_component(int key)
    {
        switch(key)
        {
        default:
            assert(false);
        }
    }
    virtual bool set_field(int key, char * value)
    {
        switch(key)
        {
        case 10:
            if(has_CheckSum == true) return false;
            CheckSum = value;
            has_CheckSum = true;
            return true;
        case 89:
            if(has_Signature == true) return false;
            Signature = value;
            has_Signature = true;
            return true;
        case 93:
            if(has_SignatureLength == true) return false;
            SignatureLength = value;
            has_SignatureLength = true;
            return true;
        default:
            assert(false);
            return false;
        }
    };

};

std::ostream& operator<<(std::ostream& os, const Trailer& msg)
{
    if(msg.has_SignatureLength) std::cout << " SignatureLength=" << msg.SignatureLength;
    if(msg.has_Signature) std::cout << " Signature=" << msg.Signature;
    if(msg.has_CheckSum) std::cout << " CheckSum=" << msg.CheckSum;
    return os;
}

struct MarketDataIncrementalRefresh : public msg_base
{
    char *                                   TradeDate;
    std::vector<MDIncGrp_Type>               MDIncGrp;
    bool has_TradeDate;
    bool has_MDIncGrp;
    virtual void reset()
    {
        has_TradeDate = false;
        has_MDIncGrp = false;
    }

    std::set<int> poss_fields = {22,48,64,75,83,107,268,269,270,271,273,274,276,277,279,286,336,346,451,1003,1020,1023,1070,5790,5797,5799};
    std::set<int> sim_fields = {75};
    std::map<int,int> field_to_comp = {};
    std::map<int,int> field_to_group_comp = {{268,4}};
    std::set<int> group_count_fields = {268};
    virtual std::set<int> & possible_fields() {
        return poss_fields;
    }
    virtual std::set<int> & simple_fields() {
        return sim_fields;
    }
    virtual int get_component_id( int field_id ) {
        assert( field_to_comp.find(field_id) != field_to_comp.end());
        return field_to_comp[field_id];
    }
    virtual int get_group_component_id( int field_id ) {
        assert(field_to_group_comp.find(field_id) != field_to_group_comp.end());
        return field_to_group_comp[field_id];
    }
    virtual bool is_group_count_field( int field_id ) {
        return (group_count_fields.find(field_id) != group_count_fields.end());
    }
    virtual void prepare_group(int key, char * value)
    {
        grp_field_id = key;
        int group_count = atoi(value);
        assert(group_count > 0);
        assert(group_count < 100);
        switch(grp_field_id)
        {
        case 268:
            MDIncGrp.resize(group_count);
            is_grp = true;
            grp_index = 0;
            num_in_grp = group_count;
            group = &MDIncGrp[0];
            break;
        default:
            assert(false);
        }
    }
    virtual bool  advance_group()
    {
        if(grp_field_id < 0) return false;
        switch(grp_field_id)
        {
        case 268:
            ++grp_index;
            group = &MDIncGrp[grp_index];
            break;
        default:
            assert(false);
            return true;
        }
    }
    virtual void prepare_component(int key)
    {
        switch(key)
        {
        default:
            assert(false);
        }
    }
    virtual bool set_field(int key, char * value)
    {
        switch(key)
        {
        case 75:
            if(has_TradeDate == true) return false;
            TradeDate = value;
            has_TradeDate = true;
            return true;
        default:
            assert(false);
            return false;
        }
    };

};

std::ostream& operator<<(std::ostream& os, const MarketDataIncrementalRefresh& msg)
{
    if(msg.has_TradeDate) std::cout << " TradeDate=" << msg.TradeDate;
    for( auto entry : msg.MDIncGrp) os << " MDIncGrp=" << entry;
    return os;
}

struct SecurityDefinition : public msg_base
{
    char *                                   MatchEventIndicator;
    char *                                   TotNumReports;
    char *                                   TradingReferencePrice;
    char *                                   SettlPriceType;
    char *                                   HighLimitPrice;
    char *                                   LowLimitPrice;
    char *                                   SecurityGroup;
    char *                                   Symbol;
    char *                                   ExpirationCycle;
    char *                                   SecurityDesc;
    char *                                   SecurityID;
    char *                                   SecurityIDSource;
    char *                                   CFICode;
    char *                                   UnderlyingProduct;
    char *                                   SecurityExchange;
    char *                                   PricingModel;
    char *                                   MinCabPrice;
    char *                                   UnitOfMeasureQty;
    char *                                   StrikePrice;
    char *                                   StrikeCurrency;
    char *                                   MinTradeVol;
    char *                                   MaxTradeVol;
    char *                                   Currency;
    char *                                   SettlCurrency;
    char *                                   MatchAlgorithm;
    char *                                   SecuritySubType;
    char *                                   MaxPriceVariation;
    char *                                   ImpliedMarketIndicator;
    char *                                   MaturityMonthYear;
    char *                                   MinPriceIncrement;
    char *                                   MinPriceIncrementAmount;
    char *                                   LastUpdateTime;
    char *                                   SecurityUpdateAction;
    char *                                   DisplayFactor;
    char *                                   ApplID;
    char *                                   MarketSegmentID;
    char *                                   TradingReferenceDate;
    char *                                   UserDefinedInstrument;
    char *                                   PriceRatio;
    char *                                   ContractMultiplierUnit;
    char *                                   FlowScheduleType;
    char *                                   ContractMultiplier;
    char *                                   UnitOfMeasure;
    char *                                   DecayQty;
    char *                                   DecayStartDate;
    char *                                   OriginalContractSize;
    char *                                   ClearedVolume;
    char *                                   OpenInterestQty;
    std::vector<EvntGrp_Type>                EvntGrp;
    std::vector<SecAltIDGrp_Type>            SecAltIDGrp;
    std::vector<MarketDataFeedTypes_Type>    MarketDataFeedTypes;
    std::vector<UndInstrmtGrp_Type>          UndInstrmtGrp;
    std::vector<AttrbGrp_Type>               AttrbGrp;
    std::vector<InstrmtLegGrp_Type>          InstrmtLegGrp;
    std::vector<LotTypeRules_Type>           LotTypeRules;
    bool has_MatchEventIndicator;
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
    bool has_EvntGrp;
    bool has_SecAltIDGrp;
    bool has_MarketDataFeedTypes;
    bool has_UndInstrmtGrp;
    bool has_AttrbGrp;
    bool has_InstrmtLegGrp;
    bool has_LotTypeRules;
    virtual void reset()
    {
        has_MatchEventIndicator = false;
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
        has_EvntGrp = false;
        has_SecAltIDGrp = false;
        has_MarketDataFeedTypes = false;
        has_UndInstrmtGrp = false;
        has_AttrbGrp = false;
        has_InstrmtLegGrp = false;
        has_LotTypeRules = false;
    }

    std::set<int> poss_fields = {15,22,48,55,107,120,200,202,207,231,264,305,309,311,454,455,456,461,462,555,556,562,566,600,602,603,608,610,612,616,620,623,624,711,731,762,764,779,827,864,865,866,867,868,870,871,872,911,942,947,969,980,996,1017,1021,1022,1093,1140,1141,1142,1143,1144,1145,1146,1147,1148,1149,1150,1151,1180,1231,1234,1300,1435,1439,5770,5791,5792,5795,5796,5799,5818,5819,5849,9779,9787,9850,9853};
    std::set<int> sim_fields = {15,22,48,55,107,120,200,202,207,231,461,462,562,731,762,779,827,911,947,969,980,996,1140,1142,1143,1144,1146,1147,1148,1149,1150,1151,1180,1300,1435,1439,5770,5791,5792,5796,5799,5818,5819,5849,9779,9787,9850,9853};
    std::map<int,int> field_to_comp = {};
    std::map<int,int> field_to_group_comp = {{864,3},{454,5},{870,0},{711,7},{555,1},{1234,2},{1141,6}};
    std::set<int> group_count_fields = {864,454,1141,711,870,555,1234};
    virtual std::set<int> & possible_fields() {
        return poss_fields;
    }
    virtual std::set<int> & simple_fields() {
        return sim_fields;
    }
    virtual int get_component_id( int field_id ) {
        assert( field_to_comp.find(field_id) != field_to_comp.end());
        return field_to_comp[field_id];
    }
    virtual int get_group_component_id( int field_id ) {
        assert(field_to_group_comp.find(field_id) != field_to_group_comp.end());
        return field_to_group_comp[field_id];
    }
    virtual bool is_group_count_field( int field_id ) {
        return (group_count_fields.find(field_id) != group_count_fields.end());
    }
    virtual void prepare_group(int key, char * value)
    {
        grp_field_id = key;
        int group_count = atoi(value);
        assert(group_count > 0);
        assert(group_count < 100);
        switch(grp_field_id)
        {
        case 864:
            EvntGrp.resize(group_count);
            is_grp = true;
            grp_index = 0;
            num_in_grp = group_count;
            group = &EvntGrp[0];
            break;
        case 454:
            SecAltIDGrp.resize(group_count);
            is_grp = true;
            grp_index = 0;
            num_in_grp = group_count;
            group = &SecAltIDGrp[0];
            break;
        case 870:
            AttrbGrp.resize(group_count);
            is_grp = true;
            grp_index = 0;
            num_in_grp = group_count;
            group = &AttrbGrp[0];
            break;
        case 711:
            UndInstrmtGrp.resize(group_count);
            is_grp = true;
            grp_index = 0;
            num_in_grp = group_count;
            group = &UndInstrmtGrp[0];
            break;
        case 555:
            InstrmtLegGrp.resize(group_count);
            is_grp = true;
            grp_index = 0;
            num_in_grp = group_count;
            group = &InstrmtLegGrp[0];
            break;
        case 1234:
            LotTypeRules.resize(group_count);
            is_grp = true;
            grp_index = 0;
            num_in_grp = group_count;
            group = &LotTypeRules[0];
            break;
        case 1141:
            MarketDataFeedTypes.resize(group_count);
            is_grp = true;
            grp_index = 0;
            num_in_grp = group_count;
            group = &MarketDataFeedTypes[0];
            break;
        default:
            assert(false);
        }
    }
    virtual bool  advance_group()
    {
        if(grp_field_id < 0) return false;
        switch(grp_field_id)
        {
        case 864:
            ++grp_index;
            group = &EvntGrp[grp_index];
            break;
        case 454:
            ++grp_index;
            group = &SecAltIDGrp[grp_index];
            break;
        case 870:
            ++grp_index;
            group = &AttrbGrp[grp_index];
            break;
        case 711:
            ++grp_index;
            group = &UndInstrmtGrp[grp_index];
            break;
        case 555:
            ++grp_index;
            group = &InstrmtLegGrp[grp_index];
            break;
        case 1234:
            ++grp_index;
            group = &LotTypeRules[grp_index];
            break;
        case 1141:
            ++grp_index;
            group = &MarketDataFeedTypes[grp_index];
            break;
        default:
            assert(false);
            return true;
        }
    }
    virtual void prepare_component(int key)
    {
        switch(key)
        {
        default:
            assert(false);
        }
    }
    virtual bool set_field(int key, char * value)
    {
        switch(key)
        {
        case 15:
            if(has_Currency == true) return false;
            Currency = value;
            has_Currency = true;
            return true;
        case 22:
            if(has_SecurityIDSource == true) return false;
            SecurityIDSource = value;
            has_SecurityIDSource = true;
            return true;
        case 48:
            if(has_SecurityID == true) return false;
            SecurityID = value;
            has_SecurityID = true;
            return true;
        case 55:
            if(has_Symbol == true) return false;
            Symbol = value;
            has_Symbol = true;
            return true;
        case 107:
            if(has_SecurityDesc == true) return false;
            SecurityDesc = value;
            has_SecurityDesc = true;
            return true;
        case 120:
            if(has_SettlCurrency == true) return false;
            SettlCurrency = value;
            has_SettlCurrency = true;
            return true;
        case 200:
            if(has_MaturityMonthYear == true) return false;
            MaturityMonthYear = value;
            has_MaturityMonthYear = true;
            return true;
        case 202:
            if(has_StrikePrice == true) return false;
            StrikePrice = value;
            has_StrikePrice = true;
            return true;
        case 207:
            if(has_SecurityExchange == true) return false;
            SecurityExchange = value;
            has_SecurityExchange = true;
            return true;
        case 231:
            if(has_ContractMultiplier == true) return false;
            ContractMultiplier = value;
            has_ContractMultiplier = true;
            return true;
        case 461:
            if(has_CFICode == true) return false;
            CFICode = value;
            has_CFICode = true;
            return true;
        case 462:
            if(has_UnderlyingProduct == true) return false;
            UnderlyingProduct = value;
            has_UnderlyingProduct = true;
            return true;
        case 562:
            if(has_MinTradeVol == true) return false;
            MinTradeVol = value;
            has_MinTradeVol = true;
            return true;
        case 731:
            if(has_SettlPriceType == true) return false;
            SettlPriceType = value;
            has_SettlPriceType = true;
            return true;
        case 762:
            if(has_SecuritySubType == true) return false;
            SecuritySubType = value;
            has_SecuritySubType = true;
            return true;
        case 779:
            if(has_LastUpdateTime == true) return false;
            LastUpdateTime = value;
            has_LastUpdateTime = true;
            return true;
        case 827:
            if(has_ExpirationCycle == true) return false;
            ExpirationCycle = value;
            has_ExpirationCycle = true;
            return true;
        case 911:
            if(has_TotNumReports == true) return false;
            TotNumReports = value;
            has_TotNumReports = true;
            return true;
        case 947:
            if(has_StrikeCurrency == true) return false;
            StrikeCurrency = value;
            has_StrikeCurrency = true;
            return true;
        case 969:
            if(has_MinPriceIncrement == true) return false;
            MinPriceIncrement = value;
            has_MinPriceIncrement = true;
            return true;
        case 980:
            if(has_SecurityUpdateAction == true) return false;
            SecurityUpdateAction = value;
            has_SecurityUpdateAction = true;
            return true;
        case 996:
            if(has_UnitOfMeasure == true) return false;
            UnitOfMeasure = value;
            has_UnitOfMeasure = true;
            return true;
        case 1140:
            if(has_MaxTradeVol == true) return false;
            MaxTradeVol = value;
            has_MaxTradeVol = true;
            return true;
        case 1142:
            if(has_MatchAlgorithm == true) return false;
            MatchAlgorithm = value;
            has_MatchAlgorithm = true;
            return true;
        case 1143:
            if(has_MaxPriceVariation == true) return false;
            MaxPriceVariation = value;
            has_MaxPriceVariation = true;
            return true;
        case 1144:
            if(has_ImpliedMarketIndicator == true) return false;
            ImpliedMarketIndicator = value;
            has_ImpliedMarketIndicator = true;
            return true;
        case 1146:
            if(has_MinPriceIncrementAmount == true) return false;
            MinPriceIncrementAmount = value;
            has_MinPriceIncrementAmount = true;
            return true;
        case 1147:
            if(has_UnitOfMeasureQty == true) return false;
            UnitOfMeasureQty = value;
            has_UnitOfMeasureQty = true;
            return true;
        case 1148:
            if(has_LowLimitPrice == true) return false;
            LowLimitPrice = value;
            has_LowLimitPrice = true;
            return true;
        case 1149:
            if(has_HighLimitPrice == true) return false;
            HighLimitPrice = value;
            has_HighLimitPrice = true;
            return true;
        case 1150:
            if(has_TradingReferencePrice == true) return false;
            TradingReferencePrice = value;
            has_TradingReferencePrice = true;
            return true;
        case 1151:
            if(has_SecurityGroup == true) return false;
            SecurityGroup = value;
            has_SecurityGroup = true;
            return true;
        case 1180:
            if(has_ApplID == true) return false;
            ApplID = value;
            has_ApplID = true;
            return true;
        case 1300:
            if(has_MarketSegmentID == true) return false;
            MarketSegmentID = value;
            has_MarketSegmentID = true;
            return true;
        case 1435:
            if(has_ContractMultiplierUnit == true) return false;
            ContractMultiplierUnit = value;
            has_ContractMultiplierUnit = true;
            return true;
        case 1439:
            if(has_FlowScheduleType == true) return false;
            FlowScheduleType = value;
            has_FlowScheduleType = true;
            return true;
        case 5770:
            if(has_PriceRatio == true) return false;
            PriceRatio = value;
            has_PriceRatio = true;
            return true;
        case 5791:
            if(has_ClearedVolume == true) return false;
            ClearedVolume = value;
            has_ClearedVolume = true;
            return true;
        case 5792:
            if(has_OpenInterestQty == true) return false;
            OpenInterestQty = value;
            has_OpenInterestQty = true;
            return true;
        case 5796:
            if(has_TradingReferenceDate == true) return false;
            TradingReferenceDate = value;
            has_TradingReferenceDate = true;
            return true;
        case 5799:
            if(has_MatchEventIndicator == true) return false;
            MatchEventIndicator = value;
            has_MatchEventIndicator = true;
            return true;
        case 5818:
            if(has_DecayQty == true) return false;
            DecayQty = value;
            has_DecayQty = true;
            return true;
        case 5819:
            if(has_DecayStartDate == true) return false;
            DecayStartDate = value;
            has_DecayStartDate = true;
            return true;
        case 5849:
            if(has_OriginalContractSize == true) return false;
            OriginalContractSize = value;
            has_OriginalContractSize = true;
            return true;
        case 9779:
            if(has_UserDefinedInstrument == true) return false;
            UserDefinedInstrument = value;
            has_UserDefinedInstrument = true;
            return true;
        case 9787:
            if(has_DisplayFactor == true) return false;
            DisplayFactor = value;
            has_DisplayFactor = true;
            return true;
        case 9850:
            if(has_MinCabPrice == true) return false;
            MinCabPrice = value;
            has_MinCabPrice = true;
            return true;
        case 9853:
            if(has_PricingModel == true) return false;
            PricingModel = value;
            has_PricingModel = true;
            return true;
        default:
            assert(false);
            return false;
        }
    };

};

std::ostream& operator<<(std::ostream& os, const SecurityDefinition& msg)
{
    if(msg.has_MatchEventIndicator) std::cout << " MatchEventIndicator=" << msg.MatchEventIndicator;
    if(msg.has_TotNumReports) std::cout << " TotNumReports=" << msg.TotNumReports;
    if(msg.has_TradingReferencePrice) std::cout << " TradingReferencePrice=" << msg.TradingReferencePrice;
    if(msg.has_SettlPriceType) std::cout << " SettlPriceType=" << msg.SettlPriceType;
    if(msg.has_HighLimitPrice) std::cout << " HighLimitPrice=" << msg.HighLimitPrice;
    if(msg.has_LowLimitPrice) std::cout << " LowLimitPrice=" << msg.LowLimitPrice;
    if(msg.has_SecurityGroup) std::cout << " SecurityGroup=" << msg.SecurityGroup;
    if(msg.has_Symbol) std::cout << " Symbol=" << msg.Symbol;
    if(msg.has_ExpirationCycle) std::cout << " ExpirationCycle=" << msg.ExpirationCycle;
    if(msg.has_SecurityDesc) std::cout << " SecurityDesc=" << msg.SecurityDesc;
    if(msg.has_SecurityID) std::cout << " SecurityID=" << msg.SecurityID;
    if(msg.has_SecurityIDSource) std::cout << " SecurityIDSource=" << msg.SecurityIDSource;
    if(msg.has_CFICode) std::cout << " CFICode=" << msg.CFICode;
    if(msg.has_UnderlyingProduct) std::cout << " UnderlyingProduct=" << msg.UnderlyingProduct;
    if(msg.has_SecurityExchange) std::cout << " SecurityExchange=" << msg.SecurityExchange;
    if(msg.has_PricingModel) std::cout << " PricingModel=" << msg.PricingModel;
    if(msg.has_MinCabPrice) std::cout << " MinCabPrice=" << msg.MinCabPrice;
    if(msg.has_UnitOfMeasureQty) std::cout << " UnitOfMeasureQty=" << msg.UnitOfMeasureQty;
    if(msg.has_StrikePrice) std::cout << " StrikePrice=" << msg.StrikePrice;
    if(msg.has_StrikeCurrency) std::cout << " StrikeCurrency=" << msg.StrikeCurrency;
    if(msg.has_MinTradeVol) std::cout << " MinTradeVol=" << msg.MinTradeVol;
    if(msg.has_MaxTradeVol) std::cout << " MaxTradeVol=" << msg.MaxTradeVol;
    if(msg.has_Currency) std::cout << " Currency=" << msg.Currency;
    if(msg.has_SettlCurrency) std::cout << " SettlCurrency=" << msg.SettlCurrency;
    if(msg.has_MatchAlgorithm) std::cout << " MatchAlgorithm=" << msg.MatchAlgorithm;
    if(msg.has_SecuritySubType) std::cout << " SecuritySubType=" << msg.SecuritySubType;
    if(msg.has_MaxPriceVariation) std::cout << " MaxPriceVariation=" << msg.MaxPriceVariation;
    if(msg.has_ImpliedMarketIndicator) std::cout << " ImpliedMarketIndicator=" << msg.ImpliedMarketIndicator;
    if(msg.has_MaturityMonthYear) std::cout << " MaturityMonthYear=" << msg.MaturityMonthYear;
    if(msg.has_MinPriceIncrement) std::cout << " MinPriceIncrement=" << msg.MinPriceIncrement;
    if(msg.has_MinPriceIncrementAmount) std::cout << " MinPriceIncrementAmount=" << msg.MinPriceIncrementAmount;
    if(msg.has_LastUpdateTime) std::cout << " LastUpdateTime=" << msg.LastUpdateTime;
    if(msg.has_SecurityUpdateAction) std::cout << " SecurityUpdateAction=" << msg.SecurityUpdateAction;
    if(msg.has_DisplayFactor) std::cout << " DisplayFactor=" << msg.DisplayFactor;
    if(msg.has_ApplID) std::cout << " ApplID=" << msg.ApplID;
    if(msg.has_MarketSegmentID) std::cout << " MarketSegmentID=" << msg.MarketSegmentID;
    if(msg.has_TradingReferenceDate) std::cout << " TradingReferenceDate=" << msg.TradingReferenceDate;
    if(msg.has_UserDefinedInstrument) std::cout << " UserDefinedInstrument=" << msg.UserDefinedInstrument;
    if(msg.has_PriceRatio) std::cout << " PriceRatio=" << msg.PriceRatio;
    if(msg.has_ContractMultiplierUnit) std::cout << " ContractMultiplierUnit=" << msg.ContractMultiplierUnit;
    if(msg.has_FlowScheduleType) std::cout << " FlowScheduleType=" << msg.FlowScheduleType;
    if(msg.has_ContractMultiplier) std::cout << " ContractMultiplier=" << msg.ContractMultiplier;
    if(msg.has_UnitOfMeasure) std::cout << " UnitOfMeasure=" << msg.UnitOfMeasure;
    if(msg.has_DecayQty) std::cout << " DecayQty=" << msg.DecayQty;
    if(msg.has_DecayStartDate) std::cout << " DecayStartDate=" << msg.DecayStartDate;
    if(msg.has_OriginalContractSize) std::cout << " OriginalContractSize=" << msg.OriginalContractSize;
    if(msg.has_ClearedVolume) std::cout << " ClearedVolume=" << msg.ClearedVolume;
    if(msg.has_OpenInterestQty) std::cout << " OpenInterestQty=" << msg.OpenInterestQty;
    for( auto entry : msg.EvntGrp) os << " EvntGrp=" << entry;
    for( auto entry : msg.SecAltIDGrp) os << " SecAltIDGrp=" << entry;
    for( auto entry : msg.MarketDataFeedTypes) os << " MarketDataFeedTypes=" << entry;
    for( auto entry : msg.UndInstrmtGrp) os << " UndInstrmtGrp=" << entry;
    for( auto entry : msg.AttrbGrp) os << " AttrbGrp=" << entry;
    for( auto entry : msg.InstrmtLegGrp) os << " InstrmtLegGrp=" << entry;
    for( auto entry : msg.LotTypeRules) os << " LotTypeRules=" << entry;
    return os;
}

struct SecurityStatus : public msg_base
{
    char *                                   SecurityID;
    char *                                   SecurityIDSource;
    char *                                   TradeDate;
    char *                                   HighPx;
    char *                                   LowPx;
    char *                                   Symbol;
    char *                                   SecurityTradingStatus;
    char *                                   HaltReasonInt;
    char *                                   SecurityTradingEvent;
    char *                                   SecurityDesc;
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
    virtual void reset()
    {
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
    }

    std::set<int> poss_fields = {22,48,55,75,107,326,327,332,333,1174};
    std::set<int> sim_fields = {22,48,55,75,107,326,327,332,333,1174};
    std::map<int,int> field_to_comp = {};
    std::map<int,int> field_to_group_comp = {};
    std::set<int> group_count_fields = {};
    virtual std::set<int> & possible_fields() {
        return poss_fields;
    }
    virtual std::set<int> & simple_fields() {
        return sim_fields;
    }
    virtual int get_component_id( int field_id ) {
        assert( field_to_comp.find(field_id) != field_to_comp.end());
        return field_to_comp[field_id];
    }
    virtual int get_group_component_id( int field_id ) {
        assert(field_to_group_comp.find(field_id) != field_to_group_comp.end());
        return field_to_group_comp[field_id];
    }
    virtual bool is_group_count_field( int field_id ) {
        return (group_count_fields.find(field_id) != group_count_fields.end());
    }
    virtual void prepare_group(int key, char * value)
    {
        grp_field_id = key;
        int group_count = atoi(value);
        assert(group_count > 0);
        assert(group_count < 100);
        switch(grp_field_id)
        {
        default:
            assert(false);
        }
    }
    virtual bool  advance_group()
    {
        if(grp_field_id < 0) return false;
        switch(grp_field_id)
        {
        default:
            assert(false);
            return true;
        }
    }
    virtual void prepare_component(int key)
    {
        switch(key)
        {
        default:
            assert(false);
        }
    }
    virtual bool set_field(int key, char * value)
    {
        switch(key)
        {
        case 22:
            if(has_SecurityIDSource == true) return false;
            SecurityIDSource = value;
            has_SecurityIDSource = true;
            return true;
        case 48:
            if(has_SecurityID == true) return false;
            SecurityID = value;
            has_SecurityID = true;
            return true;
        case 55:
            if(has_Symbol == true) return false;
            Symbol = value;
            has_Symbol = true;
            return true;
        case 75:
            if(has_TradeDate == true) return false;
            TradeDate = value;
            has_TradeDate = true;
            return true;
        case 107:
            if(has_SecurityDesc == true) return false;
            SecurityDesc = value;
            has_SecurityDesc = true;
            return true;
        case 326:
            if(has_SecurityTradingStatus == true) return false;
            SecurityTradingStatus = value;
            has_SecurityTradingStatus = true;
            return true;
        case 327:
            if(has_HaltReasonInt == true) return false;
            HaltReasonInt = value;
            has_HaltReasonInt = true;
            return true;
        case 332:
            if(has_HighPx == true) return false;
            HighPx = value;
            has_HighPx = true;
            return true;
        case 333:
            if(has_LowPx == true) return false;
            LowPx = value;
            has_LowPx = true;
            return true;
        case 1174:
            if(has_SecurityTradingEvent == true) return false;
            SecurityTradingEvent = value;
            has_SecurityTradingEvent = true;
            return true;
        default:
            assert(false);
            return false;
        }
    };

};

std::ostream& operator<<(std::ostream& os, const SecurityStatus& msg)
{
    if(msg.has_SecurityID) std::cout << " SecurityID=" << msg.SecurityID;
    if(msg.has_SecurityIDSource) std::cout << " SecurityIDSource=" << msg.SecurityIDSource;
    if(msg.has_TradeDate) std::cout << " TradeDate=" << msg.TradeDate;
    if(msg.has_HighPx) std::cout << " HighPx=" << msg.HighPx;
    if(msg.has_LowPx) std::cout << " LowPx=" << msg.LowPx;
    if(msg.has_Symbol) std::cout << " Symbol=" << msg.Symbol;
    if(msg.has_SecurityTradingStatus) std::cout << " SecurityTradingStatus=" << msg.SecurityTradingStatus;
    if(msg.has_HaltReasonInt) std::cout << " HaltReasonInt=" << msg.HaltReasonInt;
    if(msg.has_SecurityTradingEvent) std::cout << " SecurityTradingEvent=" << msg.SecurityTradingEvent;
    if(msg.has_SecurityDesc) std::cout << " SecurityDesc=" << msg.SecurityDesc;
    return os;
}


template< typename handler >
class fix_parser
{
public:

    fix_parser( handler *h): _h(h)
    {
        _tok =  new fix_tokenizer<fix_parser<handler> >(this);
    }

    fix_parser()
    {
        delete _tok;
    }

    bool parse(char * s)
    {
        return _tok->parse(s);
    }

    void on_msg_start()
    {
        _Header.reset();
        _msg_stack.push(&_Header);
    }

    void on_msg_field( int key, char * value )
    {
        //std::cout << "processing " << key << "=" << value << std::endl;


        while(true)
        {
            switch(_msg_stack.top()->on_msg_field(key, value))
            {
            case FIELD_TYPE::SIMPLE_FIELD:
                //std::cout << "stack size "<<_msg_stack.size()<< std::endl;
                return;

            case FIELD_TYPE::UNKNOWN_FIELD:
                if(_msg_stack.top() == &_Header)
                {
                    _msg_stack.pop();
                    switch( MsgTypeToInt(_Header.MsgType) )
                    {
                    case MSG::XXMarketDataIncrementalRefresh:
                        _MarketDataIncrementalRefresh.reset();
                        _msg_stack.push(&_MarketDataIncrementalRefresh);
                        break;
                    case MSG::XXSecurityDefinition:
                        _SecurityDefinition.reset();
                        _msg_stack.push(&_SecurityDefinition);
                        break;
                    case MSG::XXSecurityStatus:
                        _SecurityStatus.reset();
                        _msg_stack.push(&_SecurityStatus);
                        break;
                    }
                }
                else if(_msg_stack.size() == 1)
                {


                    if(_msg_stack.top() == &_Trailer )
                    {
                        //std::cout << "Ignoring Unknown field " << key  << std::endl;
                        return;
                    }
                    _msg_stack.pop();
                    _Trailer.reset();
                    _msg_stack.push(&_Trailer);
                }
                else if(_msg_stack.size() > 1)
                {
                    _msg_stack.pop();
                }
                break;

            case FIELD_TYPE::GROUP_COUNT_FIELD:
                _msg_stack.push(_msg_stack.top()->get_group());
                _msg_stack.top()->reset();
                //std::cout << "stack size "<<_msg_stack.size()<< std::endl;
                return;

            case FIELD_TYPE::DUPLICATE_FIELD:
                _msg_stack.pop();
                while (!_msg_stack.top()->advance_group() && _msg_stack.size() > 1) {
                    _msg_stack.pop();
                    //std::cout << "stack size "<<_msg_stack.size()<< std::endl;
                }
                _msg_stack.push(_msg_stack.top()->get_group());
                _msg_stack.top()->reset();
                break;

            case FIELD_TYPE::COMPONENT_FIELD:
                _msg_stack.push(_msg_stack.top()->get_component());
                _msg_stack.top()->reset();
                break;

            default:
                assert(false);
            };

            //std::cout << "stack size "<<_msg_stack.size()<< std::endl;
        }
    }

    void on_msg_end()
    {
        switch( MsgTypeToInt(_Header.MsgType) )
        {
        case MSG::XXMarketDataIncrementalRefresh:
            (*_h)(_Header, _MarketDataIncrementalRefresh);
            break;
        case MSG::XXSecurityDefinition:
            (*_h)(_Header, _SecurityDefinition);
            break;
        case MSG::XXSecurityStatus:
            (*_h)(_Header, _SecurityStatus);
            break;
        }
        while(!_msg_stack.empty())
            _msg_stack.pop();
    }

private:

    handler *_h;
    fix_tokenizer<fix_parser<handler> > * _tok;
    std::stack<msg_base *> _msg_stack;

    Header                                   _Header;
    Trailer                                  _Trailer;
    MarketDataIncrementalRefresh             _MarketDataIncrementalRefresh;
    SecurityDefinition                       _SecurityDefinition;
    SecurityStatus                           _SecurityStatus;
};

}

#endif // ELEMENTS_FIX_PARSER
