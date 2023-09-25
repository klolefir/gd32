#include "fsm.h"
#include "init.h"
#include "gpio.h"
#include "rcu.h"
#include "bloader.h"
#include "kestring.h"
#include "operator.h"

static void init();
static recv_state_t receive(buff_t *buff_set);
static decode_state_t decode(const buff_t *buff_set, bl_packet_t *packet);
static check_state_t check(bl_packet_t *packet);
static handle_state_t handle(bl_packet_t *packet);
static void code(buff_t *buff_set, bl_packet_t *packet);
static void respond(const buff_t *buff_set);
static void purge(buff_t *buff_set);
static void reset();
static void deinit();
static void gomain();

static fsm_state_t switch_state_after_recv(const recv_state_t recv_status);
static fsm_state_t switch_state_after_decode(const decode_state_t decode_state);
static fsm_state_t switch_state_after_check(const check_state_t check_state);
static fsm_state_t switch_state_after_handle(const handle_state_t handle_state);

void fsm_process()
{
	buff_t buff_set;
	bl_packet_t packet;
	buff_set.cnt = 0;

	recv_state_t 	recv_state;
	decode_state_t	decode_state;
	check_state_t	check_state;
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

		case fsm_receive_state:	recv_state = receive(&buff_set);
								state = switch_state_after_recv(recv_state);
								break;

		case fsm_decode_state:	decode_state = decode(&buff_set, &packet);
								state = switch_state_after_decode(decode_state);
								break;

		case fsm_check_state:	check_state = check(&packet);
								state = switch_state_after_check(check_state);
								break;

		case fsm_handle_state:	handle_state = handle(&packet);
								state = switch_state_after_handle(handle_state);
								break;

		case fsm_code_state:	code(&buff_set, &packet);
								state = fsm_respond_state;
								break;

#if 0
		case fsm_error_state:	error(&buff_set);
								state = fsm_respond_state;
								break;
#endif

		case fsm_respond_state:	respond(&buff_set);
								state = fsm_purge_state;
								break;

		case fsm_purge_state:	purge(&buff_set);
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
	operator_init();
#if 0
	init_operator();
	init_operator_timer();
#endif
}

recv_state_t receive(buff_t *buff_set)
{
	recv_state_t recv_state = operator_receive(buff_set);
	return recv_state;
#if 0
	recv_state_t recv_state = recv_state_bad;

	uint32_t *req_cnt = &(buff_set->cnt);
	buff_size_t *req_buff = (buff_set->buff);

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
#endif
}

decode_state_t decode(const buff_t *buff_set, bl_packet_t *packet)
{
	const uint32_t *req_cnt = &(buff_set->cnt);
	const buff_size_t *req_buff = (buff_set->buff);
	uint16_t sig_in;
	/*uint32_t crc, crc_len;*/

	if(!(*req_cnt))
		return decode_state_bad;

	kememcpy(&sig_in, &req_buff[bl_sig_pos], bl_sig_size);
	if(sig_in != bl_sig_in)
		return decode_state_bad;

	kememcpy(&packet->cmd, &req_buff[bl_cmd_pos], bl_cmd_size);
	kememcpy(&packet->addr, &req_buff[bl_addr_pos], bl_addr_size);
	kememcpy(&packet->len, &req_buff[bl_len_pos], bl_len_size);
	packet->data.buff = (uint32_t *)&req_buff[bl_data_pos];
	packet->data.cnt = 0;

	return decode_state_ok;
}

check_state_t check(bl_packet_t *packet)
{
	uint8_t bad_cmd, bad_addr, bad_len;
	uint32_t addr, len, max_addr;
	bl_cmd_t cmd;

	cmd = packet->cmd;
	addr = packet->addr;
	len = packet->len;

	bad_cmd = 	cmd != bl_cmd_write	&&
				cmd != bl_cmd_read 	&&
				cmd != bl_cmd_erase &&
				cmd != bl_cmd_reset	&&
				cmd != bl_cmd_gomain;

	if(bad_cmd) {
		packet->status = bl_bcmd;
		return check_state_bad;
	}

	bad_addr = (addr < bl_min_addr) || (addr > bl_max_addr);
	if(bad_addr) {
		packet->status = bl_baddr;
		return check_state_bad;
	}

	max_addr = addr + len;
	bad_len = (max_addr < bl_min_addr || max_addr > bl_max_addr)
			|| (len > bl_max_page_size);
	if(bad_len) {
		packet->status = bl_blen;
		return check_state_bad;
	}

	packet->status = bl_ok;
	return check_state_ok;
}

handle_state_t handle(bl_packet_t *packet)
{
	uint32_t *addr, *len, *data;
	addr = &packet->addr;
	len = &packet->len;
	data = packet->data.buff;

	switch(packet->cmd) {
	case bl_cmd_reset: 	return handle_state_rst;
						break;

	case bl_cmd_gomain: return handle_state_main;
						break;

	case bl_cmd_write:	packet->status = bloader_write(addr, data, len);
						packet->data.cnt = 0;
						break;

	case bl_cmd_read:	packet->status = bloader_read(addr, data, len);
						packet->data.cnt = *len;
						break;

	case bl_cmd_erase:	packet->status = bloader_erase(addr, len);
						packet->data.cnt = 0;
						break;
	}

	return handle_state_res;
}

void code(buff_t *buff_set, bl_packet_t *packet)
{
	uint16_t sig;
	uint32_t *ans_cnt = &buff_set->cnt;
	buff_size_t *ans_buff = (buff_set->buff);

	*ans_cnt = 0;
	kememcpy(&ans_buff[bl_addr_pos], &packet->addr, bl_addr_size);
	*ans_cnt += bl_addr_size;
	kememcpy(&ans_buff[bl_len_pos], &packet->len, bl_len_size);
	*ans_cnt += bl_len_size;

	if(packet->status != bl_ok) {
		kememcpy(&ans_buff[bl_status_pos], &packet->status, bl_status_size);
		*ans_cnt += bl_status_size;
		sig = bl_esig_out;
	} else {
		kememcpy(&ans_buff[bl_cmd_pos], &packet->cmd, bl_cmd_size);
		*ans_cnt += bl_status_size;
		*ans_cnt += packet->data.cnt;
		sig = bl_sig_out;
	}
	kememcpy(&ans_buff[bl_sig_pos], &sig, bl_sig_size);
	*ans_cnt += bl_sig_size;
}

void respond(const buff_t *buff_set)
{
	operator_transmit(buff_set);
#if 0
	const uint32_t ans_cnt = buff_set->cnt;
	const buff_size_t *ans_buff = (buff_set->buff);
	
	if(ans_cnt)
		xusart_put_buff(&usart0, ans_buff, ans_cnt);
#endif
}

void purge(buff_t *buff_set)
{
	buff_set->cnt = 0;
}

void deinit()
{
	rcu_deinit();
	xrcu_reset_ahb();
	xrcu_reset_apb1();
	xrcu_reset_apb2();
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
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

	__DSB();
	__ISB();

	for (int i = 0; i < 8; i++) {
		NVIC->ICER[i] = 0xFFFFFFFF; // disable IRQ
		NVIC->ICPR[i] = 0xFFFFFFFF; // clear pending IRQ
	}

	SCB->VTOR = main_addr;

	uint32_t msp = *((volatile uint32_t *)(main_addr));
	__set_MSP(msp);

	__DSB();
	__ISB();

	__enable_irq();

	uint32_t main_jump_addr = *((uint32_t *)(main_addr + 0x04));
	main_jump = (main_f)main_jump_addr;
	main_jump();
}

fsm_state_t switch_state_after_recv(const recv_state_t recv_state)
{
	switch(recv_state) {
	case recv_state_ok:		return fsm_decode_state;
	case recv_state_bad:	return fsm_receive_state;
	default:				return fsm_purge_state;
	}
}

fsm_state_t switch_state_after_decode(const decode_state_t decode_state)
{
	switch(decode_state) {
	case decode_state_bad:	return fsm_purge_state;
	case decode_state_ok:	return fsm_check_state;
	default:				return fsm_purge_state;
	}
}

fsm_state_t switch_state_after_check(const check_state_t check_state)
{
	switch(check_state) {
	case check_state_ok:	return fsm_handle_state;
	case check_state_bad:	return fsm_code_state;
	default:				return fsm_purge_state;
	}
}

fsm_state_t switch_state_after_handle(const handle_state_t handle_state)
{
	switch(handle_state) {
	case handle_state_rst:		return fsm_reset_state;
	case handle_state_main:		return fsm_deinit_state;
	case handle_state_res:		return fsm_code_state;
	default:					return fsm_purge_state;
	}
}

