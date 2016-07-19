import pybt
import numpy as np
from datetime import datetime,time, timedelta
import pandas as pd
import utils
import argparse
import random
import time as tm

def compute_alphas(lambdas, delay):
    alpha = []
    beta = []
    for l in lambdas:
        a = np.exp(- delay.total_seconds() * l)
        alpha.append(a)
        beta.append(1.0 - a)
    return (alpha,beta)

class snapshot:
    def __init__(self):
        self.abs_vol = 0 
        self.buy_vol = 0
        self.sell_vol = 0

        self.high = np.nan
        self.low = np.nan
        self.open = np.nan
        self.close = np.nan

        self.min_imbalance = np.nan
        self.max_imbalance = np.nan
        self.min_imbalance2 = np.nan
        self.max_imbalance2 = np.nan
        self.min_imbalance3 = np.nan
        self.max_imbalance3 = np.nan
        
        self.saved = []

    def save_and_reset(self, symbol, time, addon = []):
        a = [
                symbol, 
                time, 
                self.abs_vol, 
                self.buy_vol, 
                self.sell_vol,
                self.high,
                self.low,
                self.open,
                self.close,
                self.min_imbalance,
                self.max_imbalance,
                self.min_imbalance2,
                self.max_imbalance2,
                self.min_imbalance3,
                self.max_imbalance3
            ]
        a = a + addon
        self.saved.append(a)

        self.abs_vol = 0 
        self.buy_vol = 0
        self.sell_vol = 0
        self.high = np.nan 
        self.low = np.nan
        self.open = self.close 
        self.close = np.nan
        self.min_imbalance = np.nan
        self.max_imbalance = np.nan
        self.min_imbalance2 = np.nan
        self.max_imbalance2 = np.nan
        self.min_imbalance3 = np.nan
        self.max_imbalance3 = np.nan
        
    def on_trade(self, bt, r):
        if r.aggressorSide == 1:
            self.buy_vol += r.size
            self.abs_vol += r.size
        elif r.aggressorSide == 2:
            self.sell_vol += r.size
            self.abs_vol += r.size
        else:
            return
        if np.isnan(self.high) or self.high < r.price:
            self.high = r.price
        if np.isnan(self.low) or self.low > r.price:
            self.low = r.price
        self.close = r.price

    def on_market_data(self, bt, r, book):
        if not book.has_top():
            return         

        imb = book.imbalance()
        imb2 = book.imbalance2()
        imb3 = book.imbalance3()

        if np.isnan(self.min_imbalance) or imb < self.min_imbalance:
            self.min_imbalance = imb
        if np.isnan(self.max_imbalance) or imb > self.max_imbalance:
            self.max_imbalance = imb
        if np.isnan(self.min_imbalance2) or imb2 < self.min_imbalance2:
            self.min_imbalance2 = imb2
        if np.isnan(self.max_imbalance2) or imb2 > self.max_imbalance2:
            self.max_imbalance2 = imb2
        if np.isnan(self.min_imbalance3) or imb3 < self.min_imbalance3:
            self.min_imbalance3 = imb3
        if np.isnan(self.max_imbalance3) or imb3 > self.max_imbalance3:
            self.max_imbalance3 = imb3

class book_increments_counter:

    def __init__(self, lambd, levels_to_keep = 10, min_price_increment=None):
        self.data = {}
        self.last_time = datetime(1970,1,1)
        self.levels_to_keep = levels_to_keep
        self.mpi = min_price_increment
        self.lambd = lambd 
 
    def decay(self, tm, ref_price):
        td = tm - self.last_time
        if td <= timedelta( microseconds=0 ) : 
            return
        a = np.exp(- td.total_seconds() * self.lambd )
        for key in self.data.keys():
            if np.abs(key - ref_price)  <   self.levels_to_keep:
                self.data[key] *= a
            else:
                del self.data[key]
        self.last_time = tm 

    def register_increment(self, price, size):
        #assert(np.abs(size) < 1000000)
        if self.data.has_key(price):
            self.data[price] += float(size)
        else:
            self.data[price] = float(size)

    def get_increment(self, price):
        if self.data.has_key(price):
            return self.data[price]
        else:
            return 0.0

class perpetual:
    def __init__(self, lambdas =[], min_price_increment=None):
        self.min_price_increment = min_price_increment

        self.midquote = np.nan
        self.spread = np.nan
        self.microprice = np.nan
        self.microprice2 = np.nan
        
        self.top_bid_price = np.nan
        self.bid_size_0 = np.nan
        self.bid_size_1 = np.nan
        self.bid_size_2 = np.nan
        self.bid_size_3 = np.nan
        self.bid_size_4 = np.nan

        self.top_ask_price = np.nan
        self.ask_size_0 = np.nan
        self.ask_size_1 = np.nan
        self.ask_size_2 = np.nan
        self.ask_size_3 = np.nan
        self.ask_size_4 = np.nan
       
        self.lambdas = lambdas
 
        self.ask_vol_ema = []
        self.bid_vol_ema = []
        self.microprice_ema = []
        self.trade_price_ema = []
        self.ask_increments_ema = []
        self.bid_increments_ema = []

        self.bid_num_0 = np.nan
        self.bid_num_1 = np.nan
        self.bid_num_2 = np.nan
        self.bid_num_3 = np.nan
        self.bid_num_4 = np.nan
        self.ask_num_0 = np.nan
        self.ask_num_1 = np.nan
        self.ask_num_2 = np.nan
        self.ask_num_3 = np.nan
        self.ask_num_4 = np.nan
        
        self.imbalance = np.nan
        self.imbalance2 = np.nan 
        self.imbalance3 = np.nan 


    def prepare_emas(self):
        for lamb in self.lambdas:
            self.ask_vol_ema.append(0.0)
            self.bid_vol_ema.append(0.0)
            self.microprice_ema.append(np.nan)
            self.trade_price_ema.append(np.nan)
            self.ask_increments_ema.append( book_increments_counter(lamb, min_price_increment=self.min_price_increment))
            self.bid_increments_ema.append( book_increments_counter(lamb, min_price_increment=self.min_price_increment))

class symbol_state:
    def __init__(self, sym, date, staging_root='', min_price_increment = None):
        self.min_price_increment = min_price_increment
        self.symbol    = sym
        self.date      = date
        self.perpetual = perpetual(min_price_increment = min_price_increment)
        self.snapshots = []
        self.halflives = []
        self.halflive_names = []
        self.intervals = []
        self.lambdas = []
        self.last_trade = datetime(1970,1,1)
        self.last_md = datetime(1970,1,1)
        self.staging_root=staging_root

    def initialize(self, halflives, halflive_names, intervals):
        self.halflives = halflives
        self.intervals = intervals
        self.halflive_names = halflive_names
        for interval in self.intervals:
            self.snapshots.append(snapshot())
        for h in self.halflives:
            self.lambdas.append( np.log( 2.0 ) / float(h.total_seconds()))
        self.perpetual.lambdas = self.lambdas 
        self.perpetual.prepare_emas()  
 
    def on_market_data(self, bt, r):
        book = bt.states[self.symbol].book
        
        for snap in self.snapshots:
            snap.on_market_data(bt,r, book)
        
        mpi = book.min_price_increment
        if book.has_top():
            self.perpetual.midquote = book.midquote()
            self.perpetual.spread = book.spread()
            self.perpetual.microprice = book.microprice()
            self.perpetual.microprice2 = book.microprice2()
            self.perpetual.imbalance = book.imbalance()
            self.perpetual.imbalance2 = book.imbalance2()
            self.perpetual.imbalance3 = book.imbalance3()
            
            self.perpetual.top_bid_price = book.top_price('BID')
            self.perpetual.bid_size_0 = book.size_at_price('BID', self.perpetual.top_bid_price)
            self.perpetual.bid_size_1 = book.size_at_price('BID', self.perpetual.top_bid_price - 1.0 * mpi)
            self.perpetual.bid_size_2 = book.size_at_price('BID', self.perpetual.top_bid_price - 2.0 * mpi)
            self.perpetual.bid_size_3 = book.size_at_price('BID', self.perpetual.top_bid_price - 3.0 * mpi)
            self.perpetual.bid_size_4 = book.size_at_price('BID', self.perpetual.top_bid_price - 4.0 * mpi)
            
            self.perpetual.bid_num_0 = book.num_orders_at_price('BID', self.perpetual.top_bid_price)
            self.perpetual.bid_num_1 = book.num_orders_at_price('BID', self.perpetual.top_bid_price - 1.0 * mpi)
            self.perpetual.bid_num_2 = book.num_orders_at_price('BID', self.perpetual.top_bid_price - 2.0 * mpi)
            self.perpetual.bid_num_3 = book.num_orders_at_price('BID', self.perpetual.top_bid_price - 3.0 * mpi)
            self.perpetual.bid_num_4 = book.num_orders_at_price('BID', self.perpetual.top_bid_price - 4.0 * mpi)

            self.perpetual.top_ask_price = book.top_price('ASK')
            self.perpetual.ask_size_0 = book.size_at_price('ASK', self.perpetual.top_ask_price)
            self.perpetual.ask_size_1 = book.size_at_price('ASK', self.perpetual.top_ask_price + 1.0 * mpi)
            self.perpetual.ask_size_2 = book.size_at_price('ASK', self.perpetual.top_ask_price + 2.0 * mpi)
            self.perpetual.ask_size_3 = book.size_at_price('ASK', self.perpetual.top_ask_price + 3.0 * mpi)
            self.perpetual.ask_size_4 = book.size_at_price('ASK', self.perpetual.top_ask_price + 4.0 * mpi) 
            
            self.perpetual.ask_num_0 = book.num_orders_at_price('ASK', self.perpetual.top_ask_price)
            self.perpetual.ask_num_1 = book.num_orders_at_price('ASK', self.perpetual.top_ask_price + 1.0 * mpi)
            self.perpetual.ask_num_2 = book.num_orders_at_price('ASK', self.perpetual.top_ask_price + 2.0 * mpi)
            self.perpetual.ask_num_3 = book.num_orders_at_price('ASK', self.perpetual.top_ask_price + 3.0 * mpi)
            self.perpetual.ask_num_4 = book.num_orders_at_price('ASK', self.perpetual.top_ask_price + 4.0 * mpi) 

        else:
            return
        
        td = timedelta(seconds=0)
        if self.last_md < bt.time:
            td = bt.time - self.last_md
            self.last_md = bt.time
        else:
            return
        
        (alpha, beta) = compute_alphas(self.lambdas, td)
        
        for i in xrange(len(self.lambdas)):
            if np.isnan( self.perpetual.microprice_ema[i]):
                self.perpetual.microprice_ema[i] = self.perpetual.microprice 
            else:
                self.perpetual.microprice_ema[i] *= alpha[i]
                self.perpetual.microprice_ema[i] += beta[i] * self.perpetual.microprice

            self.perpetual.ask_vol_ema[i] *= alpha[i]
            self.perpetual.bid_vol_ema[i] *= alpha[i]

            self.perpetual.ask_increments_ema[i].decay(bt.time, self.perpetual.top_ask_price)
            self.perpetual.bid_increments_ema[i].decay(bt.time, self.perpetual.top_bid_price)

            if (book.change_info is not None) and (r is not None) :
                assert(np.abs(book.change_info.size) <= 1000000)
                if book.change_info.side == 'BID':
                    self.perpetual.bid_increments_ema[i].register_increment(book.change_info.price, book.change_info.size)
                elif book.change_info.side == 'ASK':
                    self.perpetual.ask_increments_ema[i].register_increment(book.change_info.price, book.change_info.size)

    def on_trade(self, bt, r):
        for snap in self.snapshots:
            snap.on_trade(bt,r)
            snap.on_market_data(bt, r, bt.states[self.symbol].book)
 
        td = timedelta(seconds=0)
        if self.last_trade < bt.time:
            td = bt.time - self.last_trade
            self.last_trade = bt.time
        (alpha, beta) = compute_alphas(self.lambdas, td)
        
        for i in xrange(len(self.lambdas)):
            if np.isnan( self.perpetual.trade_price_ema[i]):
                self.perpetual.trade_price_ema[i] = r.price
            else:
                self.perpetual.trade_price_ema[i] *= alpha[i]
                self.perpetual.trade_price_ema[i] += beta[i] * r.price 

        for i in xrange(len(self.lambdas)):
            if r.aggressorSide == 1:
                self.perpetual.ask_vol_ema[i] += r.size
            elif r.aggressorSide == 2:
                self.perpetual.bid_vol_ema[i] += r.size

    def on_timer(self, bt, timer):
        self.on_market_data(bt, None)
        perp_list = [
                self.perpetual.midquote,
                self.perpetual.spread,
                self.perpetual.microprice,
                self.perpetual.microprice2,
                self.perpetual.top_bid_price,
                self.perpetual.bid_size_0,
                self.perpetual.bid_size_1,
                self.perpetual.bid_size_2,
                self.perpetual.bid_size_3,
                self.perpetual.bid_size_4,
                self.perpetual.top_ask_price,
                self.perpetual.ask_size_0,
                self.perpetual.ask_size_1,
                self.perpetual.ask_size_2,
                self.perpetual.ask_size_3,
                self.perpetual.ask_size_4,
                self.perpetual.bid_num_0,
                self.perpetual.bid_num_1,
                self.perpetual.bid_num_2,
                self.perpetual.bid_num_3,
                self.perpetual.bid_num_4,
                self.perpetual.ask_num_0,
                self.perpetual.ask_num_1,
                self.perpetual.ask_num_2,
                self.perpetual.ask_num_3,
                self.perpetual.ask_num_4,
                self.perpetual.imbalance,
                self.perpetual.imbalance2,
                self.perpetual.imbalance3
            ]

        for i in xrange(len(self.halflives)):
            perp_list.append(self.perpetual.ask_vol_ema[i])
        for i in xrange(len(self.halflives)):
            perp_list.append(self.perpetual.bid_vol_ema[i])
        for i in xrange(len(self.halflives)):
            perp_list.append(self.perpetual.microprice_ema[i])
        for i in xrange(len(self.halflives)):
            perp_list.append(self.perpetual.trade_price_ema[i])
        #for i in xrange(len(self.halflives)):
        #    perp_list.append(self.perpetual.ask_increments_ema[i].get_increment(self.perpetual.top_ask_price))
        #    perp_list.append(self.perpetual.ask_increments_ema[i].get_increment(self.perpetual.top_ask_price - self.min_price_increment))
        #    perp_list.append(self.perpetual.ask_increments_ema[i].get_increment(self.perpetual.top_ask_price + self.min_price_increment))
        #for i in xrange(len(self.halflives)):
        #    perp_list.append(self.perpetual.bid_increments_ema[i].get_increment(self.perpetual.top_bid_price))
        #    perp_list.append(self.perpetual.bid_increments_ema[i].get_increment(self.perpetual.top_bid_price - self.min_price_increment))
        #    perp_list.append(self.perpetual.bid_increments_ema[i].get_increment(self.perpetual.top_bid_price + self.min_price_increment))
 
        self.snapshots[timer.tid].save_and_reset( self.symbol, bt.time, perp_list)

    def on_end_of_day(self):
        columns = [ 
                    'symbol', 
                    'time', 
                    
                    'abs_vol', 
                    'buy_vol', 
                    'sell_vol',
                    'high',
                    'low',
                    'open',
                    'close',
                    'min_imbalance',
                    'max_imbalance',
                    'min_imbalance2',
                    'max_imbalance2',
                    'min_imbalance3',
                    'max_imbalance3',

                    'midquote',
                    'spread',
                    'microprice',
                    'microprice2',
                    'top_bid_price',
                    'bid_size_0',
                    'bid_size_1',
                    'bid_size_2',
                    'bid_size_3',
                    'bid_size_4',
                    'top_ask_price',
                    'ask_size_0',
                    'ask_size_1',
                    'ask_size_2',
                    'ask_size_3',
                    'ask_size_4',
                    'bid_num_0',
                    'bid_num_1',
                    'bid_num_2',
                    'bid_num_3',
                    'bid_num_4',
                    'ask_num_0',
                    'ask_num_1',
                    'ask_num_2',
                    'ask_num_3',
                    'ask_num_4',
                    'imbalance',
                    'imbalance2',
                    'imbalance3'
                    ]
        
        for i in xrange(len(self.halflives)):
            columns.append('ask_vol_ema_' + self.halflive_names[i])
        for i in xrange(len(self.halflives)):
            columns.append('bid_vol_ema_' + self.halflive_names[i])
        for i in xrange(len(self.halflives)):
            columns.append('microprice_ema_' + self.halflive_names[i])
        for i in xrange(len(self.halflives)):
            columns.append('trade_price_ema_' + self.halflive_names[i])
        #for i in xrange(len(self.halflives)):
        #    columns.append('ask_increment_0_ema_' + self.halflive_names[i])
        #    columns.append('ask_increment_-1_ema_' + self.halflive_names[i])
        #    columns.append('ask_increment_+1_ema_' + self.halflive_names[i])
        #for i in xrange(len(self.halflives)):
        #    columns.append('bid_increment_0_ema_' + self.halflive_names[i])
        #    columns.append('bid_increment_-1_ema_' + self.halflive_names[i])
        #    columns.append('bid_increment_+1_ema_' + self.halflive_names[i])

        i = 0
        for snapshot in self.snapshots:
            df = pd.DataFrame(snapshot.saved, columns= columns)
            store = pd.HDFStore(self.staging_root + '%s_%d_%s.h5' % ( self.symbol, i, self.date.strftime('%Y%m%d') ))
            store.append('df', df, complib='zlib', complevel=9)
            #store['df'] = df
            i += 1

class snapshot_generator(pybt.pybt_handler):
    
    def __init__(self):
        pybt.pybt_handler.__init__(self)
        self.instruments    = None 
        self.date           = None
        self.intervals      = None
        self.symbol_states  = {} 
        self.halflives      = []
        self.halflive_names = []
        self.lambdas        = []
        self.staging_root   = "/local/disk1/staging/"
        self.verbose        = 0
        self.on_timer_ctr   = 0
 
    def on_start_of_day(self, bt):
        for sym in self.instruments:
            self.symbol_states[sym] = symbol_state(sym, self.date, staging_root=self.staging_root, 
                min_price_increment=bt.states[sym].book.min_price_increment)
            self.symbol_states[sym].initialize( self.halflives, self.halflive_names, self.intervals)

    def on_end_of_day(self, bt):
        for sym in self.symbol_states:
           self.symbol_states[sym].on_end_of_day()
 
    def on_market_data(self, bt, r):
        for sym in self.symbol_states:
            self.symbol_states[sym].on_market_data(bt, r)

        if r is None:
            return 

        if self.symbol_states.has_key(r.sym):
            if r.entryType == '2' and r.tradeCondition == '':
                self.symbol_states[r.sym].on_trade(bt, r)

    def on_timer(self, bt, timer):
        self.on_timer_ctr += 1
        if (self.on_timer_ctr % 1000 == 0 ) and (self.verbose == 1):
            print  datetime.now().strftime('%H:%M:%S'), bt.time
            
        for sym in self.symbol_states:
            self.symbol_states[sym].on_timer(bt, timer)

def main( dt = datetime(2015,4,24), symbols ={ 'ESM5', 'NQM5'}, sleep_max_sec = 60, verbose=0):
    delay = random.randint(0,sleep_max_sec)
    print "%s processing %s delay %d secs" % ( datetime.now().strftime('%H:%M:%S'), dt.strftime('%Y%m%d') , delay)   
    tm.sleep(random.randint(0,sleep_max_sec))

    gen = snapshot_generator()

    gen.instruments = symbols
    gen.date = dt

    start_snaps  = time(8, 0, 0)
    stop_snaps = time(16, 0, 0)
    gen.intervals = [ timedelta(milliseconds=100),  timedelta(seconds=1), timedelta(seconds=5) ]
    gen.interval_names = { '100ms', '1s', '5s' }
    gen.halflives      = [ timedelta(milliseconds=200), timedelta(seconds=2), timedelta(seconds=10) ]
    gen.halflive_names  = ['200ms','2s', '10s']
    gen.verbose = verbose
    
    bt = pybt.pybt()
    bt.handler = gen
    bt.date = bt.handler.date
    bt.verbose = verbose
    for ins in gen.instruments: 
        bt.symbols.append(ins)
    bt.start_time = datetime.combine( bt.date, time(7, 0, 0)) 
    bt.end_time   = datetime.combine( bt.date, time(16, 10, 0))
    
    for interval in gen.intervals:
        bt.add_timer( interval, datetime.combine(bt.date,start_snaps), datetime.combine(bt.date, stop_snaps))    

    bt.run()

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='snapshot generator')
    parser.add_argument('-d', '--date')
    parser.add_argument('-s', '--symbol', action='append')
    parser.add_argument('-S','--sleep', default=1)
    parser.add_argument('-v', '--verbose', default=0)
    args = parser.parse_args()
    main( dt = datetime.strptime(args.date,'%Y%m%d'), symbols=args.symbol, sleep_max_sec=args.sleep, verbose=int(args.verbose)) 
