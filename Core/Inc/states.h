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
#include "../otp/otp.h"

int8_t nozzleup_awaitingauth_state_not_timedOut;
uint32_t otp_codeInt1;

extern const int max_events_per_state ;

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart5;

//typedef enum
//{
//	prog_State,
//    idle_State,
//    inactive_State,
//    nozzleup_waitingforauth_State,
//	authorised_nozzledown_State,
//    authorised_nozzleup_State,
//	authorisation_paused_State,
//	filling_State,
//	filling_paused_State,
//	keypad_entry_State,
//	operator_State,
//	savesettings_State,
//	read_flash_state,
//	write_flash_state,
//    last_State,
//} eSystemState;

typedef enum
{
	prog_State,
    idle_State,
    inactive_State,
    nozzleup_waitingforauth_State,
	authorised_nozzledown_State,
    authorised_nozzleup_State,
	authorisation_paused_State,
	authorisation_resumed_State,
	filling_State,
	filling_paused_State,
	filling_resumed_State,
	keypad_entry_State,
	operator_State,
	savesettings_State,
	read_flash_State,
	write_flash_State,
	switchedoff_State,
	pnp_State,
//    fillingcomplete_State,
//	reset_State,
	filledmamo_State,
	last_State,
} eSystemState;

//typedef enum
//{
//	_progstate_Handler,
//	_idlestate_Handler,
//	_inactivestate_Handler,
//	_nozzleup_waitingforauthState_Handler,
//	_authorised_nozzledown_State_Handler,
//	_authorised_nozzleup_State_Handler,
//	_authorisation_paused_State_Handler,
//	_filling_state_Handler,
//	_filling_paused_state_Handler,
//	_keypad_entry_State_Handler,
//	_operator_State_Handler,
//	_savesettings_State_Handler,
//	_read_flash_state_Handler,
//	_write_flash_state_Handler
//}eSystemStateHandler;

typedef enum
{
	_progState_Handler,
	_idleState_Handler,
	_inactiveState_Handler,
	_nozzleup_waitingforauthState_Handler,
	_authorised_nozzledown_State_Handler,
	_authorised_nozzleup_State_Handler,
	_authorisation_paused_State_Handler,
	_authorisation_resumed_State_Handler,
	_filling_State_Handler,
	_filling_paused_State_Handler,
	_filling_resumed_State_Handler,
	_keypad_entry_State_Handler,
	_operator_State_Handler,
	_savesettings_State_Handler,
	_read_flash_State_Handler,
	_write_flash_State_Handler,
	_switchedoffState_Handler,
	_pnpState_Handler,
	_fillingcompletestate_Handler,
	_resetState_Handler,
	_filledmamo_State_Handler
}eSystemStateHandler;


////Different type events
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
//	_function_key_Event,
//    _no_Event
//} eSystemEvent;

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
	_function_key_Event,
	_stopcommand_Event,
	_resetcommand_Event,
	_fillingcomplete_Event,   //_priceupdate_Event,
	_switchoffcommand_Event,
	_authorisecommand_Event,
	_hardwarereset_Event,
	_hardwareerror_Event,
	_auth_suspendcommand_Event,
	_filling_suspendcommand_Event,
	_auth_resumecommand_Event,
	_filling_resumecommand_Event,
	_mamo_Event,
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
  eSystemEvent  states[12];
} sStateEventMachine;
//-----------------------------------------------
eSystemEvent eNewEvent1;
eSystemState eNextState1, eLastState1, ePrevState;

typedef enum
{
 non,
 level1,
 level2,
 level3
}access_lev;

typedef enum
{
	 pass1,
	 pass2,
	 pass3,
}pass_lev;

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
	authed,
	pre_otp_authed,
	otp_authed
}auth_state;



enum
{
	setmode,
	setaddress,
	setnozzle,
	display_orientation,
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
	setclock,
	number_of_sides,
	suppressed_display_vol,
	calibration_can,
	calibration_type_,
	shift_login_type_,
	number_of_shifts_,
	shift_total,
	keypress_tone_,
	communication_card,
	pulser_config,
	valve_config,
	no_calibration
};


//"Sides No", "Start CL", "Calib.Can", "Shift No"};
//"Calib.typ"

//float price_real1, amt_real1;
//float amt_, price_;

//static bool mth_success = false,
//			clock_save = false;

//int8_t pump_LitreOverflow = 0;
//static int8_t idleState_flag = 1;

extern OtpSeed_Session otpSeed_session[2];

extern float lastVolumeSale1,
	  	  	 lastVolumeSale1c,
			 lastAmountSale1,
			 lastAmountSale1c;

extern uint8_t ctTimed_day1;

extern uint8_t overflow_count1;

extern ep1_mt mt_pump[2];

extern ctTimed_settings ctTimed_settingsA;

extern _calibrationData calibrationData[2];

extern uint8_t batteryStatus;

extern uint8_t RxBuf[RxBuf_SIZE],
			   MainBuf[MainBuf_SIZE];

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx,
						 hdma_usart2_tx;

extern uint8_t mamo_reached_flag1,
			   mamo_reached_flag1_1,
			   filling_mamo_flag1,
			   stopFlag_source1,
			   hardwareError_flag1,
			   hardwareErrorFlag_source1,
			   nozzlezUp1,
			   mamo_fillingInfo_send1;

extern uint8_t nozzle_flag,
			   nozzle_flag_key1,
			   nozzle_flag_key_old1;

extern uint16_t timer_go;
uint8_t go_timeOut1;

// state Handlers definition...
eSystemState progState_Handler(void);
eSystemState idleState_Handler(void);
eSystemState inactiveState_Handler(void);
eSystemState nozzleup_waitingforauthState_Handler(void);
eSystemState authorised_nozzleup_State_Handler(void);
eSystemState authorised_nozzledown_State_Handler(void);
eSystemState authorisation_paused_State_Handler(void);
eSystemState authorisation_resumed_State_Handler(void);
eSystemState filling_State_Handler(void);
eSystemState filling_paused_State_Handler(void);
eSystemState filling_resumed_State_Handler(void);
eSystemState keypad_entry_State_Handler(void);
eSystemState operator_State_Handler(void);
eSystemState savesettings_State_Handler(void);

//eSystemState read_flash_state_Handler(void);  //moved to log.h
//eSystemState write_flash_state_Handler(void);


//////////////////////////////////////////////////////////
//////////////////// EXTRAS FOR GO ///////////////////////

eSystemState filledmamo_State_Handler(void);
eSystemState pnpState_Handler(void);
eSystemState switchedoffState_Handler(void);

///////////////////////////////////////////////////////////




//event handlers...
eSystemState timeout_Handler(void);
eSystemState authorise_Handler(void);
eSystemState auth_command_Handler(void);
eSystemState nozzleup_Handler(void);
eSystemState nozzledown_Handler(void);
eSystemState pause_Handler(void);
eSystemState resume_Handler(void);
eSystemState keyup_Handler(void);
eSystemState keydown_Handler(void);
eSystemState keypress_Handler(void);
eSystemState tot_error_Handler(void);
eSystemState key19_Handler(void);
eSystemState filling_paused_Handler(void);
eSystemState filling_resumed_Handler(void);
eSystemState filling_pulse_Handler(void);
eSystemState error_clear_Handler(void);
eSystemState operator_Handler(void);
eSystemState function_key_Handler(void);


//////////////////////////////////////////////////////////
//////////////////// EXTRAS FOR GO ///////////////////////

eSystemState stopcommand_Handler(void);
eSystemState resetcommand_Handler(void);
//eSystemState priceupdate_Handler(void);
eSystemState _fillingcomplete_Handler(void);
eSystemState switchoffcommand_Handler(void);
eSystemState authorisecommand_Handler(void);
eSystemState hardwarereset_Handler(void);
eSystemState hardwareerror_Handler(void);
eSystemState auth_suspendcommand_Handler(void);
eSystemState filling_suspendcommand_Handler(void);
eSystemState auth_resumecommand_Handler(void);
eSystemState filling_resumecommand_Handler(void);
eSystemState mamo_Handler(void);
///////////////////////////////////////////////////////////


void state_ini(void);
//void states(void);
void states_1(void);
void reset_timer(int tm);
void stop_timer(void);
void start_timer(int tm);

uint32_t price2pulser(float price);
uint32_t amt2pulser(float amt);
float pulser2price(uint32_t pulse_);
float amt2price(float amt_);
float pulser2amt(uint32_t pulse_);
float pulser2amt_R(uint32_t pulse_);
void make_string(sellmode_ sll,float pr);

uint8_t long_press_key();
uint8_t long_press_progExit();

uint8_t long_press_log();
uint8_t long_press_tot();

bool leapYear_calc(int8_t year);
void save_date(int8_t _entri);

uint16_t get_ctTime1(void);

void keypad_zerorize(void);

void keypad_fillingUpdate1(void);

void correctArray1(float v[4]);

#ifdef __cplusplus
}
#endif
#endif /* INC_STATES_H_ */

