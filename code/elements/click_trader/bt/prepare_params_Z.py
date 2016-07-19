code_root = "/home/pgrosul/git/code"

import sys
sys.path.append(code_root + '/pybt')
import pybt
reload(pybt)
from datetime import datetime,time, timedelta
import utils
reload(utils)
import glob
sys.path.append(code_root + '/elements/click_trader')
import params_pb2
from google.protobuf import text_format
import copy

import numpy as np
import pandas as pd

components = ( 
       ("AHE MOM% Index", "LAST_PRICE"),
       ("AWH TOTL Index", "LAST_PRICE"),
       ("USEMNCHG Index", "LAST_PRICE"),
       ("USMMMNCH Index", "LAST_PRICE"),
       ("NFP TCH Index",  "LAST_PRICE"),
       ("NFP PCH Index",  "LAST_PRICE"),
       ("NFP TCH Index",  "ASK"),
       ("USUDMAER Index", "LAST_PRICE"),
       ("USURTOT Index",  "LAST_PRICE"))

class symbol_vars:

    def __init__(self, root, date):
        self.root = root
        self.ticker = utils.get_front_Z_symbol(datetime.strptime(str(dt), '%Y%m%d'), root=root)
        
        # TODO
        self.beta = 1.0
        self.max_position  = 100
        self.penetration = 1
        self.stop_loss = 5
        self.stop_gain = 5

def load_params(file_name):
    f = open(file_name, 'r')
    lines = f.readlines()
    msg = params_pb2.Params()
    msg = text_format.Merge(" ".join(lines),msg )
    f.close()
    return msg

def save_params(file_name,msg):
    f = open(file_name, 'w')
    s = text_format.MessageToString(msg)
    f.write(s)
    f.close()

def make_params( dt):
    vars = [] 
    vars.append(symbol_vars('ZT', dt))
    vars.append(symbol_vars('ZF', dt))
    vars.append(symbol_vars('ZN', dt))
    vars.append(symbol_vars('ZB', dt))
    params = load_params('Z.template')

    params.strategy.date = str(dt)

    for i in xrange(4):
        root = params.strategy.instrument[i].symbol[:2]
        params.strategy.instrument[i].symbol = utils.get_front_Z_symbol(datetime.strptime(str(dt), '%Y%m%d'), root=root)
        
        if dt < 20151200:
            params.strategy.instrument[i].bt_input = '/local/disk1/cme_derived/' 
            params.strategy.instrument[i].bt_input_type = 'cme_ascii' 

    if dt < 20151200:
        params.strategy.secdef_format = 'ascii_hist'
        params.strategy.secdef_root = '/local/disk1/cme_derived/'

    params.strategy.eco_event.add()
    params.strategy.eco_event[0].__setattr__('time', '08:30:00.000')   
    params.strategy.eco_event[0].__setattr__('eco_event_id', 'Payrolls_%s' % dt)   
    i = 0
    for comp in components:    
        params.strategy.eco_event[0].eco_component.add()
        params.strategy.eco_event[0].eco_component[i].__setattr__('bb_name', comp[0])
        params.strategy.eco_event[0].eco_component[i].__setattr__('bb_field',comp[1])
        params.strategy.eco_event[0].eco_component[i].__setattr__('critical', True)
        i += 1

    params.strategy.event_trader.add()
    params.strategy.event_trader[0].__setattr__('event_trader_id','PAYROLLS_Z')
    params.strategy.event_trader[0].__setattr__('event_id','Payrolls_%s' % dt)

    i = 0
    for v in vars:
        params.strategy.event_trader[0].event_trader_instrument.add()
        params.strategy.event_trader[0].event_trader_instrument[i].__setattr__('ticker',v.ticker)
        params.strategy.event_trader[0].event_trader_instrument[i].__setattr__('beta',v.beta)
        params.strategy.event_trader[0].event_trader_instrument[i].__setattr__('max_position',v.max_position)
        params.strategy.event_trader[0].event_trader_instrument[i].__setattr__('penetration',v.penetration)
        params.strategy.event_trader[0].event_trader_instrument[i].__setattr__('stop_loss',v.stop_loss)
        params.strategy.event_trader[0].event_trader_instrument[i].__setattr__('stop_gain',v.stop_gain)
        i += 1
    
    save_params('params/Z.%s' % dt, params)


store = pd.HDFStore('/home/pgrosul/bt_data/payroll_rdf.h5','r')
rdf = store['df']
store.close()

for dt in rdf.DATE.unique():
    make_params(dt)

