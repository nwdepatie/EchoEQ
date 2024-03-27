#ifndef BIQUAD_H
#define BIQUAD_H
#define N	5
#include <stdint.h>

typedef int data_t;
typedef int	coef_t;
typedef int	acc_t;

#define N 5


data_t biquad(coef_t c[N], data_t new_data);

#endif
