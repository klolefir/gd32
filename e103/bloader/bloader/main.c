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

void hard_fault_handler()
{
	while(1) {}
}

void nmi_handler(void)
{
	while(1) {}
}

void mem_manage_handler(void)
{
	while(1) {}
}

void bus_fault_handler(void)
{
	while(1) {}
}

void usage_fault_handler(void)
{
	while(1) {}
}
