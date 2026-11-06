#include "dft.h"
#include "coefficients1024.h"

// Q9: streaming version of the Q8 design (U lanes in parallel).
//   1. Read SIZE packets from each input stream into local buffers, while zeroing
//      the accumulators and filling the replicated twiddle tables.
//   2. Same lane-parallel matrix-vector multiply as Q8 (i outer, j inner, II=1).
//   3. Write SIZE packets to each output stream; TLAST is set on the last one.
// Inputs/outputs are hls::stream references, not pointers, so nothing is
// post-incremented by hand.
#define U 8

void dft(hls::stream<transPkt>& real_sample, hls::stream<transPkt>& imag_sample,
         hls::stream<transPkt>& real_op, hls::stream<transPkt>& imag_op)
{
#pragma HLS INTERFACE axis port=real_sample
#pragma HLS INTERFACE axis port=imag_sample
#pragma HLS INTERFACE axis port=real_op
#pragma HLS INTERFACE axis port=imag_op
#pragma HLS INTERFACE s_axilite port=return

  DTYPE rs_buf[SIZE], is_buf[SIZE];

  DTYPE real_acc[SIZE], imag_acc[SIZE];
#pragma HLS ARRAY_PARTITION variable=real_acc cyclic factor=U
#pragma HLS ARRAY_PARTITION variable=imag_acc cyclic factor=U

  DTYPE cos_t[U/2][SIZE], sin_t[U/2][SIZE];
#pragma HLS ARRAY_PARTITION variable=cos_t complete dim=1
#pragma HLS ARRAY_PARTITION variable=sin_t complete dim=1

  // Read the inputs, zero the accumulators, fill the replicated tables
  init_read: for(int i = 0; i < SIZE; i++)
  {
#pragma HLS PIPELINE II=1
    transPkt rp = real_sample.read();
    transPkt ip = imag_sample.read();
    rs_buf[i] = rp.data;
    is_buf[i] = ip.data;
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
      DTYPE rs = rs_buf[i];
      DTYPE is = is_buf[i];
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

  // Stream the results out; TLAST marks the final packet of each stream
  write_out: for(int j = 0; j < SIZE; j++)
  {
#pragma HLS PIPELINE II=1
    transPkt rp, ip;
    rp.data = real_acc[j];
    ip.data = imag_acc[j];
    rp.keep = 0xf;
    rp.strb = 0xf;
    ip.keep = 0xf;
    ip.strb = 0xf;
    rp.last = (j == SIZE - 1);
    ip.last = (j == SIZE - 1);
    real_op.write(rp);
    imag_op.write(ip);
  }
}
