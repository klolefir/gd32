#include <gd32e10x.h>

#define LED_PIN GPIO_PIN_1

static void delay_ms(uint32_t);

static volatile uint32_t ticks = 0;

int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();

#if 1
	SysTick_Config(SystemCoreClock / 1000);
	nvic_irq_enable(SysTick_IRQn, 0, 0);
#endif

	rcu_periph_clock_enable(RCU_GPIOE);
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, LED_PIN);

	uint32_t usart = USART0;
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_9);
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_10);

	rcu_periph_clock_enable(RCU_USART0);
	usart_baudrate_set(USART0, 115200);
	usart_parity_config(USART0, USART_PM_NONE);
	usart_word_length_set(USART0, USART_WL_8BIT);
	usart_stop_bit_set(USART0, USART_STB_1BIT);
	usart_enable(USART0);
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);

	while(1) {
		usart_data_transmit(usart, 'a');
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
