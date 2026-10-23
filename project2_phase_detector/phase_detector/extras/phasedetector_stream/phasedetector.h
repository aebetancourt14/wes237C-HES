#ifndef PHASE_DETECTOR_H_
#define PHASE_DETECTOR_H_

#include <ap_fixed.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

const int N = 32;

#define NO_ITER 16

typedef int   coef_t;
typedef float data_t;
typedef float acc_t;

// AXI-Stream packet: float data + last bit (TLAST)
typedef hls::axis<float, 0, 0, 0> DTYPE;

// per-sample core (same math as the graded scalar design)
void phasedetector_core(data_t I, data_t Q, data_t *R, data_t *Theta);

void cordiccart2pol(data_t x, data_t y, data_t *r, data_t *theta);

void fir(data_t I, data_t Q, data_t *X, data_t *Y);

// streaming top level for the PYNQ demo
void phasedetector(hls::stream<DTYPE> &I, hls::stream<DTYPE> &Q,
                   hls::stream<DTYPE> &R, hls::stream<DTYPE> &Theta,
                   int length);

#endif
