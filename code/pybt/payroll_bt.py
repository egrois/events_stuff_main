import sys
sys.path.append('/home/pgrosul/git/code/pybt')
import numpy as np
from datetime import datetime,time, timedelta
import pandas as pd
import utils

import numpy as np
import pickle

import matplotlib.pyplot as plt


ROOTS=['ZN','ZB','ZF','ZT','ES','CL','6A','6B','6C', '6M', '6E', '6S', '6J']
INTERVALS = [-100, -10, -1, 0,1,2,3,4,5, 10,20,30,40,50, 100,200,300,400,500, 1000, 5000, 10000, 20000]


def detect(rdf,root='ZN', dtt='20110708', window=100, ax=plt, make_picture=True, offset = 1800):
    dt = datetime.strptime(str(dtt), '%Y%m%d')
    
    offset = rdf[rdf.DATE == int(dt.strftime('%Y%m%d'))]['bin'].values[0] * 10
    
    try:
        sym = utils.get_front_symbol(dt, root)
        fine = utils.load_day(dt,sym, res='0',root=root)
    except:
        return -1
        
    sample_vol = (fine.buy_vol + fine.sell_vol).values[offset-window:offset+window]
    sample_price = fine.microprice.values[offset-window:offset+window]
    sample_price_diff = np.diff(sample_price)

    fine_offset_vol = offset - window + np.argmax(sample_vol)

    fine_offset_price1 = offset - window + np.argmax(sample_price_diff) + 1
    fine_offset_price2 = offset - window + np.argmin(sample_price_diff) + 1

    fine_offset = -1
    
    if fine_offset_price1 == fine_offset_vol or fine_offset_price2 == fine_offset_vol:
        fine_offset = fine_offset_vol
    else:
        vol_thres = np.max(sample_vol) / 10 
        for i in xrange(len(sample_vol)):
            if sample_vol[i] > vol_thres:
                fine_offset_vol = offset - window + i
                if fine_offset_price1 == fine_offset_vol or fine_offset_price2 == fine_offset_vol:
                    fine_offset = fine_offset_vol
                break
                
                
    a_s = rdf[rdf.DATE == int(dtt)].a_s.values[0]

    if make_picture == True:
        ax.plot(sample_vol, '-')
        ax.twinx().plot(sample_price - sample_price[0], 'r-')
        ax.set_title( '%s %s %s %d' % (sym, dtt, fine_offset, a_s))
    
    return fine_offset

def consensus_offset(offsets, offset_est):
    candidates = []
    for key in offsets.keys():
        if offsets[key] <= offset_est:
            del offsets[key]
        else:
            candidates.append(offsets[key])
    if len(candidates) > 0:
        return np.min(candidates)
    else:
        return offset_est + 1

def detect_date(rdf,roots, dtt='20110805', make_picture = True, window=100):
    ncols = 3
    if make_picture == True:
        f, ax = plt.subplots(len(roots)/ncols + 1, ncols, figsize=(20,12))
        
    offsets = {}
        
    i = 0
    for key in roots:
        offset_est = rdf[rdf.DATE == int(dtt)]['bin'].values[0] * 10
        if make_picture == True:
            offset = detect(rdf,root=key, dtt=dtt, ax=ax[i/ncols][i%ncols], window=window, make_picture=make_picture, 
                            offset=offset_est)
        else:
            offset = detect(rdf,root=key, dtt=dtt, window=window, make_picture=make_picture, offset=offset_est)
        
        if offset > 0:
            offsets[key] = offset
        
        i += 1
    
    if make_picture == True:
        plt.show()
        
    return consensus_offset(offsets, offset_est)

def make_payroll_times(roots = ROOTS):
    rdf = make_rdf(apply_times = False)
    event_times = {}

    for dtt in rdf.DATE.unique():
        det = detect_date(rdf,roots,dtt=str(dtt), window=100, make_picture = False)
        event_times[dtt] = det
        print dtt, det
    
    import pickle
    pickle.dump(event_times, open('/home/pgrosul/bt_data/PAYROLL_TIMES', 'w')) 

def enrich_survey(survey):
    repeat_cols = ['ACTUAL_RELEASE', 'SURVEY_MEDIAN', 'SURVEY_AVG', 'SURVEY_HIGH', 'SURVEY_LOW','REVISION']
    unique_cols = ['DATE', 'TIME', 'bin']
    mm = {'NFP PCH':'a', 'NFP TCH':'b', 'USMMMNCH':'c', 'USURTOT':'d'}
    rdf = survey[survey.TICKER == 'NFP PCH']
    rdf = rdf[unique_cols]
    rdf.index = rdf.DATE
    for ticker in survey.TICKER.unique():
        rdf2 = survey[survey.TICKER == ticker]
        rdf2.index = rdf.DATE
        rdf2 = rdf2[repeat_cols]
        cols2 = []
        for col in repeat_cols:
            rdf[mm[ticker] + '_' + col] = rdf2[col]
    rdf['TIME_OF_YEAR'] = rdf.DATE.apply(lambda x: datetime.strptime(str(x), '%Y%m%d').timetuple().tm_yday)
    rdf['YEAR'] = rdf.DATE.apply(lambda x: int(str(x)[:4]))
    
    rdf['a_s'] = rdf.a_ACTUAL_RELEASE - rdf.a_SURVEY_MEDIAN
    rdf['b_s'] = rdf.b_ACTUAL_RELEASE - rdf.b_SURVEY_MEDIAN
    rdf['c_s'] = rdf.c_ACTUAL_RELEASE - rdf.c_SURVEY_MEDIAN
    rdf['d_s'] = rdf.d_ACTUAL_RELEASE - rdf.d_SURVEY_MEDIAN
    rdf['e_s'] = rdf.a_REVISION
    rdf['f_s'] = rdf.b_REVISION
    
    return rdf


def make_rdf(roots = ROOTS, apply_times = True ):
    #df = {}
    #for sym in roots:
    #    store = pd.HDFStore('/home/pgrosul/bt_data/PAYROLL_%s.h5' % sym, 'r')
    #    df[sym] = store['df']

    survey = utils.read_survey('/mnt/storage/survey/PAYROLL')
    rdf = enrich_survey(survey)

    if apply_times:
        event_times = pickle.load(open('/home/pgrosul/bt_data/PAYROLL_TIMES', 'r'))
        tt = pd.DataFrame(zip(event_times.keys(), event_times.values()), columns=['DATE', 'true_bin'])
        tt.index = tt.DATE
        rdf['true_bin'] = tt['true_bin']

    return rdf

def make_daily_returns_frame(roots,
                             true_offset,
                             dtt='20110708',
                             intervals = None):
    
    dt = datetime.strptime(str(dtt), '%Y%m%d')

    data = []
    for root in roots:
        root_data = []
        root_data.append(dtt)        
        root_data.append(root)
        sym = utils.get_front_symbol(dt, root) 
        
        has_data = True
    
        try:
            fine = utils.load_day(dt,sym, res='0',root=root)
        except:
            has_data = False
        
        if has_data and len(fine) <  288001:
            has_data = False
            print len(fine), dt, root
            
        if has_data:    
            for interval in intervals:
                root_data.append(fine.microprice.values[true_offset + interval])
        else:
            for interval in intervals:
                root_data.append(0)
        
        data.append(root_data)
        
        cols = ['DATE', 'ROOT']
        for interval in intervals:
            cols.append(interval)
        
    return pd.DataFrame(data, columns=cols)

def make_returns(   rdf,
                    roots=ROOTS,
                    intervals = INTERVALS):
    data = []
    for dt in sorted(rdf.DATE.unique()):
        true_offset = rdf[rdf.DATE == dt].true_bin.values[0]
        print dt, true_offset
        dd = make_daily_returns_frame(roots, true_offset,dtt=dt, intervals = intervals)
        data.append(dd)
        
    returns = pd.concat(data)

    for root in roots:
        tt = returns[returns.ROOT == root]
        for interval in intervals:
            if interval <= 0:
                continue
            rdf['RET_%s_%d' % (root, interval)] = ((tt[interval] - tt[-10]) / tt[-10]).values * 10000
    return rdf

def show_ret_diff(dt, rdf, window = 1000, root1 = 'ZN',root2 = 'ZB', ax = plt):
    sym1 = utils.get_front_symbol(dt, root1)
    sym2 = utils.get_front_symbol(dt, root2)

    try:
        df1 = utils.load_day(dt, sym1)
        df2 = utils.load_day(dt, sym2)  
    except:
        return
    
    
    a1 = rdf[rdf.DATE == int(dt.strftime('%Y%m%d'))].true_bin - 10
    
    a2 = a1 + window
    
    ret1 = (df1.microprice.values[a1:a2] - df1.microprice.values[a1 - 10]) / float(df1.microprice.values[a1 - 10])
    ret2 = (df2.microprice.values[a1:a2] - df2.microprice.values[a1 - 10]) / float(df2.microprice.values[a1 - 10])
    
    ax.plot(ret1/ret2)
    
    #ax.plot(ret1)
    #ax.plot(ret2)
    
    try:
        ax.set_title(dt)
    except:
        ax.title(dt)

def directional_bt(rdf,
                   stats,
                   stats2,
                   root = 'ZN',
                   entry_time = 5,
                   exit_time = 2000,
                   go_thres = 0,
                   size_mult= 10,
                  ):    
    data = {}

    for dt in sorted(rdf.DATE.unique()):
    
        ttt = rdf[rdf.DATE == dt]
    
        offset = ttt.true_bin.values[0]
        dtt = datetime.strptime(str(dt),'%Y%m%d')
        sym = utils.get_front_symbol(dtt, root) 
        try:
            df = utils.load_day(dtt, sym, res ='0', root=root)
        except:
            continue
        
        is_buy = True
        
        a_s = ttt.a_s.values[0]
              
        if a_s < go_thres:
            is_buy = True
        elif a_s  >= go_thres:
            is_buy = False
        else:
            continue
            
        res = utils.true_profit(df,
                      is_buy=is_buy,
                      size = np.abs(a_s*size_mult),
                      num_levels = max(1,int(np.abs(a_s) / 10.0)), 
                      offset1 = offset + entry_time, 
                      offset2 = offset + exit_time, 
                      mpi= utils.get_mpi(root, dt=dtt), 
                      tick_price = utils.get_dpt(root, dt=dtt))
        data[dt] = res
        
        results = {}
        results['data'] = data
        results['root'] = root
        results['entry_time'] = entry_time
        results['exit_time'] = exit_time
        results['go_thres'] = go_thres
        results['size_mult'] = size_mult
    return results
