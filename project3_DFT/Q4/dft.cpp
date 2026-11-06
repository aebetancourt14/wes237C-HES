#include "dft.h"
#include"coefficients32_2D.h"

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE real_output[SIZE], DTYPE imag_output[SIZE])
{
  //Write your code here
#pragma HLS ARRAY_PARTITION variable=real_sample type=block factor=32 dim=1
#pragma HLS ARRAY_PARTITION variable=imag_sample type=block factor=32 dim=1
#pragma HLS ARRAY_PARTITION variable=real_output type=block factor=32 dim=1
#pragma HLS ARRAY_PARTITION variable=imag_output type=block factor=32 dim=1
#pragma HLS ARRAY_PARTITION variable=cos_coeff_table type=block factor=32 dim=2
#pragma HLS ARRAY_PARTITION variable=sin_coeff_table type=block factor=32 dim=2

#pragma HLS PIPELINE II=1
  for(int i = 0; i < SIZE; i++)
  {
    real_output[i] = 0;
    imag_output[i] = 0;
    for(int j = 0; j < SIZE; j++)
    {
        real_output[i] += (real_sample[j] * cos_coeff_table[i][j] - imag_sample[j] * sin_coeff_table[i][j]);
    }
    for(int k = 0; k < SIZE; k++)
    {
        imag_output[i] += (imag_sample[k] * cos_coeff_table[i][k] + real_sample[k] * sin_coeff_table[i][k]);
    }
  }

}
