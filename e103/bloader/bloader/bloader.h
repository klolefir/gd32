#ifndef BLOADER_H_SENTRY
#define BLOADER_H_SENTRY

#include "init.h"

enum { /* bytes */
	bl_max_page_size = 256
};

enum {
	bl_sig_in 	= 0xF9BE,
	bl_sig_out 	= 0xBEF9
};

enum {
	bl_sig_size 	= 2,
	bl_status_size 	= 4,
	bl_cmd_size 	= 4,
	bl_addr_size 	= 4,
	bl_len_size 	= 4
};

enum { /* byte */
	bl_sig_pos 		= 0,
	bl_status_pos 	= bl_sig_pos + 2,
	bl_cmd_pos 		= bl_sig_pos + 2,
	bl_addr_pos 	= bl_sig_pos + 6,
	bl_len_pos		= bl_sig_pos + 10,
	bl_data_pos 	= bl_sig_pos + 14
};

typedef enum {
	/*bl_cmd_enter 	= 0x00000001,*/
	bl_cmd_reset 	= 0x00000002,
	bl_cmd_gomain 	= 0x00000003,
	bl_cmd_write	= 0x00000004,
	bl_cmd_read		= 0x00000005,
	bl_cmd_erase	= 0x00000006,
	bl_cmd_test		= 0x00000007
} bl_cmd_t;

typedef enum {
	bl_ok			= 0x000000AA,
	bl_busy			= 0x000000E4,
	bl_pgerr		= 0x000000E5,
	bl_pgaerr		= 0x000000E6,
	bl_wperr		= 0x000000E7,
	bl_toerr		= 0x000000E8,
	bl_blen			= 0x000000E9,
	bl_none			= 0x000000EE,




	bl_bad_erase 	= 0x000000E0,
	bl_ok_erase 	= 0x000000A0,

	bl_bad_write 	= 0x000000E1,
	bl_ok_write 	= 0x000000A1,

	bl_bad_read 	= 0x000000E2,
	bl_ok_read		= 0x000000A2,

	bl_bad_len		= 0x000000E3
} bl_status_t;

/*bl_status_t*/void bloader_erase(const req_buff_t *req_buff_set, ans_buff_t *ans_buff_set);
/*bl_status_t*/void bloader_write(const req_buff_t *req_buff_set, ans_buff_t *ans_buff_set);
/*bl_status_t*/void bloader_read(const req_buff_t *req_buff_set, ans_buff_t *ans_buff_set);
bl_status_t bloader_test(const req_buff_t *req_buff_set, ans_buff_t *ans_buff_set);

#endif
