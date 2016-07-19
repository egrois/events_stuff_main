#ifndef ELEMENTS_CIRCULAR_FIFO_H
#define ELEMENTS_CIRCULAR_FIFO_H

#include <atomic>

// Single producer, single consumer thread safe message queue
// Uses atomics - lockfree
// Messages are copied when pushing 
// Supports only single message type

namespace elements
{
template < typename Msg >
class circular_fifo{
public:
 
    circular_fifo() : circular_fifo(1000)
    {
    }
 
    circular_fifo(size_t size) : _tail(0), _head(0), _capacity(size + 1)
    {
        _array = new Msg[_capacity];
    }

    ~circular_fifo() 
    {
        delete [] _array;
    }
 
    bool push(const Msg& item)
    {
        const auto current_tail = _tail.load(std::memory_order_relaxed);
        const auto next_tail = increment(current_tail);
        if(next_tail != _head.load(std::memory_order_acquire))
        {
            _array[current_tail] = item;
            _tail.store(next_tail, std::memory_order_release);
            return true;
        }
        return false;
    }
  
    bool pop(Msg& item)
    {
        const auto current_head = _head.load(std::memory_order_relaxed);
        if(current_head == _tail.load(std::memory_order_acquire))
            return false;
        item = _array[current_head];
        _head.store(increment(current_head), std::memory_order_release);
        return true;
    }

    bool peek(Msg& item) 
    {
        const auto current_head = _head.load(std::memory_order_relaxed);
        if(current_head == _tail.load(std::memory_order_acquire))
            return false;
        item = _array[current_head];
        return true;
    }

    bool drop()
    {
        const auto current_head = _head.load(std::memory_order_relaxed);
        if(current_head == _tail.load(std::memory_order_acquire))
            return false;
        _head.store(increment(current_head), std::memory_order_release);
        return true;
    }
  
    bool empty() const { return (_head.load() == _tail.load()); }
  
    bool full() const 
    {
        auto index = _tail.load(std::memory_order_seq_cst);
        auto next_tail = (index + 1) % _capacity;
        return (next_tail == _head.load(std::memory_order_seq_cst));
    }

    int size()
    {
        // TODO
        return 0;
    }
    

private:
  
    size_t increment(size_t idx) const { return (idx + 1) % _capacity; }
    
    size_t _capacity;
    std::atomic <size_t>  _tail;
    Msg *_array;
    std::atomic <size_t>  _head;
};
}
#endif // ELEMENTS_CIRCULAR_FIFO_H
