#include "rcu.h"

void xrcu_reset_ahb()
{
	RCU_AHBEN = 0;
}

void xrcu_reset_apb1()
{
	RCU_APB1EN = 0;
}

void xrcu_reset_apb2()
{
	RCU_APB2EN = 0;
}
