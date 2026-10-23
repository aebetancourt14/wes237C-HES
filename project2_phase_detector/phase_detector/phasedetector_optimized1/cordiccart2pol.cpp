#include "phasedetector.h"

data_t Kvalues[NO_ITER] = {1,	0.500000000000000,	0.250000000000000,	0.125000000000000,	0.0625000000000000,	0.0312500000000000,	0.0156250000000000,	0.00781250000000000,	0.00390625000000000,	0.00195312500000000,	0.000976562500000000,	0.000488281250000000,	0.000244140625000000,	0.000122070312500000,	6.10351562500000e-05,	3.05175781250000e-05};

data_t angles[NO_ITER] = {0.785398163397448,	0.463647609000806,	0.244978663126864,	0.124354994546761,	0.0624188099959574,	0.0312398334302683,	0.0156237286204768,	0.00781234106010111,	0.00390623013196697,	0.00195312251647882,	0.000976562189559320,	0.000488281211194898,	0.000244140620149362,	0.000122070311893670,	6.10351561742088e-05,	3.05175781155261e-05};


void cordiccart2pol(data_t x, data_t y, data_t * r,  data_t * theta)
{
	// Write your code here
	const data_t PI       = 3.14159265358979f;
	const data_t INV_GAIN = 0.607252935f;   // 1 / K, K = 1.646760

	data_t x_cur = x;
	data_t y_cur = y;
	data_t offset_theta = 0;

	// The iterations only converge for x > 0; for x < 0 flip by 180 deg, add it back to theta
	if (x < 0)
	{
		x_cur = -x;
		y_cur = -y;
		offset_theta = (y >= 0) ? PI : -PI;
	}

	acc_t theta_acc = 0;

	for (int i = 0; i < NO_ITER; i++)
	{
		data_t x_shift = Kvalues[i] * x_cur;   // x * 2^-i
		data_t y_shift = Kvalues[i] * y_cur;   // y * 2^-i

		if (y_cur >= 0)   // above the axis: rotate clockwise
		{
			x_cur     = x_cur + y_shift;
			y_cur     = y_cur - x_shift;
			theta_acc = theta_acc + angles[i];
		}
		else              // below the axis: rotate counter-clockwise
		{
			x_cur     = x_cur - y_shift;
			y_cur     = y_cur + x_shift;
			theta_acc = theta_acc - angles[i];
		}
	}

	*r     = x_cur * INV_GAIN;
	*theta = theta_acc + offset_theta;
}
