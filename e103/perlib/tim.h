#ifndef TIM_H_SENTRY
#define TIM_H_SENTRY

#include <gd32e10x.h>

typedef enum {
	tim_num_1 = TIMER1,
	tim_num_4 = TIMER4,
	tim_num_5 = TIMER5,
	tim_num_6 = TIMER6, /* hold timer */
	tim_num_7 = TIMER7,
	tim_num_8 = TIMER8
} tim_num_t;

enum {
	tim1_irq_prior = 0,
	tim4_irq_prior = 0,
	tim5_irq_prior = 0,
	tim6_irq_prior = 0,
	tim7_irq_prior = 0,
	tim8_irq_prior = 0
};

enum {
	tim_rcu_bus = CK_APB1
};

enum {
	tim_overflow_irq = TIMER_INT_UP,
	tim_overflow_flag = TIMER_INT_FLAG_UP
};

typedef enum {
	tim_irq_off,
	tim_irq_on
} tim_irq_state_t;

typedef enum {
	tim_align_edge = TIMER_COUNTER_EDGE,
	tim_align_center_down = TIMER_COUNTER_CENTER_DOWN,
	tim_align_center_up = TIMER_COUNTER_CENTER_UP,
	tim_align_center_both = TIMER_COUNTER_CENTER_BOTH
} tim_align_t;

typedef enum {
	tim_counter_dir_down = TIMER_COUNTER_DOWN,
	tim_counter_dir_up = TIMER_COUNTER_UP
} tim_counter_dir_t;

typedef enum {
	tim_clkdiv_1 = TIMER_CKDIV_DIV1,
	tim_clkdiv_2 = TIMER_CKDIV_DIV2,
	tim_clkdiv_4 = TIMER_CKDIV_DIV4
} tim_clkdiv_t;

typedef struct {
	tim_num_t tim;
	uint32_t psc;
	uint32_t per;
	tim_align_t align;
	tim_counter_dir_t counter_dir; 
	tim_clkdiv_t clkdiv;
	tim_irq_state_t irq_state;
} tim_t;

void xtim_init(const tim_t *tim_set);
#if 0
void xtim_enable(tim_t *tim_setup);
void xtim_disable(tim_t *tim_setup);
#endif
void xtim_enable_irq(tim_t *tim_set);
void xtim_disable_irq(tim_t *tim_set);
uint32_t xtim_get_ticks(const tim_t *tim_set);
void xtim_set_ticks(const tim_t *tim_set, const uint32_t ticks);
void xtim_hold_init();
void xtim_delay_ms(const uint32_t ms);
void xtim_delay_us(const uint16_t us);

#endif
