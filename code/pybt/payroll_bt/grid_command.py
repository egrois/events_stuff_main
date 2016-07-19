import numpy as np
import pickle

a = {}

a['root'] = ['ZB']
a['entry_time'] = [ 2, 3, 4, 5, 10 ]
a['exit_time'] = [100,200,300,400,500,1000,2000,3000,4000,5000,10000]
a['go_thres'] = [0,20]
a['size_mult'] =[5,10,20]

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
    fn = 'input_0_%d' % (i +  1000) 
    pickle.dump(aa, open(fn, 'wb'))
    print './fitness.sh ', fn
