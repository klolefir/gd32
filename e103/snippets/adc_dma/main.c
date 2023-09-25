#include <gd32e10x.h>
#include "gpio.h"
#include "usart.h"
#include "tim.h"
#include "dac.h"
#include "adc.h"
#include "dma.h"

void init();
void init_gpio();
void init_usart();
void init_adc();
void init_dma();

usart_t usart0;
adc_t adc0;
dma_t dma0;

int main(void)
{
	float temperature, vref;
	uint32_t *data;
	init();

	while(1) {
		data = xadc_dma_conversion(&adc0);
		temperature = (1.47f - data[0] * 3.3f / 4096) / (4.3f / 1000) + 25;
		vref = data[1] * 3.3f / 4096;
	}

	return 0;
}

void init()
{
	SystemInit();
	SystemCoreClockUpdate();
	xtim_hold_init();
	init_gpio();
	init_usart();
	init_adc();
}

void init_gpio()
{
	gpio_t adc_pin;
	adc_pin.port = gpio_port_c;
	adc_pin.pin = gpio_pin_3;
	adc_pin.mode = gpio_mode_in_analog;
	adc_pin.speed = gpio_ospeed_max;
	xgpio_init(&adc_pin);
}

#if 0
void init_dma()
{
	dma0.dma = dma_num_0;
	dma0.ch = dma_ch_adc;
	dma0.paddr = dma_paddr_adc;
	dma0.pwidth = dma_pwidth_32bit;	
	dma0.pinc = dma_pinc_disable;
	dma0.maddr = (uint32_t)measure;
	dma0.mwidth = dma_mwidth_32bit;
	dma0.minc = dma_minc_enable;
	dma0.dir = dma_dir_p2m;
	dma0.num = 3;
	dma0.prior = dma_prior_high;
	dma0.circ = dma_circ_enable;
	dma0.m2m = dma_m2m_disable;
	xdma_init(&dma0);
}
#endif

void init_adc()
{
	adc0.adc = adc_num_0;
	adc0.mode = adc_mode_free;
	adc0.res = adc_res_12bit;
	adc0.align = adc_align_lsb;
	adc0.group.len = 3;
	adc0.group.sampletime = adc_sampletime_239point5;
	adc0.group.ch[0] = adc_temp;
	adc0.group.ch[1] = adc_vref;
	adc0.group.ch[2] = adc_ch13;
	adc0.scan_mode = adc_scan_mode_on;
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
