/*
 * states.h
 *
 *  Created on: Mar 7, 2022
 *      Author: tunji
 */

#ifndef INC_STATES_H_
#define INC_STATES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "settings.h"

int8_t nozzleup_awaitingauth_state_not_timedOut;

extern const int max_events_per_state ;

typedef enum
{
	prog_State,
    idle_State,
    inactive_State,
    nozzleup_waitingforauth_State,
	authorised_nozzledown_State,
    authorised_nozzleup_State,
	authorisation_paused_State,
	filling_State,
	filling_paused_State,
	keypad_entry_State,
	operator_State,
	savesettings_State,
	read_flash_state,
	write_flash_state,
    last_State,
} eSystemState;

typedef enum
{
	_progstate_Handler,
	_idlestate_Handler,
	_inactivestate_Handler,
	_nozzleup_waitingforauthState_Handler,
	_authorised_nozzledown_State_Handler,
	_authorised_nozzleup_State_Handler,
	_authorisation_paused_State_Handler,
	_filling_state_Handler,
	_filling_paused_state_Handler,
	_keypad_entry_State_Handler,
	_operator_State_Handler,
	_savesettings_State_Handler,
	_read_flash_state_Handler,
	_write_flash_state_Handler
}eSystemStateHandler;

//Different type events
typedef enum
{
    _timeout_Event = 1,
    _authorise_Event,
	_auth_command_Event,
    _nozzleup_Event,
	_nozzledown_Event,
    _pause_Event,
    _resume_Event,
	_keyup_Event,
	_keydown_Event,
	_keypress_Event,
	_tot_error_Event,
	_key19_Event,
	_filling_paused_Event,
	_filling_resumed_Event,
	_filling_pulse_Event,
	_error_clear_Event,
	_operator_Event,
    _no_Event
} eSystemEvent;

//typedef of function pointer of eventHandlers.
typedef eSystemState (*pfEventHandler)(void);

//typedef of function pointer of stateHandlers.
typedef eSystemState (*pfStateHandler)(void);
//------------------------------------------------
//structure of  event with event handler
//state and
typedef struct
{
  //eSystemState eStateMachine;
  eSystemEvent   eStateMachineEvent;
  pfEventHandler pfEventMachineHandler;
}sEventMachine;
//-----------------------------------------------
//state and state handler...
typedef struct
{
  eSystemState   eStateMachine;
  pfStateHandler pfStateMachineHandler;
} sStateMachine;
//-----------------------------------------------
//state and state handler...
typedef struct
{
  eSystemState   eStateMachine;
  pfStateHandler pfStateMachineHandler;
  eSystemEvent  states[8];
} sStateEventMachine;
//-----------------------------------------------
eSystemEvent eNewEvent;
eSystemState eNextState, eLastState, ePrevState;

typedef enum
{
 non,
 level0,
 level1,
 level2
}access_lev;

/*typedef enum
{
    side1 = 1,
	side2
}pump_side;     refer to settings.h file  */

typedef enum
{
    l1 = 1,
	l2
}sales_v;

typedef enum
{
	not_auth,
	no_auth,
	authed
}auth_state;



enum
{
	setmode,
	setaddress,
	setnozzle,
	default_sell_type,
	setprice,
	set_no_flow_time,
	setmax_amt,
	set_password,
	send_config,
	get_config,
	clr_log_,
	clr_tot_,
	calibrate_,
	display_,
	nothing,
	setclock
};

float price_real, amt_real;
float amt_, price_;

static bool mth_success = false,
			clock_save = false;

//int8_t pump_LitreOverflow = 0;
//static int8_t idleState_flag = 1;

extern float lastSale1,
	  	  	 lastSale1c;

void state_ini(void);
void states(void);
void reset_timer(int tm);
void stop_timer(void);
void start_timer(int tm);



uint8_t long_press_key();
uint8_t long_press_log();
uint8_t long_press_tot();

bool leapYear_calc(int8_t year);
void save_date(int8_t _entri);

#ifdef __cplusplus
}
#endif
#endif /* INC_STATES_H_ */

