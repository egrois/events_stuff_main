import evolver as evo
import numpy as np
import pickle


a = {}

a['pnl_target'] = [30,35,40]
a['dynamic_pnl_target_slope'] = [  0.01, 0.015, 0.02]
a['max_pos'] = [1000, 2000]
a['vol_quadratic_alpha'] =[ 100, 300, 500, 1000.0, 2000, 4000 ]
a['vol_power'] = [ 2, 4 ]

J = np.zeros(len(a), dtype=int)
JJ = np.zeros(len(a), dtype=int)

cnt = 1
j = 0
for key  in a:
    JJ[j] = cnt	
    cnt *= len(a[key])
    j += 1

for i in xrange(cnt):
    j = 0 
    aa = {}
    for key in a:
	aa[key] =  a[key][J[j]]
	if ( i + 1 ) % JJ[j] == 0:
	    J[j] += 1
	    J[j] %= len(a[key])
	j += 1
    fn = 'input_0_%d' % i 
    pickle.dump(aa, open(fn, 'wb'))
    print './fitness.sh ', fn
