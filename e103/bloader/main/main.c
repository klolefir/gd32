#include <gd32e10x.h>
#include "gpio.h"
#include "tim.h"

void init();
void init_gpio();

gpio_t led;

int main(void)
{
	init();
	__enable_irq();

	while(1) {
		xgpio_tgl(&led);
		xtim_delay_ms(1000);
	}

	return 0;
}

void init()
{
	SystemInit();
	SystemCoreClockUpdate();
	init_gpio();
	xtim_hold_init();
}

void init_gpio()
{
	led.port = gpio_port_e;
	led.pin = gpio_pin_1;
	led.mode = gpio_mode_out_pp;
	led.speed = gpio_ospeed_max;
	xgpio_init(&led);
}
