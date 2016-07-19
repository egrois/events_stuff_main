#ifndef ELEMENTS_GLOBAL_PRICE_MULTIPLIER_H
#define ELEMENTS_GLOBAL_PRICE_MULTIPLIER_H

namespace elements
{
namespace gpm
{
    const int32_t exp = 7;
    const int     mult_int = 1e7;
    const double  mult_double = (double)mult_int;
    const double  anti_mult_double = 1.0 / mult_double;
    const double  amd = anti_mult_double;
}
}

#endif //ELEMENTS_GLOBAL_PRICE_MULTIPLIER_H
