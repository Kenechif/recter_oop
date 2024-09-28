/*
 * states.c
 *
 *  Created on: Mar 7, 2022
 *      Author: tunji
 */
//======================================================
#include "command.h"
#include "thread1.h"
#include "settings.h"
#include "states.h"
#include "states2.h"
#include "pump_comm.h"

#include "stdbool.h"
#include "string.h"
#include "stddef.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

#include"myLibraries.h"

//===========================================
#include "../ds1307/ds1307_for_stm32_hal.h"
//===========================================

//===========================================
//            spi flash memory
//===========================================
#include "../flash_mem/w25qxxConf.h"
#include "../flash_mem/w25qxx.h"
#include "log.h"

#define W25QXX_DUMMY_BYTE 0xA5

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;

extern SPI_HandleTypeDef _W25QXX_SPI;
#define W25qxx_Delay(delay) HAL_Delay(delay)
//===========================================
// global variable for the flash state
//availab;le to the two sides
     extern w25qxx_t w25qxx;
//-------------------------------------------
//extern log_new log_a_new , log_b_new;

extern log_new log_a_new,
			   log_b_new,
			   synchedLog_a_new,
			   synchedLog_b_new;

extern log_new1 log_a_new1 , log_b_new1;

extern uint32_t flash_read_idA;
extern uint32_t flash_read_idB;

extern const uint32_t flash_beginA;
extern const uint32_t flash_endA;

extern const uint32_t flash_beginB;
extern const uint32_t flash_endB;

extern uint8_t prog_entry1;
extern uint8_t prog_entry2;

extern int data_size; //w25qxx.PageSize;    //0;
//===========================================

 /*
  int  timeout_Event,
    authorise_Event,
    nozzleup_Event,
	nozzledown_Event,
    pause_Event,
    resume_Event,
	key_Event,
	error_Event,
	complete_Event,
    last_Event = 0;
 */

extern pump_status_enum pump_status_1,
						pump_status_2;

extern pump_status_enum_4G pump2_status_4G;

extern log_new log_a_new,log_b_new;
extern uint8_t hour,minute,second,date, day, month,year,dayofweek;

extern float price2;
extern float amt2;
extern float litre_price2;
extern float pulser_index2;
extern float pulser_index_c2;
extern float litre_price1;
extern float litre_price2;

extern uint8_t calibration_flag2 = 0,
			   configMode2 = 0;

extern uint8_t pwr2 = NOPOWERINTERRUPTION;

extern float price_upper2 = 0.000,
			 amt_middle2 = 0.000;

extern uint8_t ep1a_priceChangeFlag2 = 0;

//extern log_ log_a[50],log_b[50];

//extern pump_settings settings[2],
//					 copy[2],
//					 settings0[2];


extern pump_settings_stream1 settings_stream1[2],
						     settings_original_stream1[2],
						     copy_stream1[2];

extern pump_settings_stream2 settings_stream2[2],
				       	     settings_original_stream2[2],
							 copy_stream2[2];

extern pump_settings_stream3 settings_stream3[2],
				       	     settings_original_stream3[2],
							 copy_stream3[2];

extern int8_t ttime2[3],
			  ddate2[4];

extern uint32_t target_pulser1,
				current_pulser1;

uint32_t target_pulser2,
		 current_pulser2 = 0;

uint8_t fastFlow2 = 0;

extern int timeout_picknozzle;
extern int timeout_dispense;

static int time2 ;
static int st2 = 0;
int timer_flag2 = 0;
int timer_flag_old2 = 0;

int tot_buttonpress_tmr2 = 0;
int log_buttonpress_tmr2 = 0;
int key_buttonpress_tmr2 = 0,
	progExit_buttonpress_tmr2 = 0;

uint8_t prog_revisit2 = 1,
		prog_revisitt2 = 1;

extern  int t2;

extern uint32_t num2 ;

extern  int16_t _tt2,
				totalizer2Timer,
				priceChange_timer2,
				timer_config2,
				key19Timer2;

extern uint32_t transaction_period2;

//extern float target_pulser1 , current_pulser1 ;
extern sellmode_ sellmode2; //int sellmode2 ;

char str_2[10]= {0};

//=== screen Arrays ====
char upper2[10]  = {0};
char middle2[10] = {0};

//=============== keyboard entry ======================
char keyboard_entry2[10] = {0};  //buffers the key press
char keyboard2[9] = {0};

char temp2[9] = {0};

char* xter_buffer12[7] = {0};
char* xter_buffer22[7] = {0};

int kkey2 = 0;
//=====================================================
//char keyboard_screen[6] = {0};
int  keypress_2 = 0;
int8_t index_2 = 0;
//uint32_t key_value = 0;
float key_value2 = 0.0;
//=====================================================
//int flow_coeff = 0;   //Already  defined.
int pulser_rem2 = 0;
int tflow2 = 0;
//=====================================================
//  prog variables.
//int progg = 0;
 int prog_index2 = 0;
 int index22 = 0;
 int index32 = 0;
 int progg2  = 0;

 extern uint32_t calib_pulser2 = 0;
 int calibr2 = 0;     //used for signaling motor and solenoid activatn in prog mode.

 auth_state auth2 = not_auth;   //first set this to
// int side = side1;           //pump side selected.
// int level2 = level0;
 int _index2 = 0;

 operatorfxn_  operatorfxn2 = totaliser_view;

extern char* menu1[4];  //= {"  tot   "," price  ","  log   "};
extern char* menu2[16]; //= {"f.nnode","ch pl","pr. set","f.id","nf time","hi arnt.","com prot","ch. pass","load cfg","dwn. cfg","set log","clr log"};
extern char* menu3[3]; // = {"tmm cfg","flo rate"};

extern char* menu_level1[4]; // = {"Shft. Tot ", "  Price ", "  Cloc"};
extern char* menu_level2[21]; // = {"  nnode ", "Address ", " Nozzle ", "Disp.Styl", "  Price ", "no flo. t", "Hi. Litre", "Ch  Pass", "Calibrat.", "DP Count", "  Cloc", "Sides No", "Start CL", "Calib.Can", "Shift.Typ", "Shift No", "  Tone  ", " Pulser", "  Valve ", "No-Calib"};
extern char* menu_level3[23]; // = {"  nnode ", "Address ", " Nozzle ", "Disp.Styl", "  Price ", "no flo. t", "Hi. Litre", "Ch  Pass", "Calibrat.", "DP Count", "  Cloc", "Sides No", "Start CL", "Calib.Can", "Shift.Typ", "Shift No", "  Tone  ", " Pulser", "  Valve ", "No-Calib", "Calib.Typ", "Conn.Card"};

extern char* pass[4]; // = {" pass 1 ", " pass 2 ", " pass 3 "};
extern char* ch_pass[4];  // = {"pass. 1 ", "pass 2 ", "pass 3 "};

extern char* login_type[4]; //= {"[ None ]", "[ PIN ] ", "[ Card ]"};

//=====================================================

extern char lafeng_keypad[17],
			lafeng_keypad_18K[18];

extern char bluesky_keypad[22];

//extern int pump_type;
extern pump pump_type;
extern uint8_t auth_cmd_flag2;
extern int8_t opmode2;
extern uint8_t error_clr_flag2;
extern uint8_t keypress__2 ;

extern char password_level1[9],
			password_level2[9],
			password_level3[9];

extern int operating_side;
extern int max_dp;

extern float totaliser_vol1;
extern float totaliser_vol1c;
extern float totaliser_vol2;
extern float totaliser_vol2c;

extern float firstTotaliser_vol1,
			 firstTotaliser_vol1c,
			 firstTotaliser_vol2,
			 firstTotaliser_vol2c;

extern float totaliser_amt1,
			 totaliser_amt1c,
			 totaliser_amt2,
			 totaliser_amt2c,
			 priceOld1,
			 priceOld2;

extern float startShiftTotaliser_vol1,
			 startShiftTotaliser_vol1c,
			 startShiftTotaliser_vol2,
			 startShiftTotaliser_vol2c;

extern float startShiftTotaliser_amt1,
			 startShiftTotaliser_amt1c,
			 startShiftTotaliser_amt2,
			 startShiftTotaliser_amt2c;

extern float working_volTotaliser1;
extern float running_volTotaliser1;

extern float working_volTotaliser1c;
extern float running_volTotaliser1c;

extern float working_volTotaliser2,
			running_volTotaliser2,
			working_volTotaliser2c,
			running_volTotaliser2c;

extern float working_amtTotaliser1,
			running_amtTotaliser1,
			working_amtTotaliser1c,
			running_amtTotaliser1c;

extern float working_amtTotaliser2,
			running_amtTotaliser2,
			working_amtTotaliser2c,
			running_amtTotaliser2c;

extern int8_t dp_amount2,
			  dp_vol2,
			  dp_unitprice2;

extern uint8_t firstTime_nozz2 = 1;

extern uint8_t stop_flag2;

int irrecov_flag2 = 0;
//extern int error_clr_flag;
//=================================================
char keypad_buf2[10] = {0};
char keypad_pw_xter2[10] = {0};

char keypad_pw2[10] = {0};
char keypad_pw_2[10] = {0};
//=================================================
//uint32_t price2pulser2(float price);
//uint32_t amt2pulser2(float amt);

uint8_t pulser_complete2 = 0; //used from the interrupt to signify the completion of pump sales.

//=================================================
char sc12[10] = {32};

uint8_t volume_flag2 = 0,
		pulser_flag2,
		valve_flag2;

float original_pulse2 = 0;

float price_real2,amt_real2 = 0.0;
float amt_2,price_2;

extern float price_real1,amt_real1;
extern float amt_,price_;

extern uint32_t pulser_new2;
uint8_t filling2 = 0;
bool lock_clr2 = 0;

uint8_t firstTime_filling2 = 1,
		totalizer2_error = 0,
		calibration2_error = 0;

extern int8_t auth_from_ctrl2;

//extern const uint16_t fast_flow_threshold;
extern  uint16_t fast_flow_threshold2,
				 slowFlow_startThreshold2,
				 slowFlow_endThreshold2;

//-------------------------------------------------
//
extern flash_store_info flash_infoA,
						flash_infoB;

extern uint32_t flash_read_idA;
extern uint32_t flash_read_idB;

extern uint32_t flash_locc;

extern ConfigChange configChange[2];

extern uint16_t otp_seed2 = 0;

//extern int  disp_type1 , disp_type2;
extern pump disp_type1,
			disp_type2;

extern uint16_t shutdown_timer2;

extern uint16_t motor_tmr2;

extern uint8_t nozzle_bit2;
extern uint8_t stop_fueling_bit2;

//uint8_t change_price = 0;
//uint8_t change_volume = 0;
float auth_v2,
	  auth_p2;

uint8_t change_p2,
		change_v2;

extern int8_t auth_from_ctrl;
extern int authorise_flag;
extern int8_t change_price_flag;
float set_p;

extern uint8_t changeLitrePrice2;

int8_t pump_LitreOverflow2 = 0,
	   display_overflow2 = 0,
//	   _litre_price2 = 0,
	   _auth_p2 = 0,
	   _auth_v2 = 0,
	   idle_backwardPulse2 = 0,
	   idle_forwardPulse2 = 0,
	   flow_loss2 = 0,
	   firstTime_display_overflow2 = 0,
	   changeLitrePrice2_2 = 0,
	   priceChange_timerMin2 = 0;

uint8_t _pump_max_litres2 = 0,
		_litre_price2 = 0,
		nonValid_sale2 = 0,
		nozzleUp_inProgMode2 = 0;

float half_litre2 = 0.00,
	  display_minimumCentilitre2 = 0.04,
	  display_minimumCentilitrePrice2 = 0.00;

uint8_t nozzleDown_source2,
		reset_flag2;

float pulser_totalizer2 = 0.00;
extern uint32_t pulser_benchMark2 = 15987;

extern uint8_t calibrationCan_measure2 = 20;

//-------------   fxn prototypes ------------------
void do_calcs2 ();
uint8_t long_press_tot2();
uint8_t long_press_log2();
//------------------------------------------------

uint32_t  r_volTotaliser2 = 0;
uint32_t  old_r_volTotaliser2 = 0;

float r_amtTotaliser2 = 0.00,
	  old_r_amtTotaliser2 = 0.00;

/*
 * sets the precision of the supplied float
 */
float dp2(float flt,int n)
{
  uint8_t chrr[10] = {0};
  uint8_t chrr_[10] = {0};


  if(n == 1) snprintf(chrr_, sizeof(chrr_), "%.1f", flt);
  else if(n == 2) snprintf(chrr_, sizeof(chrr_), "%.2f", flt);
  else if(n == 3) snprintf(chrr_, sizeof(chrr_), "%.3f", flt);

	float f = atof(chrr_);
 	f += 0.00011;  //make small correction for the inherent rounddown.
 //	f+= 0.000011;

 	if(f < 1.0)
	{
		return f;
	}

	uint32_t temp = (f * pow(10,n));


	//temp = floor(temp);
 	int nn = snprintf(chrr, sizeof(chrr_), "%d", temp);
 	uint8_t chrrr[12] = {0};
 	for(int indx = 0; indx<10;indx++)
 	{
 		chrrr[indx] = 48;
 	}
 	chrrr[9] = 0;
 	int j = 8;
 	int dpp = 0;

// 	for (int i = nn-1; i > -1;i--)
//		{
// 			chrrr[j] =  chrr[i];
// 			dpp++;
// 			if(dpp == n)  //if decimal point
//				{
// 				   j--;
// 				   chrrr[j] = '.';   // insert decimmal point
//				}
// 			j--;
//		}

 	if (temp < 100000000)
 	 	{
 	 		chrrr[9] = 0;

 			for (int8_t i = nn-1; i > -1; i--)
 			{
 	 			chrrr[j] =  chrr[i];
 	 			dpp++;
 	 			if(dpp == n)  //if decimal point
 				{
 				   j--;
 				   chrrr[j] = '.';   // insert decimmal point
 				}
 	 			j--;
 			}
 	 	}

 	 	else if( (temp >= 100000000) && (temp < 1000000000) )
 	 	{
 	 	 	chrrr[10] = 0;        //chrrr[9] = 0;
 	 	 	j = 9;				  //j = 8;

 	 	 	for (int8_t i = nn-1; i > -1;i--)
 			{
 				chrrr[j] =  chrr[i];
 				dpp++;
 				if(dpp == n)  //if decimal point
 				{
 				   j--;
 				   chrrr[j] = '.';   // insert decimmal point
 				}
 				j--;
 			}
 	 	}

 	 	else if( (temp >= 1000000000) && (temp < 10000000000) )
 		{
 			chrrr[11] = 0;        //chrrr[9] = 0;
 			j = 10;				  //j = 8;

 			for (int8_t i = nn-1; i > -1; i--)
 			{
 				chrrr[j] =  chrr[i];
 				dpp++;
 				if(dpp == n)  //if decimal point
 				{
 				   j--;
 				   chrrr[j] = '.';   // insert decimmal point
 				}
 				j--;
 			}
 		}

    /* for (int i = nn-1; n > -1;n--)
		{
			chrrr[j] =  chrr[i];
			j--;
		}
	 */
	//float temp_ = temp/pow(10,n);
	//temp_ = (temp_);

 	float temp_ = atof(chrrr);
 	//double temp1_ = atoff(chrrr);
 	return temp_;
}


// initialises the line 1 and 2 display variable.
void write_v2(int i, char lcd_[9])
{
	if (i == 1)      //first line
	{
		  for(int ii = 0 ; ii<8 ; ii++)
			{
               upper2[ii] = lcd_[ii];
               upper2[ii+1] = 0;
			}
	}

	if (i == 2)   //second line
	 {
		  for(int ii = 0 ; ii<8 ; ii++)
			{
				middle2[ii] = lcd_[ii];
				middle2[ii+1] = 0;
			}
   	}

	if (i == 3)   //third line
	{
		for(int ii = 0 ; ii<7 ; ii++)
		{
			keyboard2[ii] = lcd_[ii];
			keyboard2[ii+1] = 0;
		}
	}
}
//---------------------------------------
void reset_timer2(int tm)
{
   time2 = tm;
   timer_flag2 = 0;
   timer_flag_old2 = 1;
   st2 = 0;
}
//=======================================
void start_timer2(int tm)
{
	time2 = tm;
	st2 = 1;
}
//=======================================
void dec_timer2(void)
{
  if(st2 == 1)
	{
	  if (time2 > 0)time2--;
	   if(time2 == 0)
	   {
		   timer_flag2 = 1;
	   }
	   else
	   {
		   timer_flag2 = 0;
	   }
	}
}
//=======================================
void stop_timer2(void)
{
	timer_flag_old2 = 1;
	time2 = 0;
	st2 = 0;
}
//=======================================
int8_t read_keypad2();
uint8_t long_press_log2();
uint8_t long_press_tot2();
//=======================================


//Initialize array of structure of event and event handler
sEventMachine2 asEventMachine2 [] =
{
    {_timeout_Event, timeout_Handler2},
    {_authorise_Event, authorise_Handler2},
   	{_auth_command_Event, auth_command_Handler2},
    {_nozzleup_Event, nozzleup_Handler2},
   	{_nozzledown_Event, nozzledown_Handler2},
    {_pause_Event, pause_Handler2},
    {_resume_Event, resume_Handler2},
   	{_keyup_Event, keyup_Handler2},
   	{_keydown_Event, keydown_Handler2},
   	{_keypress_Event, keypress_Handler2},
   	{_tot_error_Event, tot_error_Handler2},
   	{_key19_Event, key19_Handler2},
   	{_filling_paused_Event, filling_paused_Handler2},
   	{_filling_resumed_Event, filling_resumed_Handler2},
   	{_filling_pulse_Event, filling_pulse_Handler2},
	{_error_clear_Event, error_clear_Handler2},
	{_operator_Event, operator_Handler2},
    {_function_key_Event, function_key_Handler2},
	{_stopcommand_Event, stopcommand_Handler2},
	{_resetcommand_Event, resetcommand_Handler2},
	{_fillingcomplete_Event, _fillingcomplete_Handler2},
	{_switchoffcommand_Event, switchoffcommand_Handler2},
	{_authorisecommand_Event, authorisecommand_Handler2},
	{_hardwarereset_Event, hardwarereset_Handler2},
	{_hardwareerror_Event, hardwareerror_Handler2},
	{_auth_suspendcommand_Event, auth_suspendcommand_Handler2},
	{_filling_suspendcommand_Event, filling_suspendcommand_Handler2},
	{_auth_resumecommand_Event, auth_resumecommand_Handler2},
	{_filling_resumecommand_Event, filling_resumecommand_Handler2},
	{_mamo_Event, mamo_Handler2},
	{_no_Event, 0},
};



//initialise the array of structure of State and state handlers and their
// allowed  events.
// { <state>,<handler>,{<allowed event1>,<allowed event2>,..,<allowed eventn>}}
sStateEventMachine2 asStateEventMachine2 [] =
{
	{prog_State, progState_Handler2, {_keydown_Event,_keypress_Event}},
	{idle_State, idleState_Handler2, {_operator_Event,_keyup_Event,_tot_error_Event, _keypress_Event,_nozzleup_Event, _auth_command_Event, _nozzledown_Event, _resetcommand_Event, _switchoffcommand_Event}},
	{inactive_State, inactiveState_Handler2, {_error_clear_Event, _keyup_Event, _keypress_Event}},
	{nozzleup_waitingforauth_State, nozzleup_waitingforauthState_Handler2, {_authorise_Event,_timeout_Event,_nozzledown_Event,_keypress_Event, _authorisecommand_Event, _stopcommand_Event, _switchoffcommand_Event, _hardwarereset_Event, _hardwareerror_Event}},
	{authorised_nozzledown_State, authorised_nozzledown_State_Handler2, {_nozzleup_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
	{authorised_nozzleup_State, authorised_nozzleup_State_Handler2, {_filling_pulse_Event,_pause_Event,_timeout_Event,_nozzledown_Event,_keypress_Event,_function_key_Event, _stopcommand_Event, _auth_suspendcommand_Event, _hardwarereset_Event, _hardwareerror_Event}},
	{authorisation_paused_State, authorisation_paused_State_Handler2, {_resume_Event,_timeout_Event,_nozzledown_Event,_keypress_Event, _auth_resumecommand_Event,  _stopcommand_Event, _hardwarereset_Event, _hardwareerror_Event, _switchoffcommand_Event}},
	{authorisation_resumed_State, authorisation_resumed_State_Handler2, {}},
	{filling_State, filling_State_Handler2, {_filling_paused_Event,_keypress_Event,_timeout_Event,_nozzledown_Event,_keypress_Event, _stopcommand_Event, _filling_suspendcommand_Event, _hardwarereset_Event, _hardwareerror_Event, _mamo_Event, _switchoffcommand_Event}},
	{filling_paused_State, filling_paused_State_Handler2, {_filling_resumed_Event, _keypress_Event, _timeout_Event,_nozzledown_Event, _filling_resumecommand_Event, _stopcommand_Event, _hardwarereset_Event, _hardwareerror_Event, _switchoffcommand_Event}},
	{filling_resumed_State, filling_resumed_State_Handler2, {}},
	{keypad_entry_State, keypad_entry_State_Handler2, {}},
	{operator_State, operator_State_Handler2, {_keypress_Event}},
	{savesettings_State, savesettings_State_Handler2, {_keypress_Event}},
	{read_flash_State, read_flash_State_Handler, {}},
	{write_flash_State, write_flash_State_Handler, {}},
	{switchedoff_State, switchedoffState_Handler2, {_stopcommand_Event, _resetcommand_Event}},
    {pnp_State, pnpState_Handler2, {_fillingcomplete_Event}},
	{filledmamo_State, filledmamo_State_Handler2, {_nozzledown_Event, _resetcommand_Event, _stopcommand_Event, _switchoffcommand_Event}},
    {last_State, 0, {}}
};

////Initialize array of structure of event and event handler
//sEventMachine2 asEventMachine2 [] =
//{
//    {_timeout_Event,timeout_Handler2},
//    {_authorise_Event,authorise_Handler2},
//   	{_auth_command_Event,auth_command_Handler2},
//    {_nozzleup_Event,nozzleup_Handler2},
//   	{_nozzledown_Event,nozzledown_Handler2},
//    {_pause_Event,pause_Handler2},
//    {_resume_Event,resume_Handler2},
//   	{_keyup_Event,keyup_Handler2},
//   	{_keydown_Event,keydown_Handler2},
//   	{_keypress_Event,keypress_Handler2},
//   	{_tot_error_Event,tot_error_Handler2},
//   	{_key19_Event,key19_Handler2},
//   	{_filling_paused_Event,filling_paused_Handler2},
//   	{_filling_resumed_Event,filling_resumed_Handler2},
//   	{_filling_pulse_Event,filling_pulse_Handler2},
//	{_error_clear_Event,error_clear_Handler2},
//	{_operator_Event,operator_Handler2},
//	{_no_Event,0}
//};
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
//sStateEventMachine2 asStateEventMachine2 [] =
//{
//	{prog_State, progstate_Handler2,{_keydown_Event,_keypress_Event}},
//    {idle_State,idlestate_Handler2,{_operator_Event,_keyup_Event,_tot_error_Event,_keypress_Event,_nozzleup_Event,_auth_command_Event, _nozzledown_Event}},
//    {inactive_State,inactivestate_Handler2,{_error_clear_Event, _keyup_Event, _keypress_Event}},
//    {nozzleup_waitingforauth_State,nozzleup_waitingforauthState_Handler2,{_authorise_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
//	{authorised_nozzledown_State,authorised_nozzledown_State_Handler2,{_nozzleup_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
//    {authorised_nozzleup_State,authorised_nozzleup_State_Handler2,{_filling_pulse_Event,_pause_Event,_timeout_Event,_nozzledown_Event,_keypress_Event,_function_key_Event}},
//	{authorisation_paused_State,authorisation_paused_State_Handler2,{_resume_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
//	{filling_State,filling_state_Handler2,{_filling_paused_Event,_keypress_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
//	{filling_paused_State,filling_paused_state_Handler2,{_filling_resumed_Event,_keypress_Event,_timeout_Event,_nozzledown_Event}},
//	{keypad_entry_State,keypad_entry_State_Handler2,{}},
//	{operator_State, operator_State_Handler2,{_keypress_Event}},
//	{savesettings_State, savesettings_State_Handler2,{_keypress_Event}},
//	{read_flash_State, read_flash_State_Handler,{}},
//	{write_flash_State, write_flash_State_Handler,{}},
//    {last_State, 0, {}}
//};

 //  {idle_State,keyup_Event,inactivestate_Handler},
 //  {inactive_State,keydown_Event,idlestate_Handler},
 //  {nozzleup_waitingforauth_State,Option_Selection_Event,OptionSelectionHandler},
 //  {Option_Selected_State,Amount_Enter_Event,EnterAmountHandler},
 //  {Amount_Entered_State,Amount_Dispatch_Event,AmountDispatchHandler}

//----------------------------------------
void clear_buffer2(void)
{
	int i;
	for( i = 0;i<sizeof(keyboard_entry2);i++)

	 {
		keyboard_entry2[i] = 0;
		keypad_pw_xter2[i] = 0;
	 }
	keypad_pw_xter2[i] = 0;
	keypad_pw_xter2[i++] = 0;
	keypad_pw_xter2[i++] = 0;
	keypad_pw_xter2[i++] = 0;
	index_2 = 0;
	_index2 = 0;
}

/*
 *
 */
eSystemState operator_Handler2(void)
{

	return operator_State;
}

eSystemState function_key_Handler2(void)
{
	;
}
eSystemState stopcommand_Handler2(void)
{
	pump_status_2 = STATUS_FILLING_COMP;
	status_change_pump2 = 1;

	return idle_State;
}

eSystemState resetcommand_Handler2(void)
{
	pump_status_2 = STATUS_RESET;
	status_change_pump2 = 1;

	return nozzleup_waitingforauth_State;
}

//eSystemState priceupdate_Handler(void)
eSystemState _fillingcomplete_Handler2(void)
{
	pump_status_2 = STATUS_FILLING_COMP;
	status_change_pump2 = 1;

	return idle_State;
}

eSystemState switchoffcommand_Handler2(void)
{
	pump_status_2 = STATUS_SWITCHED_OFF;
	status_change_pump2 = 1;

	return switchedoff_State;
}

eSystemState authorisecommand_Handler2(void)
{
	pump_status_2 = STATUS_AUTH;
	status_change_pump2 = 1;

	return authorised_nozzleup_State;
}

eSystemState hardwarereset_Handler2(void)
{
	pump_status_2 = STATUS_FILLING_COMP;
	status_change_pump2 = 1;

	return idle_State;
}

eSystemState hardwareerror_Handler2(void)
{
	pump_status_2 = STATUS_FILLING_COMP;
	status_change_pump2 = 1;

	return idle_State;
}

eSystemState auth_suspendcommand_Handler2(void)
{
	return authorisation_paused_State;
}

eSystemState filling_suspendcommand_Handler2(void)
{
	return filling_paused_State;
}

eSystemState auth_resumecommand_Handler2(void)
{
	return authorised_nozzleup_State;
}

eSystemState filling_resumecommand_Handler2(void)
{
	return filling_State;
}

eSystemState mamo_Handler2(void)
{
	pump_status_2 = STATUS_MAMO_REACHED;
	status_change_pump2 = 1;

	return filledmamo_State;
}


/////////////////////////////////////////////////////////////////
uint8_t long_press_key2()
{
	static int pressed_ = 0;
		//static int pressed_old = 0;
		int ky;

//		if(pump_type == bluesky)
//		if( (pump_type == DN_BLSKY18K) || (pump_type == DN_BLSKY22) ||
//				(pump_type == DIN_BLSKY18K) || (pump_type == DIN_BLSKY22) ||
//				(pump_type == DN_LAFNG17K) )

		if( (settings_stream1[1].keypad__ == BLSKY18_K) || (settings_stream1[1].keypad__ == BLSKY22) ||
		(settings_stream1[1].keypad__ == LAFNG17_K) || (settings_stream1[1].keypad__ == LAFNG18_K) )
		{
//		   ky = 19;  //F4 key
		   ky = 15;  //clear key
		}
		else
			return 0;      // only for bluesky boards .


		if(keypress_2 != ky)
		{
	        pressed_ = 0;
	        key_buttonpress_tmr2 = 0;  //clr timer.
		}
		 if((key_buttonpress_tmr2 >= 3)&&(pressed_ == 0) )
			 {
			   key_buttonpress_tmr2 = 3;
			    pressed_ = 1;
			    return 1;
			 }
		 return 0;
}

/////////////////////////////////////////////////////////////////

uint8_t long_press_progExit2()
{
	static int pressed_ = 0;
		//static int pressed_old = 0;
		int ky;

//		if(pump_type == bluesky)
//		if( (pump_type == DN_BLSKY18K) || (pump_type == DN_BLSKY22) ||
//			(pump_type == DIN_BLSKY18K) || (pump_type == DIN_BLSKY22) ||
//			(pump_type == DN_LAFNG17K) )
		if( (settings_stream1[1].keypad__ == BLSKY18_K) || (settings_stream1[1].keypad__ == BLSKY22) ||
			(settings_stream1[1].keypad__ == LAFNG17_K) || (settings_stream1[1].keypad__ == LAFNG18_K) )
		{
		   ky = 14;  //'.' key
		}
		else
			return 0;      // only for bluesky boards .


		if(keypress_2 != ky)
		{
	        pressed_ = 0;
	        progExit_buttonpress_tmr2 = 0;  //clr timer.
		}
		 if((progExit_buttonpress_tmr2 >= 3)&&(pressed_ == 0) )
		 {
			 progExit_buttonpress_tmr2 = 3;
			pressed_ = 1;
			return 1;
		 }
		 return 0;
}

/////////////////////////////////////////////////////////////////

uint8_t long_press_log2()
{
	static int pressed_ = 0;
	//static int pressed_old = 0;
	int ky;


	if (readkey192_state() != 1)
	{
        pressed_ = 0;
        log_buttonpress_tmr2 = 0;   //clr timer.
	}
	 if ( (log_buttonpress_tmr2 >= 3)&&(pressed_ == 0 ) )
		 {
		   log_buttonpress_tmr2 = 3;
		    pressed_ = 1;
		    return 1;
		 }
	 return 0;
}
/////////////////////////////////////////////////////////////////
uint8_t long_press_tot2()
{
	static int pressed_ = 0;
	//static int pressed_old = 0;
	uint8_t ky;

	static uint8_t	key19StateCount = 0;

	static bool doublePressDetected = false,
				click_in_progress = false;

	int doublePressThreshold = 2000;

	static uint8_t lastKey19State = 0,
				   firstTime_key192 = 1;

	uint8_t key19State;

//	if(pump_type == lafeng) ky = 11;  //keypad type mapping...
//	if(pump_type == DN_LAFNG17K)
//	if(settings_stream1[1].keypad__ == LAFNG17_K)
//		ky = 11;  //keypad type mapping...
//	else if (settings_stream1[1].keypad__ == LAFNG18_K)
//		ky = 17;
//	else
//	  ky = 21;                      //mapped to print key...

	if( (settings_stream1[1].keypad__ == LAFNG17_K) || (settings_stream1[1].keypad__ == LAFNG18_K) )
	{
//		ky = 11;  //keypad type mapping...
		key19State = readkey192_state();

		if( (firstTime_key192 == 1) && (key19State == 1) )
		{
			key19Timer2 = 0;
			firstTime_key192 = 2;
		}

		if (key19State != lastKey19State)
		{
			  //we keep checking for clicks, we don't care about the non-click detection (it will be most of the time)
			  if (key19State == 1)   // Key19 is pressed
			  {
					 if(key19StateCount == 0)
					 {
						 key19Timer2 = 0; //reset the timer only on the first click
						 click_in_progress = true; //we are in the clicking phase
					 }
					 key19StateCount++ ; // it will start as 1 and will keep incrementing
			  }

				 if(click_in_progress == true)
				 {
					 if(key19Timer2 >= 500)   //now we have passed the 500ms let's check how many clicks happened
					 {
						 if(key19StateCount > 1)
						 {

							//it's a double or multiple click
							 doublePressDetected = true;

		//	            	 key19StateCount = 0;

						 }
						 else
						 {

							 //has to be at least 1 so it's a single click, it can never be zero
						 }

						 //reset everything
						 key19StateCount = 0;
						 click_in_progress = false;
						 //no need to waste processor time resetting lastClickTime as it won't be checked until the next click_in_progress = 1 and it will be reset before that anyway

					 }
				 }
			  }

			  lastKey19State = key19State;

			  if (doublePressDetected) {
		//	    Serial.println("Double press detected!");
				  doublePressDetected = false;
				  tot_buttonpress_tmr2 = 3;
				  pressed_ = 1;
				  firstTime_key192 = 1;
				  return 1;
			  }
			}
			else
			{
				ky = 21;                      //mapped to print key...

				if(keypress_2 != ky)
				{
					pressed_ = 0;
					tot_buttonpress_tmr2 = 0;  //clr timer.
				}
				 if((tot_buttonpress_tmr2 >= 3) && (pressed_ == 0) )
				 {
				   tot_buttonpress_tmr2 = 3;
					pressed_ = 1;
					return 1;
				 }
			}


//	if(keypress_2 != ky)
//	{
//        pressed_ = 0;
//        tot_buttonpress_tmr2 = 0;  //clr timer.
//	}
//	 if((tot_buttonpress_tmr2 >= 3)&&(pressed_ == 0) )
//		 {
//		   tot_buttonpress_tmr2 = 3;
//		    pressed_ = 1;
//		    return 1;
//		 }
	 return 0;
}
/////////////////////////////////////////////////////////////////
eSystemState operator_State_Handler2(void)
{
	    static int fxn = 0;
        static int8_t indx1 = 0,
        			  indxx1 = 0;
	    static int pump_indx = 1;
	    static int8_t pump_indxx = 1;

	    extern  log_max;
	    static int8_t key19_sto_ = 0, keyy19_sto_ = 0;

	    int pkey = 0;
	    int8_t keyy19 = 0;

	    uint8_t rd19 = 0;

	   //===========================================================
	    	    // POWER FAILURE SENSE
	   //===========================================================

		#if sense_power == 1
				  if(readpwr() == 0)
				  {
					   modem_power(DEACTIVATE);

					   //count time elapsed
					   if (shutdown_timer2 > 120)
					   {
						   displayandkeypad_power(DEACTIVATE);   //shutdown... after  2 minutes
					   }
				  }
				  else
				  {
					  //clear the timer//
					  shutdown_timer2 = 0;
				  }
		#endif
		//===========================================================




	 //===========================================================

	   if(readNozzle2() == 1)    //@Nozzle-up, the below features should be disabled
	   {
		   nozzleUp_inProgMode2 = 1;

		   indx1 = 0;
		   indxx1 = 0;

		   return idle_State;
	   }


    //===========================================================
  	    rd19 = readkey192_state();

		if ( (rd19 == 1)&&( key19_sto_ == 0) )
			 {
				 if (indx1 == 1)
						indx1 = 2;
				else
						indx1 = 1;
				key19_sto_ = rd19;
			 }
		 key19_sto_ = rd19;
		 keyy19 = rd19;

	//===========================================================
	//                        totaliser view.
	//===========================================================
	  if ( (operatorfxn2 == totaliser_view) )
		{
		  // level 0. totaliser
		   if (indx1 == 0 ) indx1 = 1;
		   if (indxx1 == 0 ) indxx1 = 1;
		  // -------------- test keys....----------------
		  	     pkey = read_keypad2();

		  	         if( (pkey > 0) )  //key detected
		  	         {
		  	        	 if (pkey == 'B')  // up key
		  					{
		  	        		  //indx1++;
		  					    if (indxx1 == 1)
		  					         	indxx1 = 2;
		  					    else
		  					    		indxx1 = 1;
		  					}

		  	        	 if (pkey == 'C')  // down key
		  					{
		  	        		   //indx1--;
		  						if (indxx1 == 1)
		  							indxx1 = 2;
		  						else
		  							indxx1 = 1;
		  					}

		  	        	 if (pkey == 'A')  // back key
							{
		  	        		   send_line12(upper2);
		  	        		   send_line22(middle2);

							   char str__[10]= {0};
						  	   snprintf(str__, sizeof(str__), "%.2f", litre_price2);
							   send_line32(str__);

							   indx1 = 0;
							   indxx1 = 0;

	                           return idle_State;  //go back to idle state.
							}
		  	         } //if pkey

     	 //-----------------------------------------------------
			   if (t2 >= 500)
				 {
		  	    	   char scc[15] = {0};

		  	    	   char line1[9] = {0};
		  	    	   char line2[9] = {0};
	                   int8_t j,
					   	   	  jj = 0,
							  k = 0;
	                   int i,res = 0;

	               //===============================================

				  if (indx1 == 1)
				  {
					  if(indxx1 == 1)
					  {
						  res =  snprintf(scc, sizeof(scc), "%.2f", totaliser_vol1c);
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
							   clear_screen2();
							   printDisp_c2("l",1,0,8,LT,CLEAR);
							   printDisp_c2(line1, 1, (8-jj), 5, LT, NOCLEAR);  //display price
							   send_line22(line2);
							 }
							 else
							 {
								 send_line12("l        ");
								 send_line22("        ");
								 printDisp_f2(totaliser_vol1c, 2, 0,5,RT,CLEAR ); //send_line22(scc);
							 }
						  send_line32("tot A");
					   }  //  if (indx1 == 1)

					   else if(indxx1 == 2)
					   {
						  res =  snprintf(scc, sizeof(scc), "%.2f", totaliser_amt1c);
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
							   clear_screen2();
							   printDisp_c2("P",1,0,8,LT,CLEAR);
							   printDisp_c2(line1,1,(8-jj),5,LT,NOCLEAR);
							   send_line22(line2);
							 }
							 else
							 {
								 send_line12("P        ");
								 send_line22("        ");
								 printDisp_f2(totaliser_amt1c, 2, 0,5,RT,CLEAR ); //send_line22(scc);
							 }
						  send_line32("tot A");
					   }  //  if (indx1 == 1)
				   }

			   //-----------------------------------------------------

				  else if (indx1 == 2)
				  {
					 if(indxx1 == 1)
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
						   clear_screen2();
						   printDisp_c2("l",1,0,8,LT,CLEAR);
						   printDisp_c2(line1,1,(8-jj),5,LT,NOCLEAR);
						   send_line22(line2);
						 }
						 else
						 {
							 send_line12("l        ");
							 send_line22("        ");
							 printDisp_f2(totaliser_vol2c, 2, 0,5,RT,CLEAR ); //send_line22(scc);
						 }
					  send_line32("tot B");
					 }  //  if (indx1 == 2)

					 else if (indxx1 == 2)
					 {
						res =  snprintf(scc, sizeof(scc), "%.2f", totaliser_amt2c);
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
						   clear_screen2();
						   printDisp_c2("p",1,0,8,LT,CLEAR);
						   printDisp_c2(line1,1,(8-jj),5,LT,NOCLEAR);
						   send_line22(line2);
						 }
						 else
						 {
							 send_line12("P        ");
							 send_line22("        ");
							 printDisp_f2(totaliser_amt2c, 2, 0,5,RT,CLEAR ); //send_line22(scc);
						 }
					  send_line32("tot B");
					 }  //  if (indx1 == 2)
				   }

				   t2 = 0;    //reset the timer.
				}  //if (t > 500)
	      return operator_State;
	  } ///if ( (auth2 == no_auth)  )

//================= End of totaliser view. ========================
//=================================================================


//=================================================================
//                        log view.
//=================================================================
	         if (  (operatorfxn2 == log_view) )
	          	{
	        	    static int loop_ = 0;
	            	static int log_indx = 0;
	            	static int log_indx_indx = 0;

	            	static uint32_t flash_loc;
	            	static uint32_t log_no;

	            	float pr_  =  price_real2;
					float vl_  =  amt_real2;
					char *tm_  =  "---------";

	            	if(loop_ == 0)    //do this only the first time
						{
	            	    	// data_size = sizeof(log_a_new);
//						  if (operating_side == side_a)
//							   {
//								 flash_loc = flash_infoA.current_loc;
//								 log_no = flash_infoA.number_logs;
//								 if(log_no == 0)
//									 {
//										  pr_  =  0.00; //price_real1;
//										  vl_  =  0.00; //amt_real1;
//										  tm_  =  "--------";
//									 }
//								    else
//								    {
//								      //calculate the previous save id from the current
//								    	if (flash_loc <= flash_beginA) //at begining of storage
//								    	{
//								    		flash_loc = flash_endA;    // - data_size; //wrap
//								    	}
//								    	else
//								    	{
//								    		flash_loc = flash_loc - data_size;
//								    	   if (flash_loc == 0 )  // flash_beginA) //at begining of storage
//											{
//								    			// this shouldnt occur ,  just in case !
//								    			flash_loc = flash_endA - data_size; //wrap
//											}
//								    	}
//								    }
//								  flash_locc = flash_loc;
//							     }
						  if (operating_side == side_b)
						   {
							 flash_loc = flash_infoB.current_loc;
							 log_no = flash_infoB.number_logs;
							 if(log_no == 0)
							 {
								  pr_  =  0.00; //price_real1;
								  vl_  =  0.00; //amt_real1;
								  tm_  =  "--------";
							 }
							else
							{
								//calculate the previous save id from the current
								if (flash_loc <= flash_beginB) //at begining of storage
								{
									flash_loc = flash_endB;  // - data_size; //wrap
								}
								else
								{
									flash_loc = flash_loc - data_size;
//									if (flash_loc < flash_beginB) //at begining of storage
//									{
//										// this shouldnt occur ,  just in case !
//										flash_loc = flash_endB - data_size; //wrap
//									}
								}
									flash_locc = flash_loc;
							}
						   }

	            		  loop_ = 1;   //disable first time assignment ^^^^
	            		  log_indx_indx = log_no;
	            		  pump_indx  = 1;  // initialise the side...

	            		  flash_read_idB = flash_loc;

	            		  return read_flash_State;  //goto read the flash location...
						}
	         //==================================
	         //   display the log parameters....

	       		 if(pump_indx == 0) pump_indx = 1;
	         // -------- test keys....-----------
	          	  	     pkey = read_keypad2();

	          	  	       if( (pkey > 0) )  //key detected
	          	  	         {
//	          	  	        	   if (pkey == 'C')  // up key
	          	  	    	   	   	if (pkey == 'B')  // up key  //+
	          	  					{
	          	  					    if (log_indx_indx < log_no)
	          	  					    {
	          	  					    	log_indx_indx++;  //incr index only when less than max
//	          	  					    if (operating_side == side_a)
//	          	  					    	{
//	          	  					    		if(flash_loc >= flash_endA)
//												{
//												  flash_loc = flash_beginA; //wrap
//												}
//												else
//												{
//												  flash_loc +=  data_size;
//												}
//												flash_read_idA = flash_loc;
//												flash_locc = flash_loc;
//	          	  					    	}
	          	  					    if (operating_side == side_b)
											{
	          								if(flash_loc >= flash_endB)
												{
												  flash_loc = flash_beginB; //wrap
												}
												else
												{
												  flash_loc += data_size;
												}
												flash_read_idB = flash_loc;
												flash_locc = flash_loc;
											}
	          	  					     }

	          	  					  //  flash_read_idA = flash_loc;
	          	  					    		//flash_beginA + ( log_indx_indx *  );
	          	  					    //fetch the values...
	          	  					    return read_flash_State;
	          	  					}

	          	  	        	 if (pkey == 'C')  // down key
	          	  					{
	          	  	                 	if (log_indx_indx >= 2)
	          	  	                 	{
	          	  	                 		log_indx_indx--; //reduce index only when greater than 0.

//	          	  	                 	 if (operating_side == side_a)
//											{
//	          	  	                        	if(flash_loc <= flash_beginA)
//												{
//												  flash_loc = flash_endA - data_size; //wrap
//												}
//												else
//												{
//												  flash_loc -= data_size;
//												}
//	          	  	                          flash_locc = flash_loc;
//											  flash_read_idA = flash_loc;
//											}

											if (operating_side == side_b)
											{
												if(flash_loc <= flash_beginB)
												{
												  flash_loc = flash_endB - data_size; //wrap
												}
												else
												{
												  flash_loc -= data_size;
												}
												flash_locc = flash_loc;
												flash_read_idB = flash_loc;
											}
											//flash_read_idA = flash_loc;
											//fetch the values...
										  }
	          	  	              	    return read_flash_State;
	          	  					}

//	    		  	        	 if (pkey == 'A')  // back key
	    		  	        		if (pkey == 'A')  // stop/exit key
	    							{
	    		  	        		   loop_ = 0;  // enable the first time assignment
	    		  	        		   pump_indxx = 1;

	    		  	        		   send_line12(upper2);
	    		  	        		   send_line22(middle2);

	    							   char str__[8]= {0};
	    						  	   snprintf(str__, sizeof(str__), "%.2f", litre_price2);
	    							   send_line32(str__);
	    	                           return idle_State;  //go back to idle state.
	    							}

	          	  	           /*  if (pkey == 'F')  //change pump index.
									{
	          	  	            	  pump_indx++;
										if (pump_indx > 2) pump_indx = 1; //wrap around
									}   */
	          	  	         } //if pkey
	          	 //-----------------------------------------------------
	          		   if (t2 >= 500)
	          			 {
	          			   static int scroll,scroll_ = 0;
	          			   if (scroll >= 4)
							   {
	                              if (scroll_++ >= 3) 	  scroll_ = 1;
//	                              else
//	                            	  scroll_ = 0;
	                             scroll = 0;
							   }
	          			     scroll++;

	          	  	    	   //char scc[15] = {0};
	          			    if(pump_indx == 0) pump_indx = 1;
	          	  	    	   char line1[9] = {0};
	          	  	    	   char line2[9] = {0};
	          	  	    	   char line3[8] = {0};

	                          //   int j,k = 0;
	                          //   int i,res = 0;
	           	 if(log_no > 0)
	           	 {

	           		 if ( (keyy19 == 1) && ( keyy19_sto_ == 0) )
					 {
					   pump_indxx++;
						if (pump_indxx > 2) pump_indxx = 1;  //wrap around
						   keyy19_sto_ = keyy19;
					 }
					 keyy19_sto_ = keyy19;

	           		//=======================================================================

////	          	  	    	  if (pump_indxx == 1)
////	          	  	    	  {
//								  pr_  =  log_a_new.pr__;   //price
//								  vl_  =  log_a_new.vol__;  //volume
//								//  tm_  =  log_a_new.tm_;   //time
//
//	          	  	    		  //dd-hh:mm,    P  , L  ,    T
//	          	  	    		  printDisp_c2("p",1,0,8,LT,CLEAR); printDisp_f2(pr_,1,0,8,LT,NOCLEAR);  //display price
//
//	                              if (scroll_ == 1 )
//								   {
//									 printDisp_c2("l",2,0,8,LT,CLEAR); printDisp_f2(vl_,2,0,8,LT,NOCLEAR);  //display litre
//								   }
//	                              else if (scroll_ == 2)
//									{
//	                            	  char st__[10]= {0};
//	                            	  snprintf(st__, sizeof(st__), "%d-%02d-%d",log_a_new.date._dd,log_a_new.date._mm,log_a_new.date._yy);
//									 printDisp_c2(st__,2,0,8,LT,CLEAR);
//									}
//	                              else if (scroll_ == 3)
//	                              {
//	                             	  char st__[10]= {0};
//									  snprintf(st__, sizeof(st__), "%d%02d hrs",log_a_new.time._hh,log_a_new.time._mn);
//									 printDisp_c2(st__,2,0,8,LT,CLEAR);
//	                              }
//	                              	  snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx);
//	                                  send_line32("      ");
//	                              	  send_line32(line3);
//	          	  	    	   }  //  if (indx1 == 1)
	          	  	    	 //-----------------------------------------------------

//	          	  	    	  else if (pump_indxx == 2)
//							  {
								  pr_  =  log_b_new.pr__;   //price
								  vl_  =  log_b_new.vol__;  //volume
								  //  tm_  =  log_b_new.tm_;   //time


								  //dd-hh:mm,    P  , L  ,    T
								 printDisp_c2("P",1,0,8,LT,CLEAR); printDisp_f2(pr_,1,0,8,LT,NOCLEAR);  //display price

								 if (scroll_ == 1 )
								 {
									 printDisp_c2("L",2,0,8,LT,CLEAR); printDisp_f2(vl_,2,0,8,LT,NOCLEAR);  //display litre
								 }
								 else if (scroll_ == 2)
								 {
									  char st__[10]= {0};
									  snprintf(st__, sizeof(st__), "%d-%02d-%d",log_b_new.date._dd,log_b_new.date._mm,log_b_new.date._yy);
									  printDisp_c2(st__,2,0,8,LT,CLEAR);
								 }
								 else if (scroll_ == 3)
								  {
									  char st__[10]= {0};
									  snprintf(st__, sizeof(st__),"%d%02d hrs",log_b_new.time_e._hh,log_b_new.time_e._mn);
									  printDisp_c2(st__,2,0,8,LT,CLEAR);
								  }

								snprintf(line3, sizeof(line3), "Bn1.%d",log_indx_indx );
								send_line32("        ");
								send_line32(line3);
//							  }
           	             }
						 else
						 {
						   send_line12(" No log ");
						   send_line22("        ");
						   send_line32("        ");
						 }   // if log  > 0.

	          		       t2 = 0;    //reset the timer.
	          	        }  //if (t > 500)

	                return operator_State;
	            } ///if ( (auth2 == no_auth) && (access == level0) && (fxn == 1) )
	return operator_State;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

eSystemState error_clear_Handler2(void)
{
	if(irrecov_flag2 == 0)
		{
//			send_line12(upper2);
//			send_line22(middle2);

			if(settings_stream1[1].display_format == PL)
			  {
				 send_line12(upper2);
				 send_line22(middle2);
			  }
			  else if(settings_stream1[1].display_format == LP)
			  {
				  send_line12(middle2);
				  send_line22(upper2);
			  }

			char str__[10]= {0};
			snprintf(str__, sizeof(str__), "%.2f", litre_price2);
			send_line32(str__);
			error_clr_flag2 = 0;  //clear flag..

			if(changeLitrePrice2_2 == 1)
			{
				changeLitrePrice2_2 = 0;
			}

			else if(totalizer2_error == 1)
			{
				totalizer2_error = 0;
			}


			return idle_State;
		}
	    else
	     {
            return inactive_State;  //if the error is an irrecoverable error, dont return to idle state.
	     }
}
//============================================================================================================
uint8_t key_available2()
{
	if ( keypress__2 > 0)
		return 1;
	else
		return 0;
}
//========================================================================
int8_t read_keypad2()
{
   //uint8_t ret = 0;
   int temp = 0;

  if ( keypress__2 > 0)  //If key available.
     {
       keypress__2 = 0;
          if( (kkey2 >= '0' )&&(kkey2 <= '9') )//if numerals
          {
        	if(_index2 > 7)
        	  {
        		  _index2  = 7; temp = 7;
        	  }
            	  else
        	  {
            	  temp = _index2;
        		  keypad_buf2[_index2++] = kkey2;
        		  keypad_buf2[_index2  ] = 0;

        		  keypad_pw_xter2[temp++] = '-';
        		  keypad_pw_xter2[temp  ] = 0;

        		  if ((progg2 == 1) && (auth2 == not_auth))
        			  send_keypad2(keypad_pw_xter2);
        	  }
                return 0;
          }

          else if (kkey2 == 'F' )  //if clear key,
			 {
               _index2 = 0; temp = 0;
               for(int i = 0;i<10;i++)
                {
                	keypad_pw_xter2[i] = 0;
                	keypad_buf2[i] = 0;
                }
               //send_line22("        ");  //clear second line.
               return 'F';
			 }

          if (kkey2 == 'D' )  //if ENTER key.
			 {
        	    return 'D';
			 }

          if (kkey2 == 'B' )  //if UP key.
			 {
				return 'B';
			 }

          else if (kkey2 == 'C' )  //if DOWN key.
			 {
				return 'C';
			 }

          else if (kkey2 == 'F' )  //if clear key.
			 {
				return 'F';
			 }

          else if (kkey2 == 'A' )  //if back key.
			 {
				return 'A';
			 }
//          else if (kkey2 == '*' )  //if + key.
//			 {
//				return '*';
//			 }
//		  else if (kkey2 == '*' )  //if - key.
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
void clr_screen2(void)
{
      send_line12("        ");
      send_line22("        ");
      send_line32("        ");
}
//=============================================================================
eSystemState progState_Handler2(void)
{
	static int indx = 0;
	static int index_menu = 0;
	static int indx2 = 0;
	static int indx3 = 0;
	static int attempts = 0;
	static int fxn = 0;
	static int pump_indx = 1;
	extern  log_max;
	progg2 = 1;
	int temp = 0;
	int pkey = 0;
	static  int access = non;
	static int key19_sto_ = 0;
	static int dp_ = 0;
	static int ln_ = 0;

	static uint8_t firstTime = 1,
			   	   enter_key = 0;

	static uint8_t index_generic = 0;

//	static uint16_t otp_seed2;

	char st__[10] = {0},
		 pass_[9];

	uint8_t rd19 = 0;


#ifndef DEV_MODE
   if(prog_revisitt2 == 1)
   {
		indx = 0;
	    index_menu = 0;    //not in sidea
		indx2 = 0;
		indx3 = 0;
		attempts = 0;
		fxn = 0;
		pump_indx = 1;
		log_max;
		progg2 = 1;    //not in sidea
		temp = 0;
		pkey = 0;
		access = non;
		key19_sto_ = 0;
		dp_ = 0;
		ln_ = 0;

		firstTime = 1;
		enter_key = 0;

		prog_entry2 = 0;

		prog_revisitt2 = 0;
   }
#endif  //#ifndef DEV_MODE

   if(prog_entry2 == 0 )   // first entry...
   {
	   //clear the state variables.
	    indx = 0;
	   	index_menu = 0;
	    indx2 = 0;
	    indx3 = 0;
	    attempts = 0;
	    fxn = 0;
	    pump_indx = 1;
	      //progg = 1;
	     temp = 0;
	     pkey = 0;

	     auth2 = not_auth;
	     access = non;

	     prog_entry2 = 1;  //To prevent clearing variables on re-entry.
   }


//   //===========================================================
//
//     //if(read_event1() == _nozzleup_Event)
//     if(readNozzle2() == 1)  //@Nozzle-up, the below features should be disabled
//     {
//  	   nozzleUp_inProgMode2 = 1;
//
//  	   return idle_State;
//     }
//
//   //===========================================================


   if (auth2 == not_auth)
    {
      //clear keypad....
	  //acquire the password goto level0 access level if non supplied.

	   pkey = read_keypad2();

//	   char pass_[9];

	  if(pkey > 0)  //key detected
	   {
		   if (pkey == 'D') // enter key...
		   {
			  //password provided...  settings[0].passwd1
			   strncpy(pass_, keypad_buf2, sizeof(pass_));

				   if( (index_menu == pass1) && (strcmp(pass_, password_level1) == 0) )  //level 2 access ?
					 {
					   index_menu = 0;
					   auth2 = authed;
						access = level1;
						fxn = nothing;
						clear_buffer2();
						//keypad_buf[0] = 0;
						//keyboard_entry2[0] = 0;
						return prog_State;
					 }
//				   else if(strcmp(pass_, password_level1) == 0)   //level 3 access ?
//				   {
//					  auth2 = authed;
//					  access = level2;
//					  fxn = nothing;
//					  clear_buffer2();
//					 // keypad_buf[0] = 0;
//					 // keyboard_entry2[0] = 0;
//					  return prog_State;
//				   }
				   else if( (index_menu == pass2) && (strcmp(pass_, password_level2) == 0) ) //level 2 access ?
				   {
					   index_menu = 0;
					   auth2 = pre_otp_authed;
					   access = level2;
					   fxn = nothing;
					   clear_buffer2();
					   otp_seed2 = generate_otpVariable2();
					   return prog_State;
				   }
				   else if( (index_menu == pass3) && (strcmp(pass_, password_level3) == 0) ) //level 3 access ?
				   {
					   index_menu = 0;
					   auth2 = pre_otp_authed;
					   access = level3;
					   fxn = nothing;
					   clear_buffer2();

					   otp_seed2 = generate_otpVariable2();
					   return prog_State;
				   }
//				   else if(access == non)             //no match...
//				   {
//					  send_line22("---no---");
//					  HAL_Delay(1500);
//					  send_line22("        ");
//					  clear_buffer2();   	//clear the buffer
//					  send_line22(keypad_pw_xter2);
//					  send_line32("        ");
//				   }
				   else if(access == non)             //no match...
				   {
					  send_line22(" Access");
					  send_line32("Denied");
					  HAL_Delay(200);
					  send_line22("        ");
					  send_line32("        ");
					  HAL_Delay(200);
					  send_line22(" Access");
					  send_line32("Denied");
					  HAL_Delay(200);
					  send_line22("        ");
					  send_line32("        ");
					  HAL_Delay(200);
					  send_line22(" Access");
					  send_line32("Denied");
					  HAL_Delay(200);
					  send_line22("        ");
					  send_line32("        ");
					  clear_buffer2();   	//clear the buffer
					  send_line22(keypad_pw_xter2);
					  send_line32("        ");
				   }
    	  }// ================ if ENTER KEY PRESSED =================
    	   if (pkey == 'F') // cancel key...
		   {
			  clear_buffer2();
			  /* keypad_buf[0] = 0;
			   keyboard_entry2[0] = 0;
			   keypad_pw_xter1[0] = 0;
			   _index = 0; */
		   }
    	   else if (pkey == 'A') // stop/exit key...
		   {
				clear_buffer2();
				prog_entry2 = 0;
				prog_revisit2 = 1;
				index_menu = 0;

				#ifndef DEV_MODE
					prog_revisitt2 = 1;
				#endif //#ifndef DEV_MODE

			  return idle_State;
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
       }  //if(pkey > 0)  //key detected

	    if (t2 >= 500)  //time to display?
		{
			if(index_menu == 3) index_menu = 2;

			send_line12(pass[index_menu]);  //show password level menu
			send_line22(keypad_pw_xter2);
			send_line32("        ");
			t2 = 0;
		}

	    return prog_State;  //exit...
   }   //if (not_auth)


   //================================== MODE =================================
   //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
   //=========================================================================
   //                           MENU DISPLAY & SCROLL
   //=========================================================================
      else if ( (auth2 == authed) && (access == level1) && (fxn == nothing)  )
      {
   	   if (t2 >= 500)
   		 {
   			if(index_menu == 3) index_menu = 2;
   			send_line12(menu_level1[index_menu]);   //show menu item.
   			send_line22("        ");
   			t2 = 0;
   		 }

   	   	 // -------------- test keys....----------------
   		 pkey = read_keypad2();
   		   if(pump_indx == 0) pump_indx = 1;

   			 if( (pkey > 0) )  //key detected
   			 {
   	//          	        	 if (pkey == 'C')  // up key
   				if (pkey == 'B')  // up key
   				{
   	//          					 if (index_menu < 13)  index_menu++;
   				 if (index_menu <= 1)  index_menu++;
   				 else if(index_menu == 2) index_menu = 0;
   				}
   				if (pkey == 'C')  // down key
   				{
   					if (index_menu > 0) index_menu--;
   					else if(index_menu == 0) index_menu = 2;
   				}

   				 if (pkey == 'A')  // BACK key...
				   {
					   auth2 = authed;
					   access = level1;
					   fxn = nothing;
					   clear_buffer2();
				   }

   				 if (pkey == 'D')  // enter key
   				 {
   						   switch(index_menu)
   						   {
								  case 0 :
										fxn = shift_total;
										clear_buffer2();
										clr_screen2();
										break;

								  case 1 :
										fxn = setprice;
										pump_indx = 1;
										//clear the keyboard buffer
										clear_buffer2();
										clr_screen2();
										break;

								  case 2 :
										fxn = setclock;
										clear_buffer2();
										clr_screen2();
										break;
							}
				}
		 }
	 return prog_State;
   }

   //==================================================================================================


     else if ( (auth2 == pre_otp_authed) && ( (access == level2)||(access == level3) ) && (fxn == nothing)  )
     {
     		if (t2 >= 300)
			{
			  send_line12("OTP Seed");

			  snprintf(st__, sizeof(st__), "    %d", otp_seed2);
			  send_line22(st__);

	//   		  write_v(3, st__);
	//   		  strncpy(keyboard, st__, 8);
				for(uint8_t ii = 0 ; ii < 7; ii++)
				{
					keyboard2[ii] = st__[ii+1];
					keyboard2[ii+1] = 0;
				}
			  send_keypad2(keyboard2);


			  send_line32("  B  ");

			  t2 = 0;
			}

        	 // -------------- test keys....----------------
        	 pkey = read_keypad2();

//        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//          //====================================================================
//          //             show the  MENU and scroll thru with UP and DOWN.
//          //====================================================================
//   else if ( (auth2 == authed) && (access == level1)&&(fxn == nothing)  )
//               {
//				   if (t2 >= 500)
//					 {
//					    if(index_menu == 15) index_menu = 14;
//						send_line12(menu2[index_menu]);   //show menu item.
//						send_line22("        ");
//						t2 = 0;
//					 }
//                 // -------------- test keys....----------------
//          	     pkey = read_keypad2();

        	 if(pump_indx == 0) pump_indx = 1;

			 if( (pkey > 0) )  //key detected
			 {
				 if (pkey == 'D')  // enter key
				 {
				   index_menu = 0;
				   auth2 = otp_authed;
				   if(access == level2) access = level2;
				   else if(access == level3) access = level3;

				   otp_codeInt2 = getCode(otp_seed2);

				   sprintf(otp_code2, "%06ld", otp_codeInt2);

				   memset(keypad_buf2, '\0', sizeof(keypad_buf2));

				   fxn = nothing;
				   clear_buffer2();
				 }
			 }
		 return prog_State;
	  }
			  //      end of show menu for level1

	//=================================================================================================


     else if ( (auth2 == otp_authed) && ( (access == level2)||(access == level3) ) && (fxn == nothing)  )
     	{
     		if(keypad_buf2[0] == NULL)
     	    {
     			if ( (t2 >= 300) && (t2 <= 500) )
     			{
     			  send_line12("   OTP  ");
     			  send_line22("_       ");
     			}
     			else if ( (t2 > 500) && (t2 <= 700) )
     			{
     				  send_line22("        ");
     			}


     			  send_line32("  B  ");

     			  if (t2 > 700) t2 = 0;
     	   }

     	   else if (t2 >= 300)
		   {
			 send_line12("   OTP  ");

			 send_line22(keypad_buf2);

			  send_line32("  B  ");

			  t2 = 0;
		   }

			 // -------------- test keys....----------------
			 pkey = read_keypad2();
			   if(pump_indx == 0) pump_indx = 1;

				 if( (pkey > 0) )  //key detected
				 {
					 if (pkey == 'D')  // enter key
					 {
					   strncpy(pass_, keypad_buf2, sizeof(pass_));

					#ifdef OTP_ENABLE
						   if(strcmp(pass_, otp_code2) == 0)  //level 2 0r 3 access ?
						   {
//							   save_otp(side_b);
							   save_config_otpSeed_time_fram(side_b);
					#else
						   if( (strcmp(pass_, otp_code2) == 0) || (strcmp(pass_, otp_code2) != 0) )  //level 2 0r 3 access ?
						   {
					#endif
						   index_menu = 0;
						   auth2 = authed;
						   if(access == level2) access = level2;
						   else if(access == level3) access = level3;
						   fxn = nothing;
						   clear_buffer2();
					   }
					   else
					   {
						   send_line12(" Access ");
						   send_line22(" Denied ");
						   send_line32("        ");
						   HAL_Delay(1000);

						   memset(keypad_buf2, '\0', sizeof(keypad_buf2));
						   clear_buffer2();
						   clear_screen2();
					   }
					 }
				 }
			 return prog_State;
		}

	  //=================================================================================================


     else if ( (auth2 == authed) && ( (access == level2)||(access == level3) ) && (fxn == nothing)  )
     {

    	 	 if (t2 >= 500)
      		 {
      			if(access == level2)
      			{
      				if(index_menu == 20) index_menu = 0;
      				send_line12(menu_level2[index_menu]);   //show menu item.
      			}
      			else if(access == level3)
   			{
      				if(index_menu == 23) index_menu = 0;
      				send_line12(menu_level3[index_menu]);   //show menu item.
   			}

      			send_line22("        ");
      			t2 = 0;
      		 }

      	 // -------------- test keys....----------------
      	 pkey = read_keypad2();
      	   if(pump_indx == 0) pump_indx = 1;

      		 if( (pkey > 0) )  //key detected
      		 {
      //          	        	 if (pkey == 'C')  // up key
      			if (pkey == 'B')  // up key
      			{
      //          					 if (index_menu < 13)  index_menu++;
      			 if (access == level2)
      			 {
      				 if (index_menu <= 20)  index_menu++;
      				 else if(index_menu == 21) index_menu = 0;
      			 }
      			 else if (access == level3)
      			 {
      				if (index_menu <= 22)  index_menu++;
      				else if(index_menu == 23) index_menu = 0;
      			 }
      			}

      //						if (pkey == 'B')  // down key
      			else if (pkey == 'C')  // down key
      			{
      			    if (access == level2)
      			   	{
      			    	if (index_menu > 0) index_menu--;
      			    	else if(index_menu == 0) index_menu = 19;
      			   	}
      			    else if (access == level3)
      			   	{
      			    	if (index_menu > 0) index_menu--;
      			    	else if(index_menu == 0) index_menu = 22;
      			   	}
      			}

      			else if (pkey == 'A')  // BACK key...
				{
				   auth2 = authed;
				   if(access == level2) access = level2;
				   else if(access == level3) access = level3;
				   fxn = nothing;
				   clear_buffer2();
				}

					else if (pkey == 'D')  // enter key
					{

   //   				 if(index_menu == 14) index_menu += 1;
   //{"  nnode ", "Address ", " Nozzle ", "Disp.Form", "  Price ", "no flo. t", "Hi. Litre", "Ch  Pass", "Calibrat.", "Display ", "  Cloc",
   //"Sides No", "Start CL", "Calib.Can", "Shift No"};
   //"Calib.typ"

      					   switch(index_menu)
      						{
      							  case setmode:
      									fxn = setmode;
      									pump_indx = 1;
      									return prog_State;
      									clear_buffer2();
      									clr_screen2();
      									break;

      							  case setaddress:
      									pump_indx = 1;
      									fxn = setaddress;
      									//clear the keyboard buffer
      									//index_ = 0;
      									clear_buffer2();
      									clr_screen2();
      								    break;

      							  case setnozzle:
      									pump_indx = 1;
      									fxn = setnozzle;
      									clear_buffer2();
      									clr_screen2();
      									break;

      							  case display_orientation:
      								   pump_indx = 1;
      								   fxn = display_orientation;
      								   clear_buffer2();
      								   clr_screen2();
      								   break;

      							  case setprice:
      									fxn = setprice;
      									pump_indx = 1;
      									clear_buffer2();
      									clr_screen2();
      									break;

      							  case set_no_flow_time:
      									fxn = set_no_flow_time;
      									clear_buffer2();
      									clr_screen2();
      									break;

      							  case setmax_amt:
      								   fxn = setmax_amt;
      								   clear_buffer2();
      								   clr_screen2();
      								   break;

      							  case set_password:
      									attempts = 0;
      									fxn = set_password;
      									clear_buffer2();
      									clr_screen2();
      									break;

   //   							  case calibrate_:
      							  case 8 :
      								   fxn = calibrate_;
      								   clr_screen2();
      								   clear_buffer2();
      								   break;
   //   							  case display_:
      							  case 9 :
      								   fxn = display_;
      								   clear_buffer2();
      								   clr_screen2();
      								   dp_ = 1;
      								   ln_ = 1;
      								   break;

   //   							  case setclock:
      							  case 10 :
      									fxn = setclock;
      									clear_buffer2();
      									clr_screen2();
      									break;

      							//"Sides No"
      							  case 11 :
   									fxn = number_of_sides;
   									clear_buffer2();
   									clr_screen2();
   									break;

   							  //"Start CL"
      							  case 12:
   									fxn = suppressed_display_vol;
   									clear_buffer2();
   									clr_screen2();
   									break;

   							  //"Calib.Can"
      							  case 13:
   									fxn = calibration_can;
   									clear_buffer2();
   									clr_screen2();
   									break;

   							  //"Shift Type"
      							  case 14:
   									fxn = shift_login_type_;
   									clear_buffer2();
   									clr_screen2();
   									break;

   							  //"Shift No"
   							  case 15:
   									fxn = number_of_shifts_;
   									clear_buffer2();
   									clr_screen2();
   									break;

   							//Key Tone
							  case 16:
									fxn = keypress_tone_;
									clear_buffer2();
									clr_screen2();
									break;

							  case 17:
									fxn = pulser_config;
									clear_buffer2();
									clr_screen2();
									break;

							  //"valve_config"
							  case 18:
									fxn = valve_config;
									clear_buffer2();
									clr_screen2();
									break;

							  //"no_calibration"
							  case 19:
									fxn = no_calibration;
									clear_buffer2();
									clr_screen2();
									break;

							  //Calib.Typ
							  case 20:
									fxn = calibration_type_;
									clear_buffer2();
									clr_screen2();
									break;

							  //"Com Card"
							  case 21:
									fxn = communication_card;
									clear_buffer2();
									clr_screen2();
									break;

      						}
      				}
      		 }
      	 return prog_State;
     }
     //      end of show menu



   //================================= MODE ==================================
     //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
     //=========================================================================
     //                      SET MODE  ( ONLINE / OFFLINE )
     //=========================================================================
      else if ( (auth2 == authed) && ( (access == level2)||(access == level3) ) && (fxn == setmode)  )
      {
   	   if (t2 >= 300)
   		 {
   			//send_line22();
//   			if(copy[pump_indx-1].mode == AUTO)
   			if(copy_stream1[1].mode == AUTO_MODE)
			{
				send_line22(" Auto ");
			}
		    else
			{
				send_line22("NNanual ");
			}

//   			 if (pump_indx == 1)
//   				 {
   					//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
   					send_line32("  B  ");
//   				 }
//   			 if (pump_indx == 2)
//   				 {
//   					//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
//   					send_line32("  B  ");
//   				 }
   			t2 = 0;
   		 }
   	// -------------- test keys....----------------
   	rd19 = readkey192_state();

   	if ( (rd19 == 1)&&( key19_sto_ == 0) )
   	 {
   		pump_indx++;
   		if (pump_indx > 2) pump_indx = 1;  //wrap around
   		   key19_sto_ = rd19;
   	 }
   	key19_sto_ = rd19;

   	 pkey = read_keypad2();

   		 if( (pkey > 0) )  //key detected
   		 {
   			 if (pkey == 'B')  // up key
   				{
   				  if (copy_stream1[1].mode == AUTO_MODE) //;
   					copy_stream1[1].mode = MANUAL_MODE;
   				  else
   					copy_stream1[1].mode = AUTO_MODE;
   				}

   			 else if (pkey == 'C')  // down key
   				{
   					if (copy_stream1[1].mode == AUTO_MODE) //;
   						copy_stream1[1].mode = MANUAL_MODE;
   					  else
   						copy_stream1[1].mode = AUTO_MODE;
   				}

   			 else if (pkey == 'F')  //change pump index.
   				{
   				  // pump_indx++;
   					//if (pump_indx > 2) pump_indx = 1; //wrap around
   				}

			 else if (pkey == 'D')  // back key
			 {
				  if (copy_stream1[1].mode == AUTO_MODE) //;
					  copy_stream1[1].def_t = P;
				  else if (copy_stream1[1].mode == MANUAL_MODE)
					  copy_stream1[1].def_t = P;
//				  else if (copy[pump_indx-1].mode == VOUCHER_ONLY)
//					  copy[pump_indx-1].def_t = V;

				fxn = nothing;
				clr_screen2();
 //				send_line32("      ");
			 }

			 else if (pkey == 'A')  // back key
   				{
   					fxn = nothing;
   					clr_screen2();
//   					send_line32("        ");
   				}
   		 }
   	 return prog_State;
      }

     //=================================== MODE =================================
     //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
     //==========================================================================
     //                			   SET MODE-OVERIDE
     //==========================================================================
      else if ( (auth2 == authed) && ( (access == level2)||(access == level3) ) && (fxn == setnozzle)  )
      {
   	   if (t2 >= 300)
   	   {
   			//send_line22();
//   		  if(copy[pump_indx-1].noz == overide)
   		  if(copy_stream1[1].noz_override == override)
   		  {
   			send_line22("Active");
   		  }
   		  else
   		  {
   			send_line22("Inactive");
   		  }

//   		 if (pump_indx == 1)
//   		 {
//   			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
//   			send_line32("  A  ");
//   		 }
//   		 if (pump_indx == 2)
//   		 {
   			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
   			send_line32("  B  ");
//   		 }
   		t2 = 0;
   	   }
   	 // -------------- test keys....----------------
   		rd19 = readkey192_state();

   		if ( (rd19 == 1)&&( key19_sto_ == 0) )
   		 {
   			pump_indx++;
   			if (pump_indx > 2) pump_indx = 1;  //wrap around
   			   key19_sto_ = rd19;
   		 }
   		 key19_sto_ = rd19;

   		 pkey = read_keypad2();

   		 if( (pkey > 0) )  //key detected
   		 {
   			 if (pkey == 'B')  // up key
   				{
//   				 if (copy[pump_indx-1].noz == overide)
   				 if (copy_stream1[1].noz_override == override)
   					copy_stream1[1].noz_override = nooverride;
   				 else
   					copy_stream1[1].noz_override = override;
   				}

   			 else if (pkey == 'C')  // down key
   				{
   				 if (copy_stream1[1].noz_override == override)
   					copy_stream1[1].noz_override = nooverride;
   				 else
   					copy_stream1[1].noz_override = override;
   				}

   			 else if (pkey == 'F')  //change pump index.
   			 {
   			  // pump_indx++;
   			  // if (pump_indx > 2) pump_indx = 1; //wrap around
   			 }

   			 else if (pkey == 'D')  // back key
   				{
   					 fxn = nothing;
   					 clr_screen2();
   				}

   			 else if (pkey == 'A')  // back key
   				{
   					 fxn = nothing;
   					 clr_screen2();
   				}
   		 }
   	 return prog_State;
     }

     //================================ MODE ================================
     //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
     //======================================================================
     //                      	  DISPLAY ORIENTATION
     //======================================================================
      else if ( (auth2 == authed) && ( (access == level2)||(access == level3) ) && (fxn == display_orientation)  )
      {

   	   if (t2 >= 300)
   	   {
   		   if( (index_generic !=  PL) && (index_generic !=  LP) ) index_generic = PL;

   		   if(index_generic ==  PL)
   		   {
   			   send_line12("PL NNode");
   		   }
   		   else
   		   {
   			   send_line12("LP NNode");
   		   }

   		  send_line32("  B  ");

   			t2 = 0;
   		}
   	 // -------------- test keys....----------------
   		rd19 = readkey192_state();

   		if ( (rd19 == 1)&&( key19_sto_ == 0) )
   			 {
   				pump_indx++;
   				if (pump_indx > 2) pump_indx = 1;  //wrap around
   				   key19_sto_ = rd19;
   			 }
   		 key19_sto_ = rd19;

   		 pkey = read_keypad2();

   		 if( (pkey > 0) )  //key detected
   		 {
   			 if (pkey == 'B')  // up key
   			 {
   				 if (index_generic == PL)  index_generic = LP;
   				 else if(index_generic == LP) index_generic = PL;
   			 }

   			 else if (pkey == 'C')  // down key
   			 {
   				 if (index_generic == LP)  index_generic = PL;
   				 else if(index_generic == PL) index_generic = LP;
   			 }

   			 else if (pkey == 'F')  //change pump index.
   			 {
   				  // pump_indx++;
   				  //if (pump_indx > 2) pump_indx = 1; //wrap around
   			 }

   			 else if (pkey == 'D')  // Enter key
   			 {
   				 if(index_generic == PL)
   				 {
   					copy_stream1[1].display_format = PL;
   					copy_stream1[1].def_t = P;
   				 }
   				 else if(index_generic == LP)
   				 {
   					copy_stream1[1].display_format = LP;
   					copy_stream1[1].def_t = L;
   				 }

   				 fxn = nothing;
   				 index_generic = 0;
   				 clr_screen2();
   			 }

   			 else if (pkey == 'A')  // back key
   			 {
   				 fxn = nothing;
   				 index_generic = 0;
   				 clr_screen2();
   			 }
   		 }
   	 return prog_State;
      }

   	//============================== MODE =================================
   	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
   	//=====================================================================
   	//               			SET LITRE-PRICE
   	//=====================================================================
      else if ( (auth2 == authed) && ( (access == level1)||(access == level2)||(access == level3) ) && (fxn == setprice) )
      {
   	 if (t2 >= 300)
   	  {
   		 //copy[pump_indx-1].price_ =  atoi(keyboard_entry2);
   		 printDisp_f2(copy_stream1[pump_indx-1].price_, 1, 0, 6, RT, CLEAR);
   		 float pr = atoff(keyboard_entry2);
   		 printDisp_f2(pr,2,0,7,RT,CLEAR);

//   		 if (pump_indx == 1)
//   		 {
//   		   send_line32("  A  ");
//   		 }
//
//   		 if (pump_indx == 2)
//   		 {
   			 send_line32("  b  ");
//   		 }
   		t2 = 0;
   	  }
   	 // -------------- test keys....----------------
   	 rd19 = readkey192_state();

   	   if ( (rd19 == 1)&&( key19_sto_ == 0) )
   		 {
   		   keyboard_entry2[0] = 0;   //clear the previous entry...
   		   index_2 = 0;
   		   pump_indx++;
   			if (pump_indx > 2) pump_indx = 1;  //wrap around
   			   key19_sto_ = rd19;
   		 }
   		   key19_sto_ = rd19;


   		 pkey = read_keypad2();

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
//   				 if (pump_indx == 1)   // if side A
//   					{
//   					   copy[0].price_ =  atof(keyboard_entry2);
//   					}
//   				 if (pump_indx == 2)   // if side B
//   					{
   				copy_stream1[1].price_ =  atof(keyboard_entry2);

   				fxn = nothing;
   				clr_screen2();
//   					}
   				}

   			 if (pkey == 'A')  // back key
   				{
   					 fxn = nothing;
   					 clr_screen2();
   				}
   		 }
   	 return prog_State;
      }

   	//=========================== MODE ====================================
   	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
   	//=====================================================================
   	//                          ID SETTING
   	//=====================================================================
      else if ( (auth2 == authed) && ( (access == level2)||(access == level3) ) && (fxn == setaddress)  )
      {
   	   if (t2 >= 500)
   	   {
   			printDisp_i2(copy_stream1[pump_indx-1].noz_addr, 1, 0, 4, RT, CLEAR);
   			send_line12(keyboard_entry2);

//   			if (pump_indx == 1)
//   			 {
//   				send_line32("  A  ");
//   			 }
//
//   			if (pump_indx == 2)
//   			 {
   				send_line32("  B  ");
//   			 }
   			t2 = 0;
   		}
   	 // -------------- test keys....----------------
   		 rd19 = readkey192_state();

   		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
   			 {
   			   keyboard_entry2[0] = 0;   //clear the previous entry...
   				index_2 = 0;
   				pump_indx++;
   				if (pump_indx > 2) pump_indx = 1;  //wrap around
   				   key19_sto_ = rd19;
   			 }
   			   key19_sto_ = rd19;

   			pkey = read_keypad2();

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
//   					 if (pump_indx == 1)   // if side A
//   						{
//   						   copy[0].id_ =  atoi(keyboard_entry2);
//   						}
//   					 if (pump_indx == 2)   // if side b
//   						{
   				copy_stream1[1].noz_addr =  atoi(keyboard_entry2);

   				fxn = nothing;
			    clr_screen2();
//   						}
   			 }

   			 if (pkey == 'A')  // back key
			 {
				 fxn = nothing;
				 clr_screen2();
			 }
   		 }
   	 return prog_State;
      }

   //================================= MODE ===================================
   //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
   //==========================================================================
   //            SET NO FLOW TIME (10 - 120 sec) ==> Default : 30 sec
   //==========================================================================
      if ( (auth2 == authed) &&  ( (access == level2)||(access == level3) ) && (fxn == set_no_flow_time)  )
      {
   	   if (t2 >= 500)
   		 {
   		   printDisp_i2(copy_stream1[pump_indx-1].timeOut_noFlow, 1, 0, 4, RT, CLEAR);
   		   send_line22(keyboard_entry2);

   		 if (pump_indx == 1)
   		 {
   			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
   			send_line32("  A  ");
   		 }

   		 if (pump_indx == 2)
   		 {
   			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
   			send_line32("  B  ");
   		 }
   		t2 = 0;
    }
   	 // -------------- test keys....----------------
   		 rd19 = readkey192_state();

   		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
   			 {
   			   keyboard_entry2[0] = 0;   //clear the previous entry...
   				   index_2 = 0;
   				pump_indx++;
   				if (pump_indx > 2) pump_indx = 1;  //wrap around
   				   key19_sto_ = rd19;
   			 }
   			   key19_sto_ = rd19;

   	 pkey = read_keypad2();

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
//   					 if (pump_indx == 1)   // if side A
//   						{
//   						   copy[0].noFlow_timeOut =  atoi(keyboard_entry2);
//   						}
//   					 if (pump_indx == 2)   // if side b
//   						{
   				 copy_stream1[1].timeOut_noFlow =  atoi(keyboard_entry2);

   				 fxn = nothing;
				 clr_screen2();
//   						}
   			}

   			 if (pkey == 'A')  // back key
   				{
   					 fxn = nothing;
   					 clr_screen2();
   				}
   		 }
   	 return prog_State;
   }

     //============================ MODE ===================================
     //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
     //=====================================================================
     //                         SET MAX AMOUNT
     //=====================================================================
      else if ( (auth2 == authed) &&  ( (access == level2)||(access == level3) ) && (fxn == setmax_amt)  )
      {
   	   if (t2 >= 500)
   	   {
   		   printDisp_i2(copy_stream1[pump_indx-1].max_amt_, 1, 0, 4, RT, CLEAR);
   		   send_line12(keyboard_entry2);

//   		 if (pump_indx == 1)
//   		 {
//   			send_line32("  A  ");
//   		 }
//
//   		 if (pump_indx == 2)
//   		 {
   			send_line32("  B  ");
//   		 }
   		t2 = 0;
   	  }
   	 // -------------- test keys....----------------
   		 rd19 = readkey192_state();

   		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
   			 {
   			    keyboard_entry2[0] = 0;   //clear the previous entry...
   				index_2 = 0;
   				pump_indx++;
   				if (pump_indx > 2) pump_indx = 1;  //wrap around
   				   key19_sto_ = rd19;
   			 }
   			   key19_sto_ = rd19;

   	   pkey = read_keypad2();

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
//   					 if (pump_indx == 1)   // if side A
//   						{
//   						   copy[0].max_amt_ =  atoi(keyboard_entry2);
//   						}
//   					 if (pump_indx == 2)   // if side b
//   						{
   				copy_stream1[1].max_amt_ =  atoi(keyboard_entry2);

   				fxn = nothing;
				clr_screen2();
//   						}
   			}

   			 if (pkey == 'A')  // back key
   				{
   					 fxn = nothing;
   					 clr_screen2();
   				}
   		 }
   	 return prog_State;
      }

    //=============================== MODE =================================
    //======================================================================
    //                         	CLEAR LOG
    //======================================================================
      else if ( (auth2 == authed) && (access == level1)&&(fxn == clr_log_)  )
      {
   	   if (t2 >= 500)
   		 {
   		   //printDisp_i2(copy[pump_indx-1].max_amt_, 1, 0, 4, RT, CLEAR);
   //		   send_line22(keyboard_entry2);
   //
   //		 if (pump_indx == 1)
   //		 {
   			send_line32("  B  ");
   //		 }
   //
   //		 if (pump_indx == 2)
   //		 {
   //			send_line32("  b  ");
   //		 }
   		t2 = 0;
    }
   	 // -------------- test keys....----------------
   //		 int rd19 = readkey192_state();
   //
   //		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
   //			 {
   //			   keyboard_entry2[0] = 0;   //clear the previous entry...
   //				   index_ = 0;
   //				pump_indx++;
   //				if (pump_indx > 2) pump_indx = 1;  //wrap around
   //				   key19_sto_ = rd19;
   //			 }
   //			   key19_sto_ = rd19;

   	 pkey = read_keypad2();

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
   //					 if (pump_indx == 1)   // if side A
   //						{
   //						  //clear side A log
   //						  clear_logA();
   //						  send_line22("        ");
   //						  send_line22("cleared ");
   //						  HAL_Delay(2000);
   //						  clr_screen2(); //send_line22("        ");
   //						  fxn = nothing;
   //						}
   //
   //					 if (pump_indx == 2)   // if side b
   //						{
   //						 //clear side B log
   //						 clear_logB();
   //						 send_line22("        ");
   //						 send_line22("cleared ");
   //						 HAL_Delay(2000);
   //						 clr_screen2(); //send_line22("        ");
   //						 fxn = nothing;
   //						}
   //				}

   			 if (pkey == 'A')  // back key
   				{
   					 keyboard_entry2[0] = 0;   //clear the previous entry...
   					 index_2 = 0;
   					 fxn = nothing;
   					 clr_screen2();
   				}
   		 }
   	 return prog_State;
      }

      //================================= MODE ===============================
      //======================================================================
      //                         	CLEAR TOTALIZER
      //======================================================================
      else if ( (auth2 == authed) && (access == level1)&&(fxn == clr_tot_)  )
       {
    	   if (t2 >= 500)
    		 {
    		   //printDisp_i2(copy[pump_indx-1].max_amt_, 1, 0, 4, RT, CLEAR);
   // 		   send_line22(keyboard_entry2);
   //
   // 		 if (pump_indx == 1)
   // 		 {
    			send_line32("  B  ");
   // 		 }
   //
   // 		 if (pump_indx == 2)
   // 		 {
   // 			send_line32("  b  ");
   // 		 }
    		t2 = 0;
     }
    	 // -------------- test keys....----------------
   // 		 int rd19 = readkey192_state();
   //
   // 		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
   // 			 {
   // 			   keyboard_entry2[0] = 0;   //clear the previous entry...
   // 				   index_ = 0;
   // 				pump_indx++;
   // 				if (pump_indx > 2) pump_indx = 1;  //wrap around
   // 				   key19_sto_ = rd19;
   // 			 }
   // 			   key19_sto_ = rd19;

    	 pkey = read_keypad2();

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
   // 					 if (pump_indx == 1)   // if side A
   // 						{
   // 						  //clear side A log
   // 						  clear_totA();
   // 						  send_line22("        ");
   // 						  send_line22("cleared ");
   // 						 HAL_Delay(2000);
   // 						  fxn = nothing;
   // 						 clr_screen2();
   // 						}
   //
   // 					 if (pump_indx == 2)   // if side b
   // 						{
   // 						 //clear side B log
   // 						 clear_totB();
   // 						 send_line22("        ");
   // 						 send_line22("cleared ");
   // 						 HAL_Delay(2000);
   // 						 fxn = nothing;
   // 						 clr_screen2();
   // 						}
   // 				}

    			 if (pkey == 'A')  // back key
    				{
    					 keyboard_entry2[0] = 0;   //clear the previous entry...
    	 				 index_2 = 0;
    	 				 fxn = nothing;
    					 clr_screen2();
    				}
    		 }
    	 return prog_State;
       }

    //============================== MODE =================================
    //=====================================================================
    //                             SEND CONFIG.
    //=====================================================================
      else if ( (auth2 == authed) && (access == level1) && (fxn == send_config)  )
      {
   	   if (t2 >= 500)
   		 {
   		   //printDisp_i2(copy[pump_indx-1].max_amt_, 1, 0, 4, RT, CLEAR);
   		  // send_line22(keyboard_entry2);
   /*
   		 if (pump_indx == 1)
   		 {
   			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
   			send_line32("  a  ");
   		 }

   		 if (pump_indx == 2)
   		 {
   			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
   			send_line32("  b  ");
   		 }  */

   		t2 = 0;
    }
   	 // -------------- test keys....----------------
   		 rd19 = readkey192_state();

   		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
   			 {
   				pump_indx++;
   				if (pump_indx > 2) pump_indx = 1;  //wrap around
   				   key19_sto_ = rd19;
   			 }
   			   key19_sto_ = rd19;

   	 pkey = read_keypad2();

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
   					  for (int i = 0 ; i < 14;i++)
   					  {
   						//   send the bits to the external system

   					  }

   					  for (int i = 0 ; i < 14;i++)
   					  {
   						  //   send the bits to the external system
   					  }

   					  send_line22("Sent    ");
   					  fxn = nothing;
   					 clr_screen2();
   				}

   			 if (pkey == 'A')  // back key
   				{
   					 fxn = nothing;
   					 clr_screen2();
   				}
   		 }
   	 return prog_State;
      }


     //============================ MODE ===================================
     //=====================================================================
     //                          GET CONFIG.
     //=====================================================================
      else if ( (auth2 == authed) && (access == level1)&&(fxn == get_config)  )
       {
    	   if (t2 >= 3000)
    		 {

    		  send_line32("        ");
    		  send_line22(" Done ");
    		  fxn = nothing;
    		t2 = 0;
       }

     //-------------------------------------------------
     // Receive the bytes sent from the external system
     //       and store in the settings array.





     //-------------------------------------------------
     // -------------- test keys....--------------------
    		 rd19 = readkey192_state();

    		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
    			 {
    				pump_indx++;
    				if (pump_indx > 2) pump_indx = 1;  //wrap around
    				   key19_sto_ = rd19;
    			 }
    			   key19_sto_ = rd19;

    	          pkey = read_keypad2();

   			   if( (pkey > 0) )  //key detected
   				 {
   					 if (pkey == 'A')  // back key
   						{
   							 fxn = nothing;
   							 clr_screen2();
   						}
   				 }
    	 return prog_State;
       }

     //============================ MODE ==================================
     //====================================================================
     //                         SET PASSWORD
     //====================================================================
      else if ( (auth2 == authed) &&  ( (access == level2)||(access == level3) ) && (fxn == set_password)  )
      {
   	   static uint8_t chg_pw = 0;
   	   static char pw1[9] = { 0 };
   //	   static char pw2[9] = { 0 };
   //	   static char pw3[9] = { 0 };

   //	   static uint8_t index_pass = 0;


   	   if(prog_revisit2 == 1)
   	   {
   		   chg_pw = 0;
   		   memset(pw1, '\0', sizeof(pw1));
   //		   memset(pw2, '\0', sizeof(pw2));
   //		   memset(pw3, '\0', sizeof(pw3));

   		   prog_revisit2 = 0;
   	   }

    	   if (t2 >= 500)
    		 {
    		   if(chg_pw == 0)
    		   {
    			   if (access == level2)
    			   {
    				   if(index_generic == 2) index_generic = 0;
    			   }
    			   else if (access == level3)
   			   {
    				   if(index_generic == 3) index_generic = 0;
    			   }

   			   send_line12(keypad_pw_xter2); //keyboard_entry2);
   			   send_line22("  Enter ");
   			   send_line32(ch_pass[index_generic]);  //show password level menu
    		   }
    		   else
    		   {
    			    send_line12(keypad_pw_xter2); //keyboard_entry2);
    			    send_line22("Re-enter");
   				send_line32(" Pass");
    		   }
    	    	t2 = 0;
            }
    	 // -------------- test keys....----------------
    		 rd19 = readkey192_state();

    		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
    			 {
    				pump_indx++;
    				if (pump_indx > 2) pump_indx = 1;  //wrap around
    				   key19_sto_ = rd19;
    			 }
    			   key19_sto_ = rd19;

    	    pkey = read_keypad2();

    		 if( (pkey > 0) )  //key detected
    		 {
    			 if (pkey == 'B')  // up key
   			 {
    				if (access == level2)
   			    {
    					if (index_generic <= 1)  index_generic++;
    					else if(index_generic == 2) index_generic = 0;
   			    }
    				else if (access == level3)
     			    {
    					if (index_generic <= 2)  index_generic++;
   					else if(index_generic == 3) index_generic = 0;
     			    }
   			 }

    			 else if (pkey == 'C')  // down key
   			 {
    				if (access == level2)
    				{
    					if (index_generic > 0) index_generic--;
    					else if(index_generic == 0) index_generic = 1;
    				}
    				else if (access == level3)
    				{
    					if (index_generic > 0) index_generic--;
    					else if(index_generic == 0) index_generic = 2;
    				}
   			 }

    			 else if (pkey == 'D')  //enter key
    			 {
    				 if (chg_pw == 0)
   				  {
   //					 pw1 = atoi(keyboard_entry2);
   					 strncpy(pw1, keyboard_entry2, sizeof(pw1));
   					 chg_pw = 1;
   					 send_line22("Re-enter");
   					 send_line32(" Pass");
   //					 HAL_Delay(2500);
   					 send_line12("        ");
   					 clear_buffer2();
   					 return prog_State;
   				  }
    				 if (chg_pw == 1)
   					  {
   						 if (strcmp(pw1, keyboard_entry2) == 0)  //if strings are same
   							 {
   								 send_line12("        ");
   								 send_line22("Success ");
   								 send_line32("        ");
   //								 HAL_Delay(1000);
   								 send_line12("        ");

   								 if(index_generic == 0)
   								 {
   //									 copy[0].passwd1 = pw1;
   									 strncpy(copy_stream3[0].passwd1, pw1, sizeof(pw1));
   									 strncpy(copy_stream3[1].passwd1, pw1, sizeof(pw1));
   //									 copy[1].passwd1 = pw1;
   								 }
   								 else if(index_generic == 1)
   								 {
   									 strncpy(copy_stream3[0].passwd2, pw1, sizeof(pw1));
   									 strncpy(copy_stream3[1].passwd2, pw1, sizeof(pw1));
   								 }
   								 else if(index_generic == 2)
   								 {
   //									 copy[0].passwd3 = pw1;
   //									 copy[1].passwd3 = pw1;
   									 strncpy(copy_stream3[0].passwd3, pw1, sizeof(pw1));
   									 strncpy(copy_stream3[1].passwd3, pw1, sizeof(pw1));
   								 }

                                    HAL_Delay(1000);
                                    send_line12("Passuurd ");
                                    send_line22("Changed ");
                                    HAL_Delay(1000);
                                    fxn = nothing;
   								 chg_pw = 0;
   								 clear_buffer2();
   								 index_generic = 0;

   								// index_ = 0;
   								//keyboard_entry2[0] = 0;
   								//keypad_pw_xter1[0] = 0;
   								//_index = 0;
   							    return prog_State;

   							 }
   						 else
   						 {

   							    send_line22("---No---");
   							    HAL_Delay(1500);
   							    send_line22("        ");
   							    attempts++;
   							    if (attempts >= 3)
   							    {
   							    	send_line22("        ");
   							    	send_line22(" Denied ");
   							    	HAL_Delay(1500);
   							    	send_line22("        ");
   							    	 fxn = nothing;
   							    	 chg_pw = 1;
   									clear_buffer2();
   							    	 return prog_State;
   							    }
   							    else
   							    {
   							    	send_line22("        ");
   									send_line22(" Retry  ");
   									HAL_Delay(1500);
   									send_line22("        ");
   									return prog_State;
   							    }
   						 }
   						 chg_pw = 1;
   					  }
    				}

    			 if (pkey == 'A')  // back key
   			 {
    			chg_pw = 0;
    			fxn = nothing;
   				clr_screen2();
   				index_generic = 0;
   			 }
    		 }
    	 return prog_State;
       }


      //============================== MODE ================================
          //====================================================================
          //                             CALIBRATE
          //====================================================================
         else if ( (auth2 == authed) &&  ( (access == level2)||(access == level3) ) && (fxn == calibrate_)  )
         {

             static float calib_price = 0.0;
      	   static float inv_price   =  0.0;
      	   static int vol_sold = 1;

             if(prog_revisit2 == 1)
        	   {
        		   calib_price = 0.0;
      		   inv_price = 0.0;
      		   vol_sold = 1;
      		   volume_flag2 = 0;
      		   calibr2 = 0;        // notify the interrupt routine to off the pump
      		   prog_revisit2 = 0;
        	   }

      	   if (volume_flag2 == 0)
      	    {
      		   //select sales volume first...

      //		       snprintf(st__, sizeof(st__), "%d litre", calibrationCan_measure1);

      		       send_line12("  Start  ");

      //		       if (t >= 500)
      //			   {
      ////					if (vol_sold == l1)
      ////					{
      ////						send_line12("10 litre");
      ////					}
      ////					else
      //						send_line12(st__);
      //
      //				   if (pump_indx == 1)
      //					 {
      //						send_line32("  A  ");
      //					 }
      //
      ////				   if (pump_indx == 2)
      ////					 {
      ////						send_line32("  B  ");
      ////					 }
      //				   t = 0;
      //				}
         	 // -------------- test keys....----------------
         		 rd19 = readkey192_state();

         		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
         			 {
         				pump_indx++;
         				if (pump_indx > 2) pump_indx = 1;  //wrap around
         				   key19_sto_ = rd19;
         			 }
         			   key19_sto_ = rd19;

         	    pkey = read_keypad2();

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
      					volume_flag2  = 1;   //set flag and goto dispense
      					calib_pulser2 = 0;  //clear pulser
      		           __HAL_TIM_SET_COUNTER(&htim2,0);   //clear harware pulser here...
      					return prog_State;
         				}

         			 if (pkey == 'A')  // back key
         				{
         					 fxn = nothing;
         				     calibr2 = 0;
         				     clr_screen2();
         				}
         		 }
            	 return prog_State;
      	   }// if selected volume = 0.....
      	//========= volume has been selected =============
          // dispense
            if(volume_flag2 == 1)
            {
               send_line12("Dispense");
               HAL_Delay(2000);
               volume_flag2 = 2;
               calibr2 = 1;        /// notify the interrupt routine to on the pump
               return prog_State;
            }
          //=============================
      	  if(volume_flag2 == 2)  //dispense.
      	   {
      		 if(t2 > 60)
      		  {

      			#if _USE_SOFT_PULSER == 0
      			  calib_pulser2 = __HAL_TIM_GET_COUNTER(&htim2);  //use hardware counter
      			#else
      			  calib_pulser2++; 							// use software counter.
      			#endif

      			  send_line12(" Pulser ");
      			  printDisp_i2(calib_pulser2, 2, 0, 7, RT, CLEAR);

      		    //    power outage during filling1  end transaction...
      			#if sense_power == 1
      			  if( (readpwr() == 0)||(read_p_pwr() == 0) )
      			  {
      				  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
      				  HAL_Delay(200);
      				  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
      				  stop_flow2();
      //				  get_time2();
      //				  do_calcs();
      //				  update_info();

      				  pulser_totalizer2 = ( (calib_pulser2 / (float) (pulser_benchMark2)) * calibrationCan_measure2 );
      				  totaliser_vol2 += pulser_totalizer2;
      				  totaliser_vol2c += pulser_totalizer2;
      				  totaliser_amt2 += (pulser_totalizer2 * litre_price2);
      				  totaliser_amt2c += (pulser_totalizer2 * litre_price2);
      				  amt_real2 = pulser_totalizer2;
      				  amt_middle2 = pulser_totalizer2;
      				  price_real2 = (pulser_totalizer2 * litre_price2);
      				  price_upper2 = (pulser_totalizer2 * litre_price2);
      				  save_volumeTotaliser_fram(operating_side);
      				  save_amountTotaliser_fram(operating_side);
      				  save_lastSale_fram(operating_side);

      				  return write_flash_State;
      			  }
      			#endif
      			  t2 = 0;
      		  }

      			  pkey = read_keypad2();
      //			  if (pkey == 'A')  //off key to stop fueling...  Stop/Exit Button
      				if (pkey == 'D')  //Enter key to transit to the next page
      				{
                          //capture the pulser.
      				  calibr2 = 0;  /// notify the interrupt routine to on the pump

      				  #if _USE_SOFT_PULSER == 0
      				  	  calib_pulser2 = __HAL_TIM_GET_COUNTER(&htim2);  //use hardware counter
      				  #else
      				  	  calib_pulser2++; 							// use software counter.
      				  #endif

      				  pulser_totalizer2 = ( (calib_pulser2 / (float) (pulser_benchMark2)) * calibrationCan_measure2 );

      				  totaliser_vol2c += pulser_totalizer2;
      				  totaliser_vol2 += pulser_totalizer2;
      				  totaliser_amt2c += (pulser_totalizer2 * litre_price2);
      				  totaliser_amt2 += (pulser_totalizer2 * litre_price2);
      				  amt_real2 = pulser_totalizer2;
      				  amt_middle2 = pulser_totalizer2;
      				  price_real2 = (pulser_totalizer2 * litre_price2);
      				  price_upper2 = (pulser_totalizer2 * litre_price2);
      				  save_volumeTotaliser_fram(operating_side);
      				  save_amountTotaliser_fram(operating_side);
//      				  save_lastSale(operating_side);
      				save_lastSale_fram(operating_side);


        //				  calib_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);  //use hardware counter
      				  send_line12("Set your");
      				  send_line22("Volunne ");
      				  HAL_Delay(2000);
      				  volume_flag2 = 3;   //enter the price.


      				  return prog_State;
      				}

      //			  if (pkey == 'A')  // back key
      //				{
      //                 	volume_flag = 0;
      //                    calibr1 = 0;
      //                   // clr_screen2();
      //				}

      		     return prog_State;
      	   }
        //==========================================================

        //==========================================================
          if(volume_flag2 == 3)  //Enter the price.
      	   {
      			  if(t2 > 400)
      				  {
      					send_line12("Volunne ");
      					printDisp_f2(atoff(keyboard_entry2), 2, 0, 7, RT, CLEAR);
      					t2 = 0;
      				  }

      			  pkey = read_keypad2();
      			  if (pkey == 'D')  //ENTER key to adjusted volume.
      				{
      				  float cal_vol = atoff(keyboard_entry2);
      				  send_line12("        ");   //clear screen.
      				  send_line22("        ");
      				  clear_buffer2();
      					//index_ = 0;
      					//keyboard_entry2[0] = 0;  //clear buffer...
      					//keypad_pw_xter1[0] = 0;
      					//_index = 0;
      					//==================================
      					// calculate the price of the pulses
      					// float pop = calib_pulser1  / cal_vol;
      					int sold_v = 0;
      					float pi_c;
      					float pi;

//      					if (vol_sold == l1)
//      					{
//      						sold_v = 10;
//      						pi_c = calib_pulser2 / cal_vol; //calibrated pi.
//      						pi   = calib_pulser2 / sold_v;
//      					}
//      					else
//      					{
      						sold_v = settings_stream2[1].calibration_measureCan;
      						pi_c = calib_pulser2 / cal_vol; //calibrated pi.
      						pi   = calib_pulser2 / sold_v;
//      					}

      					vol_real2 = sold_v;
      					vol_calibrated2 = cal_vol;
      					save_ctSettings(side_b);
//      					save_calibrationPulser(side_b);
      					save_calibrationPulser_fram(side_b);

    					calibration_flag2 = CALIBRATED;
//    					save_calibrationFlag(side_b);
    					save_calibrationFlag_fram(side_b);

                        HAL_Delay(1700);

                          send_line12("  Done  ");
                          printDisp_f2(pi_c, 2,0, 8, RT, CLEAR);
                          HAL_Delay(2500);
                          //store in the settings structure.
//                          if(pump_indx == 1)
//                          {
//                          	// data for side a.
//                          	copy[0].pi_c = pi_c;
//                          	copy[0].pi_  = pi;
//                          }

//                          if(pump_indx == 2)
//      					{
      						// data for side b.
                          copy_stream1[1].pi_cal = pi_c;
      					  copy_stream1[1].pi_real	 = pi;
//      					}
      					 fxn = nothing;
      					 calibr2 = 0;
      					 clr_screen2();
      				}

      			  if (pkey == 'A')  // back key
      				{
      					volume_flag2 = 0;
      					clr_screen2();
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
         else if ( (auth2 == authed) &&  ( (access == level2)||(access == level3) ) && (fxn == display_)  )
         {

      	 if(enter_key == 1)
      	 {
      		 if(timer_config2 < 1000)
      		 {
      			 send_line22("Entered");
      		 }
      		 else
      		 {
      			 enter_key = 0;
      			 fxn = nothing;
      			 clr_screen2();
      		 }

      		 return prog_State;
      	 }

      	 if (ln_ == 0) ln_ = 1;
      	 if (dp_ == 0) dp_ = 1;

      	   if (t2 >= 500)
      		 {
      		    if (ln_ == 1)
      			 {
      				send_line12("   P    ");
      			 }

      			if (ln_ == 2)
      			 {
      				send_line12("   L    ");
      			 }

      			if (ln_ == 3)
      			 {
      				send_line12(" Unit P ");
      			 }
             //---------------------------------
      		    if (dp_ == 1)
      			 {
      				send_line22(" 999999.9");
      			 }

      		    if (dp_ == 2)
      			 {
      				send_line22(" 99999.99");
      			 }

      		    if (dp_ == 3)
      			 {
      		    	send_line22(" 9999.999");
      			 }
      		    if (dp_ == 4)
      			 {
      				send_line22(" 999.9999");
      			 }
              //--------------------------------
//      			 if (pump_indx == 1)
//      			 {
//      				send_line32("  A  ");
//      			 }
//
//      			 if (pump_indx == 2)
//      			 {
      				send_line32("  B  ");
//      			 }
      			t2 = 0;
              }
      	 // -------------- test keys....----------------
      		 rd19 = readkey192_state();

      		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
      			 {
      			   keyboard_entry2[0] = 0;   //clear the previous entry...
      								   index_2 = 0;
      				pump_indx++;
      				if (pump_indx > 2) pump_indx = 1;  //wrap around
      				   key19_sto_ = rd19;
      			 }
      			   key19_sto_ = rd19;

      	   pkey = read_keypad2();

      		 if( (pkey > 0) )  //key detected
      		 {
      			 if (pkey == 'B')  // up key
      				{
      				 	 dp_++;
      				 	 if(dp_ > max_dp) dp_ = 1;
      				}

      			 if (pkey == 'C')  // down key
      				{
      				 	 ln_++;
      				 	 if(ln_ > 3)ln_ = 1;
      				}

      			 if (pkey == 'D')  //enter key
      				{
//      					 if (pump_indx == 1)   // if side A
//      					 {
//      					   if(ln_ == 1) copy[0].dp_price  = dp_;
//      					   if(ln_ == 2) copy[0].dp_amount = dp_;
//      					   if(ln_ == 3) copy[0].dp_unitprice = dp_;
//      					 }
//      					 else if (pump_indx == 2)   // if side b
//      					 {
      					   if(ln_ == 1) copy_stream1[1].dp_amount  = dp_;
      					   if(ln_ == 2) copy_stream1[1].dp_vol = dp_;
      					   if(ln_ == 3) copy_stream1[1].dp_unitprice = dp_;
//      					 }

      					 clr_screen2();
      					 timer_config2 = 0;
      					 send_line22("Entered ");

      					 enter_key = 1;
      					 return prog_State;
      				}

      			 if (pkey == 'A')  // back key
      				{
      					 fxn = nothing;
      					 clr_screen2();
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
         else if ( (auth2 == authed) &&  ( (access == level1)||(access == level2)||(access == level3) ) && (fxn == setclock) )
      	 {

      		    static int8_t respons_dir = 0,
      						   entry_clear = 1,
      						   montth = 0,
      						   failFlag_tm = 0,
      						   failFlag_dt = 0;

      			static bool hh_success = false,
      						yy_success = false;

      		 	if(prog_revisit2 == 1)
      		    {
      		 	   respons_dir = 0;
      			   entry_clear = 1;
      			   montth = 0;
      			   failFlag_tm = 0;
      			   failFlag_dt = 0;

      		    	hh_success = false;
      		    	yy_success = false;

      		    	prog_revisit2 = 0;
      		    }

      		    int8_t respons = 0,
      		    	   tm = 0,
      		    	   dte = 0;

      //		  char st__[10] = {0};

      		  //====================================================
      		  if( (t2>200) && (respons_dir == 0) )
      		  {
      			  send_line12("1- Tinne");
      			  send_line22("2-  Date");
      			  t2 = 0;

      		  }
      		  if(entry_clear)
      			  respons = atoi(keyboard_entry2);

      		 //------------------------------------------------
      		  if( ((respons == 1) || (respons_dir == 1)) && (!failFlag_tm) )
      		  {
      			  if(entry_clear == 1)
      			  {
      				  respons = 0;
      				  respons_dir = 1;
      				  clear_buffer2();
      				  entry_clear = 0;
      			  }

      			  if (!hh_success)
      			  {
      				  snprintf(st__, sizeof(st__), "  %02d", hour);
      				  printDisp_c2("hr",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
      			  }
      			  else if(hh_success)
      			  {
      				  snprintf(st__, sizeof(st__), "  %02d", minute);
      				  printDisp_c2("NNI",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
      			  }

      			  if( (atoi(keyboard_entry2)) <= 59 )
      				  tm = atoi(keyboard_entry2);
      			  else clear_buffer2();

      			  printDisp_i2(tm,2,4,8,RT,CLEAR);

      			  //=======================================
      			  if (clock_save2)
      			  {
      				clock_save2 = false;

      				DS1307_SetHour(ttime2[0]);
      				DS1307_SetMinute(ttime2[1]);
      				DS1307_SetSecond(00);
      				HAL_Delay(15);

      				clr_screen2();
      				send_line22("  Saved");
      				HAL_Delay(1500);

      				hour = DS1307_GetHour();
      				minute = DS1307_GetMinute();
      				second = DS1307_GetSecond();

       				clear_buffer2();
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
      				  clear_buffer2();
      				  entry_clear = 0;
      			  }
      			  if ( (!yy_success) && (!mth_success2) )
      			  {
      				  snprintf(st__, sizeof(st__), "  %02d", year);
      				  printDisp_c2("Yr",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
      			  }
      			  else if( (yy_success) && (!mth_success2) )
      			  {
      				  snprintf(st__, sizeof(st__), "  %02d", month);
      				  printDisp_c2("NNO",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
      			  }
      			  else if(mth_success2)
      			  {
      				  snprintf(st__, sizeof(st__), "  %02d", day);
      				  printDisp_c2("dd",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
      			  }

      			  if( (atoi(keyboard_entry2)) <= 99 )
      				  dte = atoi(keyboard_entry2);
      			  else clear_buffer2();

      			  printDisp_i2(dte,2,4,8,RT,CLEAR);

      			  //=======================================
      			  if (clock_save2)
      			  {
      				clock_save2 = false;

      				DS1307_SetDate(ddate2[0]);
      				DS1307_SetMonth(ddate2[1]);
      				DS1307_SetYear(ddate2[2]);
      //							DS1307_SetDayOfWeek(3);
      				HAL_Delay(15);

      				clr_screen2();
      				send_line22("  Saved");
      				HAL_Delay(500);

      				day = DS1307_GetDate();
      				month = DS1307_GetMonth();
      				year = DS1307_GetYear();
      				dayofweek = DS1307_GetDayOfWeek();

       				clear_buffer2();
       				entry_clear = 1;
       				respons_dir = 0;
      			  }
         		 }

      		 pkey = read_keypad2();

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
      					 ttime2[0] = tm;
      					 clear_buffer2();
      					 hh_success = true;
      				 }
      				 else if ( ((tm <=59) && (hh_success)) && (!failFlag_tm) )
      				 {
      					 ttime2[1] = tm;
      					 clear_buffer2();
      					 hh_success = false;
      					 clock_save = true;
      				 }
      				 else
      				 {
      					 send_line22("        ");
      					 if(!hh_success)
      					 {
      						 snprintf(st__, sizeof(st__), "  %02d", hour); //,log_a_new.time._mn);
      						 printDisp_c2("hr",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
      					 }
      					 else if(hh_success)
      					 {
      						 snprintf(st__, sizeof(st__), "  %02d", minute);
      						 printDisp_c2("NNI",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
      					 }
      					 send_line22(" Failed");
      					 clear_buffer2();

      					 if(!failFlag_tm) _tt2 = 0;

      					 if( (failFlag_tm) && (_tt2 > 600) ) failFlag_tm = 0;
      					 else failFlag_tm = 1;
      				 }
      			}

      			else if( ((pkey == 'D') && ( (respons == 2) || (respons_dir == 2) )) || (failFlag_dt) )  //store entered value.
      			{
      				 if( (dte <= 99) && (!yy_success) && (!mth_success2) && (!failFlag_dt) )
      				 {
      					 ddate2[2] = dte;
      					 clear_buffer2();
      					 yy_success = true;
      				 }
      				 else if ( (dte != 0) && (dte <= 12) && (yy_success) && (!mth_success2) && (!failFlag_dt) )
      				 {
      					 ddate2[1] = dte;
      					 clear_buffer2();
      					 yy_success = false;
      					 mth_success2 = true;
      					 montth = dte;
      				 }
      				else if( (mth_success2) && ((montth == January) || (montth == March) || (montth == May) || (montth == July) || (montth == August) || (montth == October) || (montth == December)) && (!failFlag_dt) )
      				{
      					if ( (dte != 0) && (dte <= 31) )
      					{
      						save_date2(dte);
      					}
      					else goto __fail;
      				}
      				else if( (mth_success2) && ((montth == April) || (montth == June) || (montth == September) || (montth == November)) && (!failFlag_dt) )
      				{
      					if ( (dte != 0) && (dte <= 30) )
      					{
      						save_date2(dte);
      					}
      					else goto __fail;
      				}
      				else if( (mth_success2) && (montth == February) && (!failFlag_dt) )
      				{
      					if( leapYear_calc(ddate2[2]) )
      					{
      						if ( (dte != 0) && (dte <= 29) )
      						{
      							save_date2(dte);
      						}
      						else goto __fail;
      					}
      					else
      					{
      						if ( (dte != 0) && (dte <= 28) )
      						{
      							save_date2(dte);
      						}
      						else goto __fail;
      					}
      				}
      				else
      				{
      					__fail:   send_line22("        ");
      							  if( (!yy_success) && (!mth_success2) )
      							  {
      								  snprintf(st__, sizeof(st__), "  %02d", year);
      								  printDisp_c2("Yr",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
      							  }
      							  else if( (yy_success) && (!mth_success2) )
      							  {
      								  snprintf(st__, sizeof(st__), "  %02d", month);
      								  printDisp_c2("NNO",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
      							  }
      							  else if(mth_success2)
      							  {
      								  snprintf(st__, sizeof(st__), "  %02d", day);
      								  printDisp_c2("dd",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
      							  }

      							 send_line22(" Failed");
      							 clear_buffer2();

      							 if(!failFlag_dt) _tt2 = 0;

      							 if( (failFlag_dt) && (_tt2 > 600) ) failFlag_dt = 0;
      							 else failFlag_dt = 1;
      				 }
      		}

      		if (pkey == 'A')  // back key
      		{
      			 fxn = nothing;
       			 respons_dir = 0;
       			 failFlag_dt = 0;
       			 failFlag_tm = 0;
       			 hh_success = false;
      			 yy_success = false;
       			 mth_success2 = false;
       			 clock_save2 = 0;
       			 montth = 0;
       			 entry_clear = 1;
       			 clr_screen2();
       			 clear_buffer2();
      		}
      	}
      	//=======================================
      	return prog_State;
        }


        //================================ MODE ================================
        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        //======================================================================
        //                NUMBER OF SIDES  ( 1 - 2) ==> default : 2
        //======================================================================
         else if ( (auth2 == authed) && ( (access == level2)||(access == level3) ) && (fxn == number_of_sides)  )
         {

      	   if (t2 >= 300)
      	   {
      		      if( (index_generic != 1) && (index_generic != 2) ) index_generic = 1;

      			  send_line12("Sides  ");

      			  snprintf(st__, sizeof(st__), "       %d", index_generic);
      			  send_line22(st__);


      			  send_line32("  B  ");

      			t2 = 0;
      		}
      	 // -------------- test keys....----------------
      		rd19 = readkey192_state();

      		if ( (rd19 == 1)&&( key19_sto_ == 0) )
			 {
				pump_indx++;
				if (pump_indx > 2) pump_indx = 1;  //wrap around
				   key19_sto_ = rd19;
			 }
      		 key19_sto_ = rd19;

      		 pkey = read_keypad2();

      		 if( (pkey > 0) )  //key detected
      		 {
      			 if (pkey == 'B')  // up key
      			 {
      				 if (index_generic == 1)  index_generic = 2;
      				 else if(index_generic == 2) index_generic = 1;
      			 }

      			 else if (pkey == 'C')  // down key
      			 {
      				 if (index_generic == 2)  index_generic = 1;
      				 else if(index_generic == 1) index_generic = 2;
      			 }

      			 else if (pkey == 'F')  //change pump index.
      			 {
      				  // pump_indx++;
      				  //if (pump_indx > 2) pump_indx = 1; //wrap around
      			 }

      			 else if (pkey == 'D')  // Enter key
      			 {
      				 if(index_generic == 1)
      					copy_stream2[1].noz_count = 1;
      				 else if(index_generic == 2)
      					copy_stream2[1].noz_count = 2;

      				 index_generic = 0;

      				 fxn = nothing;
      				 clr_screen2();
      			 }

      			 else if (pkey == 'A')  // back key
      			 {
      				 index_generic = 0;

      				 fxn = nothing;
      				 clr_screen2();
      			 }
      		 }
      	 return prog_State;
         }


        //=============================== MODE =================================
        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        //======================================================================
        //       SUPPRESSED VOLUME @ START (0 - 10 cL) ==> default : 4cL
        //======================================================================
         else if ( (auth2 == authed) && ( (access == level2)||(access == level3) ) && (fxn == suppressed_display_vol)  )
         {

      	   if (t2 >= 300)
      	   {
      			if(index_generic > 10) index_generic = 0;

      			send_line12("Supp. Vol");

      			snprintf(st__, sizeof(st__), "   %02d cL", index_generic);
      			send_line22(st__);

      			send_line32("  B  ");

      			t2 = 0;
      		}
      	 // -------------- test keys....----------------
      		rd19 = readkey192_state();

      		if ( (rd19 == 1)&&( key19_sto_ == 0) )
			 {
				pump_indx++;
				if (pump_indx > 2) pump_indx = 1;  //wrap around
				   key19_sto_ = rd19;
			 }
      		 key19_sto_ = rd19;

      		 pkey = read_keypad2();

      		 if( (pkey > 0) )  //key detected
      		 {
      			 if (pkey == 'B')  // up key
      			 {
      				 if (index_generic < 10)  index_generic++;
      				 else if(index_generic == 10) index_generic = 0;
      			 }

      			 else if (pkey == 'C')  // down key
      			 {
      				 if (index_generic > 0) index_generic--;
      				 else if(index_generic == 0) index_generic = 10;
      			 }

      			 else if (pkey == 'F')  //change pump index.
      			 {
      				  // pump_indx++;
      				  //if (pump_indx > 2) pump_indx = 1; //wrap around
      			 }

      			 else if (pkey == 'D')  // Enter key
      			 {
      				copy_stream2[1].startUp_suppressVol = (index_generic * 0.01);

      				 index_generic = 0;
      				 fxn = nothing;
      				 clr_screen2();
      			 }

      			 else if (pkey == 'A')  // back key
      			 {
      				 index_generic = 0;

      				 fxn = nothing;
      				 clr_screen2();
      			 }
      		 }
      	 return prog_State;
         }


        //============================= MODE ===================================
        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        //======================================================================
        //       Calibration Can Size (10 L or 20 L) ==>  default : 20L
        //======================================================================
         else if ( (auth2 == authed) && ( (access == level2)||(access == level3) ) && (fxn == calibration_can)  )
         {

      	   if (t2 >= 300)
      	   {
      		   if( (index_generic != 10) && (index_generic != 20) ) index_generic = 10;

      		   send_line12("Can Size");

      		   snprintf(st__, sizeof(st__), "    %02d L", index_generic);
      		   send_line22(st__);

      		   send_line32("  B  ");

      			t2 = 0;
      		}
      	 // -------------- test keys....----------------
      		rd19 = readkey192_state();

      		if ( (rd19 == 1)&&( key19_sto_ == 0) )
			 {
				pump_indx++;
				if (pump_indx > 2) pump_indx = 1;  //wrap around
				   key19_sto_ = rd19;
			 }
      		 key19_sto_ = rd19;

      		 pkey = read_keypad2();

      		 if( (pkey > 0) )  //key detected
      		 {
      			 if (pkey == 'B')  // up key
      			 {
      				 if (index_generic == 10)  index_generic = 20;
      				 else if(index_generic == 20) index_generic = 10;
      			 }

      			 else if (pkey == 'C')  // down key
      			 {
      				 if (index_generic == 20)  index_generic = 10;
      				 else if(index_generic == 10) index_generic = 20;
      			 }

      			 else if (pkey == 'F')  //change pump index.
      			 {
      				  // pump_indx++;
      				  //if (pump_indx > 2) pump_indx = 1; //wrap around
      			 }

      			 else if (pkey == 'D')  // Enter key
      			 {
      				 if(index_generic == 10)
      					copy_stream2[1].calibration_measureCan = 10;
      				 else if(index_generic == 20)
      					copy_stream2[1].calibration_measureCan = 20;

      				 index_generic = 0;
      				 fxn = nothing;
      				 clr_screen2();
      			 }

      			 else if (pkey == 'A')  // back key
      			 {
      				 index_generic = 0;
      				 fxn = nothing;
      				 clr_screen2();
      			 }
      		 }
      	 return prog_State;
         }

        //================================= MODE ===============================
        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        //======================================================================
        //               SHIFT LOGIN-TYPE  ==>  default : None_
        //======================================================================
         else if ( (auth2 == authed) && ( (access == level2)||(access == level3) ) && (fxn == shift_login_type_)  )
         {

      	   if (t2 >= 300)
      	   {
      		   if( (index_generic != None_) && (index_generic != Code_)  && (index_generic != Card_) ) index_generic = None_;
      		   send_line12("Login.Typ");

      //		   snprintf(st__, sizeof(st__), "       %d", index_generic);
      		   send_line22(login_type[index_generic]);

      		   send_line32("  B  ");

      		   t2 = 0;
      		}
      	 // -------------- test keys....----------------
      		rd19 = readkey192_state();

      		if ( (rd19 == 1)&&( key19_sto_ == 0) )
      			 {
      				pump_indx++;
      				if (pump_indx > 2) pump_indx = 1;  //wrap around
      				   key19_sto_ = rd19;
      			 }
      		 key19_sto_ = rd19;

      		 pkey = read_keypad2();

      		 if( (pkey > 0) )  //key detected
      		 {
      			 if (pkey == 'B')  // up key
      			 {
      				 if (index_generic <= 2 )  index_generic++;
      				 else if(index_generic == 3) index_generic = 0;
      			 }

      			 else if (pkey == 'C')  // down key
      			 {
      				 if (index_generic > 0)  index_generic--;
      				 else if(index_generic == 0) index_generic = 2;
      			 }

      			 else if (pkey == 'F')  //change pump index.
      			 {
      				  // pump_indx++;
      				  //if (pump_indx > 2) pump_indx = 1; //wrap around
      			 }

      			 else if (pkey == 'D')  // Enter key
      			 {
      				 if(index_generic == None_)
      					copy_stream2[1].shift_login_type = None_;
      				 else if(index_generic == Code_)
      					copy_stream2[1].shift_login_type = Code_;
      				 else if(index_generic == Card_)
      					copy_stream2[1].shift_login_type = Card_;

      				 index_generic = 0;
      				 fxn = nothing;
      				 clr_screen2();
      			 }

      			 else if (pkey == 'A')  // back key
      			 {
      				 index_generic = 0;
      				 fxn = nothing;
      				 clr_screen2();
      			 }
      		 }
      	 return prog_State;
         }


        //=============================== MODE =================================
        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        //======================================================================
        //             NUMBER OF SHIFTS PER A DAY  ==>  default : 1
        //======================================================================
         else if ( (auth2 == authed) && ( (access == level2)||(access == level3) ) && (fxn == number_of_shifts_)  )
         {

      	   if (t2 >= 300)
      	   {
      		   if( (index_generic != 1) && (index_generic != 2) ) index_generic = 1;
      		   send_line12("Shifts   ");

      		   snprintf(st__, sizeof(st__), "       %d", index_generic);
      		   send_line22(st__);

      		   send_line32("  B  ");

      		   t2 = 0;
      		}
      	 // -------------- test keys....----------------
      		rd19 = readkey192_state();

      		if ( (rd19 == 1)&&( key19_sto_ == 0) )
      			 {
      				pump_indx++;
      				if (pump_indx > 2) pump_indx = 1;  //wrap around
      				   key19_sto_ = rd19;
      			 }
      		 key19_sto_ = rd19;

      		 pkey = read_keypad2();

      		 if( (pkey > 0) )  //key detected
      		 {
      			 if (pkey == 'B')  // up key
      			 {
      				 if (index_generic == 1)  index_generic = 2;
      				 else if(index_generic == 2) index_generic = 1;
      			 }

      			 else if (pkey == 'C')  // down key
      			 {
      				 if (index_generic == 2)  index_generic = 1;
      				 else if(index_generic == 1) index_generic = 2;
      			 }

      			 else if (pkey == 'F')  //change pump index.
      			 {
      				  // pump_indx++;
      				  //if (pump_indx > 2) pump_indx = 1; //wrap around
      			 }

      			 else if (pkey == 'D')  // Enter key
      			 {
      				 if(index_generic == 1)
      					copy_stream2[1].number_of_shifts = 1;
      				 else if(index_generic == 2)
      					copy_stream2[1].number_of_shifts = 2;

      				 index_generic = 0;
      				 fxn = nothing;
      				 clr_screen2();
      			 }

      			 else if (pkey == 'A')  // back key
      			 {
      				 index_generic = 0;
      				 fxn = nothing;
      				 clr_screen2();
      			 }
      		 }
      	 return prog_State;
         }

        //============================== MODE ==================================
        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        //======================================================================
        //               CALIBRATION TYPE  ==> default : Wizard
        //======================================================================
         else if ( (auth2 == authed) && (access == level3) && (fxn == calibration_type_)  )
         {

      	   if (t2 >= 300)
      	   {
      			  if( (index_generic !=  Wizard) && (index_generic != Manual_calib) ) index_generic = Wizard;

      			  if(index_generic ==  Wizard)
      			  {
      				send_line12("UUizard");
      			  }
      			  else
      			  {
      				send_line12("NNanual");
      			  }

      			  send_line32("  B  ");

      			t2 = 0;
      		}
      	 // -------------- test keys....----------------
      		rd19 = readkey192_state();

      		if ( (rd19 == 1)&&( key19_sto_ == 0) )
      			 {
      				pump_indx++;
      				if (pump_indx > 2) pump_indx = 1;  //wrap around
      				   key19_sto_ = rd19;
      			 }
      		 key19_sto_ = rd19;

      		 pkey = read_keypad2();

      		 if( (pkey > 0) )  //key detected
      		 {
      			 if (pkey == 'B')  // up key
      			 {
      				 if (index_generic == Wizard)  index_generic = 2;
      				 else if(index_generic == 2) index_generic = Wizard;
      			 }

      			 else if (pkey == 'C')  // down key
      			 {
      				 if (index_generic == 2)  index_generic = Wizard;
      				 else if(index_generic == Wizard) index_generic = 2;
      			 }

      			 else if (pkey == 'F')  //change pump index.
      			 {
      				  // pump_indx++;
      				  //if (pump_indx > 2) pump_indx = 1; //wrap around
      			 }

      			 else if (pkey == 'D')  // Enter key
      			 {
      				 if(index_generic == Wizard)
      					copy_stream2[1].calibration_type = Wizard;
      				 else if(index_generic == Manual_calib)
      					copy_stream2[1].calibration_type = Manual_calib;

      				 index_generic = 0;
      				 fxn = nothing;
      				 clr_screen2();
      			 }

      			 else if (pkey == 'A')  // back key
      			 {
      				 index_generic = 0;
      				 fxn = nothing;
      				 clr_screen2();
      			 }
      		 }
      	 return prog_State;
         }

       //================================== MODE ===============================
       //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
       //=======================================================================
       //                  		    SHIFT-TOTALIZER
       //=======================================================================
         else if ( (auth2 == authed) && (access == level1) && (fxn == shift_total)  )
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
      	   if (t2 >= 300)
      	   {
      		  if(index_generic == 0)
			  {
				 if(attendant2.session_id[0] != NULL)
				 {
					 shiftTotaliser_vol = (totaliser_vol2c - startShiftTotaliser_vol2c);

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
					   clear_screen2();
					   printDisp_c2("l",1,0,8,LT,CLEAR);
					   printDisp_c2(line1, 1, (8-jj), 5, LT, NOCLEAR);  //display price
					   send_line22(line2);
					 }
					 else
					 {
						 send_line12("l        ");
						 send_line22("        ");
						 printDisp_f2(shiftTotaliser_vol, 2, 0,5,RT,CLEAR ); //send_line22(scc);
					 }
				  }
				  else
				  {
					  send_line12("L      ");
				  }
			  }
			  else if(index_generic == 1)
			  {
				  if(attendant2.session_id[0] != NULL)
				  {
					  shiftTotaliser_amt = (totaliser_amt2c - startShiftTotaliser_amt2c);
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
						   clear_screen2();
						   printDisp_c2("p",1,0,8,LT,CLEAR);
						   printDisp_c2(line1,1,(8-jj),5,LT,NOCLEAR);
						   send_line22(line2);
						 }
						 else
						 {
							 send_line12("p        ");
							 send_line22("        ");
							 printDisp_f2(shiftTotaliser_amt, 2, 0,5,RT,CLEAR ); //send_line22(scc);
						 }
				  }  //  if (indx1 == 1)
				  else
				  {
					  send_line12("P      ");
				  }

			  }

			  send_line32("Sh.TotA");

      			t2 = 0;
      		}
      	 // -------------- test keys....----------------
      		rd19 = readkey192_state();

      		if ( (rd19 == 1)&&( key19_sto_ == 0) )
      			 {
      				pump_indx++;
      				if (pump_indx > 2) pump_indx = 1;  //wrap around
      				   key19_sto_ = rd19;
      			 }
      		 key19_sto_ = rd19;

      		 pkey = read_keypad2();

      		 if( (pkey > 0) )  //key detected
      		 {
      			 if (pkey == 'B')  // up key
      			 {
      				 if (index_generic == 0)  index_generic = 1;
      				 else if(index_generic == 1) index_generic = 0;
      			 }

      			 else if (pkey == 'C')  // down key
      			 {
      				 if (index_generic == 1)  index_generic = 0;
      				 else if(index_generic == 0) index_generic = 1;
      			 }

      			 else if (pkey == 'F')  //change pump index.
      			 {
      				  // pump_indx++;
      				  //if (pump_indx > 2) pump_indx = 1; //wrap around
      			 }

      //			 else if (pkey == 'D')  // Enter key
      //			 {
      //				 if(index_generic == 1)
      //				   copy[0].side_size = 1;
      //				 else if(index_generic == 2)
      //				   copy[0].side_size = 2;
      //
      //				 index_generic = 0;
      //				 fxn = nothing;
      //				 clr_screen2();
      //			 }

      			 else if (pkey == 'A')  // back key
      			 {
      				 index_generic = 0;

      				 fxn = nothing;
      				 clr_screen2();
      			 }
      		 }
      	 return prog_State;
         }

       //================================================== MODE ================================================
       //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
       //========================================================================================================
       //                  		     KEYPRESS-TONE ( Yes / No )  ==> Default : No
       //========================================================================================================
         else if ( (auth2 == authed) && ( (access == level2)||(access == level3) ) && (fxn == keypress_tone_)  )
            {

         	   if (t2 >= 300)
         	   {
         		   send_line12("Suuitch ");

         		   if( (index_generic !=  Yes) && (index_generic !=  No) ) index_generic = Yes;

         		   if(index_generic ==  Yes)
         		   {
         			   send_line22("      ON");
         		   }
         		   else
         		   {
         			   send_line22("     OFF");
         		   }

         		  send_line32("  B  ");

         			t2 = 0;
         		}
         	 // -------------- test keys....----------------
         		rd19 = readkey192_state();

         		if ( (rd19 == 1)&&( key19_sto_ == 0) )
         			 {
         				pump_indx++;
         				if (pump_indx > 2) pump_indx = 1;  //wrap around
         				   key19_sto_ = rd19;
         			 }
         		 key19_sto_ = rd19;

         		 pkey = read_keypad2();

         		 if( (pkey > 0) )  //key detected
         		 {
         			 if (pkey == 'B')  // up key
         			 {
         				 if (index_generic == Yes)  index_generic = No;
         				 else if(index_generic == No) index_generic = Yes;
         			 }

         			 else if (pkey == 'C')  // down key
         			 {
         				 if (index_generic == No)  index_generic = Yes;
         				 else if(index_generic == Yes) index_generic = No;
         			 }

         			 else if (pkey == 'F')  //change pump index.
         			 {
         				  // pump_indx++;
         				  //if (pump_indx > 2) pump_indx = 1; //wrap around
         			 }

         			 else if (pkey == 'D')  // Enter key
         			 {
         				 if(index_generic == Yes)
         				 {
         					copy_stream2[1].keypress_tone = Yes;
         				 }
         				 else if(index_generic == No)
         				 {
         					copy_stream2[1].keypress_tone = No;
         				 }

         				 fxn = nothing;
         				 index_generic = 0;
         				 clr_screen2();
         			 }

         			 else if (pkey == 'A')  // back key
         			 {
         				 fxn = nothing;
         				 index_generic = 0;
         				 clr_screen2();
         			 }
         		 }
         	 return prog_State;
            }

      	//=================================================  MODE =============================================
      	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
      	//=====================================================================================================
      	//                    ENFORCE COMMUNICATION CARD ( Yes / No )  ==> Default : Yes
      	//=====================================================================================================
      	else if ( (auth2 == authed) && (access == level3) && (fxn == communication_card)  )
      	   {

      		   if (t2 >= 300)
      		   {
      			   send_line12("Enforce ");

      			   if( (index_generic !=  Yes) && (index_generic !=  No) ) index_generic = Yes;

      			   if(index_generic ==  Yes)
      			   {
      				   send_line22("     Yes");
      			   }
      			   else
      			   {
      				   send_line22("      No");
      			   }

      			  send_line32("  B  ");

      				t2 = 0;
      			}
      		 // -------------- test keys....----------------
      			rd19 = readkey192_state();

      			if ( (rd19 == 1)&&( key19_sto_ == 0) )
      				 {
      					pump_indx++;
      					if (pump_indx > 2) pump_indx = 1;  //wrap around
      					   key19_sto_ = rd19;
      				 }
      			 key19_sto_ = rd19;

      			 pkey = read_keypad2();

      			 if( (pkey > 0) )  //key detected
      			 {
      				 if (pkey == 'B')  // up key
      				 {
      					 if (index_generic == Yes)  index_generic = No;
      					 else if(index_generic == No) index_generic = Yes;
      				 }

      				 else if (pkey == 'C')  // down key
      				 {
      					 if (index_generic == No)  index_generic = Yes;
      					 else if(index_generic == Yes) index_generic = No;
      				 }

      				 else if (pkey == 'F')  //change pump index.
      				 {
      					  // pump_indx++;
      					  //if (pump_indx > 2) pump_indx = 1; //wrap around
      				 }

      				 else if (pkey == 'D')  // Enter key
      				 {
      					 if(index_generic == Yes)
      					 {
      						copy_stream2[1].commCard_enforced = true;
      					 }
      					 else if(index_generic == No)
      					 {
      						copy_stream2[1].commCard_enforced = false;
      					 }

      					 fxn = nothing;
      					 index_generic = 0;
      					 clr_screen2();
      				 }

      				 else if (pkey == 'A')  // back key
      				 {
      					 fxn = nothing;
      					 index_generic = 0;
      					 clr_screen2();
      				 }
      			 }
      		 return prog_State;
      	   }

      //============================== MODE ================================
      //====================================================================
      //                           PULSER CONFIG.
      //====================================================================
       else if ( (auth2 == authed) && ( (access == level2)||(access == level3) ) && (fxn == pulser_config)  )
       {
     	 static uint8_t pulserr = 0,
     					pulserType_original,
     					pulserOffset_original;

           if(prog_revisit2 == 1)
      	   {
     			   pulser_flag2 = 0;
     			   prog_revisit2 = 0;
      	   }

     	   if (pulser_flag2 == 0)
     	    {
     		       if (t2 >= 500)
     			   {
     					if (pulserr == 0)
     					{
     						send_line12("  Type  ");
     					}
     					else
     						send_line12(" Offset  ");


     					send_line32("  B  ");

     				   t2 = 0;
     				}
       	 // -------------- test keys....----------------
       		  rd19 = readkey192_state();

       		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
       			 {
       				pump_indx++;
       				if (pump_indx > 2) pump_indx = 1;  //wrap around
       				   key19_sto_ = rd19;
       			 }
       			   key19_sto_ = rd19;

       	     pkey = read_keypad2();

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
     					pulser_flag2  = 1;   //set flag and goto

     					if (pulserr == 0)
     					{
     						 pulserr = 2;
     					}
     					else
     						 pulserr = 3;

     					pulserType_original = copy_stream2[1].pulser_type_;
     					pulserOffset_original = copy_stream2[1].pulser_offset;

     					return prog_State;
       			  }

       			 else if (pkey == 'A')  // back key
				 {
					 fxn = nothing;
					 clr_screen2();
				 }
       		 }
          	 return prog_State;
     	   }// if selected


     	//========= pulser type has been selected =============
     	else if(pulser_flag2 == 1)
        {
     		   if (t2 >= 500)
     		   {
     			   if (pulserr == 2)
     			   {
     				   if (copy_stream2[1].pulser_type_ == quadrature)
     					{
     						send_line12("Ouadrat. ");
     					}
     					else if (copy_stream2[1].pulser_type_ == non_quadrature)
     					{
     						send_line12("Non-Quad.");
     					}

     					send_line32("  B  ");
     			   }
     			   else if (pulserr == 3)
     			   {
     				   if(index_generic > 99) index_generic = 0;

     					send_line12("Ofset.Val");

     					snprintf(st__, sizeof(st__), "%02d units", index_generic);
     					send_line22(st__);

     					send_line32("  B  ");
     			   }

     			   t2 = 0;
     			}

     		 // -------------- test keys....----------------
     			 rd19 = readkey192_state();

     			   if ( (rd19 == 1)&&( key19_sto_ == 0) )
     				 {
     					pump_indx++;
     					if (pump_indx > 2) pump_indx = 1;  //wrap around
     					   key19_sto_ = rd19;
     				 }
     				   key19_sto_ = rd19;

     			pkey = read_keypad2();

     			 if( (pkey > 0) )  //key detected
     			 {
     				 if (pulserr == 2)
     				 {
     					 if (pkey == 'B')  // up key
     					 {
     						 if (copy_stream2[1].pulser_type_ == quadrature)
     						 {
     							copy_stream2[1].pulser_type_ = non_quadrature;
     						 }
     						 else
     							copy_stream2[1].pulser_type_ = quadrature;
     					 }

     					 else if (pkey == 'C')  // down key
     					 {
     						 if (copy_stream2[1].pulser_type_ == quadrature)
     						 {
     							copy_stream2[1].pulser_type_ = non_quadrature;
     						 }
     						 else
     							copy_stream2[1].pulser_type_ = quadrature;
     					 }

     					 else if (pkey == 'D')  //enter key
     					 {
     						pulser_flag2  = 0;
     						pulserr = 0;
     						return prog_State;
     					 }

     					 else if (pkey == 'A')  // back key
     					 {
     						 fxn = nothing;
     						 copy_stream2[1].pulser_type_ = pulserType_original;

     						 pulserr = 0;
     						 pulser_flag2 = 1;
     						 clr_screen2();
     					 }
     				 }

     				 else if (pulserr == 3)
     				 {
     					 if (pkey == 'B')  // up key
     					 {
     						 if (index_generic < 99)  index_generic++;
     						 else if(index_generic == 99) index_generic = 0;
     					 }

     					 else if (pkey == 'C')  // down key
     					 {
     						 if (index_generic > 0) index_generic--;
     						 else if(index_generic == 0) index_generic = 99;
     					 }

     					 else if (pkey == 'F')  //change pump index.
     					 {
     						  // pump_indx++;
     						  //if (pump_indx > 2) pump_indx = 1; //wrap around
     					 }
     					 else if (pkey == 'D')  // Enter key
     					 {
     						copy_stream2[1].pulser_offset = (index_generic);

     						 index_generic = 0;

     						 pulserr = 0;
     						 pulser_flag2 = 0;
     						 fxn = nothing;
     						 clr_screen2();
     					 }

     					 else if (pkey == 'A')  // back key
     					 {
     						 index_generic = 0;

     						 pulserr = 1;
     						 pulser_flag2 = 1;

     						 fxn = nothing;
     						 clr_screen2();
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
     	   else if ( (auth2 == authed) && ( (access == level2)||(access == level3) ) && (fxn == valve_config)  )
     	   {
     		   static float float_generic = 0.00,
     						salesStart_original,
     						salesEnd_original;

     		   static uint8_t _valve = 0;

     		         if(prog_revisit2 == 1)
     		    	 {
     		        	   valve_flag2 = 0;
     		   			   prog_revisit2 = 0;
     		    	 }

     		   	   if (valve_flag2 == 0)
     		   	   {
     				   if (t2 >= 500)
     				   {
     						if (_valve == 0)
     						{
     							send_line12("Ds Start");
     						}
     						else
     							send_line12("Disp. End");

     						send_line3("  B  ");

     					   t2 = 0;
     					}
     		     	 // -------------- test keys....----------------
     		     		 rd19 = readkey192_state();

     		     		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
     		     			 {
     		     				pump_indx++;
     		     				if (pump_indx > 2) pump_indx = 1;  //wrap around
     		     				   key19_sto_ = rd19;
     		     			 }
     		     			   key19_sto_ = rd19;

     		     	     pkey = read_keypad2();

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
     		     				valve_flag2  = 1;   //set flag and goto

     		   					if (_valve == 0)
     		   					{
     		   						_valve = 2;
     		   					}
     		   					else
     		   						_valve = 3;

     		   					salesStart_original = copy_stream2[1].valve_salesStart;
     		   					salesEnd_original = copy_stream2[1].valve_salesEnd;

     		   					return prog_State;
     		     			  }

     		     			 else if (pkey == 'A')  // back key
     		   				 {
     		   					 fxn = nothing;
     		   					 clr_screen2();
     		   				 }
     		     		 }
     		        	 return prog_State;
     		   	}// if selected


     		   	else if(valve_flag2 == 1)
     			{
     				   if (t2 >= 300)
     				   {
     					   if (_valve == 2)
     					   {
     						    if(float_generic > 2.00) float_generic = 0.00;
     						    else if(float_generic < 0.00) float_generic = 2.00;

     							snprintf(st__, sizeof(st__), "  %5.2f L", float_generic);
     							send_line22(st__);
     					   }
     					   else if (_valve == 3)
     					   {
     						   if(float_generic > 10.00) float_generic = 0.30;
     							else if(float_generic < 0.30) float_generic = 10.00;

     							snprintf(st__, sizeof(st__), "  %5.2f L", float_generic);
     							send_line22(st__);
     					   }

     					   send_line3("  B  ");

     					   t2 = 0;
     				}

     				 // -------------- test keys....----------------
     					 rd19 = readkey192_state();

     					   if ( (rd19 == 1)&&( key19_sto_ == 0) )
     						 {
     							pump_indx++;
     							if (pump_indx > 2) pump_indx = 1;  //wrap around
     							   key19_sto_ = rd19;
     						 }
     						   key19_sto_ = rd19;

     					pkey = read_keypad2();

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
     								valve_flag2  = 0;
     								_valve = 0;
     								copy_stream2[1].valve_salesStart = float_generic;

     								fxn = nothing;
									clr_screen2();
     								return prog_State;
     							 }
     							 else if (pkey == 'A')  // back key
     							 {
     								 fxn = nothing;
     								copy_stream2[1].valve_salesStart = salesStart_original;
     								 _valve = 0;
     								 valve_flag2 = 0;
     								 clr_screen2();
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
     								valve_flag2  = 0;
     								_valve = 0;
     								copy_stream2[1].valve_salesEnd = float_generic;
     								clr_screen2();
     								return prog_State;
     							 }
     							 else if (pkey == 'A')  // back key
     							 {
     								 fxn = nothing;
     								copy_stream2[1].valve_salesEnd = salesEnd_original;

     								 _valve = 0;
     								 valve_flag2 = 0;
     								 clr_screen2();
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
     	   else if ( (auth2 == authed) && ( (access == level2)||(access == level3) ) && (fxn == no_calibration)  )
     	   {

     		   if (t2 >= 300)
     		   {
     				snprintf(st__, sizeof(st__), "PC %05d", calib_pulser2);
     				send_line12(st__);

     				send_line32("  B  ");

     				t2 = 0;
     			}
     		 // -------------- test keys....----------------
     			rd19 = readkey192_state();

     			if ( (rd19 == 1)&&( key19_sto_ == 0) )
     				 {
     					pump_indx++;
     					if (pump_indx > 2) pump_indx = 1;  //wrap around
     					   key19_sto_ = rd19;
     				 }
     			 key19_sto_ = rd19;

     			 pkey = read_keypad2();

     			 if( (pkey > 0) )  //key detected
     			 {
     				 if (pkey == 'B')  // up key
     				 {
     					index_generic++;
     				 }

     				 else if (pkey == 'C')  // down key
     				 {
     					index_generic--;
     				 }

     				 else if (pkey == 'F')  //change pump index.
     				 {
     					 ;
     				 }

     				 else if (pkey == 'D')  // Enter key
     				 {
     					copy_stream2[1].non_calibration_seed = (index_generic);

     					 index_generic = 0;
     					 fxn = nothing;
     					 clr_screen2();
     				 }

     				 else if (pkey == 'A')  // back key
     				 {
     					 index_generic = 0;

     					 fxn = nothing;
     					 clr_screen2();
     				 }
     			 }
     		 return prog_State;
     	   }

      //============================== MODE ==================================
       return prog_State;
     } //end of prog_state.





//     		   else if ( (auth2 == authed) && ( (access == level2)||(access == level3) ) && (fxn == nothing)  )
//     		   {
//          	       if(pump_indx == 0) pump_indx = 1;
//
//          	         if( (pkey > 0) )  //key detected
//          	         {
//          	        	 if (pkey == 'B')  // up key
//          					{
////          					 if (index_menu < 13)  index_menu++;
//          					 if (index_menu <= 13)  index_menu++;
//          					 else if(index_menu == 14) index_menu = 0;
//          					}
//
//          	        	 if (pkey == 'C')  // down key
//          					{
////          						if (index_menu > 0) index_menu--;
//          						if (index_menu > 0) index_menu--;
//          						else if(index_menu == 0) index_menu = 14;
//          					}
//
//          	        	 if (pkey == 'A')  // BACK key...
//							   {
//								   auth2 = authed;
//								   access = level1;
//								   fxn = nothing;
//								   clear_buffer2();
//								  // keypad_buf[0] = 0;
//							   }
//
//          	        	 if (pkey == 'D')  // enter key
//          					{
//									if(index_menu == 14) index_menu += 1;
//
//									switch(index_menu)
//          							{
//                                          case setmode:
//												fxn = setmode;
//												pump_indx = 1;
//												return prog_State;
//												clear_buffer2();
//												clr_screen2();
//												break;
//
//                                          case setnozzle:
//                                        	    pump_indx = 1;
//												fxn = setnozzle;
//												clear_buffer2();
//												clr_screen2();
//												break;
//
//                                          case display_orientation:
//                                        	   pump_indx = 1;
//                                          	   fxn = display_orientation;
//                                          	   clear_buffer2();
//                                          	   clr_screen2();
//          									   break;
//
//                                          case setprice:
//												fxn = setprice;
//												pump_indx = 1;
//												//clear the keyboard buffer
//												clear_buffer2();
//												clr_screen2();
//												//index_ = 0;
//												//keyboard_entry2[0] = 0;
//												//keyboard_entry2[1] = 0;
//												//copy[0].price_ = litre_price1;
//												//copy[1].price_ = litre_price2;
//												break;
//
//										  case setaddress:
//											    pump_indx = 1;
//												fxn = setaddress;
//												//clear the keyboard buffer
//												//index_ = 0;
//												clear_buffer2();
//												clr_screen2();
//											  break;
//
//										  case set_no_flow_time:
//												fxn = set_no_flow_time;
//												clear_buffer2();
//												clr_screen2();
//												break;
//
//                                          case setmax_amt:
//                                          	   fxn = setmax_amt;
//                                          	   clear_buffer2();
//                                          	   clr_screen2();
//          									   break;
//
//										  case set_password:
//											    attempts = 0;
//												fxn = set_password;
//												clear_buffer2();
//												clr_screen2();
//												break;
//
//										  case send_config:
//												   fxn = send_config;
//												   clear_buffer2();
//												   clr_screen2();
//												   break;
//                                          case get_config:
//												fxn = get_config;
//												clear_buffer2();
//												clr_screen2();
//												break;
//
//										  case clr_log_:
//											   fxn = clr_log_;
//											   //clr_screen2();
//											   clear_buffer2();
//											   break;
//
//										  case clr_tot_:
//											   fxn = clr_tot_;
//											   //clr_screen2();
//											   clear_buffer2();
//											   break;
//
//										  case calibrate_:
//											   fxn = calibrate_;
//											   clr_screen2();
//											   clear_buffer2();
//											   break;
//										  case display_:
//											   fxn = display_;
//											   clear_buffer2();
//											   clr_screen2();
//											   dp_ = 1;
//											   ln_ = 1;
//											   break;
//
//										  case setclock:
//											   fxn = setclock;
//											   clear_buffer2();
//											   clr_screen2();
//											   break;
//          							}
//          					}
//          	         }
//          	     return prog_State;
//               }
//          //      end of show menu
//
// /*   //check keypad for pressed xter
//  	 if ( keypress__ > 0)  //if any key was pressed.
//  	  {
//  		keypress__ = 0;
//
//  		if (kkey == 'B')  // up key
//  			{
//  			 if (index2 < 3-1)	index2++;
//  			}
//
//  		if (kkey == 'C')  // down key
//  			{
//  				if (index2 > 0)   index2--;
//  			}
//  	   }
//  	 //-----------------------------------------------------------
//  	   if (t >= 500)
//  		 {
//  			send_line22(menu1[index2]);
//  			t = 0;
//  		 }                                     */
//			  //=======================================================
//			  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//			  //=======================================================
//			  //                set mode   ONLINE / OFFLINE.
//			  //=======================================================
//   else if ( (auth2 == authed) && (access == level1)&&(fxn == setmode)  )
//  	               {
//  					   if (t2 >= 300)
//  						 {
//  							//send_line22();
//                            if(copy[pump_indx-1].mode == AUTO)
//								{
//									send_line22(" Auto ");
//								}
//                               else
//								{
//									send_line22("manual ");
//								}
//
//  							 if (pump_indx == 1)
//								 {
//									//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
//									send_line32("  A  ");
//								 }
//  							 if (pump_indx == 2)
//								 {
//									//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
//									send_line32("  B  ");
//								 }
//  							t2 = 0;
//  						 }
//  	           // -------------- test keys....----------------
//  	    		int rd19 = readkey192_state();
//
//				if ( (rd19 == 1)&&( key19_sto_ == 0) )
//					 {
//						pump_indx++;
//						if (pump_indx > 2) pump_indx = 1;  //wrap around
//						   key19_sto_ = rd19;
//					 }
//				    key19_sto_ = rd19;
//
//  	          	     pkey = read_keypad2();
//
//  	          	         if( (pkey > 0) )  //key detected
//  	          	         {
//  	          	        	 if (pkey == 'B')  // up key
//  	          					{
//  	          	        	      if (copy[pump_indx-1].mode == AUTO) //;
//  	          	        	     	copy[pump_indx-1].mode = MANUAL;
//  	          	        	      else
//  	          	        	    	copy[pump_indx-1].mode = AUTO;
//  	          					}
//
//  	          	        	 if (pkey == 'C')  // down key
//  	          					{
//									if (copy[pump_indx-1].mode == AUTO) //;
//										copy[pump_indx-1].mode = MANUAL;
//									  else
//										copy[pump_indx-1].mode = AUTO;
//  	          					}
//
//  	          	             if (pkey == 'F')  //change pump index.
//								{
//								  // pump_indx++;
//									//if (pump_indx > 2) pump_indx = 1; //wrap around
//								}
//
//  	          	        	 if (pkey == 'A')  // back key
//  	          					{
//  	          	        		 	fxn = nothing;
//  	          	        		 	send_line32("      ");
//  	          					}
//  	          	         }
//  	          	     return prog_State;
//  	               }
//      //========================= MODE ==========================
//      //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//	  //==========================================================================
//	  //                set mode   OVERIDE.
//	  //==========================================================================
//   else if ( (auth2 == authed) && (access == level1)&&(fxn == setnozzle)  )
//		   {
//			   if (t2 >= 300)
//				 {
//					//send_line22();
//				  if(copy[pump_indx-1].noz == overide)
//				  {
//					send_line22("Active");
//				  }
//				  else
//				  {
//					send_line22("Inactive");
//				  }
//
//				 if (pump_indx == 1)
//				 {
//					//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
//					send_line32("  A  ");
//				 }
//				 if (pump_indx == 2)
//				 {
//					//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
//					send_line32("  B  ");
//				 }
//				t2 = 0;
//		 }
//			 // -------------- test keys....----------------
//				int rd19 = readkey192_state();
//
//				if ( (rd19 == 1)&&( key19_sto_ == 0) )
//					 {
//						pump_indx++;
//						if (pump_indx > 2) pump_indx = 1;  //wrap around
//						   key19_sto_ = rd19;
//					 }
//				 key19_sto_ = rd19;
//
//			 pkey = read_keypad2();
//
//				 if( (pkey > 0) )  //key detected
//				 {
//					 if (pkey == 'B')  // up key
//						{
//						 if (copy[pump_indx-1].noz == overide)
//							 copy[pump_indx-1].noz = nooveride;
//						 else
//							 copy[pump_indx-1].noz = overide;
//						}
//
//					 if (pkey == 'C')  // down key
//						{
//						 if (copy[pump_indx-1].noz == overide)
//							 copy[pump_indx-1].noz = nooveride;
//						 else
//							 copy[pump_indx-1].noz = overide;
//						}
//
//					 if (pkey == 'F')  //change pump index.
//					{
//					  // pump_indx++;
//					  // if (pump_indx > 2) pump_indx = 1; //wrap around
//					}
//
//					 if (pkey == 'A')  // back key
//						{
//							 fxn = nothing;
//							 clr_screen2();
//						}
//				 }
//			 return prog_State;
//		   }
//	      //========================= MODE =======================================
//		  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//		  //======================================================================
//		  //                    default sell/prog type .
//		  //======================================================================
//   else if ( (auth2 == authed) && (access == level1)&&(fxn == display_orientation)  )
//				   {
//					   if (t2 >= 300)
//						 {
//							//send_line22();
//							  if(copy[pump_indx-1].def_t ==  P)
//							  {
//								send_line22("price   ");
//							  }
//							  else
//							  {
//								send_line22("litre   ");
//							  }
//
//							 if (pump_indx == 1)
//							 {
//								//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
//								send_line32("  A  ");
//							 }
//							 if (pump_indx == 2)
//							 {
//								//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
//								send_line32("  B  ");
//							 }
//							t2 = 0;
//			        	 }
//					 // -------------- test keys....----------------
//						int rd19 = readkey192_state();
//
//						if ( (rd19 == 1)&&( key19_sto_ == 0) )
//							 {
//								pump_indx++;
//								if (pump_indx > 2) pump_indx = 1;  //wrap around
//								   key19_sto_ = rd19;
//							 }
//						 key19_sto_ = rd19;
//
//				    	 pkey = read_keypad2();
//
//						 if( (pkey > 0) )  //key detected
//						 {
//							 if (pkey == 'B')  // up key
//								{
//								 if(copy[pump_indx-1].def_t == P)
//								   copy[pump_indx-1].def_t = L;
//								 else
//								   copy[pump_indx-1].def_t = P;
//								}
//
//							 if (pkey == 'C')  // down key
//								{
//								 if(copy[pump_indx-1].def_t == P)
//								   copy[pump_indx-1].def_t = L;
//								 else
//								   copy[pump_indx-1].def_t = P;
//								}
//
//							 if (pkey == 'F')  //change pump index.
//								{
//								  // pump_indx++;
//								  //if (pump_indx > 2) pump_indx = 1; //wrap around
//								}
//
//							 if (pkey == 'A')  // back key
//								{
//									 fxn = nothing;
//									 clr_screen2();
//								}
//						 }
//					 return prog_State;
//				   }
//
//		  //=========================== MODE ====================================
//		  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//		  //=====================================================================
//		  //               set the  litre price
//		  //=====================================================================
//   else if ( (auth2 == authed) && (access == level1)&&(fxn == setprice) )
//			   {
//				 if (t2 >= 300)
//				  {
//					 //copy[pump_indx-1].price_ =  atoi(keyboard_entry2);
//					 printDisp_f2(copy[pump_indx-1].price_, 1, 0, 6, RT, CLEAR);
//                     float pr = atoff(keyboard_entry2);
//					 printDisp_f2(pr,2,0,7,RT,CLEAR);
//
//					 if (pump_indx == 1)
//					 {
//					   send_line32("  A  ");
//					 }
//
//					 if (pump_indx == 2)
//					 {
//						 send_line32("  B  ");
//					 }
//					t2 = 0;
//			 }
//				 // -------------- test keys....----------------
//				 int rd19 = readkey192_state();
//
//				   if ( (rd19 == 1)&&( key19_sto_ == 0) )
//					 {
//					   keyboard_entry2[0] = 0;   //clear the previous entry...
//					   index_2 = 0;
//					   pump_indx++;
//						if (pump_indx > 2) pump_indx = 1;  //wrap around
//						   key19_sto_ = rd19;
//					 }
//					   key19_sto_ = rd19;
//
//
//			         pkey = read_keypad2();
//
//					 if( (pkey > 0) )  //key detected
//					 {
//						 if (pkey == 'B')  // up key
//							{
//
//							}
//
//						 if (pkey == 'C')  // down key
//							{
//
//							}
//
//						 if (pkey == 'D')  //store entered value.
//							{
//							 if (pump_indx == 1)   // if side A
//								{
//								   copy[0].price_ =  atof(keyboard_entry2);
//								}
//							 if (pump_indx == 2)   // if side A
//								{
//								   copy[1].price_ =  atof(keyboard_entry2);
//								}
//							}
//
//						 if (pkey == 'A')  // back key
//							{
//								 fxn = nothing;
//								 clr_screen2();
//							}
//					 }
//				 return prog_State;
//			   }
//		         //=========================== MODE =====================================
//			     //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//				  //=====================================================================
//				  //                          ID SETTINGS.
//				  //=====================================================================
//   else if ( (auth2 == authed) && (access == level1)&&(fxn == setaddress)  )
//					   {
//						   if (t2 >= 500)
//							 {
//								printDisp_i2(copy[pump_indx-1].id_, 1, 0, 4, RT, CLEAR);
//								send_line22(keyboard_entry2);
//
//								if (pump_indx == 1)
//								 {
//									send_line32("  A  ");
//								 }
//
//								if (pump_indx == 2)
//								 {
//									send_line32("  b  ");
//								 }
//								t2 = 0;
//					        }
//						 // -------------- test keys....----------------
//							 int rd19 = readkey192_state();
//
//							   if ( (rd19 == 1)&&( key19_sto_ == 0) )
//								 {
//								   keyboard_entry2[0] = 0;   //clear the previous entry...
//								    index_2 = 0;
//									pump_indx++;
//									if (pump_indx > 2) pump_indx = 1;  //wrap around
//									   key19_sto_ = rd19;
//								 }
//								   key19_sto_ = rd19;
//
//				    	        pkey = read_keypad2();
//
//							 if( (pkey > 0) )  //key detected
//							 {
//								 if (pkey == 'B')  // up key
//									{
//
//									}
//
//								 if (pkey == 'C')  // down key
//									{
//
//									}
//
//								 if (pkey == 'D')  //enter key
//									{
//										 if (pump_indx == 1)   // if side A
//											{
//											   copy[0].id_ =  atoi(keyboard_entry2);
//											}
//										 if (pump_indx == 2)   // if side b
//											{
//											   copy[1].id_ =  atoi(keyboard_entry2);
//											}
//									}
//
//								 if (pkey == 'A')  // back key
//									{
//										 fxn = nothing;
//										 clr_screen2();
//									}
//							 }
//						 return prog_State;
//					   }
//	  //=============================== MODE =====================================
//	  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//	  //==========================================================================
//	  //                         SET NO FLOW TIME.
//	  //==========================================================================
//   else if ( (auth2 == authed) && (access == level1)&&(fxn == set_no_flow_time)  )
//		   {
//			   if (t2 >= 500)
//				 {
//				   printDisp_i2(copy[pump_indx-1].noFlow_timeOut, 1, 0, 4, RT, CLEAR);
//				   send_line22(keyboard_entry2);
//
//				 if (pump_indx == 1)
//				 {
//					//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
//					send_line32("  A  ");
//				 }
//
//				 if (pump_indx == 2)
//				 {
//					//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
//					send_line32("  b  ");
//				 }
//				t2 = 0;
//		 }
//			 // -------------- test keys....----------------
//				 int rd19 = readkey192_state();
//
//				   if ( (rd19 == 1)&&( key19_sto_ == 0) )
//					 {
//					   keyboard_entry2[0] = 0;   //clear the previous entry...
//					 	   index_2 = 0;
//						pump_indx++;
//						if (pump_indx > 2) pump_indx = 1;  //wrap around
//						   key19_sto_ = rd19;
//					 }
//					   key19_sto_ = rd19;
//
//			 pkey = read_keypad2();
//
//				 if( (pkey > 0) )  //key detected
//				 {
//					 if (pkey == 'B')  // up key
//						{
//
//						}
//
//					 if (pkey == 'C')  // down key
//						{
//
//						}
//
//					 if (pkey == 'D')  //enter key
//						{
//							 if (pump_indx == 1)   // if side A
//								{
//								   copy[0].noFlow_timeOut =  atoi(keyboard_entry2);
//								}
//							 if (pump_indx == 2)   // if side b
//								{
//								   copy[1].noFlow_timeOut =  atoi(keyboard_entry2);
//								}
//						}
//
//					 if (pkey == 'A')  // back key
//						{
//							 fxn = nothing;
//							 clr_screen2();
//						}
//				 }
//			 return prog_State;
//		   }
//  //============================ MODE ===================================
//  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//  //=====================================================================
//  //                         SET MAX AMOUNT.
//  //=====================================================================
//   else if ( (auth2 == authed) && (access == level1)&&(fxn == setmax_amt)  )
//	   {
//		   if (t2 >= 500)
//			 {
//			   printDisp_i2(copy[pump_indx-1].max_amt_, 1, 0, 4, RT, CLEAR);
//			   send_line22(keyboard_entry2);
//
//			 if (pump_indx == 1)
//			 {
//				send_line32("  A  ");
//			 }
//
//			 if (pump_indx == 2)
//			 {
//				send_line32("  b  ");
//			 }
//			t2 = 0;
//	 }
//		 // -------------- test keys....----------------
//			 int rd19 = readkey192_state();
//
//			   if ( (rd19 == 1)&&( key19_sto_ == 0) )
//				 {
//				   keyboard_entry2[0] = 0;   //clear the previous entry...
//				   					   index_2 = 0;
//					pump_indx++;
//					if (pump_indx > 2) pump_indx = 1;  //wrap around
//					   key19_sto_ = rd19;
//				 }
//				   key19_sto_ = rd19;
//
//		   pkey = read_keypad2();
//
//			 if( (pkey > 0) )  //key detected
//			 {
//				 if (pkey == 'B')  // up key
//					{
//
//					}
//
//				 if (pkey == 'C')  // down key
//					{
//
//					}
//
//				 if (pkey == 'D')  //enter key
//					{
//						 if (pump_indx == 1)   // if side A
//							{
//							   copy[0].max_amt_ =  atoi(keyboard_entry2);
//							}
//						 if (pump_indx == 2)   // if side b
//							{
//							   copy[1].max_amt_ =  atoi(keyboard_entry2);
//							}
//					}
//
//				 if (pkey == 'A')  // back key
//					{
//						 fxn = nothing;
//						 clr_screen2();
//					}
//			 }
//		 return prog_State;
//	   }
//
////============================== MODE ==================================
// //=====================================================================
// //                         CLEAR LOG.
// //=====================================================================
//   else if ( (auth2 == authed) && (access == level1)&&(fxn == clr_log_)  )
//   {
//	   if (t2 >= 500)
//		 {
//		   //printDisp_i2(copy[pump_indx-1].max_amt_, 1, 0, 4, RT, CLEAR);
////		   send_line22(keyboard_entry2);
////
////		 if (pump_indx == 1)
////		 {
////			send_line32("  a  ");
////		 }
////
////		 if (pump_indx == 2)
////		 {
//			send_line32("  b  ");
////		 }
//		t2 = 0;
// }
//	 // -------------- test keys....----------------
////		 int rd19 = readkey192_state();
////
////		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
////			 {
////			   keyboard_entry2[0] = 0;   //clear the previous entry...
////				   index_2 = 0;
////				pump_indx++;
////				if (pump_indx > 2) pump_indx = 1;  //wrap around
////				   key19_sto_ = rd19;
////			 }
////			   key19_sto_ = rd19;
//
//	 pkey = read_keypad2();
//
//		 if( (pkey > 0) )  //key detected
//		 {
////			 if (pkey == 'C')  // up key
////				{
////
////				}
////
////			 if (pkey == 'B')  // down key
////				{
////
////				}
//
////			 if (pkey == 'D')  //enter key
////				{
////					 if (pump_indx == 1)   // if side A
////						{
////						  //clear side A log
////						  clear_logA();
////						  send_line12("        ");
////						  send_line22("cleared ");
////						  HAL_Delay(2000);
////						  clr_screen2(); //send_line22("        ");
////						  fxn = nothing;
////						}
////
////					 if (pump_indx == 2)   // if side b
////						{
////						 //clear side B log
////						 clear_logB();
////						 send_line12("        ");
////						 send_line22("cleared ");
////						 HAL_Delay(2000);
////						 clr_screen2(); //send_line22("        ");
////						 fxn = nothing;
////						}
////				}
//
//			 if (pkey == 'A')  // back key
//				{
//					 keyboard_entry2[0] = 0;   //clear the previous entry...
//					 index_2 = 0;
//					 fxn = nothing;
//					 clr_screen2();
//				}
//		 }
//	 return prog_State;
//   }
//
// //=====================================================================
//  //                         CLEAR totaliser.
//  //=====================================================================
//   else if ( (auth2 == authed) && (access == level1)&&(fxn == clr_tot_)  )
//    {
// 	   if (t2 >= 500)
// 		 {
// 		   //printDisp_i2(copy[pump_indx-1].max_amt_, 1, 0, 4, RT, CLEAR);
//// 		   send_line22(keyboard_entry2);
//
//// 		 if (pump_indx == 1)
//// 		 {
//// 			send_line32("  a  ");
//// 		 }
//
//// 		 if (pump_indx == 2)
//// 		 {
// 		 send_line32("  b  ");
//// 		 }
// 		 t2 = 0;
//  }
// 	 // -------------- test keys....----------------
//// 		 int rd19 = readkey192_state();
////
//// 		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
//// 			 {
//// 			   keyboard_entry2[0] = 0;   //clear the previous entry...
//// 				   index_2 = 0;
//// 				pump_indx++;
//// 				if (pump_indx > 2) pump_indx = 1;  //wrap around
//// 				   key19_sto_ = rd19;
//// 			 }
//// 			   key19_sto_ = rd19;
//
// 		 pkey = read_keypad2();
//
// 		 if( (pkey > 0) )  //key detected
// 		 {
//// 			 if (pkey == 'C')  // up key
//// 				{
////
//// 				}
////
//// 			 if (pkey == 'B')  // down key
//// 				{
////
//// 				}
////
//// 			 if (pkey == 'D')  //enter key
//// 				{
//// 					 if (pump_indx == 1)   // if side A
//// 						{
//// 						  //clear side A log
//// 						  clear_totA();
//// 						  send_line12("        ");
//// 						  send_line22("cleared ");
//// 						 HAL_Delay(2000);
//// 						  fxn = nothing;
//// 						 clr_screen2();
//// 						}
////
//// 					 if (pump_indx == 2)   // if side b
//// 						{
//// 						 //clear side B log
//// 						 clear_totB();
//// 						 send_line12("        ");
//// 						 send_line22("cleared ");
//// 						 HAL_Delay(2000);
//// 						 fxn = nothing;
//// 						 clr_screen2();
//// 						}
//// 				}
//
// 			if (pkey == 'A')  // back key
//			{
//				 keyboard_entry2[0] = 0;   //clear the previous entry...
//				 index_2 = 0;
//
//				 fxn = nothing;
//				 clr_screen2();
//			}
// 		 }
// 	 return prog_State;
//    }
// //============================ MODE ===================================
// //=====================================================================
// //                         SEND CONFIG.
// //=====================================================================
//   else if ( (auth2 == authed) && (access == level1)&&(fxn == send_config)  )
//   {
//	   if (t2 >= 500)
//		 {
//		   //printDisp_i2(copy[pump_indx-1].max_amt_, 1, 0, 4, RT, CLEAR);
//		  // send_line22(keyboard_entry2);
///*
//		 if (pump_indx == 1)
//		 {
//			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
//			send_line32("  a  ");
//		 }
//
//		 if (pump_indx == 2)
//		 {
//			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
//			send_line32("  b  ");
//		 }  */
//
//		t2 = 0;
// }
//	 // -------------- test keys....----------------
//		 int rd19 = readkey192_state();
//
//		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
//			 {
//				pump_indx++;
//				if (pump_indx > 2) pump_indx = 1;  //wrap around
//				   key19_sto_ = rd19;
//			 }
//			   key19_sto_ = rd19;
//
//	 pkey = read_keypad2();
//
//		 if( (pkey > 0) )  //key detected
//		 {
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
//					    for (int i = 0 ; i < 14;i++)
//						  {
//							//   send the bits to the external system
//
//						  }
//
//						  for (int i = 0 ; i < 14;i++)
//						  {
//							  //   send the bits to the external system
//						  }
//
//					  send_line22("sent    ");
//					  fxn = nothing;
//				}
//
//			 if (pkey == 'A')  // back key
//				{
//					 fxn = nothing;
//					 clr_screen2();
//				}
//		 }
//	 return prog_State;
//   }
//
//
//  //============================ MODE ===================================
//  //=====================================================================
//  //                         get config
//  //=====================================================================
//   else if ( (auth2 == authed) && (access == level1)&&(fxn == get_config)  )
//    {
// 	   if (t2 >= 3000)
// 		 {
//
// 		  send_line32("      ");
// 		  send_line22(" done ");
// 		  fxn = nothing;
// 		t2 = 0;
//    }
//
//  //-------------------------------------------------
//  // Receive the bytes sent from the external system
//  //       and store in the settings array.
//
//
//
//
//
//  //-------------------------------------------------
//  // -------------- test keys....--------------------
// 		 int rd19 = readkey192_state();
//
// 		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
// 			 {
// 				pump_indx++;
// 				if (pump_indx > 2) pump_indx = 1;  //wrap around
// 				   key19_sto_ = rd19;
// 			 }
// 			   key19_sto_ = rd19;
//
// 	          pkey = read_keypad2();
//
//			   if( (pkey > 0) )  //key detected
//				 {
//					 if (pkey == 'A')  // back key
//						{
//							 fxn = nothing;
//							 clr_screen2();
//						}
//				 }
// 	 return prog_State;
//    }
//  //============================ MODE ==================================
//  //====================================================================
//  //                         SET PASSWORD
//  //====================================================================
//   else if ( (auth2 == authed) && (access == level1)&&(fxn == set_password)  )
//    {
//	   static uint8_t chg_pw = 0;
////	   static int pw1 = 0;
////	   static int pw2 = 0;
//	   static char pw1[9] = { 0 };
//
//	   if(prog_revisit2 == 1)
//	   {
//		   chg_pw = 0;
////		   pw1 = 0;
////		   pw2 = 0;
//
//		   memset(pw1, '\0', sizeof(pw1));
//
//		   prog_revisit2 = 0;
//	   }
//
// 	   if (t2 >= 500)
// 		 {
// 		    send_line12(keypad_pw_xter2); //keyboard_entry2);
// 		    send_line22("  Enter ");
//		    send_line32("Pass. ");
// 		   if (pump_indx == 1)
// 		  		 {
// 		  	//		send_line32("  a  ");
// 		  		 }
//
// 		  		 if (pump_indx == 2)
// 		  		 {
// 		  	//		send_line32("  b  ");
// 		  		 }
// 	    	t2 = 0;
//         }
// 	 // -------------- test keys....----------------
// 		 int rd19 = readkey192_state();
//
// 		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
// 			 {
// 				pump_indx++;
// 				if (pump_indx > 2) pump_indx = 1;  //wrap around
// 				   key19_sto_ = rd19;
// 			 }
// 			   key19_sto_ = rd19;
//
// 	    pkey = read_keypad2();
//
// 		 if( (pkey > 0) )  //key detected
// 		 {
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
// 				 if (chg_pw == 0)
//				  {
////					     pw1 = atoi(keyboard_entry2);
//					 strncpy(pw1, keyboard_entry2, sizeof(pw1));
//					 chg_pw = 1;
//					 send_line22("re-enter");
//					 HAL_Delay(2500);
//					 send_line12("        ");
//					 clear_buffer2();
//					 //index_ = 0;
//					 //keyboard_entry2[0] = 0;
//					 //keyboard_entry2[1] = 0;
//					 //keypad_pw_xter1[0] = 0;
//					 //_index = 0;
//					 return prog_State;
//				  }
// 				 else if (chg_pw == 1)
//				 {
//						 if (strcmp(pw1, keyboard_entry2) == 0)  //if strings are same
//						 {
//							 send_line12("correct ");
//							 send_line22("        ");
//							 HAL_Delay(2500);
//							 send_line12("        ");
//
////							 copy[0].passwd1 = pw1;
////							 copy[1].passwd2 = pw1;
//							 strncpy(copy[0].passwd1, pw1, sizeof(pw1));
//							 strncpy(copy[1].passwd1, pw1, sizeof(pw1));
//
//							 HAL_Delay(2500);
//							 send_line12("pass    ");
//							 send_line22("changed ");
//							 HAL_Delay(2500);
//							 fxn = nothing;
//							 chg_pw = 1;
//							 clear_buffer2();
//							// index_ = 0;
//							//keyboard_entry2[0] = 0;
//							//keypad_pw_xter1[0] = 0;
//							//_index = 0;
//							return prog_State;
//
//						 }
//					 else
//					 {
//
//							send_line22("---no---");
//							HAL_Delay(1500);
//							send_line22("        ");
//							attempts++;
//							if (attempts >= 3)
//							{
//								send_line22("        ");
//								send_line22(" denied ");
//								HAL_Delay(1500);
//								send_line22("        ");
//								 fxn = nothing;
//								 chg_pw = 1;
//								clear_buffer2();
//								// index_ = 0;
//								//keyboard_entry2[0] = 0;
//								//keypad_pw_xter1[0] = 0;
//								//_index = 0;
//								 return prog_State;
//							}
//							else
//							{
//								send_line22("        ");
//								send_line22(" retry  ");
//								HAL_Delay(1500);
//								send_line22("        ");
//								clear_buffer2();
//								// index_ = 0;
//								//keyboard_entry2[0] = 0;
//								//keypad_pw_xter1[0] = 0;
//								//_index = 0;
//								return prog_State;
//							}
//					 	 }
//						 chg_pw = 1;
//					  }
// 				}
//
// 			 if (pkey == 'A')  // back key
// 				{
// 					 fxn = nothing;
// 				   	clr_screen2();
// 				}
// 		 }
// 	 return prog_State;
//    }
//
//
//    //============================ MODE ==================================
//    //====================================================================
//    //                        CALIBRATE..
//    //====================================================================
//   else if ( (auth2 == authed) && (access == level1)&&(fxn == calibrate_)  )
//      {
//  	   static float calib_price = 0.0;
//  	   static float inv_price   =  0.0;
//  	   static int vol_sold = 1;
//
//  	 if(prog_revisit2 == 1)
//  	 {
//  		 calib_price = 0.0;
//	     inv_price   =  0.0;
//	     vol_sold = 1;
//	     volume_flag2 = 0;
//	     calibr2 = 0;        /// notify the interrupt routine to off the pump
//	     prog_revisit2 = 0;
//  	 }
//
//	   if (volume_flag2 == 0)
//	    {
//		   //select sales volume first...
//
//			   if (t2 >= 500)
//				 {
//					if (vol_sold == l1)
//					{
//						send_line12("10 litre");
//					}
//					else
//						send_line12("20 litre");
//
//				   if (pump_indx == 1)
//						 {
//							send_line32("  A  ");
//						 }
//
//				   if (pump_indx == 2)
//						 {
//							send_line32("  b  ");
//						 }
//					   t2 = 0;
//				   }
//   	 // -------------- test keys....----------------
//   		 int rd19 = readkey192_state();
//
//   		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
//   			 {
//   				pump_indx++;
//   				if (pump_indx > 2) pump_indx = 1;  //wrap around
//   				   key19_sto_ = rd19;
//   			 }
//   			   key19_sto_ = rd19;
//
//   	    pkey = read_keypad2();
//
//   		 if( (pkey > 0) )  //key detected
//   		 {
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
//
//   			 if (pkey == 'D')  //enter key
//   				{
//   				 	 //enter pressed user has made a choice.
//   				     // then dispense the selcted volume
//					volume_flag2  = 1;   //set flag and goto dispense
//					calib_pulser2 = 0;  //clear pulser
//		           __HAL_TIM_SET_COUNTER(&htim2,0);   //clear harware pulser here...
//					return prog_State;
//   				}
//
//   			 if (pkey == 'A')  // back key
//   				{
//   					 fxn = nothing;
//   				     calibr2 = 0;
//   				     clr_screen2();
//   				}
//   		 }
//      	 return prog_State;
//	   }// if selected volume = 0.....
//	//========= volume has been selected =============
//    // dispense
//      if(volume_flag2 == 1)
//      {
//         send_line12("dispense");
//         HAL_Delay(2000);
//         volume_flag2 = 2;
//         calibr2 = 1;        /// notify the interrupt routine to on the pump
//         return prog_State;
//      }
//    //=============================
//	  if(volume_flag2 == 2)  //dispense.
//	   {
//		 if(t2 > 60)
//		  {
//
//			#if _USE_SOFT_PULSER == 0
//			  calib_pulser2 = __HAL_TIM_GET_COUNTER(&htim2);  //use hardware counter
//			#else
//			  calib_pulser2++; 							// use software counter.
//			#endif
//
//			  send_line12(" Pulser ");
//			  printDisp_i2(calib_pulser2, 2, 0, 7, RT, CLEAR); //    power outage during filling1  end transaction...
//
//			#if sense_power == 1
//			  if( (readpwr() == 0)||(read_p_pwr() == 0) )
//			  {
//				  HAL_GPIO_WritePin(buzzer_GPIO_Port, GPIO_PIN_12, GPIO_PIN_SET);
//				  HAL_Delay(200);
//				  HAL_GPIO_WritePin(buzzer_GPIO_Port, GPIO_PIN_12, GPIO_PIN_RESET);
//				  stop_flow1();
//			//				  get_time();
//			//				  do_calcs();
//			//				  update_info();
//
//				  pulser_totalizer2 = ( (calib_pulser2 / (float) (pulser_benchMark2)) * calibrationCan_measure2 );
//
//				  totaliser_vol2c += pulser_totalizer2;
//				  totaliser_vol2 += pulser_totalizer2;
//				  totaliser_amt2c += (pulser_totalizer2 * litre_price2);
//				  totaliser_amt2 += (pulser_totalizer2 * litre_price2);
//				  amt_real2 = pulser_totalizer2;
//				  amt_middle2 = pulser_totalizer2;
//				  price_real2 = (pulser_totalizer2 * litre_price2);
//				  price_upper2 = (pulser_totalizer2 * litre_price2);
//				  save_volumeTotaliser(operating_side);
//				  save_amountTotaliser(operating_side);
//				  save_lastSale(operating_side);
//
//				  return write_flash_state;
//			  }
//			#endif
//
//			t2 = 0;
//		  }
//
//			  pkey = read_keypad2();
////			  if (pkey == 'A')  //off key to stop fueling...
//			  if (pkey == 'D')  //Enter key to transit to the next page
//				{
//                    //capture the pulser.
//				  calibr2 = 0;  /// notify the interrupt routine to on the pump
//				  #if _USE_SOFT_PULSER == 0
//				  	  calib_pulser2 = __HAL_TIM_GET_COUNTER(&htim2);  //use hardware counter
//				  #else
//				  	  calib_pulser2++; 							// use software counter.
//				  #endif
//
//				  pulser_totalizer2 = ( (calib_pulser2 / (float) (pulser_benchMark2)) * calibrationCan_measure2 );
//
//				  totaliser_vol2c += pulser_totalizer2;
//				  totaliser_vol2 += pulser_totalizer2;
//				  totaliser_amt2c += (pulser_totalizer2 * litre_price2);
//				  totaliser_amt2 += (pulser_totalizer2 * litre_price2);
//				  amt_real2 = pulser_totalizer2;
//				  amt_middle2 = pulser_totalizer2;
//				  price_real2 = (pulser_totalizer2 * litre_price2);
//				  price_upper2 = (pulser_totalizer2 * litre_price2);
//				  save_volumeTotaliser(operating_side);
//				  save_amountTotaliser(operating_side);
//				  save_lastSale(operating_side);
//
////				  calib_pulser2 = __HAL_TIM_GET_COUNTER(&htim2);  //use hardware counter
//				  send_line12("Set your");
//				  send_line22("  Annt  ");
//				  HAL_Delay(2000);
//				  volume_flag2 = 3;   //enter the price.
//				  return prog_State;
//				}
//
////			  if (pkey == 'A')  // back key
////				{
////                 	volume_flag2 = 0;
////                    calibr2 = 0;
////                   // clr_screen2();
////				}
//
//		     return prog_State;
//	   }
//  //==========================================================
//
//  //==========================================================
//    if(volume_flag2 == 3)  //Enter the price.
//	   {
//			  if(t2 > 400)
//				  {
//					send_line12("  Annt  ");
//					printDisp_f2(atoff(keyboard_entry2), 2, 0, 7, RT, CLEAR);
//					t2 = 0;
//				  }
//
//			  pkey = read_keypad2();
//			  if (pkey == 'D')  //ENTER key to adjusted volume.
//				{
//				  float cal_vol = atoff(keyboard_entry2);
//				  send_line12("        ");   //clear screen.
//				  send_line22("        ");
//				  clear_buffer2();
//					//index_ = 0;
//					//keyboard_entry2[0] = 0;  //clear buffer...
//					//keypad_pw_xter1[0] = 0;
//					//_index = 0;
//					//==================================
//					// calculate the price of the pulses
//					// float pop = calib_pulser1  / cal_vol;
//					int sold_v = 0;
//					float pi_c;
//					float pi;
//
//					if (vol_sold == l1)
//					{
//						sold_v = 10;
//						pi_c = calib_pulser2 / cal_vol; //calibrated pi.
//						pi   = calib_pulser2 / sold_v;
//					}
//					else
//					{
//						sold_v = 20;
//						pi_c = calib_pulser2 / cal_vol; //calibrated pi.
//						pi   = calib_pulser2 / sold_v;
//					}
//
//					vol_real2 = sold_v;
//					vol_calibrated2 = cal_vol;
//					save_ctSettings(side_b);
//					save_calibrationPulser(side_b);
//
//                    HAL_Delay(1700);
//
//                    send_line12("  done  ");
//                    printDisp_f2(pi_c, 2,0, 8, RT, CLEAR);
//                    HAL_Delay(2500);
//                    //store in the settings structure.
//                    if(pump_indx == 1)
//                    {
//                    	// data for side a.
//                    	copy[0].pi_c = pi_c;
//                    	copy[0].pi_  = pi;
//                    }
//
//                    if(pump_indx == 2)
//					{
//						// data for side b.
//						copy[1].pi_c = pi_c;
//						copy[1].pi_	 = pi;
//					}
//					 fxn = nothing;
//					 calibr2 = 0;
//					 clr_screen2();
//				}
//
//			  if (pkey == 'A')  // back key
//				{
//					volume_flag2 = 0;
//					clr_screen2();
//				}
//			 return prog_State;
//	   }
//    return prog_State;
//     }
//
////============================ MODE ===================================
////xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
////=====================================================================
////                      display precision
////=====================================================================
//   else if ( (auth2 == authed) && (access == level1)&&(fxn == display_)  )
//   {
//	 if(enter_key == 1)
//	 {
//		 if(timer_config2 < 1000)
//		 {
//			 send_line22(" Entered");
//		 }
//		 else
//		 {
//			 enter_key = 0;
//			 fxn = nothing;
//			 clr_screen2();
//		 }
//
//		 return prog_State;
//	 }
//
//	 if (ln_ == 0) ln_ = 1;
//	 if (dp_ == 0) dp_ = 1;
//
//	   if (t2 >= 500)
//		 {
//		    if (ln_ == 1)
//			 {
//				send_line12("   p    ");
//			 }
//
//			if (ln_ == 2)
//			 {
//				send_line12("   l    ");
//			 }
//
//			if (ln_ == 3)
//			 {
//				send_line12(" unit p ");
//			 }
//       //---------------------------------
//		    if (dp_ == 1)
//			 {
//				send_line22(" 999999.9");
//			 }
//
//		    if (dp_ == 2)
//			 {
//				send_line22(" 99999.99");
//			 }
//
//		    if (dp_ == 3)
//			 {
//		    	send_line22(" 9999.999");
//			 }
//		    if (dp_ == 4)
//			 {
//				send_line22(" 999.9999");
//			 }
//        //--------------------------------
//			 if (pump_indx == 1)
//			 {
//				send_line32("  A  ");
//			 }
//
//			 if (pump_indx == 2)
//			 {
//				send_line32("  b  ");
//			 }
//			t2 = 0;
//        }
//	 // -------------- test keys....----------------
//		 int rd19 = readkey192_state();
//
//		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
//			 {
//			   keyboard_entry2[0] = 0;   //clear the previous entry...
//								   index_2 = 0;
//				pump_indx++;
//				if (pump_indx > 2) pump_indx = 1;  //wrap around
//				   key19_sto_ = rd19;
//			 }
//			   key19_sto_ = rd19;
//
//	   pkey = read_keypad2();
//
//
//		 if( (pkey > 0) )  //key detected
//		 {
//			 if (pkey == 'C')  // up key
//				{
//				 	 dp_++;
//				 	 if(dp_ > max_dp) dp_ = 1;
//				}
//
//			 if (pkey == 'B')  // down key
//				{
//				 	 ln_++;
//				 	 if(ln_ > 3)ln_ = 1;
//				}
//
//			 if (pkey == 'D')  //enter key
//				{
//					 if (pump_indx == 1)   // if side A
//					 {
//					   if(ln_ == 1) copy[0].dp_price  = dp_;
//					   if(ln_ == 2) copy[0].dp_amount = dp_;
//					   if(ln_ == 3) copy[0].dp_unitprice = dp_;
//					 }
//					 if (pump_indx == 2)   // if side b
//					 {
//						   if(ln_ == 1) copy[1].dp_price  = dp_;
//						   if(ln_ == 2) copy[1].dp_amount = dp_;
//						   if(ln_ == 3) copy[1].dp_unitprice = dp_;
//					 }
//
//					 clr_screen2();
//					 timer_config2 = 0;
//					 send_line22("Entered ");
//
//					 enter_key = 1;
//					 return prog_State;
//				}
//
//			 if (pkey == 'A')  // back key
//				{
//					 fxn = nothing;
//					 clr_screen2();
//					 return prog_State;
//				}
//		 }
//	 return prog_State;
//   }
//
//
//
// //=========================== MODE ====================================
//  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//  //=====================================================================
//  //               set the  CLOCK
//  //=====================================================================
//   else if ( (auth2 == authed) && (access == level1) && (fxn == setclock) )
// 	 {
// 		    static int8_t respons_dir = 0,
// 				   entry_clear = 1,
// 				   montth = 0,
// 				   failFlag_tm = 0,
// 				   failFlag_dt = 0;
//
// 		    int8_t respons = 0,
// 		    	   tm = 0,
// 		    	   dte = 0;
//
// 		    static bool hh_success = false,
// 		    			yy_success = false;
//
// 		    char st__[10] = {0};
//
// 		   if(prog_revisit2 == 1)
//		   {
// 			   respons_dir = 0;
//			   entry_clear = 1;
//			   montth = 0;
//			   failFlag_tm = 0;
//			   failFlag_dt = 0;
// 			   hh_success = false;
// 			   yy_success = false;
// 			   prog_revisit2 = 0;
//		   }
//
// 		  //====================================================
// 		  if( (t2>200) && (respons_dir == 0) )
// 		  {
// 			  send_line12("1- tinne");
// 			  send_line22("2-  date");
// 			  t2 = 0;
//
// 		  }
// 		  if(entry_clear)
// 			  respons = atoi(keyboard_entry2);
//
// 		 //------------------------------------------------
// 		  if( ((respons == 1) || (respons_dir == 1)) && (!failFlag_tm) )
// 		  {
// 			  if(entry_clear == 1)
// 			  {
// 				  respons = 0;
// 				  respons_dir = 1;
// 				  clear_buffer2();
// 				  entry_clear = 0;
// 			  }
//
// 			  if (!hh_success)
// 			  {
// 				  snprintf(st__, sizeof(st__), "  %02d", hour);
// 				  printDisp_c2("hr",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
// 			  }
// 			  else if(hh_success)
// 			  {
// 				  snprintf(st__, sizeof(st__), "  %02d", minute);
// 				  printDisp_c2("nnI",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
// 			  }
//
// 			  if( (atoi(keyboard_entry2)) <= 59 )
// 				  tm = atoi(keyboard_entry2);
// 			  else clear_buffer2();
//
// 			  printDisp_i2(tm,2,4,8,RT,CLEAR);
//
// 			  //=======================================
// 			  if (clock_save2)
// 			  {
// 				clock_save2 = false;
//
// 				DS1307_SetHour(ttime2[0]);
// 				DS1307_SetMinute(ttime2[1]);
// 				DS1307_SetSecond(00);
// 				HAL_Delay(15);
//
// 				clr_screen2();
// 				send_line22("  saved");
// 				HAL_Delay(1000);
//
// 				hour = DS1307_GetHour();
// 				minute = DS1307_GetMinute();
// 				second = DS1307_GetSecond();
//
// 				clear_buffer2();
// 				entry_clear = 1;
// 				respons_dir = 0;
// 			  }
// 		  }
//
// 		  else if( ((respons == 2) || (respons_dir == 2)) && (!failFlag_dt) )
// 		  {
// 			  if(entry_clear == 1)
// 			  {
// 				  respons = 0;
// 				  respons_dir = 2;
// 				  clear_buffer2();
// 				  entry_clear = 0;
// 			  }
// 			  if ( (!yy_success) && (!mth_success2) )
// 			  {
// 				  snprintf(st__, sizeof(st__), "  %02d", year);
// 				  printDisp_c2("Yr",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
// 			  }
// 			  else if( (yy_success) && (!mth_success2) )
// 			  {
// 				  snprintf(st__, sizeof(st__), "  %02d", month);
// 				  printDisp_c2("nnO",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
// 			  }
// 			  else if(mth_success2)
// 			  {
// 				  snprintf(st__, sizeof(st__), "  %02d", day);
// 				  printDisp_c2("dd",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
// 			  }
//
// 			  if( (atoi(keyboard_entry2)) <= 99 )
// 				  dte = atoi(keyboard_entry2);
// 			  else clear_buffer2();
//
// 			  printDisp_i2(dte,2,4,8,RT,CLEAR);
//
// 			  //=======================================
// 			  if (clock_save2)
// 			  {
// 				clock_save2 = false;
// 				DS1307_SetDate(ddate2[0]);
// 				DS1307_SetMonth(ddate2[1]);
// 				DS1307_SetYear(ddate2[2]);
// //							DS1307_SetDayOfWeek(3);
// 				HAL_Delay(15);
//
// 				clr_screen2();
// 				send_line22("  saved");
// 				HAL_Delay(500);
//
// 				day = DS1307_GetDate();
// 				month = DS1307_GetMonth();
// 				year = DS1307_GetYear();
// 				dayofweek = DS1307_GetDayOfWeek();
//
// 				clear_buffer2();
// 				entry_clear = 1;
// 				respons_dir = 0;
// 			  }
//    		 }
//
// 		 pkey = read_keypad2();
//
// 		 if( (pkey > 0) || (failFlag_tm) || (failFlag_dt) )         //key detected
// 		 {
// 			if (pkey == 'C')  // up key
// 			{
//
// 			}
//
// 			if (pkey == 'B')  // down key
// 			{
//
// 			}
//
// 			if ( ((pkey == 'D') && ((respons == 1) || (respons_dir == 1))) || (failFlag_tm) )  //store entered value.
// 	        {
// 				 if( ((tm <= 23) && (!hh_success)) && (!failFlag_tm) )
// 				 {
// 					 ttime2[0] = tm;
// 					 clear_buffer2();
// 					 hh_success = true;
// 				 }
// 				 else if ( ((tm <=59) && (hh_success)) && (!failFlag_tm) )
// 				 {
// 					 ttime2[1] = tm;
// 					 clear_buffer2();
// 					 hh_success = false;
// 					 clock_save2 = true;
// 				 }
// 				 else
// 				 {
// 					 send_line22("        ");
// 					 if(!hh_success)
// 					 {
// 						 snprintf(st__, sizeof(st__), "  %02d", hour); //,log_a_new.time._mn);
// 						 printDisp_c2("hr",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
// 					 }
// 					 else if(hh_success)
// 					 {
// 						 snprintf(st__, sizeof(st__), "  %02d", minute);
// 						 printDisp_c2("nnI",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
// 					 }
// 					 send_line22(" failed");
// 					 clear_buffer2();
//
// 					 if(!failFlag_tm) _tt2 = 0;
//
// 					 if( (failFlag_tm) && (_tt2 > 600) ) failFlag_tm = 0;
// 					 else failFlag_tm = 1;
// 				 }
// 			}
//
// 			else if( ((pkey == 'D') && ( (respons == 2) || (respons_dir == 2) )) || (failFlag_dt) )  //store entered value.
// 			{
// 				 if( (dte <= 99) && (!yy_success) && (!mth_success2) && (!failFlag_dt) )
// 				 {
// 					 ddate2[2] = dte;
// 					 clear_buffer2();
// 					 yy_success = true;
// 				 }
// 				 else if ( (dte != 0) && (dte <= 12) && (yy_success) && (!mth_success2) && (!failFlag_dt) )
// 				 {
// 					 ddate2[1] = dte;
// 					 clear_buffer2();
// 					 yy_success = false;
// 					 mth_success2 = true;
// 					 montth = dte;
// 				 }
// 				else if( (mth_success2) && ((montth == January) || (montth == March) || (montth == May) || (montth == July) || (montth == August) || (montth == October) || (month == December)) && (!failFlag_dt) )
// 				{
// 					if ( (dte != 0) && (dte <= 31) )
// 					{
// 						save_date2(dte);
// 					}
// 					else goto __fail2;
// 				}
// 				else if( (mth_success2) && ((montth == April) || (montth == June) || (montth == September) || (montth == November)) && (!failFlag_dt) )
// 				{
// 					if ( (dte != 0) && (dte <= 30) )
// 					{
// 						save_date2(dte);
// 					}
// 					else goto __fail2;
// 				}
// 				else if( (mth_success2) && (montth == February) && (!failFlag_dt) )
// 				{
// 					if( leapYear_calc2(ddate2[2]) )
// 					{
// 						if ( (dte != 0) && (dte <= 29) )
// 						{
// 							save_date2(dte);
// 						}
// 						else goto __fail2;
// 					}
// 					else
// 					{
// 						if ( (dte != 0) && (dte <= 28) )
// 						{
// 							save_date2(dte);
// 						}
// 						else goto __fail2;
// 					}
// 				}
// 				else
// 				{
// 					__fail2:   send_line22("        ");
// 							  if( (!yy_success) && (!mth_success2) )
// 							  {
// 								  snprintf(st__, sizeof(st__), "  %02d", year);
// 								  printDisp_c2("Yr",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
// 							  }
// 							  else if( (yy_success) && (!mth_success2) )
// 							  {
// 								  snprintf(st__, sizeof(st__), "  %02d", month);
// 								  printDisp_c2("nnO",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
// 							  }
// 							  else if(mth_success2)
// 							  {
// 								  snprintf(st__, sizeof(st__), "  %02d", day);
// 								  printDisp_c2("dd",1,0,8,LT,CLEAR); printDisp_c2(st__,1,4,8,RT,NOCLEAR);
// 							  }
//
// 							 send_line22(" failed");
// 							 clear_buffer2();
//
// 							 if(!failFlag_dt) _tt2 = 0;
//
// 							 if( (failFlag_dt) && (_tt2 > 600) ) failFlag_dt = 0;
// 							 else failFlag_dt = 1;
// 				 }
// 		}
//
// 		 if (pkey == 'A')  // back key
// 		{
// 			 fxn = nothing;
// 			 respons_dir = 0;
// 			 failFlag_dt = 0;
// 			 failFlag_tm = 0;
// 			 hh_success = false;
//			 yy_success = false;
// 			 mth_success2 = false;
// 			 clock_save2 = 0;
// 			 montth = 0;
// 			 entry_clear = 1;
// 			 clr_screen2();
// 			 clear_buffer2();
// 		}
// 	}
// 	//=======================================
// 	return prog_State;
//   }
//
//
//
//
//
////============================== MODE ==================================
//
//  return prog_State;
//} //end of prog_state.

//----------------------------------------
//eSystemState idlestate_Handler2(void)
//{
//	  progg2 = 0;
//		if (t2 > 500)
//		  {
//			 send_line12(upper2);
//			 send_line22(middle2);
//			 char str__[8]= {0};
//			 snprintf(str__, sizeof(str__), "%.2f", litre_price2);
//			 send_line32(str__);
//			 t2 = 0;
//		  }
//
//#if sense_power == 1
//	  	  if(readpwr() == 0)
//			  {
//	  		  	   //count time elapsed
//	  		       if (shutdown_timer1 > 120)
//	  		    	   displayandkeypad_power(DEACTIVATE);   //shutdown... after  2 minutes
//			  }
//	  	  else
//	  	  {
//	  		  //clear the timer//
//	  		  shutdown_timer1 = 0;
//	  	  }
//#endif
//
//		return idle_State;
//}


eSystemState idleState_Handler2(void)
{
	static int  printer_status;

	static int8_t idleState_flag = 1;

	static int64_t current_pulser_ = 0,
				   old_pulser_ = 0;

	uint16_t gerCtTime;

	int pulser_diff = 0;

	pump2_status_4G = STATUS_IDLE;

	stop_fueling_bit2 = 1;

	lock_clr2 = 0;
	progg2 = 0;

	firstTime_nozz2 = 1;

	#if !defined (DEV_MODE)
		if(batteryStatus == LOWBATTERY)
		{
			send_line12("  Louu   ");
			send_line22("Battery  ");
			send_line32(" Err70 ");

			return inactive_State;
		}
		else if(batteryStatus == NOBATTERY)
		{
			send_line12("Battery ");
			send_line22(" Error  ");
			send_line32(" Err71 ");

			return inactive_State;
		}

		if(HAL_GPIO_ReadPin(pulser2_detect_GPIO_Port, pulser2_detect_Pin) == 1 )
		{
			send_line12(" Pulser ");
			send_line22("  Error ");
			send_line32(" Err24 ");

			return inactive_State;
		}

	//	if(calib_pulser2 < 15800)  //15987, 15967 .... 1106247681
		if(calibration_flag2 != CALIBRATED) //15800)  //15987, 15967 .... 1106247681
		{
			retrieve_calibrationFlag(side_b);

			if(calibration_flag2 != CALIBRATED) //takes care of accidental clearing of calibration_flag1 by F-keys
			{
				calibration2_error = 1;

				send_line12("Calibrate");
				send_line22("  Error ");
				send_line32("Err 23 ");

				return inactive_State;
			}
		}
		else
		{
			calibration2_error = 0;
		}
	#endif   //#if !defined (DEV_MODE)

	if(nozzleUp_inProgMode2 == 1)
	{
			clear_buffer2();
			prog_entry2 = 0;
			prog_revisit2 = 1;

//		#ifndef DEV_MODE
			prog_revisitt2 = 1;
//		#endif

			nozzleUp_inProgMode2 = 0;
	}
	if(ctTimed_flag2 == 1)
	{
		day = DS1307_GetDate();
		if(ctTimed_day2 == day)
		{
			gerCtTime = get_ctTime2();

			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
			//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx IF IT'S TIME xxxxxxxxxxxxxxxxxxxxxxxxxxxxx//
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
			if( (gerCtTime >= ctTimed_settingsB.startTime) && (gerCtTime <= ctTimed_settingsB.endTime) )
			{
				settings_stream1[1].pi_cal = (calib_pulser2 / vol_effective2_2);
			}
//			if(gerCtTime > ctTimed_settingsB.endTime)
//			{
//				ctTimed_flag2 = 0;
//				save_ctTimedFlag(side_b);
//			}
			else
			{
				retrieve_originalPi_c(side_b);
			}

			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
			//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx IF TIME ELAPSESxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
			if(gerCtTime > ctTimed_settingsB.endTime)
			{
				if( ( (ctTimed_settingsB.startTime - ctTimed_settingsB.endTime) < 0) || ( (ctTimed_settingsB.startTime - ctTimed_settingsB.endTime) == 0) )
				{
					ctTimed_flag2 = 0;
					save_ctTimedFlag(side_b);
				}
				else if( (ctTimed_settingsB.startTime - ctTimed_settingsB.endTime) > 0)
				{
					if( (ctTimed_day2 + 1) == day)
					{
						ctTimed_flag2 = 0;
						save_ctTimedFlag(side_b);
					}
				}
			}
		}
	}

	if( (idleState_flag == 1) && (eLastState2 != filling_State) )
	{
		old_pulser_ = current_pulser_ ;
		idleState_flag = 0;
		_tt2 = 0;
	}
	else if( (_tt2 > 3000) && (idleState_flag == 0) && (eLastState2 != filling_State) )
	{
		if(old_pulser_ > current_pulser_ )
		{
			pulser_diff = old_pulser_ - current_pulser_;

			if(pulser_diff > 4)
				idle_backwardPulse2 = 1;
		}
		else if(old_pulser_ < current_pulser_)
		{
			pulser_diff = current_pulser_ - old_pulser_;

			if(pulser_diff > 4)
				idle_forwardPulse2 = 1;
		}

		idleState_flag = 1;
	}

	if(changeLitrePrice2 == 1)
	{
//		if( ((eNextState2 == idle_State) && (eLastState2 != idle_State))
//			|| ((eNextState2 != idle_State) && (eLastState2 == idle_State)) )
//		if( ! ( (eNextState2 == idle_State) && (eLastState2 == idle_State) ) )
		if( ( (eNextState2 != idle_State) || (eLastState2 != idle_State) ) &&
			( (eNextState2 != idle_State) && (eLastState2 != inactive_State) ) )
		{
			priceChange_timer2 = 0;
		}
		else //if( (eNextState2 == idle_State) && (eLastState2 == idle_State) )
		{
			if(priceChange_timer2 >= 3000)
			{
				priceChange_timerMin2++;
				priceChange_timer2 = 0;

				if(priceChange_timerMin2 == 3)
				{
//					online_setUnitPrice2();
					go_setUnitPrice2(price_update2);
					changeLitrePrice2 = 0;
					changeLitrePrice2_2 = 1;
					priceChange_timerMin2 = 0;

					t2 = 0;
				}
			}

		}
	}

	else if ( (t2 > 500) && (nozzleup_awaitingauth_state_not_timedOut2 == 0) && (pump_LitreOverflow2 == 0) && (_litre_price2 == 0)
			&& (_auth_p2 == 0) && (_auth_v2 == 0) && (idle_backwardPulse2 == 0) && (idle_forwardPulse2 == 0)
			&& (flow_loss2 == 0) && (display_overflow2 == 0) && (changeLitrePrice2_2 == 0) && (_pump_max_litres2 == 0) && (nonValid_sale2 == 0) )
	{
		if(settings_stream1[1].display_format == PL)
		  {
			 send_line12(upper2);
			 send_line22(middle2);
		  }
		  else if(settings_stream1[1].display_format == LP)
		  {
			  send_line12(middle2);
			  send_line22(upper2);
		  }
		 send_line32("        ");
		 char str__[8]= {0};
		 snprintf(str__, sizeof(str__), "%.2f", litre_price2); send_line32(str__);
		 t2 = 0;
	 }

	else if ( (t2 > 2000) && (pump_LitreOverflow2 == 1) )
	{
		 // send_line22("n up u a");
//			 clr_screen2();
		 send_line12("  Punnp   ");
		 send_line22(" Linnit  ");
		 send_line32("Err7   ");
		 if(t2 > 6000)
		 {
			 t2 = 0;
			 pump_LitreOverflow2 = 0;
		 }
	 }
	 else if (display_overflow2 == 1)
		{
	  		 if(firstTime_display_overflow2 == 1)
			 {
	  			 if( (t2 > 2000) && (t2 <= 6000) )
	  			 {
	  				 clr_screen2();
	  				 send_line12("Display   ");
					 send_line22(" Linnit  ");
					 send_line32("Err18   ");
	  			 }
	  			 else if(t2 > 6000) //&& (display_overflow2 == 1) )
				 {
					 t2 = 0;

					 clr_screen2();

//					 send_line12(upper2);
//					 send_line22(middle2);

					 if(settings_stream1[1].display_format == PL)
					  {
						 send_line12(upper2);
						 send_line22(middle2);
					  }
					  else if(settings_stream1[1].display_format == LP)
					  {
						  send_line12(middle2);
						  send_line22(upper2);
					  }

					 send_line32("        ");
					 char str__[8]= {0};
					 snprintf(str__, sizeof(str__), "%.2f", litre_price2); send_line32(str__);
					 firstTime_display_overflow2 = 0;
				 }
			 }
	  		 else if(firstTime_display_overflow2 == 0)
			 {
	  			 if( (t2 > 6000) && (t2 <= 12000) )
	  			 {
	  				 clr_screen2();
	  				 send_line12("Display   ");
					 send_line22(" Linnit  ");
					 send_line32("Err18   ");
	  			 }
	  			 else if(t2 > 12000) //&& (display_overflow2 == 1) )
				 {
					 t2 = 0;

					 clr_screen2();
					 if(settings_stream1[1].display_format == PL)
					  {
						 send_line12(upper2);
						 send_line22(middle2);
					  }
					  else if(settings_stream1[1].display_format == LP)
					  {
						  send_line12(middle2);
						  send_line22(upper2);
					  }

					 send_line32("        ");
					 char str__[8]= {0};
					 snprintf(str__, sizeof(str__), "%.2f", litre_price2); send_line32(str__);
				 }
			 }
		}

	  	else if( (t2 > 2000) && (_litre_price2 == 1) )
	  	{
	  		 send_line12("No Unit   ");
			 send_line22("Price  ");
			 send_line32("Err8   ");
			 if(t2 > 6000)
			 {
				 t2 = 0;
				 _litre_price2 = 0;
			 }
	  	}
	  	else if( (t2 > 2000) && (_pump_max_litres2 == 1) )
	  	{
	  		 send_line12("No-punnp   ");
			 send_line22(" Linnit  ");
			 send_line32("Err21   ");
			 if(t2 > 6000)
			 {
				 t2 = 0;
				 _pump_max_litres2 = 0;
			 }
	  	}
	  	else if( (t2 > 2000) && (_auth_p2 == 1) )
		{
			 send_line12("    No   ");
			 send_line22("  Price  ");
			 send_line32("Err9   ");
			 if(t2 > 6000)
			 {
				 t2 = 0;
				 _auth_p2 = 0;
			 }
		}
		else if( (t2 > 2000) && (_auth_v2 == 1) )
		{
			 send_line12("  No   ");
			 send_line22("Volunne ");
			 send_line32("Err10   ");
			 if(t2 > 6000)
			 {
				 t2 = 0;
				 _auth_v2 = 0;
			 }
		}
//		else if( (t2 > 2000) && (_auth_v2 == 1) )
//		{
//			 send_line12("  No   ");
//			 send_line22("Annount  ");
//			 send_line32("Err17   ");
//			 if(t2 > 6000)
//			 {
//				 t2 = 0;
//				 _auth_v2 = 0;
//			 }
//		}
		else if( (t2 > 2000) && (nonValid_sale2 == 1) )
		{
			 send_line12("   Non   ");
			 send_line22("  Valid  ");
			 send_line32("Err22   ");
			 if(t2 > 6000)
			 {
				 t2 = 0;
				 nonValid_sale2 = 0;
			 }
		}
		else if( (t2 > 2000) && (idle_backwardPulse2 == 1) )
		{
			 send_line12("  Back   ");
			 send_line22("  Flouu  ");
			 send_line32("Err13   ");
			 if(t2 > 6000)
			 {
				 t2 = 0;
				 idle_backwardPulse2 = 0;
			 }
		}
		else if( (t2 > 2000) && (idle_forwardPulse2 == 1) )
		{
			 send_line12("   Idle   ");
			 send_line22("  Flouu  ");
			 send_line32("Err14  ");
			 if(t2 > 6000)
			 {
				 t2 = 0;
				 idle_forwardPulse2 = 0;
			 }
		}
		else if( (t2 > 2000) && (flow_loss2 == 1) )
		{
			 send_line12("   Flouu   ");
			 send_line22("  Loss  ");
			 send_line32("Err16  ");
			 if(t2 > 6000)
			 {
				 t2 = 0;
				 flow_loss2 = 0;
			 }
		}

		else if( (t2 > 300) && (changeLitrePrice2_2 == 1) )
		{

			 clr_screen2();
			 send_line12("  Price ");
			 send_line22(" Changed ");

			 send_line32("        ");
			 char str__[8]= {0};
			 snprintf(str__, sizeof(str__), "%.2f", litre_price2); send_line32(str__);

			 ep1a_priceChangeFlag2 = 1;

			 return inactive_State;
		}



	 if(keypress_2 == 21)
	 {
		   compose_printer();
		   HAL_Delay(1000);
		   printer_status = 1;
	 }

	 print__1();   //print the transaction.

	#if sense_power == 1
		  	  if(readpwr() == 0)
			  {
		  		   modem_power(DEACTIVATE);

				   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
				   HAL_Delay(200);
				   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);

		  		   //count time elapsed
				   if (shutdown_timer2 > 120)
				   {
					   displayandkeypad_power(DEACTIVATE);   //shutdown... after  2 minutes
				   }
			  }
		  	  else
		  	  {
		  		  //clear the timer//
		  		  shutdown_timer2 = 0;
		  	  }
	#endif

	  	return idle_State;
}

eSystemState  nozzleup_waitingforauthState_Handler2(void)
{
   	if (settings_stream1[operating_side - 1].mode == MANUAL_MODE)
		{
			//send nozzleup command only in MANUAL mode
			return authorised_nozzleup_State;     //idle_State;
		}
   	else
   	{
   		// AUTO mode...
   		if (controller_authorise())    // authed by controller...
   		{
//   			return  authorised_nozzleup_State;
   		}
   		else
   		{
   			nozzleup_awaitingauth_state_not_timedOut2 = 1;

   			//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[

			//Amount, Vol., and Alarm cleared
			//Light switched on
			//Preset-Vol Cleared to default value
			//Display cleared

			//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

   			if(t2 > 2400) t2 = 0;

   			 if ( ((t2 > 400) && (t2 <= 900)) && (nozzleup_awaitingauth_state_not_timedOut2 == 1) )
			 {
				 clr_screen2();
				 send_line12("Auth2    ");
			 }
			 else if ( ((t2 > 900) && (t2 <= 1400)) && (nozzleup_awaitingauth_state_not_timedOut2 == 1) )
			 {
				send_line12("Auth_ ");
			 }
			 else if ( ((t2 > 1400) && (t2 <= 1900)) && (nozzleup_awaitingauth_state_not_timedOut2 == 1) )
			 {
				send_line12("Auth__ ");
			 }
			 else if ( ((t2 > 1900) && (t2 <= 2400)) && (nozzleup_awaitingauth_state_not_timedOut2 == 1) )
			 {
				send_line12("Auth___ ");
			 }
   		}
   	}
	return nozzleup_waitingforauth_State;
}

eSystemState authorised_nozzleup_State_Handler2(void)
{
	float sellPrice_max_pump,
		  sellPrice_max_dpp;

	static int8_t firstTime = 1;

	pump_status_2 = STATUS_AUTH;

	slowFlow_startThreshold2 = (fast_flow_threshold2 * settings_stream2[1].valve_salesStart);
	slowFlow_endThreshold2 = (fast_flow_threshold2 * settings_stream2[1].valve_salesEnd);


	if (stop_flag2 == 1)   //if stop key pressed
	{
		filling2 = 0;
		stop_flag2 = 0;
		stop_flow2(); //send_solenoid(1);  //stop solenoid.

	  //--------------------------------------------------------
		dpFlag2 = 0;
		error_clr_flag2 = 1;
		 index_2 = 0;
		 _index2 = 0;
		 for(int i = 0; i < 9; i++)
		 {
		   keypad_pw_xter2[i] = 0;
		   keyboard_entry2[i] = 0;   //clear the buffer
		 }

		 for(int i = 0; i <= 8; i++)
		 {
			 keyboard2[i] = 0;
		 }
		 //--------------------------------------------------------------------
		 if (sellmode2 == P)
		 {
			  write_v2(3, "P     0");  //send_keypad("p    ");  //5 xters  lafeng..
		 }
		 else if(sellmode2 == L)
		 {
			  write_v2(3, "L     0");  //send_keypad("l    ");  //5 xters lafeng
		 }
	//	 else if(sellmode2 == V)
	//	 {
	//		  write_v2(3, "v    0");  //send_keypad("p    ");  //5 xters  lafeng..
	//	 }
		 send_keypad2(keyboard2);

		//--------------------------------------------------------------------

	       if (settings_stream1[1].mode == AUTO_MODE)
			 {

				 ///////////////////////////////////////////////////
				 ///////// SIGNALS GO ABOUT NOZZLE STATUS //////////

				 nozzle_out2 = false;

				 ///////////////////////////////////////////////////
			 }


			 if (settings_stream1[1].mode == MANUAL_MODE)
			 {
				 return idle_State;
			 }
		}


	if( firstTime_nozz2 == 1)
		{
			firstTime_nozz2 = 2;
			t2 = 0;

			return authorised_nozzleup_State;
		}
		else if(firstTime_nozz2 == 2)
		{
			if (t2 <= 1400)
			{
				send_line12("88888888");
				send_line22("88888888");
				send_line32("888888");
			}
			else if(t2 > 1400)
			{
				send_line32("        ");
				firstTime_nozz2 = 0;
			}

			return authorised_nozzleup_State;
		}

//	send_line32("        ");


	sellPrice_max_dpp = sellPrice_max_dp2(dp_amount2);

	nozzle_bit2 = 1;  stop_fueling_bit2 = 0;

    pump_status_2 = STATUS_AUTH;
    pump2_status_4G = STATUS_AUTHORIZED_NOZZLE_UP;

	display_minimumCentilitrePrice2 = (display_minimumCentilitre2 * litre_price2);

	 if(litre_price2 == 0)
	 {
		_litre_price2 = 1;
		return idle_State;
	 }
	 else if(pump_max_litres2 == 0)
	 {
		_pump_max_litres2 = 1;
		return idle_State;
	 }
	 else if ( (change_p2 == 1) && (auth_p2 == 0) )
	 {
		_auth_p2 = 1;
		return idle_State;
	 }
	 else if ( (change_v2 == 1) && (auth_v2 == 0) )
	 {
		_auth_v2 = 1;
		return idle_State;
	 }

	 else if(opmode2 == MANUAL_MODE)
	 {
		 if(index_2 >= 1)
		 {
			  key_value2 = atof(keyboard_entry2);


			  if(strchr(keyboard_entry2, '.'))
			  {
				  key_value2 += 0.00011;
			  }
			  //initialise the fuel and price variables

			  half_litre2 = (0.5 * litre_price2);

				if (sellmode2 == P)
				{
					  sellPrice_max_pump = (litre_price2 * pump_max_litres2);

					  if( (key_value2 > sellPrice_max_pump) || (key_value2 > sellPrice_max_dpp) ) // || (key_value2 < half_litre2) )
					  {
//						  if(key_value2 < half_litre2)
//						  {
////							 nonValid_sale2 = 1;
//							 nonValid_sale2 = 0;
//							 return idle_State;
//						  }
						  if(sellPrice_max_pump < sellPrice_max_dpp)
						  {
							  key_value2 = sellPrice_max_pump;
							  pump_LitreOverflow2 = 1;
						  }
						  else if(sellPrice_max_pump > sellPrice_max_dpp)
						  {
							  key_value2 = sellPrice_max_dpp;
							  display_overflow2 = 1;
						  }
					  }

					  target_pulser2 = price2pulser2(key_value2);  //calculate pulse frm price.

				}
				else  //amt was selected.
				{
					if( (key_value2 > pump_max_litres2) || (key_value2 > sellPrice_max_dpp) )  //|| (key_value2 < 0.5) )
					{
//						 if(key_value2 < 0.5)
//						 {
////							 nonValid_sale2 = 1;
//							 nonValid_sale2 = 0;
//							 return idle_State;
//						 }
						 if(pump_max_litres2 < sellPrice_max_dpp)
						 {
							  key_value2 = pump_max_litres2;
							  pump_LitreOverflow2 = 1;
						 }
						 else if(pump_max_litres2 > sellPrice_max_dpp)
						 {
							  key_value2 = sellPrice_max_dpp;
							  display_overflow2 = 1;
						 }
					  }

					 target_pulser2 = amt2pulser2(key_value2);   //calculate pulse frm amt.
				}
		  }
		  else
		  {
//			  key_value2 = (litre_price2 * pump_max_litres2);
//			  pump_LitreOverflow2 = 1;

			  key_value2 = sellPrice_max_dpp;
//			  display_overflow2 = 1;

			  target_pulser2 = price2pulser2(key_value2);  //calculate pulse frm price.
//			  key_value2 = 0;
		  }

	 }
	 else if(opmode2 == AUTO_MODE)
	 {
		  if (change_p2 == 1)
		  {
			  sellPrice_max_pump = (litre_price2 * pump_max_litres2);
//			  if(auth_p > sellPrice_max_pump)
//			  {
//				  auth_p = sellPrice_max_pump;
//				  pump_LitreOverflow2 = 1;
//			  }
//
//			  sprintf(keyboard_entry2,"%2f", auth_p);
//			  key_value2 = atof(keyboard_entry2);
//			  change_p = 0;      //reset tbe flag.
//			  index_2 = strlen(keyboard_entry2);
//
//			  target_pulser2 = price2pulser2(key_value2);  //calculate pulse frm price.
//		  }

//			  half_litre2 = (0.5 * litre_price2);

		  	  if( (auth_p2 > sellPrice_max_pump) || (auth_p2 > sellPrice_max_dpp) )
			  {
				  if(sellPrice_max_pump < sellPrice_max_dpp)
				  {
					  auth_p2 = sellPrice_max_pump;
					  pump_LitreOverflow2 = 1;
				  }

				  else if(sellPrice_max_pump > sellPrice_max_dpp)
				  {
					  auth_p2 = sellPrice_max_dpp;
					  display_overflow2 = 1;
				  }
			  }

		  	  sprintf(keyboard_entry2,"%2f", auth_p2);
			  key_value2 = atof(keyboard_entry2);
			  change_p2 = 0;      //reset tbe flag.
			  index_2 = strlen(keyboard_entry2);

			  target_pulser2 = price2pulser2(key_value2);  //calculate pulse frm price.
		  }


		 //-----------------------------------------
		  //authorise volume...

		  //-----------------------------------------
		  //authorise volume...
		  else if (change_v2 == 1)
		  {
			  if( (auth_v2 > pump_max_litres2) || (auth_v2 > sellPrice_max_dpp) )
			  {
				  if(pump_max_litres2 < sellPrice_max_dpp)
				  {
					  auth_v2 = pump_max_litres2;
					  pump_LitreOverflow2 = 1;
				  }

				  else if (pump_max_litres2 > sellPrice_max_dpp)
				  {
					  auth_v2 = sellPrice_max_dpp;
					  display_overflow2 = 1;
				  }
			  }

			  sprintf(keyboard_entry2,"%2f", auth_v2);
			  key_value2 = atof(keyboard_entry2);
			  change_v2 = 0;      //reset tbe flag.
			  index_2 = strlen(keyboard_entry2);

			  target_pulser2 = amt2pulser2(key_value2);   //calculate pulse frm amt.
		  }

//		  else if (change_v == 1)
//		  {
//			  if(auth_v > pump_max_litres2)
//			  {
//				  auth_v = pump_max_litres2;
//				  pump_LitreOverflow2 = 1;
//			  }
//
//			  sprintf(keyboard_entry2, "%2f", auth_v);
//			  key_value2 = atof(keyboard_entry2);
//			  change_v = 0;      //reset tbe flag.
//			  index_2 = strlen(keyboard_entry2);
//
//			  target_pulser2 = amt2pulser2(key_value2);   //calculate pulse frm amt.
//		  }


		  else if (index_2 >= 1)
		  {
				key_value2 = atof(keyboard_entry2);


				if(strchr(keyboard_entry2, '.'))
				{
					key_value2 += 0.00011;
				}

				half_litre2 = (0.5 * litre_price2);

			  //initialise the fuel and price variables
				if (sellmode2 == P)
				{
					  sellPrice_max_pump = (litre_price2 * pump_max_litres2);

					  if( (key_value2 > sellPrice_max_pump) || (key_value2 > sellPrice_max_dpp) )  //|| (key_value2 < half_litre2) )
					  {
//						  if(key_value2 < half_litre2)
//						  {
////							 nonValid_sale2 = 1;
//							 nonValid_sale2 = 0;
//							 return idle_State;
//						  }
						  if (sellPrice_max_pump < sellPrice_max_dpp)
						  {
							  key_value2 = sellPrice_max_pump;
							  pump_LitreOverflow2 = 1;
						  }

						  else if (sellPrice_max_pump > sellPrice_max_dpp)
						  {
							  key_value2 = sellPrice_max_dpp;
							  display_overflow2 = 1;
						  }
					  }

					  target_pulser2 = price2pulser2(key_value2);  //calculate pulse frm price.

				}
				else  //amt was selected.
				{
					if( (key_value2 > pump_max_litres1) || (key_value2 > sellPrice_max_dpp) ) //|| (key_value2 < 0.5) )
					  {
//						  if(key_value2 < 0.5)
//						  {
////							 nonValid_sale2 = 1;
//							 nonValid_sale2 = 0;
//							 return idle_State;
//						  }
						  if(pump_max_litres2 < sellPrice_max_dpp)
						  {
								key_value2 = pump_max_litres2;
								pump_LitreOverflow2 = 1;
						   }
						   else if (pump_max_litres2 > sellPrice_max_dpp)
						   {
								 key_value2 = sellPrice_max_dpp;
								 display_overflow2 = 1;
						   }
					}

					target_pulser2 = amt2pulser2(key_value2);   //calculate pulse frm amt.
				}
		 }
//		 		 else
//
//		  else if (index_2 >= 1)
//		  {
//			  	key_value2 = atof(keyboard_entry2);
//
//			  //initialise the fuel and price variables
//				if (sellmode2 == P)
//				{
//					  sellPrice_max_pump = (litre_price2 * pump_max_litres2);
//					  if(key_value2 > sellPrice_max_pump)
//					  {
//						  key_value2 = sellPrice_max_pump;
//						  pump_LitreOverflow2 = 1;
//					  }
//
//					  target_pulser2 = price2pulser2(key_value2);  //calculate pulse frm price.
//
//				}
//				else  //amt was selected.
//				{
//					if(key_value2 > pump_max_litres2)
//					{
//						key_value2 = pump_max_litres2;
//						pump_LitreOverflow2 = 1;
//					}
//
//					target_pulser2 = amt2pulser2(key_value2);   //calculate pulse frm amt.
//				}
//		 }
		 else
		 {
			  key_value2 = sellPrice_max_dpp;
//			  display_overflow2 = 1;

//			  key_value2 = (litre_price2 * pump_max_litres2);
//			  pump_LitreOverflow2 = 1;

			  target_pulser2 = price2pulser2(key_value2);  //calculate pulse frm price.
		 }
	   }
//============================================================================
//============================================================================
//	  if (authorise_flag == 1)
//	   {
//		  target_pulser2 = price2pulser2(5000);
//		  key_value2  = 5000;
//		  authorise_flag == 0;
//	   }
//============================================================================
//============================================================================

	  price2  = 0.0;
		  amt2  = 0.0;

		  if (key_value2 == 0)
		  {
			   target_pulser2 = 0;
		  }
		  else
			  pulser_rem2 = target_pulser2 - current_pulser2;

		  if(lock_clr2 == 0)  ///   to activate this section once.
		  {
				 current_pulser2 = 0;
				 clr_pulser2();    //clear hardware pulser

				 //current_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);
				 lock_clr2 = 1;
				 filling2 = 1;

				 firstTime = 1;
				 motor_tmr2 = 0;
		  }

		 // int cnv = 0;
	     //char temp[10] = {0};
		 //snprintf(temp , sizeof(temp), "%.2f", price);  //send_line22(upper1);
         //snprintf(middle1, sizeof(middle1), "%.2f", amt);   //send_line22(lower);

	  	  make_string2(P,price2);
		  make_string2(L,amt2);

//		 if (t2 > 300)
//		   {
//				send_line12(upper2);
//				send_line22(middle2);
//
//				char str__[8]= {0};
//				snprintf(str__, sizeof(str__), "%.2f", litre_price2);
//				send_line32(str__);
//				t2 = 0;
//
//							//trigger first pulser...
//
//			#if (_USE_SOFT_PULSER == 1)
//					 //  pulser_new += 1;
//					   current_pulser2++;
//			#else
//					 // pulser_new = __HAL_TIM_GET_COUNTER(&htim5);
//					   current_pulser2 = __HAL_TIM_GET_COUNTER(&htim2);
//			#endif
//
//	 	   }


		 if (t2 > 300)
		 {
			 if(settings_stream1[1].display_format == PL)
			{
				 send_line12(upper2);
				 send_line22(middle2);
			 }
			 else if(settings_stream1[1].display_format == LP)
			 {
				  send_line12(middle2);
				  send_line22(upper2);
			 }

				char str__[8]= {0};
				snprintf(str__, sizeof(str__), "%.2f", litre_price2);
				send_line32(str__);
				t2 = 0;

							//trigger first pulser...
				if(firstTime == 1)
				{
					firstTime = 2;
					#if (_USE_SOFT_PULSER == 1)
						   current_pulser2++;
					#endif

				}
				else if (firstTime == 2)
				{
					firstTime = 3;

					#if (_USE_SOFT_PULSER == 1)
							current_pulser2++;
					#else
							current_pulser2 = 0;
							clr_pulser2();    //clear hardware pulser
					#endif
				}
				else if (firstTime == 3)
				{
					#if (_USE_SOFT_PULSER == 1)
						   current_pulser2++;
					#else
						   current_pulser2 = __HAL_TIM_GET_COUNTER(&htim2);
					#endif
				}
		 	 }


			#if (_USE_SOFT_PULSER == 1)
				 pulser_new2 = current_pulser2;
			#endif

			if (firstTime == 3)
				pulser_new2 = current_pulser2;

//=====================================================================
 		// filling1 = 1;
		// send_pump(ACTIVATE);  //pump on...

//  if(operating_side == side_a)
//	{
//		working_volTotaliser2 = totaliser_vol1;
//		working_volTotaliser2c = totaliser_vol1c;
//	}
//  else
//   {
  		working_volTotaliser2 = totaliser_vol2;
  		working_volTotaliser2c = totaliser_vol2c;

 		working_amtTotaliser2 = totaliser_amt2;
  		working_amtTotaliser2c = totaliser_amt2c;
//  	}

  r_volTotaliser2 = floor(working_volTotaliser2c);
  old_r_volTotaliser2 = r_volTotaliser2;

//  r_amtTotaliser2 = floor(working_amtTotaliser2c);
//  old_r_amtTotaliser2 = r_amtTotaliser2;

 // slow_flow1();

	return authorised_nozzleup_State;
}


eSystemState filling_State_Handler2(void)
{
	//extern uint32_t pulser_new;
	float temp = 0.0;

	static int8_t flow_flag = 1;

	static int32_t current_pulser_ = 0,
						   old_pulser_ = 0;

	pump2_status_4G = STATUS_FILLING_;

	lock_clr2 = 0;
	filling2  = 1;

	firstTime_nozz2 = 1;

	  if(target_pulser2 > 0)
		{
		 //	pulser_rem1 = target_pulser1 - current_pulser1; //now handled in
														 //interrupt
		}

//================================================================
#if !defined (DEV_MODE)
		if(batteryStatus == NOBATTERY)
		{
			  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
			  HAL_Delay(200);
			  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
			  filling2 = 0;
			  stop_flow2();
			  get_time();
			  do_calcs2();
			  update_info();
			  save_volumeTotaliser(operating_side);
			  save_amountTotaliser(operating_side);
			  save_lastSale(operating_side);


			  if(settings_stream1[1].mode == AUTO_MODE)
			  {
				 //////////////////////////////////////////////////////////////

				 hardwareErrorFlag_source2 = 1;

				 //////////////////////////////////////////////////////////////
			  }

			  return write_flash_State;
		}

//    power outage during filling1  end transaction...
		#if sense_power == 1
	  	  if(  (readpwr() == 0)||(read_p_pwr() == 0) )
		  {
			  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
			  HAL_Delay(200);
			  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
			  filling2 = 0;
			  stop_flow2();
			  get_time();
			  do_calcs2();
			  update_info();
			  save_volumeTotaliser(operating_side);
			  save_amountTotaliser(operating_side);
			  save_lastSale(operating_side);

			  if(settings_stream1[1].mode == AUTO_MODE)
			  {
				 //////////////////////////////////////////////////////////////

				 hardwareErrorFlag_source2 = 1;

				 //////////////////////////////////////////////////////////////
			  }

			  return write_flash_State;
		  }
		#endif

	if(HAL_GPIO_ReadPin(pulser2_detect_GPIO_Port, pulser2_detect_Pin) == 1 )
	{
		 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
		  HAL_Delay(200);
		  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
		  filling2 = 0;
		  stop_flow2();
		  get_time();
		  do_calcs2();
		  update_info();
		  save_volumeTotaliser(operating_side);
		  save_amountTotaliser(operating_side);
		  save_lastSale(operating_side);

		  if(settings_stream1[1].mode == AUTO_MODE)
		  {
			 //////////////////////////////////////////////////////////////

			 hardwareErrorFlag_source2 = 1;

			 //////////////////////////////////////////////////////////////
		  }

		  return write_flash_State;
	}

#endif   //#if !defined (DEV_MODE)

	if (stop_flag2 == 1)   //if stop key pressed
	{
		filling2 = 0;
		stop_flag2 = 0;
		stop_flow2(); //send_solenoid(1);  //stop solenoid.
		get_time();
        do_calcs2();
        update_info();
//        save_volumeTotaliser(operating_side);
//        save_amountTotaliser(operating_side);
//        save_lastSale(operating_side);

        save_volumeTotaliser_fram(operating_side);
		save_amountTotaliser_fram(operating_side);
		save_lastSale_fram(operating_side);

      //--------------------------------------------------------
//		dpFlag2 = 0;
//		error_clr_flag2 = 1;
//		 index_2 = 0;
//		 _index2 = 0;
//		 for(int i = 0; i < 9; i++)
//		 {
//		   keypad_pw_xter2[i] = 0;
//		   keyboard_entry2[i] = 0;   //clear the buffer
//		 }
//
//		 for(int i = 0; i <= 8; i++)
//		 {
//			 keyboard2[i] = 0;
//		 }
//		 //--------------------------------------------------------------------
//		 if (sellmode2 == P)
//		 {
//			  write_v2(3, "P     0");  //send_keypad("p    ");  //5 xters  lafeng..
//		 }
//		 else if(sellmode2 == L)
//		 {
//			  write_v2(3, "L     0");  //send_keypad("l    ");  //5 xters lafeng
//		 }
//	//	 else if(sellmode2 == V)
//	//	 {
//	//		  write_v2(3, "v    0");  //send_keypad("p    ");  //5 xters  lafeng..
//	//	 }
//		 send_keypad2(keyboard2);

		keypad_zerorize2();
	//--------------------------------------------------------------------

		 if(settings_stream1[1].mode == AUTO_MODE)
		 {
			 //////////////////////////////////////////////////////////////
			 ///////// SIGNALS GO-CONTROLLER ABOUT NOZZLE STATUS //////////

			 status_change_noz2 = 1;
			 nozzle_out2 = false;

			 stopFlag_source2 = 1;

			 //////////////////////////////////////////////////////////////
		 }

       return write_flash_State;
	}

	if(litre_price2 == 0)
		{
			stop_flow2(); //send_solenoid(1);  //stop solenoid.
			filling2 = 0;
			do_calcs2();
			get_time();
			update_info();
//			save_volumeTotaliser(operating_side);
//			save_amountTotaliser(operating_side);
//			save_lastSale(operating_side);

	        save_volumeTotaliser_fram(operating_side);
			save_amountTotaliser_fram(operating_side);
			save_lastSale_fram(operating_side);
			_litre_price2 = 1;

			if(settings_stream1[1].mode == AUTO_MODE)
			  {
				 //////////////////////////////////////////////////////////////

				 hardwareErrorFlag_source2 = 1;

				 //////////////////////////////////////////////////////////////
			  }

			return write_flash_State;
		}

		else if(pump_max_litres2 == 0)
		{
			stop_flow2(); //send_solenoid(1);  //stop solenoid.
			filling2 = 0;
			do_calcs2();
			get_time();
			update_info();
//			save_volumeTotaliser(operating_side);
//			save_amountTotaliser(operating_side);
//			save_lastSale(operating_side);

			save_volumeTotaliser_fram(operating_side);
			save_amountTotaliser_fram(operating_side);
			save_lastSale_fram(operating_side);
			_pump_max_litres2 = 1;

			if(settings_stream1[1].mode == AUTO_MODE)
			  {
				 //////////////////////////////////////////////////////////////

				 hardwareErrorFlag_source2 = 1;

				 //////////////////////////////////////////////////////////////
			  }

			return write_flash_State;
		}

//======================== @ filling1 =============================
	  // get_time2();
	   temp = pulser2amt2(current_pulser2);
	   amt2 = dp2(temp, dp_vol2);

	   temp = amt2price2(amt2);
	   price2 = dp2(temp, dp_amount2);

	   //		temp = pulser2amt2_R(current_pulser2);
//		amt_real2 = dp2(temp,dp_amount2);
//
//		temp = amt2price2(amt2);
//		price_real2 = dp2(temp,dp_price2);

	    make_string2(P, dp2(price2, dp_amount2));
	    make_string2(L, dp2(amt2, dp_vol2));
	    //send_line22(upper1);
	    //send_line22(middle1);

//	    amt_middle2 = atof(middle2);
//	    amt_middle2 += 0.00011;
//
//		price_upper2 = atof(upper2);
//		price_upper2 += 0.00011;

		running_volTotaliser2 = working_volTotaliser2   + amt_real2;
//	   	  running_volTotaliser2c = working_volTotaliser2c + amt2;

		running_volTotaliser2c = working_volTotaliser2c + amt_middle2;

		running_amtTotaliser2  = working_amtTotaliser2  + price_real2;
//		  running_amtTotaliser2c = working_amtTotaliser2c + price2;

		running_amtTotaliser2c = working_amtTotaliser2c + price_upper2;

		   float pricecheck = running_amtTotaliser2c - priceOld2;

		   if (pricecheck >= 1000.00)
		   {
			   priceOld2 = running_amtTotaliser2c;
			   save_amountSend(side_b);

			   char str[65];
			   sprintf(str, "[Side-B]... #%0.2f intermittent worth of sales made now!", pricecheck);
			   server_write(str);
		   }


//	    running_volTotaliser2  = working_volTotaliser2  + amt_real2;
//	   	running_volTotaliser2c = working_volTotaliser2c + amt2;
//
//
//	    running_amtTotaliser2  = working_amtTotaliser2  + price_real2;
//	   	running_amtTotaliser2c = working_amtTotaliser2c + price2;

//============================================================
//         for totaliser toggle.
	  r_volTotaliser2 	  = floor( running_volTotaliser2c );

//	  r_amtTotaliser2 	  = floor(running_amtTotaliser2c);
//
//	if(r_volTotaliser2 != old_r_volTotaliser2)
//		{
//			totalizer2Timer = 0;
//			//then toggle the totaliser harware I/O.
//			drive_totaliser2(ACTIVATE);
//		}
//	else
//	{
//		if(totalizer2Timer > 300)
//		{
//			//deactivate totaliser output...
//			drive_totaliser2(DEACTIVATE);
//		}
//
//	}
//	  old_r_volTotaliser2 = r_volTotaliser2;   //update...
//
//	  old_r_amtTotaliser2 = r_amtTotaliser2;   //update...

//============================================================
	  if (t2 > LCD_UPDATE_RATE)
	  {
		  if(settings_stream1[1].display_format == PL)
		  {
			 send_line12(upper2);
			 send_line22(middle2);
		  }
		  else if(settings_stream1[1].display_format == LP)
		  {
			  send_line12(middle2);
			  send_line22(upper2);
		  }
			char str__[8]= {0};
			snprintf(str__, sizeof(str_2), "%.2f", litre_price2);
			send_line32(str__);
			t2 = 0;
		  }
	  reset_timer2(timeout_dispense); //don't time out.
//==========================pulser capture ============================
#if (_USE_SOFT_PULSER == 1)
	 // if ( (t > 50)&&(t < 200) )
	 //  			{
	 //  	    	   current_pulser1++;   //transfer this to the interrupt routine..
	 //  			}
#else
	       current_pulser2 = __HAL_TIM_GET_COUNTER(&htim2);
#endif
//======================================================================
	   if(target_pulser2 > 0)
		{
			//pulser_rem1 = target_pulser1 - current_pulser1;
		}

	 if(target_pulser2 > 0)
       {
		  if (pulser_rem2 <= 0 || pulser_complete2 == 1 )  //sales complete...
		  	 {
				stop_flow2(); //send_solenoid(1);  //stop solenoid.
				filling2 = 0;
		        do_calcs2();
		    	get_time();
		        update_info();
//		        save_volumeTotaliser(operating_side);
//		        save_amountTotaliser(operating_side);
//		        save_lastSale(operating_side);

		        save_volumeTotaliser_fram(operating_side);
				save_amountTotaliser_fram(operating_side);
		        save_lastSale_fram(operating_side);

		        pump_status_2 = STATUS_MAMO_REACHED;

				if(settings_stream1[1].mode == AUTO_MODE)
				{
					mamo_reached_flag2_1 = 1;
				}

		        return write_flash_State;
		  	 }
		  else
		  {
			//  slow_flow1();
			  if (pulser_rem2 >= slowFlow_endThreshold2)
			  {
				    if(current_pulser2 >= slowFlow_startThreshold2)
					{
						fast_flow2();
						fastFlow2 = 1;
					}
					else
					{
						slow_flow2();
						fastFlow2 = 0;
					}
			  }
			  else
			  {
				  slow_flow2();
				  fastFlow2 = 0;
			  }
		  }
       }

	   if(flow_flag == 1)
	   {
			_tt2 = 0;
	   }
	   else if( (_tt2 > 60000) && (flow_flag == 2) )
	   {
			old_pulser_ = current_pulser2;
			flow_flag = 3;
	   }
	   else if( (_tt2 > 65000) && (flow_flag == 3) )
	   {
			if(old_pulser_ > current_pulser2)
			{
				current_pulser2 = old_pulser_ ;
				flow_loss2 = 1;
				filling2 = 0,  nozzle_bit2 = 0;
				stop_flag2 = 0;
				stop_flow2(); //send_solenoid(1);  //stop solenoid.
				get_time();
				do_calcs2();
				update_info();
//				save_volumeTotaliser(operating_side);
//				save_amountTotaliser(operating_side);
//				save_lastSale(operating_side);

				save_volumeTotaliser_fram(operating_side);
				save_amountTotaliser_fram(operating_side);
				save_lastSale_fram(operating_side);

				if(settings_stream1[1].mode == AUTO_MODE)
				  {
					 //////////////////////////////////////////////////////////////

					 hardwareErrorFlag_source2 = 1;

					 //////////////////////////////////////////////////////////////
				  }

				return write_flash_State;
			}
			flow_flag = 1;
		}

	   return filling_State;
}



//-------------------------------------
eSystemState keyup_Handler2(void)
{
	progg2 = 1;
	copy_settings(move_to_copy);	//make a copy of the settings								//to be used in the prog.
	clear_buffer2();
	return prog_State;
}
//-------------------------------------
eSystemState inactiveState_Handler2(void)
{
    //error_state = 1;
		  // All errors land here....
		  // Only the CLEAR  key returns pump from this state....
          // key reception is done in the key_entry handler..

	return inactive_State;
}
//--------------------------------------
eSystemState savesettings_State_Handler2(void)
{
   static int sav = 0;
   int respons = 0;
   //====================================================
	if (sav == 0)
		{
		  if(t2 > 500)
		   {
			  send_line12("1-  Saue");
			  send_line22("2-Cancel");
			  t2 = 0;
		   }
		  respons = atoi(keyboard_entry2);
        //------------------------------------------------
		  if(respons == 1)   //save settings
		  {
			 sav = 1;
			 return savesettings_State;
		  }
		  if(respons == 2)  //delete settings don't save.
		  {
		   sav = 2;
		   return savesettings_State;
		  }
		 //------------------------------------------------
		   return savesettings_State;
		}
   //=======================================
	if (sav == 1)
		{
		 	send_line12(" Sauing ");
		 	send_line22("Settings");

		 	copy_settings(move_to_settings0);
//		 	save_settings_original();
		 	save_settings_original_fram(side_a);

			configMode2 = CONFIGMODIFIED;
//			save_configFlag(side_b);
			save_configFlag_fram(side_b);

	    	copy_settings(move_to_settings); // copy the structure.
	    	save_settings();   //save to eeprom
	    	load_settings(side_b); //load the settings into the
	        load_settings(side_a); // internal variables
	    	HAL_Delay(1500);

	    	send_line12("  Done  ");
	    	send_line12("        ");
	        sav = 0;
			progg2 = 0;
			auth2 = not_auth;
			clear_buffer2();
			return idle_State;
		}
	//=======================================
	if (sav == 2)
	{
		 sav = 0;
		 progg2 = 0;
		 auth2 = not_auth;
		 //clear keyboard buffers.
		 clear_buffer2();

		 if(settings_stream1[1].display_format == PL)
		 {
			 send_line12(upper2);
			 send_line22(middle2);
		 }
		 else if(settings_stream1[1].display_format == LP)
		 {
			  send_line12(middle2);
			  send_line22(upper2);
		 }

		 char str__[8]= {0};
		 snprintf(str__, sizeof(str__), "%.2f", litre_price2);
		 send_line32(str__);

       return idle_State;
	}
	//========================================
	return savesettings_State;    //idle_State;  //return to idle mode
}
//--------------------------------------
eSystemState keydown_Handler2(void)
{
	clear_buffer2();
	prog_entry2 = 0;
	prog_revisit2 = 1;

#ifndef DEV_MODE
	prog_revisitt2 = 1;
#endif //#ifndef DEV_MODE

	 return savesettings_State;   //
}

//-----------------------------------
eSystemState nozzleup_Handler2(void)
{
	pump2_status_4G = STATUS_NOZZLE_UP;

	reset_timer2(timeout_picknozzle);
	start_timer2(timeout_picknozzle);

	//set the motor on and solenoid off.
	get_auth2();

	if(opmode2 == AUTO_MODE)
	{
		 nozzle_out2 = true;
		 status_change_noz2 = 1;

		 if(pump_status_2 == STATUS_FILLING_COMP)
		 {
			 return idle_State;
		 }
	}

	send_line12("88888888");
	send_line22("88888888");
	send_line32("888888");
//	send_line32("      ");


//	send_solenoid(0);
//	send_pump(1);      //turn on pump.

	if (eLastState2 == authorised_nozzledown_State)
		{
		     current_pulser2 = 0;
			 clr_pulser2();    //clear hardware pulser
		    return authorised_nozzleup_State;
		}

	return nozzleup_waitingforauth_State;
}
//-------------
eSystemState auth_command_Handler2(void)
{
	  reset_timer2( timeout_picknozzle);
	  start_timer2( timeout_picknozzle);
    //-----------------------------------
	  if(opmode2 == MANUAL_MODE)
	  {
		  //check if any keypad entry
		 if(index_2 >= 1)
		  {
              key_value2 = atof(keyboard_entry2);
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

		  if(change_price_flag == 1)
		  {
				setUnitPrice();
				return idle_State;
		  }

		  // AUTO Mode
		  //authorise price...
		  if (change_p2 == 1)
			  sellmode2 = P;      //set sell type to price
		 //-----------------------------------------
		  //authorise volume...
		  else if (change_v2 == 1)
			  sellmode2 = L;      //set sell type to litre
		 //-----------------------------------------
	  }
    // GO can also send this command.
  return authorised_nozzledown_State;
}

//-------------


//----------------
eSystemState authorise_Handler2(void)
{
	reset_timer2(timeout_picknozzle);
	start_timer2(timeout_picknozzle);

//initialise the fuel and price variables
 price2  = 0.0;
 amt2 = 0.0;

 target_pulser2 = 0;  //state is coming from nozzleup ,no price/amt set

 pump_status_2 = STATUS_AUTH;
 status_change_pump2 = 1;

 clr_pulser2();    //clear hardware pulser
 current_pulser2 = 0;

// int cnv = 0;
// snprintf(str_, sizeof(str_), "%.2f", price); send_line22(str_);
// snprintf(str_, sizeof(str_), "%.2f", amt);   send_line22(str_);

 send_line12("--auth2--");
 send_line22("        ");
 char str__[8]= {0};
 snprintf(str__, sizeof(str__), "%.2f", litre_price2); send_line22(str__);

 return authorised_nozzleup_State;
}

//---------------
eSystemState authorised_nozzledown_State_Handler2(void)
{
	nozzle_bit2 = 0;
	pump2_status_4G = STATUS_AUTHORIZED_NOZZLE_DOWN;

	 //initialise the fuel and price variables

	    if(index_2 >= 1)
		  {
			  key_value2 = atof(keyboard_entry2);
		  }
		 else
		 {
			  key_value2 = 0.0;
		 }

	    //initialise the fuel and price variables
	      if (sellmode2 == P)
	    	{
			  target_pulser2 = price2pulser(key_value2);  //calculate pulse frm price.
	    	}
	      else  //amt was selected.
	      {
	    	  target_pulser2 = amt2pulser(key_value2);   //calculate pulse frm amt.
	      }

	  price2  = 0.0;
	  amt2 = 0.0;

	  current_pulser2 = 0;

   if(t2 > 300)
   {
	  send_line12(" Pic    ");
	  send_line22("No22le  ");
	  char str__[8]= {0};
	  snprintf(str__, sizeof(str__), "%.2f", litre_price2); send_line32(str__);
      t2 = 0;
   }

	if (display_overflow2 == 1)
	{
		display_overflow2 = 0;
		firstTime_display_overflow2 = 0;
	}

	return authorised_nozzledown_State;
}


//----------------------
uint32_t price2pulser2(float price2)
{
//  float temp = (price2 / litre_price2) *  pulser_index_c2;
//
//  temp -= 0.004;
//
//  original_pulse2 = temp;

  float temp = (price2 / litre_price2);
//  temp -= 0.01;

  temp -= 0.004;

  temp *= pulser_index_c2;

  original_pulse2 = temp;

  //temp = floor(temp);
  return  floor(temp );
}

uint32_t amt2pulser2(float amt)
{
	 float temp;

	 amt -= 0.004;

	 temp = amt * pulser_index_c2;
	 original_pulse2 = temp;
	 return  floor(temp );
}

float pulser2price2(uint32_t pulse_)
{
	 float temp = litre_price2 * (1.0/pulser_index_c2);
	 temp  = temp * (float) pulse_;
	 return (temp );
}

float amt2price2(float amt_)
{
	 float temp = litre_price2 * amt_;
	 return (temp );
}

float pulser2amt2(uint32_t pulse_)
{
	 float temp = (float)pulse_ * (1.0/pulser_index_c2);
	 return (temp );
}

float pulser2amt_R2(uint32_t pulse_)
{
	 float temp = pulse_ * (1.0/pulser_index2);
	 //temp  = temp * (float) pulse_;
	 return ( temp );
}
//---------------------------------------------------
void make_string2(sellmode_ sll,float pr)
{
	int8_t tmp = 1,
		   ind = 0;

	char temp[10] = {0};
	char ttmp[10] = {0};

	 if (sll == L)
	 {
		 if (dp_vol2 == 1)
		     snprintf(temp , sizeof(temp), "%.1f", pr);
		 else if (dp_vol2 == 2)
			 snprintf(temp , sizeof(temp), "%.2f", pr);
		 else if (dp_vol2 == 3)
		 	 snprintf(temp , sizeof(temp), "%.3f", pr);

		 amt_middle2 = atof(temp);
		 amt_middle2 += 0.00011;

		 if(pr < display_minimumCentilitre2)   // 9 centilitres
		 {
			 pr = 0.00;
			 if (dp_vol2 == 1)
				 snprintf(temp , sizeof(temp), "%.1f", pr);
			 else if (dp_vol2 == 2)
				 snprintf(temp , sizeof(temp), "%.2f", pr);
			 else if (dp_vol2 == 3)
				 snprintf(temp , sizeof(temp), "%.3f", pr);
		 }
	 }

	 if (sll == P)
		 {
			 if (dp_amount2 == 1)
			     snprintf(temp , sizeof(temp), "%.1f", pr);
			 else if (dp_amount2 == 2)
				 snprintf(temp , sizeof(temp), "%.2f", pr);
			 else if (dp_amount2 == 3)
			 	 snprintf(temp , sizeof(temp), "%.3f", pr);

			price_upper2 = atof(temp);
			price_upper2 += 0.00011;

			if(pr < display_minimumCentilitrePrice2)   // 9 centilitres
			{
				pr = 0.00;
				if (dp_amount2 == 1)
					 snprintf(temp , sizeof(temp), "%.1f", pr);
				else if (dp_amount2 == 2)
					 snprintf(temp , sizeof(temp), "%.2f", pr);
				else if (dp_amount2 == 3)
					 snprintf(temp , sizeof(temp), "%.3f", pr);
			}

		 }

	int index__ = 0;
		while (tmp != 0)
		{
			tmp = temp[index__++];
		}

	int space = 9 - index__;

	if(eNextState2 == filling_State)
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
		 for (int i = 0 ; i < 10 ; i++)
		   {
				if (sll == P)
					upper2[i] = ttmp[i];
				else
					middle2[i] = ttmp[i];
		   }
     int t2 = 0;
}

//---------------------------------------------------



//eSystemState authorised_nozzleup_State_Handler(void)
//{
//	float sellPrice_max_pump,
//		  sellPrice_max_dpp;
//
//	nozzle_bit = 1;  stop_fueling_bit = 0;
//
//    pump_status_ = STATUS_AUTH;
//
// 	//initialise the fuel and price variables
//	//int keyboard_val = 2000;   //dummy value...
//	//clear pulsercount in the prev. state.....
//
//	 sellPrice_max_dpp = sellPrice_max_dp(dp_amount);
//
//	 if(litre_price1 == 0)
//	 {
//		_litre_price1 = 1;
//		return idle_State;
//	 }
//	 else if ( (change_p == 1) && (auth_p == 0) )
//	 {
//		_auth_p = 1;
//		return idle_State;
//	 }
//	 else if ( (change_v == 1) && (auth_v == 0) )
//	 {
//		_auth_v = 1;
//		return idle_State;
//	 }
//	 else if(opmode == MANUAL_MODE)
//	 {
//		 if(index_ >= 1)
//		 {
//			  key_value = atof(keyboard_entry2);
//			  //initialise the fuel and price variables
//				if (sellmode == P)
//				{
//					  sellPrice_max_pump = (litre_price1 * pump_max_litres1);
//
//					  if( (key_value > sellPrice_max_pump) || (key_value > sellPrice_max_dpp) )
//					  {
//						  if(sellPrice_max_pump < sellPrice_max_dpp)
//						  {
//							  key_value = sellPrice_max_pump;
//							  pump_LitreOverflow = 1;
//						  }
//						  else if(sellPrice_max_pump > sellPrice_max_dpp)
//						  {
//							  key_value = sellPrice_max_dpp;
//						      display_overflow1 = 1;
//						  }
//					  }
//
//					  target_pulser1 = price2pulser(key_value);  //calculate pulse frm price.
//
//				}
//				else  //amt was selected.
//				{
//					if( (key_value > pump_max_litres1) || (key_value > sellPrice_max_dpp) )
//					{
//
//						 if(pump_max_litres1 < sellPrice_max_dpp)
//						 {
//							  key_value = pump_max_litres1;
//							  pump_LitreOverflow = 1;
//						 }
//						 else if(pump_max_litres1 > sellPrice_max_dpp)
//						 {
//							  key_value = sellPrice_max_dpp;
//							  display_overflow1 = 1;
//						 }
//	}
//
//					target_pulser1 = amt2pulser(key_value);   //calculate pulse frm amt.
//				}
//		  }
//		  else
//		  {
////			  key_value = (litre_price1 * pump_max_litres1);
////			  pump_LitreOverflow = 1;
//
//			  key_value = sellPrice_max_dpp;
//		      display_overflow1 = 1;
////			  key_value = 0;
//		  }
//
//	 }
//	 else if(opmode == AUTO)
//	 {
//		  if (change_p == 1)
//		  {
//			  sellPrice_max_pump = (litre_price1 * pump_max_litres1);
//
//			  if( (auth_p > sellPrice_max_pump) || (auth_p > sellPrice_max_dpp) )
//			  {
//				  if(sellPrice_max_pump < sellPrice_max_dpp)
//				  {
//					  auth_p = sellPrice_max_pump;
//					  pump_LitreOverflow = 1;
//				  }
//
//				  else if(sellPrice_max_pump > sellPrice_max_dpp)
//				  {
//					  auth_p = sellPrice_max_dpp;
//				      display_overflow1 = 1;
//				  }
//			  }
//
//			  sprintf(keyboard_entry2,"%2f",auth_p);
//			  key_value = atof(keyboard_entry2);
//			  change_p = 0;      //reset tbe flag.
//			  index_ = strlen(keyboard_entry2);
//
//			  target_pulser1 = price2pulser(key_value);  //calculate pulse frm price.
//		  }
//		 //-----------------------------------------
//		  //authorise volume...
//		  else if (change_v == 1)
//		  {
//			  if( (auth_v > pump_max_litres1) || (key_value > sellPrice_max_dpp) )
//			  {
//				  if(pump_max_litres1 < sellPrice_max_dpp)
//				  {
//					  auth_v = pump_max_litres1;
//					  pump_LitreOverflow = 1;
//				  }
//
//				  else if (pump_max_litres1 > sellPrice_max_dpp)
//				  {
//					  auth_v = sellPrice_max_dpp;
//				      display_overflow1 = 1;
//				  }
//			  }
//
//			  sprintf(keyboard_entry2,"%2f",auth_v);
//			  key_value = atof(keyboard_entry2);
//			  change_v = 0;      //reset tbe flag.
//			  index_ = strlen(keyboard_entry2);
//
//			  target_pulser1 = amt2pulser(key_value);   //calculate pulse frm amt.
//		  }
//
//		  else if (index_ >= 1)
//		  {
//			  	key_value = atof(keyboard_entry2);
//
//			  //initialise the fuel and price variables
//				if (sellmode == P)
//				{
//					  sellPrice_max_pump = (litre_price1 * pump_max_litres1);
//					  if( (key_value > sellPrice_max_pump) || (key_value > sellPrice_max_dpp) )
//					  {
//						  if (sellPrice_max_pump < sellPrice_max_dpp)
//						  {
//							  key_value = sellPrice_max_pump;
//							  pump_LitreOverflow = 1;
//						  }
//
//						  else if (sellPrice_max_pump > sellPrice_max_dpp)
//						  {
//							  key_value = sellPrice_max_dpp;
//						      display_overflow1 = 1;
//						  }
//					  }
//
//					  target_pulser1 = price2pulser(key_value);  //calculate pulse frm price.
//
//				}
//				else  //amt was selected.
//				{
//					if( (key_value > pump_max_litres1) || (key_value > sellPrice_max_dpp) )
//					{
//						if(pump_max_litres1 < sellPrice_max_dpp)
//						{
//							key_value = pump_max_litres1;
//							pump_LitreOverflow = 1;
//						}
//						else if (pump_max_litres1 > sellPrice_max_dpp)
//						{
//							 key_value = sellPrice_max_dpp;
//							 display_overflow1 = 1;
//						}
//					}
//
//					target_pulser1 = amt2pulser(key_value);   //calculate pulse frm amt.
//				}
//		 }
//		 else
//		 {
////			  key_value = (litre_price1 * pump_max_litres1);
////			  pump_LitreOverflow = 1;
//
//			  key_value = sellPrice_max_dpp;
//		      display_overflow1 = 1;
//			  target_pulser1 = price2pulser(key_value);  //calculate pulse frm price.
//		 }
//	   }
////============================================================================
////============================================================================
//	  if (authorise_flag == 1)
//	   {
//		  target_pulser1 = price2pulser(5000);
//		  key_value  = 5000;
//		  authorise_flag == 0;
//	   }
////============================================================================
////============================================================================
//
//	  price  = 0.0;
//		  amt  = 0.0;
//
//		  if (key_value == 0)
//		  {
//			   target_pulser1 = 0;
//		  }
//		  else
//			  pulser_rem1 = target_pulser1 - current_pulser1;
//
//		  if(lock_clr == 0)  ///   to activate this section once.
//		  {
//				 current_pulser1 = 0;
//				 clr_pulser();    //clear hardware pulser
//
//				 //current_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);
//				 lock_clr = 1;
//				 filling1 = 1;
//		  }
//
//		 // int cnv = 0;
//	     //char temp[10] = {0};
//		 //snprintf(temp , sizeof(temp), "%.2f", price);  //send_line22(upper1);
//         //snprintf(middle1, sizeof(middle1), "%.2f", amt);   //send_line22(lower);
//
//	  	  make_string(P,price);
//		  make_string(L,amt);
//
//		 if (t > 300)
//		   {
//				send_line22(upper1);
//				send_line22(middle1);
//
//				char str__[8]= {0};
//				snprintf(str__, sizeof(str_), "%.2f", litre_price);
//				send_line32(str__);
//				t = 0;
//
//							//trigger first pulser...
//
//			#if (_USE_SOFT_PULSER == 1)
//					 //  pulser_new += 1;
//					   current_pulser1++;
//			#else
//					 // pulser_new = __HAL_TIM_GET_COUNTER(&htim5);
//					   current_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);
//			#endif
//
//	 	   }
//
//		       pulser_new = current_pulser1;
////=====================================================================
// 		// filling1 = 1;
//		// send_pump(ACTIVATE);  //pump on...
//
//  if(operating_side == side_a)
//	{
//		working_volTotaliser1 = totaliser_vol1;
//		working_volTotaliser1c = totaliser_vol1c;
//	}
//  else
//   {
//  		working_volTotaliser1 = totaliser_vol2;
//  		working_volTotaliser1c = totaliser_vol2c;
//  	}
//
//  r_volTotaliser1 = floor( working_volTotaliser1c );
//  old_r_volTotaliser1 = r_volTotaliser1;
//
// // slow_flow1();
//
//	return authorised_nozzleup_State;
//}
//



//---------------
eSystemState pause_Handler2(void)
{
   return authorisation_paused_State;
}


//---------------
eSystemState authorisation_paused_State_Handler2(void)
{
   return authorisation_paused_State;
}
//--------------

eSystemState resume_Handler2(void)
{
	return authorised_nozzleup_State;
}

//--------------
eSystemState filling_pulse_Handler2(void)
{
	pump_status_2 = STATUS_FILLING;

	status_change_pump2 = 1;

		//initialise the solenoid and motor...
	if(settings_stream1[1].display_format == PL)
	  {
		 send_line12(upper2);
		 send_line22(middle2);
	  }
	  else if(settings_stream1[1].display_format == LP)
	  {
		  send_line12(middle2);
		  send_line22(upper2);
	  }
	char str__[10]= {0};
		snprintf(str__, sizeof(str__), "%.2f", litre_price2);
		send_line32(str__);

	return filling_State;   //filling_paused_State;
}

//--------------
/*
eSystemState filling_State_Handler(void)
{

}
*/
//---------------
eSystemState filling_paused_Handler2(void)
{

	return filling_paused_State;
}

//--------------
eSystemState filling_paused_State_Handler2(void)
{
       return filling_paused_State;
}

//---------------
eSystemState filling_resumed_Handler2(void)
{

	return filling_State;
}

//----------------------------------------
eSystemState nozzledown_Handler2(void)
{
	filling2 = 0;
	nozzle_bit2 = 0;
	stop_flow2();

	pump2_status_4G = STATUS_NOZZLE_DOWN;

	 if(nozzleup_awaitingauth_state_not_timedOut2 == 1)
	 {
		 nozzleup_awaitingauth_state_not_timedOut2 = 0;
		 clr_screen2();
	 }

	 if(nonValid_sale2 == 1)
	 {
		 return idle_State;
	 }

	//send_line22("nos dn");
	 reset_timer2(30);
	 stop_timer2();

	 if( (eLastState2 == idle_State) && (eNextState2 == idle_State) )
	 {
//		  return idle_State;
	 }
	 else
	 {
		 do_calcs2();
		  get_time();
		  update_info();
//		  save_volumeTotaliser(operating_side);
//		  save_amountTotaliser(operating_side);
//		  save_lastSale(operating_side);

		  save_volumeTotaliser_fram(operating_side);
		  save_amountTotaliser_fram(operating_side);
		  save_lastSale_fram(operating_side);
	 }
/*
		 char str__[8]= {0};
         snprintf(str__, sizeof(str__), " %.2f", litre_price);
		 send_line32(str__);   */

	char str__[10]= {0};
	snprintf(str__, sizeof(str__), "%.2f", litre_price2);
	send_line32(str__);

 	 if( ((pump_LitreOverflow2 == 1) && (pulser_rem2 > 0 )) || (display_overflow2 == 1) )    //|| ((display_overflow2 == 1) && (pulser_rem2 > 0 )) )
 	 {
 		  if(pump_LitreOverflow2 == 1)
 			  pump_LitreOverflow2 = 0;
 		  if(display_overflow2 == 1)
 		  {
 			  display_overflow2 = 0;
 			  firstTime_display_overflow2 = 0;
 		  }

 		 clr_screen2();
 	 }

 	//--------------------------------------------------------
// 		dpFlag2 = 0;
// 		error_clr_flag2 = 1;
// 		 index_2 = 0;
// 		 _index2 = 0;
// 		 for(int i = 0; i < 9; i++)
// 		 {
// 		   keypad_pw_xter2[i] = 0;
// 		   keyboard_entry2[i] = 0;   //clear the buffer
// 		 }
//
// 		 for(int i = 0; i <= 8; i++)
// 		 {
// 			 keyboard2[i] = 0;
// 		 }
// 		 //--------------------------------------------------------------------
// 		 if (sellmode2 == P)
// 		 {
// 			  write_v2(3, "P     0");  //send_keypad("p    ");  //5 xters  lafeng..
// 		 }
// 		 else if(sellmode2 == L)
// 		 {
// 			  write_v2(3, "L     0");  //send_keypad("l    ");  //5 xters lafeng
// 		 }
// 	//	 else if(sellmode2 == V)
// 	//	 {
// 	//		  write_v2(3, "v    0");  //send_keypad("p    ");  //5 xters  lafeng..
// 	//	 }
// 		 send_keypad2(keyboard2);

	 keypad_zerorize2();
 	//--------------------------------------------------------------------

	  //-------------------------------------------
	  //             saving to the log
	  if(eNextState2 == filling_State)
	  {
		  stop_fueling_bit2 = 0;

		  //===========================================================
		  //         for totaliser toggle.
		  //===========================================================

			running_volTotaliser2 = working_volTotaliser2 + amt_real2;

			running_volTotaliser2c = working_volTotaliser2c + amt_middle2;

			running_amtTotaliser2 = working_amtTotaliser2 + price_real2;

			running_amtTotaliser2c = working_amtTotaliser2c + price_upper2;

		   float pricecheck = running_amtTotaliser2c - priceOld2;

		   if (pricecheck >= 1000.00)
		   {
			   priceOld2 = running_amtTotaliser2c;
			   save_amountSend(side_b);

			   char str[65];
			   sprintf(str, "[Side-B]... #%0.2f intermittent worth of sales made now!", pricecheck);
			   server_write(str);
		   }


			r_volTotaliser2 = floor( running_volTotaliser2c );

			if(r_volTotaliser2 != old_r_volTotaliser2)
			{
				totalizer2Timer = 0;
		  //			then toggle the totaliser harware I/O.
				drive_totaliser2(ACTIVATE);
			}
			else
			{
				//deactivate totaliser output...
				if(totalizer2Timer > 200)
				{
					drive_totaliser2(DEACTIVATE);
				}

			}
			old_r_volTotaliser2 = r_volTotaliser2;   //update...

		 //============================================================

			if(settings_stream1[1].mode == AUTO_MODE)
			{

				 if( (pump_status_2 == STATUS_FILLING) || (pump_status_2 == STATUS_MAMO_REACHED) )
				 {

				 }

				 //////////////////////////////////////////////////////////////
				 ///////// SIGNALS GO-CONTROLLER ABOUT NOZZLE STATUS //////////

				 status_change_noz2 = 1;
				 nozzle_out2 = false;

				 nozzleDown_source2 = 1;

				 //////////////////////////////////////////////////////////////

			}

			return  write_flash_State;
		 }

	  //-------------------------------------------

		  pump_status_2 = STATUS_FILLING_COMP;

	      return idle_State;
}

//----------------------------------------
eSystemState timeout_Handler2(void)
{
	 if(eLastState2 != authorised_nozzleup_State)
	 {
		 send_line12(" t out  ");
		 send_line22("--------");

		 if( ((pump_LitreOverflow2 == 1) && (pulser_rem2 > 0 )) || ((display_overflow2 == 1) && (pulser_rem2 > 0 )) )
		 {
			  if(pump_LitreOverflow2 == 1)
				  pump_LitreOverflow2 = 0;
			  if(display_overflow2 == 1)
				  display_overflow2 = 0;
		 }
	 }

	  //write the commodity price...
	  // char str__[8]= {0};
	  // snprintf(str__, sizeof(str__), " %.2f", litre_price);
	  // send_line32(str__);
	  // send_line32("-----");

	  //compose the kind of timeout error
	  else if(eLastState2 == nozzleup_waitingforauth_State)
	  {
		 send_line32("Err1 ");
		 nozzleup_awaitingauth_state_not_timedOut2 = 0;
	  }

	  else if(eLastState2 == authorised_nozzledown_State)
	  {
		 send_line32("Err2 ");
	  }

	  else if(eLastState2 == authorisation_paused_State)
	  {
		 send_line32("Err3 ");
	  }

	  else if(eLastState2 == filling_paused_State)
	  {
		 send_line32("Err4 ");
	  }

	  else if(eLastState2 == authorised_nozzleup_State)
	  {
		 if( ((pump_LitreOverflow2 == 1) && (pulser_rem2 > 0 )) || ((display_overflow2 == 1) && (pulser_rem2 > 0 )) )
		 {
			  if(pump_LitreOverflow2 == 1)
				  pump_LitreOverflow2 = 0;
			  if(display_overflow2 == 1)
				  display_overflow2 = 0;
		 }

		 send_line12("    No ");
		 send_line22("  Flouu ");
		 send_line32("Err15 ");
	 }

	 pump_status_2 = STATUS_FILLING_COMP;

	 return inactive_State;
}


//----------------------------------------
eSystemState tot_error_Handler2(void)
{
   //set irrecoverable error flag.
//	irrecov_flag2 = 1;

	totalizer2_error = 1;

	send_line12("   tot   ");
	send_line22("  Error ");

    return inactive_State;
}

//----------------------------------------
eSystemState key19_Handler2(void)
{
    return idle_State;
}

//----------------------------------------
eSystemState keypress_Handler2(void)
{

	static allowed_xters = 6;
	extern uint8_t error_clr_flag2;
	extern uint8_t keyEntry2_len;

//	 if(pump_type == lafeng)
//	 if(pump_type == DN_LAFNG17K)
	 if(settings_stream1[1].keypad__ == LAFNG17_K)
	 {
	   kkey2 =  lafeng_keypad[keypress_2];
	   allowed_xters = 6;
	 }
	 else if (settings_stream1[1].keypad__ == LAFNG18_K)
	 {
	   kkey2 =  lafeng_keypad_18K[keypress_2];
	   allowed_xters = 7;
	 }
//	 else if(pump_type == bluesky)
//	 else if( (pump_type == DN_BLSKY18K) || (pump_type == DN_BLSKY22) )
	 else if( (settings_stream1[1].keypad__ == BLSKY18_K) || (settings_stream1[1].keypad__ == BLSKY22) )
	 {
	    kkey2 =  bluesky_keypad[keypress_2];
	    allowed_xters = 7;
	 }

   ePrevState2 = eLastState2;

 /*  if (progg == 1)
	 {
	    return keypad_entry_State;
	 }  */

	//if (progg == 1) //if in programming mode return.

	//char prc[7] = {'p'};
	//char amt[7] = {'l'};
	//char keyboard[6] = {0};
	//char keyboard_screen[6] = {0};

   int space = allowed_xters - index_2; //int space = 6 - index_;
	int ind = 1;

//-----------------------------------------------------------
//If only 0 - 9 is pressed.....
if(
	(kkey2 != 'A') && (kkey2 != 'B') &&
	(kkey2 != 'C') && (kkey2 != 'D') &&
	(kkey2 != 'F') && (kkey2 != '-') &&
	(kkey2 != 'G') && (kkey2 != 'J')  //&&(index_2 < 6)
  )
    {
	 if(index_2 <= allowed_xters)    //only allow 6 xters...
	  {
		keyboard_entry2[index_2] = kkey2;
		keyboard_entry2[index_2+1] = 0;         //NULL;
		index_2++;
	  }
	}
//-----------------------------------------------------------
		  if (kkey2 == 'F')      //'clear' key.
		  	{
			  //ePrevState = eLastState1;
			  //set the error clear flag..
			    dpFlag2 = 0;
			    error_clr_flag2 = 1;
                 index_2 = 0;
                 _index2 = 0;
                 for(int i = 0;i<9;i++)
				 {
				   keypad_pw_xter2[i] = 0;
				   upper2[i] = 0;
				   middle2[i] = 0;
				 }

                 for(int i = 0; i <= 6; i++)
				 {
					 keyboard_entry2[i] = 0;   //clear the buffer
					 keyboard2[i]= 0;
				 }

                 if(settings_stream1[1].display_format == PL)
				  {
					 upper2[0]  = 'P';
					 middle2[0] = 'L';
				  }
				  else if(settings_stream1[1].display_format == LP)
				  {
					 upper2[0]  = 'L';
					 middle2[0] = 'P';
				  }
		  	}
//--------------------------------------------------------------------
		  if (progg2 == 1)
		  	 {
			    send_keypad2("       ");
			    if (auth2 == not_auth)
			    {
//			    	if(keypad_pw_xter2[0] != '\0')
			    		send_keypad2(keypad_pw_xter2);
			    }
			    else
			    {
					 int8_t keyBoard_len = strlen(keyboard_entry2);
					 strncpy(keyboard2, keyboard_entry2, sizeof(keyboard2));
					 if(strchr(keyboard2, '.') )
					 {
						 if(keyboard2[keyBoard_len - 1] == '.')
						 {
							 keyboard2[keyBoard_len] = '0';

							 if(keyboard2[0] == '.')
							 {
								 keyboard2[0] = '0';
								 keyboard2[1] = '.';
								 keyboard2[2] = '0';
							 }

						 }
					 }
					 send_keypad2(keyboard2);
				}

		  	    return keypad_entry_State;
		  	 }

      if ( (kkey2 == 'D')&&(progg2 == 0) ) //fueling key.
	 	{
       	   //ePrevState = eLastState1;
           auth_cmd_flag2 = 1;  //activate auth2 cmd.

           return keypad_entry_State;
	 	}

      if ( (kkey2 == 'A')&&(progg2 == 0)&&( (eNextState2 == filling_State) || (eNextState2 == authorised_nozzleup_State)) ) //stop sales.
		{
		   stop_flag2 = 1;  //activate auth2 cmd.
		}

//====================================================
 if ( (kkey2 == 'C')&&(progg2 == 0) )  //if change sales mode
	{
	   if(sellmode2 == L)
	   {
	       sellmode2 = P;
	   }
	   else
	   {
		   sellmode2 = L;
	   }
	}
//--------------------------------------------------------------------
 int8_t space2,
 	 	b;

 keyEntry2_len = strlen(keyboard_entry2);
 if( (strchr(keyboard_entry2, '.')) && (keyboard_entry2[keyEntry2_len - 1] != '.') )
 {
	 space2 = 7 + ( -index_2 + 1);
	 dpFlag2 = 1;

	 b = 0;
	 dpCount2 = 0;
	 while( *(keyboard_entry2 + b) != '\0')
	 {
		 if( (keyboard_entry2[b] == '.') || (dpCount2 != 0) )
		 {
			dpCount2++;
		 }
		 b++;
	 }
	 dpCount2--;
  }
  else
  {
	 space2 = 7 - index_2;     //6 - index_;
  }

// int space2 = 7 - index_2;   //6 - index_2;
 if ((sellmode2 == L)&&(progg2 == 0) )
 {
	   memset(middle2, '\0', sizeof(middle2));
	   middle2[0] = 'L';     //Append price to display.
	// if((index_-1) >= 6) space = 2;
		while (space2 > 0) //write spaces first..
		{
		   middle2[ind++] = ' ';
		   space2--;
		}
	  space2 = index_2;     //reload with len of actual number
	   while(space2 > 0)
		 {
			 middle2[ind++] = keyboard_entry2[index_2 - space2];
			 space2--;
		 }
	  //  send_line22("p       ");

	  write_v2(1,"p       ");

	  //  send_line22(middle1);
   }
//--------------------------------------------------------------------

       if ( (sellmode2 == P)&&(progg2 == 0) )
	   {
    	   memset(upper2, '\0', sizeof(upper2));
    	   upper2[0] = 'p';       // append price to display
 		// if((index_-1) >= 6) space = 2;
 		   while (space2 > 0)     //write spaces first..
 		   {
 			   upper2[ind++] = ' ';
 			   space2--;
 		   }
 		 space2 = index_2 ;          //reload with len of actual number
 		    while(space2 > 0)
 			 {
 				 upper2[ind++] = keyboard_entry2[index_2 - space2];
 				 space2--;
 			 }
 			//send_line22(upper1);  //clear the price
 			//send_line22("l       ");
 		   write_v2(2,"l       ");
         }
//----------------------------------------------------------------
//                            write to the keypad
	   static int lcd_size = 5;

//	   if(disp_type2 == LAFNG885)
//	   if(disp_type2 == DN_LAFNG17K)
	   if(settings_stream1[1].keypad__  == LAFNG17_K)
	   {
		    lcd_size = 5; //change this latter to accomodate other lcds.
	   }
	   else if(settings_stream1[1].keypad__  == LAFNG18_K)
	   {
		    lcd_size = 7; //change this latter to accomodate other lcds.
	   }
//	   else if(disp_type2 == BLSKY886_N)
//	   else if( (disp_type2 == DN_BLSKY18K) ||  (disp_type2 == DN_BLSKY22) )
	   else if( (settings_stream1[1].keypad__  == BLSKY18_K) || (settings_stream1[1].keypad__  == BLSKY22) )
	   {
		    lcd_size = 7;   //6;
	   }
//================================================================
 int8_t size = lcd_size - 1;   // 1 xter to display 'P/L'

 int8_t k_index = 0;
 ind = 1;

	  if (sellmode2 == P)
	  {
		 keyboard2[0] = 'p';
		 if( (index_2 <= size) || ((index_2 <= 6) && (dpFlag2 == 1)) )
		 {
			if(dpFlag2 == 1)
			{
				space = size + (-index_2 + 1);
//		    		dpFlag = 0;
			}
			else
			{
				space = size - index_2;
			}
		 }
		 else
			 space = 0;  //full size of xters.

		 while (space > 0) //write spaces first..
		   {
			 keyboard2[ind++] = ' ';
			   space--;
		   }
	  //---------------------------------
		if( (index_2 <= size) || ((index_2 <= 6) && (dpFlag2 == 1)) )
			  space = index_2;
		else
		{
			if(dpFlag2 == 1)
			{
				space = 6;
			}
			else
				space = size;
		}

		if( (strchr(keyboard_entry2, '.')) && (index_2 > 6) ) space = 7;
		while(space > 0)
		 {
		   keyboard2[ind++] = keyboard_entry2[index_2 - space];
		   space--;
		 }
		if(keyboard2[ind - 1] == '.') keyboard2[ind] = '0';

	   }

	  //	  	  	 keyboard2[0] = 'p';
//		     if(index_2 <= size)
//		    	   space = size - index_2;
//		     else
//		    	   space = 0;  //full size of xters.
//
//		      while (space > 0) //write spaces first..
//			   {
//		    	 keyboard2[ind++] = ' ';
//				   space--;
//			   }
//            //---------------------------------
//				if(index_2 <= size)
//					  space = index_2;
//				else
//					  space = size;
//
//			    while(space > 0)
//				 {
//				   keyboard2[ind++] = keyboard_entry2[index_2 - space];
//				   space--;
//				 }
//
//	 	   }
//----------------------------------------------------------------
	  if (sellmode2 == L)
	  {

		 keyboard2[0] = 'L';
		 if( (index_2 <= size) || ((index_2 <= 6) && (dpFlag2 == 1)) )
		 {
			if(dpFlag2 == 1)
			{
				space = size + (-index_2 + 1);
 //		    		dpFlag = 0;
			}
			else
			{
				space = size - index_2;
			}
		 }
		 else
			 space = 0;  //full size of xters.

		 while (space > 0) //write spaces first..
		   {
			 keyboard2[ind++] = ' ';
			   space--;
		   }
		  //---------------------------------
			if( (index_2 <= size) || ((index_2 <= 6) && (dpFlag2 == 1)) )
				  space = index_2;
			else
			{
				if(dpFlag2 == 1)
				{
					space = 6;
				}
				else
					space = size;
			}

			if( (strchr(keyboard_entry2, '.')) && (index_2 > 6) ) space = 7;
			while(space > 0)
			 {
			   keyboard2[ind++] = keyboard_entry2[index_2 - space];
			   space--;
			 }
			if(keyboard2[ind - 1] == '.') keyboard2[ind] = '0';

		    }


//		  keyboard2[0] = 'l';
//		 if(index_2 <= size)
//			   space = size - index_2;
//		 else
//			   space = 0;  //full size of xters.
//
//		 while (space > 0) //write spaces first..
//		   {
//			 keyboard2[ind++] = ' ';
//			   space--;
//		   }
//	   //---------------------------------
//			if(index_2 <= size)
//				  space = index_2;
//			else
//				  space = size;
//
//			while(space > 0)
//			 {
//			   keyboard2[ind++] = keyboard_entry2[index_2 - space];
//			   space--;
//			 }
//	   }

	           if (progg2 == 1)
	 		  	 {
	 			    send_keypad2("      ");
	 			    if (auth2 == not_auth)
	 			    	send_keypad2(keypad_pw_xter2);
	 			    else
				    {
						 int8_t keyBoard_len = strlen(keyboard_entry2);
						 strncpy(keyboard2, keyboard_entry2, sizeof(keyboard2));
						 if(strchr(keyboard2, '.') )
						 {
							 if(keyboard2[keyBoard_len - 1] == '.')
							 {
								 keyboard2[keyBoard_len] = '0';
							 }
						 }
						 send_keypad2(keyboard2);
					}
	 		  	    return keypad_entry_State;
	 		  	 }

	             send_keypad2(keyboard2);
//----------------------------------------------------------------
      //send_keypad(keyboard);

         return keypad_entry_State;
}
//---------------------------------------------------------------------

eSystemState keypad_entry_State_Handler2(void)
{

    return  ePrevState2; //
	//return keypad_entry_State;
}

//----------------------------------------

//--------------------------------------------------------------
void do_calcs2 ()
{

 float temp;
 if(target_pulser2 > 0)  // if price or volume is programmed
 {
   if (pulser_rem2 <= 0 || pulser_complete2 == 1 )  //sales complete...
	 {
	   //get_time2();

	    if (sellmode2 == P)
	   	   {

	       //---------------------------------------------------------
	    	//price = dp(key_value,dp_price);   // temp = amt2price(amt);
	    	//amt   = dp( price_/litre_price ,dp_amount);

	    	 temp = pulser2amt2(current_pulser2);
	    		    	 amt2 = dp2(temp, dp_vol2);

			 temp = amt2price2(amt2);
						 price2 = dp2(temp, dp_amount2);

	   			///show what the user needs...
	   		 price_2 = dp2(key_value2, dp_amount2);
	   		 	 temp = price_2/litre_price2;
	   		 amt_2   = dp2(temp, dp_vol2); //calculate vol frm price.

	 		//=========================================================
	 		//    Also calculate the values based on the real P.Indx
	 		          temp  = pulser2amt_R2(current_pulser2);
	 		      amt_real2 = dp2(temp, dp_vol2);
	 		          temp = amt2price2(amt_real2);
	 		      price_real2 = dp2(temp, dp_amount2);
	 		//=========================================================
	   	   }

	   	  if (sellmode2 == L)
	   	   {
	   		    temp = pulser2amt2(target_pulser2);
	   		    amt2  = dp2(temp, dp_vol2);

	   			price2  = amt2price2(amt2);
	   			///show what the user needs to see...
	   			amt_2   = key_value2;
	   			price_2 = amt_2 * litre_price2; //calculate price from price.amt
		 		//=========================================================
		 		//    Also calculate the values based on the real P.Indx
		 		          temp  = pulser2amt_R2(current_pulser2);
		 		      amt_real2 = dp2(temp, dp_vol2);
		 		          temp = amt2price2(amt_real2);
		 		      price_real2 = dp2(temp, dp_amount2);
		 		//=========================================================
	   	   }
	   	 //------------------------------------------------------------------
	   	  make_string2(P, dp2(price_2, dp_amount2));
	   	  make_string2(L, dp2(amt_2, dp_vol2));

	   	if(settings_stream1[1].display_format == PL)
		  {
			 send_line12(upper2);
			 send_line22(middle2);
		  }
		  else if(settings_stream1[1].display_format == LP)
		  {
			  send_line12(middle2);
			  send_line22(upper2);
	   	  }

	   	  running_volTotaliser2 = working_volTotaliser2   + amt_real2;
//	   	  running_volTotaliser2c = working_volTotaliser2c + amt2;

	   	  running_volTotaliser2c = working_volTotaliser2c + amt_middle2;

		  running_amtTotaliser2  = working_amtTotaliser2  + price_real2;
//		  running_amtTotaliser2c = working_amtTotaliser2c + price2;

		  running_amtTotaliser2c = working_amtTotaliser2c + price_upper2;

		  float pricecheck = running_amtTotaliser2c - priceOld2;

		   if (pricecheck >= 1000.00)
		   {
			   priceOld2 = running_amtTotaliser2c;
			   save_amountSend(side_b);

			   char str[65];
			   sprintf(str, "[Side-B]... #%0.2f intermittent worth of sales made now!", pricecheck);
			   server_write(str);
		   }

		   totaliser_vol2 = running_volTotaliser2;   // update totaliser
		   totaliser_vol2c = running_volTotaliser2c; // update totaliser

		   totaliser_amt2 = running_amtTotaliser2;   // update totaliser
		   totaliser_amt2c = running_amtTotaliser2c; // update totaliser

	   	 //-------------------------------------------------------------------

	   //============================================================

			   //         for totaliser toggle.

				r_volTotaliser2 	  = floor( running_volTotaliser2c );

				if(r_volTotaliser2 != old_r_volTotaliser2)
				{
					totalizer2Timer = 0;

			  //			then toggle the totaliser harware I/O.

					drive_totaliser2(ACTIVATE);
				}
				else
				{
					//deactivate totaliser output...

					if(totalizer2Timer > 200)
					{
						drive_totaliser2(DEACTIVATE);
					}

				}
				  old_r_volTotaliser2 = r_volTotaliser2;   //update...

			//-------------------------------------------------------------------

	   	   return;
	 }
   else
   {
      //programmed but still dispensing @ stop pt.
		 temp = pulser2amt2(current_pulser2);  amt2   = dp2(temp, dp_vol2);
		 temp = amt2price2(amt2);   			price2 = dp2(temp, dp_amount2);

		  make_string2(P, dp2(price2, dp_amount2));
		  make_string2(L, dp2(amt2, dp_vol2));

	    //=========================================================
		//    Also calculate the values based on the real P.Indx
			          temp  = pulser2amt_R2(current_pulser2);
			      amt_real2 = dp2(temp, dp_vol2);
			          temp = amt2price2(amt_real2);
			      price_real2 = dp2(temp, dp_amount2);
		//=========================================================

		  if(settings_stream1[1].display_format == PL)
		  {
			 send_line12(upper2);
			 send_line22(middle2);
		  }
		  else if(settings_stream1[1].display_format == LP)
		  {
			  send_line12(middle2);
			  send_line22(upper2);
		   }

	   	  running_volTotaliser2 = working_volTotaliser2   + amt_real2;
//	   	  running_volTotaliser2c = working_volTotaliser2c + amt2;

	   	  running_volTotaliser2c = working_volTotaliser2c + amt_middle2;

		  running_amtTotaliser2  = working_amtTotaliser2  + price_real2;
//		  running_amtTotaliser2c = working_amtTotaliser2c + price2;

		  running_amtTotaliser2c = working_amtTotaliser2c + price_upper2;

		  float pricecheck = running_amtTotaliser2c - priceOld2;

		   if (pricecheck >= 1000.00)
		   {
			   priceOld2 = running_amtTotaliser2c;
			   save_amountSend(side_b);

			   char str[65];
			   sprintf(str, "[Side-B]... #%0.2f intermittent worth of sales made now!", pricecheck);
			   server_write(str);
		   }

		   totaliser_vol2 = running_volTotaliser2;   // update totaliser
		   totaliser_vol2c = running_volTotaliser2c; // update totaliser
		   totaliser_amt2 = running_amtTotaliser2;    // update totaliser
		   totaliser_amt2c = running_amtTotaliser2c;  // update totaliser


		 //-------------------------------------------------------------------
		   return;
   }
 }
 else   // if target_pulser1 == 0
 {
	 //if un programmed...
     //still dispensing @ stop pt.

		 temp = pulser2amt2(current_pulser2);
		      amt2 = dp2(temp, dp_vol2);

		 temp = amt2price2(amt2);
		 	 price2 = dp2(temp, dp_amount2);

		//compare the final and initial prices...
		 	 float ddp = 0;
		 	 if (dp_amount2 == 1) ddp =  0.1;
			 else if (dp_amount2 == 2) ddp =  0.01;
			 else if (dp_amount2 == 3) ddp =  0.001;

		 	 if( (temp - price2) > ( (ddp/10)*2) )  //result was rounded down.
		 	 {

		 		 //need to do a little fixing here...
		 		 price2 = price2 + ddp;
		 	 }

		  make_string2(P, price2); //,dp_price)
		  make_string2(L, amt2);  //,dp_amount)

		//=========================================================
		//    Also calculate the values based on the real P.Indx
		          temp  = pulser2amt_R2(current_pulser2);
		      amt_real2 = dp2(temp, dp_vol2);
		          temp = amt2price2(amt_real2);
		      price_real2 = dp2(temp, dp_amount2);
		//=========================================================

//		  send_line12(upper2);
//		  send_line22(middle2);

		  if(settings_stream1[1].display_format == PL)
		  {
			 send_line12(upper2);
			 send_line22(middle2);
		  }
		  else if(settings_stream1[1].display_format == LP)
		  {
			  send_line12(middle2);
			  send_line22(upper2);
		  }

	   	  running_volTotaliser2 = working_volTotaliser2   + amt_real2;
//	   	  running_volTotaliser2c = working_volTotaliser2c + amt2;

	   	  running_volTotaliser2c = working_volTotaliser2c + amt_middle2;

		  running_amtTotaliser2  = working_amtTotaliser2  + price_real2;
//		  running_amtTotaliser2c = working_amtTotaliser2c + price2;

		  running_amtTotaliser2c = working_amtTotaliser2c + price_upper2;

		  float pricecheck = running_amtTotaliser2c - priceOld2;

		   if (pricecheck >= 1000.00)
		   {
			   priceOld2 = running_amtTotaliser2c;
			   save_amountSend(side_b);

			   char str[65];
			   sprintf(str, "[Side-B]... #%0.2f intermittent worth of sales made now!", pricecheck);
			   server_write(str);
		   }

		   totaliser_vol2 = running_volTotaliser2;   // update totaliser
		   totaliser_vol2c = running_volTotaliser2c; // update totaliser
		   totaliser_amt2 = running_amtTotaliser2;    // update totaliser
		   totaliser_amt2c = running_amtTotaliser2c;  // update totaliser

		 //-------------------------------------------------------------------
		   return;
     }
  return;
}

//----------------------------------------
//
//
//
eSystemState filling_paused_state_Handler2(void)
{

    return filling_paused_State;
}




//-----------------------------------------
void state_ini2(void)
{
	float temp = 0.0;

	irrecov_flag2 = 0;
	progg2 = 0;

	pump_status_2 = STATUS_PNP;

	if(settings_stream1[1].mode != AUTO_MODE)
	{
		eNextState2 =  idle_State;
		eLastState2 =  idle_State;
	}
	else
	{
		eNextState2 =  pnp_State;
		eLastState2 =  pnp_State;
	}

	index_2 = 0;

//	filling2 = 0;
//	stop_flow2();
	if(settings_stream1[1].display_format == PL)
	{
		send_line12("P        ");
		write_v2(1, "p        ");
	}
	else if(settings_stream1[1].display_format == LP)
	{
		send_line12("L        ");
		write_v2(1, "l        ");
	}

//	write_v(2, "l        ");
	if(settings_stream1[1].display_format == PL)
	{
		send_line22("L        ");
		write_v2(2, "l        ");

//	    make_string2(P, dp2(lastAmountSale2c, dp_price2) );
//	    make_string2(L, dp2(lastVolumeSale2c, dp_amount2) );
	}
	else if(settings_stream1[1].display_format == LP)
	{
		send_line22("P        ");
		write_v2(2, "p        ");
	}


    make_string2(P, dp2(lastAmountSale2c, dp_amount2) );
    make_string2(L, dp2(lastVolumeSale2c, dp_vol2) );

	char str__[10]= {0};
	snprintf(str__, sizeof(str__), "%.2f", litre_price2);
	send_line32(str__);

	 if(sellmode2 == L)
	   {
		  write_v2(3, "L     0");  //send_keypad("l    ");  //5 xters lafeng
	   }
	   else
	   {
		  write_v2(3, "P     0");  //send_keypad("p    ");  //5 xters  lafeng..
	   }
	 send_keypad2(keyboard2);
}

void states2(void)
{
	eNewEvent2 = read_event2();
	if (eNewEvent2 != _keypress_Event)
	{
        //ePrevState = eNextState1;
	}

	eSystemEvent ev2;
	  if( (eNewEvent2 < _no_Event)  )  //if event occured, check if the current state is sensitive to it..
	  {
		  eLastState2 = eNextState2;   // store state...
           if(eNewEvent2   == _auth_command_Event) //_nozzleup_Event) //_filling_pulse_Event  _nozzleup_Event)
            {
           	  //int yiuyu = 0;
            }
          //...   scan through the allowed  events of the state if its among them..
	    for (int i = 0; i < max_events_per_state; i++)
		 {
		   ev2 = (asStateEventMachine2[eNextState2].states2[i]);  //
		   if (ev2 == 0)
			    {
			       // eNextState1 = eLastState1;   //stay in the same event..
			        break;  // we reached end of event in allowed events array, exit scan..
			    }
		   if (eNewEvent2 == ev2) // if the just triggered event is among allowed events..
		   {
                //then check if it has an handler and switch to the event Handler...
			   if (asEventMachine2[ev2-1].pfEventMachineHandler2 == NULL) //no handler supplied
			   {
				   break; //eNextState1 = (*asStateEventMachine[eNextState1].pfStateMachineHandler)(); //switch to state handler.
			   }
			      else
			   {
				   eNextState2 = (*asEventMachine2[eNewEvent2-1].pfEventMachineHandler2)();  //switch to event handler.
				   eNewEvent2 = _no_Event;
				   // int ty = 0;

			   }
		   } // if (eNewEvent1 == ev)
		 }  ///for

	     eNextState2 = (*asStateEventMachine2[eNextState2].pfStateMachineHandler2)(); //switch to state handler.
	    // int yy = 56;
	  }
	 else
	  {
		//int s0 = (asStateEventMachine[eNextState1].states[0]);
		//int s1 = (asStateEventMachine[eNextState1].states[1]);
		//int s2 = (asStateEventMachine[eNextState1].states[2]);

		  // no event, switch to same state...
		  eNextState2 = (*asStateEventMachine2[eNextState2].pfStateMachineHandler2)();  //
					  //  (*asStateEventMachine[eNextState1].pfStateMachineHandler)();
		 // int yui = 0;
		  return;
	  }


	  /*
	if((eNextState1 < last_State) && (eNewEvent1 < last_Event)&& (asStateMachine[eNextState1].eStateMachineEvent == eNewEvent1) && (asStateMachine[eNextState1].pfStateMachineEvnentHandler != NULL))
		{
			// function call as per the state and event and return the next state of the finite state machine
			eNextState1 = (*asStateMachine[eNextState1].pfStateMachineEvnentHandler)();
		}
		else
		{

		}  */
	  //eNextState1 = (*asStateEventMachine[eNextState1].pfStateMachineHandler)();
	 // return;
}


void save_date2(int8_t _entri)
{
	 ddate2[0] = _entri;
	 clear_buffer2();
	 mth_success2 = false;
	 clock_save2 = true;
}

//========= Leap-Year Calculation ============//
bool leapYear_calc2(int8_t year)
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


uint16_t get_ctTime2(void)
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



//----------------------------------------
eSystemState filledmamo_State_Handler2(void)
{
	filling2 = 0,
	nozzle_bit2 = 0;


	  if (t2 > LCD_UPDATE_RATE)
	  {

		  if(settings_stream1[1].display_format == PL)
		  {
			 send_line12(upper2);
			 send_line22(middle2);
		  }
		  else if(settings_stream1[1].display_format == LP)
		  {
			  send_line12(middle2);
			  send_line22(upper2);
		  }
		  char str__[8]= {0};
		  snprintf(str__, sizeof(str__), "%.2f", litre_price2);
		  send_line32(str__);
		  t2 = 0;
	  }

//	  reset_timer(timeout_dispense); //don't time out.

	   return filledmamo_State;
}

//----------------------------------------
eSystemState pnpState_Handler2(void)
{
	static int  printer_status;

	static int8_t idleState_flag = 1;

	static int32_t current_pulser_ = 0,
				   old_pulser_ = 0;

	uint16_t gerCtTime;

	int pulser_diff = 0;

//	display_overflow1 = 1;

//	pump1_status_4G = STATUS_IDLE;

	stop_fueling_bit2 = 1;

	lock_clr2 = 0;

	progg2 = 0;

	firstTime_nozz2 = 1;


	#if sense_power == 1
	  if(readpwr() == 0)
	  {
		   modem_power(DEACTIVATE);

		   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
		   HAL_Delay(200);
		   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);

		   //count time elapsed
		   if (shutdown_timer2 > 120)
		   {
			   displayandkeypad_power(DEACTIVATE);   //shutdown... after  2 minutes
		   }
	  }
	  else
	  {
		  //clear the timer//
		  shutdown_timer2 = 0;

		  modem_power(ACTIVATE);
		  displayandkeypad_power(ACTIVATE);
	  }
	#endif

	#if !defined (DEV_MODE)
		if(batteryStatus == LOWBATTERY)
		{
			send_line12("  Louu   ");
			send_line22("Battery  ");
			send_line32(" Err70 ");

			return inactive_State;
		}
		else if(batteryStatus == NOBATTERY)
		{
			send_line12("Battery ");
			send_line22(" Error  ");
			send_line32(" Err71 ");

			return inactive_State;
		}

		if(HAL_GPIO_ReadPin(pulser2_detect_GPIO_Port, pulser2_detect_Pin) == 1 )
		{
			send_line12(" Pulser ");
			send_line22("  Error ");
			send_line32(" Err24 ");

			return inactive_State;
		}
		else
		{

		}

	//	if(calib_pulser1 < 15800)  //15987, 15967 .... 1106247681
		if(calibration_flag2 != CALIBRATED) //15800)  //15987, 15967 .... 1106247681
		{
			retrieve_calibrationFlag(side_b);

			if(calibration_flag2 != CALIBRATED) //takes care of accidental clearing of calibration_flag1 by F-keys
			{
				calibration2_error = 1;

				send_line12("Calibrat");
				send_line22("  Error ");
				send_line32("Err 23 ");

				return inactive_State;
			}
		}
		else
		{
			calibration2_error = 0;
		}
	#endif    //#if !defined (DEV_MODE) ,,



	 if  (t2 > 500)
	 {
//		 send_line1(upper1);
//		 send_line2(middle1);
		 if(settings_stream1[1].display_format == PL)
		  {
//			 send_line1(upper1);
//			 send_line2(middle1);
		  }
		  else if(settings_stream1[1].display_format == LP)
		  {
//			  send_line1(middle1);
//			  send_line2(upper1);
		  }
		 send_line32("      ");
//		 char str__[8]= {0};
//		 snprintf(str__, sizeof(str__), "%.2f", litre_price); send_line3(str__);
		 t2 = 0;
	 }


	 if( (keypress_2 == 21) && (nozzleup_awaitingauth_state_not_timedOut2 == 0) )
	 {
		   compose_printer();
		   HAL_Delay(1000);
		   printer_status = 1;
	 }

	     print__1();   //print the transaction.

	  	return pnp_State;
}



//----------------------------------------
eSystemState switchedoffState_Handler2(void)
{
	return switchedoff_State;
}

//----------------------------------------
eSystemState authorisation_resumed_State_Handler2(void)
{
	//Motor turned on
	return _authorisation_resumed_State_Handler;
}

//----------------------------------------
eSystemState filling_resumed_State_Handler2(void)
{
	//Motor turned on
	return _filling_resumed_State_Handler;
}


void keypad_zerorize2(void)
{
	dpFlag2 = 0;
	error_clr_flag2 = 1;
	index_2 = 0;
	_index2 = 0;

	 for(uint8_t i = 0; i < 9; i++)
	 {
	   keypad_pw_xter2[i] = 0;
	   keyboard_entry2[i] = 0;   //clear the buffer
	 }

	 for(uint8_t i = 0; i <= 8; i++)
	 {
		 keyboard2[i] = 0;
	 }
	 //--------------------------------------------------------------------
	 if (sellmode2 == P)
	 {
		  write_v2(3, "P     0");  //send_keypad("p    ");  //5 xters  lafeng..
	 }
	 else if(sellmode2 == L)
	 {
		  write_v2(3, "L     0");  //send_keypad("l    ");  //5 xters lafeng
	 }
//	 else if(sellmode2 == V)
//	 {
//		  write_v2(3, "v    0");  //send_keypad("p    ");  //5 xters  lafeng..
//	 }
	 send_keypad2(keyboard2);
}

