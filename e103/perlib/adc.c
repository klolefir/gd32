#include "adc.h"
#include "tim.h"
#include "dma.h"

static void xadc_set_mode(adc_t *adc_set, adc_mode_t mode);
static void xadc_rcu_init(adc_t *adc_set);
static void xadc_scan_mode_init(adc_t *adc_set);
static adc_conv_status_t xadc_get_conv_status(adc_t *adc_set); 
static void xadc_cont_mode_init(adc_t *adc_set);
static void xadc_dma_init(adc_t *adc_set);
static uint16_t xadc_get_data(adc_t *adc_set);

static uint32_t measurement[adc_chmax];

void xadc_init(adc_t *adc_set)
{
	adc_num_t adc = adc_set->adc;
	adc_mode_t mode = adc_set->mode;
	adc_align_t align = adc_set->align;
	adc_res_t res = adc_set->res;

	adc_deinit(adc);

	xadc_rcu_init(adc_set);

	xadc_scan_mode_init(adc_set);
	xadc_cont_mode_init(adc_set);
	adc_data_alignment_config(adc, align);
	xadc_set_mode(adc_set, mode);
	adc_resolution_config(adc, res);
	xadc_set_group(adc_set);
	adc_external_trigger_source_config(adc, adc_regular_channel, ADC0_1_EXTTRIG_REGULAR_NONE);
	adc_external_trigger_config(adc, adc_regular_channel, ENABLE);
}

void xadc_dma_init(adc_t *adc_set)
{
	dma_t dma;
	dma.dma = dma_num_0;
	dma.ch = dma_ch_adc;
	dma.paddr = dma_paddr_adc;
	dma.pwidth = dma_pwidth_32bit;	
	dma.pinc = dma_pinc_disable;
	dma.maddr = (uint32_t)measurement;
	dma.mwidth = dma_mwidth_32bit;
	dma.minc = dma_minc_enable;
	dma.dir = dma_dir_p2m;
	dma.num = adc_set->group.len;
	dma.prior = dma_prior_high;
	dma.circ = dma_circ_enable;
	dma.m2m = dma_m2m_disable;
	xdma_init(&dma);
}

void xadc_enable(adc_t *adc_set)
{
	adc_num_t adc = adc_set->adc;
	adc_enable(adc);
}

void xadc_disable(adc_t *adc_set)
{
	adc_num_t adc = adc_set->adc;
	adc_disable(adc);
}

uint32_t *xadc_dma_conversion(adc_t *adc_set)
{
	xadc_enable_soft_trig(adc_set);
	xtim_delay_ms(1);
	return measurement;
}

uint16_t xadc_conversion(adc_t *adc_set, adc_ch_t ch)
{
	uint16_t data;
	xadc_set_ch(adc_set, ch);
	xadc_enable_soft_trig(adc_set);
	xtim_delay_ms(1);
	data = xadc_get_data(adc_set);
	return data;
}

uint16_t xadc_get_data(adc_t *adc_set)
{
	uint16_t data;
	adc_conv_status_t status;
	adc_num_t adc = adc_set->adc;
	do {
		status = xadc_get_conv_status(adc_set);
	} while(status != adc_conv_rdy);
	data = adc_regular_data_read(adc);
	adc_flag_clear(adc_set->adc, adc_conv_flag);
	return data;
}

void xadc_enable_soft_trig(adc_t *adc_set)
{
	adc_num_t adc = adc_set->adc;
	adc_software_trigger_enable(adc, adc_regular_channel);
}

void xadc_sensor_enable()
{
	adc_tempsensor_vrefint_enable();
}

void xadc_sensor_disable()
{
	adc_tempsensor_vrefint_disable();
}

void xadc_calib(adc_t *adc_set)
{
	adc_num_t adc = adc_set->adc;
	adc_calibration_enable(adc);
}

void xadc_enable_scan_mode(adc_t *adc_set)
{
	adc_num_t adc = adc_set->adc;
	adc_set->scan_mode = adc_scan_mode_on;
	adc_special_function_config(adc, adc_scan_mode, ENABLE);
}

void xadc_disable_scan_mode(adc_t *adc_set)
{
	adc_num_t adc = adc_set->adc;
	adc_set->scan_mode = adc_scan_mode_off;	
	adc_special_function_config(adc, adc_scan_mode, DISABLE);
}

void xadc_enable_cont_mode(adc_t *adc_set)
{
	adc_num_t adc = adc_set->adc;
	adc_set->cont_mode = adc_cont_mode_on;	
	adc_special_function_config(adc, adc_cont_mode, ENABLE);
}

void xadc_disable_cont_mode(adc_t *adc_set)
{
	adc_num_t adc = adc_set->adc;
	adc_set->cont_mode = adc_cont_mode_off;	
	adc_special_function_config(adc, adc_cont_mode, DISABLE);
}

void xadc_set_mode(adc_t *adc_set, adc_mode_t mode)
{
	adc_num_t adc = adc_set->adc;
	adc_set->mode = mode;

    ADC_CTL0(adc) &= ~(ADC_CTL0_SYNCM);
    ADC_CTL0(adc) |= mode;
}

void xadc_rcu_init(adc_t *adc_set)
{
	adc_num_t adc = adc_set->adc;
	switch(adc) {
	case adc_num_0: rcu_periph_clock_enable(RCU_ADC0);
					break;
	case adc_num_1: rcu_periph_clock_enable(RCU_ADC1);
					break;
	}
	rcu_adc_clock_config(adc_rcu_div);
}

void xadc_scan_mode_init(adc_t *adc_set)
{
	adc_scan_mode_state_t scan_mode = adc_set->scan_mode;
	switch(scan_mode) {
	case adc_scan_mode_off:	xadc_disable_scan_mode(adc_set);
							break;
	case adc_scan_mode_on:	xadc_enable_scan_mode(adc_set);
							break;
	}
}

void xadc_set_ch(adc_t *adc_set, adc_ch_t ch)
{
	adc_regular_channel_config(adc_set->adc, 0, ch, adc_sampletime_239point5);
}

adc_conv_status_t xadc_get_conv_status(adc_t *adc_set)
{
	adc_conv_status_t status = adc_flag_get(adc_set->adc, adc_conv_flag);
	return status;
}

void xadc_set_group(adc_t *adc_set)
{
	uint32_t i;
	uint8_t rank;
	adc_num_t adc;
	adc_group_len_t len;
	adc_sampletime_t sampletime;

	adc = adc_set->adc;
	len = adc_set->group.len;
	if(!len) {
		adc_dma_mode_disable(adc);
		return;
	}


	sampletime = adc_set->group.sampletime;
	adc_ch_t ch;
	adc_channel_length_config(adc, adc_regular_channel, len);
	for(i = 0; i < len; i++) {
		rank = i;
		ch = adc_set->group.ch[i];
		adc_regular_channel_config(adc, rank, ch, sampletime);
	}
	xadc_dma_init(adc_set);
	adc_dma_mode_enable(adc);
}

void xadc_cont_mode_init(adc_t *adc_set)
{
	adc_cont_mode_state_t cont_mode = adc_set->cont_mode;
	switch(cont_mode) {
	case adc_cont_mode_off:	xadc_disable_cont_mode(adc_set);
							break;
	case adc_cont_mode_on:	xadc_enable_cont_mode(adc_set);
							break;
	}
}
