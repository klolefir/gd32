#include <gd32e10x.h>
#include "gpio.h"
#include "usart.h"
#include "systick.h"
#include "gd25q16c.h"

void init();
void init_usart();
void init_spi();

usart_t usart0;

int main(void)
{
	uint8_t status;
	uint32_t id;
	init();	

	gd25q16c_read_status(&status);
	xusart_put_char(&usart0, status);

#if 0 
	gd25q16c_read_id(&id);
	xusart_put_uint32(&usart0, id);
#endif

	while(1) {
	}

	return 0;
}

void init()
{
	SystemInit();
	SystemCoreClockUpdate();

	/*xsystick_init(1000);*/
	init_usart();
	gd25q16c_init();
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
