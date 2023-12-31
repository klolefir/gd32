#include "tim.h"
#include "tim_hd.h"
#include "nvic.h"

static nvic_irqn_t xtim_switch_irqn(const tim_t *tim_set);
static int8_t xtim_switch_irq_prior(const tim_t *tim_set);
static void xtim_rcu_init(const tim_t *tim_set);
static void xtim_irq_init(const tim_t *tim_set);

void xtim_init(const tim_t *tim_set)
{
	timer_parameter_struct params;
	tim_num_t tim 			= tim_set->tim;
	params.prescaler 		= tim_set->psc;
	params.period 			= tim_set->per;
	params.alignedmode 		= tim_set->align;
	params.counterdirection = tim_set->counter_dir;
	params.clockdivision 	= tim_set->clkdiv;
	params.repetitioncounter = 1;
	
	xtim_rcu_init(tim_set);
	xtim_irq_init(tim_set);
	timer_init(tim, &params);
	timer_update_event_enable(tim);
	timer_enable(tim);
}

void xtim_rcu_init(const tim_t *tim_set)
{
	tim_num_t tim = tim_set->tim;
	switch(tim) {
	case tim_num_5:	rcu_periph_clock_enable(RCU_TIMER5);
					break;
	case tim_num_6:	rcu_periph_clock_enable(RCU_TIMER6);
					break;
	case tim_num_7:	rcu_periph_clock_enable(RCU_TIMER7);
					break;
	}
}

void xtim_irq_init(const tim_t *tim_set)
{
	tim_num_t tim = tim_set->tim;
	tim_irq_state_t irq_state = tim_set->irq_state;
	int8_t tim_irq_prior = xtim_switch_irq_prior(tim_set);
	nvic_irqn_t tim_irqn = xtim_switch_irqn(tim_set);

	nvic_irq_enable(tim_irqn, tim_irq_prior, tim_irq_prior);

	switch(irq_state) {
	case tim_irq_off: 	timer_interrupt_disable(tim, tim_overflow_irq);
					  	break;
	case tim_irq_on: 	timer_interrupt_enable(tim, tim_overflow_irq);
						break;
	}
}

void xtim_enable_irq(tim_t *tim_set)
{
	tim_num_t tim = tim_set->tim;
	tim_set->irq_state = tim_irq_on;
	timer_interrupt_enable(tim, tim_overflow_irq);
}

void xtim_disable_irq(tim_t *tim_set)
{
	tim_num_t tim = tim_set->tim;
	tim_set->irq_state = tim_irq_off;
	timer_interrupt_disable(tim, tim_overflow_irq);
}

uint32_t xtim_get_ticks(const tim_t *tim_set)
{
	uint32_t ticks;
	tim_num_t tim = tim_set->tim;

	switch(tim) {
	case tim_num_5:	ticks = tim5_get_ticks();
					break;
	case tim_num_6:	ticks = tim6_get_ticks();
					break;
	case tim_num_7:	ticks = tim7_get_ticks();
					break;
	default:		ticks = 0;
	}

	return ticks;
}

void xtim_set_ticks(const tim_t *tim_set, const uint32_t ticks)
{
	tim_num_t tim = tim_set->tim;

	switch(tim) {
	case tim_num_5:	tim5_set_ticks(ticks);
	case tim_num_6:	tim6_set_ticks(ticks);
	case tim_num_7:	tim7_set_ticks(ticks);
	}
}

nvic_irqn_t xtim_switch_irqn(const tim_t *tim_set)
{
	tim_num_t tim = tim_set->tim;
	switch(tim) {
	case tim_num_5:	return tim5_irqn;
	case tim_num_6:	return tim6_irqn;
	case tim_num_7: return tim7_irqn;
	default:		return nvic_none_irqn;
	}
}

int8_t xtim_switch_irq_prior(const tim_t *tim_set)
{
	tim_num_t tim = tim_set->tim;
	switch(tim) {
	case tim_num_5:	return tim5_irq_prior;	
	case tim_num_6:	return tim6_irq_prior;	
	case tim_num_7: return tim7_irq_prior;
	default:		return nvic_none_irq_prior;
	}
}
