import evolver as evo
import simple_backtest2 as bt
import pickle
import numpy as np
import sys
import subprocess
import glob
import os
import random
import time
import pandas as pd


out_file = 'dummy'
gen = 0

proto = evo.prototype()

#proto.add_variable(evo.variable('ut_param_1', evo.TYPE_FLOAT, sel=[ 0.00001, 0.0001, 0.001, 0.01, 0.1, 0.5, 1 ]))
proto.add_variable(evo.variable('vol_quadratic_alpha', evo.TYPE_FLOAT, sel=[ 100, 200, 300, 400, 500, 1000.0, 2000, 3000, 4000, 5000, 10000 ]))
proto.add_variable(evo.variable('vol_power', evo.TYPE_FLOAT, sel=[ 2, 4 ]))
#proto.add_variable(evo.variable('vol_power2', evo.TYPE_FLOAT, sel=[ 0, 1, 2 ]))
proto.add_variable(evo.variable('pnl_target', evo.TYPE_FLOAT, sel=[ 5, 6, 7,8,9,10,11,12,13, 14, 15, 20, 25, 30, 40, 50, 60, 70, 75, 100 ]))
proto.add_variable(evo.variable('dynamic_pnl_target_slope', evo.TYPE_FLOAT, sel=[0.000001, 0.000005,  0.00001, 0.00005, 0.00001, 0.0005, 0.0001, 0.005,  0.001, 0.005,  0.01, 0.05,  0.1]))
#proto.add_variable(evo.variable('max_prize', evo.TYPE_INT, sel=[500, 1000, 5000, 10000, 20000, 50000, 100000]))
#proto.add_variable(evo.variable('max_drawdown', evo.TYPE_FLOAT, sel=[-50000, -20000,-10000,-5000]))
#proto.add_variable(evo.variable('game_drawdown', evo.TYPE_FLOAT, sel=[-20000,-10000, -5000, -2000]))
proto.add_variable(evo.variable('max_pos', evo.TYPE_INT, sel=[50, 100, 200, 300, 400, 500, 1000]))


def get_vix():
    vix = pd.DataFrame.from_csv('/mnt/storage/pavel/vix.csv')
    vix = vix.reset_index()
    vix.Date = vix.Date.apply(lambda x: int(''.join(str(x).split(' ')[0].split('-'))) )
    vix = vix.sort(columns=['Date'])
    vix=vix.reset_index()
    vix['vol'] = vix.Close.shift(1)
    vix['vol'] = vix.vol.apply(lambda x: 14.23 if np.isnan(x) else x )
    vv = {}
    for dt in vix.Date.values:
        vv[dt] = vix[vix.Date == dt].vol.values[0]
    return vv

def fitness_individual(a):
    print a,

    #seed = random.randint(0,5)
    #data = safe_pickle_load('data/data_sample_%d' % seed)
    data = safe_pickle_load('data/data_big')
    # data = safe_pickle_load('data/data_sample_0')

    vol = get_vix()
 
    s = bt.stacker()

    s.mpi = 25
    s.num_levels = 200 
    s.skip_mask = 2
 
    s.ask_replenish_edge = 1
    s.ask_replenish_size = 1 
    s.ask_replenish = False 
    s.ask_ut_type = bt.quadratic
    s.ask_ut_param_1 = 0.0 # a['ut_param_1']
    s.ask_ut_param_2 = 1.0
 
    s.bid_replenish_edge = 1 
    s.bid_replenish_size = 1
    s.bid_replenish = False
    s.bid_ut_type = bt.quadratic
    s.bid_ut_param_1 = 0.0 # a['ut_param_1']
    s.bid_ut_param_2 = 1.0

    pnl_target                 =  a['pnl_target']
    dynamic_pnl_target_slope   =  a['dynamic_pnl_target_slope']
    start_time                 =  0 * 3600 
    max_time_to_start_game     =  3 * 3600
    max_drawdown               =  -5000000#a['max_drawdown']
    max_prize                  =  5000000 #a['max_prize'] 
    game_drawdown              =  -1000000000#a['game_drawdown']
 
    res = bt.multiday_bt(s, 
                         data,
                         pnl_target=pnl_target, 
                         dynamic_pnl_target_slope=dynamic_pnl_target_slope,
                         start_time = start_time, 
                         max_time_to_start_game=max_time_to_start_game,
                         max_drawdown=max_drawdown,
                         max_prize=max_prize,
                         game_drawdown = game_drawdown,
                         vol_quadratic_alpha = a['vol_quadratic_alpha'],
                         vol=vol,
                         vol_power = a['vol_power'],
                         max_pos = a['max_pos'],
                         vol_power2 = 0) #a['vol_power2'])
    results = {}

    results['dates'] = {}

    for dt in sorted(res.keys()):
        abs_vol = 0
        (pnl,pos,cash, games, all_trades) = res[dt]
        for trade in all_trades:
            abs_vol += np.abs(trade[2])
        net = pnl[-1] - float(abs_vol) * 0.7

        daily_results = {}
        daily_results['pnl'] = net
        daily_results['vol'] = abs_vol
        daily_results['mean_pos'] = np.mean(np.abs(pos))
        daily_results['max_pos'] = np.max(pos)
        daily_results['min_pos'] = np.min(pos)
        results['dates'][dt] = daily_results   
    
    total_pnl = 0
    total_vol = 0
    total_mean_pos = 0


    pnls = []
    for dt in results['dates']:
        res = results['dates'][dt]
        total_pnl += res['pnl']
        total_vol += res['vol']
        total_mean_pos += res['mean_pos']
        pnls.append(res['pnl'])

    risk_ratio = total_pnl / (total_mean_pos * total_mean_pos )

    risk_ratio2 = 16.0 * ( np.median(pnls) / np.std(pnls)) 
    risk_ratio3 = 16.0 * ( np.mean(pnls) / np.std(pnls)) 

    if risk_ratio3 < risk_ratio2:
        risk_ratio2 = risk_ratio3

    k = 1.0 / float(len(results['dates']))
    
    p = risk_ratio2
    #if risk_ratio2 > 2.0:
    #    p = 2.0
    #    p += total_pnl * k * 0.001  
        
    results['individual'] = a
    results['fitness'] = p
    results['total_pnl'] = total_pnl
    results['total_vol'] = total_vol
    results['total_mean_pos'] = total_mean_pos    
    results['file'] = out_file

    print   '%s FITNESS: %.2f PNL %.0f VOL %.0f POS %.0f RISK0 %.2f RISK2 %.2f' % (out_file, p, total_pnl*k, total_vol*k, total_mean_pos*k, risk_ratio, risk_ratio2 )

    pickle.dump(results, open(out_file, 'wb'))   
 
    return p

def fitness(ind):
    d = []
    for a in ind:
        a['fitness'] = fitness_individual(a)
        d.append(a)
    return d

def fitness_parallel(ind):
    
    global gen

    i = 0
    for a in ind:
        fn = 'input_%d_%d' % ( gen, i )
        pickle.dump(a, open( fn, 'wb'))
        subprocess.Popen(['./fitness.sh',fn])        
        i += 1

    while True:
        if len(glob.glob('output_%d_*' % gen)) == len(ind):
            break

    d = []
    i = 0
    for a in ind:
        res = safe_pickle_load('output_%d_%d' % (gen, i))
        a['fitness'] = res['fitness']
        d.append(a)
        i += 1     
    gen += 1
    return d

def fitness_individual_wrapper(a):
    global out_file
    out_file = a.replace('input','output')
    indiv = safe_pickle_load(a)
    r = fitness_individual(indiv)

def safe_pickle_load(f):
    err = True
    res = None
    for i in xrange(10):    
        try:
            res = pickle.load(open(f,'rb'))
            err = False
        except:
            pass
        
        if err == False:
            break
    return res

def run_evolver():
    ev = evo.evolver(proto, fitness_parallel, population_size= 16, selection_size=0, mutation_prob=0, fresh_blood_ratio=1.0 )
    ev.run_n_generations(50) 

if __name__ == "__main__":

    #print sys.argv

    if len(sys.argv) == 1:
        run_evolver()
    else:
        fitness_individual_wrapper(sys.argv[1])
