#ifndef BB_MANAGER_H
#define BB_MANAGER_H

#include <string>
#include <set>
#include <threaded_runner.h>
#include <circular_fifo.h>
#include <easy_time.h>
#include <fstream>
#include <map>
#include <tsc.h>

#include <blpapi_session.h>
#include <blpapi_eventdispatcher.h>
#include <blpapi_event.h>
#include <blpapi_message.h>
#include <blpapi_element.h>
#include <blpapi_name.h>
#include <blpapi_request.h>
#include <blpapi_subscriptionlist.h>
#include <blpapi_defs.h>
#include <blpapi_correlationid.h>

#include <eco_event.h>

namespace elements
{

enum bb_msg_type
{
    BB_SUBSCRIPTION =1,
    BB_EVENT = 2,
    BB_FAILURE = 3
};

struct bb_msg
{
    uint64_t epoch;
    bb_msg_type type;
    char symbol[40];
    bool has_actual;
    double actual;
    bool has_revised;
    double revised;
    void reset() { has_actual = has_revised = false; }
};

const BloombergLP::blpapi::Name AUTHORIZATION_SUCCESS("AuthorizationSuccess");
const BloombergLP::blpapi::Name AUTHORIZATION_FAILURE("AuthorizationFailure");
const BloombergLP::blpapi::Name TOKEN_SUCCESS("TokenGenerationSuccess");
const BloombergLP::blpapi::Name TOKEN_FAILURE("TokenGenerationFailure");
const BloombergLP::blpapi::Name TOKEN("token");
const BloombergLP::blpapi::Name EXCEPTIONS("exceptions");
const BloombergLP::blpapi::Name FIELD_ID("fieldId");
const BloombergLP::blpapi::Name REASON("reason");
const BloombergLP::blpapi::Name CATEGORY("category");
const BloombergLP::blpapi::Name DESCRIPTION("description");
const BloombergLP::blpapi::Name SlowConsumerWarning("SlowConsumerWarning");
const BloombergLP::blpapi::Name SlowConsumerWarningCleared("SlowConsumerWarningCleared");
const BloombergLP::blpapi::Name EventSubTypeName("MKTDATA_EVENT_SUBTYPE");
const char *MKTDATA_SVC = "//blp/mktdata";
const char *AUTH_SVC = "//blp/apiauth";

class bb_event_handler : public BloombergLP::blpapi::EventHandler
{
public:
    size_t getTimeStamp(char *buffer, size_t bufSize)
    {
        char buf[64], buf2[64];
        const char *fmt = "%Y/%m/%d %H:%M:%S.%%06u";
        struct timeval  tv;
        struct tm       *tm;
    
        gettimeofday(&tv, NULL);
        tm = localtime(&tv.tv_sec);
        strftime(buf, sizeof(buf), fmt, tm);
       
        return  snprintf(buffer, 64, buf, tv.tv_usec);
    }

    bool processSubscriptionStatus(const BloombergLP::blpapi::Event &event)
    {
        using namespace BloombergLP;
        using namespace blpapi;
 
        char timeBuffer[64];
        getTimeStamp(timeBuffer, sizeof(timeBuffer));

        fprintf(stdout, "Processing SUBSCRIPTION_STATUS\n");
        MessageIterator msgIter(event);
        while (msgIter.next()) 
        {
            Message msg = msgIter.message();
            std::string *topic = reinterpret_cast<std::string*>( msg.correlationId().asPointer());
            fprintf(stdout, "%s: %s - %s\n",  timeBuffer, topic->c_str(), msg.messageType().string());

            if (msg.hasElement(REASON))
            {
                // This can occur on SubscriptionFailure.
                Element reason = msg.getElement(REASON);
				std::string category;
				std::string description;
				Element tempElement;
				if (!reason.getElement(&tempElement, CATEGORY))
				{
					category = tempElement.getValueAsString();
				}

				if (!reason.getElement(&tempElement, DESCRIPTION))
				{
					description = tempElement.getValueAsString();
				}

                fprintf(stdout, "        %s: %s\n",  category.c_str(), description.c_str());
            }

            if (msg.hasElement(EXCEPTIONS))
            {
                // This can occur on SubscriptionStarted if at least
                // one field is good while the rest are bad.
                Element exceptions = msg.getElement(EXCEPTIONS);
                for (size_t i = 0; i < exceptions.numValues(); ++i)
                {
                    Element exInfo = exceptions.getValueAsElement(i);
                    Element fieldId = exInfo.getElement(FIELD_ID);
                    Element reason = exInfo.getElement(REASON);
                    fprintf(stdout, "        %s: %s\n",  fieldId.getValueAsString(), reason.getElement(CATEGORY).getValueAsString());
                }
            }
        }
        return true;
    }

    bool processSubscriptionDataEvent(const BloombergLP::blpapi::Event &event)
    {
        using namespace BloombergLP;
        using namespace blpapi;
 
        MessageIterator msgIter(event);
        while (msgIter.next()) 
        {
            Message msg = msgIter.message();
            std::string *topic = reinterpret_cast<std::string*>( msg.correlationId().asPointer());
            _msg.reset(); 
            _msg.epoch = to_epoch_nanos(clock::now());
            _msg.type = bb_msg_type::BB_EVENT; 
            strcpy(_msg.symbol, topic->c_str()); 
            int numFields = msg.asElement().numElements();
            for (int i = 0; i < numFields; ++i) 
            {
                const Element field = msg.asElement().getElement(i);

                if(strcmp(field.name().string(), "LAST_PRICE\0") == 0 && field.numValues() > 0)
                {
                    _msg.actual = field.getValueAsFloat64();
                    _msg.has_actual = true;
                }
                else if(strcmp(field.name().string(),"ASK\0") == 0 && field.numValues() > 0)
                {
                    _msg.revised = field.getValueAsFloat64();
                    _msg.has_revised = true;
                }
            }
            
            _queue->push(_msg);
        }

        return true;
    }

	void processElement(const BloombergLP::blpapi::Element &element)
	{
        using namespace BloombergLP;
        using namespace blpapi;
 
        if (element.isArray())
        {
			std::cout << "        " << element.name() << std::endl;
            // process array
            int numOfValues = element.numValues();
            for (int i = 0; i < numOfValues; ++i)
            {
                // process array data
                processElement(element.getValueAsElement(i));
            }
        }
        else if (element.numElements() > 0)
        {
            std::cout << "        " << element.name() << std::endl;
            int numOfElements = element.numElements();
            for (int i = 0; i < numOfElements; ++i)
            {
                // process child elements
                processElement(element.getElement(i));
            }
        }
        else
        {
            // Assume all values are scalar.
            fprintf(stdout, "        %s = %s\n",
                element.name().string(), element.getValueAsString());
        }
	}

    bool processMiscEvents(const BloombergLP::blpapi::Event &event)
    {
        using namespace BloombergLP;
        using namespace blpapi;
 
        char timeBuffer[64];
        getTimeStamp(timeBuffer, sizeof(timeBuffer));

        MessageIterator msgIter(event);
        while (msgIter.next()) {
            Message msg = msgIter.message();
            fprintf(stdout, "%s: %s\n",
                timeBuffer, msg.messageType().string());
        }
        return true;
    }


    bb_event_handler() {}
    virtual ~bb_event_handler() { }
    void set_queue(circular_fifo<bb_msg> *q) {_queue = q; }

    virtual bool processEvent(  const   BloombergLP::blpapi::Event&     event,  
                                        BloombergLP::blpapi::Session    *session) 
    {
        using namespace BloombergLP;
        using namespace blpapi;
 
        try {
            switch (event.eventType())
            {                
            case Event::SUBSCRIPTION_DATA:
                return processSubscriptionDataEvent(event);
                break;
            case Event::SUBSCRIPTION_STATUS:
                return processSubscriptionStatus(event);
                break;
            default:
                return processMiscEvents(event);
                break;
            }
        } catch (Exception &e) {
            fprintf(stdout, "Library Exception !!! %s\n",
                e.description().c_str());
        }
        return false; 
    }

private:
    
    circular_fifo<bb_msg>      *_queue; 
    bb_msg _msg;

};  

template <typename Handler>
class bb_manager : public threaded_runner
{
public:

    bb_manager( int affinity, 
                // std::string fields_file,
                //  std::string subscription_file,
                std::vector<eco_event> events,
                Handler *handler,
                std::string ip,
                int port,
                std::string bb_app_name,
                oob_logger<log_message> * oob_logger):
        threaded_runner(affinity),
        _oob_logger(oob_logger),
        _eco_events(events),
        _handler(handler)
    {
        using namespace BloombergLP;
        using namespace blpapi;
        
        d_hosts.push_back(ip); 
        d_port                      = port; 
        d_name                      = bb_app_name; 
        d_authOption                = "APPLICATION";

       _startup_time = clock::now(); 

        //if(!readSecurities(subscription_file))
        //{
        //    std::string err = "bb_manager: cannot read subscription file";
        //    std::cout << err << std::endl;
        //    throw std::runtime_error(err); 
        // }
        
        //if(!readFields(fields_file))
        //{
        //    std::string err = "bb_manager: cannot read fields";
        //    std::cout << err << std::endl;
        //    throw std::runtime_error(err);
        //}
        
        process_eco_events();

        if (!createSession())
        {
            std::string err = "bb_manager: cannot create session";
            std::cout << err << std::endl;
            throw std::runtime_error(err);
        }

		if (!authorize(d_session)) 
        {
            std::string err = "bb_manager: cannot authorize session";
            std::cout << err << std::endl;
            throw std::runtime_error(err);
        }
        
        for (size_t i = 0; i < d_securities.size(); ++i) 
        {
            std::cout << "subscribing for news event: " << d_securities[i] << std::endl;
            d_subscriptions.add(d_securities[i].c_str(), d_fields, d_options,
                                CorrelationId(&d_securities[i]));
        }
        std::cout << "spinning bb process" << std::endl;        
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        run(); 
    }

    virtual ~bb_manager()
    {
        delete d_eventHandler;
    }


    bool createSession() 
    {
        using namespace BloombergLP;
        using namespace blpapi;
 
		std::cout << "Connecting to port " << d_port << " on server: "; 
        for (size_t i = 0; i < d_hosts.size(); ++i) {
            d_sessionOptions.setServerAddress(d_hosts[i].c_str(), d_port, i);
			std::cout << d_hosts[i].c_str();
        }
        d_sessionOptions.setServerPort(d_port);
        d_sessionOptions.setAutoRestartOnDisconnection(true);
        d_sessionOptions.setNumStartAttempts(d_hosts.size());
		
		if (d_service.size() > 0) {
			// change subscription service
			d_sessionOptions.setDefaultSubscriptionService(d_service.c_str());
		}

		std::string authOptions = getAuthOptions();
		if (authOptions.size() > 0) {
			d_sessionOptions.setAuthenticationOptions(authOptions.c_str());
		}
        d_eventHandler = new bb_event_handler();
        d_eventHandler->set_queue(&_queue);
        d_session = new Session(d_sessionOptions, d_eventHandler);

        if (!d_session->start()) {
            fprintf(stderr, "Failed to start session\n");
            return false;
        }

        fprintf(stdout, "Connected successfully\n");

        if (!d_session->openService(MKTDATA_SVC)) {
            fprintf(stderr, "Failed to open service %s",  MKTDATA_SVC);
            d_session->stop();
            return false;
        }

        return true;
    }   

	std::string getAuthOptions()
	{
        using namespace BloombergLP;
        using namespace blpapi;
 
		std::string authOptions;
		if (!std::strcmp(d_authOption.c_str(),"APPLICATION")) { //  Authenticate application
			// Set Application Authentication Option
			authOptions = "AuthenticationMode=APPLICATION_ONLY;";
			authOptions+= "ApplicationAuthenticationType=APPNAME_AND_KEY;";
			// ApplicationName is the entry in EMRS.
			authOptions+= "ApplicationName=" + d_name;
		} else if (!strcmp(d_authOption.c_str(), "NONE")) {
			// do nothing
		} else if (!strcmp(d_authOption.c_str(), "USER_APP")) {
            // Set User and Application Authentication Option
            authOptions = "AuthenticationMode=USER_AND_APPLICATION;";
            authOptions += "AuthenticationType=OS_LOGON;";
            authOptions += "ApplicationAuthenticationType=APPNAME_AND_KEY;";
            // ApplicationName is the entry in EMRS.
            authOptions += "ApplicationName=" + d_name;
		} else if (!strcmp(d_authOption.c_str(), "DIRSVC")) {		
				// Authenticate user using active directory service property
				authOptions = "AuthenticationType=DIRECTORY_SERVICE;";
				authOptions += "DirSvcPropertyName=" + d_name;
		} else {
				// Authenticate user using windows/unix login name
				authOptions = "AuthenticationType=OS_LOGON";
		}

		std::cout << "Authentication Options = " << authOptions << std::endl;
		return authOptions;
	}

	bool authorize(BloombergLP::blpapi::Session * session)
    {
        using namespace BloombergLP;
        using namespace blpapi;
 
        EventQueue tokenEventQueue;
        session->generateToken(CorrelationId(), &tokenEventQueue);
        std::string token;
        Event event = tokenEventQueue.nextEvent();
        if (event.eventType() == Event::TOKEN_STATUS) {
            MessageIterator iter(event);
            while (iter.next()) {
                Message msg = iter.message();
                msg.print(std::cout);
                if (msg.messageType() == TOKEN_SUCCESS) {
                    token = msg.getElementAsString(TOKEN);
                }
                else if (msg.messageType() == TOKEN_FAILURE) {
                    break;
                }
            }
        }
        if (token.length() == 0) {
            std::cout << "Failed to get token" << std::endl;
            return false;
        }

        session->openService(AUTH_SVC);
		Service authService = session->getService(AUTH_SVC);
        Request authRequest = authService.createAuthorizationRequest();
        authRequest.set(TOKEN, token.c_str());

        EventQueue authQueue;
		d_identity = session->createIdentity();
        session->sendAuthorizationRequest(
            authRequest, &d_identity, CorrelationId(), &authQueue);

        while (true) {
            Event event = authQueue.nextEvent();
            if (event.eventType() == Event::RESPONSE ||
                event.eventType() == Event::REQUEST_STATUS ||
                event.eventType() == Event::PARTIAL_RESPONSE) {
                    MessageIterator msgIter(event);
                    while (msgIter.next()) {
                        Message msg = msgIter.message();
                        msg.print(std::cout);
                        if (msg.messageType() == AUTHORIZATION_SUCCESS) {
                            return true;
                        }
                        else {
                            std::cout << "Authorization failed" << std::endl;
                            return false;
                        }
                    }
            }
        }
    }

	bool readFields(std::string file)
    {
        using namespace BloombergLP;
        using namespace blpapi;
 
		bool stateFlag = false;
		int fldCount = 0;
		std::string fld;
		try {
			std::ifstream fldFile(file.c_str());
			if (fldFile) {
				d_fields.clear();
				while(getline(fldFile, fld, '\n')) {
					// process line
					d_fields.push_back(fld);
                    std::cout << fld << std::endl;
				}
			}
			fldFile.close();
			stateFlag = true;
		} catch (Exception &e) {
			fprintf(stderr, "Exception: %s\n",
				e.description().c_str());
		}
		return stateFlag;
	}

	bool readSecurities(std::string file) 
    {
        using namespace BloombergLP;
        using namespace blpapi;
 
		bool stateFlag = false;
		std::string sec;
		try {
			std::ifstream secFile(file.c_str());
			if (secFile) {
				while(getline(secFile, sec, '\n')) {
					// process line
					sec.erase(sec.find_last_not_of(" ") + 1);
					sec.erase(0, sec.find_first_not_of(" "));
					d_securities.push_back(sec);
				}
			}
			secFile.close();
			stateFlag = true;
		} catch (Exception &e) {
			fprintf(stderr, "Exception: %s\n",
				e.description().c_str());
		}
		return stateFlag;
	}

    void process_eco_events()
    {
        std::set<std::string> symbols;
        std::set<std::string> fields;

        for(auto & event : _eco_events)
        {
            for(auto & component : event.components)
            {
                symbols.insert(component.bb_name);
                fields.insert(component.bb_field);
                std::string name = component.bb_name + component.bb_field;

                if(_name_to_event.find(name) != _name_to_event.end())
                {
                    std::cout << "duplicate component name: "  << name << std::endl;
                    assert(false); 
                }

                _name_to_event[name] = &event;
                _name_to_component[name] = &component;
            }
            std::cout << "Processed event: " << event << std::endl;
            
            event.is_armed = false;
            event.is_critical_sent = false;
            event.is_all_sent = false; 
        }
        
        d_securities.assign(symbols.begin(), symbols.end());
        d_fields.assign(fields.begin(), fields.end());
    }

    bool poll()
    {
        while( !_queue.empty() )
        {
            _queue.pop(_msg);

            if(!_msg.has_actual && !_msg.has_revised)
                continue;

            //std::cout << "Event message has content "  << std::endl;

            std::map<std::string, eco_event*>::iterator event_it = _name_to_event.end();

            std::string name = std::string(_msg.symbol) + "LAST_PRICE";
            event_it = _name_to_event.find(name);
            
            if(event_it == _name_to_event.end())
            {
                std::cout << "event_it == _name_to_event.end() name=" << name << std::endl; 
                continue; 
            }
        
            bool new_actual = false;
            bool new_revised = false;
 
            if(event_it->second->is_armed)
            {
                //std::cout << "Event is armed" << std::endl;
 
                if(_msg.has_actual)
                {
                    //std::cout << "has_actual = true" << std::endl;
                    auto comp_it = _name_to_component.find(name);
                    if(comp_it == _name_to_component.end())
                    {
                        std::cout << "comp_it == _name_to_component.end() name=" << name << std::endl;
                        continue;  
                    }

                    if(!comp_it->second->out_is_ready) 
                    {
                        //std::cout << "!comp_it->second->out_is_ready" << std::endl;
                        comp_it->second->out_value =_msg.actual;
                        comp_it->second->out_is_ready = true; 
                        event_it->second->last_ts = comp_it->second->out_ts = from_epoch_nanos(_msg.epoch);
                        new_actual = true;  
                    }
                }
           
                if(_msg.has_revised)
                {
                    //std::cout << "has_revised = true" << std::endl;
                    name = std::string( _msg.symbol) + "ASK";
                    auto comp_it = _name_to_component.find(name);          
                    if(comp_it == _name_to_component.end())
                    {
                        std::cout << "comp_it == _name_to_component.end() name=" << name << std::endl;
                        continue; 
                    }

                    if(!comp_it->second->out_is_ready) 
                    {
                        //std::cout << "!comp_it->second->out_is_ready" << std::endl;
                        comp_it->second->out_value =_msg.revised;
                        comp_it->second->out_is_ready = true; 
                        event_it->second->last_ts = comp_it->second->out_ts = from_epoch_nanos(_msg.epoch);
                        new_revised = true;  
                    }
                }
                
                if(new_actual || new_revised)
                {
                    //std::cout << "new_actual=" << new_actual  << " new_revised=" << new_revised << std::endl;
                    if(!event_it->second->is_critical_sent)
                    {
                        //std::cout << "!event_it->second->is_critical_sent" << std::endl;
                        if(event_it->second->is_critical_ready())
                        {
                            //std::cout << "event_it->second->is_critical_ready()" << std::endl;
                            event_it->second->is_critical_sent = true; 
                            if(event_it->second->is_all_ready())
                            {
                                // std::cout << "event_it->second->is_all_ready()"<< std::endl;
                                event_it->second->is_all_sent = true; 
                                event_it->second->is_armed = false; 
                            }
                            std::cout << "sending event" << *event_it->second << std::endl; 
                            _handler->on_bb_event(*event_it->second);
                        }
                        else
                        {
                            if(event_it->second->is_all_ready())
                            {
                                // std::cout << " 2 event_it->second->is_all_ready()"  << std::endl;
                                event_it->second->is_all_sent = true; 
                                event_it->second->is_armed = false; 
            
                                std::cout << "sending event" << *event_it->second << std::endl; 
                                _handler->on_bb_event(*event_it->second);
                            }
                        }
                    }
                    else if(!event_it->second->is_all_sent)
                    {
                        if(event_it->second->is_all_ready())
                        {
                            // std::cout << "3 event_it->second->is_all_ready()"  << std::endl;
                            event_it->second->is_all_sent = true; 
                            event_it->second->is_armed = false; 
            
                            std::cout << "sending event" << *event_it->second << std::endl; 
                            _handler->on_bb_event(*event_it->second);
                        }
                    }
                }
            } 

            if(_oob_logger != nullptr)
            {
                _log_msg.type = MSG_TYPE::MSG_ECO_EVENT_MSG;
                _log_msg.uber.a_eco_event_msg.epoch = _msg.epoch;
                strcpy(_log_msg.uber.a_eco_event_msg.sym, _msg.symbol);
                if(_msg.has_actual)
                {
                    _log_msg.uber.a_eco_event_msg.has_actual = true;
                    _log_msg.uber.a_eco_event_msg.actual = _msg.actual;
                } 
                else
                {
                    _log_msg.uber.a_eco_event_msg.has_actual = false;
                    _log_msg.uber.a_eco_event_msg.actual = 0.0;
                }                
 
                if(_msg.has_revised)
                {
                    _log_msg.uber.a_eco_event_msg.has_revised = true;
                    _log_msg.uber.a_eco_event_msg.revised = _msg.revised;
                }
                else
                {
                    _log_msg.uber.a_eco_event_msg.has_revised = false;
                    _log_msg.uber.a_eco_event_msg.revised = 0.0;
                }
                _oob_logger->log(_log_msg);
            }
            // _handler->on_bb_event(_msg);
        }

        uint64_t cycles = _tsc();
        if( _tsc.nanos(_last_checked, cycles) > _update_interval_nanos || cycles < _last_checked )
        {
            // std::cout << "checking ARM" << std::endl;
            _last_checked = cycles;
            auto now = clock::now();
            for(auto & event : _eco_events )
            {
                //std::cout <<  "now: " << str(now) << std::endl;
                //std::cout << event << std::endl;

                if(event.is_armed || event.is_critical_sent || event.is_all_sent)
                {
                    // std::cout << "NO ARM continue" << std::endl;
                    continue;
                }
                
                // std::cout   << " event.ts=" << str(event.ts) 
                //                 << " _arm_interval_secs=" << _arm_interval_secs
                //                 << " now=" << str(now)
                //                 << "_startup_time=" << str(_startup_time)
                //                 << std::endl; 

                //std::cout << "then " << str(event.ts - seconds(_arm_interval_secs)) << std::endl;

                if( ( event.ts - now <= seconds(_arm_interval_secs) ) &&  ( now - event.ts <= seconds(_arm_interval_secs) ) && ( (now - _startup_time) > seconds(20) ) )
                {
                    std::cout << "ARMING EVENT " << event << std::endl; 
                    event.is_armed = true; 
                } 
                else
                {
                     // std::cout << "not arming " << std::endl;
                }
            }    
        }
    }
    
    virtual void _process()
    {
        static bool started = false;
        if(!started)
        {
	        d_session->subscribe(d_subscriptions, d_identity);
            std::cout << "started listening for bb events" << std::endl;
            started = true;
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(10));
    }

private:
    
    Handler                                         *_handler;
    circular_fifo<bb_msg>                           _queue; 
    bb_msg                                          _msg;
    std::string                                     _ip;
    std::string                                     _port; 
    bb_event_handler                                *d_eventHandler;
    std::vector<std::string>				        d_hosts;
    int							                    d_port;
	std::string					                    d_authOption;
	std::string					                    d_name;
    BloombergLP::blpapi::Identity					d_identity;
    BloombergLP::blpapi::SessionOptions             d_sessionOptions;
    BloombergLP::blpapi::Session                    *d_session;
    std::vector<std::string>                        d_securities;
    std::vector<std::string>                        d_fields;
    std::vector<std::string>                        d_options; 
    BloombergLP::blpapi::SubscriptionList           d_subscriptions; 
	std::string						                d_service;
    oob_logger<log_message>                         *_oob_logger = nullptr;
    log_message                                     _log_msg;
    std::vector<eco_event>                          _eco_events;
    std::map<std::string, eco_component *>          _name_to_component;
    std::map<std::string, eco_event *>              _name_to_event;
    TSC                                             _tsc;
    uint64_t                                        _last_checked = 0;
    double                                          _update_interval_nanos = 1000000;
    int32_t                                         _arm_interval_secs = 300;
    timestamp                                       _startup_time;
};

}


#endif // BB_MANAGER_H
