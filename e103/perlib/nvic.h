#ifndef NVIC_H_SENTRY
#define NVIC_H_SENTRY

#include "gd32e10x.h"

enum {
	nvic_none_irq_prior = 0
};

typedef enum {
	nvic_none_irqn  	= 0,
	systick_irqn		= SysTick_IRQn,
	exti0_irqn			= EXTI0_IRQn,
	exti1_irqn			= EXTI1_IRQn,
	exti2_irqn			= EXTI2_IRQn,
	exti3_irqn			= EXTI3_IRQn,
	exti4_irqn			= EXTI4_IRQn,
	adc_irqn			= ADC0_1_IRQn,
	tim2_irqn			= TIMER2_IRQn,
	tim3_irqn			= TIMER3_IRQn,
	tim5_irqn			= TIMER5_IRQn,
	tim6_irqn			= TIMER6_IRQn,
	tim7_irqn			= TIMER7_UP_TIMER12_IRQn,
	usart0_irqn			= USART0_IRQn,
	usart1_irqn			= USART1_IRQn,
	usart2_irqn			= USART2_IRQn,
	spi1_irqn		  	= SPI1_IRQn,
	spi2_irqn		  	= SPI2_IRQn
} nvic_irqn_t;

#if 0 
void nvic_enable_irq(nvic_irqn_t irqn);
void nvic_disable_irq(nvic_irqn_t irqn);
void nvic_set_priority(nvic_irqn_t irqn, nvic_irq_prior_t priority);
#endif

#endif 
