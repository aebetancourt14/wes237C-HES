/*
	Filename: phasedetector.cpp
		Phase detector

	INPUT:
		I: signal for I sample
		Q: signal for Q sample

	OUTPUT:
		R: Radius
		Theta: Angle
*/

#include "phasedetector.h"

void phasedetector (
  data_t I,
  data_t Q,

  data_t *R,
  data_t *Theta
  ){

	data_t X, Y;

	// complex matched filter, then Cartesian to polar
	fir(I, Q, &X, &Y);
	cordiccart2pol(X, Y, R, Theta);

}
