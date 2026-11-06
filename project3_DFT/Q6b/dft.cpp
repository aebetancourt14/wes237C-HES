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
      int index = (i * j) % SIZE;
      real_op[i] += real_sample[j] * cos_coefficients_table[index] - imag_sample[j] * sin_coefficients_table[index];
    }
    for(int k = 0; k < SIZE; k++)
    {
      int index = (i * k) % SIZE;
      imag_op[i] += imag_sample[k] * cos_coefficients_table[index] + real_sample[k] * sin_coefficients_table[index];
    }
  }
}