#include <iostream>
#include <fstream>
#include <chrono>

#include <ascii_fix_parser.h>

static int cnt = 0;

class fix_handler
{
public:

    template <typename Hdr >
    void operator()(Hdr & hdr, elements::SecurityDefinition &msg)
    {
       // std::cout << hdr << " @@@ "<< msg << std::endl;
        //if(strlen(msg.SecurityDesc) != 4)
       //     return; 
       // std::cout << hdr << " @@@ "<< msg << std::endl;
       //
       //
       std::cout << "SD " << hdr.SendingTime << " " << msg.SecurityDesc <<  std::endl;
    }
    
    template <typename Hdr >
    void operator()(Hdr & hdr, elements::MarketDataIncrementalRefresh &msg)
    {
        //if(msg.MDIncGrp.size() < 1)
        //    return;            
        
        //if(strlen(msg.MDIncGrp[0].SecurityDesc) != 4)
        //    return; 

        //std::cout << msg.TradeDate << std::endl;
        
        //std::cout << hdr << " @@@ " << std::endl;
        //for (auto entry: msg.MDIncGrp)
        //{
        //    std::cout << entry << std::endl;
        //}
       std::cout << "IR " << hdr.SendingTime  << " " << msg.MDIncGrp[0].SecurityDesc << std::endl;
    }

    template <typename Hdr >
    void operator()(Hdr & hdr, elements::SecurityStatus &msg)
    {
        //if(strlen(msg.SecurityDesc) != 4)
        //    return; 
        //
        //std::cout << msg << std::endl;
        // std::cout << hdr << " @@@ "<< msg << std::endl;
       std::cout << "SS " << hdr.SendingTime << " " << msg.SecurityDesc << std::endl;
    }

    template <typename Hdr, typename Msg >
    void operator()(Hdr & hdr, Msg &msg)
    {
      // std::cout << hdr  << std::endl;
    }
};

int main(int argc, char **argv)
{
    if ( argc < 2 )
        std::cout << "input file? " << std::endl;

    std::string file_name = argv[1];

    char *buf;
    int i = 0;
    char cmd[100];
    FILE * in;
    int r;
    size_t len = 0;

    if(file_name.find(".gz") != std::string::npos)
    {
        sprintf(cmd, "/bin/gunzip -c %s",file_name.c_str());
        in = popen(cmd, "r");
    }
    else
        in = fopen(file_name.c_str(), "r");
        
    std::ifstream f(file_name.c_str());
    fix_handler h;
    elements::fix_parser<fix_handler> t(&h); 
    
    while((r = getline(&buf, &len, in)) > 0 )
        t.parse(buf);        

    return 0;
}


