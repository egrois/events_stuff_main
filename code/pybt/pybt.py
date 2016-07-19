from qpython import qconnection,qtype,qtemporal
from collections import namedtuple
import pandas as pd 
import numpy as np
from datetime import datetime
import utils
from copy import deepcopy
import time
import random

class pybt_handler:

    def __init__(self):
        self.count = 0

    def on_start_of_day(self, bt):
        pass

    def on_end_of_day(self, bt):
        pass

    def on_market_data(self, bt, record ):
        pass    

    def on_end_of_packet(self, bt, pkt):
        pass

    def on_timer(self, bt, timer):
        pass

class pybt_price_level:

    def __init__(self, size = 0, num_orders = 0, time = 0):
        self.size = size
        self.numOrders = num_orders
        self.time = time

class pybt_book_change_record:

    def __init__(self, sym, side, price, size, num_orders):
        self.symbol = sym
        self.side = side
        self.price = price
        self.size = int(size)
        self.num_orders = num_orders
        self.key = '%s@%s@%s' % (self.symbol,self.side,self.price)           

    def __str__(self):
        return 'key=%s sz=%d num=%d' % (self.key, self.size, self.num_orders)
    
    def get_size(self):
        return self.size


class pybt_book:
    
    def __init__(self, symbol, secdef, apply_trades = True):
        self.symbol = symbol
        self.apply_trades = apply_trades
        self.min_price_increment = float(secdef['minPriceIncrement'])
        
        ref = float(secdef['refPrice'])
        #ref = 200000.0       
        
        print 'ref_price', ref
        print 'min_price_increment', self.min_price_increment

        levels_away = int(( ref * 0.25 ) / self.min_price_increment)
        lo = ref - levels_away * self.min_price_increment       
        hi = ref + levels_away * self.min_price_increment       
        self.reference_price = lo 
        self.reserve_levels = int((hi - lo) / self.min_price_increment) + 1 
        
        self.market_depth = int( secdef['mktDepthGBX'])
        
        self.top_bid = None
        self.top_ask = None
        self.bid = []
        self.ask = []
        for i in xrange( self.reserve_levels ) :
            self.bid.append( pybt_price_level() )
            self.ask.append( pybt_price_level() )

        self.clear_changes()
        self.is_implied = False
   
    def get_change_info(self):
        return self.change_info
 
    def clear_changes(self):
        self.change_info = None

    def register_change(self, side, price, size, num_orders):
        assert(np.abs(size) <= 1000000) 
        self.change_info = pybt_book_change_record(self.symbol, side, price, size, num_orders)

    def idx(self, price):
        i = int(np.round((price - self.reference_price) / self.min_price_increment))
        assert(i >= 0), 'price index below 0'
        assert(i < self.reserve_levels), 'price index above limit %s %s %s' % (price, self.reference_price, self.min_price_increment)
        return i             

    def change_info_to_str(self):
        if self.change_info is None:
            return ''
        else:
            return str(self.change_info)

    def has_top(self):
        if (self.top_bid is None) or (self.top_ask is None):
            return False
        else:
            return True

    def top_price(self, side):
        if side == 'BID':
            return self.top_bid
        else:
            return self.top_ask

    def size_at_price(self, side,  price):
        try:
            i = self.idx(price)
        except:
            return 0
        if side == 'BID':
            return int(self.bid[i].size)
        else:
            return int(self.ask[i].size)

    def num_orders_at_price(self, side, price):
        try:
            i = self.idx(price)
        except:
            return 0
        if side == 'BID':
            return int(self.bid[i].numOrders)
        else:
            return int(self.ask[i].numOrders)

    def time_at_price(self, side, price):
        try:
            i = self.idx(price)
        except:
            return 0
        if side == 'BID':
            return self.bid[i].time
        else:
            return self.ask[i].time

    def top_to_str(self):
        if not self.has_top():
            return '' 
        
        b = self.top_price('BID') 
        a = self.top_price('ASK') 

        b_size = self.size_at_price('BID', b)
        a_size = self.size_at_price('ASK', a)

        b_num = self.num_orders_at_price('BID', b)
        a_num = self.num_orders_at_price('ASK', a)

        b_time = pd.to_datetime(self.time_at_price('BID', b)).strftime('%S.%f')[:-3]
        a_time = pd.to_datetime(self.time_at_price('ASK', a)).strftime('%S.%f')[:-3]

        change_str = self.change_info_to_str()

        s = 't(b(%s %6d %3d %3d) a(%s %6d %3d %3d) s=%3d %10s)' % (b_time, b, b_size, b_num, a_time, a, a_size, a_num, a - b, change_str )        
        
        return s

    def show(self, num_levels = 5):
        if not self.has_top():
            return 

        print self.top_to_str()        
       
        side = 'BID'
        for i in xrange(num_levels):
            price = self.top_price(side) - i * self.min_price_increment
            print "%2s %8d %4d %4d %s" % (
                'BID',
                price, 
                self.size_at_price(side, price),
                self.num_orders_at_price(side,price),
                self.time_at_price(side,price)
                )
        side = 'ASK'
        for i in xrange(num_levels):
            price = self.top_price(side) + i * self.min_price_increment
            print "%2s %8d %4d %4d %s" % (
                'ASK', 
                price, 
                self.size_at_price(side, price),
                self.num_orders_at_price(side,price),
                self.time_at_price(side,price)
                )

    def on_market_data(self, bt, rec):

        if self.is_implied and rec.quoteCondition != 'K':
            return
        
        if ( not self.is_implied ) and (rec.quoteCondition != ''):
            return

        try:
            i = self.idx(rec.price)
        except:
            return

        self.clear_changes()       
        
        if rec.entryType == '0': #BID
            if rec.updateAction == '0': #NEW
                if rec.priceLevel == 1:
                    self.register_change('BID',rec.price,rec.size,rec.numOrders)
                self.bid[i] = pybt_price_level( rec.size, rec.numOrders, rec.time) 
                if rec.priceLevel == 1:
                    self.top_bid = rec.price
                    if utils.test_more_or_equal(self.top_bid, self.top_ask, self.min_price_increment):
                        self.top_ask = self.top_bid + self.min_price_increment
            elif rec.updateAction == '1': #CHANGE
                self.register_change('BID',rec.price,rec.size - self.bid[i].size,rec.numOrders - self.bid[i].numOrders)
                self.bid[i] = pybt_price_level( rec.size, rec.numOrders, rec.time) 
                if rec.priceLevel == 1:
                    self.top_bid = rec.price
            elif rec.updateAction == '2': #DELETE
                self.register_change('BID',rec.price,-self.bid[i].size,-self.bid[i].numOrders  )
                if rec.priceLevel == 1:
                    self.bid[i] = pybt_price_level( 0, 0, rec.time) 
                    if utils.test_more_or_equal( self.top_bid, rec.price, self.min_price_increment):
                        self.top_bid = rec.price - self.min_price_increment 
        elif rec.entryType == '1': #ASK
            if rec.updateAction == '0': #NEW
                if rec.priceLevel == 1:
                    self.register_change('ASK',rec.price,rec.size,rec.numOrders)
                self.ask[i] = pybt_price_level( rec.size, rec.numOrders, rec.time) 
                if rec.priceLevel == 1:
                    self.top_ask = rec.price
                    if utils.test_less_or_equal( self.top_ask, self.top_bid, self.min_price_increment):
                        self.top_bid = self.top_ask - self.min_price_increment
            elif rec.updateAction == '1': #CHANGE
                self.register_change('ASK',rec.price,rec.size - self.ask[i].size,rec.numOrders - self.ask[i].numOrders)
                self.ask[i] = pybt_price_level( rec.size, rec.numOrders, rec.time) 
                if rec.priceLevel == 1:
                    self.top_ask = rec.price
            elif rec.updateAction == '2': #DELETE
                self.register_change('ASK',rec.price,-self.ask[i].size,-self.ask[i].numOrders  )
                if rec.priceLevel == 1:
                    self.ask[i] = pybt_price_level( 0, 0, rec.time) 
                    if utils.test_less_or_equal(self.top_ask, rec.price, self.min_price_increment):
                        self.top_ask = rec.price + self.min_price_increment  
        
        elif rec.entryType == '2': #TRADE
            if self.apply_trades == True:
                if utils.test_equal(rec.price, self.top_ask, self.min_price_increment): #BUY
                    self.register_change('ASK',rec.price,-rec.size,-1)
                    self.ask[i].size -= rec.size
                    self.ask[i].numOrders -= 1
                    if self.ask[i].size <= 0:
                        self.top_ask = self.top_ask + self.min_price_increment
                    elif self.ask[i].numOrders <= 0:
                        self.ask[i].numOrders = 1
                elif utils.test_equal(rec.price,self.top_bid,self.min_price_increment): #SELL 
                    self.register_change('BID',rec.price,-rec.size,-1)
                    self.bid[i].size -= rec.size
                    self.bid[i].numOrders -= 1
                    if self.bid[i].size <= 0:
                        self.top_bid = self.top_bid - self.min_price_increment
                    elif self.bid[i].numOrders <= 0:
                        self.bid[i].numOrders = 1
        #print bt.time, self.top_to_str()
    
    def midquote(self):
        if not self.has_top():
            return np.nan
        bid_price = self.top_price('BID')
        ask_price = self.top_price('ASK')
        return float(bid_price + ask_price) * 0.5 
        
    def spread(self):
        if not self.has_top():
            return np.nan
        bid_price = self.top_price('BID')
        ask_price = self.top_price('ASK')
        return (ask_price - bid_price)

    def microprice(self):
        if not self.has_top():
            return np.nan
        bid_price = self.top_price('BID')
        bid_size = self.size_at_price('BID',bid_price)
        ask_price = self.top_price('ASK')
        ask_size = self.size_at_price('ASK',ask_price)
        try:
            mp = float(bid_price*ask_size + ask_price*bid_size) / float(bid_size + ask_size )
            return mp
        except:
            return np.nan 

    def microprice2(self):
        if not self.has_top():
            return np.nan
        bid_price = self.top_price('BID')
        bid_orders = self.num_orders_at_price('BID',bid_price)
        ask_price = self.top_price('ASK')
        ask_orders = self.num_orders_at_price('ASK',ask_price)
        try:
            mp = float(bid_price*ask_orders + ask_price*bid_orders) / float(bid_orders + ask_orders)
            return mp
        except:
            return np.nan

    def imbalance(self):
        if not self.has_top():
            return np.nan
        bid_price = self.top_price('BID')
        ask_price = self.top_price('ASK')
        bid_size = 0
        ask_size = 0
        for i in xrange(5): 
            bid_size += self.size_at_price('BID', bid_price - i * self.min_price_increment)
            ask_size += self.size_at_price('ASK', ask_price + i * self.min_price_increment)
        try:
            mp = float(-ask_size + bid_size) / float(bid_size + ask_size )
            return mp
        except:
            return np.nan 
    
    def imbalance2(self):
        if not self.has_top():
            return np.nan
        bid_price = self.top_price('BID')
        ask_price = self.top_price('ASK')
        bid_size = 0
        ask_size = 0
        for i in xrange(5): 
            bid_size += self.num_orders_at_price('BID', bid_price - i * self.min_price_increment)
            ask_size += self.num_orders_at_price('ASK', ask_price + i * self.min_price_increment)
        try:
            mp = float(-ask_size + bid_size) / float(bid_size + ask_size )
            return mp
        except:
            return np.nan 
    
    def imbalance3(self):
        if not self.has_top():
            return np.nan
        bid_price = self.top_price('BID')
        ask_price = self.top_price('ASK')
        bid_size = 0
        ask_size = 0
        for i in xrange(5): 
            bid_size += ( self.num_orders_at_price('BID', bid_price - i * self.min_price_increment) 
                        * self.size_at_price('BID', bid_price - i * self.min_price_increment))
            ask_size += ( self.num_orders_at_price('ASK', ask_price + i * self.min_price_increment) 
                        * self.size_at_price('ASK', ask_price + i * self.min_price_increment))
        try:
            mp = float(-ask_size + bid_size) / float(bid_size + ask_size )
            return mp
        except:
            return np.nan 

class pybt_state:
    
    def __init__(self):
        self.secdef = {}
        self.book = None
        self.implied_book = None
        self.has_implied_book = False

class pybt_timer:
    
    def __init__(self):
        self.tid            = None        
        self.period         = None 
        self.start          = None
        self.stop           = None
        self.times          = []

    def __str__(self):
        return 'id=%s period=%s start=%s stop=%s' % (self.tid, self.period, self.start, self.stop)

    def populate_times(self, start_run, stop_run):
        self.times = []
        cur = self.start
        while cur <= self.stop:
            if cur >= start_run and cur <= stop_run:
                self.times.append(cur)
            cur += self.period 

    def pop_time(self):
       return  self.__times.pop(0)

class pybt_packet:
    
    def __init__(self):
        self.records = []
        self.book_changes = {} 

    def apply_changes(self, change_info):
        if change_info is None:
            return 
        elif self.book_changes.has_key(change_info.key):
            self.book_changes[change_info.key].size += change_info.size
            self.book_changes[change_info.key].num_orders += change_info.num_orders
        else:
            self.book_changes[change_info.key] = deepcopy(change_info)

    def print_book_changes(self):
        for key in self.book_changes.keys():
            print self.book_changes[key]
 
class pybt:

    def __init__(self):


        self.sessions = []

        self.sessions.append({ 
            'host':'10.200.101.92',
            'port':9057,
            'username':'res1ext', 
            'password':'kx45es9057'})
        
        self.sessions.append({ 
            'host':'10.200.101.92',
            'port':9157,
            'username':'res1ext', 
            'password':'kx45es9057'})

        self.sessions.append({ 
            'host':'10.200.101.93',
            'port':9057,
            'username':'res1ext', 
            'password':'kx45es9057'})

        self.sessions.append({ 
            'host':'10.200.101.93',
            'port':9157,
            'username':'res1ext', 
            'password':'kx45es9057'})

        # Parameters configurable by a user
        self.handler            = None
        self.symbols            = []
        self.start_time         = None 
        self.end_time           = None 
        self.date               = None 

        self.kdb_host           = '10.200.101.92'
        self.kdb_port           = 9157
        self.kdb_username       = 'res1ext'
        self.kdb_password       = 'kx45es9057' 

        self.kdb_ticks_table    = 'cmeRaw'
        self.kdb_secdef_table   = 'cmeMaster'
        self.apply_trades       = True 

        # These variables get changed during the run
        self.time = None 
        self.states = {}
        self.ticks = None
        self.packet = pybt_packet()       
 
        self.__tid = 0
        self.timers = {}
        self.timer_queue = ()

        self.verbose = 0

    def get_connection(self):
        i = random.randint(0,len(self.sessions) - 1)
        q = qconnection.QConnection(    host = self.sessions[i]['host'],
                                        port = self.sessions[i]['port'],
                                        username = self.sessions[i]['username'],
                                        password = self.sessions[i]['password'])
        return q

    def read_secdefs(self, q):
        success = False
        while success == False:
            try:
                q = self.get_connection()
                q.open()
                success = True
            except:
                print 'connection refused'
                print q
                success = False
                time.sleep(1)
        
        for symbol in self.symbols:
            sym = '`$("%s")' % symbol
            self.states[symbol] = pybt_state()

            for tbl in utils.MASTER_KDB_TABLES:
                qstr =  '{[a] select from %s where date=%s, sym in ((),a) } [%s]' % ( tbl, self.date.strftime('%Y.%m.%d'), sym )
                r = q(qstr)
                if len(r) == 0:
                    continue
                for i in xrange(len(r.dtype.names)):
                    self.states[symbol].secdef[r.dtype.names[i]] = r[-1][i]
                if int(self.states[symbol].secdef['impliedMarketIndicator']) == 3:
                    self.states[symbol].has_implied_book = True
                else:
                    self.states[symbol].has_implied_book =  False
                self.states[symbol].book = pybt_book(symbol,self.states[symbol].secdef,apply_trades=self.apply_trades)
                if self.states[symbol].has_implied_book:
                    self.states[symbol].implied_book = pybt_book(symbol,self.states[symbol].secdef,apply_trades=False)
                    self.states[symbol].implied_book.is_implied = True 
                break
        q.close()

    def read_ticks(self, q):
        start_time_dt64 = utils.dt_local_to_dt64(self.start_time)
        end_time_dt64 = utils.dt_local_to_dt64(self.end_time)

        start_date_utc = utils.dt64_to_date_utc_str(start_time_dt64, fmt='%Y.%m.%d') 
        end_date_utc = utils.dt64_to_date_utc_str(end_time_dt64, fmt='%Y.%m.%d') 
        assert(start_date_utc == end_date_utc), 'spanning different UTC dates not supported %s %s' % ( start_date_utc, end_date_utc )
        assert(start_date_utc == self.date.strftime('%Y.%m.%d')), 'date not set properly %s %s' % ( self.date.strftime('%Y.%m.%d'), end_date_utc )
        start_time_utc = utils.dt64_to_dt64_utc_str(start_time_dt64, fmt='%H:%M:%S')
        end_time_utc = utils.dt64_to_dt64_utc_str(end_time_dt64, fmt='%H:%M:%S')

        success = False
        while success == False:
            try:
                q = self.get_connection()
                q.open()
                success = True
            except:
                print 'connection refused', 
                print q                 

                success = False
                time.sleep(1)
       
        tables = utils.get_raw_tables_for_symbols(self.symbols)
        assert(len(tables) == 1 ), 'cannot operate on symbols from multiple tables %s ' % str(tables)
        tbl = None
        for t in tables:
            tbl = t
 
        symbols = '`$("' + '";"'.join(self.symbols) + '")'
        columns = ' '

        ##self.ticks = self.get_ticks(columns, tbl, symbols, dt, start_utc, end_utc)

        qstr = '{[a;b;c;d] '\
	        '`time xasc select %s from %s '\
            'where date=d, sym in ((),a), time within (b;c), entryType in ("0","1","2")}'\
	        '[%s;%sT%s;%sT%s;%s]' % (
    	    columns,
            tbl,
	        symbols, 
	        self.date.strftime('%Y.%m.%d'),
	        start_time_utc,
	        self.date.strftime('%Y.%m.%d'),
	        end_time_utc,
	        self.date.strftime('%Y.%m.%d') )
        
        self.ticks = q(qstr)
        q.close()

    #def get_ticks():
    #    pass 

    def process_ticks(self):
        pybt_record = namedtuple('pybt_record', self.ticks.dtype.names, rename=True)
        for i in range(len(self.ticks)):
            eop = False
            record = self.ticks[i]
            r = pybt_record(*record)
            r = self.translate_tick_record(r)
            
            if i  < len(self.ticks) - 1:  
                record2 = self.ticks[i+1]
                r2 = pybt_record(*record2)
                r2 = self.translate_tick_record(r2)
                if r2.time > r.time:
                    eop = True
            else:
                eop = True 

            self.process_timers(r.time)
            self.states[r.sym].book.on_market_data(self, r)
            if self.states[r.sym].has_implied_book:
                self.states[r.sym].implied_book.on_market_data(self,r) 
            self.time = r.time
            self.handler.on_market_data(self, r)
            
            self.packet.records.append(r)          
            self.packet.apply_changes(self.states[r.sym].book.change_info) 
            if eop == True:
                self.handler.on_end_of_packet(self, self.packet)
                self.packet = pybt_packet()

    def translate_tick_record(self, r):
        
        if r.entryType == '2': #TRADE
            r = r._replace(
                date = qtemporal.from_raw_qtemporal(r.date, qtype.QDATE).astype(datetime),
                tradeDate = qtemporal.from_raw_qtemporal(r.tradeDate, qtype.QDATE).astype(datetime),
                time = utils.dt64_to_dt_local(qtemporal.from_raw_qtemporal(r.time, qtype.QTIMESTAMP)),
                settleDate = qtemporal.from_raw_qtemporal(r.settleDate, qtype.QDATE),
                entryTime = utils.dt64_to_dt_local(qtemporal.from_raw_qtemporal(r.entryTime, qtype.QTIMESTAMP)),
                priceLevel = 0,
                numOrders = 0,
                tradingSessionID = 0,
                openCloseSettleFlag = 0,
                quoteType = 0,
                price = float(r.price)
            )
        elif r.entryType == '0' or r.entryType == '1': #QUOTE
            r = r._replace(
                date = qtemporal.from_raw_qtemporal(r.date, qtype.QDATE).astype(datetime),
                tradeDate = qtemporal.from_raw_qtemporal(r.tradeDate, qtype.QDATE).astype(datetime),
                time = utils.dt64_to_dt_local(qtemporal.from_raw_qtemporal(r.time, qtype.QTIMESTAMP)),
                settleDate = qtemporal.from_raw_qtemporal(r.settleDate, qtype.QDATE),
                entryTime = utils.dt64_to_dt_local(qtemporal.from_raw_qtemporal(r.entryTime, qtype.QTIMESTAMP)),
                tickDirection=0,
                openCloseSettleFlag=0,
                tradeVolume=0,
                quoteType=0,
                aggressorSide=0,
                tradeID=0,
                price = float(r.price),
                size = int(r.size),
                numOrders = int(r.numOrders)
            )
        
        return r

    def record_to_str(self, rec):
        UA = { '0':'N', '1':'C', '2':'D' }
        ET = { '0':'B', '1':'A', '2':'T' }
        rtime= pd.to_datetime(rec.time).strftime('%S.%f')[:-3]
        s = 'rec=(%s %4s %1s %1s %3s %6d %3d %3d)' % (rtime,  rec.sym, UA[rec.updateAction], ET[rec.entryType], rec.priceLevel, rec.price, rec.size, rec.numOrders  )
        return s

    def run(self):
        q = qconnection.QConnection( host = self.kdb_host,port = self.kdb_port,username = self.kdb_username,
            password = self.kdb_password)
        self.prepare_timers() 
        self.read_secdefs(q)
        self.handler.on_start_of_day(self)        
        self.read_ticks(q)
        self.process_ticks()        
        self.handler.on_end_of_day(self)

    def add_timer(self,period,start,stop):
        t = pybt_timer()
        t.tid = self.__tid
        self.__tid += 1
        t.period = period
        t.start = start
        t.stop = stop
        self.timers[t.tid] = t
        return t.tid

    def remove_timer(self, timer_id):
        del self.timers[timer_id]

    def prepare_timers(self):
        for id in self.timers.keys():
            self.timers[id].populate_times(self.start_time, self.end_time)
        self.refresh_timers_queue()

    def refresh_timers_queue(self):
        self.timers_queue = deepcopy([])
        for id in self.timers.keys():
            if len(self.timers[id].times) > 0:
                self.timers_queue.append( (self.timers[id].times[0], id) )
        self.timers_queue = sorted(self.timers_queue)

    def process_timers(self, t):
        while len(self.timers_queue) > 0 and self.timers_queue[0][0] < t:
            id = self.timers_queue[0][1]            
            time = self.timers[id].times.pop(0)
            self.time = deepcopy(time)
            self.handler.on_timer( self, self.timers[id] )
            self.refresh_timers_queue()
