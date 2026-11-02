#include "dft.h"
#include "coefficients1024.h"

// Q8: DFT1024 with j-lane parallelism.
//   U lanes of the inner loop run in parallel (unroll), each lane owning its
//   own bank of the accumulators (cyclic partition by U).
//   The twiddle tables are indexed (i*j)%SIZE, so which bank an access hits
//   depends on i and cannot be partitioned away. Instead each pair of lanes
//   gets its own copy of the table (2 read ports per BRAM copy).
//   Tune U (must divide SIZE): DSP use is ~20 per lane (220 available).
#define U 8

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE real_op[SIZE], DTYPE imag_op[SIZE])
{
  DTYPE real_acc[SIZE], imag_acc[SIZE];
#pragma HLS ARRAY_PARTITION variable=real_acc cyclic factor=U
#pragma HLS ARRAY_PARTITION variable=imag_acc cyclic factor=U

  DTYPE cos_t[U/2][SIZE], sin_t[U/2][SIZE];
#pragma HLS ARRAY_PARTITION variable=cos_t complete dim=1
#pragma HLS ARRAY_PARTITION variable=sin_t complete dim=1

  // Zero the accumulators and fill the replicated twiddle tables
  init: for(int i = 0; i < SIZE; i++)
  {
#pragma HLS PIPELINE II=1
    real_acc[i] = 0;
    imag_acc[i] = 0;
    for(int c = 0; c < U/2; c++)
    {
#pragma HLS UNROLL
      cos_t[c][i] = cos_coefficients_table[i];
      sin_t[c][i] = sin_coefficients_table[i];
    }
  }

  // Matrix-vector multiply, loop-interchanged (i outer, j inner)
  outer: for(int i = 0; i < SIZE; i++)
  {
    inner: for(int jj = 0; jj < SIZE/U; jj++)
    {
#pragma HLS PIPELINE II=1
      DTYPE rs = real_sample[i];
      DTYPE is = imag_sample[i];
      lanes: for(int k = 0; k < U; k++)
      {
#pragma HLS UNROLL
        int j = jj * U + k;
        int index = (i * j) & (SIZE - 1);
        DTYPE c = cos_t[k/2][index];
        DTYPE s = sin_t[k/2][index];
        real_acc[j] += rs * c - is * s;
        imag_acc[j] += is * c + rs * s;
      }
    }
  }

  // Copy out. A separate buffer keeps the same structure ready for Q9 (streaming)
  copy: for(int j = 0; j < SIZE; j++)
  {
#pragma HLS PIPELINE II=1
    real_op[j] = real_acc[j];
    imag_op[j] = imag_acc[j];
  }
}
