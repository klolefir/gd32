#ifndef SYSTICK_H_SENTRY
#define SYSTICK_H_SENTRY

#include "gd32e10x.h"

enum {
	systick_rcu_bus 	= CK_SYS,
	systick_irq_prior 	= 0,
	systick_reset_val 	= 0
};

void xsystick_init(const uint32_t freq);
void xsystick_set_freq(const uint32_t freq);
void xsystick_delay_ms(const uint32_t ms);
void xsystick_delay_us(const uint32_t us);
void xsystick_enable();
void xsystick_disable();
void xsystick_reset();
uint32_t xsystick_get_ticks();
void xsystick_set_ticks(const uint32_t new_ticks);

#endif
