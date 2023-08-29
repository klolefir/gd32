#include "tim_hd.h"
#include "tim.h"

static volatile uint32_t tim5_ticks = 0;
static volatile uint32_t tim6_ticks = 0;
static volatile uint32_t tim7_ticks = 0;

uint32_t tim5_get_ticks()
{
	return tim5_ticks;
}

void tim5_set_ticks(const uint32_t ticks)
{
	tim5_ticks = ticks;
}

uint32_t tim6_get_ticks()
{
	return tim6_ticks;
}

void tim6_set_ticks(const uint32_t ticks)
{
	tim6_ticks = ticks;
}

uint32_t tim7_get_ticks()
{
	return tim7_ticks;
}

void tim7_set_ticks(const uint32_t ticks)
{
	tim7_ticks = ticks;
}

void tim5_handler()
{
	tim_num_t tim = tim_num_5;
	FlagStatus status = timer_interrupt_flag_get(tim, tim_overflow_flag);
	if(status) {
		uint32_t ticks = tim5_get_ticks();
		uint32_t new_ticks = ticks + 1;
		tim5_set_ticks(new_ticks);
		timer_interrupt_flag_clear(tim, tim_overflow_flag);
	}
}

void tim6_handler()
{
	tim_num_t tim = tim_num_6;
	FlagStatus status = timer_interrupt_flag_get(tim, tim_overflow_flag);
	if(status) {
		uint32_t ticks = tim6_get_ticks();
		uint32_t new_ticks = ticks + 1;
		tim6_set_ticks(new_ticks);
		timer_interrupt_flag_clear(tim, tim_overflow_flag);
	}
}

void tim7_upd_handler()
{
	tim_num_t tim = tim_num_7;
	FlagStatus status = timer_interrupt_flag_get(tim, tim_overflow_flag);
	if(status) {
		uint32_t ticks = tim7_get_ticks();
		uint32_t new_ticks = ticks + 1;
		tim6_set_ticks(new_ticks);
		timer_interrupt_flag_clear(tim, tim_overflow_flag);
	}
}

