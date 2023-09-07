#ifndef ADC_H_SENTRY
#define ADC_H_SENTRY

#include "gd32e10x.h"

enum {
	adc_max_group_num = 16,
	adc_zero_group = 0,
	adc_first_group = 4,
	adc_second_group = 8,
	adc_third_group = 12,
	adc_regular_channel = ADC_REGULAR_CHANNEL,
	adc_scan_mode = ADC_SCAN_MODE,
	adc_cont_mode = ADC_CONTINUOUS_MODE,
	adc_rcu_div = RCU_CKADC_CKAPB2_DIV4
};

typedef enum {
	adc_num_0 = ADC0,
	adc_num_1 = ADC1
} adc_num_t;

typedef enum {
	adc_mode_free 			= ADC_MODE_FREE,
	adc_mode_parallel 		= ADC_DAUL_REGULAL_PARALLEL,
	adc_mode_followup_fast 	= ADC_DAUL_REGULAL_FOLLOWUP_FAST,
	adc_mode_followup_slow 	= ADC_DAUL_REGULAL_FOLLOWUP_SLOW
} adc_mode_t;

typedef enum {
	adc_ch0 	= ADC_CHANNEL_0,
	adc_ch1 	= ADC_CHANNEL_1,
	adc_ch2 	= ADC_CHANNEL_2,
	adc_ch3 	= ADC_CHANNEL_3,
	adc_ch4 	= ADC_CHANNEL_4,
	adc_ch5 	= ADC_CHANNEL_5,
	adc_ch6 	= ADC_CHANNEL_6,
	adc_ch7 	= ADC_CHANNEL_7,
	adc_ch8 	= ADC_CHANNEL_8,
	adc_ch9 	= ADC_CHANNEL_9,
	adc_ch10	= ADC_CHANNEL_10,
	adc_ch11	= ADC_CHANNEL_11,
	adc_ch12	= ADC_CHANNEL_12,
	adc_ch13	= ADC_CHANNEL_13,
	adc_ch14	= ADC_CHANNEL_14,
	adc_ch15	= ADC_CHANNEL_15,
	adc_ch16	= ADC_CHANNEL_16,
	adc_ch17	= ADC_CHANNEL_17,
	adc_temp	= adc_ch16,
	adc_vref	= adc_ch17
} adc_ch_t;

typedef enum {
	adc_scan_mode_off,
	adc_scan_mode_on
} adc_scan_mode_state_t;

typedef enum {
	adc_cont_mode_off,
	adc_cont_mode_on
} adc_cont_mode_state_t;

#if 0
typedef enum {
	adc_dcont_mode_off,
	adc_dcont_mode_on
} adc_dcont_mode_state_t;
#endif

typedef enum {
	adc_align_lsb = ADC_DATAALIGN_RIGHT,
	adc_align_msb = ADC_DATAALIGN_LEFT
} adc_align_t;

typedef enum {
	adc_res_12bit 	= ADC_RESOLUTION_12B,
	adc_res_10bit 	= ADC_RESOLUTION_10B,
	adc_res_8bit 	= ADC_RESOLUTION_8B,
	adc_res_6bit 	= ADC_RESOLUTION_6B
} adc_res_t;

typedef enum {
	adc_group_len_1 	= 1,
	adc_group_len_2 	= 2,
	adc_group_len_3 	= 3,
	adc_group_len_4 	= 4,
	adc_group_len_5 	= 5,
	adc_group_len_6 	= 6,
	adc_group_len_7 	= 7,
	adc_group_len_8 	= 8,
	adc_group_len_9		= 9,
	adc_group_len_10 	= 10,
	adc_group_len_11 	= 11,
	adc_group_5en_12 	= 12,
	adc_group_len_13 	= 13,
	adc_group_len_14 	= 14,
	adc_group_len_15 	= 15,
	adc_group_len_16 	= 16
} adc_group_len_t;

typedef enum {
	adc_sampletime_1point5 = ADC_SAMPLETIME_1POINT5,
	adc_sampletime_7point5 = ADC_SAMPLETIME_7POINT5,
	adc_sampletime_13point5 = ADC_SAMPLETIME_13POINT5,
	adc_sampletime_28point5 = ADC_SAMPLETIME_28POINT5,
	adc_sampletime_41point5 = ADC_SAMPLETIME_41POINT5,
	adc_sampletime_55point5 = ADC_SAMPLETIME_55POINT5,
	adc_sampletime_71point5 = ADC_SAMPLETIME_71POINT5,
	adc_sampletime_239point5 = ADC_SAMPLETIME_239POINT5,
} adc_sampletime_t;

typedef struct {
	adc_ch_t 			ch[adc_max_group_num];
	adc_sampletime_t 	sampletime;
	adc_group_len_t 	len;
} adc_group_t;

typedef struct {
	adc_num_t 				adc;
	adc_mode_t 				mode;
	adc_res_t				res;
	adc_align_t				align;
	adc_group_t				group;
	adc_scan_mode_state_t 	scan_mode;
	adc_cont_mode_state_t	cont_mode;
	/*adc_dcont_mode_state_t	dcont_mode;*/
} adc_t;

void xadc_init(adc_t *adc_set);

void xadc_enable(adc_t *adc_set);
void xadc_disable(adc_t *adc_set);

uint16_t xadc_get_data(adc_t *adc_set);

void xadc_enable_scan_mode(adc_t *adc_set);
void xadc_disable_scan_mode(adc_t *adc_set);

void xadc_enable_cont_mode(adc_t *adc_set);
void xadc_disable_cont_mode(adc_t *adc_set);

void xadc_enable_soft_trig(adc_t *adc_set);

void xadc_set_group(adc_t *adc_set, adc_group_t *group);

void xadc_calib(adc_t *adc_set);

void xadc_sensor_enable();
void xadc_sensor_disable();

#if 0
void xadc_sensor_enable();
void xadc_sensor_disable();
void xadc_tempsensor_init();
void xadc_vrefinit();
#endif

#endif
