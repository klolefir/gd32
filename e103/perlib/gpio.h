#ifndef GPIO_H_SENTRY
#define GPIO_H_SENTRY

#include <gd32e10x.h>

typedef enum {
	gpio_port_a = GPIOA,
	gpio_port_b = GPIOB,
	gpio_port_c = GPIOC,
	gpio_port_d = GPIOD,
	gpio_port_e = GPIOE
} gpio_port_t;

typedef enum {
	gpio_mode_in_analog 	= GPIO_MODE_AIN,
	gpio_mode_in_float	 	= GPIO_MODE_IN_FLOATING,
	gpio_mode_in_pd		 	= GPIO_MODE_IPD,
	gpio_mode_in_pu		 	= GPIO_MODE_IPU,
	gpio_mode_out_od	 	= GPIO_MODE_OUT_OD,
	gpio_mode_out_pp 		= GPIO_MODE_OUT_PP,
	gpio_mode_af_od 		= GPIO_MODE_AF_OD,
	gpio_mode_af_pp	 		= GPIO_MODE_AF_PP
} gpio_mode_t;

typedef enum {
	gpio_pin_0 	= GPIO_PIN_0,
	gpio_pin_1 	= GPIO_PIN_1,
	gpio_pin_2 	= GPIO_PIN_2,
	gpio_pin_3 	= GPIO_PIN_3,
	gpio_pin_4 	= GPIO_PIN_4,
	gpio_pin_5 	= GPIO_PIN_5,
	gpio_pin_6 	= GPIO_PIN_6,
	gpio_pin_7 	= GPIO_PIN_7,
	gpio_pin_8 	= GPIO_PIN_8,
	gpio_pin_9 	= GPIO_PIN_9,
	gpio_pin_10 = GPIO_PIN_10,
	gpio_pin_11 = GPIO_PIN_11,
	gpio_pin_12 = GPIO_PIN_12,
	gpio_pin_13 = GPIO_PIN_13,
	gpio_pin_14 = GPIO_PIN_14,
	gpio_pin_15 = GPIO_PIN_15
} gpio_pin_t;

typedef enum {
	gpio_ospeed_10mhz 	= GPIO_OSPEED_10MHZ,
	gpio_ospeed_2mhz 	= GPIO_OSPEED_2MHZ,
	gpio_ospeed_50mhz 	= GPIO_OSPEED_50MHZ,
	gpio_ospeed_max 	= GPIO_OSPEED_MAX
} gpio_ospeed_t;

typedef struct {
	gpio_port_t 	port;
	gpio_pin_t 		pin;
	gpio_mode_t 	mode 	: 8;
	gpio_ospeed_t 	speed 	: 4;
} gpio_t;

void xgpio_init(gpio_t *pin);
void xgpio_set(gpio_t *pin);
void xgpio_clr(gpio_t *pin);
void xgpio_tgl(gpio_t *pin);
void xgpio_set_mode(gpio_t *pin, gpio_mode_t mode);

#endif
