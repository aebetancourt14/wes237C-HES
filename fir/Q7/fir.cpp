#include "fir.h"

void fir (
  data_t *y,
  data_t x
  )
{
#pragma HLS pipeline II=1

	coef_t c[N] = {10, 11, 11, 8, 3, -3, -8, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -11, -11, -8, -3, 3, 8, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 8, 3, -3, -8, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -11, -11, -8, -3, 3, 8, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 8, 3, -3, -8, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -11, -11, -8, -3, 3, 8, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
#pragma HLS array_partition variable=c complete

    static data_t shift_reg[N];
#pragma HLS array_partition variable=shift_reg complete

    acc_t acc = 0;

    TDL:
    for (int i = N - 1; i > 0; i--) {
#pragma HLS unroll
        shift_reg[i] = shift_reg[i - 1];
    }
    shift_reg[0] = x;

    MAC:
    for (int i = N - 1; i >= 0; i--) {
#pragma HLS unroll
        acc += shift_reg[i] * c[i];
    }

    *y = acc;
}
