/*
	Filename: fir.cpp
		Complex FIR or Match filter
		firI1 and firI2 share coef_t c[N]
		firQ1 and firQ2 share coef_t c[N]
		
	INPUT:
		I: signal for I sample
		I: signal for Q sample

	OUTPUT:
		X: filtered output
		Y: filtered output

*/

#include "phasedetector.h"

void firI1 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {1,    -1,    1,    -1,    -1,    -1,    1,    1,    -1,    -1,    -1,    1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    1,    1,    1,    1,    -1,    -1,    1,    1,    1,    -1,    -1,    -1};

	// Write your code here
	static data_t shift_reg[N]; // Shift register to store the last N input samples

	acc_t acc = 0; // Accumulator for the FIR filter output
	for (int i = N - 1; i > 0; i--) 
	{
		shift_reg[i] = shift_reg[i - 1]; // Shift the samples in the register
		acc += shift_reg[i] * c[i];    // Multiply and accumulate
	}
	shift_reg[0] = x; // Load the new input sample
	acc += x*c[0]; 	  // Multiply and accumulate the new sample
	*y = acc; 		  // Output the filtered result
	
}


void firI2 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {1,    -1,    1,    -1,    -1,    -1,    1,    1,    -1,    -1,    -1,    1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    1,    1,    1,    1,    -1,    -1,    1,    1,    1,    -1,    -1,    -1};

	// Write your code here
	static data_t shift_reg[N]; // Shift register to store the last N input samples

	acc_t acc = 0; // Accumulator for the FIR filter output
	for (int i = N - 1; i > 0; i--) 
	{
		shift_reg[i] = shift_reg[i - 1]; // Shift the samples in the register
		acc += shift_reg[i] * c[i];    // Multiply and accumulate
	}
	shift_reg[0] = x; // Load the new input sample
	acc += x*c[0]; 	  // Multiply and accumulate the new sample
	*y = acc; 		  // Output the filtered result	
}




void firQ1 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {-1,    -1,    1,    -1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    -1,    1,    -1,    1,    1,    -1,    1,    -1,    -1,    1,    -1,    1,    1,    1,    1,    -1,    1,    -1,    1,    1};


	// Write your code here
	static data_t shift_reg[N]; // Shift register to store the last N input samples

	acc_t acc = 0; // Accumulator for the FIR filter output
	for (int i = N - 1; i > 0; i--) 
	{
		shift_reg[i] = shift_reg[i - 1]; // Shift the samples in the register
		acc += shift_reg[i] * c[i];    // Multiply and accumulate
	}
	shift_reg[0] = x; // Load the new input sample
	acc += x*c[0]; 	  // Multiply and accumulate the new sample
	*y = acc; 		  // Output the filtered result
}

void firQ2 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {-1,    -1,    1,    -1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    -1,    1,    -1,    1,    1,    -1,    1,    -1,    -1,    1,    -1,    1,    1,    1,    1,    -1,    1,    -1,    1,    1};

	// Write your code here
	static data_t shift_reg[N]; // Shift register to store the last N input samples

	acc_t acc = 0; // Accumulator for the FIR filter output
	for (int i = N - 1; i > 0; i--) 
	{
		shift_reg[i] = shift_reg[i - 1]; // Shift the samples in the register
		acc += shift_reg[i] * c[i];    // Multiply and accumulate
	}
	shift_reg[0] = x; // Load the new input sample
	acc += x*c[0]; 	  // Multiply and accumulate the new sample
	*y = acc; 		  // Output the filtered result
}


void fir (
  data_t I,
  data_t Q,

  data_t *X,
  data_t *Y
  ) {

	// Write your code here
	data_t II, QI, IQ, QQ;

	firI1 (&II, I);
	firI2 (&QI,Q);
	firQ1 (&IQ, I);
	firQ2 (&QQ, Q);

	//Calculating X
	*X =  II + QQ;

	//Calculating Y
	*Y = QI - IQ;

}


