#ifndef FSM_H_SENTRY
#define FSM_H_SENTRY

void fsm_process();

typedef enum {
	fsm_idle_state,
	fsm_init_state,
	fsm_wait_state,
	fsm_receive_state,
	fsm_decode_state,
	fsm_handle_state,
	fsm_respond_state,
	fsm_purge_state,
	fsm_reset_state,
	fsm_error_state,
	fsm_deinit_state,
	fsm_gomain_state
} fsm_state_t;

typedef enum {
	recv_state_ok,
	recv_state_bad
} recv_state_t;

typedef enum {
	decode_state_ok,
	decode_state_bad
} decode_state_t;

typedef enum {
	handle_state_rst,
	handle_state_err,
	handle_state_main,
	handle_state_res
} handle_state_t;

#endif
