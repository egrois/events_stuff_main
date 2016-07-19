#ifndef ELEMENTS_CME_SECDEF_H
#define ELEMENTS_CME_SECDEF_H

#include <md.h>
#include <list>
#include <set>
#include <ascii_fix_parser.h>
#include <cme_ascii_secdef_parser.h>
#include <md_translate.h>
#include <feed.h>

namespace elements
{
namespace cme
{

class secdef_manager
{
public:

    void on_security_status_raw(elements::cme::SecurityStatus &msg){} 
    void on_incremental_refresh_raw(elements::cme::IncrementalRefresh &msg){}

    void on_security_definition_raw(elements::cme::SecurityDefinition &msg)
    {
        if(_symbols.find(msg.Symbol) != _symbols.end())
        {
            if(_secdefs.find(msg.Symbol) == _secdefs.end())
                _secdefs[msg.Symbol] = msg;
            else
                _stop_flag = true;
        }
    }

    void operator()(elements::AsciiSecDefMsg &msg)
    {
        cme::SecurityDefinition secdef;
        secdef.reset();
        assert(translate( &msg, &secdef));
        if(_symbols.find(secdef.SecurityDesc) != _symbols.end())
        {
            _secdefs[secdef.SecurityDesc] = secdef;
        } 
    }

    template <typename Hdr > void operator()(Hdr & hdr, elements::SecurityDefinition &msg)
    {
        cme::SecurityDefinition secdef;
        secdef.reset();

        assert(translate( &hdr, &msg, &secdef, true ));

        if(_symbols.find(secdef.SecurityDesc) != _symbols.end())
        {
            _secdefs[secdef.SecurityDesc] = secdef;
        } 
    }

    template <typename Hdr, typename Msg > void operator()(Hdr & hdr, Msg &msg){ }


    std::list<cme::SecurityDefinition>  get_secdefs( 
        std::set<std::string> symbols, 
        bool is_realtime, 
        std::string date, 
        std::string secdef_root,
        std::string secdef_format,
        const std::list<instrument_feed_config> & configs,
        std::string channel_file_root,
        std::string channel_side) 
    {
        if(secdef_format == "ascii_hist")                                   // cme hisrtorical data extract
        {
            return process_ascii_hist(symbols, date, secdef_root);    
        }
        else if(secdef_format == "ascii_daily")                             // cme daily from ftp site
        {
            return process_ascii_daily(symbols, date, secdef_root);    
        }
        else if(secdef_format == "bin_hist")                                // multicast cme recordings
        {
            assert(!is_realtime);
            return process_bin_hist(symbols, date, secdef_root, configs, channel_file_root, channel_side);    
        }
        else if(secdef_format == "bin_rt")                                  // live stream
        {
            assert(is_realtime);
            return process_bin_rt(symbols, date, secdef_root, configs,channel_file_root, channel_side);    
        }
        else
        {
            std::stringstream ss;
            ss << "Unknown data format for security definitions " << secdef_format; 
            throw std::runtime_error(ss.str());
        }
    }

private:
    
    std::list<cme::SecurityDefinition>  process_ascii_hist( 
        std::set<std::string> symbols, 
        std::string date, 
        std::string secdef_root)
    {
        _secdefs.clear();
        _symbols = symbols;

        std::set<std::string> files;
        for(auto symbol: symbols)
        {
            std::string root = symbol.substr(0, symbol.length() - 2 );
            files.insert(secdef_root + "/" + root + "/" + root + ".secdef." + date);
        }

        fix_parser <secdef_manager> parser(this);

        size_t len = 5000;

        char *buf = (char *) malloc(len);
        for(auto file: files)
        {
            std::cout << "opening security definition file=" << file << std::endl;
            FILE *in = fopen(file.c_str(), "r");
            assert(in != 0);
            while(getline(&buf, &len, in) > 0)
            {
                parser.parse(buf);
            }
            fclose(in); 
        }
        free(buf);
        
        assert(_symbols.size() == _secdefs.size());

        std::list<cme::SecurityDefinition> ret;
        for(auto s: _secdefs)
            ret.push_back(s.second);
        return ret;
    }

    std::list<cme::SecurityDefinition>  process_ascii_daily(
        std::set<std::string> symbols, 
        std::string date,
        std::string secdef_root)
    {
        _secdefs.clear();
        _symbols = symbols;


        cme_ascii_secdef_parser <secdef_manager> parser(this);

        size_t len = 5000;

        std::string year = date.substr(0,4); 
        std::string month = date.substr(4,2);

        std::string file_name = 
            secdef_root +
            "/" + year +
            "/" + month +
            "/" + date +
            "/Production/secdef.dat.gz";
        
        char *buf = (char *) malloc(len);
        std::cout << "opening security definition file=" << file_name << std::endl;
        
        char cmd[200];
        sprintf(cmd, "/bin/gunzip -c %s", file_name.c_str());
        FILE *in = popen(cmd, "r");
        
        assert(in != 0);
        int i = 0;

        while(getline(&buf, &len, in) > 0)
        {
            parser.parse(buf);
        }
    
        fclose(in); 
        free(buf);

        if(_symbols.size() != _secdefs.size())
        {
            std::cout << "ERROR: Cannot find security definitions for all symbols" << std::endl;
            std::cout <<  "  SYMBOLS:" << std::endl;
            for(auto sym: _symbols)
                std::cout << sym << std::endl;
            std::cout <<  "  SECDEFS:" << std::endl;
            for(auto sec: _secdefs)
                std::cout << sec.first << std::endl; 
        }
        assert(_symbols.size() == _secdefs.size());

        std::list<cme::SecurityDefinition> ret;
        for(auto s: _secdefs)
            ret.push_back(s.second);
        return ret;
    }

    std::list<cme::SecurityDefinition>  process_bin_hist(
        std::set<std::string> symbols, 
        std::string date,
        std::string secdef_root,
        const std::list<instrument_feed_config> & configs,
        std::string channel_file_root,
        std::string channel_side) 
    {
        _symbols = symbols;

        std::string year = date.substr(0, 4);
        std::string month =  date.substr(4, 2);
        std::string channel_file = channel_file_root + "/" + year + "/" + month+ "/" 
                            + date + "/Production/Configuration/config.xml";
        cme_channel_manager cman(channel_file);

        std::map<int, std::string> subscribed; // channel to file name
        std::map<std::string, int> symbol_channel; // symbol to channel

        for(auto conf : configs )
        {
            std::string sroot = conf.symbol.substr(0, conf.symbol.length() - 2);
            std::string year = date.substr(0, 4);
            std::string month = date.substr(4, 2);
            int channel = cman.get_channel(sroot); 
            std::stringstream ss;
            ss << channel; 
            std::string chstr = ss.str();
            std::string full_path 
                =   conf.bt_input + "/" + year + "/" + month + "/" + date + 
                    "/cme_inst_" + chstr + "N" + conf.bt_channel + chstr + "_" + date + ".pcap0.bz2";
            symbol_channel[conf.symbol] = channel;           
            subscribed[channel] = full_path;
        }

        for(auto chan : subscribed)
        {
            sbe_cme_pcap_feed<secdef_manager> f;
            if(!f.init(chan.second, this))
            {
                std::cout << "Cannot initialize secdef feed from file " << chan.second << std::endl;
                assert(false);
            }
            _stop_flag = false;
            while(f.poll()) 
            {
                if(_stop_flag)
                    break;
            }
        }       
        assert(_symbols.size() == _secdefs.size());
        std::list<cme::SecurityDefinition> ret;
        for(auto s: _secdefs)
            ret.push_back(s.second);
        return ret;
    }

    std::list<cme::SecurityDefinition>  process_bin_rt(
        std::set<std::string> symbols, 
        std::string date,
        std::string secdef_root,
        const std::list<instrument_feed_config> & configs,
        std::string channel_file_root,
        std::string channel_side) 
    {
        std::list<cme::SecurityDefinition> ret;
        return ret;
    }

    std::map <std::string, cme::SecurityDefinition> _secdefs;
    std::set <std::string> _symbols;
    bool _stop_flag = false;
};

}
}

#endif //ELEMENTS_CME_SECDEF_H 
