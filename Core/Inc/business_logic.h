/*
 * business_logic.h
 *
 *  Created on: 10 Mar 2025
 *      Author: Jheebolar
 */

#ifndef INC_BUSINESS_LOGIC_H_
#define INC_BUSINESS_LOGIC_H_


//#ifndef BUSINESS_LOGIC_H
//#define BUSINESS_LOGIC_H

#include "nozzle.h"

void business_handle_lift(Nozzle* self);
void business_handle_authorize(Nozzle* self);
void business_handle_pump_trigger(Nozzle* self);
void business_handle_pump_release(Nozzle* self);
void business_handle_payment(Nozzle* self);
float business_calculate_payment(const Nozzle* self);


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
//  initialise the array of structure of State and state handlers and their allowed  events.
//         { <state>,<handler>,{<allowed event1>,<allowed event2>,..,<allowed eventn>}}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//


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


/*********************************************************
 |
 |
 ********************************************************/
eSystemState tone_entry_State_Handler(void);



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

eSystemState tone_Handler(void);


void state_ini(void);
//void states(void);
void states_1(void);
void reset_timer(int tm);
void stop_timer(void);
void start_timer(int tm);

void resetTimer_tone1(int tm);
void startTimer_tone1(int tm);
void decTimer_tone1(void);
void stopTimer_tone1(void);

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

void correctArray1(float v[4], corrected_sid sid);

float scale_to_range1(float value);

float scale_to_original1(float scaled_value);

float get_fractional_part1(float num);


//#endif // BUSINESS_LOGIC_H


#endif /* INC_BUSINESS_LOGIC_H_ */
