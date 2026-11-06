#include "dft.h"
#include"coefficients32_2D.h"

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE])
{
  //Write your code here
  DTYPE temp_real[SIZE] = {0};
  DTYPE temp_imag[SIZE] = {0};
  for(int i = 0; i < SIZE; i++)
  {
    for(int j = 0; j < SIZE; j++)
    {
        temp_real[i] += (real_sample[j] * cos_coeff_table[i][j] - imag_sample[j] * sin_coeff_table[i][j]);
    }
    for(int k = 0; k < SIZE; k++)
    {
        temp_imag[i] += (imag_sample[k] * cos_coeff_table[i][k] + real_sample[k] * sin_coeff_table[i][k]);
    }
  }

  for (int p = 0; p < SIZE; p++)
  {
    real_sample[p] = temp_real[p];
    imag_sample[p] = temp_imag[p];
  }
}
