#include "spi.h"
#include "gpio.h"
#include "nvic.h"

static void xspi_gpio_init(spi_t *spi_set);
static void xspi_rcu_init(spi_t *spi_set);
static void xspi_irq_init(spi_t *spi_set);
static nvic_irqn_t xspi_switch_irqn(spi_t *spi_set);
static int8_t xspi_switch_irq_prior(spi_t *spi_set);


void xspi_init(spi_t *spi_set)
{
	spi_parameter_struct params;
	spi_num_t spi 				= spi_set->spi;
	params.device_mode 			= spi_set->dev_mode;
	params.trans_mode 			= spi_set->trans_mode;
	params.frame_size 			= spi_set->frame_size;
	params.endian 				= spi_set->endian;
	params.nss 					= spi_set->nss;
	params.clock_polarity_phase = spi_set->polarity 
								| spi_set->phase;
	params.prescale 			= spi_set->psc;

	spi_i2s_deinit(spi);
	xspi_gpio_init(spi_set);
	xspi_rcu_init(spi_set);
	xspi_irq_init(spi_set);
	spi_init(spi, &params);
	spi_enable(spi);
}

void xspi_enable(spi_t *spi_set)
{
	spi_num_t spi = spi_set->spi;
	spi_enable(spi);
}

void xspi_disable(spi_t *spi_set)
{
	spi_num_t spi = spi_set->spi;
	spi_disable(spi);
}

void xspi_send_byte(spi_t *spi_set, uint8_t byte)
{
	spi_num_t spi = spi_set->spi;
	spi_trans_status_t trans_status;

	do {
		trans_status = xspi_get_trans_status(spi_set);
	} while(trans_status == spi_trans_nrdy);

	spi_i2s_data_transmit(spi, byte);
}

void xspi_recv_byte(spi_t *spi_set, uint8_t *byte)
{
	spi_num_t spi = spi_set->spi;

	spi_recv_status_t recv_status;
	do {
		recv_status = xspi_get_recv_status(spi_set);
	} while(recv_status == spi_recv_nrdy);

	*byte = spi_i2s_data_receive(spi);
}

void xspi_sendrecv_byte(spi_t *spi_set, uint8_t *byte)
{
	xspi_send_byte(spi_set, *byte);
	xspi_recv_byte(spi_set, byte);
}

spi_trans_status_t xspi_get_trans_status(spi_t *spi_set)
{
	spi_num_t spi = spi_set->spi;
	FlagStatus status = spi_i2s_flag_get(spi, spi_trans_flag);
	switch(status) {
	case SET:	return spi_trans_rdy;
	case RESET:
	default:	return spi_trans_nrdy;
	}
}

spi_recv_status_t xspi_get_recv_status(spi_t *spi_set)
{
	spi_num_t spi = spi_set->spi;
	FlagStatus status = spi_i2s_flag_get(spi, spi_recv_flag);
	switch(status) {
	case SET:	return spi_recv_rdy;
	case RESET:
	default:	return spi_recv_nrdy;
	}
}

void xspi_gpio_init(spi_t *spi_set)
{
	gpio_t mosi_pin, miso_pin, sck_pin, nss_pin;
	gpio_port_t mosi_port, miso_port, sck_port, nss_port;
	gpio_pin_t mosi_pin_num, miso_pin_num, sck_pin_num, nss_pin_num;

	spi_num_t spi = spi_set->spi;

	mosi_pin.mode 	= gpio_mode_af_pp;
	mosi_pin.speed 	= gpio_ospeed_50mhz;
	miso_pin.mode 	= gpio_mode_in_float;
	miso_pin.speed 	= gpio_ospeed_50mhz;
	sck_pin.mode 	= gpio_mode_af_pp;
	sck_pin.speed 	= gpio_ospeed_50mhz;

	switch(spi) {
	case spi_num_0: mosi_port 		= spi0_gpio_port;
					miso_port 		= spi0_gpio_port;
					sck_port		= spi0_gpio_port;
					nss_port		= spi0_gpio_port;
					mosi_pin_num 	= spi0_mosi_pin;
					miso_pin_num 	= spi0_miso_pin;
					sck_pin_num		= spi0_sck_pin;
					nss_pin_num		= spi0_nss_pin;
					break;
	case spi_num_1: mosi_port 		= spi1_gpio_port;
					miso_port 		= spi1_gpio_port;
					sck_port		= spi1_gpio_port;
					nss_port		= spi1_gpio_port;
					mosi_pin_num 	= spi1_mosi_pin;
					miso_pin_num 	= spi1_miso_pin;
					sck_pin_num		= spi1_sck_pin;
					nss_pin_num		= spi1_nss_pin;
					break;
	case spi_num_2: mosi_port 		= spi2_gpio_port;
					miso_port 		= spi2_gpio_port;
					sck_port		= spi2_gpio_port;
					nss_port		= spi2_nss_gpio_port;
					mosi_pin_num 	= spi2_mosi_pin;
					miso_pin_num 	= spi2_miso_pin;
					sck_pin_num		= spi2_sck_pin;
					nss_pin_num		= spi2_nss_pin;
					break;
	default:		return;
	}

	mosi_pin.port 	= mosi_port;
	mosi_pin.pin 	= mosi_pin_num;
	miso_pin.port 	= miso_port;
	miso_pin.pin 	= miso_pin_num;
	sck_pin.port 	= sck_port;
	sck_pin.pin 	= sck_pin_num;
	nss_pin.port 	= nss_port;
	nss_pin.pin 	= nss_pin_num;

	xgpio_init(&mosi_pin);
	xgpio_init(&miso_pin);
	xgpio_init(&sck_pin);
	if(spi_set->nss == spi_nss_hard)
		xgpio_init(&nss_pin);
}

void xspi_rcu_init(spi_t *spi_set)
{
	spi_num_t spi = spi_set->spi;
	switch(spi) {
	case spi_num_0: rcu_periph_clock_enable(RCU_SPI0);
					break;
	case spi_num_1: rcu_periph_clock_enable(RCU_SPI1);
					break;
	case spi_num_2: rcu_periph_clock_enable(RCU_SPI2);
					break;
	}
}

static void xspi_irq_init(spi_t *spi_set)
{
	spi_num_t spi = spi_set->spi;
	spi_trans_irq_state_t trans_irq_state = spi_set->trans_irq_state;
	spi_recv_irq_state_t recv_irq_state = spi_set->recv_irq_state;
	int8_t spi_irq_prior = xspi_switch_irq_prior(spi_set);
	nvic_irqn_t spi_irqn = xspi_switch_irqn(spi_set);

	nvic_irq_enable(spi_irqn, spi_irq_prior, spi_irq_prior);
	switch(trans_irq_state) {
	case spi_trans_irq_off:	spi_i2s_interrupt_disable(spi, spi_trans_irq);
							break;
	case spi_trans_irq_on:	spi_i2s_interrupt_enable(spi, spi_trans_irq);
							break;
	}

	switch(recv_irq_state) {
	case spi_recv_irq_off:	spi_i2s_interrupt_disable(spi, spi_recv_irq);
							break;
	case spi_recv_irq_on:	spi_i2s_interrupt_enable(spi, spi_recv_irq);
							break;
	}
}

static nvic_irqn_t xspi_switch_irqn(spi_t *spi_set)
{
	spi_num_t spi = spi_set->spi;
	switch(spi) {
	case spi_num_0: return spi0_irqn;
	case spi_num_1:	return spi1_irqn;
	case spi_num_2:	return spi2_irqn;
	default:		return nvic_none_irqn;
	}
}

static int8_t xspi_switch_irq_prior(spi_t *spi_set)
{
	spi_num_t spi = spi_set->spi;
	switch(spi) {
	case spi_num_0: return spi0_irq_prior;
	case spi_num_1:	return spi1_irq_prior;
	case spi_num_2:	return spi2_irq_prior;
	default:		return nvic_none_irq_prior;
	}
}
