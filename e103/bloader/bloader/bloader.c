#include "bloader.h"
#include "fmc.h"
#include "kestring.h"

bl_state_t bloader_erase(const req_buff_t *req_buff_set, ans_buff_t *ans_buff_set)
{
	const buff_size_t *req_buff = (req_buff_set->buff);
	uint32_t *ans_cnt = &(ans_buff_set->cnt);
	buff_size_t *ans_buff = (ans_buff_set->buff);
	fmc_state_t fmc_state;

	uint32_t addr, len;
	kememcpy(&addr, &req_buff[bl_addr_pos], bl_addr_size);
	kememcpy(&len, &req_buff[bl_len_pos], bl_len_size);

	if(len > bl_max_page_size)
		return bl_bad_len;

	fmc_unlock();
	fmc_state = xfmc_erase_sector(addr, len);
	fmc_lock();

	if(fmc_state == fmc_bad_erase)
		return bl_bad_erase;
#if 0
	switch(fmc_state) {
	case fmc_ok_erase:	bl_state = bl_ok_erase;
						break;
	case fmc_bad_erase:
	default:			bl_state = bl_bad_erase;
	}
#endif

	kememcpy(&ans_buff[bl_addr_pos], &addr, bl_addr_size);
	kememcpy(&ans_buff[bl_len_pos], ans_cnt, bl_len_size);
	*ans_cnt += bl_addr_size;
	*ans_cnt += bl_len_size;

	return bl_ok_erase;
}

bl_state_t bloader_write(const req_buff_t *req_buff_set, ans_buff_t *ans_buff_set)
{
	const buff_size_t *req_buff = (req_buff_set->buff);
	uint32_t *ans_cnt = &(ans_buff_set->cnt);
	buff_size_t *ans_buff = (ans_buff_set->buff);
	uint32_t *data = (uint32_t *)&req_buff[bl_data_pos];
	/*bl_state_t bl_state;*/
	fmc_state_t fmc_state;

	uint32_t addr, len;
	kememcpy(&addr, &req_buff[bl_addr_pos], bl_addr_size);
	kememcpy(&len, &req_buff[bl_len_pos], bl_len_size);

	if(len > bl_max_page_size)
		return bl_bad_len;
	
	fmc_unlock();
	fmc_state = xfmc_write_sector(addr, data, len);
	fmc_lock();

	if(fmc_state == fmc_bad_write)
		return bl_bad_write;
#if 0
	switch(fmc_state) {
	case fmc_ok_write:	bl_state = bl_ok_write;
						break;
	case fmc_bad_write:	
	default:			bl_state = bl_bad_write;
	}
#endif

	kememcpy(&ans_buff[bl_addr_pos], &addr, bl_addr_size);
	kememcpy(&ans_buff[bl_len_pos], ans_cnt, bl_len_size);
	*ans_cnt += bl_addr_size;
	*ans_cnt += bl_len_size;

	return bl_ok_write;
}

bl_state_t bloader_read(const req_buff_t *req_buff_set, ans_buff_t *ans_buff_set)
{
	/*const uint32_t *req_cnt = &(req_buff_set->cnt);*/
	const buff_size_t *req_buff = (req_buff_set->buff);
	uint32_t *ans_cnt = &(ans_buff_set->cnt);
	buff_size_t *ans_buff = (ans_buff_set->buff);
	uint32_t *data = (uint32_t *)&ans_buff[bl_data_pos];

	/*bl_state_t bl_state;*/
	fmc_state_t fmc_state;

	uint32_t addr, len;
	kememcpy(&addr, &req_buff[bl_addr_pos], bl_addr_size);
	kememcpy(&len, &req_buff[bl_len_pos], bl_len_size);

	if(len > bl_max_page_size)
		return bl_bad_len;

	fmc_unlock();
	fmc_state = xfmc_read_sector(addr, data, len);
	fmc_lock();

	if(fmc_state == fmc_bad_read)
		return bl_bad_read;
#if 0
	switch(fmc_state) {
	case fmc_ok_read:	bl_state = bl_ok_read;
						break;
	case fmc_bad_read:	
	default:			bl_state = bl_bad_read;
	}
#endif

	kememcpy(&ans_buff[bl_addr_pos], &addr, bl_addr_size);
	kememcpy(&ans_buff[bl_len_pos], ans_cnt, bl_len_size);
	*ans_cnt += bl_addr_size;
	*ans_cnt += bl_len_size;
	*ans_cnt += len;

	return bl_ok_read;
}

bl_state_t bloader_test(const req_buff_t *req_buff_set, ans_buff_t *ans_buff_set)
{
	const uint32_t *req_cnt = &(req_buff_set->cnt);
	const buff_size_t *req_buff = (req_buff_set->buff);
	uint32_t *ans_cnt = &(ans_buff_set->cnt);
	buff_size_t *ans_buff = (ans_buff_set->buff);
	*ans_cnt = *req_cnt;
	kememcpy(ans_buff, req_buff, *req_cnt);
	return bl_ok_read;
}

#if 0
handle_st_t bloader_handle(const dec_buff_t *dec_buff_st, ans_buff_t *ans_buff_st)
{
	handle_st_t handle_st;

	const uint32_t *dec_cnt = &(dec_buff_st->cnt);
	const buff_size_t *dec_buff = (dec_buff_st->buff);

	uint32_t *ans_cnt = &(ans_buff_st->cnt);
	buff_size_t *ans_buff = (ans_buff_st->buff);

	uint32_t size;
	uint32_t *data;

	uint8_t cmd = dec_buff[bl_cmd_pos];
	switch(cmd) {
	case bl_clk_cmd:	usart_put_uint32(&usart1, rcc_get_system_clk() / 1000000);
						handle_st = handle_st_res;
						break;

	case bl_unlock_cmd:	handle_st = bloader_unlock();
						break;

	case bl_lock_cmd:	handle_st = bloader_lock();
						break;

	case bl_erase_cmd:	handle_st = bloader_erase();
						break;

	case bl_write_cmd:	//size = *dec_cnt;
						size = 64;
						data = (uint32_t *)dec_buff;
						handle_st = bloader_write(data, size);
						break;

	case bl_read_cmd:	size = 64;
						data = (uint32_t *)ans_buff;
						*ans_cnt = size * 4;
						handle_st = bloader_read(data, size);
						break;

	case bl_test_cmd:	data = (uint32_t *)ans_buff;
						*ans_cnt = bl_page_size;
						handle_st = bloader_test(data);
						break;

	case bl_main_cmd:	handle_st = handle_st_main;
						break;
	

	default:			handle_st = handle_st_bad;
	}

	return handle_st;
}

handle_st_t bloader_test(uint32_t *data)
{
	const uint32_t page_addr = flash_sector3_addr;
	const uint32_t page_size = 256; /*bytes*/
	const uint32_t data_len = 64;
	uint32_t *read_data = data;
	flash_status_t status;
	uint32_t i;

	uint32_t write_data[data_len];

	for(i = 0; i < data_len; i++)
		write_data[i] = 0xABCDABCD;

	flash_unlock();
	status = flash_erase(page_addr);
	if((FLASH->SR & FLASH_SR_PGAERR) || (FLASH->SR & FLASH_SR_PGSERR) || (FLASH->SR & FLASH_SR_PGPERR) || (FLASH->SR & FLASH_SR_WRPERR))
		return handle_st_bad;

	if(status == flash_ok) {
		status = flash_write_page(page_addr, write_data, page_size);
		if((FLASH->SR & FLASH_SR_PGAERR))
			usart_put_str(&usart1, "PGAERR\r");
		if((FLASH->SR & FLASH_SR_PGSERR))
			usart_put_str(&usart1, "PGSERR\r");
		if(FLASH->SR & FLASH_SR_PGPERR)
			usart_put_str(&usart1, "PGPERR\r");
		if((FLASH->SR & FLASH_SR_WRPERR))
			usart_put_str(&usart1, "WRPERR\r");

//		return handle_st_bad;
	}

	flash_lock();

	if(status == flash_ok)
		status = flash_read_page(page_addr, read_data, page_size);
	if(status == flash_ok)
		return handle_st_res;

	return handle_st_bad;
#if 0
	/********************erase************************/
	flash_unlock();
	status = flash_erase(page_addr);
	if(status != flash_ok) {
		flash_lock();
		return handle_st_bad;
	}
	/********************erase************************/

	/********************write************************/
	status = flash_write_page(page_addr, write_data, page_size);
	flash_lock();
	if(status != flash_ok) {
		return handle_st_bad;
	}
	/********************write************************/

	/********************read************************/
	status = flash_read_page(page_addr, read_data, page_size);

	if(status != flash_ok) {
		return handle_st_bad;
	}
	/********************read************************/

	return handle_st_res;
#endif
}

handle_st_t bloader_erase()
{
	flash_status_t status = flash_erase(main_sector);
	if(status != flash_ok)
		return handle_st_bad;
	return handle_st_res;
}

handle_st_t bloader_write(const uint32_t *data, const uint32_t size)
{
	uint32_t data32, addr;
	int i;
	handle_st_t handle_st = handle_st_res;
	flash_status_t flash_st;
	if(size != bl_page_size)
		return handle_st_bad;

	for(i = 0; i < (bl_page_size / 4); i++) {
		data32 = data[i];
		addr = main_addr + (i * 4);
		flash_st = flash_write(addr, data32);
		if(flash_st != flash_ok) {
			handle_st = handle_st_bad;
			break;
		}
	}

	return handle_st;
}

handle_st_t bloader_read(uint32_t *data, const uint32_t size)
{
	handle_st_t handle_st = handle_st_res;
	uint32_t data32, addr;
	flash_status_t flash_st;
	int i;
	if(size != bl_page_size)
		return handle_st_bad;

	for(i = 0; i < (bl_page_size / 4); i++) {
		addr = main_sector + i;
		flash_st = flash_read(addr, &data32);
		if(flash_st != flash_ok) {
			handle_st = handle_st_bad;
			break;
		}
		data[i] = data32;
	}
	return handle_st;
}

handle_st_t bloader_unlock()
{
	flash_unlock();
	return handle_st_res;
}

handle_st_t bloader_lock()
{
	flash_lock();
	return handle_st_res;
}
#endif

#if 0
void bloader_enter()
#endif
