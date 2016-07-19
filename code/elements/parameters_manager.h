#ifndef ELEMENTS_PARAMETERS_MANAGER_H
#define ELEMENTS_PARAMETERS_MANAGER_H

#include <google/protobuf/text_format.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <iosfwd>
#include <sys/stat.h>
#include <tsc.h>
#include <iostream>

namespace elements
{

template <typename Msg, typename Handler>
class parameters_manager 
{
public:

    std::string file_name() { return _file_name; }
    Handler * get_handler() { return _handler; }     

    bool init( const std::string & file_name, Handler *handler, double update_interval_nanos)
    {
        _update_interval_nanos = update_interval_nanos;
        _file_name = file_name;
        _handler = handler;
        std::ifstream t(file_name);
        std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

        if(!_parser.ParseFromString(str, &_msg))
            return false;

        _last_modified = get_last_modified();
        return true;        
    }

    void print( std::ostream & os)
    {
        std::string s;
        google::protobuf::TextFormat::Printer printer;
        printer.PrintToString( _msg, &s);
        os << s << std::endl;
    }
    
    void print(Msg &msg,  std::ostream & os)
    {
        std::string s;
        google::protobuf::TextFormat::Printer printer;
        printer.PrintToString( msg, &s);
        os << s << std::endl;
    }

    std::string msg_to_str(bool single_line)
    {
        std::string s;
        google::protobuf::TextFormat::Printer printer;
        printer.SetSingleLineMode(single_line);
        printer.PrintToString( _msg, &s);
        return s;
    }

    Msg& get_parameters()
    {
        return _msg;
    }

    time_t get_last_modified()
    {
        static double timeout_ns = 1e5;
        auto t1 = _tsc();
        while( stat(_file_name.c_str(), &_file_stat) != 0)
        {
            if( _tsc.nanos(t1,_tsc() ) > timeout_ns)
                return _last_modified; 
        }
        return _file_stat.st_mtime;
    }

    void poll()
    {
        uint64_t cycles = _tsc();
        if( _tsc.nanos(_last_checked, cycles) > _update_interval_nanos || cycles < _last_checked )
        {
            _last_checked = cycles;
            check_updates();
        }
    }   

    void check_updates()
    {
        time_t last_modified = get_last_modified();

        if(last_modified == _last_modified)
        {
            return;
        }
        else
        {
            std::ifstream t(_file_name);
            std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
            
            if( !_parser.ParseFromString(str, &_msg_test) )
            {
                _last_modified = last_modified;
                std::cout << "cannot parse parameters" << std::endl;
                return; 
            }
            
            if (_handler->validate_parameters(_msg_test) )
            {
                _msg = _msg_test;
            }
            else
            {
                std::cout << "cannot validate parameters" << std::endl;
                _last_modified = last_modified;
                return;
            }
            print(_msg, std::cout);  
            _handler->on_parameters_update_raw(_msg);
            _last_modified = last_modified;
        }
    } 

private:
   
    std::string _file_name; 
    Msg _msg;
    Msg _msg_test;  
    Handler *_handler = nullptr;
    struct stat _file_stat;
    time_t _last_modified;
    google::protobuf::TextFormat::Parser _parser;
    TSC _tsc;
    uint64_t _last_checked = 0;
    double _update_interval_nanos;
};

}

#endif // ELEMENTS_PARAMETERS_MANAGER_H
