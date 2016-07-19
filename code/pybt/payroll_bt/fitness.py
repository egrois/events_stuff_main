import payroll_bt as bt
import pickle
import numpy as np
import sys
import os
import random
import time
import pandas as pd


def fitness_individual(a):
    print a,

    #rdf = bt.make_rdf()
    store = pd.HDFStore('/home/pgrosul/bt_data/payroll_rdf.h5', 'r')
    rdf = store['df']

    results = bt.directional_bt(
                    rdf,
                    root = a['root'],
                    entry_time = a['entry_time'],
                    exit_time = a['exit_time'],
                    go_thres = a['go_thres'],
                    size_mult = a['size_mult'])

    pickle.dump(results, open(out_file, 'wb'))   


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

if __name__ == "__main__":
    fitness_individual_wrapper(sys.argv[1])
