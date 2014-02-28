#include "math.h"

int math_pow(int base, int exponent)
{
  int x, output = 1;

  for(x = 0; x < exponent; x++)
  {
    output = output * base;
  }

  return output;
}
