#include <stdio.h>
#include <math.h>
#include "phasedetector.h"

int main()
{
	const int SAMPLES = 1024;
	hls::stream<DTYPE> sI, sQ, sR, sT;
	float gR[SAMPLES], gT[SAMPLES];

	FILE *fg = fopen("out.gold.dat", "r");
	FILE *fi = fopen("input_i.dat", "r");
	FILE *fq = fopen("input_q.dat", "r");
	if (!fg || !fi || !fq) { printf("missing data file\n"); return 1; }

	for (int i = 0; i < SAMPLES; i++)
	{
		float v;
		fscanf(fg, "%f %f", &gR[i], &gT[i]);

		DTYPE pi, pq;
		fscanf(fi, "%f", &v); pi.data = v;
		fscanf(fq, "%f", &v); pq.data = v;
		pi.keep = -1; pi.strb = -1; pq.keep = -1; pq.strb = -1;
		pi.last = (i == SAMPLES - 1); pq.last = (i == SAMPLES - 1);
		sI.write(pi);
		sQ.write(pq);
	}
	fclose(fg); fclose(fi); fclose(fq);

	phasedetector(sI, sQ, sR, sT, SAMPLES);

	float sumR = 0, sumT = 0;
	int last_ok = 1;
	for (int i = 0; i < SAMPLES; i++)
	{
		DTYPE oR = sR.read();
		DTYPE oT = sT.read();
		float eR = (float)oR.data - gR[i];
		float eT = (float)oT.data - gT[i];
		sumR += eR * eR;
		sumT += eT * eT;
		int expect = (i == SAMPLES - 1);
		if ((int)oR.last != expect || (int)oT.last != expect) last_ok = 0;
	}
	float rmseR = sqrtf(sumR / SAMPLES);
	float rmseT = sqrtf(sumT / SAMPLES);

	printf("RMSE(R) %0.15f  RMSE(Theta) %0.15f  last_ok %d\n", rmseR, rmseT, last_ok);
	return (rmseR < 0.001 && rmseT < 0.001 && last_ok) ? 0 : 1;
}
