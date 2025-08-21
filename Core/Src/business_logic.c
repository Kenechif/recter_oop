/*
 * business_logic.c
 *
 *  Created on: 10 Mar 2025
 *      Author: Fidel
 */




#include "business_logic.h"



// --------------------------
// 4. Business Logic Implementation
// --------------------------


void business_handle_lift(Nozzle* self) {
    if (self->state == STATE_IDLE) {
        self->nozzle_lifted = true;
        self->state = STATE_AUTHORIZING;
    }
}


// ... (Implement other business_handle_* functions and business_calculate_payment)

void business_handle_authorize(Nozzle* self) {
    if(self->state == STATE_AUTHORIZING) {
        self->authorized = true;
        self->state = STATE_PUMPING;
    }
}

void business_handle_pump_trigger(Nozzle* self) {
    if(self->state == STATE_PUMPING && !self->pump_active) {
        self->pump_active = true;
    }
}

void business_handle_pump_release(Nozzle* self) {
    if(self->state == STATE_PUMPING && self->pump_active) {
        self->pump_active = false;
        self->state = STATE_PAYMENT_PENDING;
    }
}

void business_handle_payment(Nozzle* self) {
    if(self->state == STATE_PAYMENT_PENDING) {
        self->state = STATE_IDLE;
        self->fuel_dispensed = 0.0f;
        self->nozzle_lifted = false;
        self->authorized = false;
    }
}

float business_calculate_payment(const Nozzle* self) {
    return self->fuel_dispensed * self->price_per_liter;
}









GPIO_TypeDef *valve_port



/*
 * sets the precision of the supplied float
 */
float dp(float flt, int n)
{
  uint8_t chrr[10] = {0};
  uint8_t chrr_[10] = {0};

  char *endPtr;

  if(n == 1) snprintf(chrr_, sizeof(chrr_), "%.1f", flt);
  else if(n == 2) snprintf(chrr_, sizeof(chrr_), "%.2f", flt);
  else if(n == 3) snprintf(chrr_, sizeof(chrr_), "%.3f", flt);

//	float f = atof(chrr_);

	float f  = strtof(chrr_, &endPtr);
// 	f += 0.00011;  //make small correction for the inherent rounddown.
 //	f+= 0.000011;

	if(f < 1.0)
	{
		return f;
	}

	float roundedNum = round_off(f, n);

	return roundedNum;
}


// initialises the line 1 and 2 display variable.
void write_v(Nozzle* self, uint8_t i, char lcd_[9])
{
	if (i == 1)      //first line
	{
		  for(uint8_t ii = 0 ; ii < 9; ii++)
			{
			  self->upper[ii] = lcd_[ii];
			  self->upper[ii+1] = 0;
			}
	}

	else if (i == 2)   //second line
	 {
		  for(uint8_t ii = 0 ; ii < 9; ii++)
			{
			  self->middle[ii] = lcd_[ii];
			  self->middle[ii+1] = 0;
			}
   	}

	else if (i == 3)   //third line
	{
		for(uint8_t ii = 0 ; ii<7 ; ii++)
		{
			self->keyboard[ii] = lcd_[ii];
			self->keyboard[ii+1] = 0;
		}
	}
}

//--------------------------------------------
void reset_timer(Nozzle* self, int tm)
{
	self->time_e = tm;
	self->timer_flag = 0;
	self->timer_flag_old = 1;
	self->st = 0;
}

void start_timer(Nozzle* self, int tm)
{
	self->time_e = tm;
	self->st = 1;
}

//=======================================
void dec_timer(Nozzle* self)
{
  if(self->st == 1)
	{
	  if (self->time_e > 0)
		  self->time_e--;
	   if(self->time_e == 0)
	   {
		   self->timer_flag = 1;
	   }
	   else
	   {
		   self->timer_flag = 0;
	   }
	}
}
//=======================================
void stop_timer(Nozzle* self)
{
	self->timer_flag_old = 1;
	self->time_e = 0;
	self->st = 0;
}
//=======================================

//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
//--------------------------------------------
void resetTimer_tone(Nozzle* self, int tm)
{
	self->time_tone = tm;
	self->timerFlag_tone = 0;
	self->timerFlagOld_tone = 1;
	self->st_tone = 0;
}

void startTimer_tone(Nozzle* self, int tm)
{
	self->time_tone = tm;
	self->timerFlag_tone = 1;
	self->timerFlagOld_tone = 1;
	self->st_tone = 1;
}

//=======================================
void decTimer_tone(Nozzle* self)
{
  if(self->st_tone == 1)
	{
	   if (self->time_tone > 0)
		   self->time_tone--;

	   if(self->time_tone == 0)
	   {
		   self->timerFlag_tone = 0;
		   self->timerFlagOld_tone = 1;
		   self->st_tone = 0;
	   }
//	   else
//	   {
//		   timerFlag_tone1 = 0;
//	   }
	}
}
//=======================================
void stopTimer_tone(Nozzle* self)
{
	self->timerFlagOld_tone = 1;
	self->time_tone = 0;
	self->st_tone = 0;
}
//4444444444444444444444444444444444444444444444444444444

//YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY

//=======================================
int8_t read_keypad(void);
//int long_press_log();
//int long_press_tot();
//=======================================


////Initialize array of structure of event and event handler
//sEventMachine asEventMachine [] =
//{
//    {_timeout_Event,timeout_Handler},
//    {_authorise_Event,authorise_Handler},
//   	{_auth_command_Event,auth_command_Handler},
//    {_nozzleup_Event,nozzleup_Handler},
//   	{_nozzledown_Event,nozzledown_Handler},
//    {_pause_Event,pause_Handler},
//    {_resume_Event,resume_Handler},
//   	{_keyup_Event,keyup_Handler},
//   	{_keydown_Event,keydown_Handler},
//   	{_keypress_Event,keypress_Handler},
//   	{_tot_error_Event,tot_error_Handler},
//   	{_key19_Event,key19_Handler},
//   	{_filling_paused_Event,filling_paused_Handler},
//   	{_filling_resumed_Event,filling_resumed_Handler},
//   	{_filling_pulse_Event,filling_pulse_Handler},
//	{_error_clear_Event,error_clear_Handler},
//	{_operator_Event,operator_Handler},
//	{_no_Event,0},
//    {_function_key_Event,function_key_Handler}
//};


////Initialize array of structure of event and event handler
//sEventMachine asEventMachine_1 [] =
//{
//    {_timeout_Event,timeout_Handler},
//    {_authorise_Event,authorise_Handler},
//   	{_auth_command_Event,auth_command_Handler},
//    {_nozzleup_Event,nozzleup_Handler},
//   	{_nozzledown_Event,nozzledown_Handler},
//    {_pause_Event,pause_Handler},
//    {_resume_Event,resume_Handler},
//   	{_keyup_Event,keyup_Handler},
//   	{_keydown_Event,keydown_Handler},
//   	{_keypress_Event,keypress_Handler},
//   	{_tot_error_Event,tot_error_Handler},
//   	{_key19_Event,key19_Handler},
//   	{_filling_paused_Event,filling_paused_Handler},
//   	{_filling_resumed_Event, filling_resumed_Handler},
//   	{_filling_pulse_Event, filling_pulse_Handler},
//	{_error_clear_Event, error_clear_Handler},
//	{_operator_Event, operator_Handler},
//	{_no_Event,0},
//    {_function_key_Event, function_key_Handler},
//	{_stopcommand_Event, stopcommand_Handler},
//	{_resetcommand_Event, resetcommand_Handler},
//	{_priceupdate_Event, priceupdate_Handler},
//	{_switchoffcommand_Event, switchoffcommand_Handler},
//	{_authorisecommand_Event, authorisecommand_Handler},
//	{_hardwarereset_Event, hardwarereset_Handler},
//	{_hardwareerror_Event, hardwareerror_Handler},
//	{_auth_suspendcommand_Event, auth_suspendcommand_Handler},
//	{_filling_suspendcommand_Event, filling_suspendcommand_Handler},
//	{_auth_resumecommand_Event, auth_resumecommand_Handler},
//	{_filling_resumecommand_Event, filling_resumecommand_Handler},
//	{_mamo_Event, mamo_Handler},
//};


//Initialize array of structure of event and event handler
sEventMachine asEventMachine_1 [] =
{
    {_timeout_Event, timeout_Handler},
    {_authorise_Event, authorise_Handler},
   	{_auth_command_Event, auth_command_Handler},
    {_nozzleup_Event, nozzleup_Handler},
   	{_nozzledown_Event, nozzledown_Handler},
    {_pause_Event, pause_Handler},
    {_resume_Event, resume_Handler},
   	{_keyup_Event, keyup_Handler},
   	{_keydown_Event, keydown_Handler},
   	{_keypress_Event, keypress_Handler},
   	{_tot_error_Event, tot_error_Handler},
   	{_key19_Event, key19_Handler},
   	{_filling_paused_Event, filling_paused_Handler},
   	{_filling_resumed_Event, filling_resumed_Handler},
   	{_filling_pulse_Event, filling_pulse_Handler},
	{_error_clear_Event, error_clear_Handler},
	{_operator_Event, operator_Handler},
    {_function_key_Event, function_key_Handler},
	{_stopcommand_Event, stopcommand_Handler},
	{_resetcommand_Event, resetcommand_Handler},
	{_fillingcomplete_Event, _fillingcomplete_Handler},            //{_priceupdate_Event, priceupdate_Handler},
	{_switchoffcommand_Event, switchoffcommand_Handler},
	{_authorisecommand_Event, authorisecommand_Handler},
	{_hardwarereset_Event, hardwarereset_Handler},
	{_hardwareerror_Event, hardwareerror_Handler},
	{_auth_suspendcommand_Event, auth_suspendcommand_Handler},
	{_filling_suspendcommand_Event, filling_suspendcommand_Handler},
	{_auth_resumecommand_Event, auth_resumecommand_Handler},
	{_filling_resumecommand_Event, filling_resumecommand_Handler},
	{_mamo_Event, mamo_Handler},
	{_tone_Event, tone_Handler},
	{_no_Event, 0},
};



//initialise the array of structure of State and state handlers and their
// allowed  events.
// { <state>,<handler>,{<allowed event1>,<allowed event2>,..,<allowed eventn>}}
sStateEventMachine asStateEventMachine_1 [] =
{
	{prog_State, progState_Handler, {_keydown_Event,_keypress_Event, _tone_Event}},
	{idle_State, idleState_Handler, {_operator_Event, _keyup_Event,_tot_error_Event, _keypress_Event, _nozzleup_Event, _auth_command_Event, _nozzledown_Event, _resetcommand_Event, _switchoffcommand_Event, _tone_Event, _filling_resumecommand_Event, _filling_resumed_Event}},
	{inactive_State, inactiveState_Handler,{_error_clear_Event, _keyup_Event, _keypress_Event, _tone_Event}},
	{nozzleup_waitingforauth_State, nozzleup_waitingforauthState_Handler, {_authorise_Event,_timeout_Event,_nozzledown_Event,_keypress_Event, _authorisecommand_Event, _stopcommand_Event, _switchoffcommand_Event, _hardwarereset_Event, _hardwareerror_Event, _tone_Event}},
	{authorised_nozzledown_State, authorised_nozzledown_State_Handler,{_nozzleup_Event,_timeout_Event,_nozzledown_Event,_keypress_Event, _tone_Event}},
	{authorised_nozzleup_State, authorised_nozzleup_State_Handler, {_filling_pulse_Event,_pause_Event,_timeout_Event,_nozzledown_Event,_keypress_Event,_function_key_Event, _stopcommand_Event, _auth_suspendcommand_Event, _hardwarereset_Event, _hardwareerror_Event, _tone_Event}},
	{authorisation_paused_State, authorisation_paused_State_Handler, {_resume_Event,_timeout_Event,_nozzledown_Event,_keypress_Event, _auth_resumecommand_Event,  _stopcommand_Event, _hardwarereset_Event, _hardwareerror_Event, _switchoffcommand_Event, _tone_Event}},
	{authorisation_resumed_State, authorisation_resumed_State_Handler, {}},
	{filling_State, filling_State_Handler, {_filling_paused_Event, _keypress_Event, _timeout_Event,_nozzledown_Event, _stopcommand_Event, _filling_suspendcommand_Event, _hardwarereset_Event, _hardwareerror_Event, _mamo_Event, _switchoffcommand_Event, _tone_Event}},
	{filling_paused_State, filling_paused_State_Handler, {_filling_resumed_Event, _keypress_Event, _timeout_Event,_nozzledown_Event, _filling_resumecommand_Event, _stopcommand_Event, _hardwarereset_Event, _hardwareerror_Event, _switchoffcommand_Event, _tone_Event}},
	{filling_resumed_State, filling_resumed_State_Handler, {}},
	{keypad_entry_State, keypad_entry_State_Handler, {}},
	{operator_State, operator_State_Handler, {_keypress_Event, _tone_Event}},
	{savesettings_State, savesettings_State_Handler, {_keypress_Event, _tone_Event}},
	{read_flash_State, read_flash_State_Handler, {}},
	{write_flash_State, write_flash_State_Handler, {}},
	{switchedoff_State, switchedoffState_Handler, {_stopcommand_Event, _resetcommand_Event, _tone_Event}},
    {pnp_State, pnpState_Handler, {_keyup_Event, _fillingcomplete_Event, _tone_Event}},
	{filledmamo_State, filledmamo_State_Handler, {_keypress_Event, _nozzledown_Event, _resetcommand_Event, _stopcommand_Event, _switchoffcommand_Event, _tone_Event}},
	{tone_entry_State, tone_entry_State_Handler, {}},
	{last_State, 0, {}}
};


/*
//initialise the array of structure of State and state handlers
sStateMachine asStateMachine [] =
{
	{prog_State, progstate_Handler},
    {idle_State,idlestate_Handler},
    {inactive_State,inactivestate_Handler},
    {nozzleup_waitingforauth_State,nozzleup_waitingforauthState_Handler},
	{authorised_nozzledown_State,authorised_nozzledown_State_Handler},
    {authorised_nozzleup_State,authorised_nozzleup_State_Handler},
	{authorisation_paused_State,authorisation_paused_State_Handler},
	{filling_State,filling_state_Handler},
	{filling_paused_State,filling_paused_state_Handler},
	{keypad_entry_State,keypad_entry_State_Handler},
    {last_State,0}
};*/

//initialise the array of structure of State and state handlers and their
// allowed  events.
// { <state>,<handler>,{<allowed event1>,<allowed event2>,..,<allowed eventn>}}
sStateEventMachine asStateEventMachine [] =
{
	{prog_State, progState_Handler,{_keydown_Event,_keypress_Event}},
    {idle_State,idleState_Handler,{_operator_Event,_keyup_Event,_tot_error_Event,_keypress_Event,_nozzleup_Event,_auth_command_Event, _nozzledown_Event}},
    {inactive_State,inactiveState_Handler,{_error_clear_Event, _keyup_Event, _keypress_Event}},
    {nozzleup_waitingforauth_State,nozzleup_waitingforauthState_Handler,{_authorise_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
	{authorised_nozzledown_State,authorised_nozzledown_State_Handler,{_nozzleup_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
    {authorised_nozzleup_State,authorised_nozzleup_State_Handler,{_filling_pulse_Event,_pause_Event,_timeout_Event,_nozzledown_Event,_keypress_Event,_function_key_Event}},
	{authorisation_paused_State,authorisation_paused_State_Handler,{_resume_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
	{filling_State,filling_State_Handler,{_filling_paused_Event,_keypress_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
	{filling_paused_State,filling_paused_State_Handler,{_filling_resumed_Event,_keypress_Event,_timeout_Event,_nozzledown_Event}},
	{keypad_entry_State,keypad_entry_State_Handler,{}},
	{operator_State,operator_State_Handler,{_keypress_Event}},
	{savesettings_State,savesettings_State_Handler,{_keypress_Event}},
	{read_flash_State,read_flash_State_Handler,{}},
	{write_flash_State,write_flash_State_Handler,{}},
    {last_State, 0, {}}
};


//sStateEventMachine asStateEventMachine [] =
//{
//	{prog_State, progstate_Handler,{_keydown_Event, _keypress_Event}},
//    {idle_State,idlestate_Handler,{_nozzleup_Event, _auth_command_Event}},
//    {***inactive_State,inactivestate_Handler,{_error_clear_Event,_keypress_Event}},
//    {nozzleup_waitingforauth_State,nozzleup_waitingforauthState_Handler,{_authorise_Event,_timeout_Event}},
//	{pump_auth_State,{_filling_pulse_Event}},
//	{authorised_nozzledown_State,authorised_nozzledown_State_Handler,{_nozzleup_Event,_timeout_Event}},
//    {authorised_nozzleup_State,authorised_nozzleup_State_Handler,{_filling_pulse_Event,_pause_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
//	{authorisation_paused_State,authorisation_paused_State_Handler,{_timeout_Event}},
//	{filling_State,filling_state_Handler,{requested_amount_sold, stop_comd, power_down, error, no_filling, cmd,_nozzledown_Event}},
//	{filling_paused_State,filling_paused_state_Handler,{_timeout_Event}},
//
//	{keypad_entry_State,keypad_entry_State_Handler,{}},
//	{operator_State,operator_State_Handler,{_keypress_Event}},
//	{savesettings_State,savesettings_State_Handler,{_keypress_Event}},
//	{read_flash_state,read_flash_state_Handler,{}},
//	{write_flash_state,write_flash_state_Handler,{}},
//    {last_State,0,{}}
//};

 //  {idle_State,keyup_Event,inactivestate_Handler},
 //  {inactive_State,keydown_Event,idlestate_Handler},
 //  {nozzleup_waitingforauth_State,Option_Selection_Event,OptionSelectionHandler},
 //  {Option_Selected_State,Amount_Enter_Event,EnterAmountHandler},
 //  {Amount_Entered_State,Amount_Dispatch_Event,AmountDispatchHandler}

//----------------------------------------
void clear_buffer(Nozzle* self)
{
	uint8_t i;
	for(i = 0; i < sizeof(self->keyboard_entry); i++)
	 {
		self->keyboard_entry[i] = 0;
		self->keypad_pw_xter[i] = 0;
	 }
	self->keypad_pw_xter[i] = 0;
	self->keypad_pw_xter[i++] = 0;
	self->keypad_pw_xter[i++] = 0;
	self->keypad_pw_xter[i++] = 0;
	self->index_ = 0;
	self->_index = 0;
}


//-------------------------------------
eSystemState keyup_Handler(void)
{
	self->progg = 1;
	copy_settings(move_to_copy);	//make a copy of the settings								//to be used in the prog.
	self->clear_buffer();
	return prog_State;
}
//-------------------------------------
//--------------------------------------
eSystemState savesettings_State_Handler(void)
{
   static int self->sav = 0;
   uint8_t respons = 0;
   //====================================================
	if (self->sav == 0)
		{
		  if(self->t>500)
		   {
			  lcd_print_line1("1-  Saue");
			  lcd_print_line2("2-Cancel");
			  self->t = 0;
		   }
		  respons = atoi(self->keyboard_entry);
        //------------------------------------------------
		  if(respons == 1)   //save settings
		  {
			 self->sav = 1;
			 return savesettings_State;
		  }
		  if(respons == 2)  //delete settings don'self->t save.
		  {
		   self->sav = 2;
		   return savesettings_State;
		  }
		 //------------------------------------------------
		   return savesettings_State;
		}
   //=======================================
	if (self->sav == 1)
	{
		lcd_print_line1(" sauing ");
		lcd_print_line2("settings");

		copy_settings(move_to_settings0);
		save_settings_original_fram(side_a);

		copy_settings(move_to_settings); // copy the structure.
		save_settings_fram();   //save to eeprom

		self->configMode = CONFIGMODIFIED;
//		save_configFlag(side_a);
		save_configFlag_fram(side_a);
		save_config_otpSeed_time_fram(side_a);

		clear_configChange_trackNum_fram(side_a);

		load_settings(side_a); //load the settings into the
		load_settings(side_b); // internal variables
		HAL_Delay(1500);

		lcd_print_line1("  done  ");
		lcd_print_line1("        ");
		self->sav = 0;
		self->progg = 0;
		self->self->auth = not_auth;
		clear_buffer();
//	        return savesettings_State;

		if(self->eNextState == pnp_State)
			return pnp_State;
		else
			return idle_State;
	}
	//=======================================
	if (self->sav == 2)
	{
		 self->sav = 0;
		 self->progg = 0;
		 self->self->auth = not_auth;
		 //clear self->keyboard buffers.
		 clear_buffer();

//		 lcd_print_line1(self->upper);
//		 lcd_print_line2(self->middle);

		 if(self->settings_stream1[0].display_format == PL)
		 {
			 lcd_print_line1(self->upper);
			 lcd_print_line2(self->middle);
		 }
		 else if(self->settings_stream1[0].display_format == LP)
		 {
			 lcd_print_line1(self->middle);
			  lcd_print_line2(self->upper);
		 }
		 char str__[8]= {0};
		 snprintf(str__, sizeof(str__), "%.2f", self->litre_price);
		 lcd_print_line3(str__);

		 if(self->eNextState == pnp_State)
			return pnp_State;
		 else
			return idle_State;
	}
	//========================================
	return savesettings_State;    //idle_State;  //return to idle mode
}
//--------------------------------------
eSystemState keydown_Handler(void)
{
	clear_buffer();
	self->prog_entry = 0;
	self->prog_revisit = 1;

	lcd_print_line3("        ");

#ifndef DEV_MODE
	self->prog_revisitt = 1;
#endif

	return savesettings_State;   //
}

//-----------------------------------
//-------------
eSystemState auth_command_Handler(void)
{
	char *endPtr;

	reset_timer( timeout_picknozzle);
	  start_timer( timeout_picknozzle);
    //-----------------------------------
	  if(self->self->opmode == MANUAL_MODE)
	  {
		  //check if any keypad entry
		 if(self->index_ >= 1)
		  {
			 self->self->key_value  = strtof(self->keyboard_entry, &endPtr);
		  }

		  // auth_cmd_flag = 1;
	  }
	  else
	  {
//		  change_p = 1;
//		  auth_p = 500.8;

//		  change_v = 1;
//		  auth_v = 2.3;

//		  set_p = 167.00;
//		  change_price_flag = 1;

		  if(self->change_price_flag == 1)
		  {
				setUnitPrice();
				return idle_State;
		  }

		  // AUTO Mode
		  //authorise self->price...
		  if (self->self->change_p == 1)
			  self->self->sellmode = P;      //set sell type to self->price
		 //-----------------------------------------
		  //authorise volume...
		  else if (self->self->change_v == 1)
			  self->self->sellmode = L;      //set sell type to litre
		 //-----------------------------------------
	  }
    // GO can also send this command.
  return authorised_nozzledown_State;
}

//----------------
eSystemState authorise_Handler(void)
{
	reset_timer(timeout_picknozzle);
	start_timer(timeout_picknozzle);

//initialise the fuel and self->price variables
	self->self->price  = 0.0;
	self->self->amt = 0.0;

	self->amt_middle_tmin3 = 0.0;
	self->amt_middle_tmin2 = 0.0;
	self->amt_middle_tmin1 = 0.0;
	self->amt_middle = 0.0;

	self->amt_real_tmin3 = 0.0;
	self->amt_real_tmin2 = 0.0;
	self->amt_real_tmin1 = 0.0;
	self->amt_real = 0.0;




	self->target_pulser = 0;  //state is coming from nozzleup, no self->price/self->amt set

	self->pump_status = STATUS_AUTH;
	self->status_change_pump = 1;

	 clr_pulser();    //clear hardware pulser
	 self->current_pulser = 0;
	 self->overall_currentPulser = 0;

// int cnv = 0;
// snprintf(str_, sizeof(str_), "%.2f", self->price); lcd_print_line1(str_);
// snprintf(str_, sizeof(str_), "%.2f", self->amt);   lcd_print_line2(str_);

 lcd_print_line1("--self->auth--");
 lcd_print_line2("        ");
 char str__[8]= {0};
 snprintf(str__, sizeof(str__), "%.2f", self->litre_price); lcd_print_line3(str__);

 return authorised_nozzleup_State;
}

//----------------------------------------
//----------------------------------------
//-----------------------------------
eSystemState tone_Handler(void)
{
//	resetTimer_tone1(TONE_DURATION);
	startTimer_tone(tone_duration1);

	HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);

	self->self->ePrevState = self->eNextState;

	return tone_entry_State;
}


//----------------------------------------
//----------------------------------------
eSystemState key19_Handler(void)
{
	return idle_State;
}

//----------------------------------------
//---------------------------------------------------------------------

/*
 *
 */
eSystemState operator_Handler(void)
{

	return operator_State;
}


eSystemState function_key_Handler(void)
{
	;
}


eSystemState stopcommand_Handler(void)
{
	self->pump_status = STATUS_FILLING_COMP;
	self->status_change_pump = 1;

	return idle_State;
}

eSystemState resetcommand_Handler(void)
{
	self->pump_status = STATUS_RESET;
	self->status_change_pump = 1;

	return nozzleup_waitingforauth_State;
}

//eSystemState priceupdate_Handler(void)
eSystemState _fillingcomplete_Handler(void)
{
	self->pump_status = STATUS_FILLING_COMP;
	self->status_change_pump = 1;
	clr_screen();

	return idle_State;
}

eSystemState switchoffcommand_Handler(void)
{
	self->pump_status = STATUS_SWITCHED_OFF;
	self->status_change_pump = 1;

	return switchedoff_State;
}

eSystemState authorisecommand_Handler(void)
{
	self->pump_status = STATUS_AUTH;
	self->status_change_pump = 1;

	return authorised_nozzleup_State;
}

eSystemState hardwarereset_Handler(void)
{
	self->pump_status = STATUS_FILLING_COMP;
	self->status_change_pump = 1;

	return idle_State;
}

eSystemState hardwareerror_Handler(void)
{
	stop_flow1();

	self->pump_status = STATUS_FILLING_COMP;
	self->status_change_pump = 1;

	return idle_State;
}

eSystemState auth_suspendcommand_Handler(void)
{
	return authorisation_paused_State;
}

eSystemState filling_suspendcommand_Handler(void)
{
	return filling_paused_State;
}

eSystemState auth_resumecommand_Handler(void)
{
	return authorised_nozzleup_State;
}

eSystemState filling_resumecommand_Handler(void)
{
	//==========================================================================

	#if (_USE_SOFT_PULSER == 1)

		self->current_pulser = ( self->currentPulser_recovered + self->self->current_pulser);

	#else

		self->current_pulser = ( self->self->currentPulser_recovered + (__HAL_TIM_GET_COUNTER(self->flow_counter) ) );

	#endif
	//---------------------------------------------------------------------------

	self->pulser_rem = self->target_pulser - self->current_pulser;

	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//

	self->self->amt = pulser2amt(self->current_pulser);

	self->self->price = amt2price(self->self->amt);

	self->amt_middle_tmin3 = self->self->amt;
	self->amt_middle_tmin2 = self->self->amt;
	self->amt_middle_tmin1 = self->self->amt;
	self->amt_middle = self->self->amt;

	self->amt_real_tmin3 = self->self->amt;
	self->amt_real_tmin2 = self->self->amt;
	self->amt_real_tmin1 = self->self->amt;
	self->amt_real = self->self->amt;

	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//



	//////////////////////////////////////////////////////////////////////////
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
	/*             CATERS TO RESUMED SALES @ PUMP RESTART                   */

	self->self->nozzle_bit = 1;
	self->self->stop_fueling_bit = 0;

	drive_motor(ACTIVATE);

	if(self->settings_stream1[0].pump_type_ != LAFENG)
	{
		 drive_slow_sole(ACTIVATE);
		 drive_fast_sole(DEACTIVATE);
	}
	else
	{
		drive_slow_sole(DEACTIVATE);     // DEACTIVATE here actually means ACTIVATE
		drive_fast_sole(ACTIVATE);		  // ACTIVATE here actually means DEACTIVATE
	}

	//////////////////////////////////////////////////////////////////////////


	return filling_State;
}

eSystemState mamo_Handler(void)
{
	self->pump_status = STATUS_MAMO_REACHED;
	self->status_change_pump = 1;

	return filledmamo_State;
}

//pump_status_ = STATUS_MAMO_REACHED;
//pump_status_ = STATUS_RESET;
//pump_status_ = STATUS_AUTH;
//pump_status_ = STATUS_FILLING;
//pump_status_ = STATUS_FILLING_COMP;
//pump_status_ = STATUS_SWITCHED_OFF;
//pump_status_ = STATUS_PNP;


/////////////////////////////////////////////////////////////////
uint8_t long_press_key()
{
	static int8_t self->pressed_ = 0;
		//static int pressed_old = 0;
		int8_t ky;

//		if(pump_type == bluesky)
//		if( (pump_type == DN_BLSKY18K) || (pump_type == DN_BLSKY22) ||
//			(pump_type == DIN_BLSKY18K) || (pump_type == DIN_BLSKY22) ||
//			(pump_type == DN_LAFNG17K) )

		if( (self->settings_stream1[0].keypad__ == BLSKY18_K) || (self->settings_stream1[0].keypad__ == BLSKY22) ||
			(self->settings_stream1[0].keypad__ == LAFNG17_K) || (self->settings_stream1[0].keypad__ == LAFNG18_K) ||
			(self->settings_stream1[0].keypad__ == LAFNG18_K_V2) )
		{
//		   ky = 19;  //F4 key
		   ky = 15;  //clear key
		}
		else
			return 0;      // only for bluesky boards .


		if(self->keypress_ != ky)
		{
	        self->pressed_ = 0;
	        self->key_buttonpress_tmr = 0;  //clr timer.
		}
		 if( ( (self->key_buttonpress_tmr >= 3) && (self->pressed_ == 0)) && ( (self->eLastState != filling_State) && (self->eNextState != filling_State)) )
		 {
			 self->key_buttonpress_tmr = 3;
			self->pressed_ = 1;
			return 1;
		 }
		 return 0;
}

/////////////////////////////////////////////////////////////////

uint8_t long_press_progExit()
{
	static uint8_t self->pressed_ = 0;
		//static int pressed_old = 0;
	uint8_t ky;

//		if(pump_type == bluesky)
//		if( (pump_type == DN_BLSKY18K) || (pump_type == DN_BLSKY22) ||
//			(pump_type == DIN_BLSKY18K) || (pump_type == DIN_BLSKY22) ||
//			(pump_type == DN_LAFNG17K) )
		if( (self->settings_stream1[0].keypad__ == BLSKY18_K) || (self->settings_stream1[0].keypad__ == BLSKY22) ||
			(self->settings_stream1[0].keypad__ == LAFNG17_K) || (self->settings_stream1[0].keypad__ == LAFNG18_K) ||
			(self->settings_stream1[0].keypad__ == LAFNG18_K_V2) )
		{
		   ky = 14;  //'.' key
		}
		else
			return 0;      // only for bluesky boards .


		if(self->keypress_ != ky)
		{
	        self->pressed_ = 0;
	        self->progExit_buttonpress_tmr1 = 0;  //clr timer.
		}
		 if( (self->progExit_buttonpress_tmr1 >= 3) && (self->pressed_ == 0) && (self->eNextState != filling_State) )  // && (eNextState2 != filling_State)) )
		 {
			 self->progExit_buttonpress_tmr1 = 3;
			self->pressed_ = 1;
			return 1;
		 }
		 return 0;
}

/////////////////////////////////////////////////////////////////

uint8_t long_press_log()
{
	static uint8_t self->pressed_ = 0;
	//static int pressed_old = 0;

	uint8_t ky = 0;
	ky = readkey19_state();


	/***************************************************************************
	static uint32_t lastKeyPressTime = 0;
	static uint8_t lastKeyState = 0;

	uint8_t currentKeyState = readKeypad();  // Read the keypad state
	uint32_t currentTime = HAL_GetTick(); // Get current system tick

	// If the key state changed
	if (currentKeyState != lastKeyState)
	{
		lastKeyPressTime = currentTime;  // Reset debounce timer
		lastKeyState = currentKeyState;
	}

	// Check if the debounce delay has passed
	if ((currentTime - lastKeyPressTime) > DEBOUNCE_DELAY) {
		// Debounced key press - process only if key is actually pressed
//		if (currentKeyState != 0) {
//			handleKeyPress(currentKeyState);  // Process the key press
//		}
		if (currentKeyState != 1)
		{
	        self->pressed_ = 0;
	        log_buttonpress_tmr = 0;   //clr timer.
	        HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
		}
		else if (currentKeyState == 1)
		{
			if(self->settings_stream2[0].keypress_tone == Yes)
			{
				HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
				HAL_Delay(10);
			}

		}
	}
	**********************************************************************************/


	//	if (readkey19_state() != 1)
	if (ky != 1)
	{
        self->pressed_ = 0;
        self->log_buttonpress_tmr = 0;   //clr timer.
//        HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
	}
	else if (ky == 1)
	{
		if(self->self->settings_stream2[0].keypress_tone == Yes)
		{
//			HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
//			HAL_Delay(10);
			self->timerFlag_tone = 1;
			self->tone_duration = 10;
		}

	}

	 if ( (self->log_buttonpress_tmr >= 3) && (self->pressed_ == 0 ) )
	 {
		 self->log_buttonpress_tmr = 3;
		self->pressed_ = 1;

//		HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
		return 1;
	 }

//	 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
	 return 0;
}
/////////////////////////////////////////////////////////////////
uint8_t long_press_tot()
{
	static uint8_t self->pressed_ = 0;
	uint8_t ky;

	static uint8_t	self->key19StateCount = 0;

   	static bool self->doublePressDetected = false,
				self->click_in_progress = false;

	int doublePressThreshold = 2000;

	 dummyValue = 0;

	if( (self->settings_stream1[0].keypad__ == LAFNG17_K) || (self->settings_stream1[0].keypad__ == LAFNG18_K) || (self->settings_stream1[0].keypad__ == LAFNG18_K_V2))
	{
//		ky = 11;  //keypad type mapping...
		self->key19State = readkey19_state();

		if( (self->firstTime_key19 == 1) && (self->key19State == 1) )
		{
			self->key19Timer = 0;
			self->firstTime_key19 = 2;

			if(self->settings_stream2[0].keypress_tone == Yes)
			{
//				  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
//				  HAL_Delay(10);
				self->timerFlag_tone = 1;
				self->tone_duration = 10;
			}
		}

		 else if (self->key19State == 0)   // Key19 is released
		 {
//			   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
		 }


	  if (self->key19State != self->lastKey19State)
	  {
		  //we keep checking for clicks, we don'self->t care about the non-click detection (it will be most of the time)
		  if (self->key19State == 1)   // Key19 is pressed
		  {
				 if(self->key19StateCount == 0)
				 {
					 self->key19Timer = 0; //reset the timer only on the first click
					 self->self->click_in_progress = true; //we are in the clicking phase;
				 }
				 self->key19StateCount++ ; // it will start as 1 and will keep incrementing

				 if(self->self->settings_stream2[0].keypress_tone == Yes)
				 {
//					 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
//					 HAL_Delay(10);
					 self->timerFlag_tone = 1;
					 self->tone_duration = 10;
				 }
		  }

			 if(self->self->click_in_progress == true)
			 {
				 if(self->key19Timer >= 500)   //now we have passed the 500ms let's check how many clicks happened
				 {
					 if(self->key19StateCount > 1)
					 {
						//it's a double or multiple click
						 self->self->doublePressDetected = true;
					 }
					 else
					 {
						 //has to be at least 1 so it's a single click, it can never be zero
					 }
					 self->key19StateCount = 0;
					 self->self->click_in_progress = false;
				 }
			 }
	  }

	  self->lastKey19State = self->key19State;

	  if (self->self->doublePressDetected)
	  {
		  self->self->doublePressDetected = false;
		  self->tot_buttonpress_tmr = 3;
		  self->pressed_ = 1;
		  self->firstTime_key19 = 1;
		  return 1;
	  }
	}
	else
	{
		ky = 21;                      //mapped to print key...

		if(self->keypress_ != ky)
		{
			self->pressed_ = 0;
			self->tot_buttonpress_tmr = 0;  //clr timer.
		}
		 if((self->tot_buttonpress_tmr >= 3) && (self->pressed_ == 0) )
		 {
			 self->tot_buttonpress_tmr = 3;
			self->pressed_ = 1;
			return 1;
		 }
	}

	 return 0;
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

eSystemState keypress_Handler(void)
{

	static allowed_xters = 6;

	extern uint8_t error_clr_flag;

	extern uint8_t keyEntry_len;

//	 if(pump_type == lafeng)
//	 if(pump_type == DN_LAFNG17K)
//	 if( (self->settings_stream1[0].keypad__ == LAFNG17_K) || (self->settings_stream1[0].keypad__ == LAFNG18_K) )
//	 {
//	   kkey =  lafeng_keypad[self->keypress_];
//	   allowed_xters = 6;
//	 }
	 if(self->settings_stream1[0].keypad__ == LAFNG17_K)
	 {
		 self->kkey =  lafeng_keypad[self->keypress_];
	     allowed_xters = 6;
	 }
	 else if (self->settings_stream1[0].keypad__ == LAFNG18_K)
	 {
		 self->kkey =  lafeng_keypad_18K[self->keypress_];
		 allowed_xters = 7;
	 }
	 else if (self->settings_stream1[0].keypad__ == LAFNG18_K_V2)
	 {
		 self->kkey =  lafeng_keypad_18K_V2[self->keypress_];
		 allowed_xters = 7;
	 }
//	 else if(pump_type == bluesky)
//	 else if( (pump_type == DN_BLSKY18K) || (pump_type == DN_BLSKY22) ||
//			  (pump_type == DIN_BLSKY18K) || (pump_type == DIN_BLSKY22)  )
	 else if( (self->settings_stream1[0].keypad__ == BLSKY18_K) || (self->settings_stream1[0].keypad__ == BLSKY22) )
	 {
		 self->kkey =  bluesky_keypad[self->keypress_];
		 allowed_xters = 7;
	 }

	 self->self->ePrevState = self->eLastState;

 /*  if (self->progg == 1)
	 {
	    return keypad_entry_State;
	 }  */

	//if (self->progg == 1) //if in programming mode return.

	//char prc[7] = {'p'};
	//char self->amt[7] = {'l'};
	//char self->keyboard[6] = {0};
	//char keyboard_screen[6] = {0};

   int8_t space = allowed_xters - index_; //int space = 6 - index_;
   uint8_t ind = 1;

	//----------------------------------------------------------
	//If only 0 - 9 is pressed.....
	if(
		(self->kkey != 'A') && (self->kkey != 'B') &&
		(self->kkey != 'C') && (self->kkey != 'D') &&
		(self->kkey != 'F') && (self->kkey != '-') &&
		(self->kkey != 'G') && (self->kkey != 'J')  //&&(index_ < 6)
	  )
	{
	//	 keyEntry_len = strlen(self->keyboard_entry);

		 if(index_ <= allowed_xters)    //only allow 6 xters...
		 {
			 self->keyboard_entry[self->index_] = self->kkey;
			 self->keyboard_entry[self->index_ + 1] = 0;         //NULL;
	//		if(kkey == '.')
	//		{
	//			self->keyboard_entry[index_+1] = '0';         //NULL;
	//			self->keyboard_entry[index_+2] = 0;         //NULL;
	//		}
			index_++;
		  }
	}
	//--------------------------------------------------------
	else if (self->kkey == 'F')      //'clear' key.
	{
	  //self->ePrevState = self->eLastState;
	  //set the error clear flag..

		dpFlag = 0;
		self->error_clr_flag = 1;
		 index_ = 0;
		 _index = 0;
		 for(uint8_t i = 0; i < 9; i++)
		 {
			 self->keypad_pw_xter[i] = 0;
			 self->upper[i] = 0;
			 self->middle[i] = 0;
		 }

		 for(uint8_t i = 0; i <= 6; i++)
		 {
			 self->keyboard_entry[i] = 0;   //clear the buffer
			 self->keyboard[i] = 0;
		 }

		 if(self->settings_stream1[0].display_format == PL)
		 {
			 self->upper[0]  = 'P';
			 self->middle[0] = 'L';
		 }
		 else if(self->settings_stream1[0].display_format == LP)
		 {
			 self->upper[0]  = 'L';
			 self->middle[0] = 'P';
		 }
	}
//--------------------------------------------------------------------
//		  self->progg = 1;
//		  self->auth = authed;
  if (self->progg == 1)
  {
		keypad_print("       ");
		if (self->self->auth == not_auth)
			keypad_print(self->keypad_pw_xter);
		else
		{
			 int8_t keyBoard_len = strlen(self->keyboard_entry);
			 strncpy(self->keyboard, self->keyboard_entry, sizeof(self->keyboard));
			 if(strchr(self->keyboard, '.') )
			 {
				 if(self->keyboard[keyBoard_len - 1] == '.')
				 {
					 self->keyboard[keyBoard_len] = '0';
				 }
			 }
			 keypad_print(self->keyboard);
		}
		return keypad_entry_State;
	 }


      if ( ((self->kkey == 'D') && (self->progg == 0)) && (self->eNextState != filling_State) ) //fueling key.
	 	{
       	   //self->ePrevState = self->eLastState;

    	  if(self->settings_stream1[0].mode == MANUAL_MODE)
    	  {
    		  self->auth_cmd_flag = 1;  //activate self->auth cmd.

			   return keypad_entry_State;
    	  }
    	  else if(self->settings_stream1[0].mode == AUTO_MODE)
    	  {

    		  /* ---------------------------------------------------------------------------------------------------------- */
			  /*                                                  NOZZLE UP 			   							        */
			  /* 			   																						        */
			  /*      [eNextState2 == prog_State] ==> Ensures Sales doesn'self->t run in a config-mode of either/both side(s)     */
    		  /*   			   																						        */
			  /* ---------------------------------------------------------------------------------------------------------- */
    		  if ( ((self->eNextState == idle_State) || (self->eNextState == authorised_nozzledown_State)) && (self->eNextState2*** != prog_State) )
    		  {
    			  self->nozzle_flag_key = 1;
    		  }

    		  return keypad_entry_State;
    	  }
	 	}

//      if ( (kkey == 'A') && (self->progg == 0) && ( (self->eNextState == filling_State) || (self->eNextState == authorised_nozzleup_State)) ) //stop sales.
      if ( (self->kkey == 'A') && (self->progg == 0) )
	  {
    	  self->self->stop_flag = 1;  //deactivate self->auth cmd.


		   /* ----------------------------------------------------------------------------------------------------- */
		   /*                                            Nozzle Down                                                */
		   /* ----------------------------------------------------------------------------------------------------- */
		   if (
				   (self->eNextState == idle_State) || (self->eNextState == nozzleup_waitingforauth_State) ||
				   (self->eNextState == authorised_nozzledown_State) || (self->eNextState == authorised_nozzleup_State)  ||
				   (self->eNextState == authorisation_paused_State) || (self->eNextState == filling_State) ||
				   (self->eNextState == filling_paused_State) || (self->eNextState == filledmamo_State)
			  )
		   {
			   self->nozzle_flag_key = 0;
			   self->nozzle_flag_key_old = 1;
		   }

//		   keypad_zerorise1 = true;
	  }

//====================================================
	if ( (self->kkey == 'C') && (self->progg == 0) )  //if change sales mode
	{
	   if(self->self->sellmode == L)
	   {
		   self->self->sellmode = P;
//	        if(settings[0].display_mode == PL)
//	       	{
//	    	   self->sellmode = P;
//	       	}
//	       	else if(settings[0].display_mode == LP)
//	       	{
//	       		self->sellmode = L;
//	       	}
	   }
	   else if(self->self->sellmode == P)
	   {
		   self->self->sellmode = L;
	   }

//	   else if(self->sellmode == P)
//	   {
//		   self->sellmode = V;
////		    if(settings[0].display_mode == PL)
////			{
////			   self->sellmode = L;
////			}
////			else if(settings[0].display_mode == LP)
////			{
////				self->sellmode = P;
////			}
//	   }
//	   else if (self->sellmode == V)
//	   {
//		   self->sellmode = L;
//	   }
	}
//--------------------------------------------------------------------
 int8_t space2, c, b;
 keyEntry_len = strlen(self->keyboard_entry);
 if( (strchr(self->keyboard_entry, '.')) && (self->keyboard_entry[keyEntry_len - 1] != '.') )
 {
	 space2 = 7 + ( -index_ + 1);
	 dpFlag = 1;

	 b = 0;
	 dpCount = 0;
	 while( *(self->keyboard_entry + b) != '\0')
	 {
		 if( (self->keyboard_entry[b] == '.') || (dpCount != 0) )
		 {
			dpCount++;
		 }
		 b++;
	 }
	 dpCount--;

//	 space2 = 7 + ( -index_ + dpCount);
//	 dpFlag = 1;
 }
 else
 {
//	 if(strchr(self->keyboard_entry, '.') )
//	 {
//		 if(self->keyboard_entry[keyEntry_len - 1] == '.')
//			 self->keyboard_entry[keyEntry_len] = '0';
//	 }

	 space2 = 7 - index_;     //6 - index_;

//	 if( (space2 == 7) && (dpFlag != 1) )
//		 plZero_flag = 1;
 }
 if ( (self->self->sellmode == L) && (self->progg == 0) )
 {
	memset(self->middle, '\0', sizeof(self->middle));
	self->middle[0] = 'L';     //Append self->price to display.
	// if((index_-1) >= 6) space = 2;
	while (space2 > 0) //write spaces first..
	{
		self->middle[ind++] = ' ';

//	   if( (plZero_flag == 1) && (space2 == 1) )
//		   self->middle[ind - 1] = '0';

	   space2--;
	}
	space2 = index_;     //reload with len of actual number
	while(space2 > 0)
	 {
		self->middle[ind++] = self->keyboard_entry[index_ - space2];
		 space2--;
	 }
  //  lcd_print_line1("p       ");

//	if(settings[0].display_mode == PL)
//	{
//		write_v(1,"p       ");
//	}
//	else if(settings[0].display_mode == LP)
//	{
//		write_v(1,"l       ");
//	}

//	write_v(1,"p       ");
	write_v(1,"        ");
	if(index_ == 0)
		self->middle[7] = '0';
	  //  lcd_print_line2(self->middle);
   }
//--------------------------------------------------------------------
// if(settings[0].display_mode == PL)
//  {
//	 self->upper[0]  = 'P';
//	 self->middle[0] = 'L';
//  }
//  else if(settings[0].display_mode == LP)
//  {
//	 self->upper[0]  = 'L';
//	 self->middle[0] = 'P';
//  }
   else if ( (self->self->sellmode == P) && (self->progg == 0) )
   {
//	   if(settings[0].display_mode == PL)
//		{
//		   self->upper[0]  = 'P';    // append self->price to display
//		}
//		else if(settings[0].display_mode == LP)
//		{
//			self->upper[0]  = 'L';   // append self->price to display
//		}
	   memset(self->upper, '\0', sizeof(self->upper));
	   self->upper[0] = 'P';
	// if((index_-1) >= 6) space = 2;
	   while (space2 > 0)     //write spaces first..
	   {
		   self->upper[ind++] = ' ';
		   space2--;
	   }
	 space2 = index_ ;          //reload with len of actual number

// 		 if(self->keyboard_entry[index_ - 1] == '.')
// 		 {
// 			ind++;
// 		 }

		while(space2 > 0)
		 {
			self->upper[ind++] = self->keyboard_entry[index_ - space2];
			 space2--;
		 }
		//lcd_print_line1(self->upper);  //clear the self->price
		//lcd_print_line2("l       ");
//	   write_v(2,"L       ");
	   write_v(2,"        ");

	   if(index_ == 0)
		   self->upper[7] = '0';
//	   if(settings[0].display_mode == PL)
//	   	{
//	   		write_v(2,"l       ");
//	   	}
//	   	else if(settings[0].display_mode == LP)
//	   	{
//	   		write_v(2,"p       ");
//	   	}
	 }
     else if ( (self->self->sellmode == V) && (self->progg == 0) )
     {
    	 memset(self->upper, '\0', sizeof(self->upper));
    	 self->upper[0] = 'C';
		// if((index_-1) >= 6) space = 2;
		   while (space2 > 0)     //write spaces first..
		   {
			   self->upper[ind++] = ' ';
			   space2--;
		   }
		 space2 = index_ ;          //reload with len of actual number

	 // 		 if(self->keyboard_entry[index_ - 1] == '.')
	 // 		 {
	 // 			ind++;
	 // 		 }

			while(space2 > 0)
			 {
				self->upper[ind++] = self->keyboard_entry[index_ - space2];
				 space2--;
			 }
			//lcd_print_line1(self->upper);  //clear the self->price
			//lcd_print_line2("l       ");
		   write_v(2,"l       ");
     }
//----------------------------------------------------------------
//                            write to the keypad

 	   static uint8_t lcd_size = 5;

 	   if(self->settings_stream1[0].keypad__  == LAFNG17_K)
 	   {
 		    lcd_size = 5; //change this latter to accomodate other lcds.
 	   }
 	   else if( (self->settings_stream1[0].keypad__  == LAFNG18_K) || (self->settings_stream1[0].keypad__ == LAFNG18_K_V2) )
 	   {
 		    lcd_size = 7; //change this latter to accomodate other lcds.
 	   }
 	   else if( (self->settings_stream1[0].keypad__  == BLSKY18_K) || (self->settings_stream1[0].keypad__  == BLSKY22) )
 	   {
 		    lcd_size = 7;   //6;
 	   }

//================================================================
	 int8_t size = lcd_size - 1;   // 1 xter to display 'P/L'
//	 if(strchr(self->keyboard_entry, '.')) size = lcd_size;

	 int8_t k_index = 0;
	 ind = 1;

	// int8_t space2, c;
	//  c = strlen(self->keyboard_entry);
	//  if( (strchr(self->keyboard_entry, '.')) && (self->keyboard_entry[c-1] != '.') )
	//  {
	// 	 space2 = 7 + ( -index_ + 1);
	//  }
	//  else
	//  {
	// 	 space2 = 7 - index_;     //6 - index_;
	//  }
	  if (self->self->sellmode == P)
	  {
		  self->keyboard[0] = 'P';

//		  if(settings[0].display_mode == PL)
//		  {
//			  self->keyboard[0] = 'p';
//		  }
//		  else if(settings[0].display_mode == LP)
//		  {
//			  self->keyboard[0] = 'l';
//		  }
		     if( (index_ <= size) || ((index_ <= 6) && (dpFlag == 1)) )
		     {
		    	if(dpFlag == 1)
		    	{
		    		space = size + (-index_ + 1);
		    	}
		    	else
		    	{
					space = size - index_;
		    	}
		     }
		     else
		    	   space = 0;  //full size of xters.

		      while (space > 0) //write spaces first..
			  {
		    	  self->keyboard[ind++] = ' ';

		    	 if( (index_ == 0) && (space == 1) )
		    		 self->keyboard[ind - 1] = '0';

				   space--;
			  }
            //---------------------------------
				if( (index_ <= size) || ((index_ <= 6) && (dpFlag == 1)) )
					  space = index_;
				else
				{
					if(dpFlag == 1)
					{
						space = 6;
					}
					else
					{
						space = size;
					}
				}

				if( (strchr(self->keyboard_entry, '.')) && (index_ > 6) ) space = 7;
			    while(space > 0)
				 {
			    	self->keyboard[ind++] = self->keyboard_entry[index_ - space];
				   space--;
				 }

			   if(self->keyboard[ind - 1] == '.') self->keyboard[ind] = '0';

	   }
//----------------------------------------------------------------
	  else if (self->self->sellmode == L)
	  {

		  self->keyboard[0] = 'l';

//		  if(settings[0].display_mode == PL)
//		  {
//			  self->keyboard[0] = 'l';
//		  }
//		  else if(settings[0].display_mode == LP)
//		  {
//			  self->keyboard[0] = 'p';
//		  }
		 if( (index_ <= size) || ((index_ <= 6) && (dpFlag == 1)) )
		 {
			if(dpFlag == 1)
			{
				space = size + (-index_ + 1);
//		    		dpFlag = 0;
			}
			else
			{
				space = size - index_;
			}
		 }

		 else
			   space = 0;  //full size of xters.

		  while (space > 0) //write spaces first..
		   {
			  self->keyboard[ind++] = ' ';

			 if( (index_ == 0) && (space == 1) )
				 self->keyboard[ind - 1] = '0';

			   space--;
		   }
		  //---------------------------------
			if( (index_ <= size) || ((index_ <= 6) && (dpFlag == 1)) )
				  space = index_;
			else
			{
				if(dpFlag == 1)
				{
					space = 6;
				}
				else
					space = size;
			}

			if( (strchr(self->keyboard_entry, '.')) && (index_ > 6) ) space = 7;
			while(space > 0)
			 {
				self->keyboard[ind++] = self->keyboard_entry[index_ - space];
			   space--;
			 }

			if(self->keyboard[ind - 1] == '.') self->keyboard[ind] = '0';

	   }

	   if (self->progg == 1)
		 {
		    keypad_print("      ");
			if (self->self->auth == not_auth)
				keypad_print(self->keypad_pw_xter);
			else
			{
				 int8_t keyBoard_len = strlen(self->keyboard_entry);
				 strncpy(self->keyboard, self->keyboard_entry, sizeof(self->keyboard));
				 if(strchr(self->keyboard, '.') )
				 {
					 if(self->keyboard[keyBoard_len - 1] == '.')
					 {
						 self->keyboard[keyBoard_len] = '0';
					 }
				 }
				 keypad_print(self->keyboard);
			}
			return keypad_entry_State;
		 }

	   	 keypad_print(self->keyboard);
//----------------------------------------------------------------
      //send_keypad(self->keyboard);

         return keypad_entry_State;
}
eSystemState nozzleup_Handler(void)
{
	self->pump_status_4G = STATUS_NOZZLE_UP;

	reset_timer(timeout_picknozzle);
	start_timer(timeout_picknozzle);


	//set the motor on and solenoid off.
	get_auth();

	if(self->self->opmode == AUTO_MODE)
	{
		self->nozzle_out = true;
		self->status_change_noz = 1;

		 if(self->pump_status == STATUS_FILLING_COMP)
		 {
			 return idle_State;
		 }
	}

	lcd_print_line1("88888888");
	lcd_print_line2("88888888");
	lcd_print_line3("888888");


//	lcd_print_line3("      ");


//	send_solenoid(0);   //11
//	send_pump(1);      //turn on pump.

	if (self->eLastState == authorised_nozzledown_State)
	{
		self->overall_currentPulser = 0;
		clr_pulser();    //clear hardware pulser
		self->current_pulser = 0;
		return authorised_nozzleup_State;
	}

	return nozzleup_waitingforauth_State;
}

eSystemState nozzledown_Handler(void)
{
	self->filling = 0;
	self->self->nozzle_bit = 0;

	self->pump_status_4G = STATUS_NOZZLE_DOWN;

	 if(self->self->nozzleup_awaitingauth_state_not_timedOut == 1)
	 {
		 self->self->nozzleup_awaitingauth_state_not_timedOut = 0;
		 clr_screen();
	 }

	 if(self->nonValid_sale == 1)
	 {
		 return idle_State;
	 }

	 stop_flow();

	//lcd_print_line1("nos dn");
	 reset_timer(30);
	 stop_timer();

//	 if(  ( (self->eLastState == idle_State) && (self->eNextState == idle_State) ) || ( (self->eLastState == filledmamo_State) && (self->eNextState == filledmamo_State) ) )
//	 {
////		  return idle_State;
//		 ;
//	 }
//	 else
//	 if( (self->eLastState == filling_State) && (self->eNextState == filling_State) )
	 if( (self->eLastState == filling_State) && (self->eNextState == filling_State) && (self->filling_mamo_flag == 0) && (self->hardwareError_flag == 0) )
	 {
		  do_calcs();
		  get_time();
		  update_info();
//		  save_volumeTotaliser(operating_side);
//		  save_amountTotaliser(operating_side);

		  while(retrieve_totaliser_fram_check(side_a) != OK)   //If it fails, retry 5X
		  {
			  static uint8_t try = 0;
				if(try++ >= 5)
				{
					while(retrieve_totaliser_eeprom_check(operating_side) != OK)
					{
						if(try++ >= 10)
						{
							clear_totaliser_fram(operating_side);
							clear_totaliser_eeprom(operating_side);
							try = 0;
							break;
						}

					}
					try = 0;
					break;
				}
		  }

		  save_totaliser_fram(side_a);
		  save_totaliser_eeprom(side_a);

		  save_lastSale_fram(side_a);
		  save_lastSale_eeprom(side_a);

		  ////////////////////////////////////////////////
		  //---------------------------------------------
		  self->totalizer_saveStatus = SAVED_TO_MAIN_TOTALIZER;
		  save_totaliserFrequent_fram(side_a);
		  save_totaliserFrequent_eeprom(side_a);
		  ////////////////////////////////////////////////

		  self->self->nozzle_bit = 0;
	 }
/*
		 char str__[8]= {0};
         snprintf(str__, sizeof(str__), " %.2f", self->litre_price);
		 lcd_print_line3(str__);   */

		 char str__[10]= {0};
		snprintf(str__, sizeof(str_), "%.2f", self->litre_price);
		lcd_print_line3(str__);


	 if( ((self->pump_LitreOverflow == 1) && (self->pulser_rem > 0 )) || (self->display_overflow == 1) || ( (self->authorizedSale_overflow == 1) && (self->pulser_rem > 0 ) ) )  //|| ((self->display_overflow == 1) && (self->pulser_rem > 0 )) )
	 {
		  if(self->pump_LitreOverflow == 1)
			  self->pump_LitreOverflow = 0;

		  if(self->display_overflow == 1)
		  {
			  self->display_overflow = 0;
			  self->firstTime_display_overflow = 0;
		  }

		  if(self->authorizedSale_overflow == 1)
		  {
			  self->authorizedSale_overflow = 0;
			  self->firstTime_authorizedSale_overflow = 0;
		  }

		  clr_screen();
	 }

 //--------------------------------------------------------
//	dpFlag = 0;
//	error_clr_flag = 1;
//	 index_ = 0;
//	 _index = 0;
//	 for(int i = 0; i < 9; i++)
//	 {
//	   self->keypad_pw_xter[i] = 0;
//	   self->keyboard_entry[i] = 0;   //clear the buffer
//	 }
//
//	 for(int i = 0; i <= 8; i++)
//	 {
//		 self->keyboard[i] = 0;
//	 }
//	 //--------------------------------------------------------------------
//	 if (self->sellmode == P)
//	 {
//		  write_v(3, "P     0");  //send_keypad("p    ");  //5 xters  lafeng..
//	 }
//	 else if(self->sellmode == L)
//	 {
//		  write_v(3, "L     0");  //send_keypad("l    ");  //5 xters lafeng
//	 }
////	 else if(self->sellmode == V)
////	 {
////		  write_v(3, "v    0");  //send_keypad("p    ");  //5 xters  lafeng..
////	 }
//	 send_keypad(self->keyboard);

  //--------------------------------------------------------------------
//	 if( (pump_status_ == STATUS_FILLING) || (pump_status_ == STATUS_MAMO_REACHED) )
//	 {
//
//	 }
//	 else if(pump_status_ == STATUS_MAMO_REACHED)
//	 {
//
//	 }
//	 else if(pump_status_ == STATUS_FILLING_COMP)
//	 {
//
//	 }
	 //

	 if(self->programmed_sale == 1)
	 {
		 self->programmed_sale = 0;

		 self->self->incidentRecord[0].timestamp_event = RtcToInt(2019);

//		if (self->key_value_sellmodeP == 1)
//		{
//			self->incidentRecord[0].programmed_sale = self->key_value_original;
//		}
//		else
		 self->self->incidentRecord[0].programmed_sale = self->key_value;

		 self->self->incidentRecord[0].nozzleState_change_ = NOZZLE_HANGUP;
	     save_programmedSaleEvent_fram(side_a);

	 }

//	 if(keypad_zerorise1 == true)
		 keypad_zerorize();
  //---------------------------------------------------------------------
  //             saving to the log
	 if(self->eNextState == filling_State)
	 {
		 self->self->stop_fueling_bit = 0;

	  //===========================================================
	  //         for totaliser toggle.
	  //===========================================================

//		self->running_volTotaliser = self->working_volTotaliser + self->amt_real;
		//----------------------------------------------------------------------------//
		 self->running_volTotaliser_tmin3 = self->running_volTotaliser_tmin2;
		 self->running_volTotaliser_tmin2 = self->running_volTotaliser_tmin1;
		 self->running_volTotaliser_tmin1 = self->running_volTotaliser;
		 self->running_volTotaliser = (self->working_volTotaliser + self->amt_real);


		 self->running_volTotaliser_array[0] = self->running_volTotaliser;
		 self->running_volTotaliser_array[1] = self->running_volTotaliser_tmin1;
		 self->running_volTotaliser_array[2] = self->running_volTotaliser_tmin2;
		 self->running_volTotaliser_array[3] = self->running_volTotaliser_tmin3;

		correctArray1(self->running_volTotaliser_array, tot_a);

		self->running_volTotaliser = self->running_volTotaliser_array[0];
		self->running_volTotaliser_tmin1 = self->running_volTotaliser_array[1];
		self->running_volTotaliser_tmin2 = self->running_volTotaliser_array[2];
		self->running_volTotaliser_tmin3 = self->running_volTotaliser_array[3];

		self->totaliser_vol = self->running_volTotaliser;

		//============================================================================//


//		self->running_volTotaliser_cal = self->working_volTotaliser_cal + self->amt_middle;
		//----------------------------------------------------------------------------//
		self->running_volTotaliserc_tmin3 = self->running_volTotaliserc_tmin2;
		self->running_volTotaliserc_tmin2 = self->running_volTotaliserc_tmin1;
		self->running_volTotaliserc_tmin1 = self->running_volTotaliserc;
		self->running_volTotaliserc = (self->working_volTotaliserc + scale_to_range(self->amt_middle));

		self->running_volTotaliserc_array[0] = self->running_volTotaliserc;
		self->running_volTotaliserc_array[1] = self->running_volTotaliserc_tmin1;
		self->running_volTotaliserc_array[2] = self->running_volTotaliserc_tmin2;
		self->running_volTotaliserc_array[3] = self->running_volTotaliserc_tmin3;

		correctArray(self->running_volTotaliserc_array, tot_a);

		self->running_volTotaliserc = self->running_volTotaliserc_array[0];
		self->running_volTotaliserc_tmin1 = self->running_volTotaliserc_array[1];
		self->running_volTotaliserc_tmin2 = self->running_volTotaliserc_array[2];
		self->running_volTotaliserc_tmin3 = self->running_volTotaliserc_array[3];

		self->totaliser_vol_cal = scale_to_original(self->running_volTotaliserc);

		//============================================================================//


		self->running_amtTotaliser = (self->working_amtTotaliser + self->price_real);

		self->running_amtTotaliserc = (self->working_amtTotaliserc + self->price_upper);

		//============================================================================//


//		float pricecheck = self->running_amtTotaliser_cal - priceOld1;
//
//		if (pricecheck >= 1000.00)
//		{
//		   priceOld1 = self->running_amtTotaliser_cal;
//		   save_amountSend(side_a);
//
//		   char str[65];
//		   sprintf(str, "[Side-A]... #%0.2f intermittent worth of sales made now!", pricecheck);
//		   server_write(str);
//		}

		self->r_volTotaliser = floor( scale_to_original(self->running_volTotaliserc) );


		#ifdef PULSER_BASED_TV
			self->mechTotalizer = (self->mechTotalizer_ + pulser2amt(self->current_pulser));
		#else
			self->mechTotalizer = (self->mechTotalizer_ + self->amt_middle);
		#endif

		self->mech_totalizer = (int) self->mechTotalizer;

//		pulser2amt(self->current_pulser);

//		if(r_volTotaliser1 != old_r_volTotaliser1)
//		{
//			self->totalizerTimer = 0;
//	  //			then toggle the totaliser harware I/O.
//			drive_totaliser1(ACTIVATE);

//		}
		if(self->mech_totalizer != self->mech_totalizer_old)
		{
			self->totalizerTimer = 0;

	        //	then toggle the totaliser harware I/O.
	  		drive_totaliser(ACTIVATE);
		}
		else
		{
			//deactivate totaliser output...
			if(self->totalizerTimer > 200)
			{
				drive_totaliser(DEACTIVATE);
			}

		}
		self->old_r_volTotaliser = self->r_volTotaliser;   //update...

		//===================// Update... //===================//
		self->mech_totalizer_old = self->mech_totalizer;
	  	//-----------------------------------------------------//

//	  	if (self->totaliser_vol_cal - self->previous_totaliserVol_cal >= THRESHOLD_TV)
//		{
//			self->previous_totaliserVol_cal = self->totaliser_vol_cal;
//
//			save_totaliserFrequent_fram(side_a);
//			save_totaliserFrequent_eeprom(side_a);
//		}
	 //============================================================

		if(self->settings_stream1[0].mode == AUTO_MODE)
		{

			 if( (self->pump_status == STATUS_FILLING) || (self->pump_status == STATUS_MAMO_REACHED) )
			 {

			 }

			 //////////////////////////////////////////////////////////////
			 ///////// SIGNALS GO-CONTROLLER ABOUT NOZZLE STATUS //////////

			 self->status_change_noz = 1;
			 self->nozzle_out = false;

			 self->nozzleDown_source = 1;

			 //////////////////////////////////////////////////////////////

		}

		return  write_flash_State;
	 }

  //-------------------------------------------

	 self->pump_status = STATUS_FILLING_COMP;

      return idle_State;
}

eSystemState error_clear_Handler(void)
{
	if(self->self->irrecov_flag == 0)
	{
		clear_screen();

		if(self->settings_stream1[0].display_format == PL)
		{
			lcd_print_line1(self->upper);
			lcd_print_line2(self->middle);
		}
		else if(self->settings_stream1[0].display_format == LP)
		{
			lcd_print_line1(self->middle);
			lcd_print_line2(self->upper);
		}


		char str__[8]= {0};
		snprintf(str__, sizeof(str_), "%.2f", self->litre_price);
		lcd_print_line3(str__);
		self->error_clr_flag = 0;  //clear flag..

//			if(self->calibration_error == 1)
//			{
//				self->calibration_error = 0;
//			}
//
//			else

		if(self->self->changeLitrePrice1_2 == 1)
		{
			self->self->changeLitrePrice1_2 = 0;

//				if(self->settings_stream1[0].mode == AUTO_MODE)
//				{
//					if(pump_status_ != STATUS_FILLING)
//					{
//
//					}
//				}
		}

		else if(self->totalizer_error == 1)
		{
			self->totalizer_error = 0;
		}

//			else if(HAL_GPIO_ReadPin(pulser1_detect_GPIO_Port, pulser1_detect_Pin) == 0 )
//			{
//
//			}

		//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF//
		//=============== NOZZLE DOWN CLEARING ROUTINE ==================//
		//RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR//

		self->nozzle_flag_key = 0;
		self->nozzle_flag_key_old = 1;

		 //FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF//

		return idle_State;
	}
	else
	{
		return inactive_State;  //if the error is an irrecoverable error dont return to idle state.
	}
}
//============================================================================================================
eSystemState tot_error_Handler(void)
{
   //set irrecoverable error flag.
//	self->irrecov_flag = 1;

	self->totalizer_error = 1;

	lcd_print_line1("   tot   ");
	lcd_print_line2("  Error ");

    return inactive_State;
}

eSystemState inactiveState_Handler(void)
{
    //error_state = 1;
		  // All errors land here....
		  // Only the CLEAR  key returns pump from this state....
          // key reception is done in the key_entry handler..

	return inactive_State;
}
eSystemState timeout_Handler(void)
{
	 if(eLastState != authorised_nozzleup_State)
	 {
		 lcd_print_line1(" self->t out  ");
		 lcd_print_line2("--------");

		 if( ((self->pump_LitreOverflow == 1) && (self->pulser_rem > 0 )) || ((self->display_overflow == 1) && (self->pulser_rem > 0 )) || ( (self->authorizedSale_overflow == 1) && (self->pulser_rem > 0 ) ))
		 {1
			  if(self->pump_LitreOverflow == 1)
				  self->pump_LitreOverflow = 0;
			  if(self->display_overflow == 1)
				  self->display_overflow = 0;
			  if(self->authorizedSale_overflow == 1)
			  {
				  self->authorizedSale_overflow = 0;
			  }
		 }
	 }

  //write the commodity self->price...
  // char str__[8]= {0};
  // snprintf(str__, sizeof(str__), " %.2f", self->litre_price);
  // lcd_print_line3(str__);
  // lcd_print_line3("-----");

  //compose the kind of timeout error
  if(self->eLastState == nozzleup_waitingforauth_State)
  {
	 lcd_print_line3("Err1 ");
	 self->self->nozzleup_awaitingauth_state_not_timedOut = 0;
  }

  if(self->eLastState == authorised_nozzledown_State)
  {
     lcd_print_line3("Err2 ");
  }

  if(self->eLastState == authorisation_paused_State)
  {
	 lcd_print_line3("Err3 ");
  }

 if(self->eLastState == filling_paused_State)
  {
	 lcd_print_line3("Err4 ");
  }

 if(self->eLastState == authorised_nozzleup_State)
 {
	 if( ((self->pump_LitreOverflow == 1) && (self->pulser_rem > 0 )) || ((self->display_overflow == 1) && (self->pulser_rem > 0 )) || ( (self->authorizedSale_overflow == 1) && (self->pulser_rem > 0 ) ) )
	 {
		  if(self->pump_LitreOverflow == 1)
			  self->pump_LitreOverflow = 0;
		  if(self->display_overflow == 1)
			  self->display_overflow = 0;

		  if(self->authorizedSale_overflow == 1)
		  {
			  self->authorizedSale_overflow = 0;
		  }
	 }

	 lcd_print_line1("    No ");
	 lcd_print_line2("  Flouu ");
	 lcd_print_line3("Err15 ");
 }
// if(self->eLastState == power_failure)
//  {
//	 lcd_print_line3("err5 ");
//  }
// if(self->eLastState == storage_error)
//  {
//	 lcd_print_line3("err6 ");
//	 store fail
//  }
// if(self->eLastState == pump_maxLitres) //self->pump_LitreOverflow
//  {
//	 lcd_print_line3("err7 ");
//	 pump limit
//  }
// if(self->eLastState == p/l = 0 @start)
//  {
//	 lcd_print_line3("err8 ");
//  }
// if(self->eLastState == p/l = 0 @self->filling)
//  {
//	 lcd_print_line3("err9 ");
//  }
// if(self->eLastState == comm error)
//  {
//	 lcd_print_line3("err10 ");
//  }
// if(self->eLastState == data error)
//  {
//	 lcd_print_line3("err11 ");
//  }
// if(self->eLastState == back pulses from idle)
//  {
//	 lcd_print_line3("err12 ");
//  }
// if(self->eLastState == 4ward pulses from idle)
//  {
//	 lcd_print_line3("err13 ");
//  }
// if(self->eLastState == no flow timeout)
//  {
//	 lcd_print_line3("err14 ");
//  }
// if(self->eLastState == flow lost timeout)
//  {
//	 lcd_print_line3("err15 ");
//  }
// if(self->eLastState == unfinished)
//  {
//	 lcd_print_line3("err16 ");
//  }


// if(self->eLastState == currentFlow < prevFlow)
//  {
//	 lcd_print_line3("err4 ");
//  }

 	 self->pump_status = STATUS_FILLING_COMP;

	 return inactive_State;
   // return idle_State;
}

uint8_t key_available()
{
	if ( self->keypress__ > 0)
		return 1;
	else
		return 0;
}
//========================================================================
int8_t read_keypad()
{
   //uint8_t ret = 0;
   int temp = 0;

  if ( self->keypress__ > 0)  //If key available.
     {
	  self->keypress__ = 0;
          if( (self->kkey >= '0' ) && (self->kkey <= '9') )//if numerals
          {
        	if(_index > 7)
        	  {
        		  _index  = 7; temp = 7;
        	  }
            	  else
        	  {
            	  temp = _index;
            	  self->keypad_buf[_index++] = self->kkey;
            	  self->keypad_buf[_index  ] = 0;

            	  self->keypad_pw_xter[temp++] = '-';
            	  self->keypad_pw_xter[temp  ] = 0;

        		  if ((self->progg == 1) && (self->self->auth == not_auth))
        			  keypad_print(self->keypad_pw_xter);
        	  }
                return 0;
          }

          else if (self->kkey == 'F' )  //if clear key,
			 {
               _index = 0; temp = 0;
               for(uint8_t i = 0; i < 10; i++)
                {
            	   self->keypad_pw_xter[i] = 0;
            	   self->keypad_buf[i] = 0;
                }
               //lcd_print_line2("        ");  //clear second line.
               return 'F';
			 }

          else if (self->kkey == 'D' )  //if ENTER key.
			 {
        	    return 'D';
			 }

          else if (self->kkey == 'B' )  //if UP key.    // p/l -
			 {
				return 'B';
			 }

          else if (self->kkey == 'C' )  //if DOWN key.
			 {
				return 'C';
			 }

          else if (self->kkey == 'F' )  //if clear key.
			 {
				return 'F';
			 }

          else if (self->kkey == 'A' )  //if back key.    // stop/exit
			 {
				return 'A';
			 }
          else if (self->kkey == 'G' )  //if F1 key.
			 {
				return 'G';
			 }
//          else if (kkey == '*' )  //if - key.
//			 {
//				return '*';
//			 }

             return -1;   // return non-zero for non-numeric keys zero for numerals
						  // and -1 for no key pressed.
						  // store numeric keys inside buffer...
						  // the number of keys stored is also updated.
     }
  return 0;
}
//-----------------------------------------------------------------------------
void clr_screen(void)
{
	lcd_print_line1("        ");
      lcd_print_line2("        ");
      lcd_print_line3("        ");
}

//=============================================================================
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//=============================================================================
eSystemState operator_State_Handler(void)
{
	    static int8_t self->fxn = 0;
        static int8_t self->indx1 = 0,
        		   	  self->indxx1 = 0;

	    static int8_t self->pump_indx = 1;
	    static int8_t self->pump_indxx = 0;

	    extern  self->log_max;
	    static int8_t self->key19_sto_ = 0,
	    			  self->keyy19_sto_ = 0;

	    int8_t pkey = 0;
	    int8_t keyy19 = 0;
	    extern int  self->printer_status;

	    uint8_t rd19 = 0;

   //===========================================================
	    // POWER FAILURE SENSE
   //===========================================================

	if(self->settings_stream1[0].mode == AUTO_MODE)
	{
		if(self->timer_go >= TIMEOUT_GO)   //if go's timeout is 5sec threshold
		{
//			self->pump_status = STATUS_PNP;
//			return pnp_State;
		}
	}


	#if sense_power == 1
		  	  if(readpwr() == 0)
			  {
		  		   modem_power(DEACTIVATE);

		  		   //count time elapsed
				   if (self->shutdown_timer > 120)
				   {
					   displayandkeypad_power(DEACTIVATE);   //shutdown... after  2 minutes
				   }
			  }
		  	  else
		  	  {
		  		  //clear the timer//
		  		self->shutdown_timer = 0;
		  	  }
	#endif
	//===========================================================



	 //===========================================================

	   if(readNozzle1() == 1)      //@Nozzle-up, the below features should be disabled
	   {
		   self->nozzleUp_inProgMode = 1;

		   self->indx1 = 0;
		   self->indxx1 = 0;

		   return idle_State;
	   }

    //===========================================================
  	    rd19 = readkey19_state();

		if ( (rd19 == 1) && ( self->key19_sto_ == 0) )
		 {
			 if (self->indx1 == 1)
				self->indx1 = 2;
			else
				self->indx1 = 1;
			self->key19_sto_ = rd19;
		 }
		 self->key19_sto_ = rd19;
		 keyy19 = rd19;

	//===========================================================
	//                        totaliser view.
	//===========================================================
	  if ( (self->operatorfxn == totaliser_view) )
		{
		  // level 0. totaliser
		   if (self->indx1 == 0 ) self->indx1 = 1;
		   if (self->indxx1 == 0 ) self->indxx1 = 1;
		  // -------------- test keys....----------------
		  	     pkey = read_keypad();

		  	         if( (pkey > 0) )  //key detected
		  	         {
		  	        	if (pkey == 'B')  // up key  --> KeyF2
						{
						  //self->indx1++;
							if (self->indxx1 == 1)
									self->indxx1 = 2;
							else
									self->indxx1 = 1;
						}

		  	        	else if (pkey == 'C')  // down key
						{
						   //self->indx1--;
							if (self->indxx1 == 1)
								self->indxx1 = 2;
							else
								self->indxx1 = 1;
						}

		  	        	else if (pkey == 'A')  // back key
						{
		  	        	   lcd_print_line1(self->upper);
						   lcd_print_line2(self->middle);

						   char str__[10]= {0};
						   snprintf(str__, sizeof(str_), "%.2f", self->litre_price);
						   lcd_print_line3(str__);

						   self->indx1 = 0;
						   self->indxx1 = 0;

						   return idle_State;  //go back to idle state.
						}
		  	         } //if pkey

     	 //-----------------------------------------------------
			   if (self->t >= 500)
				 {
		  	    	   char scc[15] = {0};

		  	    	   char line1[9] = {0};
		  	    	   char line2[9] = {0};
	                   int8_t j,
					   	   	  jj = 0,
							  k = 0;

	                   int i,
					   	   res = 0;

	               //===============================================
		  	    	  if (self->indx1 == 1)
		  	    	  {
		  	    		  if(self->indxx1 == 1)
		  	    		  {
							  res = snprintf(scc, sizeof(scc), "%.2f", self->totaliser_vol_cal);
							  if(res > 8)  //if lenght of the number > 8
							  {
								  i = res;
								  j = 0;
								  jj = 0;

								   while(i > 8)
								   {
									   line1[j] = scc[j];
									   j++;
									   i--;
									   jj++;
								   }

								   i = 0;
								   while(i < 8)
								   {
									 line2[i] = scc[j];
									 j++;
									 i++;
								   }
								   clear_screen();
								   printDisp_c("l", 1, 0, 8, LT, CLEAR);
								   printDisp_c(line1, 1, (8-jj), 5, LT, NOCLEAR);  //display self->price
//								   printDisp_c(line1, 1, 0,5,RT,CLEAR );
								   lcd_print_line2(line2);
								 }
								 else
								 {
									 lcd_print_line1("l        ");
									 lcd_print_line2("        ");
									 printDisp_f(self->totaliser_vol_cal, 2, 0, 5, RT, CLEAR ); //lcd_print_line2(scc);
								 }
							  lcd_print_line3("tot A");
						   }  //  if (self->indx1 == 1)

		  	    		   else if(self->indxx1 == 2)
		  	    		   {
							  res =  snprintf(scc, sizeof(scc), "%.2f", self->totaliser_amt_cal);
							  if( res > 8)  //if lenght of the number > 8
								 {
								  i = res;
								  j = 0;
								  jj = 0;
								   while(i > 8)
								   {
									   line1[j] = scc[j];
									   j++; i--;
									   jj++;
								   }
								   i = 0;
								   while(i < 8)
								   {
										 line2[i] = scc[j];
										 j++; i++;
								   }
								   clear_screen();
								   printDisp_c("p", 1, 0, 8, LT,CLEAR);
								   printDisp_c(line1, 1, (8-jj), 5, LT, NOCLEAR);
//								   printDisp_c(line1, 1, 0,5,RT,CLEAR );
								   lcd_print_line2(line2);
								 }
								 else
								 {
									 lcd_print_line1("p        ");
									 lcd_print_line2("        ");
									 printDisp_f(self->totaliser_amt_cal, 2, 0, 5, RT, CLEAR ); //lcd_print_line2(scc);
								 }
							  lcd_print_line3("tot A");
						   }  //  if (self->indx1 == 1)
		  	    	   }

		  	       //-----------------------------------------------------

		  	    	  else if (self->indx1 == 2)
					  {
						 if(self->indxx1 == 1)
						 {
							res =  snprintf(scc, sizeof(scc), "%.2f", totaliser_vol2c);
							if( res > 8)  //if lenght of the number > 8
							 {
							  i = res;
							  j = 0;
							  jj = 0;
							   while(i > 8)
							   {
									  line1[j] = scc[j];
									  j++; i--;
									  jj++;
							   }
									 i = 0;
							   while(i < 8)
							   {
									 line2[i] = scc[j];
									 j++; i++;
							   }
							   clear_screen();
							   printDisp_c("l", 1, 0, 8, LT, CLEAR);
							   printDisp_c(line1, 1, (8-jj), 5, LT, NOCLEAR);
//							   printDisp_c(line1, 1, 0,5,RT,CLEAR );
							   lcd_print_line2(line2);
							 }
							 else
							 {
								 lcd_print_line1("l        ");
								 lcd_print_line2("        ");
								 printDisp_f(self->totaliser_vol_cal, 2, 0, 5, RT, CLEAR ); //lcd_print_line2(scc);
							 }
						  lcd_print_line3("tot B");
						 }  //  if (self->indx1 == 2)

						 else if (self->indxx1 == 2)
					     {
						    res =  snprintf(scc, sizeof(scc), "%.2f", self->totaliser_amtc);
							if( res > 8)  //if lenght of the number > 8
							 {
							  i = res;
							  j = 0;
							  jj = 0;
							   while(i > 8)
							   {
									  line1[j] = scc[j];
									  j++; i--;
									  jj++;
							   }
									 i = 0;
							   while(i < 8)
							   {
									 line2[i] = scc[j];
									 j++; i++;
							   }
							   clear_screen();
							   printDisp_c("p", 1, 0, 8, LT, CLEAR);
							   printDisp_c(line1, 1, (8-jj), 5, LT, NOCLEAR);
//							   printDisp_c(line1, 1, 0,5,RT,CLEAR );
							   lcd_print_line2(line2);
							 }
							 else
							 {
								 lcd_print_line1("P        ");
								 lcd_print_line2("        ");
								 printDisp_f(self->totaliser_amtc, 2, 0, 5, RT, CLEAR ); //lcd_print_line2(scc);
							 }
						  lcd_print_line3("tot B");
						 }  //  if (self->indx1 == 2)
					   }

		  	    	 self->t = 0;    //reset the timer.
		            }  //if (self->t > 500)
	      return operator_State;
	  } ///if ( (self->auth == no_auth)  )

//================= End of totaliser view. ========================
//=================================================================


//=================================================================
//                        log view.
//=================================================================
	         if (  (self->self->operatorfxn == log_view) )
	          	{
	        	    static int self->loop_ = 0;
	            	static int self->log_indx = 0;
	            	static int self->log_indx_indx = 0;

	            	static uint32_t self->flash_loc;
	            	static uint32_t self->log_no;

	            	float pr_  =  self->price_real;
					float vl_  =  self->amt_real;
					char *tm_  =  "---------";

	            	if(self->loop_ == 0)    //do this only the first time
					{
						// data_size = sizeof(log_a_new);
					  if (operating_side == side_a)
					  {
							 self->flash_loc = self->flash_info.current_loc; // - 256;
							 self->log_no = self->flash_info.number_logs;
							 if(self->log_no == 0)
							 {
								  pr_  =  0.00; //self->price_real;
								  vl_  =  0.00; //self->amt_real;
								  tm_  =  "--------";
							 }
							else
							{
							  //calculate the previous save id from the current
								if (self->flash_loc <= self->flash_begin) //at begining of storage
								{
									self->flash_loc = self->flash_end;    // - data_size; //wrap
								}
								else
								{
								   self->flash_loc = self->flash_loc - data_size;
//								    	   if (self->flash_loc == 0 )  // flash_beginA) //at begining of storage
//											{
//								    			// this shouldnt occur ,  just in case !
//								    			self->flash_loc = flash_endA - data_size; //wrap
//											}
//								    		   uint32_t next_loc = flash_infoA.current_loc + 256;  //sizeof(log_a_new);
//											 if (next_loc > flash_endA) next_loc = flash_beginA;   //flash_endA => 0x3FFFFF --> 4,194,303 pg16,383.996
//											 flash_infoA.current_loc  =  next_loc;
//											 flash_infoA.number_logs  =  flash_infoA.number_logs + 1;
//											 EEPROM_Write(flash_info_sto, flash_stoA, &flash_infoA, sizeof(flash_infoA));
								}
							}
							   flash_locc = self->flash_loc;
						}
//						  if (operating_side == side_b)
//							   {
//								 self->flash_loc = flash_infoB.current_loc;
//								 self->log_no = flash_infoB.number_logs;
//								 if(self->log_no == 0)
//									 {
//										  pr_  =  0.00; //self->price_real;
//										  vl_  =  0.00; //self->amt_real;
//										  tm_  =  "--------";
//									 }
//								    else
//									{
//								    	//calculate the previous save id from the current
//										if (self->flash_loc <= flash_beginB) //at begining of storage
//										{
//											self->flash_loc = flash_endB - data_size; //wrap
//										}
//										else
//										{
//											self->flash_loc = self->flash_loc - data_size;
//											if (self->flash_loc < flash_beginB) //at begining of storage
//											{
//												// this shouldnt occur ,  just in case !
//												self->flash_loc = flash_endB - data_size; //wrap
//											}
//										}
//										    flash_locc = self->flash_loc;
//									}
//							   }
	            		  self->loop_ = 1;   //disable first time assignment ^^^^
	            		  self->log_indx_indx = self->log_no;
	            		  self->pump_indx  = 1;  // initialise the side...

	            		  self->flash_read_id = self->flash_loc;

	            		  return read_flash_State;  //goto read the flash location...
						}
	         //==================================
	         //   display the log parameters....

	       		 if(self->pump_indx == 0) self->pump_indx = 1;
	         // -------- test keys....-----------
	          	  	     pkey = read_keypad();
//	          	  	     keyy19 = readkey19_state();

	       if(self->keypress_ == 21)
	       {
               HAL_Delay(1000);
	    	   compose_printer();
	    	   self->printer_status = 1;
	    	   print__1();   //print the transaction.
	       }

	       if(self->printer_status == 1)   print__1();   //print the transaction.

		   if( (pkey > 0) )  //key detected
			 {
//				   if (pkey == 'C')  // up key  //+
					if (pkey == 'B')  // up key  //+
					{
						if (self->log_indx_indx < self->log_no)
						{
							self->log_indx_indx++;  //incr index only when less than max
						if (operating_side == side_a)
							{
								if(self->flash_loc >= self->flash_end)
								{
								  self->flash_loc = self->flash_begin; //wrap
								}
								else
								{
								  self->flash_loc +=  data_size;
								}
								self->flash_read_id = self->flash_loc;
								flash_locc = self->flash_loc;


//												if (next_loc > flash_endA) next_loc = flash_beginA;   //flash_endA => 0x3FFFFF --> 4,194,303 pg16,383.996
//												 flash_infoA.current_loc  =  next_loc;
							}
//	          	  					    if (operating_side == side_b)
//											{
//	          								if(self->flash_loc >= flash_endB)
//												{
//												  self->flash_loc = flash_beginB; //wrap
//												}
//												else
//												{
//												  self->flash_loc += data_size;
//												}
//												flash_read_idB = self->flash_loc;
//												flash_locc = self->flash_loc;
//											}
								 }

							  //  self->flash_read_id = self->flash_loc;
										//flash_beginA + ( self->log_indx_indx *  );
								//fetch the values...
								return read_flash_State;
							}

						 if (pkey == 'C')  // down key   //-
							{
								if (self->log_indx_indx >= 2)
								{
									self->log_indx_indx--; //reduce index only when greater than 0.

								 if (operating_side == side_a)
									{
										if(self->flash_loc <= self->flash_begin)
										{
										  self->flash_loc = self->flash_end - data_size; //wrap
										}
										else
										{
										  self->flash_loc -= data_size;
										}
									  flash_locc = self->flash_loc;
									  self->flash_read_id = self->flash_loc;
								}

//											if (operating_side == side_b)
//											{
//												if(self->flash_loc <= flash_beginB)
//												{
//												  self->flash_loc = flash_endB - data_size; //wrap
//												}
//												else
//												{
//												  self->flash_loc -= data_size;
//												}
//												flash_locc = self->flash_loc;
//												flash_read_idB = self->flash_loc;
//											}
										//self->flash_read_id = self->flash_loc;
										//fetch the values...
									  }
									return read_flash_State;
								}

//							 if (pkey == 'A')  // back key
								if (pkey == 'A')  // stop/exit key
								{
								  self->loop_ = 0;  // enable the first time assignment
								  lcd_print_line1(self->upper);
								   lcd_print_line2(self->middle);

								   char str__[8]= {0};
								   snprintf(str__, sizeof(str__), "%.2f", self->litre_price);
								   lcd_print_line3(str__);
								   return idle_State;  //go back to idle state.
								}

						   /*  if (pkey == 'F')  //change pump index.
								{
								  self->pump_indx++;
									if (self->pump_indx > 2) self->pump_indx = 1; //wrap around
								}   */
						 } //if pkey
			 //-----------------------------------------------------
				   if (self->t >= 500)
					 {
					   static uint8_t scroll,
					   	   	   	  scroll_ = 0;
					   if (scroll >= 4)
						   {
							  if (scroll_++ >= 3) 	  scroll_ = 1;
//	                              else
//	                            	  scroll_ = 0;
							 scroll = 0;
						   }
						 scroll++;

						   //char scc[15] = {0};
						if(self->pump_indx == 0) self->pump_indx = 1;
						   char line1[9] = {0};
						   char line2[9] = {0};
						   char line3[8] = {0};

						  //   int j,k = 0;
						  //   int i,res = 0;
	           	 if(self->log_no > 0)
	           	 {
					   if ( (self->keyy19 == 1) && ( self->keyy19_sto_ == 0) )
						 {
						   self->pump_indxx++;
							if (self->pump_indxx > 2) self->pump_indxx = 1;  //wrap around
							   self->keyy19_sto_ = keyy19;
						 }
						   self->keyy19_sto_ = keyy19;


						   //=======================================================================
//	          	  	    	  if (self->pump_indxx == 1)
//	          	  	    	   {
								  pr_  =  self->log_new.pr__;   //self->price
								  vl_  =  self->log_new.vol__;  //volume
								//  tm_  =  log_a_new.tm_;   //time

	          	  	    		  //dd-hh:mm,    P  , L  ,    T
	          	  	    		  printDisp_c("p", 1, 0, 8, LT, CLEAR);
	          	  	    		  printDisp_f(pr_, 1, 0, 8, LT, NOCLEAR);  //display self->price

	                              if (scroll_ == 1 )
								   {
									 printDisp_c("l", 2, 0, 8, LT, CLEAR);
									 printDisp_f(vl_, 2, 0, 8, LT, NOCLEAR);  //display litre
								   }
	                              else if (scroll_ == 2)
									{
	                            	  char st__[10]= {0};
	                            	  snprintf(st__, sizeof(st__), "%d-%02d-%d", self->log_new.date._dd, self->log_new.date._mm, self->log_new.date._yy);
									 printDisp_c(st__, 2, 0, 8, LT, CLEAR);
									}
	                              else if (scroll_ == 3)
	                              {
	                             	 char st__[10] = {0};
//									  snprintf(st__, sizeof(st__), "%d%02d hrs",log_a_new.time_e._hh,log_a_new.time_e._mn);
//									 printDisp_c(st__,2,0,8,LT,CLEAR);

									 if(self->log_new.time_e._hh <= 11)
									 {
										 snprintf(st__, sizeof(st__), "%02d:%02d am", self->log_new.time_e._hh, self->log_new.time_e._mn);
									 }
									 else if(self->log_new.time_e._hh == 12)
									 {
										 snprintf(st__, sizeof(st__), "%02d:%02d pm", self->log_new.time_e._hh, self->log_new.time_e._mn);
									 }
									 else if(self->log_new.time_e._hh > 12)
									 {
										 snprintf(st__, sizeof(st__), "%02d:%02d pm", (self->log_new.time_e._hh - 12), self->log_new.time_e._mn);
									 }

									 printDisp_c(st__,2,0,8,LT,CLEAR);
	                              }
//	                              	snprintf(line3, sizeof(line3), "An1.%d",self->log_indx_indx);
	                              	snprintf(line3, sizeof(line3), " %05d", self->log_indx_indx);
	                                lcd_print_line3("      ");
	                              	lcd_print_line3(line3);
//	          	  	    	   }  //  if (self->indx1 == 1)

	          	  	    	 //-----------------------------------------------------

//	          				   if (self->pump_indxx == 2)
//	          				      {
//									  pr_  =  log_b_new.pr__;   //self->price
//									  vl_  =  log_b_new.vol__;  //volume
//									  //  tm_  =  log_b_new.tm_;   //time
//
//
//									  //dd-hh:mm,    P  , L  ,    T
//									 printDisp_c("p",1,0,8,LT,CLEAR); printDisp_f(pr_,1,0,8,LT,NOCLEAR);  //display self->price
//
//									 if (scroll_ == 1 )
//									 {
//										 printDisp_c("l",2,0,8,LT,CLEAR); printDisp_f(vl_,2,0,8,LT,NOCLEAR);  //display litre
//									 }
//									 else if (scroll_ == 2)
//									 {
//										  char st__[10]= {0};
//										  snprintf(st__, sizeof(st__), "%d-%02d-%d",log_b_new.date._dd,log_b_new.date._mm,log_b_new.date._yy);
//										  printDisp_c(st__,2,0,8,LT,CLEAR);
//									 }
//									 else if (scroll_ == 3)
//									  {
//										  char st__[10]= {0};
//										  snprintf(st__, sizeof(st__), "  %d-%02d",log_b_new.time._hh,log_b_new.time._mn);
//										  printDisp_c(st__,2,0,8,LT,CLEAR);
//									  }
//
//	          					    snprintf(line3, sizeof(line3), "bn1.%d",self->log_indx_indx );
//	          					    lcd_print_line3("      ");
//	          					    lcd_print_line3(line3);
//	          				      }  //  if (self->indx1 == 2)
	           	             }
							 else
							 {
								 lcd_print_line1(" No log ");
							   lcd_print_line2("        ");
							   lcd_print_line3("      ");
							 }   // if log  > 0.

	          		       self->t = 0;    //reset the timer.
	          	        }  //if (self->t > 500)

	                return operator_State;
	            } ///if ( (self->auth == no_auth) && (self->access == level0) && (self->fxn == 1) )
	return operator_State;
}

eSystemState progState_Handler(void)
{
	static int self->indx = 0;
//	static int index_menu = 0;
	static int self->indx2 = 0;
	static int self->indx3 = 0;
	static int self->attempts = 0;
    static int self->fxn = 0;
    static int self->pump_indx = 1;
    extern  self->log_max;
   //self->progg = 1;
   int temp = 0;
   int pkey = 0;
   static  int self->access = non;
   static int self->key19_sto_ = 0;
   static int dp_ = 0;
   static int ln_ = 0;

   static uint8_t firstTime = 1,
		   	   	  self->enter_key = 0;
//				  index_pass = 0;

   static uint8_t self->index_generic = 0;
//
//   static uint16_t otp_seed1;

   char st__[10] = {0},
		pass_[9];

   uint8_t rd19 = 0;

   char *endPtr;

#ifndef DEV_MODE
   if(self->prog_revisitt == 1)
   {
		self->indx = 0;
		//	static int index_menu = 0;
		self->indx2 = 0;
		self->indx3 = 0;
		self->attempts = 0;
		self->fxn = 0;
		self->pump_indx = 1;
		self->log_max;
		//self->progg = 1;
		temp = 0;
		pkey = 0;
		self->access = non;
		self->key19_sto_ = 0;
		dp_ = 0;
		ln_ = 0;

		firstTime = 1;
		self->enter_key = 0;

		self->prog_entry = 0;

		self->prog_revisitt = 0;
   }
#endif  //#ifndef DEV_MODE


   if(self->prog_entry == 0 )   // first entry...
   {
	   //clear the state variables.
	    self->indx = 0;
	   	index_menu = 0;
	    self->indx2 = 0;
	    self->indx3 = 0;
	    self->attempts = 0;
	    self->fxn = 0;
	    self->pump_indx = 1;
	      //self->progg = 1;
	     temp = 0;
	     pkey = 0;

	     self->self->auth = not_auth;

	     self->access = non;

	     self->prog_entry = 1;  //To prevent clearing variables on re-entry.
   }


 //===========================================================

   //if(read_event1() == _nozzleup_Event)
//   if(readNozzle1() == 1)  //@Nozzle-up, the below features should be disabled
//   {
//	   self->nozzleUp_inProgMode = 1;
//
//	   return idle_State;
//   }

 //===========================================================

//   else if (self->auth == not_auth)
   if (self->auth == not_auth)
   {
      //clear keypad....
	  //acquire the password goto level0 self->access level if non supplied.

	   pkey = read_keypad();
//	   char pass_[9];

	  if(pkey > 0)  //key detected
	   {
    	   if (pkey == 'D') // enter key...
    	   {
              //password provided...  settings[0].passwd1
			   strncpy(pass_, self->keypad_buf, sizeof(pass_));

				   if( (index_menu == pass1) && (strcmp(pass_, password_level1) == 0) ) //level 1 self->access ?
				   {
					   index_menu = 0;
					   self->auth = authed;
					   self->access = level1;
					   self->fxn = nothing;
					   clear_buffer1();
						//keypad_buf[0] = 0;
						//self->keyboard_entry[0] = 0;
					   return prog_State;
				   }
				   else if( (index_menu == pass2) && (strcmp(pass_, password_level2) == 0) ) //level 2 self->access ?
				   {
					   index_menu = 0;
					   self->auth = pre_otp_authed;
					   self->access = level2;
					   self->fxn = nothing;
					   clear_buffer1();

					   retrieve_otpSeed_session_fram(side_a);

					   if(otpSeed_session[0].otpSeed_flag == OTPSESSION_ON)
					   {
						   uint32_t time_stamp = RtcToInt(2019);
						   time_stamp = time_stamp - otpSeed_session[0].time_stamp;

						   if(time_stamp <= 4096)  //4096 translates to 1 hour
						   {
							   otp_seed1 = otpSeed_session[0].otp_seed;
						   }
						   else
						   {
							   otp_seed1 = generate_otpVariable1();
							   save_otpSeed_session_fram(side_a);
						   }
					   }
					   else
					   {
						   otp_seed1 = generate_otpVariable1();
						   save_otpSeed_session_fram(side_a);
					   }

					   return prog_State;
				   }
				   else if( (index_menu == pass3) && (strcmp(pass_, password_level3) == 0) ) //level 3 self->access ?
				   {
					   index_menu = 0;
					   self->auth = pre_otp_authed;
					   self->access = level3;
					   self->fxn = nothing;
					   clear_buffer1();

					   retrieve_otpSeed_session_fram(side_a);

					   if(self->otpSeed_session[0].otpSeed_flag == OTPSESSION_ON)
					   {
						   uint32_t time_stamp = RtcToInt(2019);
						   time_stamp = time_stamp - self->otpSeed_session[0].time_stamp;

						   if(time_stamp <= 4096)  //4096 translates to 1 hour
						   {
							   self->otp_seed = self->otpSeed_session[0].otp_seed;
						   }
						   else
						   {
							   self->otp_seed = generate_otpVariable1();
							   save_otpSeed_session_fram(side_a);
						   }
					   }
					   else
					   {
						   self->otp_seed = generate_otpVariable1();
						   save_otpSeed_session_fram(side_a);
					   }

					   return prog_State;
				   }
				   else if(self->access == non)             //no match...
				   {
					  lcd_print_line2(" Access");
					  lcd_print_line3("Denied");
					  HAL_Delay(200);
					  lcd_print_line2("        ");
					  lcd_print_line3("        ");
					  HAL_Delay(200);
					  lcd_print_line2(" Access");
					  lcd_print_line3("Denied");
					  HAL_Delay(200);
					  lcd_print_line2("        ");
					  lcd_print_line3("        ");
					  HAL_Delay(200);
					  lcd_print_line2(" Access");
					  lcd_print_line3("Denied");
					  HAL_Delay(200);
					  lcd_print_line2("        ");
					  lcd_print_line3("        ");
					  clear_buffer1();   	//clear the buffer
					  lcd_print_line2(self->keypad_pw_xter);
					  lcd_print_line3("        ");
//					  index_menu = 0;
				   }
    	  }// ================ if ENTER KEY PRESSED =================

    	   else if (pkey == 'F') // cancel key...
		   {
			  clear_buffer1();
			  /* keypad_buf[0] = 0;
			   self->keyboard_entry[0] = 0;
			   self->keypad_pw_xter[0] = 0;
			   _index = 0; */
		   }
    	   else if (pkey == 'A') // stop/exit key...
		   {
    		    clear_buffer1();
    		    self->prog_entry = 0;
				self->prog_revisit = 1;
				index_menu = 0;

				#ifndef DEV_MODE
				self->prog_revisitt = 1;
				#endif //#ifndef DEV_MODE

			  return idle_State;

			  /* keypad_buf[0] = 0;
			   self->keyboard_entry[0] = 0;
			   self->keypad_pw_xter[0] = 0;
			   _index = 0; */
		   }
    	   else if (pkey == 'B')  // up key
		   {
				 if (index_menu <= 1)  index_menu++;
				 else if(index_menu == 2) index_menu = 0;
		   }
    	   else if (pkey == 'C')  // down key
		   {
				if (index_menu > 0) index_menu--;
				else if(index_menu == 0) index_menu = 2;
		   }

        }  //if(pkey > 0)  //===== if key detected =====

	    if (self->t >= 500)  //time to display?
		{
	    	if(index_menu == 3) index_menu = 2;

//	    	lcd_print_line1("  pass  ");
	    	lcd_print_line1(pass[index_menu]);  //show password level menu
			lcd_print_line2(self->keypad_pw_xter);
			lcd_print_line3("        ");
			self->t = 0;
		}
	    return prog_State;  //exit...
   }   //if (not_auth)


//================================== MODE =================================
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//=========================================================================
//                           MENU DISPLAY & SCROLL
//=========================================================================
   else if ( (self->auth == authed) && (self->access == level1) && (self->fxn == nothing)  )
   {
	   if (self->t >= 500)
		 {
			if(index_menu == 3) index_menu = 2;
			lcd_print_line1(menu_level1[index_menu]);   //show menu item.
			lcd_print_line2("        ");
			self->t = 0;
		 }
	 // -------------- test keys....----------------
	 pkey = read_keypad();
	   if(self->pump_indx == 0) self->pump_indx = 1;

		 if( (pkey > 0) )  //key detected
		 {
//          	        	 if (pkey == 'C')  // up key
			if (pkey == 'B')  // up key
			{
//          					 if (index_menu < 13)  index_menu++;
			 if (index_menu <= 1)  index_menu++;
			 else if(index_menu == 2) index_menu = 0;
			}

//						if (pkey == 'B')  // down key
			if (pkey == 'C')  // down key
			{
				if (index_menu > 0) index_menu--;
				else if(index_menu == 0) index_menu = 2;
			}

			 if (pkey == 'A')  // BACK key...
			 {
				   self->auth = authed;
				   self->access = level1;
				   self->fxn = nothing;
				   clear_buffer1();
				  // keypad_buf[0] = 0;
			  }

			 if (pkey == 'D')  // enter key
				{
					   switch(index_menu)
						{
					   	   	  case 0 :
									self->fxn = shift_total;
									clear_buffer();
									clr_screen();
									break;

					   	   	  case 1 :
									self->fxn = setprice;
									self->pump_indx = 1;
									clear_buffer();
									clr_screen();
									break;

							  case 2 :
									self->fxn = setclock;
									clear_buffer();
									clr_screen();
									break;
						}
				}
		 }
	 return prog_State;
   }

   //==================================================================================================


   else if ( (self->auth == pre_otp_authed) && ( (self->access == level2)||(self->access == level3) ) && (self->fxn == nothing)  )
   {
		if (self->t >= 300)
   		{
			lcd_print_line1("OTP Seed");

   		  snprintf(st__, sizeof(st__), "    %d", self->otp_seed);
   		  lcd_print_line2(st__);

			for(uint8_t ii = 0; ii < 7; ii++)
			{
				self->keyboard[ii] = st__[ii+1];
				self->keyboard[ii+1] = 0;
			}
			keypad_print(self->keyboard);


   		  lcd_print_line3("  A  ");

   		  self->t = 0;
   		}

      	 // -------------- test keys....----------------
      	 pkey = read_keypad();
      	   if(self->pump_indx == 0) self->pump_indx = 1;

      		 if( (pkey > 0) )  //key detected
      		 {
//      			 if (pkey == 'A')  // BACK key...
//				   {
//					   self->auth = authed;
//					   self->access = level2;
//					   self->fxn = nothing;
//					   clear_buffer1();
//					  // keypad_buf[0] = 0;
//				   }

      			 if (pkey == 'D')  // enter key
      			 {
				   index_menu = 0;
				   self->auth = otp_authed;
				   if(self->access == level2) self->access = level2;
				   else if(self->access == level3) self->access = level3;

				   self->otp_codeInt = getCode(self->otp_seed);

				   sprintf(self->otp_code, "%06ld", self->otp_codeInt);

				   memset(self->keypad_buf, '\0', sizeof(self->keypad_buf));

				   self->fxn = nothing;
				   clear_buffer();
      			 }
      		 }
      	 return prog_State;
     }
             //      end of show menu for level1

   //=================================================================================================


	else if ( (self->auth == otp_authed) && ( (self->access == level2)||(self->access == level3) ) && (self->fxn == nothing)  )
	{
		if(self->keypad_buf[0] == NULL)
	    {
			if ( (self->t >= 300) && (self->t <= 500) )
			{
				lcd_print_line1("   OTP  ");

			  lcd_print_line2("_       ");
			}
			else if ( (self->t > 500) && (self->t <= 700) )
			{
				  lcd_print_line2("        ");
			}


			  lcd_print_line3("  A  ");

			  if (self->t > 700) self->t = 0;
	   }

		else if (self->t >= 300)
		{
			lcd_print_line1("   OTP  ");

		 lcd_print_line2(self->keypad_buf);

		  lcd_print_line3("  A  ");

		  self->t = 0;
	   }

		 // -------------- test keys....----------------
		 pkey = read_keypad();
		   if(self->pump_indx == 0) self->pump_indx = 1;

			 if( (pkey > 0) )  //key detected
			 {
	//      			 if (pkey == 'A')  // BACK key...
	//				   {
	//					   self->auth = authed;
	//					   self->access = level2;
	//					   self->fxn = nothing;
	//					   clear_buffer1();
	//					  // keypad_buf[0] = 0;
	//				   }

				 if (pkey == 'D')  // enter key
				 {
				   strncpy(pass_, self->keypad_buf, sizeof(pass_));

			#ifdef OTP_ENABLE
				   if(strcmp(pass_, self->otp_code) == 0)  //level 2 0r 3 self->access ?
				   {
//					   save_otp(side_a);
					   save_otpSeed_session_fram(side_a);
			#else
				   if( (strcmp(pass_, otp_code1) == 0) || (strcmp(pass_, self->otp_code) != 0) )  //level 2 0r 3 self->access ?
				   {
			#endif      //#ifdef OTP_ENABLE
					   index_menu = 0;
					   self->auth = authed;
					   if(self->access == level2) self->access = level2;
					   else if(self->access == level3) self->access = level3;
					   self->fxn = nothing;
					   clear_buffer();
				   }
				   else
				   {
					   lcd_print_line1(" Access ");
					   lcd_print_line2(" Denied ");
					   lcd_print_line3("        ");
					   HAL_Delay(1000);

					   memset(self->keypad_buf, '\0', sizeof(self->keypad_buf));
					   clear_buffer();
					   clear_screen();
				   }
				 }
			 }
		 return prog_State;
	}

  //=================================================================================================

   else if ( (self->auth == authed) && ( (self->access == level2)||(self->access == level3) ) && (self->fxn == nothing)  )
   {

	   if (self->t >= 500)
   		 {
   			if(self->access == level2)
   			{
   				if(index_menu == 20) index_menu = 0;
   				lcd_print_line1(menu_level2[index_menu]);   //show menu item.
   			}
   			else if(self->access == level3)
			{
   				if(index_menu == 23) index_menu = 0;
   				lcd_print_line1(menu_level3[index_menu]);   //show menu item.
			}

   			lcd_print_line2("        ");
   			self->t = 0;
   		 }
   	 // -------------- test keys....----------------
   	 pkey = read_keypad();
   	   if(self->pump_indx == 0) self->pump_indx = 1;

   		 if( (pkey > 0) )  //key detected
   		 {
   //          	        	 if (pkey == 'C')  // up key
   			if (pkey == 'B')  // up key
   			{
   //          					 if (index_menu < 13)  index_menu++;
   			 if (self->access == level2)
   			 {
   				 if (index_menu <= 20)  index_menu++;
   				 else if(index_menu == 21) index_menu = 0;
   			 }
   			 else if (self->access == level3)
   			 {
   				if (index_menu <= 22)  index_menu++;
   				else if(index_menu == 23) index_menu = 0;
   			 }
   			}

   //						if (pkey == 'B')  // down key
   			else if (pkey == 'C')  // down key
   			{
   			    if (self->access == level2)
   			   	{
   			    	if (index_menu > 0) index_menu--;
   			    	else if(index_menu == 0) index_menu = 19;
   			   	}
   			    else if (self->access == level3)
   			   	{
   			    	if (index_menu > 0) index_menu--;
   			    	else if(index_menu == 0) index_menu = 22;
   			   	}
   			}

   			else if (pkey == 'A')  // BACK key...
		    {
			   self->auth = authed;
			   if(self->access == level2) self->access = level2;
			   else if(self->access == level3) self->access = level3;
			   self->fxn = nothing;
			   clear_buffer();
			  // keypad_buf[0] = 0;
		    }

   			else if (pkey == 'D')  // enter key
   			{

//   				 if(index_menu == 14) index_menu += 1;
//{"  nnode ", "Address ", " Nozzle ", "Disp.Form", "  Price ", "no flo. self->t", "Hi. Litre", "Ch  Pass", "Calibrat.", "Display ", "  Cloc",
//"Sides No", "Start CL", "Calib.Can", "Shift No"};
//"Calib.typ"

   					   switch(index_menu)
   						{
   							  case setmode:
   									self->fxn = setmode;
   									self->pump_indx = 1;
//   									return prog_State;
   									clear_buffer();
   									clr_screen();
   									break;

   							  case setaddress:
   									self->pump_indx = 1;
   									self->fxn = setaddress;
   									//clear the self->keyboard buffer
   									//index_ = 0;
   									clear_buffer();
   									clr_screen();
   								    break;

   							  case setnozzle:
   									self->pump_indx = 1;
   									self->fxn = setnozzle;
   									clear_buffer();
   									clr_screen();
   									break;

   							  case display_orientation:
   								   self->pump_indx = 1;
   								   self->fxn = display_orientation;
   								   clear_buffer();
   								   clr_screen();
   								   break;

   							  case setprice:
   									self->fxn = setprice;
   									self->pump_indx = 1;
   									//clear the self->keyboard buffer
   									clear_buffer();
   									clr_screen();
   									//index_ = 0;
   									//self->keyboard_entry[0] = 0;
   									//self->keyboard_entry[1] = 0;
   									//copy[0].price_ = self->litre_price;
   									//copy[1].price_ = litre_price2;
   									break;

   							  case set_no_flow_time:
   									self->fxn = set_no_flow_time;
   									clear_buffer();
   									clr_screen();
   									break;

   							  case setmax_amt:
   								   self->fxn = setmax_amt;
   								   clear_buffer();
   								   clr_screen1();
   								   break;

   							  case set_password:
   									self->attempts = 0;
   									self->fxn = set_password;
   									clear_buffer();
   									clr_screen();
   									break;

//   							  case calibrate_:
   							  case 8 :
   								   self->fxn = calibrate_;
   								   clr_screen1();
   								   clear_buffer1();
   								   break;
//   							  case display_:
   							  case 9 :
   								   self->fxn = display_;
   								   clear_buffer1();
   								   clr_screen1();
   								   dp_ = 1;
   								   ln_ = 1;
   								   break;

//   							  case setclock:
   							  case 10 :
   									self->fxn = setclock;
   									clear_buffer1();
   									clr_screen1();
   									break;

   							//"Sides No"
   							  case 11 :
									self->fxn = number_of_sides;
									clear_buffer1();
									clr_screen1();
									break;

							  //"Start CL"
   							  case 12:
									self->fxn = suppressed_display_vol;
									clear_buffer1();
									clr_screen1();
									break;

							  //"Calib.Can"
   							  case 13:
									self->fxn = calibration_can;
									clear_buffer1();
									clr_screen1();
									break;

							  //"Shift Type"
   							  case 14:
									self->fxn = shift_login_type_;
									clear_buffer1();
									clr_screen1();
									break;

							  //"Shift No"
							  case 15:
									self->fxn = number_of_shifts_;
									clear_buffer1();
									clr_screen1();
									break;

							  //Key Tone
   							  case 16:
									self->fxn = keypress_tone_;
									clear_buffer1();
									clr_screen1();
									break;

							  case 17:
									self->fxn = pulser_config;
									clear_buffer1();
									clr_screen1();
									break;

							  //"valve_config"
							  case 18:
									self->fxn = valve_config;
									clear_buffer1();
									clr_screen1();
									break;

							  //"no_calibration"
							  case 19:
									self->fxn = no_calibration;
									clear_buffer1();
									clr_screen1();
									break;

							  //Calib.Typ
							  case 20:
									self->fxn = calibration_type_;
									clear_buffer1();
									clr_screen1();
									break;

							  //"Com Card"
							  case 21:
									self->fxn = communication_card;
									clear_buffer1();
									clr_screen1();
									break;
   						}
   				}
   		 }
   	 return prog_State;
  }
          //      end of show menu

 /*   //check keypad for pressed xter
  	 if ( keypress__ > 0)  //if any key was pressed.
  	  {
  		keypress__ = 0;

  		if (kkey == 'B')  // up key
  			{
  			 if (index2 < 3-1)	index2++;
  			}

  		if (kkey == 'C')  // down key
  			{
  				if (index2 > 0)   index2--;
  			}
  	   }
  	 //-----------------------------------------------------------

  	   if (self->t >= 500)
  		 {
  			lcd_print_line1(menu1[index2]);
  			self->t = 0;
  		 }                                     */


  //================================= MODE ==================================
  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  //=========================================================================
  //            SET MODE { ( ONLINE (AUTO) / OFFLINE (MANUAL) }
  //=========================================================================
   else if ( (self->auth == authed) && ( (self->access == level2)||(self->access == level3) ) && (self->fxn == setmode)  )
   {
	   if (self->t >= 300)
		 {
			//lcd_print_line1();
//			if(copy[0].mode == AUTO)
			   if(self->copy_stream1[0].mode == AUTO_MODE)
				{
					lcd_print_line2(" Auto ");
				}
			   else
				{
					lcd_print_line2("NNanual ");
				}

//			 if (self->pump_indx == 1)
//				 {
					//snprintf(line3, sizeof(line3), "an1.%d",self->log_indx_indx + 1);
					lcd_print_line3("  A  ");
//				 }
//			 if (self->pump_indx == 2)
//				 {
//					//snprintf(line3, sizeof(line3), "an1.%d",self->log_indx_indx + 1);
//					lcd_print_line3("  B  ");
//				 }
					self->t = 0;
		 }
	// -------------- test keys....----------------
	rd19 = readkey19_state();

	if ( (rd19 == 1) && ( self->key19_sto_ == 0) )
	 {
		self->pump_indx++;
		if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
		   self->key19_sto_ = rd19;
	 }
	self->key19_sto_ = rd19;

	 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
				{
				  if (self->copy_stream1[0].mode == AUTO_MODE) //;
					  self->copy_stream1[0].mode = MANUAL_MODE;
				  else
					  self->copy_stream1[0].mode = AUTO_MODE;
				}

			 else if (pkey == 'C')  // down key
				{
					if (self->copy_stream1[0].mode == AUTO_MODE) //;
						self->copy_stream1[0].mode = MANUAL_MODE;
					  else
						  self->copy_stream1[0].mode = AUTO_MODE;
				}

			 else if (pkey == 'F')  //change pump index.
				{
				  // self->pump_indx++;
					//if (self->pump_indx > 2) self->pump_indx = 1; //wrap around
				}

			 else if (pkey == 'D')  // back key
			 {
				  if (self->copy_stream1[0].mode == AUTO_MODE) //;
					  self->copy_stream1[0].def_t = P;
				  else if (self->copy_stream1[0].mode == MANUAL_MODE)
					  self->copy_stream1[0].def_t = P;
//				  else if (copy[0].mode == VOUCHER_ONLY)
//					  copy[0].def_t = V;

				self->fxn = nothing;
 //				lcd_print_line3("      ");
				clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
				{
					self->fxn = nothing;
//					lcd_print_line3("      ");
					clr_screen1();
				}
		 }
	 return prog_State;
   }

  //=================================== MODE =================================
  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  //==========================================================================
  //                			   SET MODE-OVERIDE
  //==========================================================================
   else if ( (self->auth == authed) && ( (self->access == level2)||(self->access == level3) ) && (self->fxn == setnozzle)  )
   {
	   if (self->t >= 300)
	   {
			//lcd_print_line1();
		  if(self->copy_stream1[0].noz_override == override)
		  {
			lcd_print_line2("Active");
		  }
		  else
		  {
			lcd_print_line2("Inactive");
		  }

//		 if (self->pump_indx == 1)
//		 {
			//snprintf(line3, sizeof(line3), "an1.%d",self->log_indx_indx + 1);
			lcd_print_line3("  A  ");
//		 }
//		 if (self->pump_indx == 2)
//		 {
//			//snprintf(line3, sizeof(line3), "an1.%d",self->log_indx_indx + 1);
//			lcd_print_line3("  B  ");
//		 }
			self->t = 0;
	   }
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
		 {
			self->pump_indx++;
			if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
			   self->key19_sto_ = rd19;
		 }
		 self->key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
				{
				 if (self->copy_stream1[0].noz_override == override)
					 self->copy_stream1[0].noz_override = nooverride;
				 else
					 self->copy_stream1[0].noz_override = override;
				}

			 else if (pkey == 'C')  // down key
				{
				 if (self->copy_stream1[0].noz_override == override)
					 self->copy_stream1[0].noz_override = nooverride;
				 else
					 self->copy_stream1[0].noz_override = override;
				}

			 else if (pkey == 'F')  //change pump index.
			 {
			  // self->pump_indx++;
			  // if (self->pump_indx > 2) self->pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
				 self->fxn = nothing;
 				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
				{
					 self->fxn = nothing;
					 clr_screen1();
				}
		 }
	 return prog_State;
   }

  //================================ MODE ================================
  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  //======================================================================
  //                      	  DISPLAY ORIENTATION
  //======================================================================
   else if ( (self->auth == authed) && ( (self->access == level2)||(self->access == level3) ) && (self->fxn == display_orientation)  )
   {

	   if (self->t >= 300)
	   {
//		   if( (self->index_generic !=  PL) && (self->index_generic !=  LP) ) self->index_generic = PL;

		   if(self->copy_stream1[0].display_format == PL)
//		   if(self->index_generic ==  PL)
		   {
			   lcd_print_line1("PL NNode");
		   }
		   else
		   {
			   lcd_print_line1("LP NNode");
		   }

		  lcd_print_line3("  A  ");

		  self->t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1) && ( self->key19_sto_ == 0) )
			 {
				self->pump_indx++;
				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
				   self->key19_sto_ = rd19;
			 }
		 self->key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
			 {
//				 if (self->index_generic == PL)  self->index_generic = LP;
//				 else if(self->index_generic == LP) self->index_generic = PL;
				 if(self->copy_stream1[0].display_format == PL)
					 self->copy_stream1[0].display_format = LP;
				 else if(self->copy_stream1[0].display_format == LP)
					 self->copy_stream1[0].display_format = PL;
			 }

			 else if (pkey == 'C')  // down key
			 {
//				 if (self->index_generic == LP)  self->index_generic = PL;
//				 else if(self->index_generic == PL) self->index_generic = LP;
				 if(self->copy_stream1[0].display_format == PL)
					 self->copy_stream1[0].display_format = LP;
				 else if(self->copy_stream1[0].display_format == LP)
					 self->copy_stream1[0].display_format = PL;
			 }

			 else if (pkey == 'F')  //change pump index.
			 {
				  // self->pump_indx++;
				  //if (self->pump_indx > 2) self->pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
//				 if(self->index_generic == PL)
//				 {
//					 copy_stream1[0].display_format = PL;
//					 copy_stream1[0].def_t = P;
//				 }
//				 else if(self->index_generic == LP)
//				 {
//					 copy_stream1[0].display_format = LP;
//					 copy_stream1[0].def_t = L;
//				 }

				 if(self->copy_stream1[0].display_format == PL)
					 self->copy_stream1[0].def_t = P;
				 else if(self->copy_stream1[0].display_format == LP)
					 self->copy_stream1[0].def_t = L;

				 self->fxn = nothing;
				 self->index_generic = 0;
				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
			 {
				 self->fxn = nothing;
				 self->index_generic = 0;
				 clr_screen1();
			 }
		 }
	 return prog_State;
   }

	//============================== MODE =================================
	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	//=====================================================================
	//               			SET LITRE-PRICE
	//=====================================================================
   else if ( (self->auth == authed) && ( (self->access == level1)||(self->access == level2)||(self->access == level3) ) && (self->fxn == setprice) )
   {
	 if (self->t >= 300)
	  {
		 //copy[0].price_ =  atoi(self->keyboard_entry);
		 printDisp_f(self->copy_stream1[0].price_, 1, 0, 6, RT, CLEAR);
		 float pr = strtof(self->keyboard_entry, &endPtr);
		 printDisp_f(pr, 2, 0, 7, RT, CLEAR);

//		 if (self->pump_indx == 1)
//		 {
		   lcd_print_line3("  A  ");
//		 }

//		 if (self->pump_indx == 2)
//		 {
//			 lcd_print_line3("  B  ");
//		 }
		   self->t = 0;
	  }
	 // -------------- test keys....----------------
	  rd19 = readkey19_state();

	   if ( (rd19 == 1) && ( self->key19_sto_ == 0) )
		 {
		   self->keyboard_entry[0] = 0;   //clear the previous entry...
		   index_ = 0;
		   self->pump_indx++;
			if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
			   self->key19_sto_ = rd19;
		 }
		   self->key19_sto_ = rd19;


		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
				{

				}

			 if (pkey == 'C')  // down key
				{

				}

			 if (pkey == 'D')  //store entered value.
			 {
//				 if (self->pump_indx == 1)   // if side A
//				 {
				 self->copy_stream1[0].price_   = strtof(self->keyboard_entry, &endPtr);
//				 }
//				 if (self->pump_indx == 2)   // if side B
//				 {
//					 copy_stream1[1].price_   = strtof(self->keyboard_entry, &endPtr);
//				  }

				 self->fxn = nothing;
				 clr_screen1();
			 }

			 if (pkey == 'A')  // back key
				{
					 self->fxn = nothing;
					 clr_screen1();
				}
		 }
	 return prog_State;
   }

	//=========================== NOZZLE ADDRESS ==========================
	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	//=====================================================================
	//                              ID SETTING
	//=====================================================================
   else if ( (self->auth == authed) && ( (self->access == level2)||(self->access == level3) ) && (self->fxn == setaddress)  )
   {
	   if (self->t >= 500)
	   {
			printDisp_i(self->copy_stream1[0].noz_addr, 1, 0, 4, RT, CLEAR);
			lcd_print_line2(self->keyboard_entry);

//			if (self->pump_indx == 1)
//			 {
				lcd_print_line3("  A  ");
//			 }

//			if (self->pump_indx == 2)
//			 {
//				lcd_print_line3("  B  ");
//			 }
				self->t = 0;
		}
	 // -------------- test keys....----------------
		  rd19 = readkey19_state();

		   if ( (rd19 == 1) && ( self->key19_sto_ == 0) )
			 {
			   self->keyboard_entry[0] = 0;   //clear the previous entry...
				index_ = 0;
				self->pump_indx++;
				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
				   self->key19_sto_ = rd19;
			 }
			   self->key19_sto_ = rd19;

			pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
			 {

			 }

			 else if (pkey == 'C')  // down key
			 {

			 }

			 else if (pkey == 'D')  //enter key
			 {
//					if (self->pump_indx == 1)   // if side A
//					{
				 self->copy_stream1[0].noz_addr =  atoi(self->keyboard_entry);
//					}
//					else if (self->pump_indx == 2)   // if side b
//					{
				 self->copy_stream1[1].noz_addr =  atoi(self->keyboard_entry);
//					}

					self->fxn = nothing;
					clr_screen1();
			}

			 else if (pkey == 'A')  // back key
			 {
					 self->fxn = nothing;
					 clr_screen1();
			 }
		 }
	 return prog_State;
   }

//============================ NO-FLOW TIMEOUT =============================
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//==========================================================================
//            SET NO FLOW TIME (10 - 120 sec) ==> Default : 30 sec
//==========================================================================
   if ( (self->auth == authed) &&  ( (self->access == level2)||(self->access == level3) ) && (self->fxn == set_no_flow_time)  )
   {
	   if (self->t >= 500)
		 {
		   printDisp_i(self->self->copy_stream2[0].timeOut_noFlow, 1, 0, 4, RT, CLEAR);
		   lcd_print_line2(self->keyboard_entry);

//		 if (self->pump_indx == 1)
//		 {
			//snprintf(line3, sizeof(line3), "an1.%d",self->log_indx_indx + 1);
			lcd_print_line3("  A  ");
//		 }

//		 else if (self->pump_indx == 2)
//		 {
//			//snprintf(line3, sizeof(line3), "an1.%d",self->log_indx_indx + 1);
//			lcd_print_line3("  B  ");
//		 }
		self->t = 0;
 }
	 // -------------- test keys....----------------
		  rd19 = readkey19_state();

		   if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
		   {
			   self->keyboard_entry[0] = 0;   //clear the previous entry...
				   index_ = 0;
				self->pump_indx++;
				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
				   self->key19_sto_ = rd19;
		   }
		   self->key19_sto_ = rd19;

	 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
				{

				}

			 else if (pkey == 'C')  // down key
				{

				}

			 else if (pkey == 'D')  //enter key
				{
//					 if (self->pump_indx == 1)   // if side A
//						{
				 self->self->copy_stream2[0].timeOut_noFlow =  atoi(self->keyboard_entry);
//						}
//					 else if (self->pump_indx == 2)   // if side b
//						{
//						 self->copy_stream2[1].timeOut_noFlow =  atoi(self->keyboard_entry);
//						}

					 self->fxn = nothing;
					 clr_screen1();
				}

			 else if (pkey == 'A')  // back key
				{
					 self->fxn = nothing;
					 clr_screen1();
				}
		 }
	 return prog_State;
}

  //============================ MODE ===================================
  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  //=====================================================================
  //                         SET MAX AMOUNT
  //=====================================================================
   else if ( (self->auth == authed) &&  ( (self->access == level2)||(self->access == level3) ) && (self->fxn == setmax_amt)  )
   {
	   if (self->t >= 500)
	   {
		   printDisp_i(self->copy_stream1[0].max_amt_, 1, 0, 4, RT, CLEAR);
		   lcd_print_line2(self->keyboard_entry);

//		 if (self->pump_indx == 1)
//		 {
			lcd_print_line3("  A  ");
//		 }
//
//		 else if (self->pump_indx == 2)
//		 {
//			lcd_print_line3("  B  ");
//		 }
			self->t = 0;
	  }
	 // -------------- test keys....----------------
		  rd19 = readkey19_state();

		   if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
			 {
			   self->keyboard_entry[0] = 0;   //clear the previous entry...
			   index_ = 0;
				self->pump_indx++;
				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
				   self->key19_sto_ = rd19;
			 }
			   self->key19_sto_ = rd19;

	   pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
				{

				}

			 else if (pkey == 'C')  // down key
				{

				}

			 else if (pkey == 'D')  //enter key
			 {
//					 if (self->pump_indx == 1)   // if side A
//						{
				 self->copy_stream1[0].max_amt_ =  atoi(self->keyboard_entry);
//						}
//					 if (self->pump_indx == 2)   // if side b
//						{
//						 copy_stream1[1].max_amt_ =  atoi(self->keyboard_entry);
//						}

					 self->fxn = nothing;
					 clr_screen1();
				}

			 else if (pkey == 'A')  // back key
				{
					 self->fxn = nothing;
					 clr_screen1();
				}
		 }
	 return prog_State;
   }

 //=============================== MODE =================================
 //======================================================================
 //                         	CLEAR LOG
 //======================================================================
   else if ( (self->auth == authed) && (self->access == level1)&&(self->fxn == clr_log_)  )
   {
	   if (self->t >= 500)
		 {
		   //printDisp_i(copy[0].max_amt_, 1, 0, 4, RT, CLEAR);
//		   lcd_print_line2(self->keyboard_entry);
//
//		 if (self->pump_indx == 1)
//		 {
			lcd_print_line3("  A  ");
//		 }
//
//		 if (self->pump_indx == 2)
//		 {
//			lcd_print_line3("  b  ");
//		 }
			self->t = 0;
 }
	 // -------------- test keys....----------------
//		 int rd19 = readkey19_state();
//
//		   if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
//			 {
//			   self->keyboard_entry[0] = 0;   //clear the previous entry...
//				   index_ = 0;
//				self->pump_indx++;
//				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
//				   self->key19_sto_ = rd19;
//			 }
//			   self->key19_sto_ = rd19;

	 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
//			 if (pkey == 'B')  // up key
//				{
//
//				}
//
//			 if (pkey == 'C')  // down key
//				{
//
//				}
//
//			 if (pkey == 'D')  //enter key
//				{
//					 if (self->pump_indx == 1)   // if side A
//						{
//						  //clear side A log
//						  clear_logA();
//						  lcd_print_line1("        ");
//						  lcd_print_line2("cleared ");
//						  HAL_Delay(2000);
//						  clr_screen1(); //lcd_print_line2("        ");
//						  self->fxn = nothing;
//						}
//
//					 if (self->pump_indx == 2)   // if side b
//						{
//						 //clear side B log
//						 clear_logB();
//						 lcd_print_line1("        ");
//						 lcd_print_line2("cleared ");
//						 HAL_Delay(2000);
//						 clr_screen1(); //lcd_print_line2("        ");
//						 self->fxn = nothing;
//						}
//				}

			 if (pkey == 'A')  // back key
				{
				 self->keyboard_entry[0] = 0;   //clear the previous entry...
					 index_ = 0;
					 self->fxn = nothing;
					 clr_screen1();
				}
		 }
	 return prog_State;
   }

   //================================= MODE ===============================
   //======================================================================
   //                         	CLEAR TOTALIZER
   //======================================================================
   else if ( (self->auth == authed) && (self->access == level1)&&(self->fxn == clr_tot_)  )
    {
 	   if (self->t >= 500)
 		 {
 		   //printDisp_i(copy[0].max_amt_, 1, 0, 4, RT, CLEAR);
// 		   lcd_print_line2(self->keyboard_entry);
//
// 		 if (self->pump_indx == 1)
// 		 {
 			lcd_print_line3("  A  ");
// 		 }
//
// 		 if (self->pump_indx == 2)
// 		 {
// 			lcd_print_line3("  b  ");
// 		 }
		self->t = 0;
  }
 	 // -------------- test keys....----------------
// 		 int rd19 = readkey19_state();
//
// 		   if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
// 			 {
// 			   self->keyboard_entry[0] = 0;   //clear the previous entry...
// 				   index_ = 0;
// 				self->pump_indx++;
// 				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
// 				   self->key19_sto_ = rd19;
// 			 }
// 			   self->key19_sto_ = rd19;

 	 pkey = read_keypad();

 		 if( (pkey > 0) )  //key detected
 		 {
// 			 if (pkey == 'B')  // up key
// 				{
//
// 				}
//
// 			 if (pkey == 'C')  // down key
// 				{
//
// 				}
//
// 			 if (pkey == 'D')  //enter key
// 				{
// 					 if (self->pump_indx == 1)   // if side A
// 						{
// 						  //clear side A log
// 						  clear_totA();
// 						  lcd_print_line1("        ");
// 						  lcd_print_line2("cleared ");
// 						 HAL_Delay(2000);
// 						  self->fxn = nothing;
// 						 clr_screen1();
// 						}
//
// 					 if (self->pump_indx == 2)   // if side b
// 						{
// 						 //clear side B log
// 						 clear_totB();
// 						 lcd_print_line1("        ");
// 						 lcd_print_line2("cleared ");
// 						 HAL_Delay(2000);
// 						 self->fxn = nothing;
// 						 clr_screen1();
// 						}
// 				}

 			 if (pkey == 'A')  // back key
 			 {
 				self->keyboard_entry[0] = 0;   //clear the previous entry...
 	 				 index_ = 0;
 	 				 self->fxn = nothing;
 					 clr_screen1();
 			}
 		 }
 	 return prog_State;
    }

 //============================== MODE =================================
 //=====================================================================
 //                             SEND CONFIG.
 //=====================================================================
   else if ( (self->auth == authed) && (self->access == level1) && (self->fxn == send_config)  )
   {
	   if (self->t >= 500)
		 {
		   //printDisp_i(copy[0].max_amt_, 1, 0, 4, RT, CLEAR);
		  // lcd_print_line2(self->keyboard_entry);
/*
		 if (self->pump_indx == 1)
		 {
			//snprintf(line3, sizeof(line3), "an1.%d",self->log_indx_indx + 1);
			lcd_print_line3("  a  ");
		 }

		 if (self->pump_indx == 2)
		 {
			//snprintf(line3, sizeof(line3), "an1.%d",self->log_indx_indx + 1);
			lcd_print_line3("  b  ");
		 }  */

		   self->t = 0;
 }
	 // -------------- test keys....----------------
		 rd19 = readkey19_state();

		   if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
			 {
				self->pump_indx++;
				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
				   self->key19_sto_ = rd19;
			 }
			   self->key19_sto_ = rd19;

	 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
				{

				}

			 if (pkey == 'C')  // down key
				{

				}

			 if (pkey == 'D')  //enter key
				{
					  for (uint8_t i = 0; i < 14; i++)
					  {
						//   send the bits to the external system

					  }

					  for (uint8_t i = 0; i < 14; i++)
					  {
						  //   send the bits to the external system
					  }

					  lcd_print_line2("Sent    ");
					  self->fxn = nothing;
				}

			 if (pkey == 'A')  // back key
				{
					 self->fxn = nothing;
					 clr_screen1();
				}
		 }
	 return prog_State;
   }


  //============================ MODE ===================================
  //=====================================================================
  //                          GET CONFIG.
  //=====================================================================
   else if ( (self->auth == authed) && (self->access == level1)&&(self->fxn == get_config)  )
    {
 	   if (self->t >= 3000)
 		 {

 		  lcd_print_line3("      ");
 		  lcd_print_line2(" Done ");
 		  self->fxn = nothing;
 		 self->t = 0;
    }

  //-------------------------------------------------
  // Receive the bytes sent from the external system
  //       and store in the settings array.





  //-------------------------------------------------
  // -------------- test keys....--------------------
 		  rd19 = readkey19_state();

 		   if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
 			 {
 				self->pump_indx++;
 				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
 				   self->key19_sto_ = rd19;
 			 }
 			   self->key19_sto_ = rd19;

 	          pkey = read_keypad();

			   if( (pkey > 0) )  //key detected
				 {
					 if (pkey == 'A')  // back key
						{
							 self->fxn = nothing;
							 clr_screen1();
						}
				 }
 	 return prog_State;
    }

  //============================ MODE ==================================
  //====================================================================
  //                         SET PASSWORD
  //====================================================================
   else if ( (self->auth == authed) &&  ( (self->access == level2)||(self->access == level3) ) && (self->fxn == set_password)  )
   {
	   static uint8_t self->chg_pw = 0;
	   static char pw1[9] = { 0 };
//	   static char pw2[9] = { 0 };
//	   static char pw3[9] = { 0 };

//	   static uint8_t index_pass = 0;


	   if(self->prog_revisit == 1)
	   {
		   self->chg_pw = 0;
		   memset(pw1, '\0', sizeof(pw1));
//		   memset(pw2, '\0', sizeof(pw2));
//		   memset(pw3, '\0', sizeof(pw3));

		   self->prog_revisit = 0;
	   }

 	   if (self->t >= 500)
 		 {
 		   if(self->chg_pw == 0)
 		   {
 			   if (self->access == level2)
 			   {
 				   if(self->index_generic == 2) self->index_generic = 0;
 			   }
 			   else if (self->access == level3)
			   {
 				   if(self->index_generic == 3) self->index_generic = 0;
 			   }

 			  lcd_print_line1(self->keypad_pw_xter); //self->keyboard_entry);
			   lcd_print_line2("  Enter ");
			   lcd_print_line3(ch_pass[self->index_generic]);  //show password level menu
 		   }
 		   else
 		   {
 			  lcd_print_line1(self->keypad_pw_xter); //self->keyboard_entry);
 			    lcd_print_line2("Re-enter");
				lcd_print_line3(" Pass");
 		   }
// 		   if (self->pump_indx == 1)
// 		  		 {
// 		  	//		lcd_print_line3("  a  ");
// 		  		 }
//
// 		  		 if (self->pump_indx == 2)
// 		  		 {
// 		  	//		lcd_print_line3("  b  ");
// 		  		 }
 	    	self->t = 0;
         }
 	 // -------------- test keys....----------------
 		 rd19 = readkey19_state();

 		   if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
 			 {
 				self->pump_indx++;
 				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
 				   self->key19_sto_ = rd19;
 			 }
 			   self->key19_sto_ = rd19;

 	    pkey = read_keypad();

 		 if( (pkey > 0) )  //key detected
 		 {
 			 if (pkey == 'B')  // up key
			 {
 				if (self->access == level2)
			    {
 					if (self->index_generic <= 1)  self->index_generic++;
 					else if(self->index_generic == 2) self->index_generic = 0;
			    }
 				else if (self->access == level3)
  			    {
 					if (self->index_generic <= 2)  self->index_generic++;
					else if(self->index_generic == 3) self->index_generic = 0;
  			    }
			 }

 			 else if (pkey == 'C')  // down key
			 {
 				if (self->access == level2)
 				{
 					if (self->index_generic > 0) self->index_generic--;
 					else if(self->index_generic == 0) self->index_generic = 1;
 				}
 				else if (self->access == level3)
 				{
 					if (self->index_generic > 0) self->index_generic--;
 					else if(self->index_generic == 0) self->index_generic = 2;
 				}
			 }

 			 else if (pkey == 'D')  //enter key
 			 {
 				 if (self->chg_pw == 0)
				  {
//					 pw1 = atoi(self->keyboard_entry);
					 strncpy(pw1, self->keyboard_entry, sizeof(pw1));
					 self->chg_pw = 1;
					 lcd_print_line2("Re-enter");
					 lcd_print_line3(" Pass");
//					 HAL_Delay(2500);
					 lcd_print_line1("        ");
					 clear_buffer1();
					 //index_ = 0;
					 //self->keyboard_entry[0] = 0;
					 //self->keyboard_entry[1] = 0;
					 //self->keypad_pw_xter[0] = 0;
					 //_index = 0;
					 return prog_State;
				  }
 				  else if (self->chg_pw == 1)
				  {
						 if (strcmp(pw1, self->keyboard_entry) == 0)  //if strings are same
						 {
							 lcd_print_line1("        ");
							 lcd_print_line2("Success ");
							 lcd_print_line3("        ");
							 lcd_print_line1("        ");

							 if(self->index_generic == 0)
							 {
								 strncpy(self->copy_stream3[0].passwd1, pw1, sizeof(pw1));
								 strncpy(self->copy_stream3[1].passwd1, pw1, sizeof(pw1));
							 }
							 else if(self->index_generic == 1)
							 {
								 strncpy(self->copy_stream3[0].passwd2, pw1, sizeof(pw1));
								 strncpy(self->copy_stream3[1].passwd2, pw1, sizeof(pw1));
							 }
							 else if(self->index_generic == 2)
							 {
								 strncpy(self->copy_stream3[0].passwd3, pw1, sizeof(pw1));
								 strncpy(self->copy_stream3[1].passwd3, pw1, sizeof(pw1));
							 }

							 HAL_Delay(1000);
							 lcd_print_line1("Passuurd ");
							 lcd_print_line2("Changed ");
							 HAL_Delay(1000);
							 self->fxn = nothing;
							 self->chg_pw = 0;
							 clear_buffer1();
							 self->index_generic = 0;

							return prog_State;

						 }
						 else
						 {

							    lcd_print_line2("---no---");
							    HAL_Delay(1500);
							    lcd_print_line2("        ");
							    self->attempts++;
							    if (self->attempts >= 3)
							    {
							    	lcd_print_line2("        ");
							    	lcd_print_line2(" Denied ");
							    	HAL_Delay(1500);
							    	lcd_print_line2("        ");
							    	 self->fxn = nothing;
							    	 self->chg_pw = 1;
									clear_buffer1();
//									self->index_generic = 0;
							    	// index_ = 0;
									//self->keyboard_entry[0] = 0;
									//self->keypad_pw_xter[0] = 0;
									//_index = 0;
							    	 return prog_State;
							    }
							    else
							    {
							    	lcd_print_line2("        ");
									lcd_print_line2(" Retry  ");
									HAL_Delay(1500);
									lcd_print_line2("        ");
									clear_buffer1();
//									self->index_generic = 0;
									// index_ = 0;
									//self->keyboard_entry[0] = 0;
									//self->keypad_pw_xter[0] = 0;
									//_index = 0;
									return prog_State;
							    }
						 }
						 self->chg_pw = 1;
					  }

// 				 	 self->fxn = nothing;
 				}

 			 if (pkey == 'A')  // back key
			 {
 				self->chg_pw = 0;
 				self->fxn = nothing;
				clr_screen1();
				self->index_generic = 0;
			 }
 		 }
 	 return prog_State;
    }


    //============================== MODE ================================
    //====================================================================
    //                             CALIBRATE
    //====================================================================
   else if ( (self->auth == authed) &&  ( (self->access == level2)||(self->access == level3) ) && (self->fxn == calibrate_)  )
   {

       static float calib_price = 0.0;
	   static float inv_price   =  0.0;
	   static int vol_sold = 1;

       if(self->prog_revisit == 1)
  	   {
  		   calib_price = 0.0;
		   inv_price = 0.0;
		   vol_sold = 1;
		   volume_flag = 0;
		   self->calibr = 0;        // notify the interrupt routine to off the pump
		   self->prog_revisit = 0;
  	   }

	   if (volume_flag == 0)
	    {
		   //select sales volume first...

//		       snprintf(st__, sizeof(st__), "%d litre", calibrationCan_measure1);

		   lcd_print_line1("  Start  ");

//		       if (self->t >= 500)
//			   {
////					if (vol_sold == l1)
////					{
////						lcd_print_line1("10 litre");
////					}
////					else
//						lcd_print_line1(st__);
//
//				   if (self->pump_indx == 1)
//					 {
//						lcd_print_line3("  A  ");
//					 }
//
////				   if (self->pump_indx == 2)
////					 {
////						lcd_print_line3("  B  ");
////					 }
//				   self->t = 0;
//				}
   	 // -------------- test keys....----------------
   		 rd19 = readkey19_state();

   		   if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
   			 {
   				self->pump_indx++;
   				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
   				   self->key19_sto_ = rd19;
   			 }
   			   self->key19_sto_ = rd19;

   	    pkey = read_keypad();

   		 if( (pkey > 0) )  //key detected
   		 {
//   			 if (pkey == 'B')  // up key
//   				{
//                   if (vol_sold == l1)
//                   {
//                	   vol_sold = l2;
//                   }
//                   else
//                	   vol_sold = l1;
//   				}
//
//   			 if (pkey == 'C')  // down key
//   				{
//   				 if (vol_sold == l1)
//					   {
//						   vol_sold = l2;
//					   }
//					   else
//						   vol_sold = l1;
//   				}

   			 if (pkey == 'D')  //enter key
   				{
   				 	 //enter pressed user has made a choice.
   				     // then dispense the selcted volume
					volume_flag  = 1;   //set flag and goto dispense
					self->calib_pulser = 0;  //clear pulser
		           __HAL_TIM_SET_COUNTER(&htim5, 0);   *** //clear harware pulser here...
					return prog_State;
   				}

   			 if (pkey == 'A')  // back key
   				{
   					 self->fxn = nothing;
   					self->calibr = 0;
   				     clr_screen1();
   				}
   		 }
      	 return prog_State;
	   }// if selected volume = 0.....
	//========= volume has been selected =============
    // dispense
      if(volume_flag == 1)
      {
    	  lcd_print_line1("Dispense");
         HAL_Delay(2000);
         volume_flag = 2;
         self->calibr = 1;        /// notify the interrupt routine to on the pump
         return prog_State;
      }
    //=============================
	  if(volume_flag == 2)  //dispense.
	   {
		 if(self->t > 60)
		  {

			#if _USE_SOFT_PULSER == 0
			 self->calib_pulser = __HAL_TIM_GET_COUNTER(self->flow_counter);    //(self->flow_counter);  *** //use hardware counter
			#else
			 self->calib_pulser++; 							// use software counter.
			#endif

			  lcd_print_line1(" Pulser ");
			  printDisp_i(self->calib_pulser, 2, 0, 7, RT, CLEAR);

		    //    power outage during self->filling  end transaction...
			#if sense_power == 1
			  if( (readpwr() == 0)||(read_p_pwr() == 0) )
			  {
				  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
				  HAL_Delay(100);
				  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
				  stop_flow1();

				  if(self->calibration_flag == CALIBRATED)
				  {
//					  pulser_totalizer1 = ( (self->calib_pulser / (float) (pulser_benchMark1)) * calibrationCan_measure1 );
//					  self->totaliser_vol_cal += pulser_totalizer1;
//					  totaliser_vol1 += pulser_totalizer1;
//					  self->totaliser_amt_cal += (pulser_totalizer1 * self->litre_price);
//					  self->totaliser_amt += (pulser_totalizer1 * self->litre_price);
//					  self->amt_real = pulser_totalizer1;

//					  self->amt_middle = pulser_totalizer1;
//					  self->price_real = (pulser_totalizer1 * self->litre_price);
//					  self->price_upper = (pulser_totalizer1 * self->litre_price)


					  self->amt_middle = self->calibrationCan_measure;
					  self->price_real = (self->calibrationCan_measure * self->litre_price);
					  self->price_upper = (self->calibrationCan_measure * self->litre_price);
					  self->amt_real = self->calibrationCan_measure;

					  self->totaliser_vol_cal += calibrationCan_measure1;
					  self->totaliser_vol += calibrationCan_measure1;
					  self->totaliser_amt_cal += self->price_real;
					  self->totaliser_amt += self->price_real;

					  while(retrieve_totaliser_fram_check(operating_side) != OK)   //If it fails, retry 5X
					  {
						  static uint8_t try = 0;
							if(try++ >= 5)
							{
								while(retrieve_totaliser_eeprom_check(operating_side) != OK)
								{
									if(try++ >= 10)
									{
										clear_totaliser_fram(operating_side);
										clear_totaliser_eeprom(operating_side);
										try = 0;
										break;
									}

								}
								try = 0;
								break;
							}
					  }

					  save_totaliser_fram(operating_side);
					  save_totaliser_eeprom(operating_side);

					  save_lastSale_fram(operating_side);
					  save_lastSale_eeprom(operating_side);

					  ////////////////////////////////////////////////
					  //---------------------------------------------
					  self->totalizer_saveStatus = SAVED_TO_MAIN_TOTALIZER;
					  save_totaliserFrequent_fram(operating_side);
					  save_totaliserFrequent_eeprom(operating_side);
					  ////////////////////////////////////////////////

//					  pwr1 = POWERINTERRUPTION;
//					  save_calibrationData(side_a);
				  }
				  else if(self->calibration_flag == UNCALIBRATED)
				  {
//					  calibrationData[0].pulser_value = self->calib_pulser;

//					  pulser_benchMark1 = 0;
//					  pwr1 = POWERINTERRUPTION;
//					  save_calibrationData_fram(side_a);

					  self->amt_middle = calibrationCan_measure1;
					  self->price_real = (calibrationCan_measure1 * self->litre_price);
					  self->price_upper = (calibrationCan_measure1 * self->litre_price);
					  self->amt_real = calibrationCan_measure1;

					  self->totaliser_vol_cal += calibrationCan_measure1;
					  self->totaliser_vol += calibrationCan_measure1;
					  self->totaliser_amt_cal += self->price_real;
					  self->totaliser_amt += self->price_real;

					  while(retrieve_totaliser_fram_check(operating_side) != OK)   //If it fails, retry 5X
					  {
						  static uint8_t try = 0;
							if(try++ >= 5)
							{
								while(retrieve_totaliser_eeprom_check(operating_side) != OK)
								{
									if(try++ >= 10)
									{
										clear_totaliser_fram(operating_side);
										clear_totaliser_eeprom(operating_side);
										try = 0;
										break;
									}

								}
								try = 0;
								break;
							}
					  }

					  save_totaliser_fram(operating_side);
					  save_totaliser_eeprom(operating_side);

					  save_lastSale_fram(operating_side);
					  save_lastSale_eeprom(operating_side);

					  ////////////////////////////////////////////////
					  //---------------------------------------------
					  self->totalizer_saveStatus = SAVED_TO_MAIN_TOTALIZER;
					  save_totaliserFrequent_fram(operating_side);
					  save_totaliserFrequent_eeprom(operating_side);
					  ////////////////////////////////////////////////

				  }

				  return write_flash_State;
			  }
			#endif
			  self->t = 0;
		  }

		  if(self->settings_stream1[0].mode == AUTO_MODE)
			{
				if(timer_go >= TIMEOUT_GO)   //if go's timeout is 5sec threshold
				{
					self->go_timeOut = 1;

					HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
					HAL_Delay(100);
					HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
					stop_flow1();


					if(self->calibration_flag == CALIBRATED)
					{
						  self->amt_middle = calibrationCan_measure1;
						  self->price_real = (calibrationCan_measure1 * self->litre_price);
						  self->price_upper = (calibrationCan_measure1 * self->litre_price);
						  self->amt_real = calibrationCan_measure1;

						  self->totaliser_vol_cal += calibrationCan_measure1;
						  self->totaliser_vol += calibrationCan_measure1;
						  self->totaliser_amt_cal += self->price_real;
						  self->totaliser_amt += self->price_real;

						  while(retrieve_totaliser_fram_check(operating_side) != OK)   //If it fails, retry 5X
						  {
							  static uint8_t try = 0;
								if(try++ >= 5)
								{
									while(retrieve_totaliser_eeprom_check(operating_side) != OK)
									{
										if(try++ >= 10)
										{
											clear_totaliser_fram(operating_side);
											clear_totaliser_eeprom(operating_side);
											try = 0;
											break;
										}

									}
									try = 0;
									break;
								}
						  }

						  save_totaliser_fram(operating_side);
						  save_totaliser_eeprom(operating_side);

						  save_lastSale_fram(operating_side);
						  save_lastSale_eeprom(operating_side);

						  ////////////////////////////////////////////////
						  //---------------------------------------------
						  self->totalizer_saveStatus = SAVED_TO_MAIN_TOTALIZER;
						  save_totaliserFrequent_fram(operating_side);
						  save_totaliserFrequent_eeprom(operating_side);
						  ////////////////////////////////////////////////


					}
					else if(self->calibration_flag == UNCALIBRATED)
					{
						  self->amt_middle = calibrationCan_measure1;
						  self->price_real = (calibrationCan_measure1 * self->litre_price);
						  self->price_upper = (calibrationCan_measure1 * self->litre_price);
						  self->amt_real = calibrationCan_measure1;

						  self->totaliser_vol_cal += calibrationCan_measure1;
						  self->totaliser_vol += calibrationCan_measure1;
						  self->totaliser_amt_cal += self->price_real;
						  self->totaliser_amt += self->price_real;

						  while(retrieve_totaliser_fram_check(operating_side) != OK)   //If it fails, retry 5X
						  {
							  static uint8_t try = 0;
								if(try++ >= 5)
								{
									while(retrieve_totaliser_eeprom_check(operating_side) != OK)
									{
										if(try++ >= 10)
										{
											clear_totaliser_fram(operating_side);
											clear_totaliser_eeprom(operating_side);
											try = 0;
											break;
										}

									}
									try = 0;
									break;
								}
						  }

						  save_totaliser_fram(operating_side);
						  save_totaliser_eeprom(operating_side);

						  save_lastSale_fram(operating_side);
						  save_lastSale_eeprom(operating_side);

						  ////////////////////////////////////////////////
						  //---------------------------------------------
						  self->totalizer_saveStatus = SAVED_TO_MAIN_TOTALIZER;
						  save_totaliserFrequent_fram(operating_side);
						  save_totaliserFrequent_eeprom(operating_side);
						  ////////////////////////////////////////////////

					}

					return write_flash_State;
				}
			}


			  pkey = read_keypad();
//			  if (pkey == 'A')  //off key to stop fueling...  Stop/Exit Button
				if (pkey == 'D')  //Enter key to transit to the next page
				{
                    //capture the pulser.
				  calibr1 = 0;  /// notify the interrupt routine to on the pump

				  #if _USE_SOFT_PULSER == 0
				  	  self->calib_pulser = __HAL_TIM_GET_COUNTER(self->flow_counter);  //use hardware counter
				  #else
				  	  self->calib_pulser++; 							// use software counter.
				  #endif

//				  pulser_totalizer1 = ( (self->calib_pulser / (float) (pulser_benchMark1)) * calibrationCan_measure1 );
//				  pulser_totalizer1 = ( (self->calib_pulser / (float) (self->calib_pulser)) * calibrationCan_measure1 );
//
//				  pulser_benchMark1 = self->calib_pulser;
//
//				  self->amt_real = pulser_totalizer1;
//				  self->amt_middle = pulser_totalizer1;
//				  self->price_real = (pulser_totalizer1 * self->litre_price);
//				  self->price_upper = (pulser_totalizer1 * self->litre_price);
//
//				  self->totaliser_vol_cal += pulser_totalizer1;
//				  totaliser_vol1 += pulser_totalizer1;
//				  self->totaliser_amt_cal += self->price_real;
//				  self->totaliser_amt += self->price_real;
//
//				  save_totaliser_fram(operating_side);
////				  save_amountTotaliser_fram(operating_side);
//				  save_lastSale_fram(operating_side);
//				  save_calibrationData_fram(side_a);

				  self->amt_middle = calibrationCan_measure1;
				  self->price_real = (calibrationCan_measure1 * self->litre_price);
				  self->price_upper = (calibrationCan_measure1 * self->litre_price);
				  self->amt_real = calibrationCan_measure1;

				  self->totaliser_vol_cal += calibrationCan_measure1;
				  self->totaliser_vol += calibrationCan_measure1;
				  self->totaliser_amt_cal += self->price_real;
				  self->totaliser_amt += self->price_real;

				  while(retrieve_totaliser_fram_check(operating_side) != OK)   //If it fails, retry 5X
				  {
					  static uint8_t try = 0;
						if(try++ >= 5)
						{
							while(retrieve_totaliser_eeprom_check(operating_side) != OK)
							{
								if(try++ >= 10)
								{
									clear_totaliser_fram(operating_side);
									clear_totaliser_eeprom(operating_side);
									try = 0;
									break;
								}

							}
							try = 0;
							break;
						}
				  }

				  save_totaliser_fram(operating_side);
				  save_totaliser_eeprom(operating_side);

				  save_lastSale_fram(operating_side);
				  save_lastSale_eeprom(operating_side);

				  ////////////////////////////////////////////////
				  //---------------------------------------------
				  self->totalizer_saveStatus = SAVED_TO_MAIN_TOTALIZER;
				  save_totaliserFrequent_fram(operating_side);
				  save_totaliserFrequent_eeprom(operating_side);
				  ////////////////////////////////////////////////



  //				  self->calib_pulser = __HAL_TIM_GET_COUNTER(self->flow_counter);  //use hardware counter
				  lcd_print_line1("Set your");
				  lcd_print_line2("Volunne ");
				  HAL_Delay(2000);
				  volume_flag = 3;   //enter the self->price.


				  return prog_State;
				}

//			  if (pkey == 'A')  // back key
//				{
//                 	volume_flag = 0;
//                    calibr1 = 0;
//                   // clr_screen1();
//				}

		     return prog_State;
	   }
  //==========================================================

  //==========================================================
    if(volume_flag == 3)  //Enter the self->price.
	   {
			  if(self->t > 400)
			  {
				  lcd_print_line1("Volunne ");

				  float vol = strtof(self->keyboard_entry, &endPtr);
				  printDisp_f(vol, 2, 0, 7, RT, CLEAR);
				  self->t = 0;
			  }

			  pkey = read_keypad();
			  if (pkey == 'D')  //ENTER key to adjusted volume.
				{
				  float cal_vol = strtof(self->keyboard_entry, &endPtr);
				  lcd_print_line1("        ");   //clear screen.
				  lcd_print_line2("        ");
				  clear_buffer1();
					//index_ = 0;
					//self->keyboard_entry[0] = 0;  //clear buffer...
					//self->keypad_pw_xter[0] = 0;
					//_index = 0;
					//==================================
					// calculate the self->price of the pulses
					// float pop = self->calib_pulser  / cal_vol;
					int sold_v = 0;
					float pi_c;
					float pi;

//					if (vol_sold == l1)
//					{
//						sold_v = 10;
//						pi_c = self->calib_pulser / cal_vol; //calibrated pi.
//						pi   = self->calib_pulser / sold_v;
//					}
//					else
//					{
						sold_v = self->settings_stream2[0].calibration_measureCan;
						pi_c = self->calib_pulser / cal_vol; //calibrated pi.
						pi   = self->calib_pulser / sold_v;
//						pi   = self->calib_pulser / settings[0].calibration_measureCan;
//					}

					vol_real1 = sold_v;
					vol_calibrated1 = cal_vol;
					save_ctSettings_fram(side_a);
					save_calibrationPulser_fram(side_a);

					self->calibration_flag = CALIBRATED;
					save_calibrationFlag_fram(side_a);

                    HAL_Delay(1700);

                    lcd_print_line1("  Done  ");
                    printDisp_f(pi_c, 2, 0, 8, RT, CLEAR);
                    HAL_Delay(2500);

//                    //store in the settings structure.
//                    if(self->pump_indx == 1)
//                    {
                    	// data for side a.
                    	copy_stream1[0].pi_cal = pi_c;
                    	copy_stream1[0].pi_real  = pi;
//                    }

//                    if(self->pump_indx == 2)
//					{
//						// data for side b.
//                    	copy_stream1[1].pi_cal = pi_c;
//                    	copy_stream1[1].pi_real	 = pi;
//					}
					 self->fxn = nothing;
					 calibr1 = 0;
					 clr_screen1();
				}

			  if (pkey == 'A')  // back key
				{
					volume_flag = 0;
					clr_screen1();
				}
			 return prog_State;
	   }
    return prog_State;
     }

//=====================================================================

//============================================== MODE =========================================
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//=============================================================================================
//                      DISPLAY DECIMAL-PLACES (1, 2 or 3) ==> Default : 2
//=============================================================================================
   else if ( (self->auth == authed) &&  ( (self->access == level2)||(self->access == level3) ) && (self->fxn == display_)  )
   {

	 if(self->enter_key == 1)
	 {
		 if(timer_config1 < 1000)
		 {
			 lcd_print_line2("Entered");
		 }
		 else
		 {
			 self->enter_key = 0;
			 self->fxn = nothing;
			 clr_screen1();
		 }

		 return prog_State;
	 }

	 if (ln_ == 0) ln_ = 1;
	 if (dp_ == 0) dp_ = 1;

	   if (self->t >= 500)
		 {
		    if (ln_ == 1)
			 {
		    	lcd_print_line1("   P    ");
			 }

		    else if (ln_ == 2)
			 {
		    	lcd_print_line1("   L    ");
			 }

		    else if (ln_ == 3)
			 {
		    	lcd_print_line1(" Unit p ");
			 }
       //---------------------------------
		     if (dp_ == 1)
			 {
				lcd_print_line2(" 999999.9");
			 }

		    else if (dp_ == 2)
			 {
				lcd_print_line2(" 99999.99");
			 }

		    else if (dp_ == 3)
			 {
		    	lcd_print_line2(" 9999.999");
			 }
		    else if (dp_ == 4)
			 {
				lcd_print_line2(" 999.9999");
			 }
        //--------------------------------
			 if (self->pump_indx == 1)
			 {
				lcd_print_line3("  A  ");
			 }

			 else if (self->pump_indx == 2)
			 {
				lcd_print_line3("  B  ");
			 }
			self->t = 0;
        }
	 // -------------- test keys....----------------
		 rd19 = readkey19_state();

		   if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
			 {
			   self->keyboard_entry[0] = 0;   //clear the previous entry...
								   index_ = 0;
				self->pump_indx++;
				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
				   self->key19_sto_ = rd19;
			 }
			   self->key19_sto_ = rd19;

	   pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
				{
				 	 dp_++;
				 	 if(dp_ > max_dp) dp_ = 1;
				}

			 else if (pkey == 'C')  // down key
				{
				 	 ln_++;
				 	 if(ln_ > 3)ln_ = 1;
				}

			 else if (pkey == 'D')  //enter key
				{
					 if (self->pump_indx == 1)   // if side A
					 {
					   if(ln_ == 1) copy_stream1[0].dp_amount  = dp_;
					   if(ln_ == 2) copy_stream1[0].dp_vol = dp_;
					   if(ln_ == 3) copy_stream1[0].dp_unitprice = dp_;
					 }
					 else if (self->pump_indx == 2)   // if side b
					 {
					   if(ln_ == 1) copy_stream1[1].dp_amount  = dp_;
					   if(ln_ == 2) copy_stream1[1].dp_vol = dp_;
					   if(ln_ == 3) copy_stream1[1].dp_unitprice = dp_;
					 }

					 clr_screen1();
					 timer_config1 = 0;
					 lcd_print_line2("Entered ");

					 self->enter_key = 1;
					 return prog_State;
				}

			 else if (pkey == 'A')  // back key
				{
					 self->fxn = nothing;
					 clr_screen1();
					 return prog_State;
				}
		 }
	 return prog_State;
   }


 //============================= MODE ==================================
 //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 //=====================================================================
 //               			 CLOCK SETTING
 //=====================================================================
   	 else if ( (self->auth == authed) &&  ( (self->access == level1)||(self->access == level2)||(self->access == level3) ) && (self->fxn == setclock) )
	 {

		    static int8_t respons_dir = 0,
						   entry_clear = 1,
						   montth = 0,
						   failFlag_tm = 0,
						   failFlag_dt = 0;

			static bool hh_success = false,
						yy_success = false;

		 	if(self->prog_revisit == 1)
		    {
		 	   respons_dir = 0;
			   entry_clear = 1;
			   montth = 0;
			   failFlag_tm = 0;
			   failFlag_dt = 0;

		    	hh_success = false;
		    	yy_success = false;

		    	self->prog_revisit = 0;
		    }

		    int8_t respons = 0,
		    	   tm = 0,
		    	   dte = 0;

//		  char st__[10] = {0};

		  //====================================================
		  if( (self->t>200) && (respons_dir == 0) )
		  {
			  lcd_print_line1("1- Tinne");
			  lcd_print_line2("2-  Date");
			  self->t = 0;

		  }
		  if(entry_clear)
			  respons = atoi(self->keyboard_entry);

		 //------------------------------------------------
		  if( ((respons == 1) || (respons_dir == 1)) && (!failFlag_tm) )
		  {
			  if(entry_clear == 1)
			  {
				  respons = 0;
				  respons_dir = 1;
				  clear_buffer1();
				  entry_clear = 0;
			  }

			  if (!hh_success)
			  {
				  snprintf(st__, sizeof(st__), "  %02d", hour);
				  printDisp_c("hr",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
			  }
			  else if(hh_success)
			  {
				  snprintf(st__, sizeof(st__), "  %02d", minute);
				  printDisp_c("NNi",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
			  }

			  if( (atoi(self->keyboard_entry)) <= 59 )
				  tm = atoi(self->keyboard_entry);
			  else clear_buffer1();

			  printDisp_i(tm,2,4,8,RT,CLEAR);

			  //=======================================
			  if (clock_save)
			  {
				clock_save = false;

				DS1307_SetHour(ttime[0]);
				DS1307_SetMinute(ttime[1]);
				DS1307_SetSecond(00);
				HAL_Delay(15);

				clr_screen1();
				lcd_print_line2("  Saved");
				HAL_Delay(1500);

				hour = DS1307_GetHour();
				minute = DS1307_GetMinute();
				second = DS1307_GetSecond();

 				clear_buffer1();
 				entry_clear = 1;
 				respons_dir = 0;
			  }
		  }

		  else if( ((respons == 2) || (respons_dir == 2)) && (!failFlag_dt) )
		  {
			  if(entry_clear == 1)
			  {
				  respons = 0;
				  respons_dir = 2;
				  clear_buffer1();
				  entry_clear = 0;
			  }
			  if ( (!yy_success) && (!mth_success) )
			  {
				  snprintf(st__, sizeof(st__), "  %02d", year);
				  printDisp_c("Yr",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
			  }
			  else if( (yy_success) && (!mth_success) )
			  {
				  snprintf(st__, sizeof(st__), "  %02d", month);
				  printDisp_c("NNo",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
			  }
			  else if(mth_success)
			  {
				  snprintf(st__, sizeof(st__), "  %02d", day);
				  printDisp_c("dd",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
			  }

			  if( (atoi(self->keyboard_entry)) <= 99 )
				  dte = atoi(self->keyboard_entry);
			  else clear_buffer1();

			  printDisp_i(dte,2,4,8,RT,CLEAR);

			  //=======================================
			  if (clock_save)
			  {
				clock_save = false;

				DS1307_SetDate(ddate[0]);
				DS1307_SetMonth(ddate[1]);
				DS1307_SetYear(ddate[2]);
//							DS1307_SetDayOfWeek(3);
				HAL_Delay(15);

				clr_screen1();
				lcd_print_line2("  Saved");
				HAL_Delay(500);

				day = DS1307_GetDate();
				month = DS1307_GetMonth();
				year = DS1307_GetYear();
				dayofweek = DS1307_GetDayOfWeek();

 				clear_buffer1();
 				entry_clear = 1;
 				respons_dir = 0;
			  }
   		 }

		 pkey = read_keypad();

		 if( (pkey > 0) || (failFlag_tm) || (failFlag_dt) )         //key detected
		 {
			if (pkey == 'B')  // up key
			{

			}

			if (pkey == 'C')  // down key
			{

			}

			if ( ((pkey == 'D') && ((respons == 1) || (respons_dir == 1))) || (failFlag_tm) )  //store entered value.
	        {
				 if( ((tm <= 23) && (!hh_success)) && (!failFlag_tm) )
				 {
					 ttime[0] = tm;
					 clear_buffer1();
					 hh_success = true;
				 }
				 else if ( ((tm <=59) && (hh_success)) && (!failFlag_tm) )
				 {
					 ttime[1] = tm;
					 clear_buffer1();
					 hh_success = false;
					 clock_save = true;
				 }
				 else
				 {
					 lcd_print_line2("        ");
					 if(!hh_success)
					 {
						 snprintf(st__, sizeof(st__), "  %02d", hour); //,log_a_new.time._mn);
						 printDisp_c("hr",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
					 }
					 else if(hh_success)
					 {
						 snprintf(st__, sizeof(st__), "  %02d", minute);
						 printDisp_c("NNI",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
					 }
					 lcd_print_line2(" Failed");
					 clear_buffer1();

					 if(!failFlag_tm) self->_tt = 0;

					 if( (failFlag_tm) && (self->_tt > 600) ) failFlag_tm = 0;
					 else failFlag_tm = 1;
				 }
			}

			else if( ((pkey == 'D') && ( (respons == 2) || (respons_dir == 2) )) || (failFlag_dt) )  //store entered value.
			{
				 if( (dte <= 99) && (!yy_success) && (!mth_success) && (!failFlag_dt) )
				 {
					 ddate[2] = dte;
					 clear_buffer1();
					 yy_success = true;
				 }
				 else if ( (dte != 0) && (dte <= 12) && (yy_success) && (!mth_success) && (!failFlag_dt) )
				 {
					 ddate[1] = dte;
					 clear_buffer1();
					 yy_success = false;
					 mth_success = true;
					 montth = dte;
				 }
				else if( (mth_success) && ((montth == January) || (montth == March) || (montth == May) || (montth == July) || (montth == August) || (montth == October) || (montth == December)) && (!failFlag_dt) )
				{
					if ( (dte != 0) && (dte <= 31) )
					{
						save_date(dte);
					}
					else goto __fail;
				}
				else if( (mth_success) && ((montth == April) || (montth == June) || (montth == September) || (montth == November)) && (!failFlag_dt) )
				{
					if ( (dte != 0) && (dte <= 30) )
					{
						save_date(dte);
					}
					else goto __fail;
				}
				else if( (mth_success) && (montth == February) && (!failFlag_dt) )
				{
					if( leapYear_calc(ddate[2]) )
					{
						if ( (dte != 0) && (dte <= 29) )
						{
							save_date(dte);
						}
						else goto __fail;
					}
					else
					{
						if ( (dte != 0) && (dte <= 28) )
						{
							save_date(dte);
						}
						else goto __fail;
					}
				}
				else
				{
					__fail:   lcd_print_line2("        ");
							  if( (!yy_success) && (!mth_success) )
							  {
								  snprintf(st__, sizeof(st__), "  %02d", year);
								  printDisp_c("Yr",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
							  }
							  else if( (yy_success) && (!mth_success) )
							  {
								  snprintf(st__, sizeof(st__), "  %02d", month);
								  printDisp_c("NNO",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
							  }
							  else if(mth_success)
							  {
								  snprintf(st__, sizeof(st__), "  %02d", day);
								  printDisp_c("dd",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
							  }

							 lcd_print_line2(" Failed");
							 clear_buffer1();

							 if(!failFlag_dt) self->_tt = 0;

							 if( (failFlag_dt) && (self->_tt > 600) ) failFlag_dt = 0;
							 else failFlag_dt = 1;
				 }
		}

		if (pkey == 'A')  // back key
		{
			 self->fxn = nothing;
 			 respons_dir = 0;
 			 failFlag_dt = 0;
 			 failFlag_tm = 0;
 			 hh_success = false;
			 yy_success = false;
 			 mth_success = false;
 			 clock_save = 0;
 			 montth = 0;
 			 entry_clear = 1;
 			 clr_screen1();
 			 clear_buffer1();
		}
	}
	//=======================================
	return prog_State;
  }


  //================================ MODE ================================
  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  //======================================================================
  //                NUMBER OF SIDES  ( 1 - 2 ) ==> default : 2
  //======================================================================
   else if ( (self->auth == authed) && ( (self->access == level2)||(self->access == level3) ) && (self->fxn == number_of_sides)  )
   {

	   if (self->t >= 300)
	   {
//		      if( (self->index_generic != 1) && (self->index_generic != 2) ) self->index_generic = 1;

		   lcd_print_line1("Sides  ");

//			  snprintf(st__, sizeof(st__), "       %d", self->index_generic);
			  snprintf(st__, sizeof(st__), "       %d", self->copy_stream2[0].noz_count);
			  lcd_print_line2(st__);


			  lcd_print_line3("  A  ");

			self->t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
			 {
				self->pump_indx++;
				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
				   self->key19_sto_ = rd19;
			 }
		 self->key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
			 {
//				 if (self->index_generic == 1)  self->index_generic = 2;
//				 else if(self->index_generic == 2) self->index_generic = 1;

				 if(self->copy_stream2[0].noz_count == 1)
					 self->copy_stream2[0].noz_count = 2;
				 else if(self->copy_stream2[0].noz_count == 2)
					 self->copy_stream2[0].noz_count = 1;
			 }

			 else if (pkey == 'C')  // down key
			 {
//				 if (self->index_generic == 2)  self->index_generic = 1;
//				 else if(self->index_generic == 1) self->index_generic = 2;

				 if(self->copy_stream2[0].noz_count == 1)
					 self->copy_stream2[0].noz_count = 2;
				 else if(self->copy_stream2[0].noz_count == 2)
					 self->copy_stream2[0].noz_count = 1;
			 }

			 else if (pkey == 'F')  //change pump index.
			 {
				  // self->pump_indx++;
				  //if (self->pump_indx > 2) self->pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
//				 if(self->index_generic == 1)
//					 self->copy_stream2[0].noz_count = 1;
//				 else if(self->index_generic == 2)
//					 self->copy_stream2[0].noz_count = 2;

				 self->index_generic = 0;
				 self->fxn = nothing;
				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
			 {
				 self->index_generic = 0;

				 self->fxn = nothing;
				 clr_screen1();
			 }
		 }
	 return prog_State;
   }


  //=============================== MODE =================================
  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  //======================================================================
  //       SUPPRESSED VOLUME @ START (0 - 10 cL) ==> default : 4cL
  //======================================================================
   else if ( (self->auth == authed) && ( (self->access == level2)||(self->access == level3) ) && (self->fxn == suppressed_display_vol)  )
   {
	   static uint8_t firstTime_sV = 1;

	   if(firstTime_sV == 1)
	   {
		   self->index_generic = (self->copy_stream2[0].startUp_suppressVol * 100);
		   firstTime_sV = 0;
	   }

	   if (self->t >= 300)
	   {
			if(self->index_generic > VOL_SUPPRESSED) self->index_generic = 0;

			lcd_print_line1("Supp. Vol");

			snprintf(st__, sizeof(st__), "   %02d cL", self->index_generic);
			lcd_print_line2(st__);

			lcd_print_line3("  A  ");

			self->t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1) && ( self->key19_sto_ == 0) )
			 {
				self->pump_indx++;
				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
				   self->key19_sto_ = rd19;
			 }
		 self->key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
			 {
				 if (self->index_generic < VOL_SUPPRESSED)  self->index_generic++;
				 else if(self->index_generic == VOL_SUPPRESSED) self->index_generic = 0;
			 }

			 else if (pkey == 'C')  // down key
			 {
				 if (self->index_generic > 0) self->index_generic--;
				 else if(self->index_generic == 0) self->index_generic = VOL_SUPPRESSED;
			 }

			 else if (pkey == 'F')  //change pump index.
			 {
				  // self->pump_indx++;
				  //if (self->pump_indx > 2) self->pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
				 self->copy_stream2[0].startUp_suppressVol = (self->index_generic * 0.01);

				 self->index_generic = 0;
				 self->fxn = nothing;
				 clr_screen1();

				 firstTime_sV = 1;
			 }

			 else if (pkey == 'A')  // back key
			 {
				 self->index_generic = 0;

				 self->fxn = nothing;
				 clr_screen1();

				 firstTime_sV = 1;
			 }
		 }
	 return prog_State;
   }


  //============================= MODE ===================================
  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  //======================================================================
  //       Calibration Can Size (10 L or 20 L) ==>  default : 20L
  //======================================================================
   else if ( (self->auth == authed) && ( (self->access == level2)||(self->access == level3) ) && (self->fxn == calibration_can)  )
   {

	   if (self->t >= 300)
	   {
//		   if( (self->index_generic != 10) && (self->index_generic != 20) ) self->index_generic = 10;

		   lcd_print_line1("Can Size");

		   snprintf(st__, sizeof(st__), "    %02d L", self->copy_stream2[0].calibration_measureCan);
		   lcd_print_line2(st__);

		   lcd_print_line3("  A  ");

			self->t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
		 {
			self->pump_indx++;
			if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
			   self->key19_sto_ = rd19;
		 }
		 self->key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
			 {
//				 if (self->index_generic == 10)  self->index_generic = 20;
//				 else if(self->index_generic == 20) self->index_generic = 10;

				   if(self->copy_stream2[0].calibration_measureCan == 10)
					 self->copy_stream2[0].calibration_measureCan = 20;
				   else  if(self->copy_stream2[0].calibration_measureCan == 20)
					 self->copy_stream2[0].calibration_measureCan = 10;
			 }

			 else if (pkey == 'C')  // down key
			 {
//				 if (self->index_generic == 20)  self->index_generic = 10;
//				 else if(self->index_generic == 10) self->index_generic = 20;

				   if(self->copy_stream2[0].calibration_measureCan == 10)
					 self->copy_stream2[0].calibration_measureCan = 20;
				   else  if(self->copy_stream2[0].calibration_measureCan == 20)
					 self->copy_stream2[0].calibration_measureCan = 10;
			 }

			 else if (pkey == 'F')  //change pump index.
			 {
				  // self->pump_indx++;
				  //if (self->pump_indx > 2) self->pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
//				 if(self->index_generic == 10)
//					 self->copy_stream2[0].calibration_measureCan = 10;
//				 else if(self->index_generic == 20)
//					 self->copy_stream2[0].calibration_measureCan = 20;

				 self->index_generic = 0;
				 self->fxn = nothing;
				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
			 {
				 self->index_generic = 0;
				 self->fxn = nothing;
				 clr_screen1();
			 }
		 }
	 return prog_State;
   }

  //================================= MODE ===============================
  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  //======================================================================
  //               SHIFT LOGIN-TYPE  ==>  default : None_
  //======================================================================
   else if ( (self->auth == authed) && ( (self->access == level2)||(self->access == level3) ) && (self->fxn == shift_login_type_)  )
   {

	   if (self->t >= 300)
	   {
//		   if( (self->index_generic != None_) && (self->index_generic != Code_)  && (self->index_generic != Card_) ) self->index_generic = None_;
		   lcd_print_line1("Login.Typ");

//		   snprintf(st__, sizeof(st__), "       %d", self->index_generic);
		   lcd_print_line2(login_type[self->copy_stream2[0].shift_login_type]);

		   lcd_print_line3("  A  ");

		   self->t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
			 {
				self->pump_indx++;
				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
				   self->key19_sto_ = rd19;
			 }
		 self->key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
			 {
//				 if (self->index_generic <= 2 )  self->index_generic++;
//				 else if(self->index_generic == 3) self->index_generic = 0;

				 if(self->copy_stream2[0].shift_login_type == None_)
					 self->copy_stream2[0].shift_login_type = Code_;
				 else if(self->copy_stream2[0].shift_login_type == Code_)
					 self->copy_stream2[0].shift_login_type = Card_;
				 else if(self->copy_stream2[0].shift_login_type == Card_)
					 self->copy_stream2[0].shift_login_type = None_;
			 }

			 else if (pkey == 'C')  // down key
			 {
//				 if (self->index_generic > 0)  self->index_generic--;
//				 else if(self->index_generic == 0) self->index_generic = 2;

				 if(self->copy_stream2[0].shift_login_type == None_)
					 self->copy_stream2[0].shift_login_type = Card_;
				 else if(self->copy_stream2[0].shift_login_type == Code_)
					 self->copy_stream2[0].shift_login_type = None_;
				 else if(self->copy_stream2[0].shift_login_type == Card_)
					 self->copy_stream2[0].shift_login_type = Code_;
			 }

			 else if (pkey == 'F')  //change pump index.
			 {
				  // self->pump_indx++;
				  //if (self->pump_indx > 2) self->pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
//				 if(self->index_generic == None_)
//					 self->copy_stream2[0].shift_login_type = None_;
//				 else if(self->index_generic == Code_)
//					 self->copy_stream2[0].shift_login_type = Code_;
//				 else if(self->index_generic == Card_)
//					 self->copy_stream2[0].shift_login_type = Card_;

				 self->index_generic = 0;
				 self->fxn = nothing;
				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
			 {
				 self->index_generic = 0;
				 self->fxn = nothing;
				 clr_screen1();
			 }
		 }
	 return prog_State;
   }


  //=============================== MODE =================================
  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  //======================================================================
  //             NUMBER OF SHIFTS PER A DAY  ==>  default : 1
  //======================================================================
   else if ( (self->auth == authed) && ( (self->access == level2)||(self->access == level3) ) && (self->fxn == number_of_shifts_)  )
   {

	   if (self->t >= 300)
	   {
//		   if( (self->index_generic != 1) && (self->index_generic != 2) ) self->index_generic = 1;
		   lcd_print_line1("Shifts   ");

		   snprintf(st__, sizeof(st__), "       %d", self->copy_stream2[0].number_of_shifts);
		   lcd_print_line2(st__);

		   lcd_print_line3("  A  ");

		   self->t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
			 {
				self->pump_indx++;
				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
				   self->key19_sto_ = rd19;
			 }
		 self->key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
			 {
//				 if (self->index_generic == 1)  self->index_generic = 2;
//				 else if(self->index_generic == 2) self->index_generic = 1;

				 if(self->copy_stream2[0].number_of_shifts == 1)
					 self->copy_stream2[0].number_of_shifts = 2;
				 else if(self->copy_stream2[0].number_of_shifts == 2)
					 self->copy_stream2[0].number_of_shifts = 1;
			 }

			 else if (pkey == 'C')  // down key
			 {
//				 if (self->index_generic == 2)  self->index_generic = 1;
//				 else if(self->index_generic == 1) self->index_generic = 2;

				 if(self->copy_stream2[0].number_of_shifts == 1)
					 self->copy_stream2[0].number_of_shifts = 2;
				 else if(self->copy_stream2[0].number_of_shifts == 2)
					 self->copy_stream2[0].number_of_shifts = 1;
			 }

			 else if (pkey == 'F')  //change pump index.
			 {
				  // self->pump_indx++;
				  //if (self->pump_indx > 2) self->pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
//				 if(self->index_generic == 1)
//					 self->copy_stream2[0].number_of_shifts = 1;
//				 else if(self->index_generic == 2)
//					 self->copy_stream2[0].number_of_shifts = 2;

				 self->index_generic = 0;
				 self->fxn = nothing;
				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
			 {
				 self->index_generic = 0;
				 self->fxn = nothing;
				 clr_screen1();
			 }
		 }
	 return prog_State;
   }

  //============================== MODE ==================================
  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  //======================================================================
  //               CALIBRATION TYPE  ==> default : Wizard
  //======================================================================
   else if ( (self->auth == authed) && (self->access == level3) && (self->fxn == calibration_type_)  )
   {

	   if (self->t >= 300)
	   {
//			  if( (self->index_generic !=  Wizard) && (self->index_generic != Manual_calib) ) self->index_generic = Wizard;

			  if(self->copy_stream2[0].calibration_type ==  Wizard)
			  {
				  lcd_print_line1("UUizard");
			  }
			  else
			  {
				  lcd_print_line1("NNanual");
			  }

			  lcd_print_line3("  A  ");

			self->t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
		 {
			self->pump_indx++;
			if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
			   self->key19_sto_ = rd19;
		 }
		 self->key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
			 {
//				 if (self->index_generic == Wizard)  self->index_generic = 2;
//				 else if(self->index_generic == 2) self->index_generic = Wizard;

				 if(self->copy_stream2[0].calibration_type == Wizard)
					 self->copy_stream2[0].calibration_type = Manual_calib;
				 else if(self->copy_stream2[0].calibration_type == Manual_calib)
					 self->copy_stream2[0].calibration_type = Wizard;
			 }

			 else if (pkey == 'C')  // down key
			 {
//				 if (self->index_generic == 2)  self->index_generic = Wizard;
//				 else if(self->index_generic == Wizard) self->index_generic = 2;

				 if(self->copy_stream2[0].calibration_type == Wizard)
					 self->copy_stream2[0].calibration_type = Manual_calib;
				 else if(self->copy_stream2[0].calibration_type == Manual_calib)
					 self->copy_stream2[0].calibration_type = Wizard;
			 }

			 else if (pkey == 'F')  //change pump index.
			 {
				  // self->pump_indx++;
				  //if (self->pump_indx > 2) self->pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
//				 if(self->index_generic == Wizard)
//					 self->copy_stream2[0].calibration_type = Wizard;
//				 else if(self->index_generic == Manual_calib)
//					 self->copy_stream2[0].calibration_type = Manual_calib;

				 self->index_generic = 0;
				 self->fxn = nothing;
				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
			 {
				 self->index_generic = 0;
				 self->fxn = nothing;
				 clr_screen1();
			 }
		 }
	 return prog_State;
   }

 //================================== MODE ===============================
 //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 //=======================================================================
 //                  		    SHIFT-TOTALIZER
 //=======================================================================
   else if ( (self->auth == authed) && (self->access == level1) && (self->fxn == shift_total)  )
   {
	   char scc[15] = {0};

	   char line1[9] = {0};
	   char line2[9] = {0};
	   uint8_t j,
			  jj = 0,
			  k = 0;
	   uint8_t i,res = 0;

	   float shiftTotaliser_vol,
	   	   	 shiftTotaliser_amt;

	   if (self->t >= 300)
	   {
			  if(self->index_generic == 0)
			  {
				 if(attendant1.session_id[0] != NULL)
				 {
					 shiftTotaliser_vol = (self->totaliser_vol_cal - startShiftTotaliser_vol1c);

					 res = snprintf(scc, sizeof(scc), "%.2f", shiftTotaliser_vol);
					 if(res > 8)  //if length of the number > 8
					 {
					  i = res;
					  j = 0;
					  jj = 0;
					   while(i > 8)
					   {
						   line1[j] = scc[j];
						   j++; i--;
						   jj++;
					   }
						  i = 0;
					   while(i < 8)
					   {
							 line2[i] = scc[j];
							 j++; i++;
					   }
					   clear_screen1();
					   printDisp_c("l",1,0,8,LT,CLEAR);
					   printDisp_c(line1, 1, (8-jj), 5, LT, NOCLEAR);  //display self->price
					   lcd_print_line2(line2);
					 }
					 else
					 {
						 lcd_print_line1("l        ");
						 lcd_print_line2("        ");
						 printDisp_f(shiftTotaliser_vol, 2, 0,5,RT,CLEAR ); //lcd_print_line22(scc);
					 }
				  }
				  else
				  {
					  lcd_print_line1("L      ");
				  }
			  }
			  else if(self->index_generic == 1)
			  {
				  if(attendant1.session_id[0] != NULL)
				  {
					  shiftTotaliser_amt = (self->totaliser_amt_cal - startShiftTotaliser_amt1c);
					  res =  snprintf(scc, sizeof(scc), "%.2f", shiftTotaliser_amt);
					  if( res > 8)  //if lenght of the number > 8
					  {
						  i = res;
						  j = 0;
						  jj = 0;
						   while(i > 8)
						   {
							   line1[j] = scc[j];
							   j++; i--;
							   jj++;
						   }
						   i = 0;
						   while(i < 8)
						   {
							 line2[i] = scc[j];
							 j++; i++;
						   }
						   clear_screen1();
						   printDisp_c("p",1,0,8,LT,CLEAR);
						   printDisp_c(line1,1,(8-jj),5,LT,NOCLEAR);
						   lcd_print_line2(line2);
						 }
						 else
						 {
							 lcd_print_line1("p        ");
							 lcd_print_line2("        ");
							 printDisp_f(shiftTotaliser_amt, 2, 0,5,RT,CLEAR ); //lcd_print_line22(scc);
						 }
			      }  //  if (self->indx1 == 1)
				  else
				  {
					  lcd_print_line1("P      ");
				  }

			  }

			  lcd_print_line3("Sh.TotA");
//			  lcd_print_line3("123456");

			self->t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
		 {
			self->pump_indx++;
			if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
			   self->key19_sto_ = rd19;
		 }
		 self->key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
			 {
				 if (self->index_generic == 0)  self->index_generic = 1;
				 else if(self->index_generic == 1) self->index_generic = 0;
			 }

			 else if (pkey == 'C')  // down key
			 {
				 if (self->index_generic == 1)  self->index_generic = 0;
				 else if(self->index_generic == 0) self->index_generic = 1;
			 }

			 else if (pkey == 'F')  //change pump index.
			 {
				  // self->pump_indx++;
				  //if (self->pump_indx > 2) self->pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
				 self->index_generic = 0;
				 self->fxn = nothing;
				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
			 {
				 self->index_generic = 0;

				 self->fxn = nothing;
				 clr_screen1();
			 }
		 }
	 return prog_State;
   }

 //================================================== MODE ================================================
 //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 //========================================================================================================
 //                  		     KEYPRESS-TONE ( Yes / No )  ==> Default : No
 //========================================================================================================
   else if ( (self->auth == authed) && ( (self->access == level2)||(self->access == level3) ) && (self->fxn == keypress_tone_)  )
      {

   	   if (self->t >= 300)
   	   {
   		   lcd_print_line1("Suuitch ");

//   		   if( (self->index_generic !=  Yes) && (self->index_generic !=  No) ) self->index_generic = Yes;

   		   if(self->copy_stream2[0].keypress_tone ==  Yes)
   		   {
   			   lcd_print_line2("      ON");
   		   }
   		   else
   		   {
   			   lcd_print_line2("     OFF");
   		   }

   		  lcd_print_line3("  A  ");

   			self->t = 0;
   		}
   	 // -------------- test keys....----------------
   		rd19 = readkey19_state();

   		if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
   			 {
   				self->pump_indx++;
   				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
   				   self->key19_sto_ = rd19;
   			 }
   		 self->key19_sto_ = rd19;

   		 pkey = read_keypad();

   		 if( (pkey > 0) )  //key detected
   		 {
   			 if (pkey == 'B')  // up key
   			 {
//   				 if (self->index_generic == Yes)  self->index_generic = No;
//   				 else if(self->index_generic == No) self->index_generic = Yes;


				 if(self->copy_stream2[0].keypress_tone == Yes)
 				 {
 					self->copy_stream2[0].keypress_tone = No;
 				 }
 				 else if(self->copy_stream2[0].keypress_tone == No)
 				 {
 					self->copy_stream2[0].keypress_tone = Yes;
 				 }
   			 }

   			 else if (pkey == 'C')  // down key
   			 {
//   				 if (self->index_generic == No)  self->index_generic = Yes;
//   				 else if(self->index_generic == Yes) self->index_generic = No;

   				if(self->copy_stream2[0].keypress_tone == Yes)
				 {
					self->copy_stream2[0].keypress_tone = No;
				 }
				 else if(self->copy_stream2[0].keypress_tone == No)
				 {
					self->copy_stream2[0].keypress_tone = Yes;
				 }
   			 }

   			 else if (pkey == 'F')  //change pump index.
   			 {
   				  // self->pump_indx++;
   				  //if (self->pump_indx > 2) self->pump_indx = 1; //wrap around
   			 }

   			 else if (pkey == 'D')  // Enter key
   			 {
//   				 if(self->index_generic == Yes)
//   				 {
//   					self->copy_stream2[0].keypress_tone = Yes;
//   				 }
//   				 else if(self->index_generic == No)
//   				 {
//   					self->copy_stream2[0].keypress_tone = No;
//   				 }

   				 self->fxn = nothing;
   				 self->index_generic = 0;
   				 clr_screen1();
   			 }

   			 else if (pkey == 'A')  // back key
   			 {
   				 self->fxn = nothing;
   				 self->index_generic = 0;
   				 clr_screen1();
   			 }
   		 }
   	 return prog_State;
      }

	//=================================================  MODE =============================================
	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	//=====================================================================================================
	//                    ENFORCE COMMUNICATION CARD ( Yes / No )  ==> Default : Yes
	//=====================================================================================================
	else if ( (self->auth == authed) && (self->access == level3) && (self->fxn == communication_card)  )
	   {

		   if (self->t >= 300)
		   {
			   lcd_print_line1("Enforce ");

//			   if( (self->index_generic !=  Yes) && (self->index_generic !=  No) ) self->index_generic = Yes;

			   if(self->copy_stream2[0].commCard_enforced ==  true)
			   {
				   lcd_print_line2("     Yes");
			   }
			   else
			   {
				   lcd_print_line2("      No");
			   }

			  lcd_print_line3("  A  ");

				self->t = 0;
			}
		 // -------------- test keys....----------------
			rd19 = readkey19_state();

			if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
			 {
				self->pump_indx++;
				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
				   self->key19_sto_ = rd19;
			 }
			 self->key19_sto_ = rd19;

			 pkey = read_keypad();

			 if( (pkey > 0) )  //key detected
			 {
				 if (pkey == 'B')  // up key
				 {
//					 if (self->index_generic == Yes)  self->index_generic = No;
//					 else if(self->index_generic == No) self->index_generic = Yes;

					 if(self->copy_stream2[0].commCard_enforced == true)
					 {
						 self->copy_stream2[0].commCard_enforced = false;
					 }
					 else if(self->copy_stream2[0].commCard_enforced == false)
					 {
						 self->copy_stream2[0].commCard_enforced = true;
					 }
				 }

				 else if (pkey == 'C')  // down key
				 {
//					 if (self->index_generic == No)  self->index_generic = Yes;
//					 else if(self->index_generic == Yes) self->index_generic = No;

					 if(self->copy_stream2[0].commCard_enforced == true)
					 {
						 self->copy_stream2[0].commCard_enforced = false;
					 }
					 else if(self->copy_stream2[0].commCard_enforced == false)
					 {
						 self->copy_stream2[0].commCard_enforced = true;
					 }
				 }

				 else if (pkey == 'F')  //change pump index.
				 {
					  // self->pump_indx++;
					  //if (self->pump_indx > 2) self->pump_indx = 1; //wrap around
				 }

				 else if (pkey == 'D')  // Enter key
				 {
//					 if(self->index_generic == Yes)
//					 {
//						 self->copy_stream2[0].commCard_enforced = true;
//					 }
//					 else if(self->index_generic == No)
//					 {
//						 self->copy_stream2[0].commCard_enforced = false;
//					 }

					 self->fxn = nothing;
					 self->index_generic = 0;
					 clr_screen1();
				 }

				 else if (pkey == 'A')  // back key
				 {
					 self->fxn = nothing;
					 self->index_generic = 0;
					 clr_screen1();
				 }
			 }
		 return prog_State;
	   }


   //============================== MODE ================================
   //====================================================================
   //                           PULSER CONFIG.
   //====================================================================
  else if ( (self->auth == authed) &&  ( (self->access == level2)||(self->access == level3) ) && (self->fxn == pulser_config)  )
  {
	 static uint8_t pulserr = 0,
					pulserType_original,
					pulserOffset_original,
					firstTime_pOffset = 1;

      if(self->prog_revisit == 1)
 	   {
			   pulser_flag = 0;
			   self->prog_revisit = 0;
 	   }

	   if (pulser_flag == 0)
	    {
		       if (self->t >= 500)
			   {
					if (pulserr == 0)
					{
						lcd_print_line1("  Type  ");
					}
					else
						lcd_print_line1(" Offset  ");


					lcd_print_line3("  A  ");

				   self->t = 0;
				}
  	 // -------------- test keys....----------------
  		  rd19 = readkey19_state();

  		   if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
  			 {
  				self->pump_indx++;
  				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
  				   self->key19_sto_ = rd19;
  			 }
  			   self->key19_sto_ = rd19;

  	     pkey = read_keypad();

  		 if( (pkey > 0) )  //key detected
  		 {
  			 if (pkey == 'B')  // up key
  				{
                  if (pulserr == 0)
                  {
                 	 pulserr = 1;
                  }
                  else
                 	 pulserr = 0;
  				}

  			 else if (pkey == 'C')  // down key
  				{
  				 if (pulserr == 0)
					   {
  					 pulserr = 1;
					   }
					   else
						 pulserr = 0;
  				}

  			 else if (pkey == 'D')  //enter key
  			 {
					pulser_flag  = 1;   //set flag and goto

					if (pulserr == 0)
					{
						 pulserr = 2;
					}
					else
						 pulserr = 3;

					pulserType_original = self->copy_stream2[0].pulser_type_;
					pulserOffset_original = self->copy_stream2[0].pulser_offset;

					return prog_State;
  			  }

  			 else if (pkey == 'A')  // back key
				 {
					 self->fxn = nothing;
					 clr_screen1();
				 }
  		 }
     	 return prog_State;
	   }// if selected


	//========= pulser type has been selected =============
	else if(pulser_flag == 1)
    {
		   if (self->t >= 500)
		   {
			   if (pulserr == 2)
			   {
				   if (self->copy_stream2[0].pulser_type_ == quadrature)
					{
					   lcd_print_line1("Ouadrat. ");
					}
					else if (self->copy_stream2[0].pulser_type_ == non_quadrature)
					{
						lcd_print_line1("Non-Quad.");
					}

					lcd_print_line3("  A  ");
			   }
			   else if (pulserr == 3)
			   {
				   if(firstTime_pOffset == 1)
				   {
					   self->index_generic = pulserOffset_original;
					   firstTime_pOffset = 0;
				   }
//				   if(self->index_generic > 99) self->index_generic = 0;

				   lcd_print_line1("Ofset.Val");

					snprintf(st__, sizeof(st__), "%02d units", self->index_generic);
					lcd_print_line2(st__);

					lcd_print_line3("  A  ");
			   }

			   self->t = 0;
			}

		 // -------------- test keys....----------------
			 rd19 = readkey19_state();

			   if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
				 {
					self->pump_indx++;
					if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
					   self->key19_sto_ = rd19;
				 }
				   self->key19_sto_ = rd19;

			pkey = read_keypad();

			 if( (pkey > 0) )  //key detected
			 {
				 if (pulserr == 2)
				 {
					 if (pkey == 'B')  // up key
					 {
						 if (self->copy_stream2[0].pulser_type_ == quadrature)
						 {
							 self->copy_stream2[0].pulser_type_ = non_quadrature;
						 }
						 else
							 self->copy_stream2[0].pulser_type_ = quadrature;
					 }

					 else if (pkey == 'C')  // down key
					 {
						 if (self->copy_stream2[0].pulser_type_ == quadrature)
						 {
							 self->copy_stream2[0].pulser_type_ = non_quadrature;
						 }
						 else
							 self->copy_stream2[0].pulser_type_ = quadrature;
					 }

					 else if (pkey == 'D')  //enter key
					 {
						pulser_flag  = 0;
						pulserr = 0;
						clr_screen1();

						return prog_State;
					 }

					 else if (pkey == 'A')  // back key
					 {
						 self->fxn = pulser_config;
						 self->copy_stream2[0].pulser_type_ = pulserType_original;
//						 copy[0].pulser_offset = pulserOffset_original;

						 pulserr = 0;
						 pulser_flag = 0;
						 clr_screen1();
					 }
				 }

				 else if (pulserr == 3)
				 {
					 if (pkey == 'B')  // up key
					 {
						 if (self->index_generic < 99)  self->index_generic++;
						 else if(self->index_generic == 99) self->index_generic = 0;
					 }

					 else if (pkey == 'C')  // down key
					 {
						 if (self->index_generic > 0) self->index_generic--;
						 else if(self->index_generic == 0) self->index_generic = 99;
					 }

					 else if (pkey == 'F')  //change pump index.
					 {
						  // self->pump_indx++;
						  //if (self->pump_indx > 2) self->pump_indx = 1; //wrap around
					 }
					 else if (pkey == 'D')  // Enter key
					 {
						 self->copy_stream2[0].pulser_offset = (self->index_generic);

						 self->index_generic = 0;

						 pulserr = 0;
						 pulser_flag = 0;
						 self->fxn = pulser_config;
						 clr_screen1();

						 firstTime_pOffset = 1;
					 }

					 else if (pkey == 'A')  // back key
					 {
						 self->index_generic = 0;

						 pulserr = 0;
						 pulser_flag = 0;

						 self->fxn = pulser_config;
						 clr_screen1();

						 firstTime_pOffset = 1;
					 }
				 }
			 }
		 }
		 return prog_State;
	  }

	  //=============================== MODE =================================
	  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	  //======================================================================
	  //       						VALVE CONFIG.
	  //======================================================================
	   else if ( (self->auth == authed) && ( (self->access == level2)||(self->access == level3) ) && (self->fxn == valve_config)  )
	   {
		   static float float_generic = 0.00,
						salesStart_original,
						salesEnd_original;

		   static uint8_t _valve = 0,
				   	   	  firstTime_v = 1;

		         if(self->prog_revisit == 1)
		    	 {
		        	   valve_flag = 0;
		   			   self->prog_revisit = 0;
		    	 }

		   	   if (valve_flag == 0)
		   	   {
				   if (self->t >= 500)
				   {
						if (_valve == 0)
						{
							lcd_print_line1("Ds Start");
						}
						else
							lcd_print_line1("Disp. End");

						lcd_print_line3("  A  ");

					   self->t = 0;
					}
		     	 // -------------- test keys....----------------
		     		 rd19 = readkey19_state();

		     		   if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
		     			 {
		     				self->pump_indx++;
		     				if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
		     				   self->key19_sto_ = rd19;
		     			 }
		     			 self->key19_sto_ = rd19;

		     	     pkey = read_keypad();

		     		 if( (pkey > 0) )  //key detected
		     		 {
		     			 if (pkey == 'B')  // up key
		     				{
		                     if (_valve == 0)
		                     {
		                    	 _valve = 1;
		                     }
		                     else
		                    	 _valve = 0;
		     				}

		     			 else if (pkey == 'C')  // down key
		     				{
		     				 if (_valve == 0)
		   					   {
		     					_valve = 1;
		   					   }
		   					   else
		   						_valve = 0;
		     				}

		     			 else if (pkey == 'D')  //enter key
		     			 {
		     				valve_flag  = 1;   //set flag and goto

		   					if (_valve == 0)
		   					{
		   						_valve = 2;
		   					}
		   					else
		   						_valve = 3;

		   					salesStart_original = self->copy_stream2[0].valve_salesStart;
		   					salesEnd_original = self->copy_stream2[0].valve_salesEnd;

		   					return prog_State;
		     			  }

		     			 else if (pkey == 'A')  // back key
		   				 {
		   					 self->fxn = nothing;
		   					 clr_screen1();
		   				 }
		     		 }
		        	 return prog_State;
		   	}// if selected


		   	else if(valve_flag == 1)
			{
				   if (self->t >= 300)
				   {
					   if (_valve == 2)
					   {
						    if(firstTime_v == 1)
						    {
						    	float_generic = self->copy_stream2[0].valve_salesStart;
								firstTime_v =  0;
						    }

//							if(float_generic > 2.00) float_generic = 0.00;
//						    else if(float_generic < 0.00) float_generic = 2.00;

							snprintf(st__, sizeof(st__), "  %5.2f L", float_generic);
							lcd_print_line2(st__);
					   }
					   else if (_valve == 3)
					   {
						   if(firstTime_v == 1)
							{
								float_generic = self->copy_stream2[0].valve_salesEnd;
								firstTime_v =  0;
							}

//						   if(float_generic > 10.00) float_generic = 0.30;
//							else if(float_generic < 0.30) float_generic = 10.00;

							snprintf(st__, sizeof(st__), "  %5.2f L", float_generic);
							lcd_print_line2(st__);
					   }

					   lcd_print_line3("  A  ");

					   self->t = 0;
				}

				 // -------------- test keys....----------------
					 rd19 = readkey19_state();

					   if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
						 {
							self->pump_indx++;
							if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
							   self->key19_sto_ = rd19;
						 }
						   self->key19_sto_ = rd19;

					pkey = read_keypad();

					 if( (pkey > 0) )  //key detected
					 {
						 if (_valve == 2)
						 {
							 if (pkey == 'B')  // up key
							 {
								 if(float_generic > 2.00) float_generic = 0.00;
								 float_generic = float_generic + 0.01;
							 }

							 else if (pkey == 'C')  // down key
							 {
								 if(float_generic < 0.00) float_generic = 2.00;
								 float_generic = float_generic - 0.01;
							 }

							 else if (pkey == 'D')  //enter key
							 {
								valve_flag  = 0;
								_valve = 0;
								self->copy_stream2[0].valve_salesStart = float_generic;

								self->fxn = nothing;
								clr_screen1();

								firstTime_v = 0;

								return prog_State;
							 }
							 else if (pkey == 'A')  // back key
							 {
								 self->fxn = nothing;
								 self->copy_stream2[0].valve_salesStart = salesStart_original;
								 _valve = 0;
								 valve_flag = 0;
								 clr_screen1();

								 firstTime_v = 0;
							 }
						 }
						 else if (_valve == 3)
						 {
							 if (pkey == 'B')  // up key
							 {
								 if(float_generic > 10.00) float_generic = 0.30;
								 float_generic = float_generic + 0.01;

							 }

							 else if (pkey == 'C')  // down key
							 {
								 if(float_generic < 0.30) float_generic = 10.00;
								 float_generic = float_generic - 0.01;
							 }

							 else if (pkey == 'D')  //enter key
							 {
								valve_flag  = 0;
								_valve = 0;
								self->copy_stream2[0].valve_salesEnd = float_generic;

								clr_screen1();

								firstTime_v = 0;

								return prog_State;
							 }
							 else if (pkey == 'A')  // back key
							 {
								 self->fxn = nothing;
								 self->copy_stream2[0].valve_salesEnd = salesEnd_original;

								 _valve = 0;
								 valve_flag = 0;

								 firstTime_v = 0;

								 clr_screen1();
							 }
						 }
					}
			}
		 return prog_State;
	   }

	  //=============================== MODE =================================
	  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	  //======================================================================
	  //       						NON-CALIBRATION
	  //======================================================================
	   else if ( (self->auth == authed) && ( (self->access == level2)||(self->access == level3) ) && (self->fxn == no_calibration)  )
	   {

		   if (self->t >= 300)
		   {
				snprintf(st__, sizeof(st__), "PC %05d", self->calib_pulser);
				lcd_print_line1(st__);

				lcd_print_line3("  A  ");

				self->t = 0;
			}
		 // -------------- test keys....----------------
			rd19 = readkey19_state();

			if ( (rd19 == 1)&&( self->key19_sto_ == 0) )
				 {
					self->pump_indx++;
					if (self->pump_indx > 2) self->pump_indx = 1;  //wrap around
					   self->key19_sto_ = rd19;
				 }
			 self->key19_sto_ = rd19;

			 pkey = read_keypad();

			 if( (pkey > 0) )  //key detected
			 {
				 if (pkey == 'B')  // up key
				 {
					self->index_generic++;
				 }

				 else if (pkey == 'C')  // down key
				 {
					self->index_generic--;
				 }

				 else if (pkey == 'F')  //change pump index.
				 {
					 ;
				 }

				 else if (pkey == 'D')  // Enter key
				 {
					 self->copy_stream2[0].non_calibration_seed = (self->index_generic);

					 self->index_generic = 0;
					 self->fxn = nothing;
					 clr_screen1();
				 }

				 else if (pkey == 'A')  // back key
				 {
					 self->index_generic = 0;

					 self->fxn = nothing;
					 clr_screen1();
				 }
			 }
		 return prog_State;
	   }

 //============================== MODE ==================================
  return prog_State;
} //end of prog_state.



//----------------------------------------
eSystemState pnpState_Handler(void)
{
	static int  self->printer_status;

	static int8_t self->idleState_flag = 1;

	static int32_t self->current_pulser_ = 0,
				   self->old_pulser_ = 0;

	uint16_t gerCtTime;

	int pulser_diff = 0;

	self->stop_fueling_bit = 1;

	self->lock_clr = 0;

	self->progg = 0;

	firstTime_nozz1 = 1;


	#if sense_power == 1
	  if(readpwr() == 0)
	  {
		   modem_power(DEACTIVATE);

		   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
		   HAL_Delay(100);
		   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);

		   //count time elapsed
		   if (self->shutdown_timer > 120)
		   {
			   displayandkeypad_power(DEACTIVATE);   //shutdown... after  2 minutes
			   mcu_power(DEACTIVATE);
		   }
	  }
	  else
	  {
		  //clear the timer//
		  self->shutdown_timer = 0;

		  modem_power(ACTIVATE);
		  displayandkeypad_power(ACTIVATE);
	  }
	#endif

	#if !defined (DEV_MODE)
//		if(batteryStatus == LOW_BATTERY)
//		{
//			lcd_print_line1("  Louu   ");
//			lcd_print_line2("Battery  ");
//			lcd_print_line3(" Err70 ");
//
////			return inactive_State;
//		}
//		else if(batteryStatus == NO_BATTERY)
//		{
//			lcd_print_line1("Battery ");
//			lcd_print_line2(" Error  ");
//			lcd_print_line3(" Err71 ");
//
//			return inactive_State;
//		}

		if(HAL_GPIO_ReadPin(pulser1_detect_GPIO_Port, pulser1_detect_Pin) == 1 )
		{
			lcd_print_line1(" Pulser ");
			lcd_print_line2("  Error ");
			lcd_print_line3(" Err24 ");

			return inactive_State;
		}
		else
		{

		}

	//	if(self->calib_pulser < 15800)  //15987, 15967 .... 1106247681
//		self->calibration_flag = CALIBRATED;
		if(self->calibration_flag != CALIBRATED) //15800)  //15987, 15967 .... 1106247681
		{
			retrieve_calibrationFlag_fram(side_a);

			if(self->calibration_flag != CALIBRATED) //takes care of accidental clearing of self->calibration_flag by F-keys
			{
				self->calibration_error = 1;

				lcd_print_line1("Calibrat");
				lcd_print_line2("  Error ");
				lcd_print_line3("Err 23 ");

				return inactive_State;
			}
		}
		else
		{
			self->calibration_error = 0;
		}
	#endif    //#if !defined (DEV_MODE) ,,

	if(self->fillingresume_flag1_1 == 1)
	{
		self->amt = pulser2amt(self->currentPulser_recovered);

		self->price = amt2price(self->amt);

		self->amt_middle_tmin3 = self->amt;
		self->amt_middle_tmin2 = self->amt;
		self->amt_middle_tmin1 = self->amt;
		self->amt_middle = self->amt;

		self->amt_real_tmin3 = self->amt;
		self->amt_real_tmin2 = self->amt;
		self->amt_real_tmin1 = self->amt;
		self->amt_real = self->amt;

		make_string(P, dp(self->price, self->dp_amount));
	    make_string(L, dp(self->amt, self->dp_vol));

		if  (self->t > 500)
		{
			 if(self->settings_stream1[0].display_format == PL)
			 {
				 lcd_print_line1(self->upper);
				 lcd_print_line2(self->middle);
			 }
			 else if(self->settings_stream1[0].display_format == LP)
			 {
				  lcd_print_line1(self->middle);
				  lcd_print_line2(self->upper);
			 }

			 lcd_print_line3("        ");
			 char str__[8]= {0};
			 snprintf(str__, sizeof(str__), "%.2f", self->litre_price);
			 lcd_print_line3(str__);
		}
	}
	else
	{
		 if  (self->t > 500)
		 {
			 lcd_print_line1("  Auto  ");
			 lcd_print_line2(" NNode  ");

	//		 lcd_print_line3("      ");

			 char str__[8] = {0};
			 snprintf(str__, sizeof(str__), "%.2f", self->litre_price);
			 lcd_print_line3(str__);
		 }
	}

	 if (self->t > 2000)
	 {
		  /* start the DMA again */
		  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *) RxBuf, RxBuf_SIZE);
		  __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

		  self->t = 0;
	 }


	 if( (self->keypress_ == 21) && (self->nozzleup_awaitingauth_state_not_timedOut == 0) )
	 {
		   compose_printer();
		   HAL_Delay(1000);
		   self->printer_status = 1;
	 }

	     print__1();   //print the transaction.

	  	return pnp_State;
}



eSystemState idleState_Handler(void)
{
	static int  self->printer_status;

	static int8_t self->idleState_flag = 1;

	static int32_t self->current_pulser_ = 0,
				   self->old_pulser_ = 0;

	uint16_t gerCtTime;

	int pulser_diff = 0;

	idleStateEntry_flag1 = 1;


	//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA//
	//======================= AUTOMATED SALES TEST ==========================//

	if(firstTime_idleState1 == 1)
	{
		autoSale_timer1 = 0;
		firstTime_idleState1 = 2;
	}

	//UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU//


	self->pump_status_4G = STATUS_IDLE;
//	self->pump_status = STATUS_FILLING_COMP;

	self->stop_fueling_bit = 1;

	self->stop_flag = 0;

	self->lock_clr = 0;

	self->progg = 0;

	firstTime_nozz1 = 1;

	#if sense_power == 1
	  if(readpwr() == 0)
	  {
		   modem_power(DEACTIVATE);

		   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
		   HAL_Delay(100);
		   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);

		   //count time elapsed
		   if (self->shutdown_timer > 120)
		   {
			   displayandkeypad_power(DEACTIVATE);   //shutdown... after  2 minutes
			   mcu_power(DEACTIVATE);
		   }
	  }
	  else
	  {
		  //clear the timer//
		  self->shutdown_timer = 0;

		  modem_power(ACTIVATE);
		  displayandkeypad_power(ACTIVATE);
	  }
	#endif

	#if !defined (DEV_MODE)
//	#ifndef DEV_MODE
		if(batteryStatus == LOW_BATTERY)
		{
			if ( (self->t >= 300) && (self->t <= 700) )
			{
				lcd_print_line1("  Louu   ");
				lcd_print_line2("Battery  ");
				lcd_print_line3(" Err70 ");
			}
			else if ( (self->t > 700) && (self->t <= 2000) )
			{
				  if(self->settings_stream1[0].display_format == PL)
				  {
					 lcd_print_line1(self->upper);
					 lcd_print_line2(self->middle);
				  }
				  else if(self->settings_stream1[0].display_format == LP)
				  {
					  lcd_print_line1(self->middle);
					  lcd_print_line2(self->upper);
				  }
				 lcd_print_line3("        ");
				 char str__[8] = {0};
				 snprintf(str__, sizeof(str__), "%.2f", self->litre_price);
				 lcd_print_line3(str__);
			}

			if(self->t > 2000)
			{
				self->t = 0;
			}

//			return inactive_State;
		}
		else if(batteryStatus == NO_BATTERY)
		{
			lcd_print_line1("Battery ");
			lcd_print_line2(" Error  ");
			lcd_print_line3(" Err71 ");

			return inactive_State;
		}

		if(HAL_GPIO_ReadPin(pulser1_detect_GPIO_Port, pulser1_detect_Pin) == 1 )
		{
			lcd_print_line1(" Pulser ");
			lcd_print_line2("  Error ");
			lcd_print_line3(" Err24 ");

			return inactive_State;
		}
		else
		{

		}

//		self->calibration_flag = CALIBRATED;
	//	if(self->calib_pulser < 15800)  //15987, 15967 .... 1106247681
		if(self->calibration_flag != CALIBRATED) //15800)  //15987, 15967 .... 1106247681
		{
			retrieve_calibrationFlag_fram(side_a);

			if(self->calibration_flag != CALIBRATED) //takes care of accidental clearing of self->calibration_flag by F-keys
			{
				self->calibration_error = 1;

				lcd_print_line1("Calibrat");
				lcd_print_line2("  Error ");
				lcd_print_line3("Err 23 ");

				return inactive_State;
			}
		}
		else
		{
			self->calibration_error = 0;
		}
	#endif    //#if !defined (DEV_MODE) ,,

	if(self->nozzleUp_inProgMode == 1)
	{
			clear_buffer1();
			self->prog_entry = 0;
			self->prog_revisit = 1;

//		#ifndef DEV_MODE
			self->prog_revisitt = 1;
//		#endif

			self->nozzleUp_inProgMode = 0;
	}
	if(self->ctTimed_flag == 1)
	{
		day = DS1307_GetDate();
		if( (self->ctTimed_day == day) || ((self->ctTimed_day + 1) == day) )
		{
			gerCtTime = get_ctTime1();

			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
			//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx IF IT'S TIME xxxxxxxxxxxxxxxxxxxxxxxxxxxxx//
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
			if( (gerCtTime >= self->ctTimed_settings.startTime) && (gerCtTime <= self->ctTimed_settings.endTime) )
			{
				self->settings_stream1[0].pi_cal = (self->calib_pulser / self->vol_effective1_1);
			}
			else
			{
				retrieve_originalPi_c(side_a);
			}

			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
			//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx IF TIME ELAPSESxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
			if(gerCtTime > self->ctTimed_settings.endTime)
			{
				if( ( (self->ctTimed_settings.startTime - self->ctTimed_settings.endTime) < 0) || ( (self->ctTimed_settings.startTime - self->ctTimed_settings.endTime) == 0) )
				{
					self->ctTimed_flag = 0;
					save_ctTimedFlag(side_a);
				}
				else if( (self->ctTimed_settings.startTime - self->ctTimed_settings.endTime) > 0)
				{
					if( (self->ctTimed_day + 1) == day)
					{
						self->ctTimed_flag = 0;
						save_ctTimedFlag(side_a);
					}
				}
			}

		}
	}

	if (self->settings_stream1[0].mode == AUTO_MODE)
	{
		if(self->online_calibFlag == 1)
		{
			self->settings_stream1[0].pi_cal = (self->calib_pulser / self->vol_effective);

			save_settings_fram();
			self->online_calibFlag = 0;
			save_online_calibFlag_fram(side_a);
		}

		/////////////////////////////////////////////////////
		// Resume Sales after an abrupt reboot, during sales
		/////////////////////////////////////////////////////
		if(self->fillingresume_flag1_1 == 1)
		{
			self->fillingresume_flag = 1;
			self->fillingresume_flag1_1 = 0;
		}
	}

	/////////////////////////////////////////////////////
	// Resume Sales after an abrupt reboot, during sales
	/////////////////////////////////////////////////////
	else if (self->settings_stream1[0].mode == MANUAL_MODE)
	{
		if(self->fillingresume_flag1_1 == 1)
		{
			self->amt = pulser2amt(self->currentPulser_recovered);

			self->price = amt2price(self->amt);

			self->amt_middle_tmin3 = self->amt;
			self->amt_middle_tmin2 = self->amt;
			self->amt_middle_tmin1 = self->amt;
			self->amt_middle = self->amt;

			self->amt_real_tmin3 = self->amt;
			self->amt_real_tmin2 = self->amt;
			self->amt_real_tmin1 = self->amt;
			self->amt_real = self->amt;

			make_string(P, dp(self->price, self->dp_amount));
			make_string(L, dp(self->amt, self->dp_vol));

			self->fillingresume_flag = 1;
			self->fillingresume_flag1_1 = 0;
		}
	}
	//--------------------------------------------------


	if( (self->idleState_flag == 1) && (self->eLastState != filling_State) )
	{
		self->idleState_flag = 2;
		self->_tt = 0;
	}
	else if( (self->_tt > 60000) && (self->idleState_flag == 2) && (self->eLastState != filling_State) )
	{
		self->current_pulser_ = __HAL_TIM_GET_COUNTER(self->flow_counter);
		self->old_pulser_ = self->current_pulser_ ;
		self->idleState_flag = 3;
	}
	else if( (self->_tt > 65000) && (self->idleState_flag == 3) && (self->eLastState != filling_State) )
	{
		if(self->old_pulser_ > self->current_pulser_ )
		{
			pulser_diff = self->old_pulser_ - self->current_pulser_;

			if(pulser_diff > 4)
				self->idle_backwardPulse = 1;
		}
		else if(self->old_pulser_ < self->current_pulser_)
		{
			pulser_diff = self->current_pulser_ - self->old_pulser_;

			if(pulser_diff > 4)
				self->idle_forwardPulse = 1;
		}

		self->idleState_flag = 1;
	}

//	 if(self->changeLitrePrice == 1)
//	 {
//		online_setUnitPrice1();
//		self->changeLitrePrice = 0;
//	 }
	if(self->changeLitrePrice == 1)
	{

	/********************************************************************************************
	 | if the programme state is not been in idle-state or inactive-state,
	 | reset the timer,
	 | to effect the self->price change after the set time
	*********************************************************************************************/
//		if( !( (self->eNextState == idle_State) && (self->eLastState == idle_State) ) )
		if( ( (self->eNextState != idle_State) || (self->eLastState != idle_State) ) &&
				( (self->eNextState != idle_State) && (self->eLastState != inactive_State) ) )
		{
			priceChange_timer1 = 0;
		}
		else //if( (self->eNextState == idle_State) && (self->eLastState == idle_State) )
		{
			if(priceChange_timer1 >= 3000)   //30000)
			{
				priceChange_timerMin1++;
				priceChange_timer1 = 0;

				if(priceChange_timerMin1 == 3)
				{
//					online_setUnitPrice1();
					go_setUnitPrice1(price_update1);
					self->changeLitrePrice = 0;
					self->changeLitrePrice1_2 = 1;
					priceChange_timerMin1 = 0;
					self->t = 0;
				}
			}
		}

		if(self->settings_stream1[0].display_format == PL)
		{
			 lcd_print_line1(self->upper);
			 lcd_print_line2(self->middle);
	    }
	    else if(self->settings_stream1[0].display_format == LP)
	    {
		    lcd_print_line1(self->middle);
		    lcd_print_line2(self->upper);
	    }

	    lcd_print_line3("        ");
	    char str__[8]= {0};
	    snprintf(str__, sizeof(str__), "%.2f", self->litre_price);
	    lcd_print_line3(str__);
	}

	else if( (timer_go >= TIMEOUT_GO) && (self->settings_stream1[0].mode == AUTO_MODE) )  //if go's timeout is 5sec threshold
	{
		if ( (self->t >= 300) && (self->t <= 700) )
		{
			lcd_print_line1("        ");
			lcd_print_line2(" Offline");
			lcd_print_line3("      ");
		}
		else if ( (self->t > 700) && (self->t <= 2000) )
		{
			  if(self->settings_stream1[0].display_format == PL)
			  {
				 lcd_print_line1(self->upper);
				 lcd_print_line2(self->middle);
			  }
			  else if(self->settings_stream1[0].display_format == LP)
			  {
				  lcd_print_line1(self->middle);
				  lcd_print_line2(self->upper);
			  }
			 lcd_print_line3("        ");
			 char str__[8]= {0};
			 snprintf(str__, sizeof(str__), "%.2f", self->litre_price); lcd_print_line3(str__);
		}

	   if (self->t > 2000)
	   {
		  /* start the DMA again */
		  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *) RxBuf, RxBuf_SIZE);
		  __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

		  self->t = 0;
	   }
	}

	else if ( (self->t > 500) && (self->nozzleup_awaitingauth_state_not_timedOut == 0) && (self->pump_LitreOverflow == 0) && (self->_litre_price == 0)
			&& (self->_auth_p == 0) && (self->_auth_v == 0) && (self->idle_backwardPulse == 0) && (self->idle_forwardPulse == 0)
			&& (self->flow_loss == 0) && (self->display_overflow == 0) && (self->changeLitrePrice1_2 == 0) && (self->_pump_max_litres == 0) && (self->nonValid_sale == 0)
			&& (self->authorizedSale_overflow == 0) && (self->fillingresume_flag == 0) )
	{
		 if(self->settings_stream1[0].display_format == PL)
		 {
			 lcd_print_line1(self->upper);
			 lcd_print_line2(self->middle);
		 }
		 else if(self->settings_stream1[0].display_format == LP)
		 {
			  lcd_print_line1(self->middle);
			  lcd_print_line2(self->upper);
		 }
		 lcd_print_line3("        ");
		 char str__[8]= {0};
		 snprintf(str__, sizeof(str__), "%.2f", self->litre_price);
		 lcd_print_line3(str__);
		 self->t = 0;
	 }

//	  	if ( ((self->t > 1000) && (self->t <= 1000)) && (self->nozzleup_awaitingauth_state_not_timedOut == 1) )
//		 {
//			 // lcd_print_line1("n up u a");
//			 clr_screen1();
//			 lcd_print_line1("call.  ");
//			 //self->t = 0;
//		 }
//		 else if ( ((self->t > 2000) && (self->t <= 3000)) && (self->nozzleup_awaitingauth_state_not_timedOut == 1) )
//		 {
//			lcd_print_line1("call . ");
//			//self->t = 0;
//		 }
//		 else if ( ((self->t > 3000) && (self->t <= 4000)) && (self->nozzleup_awaitingauth_state_not_timedOut == 1) )
//		 {
//			lcd_print_line1("call  .");
//			if(self->t > 4000) self->t = 0;
//		 }

	else if ( (self->t > 2000) && (self->pump_LitreOverflow == 1) )
	{
		 // lcd_print_line1("n up u a");
//			 clr_screen1();
		 lcd_print_line1("  Punnp  ");
		 lcd_print_line2(" Linnit  ");
		 lcd_print_line3("Err7   ");
		 if(self->t > 6000)
		 {
			 self->t = 0;
			 self->pump_LitreOverflow = 0;
		 }
	 }

//	 else if ( (self->t > 2000) && (self->authorizedSale_overflow == 1) )
//	 {
//			 lcd_print_line1("  Auth. ");
//			 lcd_print_line2(" Linnit  ");
//			 lcd_print_line3("Err7   ");
//			 if(self->t > 6000)
//			 {
//				 self->t = 0;
//				 self->authorizedSale_overflow = 0;
//			 }
//	  }
//	  	else if ( (self->t > 2000) && (self->display_overflow == 1) )
//		{
//			 lcd_print_line1("display   ");
//			 lcd_print_line2(" linnit  ");
//			 lcd_print_line3("err18   ");
//			 if(self->t > 6000)
//			 {
//				 self->t = 0;
//				 self->display_overflow = 0;
//			 }
//		}
	  	else if (self->display_overflow == 1)
		{
	  		 if(firstTime_display_overflow1 == 1)
			 {
	  			 if( (self->t > 2000) && (self->t <= 6000) )
	  			 {
	  				 clr_screen1();
	  				 lcd_print_line1("Display  ");
					 lcd_print_line2(" Linnit  ");
					 lcd_print_line3("Err18   ");
	  			 }
	  			 else if(self->t > 6000) //&& (self->display_overflow == 1) )
				 {
					 self->t = 0;

					 clr_screen1();

//					 lcd_print_line1(self->upper);
//					 lcd_print_line2(self->middle);

					 if(self->settings_stream1[0].display_format == PL)
				  	  {
				  		 lcd_print_line1(self->upper);
				  		 lcd_print_line2(self->middle);
				  	  }
					  else if(self->settings_stream1[0].display_format == LP)
					  {
						  lcd_print_line1(self->middle);
						  lcd_print_line2(self->upper);
					  }

					 lcd_print_line3("        ");
					 char str__[8]= {0};
					 snprintf(str__, sizeof(str__), "%.2f", self->litre_price); lcd_print_line3(str__);
					 firstTime_display_overflow1 = 0;
				 }
			 }
	  		 else if(firstTime_display_overflow1 == 0)
			 {
	  			 if( (self->t > 6000) && (self->t <= 12000) )
	  			 {
	  				 clr_screen1();
	  				 lcd_print_line1("Display  ");
					 lcd_print_line2(" Linnit  ");
					 lcd_print_line3("Err18   ");
	  			 }
	  			 else if(self->t > 12000) //&& (self->display_overflow == 1) )
				 {
					 self->t = 0;

					 clr_screen1();

					 if(self->settings_stream1[0].display_format == PL)
					  {
						 lcd_print_line1(self->upper);
						 lcd_print_line2(self->middle);
					  }
					  else if(self->settings_stream1[0].display_format == LP)
					  {
						  lcd_print_line1(self->middle);
						  lcd_print_line2(self->upper);
					  }

//					 lcd_print_line1(self->upper);
//					 lcd_print_line2(self->middle);
					 lcd_print_line3("        ");
					 char str__[8]= {0};
					 snprintf(str__, sizeof(str__), "%.2f", self->litre_price); lcd_print_line3(str__);
				 }
			 }
		}

	  	else if (self->authorizedSale_overflow == 1)
		{
	  		 if(firstTime_authorizedSale_overflow1 == 1)
			 {
	  			 if( (self->t > 2000) && (self->t <= 6000) )
	  			 {
	  				 clr_screen1();
	  				 lcd_print_line1("  Auth.  ");
					 lcd_print_line2(" Linnit  ");
					 lcd_print_line3("Err18   ");
	  			 }
	  			 else if(self->t > 6000) //&& (self->display_overflow == 1) )
				 {
					 self->t = 0;

					 clr_screen1();

//					 lcd_print_line1(self->upper);
//					 lcd_print_line2(self->middle);

					 if(self->settings_stream1[0].display_format == PL)
				  	  {
				  		 lcd_print_line1(self->upper);
				  		 lcd_print_line2(self->middle);
				  	  }
					  else if(self->settings_stream1[0].display_format == LP)
					  {
						  lcd_print_line1(self->middle);
						  lcd_print_line2(self->upper);
					  }

					 lcd_print_line3("        ");
					 char str__[8]= {0};
					 snprintf(str__, sizeof(str__), "%.2f", self->litre_price);
					 lcd_print_line3(str__);
					 firstTime_authorizedSale_overflow1 = 0;
				 }
			 }
	  		 else if(firstTime_authorizedSale_overflow1 == 0)
			 {
	  			 if( (self->t > 6000) && (self->t <= 12000) )
	  			 {
	  				 clr_screen1();
	  				 lcd_print_line1("  Auth.  ");
					 lcd_print_line2(" Linnit  ");
					 lcd_print_line3("Err18   ");
	  			 }
	  			 else if(self->t > 12000) //&& (self->display_overflow == 1) )
				 {
					 self->t = 0;

					 clr_screen1();

					 if(self->settings_stream1[0].display_format == PL)
					  {
						 lcd_print_line1(self->upper);
						 lcd_print_line2(self->middle);
					  }
					  else if(self->settings_stream1[0].display_format == LP)
					  {
						  lcd_print_line1(self->middle);
						  lcd_print_line2(self->upper);
					  }

//					 lcd_print_line1(self->upper);
//					 lcd_print_line2(self->middle);
					 lcd_print_line3("        ");
					 char str__[8]= {0};
					 snprintf(str__, sizeof(str__), "%.2f", self->litre_price);
					 lcd_print_line3(str__);
				 }
			 }
		}

	  	else if( (self->t > 2000) && (self->_litre_price == 1) )
	  	{
	  		 lcd_print_line1("No Unit   ");
			 lcd_print_line2(" Price  ");
			 lcd_print_line3("Err8   ");
			 if(self->t > 6000)
			 {
				 self->t = 0;
				 self->_litre_price = 0;
			 }
	  	}
		else if( (self->t > 2000) && (self->_pump_max_litres == 1) )
	  	{
	  		 lcd_print_line1("No-punnp  ");
			 lcd_print_line2(" Linnit  ");
			 lcd_print_line3("Err21   ");
			 if(self->t > 6000)
			 {
				 self->t = 0;
				 self->_pump_max_litres = 0;
			 }
	  	}
	  	else if( (self->t > 2000) && (self->_auth_p == 1) )
		{
			 lcd_print_line1("    No   ");
			 lcd_print_line2("  Price  ");
			 lcd_print_line3("Err9   ");
			 if(self->t > 6000)
			 {
				 self->t = 0;
				 self->_auth_p = 0;
			 }
		}
		else if( (self->t > 2000) && (self->_auth_v == 1) )
		{
			 lcd_print_line1("   No   ");
			 lcd_print_line2("Volunne ");
			 lcd_print_line3("Err10   ");
			 if(self->t > 6000)
			 {
				 self->t = 0;
				 self->_auth_v = 0;
			 }
		}
		else if( (self->t > 2000) && (self->nonValid_sale == 1) )
		{
			 lcd_print_line1("   Non   ");
			 lcd_print_line2("  Valid  ");
			 lcd_print_line3("Err22   ");
			 if(self->t > 6000)
			 {
				 self->t = 0;
				 self->nonValid_sale = 0;
			 }
		}
		else if( (self->t > 2000) && (self->idle_backwardPulse == 1) )
		{
			 lcd_print_line1("  Back   ");
			 lcd_print_line2("  Flouu  ");
			 lcd_print_line3("Err13   ");
			 if(self->t > 6000)
			 {
				 self->t = 0;
				 self->idle_backwardPulse = 0;
			 }
		}
		else if( (self->t > 2000) && (self->idle_forwardPulse == 1) )
		{
			 lcd_print_line1("   Idle   ");
			 lcd_print_line2("  Flouu  ");
			 lcd_print_line3("Err14  ");
			 if(self->t > 6000)
			 {
				 self->t = 0;
				 self->idle_forwardPulse = 0;
			 }
		}
		else if( (self->t > 2000) && (self->flow_loss == 1) )
		{
			 lcd_print_line1("   Flow   ");
			 lcd_print_line2("  Loss  ");
			 lcd_print_line3("Err16  ");
			 if(self->t > 6000)
			 {
				 self->t = 0;
				 self->flow_loss = 0;
			 }
		}
		else if( ( self->t > 300) && (self->changeLitrePrice1_2 == 1) )
		{

			 clr_screen1();
			 lcd_print_line1("  Price ");
			 lcd_print_line2(" Changed ");

			 lcd_print_line3("        ");
			 char str__[8]= {0};
			 snprintf(str__, sizeof(str__), "%.2f", self->litre_price);
			 lcd_print_line3(str__);

//			 ep1a_priceChangeFlag1 = 1;

			 return inactive_State;
		}


	 if( (self->keypress_ == 21) && (self->nozzleup_awaitingauth_state_not_timedOut == 0) )
	 {
		   compose_printer();
		   HAL_Delay(1000);
		   self->printer_status = 1;
	 }

	     print__1();   //print the transaction.

//	#if sense_power == 1
//		  	  if(readpwr() == 0)
//			  {
//				   modem_power(DEACTIVATE);
//
//				   //count time elapsed
//				   if (self->shutdown_timer > 120)
//				   {
//					   displayandkeypad_power(DEACTIVATE);   //shutdown... after  2 minutes
//				   }
//			  }
//		  	  else
//		  	  {
//		  		  //clear the timer//
//		  		  self->shutdown_timer = 0;
//		  	  }
//	#endif

//		else if( (t2 > 2000) && (changeLitrePrice2_2 == 1) )
//		{
//			 lcd_print_line12(" self->price ");
//			 lcd_print_line22("change ");
////			 lcd_print_line32("err16  ");
//
//			 return inactive_State;
////			 if(t2 > 6000)
////			 {
////				 t2 = 0;
////				 flow_loss2 = 0;
////			 }
//		}
//		else if( (self->t > 2000) && (self->_auth_v == 1) )
//				{
//					 lcd_print_line1("  no   ");
//					 lcd_print_line2("annount  ");
//					 lcd_print_line3("err17   ");
//					 if(self->t > 6000)
//					 {
//						 self->t = 0;
//						 self->_auth_v = 0;
//					 }
//				}

//		 else if ( ((self->t > 2000) && (self->t <= 3000)) && (self->pump_LitreOverflow == 1) )
//		 {
//			lcd_print_line1(" limit ");
//			//self->t = 0;
//		 }
//		 else if ( ((self->t > 3000) && (self->t <= 4000)) && (self->pump_LitreOverflow == 1) )
//		 {
//			lcd_print_line1("");
//			if(self->t > 4000) self->t = 0;
//		 }

	  	return idle_State;
}



//---------------
eSystemState authorised_nozzledown_State_Handler(void)
{
	  char *endPtr;

	  self->nozzle_bit = 0;
	  self->pump_status_4G = STATUS_AUTHORIZED_NOZZLE_DOWN;


	/* if (self->t > 1000)
		  {
				lcd_print_line1("a n dn");
			  self->t = 0;
		  }*/
	/* int keyboard_val = 2000;   //dummy value...

	    reset_timer(timeout_picknozzle);
    	start_timer(timeout_picknozzle); */

	 //initialise the fuel and self->price variables

	     if(index_ >= 1)
		  {
			  self->key_value = strtof(self->keyboard_entry, &endPtr);
		  }
		 else
		 {
			  self->key_value = 0.0;
		 }

	   // self->key_value = 5000.000;
	    //initialise the fuel and self->price variables
	      if (self->sellmode == P)
		  {
			  self->target_pulser = price2pulser(self->key_value);  //calculate pulse frm self->price.
		  }
	      else  //self->amt was selected.
	      {
	    	  self->target_pulser = amt2pulser(self->key_value);   //calculate pulse frm self->amt.
	      }

	/*
    	if (self->sellmode == P)  //self->price was selected
    	{
    	   self->target_pulser = price2pulser(keyboard_val); //calculate pulse frm self->price.
    	}
    	  else      //self->amt was selected.
    	{
    	   self->target_pulser = amt2pulser(keyboard_val); //calculate pulse frm self->amt.
    	}
    	self->filling = 1;
   */

	    //  self->target_pulser = price2pulser(5000);

	  self->price  = 0.0;
	  self->amt = 0.0;

	  self->amt_middle_tmin3 = 0.0;
	  self->amt_middle_tmin2 = 0.0;
	  self->amt_middle_tmin1 = 0.0;
	  self->amt_middle = 0.0;

	  self->amt_real_tmin3 = 0.0;
	  self->amt_real_tmin2 = 0.0;
	  self->amt_real_tmin1 = 0.0;
	  self->amt_real = 0.0;

	  self->current_pulser = 0;
	  self->overall_currentPulser = 0;

	 // int cnv = 0;
	 // snprintf(str_, sizeof(str_), "%.2f", self->price); lcd_print_line1(str_);
	 // snprintf(str_, sizeof(str_), "%.2f", self->amt);   lcd_print_line2(str_);

   if(self->t > 300)
   {
	  lcd_print_line1(" Pic    ");
	  lcd_print_line2("No22le  ");
	  char str__[8]= {0};
	  snprintf(str__, sizeof(str__), "%.2f", self->litre_price); lcd_print_line3(str__);
      self->t = 0;
   }

	if(self->display_overflow == 1)
	{
		self->display_overflow = 0;
		firstTime_display_overflow1 = 0;
	}

	return authorised_nozzledown_State;
}


//---------------
eSystemState  nozzleup_waitingforauthState_Handler(void)
{
   	if (self->settings_stream1[0].mode == MANUAL_MODE)
	{
		//send nozzleup command only in MANUAL mode
		return authorised_nozzleup_State;     //idle_State;
	}
   	else
   	{
   		// AUTO mode...
   		if (controller_authorise())    // authed by controller...
   		{

   			//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[

   			//Amount, Vol., and Alarm cleared
			//Light switched on
			//Preset-Vol Cleared to default value
			//Display cleared

   			//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

   			if(self->t > 2400) self->t = 0;

//   			 if ( ((self->t > 400) && (self->t <= 900)) && (self->nozzleup_awaitingauth_state_not_timedOut == 1) )
   			 if ( (self->t > 400) && (self->t <= 900) ) //&& (self->nozzleup_awaitingauth_state_not_timedOut == 1) )
			 {
				 clr_screen1();
				 lcd_print_line1("self->auth    ");
			 }
			 else if ( (self->t > 900) && (self->t <= 1400) ) //&& (self->nozzleup_awaitingauth_state_not_timedOut == 1) )
			 {
				lcd_print_line1("auth_ ");
			 }
			 else if ( (self->t > 1400) && (self->t <= 1900) ) //&& (self->nozzleup_awaitingauth_state_not_timedOut == 1) )
			 {
				lcd_print_line1("auth__ ");
			 }
			 else if ( (self->t > 1900) && (self->t <= 2400) ) //&& (self->nozzleup_awaitingauth_state_not_timedOut == 1) )
			 {
				lcd_print_line1("auth___ ");
			 }


//   			return  authorised_nozzleup_State;
   		}
   		else
   		{
   			self->nozzleup_awaitingauth_state_not_timedOut = 1;


   			//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[

   			//Amount, Vol., and Alarm cleared
			//Light switched on
			//Preset-Vol Cleared to default value
			//Display cleared

   			//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

   			if(self->t > 2400) self->t = 0;

   			 if ( ((self->t > 400) && (self->t <= 900)) && (self->nozzleup_awaitingauth_state_not_timedOut == 1) )
			 {
				 clr_screen1();
				 lcd_print_line1("self->auth    ");
			 }
			 else if ( ((self->t > 900) && (self->t <= 1400)) && (self->nozzleup_awaitingauth_state_not_timedOut == 1) )
			 {
				lcd_print_line1("auth_ ");
			 }
			 else if ( ((self->t > 1400) && (self->t <= 1900)) && (self->nozzleup_awaitingauth_state_not_timedOut == 1) )
			 {
				lcd_print_line1("auth__ ");
			 }
			 else if ( ((self->t > 1900) && (self->t <= 2400)) && (self->nozzleup_awaitingauth_state_not_timedOut == 1) )
			 {
				lcd_print_line1("auth___ ");
			 }
   		}
   	}
	return nozzleup_waitingforauth_State;
}

eSystemState authorised_nozzleup_State_Handler(void)
{
	float sellPrice_max_pump,
		  sellPrice_max_dpp;

	static int8_t firstTime = 1,
				  firstTime_1 = 1;
//				  firstTime_nozz = 1;

	char *endPtr;

	static uint16_t transact = 0;

	float key_value_ = 0.00;

	int8_t pkey = 0;

	self->pump_status = STATUS_AUTH;

	self->slowFlow_startThreshold = (fast_flow_threshold1 * self->settings_stream2[0].valve_salesStart);
	self->slowFlow_endThreshold = (fast_flow_threshold1 * self->settings_stream2[0].valve_salesEnd);


	if (self->stop_flag == 1)   //if stop key is pressed
	{
 		self->filling = 0,
 		self->nozzle_bit = 0;

		self->stop_flag = 0;
		stop_flow1(); //send_solenoid(1);  //stop solenoid.

		//--------------------------------------------------------
		dpFlag = 0;
		error_clr_flag = 1;
		 index_ = 0;
		 _index = 0;
		 for(uint8_t i = 0; i < 9; i++)
		 {
		   self->keypad_pw_xter[i] = 0;
		   self->keyboard_entry[i] = 0;   //clear the buffer
		 }

		 for(uint8_t i = 0; i <= 8; i++)
		 {
			 self->keyboard[i] = 0;
		 }
		 //--------------------------------------------------------------------
		 if (self->sellmode == P)
		 {
			  write_v(3, "P     0");  //send_keypad("p    ");  //5 xters  lafeng..
		 }
		 else if(self->sellmode == L)
		 {
			  write_v(3, "L     0");  //send_keypad("l    ");  //5 xters lafeng
		 }
	//	 else if(self->sellmode == V)
	//	 {
	//		  write_v(3, "v    0");  //send_keypad("p    ");  //5 xters  lafeng..
	//	 }
		 keypad_print(self->keyboard);


//		 pump_status_ = STATUS_FILLING_COMP;

		 if (self->settings_stream1[0].mode == AUTO_MODE)
		 {

			 ///////////////////////////////////////////////////
			 ///////// SIGNALS GO ABOUT NOZZLE STATUS //////////

	//		 self->status_change_noz = 1;
			 self->nozzle_out = false;

			 ///////////////////////////////////////////////////
		 }


		 if (self->settings_stream1[0].mode == MANUAL_MODE)
		 {
			 return idle_State;
		 }

	}

	if( firstTime_nozz1 == 1)
	{
		firstTime_nozz1 = 2;
		self->t = 0;

		return authorised_nozzleup_State;
	}
	else if(firstTime_nozz1 == 2)
	{
		if (self->t <= 1000)
		{
			lcd_print_line1("88888888");
			lcd_print_line2("88888888");
			lcd_print_line3("888888");
		}
		else if(self->t > 1000)
		{
			lcd_print_line3("        ");

			firstTime_nozz1 = 0;
			self->current_pulser = 0;
			self->currentPulser_recovered = 0;
			targetPulser_recovered1 = 0;
		}

		return authorised_nozzleup_State;
	}

	lcd_print_line3("        ");
	char str__[8]= {0};
	snprintf(str__, sizeof(str__), "%.2f", self->litre_price);
	lcd_print_line3(str__);

	self->nozzle_bit = 1;
	self->stop_fueling_bit = 0;

    self->pump_status_4G = STATUS_AUTHORIZED_NOZZLE_UP;

 	//initialise the fuel and self->price variables
	//int keyboard_val = 2000;   //dummy value...
	//clear pulsercount in the prev. state.....

	 sellPrice_max_dpp = sellPrice_max_dp(self->dp_amount);

//	 sellPrice_max_dpp = 10250;

	 self->display_minimumCentilitrePrice = (self->display_minimumCentilitre * self->litre_price);

	 if(self->litre_price == 0)
	 {
		self->_litre_price = 1;
		return idle_State;
	 }
	 else if(self->pump_max_litres == 0)
	 {
		 self->_pump_max_litres = 1;
		 return idle_State;
	 }
	 else if ( (self->change_p == 1) && (self->auth_p == 0) )
	 {
		self->_auth_p = 1;
		return idle_State;
	 }
	 else if ( (self->change_v == 1) && (self->auth_v == 0) )
	 {
		self->_auth_v = 1;
		return idle_State;
	 }
	 else if(self->opmode == MANUAL_MODE)
	 {
		 if(index_ >= 1)
		 {
			  self->key_value = strtof(self->keyboard_entry, &endPtr);
//
//			  half_litre1 = (0.5 * self->litre_price);

//		  //initialise the fuel and self->price variables

//		  //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA//
//		  //======================= AUTOMATED SALES TEST ==========================//
//
//		  index_ = 1;
//		  firstTime_idleState1 = 1;
//
//		  //UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU//
//
//
//		  if (index_ >= 1)
//		  {
//				self->key_value = strtof(self->keyboard_entry, &endPtr);
//	//					self->key_value = strtod(self->keyboard_entry, NULL);
//
//				//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA//
//				//======================= AUTOMATED SALES TEST ==========================//
//
//				uint16_t tk_int_;
//				uint32_t tk_int;
//				uint64_t tk_;
//
//				static uint8_t salemode = 0;
//
//				if(salemode == 0)
//					salemode = 1;
//				else
//					salemode = 0;
//
//				generate_4Rand :
//
//					HAL_RNG_GenerateRandomNumber(&hrng, &tk_int);
//
//					if(salemode == 1)
//					{
//						tk_int_ = (uint16_t)tk_int;
//
//						if( (tk_int_ < 10) || (tk_int_ > 100) )
//						{
//							goto generate_4Rand;
//						}
//
//						self->key_value = (float)tk_int_ / 10;
//
//	//							self->key_value = 1.0;
//
//						self->sellmode = L;
//					}
//					else if(salemode == 0)
//					{
//						if( (tk_int < 1000) || (tk_int > 10000) )
//						{
//							goto generate_4Rand;
//						}
//
//						self->key_value = (float)tk_int / 10;
//
//	//							self->key_value = 1050.0;
//
//						self->sellmode = P;
//					}
//
//				//UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU//



				if (self->sellmode == P)
				{
					  sellPrice_max_pump = (self->litre_price * self->pump_max_litres);

					  if( (self->key_value > sellPrice_max_pump) || (self->key_value > sellPrice_max_dpp) )  // || (self->key_value < half_litre1) )
					  {
						  if(self->key_value > sellPrice_max_pump)
						  {
							  self->key_value = sellPrice_max_pump;
							  self->pump_LitreOverflow = 1;
						  }
						  else if(self->key_value > sellPrice_max_dpp)
						  {
							  self->key_value = sellPrice_max_dpp;
						      self->display_overflow = 1;
						  }
					  }

					  self->target_pulser = price2pulser(self->key_value);  //calculate pulse frm self->price.

					  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			          //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//

					  if( (self->pump_LitreOverflow == 1) || (self->display_overflow == 1) )
					  {
						 self->sales_type = UNPROGRAMMED_SALE;
					  }
					  else
					  {
						 self->sales_type = PRICE_PROGRAMMED;
					  }

					  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				}

				else  //self->amt was selected.
				{
					key_value_ = (sellPrice_max_dpp / self->litre_price);

					if( (self->key_value > self->pump_max_litres) || (self->key_value > key_value_) )
					 {
						 if(self->key_value > self->pump_max_litres)
						 {
							  self->key_value = self->pump_max_litres;
							  self->pump_LitreOverflow = 1;
						 }
						 else if(self->key_value > key_value_)
						 {
							  self->key_value = key_value_;
							  self->display_overflow = 1;
						 }
					 }

					 self->target_pulser = amt2pulser(self->key_value);   //calculate pulse frm self->amt.

					 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			         //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//

					  if( (self->pump_LitreOverflow == 1) || (self->display_overflow == 1) )
					  {
						 self->sales_type = UNPROGRAMMED_SALE;
					  }
					  else
					  {
						 self->sales_type = LITRE_PROGRAMMED;
					  }

					  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				}
		  }
		  else
		  {
			  self->key_value = (self->litre_price * self->pump_max_litres);

			  if (self->sellmode == P)
			  {
				  if(self->key_value > sellPrice_max_dpp)
				  {
					  self->key_value = sellPrice_max_dpp;
				  }
//				  else if(self->key_value > key_value_)
//				  {
//					  self->key_value = key_value_;
//				  }

				  self->target_pulser = price2pulser(self->key_value);  //calculate pulse frm self->price.
			  }
			  else if (self->sellmode == L)
			  {
				  self->key_value = (sellPrice_max_dpp / self->litre_price);

					if(self->key_value > self->pump_max_litres)
					{
						  self->key_value = self->pump_max_litres;
					}
//					else if(self->key_value > key_value_)
//					{
//						  self->key_value = key_value_;
//					}

					self->target_pulser = amt2pulser(self->key_value);   //calculate pulse frm self->amt.
			  }

			  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	          //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//

			   self->sales_type = UNPROGRAMMED_SALE;

			  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		   }

	 }
	 else if(self->opmode == AUTO_MODE)
	 {
		  if (self->change_p == 1)
		  {
			  sellPrice_max_pump = (self->litre_price * self->pump_max_litres);
//			  sellPrice_max_dpp = (sellPrice_max_dpp / self->litre_price);

			  self->change_p = 0;      //reset tbe flag.
			  index_ = strlen(self->keyboard_entry);

			  if (index_ >= 1)    //Programmed Sale
			  {
					self->key_value = strtof(self->keyboard_entry, &endPtr);

				  	if (self->sellmode == L)
					{
				  		self->key_value_sellmodeL = 1;
				  		self->key_value_original = self->key_value;

//				  		self->sales_type = LITRE_PROGRAMMED;

				  		//======== Convert to Price-Sale =======//
				  		self->key_value = (self->key_value * self->litre_price);
					}

				  	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
					//======= Ensure keyed value doesn'self->t exceed the allowable sale from the controller =======//

					if(self->key_value > self->auth_p)
					{
						self->key_value = self->auth_p;

						//can'self->t honour the programmed sale again
						self->authorizedSale_overflow = 1;
					}

					//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//


					if( (self->key_value > sellPrice_max_pump) || (self->key_value > sellPrice_max_dpp) )     // || (self->key_value < 0.5) )
					{
						if(self->key_value > sellPrice_max_pump)
						{
							self->key_value = sellPrice_max_pump;
							self->pump_LitreOverflow = 1;
						}
						else if (self->key_value > sellPrice_max_dpp)
						{
							 self->key_value = sellPrice_max_dpp;
							 self->display_overflow = 1;
						}
					}

					self->target_pulser = price2pulser(self->key_value);  //calculate pulse frm self->price.


					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//

					if( (self->key_value_sellmodeL == 1) && (self->authorizedSale_overflow == 0) && (self->pump_LitreOverflow == 0) && (self->display_overflow == 0) )
					{
						self->key_value = self->key_value_original;
					}

					if (self->sellmode == L)
					{
						self->sales_type = LITRE_PROGRAMMED;
					}
					else if (self->sellmode == P)
					{
						self->sales_type = PRICE_PROGRAMMED;
					}

					if( (self->authorizedSale_overflow == 1) || (self->pump_LitreOverflow == 1) || (self->display_overflow == 1) )
					{
						self->sales_type = UNPROGRAMMED_SALE;
					}

					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				}  //End of self->keyboard Input Check


			  /* UNPROGRAMMED SALE */
			  else
			  {
				  if( (self->auth_p > sellPrice_max_pump) || (self->auth_p > sellPrice_max_dpp) )
				  {
					  if(self->auth_p > sellPrice_max_pump)
					  {
						  self->auth_p = sellPrice_max_pump;
						  self->pump_LitreOverflow = 1;
					  }

					  else if(self->auth_p > sellPrice_max_dpp)
					  {
						  self->auth_p = sellPrice_max_dpp;
						  self->display_overflow = 1;
					  }
				  }

				  sprintf(self->keyboard_entry,"%2f", self->auth_p);

				  self->key_value = strtof(self->keyboard_entry, &endPtr);

				  self->target_pulser = price2pulser(self->key_value);  //calculate pulse frm self->price.


				  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		          //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//

				  if( (self->pump_LitreOverflow == 1) || (self->display_overflow == 1) )
				  {
					 self->sales_type = UNPROGRAMMED_SALE;
				  }
				  else
				  {
					 self->sales_type = PRICE_PROGRAMMED;
				  }

				  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			  }
		  }
		 //-----------------------------------------


		  //authorise volume...
		  else if (self->change_v == 1)
		  {
//			  sellPrice_max_dpp = (sellPrice_max_dpp / self->litre_price);
			  key_value_ = (sellPrice_max_dpp / self->litre_price);

			  self->change_v = 0;      //reset tbe flag.
			  index_ = strlen(self->keyboard_entry);

			#if !defined(AUTO_SALE_TEST)

			  if (index_ >= 1)
			  {
					self->key_value = strtof(self->keyboard_entry, &endPtr);

		   #else

			  //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA//
			  //======================= AUTOMATED SALES TEST ==========================//

			  index_ = 1;
			  firstTime_idleState1 = 1;

			  //UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU//


			  if (index_ >= 1)
			  {
					self->key_value = strtof(self->keyboard_entry, &endPtr);
//					self->key_value = strtod(self->keyboard_entry, NULL);

					//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA//
				    //======================= AUTOMATED SALES TEST ==========================//

//					uint16_t tk_int_;
					uint32_t tk_int;
					uint64_t tk_;

					static uint8_t salemode = 0;

					if(salemode == 0)
						salemode = 1;
					else
						salemode = 0;

					generate_4Rand1 :

						HAL_RNG_GenerateRandomNumber(&hrng, &tk_int);

						if(salemode == 1)
						{
							tk_int_ = (uint16_t)tk_int;

							if( (tk_int_ < 10) || (tk_int_ > 150) )   //between 1L and 100L, sustained by the subsequent divisor 10
							{
								goto generate_4Rand1;
							}

							self->key_value = (float)tk_int_ / 10;

//							self->key_value = 1.0;

							self->sellmode = L;
						}
						else if(salemode == 0)
						{
//							if( (tk_int < 1000) || (tk_int > 10000) )

							tk_int_ = (uint16_t)tk_int;

							if( (tk_int_ < 2000) || (tk_int_ > 30000) )
							{
								goto generate_4Rand1;
							}

							self->key_value = (float)tk_int_ / 10;

//							self->key_value = 1050.0;

							self->sellmode = P;
						}

					   #if DEBUG1
						char str1 [50] = {0};
						sprintf(str1,
								    "Transaction [Side-A] : #%d\n\n",
									transact++);

						HAL_UART_Transmit(&huart3, str1, strlen((char*)str1), HAL_MAX_DELAY);

						HAL_Delay(50);

						if(self->sellmode == P)
						{
							sprintf(str1,
										"Programmed Sale : #%0.2f\n\n",
										self->key_value);
						}
						else if(self->sellmode == L)
						{
							sprintf(str1,
										"Programmed Sale : %0.2f Litres\n\n",
										self->key_value);
						}

						HAL_UART_Transmit(&huart3, str1, strlen((char*)str1), HAL_MAX_DELAY);

						HAL_Delay(50);

				   	   #endif

					//UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU//

				#endif //#ifndef AUTO_SALE_TEST

				  	if (self->sellmode == P)
					{
				  		self->key_value_sellmodeP = 1;
				  		self->key_value_original = self->key_value;

				  		//======== Convert to Volume Equivalent =======//
						self->key_value = (self->key_value / self->litre_price);
					}

				  	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
				  	//======= Ensure keyed value doesn'self->t exceed the allowable sale from the controller =======//

					if(self->key_value > self->auth_v)
					{
						self->key_value = self->auth_v;

						//can'self->t honour the programmed sale again
						self->authorizedSale_overflow = 1;
					}

					//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//

//					key_value_ = (sellPrice_max_dpp / self->litre_price);

					if( (self->key_value > self->pump_max_litres) || (self->key_value > key_value_) )
					{
						if(self->key_value > self->pump_max_litres)
						{
							self->key_value = self->pump_max_litres;
							self->pump_LitreOverflow = 1;
						}
						else if (self->key_value > key_value_)
						{
							 self->key_value = key_value_;
							 self->display_overflow = 1;
						}
					}

					self->target_pulser = amt2pulser(self->key_value);   //calculate pulse frm self->amt.

				#if defined (INCIDENT_RECORD)

				    self->incidentRecord[0].timestamp_event = RtcToInt(2019);

					if (self->key_value_sellmodeP == 1)
					{
						self->incidentRecord[0].programmed_sale = self->key_value_original;
					}
					else
						self->incidentRecord[0].programmed_sale = self->key_value;

				   self->incidentRecord[0].nozzleState_change_ = NOZZLE_PICKUP;
				   save_programmedSaleEvent_fram(side_a);

				   programmed_sale1 = 1;

				#endif


					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
					if( (self->key_value_sellmodeP == 1) && (self->authorizedSale_overflow == 0) && (self->pump_LitreOverflow == 0) && (self->display_overflow == 0) )
					{
						self->key_value = self->key_value_original;
					}

					if (self->sellmode == L)
					{
						self->sales_type = LITRE_PROGRAMMED;
					}
					else if (self->sellmode == P)
					{
						self->sales_type = PRICE_PROGRAMMED;
					}

					if( (self->authorizedSale_overflow == 1) || (self->pump_LitreOverflow == 1) || (self->display_overflow == 1) )
					{
						self->sales_type = UNPROGRAMMED_SALE;
					}

					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				}  //End of self->keyboard Input Check


			    /* UNPROGRAMMED SALE */
			    else
			    {
//			    	key_value_ = (sellPrice_max_dpp / self->litre_price);

			    	if( (self->auth_v > self->pump_max_litres) || (self->auth_v > key_value_) )
			    	{
						  if(self->auth_v > self->pump_max_litres)
						  {
							  self->auth_v = self->pump_max_litres;
							  self->pump_LitreOverflow = 1;
						  }

						  else if (self->auth_v > key_value_)
						  {
							  self->auth_v = key_value_;
							  self->display_overflow = 1;
						  }
			    	}

				  sprintf(self->keyboard_entry,"%2f", self->auth_v);

				  self->key_value = strtof(self->keyboard_entry, &endPtr);

				  self->target_pulser = amt2pulser(self->key_value);   //calculate pulse frm self->amt.

				  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		          //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//

				  if( (self->pump_LitreOverflow == 1) || (self->display_overflow == 1) )
				  {
					 self->sales_type = UNPROGRAMMED_SALE;
				  }
				  else
				  {
					 self->sales_type = LITRE_PROGRAMMED;
				  }

				  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			   }

		 }
//		 else
//		 {
//			  self->key_value = sellPrice_max_dpp;
//		      self->display_overflow = 1;
//			  self->target_pulser = price2pulser(self->key_value);  //calculate pulse frm self->price.
//		 }
	   }
//============================================================================
//============================================================================
//	  if (authorise_flag == 1)
//	   {
//		  self->target_pulser = price2pulser(5000);
//		  self->key_value  = 5000;
//		  authorise_flag == 0;
//	   }
//============================================================================
//============================================================================

	  	  self->price  = 0.0;
		  self->amt  = 0.0;

		  self->amt_middle_tmin3 = 0.0;
		  self->amt_middle_tmin2 = 0.0;
		  self->amt_middle_tmin1 = 0.0;
		  self->amt_middle = 0.0;

		  self->amt_real_tmin3 = 0.0;
		  self->amt_real_tmin2 = 0.0;
		  self->amt_real_tmin1 = 0.0;
		  self->amt_real = 0.0;

		  if (self->key_value == 0)
		  {
			   self->target_pulser = 0;
		  }
		  else
			  self->pulser_rem = self->target_pulser - self->current_pulser;

		  if( (self->target_pulser == 0) || (self->key_value == 0) )
		  {
			  self->hardwareError_flag = 1;

//			  make_string(P, dp(0.00, self->dp_amount));
//			  make_string(L, dp(0.00, self->dp_vol));
		  }

		  if(firstTime_1 == 1)
		  {
		 	motor_tmr1 = 0;
		 	firstTime_1 = 0;
		  }

		  if (motor_tmr1 >= 1000)    //2000)
		  {
				drive_motor1(ACTIVATE);

				if(self->settings_stream1[0].pump_type_ != LAFENG)
				{
					 drive_slow_sole1(ACTIVATE);
					 drive_fast_sole1(DEACTIVATE);
				}
				else
				{
					drive_slow_sole1(DEACTIVATE);     // DEACTIVATE here actually means ACTIVATE
					drive_fast_sole1(ACTIVATE);		  // ACTIVATE here actually means DEACTIVATE
				}


			  if( (self->lock_clr == 0) && (firstTime_1 == 0) ) ///   to activate this section once.
			  {
					 self->overall_currentPulser = 0;
					 clr_pulser1();    //clear hardware pulser
					 self->current_pulser = 0;

					 //self->current_pulser = __HAL_TIM_GET_COUNTER(self->flow_counter);
					 self->lock_clr = 1;
					 self->filling = 1;

					 firstTime = 1;
					 firstTime_1 = 1;
	//				 motor_tmr1 = 0;
			  }
		  }

//		 pkey = read_keypad();
//
//		 if(firstTime_1 == 1)
//		  {
//		 	motor_tmr1 = 0;
//		 	firstTime_1 = 0;
//		 	HAL_Delay(2000);
//		 	drive_motor1(ACTIVATE);
////				drive_slow_sole1(ACTIVATE);
//			drive_fast_sole1(DEACTIVATE);
//		  }
//
//		 else if ( (firstTime_1 == 0) && (motor_tmr1 >= 2000) )
//		  {
////				drive_motor1(ACTIVATE);
//////				drive_slow_sole1(ACTIVATE);
////				drive_fast_sole1(DEACTIVATE);
//
//			  if( (self->lock_clr == 0) && (firstTime_1 == 0) && (pkey == 'D') ) //  to activate this section once.
//				  	  	  	  	  	  	  	  	  	  	  	  	  	  	   // 'D' is an Enter Key
//			  {
//					 drive_slow_sole1(ACTIVATE);
//
//					 self->current_pulser = 0;
//					 clr_pulser();    //clear hardware pulser
//
//					 //self->current_pulser = __HAL_TIM_GET_COUNTER(self->flow_counter);
//					 self->lock_clr = 1;
//					 self->filling = 1;
//
//					 firstTime = 1;
//					 firstTime_1 = 1;
//	//				 motor_tmr1 = 0;
//			  }
//		  }


//		  slow_flow1();



		 // int cnv = 0;
	     //char temp[10] = {0};
		 //snprintf(temp , sizeof(temp), "%.2f", self->price);  //lcd_print_line1(self->upper);
         //snprintf(self->middle, sizeof(self->middle), "%.2f", self->amt);   //lcd_print_line2(lower);

	  	  make_string(P, self->price);
		  make_string(L, self->amt);

//		 if (self->t > 300)
		 if (self->t > 100)
		 {
//				lcd_print_line1(self->upper);
//				lcd_print_line2(self->middle);

				if(self->settings_stream1[0].display_format == PL)
				{
					 lcd_print_line1(self->upper);
					 lcd_print_line2(self->middle);
				 }
				 else if(self->settings_stream1[0].display_format == LP)
				 {
					  lcd_print_line1(self->middle);
					  lcd_print_line2(self->upper);
				 }

				char str__[8]= {0};
				snprintf(str__, sizeof(str_), "%.2f", self->litre_price);
				lcd_print_line3(str__);
				self->t = 0;

							//trigger first pulser...
				if(firstTime == 1)
				{
					firstTime = 2;
					#if (_USE_SOFT_PULSER == 1)
						 //  pulser_new += 1;
						   self->current_pulser++;
					#endif

				}
				else if (firstTime == 2)
				{
					firstTime = 3;

					#if (_USE_SOFT_PULSER == 1)
							 //  pulser_new += 1;
							self->current_pulser++;
					#else
							self->current_pulser = 0;
							self->overall_currentPulser = 0;
							clr_pulser1();    //clear hardware pulser
					#endif
				}
				else if (firstTime == 3)
				{
					#if (_USE_SOFT_PULSER == 1)
						 //  pulser_new += 1;
					self->current_pulser++;
					#else
						 // pulser_new = __HAL_TIM_GET_COUNTER(self->flow_counter);
						   self->current_pulser = __HAL_TIM_GET_COUNTER(self->flow_counter);
						   self->overall_currentPulser = self->current_pulser;
					#endif
				}


//			#if (_USE_SOFT_PULSER == 1)
//					 //  pulser_new += 1;
//					   self->current_pulser++;
//			#else
//					 // pulser_new = __HAL_TIM_GET_COUNTER(self->flow_counter);
//					   self->current_pulser = __HAL_TIM_GET_COUNTER(self->flow_counter);
//			#endif

	 	   }

		   #if (_USE_SOFT_PULSER == 1)
		 	 pulser_new = self->current_pulser;
		   #endif

		   if (firstTime == 3)
		       pulser_new = self->current_pulser;
//=====================================================================
 		// self->filling = 1;
		// send_pump(ACTIVATE);  //pump on...


//  if(operating_side == side_a)
//	{
//		self->working_volTotaliser = totaliser_vol1;
//		self->working_volTotaliser_cal = self->totaliser_vol_cal;
//		self->working_amtTotaliser = self->totaliser_amt;
//		self->working_amtTotaliser_cal = self->totaliser_amt_cal;
////	}
////  else
////   {
////  		self->working_volTotaliser = totaliser_vol2;
////  		self->working_volTotaliser_cal = totaliser_vol2c;
////  		self->working_amtTotaliser = totaliser_amt2;
////  		self->working_amtTotaliser_cal = totaliser_amt2c;
////  	}
//
//  r_volTotaliser1 = floor( self->working_volTotaliser_cal );
//  old_r_volTotaliser1 = r_volTotaliser1;

//  r_amtTotaliser = floor(self->working_amtTotaliser_cal);
//  old_r_amtTotaliser = r_amtTotaliser;

 // slow_flow1();

	   ////////////////////////////////////////////////////////////////////////////////////////////////////////
	   // DO A TOTALIZER DATA RETRIEVAL TO GUARD AGAINST WORKING WITH ANY POSSIBLE CORRUPT TOTALIZER RAM VALUE
	   ////////////////////////////////////////////////////////////////////////////////////////////////////////
	   while(retrieve_totaliser_fram(operating_side) != OK)   //If it fails, retry 5X
	   {
			static uint8_t try = 0;
			if(try++ >= 5)
			{
				while(retrieve_totaliser_eeprom(operating_side) != OK)
				{
					if(try++ >= 10)
					{
						clear_totaliser_fram(operating_side);
						clear_totaliser_eeprom(operating_side);
						try = 0;
						break;
					}

				}
				try = 0;
				break;
			}
		}
	   //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
//	    self->totaliser_vol_cal = 21474836.00;  //1000000.00; // 9000000; //21474836;   // 5000000;
//	    self->totaliser_vol = 0.0;  //21474836;    // 5000000;
	   //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//

//		self->working_volTotaliser = totaliser_vol1;
//		self->working_volTotaliser_cal = self->totaliser_vol_cal;
//
//		self->working_amtTotaliser = self->totaliser_amt;
//		self->working_amtTotaliser_cal = self->totaliser_amt_cal;
//
//		self->running_volTotaliser = totaliser_vol1;
//		self->running_volTotaliser_tmin1 = totaliser_vol1;
//		self->running_volTotaliser_tmin2 = totaliser_vol1;
//		self->running_volTotaliser_tmin3 = totaliser_vol1;
//
//		self->running_volTotaliser_cal = self->totaliser_vol_cal;
//		self->running_volTotaliser_cal_tmin1 = self->totaliser_vol_cal;
//		self->running_volTotaliser_cal_tmin2 = self->totaliser_vol_cal;
//		self->running_volTotaliser_cal_tmin3 = self->totaliser_vol_cal;

	    self->mechTotalizer_ = get_fractional_part1(self->totaliser_vol_cal);
		self->working_volTotaliser = scale_to_range1(self->totaliser_vol);
		self->working_volTotaliser_cal = scale_to_range1(self->totaliser_vol_cal);

		self->working_amtTotaliser = self->totaliser_amt;
		self->working_amtTotaliser_cal = self->totaliser_amt_cal;

		self->running_volTotaliser = self->working_volTotaliser;
		self->running_volTotaliser_tmin1 = self->working_volTotaliser;
		self->running_volTotaliser_tmin2 = self->working_volTotaliser;
		self->running_volTotaliser_tmin3 = self->working_volTotaliser;

		self->running_volTotaliser_cal = self->working_volTotaliser_cal;
		self->running_volTotaliser_cal_tmin1 = self->working_volTotaliser_cal;
		self->running_volTotaliser_cal_tmin2 = self->working_volTotaliser_cal;
		self->running_volTotaliser_cal_tmin3 = self->working_volTotaliser_cal;

		self->price  = 0.0;
		self->amt  = 0.0;

		self->amt_middle_tmin3 = 0.0;
		self->amt_middle_tmin2 = 0.0;
		self->amt_middle_tmin1 = 0.0;
		self->amt_middle = 0.0;

		self->amt_real_tmin3 = 0.0;
		self->amt_real_tmin2 = 0.0;
		self->amt_real_tmin1 = 0.0;
		self->amt_real = 0.0;




//		self->running_volTotaliser_cal_tmin1 = self->totaliser_vol_cal;

//		self->working_amtTotaliser_cal = self->totaliser_amt_cal;

//		self->running_volTotaliser_cal_tmin2 = self->totaliser_vol_cal;


		r_volTotaliser1 = floor(scale_to_original1(self->working_volTotaliser_cal));

//		currentValue_tv1 = self->amt_middle;
//		previousValue_tv1 = currentValue_tv1;

		self->previous_totaliserVol_cal = self->totaliser_vol_cal;

//		self->previous_totaliserVol_cal = self->amt_middle;

//		self->mechTotalizer = (self->mechTotalizer_ + self->amt_middle);

		#ifdef PULSER_BASED_TV
				self->mechTotalizer = (self->mechTotalizer_ + pulser2amt(self->current_pulser));
		#else
				self->mechTotalizer = (self->mechTotalizer_ + self->amt_middle);
		#endif

		self->mech_totalizer = (int) self->mechTotalizer;
		self->mech_totalizer_old = self->mech_totalizer;



//		if (self->totaliser_vol_cal - self->previous_totaliserVol_cal >= THRESHOLD_TV)
//		{
//			self->previous_totaliserVol_cal = self->totaliser_vol_cal;
//
//			save_totaliserFrequent_fram(side_a);
//			save_totaliserFrequent_eeprom(side_a);
//		}

//		self->running_volTotaliser_cal_tmin3 = self->totaliser_vol_cal;

		old_r_volTotaliser1 = r_volTotaliser1;

//		self->running_volTotaliser_tmin3 = totaliser_vol1;

	return authorised_nozzleup_State;
}



//------------------
eSystemState filling_State_Handler(void)
{
	//extern uint32_t pulser_new;

	if(self->filling_mamo_flag == 1)
	{
		if(mamo_fillingInfo_send1 == 0)
		{
			mamo_reached_flag1 = 1;
		}

	    return filling_State;
	}

   if (self->hardwareError_flag == 1)
   {
	   return filling_State;
   }

	float temp = 0.0;

	static int8_t self->flow_flag = 1;

	static int32_t self->current_pulser_ = 0,
					   self->old_pulser_ = 0;

	self->pump_status_4G = STATUS_FILLING_;
	self->pump_status = STATUS_FILLING;

    self->filling  = 1;

    firstTime_nozz1 = 1;

    self->lock_clr = 0;

	  if(self->target_pulser > 0)
	  {
		 //	self->pulser_rem = self->target_pulser - self->current_pulser; //now handled in
														 //interrupt
	  }

//================================================================

//	#if !defined (DEV_MODE)
	#if sense_battery == 1
	  	if(batteryStatus == NO_BATTERY)
		{
			  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
			  HAL_Delay(200);
			  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
			  self->filling = 0;
			  stop_flow1();
			  get_time();
			  do_calcs();
			  update_info();

			  save_totaliser_fram(operating_side);
			  save_totaliser_eeprom(operating_side);

			  save_lastSale_fram(operating_side);
			  save_lastSale_eeprom(operating_side);

			  ////////////////////////////////////////////////
			  //---------------------------------------------
			  self->totalizer_saveStatus = SAVED_TO_MAIN_TOTALIZER;
			  save_totaliserFrequent_fram(operating_side);
			  save_totaliserFrequent_eeprom(operating_side);
			  ////////////////////////////////////////////////

			  if(self->settings_stream1[0].mode == AUTO_MODE)
			  {
				 //////////////////////////////////////////////////////////////

				 self->hardwareErrorFlag_source = 1;

				 //////////////////////////////////////////////////////////////
			  }

			  return write_flash_State;
		}
	#endif     //#if sense_battery == 1

//    power outage during self->filling  end transaction...
	#if sense_power == 1
	  if( (readpwr() == 0)||(read_p_pwr() == 0) )
	  {
		  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
		  HAL_Delay(100);
		  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
		  self->filling = 0;
		  stop_flow1();
		  get_time();
		  do_calcs();
		  update_info();
//		  save_volumeTotaliser(operating_side);
//		  save_amountTotaliser(operating_side);
//		  save_lastSale(operating_side);


		  while(retrieve_totaliser_fram_check(operating_side) != OK)   //If it fails, retry 5X
		  {
			  static uint8_t try = 0;
				if(try++ >= 5)
				{
					while(retrieve_totaliser_eeprom_check(operating_side) != OK)
					{
						if(try++ >= 10)
						{
							clear_totaliser_fram(operating_side);
							clear_totaliser_eeprom(operating_side);
							try = 0;
							break;
						}

					}
					try = 0;
					break;
				}
		  }

		  save_totaliser_fram(side_a);
		  save_totaliser_eeprom(side_a);

		  save_lastSale_fram(side_a);
		  save_lastSale_eeprom(operating_side);

		  ////////////////////////////////////////////////
		  //---------------------------------------------
		  self->totalizer_saveStatus = SAVED_TO_MAIN_TOTALIZER;
		  save_totaliserFrequent_fram(operating_side);
		  save_totaliserFrequent_eeprom(operating_side);
		  ////////////////////////////////////////////////


		  if(self->settings_stream1[0].mode == AUTO_MODE)
		  {
			 //////////////////////////////////////////////////////////////

			 self->hardwareErrorFlag_source = 1;

			 //////////////////////////////////////////////////////////////
		  }

		  return write_flash_State;
	  }
	#endif   //#if sense_power == 1

	#if !defined (DEV_MODE)
		if(HAL_GPIO_ReadPin(pulser1_detect_GPIO_Port, pulser1_detect_Pin) == 1 )
		{
			  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
			  HAL_Delay(200);
			  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
			  self->filling = 0;
			  stop_flow1();
			  get_time();
			  do_calcs();
			  update_info();

			  while(retrieve_totaliser_fram_check(operating_side) != OK)   //If it fails, retry 5X
			  {
				  static uint8_t try = 0;
					if(try++ >= 5)
					{
						while(retrieve_totaliser_eeprom_check(operating_side) != OK)
						{
							if(try++ >= 10)
							{
								clear_totaliser_fram(operating_side);
								clear_totaliser_eeprom(operating_side);
								try = 0;
								break;
							}

						}
						try = 0;
						break;
					}
			  }

			  save_totaliser_fram(operating_side);
			  save_totaliser_eeprom(operating_side);
			  save_lastSale_fram(operating_side);
			  save_lastSale_eeprom(operating_side);

			  ////////////////////////////////////////////////
			  //---------------------------------------------
			  self->totalizer_saveStatus = SAVED_TO_MAIN_TOTALIZER;
			  save_totaliserFrequent_fram(operating_side);
			  save_totaliserFrequent_eeprom(operating_side);
			  ////////////////////////////////////////////////


			  if(self->settings_stream1[0].mode == AUTO_MODE)
			  {
				 //////////////////////////////////////////////////////////////

				 self->hardwareErrorFlag_source = 1;

				 //////////////////////////////////////////////////////////////
			  }

			  return write_flash_State;
		}
	#endif   //#if !defined (DEV_MODE)

	if(self->settings_stream1[0].mode == AUTO_MODE)
	{
		if(timer_go >= TIMEOUT_GO)   //if go's timeout is 5sec threshold
		{
			self->go_timeOut = 1;

			HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
			self->filling = 0;
			stop_flow1();
			get_time();
			do_calcs();
			update_info();

			while(retrieve_totaliser_fram_check(operating_side) != OK)   //If it fails, retry 5X
			{
				static uint8_t try = 0;
				if(try++ >= 5)
				{
					while(retrieve_totaliser_eeprom_check(operating_side) != OK)
					{
						if(try++ >= 10)
						{
							clear_totaliser_fram(operating_side);
							clear_totaliser_eeprom(operating_side);
							try = 0;
							break;
						}

					}
					try = 0;
					break;
				}
			 }

			save_totaliser_fram(side_a);
			save_totaliser_eeprom(side_a);

			save_lastSale_fram(side_a);
			save_lastSale_eeprom(operating_side);

		  ////////////////////////////////////////////////
		  //---------------------------------------------
		  self->totalizer_saveStatus = SAVED_TO_MAIN_TOTALIZER;
		  save_totaliserFrequent_fram(operating_side);
		  save_totaliserFrequent_eeprom(operating_side);
		  ////////////////////////////////////////////////


			return write_flash_State;
		}
	}


	if (self->stop_flag == 1)   //if stop key pressed
	{
		self->filling = 0,
		self->nozzle_bit = 0;

		self->stop_flag = 0;
		stop_flow1(); //send_solenoid(1);  //stop solenoid.
		get_time();
        do_calcs();
        update_info();
//        save_volumeTotaliser(operating_side);
//        save_amountTotaliser(operating_side);
//        save_lastSale(operating_side);

        while(retrieve_totaliser_fram_check(operating_side) != OK)   //If it fails, retry 5X
		{
        	static uint8_t try = 0;
			if(try++ >= 5)
			{
				while(retrieve_totaliser_eeprom_check(operating_side) != OK)
				{
					if(try++ >= 10)
					{
						clear_totaliser_fram(operating_side);
						clear_totaliser_eeprom(operating_side);
						try = 0;
						break;
					}

				}
				try = 0;
				break;
			}
		 }

        save_totaliser_fram(side_a);
        save_totaliser_eeprom(side_a);

        save_lastSale_fram(side_a);
        save_lastSale_eeprom(operating_side);

          ////////////////////////////////////////////////
		  //---------------------------------------------
		  self->totalizer_saveStatus = SAVED_TO_MAIN_TOTALIZER;
		  save_totaliserFrequent_fram(operating_side);
		  save_totaliserFrequent_eeprom(operating_side);
		  ////////////////////////////////////////////////


        //--------------------------------------------------------
//        	dpFlag = 0;
//        	error_clr_flag = 1;
//        	 index_ = 0;
//        	 _index = 0;
//        	 for(int i = 0; i < 9; i++)
//        	 {
//        	   self->keypad_pw_xter[i] = 0;
//        	   self->keyboard_entry[i] = 0;   //clear the buffer
//        	 }
//
//        	 for(int i = 0; i <= 8; i++)
//        	 {
//        		 self->keyboard[i] = 0;
//        	 }
//        	 //--------------------------------------------------------------------
//        	 if (self->sellmode == P)
//        	 {
//        		  write_v(3, "P     0");  //send_keypad("p    ");  //5 xters  lafeng..
//        	 }
//        	 else if(self->sellmode == L)
//        	 {
//        		  write_v(3, "L     0");  //send_keypad("l    ");  //5 xters lafeng
//        	 }
//        //	 else if(self->sellmode == V)
//        //	 {
//        //		  write_v(3, "v    0");  //send_keypad("p    ");  //5 xters  lafeng..
//        //	 }
//        	 send_keypad(self->keyboard);

        	 keypad_zerorize();

//        	 pump_status_ = STATUS_FILLING_COMP;

        	 if(self->settings_stream1[0].mode == AUTO_MODE)
        	 {
				 //////////////////////////////////////////////////////////////
				 ///////// SIGNALS GO-CONTROLLER ABOUT NOZZLE STATUS //////////

				 self->status_change_noz = 1;
				 self->nozzle_out = false;

				 self->stopFlag_source = 1;

				 //////////////////////////////////////////////////////////////
    		 }

       return write_flash_State;
	}

	if(self->litre_price == 0)
	{
		stop_flow1(); //send_solenoid(1);  //stop solenoid.
		self->filling = 0;
		do_calcs();
		get_time();
		update_info();
//		save_volumeTotaliser(operating_side);
//		save_amountTotaliser(operating_side);
//		save_lastSale(operating_side);

		while(retrieve_totaliser_fram_check(operating_side) != OK)   //If it fails, retry 5X
		{
			static uint8_t try = 0;
			if(try++ >= 5)
			{
				while(retrieve_totaliser_eeprom_check(operating_side) != OK)
				{
					if(try++ >= 10)
					{
						clear_totaliser_fram(operating_side);
						clear_totaliser_eeprom(operating_side);
						try = 0;
						break;
					}

				}
				try = 0;
				break;
			}
		 }


		save_totaliser_fram(side_a);
		save_totaliser_eeprom(side_a);

		save_lastSale_fram(side_a);
		save_lastSale_eeprom(operating_side);

		  ////////////////////////////////////////////////
		  //---------------------------------------------
		  self->totalizer_saveStatus = SAVED_TO_MAIN_TOTALIZER;
		  save_totaliserFrequent_fram(operating_side);
		  save_totaliserFrequent_eeprom(operating_side);
		  ////////////////////////////////////////////////


		self->_litre_price = 1;

		  if(self->settings_stream1[0].mode == AUTO_MODE)
		  {
			 //////////////////////////////////////////////////////////////

			 self->hardwareErrorFlag_source = 1;

			 //////////////////////////////////////////////////////////////
		  }

		return write_flash_State;
	}
	else if(self->pump_max_litres == 0)
	{
		stop_flow1(); //send_solenoid(1);  //stop solenoid.
		self->filling = 0;
		do_calcs();
		get_time();
		update_info();
//		save_volumeTotaliser(operating_side);
//		save_amountTotaliser(operating_side);
//		save_lastSale(operating_side);

		while(retrieve_totaliser_fram_check(operating_side) != OK)   //If it fails, retry 5X
		{
			static uint8_t try = 0;
			if(try++ >= 5)
			{
				while(retrieve_totaliser_eeprom_check(operating_side) != OK)
				{
					if(try++ >= 10)
					{
						clear_totaliser_fram(operating_side);
						clear_totaliser_eeprom(operating_side);
						try = 0;
						break;
					}

				}
				try = 0;
				break;
			}
		 }


		save_totaliser_fram(side_a);
		save_totaliser_eeprom(side_a);

		save_lastSale_fram(side_a);
		save_lastSale_eeprom(operating_side);

	  ////////////////////////////////////////////////
	  //---------------------------------------------
	  self->totalizer_saveStatus = SAVED_TO_MAIN_TOTALIZER;
	  save_totaliserFrequent_fram(operating_side);
	  save_totaliserFrequent_eeprom(operating_side);
	  ////////////////////////////////////////////////


		self->_pump_max_litres = 1;

		  if(self->settings_stream1[0].mode == AUTO_MODE)
		  {
			 //////////////////////////////////////////////////////////////

			 self->hardwareErrorFlag_source = 1;

			 //////////////////////////////////////////////////////////////
		  }

		return write_flash_State;
	}
//======================== @ self->filling =============================
	  // get_time2();
			   temp = pulser2amt_R(self->current_pulser);
	   self->amt_real = dp(temp, self->dp_vol);
	   	   temp = pulser2amt(self->current_pulser);
	   self->amt = dp(temp, self->dp_vol);
	   	   	   temp = amt2price(self->amt);
	   self->price = dp(temp, self->dp_amount);

	   self->amt_real_tmin3 = self->amt_real_tmin2;
	   self->amt_real_tmin2 = self->amt_real_tmin1;
	   self->amt_real_tmin1 = self->amt_real;


	   //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//

	   temp = pulser2amt_R(self->current_pulser);
	   self->amt_real = dp(temp, self->dp_vol);

	   //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//

	   self->amt_real_array[0] = self->amt_real;
	   self->amt_real_array[1] = self->amt_real_tmin1;
	   self->amt_real_array[2] = self->amt_real_tmin2;
	   self->amt_real_array[3] = self->amt_real_tmin3;

		correctArray1(self->amt_real_array, amt_a);

		self->amt_real = self->amt_real_array[0];
		self->amt_real_tmin1 = self->amt_real_array[1];
		self->amt_real_tmin2 = self->amt_real_array[2];
		self->amt_real_tmin3 = self->amt_real_array[3];


		//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//

		temp = pulser2amt(self->current_pulser);
		self->amt = dp(temp, self->dp_vol);

		//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//


		//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//

		temp = amt2price(self->amt);
		self->price = dp(temp, self->dp_amount);

		//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//

	    make_string(P, dp(self->price, self->dp_amount));
	    make_string(L, dp(self->amt, self->dp_vol));
	    //lcd_print_line1(self->upper);
	    //lcd_print_line2(self->middle);

//	    self->running_volTotaliser  = self->working_volTotaliser  + self->amt_real;
//	   	self->running_volTotaliser_cal = self->working_volTotaliser_cal + self->amt;
//
//	    self->running_amtTotaliser  = self->working_amtTotaliser  + self->price_real;
//		self->running_amtTotaliser_cal = self->working_amtTotaliser_cal + self->price;

//		self->amt_middle = atof(self->middle);
//		self->amt_middle += 0.00011;
//
//		self->price_upper = atof(self->upper);
//		self->price_upper += 0.00011;

//		self->running_volTotaliser = self->working_volTotaliser + self->amt_real;
		//----------------------------------------------------------------------------//
	    self->running_volTotaliser_tmin3 = self->running_volTotaliser_tmin2;
	    self->running_volTotaliser_tmin2 = self->running_volTotaliser_tmin1;
		self->running_volTotaliser_tmin1 = self->running_volTotaliser;
		self->running_volTotaliser = (self->working_volTotaliser + self->amt_real);

		self->running_volTotaliser_array[0] = self->running_volTotaliser;
		self->running_volTotaliser_array[1] = self->running_volTotaliser_tmin1;
		self->running_volTotaliser_array[2] = self->running_volTotaliser_tmin2;
		self->running_volTotaliser_array[3] = self->running_volTotaliser_tmin3;

		correctArray1(self->running_volTotaliser_array, tot_a);

		self->running_volTotaliser = self->running_volTotaliser_array[0];
		self->running_volTotaliser_tmin1 = self->running_volTotaliser_array[1];
		self->running_volTotaliser_tmin2 = self->running_volTotaliser_array[2];
		self->running_volTotaliser_tmin3 = self->running_volTotaliser_array[3];

		self->totaliser_vol = scale_to_original1(self->running_volTotaliser);

		//============================================================================//

	//	   	  self->running_volTotaliser_cal = self->working_volTotaliser_cal + self->amt;

//		self->running_volTotaliser_cal = self->working_volTotaliser_cal + self->amt_middle;
		//----------------------------------------------------------------------------//
		self->running_volTotaliser_cal_tmin3 = self->running_volTotaliser_cal_tmin2;
		self->running_volTotaliser_cal_tmin2 = self->running_volTotaliser_cal_tmin1;
		self->running_volTotaliser_cal_tmin1 = self->running_volTotaliser_cal;
		self->running_volTotaliser_cal = (self->working_volTotaliser_cal + scale_to_range1(self->amt_middle));

		self->running_volTotaliser_cal_array[0] = self->running_volTotaliser_cal;
		self->running_volTotaliser_cal_array[1] = self->running_volTotaliser_cal_tmin1;
		self->running_volTotaliser_cal_array[2] = self->running_volTotaliser_cal_tmin2;
		self->running_volTotaliser_cal_array[3] = self->running_volTotaliser_cal_tmin3;

		correctArray1(self->running_volTotaliser_cal_array, tot_a);

		self->running_volTotaliser_cal = self->running_volTotaliser_cal_array[0];
		self->running_volTotaliser_cal_tmin1 = self->running_volTotaliser_cal_array[1];
		self->running_volTotaliser_cal_tmin2 = self->running_volTotaliser_cal_array[2];
		self->running_volTotaliser_cal_tmin3 = self->running_volTotaliser_cal_array[3];

		self->totaliser_vol_cal = scale_to_original1(self->running_volTotaliser_cal);

		//============================================================================//


		self->running_amtTotaliser = (self->working_amtTotaliser + self->price_real);

	   	self->running_amtTotaliser_cal = (self->working_amtTotaliser_cal + self->price_upper);



	   	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//

	   	self->running_volTotaliser_tmin3 = self->running_volTotaliser_tmin2;
		self->running_volTotaliser_tmin2 = self->running_volTotaliser_tmin1;
		self->running_volTotaliser_tmin1 = self->running_volTotaliser;
		self->running_volTotaliser = (self->working_volTotaliser + self->amt_real);

		self->running_volTotaliser_array[0] = self->running_volTotaliser;
		self->running_volTotaliser_array[1] = self->running_volTotaliser_tmin1;
		self->running_volTotaliser_array[2] = self->running_volTotaliser_tmin2;
		self->running_volTotaliser_array[3] = self->running_volTotaliser_tmin3;

		correctArray1(self->running_volTotaliser_array, tot_a);

		self->running_volTotaliser = self->running_volTotaliser_array[0];
		self->running_volTotaliser_tmin1 = self->running_volTotaliser_array[1];
		self->running_volTotaliser_tmin2 = self->running_volTotaliser_array[2];
		self->running_volTotaliser_tmin3 = self->running_volTotaliser_array[3];

		self->totaliser_vol = scale_to_original1(self->running_volTotaliser);


		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//

		self->running_volTotaliser_cal_tmin3 = self->running_volTotaliser_cal_tmin2;
		self->running_volTotaliser_cal_tmin2 = self->running_volTotaliser_cal_tmin1;
		self->running_volTotaliser_cal_tmin1 = self->running_volTotaliser_cal;
		self->running_volTotaliser_cal = (self->working_volTotaliser_cal + scale_to_range1(self->amt_middle));

		self->running_volTotaliser_cal_array[0] = self->running_volTotaliser_cal;
		self->running_volTotaliser_cal_array[1] = self->running_volTotaliser_cal_tmin1;
		self->running_volTotaliser_cal_array[2] = self->running_volTotaliser_cal_tmin2;
		self->running_volTotaliser_cal_array[3] = self->running_volTotaliser_cal_tmin3;

		correctArray1(self->running_volTotaliser_cal_array, tot_a);

		self->running_volTotaliser_cal = self->running_volTotaliser_cal_array[0];
		self->running_volTotaliser_cal_tmin1 = self->running_volTotaliser_cal_array[1];
		self->running_volTotaliser_cal_tmin2 = self->running_volTotaliser_cal_array[2];
		self->running_volTotaliser_cal_tmin3 = self->running_volTotaliser_cal_array[3];

		self->totaliser_vol_cal = scale_to_original1(self->running_volTotaliser_cal);


		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//


		self->running_amtTotaliser  = (self->working_amtTotaliser  + self->price_real);

		self->running_amtTotaliser_cal = (self->working_amtTotaliser_cal + self->price_upper);


//	   	float pricecheck = self->running_amtTotaliser_cal - priceOld1;
//
//	    if (pricecheck >= 1000.00)
//	    {
//		   priceOld1 = self->running_amtTotaliser_cal;
//		   save_amountSend(side_a);
//
//		   char str[65];
//		   sprintf(str, "[Side-A]... #%0.2f intermittent worth of sales made now!", pricecheck);
//		   server_write(str);
//	    }
////============================================================
//         for totaliser toggle.
	  r_volTotaliser1 	  = floor(scale_to_original1(self->running_volTotaliser_cal) );

//	  self->mechTotalizer = (self->mechTotalizer_ + self->amt_middle);

		#ifdef PULSER_BASED_TV
				self->mechTotalizer = (self->mechTotalizer_ + pulser2amt(self->current_pulser));
		#else
				self->mechTotalizer = (self->mechTotalizer_ + self->amt_middle);
		#endif

	  self->mech_totalizer = (int)self->mechTotalizer;

//	  r_amtTotaliser 	  = floor(self->running_amtTotaliser_cal);
//
//	if(r_volTotaliser1 != old_r_volTotaliser1)
//	{
//		self->totalizerTimer = 0;
////			then toggle the totaliser harware I/O.
//		drive_totaliser1(ACTIVATE);
//		countar++;
//	}
//	else
//	{
//		//deactivate totaliser output...
//		if(self->totalizerTimer > 200)
//		{
//			drive_totaliser1(DEACTIVATE);
//			countar2++;
//		}
//
//	}
//	  old_r_volTotaliser1 = r_volTotaliser1;   //update...
//	  old_r_amtTotaliser = r_amtTotaliser;

	  if (self->totaliser_vol_cal - self->previous_totaliserVol_cal >= THRESHOLD_TV)
//	  if (self->amt_middle - self->previous_totaliserVol_cal >= THRESHOLD_TV)
	  {
			self->previous_totaliserVol_cal = self->totaliser_vol_cal;
//			self->previous_totaliserVol_cal = self->amt_middle;

			self->totalizer_saveStatus = UNSAVED_TO_MAIN_TOTALIZER;
			save_totaliserFrequent_fram(side_a);
			save_totaliserFrequent_eeprom(side_a);
	  }
//============================================================
	  if (self->t > LCD_UPDATE_RATE)
	  {
		    if(self->settings_stream1[0].display_format == PL)
			{
				lcd_print_line1(self->upper);
				lcd_print_line2(self->middle);
			}
			else if(self->settings_stream1[0].display_format == LP)
			{
				lcd_print_line1(self->middle);
				lcd_print_line2(self->upper);
			}

			char str__[9]= {0};
			snprintf(str__, sizeof(str__), "%.2f", self->litre_price);
			lcd_print_line3(str__);

			keypad_fillingUpdate1();

		    self->t = 0;
	  }

	  reset_timer(timeout_dispense); //don'self->t time out.
//==========================pulser capture ============================
#if (_USE_SOFT_PULSER == 1)
	 // if ( (self->t > 50)&&(self->t < 200) )
	 //  			{
	 //  	    	   self->current_pulser++;   //transfer this to the interrupt routine..
	 //  			}
#else
//	       self->current_pulser = __HAL_TIM_GET_COUNTER(self->flow_counter);
#endif
//======================================================================
//	   if(self->target_pulser > 0)
//	   {
//			//self->pulser_rem = self->target_pulser - self->current_pulser;
//	   }

	 if(self->target_pulser > 0)
     {
		  if (self->pulser_rem <= 0 || self->pulser_complete == 1 )  //sales complete...
		  {
				stop_flow1(); //send_solenoid(1);  //stop solenoid.
				self->filling = 0;
				do_calcs();
				get_time();
				update_info();
	//		        save_volumeTotaliser(operating_side);
	//		        save_amountTotaliser(operating_side);
	//		        save_lastSale(operating_side);

				while(retrieve_totaliser_fram_check(operating_side) != OK)   //If it fails, retry 5X
				{
					static uint8_t try = 0;
					if(try++ >= 5)
					{
						while(retrieve_totaliser_eeprom_check(operating_side) != OK)
						{
							if(try++ >= 10)
							{
								clear_totaliser_fram(operating_side);
								clear_totaliser_eeprom(operating_side);
								try = 0;
								break;
							}

						}
						try = 0;
						break;
					}
				 }


				save_totaliser_fram(side_a);
				save_totaliser_eeprom(side_a);

				save_lastSale_fram(side_a);
				save_lastSale_eeprom(operating_side);

				  ////////////////////////////////////////////////
				  //---------------------------------------------
				  self->totalizer_saveStatus = SAVED_TO_MAIN_TOTALIZER;
				  save_totaliserFrequent_fram(operating_side);
				  save_totaliserFrequent_eeprom(operating_side);
				  ////////////////////////////////////////////////


//				self->pump_status = STATUS_MAMO_REACHED;

//				self->target_pulser = 0;

				if(self->settings_stream1[0].mode == AUTO_MODE)
				{
					self->mamo_reached_flag1_1 = 1;
				}

				return write_flash_State;
		  }
		  else
		  {
			//  slow_flow1();
//			  if (self->pulser_rem >= fast_flow_threshold)
//				  {
//				    fast_flow1();
//				  }
			  if(self->pulser_rem >= self->slowFlow_endThreshold)
			  {
					if(self->current_pulser >= self->slowFlow_startThreshold)
					{
						fast_flow1();

						self->fastFlow = 1;
					}
					else
					{
						slow_flow1();

						self->fastFlow = 0;

					}
			  }
			  else
			  {
				  slow_flow1();

				  self->fastFlow = 0;
			  }
		  }
       }

	   if(self->flow_flag == 1)
	   {
	 		self->_tt = 0;
	   }
	   else if( (self->_tt > 60000) && (self->flow_flag == 2) )
	   {
	 		self->old_pulser_ = self->current_pulser;
	 		self->flow_flag = 3;
	   }
	   else if( (self->_tt > 65000) && (self->flow_flag == 3) )
	   {
	 		if(self->old_pulser_ > self->current_pulser)
	 		{
	 			self->current_pulser = self->old_pulser_;
	 			self->flow_loss = 1;
	 			self->filling = 0,
	 			self->nozzle_bit = 0;

				self->stop_flag = 0;
				stop_flow1(); //send_solenoid(1);  //stop solenoid.
				get_time();
				do_calcs();
				update_info();
//				save_volumeTotaliser(operating_side);
//				save_amountTotaliser(operating_side);
//				save_lastSale(operating_side);
//				save_lastSale_fram(operating_side);

				while(retrieve_totaliser_fram_check(operating_side) != OK)   //If it fails, retry 5X
				{
					static uint8_t try = 0;
					if(try++ >= 5)
					{
						while(retrieve_totaliser_eeprom_check(operating_side) != OK)
						{
							if(try++ >= 10)
							{
								clear_totaliser_fram(operating_side);
								clear_totaliser_eeprom(operating_side);
								try = 0;
								break;
							}

						}
						try = 0;
						break;
					}
				 }


				save_totaliser_fram(side_a);
				save_totaliser_eeprom(side_a);

				save_lastSale_fram(side_a);
				save_lastSale_eeprom(operating_side);

				  ////////////////////////////////////////////////
				  //---------------------------------------------
				  self->totalizer_saveStatus = SAVED_TO_MAIN_TOTALIZER;
				  save_totaliserFrequent_fram(operating_side);
				  save_totaliserFrequent_eeprom(operating_side);
				  ////////////////////////////////////////////////


				keypad_zerorize();

				  if(self->settings_stream1[0].mode == AUTO_MODE)
				  {
					 //////////////////////////////////////////////////////////////

					 self->hardwareErrorFlag_source = 1;

					 //////////////////////////////////////////////////////////////
				  }

			    return write_flash_State;
	 		}
	 		self->flow_flag = 1;
	 	}

	   return filling_State;
}



eSystemState filledmamo_State_Handler(void)
{
//	self->filling = 0,
//	self->nozzle_bit = 0;

	self->filling_mamo_flag = 0;

	if(self->settings_stream1[0].noz_override == override)
	{
		self->nozzle_flag_key = 0;
		self->nozzle_flag_key_old = 1;
	}

//	keypad_zerorise1 = false;

//	for(uint8_t i = 0; i <= 8; i++)
//	{
//		self->keyboard[i] = 0;
//	}

//	index_ = 0;
//	_index = 0;
//
//	for(uint8_t i = 0; i < 9; i++)
//	{
//		self->keypad_pw_xter[i] = 0;
//		self->keyboard_entry[i] = 0;   //clear the buffer
//	}


	  if (self->t > LCD_UPDATE_RATE)
	  {

		  if(self->settings_stream1[0].display_format == PL)
		  {
			 lcd_print_line1(self->upper);
			 lcd_print_line2(self->middle);
		  }
		  else if(self->settings_stream1[0].display_format == LP)
		  {
			  lcd_print_line1(self->middle);
			  lcd_print_line2(self->upper);
		  }
		  char str__[8]= {0};
		  snprintf(str__, sizeof(str_), "%.2f", self->litre_price);
		  lcd_print_line3(str__);
		  self->t = 0;
	  }

//	  reset_timer(timeout_dispense); //don'self->t time out.

	  return filledmamo_State;
}

uint32_t price2pulser(float self->price)
{
//  float temp = (self->price / self->litre_price) *  self->pulser_index_c;
//  display_minimumPulser = (0.09 * self->pulser_index_c);   //9 centilitres
//  original_pulse = temp;
  //temp = floor(temp);
  float temp = (self->price / self->litre_price);
//  temp -= 0.01;

  temp -= 0.004;

  temp *= self->pulser_index_c;
  original_pulse = temp;
//  temp -= 4;
  return  floor(temp);
}

uint32_t amt2pulser(float amt)
{
	 float temp;

//	 self->amt -= 0.01;

	 amt -= 0.004;

	 temp = amt * self->pulser_index_c;
//	 display_minimumPulser = (0.09 * self->pulser_index_c);   //9 centilitres
	 original_pulse = temp;
	 return  floor(temp);
}

float pulser2price(uint32_t pulse_)
{
	 float temp = self->litre_price * (1.0/self->pulser_index_c);
	 temp  = temp * (float) pulse_;
	 return (temp );
}

float amt2price(Nozzle *self, float amt_)
{
	 float temp = self->litre_price * amt_;
	 return (temp );
}

float pulser2amt(Nozzle *self, uint32_t pulse_)
{
	 float temp = (float)pulse_ * (1.0/self->pulser_index_c);
	 return (temp );
}

float pulser2amt_R(Nozzle *self, uint32_t pulse_)
{
	 float temp = pulse_ * (1.0/self->pulser_index);
	 //temp  = temp * (float) pulse_;
	 return (temp );
}
//---------------------------------------------------
void make_string(Nozzle *self, sellmode_ sll, float pr)
{
    int8_t tmp = 1,
    	   ind = 0;

    char *endPtr;

	char temp[10] = {0};
	char ttmp[10] = {0};

	 if (sll == L)
	 {
		 if (self->dp_vol == 1)
		     snprintf(temp, sizeof(temp), "%.1f", pr);
		 else if (self->dp_vol == 2)
			 snprintf(temp, sizeof(temp), "%.2f", pr);
		 else if (self->dp_vol == 3)
		 	 snprintf(temp, sizeof(temp), "%.3f", pr);

		 self->amt_middle_tmin3 = self->amt_middle_tmin2;
		 self->amt_middle_tmin2 = self->amt_middle_tmin1;
		 self->amt_middle_tmin1 = self->amt_middle;


//		 self->amt_middle = atof(temp);
//		 self->amt_middle += 0.00011;
		 self->amt_middle = strtof(temp, &endPtr);

 		 self->amt_middle_array[0] = self->amt_middle;
		 self->amt_middle_array[1] = self->amt_middle_tmin1;
		 self->amt_middle_array[2] = self->amt_middle_tmin2;
		 self->amt_middle_array[3] = self->amt_middle_tmin3;

		 correctArray1(self->amt_middle_array, amt_a);

		 self->amt_middle = self->amt_middle_array[0];
		 self->amt_middle_tmin1 = self->amt_middle_array[1];
		 self->amt_middle_tmin2 = self->amt_middle_array[2];
		 self->amt_middle_tmin3 = self->amt_middle_array[3];

		 if(pr < self->display_minimumCentilitre)   // 9 centilitres
		 {
			 pr = 0.00;

			 if (self->dp_vol == 1)
				 snprintf(temp, sizeof(temp), "%.1f", pr);
			 else if (self->dp_vol == 2)
				 snprintf(temp, sizeof(temp), "%.2f", pr);
			 else if (self->dp_vol == 3)
				 snprintf(temp, sizeof(temp), "%.3f", pr);
		 }
	 }

	 if (sll == P)
	 {
		 if (self->dp_amount == 1)
			 snprintf(temp, sizeof(temp), "%.1f", pr);
		 else if (self->dp_amount == 2)
			 snprintf(temp, sizeof(temp), "%.2f", pr);
		 else if (self->dp_amount == 3)
			 snprintf(temp, sizeof(temp), "%.3f", pr);

//		self->price_upper = atof(temp);
//		self->price_upper += 0.00011;

		self->price_upper = strtof(temp, &endPtr);

		if(pr < self->display_minimumCentilitrePrice)   // 9 centilitres
		{
			 pr = 0.00;

			if (self->dp_amount == 1)
				 snprintf(temp, sizeof(temp), "%.1f", pr);
			else if (self->dp_amount == 2)
				 snprintf(temp, sizeof(temp), "%.2f", pr);
			else if (self->dp_amount == 3)
				 snprintf(temp, sizeof(temp), "%.3f", pr);
		}

	 }


//	 temppp_ = atof(temp);
//
//	 temppp_ += 0.00011;  //make small correction for the inherent rounddown.
//	 temppp_ = roundf(temppp_ * 100) / 100;


	 uint8_t index__ = 0;
	while (tmp != 0)
	{
		tmp = temp[index__++];
	}

	int8_t space = 9 - index__;

	if(self->eNextState == filling_State)
	{
		ind = 0;
		space += 1;
	}
	else
		ind = 1;

	 //--------------------------
		  if (sll == L)
				ttmp[0] = 'l';
		  else
				ttmp[0] = 'p';
	 //--------------------------
		while (space > 0) //write spaces first..
		{
		   ttmp[ind++] = ' ';
		   space--;
		}
	  space = index__;  //reload with len of actual number
	   while(space > 0)
		 {
			 ttmp[ind++] = temp[index__ - space];
			 space--;
		 }
    //-----------------------------------------------------
		 for (uint8_t i = 0; i < 10; i++)
		   {
				if (sll == P)
				{
					self->upper[i] = ttmp[i];

//					if(settings[0].display_mode == PL)
//						self->upper[i] = ttmp[i];
//					else if(settings[0].display_mode == LP)
//						self->middle[i] = ttmp[i];
				}
				else
				{
					self->middle[i] = ttmp[i];

//					if(settings[0].display_mode == PL)
//						self->middle[i] = ttmp[i];
//					else if(settings[0].display_mode == LP)
//						self->upper[i] = ttmp[i];
				}
		   }
//     int t = 0;
}

//---------------------------------------------------
//---------------
eSystemState pause_Handler(void)
{
   return authorisation_paused_State;
}

//---------------
eSystemState authorisation_paused_State_Handler(void)
{
	//Motor turned off
	return authorisation_paused_State;
}
//--------------

eSystemState resume_Handler(void)
{
	return authorised_nozzleup_State;
}

//--------------
eSystemState filling_pulse_Handler(Nozzle *self)
{
    //initialise the solenoid and motor...

	self->pump_status = STATUS_FILLING;

	self->status_change_pump = 1;

//	lcd_print_line1(self->upper);
//	lcd_print_line2(self->middle);

	if(self->settings_stream1[0].display_format == PL)
	  {
		 lcd_print_line1(self->upper);
		 lcd_print_line2(self->middle);
	  }
	  else if(self->settings_stream1[0].display_format == LP)
	  {
		  lcd_print_line1(self->middle);
		  lcd_print_line2(self->upper);
	  }

	char str__[10]= {0};
		snprintf(str__, sizeof(str__), "%.2f", self->litre_price);
		lcd_print_line3(str__);
	return filling_State;   //filling_paused_State;
}

//--------------
/*
eSystemState filling_State_Handler(void)
{

}
*/
//---------------
eSystemState filling_paused_Handler(void)
{

	return filling_paused_State;
}

//--------------
eSystemState filling_paused_State_Handler(void)
{
	//Motor turned off
    return filling_paused_State;
}

//---------------
eSystemState filling_resumed_Handler(Nozzle *self)
{

	//==========================================================================

	#if (_USE_SOFT_PULSER == 1)

	 	 self->current_pulser = ( self->currentPulser_recovered + self->current_pulser);

	#else

	    self->current_pulser = ( self->currentPulser_recovered + (__HAL_TIM_GET_COUNTER(self->flow_counter) ) );

	#endif
	//--------------------------------------------------------------------------

    self->pulser_rem = self->target_pulser - self->current_pulser;

	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//

	self->amt = pulser2amt(self->current_pulser);

	self->price = amt2price(self->amt);

	self->amt_middle_tmin3 = self->amt;
	self->amt_middle_tmin2 = self->amt;
	self->amt_middle_tmin1 = self->amt;
	self->amt_middle = self->amt;

	self->amt_real_tmin3 = self->amt;
	self->amt_real_tmin2 = self->amt;
	self->amt_real_tmin1 = self->amt;
	self->amt_real = self->amt;

	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//



	//////////////////////////////////////////////////////////////////////////
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
	/*             CATERS TO RESUMED SALES @ PUMP RESTART                   */

	self->nozzle_bit = 1;
	self->stop_fueling_bit = 0;

	drive_motor1(ACTIVATE);

	if(self->settings_stream1[0].pump_type_ != LAFENG)
	{
		 drive_slow_sole1(ACTIVATE);
		 drive_fast_sole1(DEACTIVATE);
	}
	else
	{
		drive_slow_sole1(DEACTIVATE);     // DEACTIVATE here actually means ACTIVATE
		drive_fast_sole1(ACTIVATE);		  // ACTIVATE here actually means DEACTIVATE
	}

	//////////////////////////////////////////////////////////////////////////


	return filling_State;

}



eSystemState keypad_entry_State_Handler(Nozzle *self)
{

    return  self->ePrevState; //
	//return keypad_entry_State;
}

eSystemState tone_entry_State_Handler(Nozzle *self)
{
    return  self->ePrevState;
}

//----------------------------------------


//--------------------------------------------------------------
void do_calcs (Nozzle *self)
{

	float temp;
	 if(self->target_pulser > 0)  // if self->price or volume is programmed
	 {
	   if (self->pulser_rem <= 0 || self->pulser_complete == 1 )  //sales complete...
	   {
		   //get_time2();

			if (self->sellmode == P)
			{

			   //---------------------------------------------------------
				//self->price = dp(self->key_value,dp_price);   // temp = amt2price(self->amt);
				//self->amt   = dp( price_/self->litre_price ,dp_amount);

				 temp = pulser2amt(self->current_pulser);
							 self->amt = dp(temp, self->dp_vol);

				 temp = amt2price(self->amt);
							 self->price = dp(temp, self->dp_amount);

				if( (self->key_value_sellmodeP == 1) && (self->authorizedSale_overflow == 0) && (self->pump_LitreOverflow == 0) && (self->display_overflow == 0) )
				{
					self->key_value_sellmodeP = 0;
					self->key_value = self->key_value_original;
				}

				/********************************************
				 *
				 * SHOW WHAT THE USER NEEDS...
				 *
				 * ******************************************/
				 price_ = dp(self->key_value, self->dp_amount);
					 temp = price_/self->litre_price;
				 amt_   = dp(temp, self->dp_vol); //calculate vol frm self->price.

				//=========================================================
				//    Also calculate the values based on the real P.Indx
						  temp  = pulser2amt_R(self->current_pulser);
					  self->amt_real = dp(temp, self->dp_vol);
						  temp = amt2price(self->amt_real);
					  self->price_real = dp(temp, self->dp_amount);
				//=========================================================
			   }

			  if (self->sellmode == L)
			  {
					temp = pulser2amt(self->target_pulser);
					self->amt  = dp(temp, self->dp_vol);

					self->price  = amt2price(self->amt);

					if( (self->key_value_sellmodeL == 1) && (self->authorizedSale_overflow == 0) && (self->pump_LitreOverflow == 0) && (self->display_overflow == 0) )
					{
						self->key_value_sellmodeL = 0;
						self->key_value = self->key_value_original;
					}

					/********************************************
					 *
					 * SHOW WHAT THE USER NEEDS TO SEE...
					 *
					 * ******************************************/
					amt_   = self->key_value;
					price_ = amt_ * self->litre_price; //calculate self->price from self->price.self->amt
					//=========================================================
					//    Also calculate the values based on the real P.Indx
							  temp  = pulser2amt_R(self->current_pulser);
						  self->amt_real = dp(temp, self->dp_vol);
							  temp = amt2price(self->amt_real);
						  self->price_real = dp(temp, self->dp_amount);
					//=========================================================
			   }
			 //------------------------------------------------------------------

			  make_string(P, dp(price_, self->dp_amount));
			  make_string(L, dp(amt_, self->dp_vol));

			  if(self->settings_stream1[0].display_format == PL)
			  {
				 lcd_print_line1(self->upper);
				 lcd_print_line2(self->middle);
			  }
			  else if(self->settings_stream1[0].display_format == LP)
			  {
				  lcd_print_line1(self->middle);
				  lcd_print_line2(self->upper);
			  }

//			  self->running_volTotaliser = self->working_volTotaliser + self->amt_real;
			  //----------------------------------------------------------------------------//
			    self->running_volTotaliser_tmin3 = self->running_volTotaliser_tmin2;
			    self->running_volTotaliser_tmin2 = self->running_volTotaliser_tmin1;
				self->running_volTotaliser_tmin1 = self->running_volTotaliser;
				self->running_volTotaliser = (self->working_volTotaliser + self->amt_real);

				self->running_volTotaliser_array[0] = self->running_volTotaliser;
				self->running_volTotaliser_array[1] = self->running_volTotaliser_tmin1;
				self->running_volTotaliser_array[2] = self->running_volTotaliser_tmin2;
				self->running_volTotaliser_array[3] = self->running_volTotaliser_tmin3;

				correctArray1(self->running_volTotaliser_array, tot_a);

				self->running_volTotaliser = self->running_volTotaliser_array[0];
				self->running_volTotaliser_tmin1 = self->running_volTotaliser_array[1];
				self->running_volTotaliser_tmin2 = self->running_volTotaliser_array[2];
				self->running_volTotaliser_tmin3 = self->running_volTotaliser_array[3];

				self->totaliser_vol = scale_to_original1(self->running_volTotaliser);

				//============================================================================//

	//	   	  self->running_volTotaliser_cal = self->working_volTotaliser_cal + self->amt;

//			  self->running_volTotaliser_cal = self->working_volTotaliser_cal + self->amt_middle;
			  //----------------------------------------------------------------------------//
				self->running_volTotaliser_cal_tmin3 = self->running_volTotaliser_cal_tmin2;
				self->running_volTotaliser_cal_tmin2 = self->running_volTotaliser_cal_tmin1;
				self->running_volTotaliser_cal_tmin1 = self->running_volTotaliser_cal;
				self->running_volTotaliser_cal = (self->working_volTotaliser_cal + scale_to_range1(self->amt_middle));

				self->running_volTotaliser_cal_array[0] = self->running_volTotaliser_cal;
				self->running_volTotaliser_cal_array[1] = self->running_volTotaliser_cal_tmin1;
				self->running_volTotaliser_cal_array[2] = self->running_volTotaliser_cal_tmin2;
				self->running_volTotaliser_cal_array[3] = self->running_volTotaliser_cal_tmin3;

				correctArray1(self->running_volTotaliser_cal_array, tot_a);

				self->running_volTotaliser_cal = self->running_volTotaliser_cal_array[0];
				self->running_volTotaliser_cal_tmin1 = self->running_volTotaliser_cal_array[1];
				self->running_volTotaliser_cal_tmin2 = self->running_volTotaliser_cal_array[2];
				self->running_volTotaliser_cal_tmin3 = self->running_volTotaliser_cal_array[3];

//				self->totaliser_vol_cal = scale_to_original1(self->running_volTotaliser_cal);

				//============================================================================//


			  self->running_amtTotaliser = (self->working_amtTotaliser + self->price_real);

			  self->running_amtTotaliser_cal = (self->working_amtTotaliser_cal + self->price_upper);

//			  float pricecheck = self->running_amtTotaliser_cal - priceOld1;
//
//				if (pricecheck >= 1000.00)
//				{
//				   priceOld1 = self->running_amtTotaliser_cal;
//				   save_amountSend(side_a);
//
//				   char str[65];
//				   sprintf(str, "[Side-A]... #%0.2f intermittent worth of sales made now!", pricecheck);
//				   server_write(str);
//				}

			   self->totaliser_vol = scale_to_original1(self->running_volTotaliser);    // update totaliser
			   self->totaliser_vol_cal = scale_to_original1(self->running_volTotaliser_cal);  // update totaliser
			   self->totaliser_amt = self->running_amtTotaliser;    // update totaliser
			   self->totaliser_amt_cal = self->running_amtTotaliser_cal;  // update totaliser

			//============================================================

			   //         for totaliser toggle.

				r_volTotaliser1 = floor( scale_to_original1(self->running_volTotaliser_cal) );


//				self->mechTotalizer = (self->mechTotalizer_ + self->amt_middle);

				#ifdef PULSER_BASED_TV
						self->mechTotalizer = (self->mechTotalizer_ + pulser2amt(self->current_pulser));
				#else
						self->mechTotalizer = (self->mechTotalizer_ + self->amt_middle);
				#endif

				self->mech_totalizer = (int) self->mechTotalizer;

		//		if(r_volTotaliser1 != old_r_volTotaliser1)
		//		{
		//			self->totalizerTimer = 0;
		//	  //			then toggle the totaliser harware I/O.
		//			drive_totaliser1(ACTIVATE);

//				}
				if(self->mech_totalizer != self->mech_totalizer_old)
				{
			  		self->totalizerTimer = 0;

			        //	then toggle the totaliser harware I/O.
			  		drive_totaliser1(ACTIVATE);
				}
				else
				{
					//deactivate totaliser output...
					if(self->totalizerTimer > 200)
					{
						drive_totaliser1(DEACTIVATE);
					}

				}
				old_r_volTotaliser1 = r_volTotaliser1;   //update...

				//===================// Update... //===================//
			  	  self->mech_totalizer_old = self->mech_totalizer;
			  	//-----------------------------------------------------//


//			  	if (self->totaliser_vol_cal - self->previous_totaliserVol_cal >= THRESHOLD_TV)
//				{
//					self->previous_totaliserVol_cal = self->totaliser_vol_cal;
//
//					save_totaliserFrequent_fram(side_a);
//					save_totaliserFrequent_eeprom(side_a);
//				}
			//-------------------------------------------------------------------

			   return;
		 }
	   else
	   {
		  //programmed but still dispensing @ stop pt.
			 temp = pulser2amt(self->current_pulser);
			 self->amt   = dp(temp, self->dp_vol);
			 temp = amt2price(self->amt);
			 self->price = dp(temp, self->dp_amount);

			  make_string(P, dp(self->price, self->dp_amount));
			  make_string(L, dp(self->amt, self->dp_vol));

			//=========================================================
			//    Also calculate the values based on the real P.Indx
						  temp  = pulser2amt_R(self->current_pulser);
					  self->amt_real = dp(temp,self->dp_vol);
						  temp = amt2price(self->amt_real);
					  self->price_real = dp(temp, self->dp_amount);
			//=========================================================

	//		  lcd_print_line1(self->upper);
	//		  lcd_print_line2(self->middle);
			  if(self->settings_stream1[0].display_format == PL)
			  {
				 lcd_print_line1(self->upper);
				 lcd_print_line2(self->middle);
			  }
			  else if(self->settings_stream1[0].display_format == LP)
			  {
				  lcd_print_line1(self->middle);
				  lcd_print_line2(self->upper);
			  }


//			  self->running_volTotaliser = self->working_volTotaliser + self->amt_real;
			  //----------------------------------------------------------------------------//
			    self->running_volTotaliser_tmin3 = self->running_volTotaliser_tmin2;
			    self->running_volTotaliser_tmin2 = self->running_volTotaliser_tmin1;
				self->running_volTotaliser_tmin1 = self->running_volTotaliser;
				self->running_volTotaliser = (self->working_volTotaliser + self->amt_real);

				self->running_volTotaliser_array[0] = self->running_volTotaliser;
				self->running_volTotaliser_array[1] = self->running_volTotaliser_tmin1;
				self->running_volTotaliser_array[2] = self->running_volTotaliser_tmin2;
				self->running_volTotaliser_array[3] = self->running_volTotaliser_tmin3;

				correctArray1(self->running_volTotaliser_array, tot_a);

				self->running_volTotaliser = self->running_volTotaliser_array[0];
				self->running_volTotaliser_tmin1 = self->running_volTotaliser_array[1];
				self->running_volTotaliser_tmin2 = self->running_volTotaliser_array[2];
				self->running_volTotaliser_tmin3 = self->running_volTotaliser_array[3];

				self->totaliser_vol = scale_to_original1(self->running_volTotaliser);

				//============================================================================//

	//	   	  self->running_volTotaliser_cal = self->working_volTotaliser_cal + self->amt;

//			  self->running_volTotaliser_cal = self->working_volTotaliser_cal + self->amt_middle;
			  //----------------------------------------------------------------------------//
				self->running_volTotaliser_cal_tmin3 = self->running_volTotaliser_cal_tmin2;
				self->running_volTotaliser_cal_tmin2 = self->running_volTotaliser_cal_tmin1;
				self->running_volTotaliser_cal_tmin1 = self->running_volTotaliser_cal;
				self->running_volTotaliser_cal = (self->working_volTotaliser_cal + scale_to_range1(self->amt_middle));

				self->running_volTotaliser_cal_array[0] = self->running_volTotaliser_cal;
				self->running_volTotaliser_cal_array[1] = self->running_volTotaliser_cal_tmin1;
				self->running_volTotaliser_cal_array[2] = self->running_volTotaliser_cal_tmin2;
				self->running_volTotaliser_cal_array[3] = self->running_volTotaliser_cal_tmin3;

				correctArray1(self->running_volTotaliser_cal_array, tot_a);

				self->running_volTotaliser_cal = self->running_volTotaliser_cal_array[0];
				self->running_volTotaliser_cal_tmin1 = self->running_volTotaliser_cal_array[1];
				self->running_volTotaliser_cal_tmin2 = self->running_volTotaliser_cal_array[2];
				self->running_volTotaliser_cal_tmin3 = self->running_volTotaliser_cal_array[3];

				self->totaliser_vol_cal = scale_to_original1(self->running_volTotaliser_cal);

				//============================================================================//


			  self->running_amtTotaliser = (self->working_amtTotaliser + self->price_real);

			  self->running_amtTotaliser_cal = (self->working_amtTotaliser_cal + self->price_upper);

//			  float pricecheck = self->running_amtTotaliser_cal - priceOld1;
//
//				if (pricecheck >= 1000.00)
//				{
//				   priceOld1 = self->running_amtTotaliser_cal;
//				   save_amountSend(side_a);
//
//				   char str[65];
//				   sprintf(str, "[Side-A]... #%0.2f intermittent worth of sales made now!", pricecheck);
//				   server_write(str);
//				}

			   self->totaliser_vol = scale_to_original1(self->running_volTotaliser);    // update totaliser
			   self->totaliser_vol_cal = scale_to_original1(self->running_volTotaliser_cal);  // update totaliser
			   self->totaliser_amt = self->running_amtTotaliser;    // update totaliser
			   self->totaliser_amt_cal = self->running_amtTotaliser_cal;  // update totaliser

			 //-------------------------------------------------------------------
			   return;
	   }
	 }
	 else   // if self->target_pulser == 0
	 {
		 //if un programmed...
		 //still dispensing @ stop pt.

			 temp = pulser2amt(self->current_pulser);
				  self->amt = dp(temp, self->dp_vol);

			 temp = amt2price(self->amt);
				 self->price = dp(temp, self->dp_amount);

			//compare the final and initial prices...
				 float ddp = 0;
				 if      (self->dp_amount == 1) ddp =  0.1;
				 else if (self->dp_amount == 2) ddp =  0.01;
				 else if (self->dp_amount == 3) ddp =  0.001;

				 if( (temp - self->price) > ( (ddp/10)*2) )  //result was rounded down.
				 {

					 //need to do a little fixing here...
					 self->price = self->price + ddp;
				 }

			  make_string(P, self->price); //,dp_price)
			  make_string(L, self->amt);  //,dp_amount)

			//=========================================================
			//    Also calculate the values based on the real P.Indx
					  temp  = pulser2amt_R(self->current_pulser);
				  self->amt_real = dp(temp,self->dp_vol);
					  temp = amt2price(self->amt_real);
				  self->price_real = dp(temp, self->dp_amount);
			//=========================================================

	//		  lcd_print_line1(self->upper);
	//		  lcd_print_line2(self->middle);

			  if(self->settings_stream1[0].display_format == PL)
			  {
				 lcd_print_line1(self->upper);
				 lcd_print_line2(self->middle);
			  }
			  else if(self->settings_stream1[0].display_format == LP)
			  {
				  lcd_print_line1(self->middle);
				  lcd_print_line2(self->upper);
			  }

//			  self->running_volTotaliser = self->working_volTotaliser + self->amt_real;
			  //----------------------------------------------------------------------------//
			    self->running_volTotaliser_tmin3 = self->running_volTotaliser_tmin2;
			    self->running_volTotaliser_tmin2 = self->running_volTotaliser_tmin1;
				self->running_volTotaliser_tmin1 = self->running_volTotaliser;
				self->running_volTotaliser = (self->working_volTotaliser + self->amt_real);

				self->running_volTotaliser_array[0] = self->running_volTotaliser;
				self->running_volTotaliser_array[1] = self->running_volTotaliser_tmin1;
				self->running_volTotaliser_array[2] = self->running_volTotaliser_tmin2;
				self->running_volTotaliser_array[3] = self->running_volTotaliser_tmin3;

				correctArray1(self->running_volTotaliser_array, tot_a);

				self->running_volTotaliser = self->running_volTotaliser_array[0];
				self->running_volTotaliser_tmin1 = self->running_volTotaliser_array[1];
				self->running_volTotaliser_tmin2 = self->running_volTotaliser_array[2];
				self->running_volTotaliser_tmin3 = self->running_volTotaliser_array[3];

				self->totaliser_vol = scale_to_original1(self->running_volTotaliser);
				//============================================================================//

	//	   	  self->running_volTotaliser_cal = self->working_volTotaliser_cal + self->amt;

//			  self->running_volTotaliser_cal = self->working_volTotaliser_cal + self->amt_middle;
			  //----------------------------------------------------------------------------//
				self->running_volTotaliser_cal_tmin3 = self->running_volTotaliser_cal_tmin2;
				self->running_volTotaliser_cal_tmin2 = self->running_volTotaliser_cal_tmin1;
				self->running_volTotaliser_cal_tmin1 = self->running_volTotaliser_cal;
				self->running_volTotaliser_cal = (self->working_volTotaliser_cal + scale_to_range1(self->amt_middle));

				self->running_volTotaliser_cal_array[0] = self->running_volTotaliser_cal;
				self->running_volTotaliser_cal_array[1] = self->running_volTotaliser_cal_tmin1;
				self->running_volTotaliser_cal_array[2] = self->running_volTotaliser_cal_tmin2;
				self->running_volTotaliser_cal_array[3] = self->running_volTotaliser_cal_tmin3;

				correctArray1(self->running_volTotaliser_cal_array, tot_a);

				self->running_volTotaliser_cal = self->running_volTotaliser_cal_array[0];
				self->running_volTotaliser_cal_tmin1 = self->running_volTotaliser_cal_array[1];
				self->running_volTotaliser_cal_tmin2 = self->running_volTotaliser_cal_array[2];
				self->running_volTotaliser_cal_tmin3 = self->running_volTotaliser_cal_array[3];

				self->totaliser_vol_cal = scale_to_original1(self->running_volTotaliser_cal);

				//============================================================================//


			  self->running_amtTotaliser = (self->working_amtTotaliser + self->price_real);

			  self->running_amtTotaliser_cal = (self->working_amtTotaliser_cal + self->price_upper);

//			  float pricecheck = self->running_amtTotaliser_cal - priceOld1;
//
//				if (pricecheck >= 1000.00)
//				{
//				   priceOld1 = self->running_amtTotaliser_cal;
//				   save_amountSend(side_a);
//
//				   char str[65];
//				   sprintf(str, "[Side-A]... #%0.2f intermittent worth of sales made now!", pricecheck);
//				   server_write(str);
//				}

			   self->totaliser_vol = scale_to_original1(self->running_volTotaliser);    // update totaliser
			   self->totaliser_vol_cal = scale_to_original1(self->running_volTotaliser_cal);  // update totaliser
			   self->totaliser_amt = self->running_amtTotaliser;    // update totaliser
			   self->totaliser_amt_cal = self->running_amtTotaliser_cal;  // update totaliser

			 //-------------------------------------------------------------------
			   return;
		 }
	  return;
}

//----------------------------------------
//
//
//





//-----------------------------------------
void state_ini(Nozzle *self)
{
	float temp = 0.0;

	self->irrecov_flag = 0;
	self->progg = 0;

	self->pump_status = STATUS_PNP;
//    pump_status_2 = STATUS_PNP;

    if(self->settings_stream1[0].mode != AUTO_MODE)
	{
		self->eNextState =  idle_State; //   prog_State; //
		self->eLastState =  idle_State;
	}
    else
    {
    	self->eNextState =  pnp_State;
		self->eLastState =  pnp_State;
    }

	index_ = 0;

	self->filling = 0;
	stop_flow1();


//	lastVolumeSale1 = lastSale_storeA.lastVolumeSale_real;
//	self->lastVolumeSale_cal =  lastSale_storeA.lastVolumeSale_cal;

//	temp = pulser2amt(self->current_pulser);
//	self->amt = dp(temp,dp_amount);
//    temp = amt2price(lastVolumeSale1);
//	self->price = dp(temp, dp_price);

//	char str_p[10] = {0};
//	char str_l[10] = {0};
//
//	snprintf(str_p, sizeof(str_p), "%.2f", self->price);
//	snprintf(str_l, sizeof(str_l), "%.2f", lastVolumeSale1);

//	write_v(1, "p        ");
	if(self->settings_stream1[0].display_format == PL)
	{
		lcd_print_line1("P        ");
		write_v(1, "p        ");
	}
	else if(self->settings_stream1[0].display_format == LP)
	{
		lcd_print_line1("L        ");
		write_v(1, "l        ");
	}

//	write_v(2, "l        ");
	if(self->settings_stream1[0].display_format == PL)
	{
		lcd_print_line2("L        ");
		write_v(2, "l        ");

		make_string(P, dp(self->lastAmountSale_cal, self->dp_amount) );
		    make_string(L, dp(self->lastVolumeSale_cal, self->dp_vol) );
	}
	else if(self->settings_stream1[0].display_format == LP)
	{
		lcd_print_line2("P        ");
		write_v(2, "p        ");
	}

//    temp = amt2price(self->lastVolumeSale_cal);
//	self->price = dp(temp, self->dp_amount);
//	make_string(P, dp(self->price, self->dp_amount) );

	make_string(P, dp(self->lastAmountSale_cal, self->dp_amount) );
    make_string(L, dp(self->lastVolumeSale_cal, self->dp_vol) );

//	lcd_print_line1(str_p);
//	lcd_print_line2(str_l);

//	printDisp_c("p",1,0,8,LT,CLEAR); printDisp_f(str_p,1,0,8,LT,NOCLEAR);  //display self->price
//
//	printDisp_c("l",2,0,8,LT,CLEAR); printDisp_f(str_l,2,0,8,LT,NOCLEAR);  //display litre

	char str__[10] = {0};
	snprintf(str__, sizeof(str__), "%.2f", self->litre_price);
	lcd_print_line3(str__);

	 if(self->sellmode == L)
	   {
		  write_v(3, "L     0");  //send_keypad("l    ");  //5 xters lafeng
//		    if(settings[0].display_mode == PL)
//		  	{
//		    	 write_v(3, "l    0");  //send_keypad("l    ");  //5 xters lafeng
//		  	}
//		  	else if(settings[0].display_mode == LP)
//		  	{
//		  		 write_v(3, "p    0");  //send_keypad("l    ");  //5 xters lafeng
//		  	}
	   }
	   else
	   {
		  write_v(3, "P     0");  //send_keypad("p    ");  //5 xters  lafeng..
//		    if(settings[0].display_mode == PL)
//			{
//				 write_v(3, "p    0");  //send_keypad("l    ");  //5 xters lafeng
//			}
//			else if(settings[0].display_mode == LP)
//			{
//				 write_v(3, "l    0");  //send_keypad("l    ");  //5 xters lafeng
//			}
	   }
	 keypad_print(self->keyboard);
}

//void states(void)
//{
//	self->eNewEvent = read_event1();
//	if (self->eNewEvent != _keypress_Event)
//	{
//        //self->ePrevState = self->eNextState;
//	}
//
//	eSystemEvent ev;
//	  if( (self->eNewEvent < _no_Event)  )  //if event occured, check if the current state is sensitive to it..
//	  {
//		  self->eLastState = self->eNextState;   // store state...
//           if(self->eNewEvent   == _auth_command_Event) //_nozzleup_Event) //_filling_pulse_Event  _nozzleup_Event)
//            {
//           	  //int yiuyu = 0;
//            }
//          //...   scan through the allowed  events of the state if its among them..
//	    for (int i = 0; i < max_events_per_state; i++)
//		 {
//		   ev = (asStateEventMachine[self->eNextState].states[i]);  //
//		   if (ev == 0)
//			{
//			   // self->eNextState = self->eLastState;   //stay in the same event..
//				break;  // we reached end of event in allowed events array, exit scan..
//			}
//		   if (self->eNewEvent == ev) // if the just triggered event is among allowed events..
//		   {
//                //then check if it has an handler and switch to the event Handler...
//			   if (asEventMachine[ev-1].pfEventMachineHandler == NULL) //no handler supplied
//			   {
//				   break; //self->eNextState = (*asStateEventMachine[self->eNextState].pfStateMachineHandler)(); //switch to state handler.
//			   }
//			   else
//			   {
//				   self->eNextState = (*asEventMachine[self->eNewEvent-1].pfEventMachineHandler)();  //switch to event handler.
//				   self->eNewEvent = _no_Event;
//				   // int ty = 0;
//
//			   }
//		   } // if (self->eNewEvent == ev)
//		 }  ///for
//
//	     self->eNextState = (*asStateEventMachine[self->eNextState].pfStateMachineHandler)(); //switch to state handler.
//	    // int yy = 56;
//	  }
//	 else
//	  {
//		//int s0 = (asStateEventMachine[self->eNextState].states[0]);
//		//int s1 = (asStateEventMachine[self->eNextState].states[1]);
//		//int s2 = (asStateEventMachine[self->eNextState].states[2]);
//
//		  // no event, switch to same state...
//		  self->eNextState = (*asStateEventMachine[self->eNextState].pfStateMachineHandler)();  //
//					  //  (*asStateEventMachine[self->eNextState].pfStateMachineHandler)();
//		 // int yui = 0;
//		  return;
//	  }
//
//
//	  /*
//	if((self->eNextState < last_State) && (self->eNewEvent < last_Event)&& (asStateMachine[self->eNextState].eStateMachineEvent == self->eNewEvent) && (asStateMachine[self->eNextState].pfStateMachineEvnentHandler != NULL))
//		{
//			// function call as per the state and event and return the next state of the finite state machine
//			self->eNextState = (*asStateMachine[self->eNextState].pfStateMachineEvnentHandler)();
//		}
//		else
//		{
//
//		}  */
//	  //self->eNextState = (*asStateEventMachine[self->eNextState].pfStateMachineHandler)();
//	 // return;
//}


//void states2(void)
//{
//	eNewEvent2 = read_event2();
//	if (eNewEvent2 != _keypress_Event)
//	{
//        //self->ePrevState = self->eNextState;
//	}
//
//	eSystemEvent ev;
//	  if( (eNewEvent2 < _no_Event)  )  //if event occured, check if the current state is sensitive to it..
//	  {
//		  eLastState2 = eNextState2;   // store state...
//           if(eNewEvent2 == _auth_command_Event) //_nozzleup_Event) //_filling_pulse_Event  _nozzleup_Event)
//            {
//           	  //int yiuyu = 0;
//            }
//          //...   scan through the allowed  events of the state if its among them..
//	    for (int i = 0; i < max_events_per_state; i++)
//		 {
//		   ev = (asStateEventMachine[eNextState2].states[i]);  //
//		   if (ev == 0)
//			    {
//			       // self->eNextState = self->eLastState;   //stay in the same event..
//			        break;  // we reached end of event in allowed events array, exit scan..
//			    }
//		   if (eNewEvent2 == ev) // if the just triggered event is among allowed events..
//		   {
//                //then check if it has an handler and switch to the event Handler...
//			   if (asEventMachine[ev-1].pfEventMachineHandler == NULL) //no handler supplied
//			   {
//				   break; //self->eNextState = (*asStateEventMachine[self->eNextState].pfStateMachineHandler)(); //switch to state handler.
//			   }
//			      else
//			   {
//				   eNextState2 = (*asEventMachine[eNewEvent2-1].pfEventMachineHandler)();  //switch to event handler.
//				   eNewEvent2 = _no_Event;
//				   // int ty = 0;
//
//			   }
//		   } // if (self->eNewEvent == ev)
//		 }  ///for
//
//	     eNextState2 = (*asStateEventMachine[eNextState2].pfStateMachineHandler)(); //switch to state handler.
//	    // int yy = 56;
//	  }
//	 else
//	  {
//		//int s0 = (asStateEventMachine[self->eNextState].states[0]);
//		//int s1 = (asStateEventMachine[self->eNextState].states[1]);
//		//int s2 = (asStateEventMachine[self->eNextState].states[2]);
//
//		  // no event, switch to same state...
//		  eNextState2 = (*asStateEventMachine[eNextState2].pfStateMachineHandler)();  //
//					  //  (*asStateEventMachine[self->eNextState].pfStateMachineHandler)();
//		 // int yui = 0;
//		  return;
//	  }
//
//
//	  /*
//	if((self->eNextState < last_State) && (self->eNewEvent < last_Event)&& (asStateMachine[self->eNextState].eStateMachineEvent == self->eNewEvent) && (asStateMachine[self->eNextState].pfStateMachineEvnentHandler != NULL))
//		{
//			// function call as per the state and event and return the next state of the finite state machine
//			self->eNextState = (*asStateMachine[self->eNextState].pfStateMachineEvnentHandler)();
//		}
//		else
//		{
//
//		}  */
//	  //self->eNextState = (*asStateEventMachine[self->eNextState].pfStateMachineHandler)();
//	 // return;
//}


void save_date(int8_t _entri)
{
	 ddate[0] = _entri;
	 clear_buffer1();
	 mth_success = false;
	 clock_save = true;
}

//========= Leap-Year Calculation ============//
bool leapYear_calc(int8_t year)
{
	int lp;
	int entrii = year + 2000;

	lp = entrii % 4;
	if(lp)                 //Not a Leap Year
		return 0;          //day_max_feb = 28;
	else if (!lp)
	{
		lp = entrii % 100;
		if(lp)
			return 1;      //day_max_feb = 29;
		else if(!lp)
		{
			lp = entrii % 400;
			if(!lp)
				return 1;	//day_max_feb = 29;
			else
				return 0;    //Not a leap year
		}
	}

}

uint16_t get_ctTime1(void)
{
	char time_tm[6];
	uint8_t time_t1,
			time_t2,
			i;
	uint16_t time_tme;

	time_t1 = DS1307_GetHour();
	time_t2 = DS1307_GetMinute();

	memset(time_tm, '\0', sizeof(time_tm));
	time_tm[0] = ( (time_t1 / 10) + 48);
	time_tm[1] = ( (time_t1 % 10) + 48);
	time_tm[2] = ( (time_t2 / 10) + 48);
	time_tm[3] = ( (time_t2 % 10) + 48);
	time_tme = atoi(time_tm);
	return time_tme;
}


void states_1(void)
{
	self->eNewEvent = read_event1_1();
	if (self->eNewEvent != _keypress_Event)
	{
        //self->ePrevState = self->eNextState;
	}

	if (self->settings_stream1[0].mode == MANUAL_MODE)
	{
		if ( (self->eNewEvent == _nozzleup_Event) && (idleStateEntry_flag1 == 0) )
		{
			self->eNewEvent = _no_Event;
		}
	}

	eSystemEvent ev;
	  if( (self->eNewEvent < _no_Event)  )  //if event occurred, check if the current state is sensitive to it..
	  {
		  self->eLastState = self->eNextState;   // store state...
           if(self->eNewEvent   == _auth_command_Event) //_nozzleup_Event) //_filling_pulse_Event  _nozzleup_Event)
            {
           	  uint8_t yiuyu = 0;
            }
          //...   scan through the allowed  events of the state if its among them..
	    for (uint8_t i = 0; i < MAX_EVENTS_PER_STATE; i++)
		{
		   ev = (asStateEventMachine_1[self->eNextState].states[i]);  //
		   if (ev == 0)
			{
			   // self->eNextState = self->eLastState;   //stay in the same event..
				break;  // we reached end of event in allowed events array, exit scan..
			}
		   if (self->eNewEvent == ev) // if the just triggered event is among allowed events..
		   {
                //then check if it has a handler and switch to the event Handler...
			   if (asEventMachine_1[ev-1].pfEventMachineHandler == NULL) //no handler supplied
			   {
				   break; //self->eNextState = (*asStateEventMachine[self->eNextState].pfStateMachineHandler)(); //switch to state handler.
			   }
			   else
			   {

				   eNextState1_1 = self->eNextState;
				   eNewEvent1_1 = self->eNewEvent;

				   self->eNextState = (*asEventMachine_1[self->eNewEvent-1].pfEventMachineHandler)();  //switch to event handler.
				   self->eNewEvent = _no_Event;
				   // int ty = 0;

//				   if( (self->pump_status == STATUS_RESET) && (self->eNextState == idle_State) )
//				   {
//					   dummyData = 0;
//				   }

			   }
		   } // if (self->eNewEvent == ev)
		 }  ///for

	     eNextState1_1 = self->eNextState;

	     self->eNextState = (*asStateEventMachine_1[self->eNextState].pfStateMachineHandler)(); //switch to state handler.
	    // int yy = 56;

//	     if( (self->pump_status == STATUS_RESET) && (self->eNextState == idle_State) )
//		   {
//			   dummyData = 0;
//		   }

	  }
	 else
	  {
		//int s0 = (asStateEventMachine[self->eNextState].states[0]);
		//int s1 = (asStateEventMachine[self->eNextState].states[1]);
		//int s2 = (asStateEventMachine[self->eNextState].states[2]);

		  eNextState1_1 = self->eNextState;

		  // no event, switch to same state...
		  self->eNextState = (*asStateEventMachine_1[self->eNextState].pfStateMachineHandler)();  //
					  //  (*asStateEventMachine[self->eNextState].pfStateMachineHandler)();
		 // int yui = 0;

		  if( (self->pump_status == STATUS_RESET) && (self->eNextState == idle_State) )
		   {
			   dummyData = 0;
		   }

		  return;
	  }


	  /*
	if((self->eNextState < last_State) && (self->eNewEvent < last_Event)&& (asStateMachine[self->eNextState].eStateMachineEvent == self->eNewEvent) && (asStateMachine[self->eNextState].pfStateMachineEvnentHandler != NULL))
		{
			// function call as per the state and event and return the next state of the finite state machine
			self->eNextState = (*asStateMachine[self->eNextState].pfStateMachineEvnentHandler)();
		}
		else
		{

		}  */
	  //self->eNextState = (*asStateEventMachine[self->eNextState].pfStateMachineHandler)();
	 // return;
}


//----------------------------------------
//----------------------------------------
//----------------------------------------
eSystemState switchedoffState_Handler(void)
{
	return switchedoff_State;
}

//----------------------------------------
eSystemState authorisation_resumed_State_Handler(void)
{
	//Motor turned on
	return _authorisation_resumed_State_Handler;
}

//----------------------------------------
eSystemState filling_resumed_State_Handler(void)
{
	//Motor turned on
	return _filling_resumed_State_Handler;
}


void keypad_zerorize(void)
{
	//--------------------------------------------------------
	dpFlag = 0;
	error_clr_flag = 1;
	index_ = 0;
	_index = 0;

	self->nozzle_flag_key = 0;
	self->nozzle_flag_key_old = 0;

	for(uint8_t i = 0; i < 9; i++)
	{
		self->keypad_pw_xter[i] = 0;
		self->keyboard_entry[i] = 0;   //clear the buffer
	}

	for(uint8_t i = 0; i <= 8; i++)
	{
		self->keyboard[i] = 0;
	}

	//--------------------------------------------------------------------
	if (self->sellmode == P)
	{
		write_v(3, "P     0");  //send_keypad("p    ");  //5 xters  lafeng..
	}
	else if(self->sellmode == L)
	{
		write_v(3, "L     0");  //send_keypad("l    ");  //5 xters lafeng
	}

	//	 else if(self->sellmode == V)
	//	 {
	//		  write_v(3, "v    0");  //send_keypad("p    ");  //5 xters  lafeng..
	//	 }

	keypad_print(self->keyboard);
}


void keypad_fillingUpdate1(void)
{
	static uint8_t lcd_size = 5;

	if(self->settings_stream1[0].keypad__  == LAFNG17_K)
	{
		lcd_size = 5; //change this latter to accomodate other lcds.
	}
	else if( (self->settings_stream1[0].keypad__  == LAFNG18_K) || (self->settings_stream1[0].keypad__ == LAFNG18_K_V2) )
	{
		lcd_size = 7; //change this latter to accomodate other lcds.
	}
	else if( (self->settings_stream1[0].keypad__  == BLSKY18_K) || (self->settings_stream1[0].keypad__  == BLSKY22) )
	{
		lcd_size = 7;   //6;
	}

  //================================================================


	int8_t size = lcd_size;   // No xter to display 'P/L'

	int8_t indexx_ = 0,
		   space = 0;

	uint8_t ind = 0;

	if(self->sellmode == P)
	{
	indexx_ = strlen(self->upper);

	if( (indexx_ <= size) || (indexx_ <= 6) )
	{
		space = size - indexx_;
	}
	else
	   space = 0;  //full size of xters.

	while (space > 0) //write spaces first..
	{
	 self->keyboard[ind++] = ' ';
	   space--;
	}
	//---------------------------------
	if( (indexx_ <= size) || (indexx_ <= 6) )
		  space = indexx_;
	else
	{
		space = size;
	}

	if( (strchr(self->upper, '.')) && (indexx_ > 6) ) space = 8;
	while(space > 0)
	{
	   self->keyboard[ind++] = self->upper[indexx_ - space];
	   space--;
	}

	if(self->keyboard[ind - 1] == '.') self->keyboard[ind] = '0';

	}
	else if(self->sellmode == L)
	{
	indexx_ = strlen(self->middle);

	if( (indexx_ <= size) || (indexx_ <= 6) )
	{
		space = size - indexx_;
	}
	else
	   space = 0;  //full size of xters.

	while (space > 0) //write spaces first..
	{
	 self->keyboard[ind++] = ' ';
	   space--;
	}
	//---------------------------------
	if( (indexx_ <= size) || (indexx_ <= 6) )
		  space = indexx_;
	else
	{
		space = size;
	}

	if( (strchr(self->middle, '.')) && (indexx_ > 6) ) space = 8;
	while(space > 0)
	{
	   self->keyboard[ind++] = self->middle[indexx_ - space];
	   space--;
	}

	if(self->keyboard[ind - 1] == '.') self->keyboard[ind] = '0';

	}

	keypad_print(self->keyboard);
}

// Function to check and correct questionable values in the array
void correctArray1(float v[4], corrected_sid sid)
{
    bool corrected;
    char str[200];

    //v[0] is the current value

    do {
        corrected = false;

        // Check if v[3] <= v[2] <= v[1] <= v[0] with differences <= 0.9
        for (int8_t i = 3; i > 0; i--)
        {
            if (v[i - 1] < v[i] || fabs(v[i - 1] - v[i]) > 0.9)
            {
			  #if DEBUG_0
            	if(sid == tot_a)
            	{
					sprintf(str,
							"\n\nTotalizer1 Error detected @ v[%d]: %.2f\n",
							i - 1, v[i - 1]);
            	}
            	else if(sid == amt_a)
            	{
					sprintf(str,
							"\n\nLitre-Transaction1 Error detected @ v[%d]: %.2f\n",
							i - 1, v[i - 1]);
            	}

				HAL_UART_Transmit(&huart3, str, strlen((char*)str), HAL_MAX_DELAY);

				HAL_Delay(1);

				memset(str, '\0', sizeof(str));
				sprintf(str,
						"Pump-1 Status : [ %d ]\n",
						self->pump_status);
				HAL_UART_Transmit(&huart3, str, strlen((char*)str), HAL_MAX_DELAY);

				HAL_Delay(1);

			  #endif

                // Correct the error based on the more recent or older value
                if (i - 1 == 0)
                {
                    v[i - 1] = v[i]; // v[0] takes the value of v[1]
                }
                else
                {
                    v[i - 1] = v[i]; // Other elements take the value of the more recent element
                }

			  #if DEBUG_0
                if(sid == tot_a)
				{
					sprintf(str,
							"Corrected Totalizer1 @ v[%d] to: %.2f\n",
							i - 1, v[i - 1]);
				}
				else if(sid == amt_a)
				{
					sprintf(str,
							"Corrected Litre-Transaction1 @ v[%d] to: %.2f\n",
							i - 1, v[i - 1]);
				}

				HAL_UART_Transmit(&huart3, str, strlen((char*)str), HAL_MAX_DELAY);
			  #endif

                corrected = true;
            }
        }
    } while (corrected); // Repeat until no corrections are needed
}


// Scale value to range [0, 1,000,000]
float scale_to_range1(float value)
{
    if (MAX_VAL <= MIN_VAL)
    {
//        printf("Invalid range: max_val should be greater than min_val.\n");
        return 0.0f;
    }

    float normalized = (value - MIN_VAL) / (MAX_VAL - MIN_VAL); // Normalize to 0-1
    float scaled_value = normalized * 1000000.0f;              // Scale to 0-1,000,000

    if (scaled_value < 0.0f) scaled_value = 0.0f;
    if (scaled_value > 1000000.0f) scaled_value = 1000000.0f;

    return scaled_value;
}

// Convert scaled value back to the original range
float scale_to_original1(float scaled_value)
{
    if (MAX_VAL <= MIN_VAL)
    {
//        printf("Invalid range: max_val should be greater than min_val.\n");
        return 0.0f;
    }

    float normalized = scaled_value / 1000000.0f;             // Normalize to 0-1
    float original_value = normalized * (MAX_VAL - MIN_VAL) + MIN_VAL; // Scale back to original range

    return original_value;
}

// Function to extract the fractional part of a float
float get_fractional_part1(float num)
{
    // Get the integer part using floorf
    float int_part = floorf(num);

    // Subtract the integer part from the original number to get the fractional part
    float fractional_part = num - int_part;

    return fractional_part;
}

