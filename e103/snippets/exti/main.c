#include <gd32e10x.h>
#include "gpio.h"

void init();


int main(void)
{
	gpio_t exti_pin;
	exti_pin.port = gpio_port_b;
	exti_pin.pin = gpio_pin_14;
	exti_pin.mode = gpio_mode_in_float;
	exti_pin.speed = gpio_ospeed_max;
	xgpio_init(&exti_pin);
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, GPIO_PIN_SOURCE_9);


	while(1)
	{}
	return 0;
}

void init()
{
	SystemInit();
	SystemCoreClockUpdate();
}
