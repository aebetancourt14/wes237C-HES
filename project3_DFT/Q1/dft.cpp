#include<math.h>
#include "dft.h"
#include"coefficients32.h"
#define PI  3.141592653589793
void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE])
{
  //Write your code here
  DTYPE temp_real[SIZE] = {0};
  DTYPE temp_imag[SIZE] = {0};
  for(int i = 0; i < SIZE; i++)
  {
    for(int j = 0; j < SIZE; j++)
    {
        temp_real[i] += (real_sample[j] * cos(2*PI/SIZE*i*j) + imag_sample[j] * sin(2*PI/SIZE*i*j));
    }
    for(int k = 0; k < SIZE; k++)
    {
        temp_imag[i] += (imag_sample[k] * cos(2*PI/SIZE*i*k) - real_sample[k] * sin(2*PI/SIZE*i*k));
    }
  }

  for (int p = 0; p < SIZE; p++)
  {
    real_sample[p] = temp_real[p];
    imag_sample[p] = temp_imag[p];
  }
}
