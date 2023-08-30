#include <gd32e10x.h>
#include <gd32e10x_eval.h>
#include "gpio.h"
#include "usart.h"
#include "systick.h"

int main(void)
{
	char in_data[12] = {
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		0xFF,
		0xFF,
		0xFF
	};
	uint32_t out_data;
	uint32_t dead_beef = 0xEFBEADDE;
	uint32_t i;
	usart_t usart0;

	SystemInit();
	SystemCoreClockUpdate();

	xsystick_init(1000);

	usart0.usart = usart_num_0;
	usart0.baud = usart_baud_115200;
	usart0.parity = usart_par_none;
	usart0.word_len = usart_wl_8bit;
	usart0.stop_bit = usart_stb_1bit;
	usart0.tx_state = usart_tx_on;
	usart0.rx_state = usart_rx_off;
	usart0.tx_irq_state = usart_tx_irq_off;
	usart0.rx_irq_state = usart_rx_irq_off;
	xusart_init(&usart0);

	rcu_periph_clock_enable(RCU_CRC);
#if 0
	for(i = 1; i <= 9; i++)
		in_data[i - 1] = i;
	in_data[9] = 0xFF;
	in_data[10] = 0xFF;
	in_data[11] = 0xFF;
#endif


	/*out_data = crc_single_data_calculate((uint32_t *)in_data);*/
	out_data = crc_block_data_calculate((uint32_t *)in_data, 3);
	xusart_put_uint32(&usart0, out_data);

	while(1) {
	}

	return 0;
}
