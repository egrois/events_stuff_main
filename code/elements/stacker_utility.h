#ifndef ELEMENTS_STACKER_UTILITY_H
#define ELEMENTS_STACKER_UTILITY_H

namespace elements
{
class utility 
{
public:
    virtual int operator()( int old_size, int distance) = 0;
    virtual bool compare(utility & u) { return false; }
};

class constant_utility : public utility
{
public:

    constant_utility(int max_x, int size) : _max_x(max_x), _size(size) {}

    virtual ~constant_utility(){}

    virtual int operator()( int old_size, int distance) 
    {
        if(distance >= _max_x)
            return 0;
        return _size;
    }
   
    virtual bool compare(utility & u)
    {
        constant_utility * z = dynamic_cast< constant_utility *> (&u);
        if( z == NULL)
            return false; 
        if(z->_size != _size)
            return false; 
        if(z->_max_x != _max_x)
            return false;
        return true; 
    }
 
private:
    int _size = 0;   
    int _max_x = 0; 
};

class linear_utility : public utility
{
public:
    linear_utility(int max_x, double a, double b, double tolerance) 
        : _max_x(max_x), _a(a), _b(b), _tolerance(tolerance) {} 

    virtual ~linear_utility() { } 

    virtual int operator()( int old_size, int distance) 
    {
       if( distance >= _max_x )
            return 0;
        double v = _a * (double)distance + _b + 1.0e-10;
       
        if( v <= 0 )
            return 0;
        
        if( old_size <= 0 )
            return (int) v;

        if( abs( v / (double) old_size - 1.0 ) < _tolerance )
            return old_size;
        
        return (int) v;
    }

    virtual bool compare(utility & u) 
    { 
        linear_utility * z = dynamic_cast< linear_utility *> (&u);
        if( z == NULL )
            return false; 
        if(_max_x == z->_max_x && _a == z->_a && _b == z->_b && _tolerance ==z->_tolerance)
            return true;
        return false;    
    }

protected:
    int _max_x = 0;
    double _a,_b; 
    double _tolerance;
};

class exponential_utility : public utility
{
public:
    exponential_utility(int max_x, double a, double b, double tolerance) 
        : _max_x(max_x), _a(a), _b(b), _tolerance(tolerance) {} 

    virtual ~exponential_utility() { } 
    
    virtual int operator()( int old_size, int distance) 
    {
        if( distance >= _max_x )
            return 0;
        
        double v = _a * std::exp((double)distance) + _b + 1.0e-10;
       
        if( v <= 0 )
            return 0;
        
        if( old_size <= 0 )
            return (int) v;

        if( abs( v / (double) old_size - 1.0 ) < _tolerance )
            return old_size;
        
        return (int) v;
    }
    
    virtual bool compare(utility & u) 
    { 
        exponential_utility * z = dynamic_cast<exponential_utility *> (&u);
        if( z == NULL)
            return false; 
        if(_max_x == z->_max_x && _a == z->_a && _b == z->_b && _tolerance == z->_tolerance)
            return true;
        return false;    
    }
    
protected:
    int _max_x = 0;
    double _a,_b; 
    double _tolerance;
};

class quadratic_utility : public utility
{
public:
    quadratic_utility(int max_x, double a, double b, double tolerance) 
        : _max_x(max_x), _a(a), _b(b), _tolerance(tolerance) {} 

    virtual ~quadratic_utility() { } 
    
    virtual int operator()( int old_size, int distance) 
    {
        if( distance >= _max_x )
            return 0;
        
        double v = _a * (double)distance * (double)distance + _b + 1.0e-10;
       
        if( v <= 0 )
            return 0;
        
        if( old_size <= 0 )
            return (int) v;

        if( abs( v / (double) old_size - 1.0 ) < _tolerance )
            return old_size;
        
        return (int) v;
    }
    
    virtual bool compare(utility  & u) 
    {
        quadratic_utility * z =   dynamic_cast<quadratic_utility *> (&u);
        if( z == NULL)
            return false; 
        if(_max_x == z->_max_x && _a == z->_a && _b == z->_b && _tolerance == z->_tolerance)
            return true;
        return false;    
    }

    
protected:
    int _max_x = 0;
    double _a,_b; 
    double _tolerance;
};

struct price_size
{
    int64_t price;
    int32_t size;
};

}

#endif
