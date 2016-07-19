#ifndef ELEMENTS_BT_EVENTS_MANAGER_H
#define ELEMENTS_BT_EVENTS_MANGER_H

#include <eco_event.h>
#include <vector>
#include <string>


namespace elements
{

struct event_file_entry
{
    std::string ticker;
    std::string sub_ticker;
    double value;
};

struct event_file_event
{
    timestamp ts;
    std::vector<event_file_entry> entries;
    
    bool operator < (event_file_event const & a) { return ts < a.ts; } 
};


struct eco_event_holder
{
    timestamp ts;
    eco_event ev;

    bool operator < ( eco_event_holder const & a) { return ts < a.ts; }
};

template <typename Handler>
class bt_events_manager : public feed
{
public:
    bt_events_manager(  Handler *handler,
                        std::string file_name, 
                        std::vector<eco_event> events,
                        std::string date) 
        : _handler(handler),
          _file_name(file_name),
          _date(date),
          _events(events) 
    {
        read_bt_events();
        build_prepared_events();
        _far = make_time(_date, "23:59:59");
    }

    virtual bool peek_time(timestamp &ts) 
    {
        if( _index >=_prepared_events.size() )
            ts = _far; 
        else
        {
            ts = _prepared_events[_index].ts;
        }
        return true;
    }

    virtual bool poll() 
    {
        timestamp ts;
        peek_time(ts);

        auto now = _handler->now();

        // std::cout << str_date(ts) << " " << str(ts) << " " << str_date(now) << " " << str(now) << std::endl; 

        if( now >= ts)
        {
                // std::cout << "==================== bt_event_manager poll() " << std::endl;
            _handler->on_bb_event(_prepared_events[_index]);
            ++_index;
        }
    }

private:

    void read_bt_events()
    {
        size_t len = 256;
        char *buf = (char *) malloc(len);
        std::string file = _file_name + "." + _date; 
        std::cout << "opening events file=" << file << std::endl; 
        FILE *in = fopen(file.c_str(), "r");
        assert(in != 0);
        event_file_event ev;
        while(getline(&buf, &len, in) > 0)
        {
            if(buf[0] == '<')
            {
                char *temp = strtok(buf, "=");   
                temp = strtok(NULL, "=");
                char *temp2 =  strtok(temp, ">");
                std::string tm = temp2;

                if(ev.entries.size() > 0)
                {
                    _bt_events.push_back(ev);
                    ev.entries.clear();           
                }
                ev.ts = make_time(_date, tm);
            }
            else
            {
                event_file_entry entry;
                char *temp = strtok(buf, ",");   
                entry.ticker = std::string(temp);       

                temp = strtok(NULL, ",");
                entry.sub_ticker = std::string(temp);

                temp = strtok(NULL, ",");
                entry.value = atof(temp);
                ev.entries.push_back(entry);
            }
        }
        
        if(ev.entries.size() > 0)
            _bt_events.push_back(ev);
        
        fclose(in);
        free(buf);
        //print_bt_events();
    }

    event_file_event & get_event(std::string ticker)
    {
        // std::cout << "finding " << ticker << std::endl;

        for(auto & ev: _bt_events)
        {
            for(auto entry: ev.entries)
            {
                // std::cout << "Looking at " << entry.ticker << std::endl;
                if(ticker == entry.ticker)
                    return ev;
            }
        }
        std::cout << "cannot find bt event with ticker: " << ticker<< std::endl;
        assert(false); 
    }    

    void build_prepared_events()
    {
        for(auto & e: _events)
        {
            auto first_component = e.components[0];
            event_file_event & ev = get_event(first_component.bb_name);

            for(auto &comp: e.components)
            {
                bool found = false;
                for(auto entry: ev.entries)
                {
                    if(comp.bb_name == entry.ticker && comp.bb_field == entry.sub_ticker)
                    {
                        comp.out_value = entry.value;
                        comp.out_is_ready = true;
                        comp.out_ts = ev.ts;

                        
                        found = true;
                    }
                }

                if(found == false)
                {
                    std::cout   << "Cannot find component: " 
                                << comp.bb_name << " " <<  comp.bb_field << std::endl; 
                    assert(false);
                }                
            }
            e.is_critical_sent == true;
            e.is_all_sent = true;
            e.ts = ev.ts;
            e.last_ts = ev.ts;

            _prepared_events.push_back(e);
        }

        std::sort(_prepared_events.begin(),_prepared_events.end());

        for(auto &e: _prepared_events)
        {
           std::cout << e << std::endl;
        } 
    }

    void print_bt_events()
    {
        for(auto ev: _bt_events)
        {
            std::cout << "<TS=" << str(ev.ts) << ">" << std::endl;
            for(auto e: ev.entries)
            {
                std::cout << e.ticker << ","
                          << e.sub_ticker << ","
                          << e.value  << std::endl;
            }
        }
    }

    std::vector<event_file_event> _bt_events;
    Handler *_handler;
    std::string _file_name;
    std::vector<eco_event> _events;
    std::string _date; 
    std::vector<eco_event> _prepared_events;

    int _index = 0;

    timestamp _far;

};

}

#endif // ELEMENTS_BT_EVENTS_MANAGER_H 
