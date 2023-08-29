#include <gd32e10x.h>
#include "usart.h"
#include "tim.h"

static void init();
static void delay_ms(uint32_t ms);

usart_t usart0;
tim_t tim5;

int main(void)
{
	init();
	while(1) {
		xusart_put_char(&usart0, 'a');
		delay_ms(1000);
	}

	return 0;
}

void init()
{
	SystemInit();
	SystemCoreClockUpdate();

	usart0.usart = usart_num_0;
	usart0.baud = usart_baud_115200;
	usart0.parity = usart_par_none;
	usart0.word_len = usart_wl_8bit;
	usart0.stop_bit = usart_stb_1bit;
	usart0.tx_state = usart_tx_on;
	usart0.rx_state = usart_rx_off;
	usart0.rx_irq_state = usart_tx_irq_off;
	usart0.rx_irq_state = usart_rx_irq_off;
	xusart_init(&usart0);

	tim5.tim = tim_num_5;
	tim5.psc = rcu_clock_freq_get(CK_APB1) / 1000;
	tim5.per = 1;
	tim5.align = tim_align_edge;
	tim5.counter_dir = tim_counter_dir_up;
	tim5.clkdiv = tim_clkdiv_1;
	tim5.irq_state = tim_irq_on;
	xtim_init(&tim5);
}

void delay_ms(uint32_t ms)
{
	uint32_t ticks;
	uint32_t start = xtim_get_ticks(&tim5);
	uint32_t end = start + ms;

	if(end < start) {
		do {
			ticks = xtim_get_ticks(&tim5);
		} while(ticks > start);
	}

	do {
		ticks = xtim_get_ticks(&tim5);
	} while(ticks < end);
}
