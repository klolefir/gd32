#ifndef ADF4372_H_SENTRY
#define ADF4372_H_SENTRY

#include "spi.h"
#include "gpio.h"

enum {
	adf4372_cs_port = gpio_port_e,
	adf4372_cs_pin 	= gpio_pin_7,
	adf4372_spi_num = spi_num_0,
	adf4372_spi_psc = spi_psc_32
};

enum {
	adf4372_reg0000 = 0x0000,
	adf4372_reg0001 = 0x0001,
	adf4372_reg0003 = 0x0003,
	adf4372_reg0004 = 0x0004,
	adf4372_reg0005 = 0x0005
};

void adf4372_init();
void adf4372_get_id(uint16_t *id);
void adf4372_read_reg(const uint8_t addr, uint8_t *reg);
void adf4372_select();
void adf4372_deselect();

#endif
