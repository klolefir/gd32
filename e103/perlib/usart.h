#ifndef USART_H_SENTRY
#define USART_H_SENTRY

#include "gpio.h"

enum {
	usart0_gpio_port	= gpio_port_a,
	usart0_tx_pin 		= gpio_pin_9,
	usart0_rx_pin 		= gpio_pin_10,
	usart0_irq_prior 	= 0
};

enum {
	usart1_gpio_port 	= gpio_port_a,
	usart1_tx_pin 		= gpio_pin_2,
	usart1_rx_pin 		= gpio_pin_3,
	usart1_irq_prior	= 0
};

enum {
	usart2_gpio_port 	= gpio_port_b,
	usart2_tx_pin 		= gpio_pin_10,
	usart2_rx_pin 		= gpio_pin_11,
	usart2_irq_prior	= 0
};

enum {
	usart_rx_irq	= USART_INT_RBNE,
	usart_tx_irq	= USART_INT_TBE 
};

typedef enum {
	usart_num_0 = USART0,
	usart_num_1 = USART1,
	usart_num_2 = USART2
} usart_num_t;

/* baudrate */
typedef enum {
	usart_baud_9600 	= 9600,
	usart_baud_115200 	= 115200,
	usart_baud_921600 	= 921600
} usart_baud_t;

/* parity */
typedef enum {
	usart_par_none 	= USART_PM_NONE,
	usart_par_odd 	= USART_PM_ODD,
	usart_par_even 	= USART_PM_EVEN
} usart_par_t;

/* word length */
typedef enum {
	usart_wl_8bit 	= USART_WL_8BIT,
	usart_wl_9bit 	= USART_WL_9BIT,
} usart_wl_t;

/* stop bit */
typedef enum {
	usart_stb_1bit 		= USART_STB_1BIT,
	usart_stb_0_5bit	= USART_STB_0_5BIT,
	usart_stb_2bit		= USART_STB_2BIT,
	usart_stb_1_5bit	= USART_STB_1_5BIT
} usart_stb_t;

typedef enum {
	usart_rx_off 	= USART_RECEIVE_DISABLE,
	usart_rx_on		= USART_RECEIVE_ENABLE 
} usart_rx_state_t;

typedef enum {
	usart_tx_off 	= USART_TRANSMIT_DISABLE,
	usart_tx_on 	= USART_TRANSMIT_ENABLE 
} usart_tx_state_t;

typedef enum {
	usart_rx_rdy = SET,
	usart_rx_nrdy = RESET
} usart_rx_status_t;

typedef enum {
	usart_tx_rdy = SET,
	usart_tx_nrdy = RESET
} usart_tx_status_t;

typedef enum {
	usart_rx_irq_off,
	usart_rx_irq_on
} usart_rx_irq_state_t;

typedef enum {
	usart_tx_irq_off,
	usart_tx_irq_on,
} usart_tx_irq_state_t;

typedef struct {
	usart_num_t 			usart;
	usart_baud_t			baud;
	usart_par_t				parity;
	usart_wl_t				word_len;
	usart_stb_t				stop_bit;
	usart_rx_state_t		rx_state;
	usart_rx_state_t		tx_state;
	usart_rx_irq_state_t	rx_irq_state;
	usart_tx_irq_state_t	tx_irq_state;
} usart_t;

void xusart_init(usart_t *usart_set);

void xusart_put_char(usart_t *usart_set, const char c);
void xusart_put_uint32(usart_t *usart_set, const uint32_t data);
void xusart_put_str(usart_t *usart_set, const char *str);
void xusart_put_buff(usart_t *usart_set, const void *buff, const uint32_t byte_num);
void xusart_put_current_flags(usart_t *usart_set);
void xusart_get_char(usart_t *usart_set, char *c);
//void usart_get_str(usart_t *usart_st, char *str, char term);

void xusart_enable(usart_t *usart_set);
void xusart_disable(usart_t *usart_set);

void xusart_rx_enable(usart_t *usart_set);
void xusart_rx_disable(usart_t *usart_set);

void xusart_tx_enable(usart_t *usart_set);
void xusart_tx_disable(usart_t *usart_set);

void xusart_rx_irq_enalbe(usart_t *usart_set);
void xusart_rx_irq_disable(usart_t *usart_set);

void xusart_tx_irq_enalbe(usart_t *usart_set);
void xusart_tx_irq_disable(usart_t *usart_set);

usart_rx_status_t xusart_get_rx_status(usart_t *usart_set);
usart_tx_status_t xusart_get_tx_status(usart_t *usart_set);

#endif
