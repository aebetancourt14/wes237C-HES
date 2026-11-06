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
  }
  for(int i = 0; i < SIZE; i++)
  {
    for(int j = 0; j < SIZE; j++)
    {
      int index = (i * j) % SIZE;
      real_op[j] += real_sample[i] * cos_coefficients_table[index] - imag_sample[i] * sin_coefficients_table[index];
      imag_op[j] += imag_sample[i] * cos_coefficients_table[index] + real_sample[i] * sin_coefficients_table[index];   
    }  
  }
}