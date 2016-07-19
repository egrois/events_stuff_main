#ifndef  ELEMENTS_MESSAGES_H
#define  ELEMENTS_MESSAGES_H

#include <iosfwd>
namespace elements
{
    enum ORDER_SIDE
    {
        BUY = 1,
        SELL = 2
    };

    std::string ORDER_SIDE_to_string(ORDER_SIDE var)
    {
        switch(var)
        {
        case BUY:
            return "BUY";
        case SELL:
            return "SELL";
        default:
            return "NA";
        }
    }
    enum ORDER_TIF
    {
        DAY = '0',
        FILL_OR_KILL = '3'
    };

    std::string ORDER_TIF_to_string(ORDER_TIF var)
    {
        switch(var)
        {
        case DAY:
            return "DAY";
        case FILL_OR_KILL:
            return "FILL_OR_KILL";
        default:
            return "NA";
        }
    }
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

    std::string ORDER_EVENT_to_string(ORDER_EVENT var)
    {
        switch(var)
        {
        case NEW_SUB:
            return "NEW_SUB";
        case CANCEL_SUB:
            return "CANCEL_SUB";
        case MODIFY_SUB:
            return "MODIFY_SUB";
        case NEW_ACK:
            return "NEW_ACK";
        case CANCEL_ACK:
            return "CANCEL_ACK";
        case MODIFY_ACK:
            return "MODIFY_ACK";
        case FILL:
            return "FILL";
        case PARTIAL_FILL:
            return "PARTIAL_FILL";
        case EXPIRED:
            return "EXPIRED";
        case NEW_REJECT:
            return "NEW_REJECT";
        case CANCEL_REJECT:
            return "CANCEL_REJECT";
        case MODIFY_REJECT:
            return "MODIFY_REJECT";
        default:
            return "NA";
        }
    }
    enum ORDER_ACTION
    {
        NEW = 1,
        CANCEL = 2,
        MODIFY = 3
    };

    std::string ORDER_ACTION_to_string(ORDER_ACTION var)
    {
        switch(var)
        {
        case NEW:
            return "NEW";
        case CANCEL:
            return "CANCEL";
        case MODIFY:
            return "MODIFY";
        default:
            return "NA";
        }
    }
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
    };

    std::string LATENCY_TYPE_to_string(LATENCY_TYPE var)
    {
        switch(var)
        {
        case LT_FEED:
            return "LT_FEED";
        case LT_PARAMS:
            return "LT_PARAMS";
        case LT_TIMERS:
            return "LT_TIMERS";
        case LT_PARSE:
            return "LT_PARSE";
        case LT_DEFAULT:
            return "LT_DEFAULT";
        case LT_LX_POLL:
            return "LT_LX_POLL";
        case LT_EPOLL:
            return "LT_EPOLL";
        case LT_CME_OMS:
            return "LT_CME_OMS";
        case LT_CME_OMS2:
            return "LT_CME_OMS2";
        default:
            return "NA";
        }
    }
    enum EVENT_PROGRESS
    {
        START = 0,
        MIDDLE = 1,
        END = 2
    };

    std::string EVENT_PROGRESS_to_string(EVENT_PROGRESS var)
    {
        switch(var)
        {
        case START:
            return "START";
        case MIDDLE:
            return "MIDDLE";
        case END:
            return "END";
        default:
            return "NA";
        }
    }

    enum MSG_TYPE
    {
        MSG_ORDER_BOOK_ENTRY = 0,
        MSG_LATENCY = 1,
        MSG_MD_BOOK = 2,
        MSG_ORDER_EVENT = 3,
        MSG_PNL_STATUS = 4,
        MSG_ECO_EVENT_MSG = 5
    };

    struct message_base
    {
        uint64_t epoch;

    };

    struct order_book_entry : public message_base
    {
        EVENT_PROGRESS progress;
        char sym[10];
        ORDER_SIDE side;
        double price;
        int size;
        int64_t id;
    };

    struct latency : public message_base
    {
        LATENCY_TYPE type;
        int nanos;
    };

    std::ostream& operator <<(std::ostream& os, const latency & msg)
    {
        return  os << str(from_epoch_nanos(msg.epoch)) << " MSG_LATENCY " << " type=" << LATENCY_TYPE_to_string(msg.type)  << " nanos=" << msg.nanos ;
    }

    struct md_book : public message_base
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

    struct order_event : public message_base
    {
        char sym[10];
        ORDER_EVENT type;
        ORDER_ACTION action;
        int64_t order_id;
        int sec_id;
        ORDER_SIDE side;
        double price;
        int size;
        ORDER_TIF tif;
        int last_qty;
        int leaves_qty;
        double last_px;
    };

    std::ostream& operator <<(std::ostream& os, const order_event & msg)
    {
        return  os << str(from_epoch_nanos(msg.epoch)) << " MSG_ORDER_EVENT " << " sym=" << msg.sym  << " type=" << ORDER_EVENT_to_string(msg.type)  << " action=" << ORDER_ACTION_to_string(msg.action)  << " order_id=" << msg.order_id  << " sec_id=" << msg.sec_id  << " side=" << ORDER_SIDE_to_string(msg.side)  << " price=" << msg.price  << " size=" << msg.size  << " tif=" << ORDER_TIF_to_string(msg.tif)  << " last_qty=" << msg.last_qty  << " leaves_qty=" << msg.leaves_qty  << " last_px=" << msg.last_px ;
    }

    struct pnl_status : public message_base
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

    std::ostream& operator <<(std::ostream& os, const pnl_status & msg)
    {
        return  os << str(from_epoch_nanos(msg.epoch)) << " MSG_PNL_STATUS " << " sym=" << msg.sym  << " pnl=" << msg.pnl  << " bid=" << msg.bid  << " ask=" << msg.ask  << " bsz=" << msg.bsz  << " asz=" << msg.asz  << " pos=" << msg.pos  << " vol=" << msg.vol  << " mp=" << msg.mp  << " cash=" << msg.cash ;
    }

    struct eco_event_msg : public message_base
    {
        char sym[40];
        bool has_actual;
        double actual;
        bool has_revised;
        double revised;
    };

    std::ostream& operator <<(std::ostream& os, const eco_event_msg & msg)
    {
        return  os << str(from_epoch_nanos(msg.epoch)) << " MSG_ECO_EVENT_MSG " << " sym=" << msg.sym  << " has_actual=" << msg.has_actual  << " actual=" << msg.actual  << " has_revised=" << msg.has_revised  << " revised=" << msg.revised ;
    }

    struct log_message
    {
        MSG_TYPE type;
        union
        {
            order_book_entry a_order_book_entry;
            latency a_latency;
            md_book a_md_book;
            order_event a_order_event;
            pnl_status a_pnl_status;
            eco_event_msg a_eco_event_msg;
        } uber;
    };
    
#include <printers.h>

    std::ostream& operator <<(std::ostream& os, const log_message & msg)
    {
        switch(msg.type)
        {
        case MSG_TYPE::MSG_ORDER_BOOK_ENTRY:
            os << msg.uber.a_order_book_entry << std::endl;
            break;
        case MSG_TYPE::MSG_LATENCY:
            os << msg.uber.a_latency << std::endl;
            break;
        case MSG_TYPE::MSG_MD_BOOK:
            os << msg.uber.a_md_book << std::endl;
            break;
        case MSG_TYPE::MSG_ORDER_EVENT:
            os << msg.uber.a_order_event << std::endl;
            break;
        case MSG_TYPE::MSG_PNL_STATUS:
            os << msg.uber.a_pnl_status << std::endl;
            break;
        case MSG_TYPE::MSG_ECO_EVENT_MSG:
            os << msg.uber.a_eco_event_msg << std::endl;
            break;
        }
    }
}
#endif // ELEMENTS_MESSAGES_H
