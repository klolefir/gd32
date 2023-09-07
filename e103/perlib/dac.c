#include "dac.h"

static void xdac_gpio_init();
static void xdac_rcu_init();

void xdac_init(dac_t *dac_set)
{
	dac_num_t dac 		= dac_set->dac;

	dac_deinit();
	xdac_gpio_init(dac_set);
	xdac_rcu_init(dac_set);

	dac_enable(dac);
}

void xdac_enable(dac_t *dac_set)
{
	dac_num_t dac = dac_set->dac;
	dac_enable(dac);
}

void xdac_disable(dac_t *dac_set)
{
	dac_num_t dac = dac_set->dac;
	dac_disable(dac);
}

void xdac_set_align(dac_t *dac_set, dac_align_t align)
{
	dac_set->align = align;
}

void xdac_set_data(dac_t *dac_set, uint16_t data)
{
	dac_num_t dac = dac_set->dac;
	dac_align_t align = dac_set->align;
	dac_data_set(dac, align, data);
}

uint16_t xdac_get_data(dac_t *dac_set)
{
	uint16_t data;
	dac_num_t dac = dac_set->dac;
	data = dac_output_value_get(dac);
	return data;
}

void xdac_gpio_init(dac_t *dac_set)
{
	gpio_t dac_pin;
	gpio_port_t dac_port;
	gpio_pin_t dac_pin_num;

	dac_num_t dac = dac_set->dac;

	dac_pin.mode 	= gpio_mode_in_analog;
	dac_pin.speed 	= gpio_ospeed_50mhz;

	switch(dac) {
	case dac_num_0:	dac_port 	= dac0_gpio_port;
					dac_pin_num = dac0_dac_pin;
					break;
	case dac_num_1:	dac_port 	= dac1_gpio_port;
					dac_pin_num = dac1_dac_pin;
					break;
	default:		return;
	}

	dac_pin.port 	= dac_port;
	dac_pin.pin 	= dac_pin_num;

	xgpio_init(&dac_pin);
}

void xdac_rcu_init(dac_t *dac_set)
{
	rcu_periph_clock_enable(RCU_DAC);
}
