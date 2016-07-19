#ifndef ELEMENTS_ELOOP_H
#define ELEMENTS_ELOOP_H

#include <list>
#include <string>
#include <parameters_manager.h>
#include <timers_manager.h>
#include <feed.h>
#include <easy_time.h>
#include <oob_logger.h>
#include <log_messages.h>
#include <tsc.h>
#include <secdef.h>
#include <instrument.h>
#include <cme_fill_simulator.h>
#include <order_entry_manager.h>
#include <bb_manager.h>
#include <cme_order_manager.h>
#include <bt_events_manager.h>

namespace elements
{

template <typename ParamMsg, typename Handler>
class eloop 
{
public:
    
    eloop() {}

    ~eloop() { delete _oob_logger; }

    std::list<instrument_feed_config> make_instrument_feed_config()
    {
        std::list<instrument_feed_config> configs;
        int num = _parameters_adaptor.get_parameters().strategy().instrument_size(); 
        for(int i = 0; i < num; ++i)
        {
            auto ins =_parameters_adaptor.get_parameters().strategy().instrument(i); 
            instrument_feed_config config;
            config.symbol = ins.symbol();
            config.bt_input = ins.bt_input();
            config.bt_input_type = ins.bt_input_type();
            config.bt_channel = ins.bt_channel(); 
            configs.push_back(config);
        }
        return configs;
    }

    std::string get_date()
    {
        auto params =  _parameters_adaptor.get_parameters().strategy(); 
        return params.date();
    } 

    bool initialize( const std::string & config, Handler &h ) 
    {
        log("INFO", "initializing event loop");

        h.on_before_eloop_initialize();

        // PARAMS
        if( !_parameters_adaptor.init(config, &h, 1.0e9)) 
        {
            log("ERROR", "cannot initialize parameters");       
            return false;
        }
        log("INFO","successfully initialized parameters from file " + config);     
        log("INFO", _parameters_adaptor.msg_to_str(true));
        log("INFO", "\n" + _parameters_adaptor.msg_to_str(false));
        h.set_parameters_manager(&_parameters_adaptor);

        auto params =  _parameters_adaptor.get_parameters().strategy();     
        _is_realtime = params.is_realtime();
        
        std::list<instrument_feed_config> instrument_configs = make_instrument_feed_config();
        
        // AFFINITY
        if(params.eloop_cpu_affinity() >= 0 && _is_realtime )
        {
            cpu_set_t mask;
            CPU_ZERO(&mask);
            CPU_SET((unsigned int)params.eloop_cpu_affinity(), &mask);  
            sched_setaffinity(0,sizeof(mask), &mask);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
        log("INFO","locked eloop to processor ");     
        
        // OOB_LOGGER
        if( params.enable_oob_log())
        {
            _oob_logger =  new oob_logger<log_message>( 
                params.oob_logger_file_prefix() + "." + get_date(), 
                100000, 
                params.oob_logger_affinity());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
        
        // SECDEF
        std::set<std::string> symbols;
        for(auto config : instrument_configs)
            symbols.insert( config.symbol);
        cme::secdef_manager secdef_man;
        std::list<cme::SecurityDefinition> secdefs = secdef_man.get_secdefs(
                symbols, 
                _is_realtime, 
                get_date(),
                params.secdef_root(),
                params.secdef_format(),
                instrument_configs,
                params.cme_channel_config(),
                params.cme_channel_side());
 
        for(auto secdef : secdefs)
        {
            int num = _parameters_adaptor.get_parameters().strategy().instrument_size(); 
            for(int i = 0; i < num; ++i)
            {
                auto ins =_parameters_adaptor.get_parameters().strategy().instrument(i); 
                if(ins.symbol() == secdef.SecurityDesc)
                {
                    _instruments_map[secdef.SecurityID] = _instruments.size(); 
                    _symbol_map[secdef.SecurityDesc] = secdef.SecurityID; 
                    _instruments.push_back(
                        new instrument(
                            secdef, 
                            ins.apply_trades(),
                            ins.maintain_implied(),
                            ins.dollars_per_tick(),
                            i,
                            ins.pos_offset(),
                            ins.cash_offset()));
                    log("INFO", "configured " + secdef.SecurityDesc); 
                    std::cout << secdef << std::endl;
                }             
            }
        }

        // Extract events from parameters
        process_eco_events();


        // Any extra feeds?
        std::vector<feed*> additional_feeds;
        
        if(_parameters_adaptor.get_parameters().strategy().use_bt_events()
                && !_parameters_adaptor.get_parameters().strategy().use_bb() 
                && _eco_events.size() > 0)
        {
            _bt_events_adaptor = new bt_events_manager<Handler>(&h, _parameters_adaptor.get_parameters().strategy().bt_events_file(), 
                _eco_events, get_date());
            additional_feeds.push_back(_bt_events_adaptor);
        } 
 
        // FEED 
        if( !_feed_adaptor.init(_is_realtime,
                                get_date(),
                                instrument_configs, 
                                &h, 
                                params.cme_channel_config(),
                                params.cme_channel_side(),
                                params.net_interface(),
                                params.cme_feed_server_active(),
                                params.cme_md_config(),
                                secdefs,
                                additional_feeds))
        {
            log("ERROR", "cannot initialize feed manager");       
            return false;
        }
        h.set_feed_manager(&_feed_adaptor);

        // TIMERS
        if( !_timers_adaptor.init(&h, this))
        {
            log("ERROR", "cannot initialize timers");       
            return false;
        } 
        log("INFO","successfully initialized timers manager");     
        h.set_timers_manager(&_timers_adaptor);


        std::cout << "trying to init order manager" << std::endl;

        // CONFIGURE ORDER MANAGER 
        if(_is_realtime && params.cme_om_active())
        {
            try
            {
                _cme_om_adaptor = new cme_order_manager(&h, &_instruments, params.cme_om_config(), _oob_logger);  
            }
            catch(std::logic_error e)
            {
                std::cout << "cannot initialize order manager " << e.what()  << std::endl;
                assert(false);
            }
        }

        h.set_order_manager(_cme_om_adaptor);
        log("INFO","successfully initialized order manager");     

        // CME_FILL_SIMULATOR
        if(!_is_realtime || (params.fs_active() && !params.cme_om_active()) )
        {
            _bt_cme_adaptor.install_handler(&h);
            _bt_cme_adaptor.set_delays(params.fs_delay1(), params.fs_delay2());
            
            if(!_is_realtime)
                _feed_adaptor.add_feed(&_bt_cme_adaptor);
            
            h.set_order_manager(&_bt_cme_adaptor);
            _bt_cme_adaptor.install_handler(&h);
            _bt_cme_adaptor.set_instruments(&_instruments); 
            if(_is_realtime)
                _bt_cme_adaptor.set_realtime();
            _bt_cme_adaptor.init();
        }

        // ORDER ENTRY
        if(_is_realtime)
        {
            _order_entry_adaptor.init(params.oe_file(), h);
        }


        // BLOOMBERG MANAGER - not working in bt yet
        if(_is_realtime && _parameters_adaptor.get_parameters().strategy().use_bb() && _eco_events.size() > 0)
        {
            auto & params =  _parameters_adaptor.get_parameters().strategy();
            _bb_adaptor = new bb_manager<Handler>(
                        params.bb_affinity(), 
                        // params.bb_fields_file(),
                        // params.bb_subscription_file(),
                        _eco_events,
                        &h,
                        params.bb_ip(),
                        params.bb_port(),
                        params.bb_app_name(),
                        _oob_logger);
        }
        
        // ELOOP 
        h.set_event_loop(this);
        h.on_after_eloop_initialize();

        log("INFO","finished initializing event loop");
    }

    void run()
    {
        while(true)
        {
            auto & params =  _parameters_adaptor.get_parameters().strategy();
            if(_terminate_flag == true)
            {
                log("INFO","exiting event loop");
                break;
            } 
 
            auto t10 = _tsc();
            if(_is_realtime && params.cme_om_active())
            {
                // log("INFO","before OMS");     
                _cme_om_adaptor->poll();
                // log("INFO","after OMS");     
            }
            else if(_is_realtime && (params.fs_active() && !params.cme_om_active()))
            {
                _bt_cme_adaptor.poll();
            }
            //else if( !_is_realtime && params.fs_active() )
            //{
            //    _bt_cme_adaptor.poll();
            //:}
            
            auto t0 = _tsc();
            _feed_adaptor.poll();
            auto t1 = _tsc();
            if(_is_realtime) 
                _parameters_adaptor.poll();
            auto t2 = _tsc();   
            _timers_adaptor.poll();
            auto t3 = _tsc();
      
            if(params.use_bb() && _is_realtime && _eco_events.size() > 0 )
            {
                _bb_adaptor->poll(); 
            }
            else if( params.use_bt_events() && _eco_events.size() > 0 )
            {
                _bt_events_adaptor->poll(); 
            }
 
            if(_is_realtime)
                _order_entry_adaptor.poll();           
            
            if( params.enable_oob_log() && params.log_timing() )
            {
                auto ts = now();
                auto epoch = to_epoch_nanos(ts);

                _log_msg.type = MSG_TYPE::MSG_LATENCY;
                _log_msg.uber.a_latency.epoch = epoch;
                _log_msg.uber.a_latency.nanos = _tsc.nanos(t0,t1);
                _log_msg.uber.a_latency.type = LATENCY_TYPE::LT_FEED;
                _oob_logger->log(_log_msg);

                _log_msg.type = MSG_TYPE::MSG_LATENCY;
                _log_msg.uber.a_latency.epoch = epoch;
                _log_msg.uber.a_latency.nanos = _tsc.nanos(t1,t2);
                _log_msg.uber.a_latency.type = LATENCY_TYPE::LT_PARAMS;
                _oob_logger->log(_log_msg);
               

                 _log_msg.type = MSG_TYPE::MSG_LATENCY;
                 _log_msg.uber.a_latency.epoch = epoch;
                 _log_msg.uber.a_latency.nanos = _tsc.nanos(t2,t3);
                 _log_msg.uber.a_latency.type = LATENCY_TYPE::LT_TIMERS;
                 _oob_logger->log(_log_msg);
                 
                 _log_msg.type = MSG_TYPE::MSG_LATENCY;
                 _log_msg.uber.a_latency.epoch = epoch;
                 _log_msg.uber.a_latency.nanos = _tsc.nanos(t10,t0);
                 _log_msg.uber.a_latency.type = LATENCY_TYPE::LT_CME_OMS;
                 _oob_logger->log(_log_msg);
            }
            std::this_thread::sleep_for(std::chrono::nanoseconds(10));
        }
    }

    template <typename T>
    void log(std::string prefix, T msg)
    {
        std::cout << str(now()) << " " << prefix << " "<< std::string(msg) << std::endl;
    }

    timestamp now() 
    {
        if(_is_realtime )
            return clock::now(); 
        else
            return _feed_adaptor.now();
    }

    timestamp next()
    {
        if(_is_realtime )
            return clock::now(); 
        else
            return _feed_adaptor.next();
    } 

    void set_now(timestamp const &ts)
    {
        if(!_is_realtime )
            _feed_adaptor.set_now(ts);
    }

    std::vector<instrument *> & instruments() { return _instruments; }
    std::map<int32_t,int32_t> * instruments_map() { return &_instruments_map; }
    std::map<std::string,int32_t> * symbol_map() { return &_symbol_map; }

    void terminate() { _terminate_flag = true; } 

    oob_logger<log_message> * get_oob_logger(){ return _oob_logger; }

    void process_eco_events()
    {
        auto &param = _parameters_adaptor.get_parameters().strategy();
        int num = param.eco_event_size(); 
        for(int i = 0; i < num; ++i)
        {
            auto ev = param.eco_event(i); 
            eco_event event;
            event.ts = make_time( get_date(), ev.time());
            event.event_id = ev.eco_event_id();
            
            int num_comp = ev.eco_component_size();
            for(int j = 0; j < num_comp; ++j)
            {
                eco_component comp;
                auto co = ev.eco_component(j);
                comp.bb_name = co.bb_name(); 
                comp.bb_field = co.bb_field();
                comp.critical = co.critical();
                event.components.push_back(comp); 
            }            
            _eco_events.push_back(event);
        }
    }

private:

    bool _is_realtime = true;
    bool _terminate_flag = false;       

    parameters_manager<ParamMsg,Handler>                _parameters_adaptor;
    timers_manager<Handler, eloop<ParamMsg,Handler> >   _timers_adaptor;
    feed_manager<Handler>                               _feed_adaptor;
    cme_fill_simulator                                  _bt_cme_adaptor;
    oob_logger<log_message> *                           _oob_logger = nullptr;
    log_message                                         _log_msg;
    TSC                                                 _tsc; 
    std::vector<instrument *>                           _instruments;
    std::map<int32_t, int32_t>                          _instruments_map;
    std::map<std::string, int32_t>                      _symbol_map;
    order_entry_manager<Handler>                        _order_entry_adaptor;
    bb_manager<Handler>*                                _bb_adaptor = nullptr;
    bt_events_manager<Handler>*                         _bt_events_adaptor = nullptr;
    std::vector<eco_event>                              _eco_events;
    cme_order_manager                                  *_cme_om_adaptor = nullptr;
};

}

#endif // ELEMENTS_ELOOP_H
