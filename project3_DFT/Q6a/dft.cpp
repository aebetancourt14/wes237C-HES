#include<math.h>
#include "dft.h"
#include"coefficients1024.h"

#define PI 3.141592653589793

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE real_op[SIZE], DTYPE imag_op[SIZE])
{
  for(int i = 0; i < SIZE; i++)
  {
    real_op[i] = 0;
    imag_op[i] = 0;
    for(int j = 0; j < SIZE; j++)
    {
      real_op[i] += real_sample[j] * cos(2 * PI * i * j / SIZE) + imag_sample[j] * sin(2 * PI * i * j / SIZE);
    }
    for(int k = 0; k < SIZE; k++)
    {
      imag_op[i] += imag_sample[k] * cos(2 * PI * i * k / SIZE) - real_sample[k] * sin(2 * PI * i * k / SIZE);
    }
  }
}