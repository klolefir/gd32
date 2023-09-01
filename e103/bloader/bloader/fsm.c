#include "fsm.h"
#include "init.h"
#include "gpio.h"
#include "rcu.h"
#include "usart.h"
#include "tim.h"
#include "bloader.h"
#include "kestring.h"

static void init();
static recv_state_t receive(req_buff_t *req_buff_set);
static decode_state_t decode(const req_buff_t *req_buff_set);
static handle_state_t handle(const req_buff_t *req_buff_set, ans_buff_t *ans_buff_set);
static void code(ans_buff_t *ans_buff_set);
static void respond(const ans_buff_t *ans_buff_set);
static void purge(req_buff_t *req_buff_set, ans_buff_t *ans_buff_set);
static void reset();
static void deinit();
static void gomain();
static void error(ans_buff_t *ans_buff_set);

fsm_state_t switch_state_after_recv(const recv_state_t recv_status);
fsm_state_t switch_state_after_decode(const decode_state_t decode_state);
fsm_state_t switch_state_after_handle(const handle_state_t handle_stateatus);

void fsm_process()
{
	req_buff_t req_buff_set;
	req_buff_set.cnt = 0;

	ans_buff_t ans_buff_set;
	ans_buff_set.cnt = 0;

	recv_state_t 	recv_state;
	decode_state_t	decode_state;
	handle_state_t 	handle_state;

	fsm_state_t state = fsm_idle_state;
	while(1) {
		switch(state) {

		case fsm_idle_state:	state = fsm_init_state;
								break;

		case fsm_init_state:	init();
								state = fsm_wait_state;
								break;

		case fsm_wait_state:	state = fsm_receive_state;
								break;

		case fsm_receive_state:	recv_state = receive(&req_buff_set);
								state = switch_state_after_recv(recv_state);
								break;

		case fsm_decode_state:	decode_state = decode(&req_buff_set);
								state = switch_state_after_decode(decode_state);
								break;

		case fsm_handle_state:	handle_state= handle(&req_buff_set, &ans_buff_set);
								state = switch_state_after_handle(handle_state);
								break;

		case fsm_code_state:	code(&ans_buff_set);
								state = fsm_respond_state;
								break;

		case fsm_error_state:	error(&ans_buff_set);
								state = fsm_respond_state;
								break;

		case fsm_respond_state:	respond(&ans_buff_set);
								state = fsm_purge_state;
								break;

		case fsm_purge_state:	purge(&req_buff_set, &ans_buff_set);
								state = fsm_receive_state;
								break;

		case fsm_deinit_state:	deinit();
								state = fsm_gomain_state;
								break;

		case fsm_gomain_state:	gomain();
								break;

		case fsm_reset_state:	reset();
								break;

		}
	}
}

void reset()
{
	NVIC_SystemReset();
}

void init()
{
	SystemInit();
	SystemCoreClockUpdate();
	init_operator();
	init_operator_timer();
}

recv_state_t receive(req_buff_t *req_buff_set)
{
	recv_state_t recv_state = recv_state_bad;

	uint32_t *req_cnt = &(req_buff_set->cnt);
	buff_size_t *req_buff = (req_buff_set->buff);

	static uint32_t timer = 0;
	uint32_t usart_ticks = xtim_get_ticks(&usart_tim);
	char c;

	usart_rx_status_t rx_status = xusart_get_rx_status(&usart0);
	if(rx_status == usart_rx_rdy) {
		/*xgpio_tgl(&debug_led);*/
		xusart_get_char(&usart0, &c);
		req_buff[*req_cnt] = c;
		(*req_cnt)++;
		timer = usart_ticks + 2; /* delay ms */
	}

	if(((timer) && (usart_ticks >= timer) && (*req_cnt)) ||
		(*req_cnt >= req_buff_len))
	{
		recv_state = recv_state_ok;
		timer = 0;
	}

	return recv_state;
}

decode_state_t decode(const req_buff_t *req_buff_set)
{
	decode_state_t decode_state;
	const uint32_t *req_cnt = &(req_buff_set->cnt);
	const buff_size_t *req_buff = (req_buff_set->buff);
	uint16_t sig_in;
	/*kelmemcpy(&sig_in, &req_buff[bl_sig_pos], 2 * sizeof(char));*/
	kememcpy(&sig_in, &req_buff[bl_sig_pos], 2 * sizeof(char));
	/*uint32_t crc, crc_len;*/

	if(!(*req_cnt))
		decode_state = decode_state_bad;
	else if(sig_in != bl_sig_in)
		decode_state = decode_state_bad;
	else
		decode_state = decode_state_ok;
#if 0
	if(req_buff[bl_sig_pos] != bl_sig_in) {
		decode_state = decode_state_bad;
	} else {
		crc_len = *req_cnt - 1;
		crc = bloader_crc(req_buff, crc_len);
		if(req_buff[*req_cnt] != crc)
			decode_state = decode_state_bad;
		else
			decode_state = decode_state_ok;
	}
#endif

	return decode_state;
}

handle_state_t handle(const req_buff_t *req_buff_set, ans_buff_t *ans_buff_set)
{
	/*const uint32_t *req_cnt = &(req_buff_set->cnt);*/
	const buff_size_t *req_buff = (req_buff_set->buff);
	uint32_t *ans_cnt = &(ans_buff_set->cnt);
	buff_size_t *ans_buff = (ans_buff_set->buff);

	bl_cmd_t cmd;
	handle_state_t handle_state;
	bl_state_t bl_state;

	kememcpy(&cmd, &req_buff[bl_cmd_pos], 4 * sizeof(char));

	switch(cmd) {
	case bl_cmd_test: 	bloader_test(req_buff_set, ans_buff_set);		
						handle_state = handle_state_res;
						break;

	case bl_cmd_reset: 	handle_state = handle_state_rst;
						break;

	case bl_cmd_gomain: handle_state = handle_state_main;	
						break;

	case bl_cmd_write:	bl_state = bloader_write(req_buff_set, ans_buff_set);
						if(bl_state == bl_bad_write)
							handle_state = handle_state_err;
						else
							handle_state = handle_state_res;
						break;

	case bl_cmd_read:	bl_state = bloader_read(req_buff_set, ans_buff_set);
						if(bl_state == bl_bad_read)
							handle_state = handle_state_err;
						else
							handle_state = handle_state_res;
						break;

	case bl_cmd_erase:	bl_state = bloader_erase(req_buff_set, ans_buff_set);
						if(bl_state == bl_bad_erase)
							handle_state = handle_state_err;
						else
							handle_state = handle_state_res;
						break;

	default:			handle_state = handle_state_err;
	}

	kememcpy(&ans_buff[bl_cmd_pos], &cmd, bl_cmd_size);
	*ans_cnt += bl_cmd_size;

	return handle_state;
}

void error(ans_buff_t *ans_buff_set)
{
	/*err_t err = get_err()*/
	/*switch(err) { ... }*/
	uint32_t *ans_cnt = &(ans_buff_set->cnt);
	buff_size_t *ans_buff = (ans_buff_set->buff);

	const char err_str[] = "Error!\r";
	*ans_cnt = kestrlen(err_str);
	kememcpy(ans_buff, err_str, *ans_cnt);
}

void code(ans_buff_t *ans_buff_set)
{
	uint32_t *ans_cnt = &ans_buff_set->cnt;
	buff_size_t *ans_buff = (ans_buff_set->buff);
	uint16_t sig = bl_sig_out;
	kememcpy(&ans_buff[bl_sig_pos], &sig, bl_sig_size);
	*ans_cnt += bl_sig_size;
}

void respond(const ans_buff_t *ans_buff_set)
{
	const uint32_t ans_cnt = ans_buff_set->cnt;
	const buff_size_t *ans_buff = (ans_buff_set->buff);
	
	if(ans_cnt)
		xusart_put_buff(&usart0, ans_buff, ans_cnt);
}

void purge(req_buff_t *req_buff_set, ans_buff_t *ans_buff_set)
{
	req_buff_set->cnt = 0;
	ans_buff_set->cnt = 0;
}

void deinit()
{
	rcu_deinit();
	xrcu_reset_ahb();
	xrcu_reset_apb1();
	xrcu_reset_apb2();
#if 0
	xsystick_disable();
	xsystick_reset();
#endif
}

void gomain()
{
	typedef void(*main_f)(void);
	main_f main_jump;

	__disable_irq();
//	__DSB();
//	__ISB();
	SCB->VTOR = main_addr;

	uint32_t msp = *((volatile uint32_t *)(main_addr));
	__set_MSP(msp);

	uint32_t main_jump_addr = *((uint32_t *)(main_addr + 0x04));
	main_jump = (main_f)main_jump_addr;
	main_jump();
}

fsm_state_t switch_state_after_recv(const recv_state_t recv_state)
{
	fsm_state_t next_state;
	switch(recv_state) {
	case recv_state_ok:		next_state = fsm_decode_state;
							break;
	case recv_state_bad:	next_state = fsm_receive_state;
							break;
	default:				next_state = fsm_purge_state;
	}
	return next_state;
}

fsm_state_t switch_state_after_decode(const decode_state_t decode_state)
{
	fsm_state_t next_state;
	switch(decode_state) {
	case decode_state_bad:	next_state = fsm_purge_state;
							break;
	case decode_state_ok:	next_state = fsm_handle_state;
							break;
	default:				next_state = fsm_purge_state;
	}
	return next_state;
}

fsm_state_t switch_state_after_handle(const handle_state_t handle_state)
{
	fsm_state_t next_state;
	switch(handle_state) {
	case handle_state_rst:		next_state = fsm_reset_state;
								break;

	case handle_state_main:		next_state = fsm_deinit_state;
								break;

	case handle_state_res:		/*next_state = fsm_respond_state;*/
								next_state = fsm_code_state;
								break;

	case handle_state_err:		next_state = fsm_error_state;
								break;

	default:					next_state = fsm_purge_state;
								break;
	}
	return next_state;
}

