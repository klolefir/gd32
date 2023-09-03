#ifndef SPI_H_SENTRY
#define SPI_H_SENTRY

#include <gd32e10x.h>
#include "gpio.h"

enum {
	spi0_gpio_port 	= gpio_port_a,
	spi0_nss_pin	= gpio_pin_4,
	spi0_sck_pin	= gpio_pin_5,
	spi0_miso_pin	= gpio_pin_6,
	spi0_mosi_pin	= gpio_pin_7,
	spi0_irq_prior	= 0
};

enum {
	spi1_gpio_port	= gpio_port_b,
	spi1_nss_pin	= gpio_pin_12,
	spi1_sck_pin	= gpio_pin_13,
	spi1_miso_pin	= gpio_pin_14,
	spi1_mosi_pin	= gpio_pin_15,
	spi1_irq_prior	= 0
};

enum {
	spi2_nss_gpio_port	= gpio_port_a,
	spi2_gpio_port		= gpio_port_b,
	spi2_nss_pin		= gpio_pin_15,
	spi2_sck_pin		= gpio_pin_3,
	spi2_miso_pin		= gpio_pin_4,
	spi2_mosi_pin		= gpio_pin_5,
	spi2_irq_prior		= 0
};

enum {
	spi_trans_irq 		= SPI_I2S_INT_TBE,
	spi_recv_irq		= SPI_I2S_INT_RBNE,
	spi_trans_irq_flag 	= SPI_I2S_INT_FLAG_TBE,
	spi_recv_irq_flag	= SPI_I2S_INT_FLAG_RBNE,
	spi_trans_flag		= SPI_FLAG_TBE,
	spi_recv_flag 		= SPI_FLAG_RBNE
};

typedef enum {
	spi_num_0 = SPI0,
	spi_num_1 = SPI1,
	spi_num_2 = SPI2
} spi_num_t;

typedef enum {
	spi_dev_mode_slave 	= SPI_SLAVE,
	spi_dev_mode_master = SPI_MASTER
} spi_dev_mode_t;

typedef enum {
	spi_trans_mode_fullduplex	= SPI_TRANSMODE_FULLDUPLEX,
	spi_trans_mode_receiveonly	= SPI_TRANSMODE_RECEIVEONLY,
	spi_trans_mode_bdreceive	= SPI_TRANSMODE_BDRECEIVE,
	spi_trans_mode_bdtransmit	= SPI_TRANSMODE_BDTRANSMIT
} spi_trans_mode_t;

typedef enum {
	spi_frame_size_16bit 	= SPI_FRAMESIZE_16BIT,
	spi_frame_size_8bit		= SPI_FRAMESIZE_8BIT
} spi_frame_size_t;

typedef enum {
	spi_nss_soft = SPI_NSS_SOFT,
	spi_nss_hard = SPI_NSS_HARD
} spi_nss_t;

typedef enum {
	spi_endian_msb = SPI_ENDIAN_MSB,
	spi_endian_lsb = SPI_ENDIAN_LSB
} spi_endian_t;

typedef enum {
	spi_polarity_low 	= SPI_CK_PL_LOW_PH_1EDGE,
	spi_polarity_high 	= SPI_CK_PL_HIGH_PH_1EDGE
} spi_polarity_t;

typedef enum {
	spi_phase_1edge = SPI_CK_PL_LOW_PH_1EDGE,
	spi_phase_2edge = SPI_CK_PL_LOW_PH_2EDGE
} spi_phase_t;

typedef enum {
	spi_psc_2 	= SPI_PSC_2,
	spi_psc_4 	= SPI_PSC_4,
	spi_psc_8 	= SPI_PSC_8,
	spi_psc_16 	= SPI_PSC_16,
	spi_psc_32 	= SPI_PSC_32,
	spi_psc_64 	= SPI_PSC_64,
	spi_psc_128 = SPI_PSC_128,
	spi_psc_256 = SPI_PSC_256,
} spi_psc_t;

typedef enum {
	spi_trans_rdy 	= SET,
	spi_trans_nrdy 	= RESET
} spi_trans_status_t;

typedef enum {
	spi_recv_rdy 	= SET,
	spi_recv_nrdy 	= RESET
} spi_recv_status_t;

typedef enum {
	spi_trans_irq_off,
	spi_trans_irq_on
} spi_trans_irq_state_t;


typedef enum {
	spi_recv_irq_off,
	spi_recv_irq_on
} spi_recv_irq_state_t;

typedef struct {
	spi_num_t				spi;
	spi_dev_mode_t			dev_mode;
	spi_trans_mode_t		trans_mode;
	spi_frame_size_t		frame_size;
	spi_nss_t				nss;
	spi_endian_t			endian;
	spi_polarity_t			polarity;
	spi_phase_t				phase;
	spi_psc_t				psc;
	spi_trans_irq_state_t 	trans_irq_state;	
	spi_recv_irq_state_t 	recv_irq_state;	
} spi_t;

void xspi_init(spi_t *spi_set);
void xspi_enable(spi_t *spi_set);
void xspi_disable(spi_t *spi_set);
void xspi_send_byte(spi_t *spi_set, uint8_t byte);
void xspi_recv_byte(spi_t *spi_set, uint8_t *byte);
void xspi_sendrecv_byte(spi_t *spi_set, uint8_t *byte);
spi_trans_status_t xspi_get_trans_status(spi_t *spi_set);
spi_recv_status_t xspi_get_recv_status(spi_t *spi_set);

#endif
