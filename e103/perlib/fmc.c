#include "fmc.h"

void fmc_write_sector(uint32_t addr, uint32_t *data, uint32_t size /* bytes */)
{
	uint32_t i, curr_addr;
	if(size > fmc_max_write_size)
		return;

	for(i = 0; i < (size / fmc_word_size); i++) {
		curr_addr = addr + i * fmc_addr_offset;
		fmc_word_program(curr_addr, data[i]);
	}
}

void fmc_read_sector(uint32_t addr, uint32_t *data, uint32_t size /* bytes*/)
{
	uint32_t i;
	if(size > fmc_max_read_size)
		return;

	for(i = 0; i < (size / fmc_word_size); i++)
		data[i] = *((uint32_t *)(addr + i * fmc_addr_offset));
}
