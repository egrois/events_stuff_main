#include "params.pb.h"
#include "strategy.h" 
#include <cmath>
#include <math.h>
#include <sstream>
#include <limits>
#include <snapshot.h>

using namespace elements;

struct perpetual
{
    perpetual(std::vector<double> lambdas_in, int64_t min_price_increment_in)
    {
        lambdas = lambdas_in;
        min_price_increment = min_price_increment_in; 
    }

    ~perpetual(){}

    int64_t min_price_increment;

    double midquote = NAN;
    double spread = NAN;
    double microprice = NAN;
    double microprice2 = NAN;
    
    double top_bid_price = NAN;
    double bid_size_0 = NAN;
    double bid_size_1 = NAN;
    double bid_size_2 = NAN;
    double bid_size_3 = NAN;
    double bid_size_4 = NAN;

    double top_ask_price = NAN;
    double ask_size_0 = NAN;
    double ask_size_1 = NAN;
    double ask_size_2 = NAN;
    double ask_size_3 = NAN;
    double ask_size_4 = NAN;
        
    std::vector<double> lambdas;
    std::vector<double> ask_vol_ema;
    std::vector<double> bid_vol_ema;
    std::vector<double> microprice_ema;
    std::vector<double> trade_price_ema;
   
    double bid_num_0 = NAN;
    double bid_num_1 = NAN;
    double bid_num_2 = NAN;
    double bid_num_3 = NAN;
    double bid_num_4 = NAN;
    double ask_num_0 = NAN;
    double ask_num_1 = NAN;
    double ask_num_2 = NAN;
    double ask_num_3 = NAN;
    double ask_num_4 = NAN;
    double imbalance = NAN;
    double imbalance2 = NAN; 
    double imbalance3 = NAN; 
 
    void prepare_emas()
    {
        for(auto lam : lambdas)
        {
            ask_vol_ema.push_back(0.0);
            bid_vol_ema.push_back(0.0);
            microprice_ema.push_back(NAN);
            trade_price_ema.push_back(NAN);
        }
    } 
};

struct symbol_state
{
    int64_t                     min_price_increment;
    std::string                 symbol;
    std::string                 date;
    perpetual                   *perpet;
    std::vector<snapshot*>      snapshots;
    std::vector<milliseconds>   halflives;
    std::vector<std::string>    halflive_names;
    std::vector<milliseconds>   intervals;
    std::vector<double>         lambdas;
    std::vector<double>         alpha;
    std::vector<double>         beta;
    timestamp                   last_trade;
    timestamp                   last_md;
    std::string                 staging_root;
    price_level_book            *book; 
    std::map<uint32_t, int32_t> *timer_id_to_index;   

    symbol_state(   std::string                     _symbol,
                    std::string                     _date,
                    std::string                     _staging_root,
                    int64_t                         _min_price_increment,
                    std::vector<milliseconds>       _halflives,
                    std::vector<std::string>        _halflive_names,
                    std::vector<milliseconds>       _intervals,
                    price_level_book                *_book,
                    std::map<uint32_t,int32_t>      *_timer_id_to_index)
    {
        symbol              = _symbol;
        date                = _date;
        staging_root        = _staging_root; 
        min_price_increment = _min_price_increment;
        halflives           = _halflives;
        halflive_names      = _halflive_names;
        intervals           = _intervals;
        book                = _book;
        timer_id_to_index   = _timer_id_to_index; 

        snapshot *tyty = new snapshot();
        for(auto interval: intervals)
            snapshots.push_back(new snapshot());

        for(auto h: halflives)
        {
            double secs = ((double)std::chrono::duration_cast<microseconds>(h).count()) * 1.0e-6 ;
            double lambda =  std::log( 2.0 ) / secs;

            lambdas.push_back(lambda);
            alpha.push_back(0.0);
            beta.push_back(0.0);
        }
        perpet = new perpetual(lambdas, min_price_increment);
        perpet->prepare_emas();  
    }
    
    ~symbol_state()
    {
        for(auto snap: snapshots)
            delete snap;
        delete perpet;
    }

    void compute_alphas(microseconds dt)
    {
        for(int i = 0; i < lambdas.size(); ++i)
        {
            alpha[i] = std::exp( - (double)lambdas[i] * (double)std::chrono::duration_cast<microseconds>(dt).count() * 1.e-6 );
            beta[i] = 1.0 - alpha[i];
        }
    }

    void on_market_data(timestamp ts) 
    {
        int64_t mpi = min_price_increment;
        if(!book->has_top())
            return;

        perpet->midquote = book->midquote();
        perpet->spread = book->spread();
        perpet->microprice = book->microprice();
        perpet->microprice2 = book->microprice2();
            
        perpet->top_bid_price = book->top_price(BookSide::BID);
        perpet->bid_size_0 = book->size_at_price(perpet->top_bid_price,             BookSide::BID);
        perpet->bid_size_1 = book->size_at_price(perpet->top_bid_price - 1.0 * mpi, BookSide::BID);
        perpet->bid_size_2 = book->size_at_price(perpet->top_bid_price - 2.0 * mpi, BookSide::BID);
        perpet->bid_size_3 = book->size_at_price(perpet->top_bid_price - 3.0 * mpi, BookSide::BID);
        perpet->bid_size_4 = book->size_at_price(perpet->top_bid_price - 4.0 * mpi, BookSide::BID);

        perpet->top_ask_price = book->top_price(BookSide::ASK);
        perpet->ask_size_0 = book->size_at_price(perpet->top_ask_price,             BookSide::ASK);
        perpet->ask_size_1 = book->size_at_price(perpet->top_ask_price + 1.0 * mpi, BookSide::ASK);
        perpet->ask_size_2 = book->size_at_price(perpet->top_ask_price + 2.0 * mpi, BookSide::ASK);
        perpet->ask_size_3 = book->size_at_price(perpet->top_ask_price + 3.0 * mpi, BookSide::ASK);
        perpet->ask_size_4 = book->size_at_price(perpet->top_ask_price + 4.0 * mpi, BookSide::ASK); 
      
        perpet->bid_num_0 = book->num_orders_at_price(perpet->top_bid_price,              BookSide::BID);
        perpet->bid_num_1 = book->num_orders_at_price(perpet->top_bid_price  - 1.0 * mpi, BookSide::BID);
        perpet->bid_num_2 = book->num_orders_at_price(perpet->top_bid_price  - 2.0 * mpi, BookSide::BID);
        perpet->bid_num_3 = book->num_orders_at_price(perpet->top_bid_price  - 3.0 * mpi, BookSide::BID);
        perpet->bid_num_4 = book->num_orders_at_price(perpet->top_bid_price  - 4.0 * mpi, BookSide::BID);
        
        perpet->ask_num_0 = book->num_orders_at_price(perpet->top_ask_price,              BookSide::ASK);
        perpet->ask_num_1 = book->num_orders_at_price(perpet->top_ask_price + 1.0 * mpi,  BookSide::ASK);
        perpet->ask_num_2 = book->num_orders_at_price(perpet->top_ask_price + 2.0 * mpi,  BookSide::ASK);
        perpet->ask_num_3 = book->num_orders_at_price(perpet->top_ask_price + 3.0 * mpi,  BookSide::ASK);
        perpet->ask_num_4 = book->num_orders_at_price(perpet->top_ask_price + 4.0 * mpi,  BookSide::ASK);
        
        perpet->imbalance = book->imbalance(); 
        perpet->imbalance2 = book->imbalance2(); 
        perpet->imbalance3 =  book->imbalance3();
 
        if(last_md >= ts)
            return;
       
        auto td = ts - last_md;
        last_md = ts;

        compute_alphas(std::chrono::duration_cast<microseconds>(td));
       
        for(int i = 0; i < lambdas.size(); ++i)
        {
            if( isnan( perpet->microprice_ema[i]))
            {
                perpet->microprice_ema[i] = perpet->microprice * 1.0e-7;
            } 
            else
            {
                perpet->microprice_ema[i] *= alpha[i];
                perpet->microprice_ema[i] += beta[i] * perpet->microprice * 1.0e-7;
            }
            perpet->ask_vol_ema[i] *= alpha[i];
            perpet->bid_vol_ema[i] *= alpha[i];
            
        }
    }

    void on_trade(cme::MDEntry &r, timestamp ts) 
    {
        for(auto snap: snapshots)
            snap->on_trade(r);
        
        auto td = milliseconds(0);
        if(last_trade < ts)
        {
            td = std::chrono::duration_cast<milliseconds>(ts - last_trade);
            last_trade = ts;
        }

        compute_alphas(td);
        
        for(int i = 0; i < lambdas.size(); ++i)
        { 
            if(isnan(perpet->trade_price_ema[i]))
            {
                perpet->trade_price_ema[i] = (double)r.MDEntryPx * 1.0e-7;
            }
            else
            {
                perpet->trade_price_ema[i] *= alpha[i];
                perpet->trade_price_ema[i] += beta[i] * (double)r.MDEntryPx * 1.0e-7; 
            }
        } 

        for(int i = 0; i < lambdas.size(); ++i)
        { 
            if(r.AggressorSide == cme::AggressorSideEnum::BUY)
            {
                perpet->ask_vol_ema[i] += (double)r.MDEntrySize;
            }
            else if (r.AggressorSide == cme::AggressorSideEnum::SELL)
            {
                perpet->bid_vol_ema[i] += (double)r.MDEntrySize;
            }
        }
    }

    void on_timer( timestamp ts, uint32_t id)
    {
        on_market_data(ts);
        
        std::stringstream ss;
        
        double_out(ss,(double)perpet->midquote * 1e-7) << ",";
        double_out(ss,perpet->spread * 1e-7) << ",";
        double_out(ss,perpet->microprice * 1e-7) << ",";
        double_out(ss,perpet->microprice2 * 1e-7) << ",";
        double_out(ss,perpet->top_bid_price * 1e-7) << ",";
        ss << perpet->bid_size_0 << ",";
        ss << perpet->bid_size_1 << ",";
        ss << perpet->bid_size_2 << ",";
        ss << perpet->bid_size_3 << ",";
        ss << perpet->bid_size_4 << ",";
        double_out(ss,perpet->top_ask_price * 1e-7) << ",";
        ss << perpet->ask_size_0 << ",";
        ss << perpet->ask_size_1 << ",";
        ss << perpet->ask_size_2 << ",";
        ss << perpet->ask_size_3 << ",";
        ss << perpet->ask_size_4 << ",";
        ss << perpet->bid_num_0 << ",";
        ss << perpet->bid_num_1 << ",";
        ss << perpet->bid_num_2 << ",";
        ss << perpet->bid_num_3 << ",";
        ss << perpet->bid_num_4 << ",";
        ss << perpet->ask_num_0 << ",";
        ss << perpet->ask_num_1 << ",";
        ss << perpet->ask_num_2 << ",";
        ss << perpet->ask_num_3 << ",";
        ss << perpet->ask_num_4 << ",";
        ss << perpet->imbalance << ",";
        ss << perpet->imbalance2 << ",";
        ss << perpet->imbalance3 << ",";

        for(int i = 0; i < halflives.size(); ++i)
            double_out(ss,perpet->ask_vol_ema[i]) << ",";

        for(int i = 0; i < halflives.size(); ++i)
            double_out(ss,perpet->bid_vol_ema[i]) << ",";

        for(int i = 0; i < halflives.size(); ++i)
            double_out(ss,perpet->microprice_ema[i]) << ",";

        for(int i = 0; i < halflives.size(); ++i)
        {
            if( i ==  halflives.size() -1 )
                double_out(ss,perpet->trade_price_ema[i]);
            else
                double_out(ss,perpet->trade_price_ema[i]) << ",";
                
        }
        snapshots[(*timer_id_to_index)[id]]->save_and_reset( symbol, ts, ss.str());
    }
    
    void on_end_of_day()
    {
        std::list<std::string> columns = { 
                    "symbol", 
                    "time", 
                    "abs_vol", 
                    "buy_vol", 
                    "sell_vol",
                    "high",
                    "low",
                    "open",
                    "close",
                    "midquote",
                    "spread",
                    "microprice",
                    "microprice2",
                    "top_bid_price",
                    "bid_size_0",
                    "bid_size_1",
                    "bid_size_2",
                    "bid_size_3",
                    "bid_size_4",
                    "top_ask_price",
                    "ask_size_0",
                    "ask_size_1",
                    "ask_size_2",
                    "ask_size_3",
                    "ask_size_4",
                    "bid_num_0",
                    "bid_num_1",
                    "bid_num_2",
                    "bid_num_3",
                    "bid_num_4",
                    "ask_num_0",
                    "ask_num_1",
                    "ask_num_2",
                    "ask_num_3",
                    "ask_num_4",
                    "imbalance",
                    "imbalance2",
                    "imbalance3"
                    };
        
        for(int i = 0; i < halflives.size(); ++i)
            columns.push_back("ask_vol_ema_" + halflive_names[i]);
        for(int i = 0; i < halflives.size(); ++i)
            columns.push_back("bid_vol_ema_" + halflive_names[i]);
        for(int i = 0; i < halflives.size(); ++i)
            columns.push_back("microprice_ema_" + halflive_names[i]);
        for(int i = 0; i < halflives.size(); ++i)
            columns.push_back("trade_price_ema_" + halflive_names[i]);
        
        std::stringstream s;
        for(auto col: columns)
        {
            s << col << ",";
        }
        
        std::string col_names = s.str();
        col_names = col_names.substr(0, col_names.size() - 1);        

        for(int i = 0; i < snapshots.size(); ++i)
        {
            std::stringstream ss;
            ss << staging_root;
            ss << "/";
            ss << symbol;
            ss << "_";
            ss << i;
            ss << "_";
            ss << date;
            ss << ".txt"; 

            std::cout << "writing " << ss.str() << std::endl;

            std::ofstream ofs (ss.str().c_str(), std::ofstream::out);
            ofs << col_names << std::endl;
            for(auto rec: snapshots[i]->saved)
                ofs << rec << std::endl;
        }
    }
};

class snapgen : public strategy<params::Params>
{
public:
    snapgen()
    {
    }

    virtual ~snapgen()
    {
       for(auto state: _states) delete state;
    }

    virtual void on_bb_event(eco_event const &event) 
    {
    }
    virtual void on_security_status(cme::SecurityStatus &msg) 
    {
    }

    virtual void on_start_of_day()
    {
        log("INFO", "on_start_of_day");
        for( auto ins : get_instruments())
        {
           _states.push_back(
                new symbol_state(   
                        ins->symbol(),
                        get_date(),
                        _staging_root,
                        ins->book().min_price_increment(),
                        _halflives,
                        _halflive_names,
                        _intervals,
                        (price_level_book *)&ins->book(),
                        &_timer_id_to_index));
            
            _states2[ins->secdef().SecurityID] = _states.back();
        }

        timestamp end_of_day = make_time(get_date(), _end_of_day_time);
        timestamp snap_start = make_time(get_date(), _snap_start_time);
        timestamp snap_stop  = make_time(get_date(), _snap_stop_time);

        log("INFO", " eod=" +str(end_of_day) +" snap_start="+ str(snap_start) + " snap_stop=" + str(snap_stop));

        _eod_timer_id = register_timer(end_of_day);
        for(int i = 0; i < _intervals.size(); ++i)
        {
            uint32_t id = register_timer(snap_start, snap_stop, _intervals[i]);
            _timer_id_to_index[id] = i;
            std::cout << "register timer " << id << std::endl;
        }
    }

    virtual void on_incremental_refresh(cme::IncrementalRefresh &msg) 
    {
        for(auto state: _states)
            state->on_market_data(now());
 
        for(auto entry: msg.MDEntries)
        {
            if(entry.MDEntryType != cme::MDEntryTypeEnum::TRADE)
                continue;

            if( entry.has_TradeCondition)
                continue;

            auto it = _states2.find( entry.SecurityID );
            if(it != _states2.end())
                it->second->on_trade(entry, now());
        }
    }

    virtual void on_timer(timestamp & t, int32_t timer_id)
    {
        if(timer_id == _eod_timer_id)
        {
            for (auto state: _states)
                state->on_end_of_day();
            terminate();
        }
        else if(_timer_id_to_index.find(timer_id) != _timer_id_to_index.end())
        {
            for (auto state: _states)
                state->on_timer(t,timer_id);
        }
    }

private:
    
    std::vector<symbol_state *> _states;
    std::map<int32_t, symbol_state * > _states2; 
    std::string                 _staging_root       = "/local/disk1/staging/";
    std::vector<milliseconds>   _halflives          = { milliseconds(200), milliseconds(2000), milliseconds(10000)};
    std::vector<std::string>    _halflive_names     = { "200ms", "2s", "10s" };
    std::vector<milliseconds>   _intervals          = { milliseconds(100), milliseconds(1000), milliseconds(5000)};
    std::map<uint32_t,int32_t>  _timer_id_to_index;
    uint32_t                    _eod_timer_id;
    std::string                 _snap_start_time    = "08:00:00.000";
    std::string                 _snap_stop_time     = "16:00:00.000";
    std::string                 _end_of_day_time    = "16:00:00.001";
};

int main( int argc, char ** argv)
{
    std:: string param_file = "strategy.param";

    if(argc > 1)
    {
        param_file = argv[1];
    }

    snapgen handler;
    snapgen::EventLoopType e;
    e.initialize(param_file, handler);
    e.run();
    return 0;
}
