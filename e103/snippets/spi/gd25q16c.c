#include "gd25q16c.h"

#include "gpio.h"
#include "spi.h"

static void gd25q16c_spi_init();
static void gd25q16c_cs_init();

static gpio_t cs_pin;
static spi_t spi_flash;

void gd25q16c_init()
{
	gd25q16c_spi_init();
	gd25q16c_cs_init();
}

void gd25q16c_select()
{
	xgpio_clr(&cs_pin);
}

void gd25q16c_deselect()
{
	xgpio_set(&cs_pin);
}

void gd25q16c_read_status(uint8_t *status)
{
	gd25q16c_select();
	uint8_t reg = gd25q16c_rdsr;
	xspi_sendrecv_byte(&spi_flash, &reg);
	gd25q16c_deselect();
	*status = reg;
}

void gd25q16c_read_id(uint32_t *id)
{
	uint8_t reg = gd25q16c_rdid;
	uint32_t temp0 = 0x00, temp1 = 0x00, temp2 = 0x00;
	gd25q16c_select();
	xspi_sendrecv_byte(&spi_flash, &reg);
	xspi_sendrecv_byte(&spi_flash, (uint8_t *)&temp0);
	xspi_sendrecv_byte(&spi_flash, (uint8_t *)&temp1);
	xspi_sendrecv_byte(&spi_flash, (uint8_t *)&temp2);
	gd25q16c_deselect();
	*id = (temp0 << 16) | (temp1 << 8) | (temp2);
}

void gd25q16c_spi_init()
{
	spi_flash.spi				= gd25q16c_spi_num;
	spi_flash.dev_mode			= spi_dev_mode_master;
	spi_flash.trans_mode		= spi_trans_mode_fullduplex;
	spi_flash.frame_size		= spi_frame_size_8bit;
	spi_flash.nss				= spi_nss_soft;
	spi_flash.endian			= spi_endian_msb;
	spi_flash.polarity			= spi_polarity_low;
	spi_flash.phase				= spi_phase_1edge;
	spi_flash.psc				= gd25q16c_spi_psc;
	spi_flash.trans_irq_state 	= spi_trans_irq_off;
	spi_flash.recv_irq_state 	= spi_recv_irq_off;
	xspi_init(&spi_flash);
}

void gd25q16c_cs_init()
{
	cs_pin.port 	= gd25q16c_cs_port;
	cs_pin.pin 		= gd25q16c_cs_pin;
	cs_pin.mode 	= gpio_mode_out_pp;
	cs_pin.speed 	= gpio_ospeed_50mhz;
	xgpio_init(&cs_pin);
	xgpio_set(&cs_pin);
}
