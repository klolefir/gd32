#include <gd32e10x.h>
#include "usart.h"
#include "gpio.h"
#include "tim.h"

static void init();
static void tgl(gpio_t *gpio_set);

gpio_t out, debugpin;
tim_t tim;

static volatile uint8_t flag = 0;

int main(void)
{
	__enable_irq();
	init();
	xtim_enable_irq(&tim);
	while(1) {
#if 0
	if(flag == 1) {
			xgpio_ttgl(&out);
			flag = 0;
		}
#endif
	}

	return 0;
}

void init()
{
	uint32_t freq = 1000000;
	uint32_t psc = 0;
	uint32_t per = 120000000 / (freq * 2);
	SystemInit();
	SystemCoreClockUpdate();

	tim.tim = tim_num_8;
	tim.psc = psc ;
	tim.per = per;
	tim.align = tim_align_edge;
	tim.counter_dir = tim_counter_dir_up;
	tim.clkdiv = tim_clkdiv_1;
	tim.irq_state = tim_irq_off;
	xtim_init(&tim);

	debugpin.port = gpio_port_e;
	debugpin.pin = gpio_pin_7;
	debugpin.mode = gpio_mode_out_pp;
	debugpin.speed = gpio_ospeed_max;
	xgpio_init(&debugpin);
	xgpio_set(&debugpin);

	out.port = gpio_port_e;
	out.pin = gpio_pin_9;
	out.mode = gpio_mode_out_pp;
	out.speed = gpio_ospeed_max;
	xgpio_init(&out);
}

void tim0_brk_tim8_handler()
{
#if 0
	flag = 1;
#else
#if 0
	FlagStatus status;
    if((uint32_t)RESET !=(GPIO_OCTL(gpio_port_e) & (gpio_pin_9))){
        status = SET;
    }else{
        status = RESET;
    }
	/*FlagStatus status = gpio_output_bit_get(out.port, out.pin);*/
    if(RESET == status){
        GPIO_BOP(gpio_port_e) = (uint32_t)gpio_pin_9;
    }else{
        GPIO_BC(gpio_port_e) = (uint32_t)gpio_pin_9;
    }
#else
	xgpio_ttgl(&out);
#endif
#endif
    TIMER_INTF(tim.tim) = (~(uint32_t)TIMER_INT_FLAG_UP);
}

void tgl(gpio_t *gpio_set)
{
    if((GPIO_OCTL(gpio_set->port) & (gpio_set->pin)) != (uint32_t)RESET) {
        GPIO_BC(gpio_set->port) = (uint32_t)gpio_set->pin;
    } else {
        GPIO_BOP(gpio_set->port) = (uint32_t)gpio_set->pin;
    }
}
