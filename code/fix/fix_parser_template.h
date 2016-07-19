#ifndef ELEMENTS_FIX_PARSER_H
#define ELEMENTS_FIX_PARSER_H 

#include <assert.h>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <cstring>
#include <fix_tokenizer.h> 
#include <iosfwd>

namespace elements
{

int MsgTypeToInt(std::string &s)
{
    assert(s.length() < 3);
    if( s.length() == 1 )
        return int(s[0]);
    else if ( s.length() == 2 )
        return 1000 * int(s[0]) + int(s[1]);
}

int MsgTypeToInt(const char * s)
{
    int len = strlen(s);
    assert( len < 3);
    if( len == 1 )
        return int(s[0]);
    else if ( len == 2 )
        return 1000 * int(s[0]) + int(s[1]);
}

enum FIELD_TYPE
{
    SIMPLE_FIELD = 1,
    UNKNOWN_FIELD = 2,
    GROUP_COUNT_FIELD =3,
    DUPLICATE_FIELD = 4,
    COMPONENT_FIELD = 5
};

struct msg_base
{
    
    FIELD_TYPE on_msg_field(int key, char * value)
    {
        if(!is_possible_field(key))
        {
            //std::cout << "UNKNOWN "  << key<< std::endl;
            return FIELD_TYPE::UNKNOWN_FIELD;
        }

        if(is_simple_field(key))
        {
            if(set_field(key, value))
            {
                //std::cout << "SIMPLE "<< key << std::endl;
                return FIELD_TYPE::SIMPLE_FIELD;
            }
            else
            {
                //std::cout << "DUPLICATE " << key << std::endl;
                return FIELD_TYPE::DUPLICATE_FIELD;
            }
        }


        if(is_group_count_field(key))
        {
            //std::cout << "GROUP_COUNT " << key << std::endl;
            prepare_group(key, value);
            return FIELD_TYPE::GROUP_COUNT_FIELD;
        }
       
        //std::cout << "COMPONENT "<< key << std::endl;
        prepare_component(key); 
        return FIELD_TYPE::COMPONENT_FIELD;
    }

    bool is_possible_field( int key )
    {
        auto a = possible_fields();
        if(a.find(key) != a.end())
            return true;
        else
            return false;
    }

    bool is_simple_field( int key )
    {
        auto a = simple_fields();
        if(a.find(key) != a.end())
            return true;
        else
            return false;
    }

    virtual int get_component_id(int field_id ) = 0;
    virtual int get_group_component_id(int field_id ) = 0;
    virtual bool set_field(int key, char *value) = 0;
    virtual std::set<int> & possible_fields() = 0;
    virtual std::set<int> & simple_fields() = 0;
    virtual bool is_group_count_field(int) = 0;

    virtual void prepare_group(int key, char * value) = 0;
    virtual void prepare_component(int key) = 0;
    virtual bool advance_group() = 0;
    virtual void reset() = 0;

    msg_base * get_group() { return group; }
    msg_base * get_component() { return component; }

    bool is_grp = false; 
    int grp_index = 0;
    int num_in_grp; 
    int grp_field_id = -1;

    msg_base * group = nullptr;
    msg_base * component = nullptr;
};

// @@INSERT <FIELDS_ENUM>
// @@INSERT <MSG_ENUM>
// @@INSERT <COMP_ENUM>

// @@INSERT <COMP_DEF>
// @@INSERT <MSG_DEF>

template< typename handler >
class fix_parser
{
public:
    
    fix_parser( handler *h): _h(h)
    {
        _tok =  new fix_tokenizer<fix_parser<handler> >(this);
    }

    fix_parser()
    {
        delete _tok;
    }

    bool parse(char * s)
    {
        return _tok->parse(s);
    }

    void on_msg_start()
    {
        _Header.reset();
        _msg_stack.push(&_Header);
    }
 
    void on_msg_field( int key, char * value )
    {
        //std::cout << "processing " << key << "=" << value << std::endl;


        while(true) 
        {
            switch(_msg_stack.top()->on_msg_field(key, value))
            {
                case FIELD_TYPE::SIMPLE_FIELD:
                    //std::cout << "stack size "<<_msg_stack.size()<< std::endl;
                    return; 

                case FIELD_TYPE::UNKNOWN_FIELD:
                    if(_msg_stack.top() == &_Header)
                    {
                        _msg_stack.pop();
                        switch( MsgTypeToInt(_Header.MsgType) )
                        {
                            // @@INSERT <MSG_TYPE_SWITCH>
                        }
                    }
                    else if(_msg_stack.size() == 1)
                    {

                        
                        if(_msg_stack.top() == &_Trailer )
                        {
                            //std::cout << "Ignoring Unknown field " << key  << std::endl;
                            return; 
                        }
                        _msg_stack.pop();
                        _Trailer.reset();
                        _msg_stack.push(&_Trailer);
                    }
                    else if(_msg_stack.size() > 1)
                    {
                        _msg_stack.pop();
                    }
                    break;

                case FIELD_TYPE::GROUP_COUNT_FIELD:
                    _msg_stack.push(_msg_stack.top()->get_group());
                    _msg_stack.top()->reset();
                    //std::cout << "stack size "<<_msg_stack.size()<< std::endl;
                    return;

                case FIELD_TYPE::DUPLICATE_FIELD:
                    _msg_stack.pop();
                    while (!_msg_stack.top()->advance_group() && _msg_stack.size() > 1) {
                        _msg_stack.pop();
                        //std::cout << "stack size "<<_msg_stack.size()<< std::endl;
                    }
                    _msg_stack.push(_msg_stack.top()->get_group());
                    _msg_stack.top()->reset();
                    break;
                
                case FIELD_TYPE::COMPONENT_FIELD:
                    _msg_stack.push(_msg_stack.top()->get_component());
                    _msg_stack.top()->reset();
                    break;
 
                default:
                    assert(false);
            };

            //std::cout << "stack size "<<_msg_stack.size()<< std::endl;
        }
    }
        
    void on_msg_end()
    {
        switch( MsgTypeToInt(_Header.MsgType) )
        {
            // @@INSERT <MSG_TYPE_SW2>
        }
        while(!_msg_stack.empty())
            _msg_stack.pop();
    }

private:

    handler *_h;
    fix_tokenizer<fix_parser<handler> > * _tok;
    std::stack<msg_base *> _msg_stack;

    // @@INSERT <MSG_INIT>
};

}

#endif // ELEMENTS_FIX_PARSER
