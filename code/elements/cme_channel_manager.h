#ifndef ELEMENTS_CME_CHANNEL_MANAGER_H
#define ELEMENTS_CME_CHANNEL_MANAGER_H

#include <iostream>

#include <boost/foreach.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <map>
#include <set>
#include <sstream>

namespace elements
{

struct cme_connection_info
{
    std::string ip;
    std::string port;

    bool operator< (cme_connection_info const & c) const
    {
        if(ip != c.ip)
            return ip < c.ip;
        else
            return port < c.port;
    }
};

class cme_channel_manager
{
public:
    cme_channel_manager(std::string config)
    {
        read_xml(config, _pt);
    }

    ~cme_channel_manager()
    {
    }

    int get_channel( std::string code) 
    {
        std::map<std::string, int> m;

        BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, _pt) 
        {
            BOOST_FOREACH(const boost::property_tree::ptree::value_type &v1, v.second)
            {
                int channel =  v1.second.get<int>("<xmlattr>.id", false);
                BOOST_FOREACH(const boost::property_tree::ptree::value_type &v2, v1.second)
                {
                    BOOST_FOREACH(const boost::property_tree::ptree::value_type &v3, v2.second)
                    {
                        if(v3.first != "product")
                            continue;

                        BOOST_FOREACH(const boost::property_tree::ptree::value_type &v4, v3.second)
                        {
                            if(v4.first != "group")
                                continue;
                           
                            std::string code = v4.second.get<std::string>("<xmlattr>.code");
                            m[code] = channel;
                        }
                    }
                }
            } 
        }

        if(m.find(code) != m.end())
            return m[code];
        else
            return -1;
    }  

    std::set<cme_connection_info> get_connection_info( std::set<std::string> const syms, std::string channel_type)
    {
        std::set<cme_connection_info> con;
        
        std::set<std::string> channels;
        for( auto sym: syms)
        {
            int channel_int = get_channel(sym);
            if(channel_int < 0)
                return con;
            std::stringstream ss;
            ss << channel_int << channel_type;
            channels.insert(ss.str());
        }
        
        BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, _pt) 
        {
            BOOST_FOREACH(const boost::property_tree::ptree::value_type &v1, v.second)
            {
                BOOST_FOREACH(const boost::property_tree::ptree::value_type &v2, v1.second)
                {
                    if (v2.first != "connections")
                        continue;

                    BOOST_FOREACH(const boost::property_tree::ptree::value_type &v3, v2.second)
                    {
                        if(v3.first != "connection")
                            continue;                    
   
                        std::string chan =  v3.second.get<std::string>("<xmlattr>.id");
                        if(channels.find(chan) == channels.end())
                            continue;                       
                       
                        cme_connection_info info;
 
                        info.port = v3.second.get<std::string>("port");
                        info.ip = v3.second.get<std::string>("ip");
                        
                        con.insert(info);
                    }
                }
            } 
        }
        
        return con;
    }

private:

    boost::property_tree::ptree _pt; 
};
}

#endif //ELEMENTS_CME_CHANNEL_MANAGER_H
