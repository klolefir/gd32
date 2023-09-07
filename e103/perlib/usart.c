#include "usart.h"
#include "kestring.h"
#include "gpio.h"
#include "nvic.h"

static void xusart_gpio_init(usart_t *usart_set);
static void xusart_rcu_init(usart_t *usart_set);
static void xusart_irq_init(usart_t *usart_set);
static nvic_irqn_t xusart_switch_irqn(usart_t *usart_set);
static int8_t xusart_switch_irq_prior(usart_t *usart_set);

void xusart_init(usart_t *usart_set)
{
	usart_num_t usart					= usart_set->usart;
	usart_baud_t baud					= usart_set->baud;
	usart_par_t parity					= usart_set->parity;
	usart_par_t word_len				= usart_set->word_len;
	usart_stb_t stop_bit				= usart_set->stop_bit;
	usart_tx_state_t tx_state			= usart_set->tx_state;
	usart_rx_state_t rx_state			= usart_set->rx_state;

	xusart_gpio_init(usart_set);
	xusart_rcu_init(usart_set);

	usart_deinit(usart);

	usart_baudrate_set(usart, baud);
	usart_parity_config(usart, parity);
	usart_word_length_set(usart, word_len);
	usart_stop_bit_set(usart, stop_bit);
	usart_transmit_config(usart, tx_state);
	usart_receive_config(usart, rx_state);
	xusart_irq_init(usart_set);
	usart_enable(usart);
}

void xusart_enable(usart_t *usart_set)
{
	xusart_tx_enable(usart_set);
	xusart_rx_enable(usart_set);
}

void xusart_tx_enable(usart_t *usart_set)
{
	usart_set->tx_state = usart_tx_on;
	usart_num_t usart = usart_set->usart;
	usart_transmit_config(usart, usart_tx_on);
}

void xusart_rx_enable(usart_t *usart_set)
{
	usart_set->rx_state = usart_rx_on;
	usart_num_t usart = usart_set->usart;
	usart_receive_config(usart, usart_rx_on);
}

void xusart_tx_disable(usart_t *usart_set)
{
	usart_set->tx_state = usart_tx_off;
	usart_num_t usart = usart_set->usart;
	usart_transmit_config(usart, usart_tx_off);
}

void xusart_rx_disable(usart_t *usart_set)
{
	usart_set->rx_state = usart_rx_off;
	usart_num_t usart = usart_set->usart;
	usart_receive_config(usart, usart_rx_off);
}

void xusart_tx_irq_enable(usart_t *usart_set)
{
	usart_set->tx_irq_state = usart_tx_irq_on;
	usart_num_t usart = usart_set->usart;
	usart_interrupt_enable(usart, usart_tx_irq);
}

void xusart_rx_irq_enable(usart_t *usart_set)
{
	usart_set->rx_irq_state = usart_rx_irq_on;
	usart_num_t usart = usart_set->usart;
	usart_interrupt_enable(usart, usart_rx_irq);
}

void xusart_tx_irq_disable(usart_t *usart_set)
{
	usart_set->tx_irq_state = usart_tx_irq_off;
	usart_num_t usart = usart_set->usart;
	usart_interrupt_disable(usart, usart_tx_irq);
}

void xusart_rx_irq_disable(usart_t *usart_set)
{
	usart_set->rx_irq_state = usart_rx_irq_off;
	usart_num_t usart = usart_set->usart;
	usart_interrupt_disable(usart, usart_rx_irq);
}

void xusart_put_char(usart_t *usart_set, const char c)
{
	usart_num_t usart = usart_set->usart;
	usart_tx_status_t tx_status;

	do {
		tx_status = xusart_get_tx_status(usart_set);
	} while(tx_status == usart_tx_nrdy);

	usart_data_transmit(usart, c);
}

void xusart_put_uint32(usart_t *usart_set, const uint32_t data)
{
	char data_str[6];
	kememcpy(data_str, &data, sizeof(uint32_t));
#if 0
	data_str[4] = '\r';
	data_str[5] = '\0';
#else
	data_str[4] = '\0';
#endif
	xusart_put_str(usart_set, data_str);
}

void xusart_put_str(usart_t *usart_set, const char *str)
{
	while(*str) {
		xusart_put_char(usart_set, *str);
		str++;
	}
}

void xusart_put_buff(usart_t *usart_set, const void *buff, const uint32_t byte_num)
{
	uint8_t *buff_ptr = (uint8_t *)buff;
	int i;
	for(i = 0; i < byte_num; i++)
		xusart_put_char(usart_set, buff_ptr[i]);
}

usart_tx_status_t xusart_get_tx_status(usart_t *usart_set)
{
	usart_num_t usart = usart_set->usart;
	FlagStatus status = usart_flag_get(usart, USART_FLAG_TBE);
	switch(status) {
	case SET:	return usart_tx_rdy;
	case RESET:	return usart_tx_nrdy;
	default:	return usart_tx_nrdy;
	}
}

usart_rx_status_t xusart_get_rx_status(usart_t *usart_set)
{
	usart_num_t usart = usart_set->usart;
	FlagStatus status = usart_flag_get(usart, USART_FLAG_RBNE);
	switch(status) {
	case SET:	return usart_rx_rdy;
	case RESET:	return usart_rx_nrdy;
	default:	return usart_rx_nrdy;
	}
}

void xusart_get_char(usart_t *usart_set, char *c)
{
	usart_num_t usart = usart_set->usart;
#if 0
	usart_rx_status_t rx_status;
	do {
		rx_status = xusart_get_rx_status(usart_set);
	} while(rx_status == rx_status_nrdy);
#endif
	*c = (uint8_t)usart_data_receive(usart);
}

void xusart_put_current_flags(usart_t *usart_set)
{
	usart_num_t usart = usart_set->usart;
	FlagStatus status;

	status = usart_flag_get(usart, USART_FLAG_CTS);
	if(status)
		xusart_put_str(usart_set,"USART_FLAG_CTS\r");
	status = usart_flag_get(usart, USART_FLAG_LBD);
	if(status)
		xusart_put_str(usart_set,"USART_FLAG_LBD\r");
	status = usart_flag_get(usart, USART_FLAG_TBE);
	if(status)
		xusart_put_str(usart_set,"USART_FLAG_TBE\r");
	status = usart_flag_get(usart, USART_FLAG_TC);
	if(status)
		xusart_put_str(usart_set,"USART_FLAG_TC\r");
	status = usart_flag_get(usart, USART_FLAG_RBNE);
	if(status)
		xusart_put_str(usart_set,"USART_FLAG_RBNE\r");
	status = usart_flag_get(usart, USART_FLAG_IDLE);
	if(status)
		xusart_put_str(usart_set,"USART_FLAG_IDLE\r");
	status = usart_flag_get(usart, USART_FLAG_ORERR);
	if(status)
		xusart_put_str(usart_set,"USART_FLAG_ORERR\r");
	status = usart_flag_get(usart, USART_FLAG_NERR);
	if(status)
		xusart_put_str(usart_set,"USART_FLAG_NERR\r");
	status = usart_flag_get(usart, USART_FLAG_FERR);
	if(status)
		xusart_put_str(usart_set,"USART_FLAG_FERR\r");
	status = usart_flag_get(usart, USART_FLAG_PERR);
	if(status)
		xusart_put_str(usart_set,"USART_FLAG_PERR\r");
	status = usart_flag_get(usart, USART_FLAG_BSY);
	if(status)
		xusart_put_str(usart_set, "USART_FLAG_BSY\r");
	status = usart_flag_get(usart, USART_FLAG_EB);
	if(status)
		xusart_put_str(usart_set,"USART_FLAG_EB\r");
	status = usart_flag_get(usart, USART_FLAG_RT);
	if(status)
		xusart_put_str(usart_set,"USART_FLAG_RT\r");
	status = usart_flag_get(usart, USART_FLAG_EPERR);
	if(status)
		xusart_put_str(usart_set, "USART_FLAG_EPERR\r");
}

void xusart_gpio_init(usart_t *usart_set)
{
	gpio_t tx_pin, rx_pin;
	gpio_port_t tx_port, rx_port;
	gpio_pin_t tx_pin_num, rx_pin_num;

	usart_num_t usart = usart_set->usart;

	tx_pin.mode 	= gpio_mode_af_pp;
	tx_pin.speed 	= gpio_ospeed_50mhz;
	/*rx_pin.mode 	= gpio_mode_af_pp;*/
	rx_pin.mode 	= gpio_mode_in_float;
	rx_pin.speed 	= gpio_ospeed_50mhz;

	switch(usart) {
	case usart_num_0:	tx_port 	= usart0_gpio_port;
						rx_port 	= usart0_gpio_port;
						tx_pin_num 	= usart0_tx_pin;
						rx_pin_num 	= usart0_rx_pin;
						break;
	case usart_num_1:	tx_port 	= usart1_gpio_port;
						rx_port 	= usart1_gpio_port;
						tx_pin_num 	= usart1_tx_pin;
						rx_pin_num 	= usart1_rx_pin;
						break;
	case usart_num_2:	tx_port 	= usart2_gpio_port;
						rx_port 	= usart2_gpio_port;
						tx_pin_num 	= usart2_tx_pin;
						rx_pin_num 	= usart2_rx_pin;
						break;
	default:			return;
	}

	tx_pin.port = tx_port;
	tx_pin.pin = tx_pin_num;
	rx_pin.port = rx_port;
	rx_pin.pin = rx_pin_num;

	xgpio_init(&rx_pin);
	xgpio_init(&tx_pin);
}

void xusart_rcu_init(usart_t *usart_set)
{
	usart_num_t usart = usart_set->usart;
	switch(usart) {
	case usart_num_0:	rcu_periph_clock_enable(RCU_USART0);
						break;
	case usart_num_1:	rcu_periph_clock_enable(RCU_USART1);
						break;
	case usart_num_2:	rcu_periph_clock_enable(RCU_USART2);
						break;
	}
}

void xusart_irq_init(usart_t *usart_set)
{
	usart_num_t usart					= usart_set->usart;
	usart_tx_irq_state_t tx_irq_state 	= usart_set->tx_irq_state;
	usart_rx_irq_state_t rx_irq_state 	= usart_set->rx_irq_state;
	int8_t usart_irq_prior 				= xusart_switch_irq_prior(usart_set);
	nvic_irqn_t usart_irqn 				= xusart_switch_irqn(usart_set);

	nvic_irq_enable(usart_irqn, usart_irq_prior, usart_irq_prior);

	switch(tx_irq_state) {
	case usart_tx_irq_off:	usart_interrupt_disable(usart, usart_tx_irq);
							break;
	case usart_tx_irq_on:	usart_interrupt_enable(usart, usart_tx_irq);
							break;
	};
	
	switch(rx_irq_state) {
	case usart_rx_irq_off:	usart_interrupt_disable(usart, usart_rx_irq);
							break;
	case usart_rx_irq_on:	usart_interrupt_enable(usart, usart_rx_irq);
							break;
	};
}

nvic_irqn_t xusart_switch_irqn(usart_t *usart_set)
{
	usart_num_t usart = usart_set->usart;
	switch(usart) {
	case usart_num_0:	return usart0_irqn;
	case usart_num_1: 	return usart1_irqn;
	case usart_num_2: 	return usart2_irqn;
	default:			return nvic_none_irqn;
	}
}

int8_t xusart_switch_irq_prior(usart_t *usart_set)
{
	usart_num_t usart = usart_set->usart;
	switch(usart) {
	case usart_num_0:	return usart0_irq_prior;
	case usart_num_1: 	return usart1_irq_prior;
	case usart_num_2: 	return usart2_irq_prior;
	default:			return nvic_none_irq_prior;
	}
}
