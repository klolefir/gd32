#include "bloader.h"
#include "fmc.h"
#include "kestring.h"

static bl_status_t bloader_get_status(fmc_status_t status);

bl_status_t bloader_erase(uint32_t *addr, uint32_t *len)
{
	fmc_status_t fmc_status;
	bl_status_t bl_status;

	fmc_unlock();
	fmc_status = xfmc_erase_sector(addr, len);
	fmc_lock();

	bl_status = bloader_get_status(fmc_status);
	return bl_status;

#if 0
ans:

	kememcpy(&ans_buff[bl_status_pos], &bl_status, bl_status_size);
	kememcpy(&ans_buff[bl_addr_pos], &addr, bl_addr_size);
	kememcpy(&ans_buff[bl_len_pos], &len, bl_len_size);
	*ans_cnt += bl_status_size;
	*ans_cnt += bl_addr_size;
	*ans_cnt += bl_len_size;
#endif
}

bl_status_t bloader_write(uint32_t *addr, const uint32_t *data, uint32_t *len)
{
	bl_status_t bl_status;
	fmc_status_t fmc_status;
	
	fmc_unlock();
	fmc_status = xfmc_write_sector(addr, data, len);
	fmc_lock();

	bl_status = bloader_get_status(fmc_status);
	return bl_status;
}

bl_status_t bloader_read(uint32_t *addr, uint32_t *data, uint32_t *len)
{
	bl_status_t bl_status;
	fmc_status_t fmc_status;

	fmc_unlock();
	fmc_status = xfmc_read_sector(addr, data, len);
	fmc_lock();

	bl_status = bloader_get_status(fmc_status);

	return bl_status;

#if 0
ans:
	kememcpy(&ans_buff[bl_status_pos], &bl_status, bl_status_size);
	kememcpy(&ans_buff[bl_addr_pos], &addr, bl_addr_size);
	/*kememcpy(&ans_buff[bl_len_pos], ans_cnt, bl_len_size);*/
	kememcpy(&ans_buff[bl_len_pos], &len, bl_len_size);
	*ans_cnt += bl_status_size;
	*ans_cnt += bl_addr_size;
	*ans_cnt += bl_len_size;
	*ans_cnt += len;
#endif
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
