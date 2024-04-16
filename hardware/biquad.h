#ifndef BIQUAD_H
#define BIQUAD_H
#define N	5
#include <stdint.h>
#include <ap_fixed.h>

typedef ap_fixed<16, 12> data_t;
typedef ap_fixed<16, 1>	coef_t;
typedef ap_fixed<16, 12> acc_t;

#define N 5


data_t biquad(coef_t c[N], data_t new_data);

#endif
