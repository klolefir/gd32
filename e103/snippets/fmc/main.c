#include <gd32e10x.h>
#include "usart.h"
#include "fmc.h"

static void init();

static usart_t usart0;

int main(void)
{
	init();	

	const uint32_t addr = fmc_page100_addr; 
	const uint32_t write_data = 0xDEADBEEF;
	uint32_t read_data;

	xusart_put_str(&usart0, "WRITE DATA: ");
	xusart_put_uint32(&usart0, write_data);
	xusart_put_char(&usart0, '\r');

	fmc_unlock();

	fmc_page_erase(addr);
	fmc_word_program(addr, write_data);

	read_data = *((uint32_t *)addr);
	xusart_put_str(&usart0, "READ DATA: ");
	xusart_put_uint32(&usart0, read_data);
	xusart_put_char(&usart0, '\r');

	fmc_lock();

	while(1) {}

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
}
