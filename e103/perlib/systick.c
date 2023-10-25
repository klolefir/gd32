#include "systick.h"
#include "nvic.h"

static volatile uint32_t ticks = 0;

void xsystick_init(const uint32_t freq)
{
	xsystick_set_freq(freq);
	xsystick_reset();
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	nvic_irq_enable(systick_irqn, systick_irq_prior, systick_irq_prior);
	xsystick_enable();
}

void xsystick_delay_ms(const uint32_t ms)
{
	uint32_t start = ticks;
	uint32_t end = start + ms;

	if(end < start)
		while(ticks > start) {}

	while(ticks < end) {}
}

uint32_t xsystick_get_ticks()
{
	return ticks;
}

void xsystick_set_ticks(const uint32_t new_ticks)
{
	ticks = new_ticks;
}

void xsystick_enable()
{
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void xsystick_disable()
{
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void xsystick_set_freq(const uint32_t freq)
{
	uint32_t clk = rcu_clock_freq_get(systick_rcu_bus);
	SysTick->LOAD = clk / freq;
}

void xsystick_reset()
{
	SysTick->VAL = systick_reset_val;
}

void systick_handler()
{
	uint32_t ticks = xsystick_get_ticks();
	uint32_t new_ticks = ticks - 1;
	xsystick_set_ticks(new_ticks);
}

