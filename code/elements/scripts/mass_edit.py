from google.protobuf import text_format
import params_pb2
import sys
import time


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


def read_config(change_config):
    f = open(change_config, 'r')
    lines = f.readlines()
    config = []
    for line in lines:
        ii = line.strip().split(',')
        if len(ii[0])>0:
            config.append(ii)   
    return config


def boolify(s):
    if s == 'True':
        return True
    if s == 'False':
        return False
    raise ValueError("huh?")

def autoconvert(s):
    for fn in (boolify, int, float):
        try:
            return fn(s)
        except ValueError:
            pass
    return s


def apply_change(conf, msg):
    for field in msg.strategy.ListFields():
        if field[0].name == 'instrument':
            for ins in field[1]:
                if ins.symbol == conf[0]:
                    i = 1
                    item = ins
                    while i < len(conf) - 2:
                        try:
                            item = item.__getattribute__(conf[i])
                        except:
                            item = item[0].__getattribute__(conf[i])
                        i += 1
                    item.__setattr__( conf[len(conf) -2], autoconvert(conf[len(conf) -1]))

if __name__ == "__main__":
    param_file = 'click_trader.param'
    msg = load_params(param_file)
    config = read_config(sys.argv[1]) 
    for c in config:
        if c[0] == 'Commit':
            save_params(param_file, msg)
            print 'sleeping'
            time.sleep(1)
        else:
            print c
            apply_change(c, msg)
    save_params(param_file, msg)
                




