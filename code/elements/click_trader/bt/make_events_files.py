code_root = "/home/pgrosul/git/code/"

import sys
sys.path.append(code_root + '/pybt')
import pybt
reload(pybt)
from datetime import datetime,time, timedelta
import utils
reload(utils)
import glob
sys.path.append(code_root + '/elements/snapgen')
import params_pb2
from google.protobuf import text_format
import copy

import numpy as np
import pandas as pd

store = pd.HDFStore('/home/pgrosul/bt_data/payroll_rdf.h5','r')
rdf = store['df']
store.close()

survey = utils.read_survey('PAYROLL')

for dt in survey.DATE.unique():
    su = survey[survey.DATE == dt]
    f = open( 'params/events.%s' % dt, 'w')

    tt = rdf[rdf.DATE == dt]
    dt = datetime.strptime('08:30:00', '%H:%M:%S')
    td = timedelta(milliseconds = (tt['true_bin'].values[0] - 18000) *100 )
    dt += td
    str_dt = dt.strftime('%H:%M:%S.%f')

    f.write('<TS=%s>\n' % str_dt)
    for tm in su.TIME.unique():
        ss = su[su.TIME == tm]
        for ticker in ss.TICKER.unique():
            tt = ticker
            if (tt.count('YOY') > 0) or (tt.count('MOM') > 0 ):
                tt +='%'
            tt +=' Index'
           
            ttt = ss[ss.TICKER == ticker]
 
            field = 'LAST_PRICE'
            value = ttt.ACTUAL_RELEASE.values[0]           
            f.write('%s,%s,%s\n' % (tt, field, value) )

            value = ttt.REVISION_TOT.values[0]           
            if not np.isnan(value):
                field = 'ASK'
                value = ttt.REVISION_TOT.values[0]           
                f.write('%s,%s,%s\n' % (tt, field, value) )

    f.close()
