import argparse
from copy import deepcopy

TYPES=set([ 'double', 'int', 'char', 'bool', 'int64_t'])

parser = argparse.ArgumentParser(description='msg compiler')
parser.add_argument('-i', '--input', action='append')
parser.add_argument('-n','--namespace')
parser.add_argument('-o', '--output')
parser.add_argument('-p', '--printer')
args = parser.parse_args()

have_printer = set([])
printer = open(args.printer)
for line in printer.readlines():
    if line.count('operator')> 0:
        have_printer.add(line.replace('const', '@').replace('&', '@').split('@')[3].strip())

defs = []
for input in args.input:
    with open (input, "r") as f:
        defs.append(f.read().replace('\n', ''))
defs = ' '.join(defs)
defs = defs.replace(',', ' ')
defs = defs.replace('=', ' ')
defs = defs.replace(';', ' ')
defs = defs.replace('{', ' ')
defs = defs.replace('}', ' ')
defs = defs.replace(' [', '[')
defs = defs.replace('] ', ']')
defs = defs.replace(' ]', ']')
defs = defs.replace('[ ', '[')

defs=defs.split()
defs.append('enum')

enums = {}
structs ={} 

current_enum = None
current_struct = None

order = 0
for tok in defs:
    if tok == 'enum':
   
        if current_enum is not None:
            if current_enum['state'] == 'ok':
                enums[current_enum['name']] = deepcopy(current_enum)
            else:
                raise "ERROR: starting new enum not finished old enum"
        elif current_struct is not None:
            if current_struct['state'] == 'ok':
                structs[current_struct['name']] = deepcopy(current_struct)
            else:
                raise "ERROR: starting new enum not finished old struct"
        
        current_enum = {'state': 'name', 'vars': [] , 'current_var' : None }
        current_struct = None
 
    elif tok == 'struct':
   
        if current_struct is not None:
            if current_struct['state'] == 'ok':
                structs[current_struct['name']] = deepcopy(current_struct)
            else:
                raise "ERROR: starting new struct not finished old struct"
        elif current_enum is not None:
            if current_enum['state'] == 'ok':
                enums[current_enum['name']] = deepcopy(current_enum)
            else:
                raise "ERROR: starting new struct not finished old enum"
        
        current_struct = {'state': 'name', 'vars':  [] , 'current_var' : None }
        current_enum = None

    elif current_enum is not None:
        if current_enum['state'] == 'name':
            current_enum['name'] = tok
            current_enum['state'] = 'ok'
        elif current_enum['state'] == 'ok':
            current_enum['current_var'] = tok
            current_enum['state'] = 'partial'
        elif current_enum['state'] == 'partial':
            current_enum['vars'].append( ( current_enum['current_var'], tok ) ) 
            current_enum['current_var'] = None
            current_enum['state'] = 'ok'
 
    elif current_struct is not None:
        if current_struct['state'] == 'name':
            current_struct['name'] = tok
            current_struct['state'] = 'ok'
        elif current_struct['state'] == 'ok':
            current_struct['current_var'] = tok
            current_struct['state'] = 'partial'
        elif current_struct['state'] == 'partial':
            current_struct['vars'].append( ( current_struct['current_var'], tok ) ) 
            current_struct['current_var'] = None
            current_struct['state'] = 'ok'

# VERIFY



ff = open(args.output, 'w')
ff.write('#ifndef  %s_MESSAGES_H\n' % args.namespace.upper() )
ff.write('#define  %s_MESSAGES_H\n' % args.namespace.upper() )
ff.write('\n')
ff.write('#include <iosfwd>\n')
ff.write('namespace %s\n' % args.namespace )
ff.write('{\n')

tab = '    '
for s in enums:
    enum = enums[s]
    ff.write('%senum %s\n' % (tab, enum['name']) )
    ff.write('%s{\n' % tab )
    toks = []
    for var in enum['vars']:
        toks.append('%s%s%s = %s' % (tab, tab, var[0], var[1]))
    ff.write(',\n'.join(toks))
    ff.write('\n')
    ff.write('%s};\n' % tab)
    ff.write('\n') 

    ff.write('%sstd::string %s_to_string(%s var)\n' % (tab, enum['name'], enum['name']))
    ff.write('%s{\n' % tab)
    ff.write('%s%sswitch(var)\n' % (tab,tab))
    ff.write('%s%s{\n' % (tab,tab))
    
    for var in enum['vars']:
        ff.write('%s%scase %s:\n' % (tab, tab, var[0]))
        ff.write('%s%s%sreturn "%s";\n' % (tab, tab, tab, var[0]))
    ff.write('%s%sdefault:\n' % (tab, tab))
    ff.write('%s%s%sreturn "%s";\n' % (tab, tab, tab, 'NA'))
    ff.write('%s%s}\n' % (tab,tab))
    ff.write('%s}\n'% tab)
ff.write('\n') 

ff.write('%senum MSG_TYPE\n' % tab)
ff.write('%s{\n' % tab)
toks = []
i = 0
for s in structs:
    toks.append( '%s%sMSG_%s = %d' % (tab,tab, s.upper(), i) )
    i += 1
ff.write( ',\n'.join(toks) )
ff.write('\n')
ff.write('%s};\n' % tab)
ff.write('\n')

ff.write('%sstruct message_base\n' % tab)
ff.write('%s{\n' % tab )
ff.write('%s%suint64_t epoch;\n' % (tab,tab))
ff.write('\n')
ff.write('%s};\n' % tab)
ff.write('\n') 

for s in structs:
    struct = structs[s]
    ff.write('%sstruct %s : public message_base\n' % (tab, struct['name']))
    ff.write('%s{\n' % tab)
    toks = []
    for var in struct['vars']:
        toks.append('%s%s%s %s;' % (tab, tab, var[0], var[1]))
    ff.write('\n'.join(toks))
    ff.write('\n') 
    ff.write('%s};\n' % tab)
    ff.write('\n') 

    if s in have_printer:
        continue
 
    ff.write('%sstd::ostream& operator <<(std::ostream& os, const %s & msg)\n' % (tab, s) )
    ff.write('%s{\n'%tab)
    ff.write('%s%sreturn  os << str(from_epoch_nanos(msg.epoch)) << " MSG_%s "' % (tab,tab, s.upper()))
    for var in struct['vars']:
        if var[1].count('[') > 0 and var[0] != 'char':
            cnt = int(var[1].split('[')[1].split(']')[0]) 
            nname = var[1].split('[')[0]
            for i in xrange(cnt):
                ff.write(' << " %s[%d]=" << msg.%s[%d] ' % ( nname,i, nname, i ) )
        elif var[0] == 'char':
            nname = var[1].split('[')[0]
            ff.write(' << " %s=" << msg.%s ' % ( nname, nname ))
        else:
            if var[0] in TYPES:
                ff.write(' << " %s=" << msg.%s ' % ( var[1], var[1] ) )
            else:
                ff.write(' << " %s=" << %s_to_string(msg.%s) ' % ( var[1],var[0],var[1] ) )
    ff.write(';\n')
    ff.write('%s}\n'%tab)
    ff.write('\n')

ff.write('%sstruct log_message\n' % tab)
ff.write('%s{\n' % tab)

ff.write('%s%sMSG_TYPE type;\n' % (tab,tab))
ff.write('%s%sunion\n' % (tab,tab) )
ff.write('%s%s{\n' % (tab,tab))
for s in structs:
    ff.write('%s%s%s%s a_%s;\n' % (tab,tab,tab, s, s))
ff.write('%s%s} uber;\n' % (tab, tab))
ff.write('%s};\n' % tab)
ff.write('%s\n' % tab)

ff.write('#include <%s>\n\n' % args.printer)

ff.write('%sstd::ostream& operator <<(std::ostream& os, const log_message & msg)\n' % tab )
ff.write('%s{\n' % tab)
ff.write('%s%sswitch(msg.type)\n' % (tab,tab))
ff.write('%s%s{\n' % (tab, tab))

for s in structs:
    ff.write('%s%scase MSG_TYPE::%s:\n' % (tab,tab, 'MSG_' + s.upper()))
    ff.write('%s%s%sos << msg.uber.a_%s << std::endl;\n' % (tab,tab,tab,s))
    ff.write('%s%s%sbreak;\n' % (tab,tab,tab))
ff.write('%s%s}\n' % (tab, tab))
ff.write('%s}\n' % tab)

ff.write('}\n')
ff.write('#endif // %s_MESSAGES_H\n' % args.namespace.upper() )
