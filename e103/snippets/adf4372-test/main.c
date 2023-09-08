#include <gd32e10x.h>
#include "gpio.h"
#include "usart.h"
#include "systick.h"
#include "adf4372.h"

void init();
void init_usart();

usart_t usart0;

int main(void)
{
	/*__enable_irq();*/
	uint16_t id;
	uint8_t reg;

	init();	
	/*adf4372_read_reg(0x41, &reg);*/
	/*adf4372_get_id(&id);*/
	/*xusart_put_uint32(&usart0, (uint32_t)id);*/

	while(1) {
	}

	return 0;
}

void init()
{
	SystemInit();
	SystemCoreClockUpdate();

	xsystick_init(1000000);
	init_usart();
	adf4372_init();
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
