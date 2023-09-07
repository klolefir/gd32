#include <gd32e10x.h>
#include "gpio.h"
#include "usart.h"
#include "systick.h"
#include "dac.h"

void init();
void init_usart();
void init_dac();

usart_t usart0;
dac_t dac0;

int main(void)
{
	uint32_t data;
	init();	
	xusart_put_str(&usart0, "Ok init\n");

	while(1) {
		xdac_set_data(&dac0, 1000);
		xsystick_delay_ms(200);

		data = xdac_get_data(&dac0);
		xusart_put_uint32(&usart0, data);

		xdac_set_data(&dac0, 5000);
		xsystick_delay_ms(200);

		data = xdac_get_data(&dac0);
		xusart_put_uint32(&usart0, data);
	}

	return 0;
}

void init()
{
	SystemInit();
	SystemCoreClockUpdate();

	xsystick_init(1000);
	init_usart();
	init_dac();
}

void init_dac()
{
	dac0.dac = dac_num_0;
	dac0.align = dac_align_12br;
	xdac_init(&dac0);
}

void init_usart()
{
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
}
