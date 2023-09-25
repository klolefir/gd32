#include "bloader.h"
#include "fmc.h"
#include "kestring.h"

static bl_status_t bloader_get_status(fmc_status_t status);

void bloader_erase(const req_buff_t *req_buff_set, ans_buff_t *ans_buff_set)
{
	const buff_size_t *req_buff = (req_buff_set->buff);
	uint32_t *ans_cnt = &(ans_buff_set->cnt);
	buff_size_t *ans_buff = (ans_buff_set->buff);
	uint32_t bl_status;
	fmc_status_t fmc_status;

	uint32_t addr, len;
	kememcpy(&addr, &req_buff[bl_addr_pos], bl_addr_size);
	kememcpy(&len, &req_buff[bl_len_pos], bl_len_size);

	if(len > bl_max_page_size) {
		bl_status = bl_blen;
		goto ans;
	}

	fmc_unlock();
	fmc_status = xfmc_erase_sector(&addr, &len);
	fmc_lock();

	bl_status = bloader_get_status(fmc_status);

ans:
	kememcpy(&ans_buff[bl_status_pos], &bl_status, bl_status_size);
	kememcpy(&ans_buff[bl_addr_pos], &addr, bl_addr_size);
	/*kememcpy(&ans_buff[bl_len_pos], ans_cnt, bl_len_size);*/
	kememcpy(&ans_buff[bl_len_pos], &len, bl_len_size);
	*ans_cnt += bl_status_size;
	*ans_cnt += bl_addr_size;
	*ans_cnt += bl_len_size;

	/*return bl_ok_erase;*/
}

void bloader_write(const req_buff_t *req_buff_set, ans_buff_t *ans_buff_set)
{
	const buff_size_t *req_buff = (req_buff_set->buff);
	uint32_t *ans_cnt = &(ans_buff_set->cnt);
	buff_size_t *ans_buff = (ans_buff_set->buff);
	uint32_t *data = (uint32_t *)&req_buff[bl_data_pos];
	uint32_t bl_status;
	fmc_status_t fmc_status;
	uint32_t addr, len;

	kememcpy(&addr, &req_buff[bl_addr_pos], bl_addr_size);
	kememcpy(&len, &req_buff[bl_len_pos], bl_len_size);

	if(len > bl_max_page_size) {
		bl_status = bl_blen;
		goto ans;
	}
	
	fmc_unlock();
	fmc_status = xfmc_write_sector(&addr, data, &len);
	fmc_lock();

	bl_status = bloader_get_status(fmc_status);

ans:
	kememcpy(&ans_buff[bl_status_pos], &bl_status, bl_status_size);
	kememcpy(&ans_buff[bl_addr_pos], &addr, bl_addr_size);
	/*kememcpy(&ans_buff[bl_len_pos], ans_cnt, bl_len_size);*/
	kememcpy(&ans_buff[bl_len_pos], &len, bl_len_size);
	*ans_cnt += bl_status_size;
	*ans_cnt += bl_addr_size;
	*ans_cnt += bl_len_size;

	/*return bl_ok_write;*/
}

void bloader_read(const req_buff_t *req_buff_set, ans_buff_t *ans_buff_set)
{
	/*const uint32_t *req_cnt = &(req_buff_set->cnt);*/
	const buff_size_t *req_buff = (req_buff_set->buff);
	uint32_t *ans_cnt = &(ans_buff_set->cnt);
	buff_size_t *ans_buff = (ans_buff_set->buff);
	uint32_t *data = (uint32_t *)&ans_buff[bl_data_pos];

	uint32_t bl_status;
	fmc_status_t fmc_status;

	uint32_t addr, len;
	kememcpy(&addr, &req_buff[bl_addr_pos], bl_addr_size);
	kememcpy(&len, &req_buff[bl_len_pos], bl_len_size);

	if(len > bl_max_page_size) {
		bl_status = bl_blen;
		goto ans;
	}

	fmc_unlock();
	fmc_status = xfmc_read_sector(&addr, data, &len);
	fmc_lock();

	bl_status = bloader_get_status(fmc_status);

ans:
	kememcpy(&ans_buff[bl_status_pos], &bl_status, bl_status_size);
	kememcpy(&ans_buff[bl_addr_pos], &addr, bl_addr_size);
	/*kememcpy(&ans_buff[bl_len_pos], ans_cnt, bl_len_size);*/
	kememcpy(&ans_buff[bl_len_pos], &len, bl_len_size);
	*ans_cnt += bl_status_size;
	*ans_cnt += bl_addr_size;
	*ans_cnt += bl_len_size;
	*ans_cnt += len;

/*	return bl_ok_read;*/
}

bl_status_t bloader_test(const req_buff_t *req_buff_set, ans_buff_t *ans_buff_set)
{
	const uint32_t *req_cnt = &(req_buff_set->cnt);
	const buff_size_t *req_buff = (req_buff_set->buff);
	uint32_t *ans_cnt = &(ans_buff_set->cnt);
	buff_size_t *ans_buff = (ans_buff_set->buff);
	*ans_cnt = *req_cnt;
	kememcpy(ans_buff, req_buff, *req_cnt);
	return bl_ok_read;
}

bl_status_t bloader_get_status(fmc_status_t status)
{
	switch(status) {
	case fmc_ready:		return bl_ok;
	case fmc_busy:		return bl_busy;
	case fmc_pgerr:		return bl_pgerr;
	case fmc_pgaerr:	return bl_pgaerr;
	case fmc_wperr:		return bl_wperr;
	case fmc_toerr:		return bl_toerr;
	case fmc_blen:		return bl_blen;
	default:			return bl_none;
	}
}

#if 0
void bloader_enter()
#endif
