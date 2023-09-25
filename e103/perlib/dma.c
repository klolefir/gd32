#include "dma.h"

static void xdma_rcu_init(const dma_t *dma_set);

void xdma_init(const dma_t *dma_set)
{
	dma_parameter_struct params;
	dma_num_t dma = dma_set->dma;
	dma_ch_t ch = dma_set->ch;
	dma_deinit(dma, ch);
	xdma_rcu_init(dma_set);
	
	params.direction = dma_set->dir;

	params.periph_addr = dma_set->paddr;
	params.periph_width = dma_set->pwidth;
	params.periph_inc = dma_set->pinc;
	params.memory_addr = dma_set->maddr;
	params.memory_width = dma_set->mwidth;
	params.memory_inc = dma_set->minc;
	params.priority = dma_set->prior;
	params.number = dma_set->num;

	dma_init(dma, ch, &params);
	if(dma_set->circ == dma_circ_enable)
		dma_circulation_enable(dma, ch);
	else
		dma_circulation_disable(dma, ch);

#if 1
	if(dma_set->m2m == dma_m2m_enable)
		dma_memory_to_memory_enable(dma, ch);
	else
		dma_memory_to_memory_disable(dma, ch);
#endif

	xdma_channel_enable(dma_set);
}

void xdma_channel_enable(const dma_t *dma_set)
{
	dma_channel_enable(dma_set->dma, dma_set->ch);
}

void xdma_rcu_init(const dma_t *dma_set)
{
	switch(dma_set->dma) {
	case dma_num_0:	rcu_periph_clock_enable(RCU_DMA0);
					break;
	case dma_num_1:	rcu_periph_clock_enable(RCU_DMA1);
					break;
	}
}
