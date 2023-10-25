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
	params.repetitioncounter = 0;
	
	xtim_rcu_init(tim_set);
	timer_init(tim, &params);
	timer_update_event_enable(tim);
	xtim_irq_init(tim_set);
	timer_enable(tim);
}

void xtim_hold_init()
{
	timer_parameter_struct params;
	uint32_t freq = rcu_clock_freq_get(tim_rcu_bus) * 2;
	uint32_t psc = freq / 1000;
	uint32_t per = 1;
	tim_num_t tim 			= tim_num_6;
	params.prescaler 		= psc;
	params.period 			= per;
	params.alignedmode 		= tim_align_edge;
	params.counterdirection = tim_counter_dir_up;
	params.clockdivision 	= tim_clkdiv_1;
	params.repetitioncounter = 0;

	rcu_periph_clock_enable(RCU_TIMER6);
	nvic_irq_enable(tim6_irqn, tim6_irq_prior, tim6_irq_prior);
	timer_interrupt_enable(tim, tim_overflow_irq);
	timer_init(tim, &params);
	timer_update_event_enable(tim);
	timer_enable(tim);
}

void xtim_delay_ms(const uint32_t ms)
{
	uint32_t time = 0;
	tim6_set_ticks(time);
	while(time != ms) {
		time = tim6_get_ticks();
	}
#if 0
	uint32_t time = ms;
	while(time) {
		xtim_delay_us(1000);
		time--;
	}
#endif
}

void xtim_delay_us(const uint16_t us)
{
	uint32_t time = 0;
	tim6_set_ticks(time);
	while(time != us) {
		time = tim6_get_ticks();
	}
}

void xtim_rcu_init(const tim_t *tim_set)
{
	tim_num_t tim = tim_set->tim;
	switch(tim) {
	case tim_num_1:		rcu_periph_clock_enable(RCU_TIMER1); break;
	case tim_num_4:		rcu_periph_clock_enable(RCU_TIMER4); break;
	case tim_num_5:		rcu_periph_clock_enable(RCU_TIMER5); break;
	case tim_num_6:		rcu_periph_clock_enable(RCU_TIMER6); break;
	case tim_num_7:		rcu_periph_clock_enable(RCU_TIMER7); break;
	case tim_num_8:		rcu_periph_clock_enable(RCU_TIMER8); break;
	case tim_num_11:	rcu_periph_clock_enable(RCU_TIMER11); break;
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
	xtim_set_ticks(tim_set, 0);
}

void xtim_clear_irq(tim_t *tim_set)
{
    TIMER_INTF(tim_set->tim) = ~(uint32_t)TIMER_INT_FLAG_UP;
}

void xtim_tenable_irq(tim_t *tim_set)
{
	tim_set->irq_state = tim_irq_on;
    TIMER_DMAINTEN(tim_set->tim) |= (uint32_t)tim_overflow_irq; 
}

void xtim_disable_irq(tim_t *tim_set)
{
	tim_num_t tim = tim_set->tim;
	tim_set->irq_state = tim_irq_off;
	timer_interrupt_disable(tim, tim_overflow_irq); xtim_set_ticks(tim_set, 0);
}

void xtim_tdisable_irq(tim_t *tim_set)
{
	tim_set->irq_state = tim_irq_off;
    TIMER_DMAINTEN(tim_set->tim) &= (~(uint32_t)tim_overflow_irq); 
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
	default:		break;
	}
}

nvic_irqn_t xtim_switch_irqn(const tim_t *tim_set)
{
	tim_num_t tim = tim_set->tim;
	switch(tim) {
	case tim_num_1:		return tim1_irqn;
	case tim_num_4:		return tim4_irqn;
	case tim_num_5:		return tim5_irqn;
	case tim_num_6:		return tim6_irqn;
	case tim_num_7:		return tim7_irqn;
	case tim_num_8:		return tim8_irqn;
	case tim_num_11:	return tim11_irqn;
	default:			return nvic_none_irqn;
	}
}

int8_t xtim_switch_irq_prior(const tim_t *tim_set)
{
	tim_num_t tim = tim_set->tim;
	switch(tim) {
	case tim_num_1:		return tim1_irq_prior;
	case tim_num_4:		return tim4_irq_prior;
	case tim_num_5:		return tim5_irq_prior;
	case tim_num_6:		return tim6_irq_prior;
	case tim_num_7:		return tim7_irq_prior;
	case tim_num_8:		return tim8_irq_prior;
	case tim_num_11: 	return tim11_irq_prior;
	default:		return nvic_none_irq_prior;
	}
}
