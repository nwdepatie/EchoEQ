#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include "biquad.h"

typedef ap_axis<32,1,1,1> AXI_VAL;

data_t biquad(coef_t c[N], data_t new_data) {
	coef_t b0, b1, b2, a1, a2;
	acc_t acc;
	static data_t x_arr[N] = { 0 };
	static data_t y_arr[N] = { 0 };

	/* Load in Coefficients */
	b0=c[0]; b1=c[1]; b2=c[2]; a1=c[3]; a2=c[4];

	/* feed forward shift register */
	x_arr[2] = x_arr[1];
	x_arr[1] = x_arr[0];
	x_arr[0] = new_data;

	/* filter the signal */
	acc = b0*x_arr[0] + b1*x_arr[1] + b2*x_arr[2] - a1*x_arr[1] - a2*x_arr[2];
	y_arr[0] = acc;

	/* feedback shift register */
	y_arr[2] = y_arr[1];
	y_arr[1] = y_arr[0];
	return y_arr[0];
}

void filt (hls::stream<AXI_VAL>& y, coef_t c[N], hls::stream<AXI_VAL>& x) {
#pragma HLS INTERFACE m_axi depth=11 port=c
#pragma HLS INTERFACE axis register both port=x
#pragma HLS INTERFACE axis register both port=y
#pragma HLS INTERFACE ap_ctrl_none port=return
	// coef_t taps[N] = {0,-10,-9,23,56,63,56,23,-9,-10,0};

	while(1) {
#pragma HLS PIPELINE II=1

		AXI_VAL tmp1;
		x.read(tmp1);

		AXI_VAL output;
		output.data = biquad(c, tmp1.data);;
		output.keep = tmp1.keep;
		output.strb = tmp1.strb;
		output.last = tmp1.last;
		output.dest = tmp1.dest;
		output.id = tmp1.id;
		output.user = tmp1.user;
		y.write(output);

		if (tmp1.last) {
			break;
		}
	}
}
