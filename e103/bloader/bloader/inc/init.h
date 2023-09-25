#ifndef INIT_H_SENTRY
#define INIT_H_SENTRY

#include "tim.h"
#include "usart.h"
#include "fmc.h"
#include "gpio.h"

typedef uint8_t buff_size_t;

enum {
	//main_addr 	= fmc_page100_addr,
	main_addr 	= 0x800c800,
	main_sector = main_addr
};

enum {
	max_buff_len = 300,
};

typedef struct {
	buff_size_t buff[max_buff_len];
	uint32_t cnt;
} buff_t;

typedef struct {
	uint32_t *buff;
	uint32_t cnt;
} pbuff_t;

#endif
