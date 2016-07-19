import evolver as evo
import simple_backtest as bt
import pickle
import numpy as np
import sys
import subprocess
import glob
import os
import random

out_file = 'dummy'
gen = 0

proto = evo.prototype()
#proto.add_variable(evo.variable('replenish_edge', evo.TYPE_INT, sel=[0,1,2,3,4,5]))
#proto.add_variable(evo.variable('replenish', evo.TYPE_BOOL))
#proto.add_variable(evo.variable('ut_param_1', evo.TYPE_FLOAT, sel=[0.1, 0.2, 0.5, 1.0, 2.0, 5.0 ]))
proto.add_variable(evo.variable('pnl_target', evo.TYPE_FLOAT, sel=[0,5,10,20,50,100]))
#proto.add_variable(evo.variable('pnl_target_thres', evo.TYPE_INT, sel=[1,2,5,10,20,50,100]))
#proto.add_variable(evo.variable('use_dynamic_pnl_target', evo.TYPE_BOOL))
#proto.add_variable(evo.variable('dynamic_pnl_target_slope', evo.TYPE_FLOAT, sel=[0.0001,0.0005, 0.001, 0.005, 0.01, 0.05, 0.1]))

proto.add_variable(evo.variable('r2_pnl_target', evo.TYPE_FLOAT, sel=[0,5,10,20,50,100]))
#proto.add_variable(evo.variable('r2_pnl_target_thres', evo.TYPE_INT, sel=[1,2,5,10,20,50,100]))
#proto.add_variable(evo.variable('r2_use_dynamic_pnl_target', evo.TYPE_BOOL))
#proto.add_variable(evo.variable('r2_dynamic_pnl_target_slope', evo.TYPE_FLOAT, sel=[0.0001,0.0005, 0.001, 0.005, 0.01, 0.05, 0.1]))

#proto.add_variable(evo.variable('use_dynamic_replenish_size', evo.TYPE_BOOL))
#proto.add_variable(evo.variable('dynamic_replenish_coef', evo.TYPE_FLOAT, sel=[0.0, 0.1, 0.2, 0.4, 0.6, 0.8, 1.0]))
#proto.add_variable(evo.variable('skip_mask', evo.TYPE_INT, sel=[1,2,3,4,8]))
#proto.add_variable(evo.variable('max_prize', evo.TYPE_INT, sel=[500]))
proto.add_variable(evo.variable('game_drawdown', evo.TYPE_FLOAT, sel=[-50000,-20000,-10000,-5000, -2000, -1000, -500, -100, -50]))
proto.add_variable(evo.variable('r2_game_drawdown', evo.TYPE_FLOAT, sel=[-50000,-20000,-10000,-5000, -2000, -1000, -500, -100, -50]))
#proto.add_variable(evo.variable('r2_threshold', evo.TYPE_FLOAT, sel=[0.5, 1.0, 1.1, 1.2, 1.5, 2.0, 3.0]))


def fitness_individual(a):

    seed = random.randint(0,24)
    seed = 5

    data = pickle.load(open('data/data_sample_%d' % seed, 'rb'))

    s = bt.stacker()

    s.mpi = 25
    s.num_levels = 300
    s.skip_mask = 1 #a['skip_mask'] 
 
    s.ask_replenish_edge = 1 # a['replenish_edge']
    s.ask_replenish_size = 1 
    s.ask_replenish = False 
    s.ask_ut_type = bt.linear
    s.ask_ut_param_1 = 0.01 # a['ut_param_1']
    s.ask_ut_param_2 = 1.0
 
    s.bid_replenish_edge = 1 #a['replenish_edge']
    s.bid_replenish_size = 1
    s.bid_replenish = False
    s.bid_ut_type = bt.linear
    s.bid_ut_param_1 = 0.01 #a['ut_param_1']
    s.bid_ut_param_2 = 1.0

    use_r2                     =  True

    pnl_target                 =  a['pnl_target']
    pnl_target_thres           =  1 # a['pnl_target_thres']
    use_dynamic_pnl_target     =  False #a['use_dynamic_pnl_target']
    dynamic_pnl_target_slope   =  0#a['dynamic_pnl_target_slope']

    r2_pnl_target                 =  a['r2_pnl_target']
    r2_pnl_target_thres           =  1 # a['r2_pnl_target_thres']
    r2_use_dynamic_pnl_target     =  False #a['r2_use_dynamic_pnl_target']
    r2_dynamic_pnl_target_slope   =  0#a['r2_dynamic_pnl_target_slope']

    max_time_to_start_game     =  28000
    max_drawdown               =  -10000000000 
    max_prize                  =  100000000000 #a['max_prize'] 
    dynamic_replenish_coef     =  0.0 # a['dynamic_replenish_coef']
    use_dynamic_replenish_size =  False #a['use_dynamic_replenish_size']
    
    game_drawdown              =  a['game_drawdown']   
    r2_game_drawdown           =  a['r2_game_drawdown']
    r2_threshold               =  2 #a['r2_threshold']
 
    res = bt.multiday_bt(s, 
                         data,
                         pnl_target=pnl_target, 
                         pnl_target_thres=pnl_target_thres, 
                         use_dynamic_pnl_target=use_dynamic_pnl_target, 
                         dynamic_pnl_target_slope=dynamic_pnl_target_slope,
                         
                         r2_pnl_target=r2_pnl_target, 
                         r2_pnl_target_thres=r2_pnl_target_thres, 
                         r2_use_dynamic_pnl_target=r2_use_dynamic_pnl_target, 
                         r2_dynamic_pnl_target_slope=r2_dynamic_pnl_target_slope,

                         max_time_to_start_game=max_time_to_start_game,
                         max_drawdown=max_drawdown,
                         max_prize=max_prize,
                         dynamic_replenish_coef = dynamic_replenish_coef,
                         use_dynamic_replenish_size=use_dynamic_replenish_size,
                         game_drawdown = game_drawdown,
                         r2_game_drawdown = r2_game_drawdown,
                         r2_threshold = r2_threshold,
                         use_r2 = use_r2,
                        )
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

    risk_ratio2 = 16.0 * ( np.mean(pnls) / np.std(pnls)) 

    p = risk_ratio2
        
    results['individual'] = a
    results['fitness'] = p
    results['total_pnl'] = total_pnl
    results['total_vol'] = total_vol
    results['total_mean_pos'] = total_mean_pos    
    results['file'] = out_file

    k = 1.0 / float(len(results['dates']))
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
        res = pickle.load(open('output_%d_%d' % (gen, i)))
        a['fitness'] = res['fitness']
        d.append(a)
        i += 1     
    gen += 1
    return d

def fitness_individual_wrapper(a):
    global out_file
    out_file = a.replace('input','output')
    indiv = pickle.load(open(a, 'rb'))
    r = fitness_individual(indiv)


def run_evolver():
    ev = evo.evolver(proto, fitness_parallel, population_size=32, selection_size=8, mutation_prob=0.5, fresh_blood_ratio=0.3)
    ev.run_n_generations(50) 

if __name__ == "__main__":

    #print sys.argv

    if len(sys.argv) == 1:
        run_evolver()
    else:
        fitness_individual_wrapper(sys.argv[1])
