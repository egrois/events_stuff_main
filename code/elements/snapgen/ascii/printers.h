std::ostream& operator <<(std::ostream& os, const order_book_entry & msg)
{
    return  os << str(from_epoch_nanos(msg.epoch)) << " MSG_ORDER_BOOK_ENTRY " 
            << " progress=" << EVENT_PROGRESS_to_string(msg.progress)  
            << " sym=" << msg.sym  
            << " side=" << ORDER_SIDE_to_string(msg.side)  
            << " price="  <<  std::setprecision(10) << msg.price  
            << " size=" << msg.size  
            << " id=" << msg.id ;
}

std::ostream& operator <<(std::ostream& os, const md_book & msg)
{
        return  os << str(from_epoch_nanos(msg.epoch)) << " MSG_MD_BOOK " << " sym=" << msg.sym  
             << " mpi="             << std::setprecision(10) << msg.mpi 
             << " top_bid_price="   << std::setprecision(10) << msg.top_bid_price  
             << " top_ask_price="   << std::setprecision(10) << msg.top_ask_price 
             << " pos=" << msg.pos
             << " last_trade_price=" << std::setprecision(10) << msg.last_trade_price
 
             << " b0=" << msg.b0  << " b1=" << msg.b1  << " b2=" << msg.b2  << " b3=" << msg.b3  << " b4=" << msg.b4  << " b5=" << msg.b5  << " b6=" << msg.b6  << " b7=" << msg.b7  << " b8=" << msg.b8  << " b9=" << msg.b9  << " a0=" << msg.a0  << " a1=" << msg.a1  << " a2=" << msg.a2  << " a3=" << msg.a3  << " a4=" << msg.a4  << " a5=" << msg.a5  << " a6=" << msg.a6  << " a7=" << msg.a7  << " a8=" << msg.a8  << " a9=" << msg.a9 ;
}
