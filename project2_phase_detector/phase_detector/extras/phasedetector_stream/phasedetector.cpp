#include "phasedetector.h"

void phasedetector_core(data_t I, data_t Q, data_t *R, data_t *Theta)
{
	data_t X, Y;
	fir(I, Q, &X, &Y);
	cordiccart2pol(X, Y, R, Theta);
}

void phasedetector(hls::stream<DTYPE> &I, hls::stream<DTYPE> &Q,
                   hls::stream<DTYPE> &R, hls::stream<DTYPE> &Theta,
                   int length)
{
#pragma HLS INTERFACE axis port=I
#pragma HLS INTERFACE axis port=Q
#pragma HLS INTERFACE axis port=R
#pragma HLS INTERFACE axis port=Theta
#pragma HLS INTERFACE s_axilite port=length
#pragma HLS INTERFACE s_axilite port=return

	for (int i = 0; i < length; i++)
	{
#pragma HLS PIPELINE II=4
		DTYPE pi = I.read();
		DTYPE pq = Q.read();

		data_t r, th;
		phasedetector_core(pi.data, pq.data, &r, &th);

		DTYPE outR = pi;   // reuse packet so side channels pass through
		DTYPE outT = pq;
		outR.data = r;
		outT.data = th;
		outR.last = (i == length - 1) ? 1 : 0;   // last only on final output
		outT.last = (i == length - 1) ? 1 : 0;

		R.write(outR);
		Theta.write(outT);
	}
}
