#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef float DTYPE;
#define SIZE 1024 		/* SIZE OF DFT */

// AXI-Stream packet carrying one float; no TUSER/TID/TDEST (same as the DMA lab).
// TKEEP, TSTRB and TLAST are still present.
typedef hls::axis<DTYPE, 0, 0, 0> transPkt;

// Streaming interface for the PYNQ demo: two input streams (real, imag) and two
// output streams (real, imag), SIZE packets each, TLAST on the final packet.
void dft(hls::stream<transPkt>& real_sample, hls::stream<transPkt>& imag_sample,
         hls::stream<transPkt>& real_op, hls::stream<transPkt>& imag_op);
