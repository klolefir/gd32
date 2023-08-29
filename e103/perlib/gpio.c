#include "gpio.h"

static void xgpio_rcu_init(gpio_t *gpio_set);

void xgpio_init(gpio_t *gpio_set)
{
	xgpio_rcu_init(gpio_set);
	gpio_port_t 	port 	= gpio_set->port;
	gpio_pin_t 		pin 	= gpio_set->pin;
	gpio_mode_t 	mode 	= gpio_set->mode;
	gpio_ospeed_t 	speed 	= gpio_set->speed;

	gpio_init(port, mode, speed, pin);
}

void xgpio_set(gpio_t *gpio_set)
{
	gpio_port_t port = gpio_set->port;
	gpio_pin_t pin = gpio_set->pin;
	gpio_bit_set(port, pin);
}

void xgpio_clr(gpio_t *gpio_set)
{
	gpio_port_t port = gpio_set->port;
	gpio_pin_t pin = gpio_set->pin;
	gpio_bit_reset(port, pin);
}

void xgpio_tgl(gpio_t *gpio_set)
{
	uint8_t data;
	gpio_port_t port = gpio_set->port;
	gpio_pin_t pin = gpio_set->pin;
	FlagStatus status = gpio_output_bit_get(port, pin);

	data = (status == SET) ? 0 : 1;
	gpio_bit_write(port, pin, data);
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
