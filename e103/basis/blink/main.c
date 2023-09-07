#include <gd32e10x.h>

#define LED_PIN GPIO_PIN_1

static void delay_ms(uint32_t);

static volatile uint32_t ticks = 0;

int main(void)
{
	SystemInit();

	SysTick_Config(1000);
	nvic_irq_enable(SysTick_IRQn, 0, 0);

	rcu_periph_clock_enable(RCU_GPIOE);
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, LED_PIN);

	while(1) {
		gpio_bit_write(GPIOE, LED_PIN, SET);
		delay_ms(10000);
		gpio_bit_write(GPIOE, LED_PIN, RESET);
		delay_ms(10000);
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
