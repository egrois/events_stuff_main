import xml.etree.ElementTree as et

def get_single_fields(example='/tmp/ttt'):
    f= open(example)
    lines = f.readlines()

    single_fields = set([])
    repeating_fields = set([])

    for line in lines:
        toks=line.split('\1')
        keys = {}
        for tok in toks:
            try:
                key = int(tok.split('=')[0])
            except:
                continue
            if keys.has_key(key):
                keys[key] += 1
            else:
                keys[key] = 1
        for key in keys:
            if keys[key] == 1:
                single_fields.add(key)
            else:
                repeating_fields.add(key)
    return sorted(single_fields - repeating_fields)

single = set([  15, 22, 35, 48, 55, 120, 167, 200, 201, 202, 207, 231, 305, 309, 311, 461, 
                462, 555, 562, 711, 731, 762, 779, 864, 870, 947, 969, 980, 996, 1140, 
                1141, 1142, 1143, 1146, 1147, 1148, 1149, 1150, 1151, 1180, 1234, 1300, 1435, 
                5770, 5791, 5792, 5796, 5799, 5818, 5819, 5849, 6350, 6937, 9779, 9787, 9800, 
                9850, 37702, 37703])


def get_secdef_fields(single_fields, file_name = '/mnt/raw/cme/futures/2015/09/20150924/Production/Templates/templates_FixBinary.xml'):
    tree = et.parse(file_name)
    root = tree.getroot()
    fields = {}
    for child in root:
        if child.attrib.has_key('name') and child.attrib['name'] == 'MDInstrumentDefinitionFuture27':
            for ch in child:
                if int(ch.attrib['id']) in single_fields:
                    f = {}
                    f['name'] = ch.attrib['name']
                    f['id'] = ch.attrib['id']
                    fields[ch.attrib['name']] = f
    return fields


fields = get_secdef_fields(single)

for fn in fields:
    print '    char *', fields[fn]['name'] + ";"

for fn in fields:
    print '    bool has_%s;'% fields[fn]['name']

for fn in fields:
    print '    has_%s = false;'% fields[fn]['name']

for fn in fields:
    print '   if(msg.has_%s == true) os << " %s=" << msg.%s;'% (
        fields[fn]['name'], 
        fields[fn]['name'], 
        fields[fn]['name']) 


for fn in fields:
    print 'case %s:' % fields[fn]['id'] 
    print '%s = value;' % fields[fn]['name']
    print 'has_%s = true;' % fields[fn]['name']

