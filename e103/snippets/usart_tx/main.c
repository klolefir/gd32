#include <gd32e10x.h>
#include "gpio.h"
#include "usart.h"
#include "systick.h"

int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();

	xsystick_init(1000);
	
	usart_t usart0;
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

	while(1) {
		xusart_put_char(&usart0, 'a');
		xsystick_delay_ms(1000);
	}

	return 0;
}
