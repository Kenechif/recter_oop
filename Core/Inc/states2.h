/*
 * states.h
 *
 *  Created on: Mar 7, 2022
 *      Author: tunji
 */

#ifndef INC_STATES_H_2
#define INC_STATES_H_2

#ifdef __cplusplus
extern "C" {
#endif

#include "settings.h"

extern const int max_events_per_state ;


typedef enum
{
	_progstate_Handler2,
	_idlestate_Handler2,
	_inactivestate_Handler2,
	_nozzleup_waitingforauthState_Handler2,
	_authorised_nozzledown_State_Handler2,
	_authorised_nozzleup_State_Handler2,
	_authorisation_paused_State_Handler2,
	_filling_state_Handler2,
	_filling_paused_state_Handler2,
	_keypad_entry_State_Handler2,
	_operator_State_Handler2,
	_savesettings_State_Handler2,
	_read_flash_state_Handler2,
	_write_flash_state_Handler2
}eSystemStateHandler2;

//Different type events
//typedef enum
//{
//    _timeout_Event = 1,
//    _authorise_Event,
//	_auth_command_Event,
//    _nozzleup_Event,
//	_nozzledown_Event,
//    _pause_Event,
//    _resume_Event,
//	_keyup_Event,
//	_keydown_Event,
//	_keypress_Event,
//	_tot_error_Event,
//	_key19_Event,
//	_filling_paused_Event,
//	_filling_resumed_Event,
//	_filling_pulse_Event,
//	_error_clear_Event,
//	_operator_Event,
//    _no_Event
//} eSystemEvent2;

//typedef of function pointer of eventHandlers.
typedef eSystemState (*pfEventHandler2)(void);

//typedef of function pointer of stateHandlers.
typedef eSystemState (*pfStateHandler2)(void);
//------------------------------------------------
//structure of  event with event handler
//state and
typedef struct
{
  //eSystemState eStateMachine;
  eSystemEvent   eStateMachineEvent2;
  pfEventHandler2 pfEventMachineHandler2;
}sEventMachine2;
//-----------------------------------------------
//state and state handler...
typedef struct
{
  eSystemState   eStateMachine2;
  pfStateHandler2 pfStateMachineHandler2;
} sStateMachine2;
//-----------------------------------------------
//state and state handler...
typedef struct
{
  eSystemState   eStateMachine2;
  pfStateHandler2 pfStateMachineHandler2;
  eSystemEvent  states2[8];
} sStateEventMachine2;
//-----------------------------------------------
eSystemEvent eNewEvent2;
eSystemState eNextState2,eLastState2,ePrevState2;

//typedef enum
//{
// non,
// level0,
// level1,
// level2
//}access_lev;
//
///*typedef enum
//{
//    side1 = 1,
//	side2
//}pump_side;     refer to settings.h file  */
//
//typedef enum
//{
//    l1 = 1,
//	l2
//}sales_v;
//
//typedef enum
//{
//	not_auth,
//	no_auth,
//	authed
//}auth_state;
//
//
//
//enum
//{
//	setmode,
//	setaddress,
//	setnozzle,
//	default_sell_type,
//	setprice,
//	set_no_flow_time,
//	setmax_amt,
//	set_password,
//	send_config,
//	get_config,
//	clr_log_,
//	clr_tot_,
//	calibrate_,
//	display_,
//	nothing
//};

int8_t nozzleup_awaitingauth_state_not_timedOut2;

void state_ini2(void);
void states2(void);
void reset_timer2(int tm);
void stop_timer2(void);
void start_timer2(int tm);

uint32_t price2pulser2(float price);
uint32_t amt2pulser2(float amt);
float pulser2price2(uint32_t pulse_);
float amt2price2(float amt_);
float pulser2amt2(uint32_t pulse_);
float pulser2amt_R2(uint32_t pulse_);
void make_string2(sellmode_ sll,float pr);


uint8_t long_press_key2();
uint8_t long_press_progExit2();

uint8_t long_press_log2();
uint8_t long_press_tot2();


static bool mth_success2 = false,
			clock_save2 = false;

extern float lastVolumeSale2,
	  	  	 lastVolumeSale2c;

extern ctTimed_settings ctTimed_settingsB;

bool leapYear_calc2(int8_t year);
void save_date2(int8_t _entri);

uint16_t get_ctTime2(void);

float dp2(float flt,int n);
float amt2price2(float amt_);


#ifdef __cplusplus
}
#endif
#endif /* INC_STATES_H_2 */

