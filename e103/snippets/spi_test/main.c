#include <gd32e10x.h>
#include "gpio.h"
#include "usart.h"
#include "systick.h"
#include "spi.h"

void init();
void init_usart();
void init_spi();
void init_cs();

usart_t usart0;
spi_t spi0;
gpio_t cs_pin;

int main(void)
{
	uint8_t c = 'a';
	init();	
	xsystick_delay_ms(1);
	xgpio_clr(&cs_pin);
	xsystick_delay_ms(1);
	xspi_sendrecv_byte(&spi0, &c);
	xsystick_delay_ms(1);
	xgpio_set(&cs_pin);

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
	init_cs();
	init_spi();
}

void init_spi()
{
	spi0.spi				= spi_num_0;
	spi0.dev_mode			= spi_dev_mode_master;
	spi0.trans_mode			= spi_trans_mode_fullduplex;
	spi0.frame_size			= spi_frame_size_8bit;
	spi0.nss				= spi_nss_soft;
	spi0.endian				= spi_endian_lsb;
	spi0.polarity			= spi_polarity_low;
	spi0.phase				= spi_phase_1edge;
	spi0.psc				= spi_psc_32;
	spi0.trans_irq_state 	= spi_trans_irq_off;
	spi0.recv_irq_state 	= spi_recv_irq_off;
	xspi_init(&spi0);
}

void init_cs()
{
	cs_pin.port 	= gpio_port_e;
	cs_pin.pin 		= gpio_pin_4;
	cs_pin.mode 	= gpio_mode_out_pp;
	cs_pin.speed 	= gpio_ospeed_max;
	xgpio_init(&cs_pin);
	xgpio_set(&cs_pin);
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
