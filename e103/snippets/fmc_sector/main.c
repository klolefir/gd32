#include <gd32e10x.h>
#include "usart.h"
#include "fmc.h"

static void init();

static usart_t usart0;

int main(void)
{
	init();

	int i;
	const uint32_t addr = fmc_page100_addr;
	const uint32_t data_size = 50;
	uint32_t write_data[data_size];
	uint32_t read_data[data_size];

	for(i = 0; i < data_size; i++)
		write_data[i] = 0xDEADBEEF;

	fmc_unlock();

	for(i = 0; i < (data_size * fmc_word_size); i++) {
		if(!(i % fmc_page_size))
			fmc_page_erase(addr + i);
	}
	fmc_write_sector(addr, write_data, data_size * fmc_word_size);
	fmc_read_sector(addr, read_data, data_size * fmc_word_size);

	xusart_put_str(&usart0, "READ DATA: \r");
	for(i = 0; i < data_size; i++) {
		xusart_put_uint32(&usart0, read_data[i]);
		xusart_put_char(&usart0, '\r');
	}

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
