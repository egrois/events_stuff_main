import xml.etree.ElementTree as et
import argparse
import copy

class fix_field:
    def __init__(self):
        self.number = None
        self.name = None
        self.type = None
        self.enum = None

class fix_component:
    def __init__(self):
        self.is_group = False
        self.group_name = None
        self.group_required = False
        self.name = None
        self.fields = []
        self.enum = None

class fix_message:
    def __init__(self):
        self.name = None
        self.msgtype = None
        self.fields = []

class fix_header:
    def __init__(self):
        self.fields = []
        self.name = 'Header'

class fix_trailer:
    def __init__(self):
        self.fields = []
        self.name = 'Trailer'

def get_fields(root):
    fields = []
    for child in root:
        if child.tag != 'fields':
            continue
        for ch in child:
            if ch.tag == 'field':
                f = fix_field()
                f.name = ch.attrib['name']
                f.number = int(ch.attrib['number'])
                f.type = ch.attrib['type']
                f.enum = {}
                for c in ch:
                    f.enum[c.attrib['enum']] = c.attrib['description']
                fields.append(f)
    return fields

def get_components(root):
    components = []
    for child in root:
        if child.tag != 'components':
            continue
        for ch in child:
            co = fix_component()
            co.name = ch.attrib['name']
            if len(ch) == 1 and ch[0].tag == 'group':
                co.is_group = True
                co.group_name = ch[0].attrib['name']
                co.group_required = (ch[0].attrib['required'] == 'Y')
                for c in ch[0]:
                    co.fields.append((c.attrib['name'],c.attrib['required']=='Y' ))
            else:
                for c in ch:
                    co.fields.append((c.attrib['name'],c.attrib['required']=='Y' ))
            components.append(co)

        i = 0
        for comp in components:
            comp.enum = i
            i += 1

        return components

def get_messages(root):
    messages = []
    for child in root:
        if child.tag != 'messages':
            continue
        for ch in child:
            m = fix_message()
            m.name = ch.attrib['name']
            m.msgtype = ch.attrib['msgtype']
            for c in ch:
                m.fields.append((c.attrib['name'],c.attrib['required']=='Y' ))
            messages.append(m)
    return messages

def get_header(root):
    h = fix_header()
    for child in root:
        if child.tag == 'header':
            for ch in child:
                h.fields.append((ch.attrib['name'],ch.attrib['required']=='Y' ))
    return h

def get_trailer(root):
    h = fix_trailer()
    for child in root:
        if child.tag == 'trailer':
            for ch in child:
                h.fields.append((ch.attrib['name'],ch.attrib['required']=='Y' ))
    return h

class compiler:

    def __init__(self, input='/home/pgrosul/git/code/fix/FIX50CME.xml', output='/home/pgrosul/git/code/fix/ascii_fix_parser.h', template = '/home/pgrosul/git/code/fix/fix_parser_template.h'):
        self.input = input
        self.output =  output
        self.template =  template
        
        tree = et.parse(input)
        root = tree.getroot()
        
        self.messages = get_messages(root)
        self.fields = get_fields(root)
        self.components = get_components(root)

        self.order_components()
        
        self.header = get_header(root)
        self.trailer = get_trailer(root) 
        
        self.message_map = {}
        for i in xrange(len(self.messages)):
            self.message_map[self.messages[i].msgtype] = i
        
        self.field_map = {}
        for i in xrange(len(self.fields)):
            self.field_map[self.fields[i].number] = i
        
        self.field_name_type_map = {}
        self.field_name_number_map = {}
        for i in xrange(len(self.fields)):
            self.field_name_type_map[self.fields[i].name] = self.fields[i].type
            self.field_name_number_map[self.fields[i].name] = self.fields[i].number

        self.component_names = set([])
        self.comp_map = {}
        i = 0
        for comp in self.components:
            self.component_names.add(comp.name)
            self.comp_map[comp.name] = i
            comp.enum = i
            i += 1

        self.compile()

    def get_simple_fields(self,msg):
        fff = []
        for f in msg.fields:
            if self.field_name_number_map.has_key(f[0]):
                fff.append(self.field_name_number_map[f[0]])
        return sorted(list(set(fff)))
    
    def get_possible_fields(self,msg):
        fff = []
        for f in msg.fields:
            if self.field_name_number_map.has_key(f[0]):
                fff.append(self.field_name_number_map[f[0]])
            else:
                comp = self.components[self.comp_map[f[0]]]
                if comp.is_group:
                    fff.append(self.field_name_number_map[comp.group_name])
                fff.extend(self.get_possible_fields(self.components[self.comp_map[f[0]]]))
        return sorted(list(set(fff)))

    def msg_type_to_number(self, t):
        assert(len(t)<3)
        if len(t) == 1:
            return ord(t)
        elif len(t) == 2:
            return ord(t[0]) + ord(t[1]) * 1000

    def order_components(self):

        field_set = set([])
        for field in self.fields:
            field_set.add(field.name)
        field_list = list(field_set)

        dep = {}
        for comp in self.components:
            dep[comp.name] = []
            for field in comp.fields:
                if field[0] in field_set:
                    continue
                dep[comp.name].append(field[0])

        satisfied_list = []
        satisfied_set = set([])
        num = len(satisfied_set)
        while( True ):
            for key in dep.keys():
                if key in satisfied_set:
                    continue
                has_deps = True
                for val in dep[key]:
                    if val not in satisfied_set:
                        has_deps = False
                if has_deps:
                    satisfied_list.append(key)
                    satisfied_set.add(key)
            if len(satisfied_set) == num:
                break
            else:
                num = len(satisfied_set)
        assert(len(satisfied_set) == len(dep))

        components2 = []
        for e in satisfied_list:
            for ee in self.components:
                if ee.name == e:
                    components2.append(ee)
        assert(len(components2) == len(self.components))    
        self.components = components2
    
    def convert_to_c_type(self, field):
        if not self.field_name_type_map.has_key(field):
            if field in self.component_names:
                comp = None
                for c in self.components:
                    if c.name == field:
                        comp = c
                        break
                if comp.is_group:
                    return 'std::vector<%s_Type>' % field     
                else:
                    return "%s_Type" % field
            else:
                return 'char *'
        else:
            return 'char *'
   
    def print_set_field(self, f, msg):
        f.write('virtual bool set_field(int key, char * value)\n')
        f.write( "{\n") 
        f.write( "switch(key)\n") 
        f.write( "{\n") 
        for id in self.get_simple_fields(msg):
            field_name = self.fields[self.field_map[id]].name
            f.write( "case %s:\n" %  id) 
            f.write('if(has_%s == true) return false;\n' %  field_name) 
            f.write( "%s = value;\n" %  field_name) 
            f.write('has_%s = true;\n' % field_name) 
            f.write('return true;\n') 
        f.write('default:\n')
        f.write('assert(false);\n')
        f.write('return false;\n')
        f.write( "}\n") 
        f.write( "};\n") 

    def get_field_to_comp_map(self, msg):
        field_to_comp = copy.deepcopy({})
        for field in msg.fields:
            if self.comp_map.has_key(field[0]):
                comp = self.components[self.comp_map[field[0]]]
                if comp.is_group == True:
                    continue 
                for ff in self.get_simple_fields(comp):
                    field_to_comp[ff] = comp.enum 
                    #print msg.name, comp.name, ff
        return field_to_comp 

    def get_field_to_group_comp_map(self, msg):
        field_to_comp2 = copy.deepcopy({})
        for field in msg.fields:
            if self.comp_map.has_key(field[0]):
                comp_id = self.comp_map[field[0]]
                comp = self.components[comp_id]
                if comp.is_group == True:
                    field_to_comp2[self.field_name_number_map[ comp.group_name]] = comp_id
        return field_to_comp2 
    
    def get_group_count_fields(self, msg):
        fields = copy.deepcopy([])
        for field in msg.fields:
            if self.comp_map.has_key(field[0]):
                comp = self.components[self.comp_map[field[0]]]
                if comp.is_group == True:
                    fields.append(self.field_name_number_map[ comp.group_name])
        return fields
    
    def print_msg_def(self, f, msg, is_component=False):
        if is_component:
            f.write( "struct %s_Type : public msg_base\n" %  msg.name ) 
        else:
            f.write( "struct %s : public msg_base\n" %  msg.name ) 
        f.write( "{\n") 
        for field in msg.fields:
            f.write('%-40s %s;\n' % (self.convert_to_c_type(field[0]), field[0]))            
        for field in msg.fields:
            f.write('bool has_%s;\n'% field[0])            
        f.write('virtual void reset()\n')
        f.write('{\n')
        for field in msg.fields:
            f.write('has_%s = false;\n'% field[0])            
        f.write('}\n')

        f.write( "\n") 
        f.write('std::set<int> poss_fields = {%s};\n' % (  ','.join( map(str, self.get_possible_fields(msg)))))
        f.write('std::set<int> sim_fields = {%s};\n' % ( ','.join( map(str, self.get_simple_fields(msg)))))
        
        field_to_comp = self.get_field_to_comp_map(msg)
        
        aa = []
        for k in field_to_comp.keys():
            aa.append('{%d,%d}' % (k, field_to_comp[k]))
        f.write('std::map<int,int> field_to_comp = {%s};' % ','.join(aa))

        bb = [] 
        field_to_group_comp = self.get_field_to_group_comp_map(msg)
        for k in field_to_group_comp.keys():
            bb.append('{%d,%d}' % (k, field_to_group_comp[k]))
        f.write('std::map<int,int> field_to_group_comp = {%s};' % ','.join(bb))
            
        group_count_fields = self.get_group_count_fields(msg)
        f.write('std::set<int> group_count_fields = {%s};' % ','.join(map(str, group_count_fields)))

        f.write('virtual std::set<int> & possible_fields() { return poss_fields; }\n')
        f.write('virtual std::set<int> & simple_fields() { return sim_fields; }\n')
        f.write('virtual int get_component_id( int field_id ) {  assert( field_to_comp.find(field_id) != field_to_comp.end()); return field_to_comp[field_id];}\n')
        f.write('virtual int get_group_component_id( int field_id ) { assert(field_to_group_comp.find(field_id) != field_to_group_comp.end()); return field_to_group_comp[field_id]; }\n')
        f.write('virtual bool is_group_count_field( int field_id ) { return (group_count_fields.find(field_id) != group_count_fields.end()); }\n')
       
        f.write('virtual void prepare_group(int key, char * value)\n')
        f.write('{\n')
        f.write('grp_field_id = key;\n') 
        f.write('int group_count = atoi(value);\n')
        f.write('assert(group_count > 0);\n')
        f.write('assert(group_count < 100);\n')
        f.write('switch(grp_field_id)\n')
        f.write('{')
        for field_id in field_to_group_comp.keys():
            f.write('case %s:\n' % field_id)
            comp_id = field_to_group_comp[field_id]
            assert(comp_id < len(self.components))
            comp_name = self.components[comp_id].name
            f.write('%s.resize(group_count);\n' % comp_name) 
            f.write('is_grp = true;\n')
            f.write('grp_index = 0;\n')
            f.write('num_in_grp = group_count;\n')
            f.write('group = &%s[0];\n' % comp_name);           
            f.write('break;\n') 
        f.write('default:\n')
        #f.write('std::cout  << " message=%s" << " field_id=" << grp_field_id << std::endl;\n' % msg.name)
        f.write('assert(false);\n')
        f.write('}')        
        f.write( '}\n')



        f.write('virtual bool  advance_group()\n')
        f.write('{\n')
        f.write('if(grp_field_id < 0) return false;\n')
        f.write('switch(grp_field_id)\n')
        f.write('{')
        for field_id in field_to_group_comp.keys():
            f.write('case %s:\n' % field_id)
            comp_id = field_to_group_comp[field_id]
            assert(comp_id < len(self.components))
            comp_name = self.components[comp_id].name
            f.write('++grp_index;\n')
            f.write('group = &%s[grp_index];\n' % comp_name)           
            f.write('break;\n') 
        f.write('default:\n')
        # f.write('std::cout  << " message=%s" << " field_id=" << grp_field_id << std::endl;\n' % msg.name)
        f.write('assert(false);\n')
        f.write('return true;')
        f.write('}\n')        
        f.write('}\n')

 
        f.write(' virtual void prepare_component(int key)\n')
        f.write('{\n') 
        #f.write('int id  = get_component_id(key);\n') 
        f.write('switch(key)\n')
        f.write('{\n')
        for field_id in field_to_comp.keys():
            comp = self.components[field_to_comp[field_id]]
            #print  msg.name, comp.name, field_id
            f.write('case %s:\n' % field_id )
            f.write('component = &%s;\n' % comp.name)
            f.write('break;\n') 
        f.write('default:\n')
        #f.write('std::cout  << " message=%s" << " field_id=" << grp_field_id << std::endl;\n' % msg.name)
        f.write('assert(false);\n')
        f.write('}\n')
        f.write('}\n') 

 
        self.print_set_field(f, msg)
        f.write( "\n") 

        f.write( "};\n") 
        f.write( "\n") 
        if is_component == True:
            self.make_print_operator(f, msg, type_postfix = '_Type' )   
        else:
            self.make_print_operator(f, msg)   
        f.write( "\n") 
 
    def print_msg_defs(self, f):
        self.print_msg_def(f, self.header)
        self.print_msg_def(f, self.trailer)
        for msg in self.messages:
            self.print_msg_def(f, msg)
 
    def print_comp_defs(self, f):
        for comp in self.components:
            self.print_msg_def(f, comp, is_component = True)

    def make_print_operator(self, f, msg, type_postfix=''):
        f.write('std::ostream& operator<<(std::ostream& os, const %s%s& msg)\n' % (msg.name, type_postfix ))
        f.write('{\n')
        for field in msg.fields:
            if self.field_name_number_map.has_key(field[0]):
                f.write('if(msg.has_%s) std::cout << " %s=" << msg.%s;\n' % (field[0],field[0], field[0]))
            elif self.comp_map.has_key(field[0]):
                if self.components[self.comp_map[field[0]]].is_group == True:
                    f.write('for( auto entry : msg.%s) os << " %s=" << entry;\n' % (field[0], field[0]))    
                else:
                    f.write('if(msg.has_%s) os << " %s=" << msg.%s;\n' % (field[0], field[0], field[0]))
        f.write('return os;\n')
        f.write('}\n')
   
    def print_msg_init(self,f):
        f.write( "%-40s _%s;\n" %(  'Header', 'Header') ) 
        f.write( "%-40s _%s;\n" %(  'Trailer', 'Trailer') ) 
        for msg in self.messages:
            f.write( "%-40s _%s;\n" %(  msg.name, msg.name) ) 
    
    def print_predefine(self,f): 
        for comp in self.components:
            f.write( "struct %s_Type;\n" % comp.name) 
        f.write( "struct %s;\n" %  'Header' ) 
        f.write( "struct %s;\n" %  'Trailer' ) 
        for msg in self.messages:
            f.write( "struct %s;\n" % msg.name ) 
        f.write('\n')  
 
    def print_fields_enum(self,f):
        f.write('enum FLD\n')   
        f.write('{\n')
        ff = []
        for field in self.fields:
            ff.append('%s = %d' % ( field.name, field.number ))
        f.write(',\n'.join(ff))
        f.write('\n')
        f.write('};\n')   
 
    def print_msg_enum(self,f ): 
        f.write('enum MSG\n')   
        f.write('{\n')
        ff = []
        for msg in self.messages:
            ff.append('XX%s = %d' % ( msg.name, self.msg_type_to_number(msg.msgtype)))
        f.write(',\n'.join(ff))
        f.write('\n')
        f.write('};\n')   
 
    def print_msg_type_switch(self,f ): 
        for msg in self.messages:
            f.write('case MSG::XX%s:\n' %  msg.name) 
            f.write( '_%s.reset();\n' % msg.name)
            f.write('_msg_stack.push(&_%s);\n' % msg.name)
            f.write( 'break;\n')

    def print_msg_type_switch2(self,f): 
        for msg in self.messages:
            f.write('case MSG::XX%s:\n' %  msg.name) 
            f.write('(*_h)(_Header, _%s);\n' %  msg.name)
            f.write('break;\n')
 
    def print_comp_enum(self, f): 
        enums = []
        for comp in self.components:
            enums.append('XX%s = %d' % (comp.name, comp.enum))
        f.write('\n')
        f.write('enum COMP\n')
        f.write('{\n')
        f.write(',\n'.join(enums))
        f.write('};\n')
        f.write('\n')
 
    def compile(self):
        t = open(self.template)
        template_lines = t.readlines()

        f = open(self.output, 'w')
        
        for line in template_lines:
            if line.count('@@') == 0:
                f.write(line)
            else:
                if line.count('<MSG_DEF>') == 1:
                   self.print_msg_defs(f ) 
                elif line.count('<COMP_DEF>') == 1:
                   self.print_comp_defs(f ) 
                elif line.count('<MSG_INIT>') == 1:
                   self.print_msg_init(f) 
                elif line.count('<PREDEFINE>') == 1:
                   self.print_predefine(f) 
                elif line.count('<FIELDS_ENUM>') == 1:
                   self.print_fields_enum(f ) 
                elif line.count('<MSG_ENUM>') == 1:
                   self.print_msg_enum(f) 
                elif line.count('<MSG_TYPE_SWITCH>') == 1:
                   self.print_msg_type_switch(f) 
                elif line.count('<MSG_TYPE_SW2>') == 1:
                   self.print_msg_type_switch2(f) 
                elif line.count('<COMP_ENUM>') == 1:
                   self.print_comp_enum(f) 
 
if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='fix_compiler')
    parser.add_argument('-i', '--input', default = 'FIX50CME.xml')
    parser.add_argument('-o', '--output',default = 'ascii_fix_parser.h' )
    parser.add_argument('-t', '--template', default = 'fix_parser_template.h')
    args = parser.parse_args()
    c = compiler( args.input, args.output, args.template)
    from subprocess import call
    call(["/usr/bin/astyle",args.output])
    call(["/bin/rm",args.output + '.orig'])
