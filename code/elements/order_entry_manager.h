#ifndef ELEMENTS_ORDER_ENTRY_MANAGER_H
#define ELEMENTS_ORDER_ENTRY_MANAGER_H

#include <fstream>

namespace elements
{

enum oem_action
{
    OEM_NEW = 0,
    OEM_CANCEL = 1,
    OEM_MODIFY = 2
};

struct oem_ticket
{
    std::string oem_id;
    oem_action action;
    std::string symbol;
    int64_t price;
    int32_t size;
    int64_t order_id;
    bool is_ioc;
    bool is_bid; 
};

template <typename Handler>
class order_entry_manager
{
public:
    
    void init( std::string file_name, Handler & handler)
    {
        _handler = &handler;

        std::ofstream f;
        f.open(file_name.c_str());
        if(!f.is_open())
        {
            std::cout << "cannot create order entry file: " << file_name << std::endl;
            assert(false);
        }   
        f.close();

        _f.open(file_name.c_str());
        
        if(!_f.is_open())
        {
            std::cout << "cannot read order entry file:" << file_name << std::endl;
            assert(false);
        }
    }

    void poll()
    {
        if(_f.is_open())
        {
            try
            {
                while(std::getline(_f, _line))
                {
                    process_line();
                }
            }
            catch(...)
            {
                std::cout << "getline error in order entry" << std::endl;
                assert(false);
            }
        
            if(!_f.eof())
            {
                std::cout << "stopped reading order entry file" << std::endl;
                assert(false);
            } 
            _f.clear();  
        }
        else
        {   
            std::cout <<"order entry file not open" << std::endl;
            assert(false);
        }
    }

    void process_line()
    {
        std::cout << "processing order entry: " << _line << std::endl;
      
        oem_ticket ticket;
        
        try
        {
            oem_ticket ticket;
            char line[100];
            strcpy(line, _line.c_str());

            char *temp = strtok(line, ",");   
            ticket.oem_id = temp;       
            
            temp = strtok(NULL, ",");   
            ticket.action = (oem_action)atoi(temp);
            
            temp = strtok(NULL, ",");   
            ticket.symbol = temp;

            temp = strtok(NULL, ",");   
            ticket.price = atol(temp);
            
            temp = strtok(NULL, ",");   
            ticket.size = atoi(temp);
            
            temp = strtok(NULL, ",");   
            ticket.order_id = atoi(temp);

            temp = strtok(NULL, ",");   
            ticket.is_ioc = (atoi(temp) == 1) ? true : false;
            
            temp = strtok(NULL, ",");   
            ticket.is_bid = (atoi(temp) == 1) ? true : false;

            if(_tickets.find(ticket.oem_id) != _tickets.end())
            {
                std::cout << "duplicate order entry " << std::endl;
                return;
            }
            
            _handler->on_order_entry(ticket);
            _tickets[ticket.oem_id] = ticket;
        }
        catch(...)
        {
            std::cout << "failed reading order entry ticket - skipping" << std::endl;
        }
    }

private:

    std::ifstream                       _f;
    std::map<std::string, oem_ticket>   _tickets;
    Handler *                           _handler = nullptr;
    std::string                         _line;
};



}
#endif //ELEMENTS_ORDER_ENTRY_MANAGER_H
