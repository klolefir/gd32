#include <gd32e10x.h>
#include "fsm.h"

int main(void)
{
	__enable_irq();

	fsm_process();
	while(1) {
	}
}

#if 0
void get_err()
{
}

void set_err
{
}
#endif