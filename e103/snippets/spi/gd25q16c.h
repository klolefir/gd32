#ifndef GD25Q16C_H_SENTRY
#define GD25Q16C_H_SENTRY

#include "spi.h"

enum {
	gd25q16c_spi_num 	= spi_num_0,
	gd25q16c_spi_psc 	= spi_psc_8,
	gd25q16c_cs_port 	= gpio_port_e,
	/*gd25q16c_cs_pin 	= gpio_pin_3*/
	gd25q16c_cs_pin 	= gpio_pin_7
};

enum {
	gd25q16c_rdsr = 0x05,
	gd25q16c_rems = 0x90,
	gd25q16c_rdid = 0x9F
};

void gd25q16c_init();
void gd25q16c_select();
void gd25q16c_deselect();
void gd25q16c_read_status(uint8_t *status);
void gd25q16c_read_id(uint32_t *id);

#endif
