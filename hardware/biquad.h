#ifndef BIQUAD_H
#define BIQUAD_H
#define N	5
#include <stdint.h>
#include "ap_fixed.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"

typedef ap_fixed<16, 12> data_t;
typedef int16_t	coef_t;
typedef ap_fixed<16, 12> acc_t;

typedef ap_axis<32,1,1,1> AXI_VAL;

#define N 5

data_t biquad(coef_t c[N], data_t new_data);

void biquad_filt (hls::stream<AXI_VAL>& y, coef_t c[N], hls::stream<AXI_VAL>& x);

#endif
