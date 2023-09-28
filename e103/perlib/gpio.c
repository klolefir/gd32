#include "gpio.h"

static void xgpio_rcu_init(gpio_t *gpio_set);

void xgpio_init(gpio_t *gpio_set)
{
	xgpio_rcu_init(gpio_set);
	gpio_init(gpio_set->port, gpio_set->mode, gpio_set->speed, gpio_set->pin);
}

void xgpio_set(gpio_t *gpio_set)
{
	gpio_bit_set(gpio_set->port, gpio_set->pin);
}

void xgpio_clr(gpio_t *gpio_set)
{
	gpio_bit_reset(gpio_set->port, gpio_set->pin);
}

void xgpio_set_mode(gpio_t *gpio_set, gpio_mode_t mode)
{
	gpio_set->mode = mode;
	gpio_init(gpio_set->port, gpio_set->mode, gpio_set->speed, gpio_set->pin);
}

void xgpio_tgl(gpio_t *gpio_set)
{
	uint8_t data;
	FlagStatus status = gpio_output_bit_get(gpio_set->port, gpio_set->pin);
	data = (status == SET) ? 0 : 1;
	gpio_bit_write(gpio_set->port, gpio_set->pin, data);
}

void xgpio_sw(gpio_t *gpio_set, gpio_state_t state)
{
	switch(state) {
	case gpio_state_on:		xgpio_set(gpio_set);
							break;

	case gpio_state_off:	xgpio_clr(gpio_set);
							break;
	}
}

void xgpio_rcu_init(gpio_t *gpio_set)
{
	switch(gpio_set->port) {
	case GPIOA:	rcu_periph_clock_enable(RCU_GPIOA);
				break;
	case GPIOB:	rcu_periph_clock_enable(RCU_GPIOB);
				break;
	case GPIOC:	rcu_periph_clock_enable(RCU_GPIOC);
				break;
	case GPIOD:	rcu_periph_clock_enable(RCU_GPIOD);
				break;
	case GPIOE:	rcu_periph_clock_enable(RCU_GPIOE);
				break;
	}
}
