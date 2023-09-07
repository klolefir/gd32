#ifndef DAC_H_SENTRY
#define DAC_H_SENTRY

#include <gd32e10x.h>

#include "gpio.h"

enum {
	dac0_gpio_port 	= gpio_port_a,
	dac0_dac_pin 	= gpio_pin_4
};

enum {
	dac1_gpio_port 	= gpio_port_a,
	dac1_dac_pin 	= gpio_pin_5
};

typedef enum {
	dac_num_0,
	dac_num_1
} dac_num_t;

typedef enum {
	dac_align_8br 	= DAC_ALIGN_8B_R,
	dac_align_12br 	= DAC_ALIGN_12B_R,
	dac_align_12bl 	= DAC_ALIGN_12B_L,
} dac_align_t;

typedef struct {
	dac_num_t dac;
	dac_align_t align;
} dac_t;

void xdac_init(dac_t *dac_set);
void xdac_enable(dac_t *dac_set);
void xdac_disable(dac_t *dac_set);
void xdac_set_align(dac_t *dac_set, dac_align_t align);
void xdac_set_data(dac_t *dac_set, uint16_t data);
uint16_t xdac_get_data(dac_t *dac_set);

#endif
