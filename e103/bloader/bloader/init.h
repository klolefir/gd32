#ifndef INIT_H_SENTRY
#define INIT_H_SENTRY

#include "tim.h"
#include "usart.h"
#include "fmc.h"
#include "gpio.h"

extern usart_t usart0;
extern tim_t usart_tim;
extern gpio_t debug_led;

typedef uint8_t buff_size_t;

enum {
	main_addr = fmc_page100_addr,
	main_sector = main_addr
};

enum {
	req_buff_len = 512,
	dec_buff_len = 512,
	ans_buff_len = 512
};

typedef struct {
	buff_size_t buff[req_buff_len];
	uint32_t cnt;
} req_buff_t;

typedef struct {
	buff_size_t buff[dec_buff_len];
	uint32_t cnt;
} dec_buff_t;

typedef struct {
	buff_size_t buff[ans_buff_len];
	uint32_t cnt;
} ans_buff_t;

void init_operator();
void init_operator_timer();
void init_opeartor_led();

#endif
