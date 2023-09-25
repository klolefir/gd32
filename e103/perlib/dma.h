#ifndef DMA_H_SENTRY
#define DMA_H_SENTRY

#include "gd32e10x.h"

typedef enum {
	dma_num_0 = DMA0,
	dma_num_1 = DMA1
} dma_num_t;

typedef enum {
	dma_ch_adc = DMA_CH0
} dma_ch_t;

typedef enum {
	dma_dir_m2p = DMA_MEMORY_TO_PERIPHERAL,
	dma_dir_p2m = DMA_PERIPHERAL_TO_MEMORY
} dma_dir_t;

typedef enum {
	dma_paddr_adc = (uint32_t)(&ADC_RDATA(ADC0))
} dma_paddr_t;

typedef enum {
	dma_pwidth_8bit = DMA_PERIPHERAL_WIDTH_8BIT,
	dma_pwidth_16bit = DMA_PERIPHERAL_WIDTH_16BIT,
	dma_pwidth_32bit = DMA_PERIPHERAL_WIDTH_32BIT
} dma_pwidth_t;

typedef enum {
	dma_pinc_disable = DMA_PERIPH_INCREASE_DISABLE,
	dma_pinc_enable = DMA_PERIPH_INCREASE_ENABLE
} dma_pinc_t;


typedef enum {
	dma_mwidth_8bit = DMA_MEMORY_WIDTH_8BIT,
	dma_mwidth_16bit = DMA_MEMORY_WIDTH_16BIT,
	dma_mwidth_32bit = DMA_MEMORY_WIDTH_32BIT
} dma_mwidth_t;

typedef enum {
	dma_minc_disable = DMA_MEMORY_INCREASE_DISABLE,
	dma_minc_enable = DMA_MEMORY_INCREASE_ENABLE
} dma_minc_t;

typedef enum {
	dma_prior_low = DMA_PRIORITY_LOW,
	dma_prior_medium = DMA_PRIORITY_MEDIUM,
	dma_prior_high = DMA_PRIORITY_HIGH,
	dma_prior_uhigh = DMA_PRIORITY_ULTRA_HIGH
} dma_prior_t;

typedef enum {
	dma_circ_enable,
	dma_circ_disable
} dma_circ_t;

typedef enum {
	dma_m2m_enable,
	dma_m2m_disable
} dma_m2m_t;

typedef struct {
	dma_num_t dma;
	dma_ch_t ch;
	dma_dir_t dir;
	dma_paddr_t paddr;
	dma_pwidth_t pwidth;
	dma_pinc_t pinc;
	uint32_t maddr;
	dma_mwidth_t mwidth;
	dma_minc_t minc;
	uint8_t num;
	dma_prior_t prior;
	dma_circ_t circ;
	dma_m2m_t m2m;
} dma_t;

void xdma_init(const dma_t *dma_set);
void xdma_channel_enable(const dma_t *dma_set);

#endif
