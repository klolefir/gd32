#include "init.h"

usart_t usart0;
tim_t usart_tim;
gpio_t debug_led;

void init_operator()
{
	usart0.usart = usart_num_0;
	usart0.baud = usart_baud_115200;
	usart0.parity = usart_par_none;
	usart0.word_len = usart_wl_8bit;
	usart0.stop_bit = usart_stb_1bit;
	usart0.tx_state = usart_tx_on;
	usart0.rx_state = usart_rx_on;
	usart0.tx_irq_state = usart_tx_irq_off;
	usart0.rx_irq_state = usart_rx_irq_off;
	xusart_init(&usart0);
}

void init_operator_timer()
{
	usart_tim.tim = tim_num_5;
	usart_tim.psc = rcu_clock_freq_get(CK_APB1) / 1000;
	usart_tim.per = 1;
	usart_tim.align = tim_align_edge;
	usart_tim.counter_dir = tim_counter_dir_up;
	usart_tim.clkdiv = tim_clkdiv_1;
	usart_tim.irq_state = tim_irq_on;
	xtim_init(&usart_tim);
}

void init_operator_led()
{
	gpio_t debug_led;
	debug_led.port = gpio_port_e;
	debug_led.pin = gpio_pin_1;
	debug_led.mode = gpio_mode_out_pp;
	debug_led.speed = gpio_ospeed_max;
	xgpio_init(&debug_led);
}
