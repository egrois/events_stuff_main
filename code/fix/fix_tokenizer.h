#ifndef ELEMENTS_FIX_UTILITIES_H
#define ELEMENTS_FIX_UTILITIES_H

namespace elements
{

template <typename handler >
class fix_tokenizer
{
public:

    fix_tokenizer(handler *h) : _h(h) {}
    ~fix_tokenizer(){}

    bool parse( char * s )
    {
        _h->on_msg_start();
        char * begin = s;
        char * end = s;
        char * key = s;
        char * value= 0;
     
        while( *end != '\n' &&  *end != '\0' && (end - s) < MAX_LENGTH )
        {
            if( *end == '=' )
            {
                *end = '\0';
                value = end + 1;
            }
            else if( *end == SOH )
            {
                *end = '\0';
                _h->on_msg_field( atoi(key), value);
                key = end + 1;
            }
            ++end;
        }
        _h->on_msg_end();
    }
    
private:
    char SOH = 1;
    int MAX_LENGTH = 10000;
    handler *_h;
};

};
#endif //ELEMENTS_FIX_UTILITIES_H
