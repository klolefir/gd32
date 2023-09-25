#include <gd32e10x.h>
#include "gpio.h"
#include "usart.h"
#include "tim.h"
#include "dac.h"
#include "adc.h"

void init();
void init_usart();
void init_adc();

usart_t usart0;
adc_t adc0;

int main(void)
{
	float temperature, vref;
	uint16_t data;
	init();

	while(1) {
#if 0
		xadc_set_ch(&adc0, adc_temp);
		xadc_enable_soft_trig(&adc0);
		xtim_delay_ms(1);
		data = xadc_get_data(&adc0);
#endif
		data = xadc_conversion(&adc0, adc_temp);
		temperature = (1.47f - data * 3.3f / 4096) / (4.3f / 1000) + 25;
#if 0
		xadc_set_ch(&adc0, adc_vref);
		xadc_enable_soft_trig(&adc0);
		xtim_delay_ms(1);
		data = xadc_get_data(&adc0);
#endif
		data = xadc_conversion(&adc0, adc_vref);
		vref = data * 3.3f / 4096;
	}

	return 0;
}

void init()
{
	SystemInit();
	SystemCoreClockUpdate();
	xtim_hold_init();
	init_usart();
	init_adc();
}

void init_adc()
{
	adc0.adc = adc_num_0;
	adc0.mode = adc_mode_free;
	adc0.res = adc_res_12bit;
	adc0.align = adc_align_lsb;
	adc0.group.len = 0;
	adc0.scan_mode = adc_scan_mode_off;
	adc0.cont_mode = adc_cont_mode_off;
	xadc_init(&adc0);
	xadc_sensor_enable();
	xadc_enable(&adc0);
	xtim_delay_ms(1);
	xadc_calib(&adc0);
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
