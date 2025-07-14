#include "MathFunctions.h"
#include <cmath>
#include <iostream>
#ifndef USE_MYMATH
#include <cmath>
#else
#include "mysqrt.h"
#endif

namespace mathfunctions {
double sqrt(double x)
{
  // Otherwise, use std::sqrt.
  if (USE_MYMATH)
  {
    std::cout << USE_MYMATH << std::endl;
    std::cout << "Using custom sqrt" << std::endl;
    return detail::mysqrt(x);
  }
  else
  {
    std::cout << "Using std::sqrt" << std::endl;  
    return std::sqrt(x);
  }
}
}