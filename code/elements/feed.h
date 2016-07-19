#ifndef ELEMENTS_FEED_H
#define ELEMENTS_FEED_H

#include <ascii_fix_parser.h>
#include <cme_sbe_parser.h>
#include <easy_time.h>
#include <vector>
#include <list>
#include <cme_channel_manager.h>
#include <linux_event_loop.h>
#include <unordered_map>
#include <tsc.h>
#include <log_messages.h>
#include <ClientManager.h>
#include <macros.h>
#include <cme_server_message_normalizer.h>


namespace elements
{

class feed
{
public:
    virtual bool peek_time(timestamp &ts) = 0;
    virtual bool poll() = 0;     
};

class feed_dispatcher
{
public:

    feed_dispatcher() { }
    virtual ~feed_dispatcher() { }

    void add_feed(feed * f){ _feeds.push_back(f); }
   
    int peek_time(timestamp &ts)
    {
        timestamp ts_min(hours(24 * 365 * 10000));
        int best_index = -1;

        for( int i = 0; i < _feeds.size(); ++i )
        {
            if(!_feeds[i]->peek_time(ts))
                continue; 

            if( ts < ts_min )
            {
                ts_min = ts;
                best_index = i;
            }
        }
        ts = ts_min;
        return best_index;
    }
 
    bool poll()
    {
        if(_is_realtime)
	    {
            for(int i = 0; i< _feeds.size(); ++i)
                 _feeds[i]->poll();
            return true;
	    }
	    else
	    {
            int best_index = peek_time(_next);
 
            if( best_index < 0)
                return true;
            _now  = _next;
            _feeds[best_index]->poll();
            assert(peek_time(_next) >= 0);
            return true;
	    }
    }

    timestamp const & now(){ return _now; }
    timestamp const & next(){ return _next; }

    void set_now(timestamp const & ts) 
    {
        assert(ts <= _next);
        _now = ts;
    }
    
protected:

    void make_current()
    {
        assert(peek_time(_next) >= 0);
        _now = _next - nanoseconds(1);
    }

    std::vector<feed*> _feeds;
    timestamp _next, _now;
    bool _is_realtime = false;
};

template <typename Handler>
class ascii_cme_feed : public feed
{
public:
    bool init(std::string file_name, Handler *handler)
    {
        _parser = new fix_parser<Handler>(handler); // leak ok
        char cmd[200];
        sprintf(cmd, "/bin/gunzip -c %s", file_name.c_str());
        _in = popen(cmd, "r");
        if(!_in)
        {
            std::cout << "cannot open " << file_name << std::endl;
            return false;
        }
        else
        {
            std::cout << "opened " << file_name << std::endl;
        }
        _buf = (char *) malloc(_len); // leak ok
        return true;
    }
    
    virtual bool peek_time(timestamp &ts)
    {
        if(_buf_valid)
        {
            ts = _current_ts;
            return true;
        }

        if(getline(&_buf, &_len, _in) <= 0)
        {
            assert(false);
            return false;
        }
        _buf_valid = true;
            
        for( int i = 0; i < strlen(_buf) - 19 ; ++i)
        {
            if ( _buf[i] == '5' && _buf[i + 1] == '2' && _buf[i + 2 ] == '=')
            {
                ts = _current_ts = from_fix_time(&_buf[i + 3]);
                return true;
            }
        }
        assert(false);
        return false;
    }

    virtual bool poll()
    {
        if(_buf_valid)
        {
            _parser->parse(_buf);        
        }
        else
        {
            if(getline(&_buf, &_len, _in) > 0)
                _parser->parse(_buf);        
        }
        _buf_valid = false;
        return true;
    } 

protected:
    FILE * _in = nullptr;
    char *_buf = nullptr;
    size_t _len = 10000;
    bool _buf_valid = false;
    timestamp _current_ts;
    fix_parser<Handler> * _parser;
}; 

template <typename Handler>
class sbe_cme_pcap_feed : public feed
{

struct pcap_hdr 
{
    uint32_t magic_number;   /* magic number */
    uint16_t version_major;  /* major version number */
    uint16_t version_minor;  /* minor version number */
    int32_t  thiszone;       /* GMT to local correction */
    uint32_t sigfigs;        /* accuracy of timestamps */
    uint32_t snaplen;        /* max length of captured packets, in octets */
    uint32_t network;        /* data link type */
} __attribute__((packed));

struct pcaprec_hdr 
{
    uint32_t sec;
    uint32_t usec; 
    uint32_t incl_len;       /* number of octets of packet saved in file */
    uint32_t orig_len;       /* actual length of packet */
} __attribute__((packed));

public:
    bool init(std::string file_name, Handler *handler)
    {
        _parser = new cme_sbe_parser<Handler>(*handler); // leak
       
        char cmd[200];

        sprintf(cmd, "/usr/bin/bzip2 -d -c %s", file_name.c_str());
        _in = popen(cmd, "re");

        //_in = fopen(file_name.c_str(), "r");

        if(!_in) 
        {
            std::cout << "cannot open " << file_name << std::endl;
            return false;
        }
        else
        {
            std::cout << "opened " << file_name << std::endl;
        }

        if( fread(&_gh, sizeof(_gh), 1, _in) == 0)
        {
            std::cout << "cannot read header" << std::endl;
            return false;
        }

        return true;
    }

    bool read_next_packet()
    {
        if( fread(&_rh, sizeof(_rh), 1, _in) == 0)
        {
            std::cout << "cannot read record header" << std::endl;
            return false;
        }
        
        if(_rh.incl_len != _rh.orig_len)
        {
            std::cout << "record size mismatch inc_len=" << _rh.incl_len << " orig_len=" << _rh.orig_len   << " time="<< str(_current_ts) << std::endl;
            return false; 
        }        

        if( fread(&_buf, _rh.incl_len, 1, _in) == 0)
        {
            std::cout  << "cannot read packet" << strerror(errno)<< std::endl;
            assert(false);
            return false;
        }
 
        _ts.tv_sec = _rh.sec; 
        _ts.tv_nsec = _rh.usec;

        auto old = _current_ts;

        _current_ts = from_timespec(_ts);

        assert(_current_ts > old);
 
        return true;
    }

    virtual bool peek_time(timestamp &ts)
    {
        if(_buf_valid)
        {
            ts = _current_ts;
            return true;
        }

        if(!read_next_packet())
        {
            assert(false);
            return false;
        }
        
        _buf_valid = true;
   
        ts = _current_ts;
        return true;
    }

    virtual bool poll()
    {
        if(_buf_valid)
        {
            _parser->parse(_buf, _rh.orig_len, _ts, 46);
        }
        else
        {
            if(read_next_packet())
            {
                _parser->parse(_buf, _rh.orig_len, _ts, 46);
            }
            else
            {
                return false;
            }
        }
        _buf_valid = false;
        return true;
    } 

protected:
    Handler                 *_handler;
    cme_sbe_parser<Handler> *_parser;
    pcap_hdr                _gh;
    pcaprec_hdr             _rh;
    FILE                    *_in;
    char                    _buf[10000];
    bool _buf_valid         = false;
    timestamp               _current_ts;
    timespec                _ts;
};

template <typename Handler>
class sbe_cme_live_feed : public feed
{
public:

    sbe_cme_live_feed(){}

    virtual ~sbe_cme_live_feed()
    {
        delete _loop;
    }

    bool init( std::set<cme_connection_info> info, Handler *handler, int interface)
    {
        _handler = handler;
        _loop = new linux_event_loop<sbe_cme_live_feed<Handler> > (this);
        for(auto con : info)
        {
            int fd = _loop->add_udp(con.ip, con.port, 20000000, true, true, interface);
            if(fd < 0) return false;  
            assert(_parsers.find(fd) == _parsers.end());
            _parsers[fd] = new cme_sbe_parser<Handler> (*handler); 
        }
        return true;
    }

    virtual bool peek_time(timestamp &ts)
    {
        ts = _ts;
        return false;
    }

    virtual bool poll()
    {
        //auto t0 = _tsc();
        //_loop->set_oob_logger(_handler->get_oob_logger());
        //_loop->set_params(&_handler->get_params_manager()->get_parameters());
        _loop->poll();
        //auto t1 = _tsc();

       /* auto param =  _handler->get_params_manager()->get_parameters().strategy();
        if( true && param.enable_oob_log() && param.log_timing() )
        { 
            auto _oob_logger = _handler->get_oob_logger();
            _log_msg.type = MSG_TYPE::MSG_LATENCY;
            _log_msg.uber.a_latency.epoch = to_epoch_nanos(_ts);
            _log_msg.uber.a_latency.nanos = _tsc.nanos(t0,t1);
            _log_msg.uber.a_latency.type = LATENCY_TYPE::LT_LX_POLL;
            _oob_logger->log(_log_msg);
        }*/

        return true;
    }

    void on_udp_packet(int fd, const char * buf, int size, timestamp ts)
    {
        if(ts > _ts)
            _ts = ts;
    
        //auto t0 = _tsc();
        _parsers[fd]->parse(buf, size, ts, 0);
        //auto t1 = _tsc();

       /* auto param =  _handler->get_params_manager()->get_parameters().strategy();
        if( true && param.enable_oob_log() && param.log_timing() )
        { 
            auto _oob_logger = _handler->get_oob_logger();
            _log_msg.type = MSG_TYPE::MSG_LATENCY;
            _log_msg.uber.a_latency.epoch = to_epoch_nanos(ts);
            _log_msg.uber.a_latency.nanos = _tsc.nanos(t0,t1);
            _log_msg.uber.a_latency.type = LATENCY_TYPE::LT_PARSE;
            _oob_logger->log(_log_msg);
        }*/
    }

    void on_tcp_packet(int fd, const char * buf, int size, timestamp ts){}
    void on_timer(int fd, char *buf, int size, timestamp ts){} 
    void register_linux_event_loop(linux_event_loop<sbe_cme_live_feed<Handler> > *p){}   
    void on_disconnect(int fd){}
    
protected:
    Handler                                             *_handler;
    linux_event_loop<sbe_cme_live_feed<Handler> >       *_loop;
    timestamp                                           _ts;
    std::unordered_map<int, cme_sbe_parser<Handler> * >   _parsers;
    log_message                                         _log_msg;
    TSC                                                 _tsc; 
};


template <typename Handler>
class cme_server_live_feed : public feed
{
public:
    cme_server_live_feed(){}
    ~cme_server_live_feed(){}    

    bool init( std::string conf, std::list<cme::SecurityDefinition> & secdefs, Handler * handler )
    {
        _normalizer = new cme_server_message_normalizer<Handler>(handler);

        std::string CLIENT_CONF_FILE(conf);
        //starks::ClientManager& mgr = starks::ClientManager::getInstance(CLIENT_CONF_FILE);
        starks::ClientManager& mgr = starks::ClientManager::getInstance();
        mgr.init(CLIENT_CONF_FILE, true);

        std::set<int64_t> ids;
        for(auto e: secdefs)
            ids.insert(e.SecurityID);
            
        SecurityDefinitionIteratorPtr sdIt = mgr.getSecurityDefinitionIterator();
        if (sdIt == NULL)
        {
            std::cout << "init: cannot initialize cme feed from server secdef" << std::endl;
            return false;
        }
        
        sdIt->pause();
        SecurityDefinitionPtr sdMsg = nullptr;
        while ( (sdMsg = sdIt->nextMessage()) != nullptr && ids.size() != 0)
        {                    
            cmeMdp::MDInstrumentDefinitionFuture27 *p = &(sdMsg->mDInstrumentDefinitionFuture27);
            auto it = ids.find(p->_SecurityID);
            if(it != ids.end())
            ids.erase(it);
        }
        sdIt->resume();

        if(ids.size() > 0 )
        {
            std::cout << "cannot find all the secdefs" << std::endl;
            for (auto e: ids)
                std::cout << e << std::endl;
            assert(false);
        }

        _irIt = mgr.getIncrementalRefreshIterator();
        if( _irIt == nullptr )
        {
            std::cout << "init: cannot initialize cme feed from server" << std::endl;
            return false;
        }

        flush();

        return true;
    }
    
    virtual bool peek_time(timestamp &ts)
    {
        ts = _ts;
        return false;
    }
    
    virtual bool poll()
    {

        //auto t0 = clock::now();

        //auto t0 = _tsc();
        //std::cout << "before pause "  << str(clock::now()) << std::endl; 
        bool ret = _irIt->pause();
        //std::cout << "pause " << ret <<" " << str(clock::now()) << std::endl;
        //int i = 0;
        while( (_irMsg = _irIt->nextMessage()) != nullptr)
        {
            cmeMdp::Message *msg = (cmeMdp::Message*) &(_irMsg->metadata);
            _ts = from_epoch_nanos( msg->_recvTime );
            _normalizer->convert(_irMsg);
        //    ++i;
        }
        //std::this_thread::sleep_for(std::chrono::seconds(1)); 
       
        //std::cout << "before resume " << str(clock::now()) << std::endl; 
        ret = _irIt->resume();
        //std::cout << "resume " << ret << " " << str(clock::now())  << std::endl;
        // auto t1 = clock::now();

        // std::cout << str(t1) << " " << str(t0) << std::endl;

        //std::cout << str(_ts) << " " << str(clock::now()) << std::endl;

        //auto t1 = _tsc();
        //if(i > 0)
        //    std::cout << "latency " <<  _tsc.nanos(t0,t1)  << std::endl;
        return true;
    }
    
    void flush()
    {
        std::cout << "ENTERING FLUSH" << std::endl;
        auto ret = _irIt->pause();
        std::cout << "after pause " << ret << std::endl;
        while( (_irMsg = _irIt->nextMessage()) != nullptr){}
        std::cout << "before resume" << std::endl;
        ret = _irIt->resume();
        std::cout << "EXITING FLUSH " << ret << std::endl;
    }


protected:
    Handler                                             *_handler;
    timestamp                                           _ts;
    log_message                                         _log_msg;
    TSC                                                 _tsc; 
    IncrementalRefreshIteratorPtr                       _irIt = nullptr;
    IncrementalRefreshPtr                               _irMsg = nullptr;
    cme_server_message_normalizer<Handler>             *_normalizer; 
};

struct instrument_feed_config
{
    std::string symbol;
    std::string bt_input;
    std::string bt_input_type;
    std::string bt_channel;
};

template <typename Handler>
class feed_manager : public feed_dispatcher
{
public:
    feed_manager(){}
    virtual ~feed_manager()
    {
        for(auto f : _feeds) delete f;
    }

    cme_channel_manager * channel_manager()
    {
        if(_channel_manager == nullptr)
        {
            std::string year = _date.substr(0, 4);
            std::string month = _date.substr(4, 2);
            std::string channel_file = _channel_file_root + "/" + year + "/" + month+ "/" 
                            + _date + "/Production/Configuration/config.xml";
            _channel_manager = new cme_channel_manager(channel_file);
        }
        return _channel_manager;
    }

    bool init(  bool is_realtime, 
                std::string date, 
                const std::list<instrument_feed_config> & configs, 
                Handler *h, 
                std::string channel_file_root, 
                std::string channel_side,
		        int interface,
                bool cme_feed_server_active,
                std::string cme_md_config,
                std::list<cme::SecurityDefinition> & secdefs,
                std::vector<feed *> additional_feeds)
    {
        _is_realtime = is_realtime;
        _channel_file_root = channel_file_root;
        _date = date;
        _cme_feed_server_active = cme_feed_server_active;
        _cme_md_config = cme_md_config;


        if(!_is_realtime)
        {
            std::set<std::string> subscribed;

            for(auto conf : configs )
            {
                std::string sroot = conf.symbol.substr(0, conf.symbol.length() - 2);
                std::string year = date.substr(0, 4);
                std::string month = date.substr(4, 2);

                if( conf.bt_input_type == "cme_ascii" )
                {
                    std::string full_path = conf.bt_input + sroot + "/" + sroot + "." + date + ".gz";
                    
                    std::cout << full_path << std::endl;
                    
                    if(subscribed.find(full_path) == subscribed.end())
                    {
                        ascii_cme_feed<Handler> * f = new ascii_cme_feed<Handler> ();
                        if(!f->init(full_path, h))
                        {
                            std::cout << "Cannot initialize feed from file " << full_path << std::endl;
                            return false;
                        }
                        add_feed(f); 
                        subscribed.insert(full_path);
                    } 
                }
                else if( conf.bt_input_type == "sbe_cme_pcap" )
                {
                    int channel = channel_manager()->get_channel(sroot); 
                    std::stringstream ss;
                    ss << channel; 
                    std::string chstr = ss.str();

                    std::string full_path 
                        =   conf.bt_input + "/" + year + "/" + month + "/" + date + 
                            "/cme_inc_" + chstr + "I" + conf.bt_channel + chstr + "_" + date + ".pcap.bz2";

                    std::cout << full_path << std::endl;

                    if(subscribed.find(full_path) == subscribed.end())
                    {
                        sbe_cme_pcap_feed<Handler> * f = new sbe_cme_pcap_feed<Handler> ();
                        if(!f->init(full_path, h))
                        {
                            std::cout << "Cannot initialize feed from file " << full_path << std::endl;
                            return false;
                        }
                        add_feed(f); 
                        subscribed.insert(full_path);
                    } 
                }
                else
                {
                    std::cout << "Unsupported feed type " << conf.bt_input_type << std::endl; 
                    return false;
                }
            }
    
            for(auto f2: additional_feeds)
                add_feed(f2);

            make_current();
            return true;
        }
        else if ( _cme_feed_server_active )
        {
            cme_server_live_feed<Handler> * f = new cme_server_live_feed<Handler> ();
            if(!f->init( _cme_md_config, secdefs, h ))
            {
                std::cout << "cannot initialize live feeds from server" << std::endl;
                return false;
            }
            add_feed(f);
            for(auto f2: additional_feeds)
                add_feed(f2);
            return true;
        }
        else
        {
            std::set<std::string> syms;
            for(auto conf : configs) 
                syms.insert(conf.symbol.substr(0, conf.symbol.length() - 2));
            
            sbe_cme_live_feed<Handler> * f = new sbe_cme_live_feed<Handler> ();
            if(!f->init( channel_manager()->get_connection_info(syms, channel_side), h, interface))
            {
                std::cout << "cannot initialize live feeds" << std::endl;
                return false;
            }
            add_feed(f);
            for(auto f2: additional_feeds)
                add_feed(f2);
            return true;
        }
    }

private:
    cme_channel_manager *_channel_manager   = nullptr;
    std::string _channel_file_root;
    std::string _date;
    bool _cme_feed_server_active = false; 
    std::string _cme_md_config;
};

}

#endif // ELEMENTS_FEED_H
