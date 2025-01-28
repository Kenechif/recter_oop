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
#include "../otp/otp.h"

extern const int max_events_per_state;

uint32_t otp_codeInt2;


typedef enum
{
	_progState_Handler2,
	_idleState_Handler2,
	_inactiveState_Handler2,
	_nozzleup_waitingforauthState_Handler2,
	_authorised_nozzledown_State_Handler2,
	_authorised_nozzleup_State_Handler2,
	_authorisation_paused_State_Handler2,
	_authorisation_resumed_State_Handler2,
	_filling_State_Handler2,
	_filling_paused_State_Handler2,
	_filling_resumed_State_Handler2,
	_keypad_entry_State_Handler2,
	_operator_State_Handler2,
	_savesettings_State_Handler2,
	_read_flash_State_Handler2,
	_write_flash_State_Handler2,
	_switchedoffState_Handler2,
	_pnpState_Handler2,
	_fillingcompleteState_Handler2,
	_resetState_Handler2,
	_filledmamo_State_Handler2,
	_tone_entry_State_Handler2
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
  eSystemEvent  states2[12];
} sStateEventMachine2;
//-----------------------------------------------
eSystemEvent eNewEvent2;
eSystemState eNextState2,eLastState2,ePrevState2;



// state Handlers definition...
eSystemState progState_Handler2(void);
eSystemState idleState_Handler2(void);
eSystemState inactiveState_Handler2(void);
eSystemState nozzleup_waitingforauthState_Handler2(void);
eSystemState authorised_nozzleup_State_Handler2(void);
eSystemState authorised_nozzledown_State_Handler2(void);
eSystemState authorisation_paused_State_Handler2(void);
eSystemState authorisation_resumed_State_Handler2(void);
eSystemState filling_State_Handler2(void);
eSystemState filling_paused_State_Handler2(void);
eSystemState filling_resumed_State_Handler2(void);
eSystemState keypad_entry_State_Handler2(void);
eSystemState operator_State_Handler2(void);
eSystemState savesettings_State_Handler2(void);



//////////////////////////////////////////////////////////
//////////////////// EXTRAS FOR GO ///////////////////////

eSystemState filledmamo_State_Handler2(void);
eSystemState pnpState_Handler2(void);
eSystemState switchedoffState_Handler2(void);

///////////////////////////////////////////////////////////

eSystemState tone_entry_State_Handler2(void);


//eSystemState read_flash_state_Handler(void);  //moved to log.h
//eSystemState write_flash_state_Handler(void);

//event handlers...
eSystemState timeout_Handler2(void);
eSystemState authorise_Handler2(void);
eSystemState auth_command_Handler2(void);
eSystemState nozzleup_Handler2(void);
eSystemState nozzledown_Handler2(void);
eSystemState pause_Handler2(void);
eSystemState resume_Handler2(void);
eSystemState keyup_Handler2(void);
eSystemState keydown_Handler2(void);
eSystemState keypress_Handler2(void);
eSystemState tot_error_Handler2(void);
eSystemState key19_Handler2(void);
eSystemState filling_paused_Handler2(void);
eSystemState filling_resumed_Handler2(void);
eSystemState filling_pulse_Handler2(void);
eSystemState error_clear_Handler2(void);
eSystemState operator_Handler2(void);
eSystemState function_key_Handler2(void);



//////////////////////////////////////////////////////////
//////////////////// EXTRAS FOR GO ///////////////////////

eSystemState stopcommand_Handler2(void);
eSystemState resetcommand_Handler2(void);
eSystemState _fillingcomplete_Handler2(void);
eSystemState switchoffcommand_Handler2(void);
eSystemState authorisecommand_Handler2(void);
eSystemState hardwarereset_Handler2(void);
eSystemState hardwareerror_Handler2(void);
eSystemState auth_suspendcommand_Handler2(void);
eSystemState filling_suspendcommand_Handler2(void);
eSystemState auth_resumecommand_Handler2(void);
eSystemState filling_resumecommand_Handler2(void);
eSystemState mamo_Handler2(void);
///////////////////////////////////////////////////////////

eSystemState tone_Handler2(void);


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

extern uint8_t mamo_reached_flag2,
			   mamo_reached_flag2_1,
			   filling_mamo_flag2,
			   stopFlag_source2,
			   hardwareError_flag2,
			   hardwareErrorFlag_source2,
			   nozzlezUp2,
			   mamo_fillingInfo_send2;

extern uint8_t nozzle_flag2,
			   nozzle_flag_key2,
			   nozzle_flag_key_old2;

extern uint16_t timer_go;
uint8_t go_timeOut2;

//static bool mth_success2 = false,
//			clock_save2 = false;

extern float lastVolumeSale2,
	  	  	 lastVolumeSale2c,
			 lastAmountSale2,
			 lastAmountSale2c;

extern uint8_t totalizer_saveStatus2;

extern uint8_t ctTimed_day2;

extern ctTimed_settings ctTimed_settingsB;

extern _calibrationData calibrationData[2];

extern OtpSeed_Session otpSeed_session[2];

int8_t nozzleup_awaitingauth_state_not_timedOut2;

extern uint8_t batteryStatus;

extern uint8_t RxBuf[RxBuf_SIZE],
			   MainBuf[MainBuf_SIZE];

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx,
						 hdma_usart2_tx;

extern uint32_t mech_totalizer1,
		 	    mech_totalizer2,
				mech_totalizer_old1,
				mech_totalizer_old2;

extern float mechTotalizer1,
	  	  	 mechTotalizer2,
			 mechTotalizer1_,
	  	  	 mechTotalizer2_;

void state_ini2(void);
void states2(void);
void reset_timer2(int tm);
void stop_timer2(void);
void start_timer2(int tm);

void resetTimer_tone2(int tm);
void startTimer_tone2(int tm);
void decTimer_tone2(void);
void stopTimer_tone2(void);

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

bool leapYear_calc2(int8_t year);
void save_date2(int8_t _entri);

uint16_t get_ctTime2(void);

float dp2(float flt,int n);
float amt2price2(float amt_);

void keypad_zerorize2(void);

void keypad_fillingUpdate2(void);

void correctArray2(float v[4], corrected_sid sid);

float scale_to_range2(float value);

float scale_to_original2(float scaled_value);

float get_fractional_part2(float num);


#ifdef __cplusplus
}
#endif
#endif /* INC_STATES_H_2 */

