import sys
sys.path.append('/home/pgrosul/git/code/pybt')
import pybt
reload(pybt)
from datetime import datetime,time, timedelta
import utils
reload(utils)
import glob

SYMBOL = sys.argv[1]

file_name='/local/disk1/cme_derived/%s/%s.secdef.' % (SYMBOL,SYMBOL)
start = datetime(2011,6,24)
end=datetime(2013,4,1)
workdays = utils.get_workdays(start, end)
good = None
for dt in workdays:
    dtstr = dt.strftime('%Y%m%d')
    ff = file_name + dtstr
    if len(glob.glob(ff)) > 0:
        good = ff
    elif good is not None:
        print 'cp', good, ff
        #subprocess.call('cp',good,ff)
