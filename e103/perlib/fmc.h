#ifndef FMC_H_SENTRY
#define FMC_H_SENTRY

#include <gd32e10x.h>

enum {
	fmc_start_addr		= 0x08000000,
	fmc_page_size 		= 0x400, /* 1024 bytes */

	fmc_page0_addr		= fmc_start_addr,
	fmc_page1_addr		= fmc_page0_addr + 1 * fmc_page_size,
	fmc_page2_addr		= fmc_page0_addr + 2 * fmc_page_size,
	fmc_page3_addr		= fmc_page0_addr + 3 * fmc_page_size,
	fmc_page4_addr		= fmc_page0_addr + 4 * fmc_page_size,
	fmc_page5_addr		= fmc_page0_addr + 5 * fmc_page_size,
	fmc_page100_addr	= fmc_page0_addr + 100 * fmc_page_size,
	fmc_page101_addr	= fmc_page0_addr + 101 * fmc_page_size,
	fmc_page102_addr	= fmc_page0_addr + 102 * fmc_page_size,

	fmc_word_size		= 0x04,
	fmc_addr_offset 	= 0x04,
	fmc_max_sector_size	= 0x100,	/* 256 bytes */
	fmc_max_read_size	= 0x100, 	/* 256 bytes */
	fmc_max_write_size	= 0x100  	/* 256 bytes */
};

void fmc_erase_sector(uint32_t page_addr, uint32_t size);
void fmc_write_sector(uint32_t addr, uint32_t *data, uint32_t size);
void fmc_read_sector(uint32_t addr, uint32_t *data, uint32_t size);

#if 0
void flash_unlock();
void flash_lock();
flash_status_t flash_write(uint32_t data_addr, uint32_t data);
flash_status_t flash_write_page(uint32_t page_addr, uint32_t *data, uint32_t page_size);
flash_status_t flash_read(uint32_t data_addr, uint32_t *data);
flash_status_t flash_read_page(uint32_t page_addr, uint32_t *data, uint32_t page_size);
flash_status_t flash_erase(uint32_t sector_addr);
#endif

#endif
