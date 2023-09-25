#include <gd32e10x.h>
#include "usart.h"
#include "tim.h"

static void init();

usart_t usart0;

int main(void)
{
	init();
	while(1) {
		xusart_put_char(&usart0, 'a');
		xtim_delay_ms(1000);
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

	xtim_hold_init();
}
