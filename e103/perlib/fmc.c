#include "fmc.h"

static uint32_t xfmc_switch_page_addr(const uint32_t addr);

fmc_status_t xfmc_write_sector(uint32_t *addr, const uint32_t *data, uint32_t *size /* bytes */)
{
	uint32_t i, curr_addr = 0;
	fmc_status_t status;

	if(*size > fmc_max_write_size)
		return fmc_blen;

	for(i = 0; i < (*size / fmc_word_size); i++) {
		curr_addr = *addr + i * fmc_addr_offset;
		status = fmc_word_program(curr_addr, data[i]);
		if(status != fmc_ready)
			break;
	}
	*addr = curr_addr;
	*size = i * fmc_word_size;
	return status;
}

#if 0
fmc_status_t xfmc_write_sector(const uint32_t addr, const uint32_t *data, const uint32_t size /* bytes */)
{
	uint32_t i, curr_addr = 0;
	fmc_status_t status;

	if(size > fmc_max_write_size)
		return fmc_blen;

	for(i = 0; i < (*size / fmc_word_size); i++) {
		curr_addr = addr + i * fmc_addr_offset;
		status = fmc_word_program(curr_addr, data[i]);
		if(status != fmc_ready)
			break;
	}
	*addr = curr_addr;
	*size = i * fmc_word_size;
	return status;
}
#endif

fmc_status_t xfmc_read_sector(uint32_t *addr, uint32_t *data, uint32_t *size /* bytes*/)
{
	uint32_t i;
	uint32_t curr_addr = 0;
	if(*size > fmc_max_read_size) {
		*size = 0;
		return fmc_blen;
	}

	for(i = 0; i < (*size / fmc_word_size); i++) {
		curr_addr = *addr + i * fmc_addr_offset;
		data[i] = *((uint32_t * )curr_addr);
		/*data[i] = *((uint32_t *)(*addr + i * fmc_addr_offset));*/
	}
	*addr = curr_addr;
	return fmc_ready;
}

#if 0
fmc_status_t xfmc_read_sector(const uint32_t addr, uint32_t *data, const uint32_t size /* bytes*/)
{
	uint32_t i;
	if(size > fmc_max_read_size)
		return fmc_blen;

	for(i = 0; i < (size / fmc_word_size); i++)
		data[i] = *((uint32_t *)(addr + i * fmc_addr_offset));
	return fmc_ready;
}
#endif

fmc_status_t xfmc_erase_sector(uint32_t *addr, uint32_t *size /* bytes */)
{
	uint32_t i;
	uint32_t curr_page_addr = 0;
	fmc_status_t status;

	if(*size > fmc_max_erase_size) {
		*size = 0;
		return fmc_blen;
	}
	
	for(i = 0; i < *size; i++) {
		if(!(i % fmc_page_size)) {
			curr_page_addr = xfmc_switch_page_addr(*addr + i);
			if(!curr_page_addr)
				return fmc_wperr;
			status = fmc_page_erase(curr_page_addr);
			if(status != fmc_ready)
				break;
		}
	}
	*size = i;
	*addr = curr_page_addr;
	return status;
}
#if 0
fmc_status_t xfmc_erase_sector(const uint32_t addr, const uint32_t size /* bytes */)
{
	uint32_t i;
	uint32_t curr_page_addr;
	fmc_status_t status;

	if(size > fmc_max_erase_size)
		return fmc_blen;
	
	for(i = 0; i < size; i++) {
		if(!(i % fmc_page_size)) {
			curr_page_addr = xfmc_switch_page_addr(addr + i);
			if(!curr_page_addr)
				return fmc_wperr;
			status = fmc_page_erase(curr_page_addr);
			if(status != fmc_ready)
				break;
		}
	}
	return status;
}
#endif

uint32_t xfmc_switch_page_addr(const uint32_t addr)
{
	uint32_t i, curr_page_addr, page_addr = 0;
	if((addr < fmc_page0_addr) || (addr > fmc_page127_addr + fmc_page_size))
		return 0;

	for(i = 0; i < fmc_page_num; i++) {
		curr_page_addr = fmc_page0_addr + i * fmc_page_size;
		if((addr >= curr_page_addr) && (addr < curr_page_addr + fmc_page_size)) {
			page_addr = curr_page_addr;
			break;
		}
	}

	return page_addr;
}
