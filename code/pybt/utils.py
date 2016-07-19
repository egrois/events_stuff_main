import os
from dateutil import tz

import numpy as np
from datetime import date, datetime, time, timedelta
from qpython import qconnection,qtype, qtemporal
import matplotlib.pyplot as plt
import glob
import pandas as pd

import pybrain
from pybrain.tools.shortcuts import buildNetwork
from pybrain.datasets import SupervisedDataSet
from pybrain.structure import TanhLayer
from pybrain.structure import SigmoidLayer
from pybrain.supervised.trainers import BackpropTrainer

from sklearn.ensemble import GradientBoostingRegressor

RAW_KDB_TABLES=['cmeRaw', 'cbtRaw', 'cecRaw', 'nymRaw' ]
MASTER_KDB_TABLES = ['cmeMaster', 'cbtMaster', 'cecMaster', 'nymMaster']
ROOT_TO_KDB_TABLE = {
    'ES' : 'cmeRaw',
    'NQ' : 'cmeRaw',
    '6A' : 'cmeRaw',
    '6B' : 'cmeRaw',
    '6C' : 'cmeRaw',
    '6E' : 'cmeRaw',
    '6J' : 'cmeRaw',
    '6M' : 'cmeRaw',
    '6S' : 'cmeRaw',
    'GE' : 'cmeRaw',
    'CL' : 'nymRaw',
    'NG' : 'nymRaw',
    'ZN' : 'cbtRaw',
    'ZB' : 'cbtRaw',
    'ZF' : 'cbtRaw',
    'ZT' : 'cbtRaw'
}

def get_raw_tables_for_symbols(syms):
    tab = set([])
    for sym in syms:
        if ROOT_TO_KDB_TABLE.has_key(sym[:2]):
            tab.add(ROOT_TO_KDB_TABLE[sym[:2]])
    return tab

def dt64_to_dt64_utc_str(dt64, fmt='%Y-%m-%dT%H:%M:%S.%f'):
    return dt64.astype(datetime).strftime(fmt) 

def dt64_to_date_utc_str(dt64, fmt='%Y-%m-%d'):
    return dt64.astype(datetime).strftime(fmt)

def dt64_to_dt64_usec(dt64):
    toks = str(dt64).split('T')
    toks2 = toks[1].split('.')
    toks3 = toks2[1].split('-')
    toks3[0] = toks3[0][:6] if len(toks3[0]) > 6 else toks3[0]
    return np.datetime64('%sT%s.%s-%s' % (toks[0],toks2[0], toks3[0], toks3[1])) 

def dt_to_epoch(dt):
    epoch = datetime.utcfromtimestamp(0)
    delta = dt - epoch
    return int(delta.total_seconds())

def dt64_to_dt_local(dt64):
    dt = dt64_to_dt64_usec(dt64).astype(datetime)
    dt2 = datetime.fromtimestamp(dt_to_epoch(dt))
    dt = dt2 + timedelta(microseconds=dt.microsecond)
    return dt

def dt_local_to_dt64(dt, fmt='%Y-%m-%dT%H:%M:%S'):
    return np.datetime64(dt.strftime(fmt))

def sym_vols(dt,host='10.200.101.92', port=9057, username='res1ext', password='kx45es9057'):
    q = qconnection.QConnection(host, port, username, password)
    aa = []
    q.open()
    for tbl in RAW_KDB_TABLES:
        qstr = 'select sum(size) by sym from %s where date=%s, entryType="2"' % (tbl, dt.strftime('%Y.%m.%d'))
        res = q(qstr)
        for item in res.iteritems():
            aa.append( ( item[0][0], item[1][0], tbl ) )
    q.close()
    return sorted(aa)

def sec_def(dt,symbol,host='10.200.101.92', port=9057, username='res1ext', password='kx45es9057'):
    q = qconnection.QConnection(host, port, username, password)
    q.open()
    sym = '`$("%s")' % symbol
    secdef = {}
    for tbl in MASTER_KDB_TABLES:
        qstr =  '{[a] select from %s where date=%s, sym in ((),a) } [%s]' % (tbl, dt.strftime('%Y.%m.%d'), sym )
        res = q(qstr)
        if len(res) == 0:
            continue
        for i in xrange(len(res.dtype.names)):
            secdef[res.dtype.names[i]] = res[-1][i]
    q.close()
    return secdef 

def show_frame(df, ncols=3, width=20, height=20):
    cols= df.columns
    f, ax = plt.subplots(len(cols)/ncols, ncols, figsize=(width,height))
    i=0
    for col in cols:
        try:
            ax[i/ncols][i%ncols].plot(df.time, df[col])
            ax[i/ncols][i%ncols].set_title(col)
        except:
            pass
        i += 1

def show_frame2(df, ncols=3, width=20, height=20):
    cols= df.columns
    f, ax = plt.subplots(len(cols)/ncols, ncols, figsize=(width,height))
    i=0
    for col in cols:
        try:
            ax[i/ncols][i%ncols].plot(df[col])
            ax[i/ncols][i%ncols].set_title(col)
        except:
            pass
        i += 1


DEFAULT_ROOTS   = { 'ES':10000,'NQ':10000, 'GE':10000, 
                    'ZN':10000,'ZB':10000,'ZF':10000,'ZT':10000,'6E':10000,'6J':10000,
                    '6B':10000,'6S':10000,'6A':10000,'6C':10000,'6M':10000,
                    'CL':10000,'NG':10000}

def get_liquid_symbols(dt,roots = DEFAULT_ROOTS): 
    slist = []
    all_syms = sym_vols(dt)
    for sym in all_syms:
        if len(sym[0]) != 4: # TODO
            continue
        for root in roots.keys():
            if sym[0][:len(root)].count(root):
                if sym[1] > roots[root]:
                    slist.append(sym[0])
    return slist

def get_liquid_symbols_verify(dt, roots = DEFAULT_ROOTS, root_dir = None):
    slist = []
    all_syms = sym_vols(dt)
    for sym in all_syms:
        if len(sym[0]) != 4: #TODO
            continue
        for root in roots.keys():
            if sym[0][:len(root)].count(root):
                if sym[1] > roots[root]:
                    if len(glob.glob('%s%s/%s_0_%s.h5' % (root_dir, root, sym[0], dt.strftime('%Y%m%d')))) == 0: 
                        slist.append(sym[0])
    return slist

def verify_snapshots(start, end, root_dir='/local/disk1/data/snapshots/0/'):
    days = get_workdays(start, end)
    for dt in days:
        syms = get_liquid_symbols_verify(dt, root_dir=root_dir)
        if len(syms) > 0:
            print dt, syms

def verify_kdb_raw(start, stop, thres_num_syms = 20):
    days = get_workdays(start,stop)
    for dt in days:
        aa = get_liquid_symbols(dt)
        if len(aa) < thres_num_syms:
            print dt, aa

def get_liquid_symbols_string(d, roots =[], split =1, verify=False, root_dir=None):
    syms = []
    if len(roots) == 0:
        if verify == False:
            syms = get_liquid_symbols(d)
        else:
            syms = get_liquid_symbols_verify(d, root_dir=root_dir)
    else:
        if verify == False:
            syms = get_liquid_symbols(d, roots)
        else:
            syms = get_liquid_symbols_verify(d, roots, root_dir=root_dir)

    ret = []

    for s in syms:
        ret.append(" -s " + s)

    #s = split_symbol_list_by_source(syms)
 
    #for li in s:
    #    if ROOT_TO_KDB_TABLE[li[0][:2]] == 'cmeRaw':
    #        s = split_list(li, split)
    #        for l in s:
    #            ret.append( ' -s '+ ' -s '.join(l) )
    #    else:
    #        ret.append( ' -s '+ ' -s '.join(li) )
    
    return ret


def split_symbol_list_by_source(syms):
    result = {}
    for sym in syms:
        key = ROOT_TO_KDB_TABLE[sym[:2]]
        if result.has_key(key):
            result[key].append(sym) 
        else:
            result[key] = []
            result[key].append(sym) 
    res = []
    for ll in result.keys():
        res.append(result[ll])
    return res


HOLIDAYS = set([
            datetime(2011,1,17),
            datetime(2011,2,21),
            datetime(2011,4,22),
            datetime(2011,5,30),
            datetime(2011,7,4),
            datetime(2011,9,5),
            datetime(2011,11,24),
            datetime(2011,11,25), #half
            datetime(2011,12,26),

            datetime(2012,1,2),
            datetime(2012,1,16),
            datetime(2012,2,20),
            datetime(2012,4,6),
            datetime(2012,5,28),
            datetime(2012,7,3), # half
            datetime(2012,7,4),
            datetime(2012,9,3),
            datetime(2012,11,22),
            datetime(2012,12,24), # half
            datetime(2012,12,25),

            datetime(2013,1,1),
            datetime(2013,1,21),
            datetime(2013,2,18),
            datetime(2013,3,29),
            datetime(2013,5,27),
            datetime(2013,7,3), # half
            datetime(2013,7,4),
            datetime(2013,9,2),
            datetime(2013,11,28),
            datetime(2013,11,29), #half
            datetime(2013,12,24), #half
            datetime(2013,12,25),

            datetime(2014,1,1),
            datetime(2014,1,20),
            datetime(2014,2,17),
            datetime(2014,4,18),
            datetime(2014,5,26),
            datetime(2014,7,3), #half
            datetime(2014,7,4),
            datetime(2014,9,01),
            datetime(2014,11,27),
            datetime(2014,11,28), #half
            datetime(2014,12,24), #half
            datetime(2014,12,25),

            datetime(2015,1,1),
            datetime(2015,1,19),
            datetime(2015,2,16),
            datetime(2015,4,3),
            datetime(2015,5,25),
            datetime(2015,7,3), #half
            datetime(2015,7,4),
            datetime(2015,9,7),
            datetime(2015,11,26),
            datetime(2015,11,27), #half
            datetime(2015,12,24), #half
            datetime(2015,12,25),

            datetime(2016,1,1),
            datetime(2016,1,18),
            datetime(2016,2,15),
            datetime(2016,3,25),
            datetime(2016,5,30),
            datetime(2016,7,3), #half
            datetime(2016,7,4),
            datetime(2016,9,5),
            datetime(2016,11,24),
            datetime(2016,11,25), #half
            datetime(2016,12,26),
            ])

def get_workdays(start, end):
    workdays = []
    dt = start
    d = timedelta(days=1)
    while dt <= end:
        if dt.weekday() < 5 and ( dt not in HOLIDAYS ):
            workdays.append(dt.date())
        dt += d
    return workdays

def get_workdays2(start, end):
    workdays = []
    dt = start
    d = timedelta(days=1)
    while dt <= end:
        if dt.weekday() < 5 and ( dt not in HOLIDAYS ):
            workdays.append(dt)
        dt += d
    return workdays

def print_generator_command_strings( start, end, command='python snapshot_generator.py',roots=[], split=1, verify=False, root_dir='/local/disk1/data/snapshots/0/'):
    dates = get_workdays(start,end)
    for dt in dates:
        s = get_liquid_symbols_string(dt, roots, split, verify=verify, root_dir = root_dir)
        for ss in s:
            if len(ss) > 4:
                print command + ' -d %s ' % dt.strftime('%Y%m%d') + ss 

def split_list(li, num_parts=1):
    import math
    zz = int(math.ceil(float(len(li)) / float(num_parts)))
    b=[]
    for i in xrange(num_parts):
        if i*zz > len(li) - 1:
            continue
        if i == num_parts - 1:
            b.append(li[i*zz : ])
        else:
            b.append(li[i*zz : (i+1)*zz ])
    return b

def pplot0( df, 
            cols=['microprice_ema_10s','microprice_ema_2s','microprice_ema_200ms'],
            figsize=(10,10),
            moment=time(10,0,0),
            span = 100,
            usebar=False,
            ax=plt,
            lab=True):
    
    dt = dt64_to_dt_local(df.head(1).time.values[0]).date()
    dt = datetime.combine(dt, moment)
    start = dt - timedelta(seconds=span)
    stop = dt + timedelta(seconds=span)
    tt = df[(df.time>=start)&(df.time<=stop)]
    plt.figure(figsize=figsize)
   
    colors = ['b','r','y']
 
    if usebar == False:
        for col in cols:
            if lab == True:
                ax.plot(tt.time, tt[col],'.-', linewidth=2,label=col)
            else:
                ax.plot(tt.time, tt[col],'.-', linewidth=2)
    else:
        i = 0
        for col in cols:
            ax.bar(xrange(len(tt)), tt[col],color= colors[i%3], label=col)
            i = i + 1 
    if lab == True:
        ax.legend()

def load_day(dt, sym, res='0', root=None, snap_root='/local/disk1/data/snapshots/'):
    if root is None:
        root = sym[:2]
    store = pd.HDFStore('%s%s/%s/%s_%s_%s.h5' %(snap_root,res, root, sym, res, dt.strftime('%Y%m%d')))
    df = store['df']
    store.close()
    return df


def pplot1(dt=None, sym=None, moment=None, span=1, figw=20, figh=10, lab=True):
    fig,ax = plt.subplots(2,1)
    fig.set_figwidth(figw)
    fig.set_figheight(figh)
    df = load_day(dt, sym)
    pplot0(df,moment=moment,span=span, ax=ax[0], lab=lab)
    pplot0(load_day(dt, sym),moment=moment,span=span, 
             cols=['ask_vol_ema_10s','bid_vol_ema_10s'], ax=ax[1], lab=lab)

def pplot2( df,
             cols=['microprice_ema_10s','microprice_ema_2s','microprice_ema_200ms'],
             figsize=(10,10),
             moment=time(10,0,0),
             span = 100,
             ax=plt,
             lab=True,
             show_ticks = False,
             tick_disp = '.-'):

    dt =dt64_to_dt_local(df.head(1).time.values[0]).date()
    dt = datetime.combine(dt, moment)
    start = dt - timedelta(seconds=span)
    stop = dt + timedelta(seconds=span)
    tt = df[(df.time>=start)&(df.time<=stop)]
    plt.figure(figsize=figsize)

    if show_ticks == True:
        for col in ['bid_price','ask_price']:
            ax.plot(tt.time, tt[col],'k-', linewidth=1,label=col, alpha=0.5)
    
    for col in cols:
        ax.plot(tt.time, tt[col],tick_disp, linewidth=2,label=col)
    
    if lab == True:
        ax.legend()
        
    plt.grid(True)

FUTURE_CODES = {
        'F':1,
        'G':2,
        'H':3,
        'J':4,
        'K':5,
        'M':6,
        'N':7,
        'Q':8,
        'U':9,
        'V':10,
        'X':11,
        'Z':12 }

FUTURE_CODES2 = {
        1:'F',
        2:'G',
        3:'H',
        4:'J',
        5:'K',
        6:'M',
        7:'N',
        8:'Q',
        9:'U',
        10:'V',
        11:'X',
        12:'Z' }

def order_futures(dt, symbols):
    temp=[]
    for sym in symbols:
        year = int(sym[-1])
        if (int(dt.year%10) > int(sym[-1]) )  or ( (int(dt.year%10) == int(sym[-1])) and (dt.month > FUTURE_CODES[sym[-2]]) ):
            year = 10 + int(sym[-1])
        h = year * 100 + FUTURE_CODES[sym[-2]]
        temp.append((sym, h))
    temp.sort(key=lambda x: x[1])
    syms = []
    for entry in temp:
        syms.append(entry[0])
    return syms


def test_equal(price1, price2, min_price_increment):
    assert( np.abs(min_price_increment) > 1e-10 ), 'min_price_increment is too small'
    try:
        if np.isnan(price1) or np.isnan(price2): 
            return False
    except:
        return False 
    return np.round(price1/min_price_increment) == np.round(price2/min_price_increment)   

def test_less(price1, price2, min_price_increment):
    assert( np.abs(min_price_increment) > 1e-10 ), 'min_price_increment is too small'
    try:
        if np.isnan(price1) or np.isnan(price2): 
            return False
    except:
        return False 
    return np.round(price1/min_price_increment) < np.round(price2/min_price_increment)   

def test_more(price1, price2, min_price_increment):
    assert( np.abs(min_price_increment) > 1e-10 ), 'min_price_increment is too small'
    try:
        if np.isnan(price1) or np.isnan(price2): 
            return False
    except:
        return False 
    return np.round(price1/min_price_increment) > np.round(price2/min_price_increment)   

def test_less_or_equal(price1, price2, min_price_increment):
    assert( np.abs(min_price_increment) > 1e-10 ), 'min_price_increment is too small'
    try:
        if np.isnan(price1) or np.isnan(price2): 
            return False
    except:
        return False 
    return np.round(price1/min_price_increment) <= np.round(price2/min_price_increment)   

def test_more_or_equal(price1, price2, min_price_increment):
    assert( np.abs(min_price_increment) > 1e-10 ), 'min_price_increment is too small'
    try:
        if np.isnan(price1) or np.isnan(price2): 
            return False
    except:
        return False 
    return np.round(price1/min_price_increment) >= np.round(price2/min_price_increment)   

def group_scatter(x,y,nbins=20, ax = plt):
    try:
        x = x.values
        y = y.values
    except:
        pass
    df = pd.DataFrame(zip(x,y), columns=['x','y']).sort(columns=['x'])
    size = len(df) / ( nbins -  1)
    df['inx'] = range(len(df))
    df['inx'] = df['inx'].apply(lambda x: int(x / size ))
    df = df.groupby('inx').mean()
    ax.grid(True)
    ax.plot(df.x, df.y, 'yo')

def getSymbol(instrument_root, event_datetime_obj):
    tuples = zip(*sym_vols(event_datetime_obj))
    symbols_dict = dict(zip(tuples[0], tuples[1]))
    instr_symbols_dict = dict((k, v) for k,v in symbols_dict.iteritems() if k.startswith(instrument_root))
    symbol = max(instr_symbols_dict.keys(),  key=(lambda k: instr_symbols_dict[k]))
    return symbol

def getMarketDataFrameForEvent(event_date, instrument_root, symbol):
    store_filename = '/local/disk1/data/snapshots/0/' + instrument_root + '/' + symbol + '_0_' + event_date.replace('-', '') + '.h5'
    #print store_filename
    store = pd.HDFStore(store_filename)
    df = store['df']
    return df

def extractTrainDataFrame(df, event_datetime_obj, stabilize_time_s, event_duration_s, prediction_lookahead_s):
    df.set_index(df['time'], inplace=True)
    df.head().transpose()
    train_start_time_obj = event_datetime_obj + timedelta(seconds=stabilize_time_s)
    train_end_time_obj = event_datetime_obj + timedelta(seconds=event_duration_s + prediction_lookahead_s + 10)  # make sure series is long enough to include looking ahead at the end
    train_start_time_str = train_start_time_obj.strftime("%Y-%m-%d %H:%M:%S")
    train_end_time_str = train_end_time_obj.strftime("%Y-%m-%d %H:%M:%S")
    train_df = df[train_start_time_str:train_end_time_str]
    return train_df

def computeReturns(df):
    returns_df = df[['microprice_ema_200ms', 'microprice_ema_2s', 'microprice_ema_10s', 
                     'ask_vol_ema_200ms', 'ask_vol_ema_2s', 'ask_vol_ema_10s', 'bid_vol_ema_200ms', 
                     'bid_vol_ema_2s', 'bid_vol_ema_10s']].pct_change() 
    df[['microprice_ema_200ms_ret', 'microprice_ema_2s_ret', 
        'microprice_ema_10s_ret', 'ask_vol_ema_200ms_pchg', 'ask_vol_ema_2s_pchg', 'ask_vol_ema_10s_pchg', 
        'bid_vol_ema_200ms_pchg', 'bid_vol_ema_2s_pchg', 'bid_vol_ema_10s_pchg']] = returns_df
    for col in df[['microprice_ema_200ms_ret', 'microprice_ema_2s_ret', 'microprice_ema_10s_ret']]:
        df[col] *= 100
    df.fillna(0, inplace=True)
    return df

def computeDerivedFeatures(train_df):
    train_df['price_imbalance'] = 100.0 * ((train_df['microprice'] - train_df['midquote']) / train_df['midquote'])
    train_df['trade_volume_imbalance'] = ((train_df['buy_vol'] - train_df['sell_vol']) / train_df['abs_vol']).fillna(0,inplace = True)
    return train_df

def prepare_data(   
                    dates=[], 
                    event_start_time='08:15:00', 
                    instrument_root = '6E', 
                    stabilize_time_s = 2,
                    event_duration_s = 120,
                    prediction_lookahead_s = 0.1,
                    event_duration_intervals = 1200,
                    lookback_window = 10,
                    prediction_lookahead_intervals = 1,
                    features = [],
                    metric = None):
    ddf = []
    for train_date in dates:
        event_date = train_date
        event_datetime_obj = datetime.strptime(event_date + " " + event_start_time, '%Y-%m-%d %H:%M:%S')
        symbol = getSymbol(instrument_root, event_datetime_obj)
        df = getMarketDataFrameForEvent(event_date, instrument_root, symbol)
        train_df = extractTrainDataFrame(df, event_datetime_obj, stabilize_time_s, event_duration_s, prediction_lookahead_s)
        train_df = computeDerivedFeatures(train_df)
        train_df = computeReturns(train_df)
        train_df = train_df[features]
        train_df['metric'] = train_df[metric].shift(-lookback_window - prediction_lookahead_intervals)
        train_df = train_df.head(event_duration_intervals - lookback_window + 1)
        train_df = train_df.tail(len(train_df) - 1)
        ddf.append(train_df)
    ddf = pd.concat(ddf)
    ds = create_pybrain_dataset(ddf, features, metric='metric')
    return (ds,ddf)


def create_pybrain_dataset(df, features, metric='metric'):
    ds = SupervisedDataSet(len(features), 1)
    for i in xrange(len(df)):
        instance_features = []
        instance_slice = df.iloc[i]
        for feature in features:
            instance_features.append(float(instance_slice[feature]))
        ds.addSample(instance_features, [float(instance_slice[metric])])
    return ds

def compute_net_forecast(net, df, features):
    df['net_forecast'] = np.array([net.activate(x) for x in df[features].values ])
    df['net_error'] = df['net_forecast'] - df['metric']
    df['net_error2'] = df['net_error'] * df['net_error']
    rmse = np.sqrt(df['net_error2'].sum() / len(df))
    me = df['net_error'].sum() / len(df)
    corr = np.corrcoef(df['metric'],df['net_forecast'])[1][0]
    return (df,rmse, me, corr)

def compute_gb_forecast(gbr, df, features, stage):
    df['gb_forecast'] = predict_at_stage(gbr, df[features], stage)
    df['gb_error'] = df['gb_forecast'] - df['metric']
    df['gb_error2'] = df['gb_error'] * df['gb_error']
    rmse = np.sqrt(df['gb_error2'].sum() / len(df))
    me = df['gb_error'].sum() / len(df)
    try:
        corr = np.corrcoef(df['metric'],df['gb_forecast'])[1][0]
    except:
        corr= np.nan
    return (df,rmse, me, corr)

def predict_at_stage(gbr, x, stage):
    k = 0 
    for i in gbr.staged_predict(x):
        if k == stage - 1:
            return i
        k +=1

def print_feature_importances(gbr, features):
    i = 0
    tuples = []
    for feature in features:
        tuples.append((feature, gbr.feature_importances_[i]))
        i += 1
    tuples = sorted(tuples, key =lambda x: x[1], reverse=True)
    return tuples

def get_cols_like(df, tok):
    cols = []
    for col in df.columns:
        if col.count(tok) > 0:
            cols.append(col)
    return cols

def extract_effective_date_from_fix_line(line):
    from_zone = tz.gettz('UTC')
    to_zone = tz.gettz('America/Chicago')
    toks = line.split(chr(1))
    for tok in toks:
        t = tok.split('=')
        if t[0] == '52':
            tm = datetime.strptime(t[1],'%Y%m%d%H%M%S%f').replace(tzinfo=from_zone).astimezone(to_zone)
            weekday = tm.weekday()
            if( tm.weekday() == 6):
                tm = tm + timedelta(seconds = 16 * 3600)
                return tm.date().strftime('%Y%m%d')
            else:
                tm = tm + timedelta(seconds = 7.5 * 3600)
                return tm.date().strftime('%Y%m%d')  
            
    return None


def split_fix_file_by_date(file_name = '/local/disk1/cme_derived/ES_20150105_20150109'):
    base_name = os.path.basename(file_name)
    dir_name = os.path.dirname(file_name)
    start = base_name.split('.')[0].split('_')[1]
    end = base_name.split('.')[0].split('_')[2]
    prefix = base_name.split('.')[0].split('_')[0]

    workdays = get_workdays(datetime.strptime(start, '%Y%m%d'), datetime.strptime(end, '%Y%m%d'))
    files = {}
    fns = []
    
    for d in workdays:
        dt = d.strftime('%Y%m%d')
        fn = dir_name + '/' + prefix + '.' + dt
        fns.append(fn)
        print fn
        files[dt] = open(fn, 'w')
    
    with open(file_name) as f:
        for line in f:
            dt = extract_effective_date_from_fix_line(line)
            if files.has_key(dt):
                files[dt].write(line)
    import subprocess
    for f in fns:
        subprocess.call(["gzip", f])


def get_active_symbols_ascii(symbol='6E', 
                            file_root='/local/disk1/cme_derived/',
                            start=datetime(2011,7,1),
                            end=datetime(2014,1,1),
                            restrict_to=2):
    day_symbols = {}
    file_name=file_root + '/%s/%s.secdef.' % (symbol, symbol)
    workdays = get_workdays(start, end)
    for dt in workdays:
        strdt = dt.strftime('%Y%m%d')
        day_symbols[strdt]=set([])
        ff = file_name + strdt
        f = open(ff)
        for line in f.readlines():
            toks = line.split(chr(1))
            for tok in toks:
                tt = tok.split('=')
                if tt[0] == '107':
                    day_symbols[strdt].add(tt[1])
        for e in day_symbols.keys():
            day_symbols[e] = order_futures(dt, day_symbols[e])[:restrict_to]
    return day_symbols


def txt_to_h5(file_name = '/local/disk1/staging/6EZ1_0_20111012.txt'):
    file_root = file_name.split('.')[0]
    df = pd.read_csv(file_name)
    store = pd.HDFStore(file_root + '.h5')
    store.append('df', df, complib='zlib', complevel=9)
    os.remove(file_name)

def get_vix(tok='vix'):
    vix = pd.DataFrame.from_csv( '/mnt/storage/volatility/%s.csv' % tok )
    vix = vix.reset_index()
    vix.Date = vix.Date.apply(lambda x: int(''.join(str(x).split(' ')[0].split('-'))) )
    vix = vix.sort(columns=['Date'])
    vix=vix.reset_index()
    vix['vol'] = vix.Close.shift(1)
    vix['vol'] = vix.vol.apply(lambda x: 17 if np.isnan(x) else x )
    vv = {}
    for dt in sorted(vix.Date.values):
        vv[dt] = vix[vix.Date == dt].vol.values[0]
    return vv

def get_most_liquid_symbol(dt, root):
    sv = sym_vols(dt)
    mm = 0
    ret = None
    for entry in sv:
        if len(entry[0]) != 4:
            continue
        if entry[0][:2] != root:
            continue
        if mm < entry[1]:
            mm = entry[1]
            ret = entry[0]

    if ret is None:
        syms = get_active_symbols_ascii(symbol=root, start=dt, end=dt, restrict_to=1)
        if len(syms) > 0:
            return (syms[syms.keys()[0]][0], 1000000)
        else:
            print dt
    else:    
        return (ret, mm)

def get_num_weekdays_of_kind(date, kind):
    num = 0 
    day = date.day
    day_of_week = date.weekday()
    while day > 1:
        if day_of_week == kind:
            num += 1
        day -= 1
        day_of_week -= 1
        if day_of_week < 0:
            day_of_week += 7
    return num

def get_front_ES_symbol(dt, root = 'ES'):
    if dt.month == 1 or dt.month == 2: #jan
        return '%s%s%s' % (root,'H', str(dt.year)[-1])
    elif dt.month == 3: #mar
        if get_num_weekdays_of_kind(dt, 4) <= 1 :
            return '%s%s%s' % (root,'H', str(dt.year)[-1])
        else:
            return '%s%s%s' % (root,'M', str(dt.year)[-1])
    elif dt.month == 4 or dt.month == 5: #apr
        return '%s%s%s' % (root,'M', str(dt.year)[-1])
    elif dt.month == 6: #june
        if get_num_weekdays_of_kind(dt, 4) <= 1 :
            return '%s%s%s' % (root,'M', str(dt.year)[-1])
        else:
            return '%s%s%s' % (root,'U', str(dt.year)[-1])
    elif dt.month == 7 or dt.month == 8: #july
        return '%s%s%s' % (root,'U', str(dt.year)[-1])
    if dt.month == 9: #sep
        if get_num_weekdays_of_kind(dt, 4) <= 1 :
            return '%s%s%s' % (root,'U', str(dt.year)[-1])
        else:
            return '%s%s%s' % (root,'Z', str(dt.year)[-1])
    elif dt.month == 10 or dt.month == 11: #oct
        return '%s%s%s' % (root,'Z', str(dt.year)[-1])
    elif dt.month == 12: #dec
        if get_num_weekdays_of_kind(dt, 4) <= 1 :
            return '%s%s%s' % (root,'Z', str(dt.year)[-1])
        else:
            return '%s%s%s' % (root,'H', str(dt.year + 1)[-1])

def get_front_CL_symbol(dt):
    weekday = dt.day
    if dt.month < 11:
        if weekday < 17:
            return '%s%s%s' % ('CL',FUTURE_CODES2[dt.month + 1], str(dt.year)[-1])
        else:
            return '%s%s%s' % ('CL',FUTURE_CODES2[dt.month + 2], str(dt.year)[-1])
    elif dt.month == 11:
        if weekday < 17:
            return '%s%s%s' % ('CL',FUTURE_CODES2[dt.month + 1], str(dt.year)[-1])
        else:
            return '%s%s%s' % ('CL',FUTURE_CODES2[1], str(dt.year + 1)[-1])
    elif dt.month == 12:
        if weekday < 17:
            return '%s%s%s' % ('CL',FUTURE_CODES2[1], str(dt.year + 1)[-1])
        else:
            return '%s%s%s' % ('CL',FUTURE_CODES2[2], str(dt.year + 1)[-1])

def get_front_NG_symbol(dt):
    weekday = dt.day
    if dt.month < 11:
        if weekday < 17:
            return '%s%s%s' % ('NG',FUTURE_CODES2[dt.month + 1], str(dt.year)[-1])
        else:
            return '%s%s%s' % ('NG',FUTURE_CODES2[dt.month + 2], str(dt.year)[-1])
    elif dt.month == 11:
        if weekday < 17:
            return '%s%s%s' % ('NG',FUTURE_CODES2[dt.month + 1], str(dt.year)[-1])
        else:
            return '%s%s%s' % ('NG',FUTURE_CODES2[1], str(dt.year + 1)[-1])
    elif dt.month == 12:
        if weekday < 17:
            return '%s%s%s' % ('NG',FUTURE_CODES2[1], str(dt.year + 1)[-1])
        else:
            return '%s%s%s' % ('NG',FUTURE_CODES2[2], str(dt.year + 1)[-1])

def get_front_Z_symbol(dt, root):
    weekday = dt.day
    if dt.month == 1:
        return '%s%s%s' % (root, 'H',str(dt.year)[-1])
    elif dt.month == 2:
        if weekday < 26:
            return '%s%s%s' % (root, 'H',str(dt.year)[-1])
        else:
            return '%s%s%s' % (root, 'M',str(dt.year)[-1])
    elif dt.month == 3 or dt.month == 4:
        return '%s%s%s' % (root, 'M',str(dt.year)[-1])
    elif dt.month ==5:
        if weekday < 26:
            return '%s%s%s' % (root, 'M',str(dt.year)[-1])
        else:
            return '%s%s%s' % (root, 'U',str(dt.year)[-1])

    elif dt.month == 6 or dt.month == 7:
        return '%s%s%s' % (root, 'U',str(dt.year)[-1])
    elif dt.month == 8:
        if weekday < 26:
            return '%s%s%s' % (root, 'U',str(dt.year)[-1])
        else:
            return '%s%s%s' % (root, 'Z',str(dt.year)[-1])
    elif dt.month == 9 or dt.month == 10:
        return '%s%s%s' % (root, 'Z',str(dt.year)[-1])
    elif dt.month == 11:
        if weekday < 26:
            return '%s%s%s' % (root, 'Z',str(dt.year)[-1])
        else:
            return '%s%s%s' % (root, 'H',str(dt.year + 1)[-1])
    else:
        return '%s%s%s' % (root, 'H',str(dt.year + 1)[-1])
        

def get_front_FX_symbol(dt, root):
    if dt.month == 1 or dt.month == 2: #jan
        return '%s%s%s' % (root,'H', str(dt.year)[-1])
    elif dt.month == 3: #mar
        if get_num_weekdays_of_kind(dt, 2) <= 1 :
            return '%s%s%s' % (root,'H', str(dt.year)[-1])
        else:
            return '%s%s%s' % (root,'M', str(dt.year)[-1])
    elif dt.month == 4 or dt.month == 5: #apr
        return '%s%s%s' % (root,'M', str(dt.year)[-1])
    elif dt.month == 6: #june
        if get_num_weekdays_of_kind(dt, 2) <= 1 :
            return '%s%s%s' % (root,'M', str(dt.year)[-1])
        else:
            return '%s%s%s' % (root, 'U', str(dt.year)[-1])
    elif dt.month == 7 or dt.month == 8: #july
        return '%s%s%s' % (root, 'U', str(dt.year)[-1])
    if dt.month == 9: #sep
        if get_num_weekdays_of_kind(dt, 2) <= 1 :
            return '%s%s%s' % (root, 'U', str(dt.year)[-1])
        else:
            return '%s%s%s' % (root, 'Z', str(dt.year)[-1])
    elif dt.month == 10 or dt.month == 11: #oct
        return '%s%s%s' % (root, 'Z', str(dt.year)[-1])
    elif dt.month == 12: #dec
        if get_num_weekdays_of_kind(dt, 2) <= 1 :
            return '%s%s%s' % (root, 'Z', str(dt.year)[-1])
        else:
            return '%s%s%s' % (root, 'H', str(dt.year + 1)[-1])


def make_simple_data_CL(start=datetime(2011, 7, 1), end=datetime(2016,5,13)):
    workdays =get_workdays2(start,end)
    data = {}
    for dt in workdays:
        ss = get_front_CL_symbol(dt)
        try:
            tt = load_day(dt, ss, res='1')
        except:
            #print dt
            continue
        dt2 = int(dt.strftime('%Y%m%d'))
        data[dt2] = {}
        data[dt2]['bid'] = tt.top_bid_price.values.astype(int)
        data[dt2]['ask'] = tt.top_ask_price.values.astype(int)
    return data

def get_front_symbol(dt, root):
    if root in ('ZN','ZB','ZF','ZT'):
        return get_front_Z_symbol(dt, root=root) 
    elif root == 'NG':
        return get_front_NG_symbol(dt) 
    elif root == 'CL':
        return get_front_CL_symbol(dt) 
    elif root in ('6A','6B','6C','6E','6S','6M', '6J'):
        return get_front_FX_symbol(dt, root=root) 
    elif root in ('ES','NQ'):
        return get_front_ES_symbol(dt, root=root) 
    else:
        return None

def get_simple_df(start=datetime(2011, 7, 1), end=datetime(2016,5,27), res='1', root='ZN'):
    workdays = get_workdays2(start,end)
    cols = ['buy_vol', 'sell_vol',
            'high', 'low', 
            'open', 'close',
            'top_bid_price', 'top_ask_price',
            'bid_size_0', 'ask_size_0']
    data = []
    for dt in workdays:
        sym = get_front_symbol(dt, root=root) 
        try:
            tt = load_day(dt, sym, res=res)
        except:
            print 'missing', dt, sym
            continue 
        tt = tt[cols]
        tt['order'] = range(len(tt))
        tt['date'] = int(dt.strftime('%Y%m%d'))
        data.append(tt)
    df = pd.concat(data)
    return df

def get_df(start=datetime(2011, 7, 1), end=datetime(2016,5,27), res='1', root='6A'):
    workdays = get_workdays2(start,end)
    data = []
    for dt in workdays:
        sym = get_front_symbol(dt, root)
        try:
            tt = load_day(dt, sym, res=res)
        except:
            continue
        tt['order'] = range(len(tt))
        tt['date'] = int(dt.strftime('%Y%m%d'))
        data.append(tt)
    df = pd.concat(data)
    return df

def read_plain_survey(f):
    return pd.DataFrame.from_csv(f,index_col=None)


def append_nans_to_survey(survey):
    import copy

    tickers = (survey.TICKER.unique())
    dates = (survey.DATE.unique())

    empty_rows = []

    dummy = {}
    for col in survey.columns:
        dummy[col] = np.nan

        dummy['TIME'] = '08:30:00'

    for dt in dates:
        for ticker in tickers:
            s = survey[(survey.DATE ==dt) & (survey.TICKER == ticker)]
            if len(s) < 1:
                dummy['TICKER'] = ticker
                dummy['DATE'] = dt
                empty_rows.append(copy.deepcopy(dummy))
            
    empty_rows = pd.DataFrame(empty_rows, columns=survey.columns)
    survey2 = pd.concat([survey, empty_rows])
    survey = survey2.sort(columns=['DATE', 'TICKER'])
    survey.index = range(len(survey))
    
    return survey

def read_survey(f, dates = None):
    survey = read_plain_survey(f)
    survey = append_nans_to_survey(survey)
    survey['dt'] = (survey['DATE'].apply(lambda x: str(x)) + ' ' + survey['TIME'])
    survey['dt'] = survey['dt'].apply(lambda x: datetime.strptime(x, '%Y%m%d %H:%M:%S')).astype(datetime)
    survey['bin'] = survey.dt.apply(lambda x: x.hour * 3600 + x.minute * 60 + x.second - 3600 * 8)
    del survey['dt']
    if dates is not None:
        survey = survey[survey['DATE'].isin(dates)]
    return survey

def create_event_df(root='6A', path='/mnt/storage/survey/', event_type='PAYROLL'):
    survey = read_plain_survey(path + event_type)
    df = get_simple_df(root=root)
    df = df[df.date.isin(survey.DATE.unique())]
    store = pd.HDFStore('/home/pgrosul/bt_data/%s_%s.h5' % (event_type,root))
    store.append('df', df, complib='zlib', complevel=9)
    store.close()

def price_for_size(df, is_buy = True, size = 0, num_levels= 5, offset= 0, mpi = 0):
    
    count = 0
    tot = 0.0

    if is_buy:
        for i in xrange(num_levels):
            if i < 5:
                sz  = df['ask_size_%s' % i].values[offset] 
            else:
                sz = df['ask_size_%s' % 4].values[offset]
                
            price = df.top_ask_price[offset] + i * mpi
            
            if size > 0:
                if count + sz >= size:
                    sz = size - count
                    count += sz
                    tot += sz * price 
                    break
                    
            count += sz
            tot += sz * price
    else:
        for i in xrange(num_levels):
            if i < 5:
                sz= df['bid_size_%s' % i].values[offset]
            else:
                sz = df['bid_size_%s' % 4].values[offset]
            price = df.top_bid_price[offset] - i * mpi
            
            if size > 0:
                if count + sz >= size:
                    sz = size - count
                    count += sz
                    tot += sz * price 
                    break
                    
            count += sz
            tot += sz * price

    return (count, tot / count)

def true_profit(df, is_buy=True, size = 0, num_levels = 5 , offset1 = 0, offset2 = 0, mpi = 0, tick_price = 0):
    buy = price_for_size(df, is_buy = is_buy, size = size, num_levels = num_levels, offset = offset1, mpi = mpi)
    sell = price_for_size(df, is_buy = (not is_buy), size = buy[0], num_levels = 100, offset = offset2, mpi = mpi )    
    pnl = (sell[1] - buy[1]) * buy[0] * (tick_price / mpi)
    return(buy[0], pnl)


SECD = {
        'ZB' :  {   'mpi' : 0.03125,  'dpt' : 31.25 },  # 1 / 32   
        'ZN' :  {   'mpi' : 0.015625, 'dpt' : 15.625 }, # 1 / 64
        'ZF' :  {   'mpi' : 0.0078125,'dpt' : 7.125 },
        'ZT' :  {   'mpi' : 0.0078125,'dpt' : 15.625 },
        '6A' :  {   'mpi' : 1.0,      'dpt' : 10.0 },
        '6B' :  {   'mpi' : 1.0,      'dpt' : 10.0 },
        '6C' :  {   'mpi' : 1.0,      'dpt' : 10.0 },
        '6E' :  {   'mpi' : 1.0,      'dpt' : 12.5 },   # before 2016
        '6S' :  {   'mpi' : 1.0,      'dpt' : 12.5 },
        '6J' :  {   'mpi' : 1.0,      'dpt' : 12.5 },   # before 2015 
        '6M' :  {   'mpi' : 25,       'dpt' : 5.0 },
        'ES' :  {   'mpi' : 25,       'dpt' : 12.5 },
        'NQ' :  {   'mpi' : 25,       'dpt' : 5.0 },
        'CL' :  {   'mpi' : 1.0,      'dpt' : 10.0 },
        'NG' :  {   'mpi' : 1.0,      'dpt' : 10.0 },
    }

def get_mpi(root, dt  = datetime(2016,1,1)):
    mpi = SECD[root]['mpi']
    if root == '6E' and dt >= datetime(2016,1,1):
        mpi = float(mpi * 0.5)
    return mpi

def get_dpt(root, dt = datetime(2016,1,1) ):
    dpt = SECD[root]['dpt']
    if root == '6E' and dt >= datetime(2016,1,1):
        dpt = float(dpt * 0.5)
    return dpt
