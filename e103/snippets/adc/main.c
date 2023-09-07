#include <gd32e10x.h>
#include "gpio.h"
#include "usart.h"
#include "systick.h"
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
		xadc_enable_soft_trig(&adc0);
		xsystick_delay_ms(2000);
		data = xadc_get_data(&adc0);
		temperature = (1.47f - data * 3.3f / 4096) / (4.3f / 1000) + 25;
		data = xadc_get_data(&adc0);
		vref = data * 3.3f / 4096;
	}

	return 0;
}

void init()
{
	SystemInit();
	SystemCoreClockUpdate();

	xsystick_init(1000);
	init_usart();
	init_adc();
}

void init_adc()
{
	adc_group_t group;
	group.len = 2;
	group.sampletime = adc_sampletime_239point5;
	group.ch[0] = adc_temp;
	group.ch[1] = adc_vref;

	adc0.adc = adc_num_0;
	adc0.mode = adc_mode_free;
	adc0.res = adc_res_12bit;
	adc0.align = adc_align_lsb;
	adc0.group = group;
	adc0.scan_mode = adc_scan_mode_on;
	adc0.cont_mode = adc_cont_mode_off;
	xadc_init(&adc0);
	xadc_sensor_enable();
	xadc_enable(&adc0);
	xsystick_delay_ms(1);
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
