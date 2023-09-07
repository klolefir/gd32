#include <gd32e10x.h>
#include "gpio.h"

enum {
	led1_pin = gpio_pin_1
};

static void delay_ms(uint32_t);

static volatile uint32_t ticks = 0;

int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();

	SysTick_Config(SystemCoreClock / 1000);
	nvic_irq_enable(SysTick_IRQn, 0, 0);

	gpio_t led_pin;
	led_pin.port = gpio_port_e;
	led_pin.pin = led1_pin;
	led_pin.mode = gpio_mode_out_pp;
	led_pin.speed = gpio_ospeed_max;
	xgpio_init(&led_pin);

	while(1) {
		xgpio_tgl(&led_pin);
		delay_ms(1000);
	}

	return 0;
}

void systick_handler()
{
	ticks++;
}

void delay_ms(uint32_t ms)
{
	uint32_t start = ticks;
	uint32_t end = start + ms;

	if(end < start)
		while(ticks > start) {}

	while(ticks < end) {}
}
