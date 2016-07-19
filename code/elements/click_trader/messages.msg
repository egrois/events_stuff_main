enum LATENCY_TYPE 
{
    LT_FEED = 1,
    LT_PARAMS = 2,
    LT_TIMERS = 3,
    LT_PARSE = 4,
    LT_DEFAULT = 5,
    LT_LX_POLL = 6,
    LT_EPOLL = 7,
    LT_CME_OMS = 8,
    LT_CME_OMS2 = 9
}

struct latency
{
    LATENCY_TYPE type;
    int nanos;
};

struct pnl_status
{
    char sym[10];
    double pnl;
    double bid;
    double ask;
    int bsz;
    int asz;
    int pos;
    int vol;
    double mp;
    double cash;
};

enum ORDER_ACTION 
{
    NEW = 1,
    CANCEL = 2,
    MODIFY = 3
};

enum ORDER_SIDE 
{
    BUY = 1,
    SELL = 2    
};

enum ORDER_TIF 
{
    DAY = '0',
    FILL_OR_KILL = '3'
};

enum ORDER_EVENT 
{
    NEW_SUB = 0,
    CANCEL_SUB = 1,
    MODIFY_SUB = 2,
    NEW_ACK = 3,
    CANCEL_ACK = 4,
    MODIFY_ACK = 5,
    FILL = 6,
    PARTIAL_FILL = 7,
    EXPIRED = 8,
    NEW_REJECT = 9,
    CANCEL_REJECT = 10, 
    MODIFY_REJECT = 11
};

struct order_event 
{
    char                sym[10];
    ORDER_EVENT         type;
    ORDER_ACTION        action;
    int64_t             order_id;
    int                 sec_id;
    ORDER_SIDE          side;
    double              price;
    int                 size;
    ORDER_TIF           tif;
    int                 last_qty;
    int                 leaves_qty;
    double              last_px;
};

enum EVENT_PROGRESS
{
    START = 0,
    MIDDLE = 1,
    END = 2
};

struct order_book_entry
{
    EVENT_PROGRESS  progress;
    char            sym[10];
    ORDER_SIDE      side;
    double          price;
    int             size;
    int64_t         id;
};

struct eco_event_msg 
{
    char sym[40];
    bool has_actual;
    double actual;
    bool has_revised;
    double revised;
};

struct md_book
{
    char sym[40];
    double mpi;
    double top_bid_price;
    double top_ask_price;
    int b0;
    int b1;
    int b2;
    int b3;
    int b4;
    int b5;
    int b6;
    int b7;
    int b8;
    int b9;
    int a0;
    int a1;
    int a2;
    int a3;
    int a4;
    int a5;
    int a6;
    int a7;
    int a8;
    int a9;
    int pos;
    double last_trade_price; 
};
