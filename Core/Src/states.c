/*
 * states.c
 *
 *  Created on: Mar 7, 2022
 *      Author: tunji
 */

#include "command.h"
#include "thread1.h"
#include "settings.h"
#include "states.h"
#include "pump_comm.h"

#include "stdbool.h"
#include "string.h"
#include "stddef.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

#include "myLibraries.h"

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

//extern log_new1 log_a_new1 , log_b_new1;

extern uint32_t flash_read_idA;
extern uint32_t flash_read_idB;

extern const uint32_t flash_beginA;
extern const uint32_t flash_endA;

extern const uint32_t flash_beginB;
extern const uint32_t flash_endB;

extern uint8_t prog_entry1;

extern ConfigChange configChange[2];

int8_t index_menu = 0;

uint8_t dummyValue = 0;

static bool mth_success = false,
			clock_save = false;

uint8_t  prog_revisit1 = 1,
		 prog_revisitt1 = 1,
		 firstTime_key19 = 1;

uint8_t key19State = 0,
		lastKey19State = 0;


int data_size = 0; //w25qxx.PageSize;    //0;

uint16_t countar = 0,
		 countar2 = 0;

float price_dp;

extern float temppp_ = 0.000;


extern uint16_t otp_seed1 = 0;

eSystemState eNextState1_1;
eSystemEvent eNewEvent1_1;
uint8_t dummyData = 0;

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

extern pump_status_enum pump_status_1;
extern pump_status_enum_4G pump_status_4G;

extern log_new log_a_new,log_b_new;
extern uint8_t hour,minute,second,date, day, month,year,dayofweek;

extern float price;
extern float amt;
extern float litre_price;
extern float pulser_index;
extern float pulser_index_c ;
extern float litre_price1;
extern float litre_price2;

extern uint8_t calibration_flag1 = 0,
			   configMode1 = 0;

extern uint8_t online_calibFlag1 = 0;

extern uint8_t pwr1 = NOPOWERINTERRUPTION;

extern float price_upper1 = 0.000,
			 amt_middle1 = 0.000;

float display_minimumPulser = 0.00;

extern uint8_t ep1a_priceChangeFlag1 = 0;

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

extern int8_t ttime[3],
			  ddate[4];

uint32_t target_pulser1,
		 current_pulser1 = 0,
		 overall_currentPulser1 = 0;

uint8_t fastFlow1 = 0;

extern int timeout_picknozzle;
extern int timeout_dispense;

static int time_e ;
static int st = 0;
int timer_flag = 0;
int timer_flag_old = 0;

int tot_buttonpress_tmr = 0;
int log_buttonpress_tmr = 0;
int key_buttonpress_tmr = 0,
	progExit_buttonpress_tmr1 = 0;

extern  int t;
extern uint16_t _tt1,
				totalizer1Timer,
				priceChange_timer1,
				timer_config1,
				key19Timer1;

extern uint32_t num ;

extern uint32_t transaction_period;

//extern float target_pulser1 , current_pulser1 ;
extern sellmode_ sellmode ;

char str_[10]= {0};

//=== screen Arrays ====
char upper1[10]  = {0};
char middle1[10] = {0};

//=============== keyboard entry ======================
extern char keyboard_entry[10] = {0};  //buffers the key press
char keyboard[9] = {0};

//char temp[9] = {0};

char* xter_buffer1[7] = {0};
char* xter_buffer2[7] = {0};

int kkey = 0;
//=====================================================
//char keyboard_screen[6] = {0};
int  keypress_ = 0;
int8_t index_ = 0;
//uint32_t key_value = 0;
float key_value = 0.0,
	  key_value_original1 = 0.00;

uint8_t key_value_sellmodeP1 = 0,
		key_value_sellmodeL1 = 0;

//=====================================================
int flow_coeff = 0;
int pulser_rem1 = 0;
int tflow = 0;
//=====================================================
//  prog variables.
//int progg = 0;
 int prog_index = 0;
 int index2 = 0;
 int index3 = 0;
 int progg  = 0;

 extern uint32_t calib_pulser1 = 0;
  int calibr1 = 0;     //used for signaling motor and solenoid activatn in prog mode.

 auth_state auth = not_auth;   //first set this to
// int side = side1;           //pump side selected.
 int level = level1;
 int _index = 0;

 operatorfxn_  operatorfxn = totaliser_view;

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
			lafeng_keypad_18K[18],
			lafeng_keypad_18K_V2[18];

extern char bluesky_keypad[22];

//extern int pump_type;
extern pump pump_type;
extern uint8_t auth_cmd_flag;
extern int8_t opmode;
extern uint8_t error_clr_flag;
extern uint8_t keypress__ ;

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

extern uint8_t dp_amount1,
			   dp_vol1,
			   dp_unitprice1;

extern uint8_t firstTime_nozz1 = 1;

extern uint8_t stop_flag;

int irrecov_flag = 0;
//extern int error_clr_flag;
//=================================================
char keypad_buf[10] = {0};
char keypad_pw_xter1[10] = {0};

char keypad_pw[10] = {0};
char keypad_pw_[10] = {0};
//=================================================
//uint32_t price2pulser(float price);
//uint32_t amt2pulser(float amt);

uint8_t pulser_complete1 = 0; //used from the interrupt to signify the completion of pump sales.

//=================================================
char sc1[10] = {32};

uint8_t volume_flag = 0,
		pulser_flag,
		valve_flag;

float original_pulse = 0;

//float price_real1,amt_real1 = 0.0;
//float amt_,price_;

extern uint32_t pulser_new;
uint8_t filling1 = 0;
bool lock_clr = 0;

uint8_t firstTime_filling1 = 1,
		totalizer1_error = 0,
		calibration1_error = 0;

extern  uint16_t fast_flow_threshold1,
				 slowFlow_startThreshold1,
				 slowFlow_endThreshold1;

//================================================
//================================================
//extern uint8_t filling_ = 0;
extern uint8_t nozzle_bit;
extern uint8_t stop_fueling_bit;

uint8_t change_price = 0;
uint8_t change_volume = 0;

float auth_v1,
	  auth_p1;

uint8_t change_p1,
		change_v1;

extern int8_t auth_from_ctrl;
extern int8_t authorise_flag;
extern int8_t change_price_flag;
extern float set_p;


extern uint8_t changeLitrePrice1;
//-------------------------------------------------
//
extern flash_store_info flash_infoA,flash_infoB;
extern uint32_t flash_read_idA;
extern uint32_t flash_read_idB;

uint32_t flash_locc = 0;

extern pump disp_type1,
            disp_type2;

extern uint16_t shutdown_timer1;
extern uint16_t motor_tmr1;

int8_t pump_LitreOverflow = 0,
	   display_overflow1 = 0,
//	   _litre_price1 = 0,
	   _auth_p = 0,
	   _auth_v = 0,
	   idle_backwardPulse = 0,
	   idle_forwardPulse = 0,
	   flow_loss = 0,
	   changeLitrePrice1_2 = 0,
	   firstTime_display_overflow1 = 0,
	   priceChange_timerMin1 = 0;

uint8_t _pump_max_litres1 = 0,
		_litre_price1 = 0,
		nonValid_sale1 = 0,
		nozzleUp_inProgMode1 = 0;

float half_litre1 = 0.00,
	  display_minimumCentilitrePrice1 = 0.00;

uint8_t nozzleDown_source1,
		reset_flag1;

float pulser_totalizer1 = 0.00;
extern uint32_t pulser_benchMark1 = 15987;

float display_minimumCentilitre1;  // = ( (float)(settings[0].startUp_suppressVol) / 100 );

extern uint8_t calibrationCan_measure1;   //settings[0].calibration_measureCan;


//-------------   fxn prototypes ------------------
void do_calcs ();
//int long_press_tot();
//int long_press_log();
//------------------------------------------------

uint32_t  r_volTotaliser1 = 0;
uint32_t  old_r_volTotaliser1 = 0;

float r_amtTotaliser = 0.00,
	  old_r_amtTotaliser = 0.00;

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

	uint32_t temp = (f * pow(10,n));


	//temp = floor(temp);
 	int8_t nn = snprintf(chrr, sizeof(chrr_), "%d", temp);
 	uint8_t chrrr[12] = {0};			//uint8_t chrrr[10] = {0};
// 	for(int indx = 0; indx<10;indx++)
// 	{
// 		chrrr[indx] = 48;
// 	}

	memset(chrrr, '0', sizeof(chrrr));
// 	chrrr[9] = 0;
 	int8_t j = 8;
 	int8_t dpp = 0;


 	if (temp < 100000000)
 	{
 		chrrr[9] = 0;

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

 	else if( (temp >= 100000000) && (temp < 1000000000) )
 	{
 	 	chrrr[10] = 0;        //chrrr[9] = 0;
 	 	j = 9;				  //j = 8;

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

// 	float temp_ = atof(chrrr);
 	//double temp1_ = atoff(chrrr);

// 	temp_ += 0.00011;  //make small correction for the inherent rounddown.

	float temp_  = strtof(chrrr, &endPtr);

// 	price_dp = temp_;

 	return temp_;
}


// initialises the line 1 and 2 display variable.
void write_v(int i, char lcd_[9])
{
	if (i == 1)      //first line
	{
		  for(int ii = 0 ; ii < 9; ii++)
			{
               upper1[ii] = lcd_[ii];
               upper1[ii+1] = 0;
			}
	}

	if (i == 2)   //second line
	 {
		  for(int ii = 0 ; ii < 9; ii++)
			{
				middle1[ii] = lcd_[ii];
				middle1[ii+1] = 0;
			}
   	}

	if (i == 3)   //third line
	{
		for(int ii = 0 ; ii<7 ; ii++)
		{
			keyboard[ii] = lcd_[ii];
			keyboard[ii+1] = 0;
		}
	}
}
//--------------------------------------------
void reset_timer(int tm)
{
   time_e = tm;
   timer_flag = 0;
   timer_flag_old = 1;
   st = 0;
}

void start_timer(int tm)
{
	time_e = tm;
	st = 1;
}
//=======================================
void dec_timer(void)
{
  if(st == 1)
	{
	  if (time_e > 0)time_e--;
	   if(time_e == 0)
	   {
		   timer_flag = 1;
	   }
	   else
	   {
		   timer_flag = 0;
	   }
	}
}
//=======================================
void stop_timer(void)
{
	timer_flag_old = 1;
	time_e = 0;
	st = 0;
}
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
	{_no_Event, 0},
};



//initialise the array of structure of State and state handlers and their
// allowed  events.
// { <state>,<handler>,{<allowed event1>,<allowed event2>,..,<allowed eventn>}}
sStateEventMachine asStateEventMachine_1 [] =
{
	{prog_State, progState_Handler,{_keydown_Event,_keypress_Event}},
	{idle_State, idleState_Handler, {_operator_Event,_keyup_Event,_tot_error_Event, _keypress_Event,_nozzleup_Event, _auth_command_Event, _nozzledown_Event, _resetcommand_Event, _switchoffcommand_Event}},
	{inactive_State, inactiveState_Handler,{_error_clear_Event, _keyup_Event, _keypress_Event}},
	{nozzleup_waitingforauth_State, nozzleup_waitingforauthState_Handler, {_authorise_Event,_timeout_Event,_nozzledown_Event,_keypress_Event, _authorisecommand_Event, _stopcommand_Event, _switchoffcommand_Event, _hardwarereset_Event, _hardwareerror_Event}},
	{authorised_nozzledown_State, authorised_nozzledown_State_Handler,{_nozzleup_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
	{authorised_nozzleup_State, authorised_nozzleup_State_Handler, {_filling_pulse_Event,_pause_Event,_timeout_Event,_nozzledown_Event,_keypress_Event,_function_key_Event, _stopcommand_Event, _auth_suspendcommand_Event, _hardwarereset_Event, _hardwareerror_Event}},
	{authorisation_paused_State, authorisation_paused_State_Handler, {_resume_Event,_timeout_Event,_nozzledown_Event,_keypress_Event, _auth_resumecommand_Event,  _stopcommand_Event, _hardwarereset_Event, _hardwareerror_Event, _switchoffcommand_Event}},
	{authorisation_resumed_State, authorisation_resumed_State_Handler, {}},
	{filling_State, filling_State_Handler, {_filling_paused_Event,_keypress_Event,_timeout_Event,_nozzledown_Event,_keypress_Event, _stopcommand_Event, _filling_suspendcommand_Event, _hardwarereset_Event, _hardwareerror_Event, _mamo_Event, _switchoffcommand_Event}},
	{filling_paused_State, filling_paused_State_Handler, {_filling_resumed_Event, _keypress_Event, _timeout_Event,_nozzledown_Event, _filling_resumecommand_Event, _stopcommand_Event, _hardwarereset_Event, _hardwareerror_Event, _switchoffcommand_Event}},
	{filling_resumed_State, filling_resumed_State_Handler, {}},
	{keypad_entry_State, keypad_entry_State_Handler, {}},
	{operator_State, operator_State_Handler, {_keypress_Event}},
	{savesettings_State, savesettings_State_Handler, {_keypress_Event}},
	{read_flash_State, read_flash_State_Handler, {}},
	{write_flash_State, write_flash_State_Handler, {}},
	{switchedoff_State, switchedoffState_Handler, {_stopcommand_Event, _resetcommand_Event}},
    {pnp_State, pnpState_Handler, {_keyup_Event, _fillingcomplete_Event}},
	{filledmamo_State, filledmamo_State_Handler, {_keypress_Event, _nozzledown_Event, _resetcommand_Event, _stopcommand_Event, _switchoffcommand_Event}},
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
void clear_buffer1(void)
{
	int i;
	for( i = 0;i<sizeof(keyboard_entry);i++)

	 {
		keyboard_entry[i] = 0;
		keypad_pw_xter1[i] = 0;
	 }
	keypad_pw_xter1[i] = 0;
	keypad_pw_xter1[i++] = 0;
	keypad_pw_xter1[i++] = 0;
	keypad_pw_xter1[i++] = 0;
	index_ = 0;
	_index = 0;
}


//-------------------------------------
eSystemState keyup_Handler(void)
{
	progg = 1;
	copy_settings(move_to_copy);	//make a copy of the settings								//to be used in the prog.
	clear_buffer1();
	return prog_State;
}
//-------------------------------------
eSystemState inactiveState_Handler(void)
{
    //error_state = 1;
		  // All errors land here....
		  // Only the CLEAR  key returns pump from this state....
          // key reception is done in the key_entry handler..

	return inactive_State;
}
//--------------------------------------
eSystemState savesettings_State_Handler(void)
{
   static int sav = 0;
   int respons = 0;
   //====================================================
	if (sav == 0)
		{
		  if(t>500)
		   {
			  lcd_print_line1("1-  Saue");
			  lcd_print_line2("2-Cancel");
			  t = 0;
		   }
		  respons = atoi(keyboard_entry);
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
		lcd_print_line1(" sauing ");
		lcd_print_line2("settings");

		copy_settings(move_to_settings0);
		save_settings_original_fram(side_a);

		copy_settings(move_to_settings); // copy the structure.
		save_settings_fram();   //save to eeprom

		configMode1 = CONFIGMODIFIED;
//		save_configFlag(side_a);
		save_configFlag_fram(side_a);
		save_config_otpSeed_time_fram(side_a);

		clear_configChange_trackNum_fram(side_a);

		load_settings(side_a); //load the settings into the
		load_settings(side_b); // internal variables
		HAL_Delay(1500);

		lcd_print_line1("  done  ");
		lcd_print_line1("        ");
		sav = 0;
		progg = 0;
		auth = not_auth;
		clear_buffer1();
//	        return savesettings_State;
		return idle_State;
	}
	//=======================================
	if (sav == 2)
	{
		 sav = 0;
		 progg = 0;
		 auth = not_auth;
		 //clear keyboard buffers.
		 clear_buffer1();

//		 lcd_print_line1(upper1);
//		 lcd_print_line2(middle1);

		 if(settings_stream1[0].display_format == PL)
		 {
			 lcd_print_line1(upper1);
			 lcd_print_line2(middle1);
		 }
		 else if(settings_stream1[0].display_format == LP)
		 {
			 lcd_print_line1(middle1);
			  lcd_print_line2(upper1);
		 }
		 char str__[8]= {0};
		 snprintf(str__, sizeof(str__), "%.2f", litre_price);
		 lcd_print_line3(str__);
		 return idle_State;
	}
	//========================================
	return savesettings_State;    //idle_State;  //return to idle mode
}
//--------------------------------------
eSystemState keydown_Handler(void)
{
	clear_buffer1();
	prog_entry1 = 0;
	prog_revisit1 = 1;

	lcd_print_line3("        ");

#ifndef DEV_MODE
	prog_revisitt1 = 1;
#endif

	return savesettings_State;   //
}

//-----------------------------------
eSystemState nozzleup_Handler(void)
{
	pump1_status_4G = STATUS_NOZZLE_UP;

	reset_timer(timeout_picknozzle);
	start_timer(timeout_picknozzle);


	//set the motor on and solenoid off.
	get_auth();

	if(opmode == AUTO_MODE)
	{
		 nozzle_out1 = true;
		 status_change_noz1 = 1;

		 if(pump_status_1 == STATUS_FILLING_COMP)
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

	if (eLastState1 == authorised_nozzledown_State)
	{
		 current_pulser1 = 0;
		 overall_currentPulser1 = 0;
		 clr_pulser1();    //clear hardware pulser
		 return authorised_nozzleup_State;
	}

	return nozzleup_waitingforauth_State;
}

//-------------
eSystemState auth_command_Handler(void)
{
	char *endPtr;

	reset_timer( timeout_picknozzle);
	  start_timer( timeout_picknozzle);
    //-----------------------------------
	  if(opmode == MANUAL_MODE)
	  {
		  //check if any keypad entry
		 if(index_ >= 1)
		  {
              key_value  = strtof(keyboard_entry, &endPtr);
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
		  if (change_p1 == 1)
			  sellmode = P;      //set sell type to price
		 //-----------------------------------------
		  //authorise volume...
		  else if (change_v1 == 1)
			  sellmode = L;      //set sell type to litre
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

//initialise the fuel and price variables
 price  = 0.0;
 amt = 0.0;


 target_pulser1 = 0;  //state is coming from nozzleup ,no price/amt set

 pump_status_1 = STATUS_AUTH;
 status_change_pump1 = 1;

 clr_pulser1();    //clear hardware pulser
 current_pulser1 = 0;
 overall_currentPulser1 = 0;

// int cnv = 0;
// snprintf(str_, sizeof(str_), "%.2f", price); lcd_print_line1(str_);
// snprintf(str_, sizeof(str_), "%.2f", amt);   lcd_print_line2(str_);

 lcd_print_line1("--auth--");
 lcd_print_line2("        ");
 char str__[8]= {0};
 snprintf(str__, sizeof(str__), "%.2f", litre_price); lcd_print_line3(str__);

 return authorised_nozzleup_State;
}

//----------------------------------------
eSystemState nozzledown_Handler(void)
{
	 filling1 = 0;
	 nozzle_bit = 0;

	 pump1_status_4G = STATUS_NOZZLE_DOWN;

	 if(nozzleup_awaitingauth_state_not_timedOut == 1)
	 {
		 nozzleup_awaitingauth_state_not_timedOut = 0;
		 clr_screen1();
	 }

	 if(nonValid_sale1 == 1)
	 {
		 return idle_State;
	 }

	 stop_flow1();

	//lcd_print_line1("nos dn");
	 reset_timer(30);
	 stop_timer();

	 if( (eLastState1 == idle_State) && (eNextState1 == idle_State) )
	 {
//		  return idle_State;
	 }
	 else
	 {
		  do_calcs();
		  get_time();
		  update_info();
//		  save_volumeTotaliser(operating_side);
//		  save_amountTotaliser(operating_side);
		  save_totaliser_fram(side_a);
//		  save_amountTotaliser_fram(side_a);
		  save_lastSale_fram(side_a);
		  nozzle_bit = 0;
	 }
/*
		 char str__[8]= {0};
         snprintf(str__, sizeof(str__), " %.2f", litre_price);
		 lcd_print_line3(str__);   */

		 char str__[10]= {0};
		snprintf(str__, sizeof(str_), "%.2f", litre_price);
		lcd_print_line3(str__);


	 if( ((pump_LitreOverflow == 1) && (pulser_rem1 > 0 )) || (display_overflow1 == 1) )  //|| ((display_overflow1 == 1) && (pulser_rem1 > 0 )) )
	 {
		  if(pump_LitreOverflow == 1)
			  pump_LitreOverflow = 0;

		  if(display_overflow1 == 1)
		  {
			  display_overflow1 = 0;
			  firstTime_display_overflow1 = 0;
		  }

		  clr_screen1();
	 }

 //--------------------------------------------------------
//	dpFlag = 0;
//	error_clr_flag = 1;
//	 index_ = 0;
//	 _index = 0;
//	 for(int i = 0; i < 9; i++)
//	 {
//	   keypad_pw_xter1[i] = 0;
//	   keyboard_entry[i] = 0;   //clear the buffer
//	 }
//
//	 for(int i = 0; i <= 8; i++)
//	 {
//		 keyboard[i] = 0;
//	 }
//	 //--------------------------------------------------------------------
//	 if (sellmode == P)
//	 {
//		  write_v(3, "P     0");  //send_keypad("p    ");  //5 xters  lafeng..
//	 }
//	 else if(sellmode == L)
//	 {
//		  write_v(3, "L     0");  //send_keypad("l    ");  //5 xters lafeng
//	 }
////	 else if(sellmode == V)
////	 {
////		  write_v(3, "v    0");  //send_keypad("p    ");  //5 xters  lafeng..
////	 }
//	 send_keypad(keyboard);

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


	 keypad_zerorize();
  //---------------------------------------------------------------------
  //             saving to the log
	 if(eNextState1 == filling_State)
	 {
		stop_fueling_bit = 0;

	  //===========================================================
	  //         for totaliser toggle.
	  //===========================================================

		running_volTotaliser1 = working_volTotaliser1 + amt_real1;

		running_volTotaliser1c = working_volTotaliser1c + amt_middle1;

		running_amtTotaliser1 = working_amtTotaliser1 + price_real1;

		running_amtTotaliser1c = working_amtTotaliser1c + price_upper1;

		float pricecheck = running_amtTotaliser1c - priceOld1;

		if (pricecheck >= 1000.00)
		{
		   priceOld1 = running_amtTotaliser1c;
		   save_amountSend(side_a);

		   char str[65];
		   sprintf(str, "[Side-A]... #%0.2f intermittent worth of sales made now!", pricecheck);
		   server_write(str);
		}

		r_volTotaliser1 = floor( running_volTotaliser1c );

		if(r_volTotaliser1 != old_r_volTotaliser1)
		{
			totalizer1Timer = 0;
	  //			then toggle the totaliser harware I/O.
			drive_totaliser1(ACTIVATE);

		}
		else
		{
			//deactivate totaliser output...
			if(totalizer1Timer > 200)
			{
				drive_totaliser1(DEACTIVATE);
			}

		}
		old_r_volTotaliser1 = r_volTotaliser1;   //update...


	 //============================================================

		if(settings_stream1[0].mode == AUTO_MODE)
		{

			 if( (pump_status_1 == STATUS_FILLING) || (pump_status_1 == STATUS_MAMO_REACHED) )
			 {

			 }

			 //////////////////////////////////////////////////////////////
			 ///////// SIGNALS GO-CONTROLLER ABOUT NOZZLE STATUS //////////

			 status_change_noz1 = 1;
			 nozzle_out1 = false;

			 nozzleDown_source1 = 1;

			 //////////////////////////////////////////////////////////////

		}

		return  write_flash_State;
	 }

  //-------------------------------------------

	 pump_status_1 = STATUS_FILLING_COMP;

      return idle_State;
}

//----------------------------------------
eSystemState timeout_Handler(void)
{
	 if(eLastState1 != authorised_nozzleup_State)
	 {
		 lcd_print_line1(" t out  ");
		 lcd_print_line2("--------");

		 if( ((pump_LitreOverflow == 1) && (pulser_rem1 > 0 )) || ((display_overflow1 == 1) && (pulser_rem1 > 0 )) )
		 {
			  if(pump_LitreOverflow == 1)
				  pump_LitreOverflow = 0;
			  if(display_overflow1 == 1)
				  display_overflow1 = 0;
		 }
	 }

  //write the commodity price...
  // char str__[8]= {0};
  // snprintf(str__, sizeof(str__), " %.2f", litre_price);
  // lcd_print_line3(str__);
  // lcd_print_line3("-----");

  //compose the kind of timeout error
  if(eLastState1 == nozzleup_waitingforauth_State)
  {
	 lcd_print_line3("Err1 ");
	 nozzleup_awaitingauth_state_not_timedOut = 0;
  }

  if(eLastState1 == authorised_nozzledown_State)
  {
     lcd_print_line3("Err2 ");
  }

  if(eLastState1 == authorisation_paused_State)
  {
	 lcd_print_line3("Err3 ");
  }

 if(eLastState1 == filling_paused_State)
  {
	 lcd_print_line3("Err4 ");
  }

 if(eLastState1 == authorised_nozzleup_State)
 {
	 if( ((pump_LitreOverflow == 1) && (pulser_rem1 > 0 )) || ((display_overflow1 == 1) && (pulser_rem1 > 0 )) )
	 {
		  if(pump_LitreOverflow == 1)
			  pump_LitreOverflow = 0;
		  if(display_overflow1 == 1)
			  display_overflow1 = 0;
	 }

	 lcd_print_line1("    No ");
	 lcd_print_line2("  Flouu ");
	 lcd_print_line3("Err15 ");
 }
// if(eLastState1 == power_failure)
//  {
//	 lcd_print_line3("err5 ");
//  }
// if(eLastState1 == storage_error)
//  {
//	 lcd_print_line3("err6 ");
//	 store fail
//  }
// if(eLastState1 == pump_maxLitres) //pump_LitreOverflow
//  {
//	 lcd_print_line3("err7 ");
//	 pump limit
//  }
// if(eLastState1 == p/l = 0 @start)
//  {
//	 lcd_print_line3("err8 ");
//  }
// if(eLastState1 == p/l = 0 @filling1)
//  {
//	 lcd_print_line3("err9 ");
//  }
// if(eLastState1 == comm error)
//  {
//	 lcd_print_line3("err10 ");
//  }
// if(eLastState1 == data error)
//  {
//	 lcd_print_line3("err11 ");
//  }
// if(eLastState1 == back pulses from idle)
//  {
//	 lcd_print_line3("err12 ");
//  }
// if(eLastState1 == 4ward pulses from idle)
//  {
//	 lcd_print_line3("err13 ");
//  }
// if(eLastState1 == no flow timeout)
//  {
//	 lcd_print_line3("err14 ");
//  }
// if(eLastState1 == flow lost timeout)
//  {
//	 lcd_print_line3("err15 ");
//  }
// if(eLastState1 == unfinished)
//  {
//	 lcd_print_line3("err16 ");
//  }


// if(eLastState1 == currentFlow < prevFlow)
//  {
//	 lcd_print_line3("err4 ");
//  }

 	 pump_status_1 = STATUS_FILLING_COMP;

	 return inactive_State;
   // return idle_State;
}

//----------------------------------------
eSystemState tot_error_Handler(void)
{
   //set irrecoverable error flag.
//	irrecov_flag = 1;

	totalizer1_error = 1;

	lcd_print_line1("   tot   ");
	lcd_print_line2("  Error ");

    return inactive_State;
}

//----------------------------------------
eSystemState key19_Handler(void)
{
	return idle_State;
}

//----------------------------------------
eSystemState keypress_Handler(void)
{

	static allowed_xters = 6;

	extern uint8_t error_clr_flag;

	extern uint8_t keyEntry_len;

//	 if(pump_type == lafeng)
//	 if(pump_type == DN_LAFNG17K)
//	 if( (settings_stream1[0].keypad__ == LAFNG17_K) || (settings_stream1[0].keypad__ == LAFNG18_K) )
//	 {
//	   kkey =  lafeng_keypad[keypress_];
//	   allowed_xters = 6;
//	 }
	 if(settings_stream1[0].keypad__ == LAFNG17_K)
	 {
	   kkey =  lafeng_keypad[keypress_];
	   allowed_xters = 6;
	 }
	 else if (settings_stream1[0].keypad__ == LAFNG18_K)
	 {
	   kkey =  lafeng_keypad_18K[keypress_];
	   allowed_xters = 7;
	 }
	 else if (settings_stream1[0].keypad__ == LAFNG18_K_V2)
	 {
	   kkey =  lafeng_keypad_18K_V2[keypress_];
	   allowed_xters = 7;
	 }
//	 else if(pump_type == bluesky)
//	 else if( (pump_type == DN_BLSKY18K) || (pump_type == DN_BLSKY22) ||
//			  (pump_type == DIN_BLSKY18K) || (pump_type == DIN_BLSKY22)  )
	 else if( (settings_stream1[0].keypad__ == BLSKY18_K) || (settings_stream1[0].keypad__ == BLSKY22) )
	 {
	    kkey =  bluesky_keypad[keypress_];
	    allowed_xters = 7;
	 }

   ePrevState = eLastState1;

 /*  if (progg == 1)
	 {
	    return keypad_entry_State;
	 }  */

	//if (progg == 1) //if in programming mode return.

	//char prc[7] = {'p'};
	//char amt[7] = {'l'};
	//char keyboard[6] = {0};
	//char keyboard_screen[6] = {0};

   int8_t space = allowed_xters - index_; //int space = 6 - index_;
   uint8_t ind = 1;

	//----------------------------------------------------------
	//If only 0 - 9 is pressed.....
	if(
		(kkey != 'A') && (kkey != 'B') &&
		(kkey != 'C') && (kkey != 'D') &&
		(kkey != 'F') && (kkey != '-') &&
		(kkey != 'G') && (kkey != 'J')  //&&(index_ < 6)
	  )
	{
	//	 keyEntry_len = strlen(keyboard_entry);

		 if(index_ <= allowed_xters)    //only allow 6 xters...
		 {
			keyboard_entry[index_] = kkey;
			keyboard_entry[index_+1] = 0;         //NULL;
	//		if(kkey == '.')
	//		{
	//			keyboard_entry[index_+1] = '0';         //NULL;
	//			keyboard_entry[index_+2] = 0;         //NULL;
	//		}
			index_++;
		  }
		}
	//--------------------------------------------------------
		  if (kkey == 'F')      //'clear' key.
		  	{
			  //ePrevState = eLastState1;
			  //set the error clear flag..

			    dpFlag = 0;
			    error_clr_flag = 1;
                 index_ = 0;
                 _index = 0;
                 for(uint8_t i = 0; i < 9; i++)
				 {
				   keypad_pw_xter1[i] = 0;
				   upper1[i] = 0;
				   middle1[i] = 0;
				 }

                 for(uint8_t i = 0; i <= 6; i++)
				 {
					 keyboard_entry[i] = 0;   //clear the buffer
					 keyboard[i] = 0;
				 }

                 if(settings_stream1[0].display_format == PL)
                 {
                	 upper1[0]  = 'P';
                	 middle1[0] = 'L';
                 }
                 else if(settings_stream1[0].display_format == LP)
                 {
                	 upper1[0]  = 'L';
                	 middle1[0] = 'P';
                 }
		  	}
//--------------------------------------------------------------------
//		  progg = 1;
//		  auth = authed;
		  if (progg == 1)
		  	 {
			  	keypad_print("       ");
			    if (auth == not_auth)
			    	keypad_print(keypad_pw_xter1);
			    else
			    {
					 int8_t keyBoard_len = strlen(keyboard_entry);
					 strncpy(keyboard, keyboard_entry, sizeof(keyboard));
					 if(strchr(keyboard, '.') )
					 {
						 if(keyboard[keyBoard_len - 1] == '.')
						 {
							 keyboard[keyBoard_len] = '0';
						 }
					 }
					 keypad_print(keyboard);
				}
		  	    return keypad_entry_State;
		  	 }


      if ( ((kkey == 'D') && (progg == 0)) && (eNextState1 != filling_State) ) //fueling key.
	 	{
       	   //ePrevState = eLastState1;

    	  if(settings_stream1[0].mode == MANUAL_MODE)
    	  {
			   auth_cmd_flag = 1;  //activate auth cmd.

			   return keypad_entry_State;
    	  }
    	  else if(settings_stream1[0].mode == AUTO_MODE)
    	  {
    		  nozzle_flag_key1 = 1;

    		  return keypad_entry_State;
    	  }
	 	}

//      if ( (kkey == 'A') && (progg == 0) && ( (eNextState1 == filling_State) || (eNextState1 == authorised_nozzleup_State)) ) //stop sales.
      if ( (kkey == 'A') && (progg == 0) )
	  {
		   stop_flag = 1;  //deactivate auth cmd.

		   nozzle_flag_key1 = 0;
		   nozzle_flag_key_old1 = 1;
	  }

//====================================================
 if ( (kkey == 'C') && (progg == 0) )  //if change sales mode
	{
	   if(sellmode == L)
	   {
	       sellmode = P;
//	        if(settings[0].display_mode == PL)
//	       	{
//	    	   sellmode = P;
//	       	}
//	       	else if(settings[0].display_mode == LP)
//	       	{
//	       		sellmode = L;
//	       	}
	   }
	   else if(sellmode == P)
	   {
	   	 sellmode = L;
	   }

//	   else if(sellmode == P)
//	   {
//		   sellmode = V;
////		    if(settings[0].display_mode == PL)
////			{
////			   sellmode = L;
////			}
////			else if(settings[0].display_mode == LP)
////			{
////				sellmode = P;
////			}
//	   }
//	   else if (sellmode == V)
//	   {
//		   sellmode = L;
//	   }
	}
//--------------------------------------------------------------------
 int8_t space2, c, b;
 keyEntry_len = strlen(keyboard_entry);
 if( (strchr(keyboard_entry, '.')) && (keyboard_entry[keyEntry_len - 1] != '.') )
 {
	 space2 = 7 + ( -index_ + 1);
	 dpFlag = 1;

	 b = 0;
	 dpCount = 0;
	 while( *(keyboard_entry + b) != '\0')
	 {
		 if( (keyboard_entry[b] == '.') || (dpCount != 0) )
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
//	 if(strchr(keyboard_entry, '.') )
//	 {
//		 if(keyboard_entry[keyEntry_len - 1] == '.')
//			 keyboard_entry[keyEntry_len] = '0';
//	 }

	 space2 = 7 - index_;     //6 - index_;
 }
 if ((sellmode == L)&&(progg == 0) )
 {
	memset(middle1, '\0', sizeof(middle1));
	middle1[0] = 'L';     //Append price to display.
	// if((index_-1) >= 6) space = 2;
	while (space2 > 0) //write spaces first..
	{
	   middle1[ind++] = ' ';
	   space2--;
	}
	space2 = index_;     //reload with len of actual number
	while(space2 > 0)
	 {
		 middle1[ind++] = keyboard_entry[index_ - space2];
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

	write_v(1,"p       ");
	  //  lcd_print_line2(middle1);
   }
//--------------------------------------------------------------------
// if(settings[0].display_mode == PL)
//  {
//	 upper1[0]  = 'P';
//	 middle1[0] = 'L';
//  }
//  else if(settings[0].display_mode == LP)
//  {
//	 upper1[0]  = 'L';
//	 middle1[0] = 'P';
//  }
   else if ( (sellmode == P)&&(progg == 0) )
   {
//	   if(settings[0].display_mode == PL)
//		{
//		   upper1[0]  = 'P';    // append price to display
//		}
//		else if(settings[0].display_mode == LP)
//		{
//			upper1[0]  = 'L';   // append price to display
//		}
	   memset(upper1, '\0', sizeof(upper1));
	   upper1[0] = 'P';
	// if((index_-1) >= 6) space = 2;
	   while (space2 > 0)     //write spaces first..
	   {
		   upper1[ind++] = ' ';
		   space2--;
	   }
	 space2 = index_ ;          //reload with len of actual number

// 		 if(keyboard_entry[index_ - 1] == '.')
// 		 {
// 			ind++;
// 		 }

		while(space2 > 0)
		 {
			 upper1[ind++] = keyboard_entry[index_ - space2];
			 space2--;
		 }
		//lcd_print_line1(upper1);  //clear the price
		//lcd_print_line2("l       ");
	   write_v(2,"L       ");
//	   if(settings[0].display_mode == PL)
//	   	{
//	   		write_v(2,"l       ");
//	   	}
//	   	else if(settings[0].display_mode == LP)
//	   	{
//	   		write_v(2,"p       ");
//	   	}
	 }
     else if ( (sellmode == V)&&(progg == 0) )
     {
    	 memset(upper1, '\0', sizeof(upper1));
    	 upper1[0] = 'C';
		// if((index_-1) >= 6) space = 2;
		   while (space2 > 0)     //write spaces first..
		   {
			   upper1[ind++] = ' ';
			   space2--;
		   }
		 space2 = index_ ;          //reload with len of actual number

	 // 		 if(keyboard_entry[index_ - 1] == '.')
	 // 		 {
	 // 			ind++;
	 // 		 }

			while(space2 > 0)
			 {
				 upper1[ind++] = keyboard_entry[index_ - space2];
				 space2--;
			 }
			//lcd_print_line1(upper1);  //clear the price
			//lcd_print_line2("l       ");
		   write_v(2,"l       ");
     }
//----------------------------------------------------------------
//                            write to the keypad

 	   static uint8_t lcd_size = 5;

 	   if(settings_stream1[0].keypad__  == LAFNG17_K)
 	   {
 		    lcd_size = 5; //change this latter to accomodate other lcds.
 	   }
 	   else if( (settings_stream1[0].keypad__  == LAFNG18_K) || (settings_stream1[0].keypad__ == LAFNG18_K_V2) )
 	   {
 		    lcd_size = 7; //change this latter to accomodate other lcds.
 	   }
 	   else if( (settings_stream1[0].keypad__  == BLSKY18_K) || (settings_stream1[0].keypad__  == BLSKY22) )
 	   {
 		    lcd_size = 7;   //6;
 	   }

//================================================================
	 int8_t size = lcd_size - 1;   // 1 xter to display 'P/L'
//	 if(strchr(keyboard_entry, '.')) size = lcd_size;

	 int8_t k_index = 0;
	 ind = 1;

	// int8_t space2, c;
	//  c = strlen(keyboard_entry);
	//  if( (strchr(keyboard_entry, '.')) && (keyboard_entry[c-1] != '.') )
	//  {
	// 	 space2 = 7 + ( -index_ + 1);
	//  }
	//  else
	//  {
	// 	 space2 = 7 - index_;     //6 - index_;
	//  }
	  if (sellmode == P)
	  {
		  keyboard[0] = 'P';

//		  if(settings[0].display_mode == PL)
//		  {
//			  keyboard[0] = 'p';
//		  }
//		  else if(settings[0].display_mode == LP)
//		  {
//			  keyboard[0] = 'l';
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
		    	 keyboard[ind++] = ' ';
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

				if( (strchr(keyboard_entry, '.')) && (index_ > 6) ) space = 7;
			    while(space > 0)
				 {
				   keyboard[ind++] = keyboard_entry[index_ - space];
				   space--;
				 }

			   if(keyboard[ind - 1] == '.') keyboard[ind] = '0';

	   }
//----------------------------------------------------------------
	  if (sellmode == L)
	  {

		  keyboard[0] = 'l';

//		  if(settings[0].display_mode == PL)
//		  {
//			  keyboard[0] = 'l';
//		  }
//		  else if(settings[0].display_mode == LP)
//		  {
//			  keyboard[0] = 'p';
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
			 keyboard[ind++] = ' ';
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

			if( (strchr(keyboard_entry, '.')) && (index_ > 6) ) space = 7;
			while(space > 0)
			 {
			   keyboard[ind++] = keyboard_entry[index_ - space];
			   space--;
			 }

			if(keyboard[ind - 1] == '.') keyboard[ind] = '0';

	   }

	   if (progg == 1)
		 {
		    keypad_print("      ");
			if (auth == not_auth)
				keypad_print(keypad_pw_xter1);
			else
			{
				 int8_t keyBoard_len = strlen(keyboard_entry);
				 strncpy(keyboard, keyboard_entry, sizeof(keyboard));
				 if(strchr(keyboard, '.') )
				 {
					 if(keyboard[keyBoard_len - 1] == '.')
					 {
						 keyboard[keyBoard_len] = '0';
					 }
				 }
				 keypad_print(keyboard);
			}
			return keypad_entry_State;
		 }

	   	 keypad_print(keyboard);
//----------------------------------------------------------------
      //send_keypad(keyboard);

         return keypad_entry_State;
}
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
	pump_status_1 = STATUS_FILLING_COMP;
	status_change_pump1 = 1;

	return idle_State;
}

eSystemState resetcommand_Handler(void)
{
	pump_status_1 = STATUS_RESET;
	status_change_pump1 = 1;

	return nozzleup_waitingforauth_State;
}

//eSystemState priceupdate_Handler(void)
eSystemState _fillingcomplete_Handler(void)
{
	pump_status_1 = STATUS_FILLING_COMP;
	status_change_pump1 = 1;
	clr_screen1();

	return idle_State;
}

eSystemState switchoffcommand_Handler(void)
{
	pump_status_1 = STATUS_SWITCHED_OFF;
	status_change_pump1 = 1;

	return switchedoff_State;
}

eSystemState authorisecommand_Handler(void)
{
	pump_status_1 = STATUS_AUTH;
	status_change_pump1 = 1;

	return authorised_nozzleup_State;
}

eSystemState hardwarereset_Handler(void)
{
	pump_status_1 = STATUS_FILLING_COMP;
	status_change_pump1 = 1;

	return idle_State;
}

eSystemState hardwareerror_Handler(void)
{
	stop_flow1();

	pump_status_1 = STATUS_FILLING_COMP;
	status_change_pump1 = 1;

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
	return filling_State;
}

eSystemState mamo_Handler(void)
{
	pump_status_1 = STATUS_MAMO_REACHED;
	status_change_pump1 = 1;

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
	static int pressed_ = 0;
		//static int pressed_old = 0;
		int ky;

//		if(pump_type == bluesky)
//		if( (pump_type == DN_BLSKY18K) || (pump_type == DN_BLSKY22) ||
//			(pump_type == DIN_BLSKY18K) || (pump_type == DIN_BLSKY22) ||
//			(pump_type == DN_LAFNG17K) )

		if( (settings_stream1[0].keypad__ == BLSKY18_K) || (settings_stream1[0].keypad__ == BLSKY22) ||
			(settings_stream1[0].keypad__ == LAFNG17_K) || (settings_stream1[0].keypad__ == LAFNG18_K) ||
			(settings_stream1[0].keypad__ == LAFNG18_K_V2) )
		{
//		   ky = 19;  //F4 key
		   ky = 15;  //clear key
		}
		else
			return 0;      // only for bluesky boards .


		if(keypress_ != ky)
		{
	        pressed_ = 0;
	        key_buttonpress_tmr = 0;  //clr timer.
		}
		 if((key_buttonpress_tmr >= 3)&&(pressed_ == 0) )
		 {
		   key_buttonpress_tmr = 3;
			pressed_ = 1;
			return 1;
		 }
		 return 0;
}

/////////////////////////////////////////////////////////////////

uint8_t long_press_progExit()
{
	static uint8_t pressed_ = 0;
		//static int pressed_old = 0;
	uint8_t ky;

//		if(pump_type == bluesky)
//		if( (pump_type == DN_BLSKY18K) || (pump_type == DN_BLSKY22) ||
//			(pump_type == DIN_BLSKY18K) || (pump_type == DIN_BLSKY22) ||
//			(pump_type == DN_LAFNG17K) )
		if( (settings_stream1[0].keypad__ == BLSKY18_K) || (settings_stream1[0].keypad__ == BLSKY22) ||
			(settings_stream1[0].keypad__ == LAFNG17_K) || (settings_stream1[0].keypad__ == LAFNG18_K) ||
			(settings_stream1[0].keypad__ == LAFNG18_K_V2) )
		{
		   ky = 14;  //'.' key
		}
		else
			return 0;      // only for bluesky boards .


		if(keypress_ != ky)
		{
	        pressed_ = 0;
	        progExit_buttonpress_tmr1 = 0;  //clr timer.
		}
		 if((progExit_buttonpress_tmr1 >= 3) && (pressed_ == 0) )
		 {
			 progExit_buttonpress_tmr1 = 3;
			pressed_ = 1;
			return 1;
		 }
		 return 0;
}

/////////////////////////////////////////////////////////////////

uint8_t long_press_log()
{
	static uint8_t pressed_ = 0;
	//static int pressed_old = 0;
	uint8_t ky = 0;
	ky = readkey19_state();
	//	if (readkey19_state() != 1)
	if (ky != 1)
	{
        pressed_ = 0;
        log_buttonpress_tmr = 0;   //clr timer.
        HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
	}
	else if (ky == 1)
	{
		if(settings_stream2[0].keypress_tone == Yes)
		{
			HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
			HAL_Delay(10);
		}

	}

	 if ( (log_buttonpress_tmr >= 3) && (pressed_ == 0 ) )
	 {
	   log_buttonpress_tmr = 3;
		pressed_ = 1;

		HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
		return 1;
	 }

	 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
	 return 0;
}
/////////////////////////////////////////////////////////////////
uint8_t long_press_tot()
{
	static uint8_t pressed_ = 0;
	uint8_t ky;

	static uint8_t	key19StateCount = 0;

   	static bool doublePressDetected = false,
				click_in_progress = false;

	int doublePressThreshold = 2000;

	 dummyValue = 0;

	if( (settings_stream1[0].keypad__ == LAFNG17_K) || (settings_stream1[0].keypad__ == LAFNG18_K) || (settings_stream1[0].keypad__ == LAFNG18_K_V2))
	{
//		ky = 11;  //keypad type mapping...
		key19State = readkey19_state();

		if( (firstTime_key19 == 1) && (key19State == 1) )
		{
			key19Timer1 = 0;
			firstTime_key19 = 2;

			if(settings_stream2[0].keypress_tone == Yes)
			{
				  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
				  HAL_Delay(10);
			}
		}

		 else if (key19State == 0)   // Key19 is released
		 {
			   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
		 }


	  if (key19State != lastKey19State)
	  {
		  //we keep checking for clicks, we don't care about the non-click detection (it will be most of the time)
		  if (key19State == 1)   // Key19 is pressed
		  {
				 if(key19StateCount == 0)
				 {
					 key19Timer1 = 0; //reset the timer only on the first click
					 click_in_progress = true; //we are in the clicking phase;
				 }
				 key19StateCount++ ; // it will start as 1 and will keep incrementing

				 if(settings_stream2[0].keypress_tone == Yes)
				 {
					 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
					 HAL_Delay(10);
				 }
		  }

			 if(click_in_progress == true)
			 {
				 if(key19Timer1 >= 500)   //now we have passed the 500ms let's check how many clicks happened
				 {
					 if(key19StateCount > 1)
					 {
						//it's a double or multiple click
						 doublePressDetected = true;
					 }
					 else
					 {
						 //has to be at least 1 so it's a single click, it can never be zero
					 }
					 key19StateCount = 0;
					 click_in_progress = false;
				 }
			 }
	  }

	  lastKey19State = key19State;

	  if (doublePressDetected)
	  {
		  doublePressDetected = false;
		  tot_buttonpress_tmr = 3;
		  pressed_ = 1;
		  firstTime_key19 = 1;
		  return 1;
	  }
	}
	else
	{
		ky = 21;                      //mapped to print key...

		if(keypress_ != ky)
		{
			pressed_ = 0;
			tot_buttonpress_tmr = 0;  //clr timer.
		}
		 if((tot_buttonpress_tmr >= 3) && (pressed_ == 0) )
		 {
		   tot_buttonpress_tmr = 3;
			pressed_ = 1;
			return 1;
		 }
	}

	 return 0;
}
/////////////////////////////////////////////////////////////////
eSystemState operator_State_Handler(void)
{
	    static int8_t fxn = 0;
        static int8_t indx1 = 0,
        		   	  indxx1 = 0;

	    static int8_t pump_indx = 1;
	    static int8_t pump_indxx = 0;

	    extern  log_max;
	    static int8_t key19_sto_ = 0,
	    			  keyy19_sto_ = 0;

	    int8_t pkey = 0;
	    int8_t keyy19 = 0;
	    extern int  printer_status;

	    uint8_t rd19 = 0;

   //===========================================================
	    // POWER FAILURE SENSE
   //===========================================================

	if(settings_stream1[0].mode == AUTO_MODE)
	{
		if(timer_go >= TIMEOUT_GO)   //if go's timeout is 5sec threshold
		{
//			pump_status_1 = STATUS_PNP;
//			return pnp_State;
		}
	}


	#if sense_power == 1
		  	  if(readpwr() == 0)
			  {
		  		   modem_power(DEACTIVATE);

		  		   //count time elapsed
				   if (shutdown_timer1 > 120)
				   {
					   displayandkeypad_power(DEACTIVATE);   //shutdown... after  2 minutes
				   }
			  }
		  	  else
		  	  {
		  		  //clear the timer//
		  		  shutdown_timer1 = 0;
		  	  }
	#endif
	//===========================================================



	 //===========================================================

	   if(readNozzle1() == 1)      //@Nozzle-up, the below features should be disabled
	   {
		   nozzleUp_inProgMode1 = 1;

		   indx1 = 0;
		   indxx1 = 0;

		   return idle_State;
	   }

    //===========================================================
  	    rd19 = readkey19_state();

		if ( (rd19 == 1) && ( key19_sto_ == 0) )
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
	  if ( (operatorfxn == totaliser_view) )
		{
		  // level 0. totaliser
		   if (indx1 == 0 ) indx1 = 1;
		   if (indxx1 == 0 ) indxx1 = 1;
		  // -------------- test keys....----------------
		  	     pkey = read_keypad();

		  	         if( (pkey > 0) )  //key detected
		  	         {
		  	        	 if (pkey == 'B')  // up key  --> KeyF2
		  					{
		  	        		  //indx1++;
		  					    if (indxx1 == 1)
		  					         	indxx1 = 2;
		  					    else
		  					    		indxx1 = 1;
		  					}

		  	        	 else if (pkey == 'C')  // down key
		  					{
		  	        		   //indx1--;
		  						if (indxx1 == 1)
		  							indxx1 = 2;
		  						else
		  							indxx1 = 1;
		  					}

		  	        	 else if (pkey == 'A')  // back key
							{
		  	        		lcd_print_line1(upper1);
		  	        		   lcd_print_line2(middle1);

							   char str__[10]= {0};
						  	   snprintf(str__, sizeof(str_), "%.2f", litre_price);
							   lcd_print_line3(str__);

							   indx1 = 0;
							   indxx1 = 0;

	                           return idle_State;  //go back to idle state.
							}
		  	         } //if pkey

     	 //-----------------------------------------------------
			   if (t >= 500)
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
		  	    	  if (indx1 == 1)
		  	    	  {
		  	    		  if(indxx1 == 1)
		  	    		  {
							  res = snprintf(scc, sizeof(scc), "%.2f", totaliser_vol1c);
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
								   clear_screen1();
								   printDisp_c("l", 1, 0, 8, LT, CLEAR);
								   printDisp_c(line1, 1, (8-jj), 5, LT, NOCLEAR);  //display price
//								   printDisp_c(line1, 1, 0,5,RT,CLEAR );
								   lcd_print_line2(line2);
								 }
								 else
								 {
									 lcd_print_line1("l        ");
									 lcd_print_line2("        ");
									 printDisp_f(totaliser_vol1c, 2, 0, 5, RT, CLEAR ); //lcd_print_line2(scc);
								 }
							  lcd_print_line3("tot A");
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
								   clear_screen1();
								   printDisp_c("p", 1, 0, 8, LT,CLEAR);
								   printDisp_c(line1, 1, (8-jj), 5, LT, NOCLEAR);
//								   printDisp_c(line1, 1, 0,5,RT,CLEAR );
								   lcd_print_line2(line2);
								 }
								 else
								 {
									 lcd_print_line1("p        ");
									 lcd_print_line2("        ");
									 printDisp_f(totaliser_amt1c, 2, 0, 5, RT, CLEAR ); //lcd_print_line2(scc);
								 }
							  lcd_print_line3("tot A");
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
							   clear_screen1();
							   printDisp_c("l",1,0,8,LT,CLEAR);
							   printDisp_c(line1,1,(8-jj),5,LT,NOCLEAR);
//							   printDisp_c(line1, 1, 0,5,RT,CLEAR );
							   lcd_print_line2(line2);
							 }
							 else
							 {
								 lcd_print_line1("l        ");
								 lcd_print_line2("        ");
								 printDisp_f(totaliser_vol2c, 2, 0,5,RT,CLEAR ); //lcd_print_line2(scc);
							 }
						  lcd_print_line3("tot B");
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
							   clear_screen1();
							   printDisp_c("p",1,0,8,LT,CLEAR);
							   printDisp_c(line1,1,(8-jj),5,LT,NOCLEAR);
//							   printDisp_c(line1, 1, 0,5,RT,CLEAR );
							   lcd_print_line2(line2);
							 }
							 else
							 {
								 lcd_print_line1("P        ");
								 lcd_print_line2("        ");
								 printDisp_f(totaliser_amt2c, 2, 0,5,RT,CLEAR ); //lcd_print_line2(scc);
							 }
						  lcd_print_line3("tot B");
						 }  //  if (indx1 == 2)
					   }

					   t = 0;    //reset the timer.
		            }  //if (t > 500)
	      return operator_State;
	  } ///if ( (auth == no_auth)  )

//================= End of totaliser view. ========================
//=================================================================


//=================================================================
//                        log view.
//=================================================================
	         if (  (operatorfxn == log_view) )
	          	{
	        	    static int loop_ = 0;
	            	static int log_indx = 0;
	            	static int log_indx_indx = 0;

	            	static uint32_t flash_loc;
	            	static uint32_t log_no;

	            	float pr_  =  price_real1;
					float vl_  =  amt_real1;
					char *tm_  =  "---------";

	            	if(loop_ == 0)    //do this only the first time
					{
						// data_size = sizeof(log_a_new);
					  if (operating_side == side_a)
					  {
							 flash_loc = flash_infoA.current_loc; // - 256;
							 log_no = flash_infoA.number_logs;
							 if(log_no == 0)
							 {
								  pr_  =  0.00; //price_real1;
								  vl_  =  0.00; //amt_real1;
								  tm_  =  "--------";
							 }
							else
							{
							  //calculate the previous save id from the current
								if (flash_loc <= flash_beginA) //at begining of storage
								{
									flash_loc = flash_endA;    // - data_size; //wrap
								}
								else
								{
								   flash_loc = flash_loc - data_size;
//								    	   if (flash_loc == 0 )  // flash_beginA) //at begining of storage
//											{
//								    			// this shouldnt occur ,  just in case !
//								    			flash_loc = flash_endA - data_size; //wrap
//											}
//								    		   uint32_t next_loc = flash_infoA.current_loc + 256;  //sizeof(log_a_new);
//											 if (next_loc > flash_endA) next_loc = flash_beginA;   //flash_endA => 0x3FFFFF --> 4,194,303 pg16,383.996
//											 flash_infoA.current_loc  =  next_loc;
//											 flash_infoA.number_logs  =  flash_infoA.number_logs + 1;
//											 EEPROM_Write(flash_info_sto, flash_stoA, &flash_infoA, sizeof(flash_infoA));
								}
							}
							   flash_locc = flash_loc;
						}
//						  if (operating_side == side_b)
//							   {
//								 flash_loc = flash_infoB.current_loc;
//								 log_no = flash_infoB.number_logs;
//								 if(log_no == 0)
//									 {
//										  pr_  =  0.00; //price_real1;
//										  vl_  =  0.00; //amt_real1;
//										  tm_  =  "--------";
//									 }
//								    else
//									{
//								    	//calculate the previous save id from the current
//										if (flash_loc <= flash_beginB) //at begining of storage
//										{
//											flash_loc = flash_endB - data_size; //wrap
//										}
//										else
//										{
//											flash_loc = flash_loc - data_size;
//											if (flash_loc < flash_beginB) //at begining of storage
//											{
//												// this shouldnt occur ,  just in case !
//												flash_loc = flash_endB - data_size; //wrap
//											}
//										}
//										    flash_locc = flash_loc;
//									}
//							   }
	            		  loop_ = 1;   //disable first time assignment ^^^^
	            		  log_indx_indx = log_no;
	            		  pump_indx  = 1;  // initialise the side...

	            		  flash_read_idA = flash_loc;

	            		  return read_flash_State;  //goto read the flash location...
						}
	         //==================================
	         //   display the log parameters....

	       		 if(pump_indx == 0) pump_indx = 1;
	         // -------- test keys....-----------
	          	  	     pkey = read_keypad();
//	          	  	     keyy19 = readkey19_state();

	       if(keypress_ == 21)
	       {
               HAL_Delay(1000);
	    	   compose_printer();
	    	   printer_status = 1;
	    	   print__1();   //print the transaction.
	       }

	       if(printer_status == 1)   print__1();   //print the transaction.

		   if( (pkey > 0) )  //key detected
			 {
//				   if (pkey == 'C')  // up key  //+
					if (pkey == 'B')  // up key  //+
					{
						if (log_indx_indx < log_no)
						{
							log_indx_indx++;  //incr index only when less than max
						if (operating_side == side_a)
							{
								if(flash_loc >= flash_endA)
								{
								  flash_loc = flash_beginA; //wrap
								}
								else
								{
								  flash_loc +=  data_size;
								}
								flash_read_idA = flash_loc;
								flash_locc = flash_loc;


//												if (next_loc > flash_endA) next_loc = flash_beginA;   //flash_endA => 0x3FFFFF --> 4,194,303 pg16,383.996
//												 flash_infoA.current_loc  =  next_loc;
							}
//	          	  					    if (operating_side == side_b)
//											{
//	          								if(flash_loc >= flash_endB)
//												{
//												  flash_loc = flash_beginB; //wrap
//												}
//												else
//												{
//												  flash_loc += data_size;
//												}
//												flash_read_idB = flash_loc;
//												flash_locc = flash_loc;
//											}
								 }

							  //  flash_read_idA = flash_loc;
										//flash_beginA + ( log_indx_indx *  );
								//fetch the values...
								return read_flash_State;
							}

						 if (pkey == 'C')  // down key   //-
							{
								if (log_indx_indx >= 2)
								{
									log_indx_indx--; //reduce index only when greater than 0.

								 if (operating_side == side_a)
									{
										if(flash_loc <= flash_beginA)
										{
										  flash_loc = flash_endA - data_size; //wrap
										}
										else
										{
										  flash_loc -= data_size;
										}
									  flash_locc = flash_loc;
									  flash_read_idA = flash_loc;
								}

//											if (operating_side == side_b)
//											{
//												if(flash_loc <= flash_beginB)
//												{
//												  flash_loc = flash_endB - data_size; //wrap
//												}
//												else
//												{
//												  flash_loc -= data_size;
//												}
//												flash_locc = flash_loc;
//												flash_read_idB = flash_loc;
//											}
										//flash_read_idA = flash_loc;
										//fetch the values...
									  }
									return read_flash_State;
								}

//							 if (pkey == 'A')  // back key
								if (pkey == 'A')  // stop/exit key
								{
								  loop_ = 0;  // enable the first time assignment
								  lcd_print_line1(upper1);
								   lcd_print_line2(middle1);

								   char str__[8]= {0};
								   snprintf(str__, sizeof(str__), "%.2f", litre_price);
								   lcd_print_line3(str__);
								   return idle_State;  //go back to idle state.
								}

						   /*  if (pkey == 'F')  //change pump index.
								{
								  pump_indx++;
									if (pump_indx > 2) pump_indx = 1; //wrap around
								}   */
						 } //if pkey
			 //-----------------------------------------------------
				   if (t >= 500)
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
//	          	  	    	  if (pump_indxx == 1)
//	          	  	    	   {
								  pr_  =  log_a_new.pr__;   //price
								  vl_  =  log_a_new.vol__;  //volume
								//  tm_  =  log_a_new.tm_;   //time

	          	  	    		  //dd-hh:mm,    P  , L  ,    T
	          	  	    		  printDisp_c("p",1,0,8,LT,CLEAR); printDisp_f(pr_,1,0,8,LT,NOCLEAR);  //display price

	                              if (scroll_ == 1 )
								   {
									 printDisp_c("l",2,0,8,LT,CLEAR); printDisp_f(vl_,2,0,8,LT,NOCLEAR);  //display litre
								   }
	                              else if (scroll_ == 2)
									{
	                            	  char st__[10]= {0};
	                            	  snprintf(st__, sizeof(st__), "%d-%02d-%d",log_a_new.date._dd,log_a_new.date._mm,log_a_new.date._yy);
									 printDisp_c(st__,2,0,8,LT,CLEAR);
									}
	                              else if (scroll_ == 3)
	                              {
	                             	  char st__[10]= {0};
									  snprintf(st__, sizeof(st__), "%d%02d hrs",log_a_new.time_e._hh,log_a_new.time_e._mn);
									 printDisp_c(st__,2,0,8,LT,CLEAR);
	                              }
	                              	  snprintf(line3, sizeof(line3), "An1.%d",log_indx_indx);
	                                  lcd_print_line3("      ");
	                              	  lcd_print_line3(line3);
//	          	  	    	   }  //  if (indx1 == 1)

	          	  	    	 //-----------------------------------------------------

//	          				   if (pump_indxx == 2)
//	          				      {
//									  pr_  =  log_b_new.pr__;   //price
//									  vl_  =  log_b_new.vol__;  //volume
//									  //  tm_  =  log_b_new.tm_;   //time
//
//
//									  //dd-hh:mm,    P  , L  ,    T
//									 printDisp_c("p",1,0,8,LT,CLEAR); printDisp_f(pr_,1,0,8,LT,NOCLEAR);  //display price
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
//	          					    snprintf(line3, sizeof(line3), "bn1.%d",log_indx_indx );
//	          					    lcd_print_line3("      ");
//	          					    lcd_print_line3(line3);
//	          				      }  //  if (indx1 == 2)
	           	             }
							 else
							 {
								 lcd_print_line1(" No log ");
							   lcd_print_line2("        ");
							   lcd_print_line3("      ");
							 }   // if log  > 0.

	          		       t = 0;    //reset the timer.
	          	        }  //if (t > 500)

	                return operator_State;
	            } ///if ( (auth == no_auth) && (access == level0) && (fxn == 1) )
	return operator_State;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

eSystemState error_clear_Handler(void)
{
	if(irrecov_flag == 0)
	{
		clear_screen1();

		if(settings_stream1[0].display_format == PL)
		  {
			lcd_print_line1(upper1);
			 lcd_print_line2(middle1);
		  }
		  else if(settings_stream1[0].display_format == LP)
		  {
			  lcd_print_line1(middle1);
			  lcd_print_line2(upper1);
		  }


		char str__[8]= {0};
		snprintf(str__, sizeof(str_), "%.2f", litre_price);
		lcd_print_line3(str__);
		error_clr_flag = 0;  //clear flag..

//			if(calibration1_error == 1)
//			{
//				calibration1_error = 0;
//			}
//
//			else

		if(changeLitrePrice1_2 == 1)
		{
			changeLitrePrice1_2 = 0;

//				if(settings_stream1[0].mode == AUTO_MODE)
//				{
//					if(pump_status_ != STATUS_FILLING)
//					{
//
//					}
//				}
		}

		else if(totalizer1_error == 1)
		{
			totalizer1_error = 0;
		}

//			else if(HAL_GPIO_ReadPin(pulser1_detect_GPIO_Port, pulser1_detect_Pin) == 0 )
//			{
//
//			}



		return idle_State;
	}
	else
	 {
		return inactive_State;  //if the error is an irrecoverable error dont return to idle state.
	 }
}
//============================================================================================================
uint8_t key_available()
{
	if ( keypress__ > 0)
		return 1;
	else
		return 0;
}
//========================================================================
int8_t read_keypad()
{
   //uint8_t ret = 0;
   int temp = 0;

  if ( keypress__ > 0)  //If key available.
     {
       keypress__ = 0;
          if( (kkey >= '0' )&&(kkey <= '9') )//if numerals
          {
        	if(_index > 7)
        	  {
        		  _index  = 7; temp = 7;
        	  }
            	  else
        	  {
            	  temp = _index;
        		  keypad_buf[_index++] = kkey;
        		  keypad_buf[_index  ] = 0;

        		  keypad_pw_xter1[temp++] = '-';
        		  keypad_pw_xter1[temp  ] = 0;

        		  if ((progg == 1) && (auth == not_auth))
        			  keypad_print(keypad_pw_xter1);
        	  }
                return 0;
          }

          else if (kkey == 'F' )  //if clear key,
			 {
               _index = 0; temp = 0;
               for(int i = 0;i<10;i++)
                {
                	keypad_pw_xter1[i] = 0;
                	keypad_buf[i] = 0;
                }
               //lcd_print_line2("        ");  //clear second line.
               return 'F';
			 }

          else if (kkey == 'D' )  //if ENTER key.
			 {
        	    return 'D';
			 }

          else if (kkey == 'B' )  //if UP key.    // p/l -
			 {
				return 'B';
			 }

          else if (kkey == 'C' )  //if DOWN key.
			 {
				return 'C';
			 }

          else if (kkey == 'F' )  //if clear key.
			 {
				return 'F';
			 }

          else if (kkey == 'A' )  //if back key.    // stop/exit
			 {
				return 'A';
			 }
          else if (kkey == 'G' )  //if F1 key.
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
void clr_screen1(void)
{
	lcd_print_line1("        ");
      lcd_print_line2("        ");
      lcd_print_line3("        ");
}

//=============================================================================
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//=============================================================================
eSystemState progState_Handler(void)
{
	static int indx = 0;
//	static int index_menu = 0;
	static int indx2 = 0;
	static int indx3 = 0;
	static int attempts = 0;
    static int fxn = 0;
    static int pump_indx = 1;
    extern  log_max;
   //progg = 1;
   int temp = 0;
   int pkey = 0;
   static  int access = non;
   static int key19_sto_ = 0;
   static int dp_ = 0;
   static int ln_ = 0;

   static uint8_t firstTime = 1,
		   	   	  enter_key = 0;
//				  index_pass = 0;

   static uint8_t index_generic = 0;
//
//   static uint16_t otp_seed1;

   char st__[10] = {0},
		pass_[9];

   uint8_t rd19 = 0;

   char *endPtr;

#ifndef DEV_MODE
   if(prog_revisitt1 == 1)
   {
		indx = 0;
		//	static int index_menu = 0;
		indx2 = 0;
		indx3 = 0;
		attempts = 0;
		fxn = 0;
		pump_indx = 1;
		log_max;
		//progg = 1;
		temp = 0;
		pkey = 0;
		access = non;
		key19_sto_ = 0;
		dp_ = 0;
		ln_ = 0;

		firstTime = 1;
		enter_key = 0;

		prog_entry1 = 0;

		prog_revisitt1 = 0;
   }
#endif  //#ifndef DEV_MODE


   if(prog_entry1 == 0 )   // first entry...
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

	     auth = not_auth;

	     access = non;

	     prog_entry1 = 1;  //To prevent clearing variables on re-entry.
   }


 //===========================================================

   //if(read_event1() == _nozzleup_Event)
//   if(readNozzle1() == 1)  //@Nozzle-up, the below features should be disabled
//   {
//	   nozzleUp_inProgMode1 = 1;
//
//	   return idle_State;
//   }

 //===========================================================

//   else if (auth == not_auth)
   if (auth == not_auth)
   {
      //clear keypad....
	  //acquire the password goto level0 access level if non supplied.

	   pkey = read_keypad();
//	   char pass_[9];

	  if(pkey > 0)  //key detected
	   {
    	   if (pkey == 'D') // enter key...
    	   {
              //password provided...  settings[0].passwd1
			   strncpy(pass_, keypad_buf, sizeof(pass_));

				   if( (index_menu == pass1) && (strcmp(pass_, password_level1) == 0) ) //level 1 access ?
				   {
					   index_menu = 0;
					   auth = authed;
					   access = level1;
					   fxn = nothing;
					   clear_buffer1();
						//keypad_buf[0] = 0;
						//keyboard_entry[0] = 0;
					   return prog_State;
				   }
				   else if( (index_menu == pass2) && (strcmp(pass_, password_level2) == 0) ) //level 2 access ?
				   {
					   index_menu = 0;
					   auth = pre_otp_authed;
					   access = level2;
					   fxn = nothing;
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
				   else if( (index_menu == pass3) && (strcmp(pass_, password_level3) == 0) ) //level 3 access ?
				   {
					   index_menu = 0;
					   auth = pre_otp_authed;
					   access = level3;
					   fxn = nothing;
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
				   else if(access == non)             //no match...
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
					  lcd_print_line2(keypad_pw_xter1);
					  lcd_print_line3("        ");
//					  index_menu = 0;
				   }
    	  }// ================ if ENTER KEY PRESSED =================

    	   else if (pkey == 'F') // cancel key...
		   {
			  clear_buffer1();
			  /* keypad_buf[0] = 0;
			   keyboard_entry[0] = 0;
			   keypad_pw_xter1[0] = 0;
			   _index = 0; */
		   }
    	   else if (pkey == 'A') // stop/exit key...
		   {
    		    clear_buffer1();
				prog_entry1 = 0;
				prog_revisit1 = 1;
				index_menu = 0;

				#ifndef DEV_MODE
					prog_revisitt1 = 1;
				#endif //#ifndef DEV_MODE

			  return idle_State;

			  /* keypad_buf[0] = 0;
			   keyboard_entry[0] = 0;
			   keypad_pw_xter1[0] = 0;
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

	    if (t >= 500)  //time to display?
		{
	    	if(index_menu == 3) index_menu = 2;

//	    	lcd_print_line1("  pass  ");
	    	lcd_print_line1(pass[index_menu]);  //show password level menu
			lcd_print_line2(keypad_pw_xter1);
			lcd_print_line3("        ");
			t = 0;
		}
	    return prog_State;  //exit...
   }   //if (not_auth)


//================================== MODE =================================
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//=========================================================================
//                           MENU DISPLAY & SCROLL
//=========================================================================
   else if ( (auth == authed) && (access == level1) && (fxn == nothing)  )
   {
	   if (t >= 500)
		 {
			if(index_menu == 3) index_menu = 2;
			lcd_print_line1(menu_level1[index_menu]);   //show menu item.
			lcd_print_line2("        ");
			t = 0;
		 }
	 // -------------- test keys....----------------
	 pkey = read_keypad();
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

//						if (pkey == 'B')  // down key
			if (pkey == 'C')  // down key
			{
				if (index_menu > 0) index_menu--;
				else if(index_menu == 0) index_menu = 2;
			}

			 if (pkey == 'A')  // BACK key...
				   {
					   auth = authed;
					   access = level1;
					   fxn = nothing;
					   clear_buffer1();
					  // keypad_buf[0] = 0;
				   }

			 if (pkey == 'D')  // enter key
				{
					   switch(index_menu)
						{
					   	   	  case 0 :
									fxn = shift_total;
									clear_buffer1();
									clr_screen1();
									break;

					   	   	  case 1 :
									fxn = setprice;
									pump_indx = 1;
									clear_buffer1();
									clr_screen1();
									break;

							  case 2 :
									fxn = setclock;
									clear_buffer1();
									clr_screen1();
									break;
						}
				}
		 }
	 return prog_State;
   }

   //==================================================================================================


   else if ( (auth == pre_otp_authed) && ( (access == level2)||(access == level3) ) && (fxn == nothing)  )
   {
		if (t >= 300)
   		{
			lcd_print_line1("OTP Seed");

   		  snprintf(st__, sizeof(st__), "    %d", otp_seed1);
   		  lcd_print_line2(st__);

			for(uint8_t ii = 0 ; ii < 7; ii++)
			{
				keyboard[ii] = st__[ii+1];
				keyboard[ii+1] = 0;
			}
			keypad_print(keyboard);


   		  lcd_print_line3("  A  ");

   		  t = 0;
   		}

      	 // -------------- test keys....----------------
      	 pkey = read_keypad();
      	   if(pump_indx == 0) pump_indx = 1;

      		 if( (pkey > 0) )  //key detected
      		 {
//      			 if (pkey == 'A')  // BACK key...
//				   {
//					   auth = authed;
//					   access = level2;
//					   fxn = nothing;
//					   clear_buffer1();
//					  // keypad_buf[0] = 0;
//				   }

      			 if (pkey == 'D')  // enter key
      			 {
				   index_menu = 0;
				   auth = otp_authed;
				   if(access == level2) access = level2;
				   else if(access == level3) access = level3;

				   otp_codeInt1 = getCode(otp_seed1);

				   sprintf(otp_code1, "%06ld", otp_codeInt1);

				   memset(keypad_buf, '\0', sizeof(keypad_buf));

				   fxn = nothing;
				   clear_buffer1();
      			 }
      		 }
      	 return prog_State;
     }
             //      end of show menu for level1

   //=================================================================================================


	else if ( (auth == otp_authed) && ( (access == level2)||(access == level3) ) && (fxn == nothing)  )
	{
		if(keypad_buf[0] == NULL)
	    {
			if ( (t >= 300) && (t <= 500) )
			{
				lcd_print_line1("   OTP  ");

			  lcd_print_line2("_       ");
			}
			else if ( (t > 500) && (t <= 700) )
			{
				  lcd_print_line2("        ");
			}


			  lcd_print_line3("  A  ");

			  if (t > 700) t = 0;
	   }

		else if (t >= 300)
		{
			lcd_print_line1("   OTP  ");

		 lcd_print_line2(keypad_buf);

		  lcd_print_line3("  A  ");

		  t = 0;
	   }

		 // -------------- test keys....----------------
		 pkey = read_keypad();
		   if(pump_indx == 0) pump_indx = 1;

			 if( (pkey > 0) )  //key detected
			 {
	//      			 if (pkey == 'A')  // BACK key...
	//				   {
	//					   auth = authed;
	//					   access = level2;
	//					   fxn = nothing;
	//					   clear_buffer1();
	//					  // keypad_buf[0] = 0;
	//				   }

				 if (pkey == 'D')  // enter key
				 {
				   strncpy(pass_, keypad_buf, sizeof(pass_));

			#ifdef OTP_ENABLE
				   if(strcmp(pass_, otp_code1) == 0)  //level 2 0r 3 access ?
				   {
//					   save_otp(side_a);
					   save_otpSeed_session_fram(side_a);
			#else
				   if( (strcmp(pass_, otp_code1) == 0) || (strcmp(pass_, otp_code1) != 0) )  //level 2 0r 3 access ?
				   {
			#endif      //#ifdef OTP_ENABLE
					   index_menu = 0;
					   auth = authed;
					   if(access == level2) access = level2;
					   else if(access == level3) access = level3;
					   fxn = nothing;
					   clear_buffer1();
				   }
				   else
				   {
					   lcd_print_line1(" Access ");
					   lcd_print_line2(" Denied ");
					   lcd_print_line3("        ");
					   HAL_Delay(1000);

					   memset(keypad_buf, '\0', sizeof(keypad_buf));
					   clear_buffer1();
					   clear_screen1();
				   }
				 }
			 }
		 return prog_State;
	}

  //=================================================================================================

   else if ( (auth == authed) && ( (access == level2)||(access == level3) ) && (fxn == nothing)  )
   {

	   if (t >= 500)
   		 {
   			if(access == level2)
   			{
   				if(index_menu == 20) index_menu = 0;
   				lcd_print_line1(menu_level2[index_menu]);   //show menu item.
   			}
   			else if(access == level3)
			{
   				if(index_menu == 23) index_menu = 0;
   				lcd_print_line1(menu_level3[index_menu]);   //show menu item.
			}

   			lcd_print_line2("        ");
   			t = 0;
   		 }
   	 // -------------- test keys....----------------
   	 pkey = read_keypad();
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
			   auth = authed;
			   if(access == level2) access = level2;
			   else if(access == level3) access = level3;
			   fxn = nothing;
			   clear_buffer1();
			  // keypad_buf[0] = 0;
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
   									clear_buffer1();
   									clr_screen1();
   									break;

   							  case setaddress:
   									pump_indx = 1;
   									fxn = setaddress;
   									//clear the keyboard buffer
   									//index_ = 0;
   									clear_buffer1();
   									clr_screen1();
   								    break;

   							  case setnozzle:
   									pump_indx = 1;
   									fxn = setnozzle;
   									clear_buffer1();
   									clr_screen1();
   									break;

   							  case display_orientation:
   								   pump_indx = 1;
   								   fxn = display_orientation;
   								   clear_buffer1();
   								   clr_screen1();
   								   break;

   							  case setprice:
   									fxn = setprice;
   									pump_indx = 1;
   									//clear the keyboard buffer
   									clear_buffer1();
   									clr_screen1();
   									//index_ = 0;
   									//keyboard_entry[0] = 0;
   									//keyboard_entry[1] = 0;
   									//copy[0].price_ = litre_price1;
   									//copy[1].price_ = litre_price2;
   									break;

   							  case set_no_flow_time:
   									fxn = set_no_flow_time;
   									clear_buffer1();
   									clr_screen1();
   									break;

   							  case setmax_amt:
   								   fxn = setmax_amt;
   								   clear_buffer1();
   								   clr_screen1();
   								   break;

   							  case set_password:
   									attempts = 0;
   									fxn = set_password;
   									clear_buffer1();
   									clr_screen1();
   									break;

//   							  case calibrate_:
   							  case 8 :
   								   fxn = calibrate_;
   								   clr_screen1();
   								   clear_buffer1();
   								   break;
//   							  case display_:
   							  case 9 :
   								   fxn = display_;
   								   clear_buffer1();
   								   clr_screen1();
   								   dp_ = 1;
   								   ln_ = 1;
   								   break;

//   							  case setclock:
   							  case 10 :
   									fxn = setclock;
   									clear_buffer1();
   									clr_screen1();
   									break;

   							//"Sides No"
   							  case 11 :
									fxn = number_of_sides;
									clear_buffer1();
									clr_screen1();
									break;

							  //"Start CL"
   							  case 12:
									fxn = suppressed_display_vol;
									clear_buffer1();
									clr_screen1();
									break;

							  //"Calib.Can"
   							  case 13:
									fxn = calibration_can;
									clear_buffer1();
									clr_screen1();
									break;

							  //"Shift Type"
   							  case 14:
									fxn = shift_login_type_;
									clear_buffer1();
									clr_screen1();
									break;

							  //"Shift No"
							  case 15:
									fxn = number_of_shifts_;
									clear_buffer1();
									clr_screen1();
									break;

							  //Key Tone
   							  case 16:
									fxn = keypress_tone_;
									clear_buffer1();
									clr_screen1();
									break;

							  case 17:
									fxn = pulser_config;
									clear_buffer1();
									clr_screen1();
									break;

							  //"valve_config"
							  case 18:
									fxn = valve_config;
									clear_buffer1();
									clr_screen1();
									break;

							  //"no_calibration"
							  case 19:
									fxn = no_calibration;
									clear_buffer1();
									clr_screen1();
									break;

							  //Calib.Typ
							  case 20:
									fxn = calibration_type_;
									clear_buffer1();
									clr_screen1();
									break;

							  //"Com Card"
							  case 21:
									fxn = communication_card;
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

  	   if (t >= 500)
  		 {
  			lcd_print_line1(menu1[index2]);
  			t = 0;
  		 }                                     */


  //================================= MODE ==================================
  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  //=========================================================================
  //            SET MODE { ( ONLINE (AUTO) / OFFLINE (MANUAL) }
  //=========================================================================
   else if ( (auth == authed) && ( (access == level2)||(access == level3) ) && (fxn == setmode)  )
   {
	   if (t >= 300)
		 {
			//lcd_print_line1();
//			if(copy[0].mode == AUTO)
			   if(copy_stream1[0].mode == AUTO_MODE)
				{
					lcd_print_line2(" Auto ");
				}
			   else
				{
					lcd_print_line2("NNanual ");
				}

//			 if (pump_indx == 1)
//				 {
					//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
					lcd_print_line3("  A  ");
//				 }
//			 if (pump_indx == 2)
//				 {
//					//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
//					lcd_print_line3("  B  ");
//				 }
			t = 0;
		 }
	// -------------- test keys....----------------
	rd19 = readkey19_state();

	if ( (rd19 == 1)&&( key19_sto_ == 0) )
	 {
		pump_indx++;
		if (pump_indx > 2) pump_indx = 1;  //wrap around
		   key19_sto_ = rd19;
	 }
	key19_sto_ = rd19;

	 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
				{
				  if (copy_stream1[0].mode == AUTO_MODE) //;
					copy_stream1[0].mode = MANUAL_MODE;
				  else
					  copy_stream1[0].mode = AUTO_MODE;
				}

			 else if (pkey == 'C')  // down key
				{
					if (copy_stream1[0].mode == AUTO_MODE) //;
						copy_stream1[0].mode = MANUAL_MODE;
					  else
						  copy_stream1[0].mode = AUTO_MODE;
				}

			 else if (pkey == 'F')  //change pump index.
				{
				  // pump_indx++;
					//if (pump_indx > 2) pump_indx = 1; //wrap around
				}

			 else if (pkey == 'D')  // back key
			 {
				  if (copy_stream1[0].mode == AUTO_MODE) //;
					  copy_stream1[0].def_t = P;
				  else if (copy_stream1[0].mode == MANUAL_MODE)
					  copy_stream1[0].def_t = P;
//				  else if (copy[0].mode == VOUCHER_ONLY)
//					  copy[0].def_t = V;

				fxn = nothing;
 //				lcd_print_line3("      ");
				clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
				{
					fxn = nothing;
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
   else if ( (auth == authed) && ( (access == level2)||(access == level3) ) && (fxn == setnozzle)  )
   {
	   if (t >= 300)
	   {
			//lcd_print_line1();
		  if(copy_stream1[0].noz_override == override)
		  {
			lcd_print_line2("Active");
		  }
		  else
		  {
			lcd_print_line2("Inactive");
		  }

//		 if (pump_indx == 1)
//		 {
			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
			lcd_print_line3("  A  ");
//		 }
//		 if (pump_indx == 2)
//		 {
//			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
//			lcd_print_line3("  B  ");
//		 }
		t = 0;
	   }
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( key19_sto_ == 0) )
		 {
			pump_indx++;
			if (pump_indx > 2) pump_indx = 1;  //wrap around
			   key19_sto_ = rd19;
		 }
		 key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
				{
				 if (copy_stream1[0].noz_override == override)
					 copy_stream1[0].noz_override = nooverride;
				 else
					 copy_stream1[0].noz_override = override;
				}

			 else if (pkey == 'C')  // down key
				{
				 if (copy_stream1[0].noz_override == override)
					 copy_stream1[0].noz_override = nooverride;
				 else
					 copy_stream1[0].noz_override = override;
				}

			 else if (pkey == 'F')  //change pump index.
			 {
			  // pump_indx++;
			  // if (pump_indx > 2) pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
				 fxn = nothing;
 				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
				{
					 fxn = nothing;
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
   else if ( (auth == authed) && ( (access == level2)||(access == level3) ) && (fxn == display_orientation)  )
   {

	   if (t >= 300)
	   {
//		   if( (index_generic !=  PL) && (index_generic !=  LP) ) index_generic = PL;

		   if(copy_stream1[0].display_format == PL)
//		   if(index_generic ==  PL)
		   {
			   lcd_print_line1("PL NNode");
		   }
		   else
		   {
			   lcd_print_line1("LP NNode");
		   }

		  lcd_print_line3("  A  ");

			t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( key19_sto_ == 0) )
			 {
				pump_indx++;
				if (pump_indx > 2) pump_indx = 1;  //wrap around
				   key19_sto_ = rd19;
			 }
		 key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
			 {
//				 if (index_generic == PL)  index_generic = LP;
//				 else if(index_generic == LP) index_generic = PL;
				 if(copy_stream1[0].display_format == PL)
					copy_stream1[0].display_format = LP;
				 else if(copy_stream1[0].display_format == LP)
					copy_stream1[0].display_format = PL;
			 }

			 else if (pkey == 'C')  // down key
			 {
//				 if (index_generic == LP)  index_generic = PL;
//				 else if(index_generic == PL) index_generic = LP;
				 if(copy_stream1[0].display_format == PL)
					copy_stream1[0].display_format = LP;
				 else if(copy_stream1[0].display_format == LP)
					copy_stream1[0].display_format = PL;
			 }

			 else if (pkey == 'F')  //change pump index.
			 {
				  // pump_indx++;
				  //if (pump_indx > 2) pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
//				 if(index_generic == PL)
//				 {
//					 copy_stream1[0].display_format = PL;
//					 copy_stream1[0].def_t = P;
//				 }
//				 else if(index_generic == LP)
//				 {
//					 copy_stream1[0].display_format = LP;
//					 copy_stream1[0].def_t = L;
//				 }

				 if(copy_stream1[0].display_format == PL)
					 copy_stream1[0].def_t = P;
				 else if(copy_stream1[0].display_format == LP)
					 copy_stream1[0].def_t = L;

				 fxn = nothing;
				 index_generic = 0;
				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
			 {
				 fxn = nothing;
				 index_generic = 0;
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
   else if ( (auth == authed) && ( (access == level1)||(access == level2)||(access == level3) ) && (fxn == setprice) )
   {
	 if (t >= 300)
	  {
		 //copy[0].price_ =  atoi(keyboard_entry);
		 printDisp_f(copy_stream1[0].price_, 1, 0, 6, RT, CLEAR);
		 float pr = strtof(keyboard_entry, &endPtr);
		 printDisp_f(pr,2,0,7,RT,CLEAR);

//		 if (pump_indx == 1)
//		 {
		   lcd_print_line3("  A  ");
//		 }

//		 if (pump_indx == 2)
//		 {
//			 lcd_print_line3("  B  ");
//		 }
		t = 0;
	  }
	 // -------------- test keys....----------------
	  rd19 = readkey19_state();

	   if ( (rd19 == 1) && ( key19_sto_ == 0) )
		 {
		   keyboard_entry[0] = 0;   //clear the previous entry...
		   index_ = 0;
		   pump_indx++;
			if (pump_indx > 2) pump_indx = 1;  //wrap around
			   key19_sto_ = rd19;
		 }
		   key19_sto_ = rd19;


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
//				 if (pump_indx == 1)   // if side A
//				 {
					 copy_stream1[0].price_   = strtof(keyboard_entry, &endPtr);
//				 }
//				 if (pump_indx == 2)   // if side B
//				 {
//					 copy_stream1[1].price_   = strtof(keyboard_entry, &endPtr);
//				  }

				 fxn = nothing;
				 clr_screen1();
			 }

			 if (pkey == 'A')  // back key
				{
					 fxn = nothing;
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
   else if ( (auth == authed) && ( (access == level2)||(access == level3) ) && (fxn == setaddress)  )
   {
	   if (t >= 500)
	   {
			printDisp_i(copy_stream1[0].noz_addr, 1, 0, 4, RT, CLEAR);
			lcd_print_line2(keyboard_entry);

//			if (pump_indx == 1)
//			 {
				lcd_print_line3("  A  ");
//			 }

//			if (pump_indx == 2)
//			 {
//				lcd_print_line3("  B  ");
//			 }
			t = 0;
		}
	 // -------------- test keys....----------------
		  rd19 = readkey19_state();

		   if ( (rd19 == 1) && ( key19_sto_ == 0) )
			 {
			   keyboard_entry[0] = 0;   //clear the previous entry...
				index_ = 0;
				pump_indx++;
				if (pump_indx > 2) pump_indx = 1;  //wrap around
				   key19_sto_ = rd19;
			 }
			   key19_sto_ = rd19;

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
//					if (pump_indx == 1)   // if side A
//					{
						copy_stream1[0].noz_addr =  atoi(keyboard_entry);
//					}
//					else if (pump_indx == 2)   // if side b
//					{
						copy_stream1[1].noz_addr =  atoi(keyboard_entry);
//					}

					fxn = nothing;
					clr_screen1();
			}

			 else if (pkey == 'A')  // back key
			 {
					 fxn = nothing;
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
   if ( (auth == authed) &&  ( (access == level2)||(access == level3) ) && (fxn == set_no_flow_time)  )
   {
	   if (t >= 500)
		 {
		   printDisp_i(copy_stream2[0].timeOut_noFlow, 1, 0, 4, RT, CLEAR);
		   lcd_print_line2(keyboard_entry);

//		 if (pump_indx == 1)
//		 {
			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
			lcd_print_line3("  A  ");
//		 }

//		 else if (pump_indx == 2)
//		 {
//			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
//			lcd_print_line3("  B  ");
//		 }
		t = 0;
 }
	 // -------------- test keys....----------------
		  rd19 = readkey19_state();

		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
			 {
			   keyboard_entry[0] = 0;   //clear the previous entry...
				   index_ = 0;
				pump_indx++;
				if (pump_indx > 2) pump_indx = 1;  //wrap around
				   key19_sto_ = rd19;
			 }
			   key19_sto_ = rd19;

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
//					 if (pump_indx == 1)   // if side A
//						{
						 copy_stream2[0].timeOut_noFlow =  atoi(keyboard_entry);
//						}
//					 else if (pump_indx == 2)   // if side b
//						{
//						 copy_stream2[1].timeOut_noFlow =  atoi(keyboard_entry);
//						}

					 fxn = nothing;
					 clr_screen1();
				}

			 else if (pkey == 'A')  // back key
				{
					 fxn = nothing;
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
   else if ( (auth == authed) &&  ( (access == level2)||(access == level3) ) && (fxn == setmax_amt)  )
   {
	   if (t >= 500)
	   {
		   printDisp_i(copy_stream1[0].max_amt_, 1, 0, 4, RT, CLEAR);
		   lcd_print_line2(keyboard_entry);

//		 if (pump_indx == 1)
//		 {
			lcd_print_line3("  A  ");
//		 }
//
//		 else if (pump_indx == 2)
//		 {
//			lcd_print_line3("  B  ");
//		 }
		t = 0;
	  }
	 // -------------- test keys....----------------
		  rd19 = readkey19_state();

		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
			 {
			   keyboard_entry[0] = 0;   //clear the previous entry...
								   index_ = 0;
				pump_indx++;
				if (pump_indx > 2) pump_indx = 1;  //wrap around
				   key19_sto_ = rd19;
			 }
			   key19_sto_ = rd19;

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
//					 if (pump_indx == 1)   // if side A
//						{
						 copy_stream1[0].max_amt_ =  atoi(keyboard_entry);
//						}
//					 if (pump_indx == 2)   // if side b
//						{
//						 copy_stream1[1].max_amt_ =  atoi(keyboard_entry);
//						}

					 fxn = nothing;
					 clr_screen1();
				}

			 else if (pkey == 'A')  // back key
				{
					 fxn = nothing;
					 clr_screen1();
				}
		 }
	 return prog_State;
   }

 //=============================== MODE =================================
 //======================================================================
 //                         	CLEAR LOG
 //======================================================================
   else if ( (auth == authed) && (access == level1)&&(fxn == clr_log_)  )
   {
	   if (t >= 500)
		 {
		   //printDisp_i(copy[0].max_amt_, 1, 0, 4, RT, CLEAR);
//		   lcd_print_line2(keyboard_entry);
//
//		 if (pump_indx == 1)
//		 {
			lcd_print_line3("  A  ");
//		 }
//
//		 if (pump_indx == 2)
//		 {
//			lcd_print_line3("  b  ");
//		 }
		t = 0;
 }
	 // -------------- test keys....----------------
//		 int rd19 = readkey19_state();
//
//		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
//			 {
//			   keyboard_entry[0] = 0;   //clear the previous entry...
//				   index_ = 0;
//				pump_indx++;
//				if (pump_indx > 2) pump_indx = 1;  //wrap around
//				   key19_sto_ = rd19;
//			 }
//			   key19_sto_ = rd19;

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
//					 if (pump_indx == 1)   // if side A
//						{
//						  //clear side A log
//						  clear_logA();
//						  lcd_print_line1("        ");
//						  lcd_print_line2("cleared ");
//						  HAL_Delay(2000);
//						  clr_screen1(); //lcd_print_line2("        ");
//						  fxn = nothing;
//						}
//
//					 if (pump_indx == 2)   // if side b
//						{
//						 //clear side B log
//						 clear_logB();
//						 lcd_print_line1("        ");
//						 lcd_print_line2("cleared ");
//						 HAL_Delay(2000);
//						 clr_screen1(); //lcd_print_line2("        ");
//						 fxn = nothing;
//						}
//				}

			 if (pkey == 'A')  // back key
				{
					 keyboard_entry[0] = 0;   //clear the previous entry...
					 index_ = 0;
					 fxn = nothing;
					 clr_screen1();
				}
		 }
	 return prog_State;
   }

   //================================= MODE ===============================
   //======================================================================
   //                         	CLEAR TOTALIZER
   //======================================================================
   else if ( (auth == authed) && (access == level1)&&(fxn == clr_tot_)  )
    {
 	   if (t >= 500)
 		 {
 		   //printDisp_i(copy[0].max_amt_, 1, 0, 4, RT, CLEAR);
// 		   lcd_print_line2(keyboard_entry);
//
// 		 if (pump_indx == 1)
// 		 {
 			lcd_print_line3("  A  ");
// 		 }
//
// 		 if (pump_indx == 2)
// 		 {
// 			lcd_print_line3("  b  ");
// 		 }
 		t = 0;
  }
 	 // -------------- test keys....----------------
// 		 int rd19 = readkey19_state();
//
// 		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
// 			 {
// 			   keyboard_entry[0] = 0;   //clear the previous entry...
// 				   index_ = 0;
// 				pump_indx++;
// 				if (pump_indx > 2) pump_indx = 1;  //wrap around
// 				   key19_sto_ = rd19;
// 			 }
// 			   key19_sto_ = rd19;

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
// 					 if (pump_indx == 1)   // if side A
// 						{
// 						  //clear side A log
// 						  clear_totA();
// 						  lcd_print_line1("        ");
// 						  lcd_print_line2("cleared ");
// 						 HAL_Delay(2000);
// 						  fxn = nothing;
// 						 clr_screen1();
// 						}
//
// 					 if (pump_indx == 2)   // if side b
// 						{
// 						 //clear side B log
// 						 clear_totB();
// 						 lcd_print_line1("        ");
// 						 lcd_print_line2("cleared ");
// 						 HAL_Delay(2000);
// 						 fxn = nothing;
// 						 clr_screen1();
// 						}
// 				}

 			 if (pkey == 'A')  // back key
 			 {
 					 keyboard_entry[0] = 0;   //clear the previous entry...
 	 				 index_ = 0;
 	 				 fxn = nothing;
 					 clr_screen1();
 			}
 		 }
 	 return prog_State;
    }

 //============================== MODE =================================
 //=====================================================================
 //                             SEND CONFIG.
 //=====================================================================
   else if ( (auth == authed) && (access == level1) && (fxn == send_config)  )
   {
	   if (t >= 500)
		 {
		   //printDisp_i(copy[0].max_amt_, 1, 0, 4, RT, CLEAR);
		  // lcd_print_line2(keyboard_entry);
/*
		 if (pump_indx == 1)
		 {
			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
			lcd_print_line3("  a  ");
		 }

		 if (pump_indx == 2)
		 {
			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
			lcd_print_line3("  b  ");
		 }  */

		t = 0;
 }
	 // -------------- test keys....----------------
		 rd19 = readkey19_state();

		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
			 {
				pump_indx++;
				if (pump_indx > 2) pump_indx = 1;  //wrap around
				   key19_sto_ = rd19;
			 }
			   key19_sto_ = rd19;

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
					  for (uint8_t i = 0 ; i < 14;i++)
					  {
						//   send the bits to the external system

					  }

					  for (uint8_t i = 0 ; i < 14;i++)
					  {
						  //   send the bits to the external system
					  }

					  lcd_print_line2("Sent    ");
					  fxn = nothing;
				}

			 if (pkey == 'A')  // back key
				{
					 fxn = nothing;
					 clr_screen1();
				}
		 }
	 return prog_State;
   }


  //============================ MODE ===================================
  //=====================================================================
  //                          GET CONFIG.
  //=====================================================================
   else if ( (auth == authed) && (access == level1)&&(fxn == get_config)  )
    {
 	   if (t >= 3000)
 		 {

 		  lcd_print_line3("      ");
 		  lcd_print_line2(" Done ");
 		  fxn = nothing;
 		t = 0;
    }

  //-------------------------------------------------
  // Receive the bytes sent from the external system
  //       and store in the settings array.





  //-------------------------------------------------
  // -------------- test keys....--------------------
 		  rd19 = readkey19_state();

 		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
 			 {
 				pump_indx++;
 				if (pump_indx > 2) pump_indx = 1;  //wrap around
 				   key19_sto_ = rd19;
 			 }
 			   key19_sto_ = rd19;

 	          pkey = read_keypad();

			   if( (pkey > 0) )  //key detected
				 {
					 if (pkey == 'A')  // back key
						{
							 fxn = nothing;
							 clr_screen1();
						}
				 }
 	 return prog_State;
    }

  //============================ MODE ==================================
  //====================================================================
  //                         SET PASSWORD
  //====================================================================
   else if ( (auth == authed) &&  ( (access == level2)||(access == level3) ) && (fxn == set_password)  )
   {
	   static uint8_t chg_pw = 0;
	   static char pw1[9] = { 0 };
//	   static char pw2[9] = { 0 };
//	   static char pw3[9] = { 0 };

//	   static uint8_t index_pass = 0;


	   if(prog_revisit1 == 1)
	   {
		   chg_pw = 0;
		   memset(pw1, '\0', sizeof(pw1));
//		   memset(pw2, '\0', sizeof(pw2));
//		   memset(pw3, '\0', sizeof(pw3));

		   prog_revisit1 = 0;
	   }

 	   if (t >= 500)
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

 			  lcd_print_line1(keypad_pw_xter1); //keyboard_entry);
			   lcd_print_line2("  Enter ");
			   lcd_print_line3(ch_pass[index_generic]);  //show password level menu
 		   }
 		   else
 		   {
 			  lcd_print_line1(keypad_pw_xter1); //keyboard_entry);
 			    lcd_print_line2("Re-enter");
				lcd_print_line3(" Pass");
 		   }
// 		   if (pump_indx == 1)
// 		  		 {
// 		  	//		lcd_print_line3("  a  ");
// 		  		 }
//
// 		  		 if (pump_indx == 2)
// 		  		 {
// 		  	//		lcd_print_line3("  b  ");
// 		  		 }
 	    	t = 0;
         }
 	 // -------------- test keys....----------------
 		 rd19 = readkey19_state();

 		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
 			 {
 				pump_indx++;
 				if (pump_indx > 2) pump_indx = 1;  //wrap around
 				   key19_sto_ = rd19;
 			 }
 			   key19_sto_ = rd19;

 	    pkey = read_keypad();

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
//					 pw1 = atoi(keyboard_entry);
					 strncpy(pw1, keyboard_entry, sizeof(pw1));
					 chg_pw = 1;
					 lcd_print_line2("Re-enter");
					 lcd_print_line3(" Pass");
//					 HAL_Delay(2500);
					 lcd_print_line1("        ");
					 clear_buffer1();
					 //index_ = 0;
					 //keyboard_entry[0] = 0;
					 //keyboard_entry[1] = 0;
					 //keypad_pw_xter1[0] = 0;
					 //_index = 0;
					 return prog_State;
				  }
 				  else if (chg_pw == 1)
				  {
						 if (strcmp(pw1, keyboard_entry) == 0)  //if strings are same
						 {
							 lcd_print_line1("        ");
							 lcd_print_line2("Success ");
							 lcd_print_line3("        ");
							 lcd_print_line1("        ");

							 if(index_generic == 0)
							 {
								 strncpy(copy_stream3[0].passwd1, pw1, sizeof(pw1));
								 strncpy(copy_stream3[1].passwd1, pw1, sizeof(pw1));
							 }
							 else if(index_generic == 1)
							 {
								 strncpy(copy_stream3[0].passwd2, pw1, sizeof(pw1));
								 strncpy(copy_stream3[1].passwd2, pw1, sizeof(pw1));
							 }
							 else if(index_generic == 2)
							 {
								 strncpy(copy_stream3[0].passwd3, pw1, sizeof(pw1));
								 strncpy(copy_stream3[1].passwd3, pw1, sizeof(pw1));
							 }

							 HAL_Delay(1000);
							 lcd_print_line1("Passuurd ");
							 lcd_print_line2("Changed ");
							 HAL_Delay(1000);
							 fxn = nothing;
							 chg_pw = 0;
							 clear_buffer1();
							 index_generic = 0;

							return prog_State;

						 }
						 else
						 {

							    lcd_print_line2("---no---");
							    HAL_Delay(1500);
							    lcd_print_line2("        ");
							    attempts++;
							    if (attempts >= 3)
							    {
							    	lcd_print_line2("        ");
							    	lcd_print_line2(" Denied ");
							    	HAL_Delay(1500);
							    	lcd_print_line2("        ");
							    	 fxn = nothing;
							    	 chg_pw = 1;
									clear_buffer1();
//									index_generic = 0;
							    	// index_ = 0;
									//keyboard_entry[0] = 0;
									//keypad_pw_xter1[0] = 0;
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
//									index_generic = 0;
									// index_ = 0;
									//keyboard_entry[0] = 0;
									//keypad_pw_xter1[0] = 0;
									//_index = 0;
									return prog_State;
							    }
						 }
						 chg_pw = 1;
					  }

// 				 	 fxn = nothing;
 				}

 			 if (pkey == 'A')  // back key
			 {
 				chg_pw = 0;
 				fxn = nothing;
				clr_screen1();
				index_generic = 0;
			 }
 		 }
 	 return prog_State;
    }


    //============================== MODE ================================
    //====================================================================
    //                             CALIBRATE
    //====================================================================
   else if ( (auth == authed) &&  ( (access == level2)||(access == level3) ) && (fxn == calibrate_)  )
   {

       static float calib_price = 0.0;
	   static float inv_price   =  0.0;
	   static int vol_sold = 1;

       if(prog_revisit1 == 1)
  	   {
  		   calib_price = 0.0;
		   inv_price = 0.0;
		   vol_sold = 1;
		   volume_flag = 0;
		   calibr1 = 0;        // notify the interrupt routine to off the pump
		   prog_revisit1 = 0;
  	   }

	   if (volume_flag == 0)
	    {
		   //select sales volume first...

//		       snprintf(st__, sizeof(st__), "%d litre", calibrationCan_measure1);

		   lcd_print_line1("  Start  ");

//		       if (t >= 500)
//			   {
////					if (vol_sold == l1)
////					{
////						lcd_print_line1("10 litre");
////					}
////					else
//						lcd_print_line1(st__);
//
//				   if (pump_indx == 1)
//					 {
//						lcd_print_line3("  A  ");
//					 }
//
////				   if (pump_indx == 2)
////					 {
////						lcd_print_line3("  B  ");
////					 }
//				   t = 0;
//				}
   	 // -------------- test keys....----------------
   		 rd19 = readkey19_state();

   		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
   			 {
   				pump_indx++;
   				if (pump_indx > 2) pump_indx = 1;  //wrap around
   				   key19_sto_ = rd19;
   			 }
   			   key19_sto_ = rd19;

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
					calib_pulser1 = 0;  //clear pulser
		           __HAL_TIM_SET_COUNTER(&htim5, 0);   //clear harware pulser here...
					return prog_State;
   				}

   			 if (pkey == 'A')  // back key
   				{
   					 fxn = nothing;
   				     calibr1 = 0;
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
         calibr1 = 1;        /// notify the interrupt routine to on the pump
         return prog_State;
      }
    //=============================
	  if(volume_flag == 2)  //dispense.
	   {
		 if(t > 60)
		  {

			#if _USE_SOFT_PULSER == 0
			  calib_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);  //use hardware counter
			#else
			  calib_pulser1++; 							// use software counter.
			#endif

			  lcd_print_line1(" Pulser ");
			  printDisp_i(calib_pulser1, 2, 0, 7, RT, CLEAR);

		    //    power outage during filling1  end transaction...
			#if sense_power == 1
			  if( (readpwr() == 0)||(read_p_pwr() == 0) )
			  {
				  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
				  HAL_Delay(200);
				  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
				  stop_flow1();

				  if(calibration_flag1 == CALIBRATED)
				  {
//					  pulser_totalizer1 = ( (calib_pulser1 / (float) (pulser_benchMark1)) * calibrationCan_measure1 );
//					  totaliser_vol1c += pulser_totalizer1;
//					  totaliser_vol1 += pulser_totalizer1;
//					  totaliser_amt1c += (pulser_totalizer1 * litre_price);
//					  totaliser_amt1 += (pulser_totalizer1 * litre_price);
//					  amt_real1 = pulser_totalizer1;

//					  amt_middle1 = pulser_totalizer1;
//					  price_real1 = (pulser_totalizer1 * litre_price);
//					  price_upper1 = (pulser_totalizer1 * litre_price)


					  amt_middle1 = calibrationCan_measure1;
					  price_real1 = (calibrationCan_measure1 * litre_price);
					  price_upper1 = (calibrationCan_measure1 * litre_price);
					  amt_real1 = calibrationCan_measure1;

					  totaliser_vol1c += calibrationCan_measure1;
					  totaliser_vol1 += calibrationCan_measure1;
					  totaliser_amt1c += price_real1;
					  totaliser_amt1 += price_real1;

					  save_totaliser_fram(operating_side);
//					  save_amountTotaliser_fram(operating_side);
					  save_lastSale_fram(operating_side);

//					  pwr1 = POWERINTERRUPTION;
//					  save_calibrationData(side_a);
				  }
				  else if(calibration_flag1 == UNCALIBRATED)
				  {
//					  calibrationData[0].pulser_value = calib_pulser1;

//					  pulser_benchMark1 = 0;
//					  pwr1 = POWERINTERRUPTION;
//					  save_calibrationData_fram(side_a);

					  amt_middle1 = calibrationCan_measure1;
					  price_real1 = (calibrationCan_measure1 * litre_price);
					  price_upper1 = (calibrationCan_measure1 * litre_price);
					  amt_real1 = calibrationCan_measure1;

					  totaliser_vol1c += calibrationCan_measure1;
					  totaliser_vol1 += calibrationCan_measure1;
					  totaliser_amt1c += price_real1;
					  totaliser_amt1 += price_real1;

					  save_totaliser_fram(operating_side);
//					  save_amountTotaliser_fram(operating_side);
					  save_lastSale_fram(operating_side);
				  }

				  return write_flash_State;
			  }
			#endif
			  t = 0;
		  }

		  if(settings_stream1[0].mode == AUTO_MODE)
			{
				if(timer_go >= TIMEOUT_GO)   //if go's timeout is 5sec threshold
				{
					go_timeOut1 = 1;

					HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
					HAL_Delay(200);
					HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
					stop_flow1();


					if(calibration_flag1 == CALIBRATED)
					{
						  amt_middle1 = calibrationCan_measure1;
						  price_real1 = (calibrationCan_measure1 * litre_price);
						  price_upper1 = (calibrationCan_measure1 * litre_price);
						  amt_real1 = calibrationCan_measure1;

						  totaliser_vol1c += calibrationCan_measure1;
						  totaliser_vol1 += calibrationCan_measure1;
						  totaliser_amt1c += price_real1;
						  totaliser_amt1 += price_real1;

						  save_totaliser_fram(operating_side);
//						  save_amountTotaliser_fram(operating_side);
						  save_lastSale_fram(operating_side);

					}
					else if(calibration_flag1 == UNCALIBRATED)
					{
						  amt_middle1 = calibrationCan_measure1;
						  price_real1 = (calibrationCan_measure1 * litre_price);
						  price_upper1 = (calibrationCan_measure1 * litre_price);
						  amt_real1 = calibrationCan_measure1;

						  totaliser_vol1c += calibrationCan_measure1;
						  totaliser_vol1 += calibrationCan_measure1;
						  totaliser_amt1c += price_real1;
						  totaliser_amt1 += price_real1;

						  save_totaliser_fram(operating_side);
//						  save_amountTotaliser_fram(operating_side);
						  save_lastSale_fram(operating_side);
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
				  	  calib_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);  //use hardware counter
				  #else
				  	  calib_pulser1++; 							// use software counter.
				  #endif

//				  pulser_totalizer1 = ( (calib_pulser1 / (float) (pulser_benchMark1)) * calibrationCan_measure1 );
				  pulser_totalizer1 = ( (calib_pulser1 / (float) (calib_pulser1)) * calibrationCan_measure1 );

				  pulser_benchMark1 = calib_pulser1;

				  amt_real1 = pulser_totalizer1;
				  amt_middle1 = pulser_totalizer1;
				  price_real1 = (pulser_totalizer1 * litre_price);
				  price_upper1 = (pulser_totalizer1 * litre_price);

				  totaliser_vol1c += pulser_totalizer1;
				  totaliser_vol1 += pulser_totalizer1;
				  totaliser_amt1c += price_real1;
				  totaliser_amt1 += price_real1;

				  save_totaliser_fram(operating_side);
//				  save_amountTotaliser_fram(operating_side);
				  save_lastSale_fram(operating_side);
				  save_calibrationData_fram(side_a);


  //				  calib_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);  //use hardware counter
				  lcd_print_line1("Set your");
				  lcd_print_line2("Volunne ");
				  HAL_Delay(2000);
				  volume_flag = 3;   //enter the price.


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
    if(volume_flag == 3)  //Enter the price.
	   {
			  if(t > 400)
			  {
				  lcd_print_line1("Volunne ");

				  float vol = strtof(keyboard_entry, &endPtr);
				  printDisp_f(vol, 2, 0, 7, RT, CLEAR);
				  t = 0;
			  }

			  pkey = read_keypad();
			  if (pkey == 'D')  //ENTER key to adjusted volume.
				{
				  float cal_vol = strtof(keyboard_entry, &endPtr);
				  lcd_print_line1("        ");   //clear screen.
				  lcd_print_line2("        ");
				  clear_buffer1();
					//index_ = 0;
					//keyboard_entry[0] = 0;  //clear buffer...
					//keypad_pw_xter1[0] = 0;
					//_index = 0;
					//==================================
					// calculate the price of the pulses
					// float pop = calib_pulser1  / cal_vol;
					int sold_v = 0;
					float pi_c;
					float pi;

//					if (vol_sold == l1)
//					{
//						sold_v = 10;
//						pi_c = calib_pulser1 / cal_vol; //calibrated pi.
//						pi   = calib_pulser1 / sold_v;
//					}
//					else
//					{
						sold_v = settings_stream2[0].calibration_measureCan;
						pi_c = calib_pulser1 / cal_vol; //calibrated pi.
						pi   = calib_pulser1 / sold_v;
//						pi   = calib_pulser1 / settings[0].calibration_measureCan;
//					}

					vol_real1 = sold_v;
					vol_calibrated1 = cal_vol;
					save_ctSettings_fram(side_a);
					save_calibrationPulser_fram(side_a);

					calibration_flag1 = CALIBRATED;
					save_calibrationFlag_fram(side_a);

                    HAL_Delay(1700);

                    lcd_print_line1("  Done  ");
                    printDisp_f(pi_c, 2, 0, 8, RT, CLEAR);
                    HAL_Delay(2500);

//                    //store in the settings structure.
//                    if(pump_indx == 1)
//                    {
                    	// data for side a.
                    	copy_stream1[0].pi_cal = pi_c;
                    	copy_stream1[0].pi_real  = pi;
//                    }

//                    if(pump_indx == 2)
//					{
//						// data for side b.
//                    	copy_stream1[1].pi_cal = pi_c;
//                    	copy_stream1[1].pi_real	 = pi;
//					}
					 fxn = nothing;
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
   else if ( (auth == authed) &&  ( (access == level2)||(access == level3) ) && (fxn == display_)  )
   {

	 if(enter_key == 1)
	 {
		 if(timer_config1 < 1000)
		 {
			 lcd_print_line2("Entered");
		 }
		 else
		 {
			 enter_key = 0;
			 fxn = nothing;
			 clr_screen1();
		 }

		 return prog_State;
	 }

	 if (ln_ == 0) ln_ = 1;
	 if (dp_ == 0) dp_ = 1;

	   if (t >= 500)
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
			 if (pump_indx == 1)
			 {
				lcd_print_line3("  A  ");
			 }

			 else if (pump_indx == 2)
			 {
				lcd_print_line3("  B  ");
			 }
			t = 0;
        }
	 // -------------- test keys....----------------
		 rd19 = readkey19_state();

		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
			 {
			   keyboard_entry[0] = 0;   //clear the previous entry...
								   index_ = 0;
				pump_indx++;
				if (pump_indx > 2) pump_indx = 1;  //wrap around
				   key19_sto_ = rd19;
			 }
			   key19_sto_ = rd19;

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
					 if (pump_indx == 1)   // if side A
					 {
					   if(ln_ == 1) copy_stream1[0].dp_amount  = dp_;
					   if(ln_ == 2) copy_stream1[0].dp_vol = dp_;
					   if(ln_ == 3) copy_stream1[0].dp_unitprice = dp_;
					 }
					 else if (pump_indx == 2)   // if side b
					 {
					   if(ln_ == 1) copy_stream1[1].dp_amount  = dp_;
					   if(ln_ == 2) copy_stream1[1].dp_vol = dp_;
					   if(ln_ == 3) copy_stream1[1].dp_unitprice = dp_;
					 }

					 clr_screen1();
					 timer_config1 = 0;
					 lcd_print_line2("Entered ");

					 enter_key = 1;
					 return prog_State;
				}

			 else if (pkey == 'A')  // back key
				{
					 fxn = nothing;
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
   	 else if ( (auth == authed) &&  ( (access == level1)||(access == level2)||(access == level3) ) && (fxn == setclock) )
	 {

		    static int8_t respons_dir = 0,
						   entry_clear = 1,
						   montth = 0,
						   failFlag_tm = 0,
						   failFlag_dt = 0;

			static bool hh_success = false,
						yy_success = false;

		 	if(prog_revisit1 == 1)
		    {
		 	   respons_dir = 0;
			   entry_clear = 1;
			   montth = 0;
			   failFlag_tm = 0;
			   failFlag_dt = 0;

		    	hh_success = false;
		    	yy_success = false;

		    	prog_revisit1 = 0;
		    }

		    int8_t respons = 0,
		    	   tm = 0,
		    	   dte = 0;

//		  char st__[10] = {0};

		  //====================================================
		  if( (t>200) && (respons_dir == 0) )
		  {
			  lcd_print_line1("1- Tinne");
			  lcd_print_line2("2-  Date");
			  t = 0;

		  }
		  if(entry_clear)
			  respons = atoi(keyboard_entry);

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

			  if( (atoi(keyboard_entry)) <= 59 )
				  tm = atoi(keyboard_entry);
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

			  if( (atoi(keyboard_entry)) <= 99 )
				  dte = atoi(keyboard_entry);
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

					 if(!failFlag_tm) _tt1 = 0;

					 if( (failFlag_tm) && (_tt1 > 600) ) failFlag_tm = 0;
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

							 if(!failFlag_dt) _tt1 = 0;

							 if( (failFlag_dt) && (_tt1 > 600) ) failFlag_dt = 0;
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
  //                NUMBER OF SIDES  ( 1 - 2) ==> default : 2
  //======================================================================
   else if ( (auth == authed) && ( (access == level2)||(access == level3) ) && (fxn == number_of_sides)  )
   {

	   if (t >= 300)
	   {
//		      if( (index_generic != 1) && (index_generic != 2) ) index_generic = 1;

		   lcd_print_line1("Sides  ");

//			  snprintf(st__, sizeof(st__), "       %d", index_generic);
			  snprintf(st__, sizeof(st__), "       %d", copy_stream2[0].noz_count);
			  lcd_print_line2(st__);


			  lcd_print_line3("  A  ");

			t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( key19_sto_ == 0) )
			 {
				pump_indx++;
				if (pump_indx > 2) pump_indx = 1;  //wrap around
				   key19_sto_ = rd19;
			 }
		 key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
			 {
//				 if (index_generic == 1)  index_generic = 2;
//				 else if(index_generic == 2) index_generic = 1;

				 if(copy_stream2[0].noz_count == 1)
					 copy_stream2[0].noz_count = 2;
				 else if(copy_stream2[0].noz_count == 2)
					 copy_stream2[0].noz_count = 1;
			 }

			 else if (pkey == 'C')  // down key
			 {
//				 if (index_generic == 2)  index_generic = 1;
//				 else if(index_generic == 1) index_generic = 2;

				 if(copy_stream2[0].noz_count == 1)
					 copy_stream2[0].noz_count = 2;
				 else if(copy_stream2[0].noz_count == 2)
					 copy_stream2[0].noz_count = 1;
			 }

			 else if (pkey == 'F')  //change pump index.
			 {
				  // pump_indx++;
				  //if (pump_indx > 2) pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
//				 if(index_generic == 1)
//					 copy_stream2[0].noz_count = 1;
//				 else if(index_generic == 2)
//					 copy_stream2[0].noz_count = 2;

				 index_generic = 0;
				 fxn = nothing;
				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
			 {
				 index_generic = 0;

				 fxn = nothing;
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
   else if ( (auth == authed) && ( (access == level2)||(access == level3) ) && (fxn == suppressed_display_vol)  )
   {
	   static uint8_t firstTime_sV = 1;

	   if(firstTime_sV == 1)
	   {
		   index_generic = (copy_stream2[0].startUp_suppressVol * 100);
		   firstTime_sV = 0;
	   }

	   if (t >= 300)
	   {
			if(index_generic > 10) index_generic = 0;

			lcd_print_line1("Supp. Vol");

			snprintf(st__, sizeof(st__), "   %02d cL", index_generic);
			lcd_print_line2(st__);

			lcd_print_line3("  A  ");

			t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( key19_sto_ == 0) )
			 {
				pump_indx++;
				if (pump_indx > 2) pump_indx = 1;  //wrap around
				   key19_sto_ = rd19;
			 }
		 key19_sto_ = rd19;

		 pkey = read_keypad();

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
				 copy_stream2[0].startUp_suppressVol = (index_generic * 0.01);

				 index_generic = 0;
				 fxn = nothing;
				 clr_screen1();

				 firstTime_sV = 1;
			 }

			 else if (pkey == 'A')  // back key
			 {
				 index_generic = 0;

				 fxn = nothing;
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
   else if ( (auth == authed) && ( (access == level2)||(access == level3) ) && (fxn == calibration_can)  )
   {

	   if (t >= 300)
	   {
//		   if( (index_generic != 10) && (index_generic != 20) ) index_generic = 10;

		   lcd_print_line1("Can Size");

		   snprintf(st__, sizeof(st__), "    %02d L", copy_stream2[0].calibration_measureCan);
		   lcd_print_line2(st__);

		   lcd_print_line3("  A  ");

			t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( key19_sto_ == 0) )
		 {
			pump_indx++;
			if (pump_indx > 2) pump_indx = 1;  //wrap around
			   key19_sto_ = rd19;
		 }
		 key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
			 {
//				 if (index_generic == 10)  index_generic = 20;
//				 else if(index_generic == 20) index_generic = 10;

				   if(copy_stream2[0].calibration_measureCan == 10)
					 copy_stream2[0].calibration_measureCan = 20;
				   else  if(copy_stream2[0].calibration_measureCan == 20)
					 copy_stream2[0].calibration_measureCan = 10;
			 }

			 else if (pkey == 'C')  // down key
			 {
//				 if (index_generic == 20)  index_generic = 10;
//				 else if(index_generic == 10) index_generic = 20;

				   if(copy_stream2[0].calibration_measureCan == 10)
					 copy_stream2[0].calibration_measureCan = 20;
				   else  if(copy_stream2[0].calibration_measureCan == 20)
					 copy_stream2[0].calibration_measureCan = 10;
			 }

			 else if (pkey == 'F')  //change pump index.
			 {
				  // pump_indx++;
				  //if (pump_indx > 2) pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
//				 if(index_generic == 10)
//					 copy_stream2[0].calibration_measureCan = 10;
//				 else if(index_generic == 20)
//					 copy_stream2[0].calibration_measureCan = 20;

				 index_generic = 0;
				 fxn = nothing;
				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
			 {
				 index_generic = 0;
				 fxn = nothing;
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
   else if ( (auth == authed) && ( (access == level2)||(access == level3) ) && (fxn == shift_login_type_)  )
   {

	   if (t >= 300)
	   {
//		   if( (index_generic != None_) && (index_generic != Code_)  && (index_generic != Card_) ) index_generic = None_;
		   lcd_print_line1("Login.Typ");

//		   snprintf(st__, sizeof(st__), "       %d", index_generic);
		   lcd_print_line2(login_type[copy_stream2[0].shift_login_type]);

		   lcd_print_line3("  A  ");

		   t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( key19_sto_ == 0) )
			 {
				pump_indx++;
				if (pump_indx > 2) pump_indx = 1;  //wrap around
				   key19_sto_ = rd19;
			 }
		 key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
			 {
//				 if (index_generic <= 2 )  index_generic++;
//				 else if(index_generic == 3) index_generic = 0;

				 if(copy_stream2[0].shift_login_type == None_)
					 copy_stream2[0].shift_login_type = Code_;
				 else if(copy_stream2[0].shift_login_type == Code_)
					 copy_stream2[0].shift_login_type = Card_;
				 else if(copy_stream2[0].shift_login_type == Card_)
					 copy_stream2[0].shift_login_type = None_;
			 }

			 else if (pkey == 'C')  // down key
			 {
//				 if (index_generic > 0)  index_generic--;
//				 else if(index_generic == 0) index_generic = 2;

				 if(copy_stream2[0].shift_login_type == None_)
					 copy_stream2[0].shift_login_type = Card_;
				 else if(copy_stream2[0].shift_login_type == Code_)
					 copy_stream2[0].shift_login_type = None_;
				 else if(copy_stream2[0].shift_login_type == Card_)
					 copy_stream2[0].shift_login_type = Code_;
			 }

			 else if (pkey == 'F')  //change pump index.
			 {
				  // pump_indx++;
				  //if (pump_indx > 2) pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
//				 if(index_generic == None_)
//					 copy_stream2[0].shift_login_type = None_;
//				 else if(index_generic == Code_)
//					 copy_stream2[0].shift_login_type = Code_;
//				 else if(index_generic == Card_)
//					 copy_stream2[0].shift_login_type = Card_;

				 index_generic = 0;
				 fxn = nothing;
				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
			 {
				 index_generic = 0;
				 fxn = nothing;
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
   else if ( (auth == authed) && ( (access == level2)||(access == level3) ) && (fxn == number_of_shifts_)  )
   {

	   if (t >= 300)
	   {
//		   if( (index_generic != 1) && (index_generic != 2) ) index_generic = 1;
		   lcd_print_line1("Shifts   ");

		   snprintf(st__, sizeof(st__), "       %d", copy_stream2[0].number_of_shifts);
		   lcd_print_line2(st__);

		   lcd_print_line3("  A  ");

		   t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( key19_sto_ == 0) )
			 {
				pump_indx++;
				if (pump_indx > 2) pump_indx = 1;  //wrap around
				   key19_sto_ = rd19;
			 }
		 key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
			 {
//				 if (index_generic == 1)  index_generic = 2;
//				 else if(index_generic == 2) index_generic = 1;

				 if(copy_stream2[0].number_of_shifts == 1)
					 copy_stream2[0].number_of_shifts = 2;
				 else if(copy_stream2[0].number_of_shifts == 2)
					 copy_stream2[0].number_of_shifts = 1;
			 }

			 else if (pkey == 'C')  // down key
			 {
//				 if (index_generic == 2)  index_generic = 1;
//				 else if(index_generic == 1) index_generic = 2;

				 if(copy_stream2[0].number_of_shifts == 1)
					 copy_stream2[0].number_of_shifts = 2;
				 else if(copy_stream2[0].number_of_shifts == 2)
					 copy_stream2[0].number_of_shifts = 1;
			 }

			 else if (pkey == 'F')  //change pump index.
			 {
				  // pump_indx++;
				  //if (pump_indx > 2) pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
//				 if(index_generic == 1)
//					 copy_stream2[0].number_of_shifts = 1;
//				 else if(index_generic == 2)
//					 copy_stream2[0].number_of_shifts = 2;

				 index_generic = 0;
				 fxn = nothing;
				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
			 {
				 index_generic = 0;
				 fxn = nothing;
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
   else if ( (auth == authed) && (access == level3) && (fxn == calibration_type_)  )
   {

	   if (t >= 300)
	   {
//			  if( (index_generic !=  Wizard) && (index_generic != Manual_calib) ) index_generic = Wizard;

			  if(copy_stream2[0].calibration_type ==  Wizard)
			  {
				  lcd_print_line1("UUizard");
			  }
			  else
			  {
				  lcd_print_line1("NNanual");
			  }

			  lcd_print_line3("  A  ");

			t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( key19_sto_ == 0) )
		 {
			pump_indx++;
			if (pump_indx > 2) pump_indx = 1;  //wrap around
			   key19_sto_ = rd19;
		 }
		 key19_sto_ = rd19;

		 pkey = read_keypad();

		 if( (pkey > 0) )  //key detected
		 {
			 if (pkey == 'B')  // up key
			 {
//				 if (index_generic == Wizard)  index_generic = 2;
//				 else if(index_generic == 2) index_generic = Wizard;

				 if(copy_stream2[0].calibration_type == Wizard)
					 copy_stream2[0].calibration_type = Manual_calib;
				 else if(copy_stream2[0].calibration_type == Manual_calib)
					 copy_stream2[0].calibration_type = Wizard;
			 }

			 else if (pkey == 'C')  // down key
			 {
//				 if (index_generic == 2)  index_generic = Wizard;
//				 else if(index_generic == Wizard) index_generic = 2;

				 if(copy_stream2[0].calibration_type == Wizard)
					 copy_stream2[0].calibration_type = Manual_calib;
				 else if(copy_stream2[0].calibration_type == Manual_calib)
					 copy_stream2[0].calibration_type = Wizard;
			 }

			 else if (pkey == 'F')  //change pump index.
			 {
				  // pump_indx++;
				  //if (pump_indx > 2) pump_indx = 1; //wrap around
			 }

			 else if (pkey == 'D')  // Enter key
			 {
//				 if(index_generic == Wizard)
//					 copy_stream2[0].calibration_type = Wizard;
//				 else if(index_generic == Manual_calib)
//					 copy_stream2[0].calibration_type = Manual_calib;

				 index_generic = 0;
				 fxn = nothing;
				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
			 {
				 index_generic = 0;
				 fxn = nothing;
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
   else if ( (auth == authed) && (access == level1) && (fxn == shift_total)  )
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

	   if (t >= 300)
	   {
			  if(index_generic == 0)
			  {
				 if(attendant1.session_id[0] != NULL)
				 {
					 shiftTotaliser_vol = (totaliser_vol1c - startShiftTotaliser_vol1c);

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
					   printDisp_c(line1, 1, (8-jj), 5, LT, NOCLEAR);  //display price
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
			  else if(index_generic == 1)
			  {
				  if(attendant1.session_id[0] != NULL)
				  {
					  shiftTotaliser_amt = (totaliser_amt1c - startShiftTotaliser_amt1c);
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
			      }  //  if (indx1 == 1)
				  else
				  {
					  lcd_print_line1("P      ");
				  }

			  }

			  lcd_print_line3("Sh.TotA");
//			  lcd_print_line3("123456");

			t = 0;
		}
	 // -------------- test keys....----------------
		rd19 = readkey19_state();

		if ( (rd19 == 1)&&( key19_sto_ == 0) )
		 {
			pump_indx++;
			if (pump_indx > 2) pump_indx = 1;  //wrap around
			   key19_sto_ = rd19;
		 }
		 key19_sto_ = rd19;

		 pkey = read_keypad();

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

			 else if (pkey == 'D')  // Enter key
			 {
				 index_generic = 0;
				 fxn = nothing;
				 clr_screen1();
			 }

			 else if (pkey == 'A')  // back key
			 {
				 index_generic = 0;

				 fxn = nothing;
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
   else if ( (auth == authed) && ( (access == level2)||(access == level3) ) && (fxn == keypress_tone_)  )
      {

   	   if (t >= 300)
   	   {
   		   lcd_print_line1("Suuitch ");

//   		   if( (index_generic !=  Yes) && (index_generic !=  No) ) index_generic = Yes;

   		   if(copy_stream2[0].keypress_tone ==  Yes)
   		   {
   			   lcd_print_line2("      ON");
   		   }
   		   else
   		   {
   			   lcd_print_line2("     OFF");
   		   }

   		  lcd_print_line3("  A  ");

   			t = 0;
   		}
   	 // -------------- test keys....----------------
   		rd19 = readkey19_state();

   		if ( (rd19 == 1)&&( key19_sto_ == 0) )
   			 {
   				pump_indx++;
   				if (pump_indx > 2) pump_indx = 1;  //wrap around
   				   key19_sto_ = rd19;
   			 }
   		 key19_sto_ = rd19;

   		 pkey = read_keypad();

   		 if( (pkey > 0) )  //key detected
   		 {
   			 if (pkey == 'B')  // up key
   			 {
//   				 if (index_generic == Yes)  index_generic = No;
//   				 else if(index_generic == No) index_generic = Yes;


				 if(copy_stream2[0].keypress_tone == Yes)
 				 {
 					copy_stream2[0].keypress_tone = No;
 				 }
 				 else if(copy_stream2[0].keypress_tone == No)
 				 {
 					copy_stream2[0].keypress_tone = Yes;
 				 }
   			 }

   			 else if (pkey == 'C')  // down key
   			 {
//   				 if (index_generic == No)  index_generic = Yes;
//   				 else if(index_generic == Yes) index_generic = No;

   				if(copy_stream2[0].keypress_tone == Yes)
				 {
					copy_stream2[0].keypress_tone = No;
				 }
				 else if(copy_stream2[0].keypress_tone == No)
				 {
					copy_stream2[0].keypress_tone = Yes;
				 }
   			 }

   			 else if (pkey == 'F')  //change pump index.
   			 {
   				  // pump_indx++;
   				  //if (pump_indx > 2) pump_indx = 1; //wrap around
   			 }

   			 else if (pkey == 'D')  // Enter key
   			 {
//   				 if(index_generic == Yes)
//   				 {
//   					copy_stream2[0].keypress_tone = Yes;
//   				 }
//   				 else if(index_generic == No)
//   				 {
//   					copy_stream2[0].keypress_tone = No;
//   				 }

   				 fxn = nothing;
   				 index_generic = 0;
   				 clr_screen1();
   			 }

   			 else if (pkey == 'A')  // back key
   			 {
   				 fxn = nothing;
   				 index_generic = 0;
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
	else if ( (auth == authed) && (access == level3) && (fxn == communication_card)  )
	   {

		   if (t >= 300)
		   {
			   lcd_print_line1("Enforce ");

//			   if( (index_generic !=  Yes) && (index_generic !=  No) ) index_generic = Yes;

			   if(copy_stream2[0].commCard_enforced ==  true)
			   {
				   lcd_print_line2("     Yes");
			   }
			   else
			   {
				   lcd_print_line2("      No");
			   }

			  lcd_print_line3("  A  ");

				t = 0;
			}
		 // -------------- test keys....----------------
			rd19 = readkey19_state();

			if ( (rd19 == 1)&&( key19_sto_ == 0) )
			 {
				pump_indx++;
				if (pump_indx > 2) pump_indx = 1;  //wrap around
				   key19_sto_ = rd19;
			 }
			 key19_sto_ = rd19;

			 pkey = read_keypad();

			 if( (pkey > 0) )  //key detected
			 {
				 if (pkey == 'B')  // up key
				 {
//					 if (index_generic == Yes)  index_generic = No;
//					 else if(index_generic == No) index_generic = Yes;

					 if(copy_stream2[0].commCard_enforced == true)
					 {
						 copy_stream2[0].commCard_enforced = false;
					 }
					 else if(copy_stream2[0].commCard_enforced == false)
					 {
						 copy_stream2[0].commCard_enforced = true;
					 }
				 }

				 else if (pkey == 'C')  // down key
				 {
//					 if (index_generic == No)  index_generic = Yes;
//					 else if(index_generic == Yes) index_generic = No;

					 if(copy_stream2[0].commCard_enforced == true)
					 {
						 copy_stream2[0].commCard_enforced = false;
					 }
					 else if(copy_stream2[0].commCard_enforced == false)
					 {
						 copy_stream2[0].commCard_enforced = true;
					 }
				 }

				 else if (pkey == 'F')  //change pump index.
				 {
					  // pump_indx++;
					  //if (pump_indx > 2) pump_indx = 1; //wrap around
				 }

				 else if (pkey == 'D')  // Enter key
				 {
//					 if(index_generic == Yes)
//					 {
//						 copy_stream2[0].commCard_enforced = true;
//					 }
//					 else if(index_generic == No)
//					 {
//						 copy_stream2[0].commCard_enforced = false;
//					 }

					 fxn = nothing;
					 index_generic = 0;
					 clr_screen1();
				 }

				 else if (pkey == 'A')  // back key
				 {
					 fxn = nothing;
					 index_generic = 0;
					 clr_screen1();
				 }
			 }
		 return prog_State;
	   }


   //============================== MODE ================================
   //====================================================================
   //                           PULSER CONFIG.
   //====================================================================
  else if ( (auth == authed) &&  ( (access == level2)||(access == level3) ) && (fxn == pulser_config)  )
  {
	 static uint8_t pulserr = 0,
					pulserType_original,
					pulserOffset_original,
					firstTime_pOffset = 1;

      if(prog_revisit1 == 1)
 	   {
			   pulser_flag = 0;
			   prog_revisit1 = 0;
 	   }

	   if (pulser_flag == 0)
	    {
		       if (t >= 500)
			   {
					if (pulserr == 0)
					{
						lcd_print_line1("  Type  ");
					}
					else
						lcd_print_line1(" Offset  ");


					lcd_print_line3("  A  ");

				   t = 0;
				}
  	 // -------------- test keys....----------------
  		  rd19 = readkey19_state();

  		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
  			 {
  				pump_indx++;
  				if (pump_indx > 2) pump_indx = 1;  //wrap around
  				   key19_sto_ = rd19;
  			 }
  			   key19_sto_ = rd19;

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

					pulserType_original = copy_stream2[0].pulser_type_;
					pulserOffset_original = copy_stream2[0].pulser_offset;

					return prog_State;
  			  }

  			 else if (pkey == 'A')  // back key
				 {
					 fxn = nothing;
					 clr_screen1();
				 }
  		 }
     	 return prog_State;
	   }// if selected


	//========= pulser type has been selected =============
	else if(pulser_flag == 1)
    {
		   if (t >= 500)
		   {
			   if (pulserr == 2)
			   {
				   if (copy_stream2[0].pulser_type_ == quadrature)
					{
					   lcd_print_line1("Ouadrat. ");
					}
					else if (copy_stream2[0].pulser_type_ == non_quadrature)
					{
						lcd_print_line1("Non-Quad.");
					}

					lcd_print_line3("  A  ");
			   }
			   else if (pulserr == 3)
			   {
				   if(firstTime_pOffset == 1)
				   {
					   index_generic = pulserOffset_original;
					   firstTime_pOffset = 0;
				   }
//				   if(index_generic > 99) index_generic = 0;

				   lcd_print_line1("Ofset.Val");

					snprintf(st__, sizeof(st__), "%02d units", index_generic);
					lcd_print_line2(st__);

					lcd_print_line3("  A  ");
			   }

			   t = 0;
			}

		 // -------------- test keys....----------------
			 rd19 = readkey19_state();

			   if ( (rd19 == 1)&&( key19_sto_ == 0) )
				 {
					pump_indx++;
					if (pump_indx > 2) pump_indx = 1;  //wrap around
					   key19_sto_ = rd19;
				 }
				   key19_sto_ = rd19;

			pkey = read_keypad();

			 if( (pkey > 0) )  //key detected
			 {
				 if (pulserr == 2)
				 {
					 if (pkey == 'B')  // up key
					 {
						 if (copy_stream2[0].pulser_type_ == quadrature)
						 {
							 copy_stream2[0].pulser_type_ = non_quadrature;
						 }
						 else
							 copy_stream2[0].pulser_type_ = quadrature;
					 }

					 else if (pkey == 'C')  // down key
					 {
						 if (copy_stream2[0].pulser_type_ == quadrature)
						 {
							 copy_stream2[0].pulser_type_ = non_quadrature;
						 }
						 else
							 copy_stream2[0].pulser_type_ = quadrature;
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
						 fxn = pulser_config;
						 copy_stream2[0].pulser_type_ = pulserType_original;
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
						 copy_stream2[0].pulser_offset = (index_generic);

						 index_generic = 0;

						 pulserr = 0;
						 pulser_flag = 0;
						 fxn = pulser_config;
						 clr_screen1();

						 firstTime_pOffset = 1;
					 }

					 else if (pkey == 'A')  // back key
					 {
						 index_generic = 0;

						 pulserr = 0;
						 pulser_flag = 0;

						 fxn = pulser_config;
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
	   else if ( (auth == authed) && ( (access == level2)||(access == level3) ) && (fxn == valve_config)  )
	   {
		   static float float_generic = 0.00,
						salesStart_original,
						salesEnd_original;

		   static uint8_t _valve = 0,
				   	   	  firstTime_v = 1;

		         if(prog_revisit1 == 1)
		    	 {
		        	   valve_flag = 0;
		   			   prog_revisit1 = 0;
		    	 }

		   	   if (valve_flag == 0)
		   	   {
				   if (t >= 500)
				   {
						if (_valve == 0)
						{
							lcd_print_line1("Ds Start");
						}
						else
							lcd_print_line1("Disp. End");

						lcd_print_line3("  A  ");

					   t = 0;
					}
		     	 // -------------- test keys....----------------
		     		 rd19 = readkey19_state();

		     		   if ( (rd19 == 1)&&( key19_sto_ == 0) )
		     			 {
		     				pump_indx++;
		     				if (pump_indx > 2) pump_indx = 1;  //wrap around
		     				   key19_sto_ = rd19;
		     			 }
		     			 key19_sto_ = rd19;

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

		   					salesStart_original = copy_stream2[0].valve_salesStart;
		   					salesEnd_original = copy_stream2[0].valve_salesEnd;

		   					return prog_State;
		     			  }

		     			 else if (pkey == 'A')  // back key
		   				 {
		   					 fxn = nothing;
		   					 clr_screen1();
		   				 }
		     		 }
		        	 return prog_State;
		   	}// if selected


		   	else if(valve_flag == 1)
			{
				   if (t >= 300)
				   {
					   if (_valve == 2)
					   {
						    if(firstTime_v == 1)
						    {
						    	float_generic = copy_stream2[0].valve_salesStart;
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
								float_generic = copy_stream2[0].valve_salesEnd;
								firstTime_v =  0;
							}

//						   if(float_generic > 10.00) float_generic = 0.30;
//							else if(float_generic < 0.30) float_generic = 10.00;

							snprintf(st__, sizeof(st__), "  %5.2f L", float_generic);
							lcd_print_line2(st__);
					   }

					   lcd_print_line3("  A  ");

					   t = 0;
				}

				 // -------------- test keys....----------------
					 rd19 = readkey19_state();

					   if ( (rd19 == 1)&&( key19_sto_ == 0) )
						 {
							pump_indx++;
							if (pump_indx > 2) pump_indx = 1;  //wrap around
							   key19_sto_ = rd19;
						 }
						   key19_sto_ = rd19;

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
								copy_stream2[0].valve_salesStart = float_generic;

								fxn = nothing;
								clr_screen1();

								firstTime_v = 0;

								return prog_State;
							 }
							 else if (pkey == 'A')  // back key
							 {
								 fxn = nothing;
								 copy_stream2[0].valve_salesStart = salesStart_original;
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
								copy_stream2[0].valve_salesEnd = float_generic;

								clr_screen1();

								firstTime_v = 0;

								return prog_State;
							 }
							 else if (pkey == 'A')  // back key
							 {
								 fxn = nothing;
								 copy_stream2[0].valve_salesEnd = salesEnd_original;

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
	   else if ( (auth == authed) && ( (access == level2)||(access == level3) ) && (fxn == no_calibration)  )
	   {

		   if (t >= 300)
		   {
				snprintf(st__, sizeof(st__), "PC %05d", calib_pulser1);
				lcd_print_line1(st__);

				lcd_print_line3("  A  ");

				t = 0;
			}
		 // -------------- test keys....----------------
			rd19 = readkey19_state();

			if ( (rd19 == 1)&&( key19_sto_ == 0) )
				 {
					pump_indx++;
					if (pump_indx > 2) pump_indx = 1;  //wrap around
					   key19_sto_ = rd19;
				 }
			 key19_sto_ = rd19;

			 pkey = read_keypad();

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
					 copy_stream2[0].non_calibration_seed = (index_generic);

					 index_generic = 0;
					 fxn = nothing;
					 clr_screen1();
				 }

				 else if (pkey == 'A')  // back key
				 {
					 index_generic = 0;

					 fxn = nothing;
					 clr_screen1();
				 }
			 }
		 return prog_State;
	   }

 //============================== MODE ==================================
  return prog_State;
} //end of prog_state.



//----------------------------------------
eSystemState idleState_Handler(void)
{
	static int  printer_status;

	static int8_t idleState_flag = 1;

	static int32_t current_pulser_ = 0,
				   old_pulser_ = 0;

	uint16_t gerCtTime;

	int pulser_diff = 0;

//	display_overflow1 = 1;

	pump1_status_4G = STATUS_IDLE;

	stop_fueling_bit = 1;

	stop_flag = 0;

	lock_clr = 0;

	progg = 0;

	firstTime_nozz1 = 1;

	#if sense_power == 1
	  if(readpwr() == 0)
	  {
		   modem_power(DEACTIVATE);

		   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
		   HAL_Delay(200);
		   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);

		   //count time elapsed
		   if (shutdown_timer1 > 120)
		   {
			   displayandkeypad_power(DEACTIVATE);   //shutdown... after  2 minutes
		   }
	  }
	  else
	  {
		  //clear the timer//
		  shutdown_timer1 = 0;

		  modem_power(ACTIVATE);
		  displayandkeypad_power(ACTIVATE);
	  }
	#endif

	#if !defined (DEV_MODE)
		if(batteryStatus == LOWBATTERY)
		{
			lcd_print_line1("  Louu   ");
			lcd_print_line2("Battery  ");
			lcd_print_line3(" Err70 ");

			return inactive_State;
		}
		else if(batteryStatus == NOBATTERY)
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

		calibration_flag1 = CALIBRATED;
	//	if(calib_pulser1 < 15800)  //15987, 15967 .... 1106247681
		if(calibration_flag1 != CALIBRATED) //15800)  //15987, 15967 .... 1106247681
		{
			retrieve_calibrationFlag(side_a);

			if(calibration_flag1 != CALIBRATED) //takes care of accidental clearing of calibration_flag1 by F-keys
			{
				calibration1_error = 1;

				lcd_print_line1("Calibrat");
				lcd_print_line2("  Error ");
				lcd_print_line3("Err 23 ");

				return inactive_State;
			}
		}
		else
		{
			calibration1_error = 0;
		}
	#endif    //#if !defined (DEV_MODE) ,,

	if(nozzleUp_inProgMode1 == 1)
	{
			clear_buffer1();
			prog_entry1 = 0;
			prog_revisit1 = 1;

//		#ifndef DEV_MODE
			prog_revisitt1 = 1;
//		#endif

			nozzleUp_inProgMode1 = 0;
	}
	if(ctTimed_flag1 == 1)
	{
		day = DS1307_GetDate();
		if( (ctTimed_day1 == day) || ((ctTimed_day1 + 1) == day) )
		{
			gerCtTime = get_ctTime1();

			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
			//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx IF IT'S TIME xxxxxxxxxxxxxxxxxxxxxxxxxxxxx//
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
			if( (gerCtTime >= ctTimed_settingsA.startTime) && (gerCtTime <= ctTimed_settingsA.endTime) )
			{
				settings_stream1[0].pi_cal = (calib_pulser1 / vol_effective1_1);
			}
			else
			{
				retrieve_originalPi_c(side_a);
			}

			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
			//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx IF TIME ELAPSESxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
			if(gerCtTime > ctTimed_settingsA.endTime)
			{
				if( ( (ctTimed_settingsA.startTime - ctTimed_settingsA.endTime) < 0) || ( (ctTimed_settingsA.startTime - ctTimed_settingsA.endTime) == 0) )
				{
					ctTimed_flag1 = 0;
					save_ctTimedFlag(side_a);
				}
				else if( (ctTimed_settingsA.startTime - ctTimed_settingsA.endTime) > 0)
				{
					if( (ctTimed_day1 + 1) == day)
					{
						ctTimed_flag1 = 0;
						save_ctTimedFlag(side_a);
					}
				}
			}

		}
	}

	if (settings_stream1[0].mode == AUTO_MODE)
	{
		if(online_calibFlag1 == 1)
		{
			settings_stream1[0].pi_cal = (calib_pulser1 / vol_effective1);

			save_settings_fram();
			online_calibFlag1 = 0;
			save_online_calibFlag_fram(side_a);
		}
	}

	if( (idleState_flag == 1) && (eLastState1 != filling_State) )
	{
		idleState_flag = 2;
		_tt1 = 0;
	}
	else if( (_tt1 > 60000) && (idleState_flag == 2) && (eLastState1 != filling_State) )
	{
		current_pulser_ = __HAL_TIM_GET_COUNTER(&htim5);
		old_pulser_ = current_pulser_ ;
		idleState_flag = 3;
	}
	else if( (_tt1 > 65000) && (idleState_flag == 3) && (eLastState1 != filling_State) )
	{
		if(old_pulser_ > current_pulser_ )
		{
			pulser_diff = old_pulser_ - current_pulser_;

			if(pulser_diff > 4)
				idle_backwardPulse = 1;
		}
		else if(old_pulser_ < current_pulser_)
		{
			pulser_diff = current_pulser_ - old_pulser_;

			if(pulser_diff > 4)
				idle_forwardPulse = 1;
		}

		idleState_flag = 1;
	}

//	 if(changeLitrePrice1 == 1)
//	 {
//		online_setUnitPrice1();
//		changeLitrePrice1 = 0;
//	 }
	if(changeLitrePrice1 == 1)
	{
//		if( !( (eNextState1 == idle_State) && (eLastState1 == idle_State) ) )
		if( ( (eNextState1 != idle_State) || (eLastState1 != idle_State) ) &&
				( (eNextState1 != idle_State) && (eLastState1 != inactive_State) ) )
		{
			priceChange_timer1 = 0;
		}
		else //if( (eNextState2 == idle_State) && (eLastState2 == idle_State) )
		{
			if(priceChange_timer1 >= 3000)   //30000)
			{
				priceChange_timerMin1++;
				priceChange_timer1 = 0;

				if(priceChange_timerMin1 == 3)
				{
//					online_setUnitPrice1();
					go_setUnitPrice1(price_update1);
					changeLitrePrice1 = 0;
					changeLitrePrice1_2 = 1;
					priceChange_timerMin1 = 0;
					t = 0;
				}
			}
		}
	}

	else if( (timer_go >= TIMEOUT_GO) && (settings_stream1[0].mode == AUTO_MODE) )  //if go's timeout is 5sec threshold
	{
		if ( (t >= 300) && (t <= 700) )
		{
			lcd_print_line1("        ");
			lcd_print_line2(" Offline");
			lcd_print_line3("      ");
		}
		else if ( (t > 700) && (t <= 2000) )
		{
			  if(settings_stream1[0].display_format == PL)
			  {
				 lcd_print_line1(upper1);
				 lcd_print_line2(middle1);
			  }
			  else if(settings_stream1[0].display_format == LP)
			  {
				  lcd_print_line1(middle1);
				  lcd_print_line2(upper1);
			  }
			 lcd_print_line3("        ");
			 char str__[8]= {0};
			 snprintf(str__, sizeof(str__), "%.2f", litre_price); lcd_print_line3(str__);
		}

	   if (t > 2000)
	   {
		  /* start the DMA again */
		  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *) RxBuf, RxBuf_SIZE);
		  __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

		  t = 0;
	   }
	}

	else if ( (t > 500) && (nozzleup_awaitingauth_state_not_timedOut == 0) && (pump_LitreOverflow == 0) && (_litre_price1 == 0)
			&& (_auth_p == 0) && (_auth_v == 0) && (idle_backwardPulse == 0) && (idle_forwardPulse == 0)
			&& (flow_loss == 0) && (display_overflow1 == 0) && (changeLitrePrice1_2 == 0) && (_pump_max_litres1 == 0) && (nonValid_sale1 == 0) )
	{
		 if(settings_stream1[0].display_format == PL)
		  {
			 lcd_print_line1(upper1);
			 lcd_print_line2(middle1);
		  }
		  else if(settings_stream1[0].display_format == LP)
		  {
			  lcd_print_line1(middle1);
			  lcd_print_line2(upper1);
		  }
		 lcd_print_line3("        ");
		 char str__[8]= {0};
		 snprintf(str__, sizeof(str__), "%.2f", litre_price);
		 lcd_print_line3(str__);
		 t = 0;
	 }

//	  	if ( ((t > 1000) && (t <= 1000)) && (nozzleup_awaitingauth_state_not_timedOut == 1) )
//		 {
//			 // lcd_print_line1("n up u a");
//			 clr_screen1();
//			 lcd_print_line1("call.  ");
//			 //t = 0;
//		 }
//		 else if ( ((t > 2000) && (t <= 3000)) && (nozzleup_awaitingauth_state_not_timedOut == 1) )
//		 {
//			lcd_print_line1("call . ");
//			//t = 0;
//		 }
//		 else if ( ((t > 3000) && (t <= 4000)) && (nozzleup_awaitingauth_state_not_timedOut == 1) )
//		 {
//			lcd_print_line1("call  .");
//			if(t > 4000) t = 0;
//		 }

	else if ( (t > 2000) && (pump_LitreOverflow == 1) )
	{
		 // lcd_print_line1("n up u a");
//			 clr_screen1();
		 lcd_print_line1("  Punnp  ");
		 lcd_print_line2(" Linnit  ");
		 lcd_print_line3("Err7   ");
		 if(t > 6000)
		 {
			 t = 0;
			 pump_LitreOverflow = 0;
		 }
	 }
//	  	else if ( (t > 2000) && (display_overflow1 == 1) )
//		{
//			 lcd_print_line1("display   ");
//			 lcd_print_line2(" linnit  ");
//			 lcd_print_line3("err18   ");
//			 if(t > 6000)
//			 {
//				 t = 0;
//				 display_overflow1 = 0;
//			 }
//		}
	  	else if (display_overflow1 == 1)
		{
	  		 if(firstTime_display_overflow1 == 1)
			 {
	  			 if( (t > 2000) && (t <= 6000) )
	  			 {
	  				 clr_screen1();
	  				 lcd_print_line1("Display  ");
					 lcd_print_line2(" Linnit  ");
					 lcd_print_line3("Err18   ");
	  			 }
	  			 else if(t > 6000) //&& (display_overflow2 == 1) )
				 {
					 t = 0;

					 clr_screen1();

//					 lcd_print_line1(upper1);
//					 lcd_print_line2(middle1);

					 if(settings_stream1[0].display_format == PL)
				  	  {
				  		 lcd_print_line1(upper1);
				  		 lcd_print_line2(middle1);
				  	  }
					  else if(settings_stream1[0].display_format == LP)
					  {
						  lcd_print_line1(middle1);
						  lcd_print_line2(upper1);
					  }

					 lcd_print_line3("        ");
					 char str__[8]= {0};
					 snprintf(str__, sizeof(str__), "%.2f", litre_price); lcd_print_line3(str__);
					 firstTime_display_overflow1 = 0;
				 }
			 }
	  		 else if(firstTime_display_overflow1 == 0)
			 {
	  			 if( (t > 6000) && (t <= 12000) )
	  			 {
	  				 clr_screen1();
	  				 lcd_print_line1("Display  ");
					 lcd_print_line2(" Linnit  ");
					 lcd_print_line3("Err18   ");
	  			 }
	  			 else if(t > 12000) //&& (display_overflow2 == 1) )
				 {
					 t = 0;

					 clr_screen1();

					 if(settings_stream1[0].display_format == PL)
					  {
						 lcd_print_line1(upper1);
						 lcd_print_line2(middle1);
					  }
					  else if(settings_stream1[0].display_format == LP)
					  {
						  lcd_print_line1(middle1);
						  lcd_print_line2(upper1);
					  }

//					 lcd_print_line1(upper1);
//					 lcd_print_line2(middle1);
					 lcd_print_line3("        ");
					 char str__[8]= {0};
					 snprintf(str__, sizeof(str__), "%.2f", litre_price); lcd_print_line3(str__);
				 }
			 }
		}

	  	else if( (t > 2000) && (_litre_price1 == 1) )
	  	{
	  		 lcd_print_line1("No Unit   ");
			 lcd_print_line2(" Price  ");
			 lcd_print_line3("Err8   ");
			 if(t > 6000)
			 {
				 t = 0;
				 _litre_price1 = 0;
			 }
	  	}
		else if( (t > 2000) && (_pump_max_litres1 == 1) )
	  	{
	  		 lcd_print_line1("No-punnp  ");
			 lcd_print_line2(" Linnit  ");
			 lcd_print_line3("Err21   ");
			 if(t > 6000)
			 {
				 t = 0;
				 _pump_max_litres1 = 0;
			 }
	  	}
	  	else if( (t > 2000) && (_auth_p == 1) )
		{
			 lcd_print_line1("    No   ");
			 lcd_print_line2("  Price  ");
			 lcd_print_line3("Err9   ");
			 if(t > 6000)
			 {
				 t = 0;
				 _auth_p = 0;
			 }
		}
		else if( (t > 2000) && (_auth_v == 1) )
		{
			 lcd_print_line1("   No   ");
			 lcd_print_line2("Volunne ");
			 lcd_print_line3("Err10   ");
			 if(t > 6000)
			 {
				 t = 0;
				 _auth_v = 0;
			 }
		}
		else if( (t > 2000) && (nonValid_sale1 == 1) )
		{
			 lcd_print_line1("   Non   ");
			 lcd_print_line2("  Valid  ");
			 lcd_print_line3("Err22   ");
			 if(t > 6000)
			 {
				 t = 0;
				 nonValid_sale1 = 0;
			 }
		}
		else if( (t > 2000) && (idle_backwardPulse == 1) )
		{
			 lcd_print_line1("  Back   ");
			 lcd_print_line2("  Flouu  ");
			 lcd_print_line3("Err13   ");
			 if(t > 6000)
			 {
				 t = 0;
				 idle_backwardPulse = 0;
			 }
		}
		else if( (t > 2000) && (idle_forwardPulse == 1) )
		{
			 lcd_print_line1("   Idle   ");
			 lcd_print_line2("  Flouu  ");
			 lcd_print_line3("Err14  ");
			 if(t > 6000)
			 {
				 t = 0;
				 idle_forwardPulse = 0;
			 }
		}
		else if( (t > 2000) && (flow_loss == 1) )
		{
			 lcd_print_line1("   Flow   ");
			 lcd_print_line2("  Loss  ");
			 lcd_print_line3("Err16  ");
			 if(t > 6000)
			 {
				 t = 0;
				 flow_loss = 0;
			 }
		}
		else if( ( t > 300) && (changeLitrePrice1_2 == 1) )
		{

			 clr_screen1();
			 lcd_print_line1("  Price ");
			 lcd_print_line2(" Changed ");

			 lcd_print_line3("        ");
			 char str__[8]= {0};
			 snprintf(str__, sizeof(str__), "%.2f", litre_price); lcd_print_line3(str__);

			 ep1a_priceChangeFlag1 = 1;

			 return inactive_State;
		}


	 if( (keypress_ == 21) && (nozzleup_awaitingauth_state_not_timedOut == 0) )
	 {
		   compose_printer();
		   HAL_Delay(1000);
		   printer_status = 1;
	 }

	     print__1();   //print the transaction.

//	#if sense_power == 1
//		  	  if(readpwr() == 0)
//			  {
//				   modem_power(DEACTIVATE);
//
//				   //count time elapsed
//				   if (shutdown_timer1 > 120)
//				   {
//					   displayandkeypad_power(DEACTIVATE);   //shutdown... after  2 minutes
//				   }
//			  }
//		  	  else
//		  	  {
//		  		  //clear the timer//
//		  		  shutdown_timer1 = 0;
//		  	  }
//	#endif

//		else if( (t2 > 2000) && (changeLitrePrice2_2 == 1) )
//		{
//			 lcd_print_line12(" price ");
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
//		else if( (t > 2000) && (_auth_v == 1) )
//				{
//					 lcd_print_line1("  no   ");
//					 lcd_print_line2("annount  ");
//					 lcd_print_line3("err17   ");
//					 if(t > 6000)
//					 {
//						 t = 0;
//						 _auth_v = 0;
//					 }
//				}

//		 else if ( ((t > 2000) && (t <= 3000)) && (pump_LitreOverflow == 1) )
//		 {
//			lcd_print_line1(" limit ");
//			//t = 0;
//		 }
//		 else if ( ((t > 3000) && (t <= 4000)) && (pump_LitreOverflow == 1) )
//		 {
//			lcd_print_line1("");
//			if(t > 4000) t = 0;
//		 }

	  	return idle_State;
}



//---------------
eSystemState authorised_nozzledown_State_Handler(void)
{
	  char *endPtr;

	  nozzle_bit = 0;
	  pump1_status_4G = STATUS_AUTHORIZED_NOZZLE_DOWN;


	/* if (t > 1000)
		  {
				lcd_print_line1("a n dn");
			  t = 0;
		  }*/
	/* int keyboard_val = 2000;   //dummy value...

	    reset_timer(timeout_picknozzle);
    	start_timer(timeout_picknozzle); */

	 //initialise the fuel and price variables

	     if(index_ >= 1)
		  {
			  key_value = strtof(keyboard_entry, &endPtr);
		  }
		 else
		 {
			  key_value = 0.0;
		 }

	   // key_value = 5000.000;
	    //initialise the fuel and price variables
	      if (sellmode == P)
		  {
			  target_pulser1 = price2pulser(key_value);  //calculate pulse frm price.
		  }
	      else  //amt was selected.
	      {
	    	  target_pulser1 = amt2pulser(key_value);   //calculate pulse frm amt.
	      }

	/*
    	if (sellmode == P)  //price was selected
    	{
    	   target_pulser1 = price2pulser(keyboard_val); //calculate pulse frm price.
    	}
    	  else      //amt was selected.
    	{
    	   target_pulser1 = amt2pulser(keyboard_val); //calculate pulse frm amt.
    	}
    	filling1 = 1;
   */

	    //  target_pulser1 = price2pulser(5000);

	  price  = 0.0;
	  amt = 0.0;

	  current_pulser1 = 0;
	  overall_currentPulser1 = 0;

	 // int cnv = 0;
	 // snprintf(str_, sizeof(str_), "%.2f", price); lcd_print_line1(str_);
	 // snprintf(str_, sizeof(str_), "%.2f", amt);   lcd_print_line2(str_);

   if(t > 300)
   {
	  lcd_print_line1(" Pic    ");
	  lcd_print_line2("No22le  ");
	  char str__[8]= {0};
	  snprintf(str__, sizeof(str__), "%.2f", litre_price); lcd_print_line3(str__);
      t = 0;
   }

	if(display_overflow1 == 1)
	{
		display_overflow1 = 0;
		firstTime_display_overflow1 = 0;
	}

	return authorised_nozzledown_State;
}


//---------------
eSystemState  nozzleup_waitingforauthState_Handler(void)
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
   			nozzleup_awaitingauth_state_not_timedOut = 1;


   			//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[

   			//Amount, Vol., and Alarm cleared
			//Light switched on
			//Preset-Vol Cleared to default value
			//Display cleared

   			//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

   			if(t > 2400) t = 0;

   			 if ( ((t > 400) && (t <= 900)) && (nozzleup_awaitingauth_state_not_timedOut == 1) )
			 {
				 clr_screen1();
				 lcd_print_line1("auth    ");
			 }
			 else if ( ((t > 900) && (t <= 1400)) && (nozzleup_awaitingauth_state_not_timedOut == 1) )
			 {
				lcd_print_line1("auth_ ");
			 }
			 else if ( ((t > 1400) && (t <= 1900)) && (nozzleup_awaitingauth_state_not_timedOut == 1) )
			 {
				lcd_print_line1("auth__ ");
			 }
			 else if ( ((t > 1900) && (t <= 2400)) && (nozzleup_awaitingauth_state_not_timedOut == 1) )
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


	int8_t pkey = 0;

	pump_status_1 = STATUS_AUTH;

	slowFlow_startThreshold1 = (fast_flow_threshold1 * settings_stream2[0].valve_salesStart);
	slowFlow_endThreshold1 = (fast_flow_threshold1 * settings_stream2[0].valve_salesEnd);


	if (stop_flag == 1)   //if stop key is pressed
	{
 		filling1 = 0,  nozzle_bit = 0;
		stop_flag = 0;
		stop_flow1(); //send_solenoid(1);  //stop solenoid.

		//--------------------------------------------------------
		dpFlag = 0;
		error_clr_flag = 1;
		 index_ = 0;
		 _index = 0;
		 for(uint8_t i = 0; i < 9; i++)
		 {
		   keypad_pw_xter1[i] = 0;
		   keyboard_entry[i] = 0;   //clear the buffer
		 }

		 for(uint8_t i = 0; i <= 8; i++)
		 {
			 keyboard[i] = 0;
		 }
		 //--------------------------------------------------------------------
		 if (sellmode == P)
		 {
			  write_v(3, "P     0");  //send_keypad("p    ");  //5 xters  lafeng..
		 }
		 else if(sellmode == L)
		 {
			  write_v(3, "L     0");  //send_keypad("l    ");  //5 xters lafeng
		 }
	//	 else if(sellmode == V)
	//	 {
	//		  write_v(3, "v    0");  //send_keypad("p    ");  //5 xters  lafeng..
	//	 }
		 keypad_print(keyboard);


//		 pump_status_ = STATUS_FILLING_COMP;

		 if (settings_stream1[0].mode == AUTO_MODE)
		 {

			 ///////////////////////////////////////////////////
			 ///////// SIGNALS GO ABOUT NOZZLE STATUS //////////

	//		 status_change_noz1 = 1;
			 nozzle_out1 = false;

			 ///////////////////////////////////////////////////
		 }


		 if (settings_stream1[0].mode == MANUAL_MODE)
		 {
			 return idle_State;
		 }

	}

	if( firstTime_nozz1 == 1)
	{
		firstTime_nozz1 = 2;
		t = 0;

		return authorised_nozzleup_State;
	}
	else if(firstTime_nozz1 == 2)
	{
		if (t <= 1400)
		{
			lcd_print_line1("88888888");
			lcd_print_line2("88888888");
			lcd_print_line3("888888");
		}
		else if(t > 1400)
		{
			lcd_print_line3("        ");

			firstTime_nozz1 = 0;
		}

		return authorised_nozzleup_State;
	}

	lcd_print_line3("        ");
	char str__[8]= {0};
	snprintf(str__, sizeof(str__), "%.2f", litre_price); lcd_print_line3(str__);

	nozzle_bit = 1;  stop_fueling_bit = 0;

    pump1_status_4G = STATUS_AUTHORIZED_NOZZLE_UP;

 	//initialise the fuel and price variables
	//int keyboard_val = 2000;   //dummy value...
	//clear pulsercount in the prev. state.....

	 sellPrice_max_dpp = sellPrice_max_dp(dp_amount1);

//	 sellPrice_max_dpp = 10250;

	 display_minimumCentilitrePrice1 = (display_minimumCentilitre1 * litre_price1);

	 if(litre_price1 == 0)
	 {
		_litre_price1 = 1;
		return idle_State;
	 }
	 else if(pump_max_litres1 == 0)
	 {
		 _pump_max_litres1 = 1;
		 return idle_State;
	 }
	 else if ( (change_p1 == 1) && (auth_p1 == 0) )
	 {
		_auth_p = 1;
		return idle_State;
	 }
	 else if ( (change_v1 == 1) && (auth_v1 == 0) )
	 {
		_auth_v = 1;
		return idle_State;
	 }
	 else if(opmode == MANUAL_MODE)
	 {
		 if(index_ >= 1)
		 {
			  key_value = strtof(keyboard_entry, &endPtr);

			  half_litre1 = (0.5 * litre_price1);

			  //initialise the fuel and price variables

				if (sellmode == P)
				{
					  sellPrice_max_pump = (litre_price1 * pump_max_litres1);

					  if( (key_value > sellPrice_max_pump) || (key_value > sellPrice_max_dpp) )  // || (key_value < half_litre1) )
					  {
						  if(sellPrice_max_pump < sellPrice_max_dpp)
						  {
							  key_value = sellPrice_max_pump;
							  pump_LitreOverflow = 1;
						  }
						  else if(sellPrice_max_pump > sellPrice_max_dpp)
						  {
							  key_value = sellPrice_max_dpp;
						      display_overflow1 = 1;
						  }
					  }

					  target_pulser1 = price2pulser(key_value);  //calculate pulse frm price.
//					  }

				}
				else  //amt was selected.
				{
					float key_value_ = (sellPrice_max_dpp / litre_price1);

					if( (key_value > pump_max_litres1) || (key_value > key_value_) )
					 {
						 if(pump_max_litres1 < key_value_)
						 {
							  key_value = pump_max_litres1;
							  pump_LitreOverflow = 1;
						 }
						 else if(pump_max_litres1 > key_value_)
						 {
							  key_value = key_value_;
							  display_overflow1 = 1;
						 }
					 }

					 target_pulser1 = amt2pulser(key_value);   //calculate pulse frm amt.
				}
		  }
		  else
		  {
			  key_value = (litre_price1 * pump_max_litres1);

			  if (sellmode == P)
			  {
				  if(key_value < sellPrice_max_dpp)
				  {
					  key_value = key_value;
				  }
				  else if(key_value > sellPrice_max_dpp)
				  {
					  key_value = sellPrice_max_dpp;
				  }

				  target_pulser1 = price2pulser(key_value);  //calculate pulse frm price.
			  }
			  else if (sellmode == L)
			  {
					key_value = (sellPrice_max_dpp / litre_price1);

					if(pump_max_litres1 < key_value)
					{
						  key_value = pump_max_litres1;
					}
					else if(pump_max_litres1 > key_value)
					{
						  key_value = key_value;
					}

					target_pulser1 = amt2pulser(key_value);   //calculate pulse frm amt.
			  }

		   }

	 }
	 else if(opmode == AUTO_MODE)
	 {
		  if (change_p1 == 1)
		  {
			  sellPrice_max_pump = (litre_price1 * pump_max_litres1);
			  sellPrice_max_dpp = (sellPrice_max_dpp / litre_price1);

			  change_p1 = 0;      //reset tbe flag.
			  index_ = strlen(keyboard_entry);

			  if (index_ >= 1)
			  {
					key_value = strtof(keyboard_entry, &endPtr);

				  	if (sellmode == L)
					{
				  		key_value_sellmodeL1 = 1;
				  		key_value_original1 = key_value;


				  		//======== Convert to Volume =======//
				  		key_value = (key_value * litre_price1);
					}

					if(key_value > auth_p1)
						key_value = auth_p1;

					if( (key_value > sellPrice_max_pump) || (key_value > sellPrice_max_dpp) )     // || (key_value < 0.5) )
					{
						if(sellPrice_max_pump < sellPrice_max_dpp)
						{
							key_value = sellPrice_max_pump;
							pump_LitreOverflow = 1;
						}
						else if (sellPrice_max_pump > sellPrice_max_dpp)
						{
							 key_value = sellPrice_max_dpp;
							 display_overflow1 = 1;
						}
					}

					target_pulser1 = price2pulser(key_value);  //calculate pulse frm price.

				}  //End of keyboard Input Check


			  else
			  {
				  if( (auth_p1 > sellPrice_max_pump) || (auth_p1 > sellPrice_max_dpp) )
				  {
					  if(sellPrice_max_pump < sellPrice_max_dpp)
					  {
						  auth_p1 = sellPrice_max_pump;
						  pump_LitreOverflow = 1;
					  }

					  else if(sellPrice_max_pump > sellPrice_max_dpp)
					  {
						  auth_p1 = sellPrice_max_dpp;
						  display_overflow1 = 1;
					  }
				  }

				  sprintf(keyboard_entry,"%2f", auth_p1);

				  key_value = strtof(keyboard_entry, &endPtr);

				  target_pulser1 = price2pulser(key_value);  //calculate pulse frm price.
			  }
		  }
		 //-----------------------------------------
		  //authorise volume...
		  else if (change_v1 == 1)
		  {
			  sellPrice_max_dpp = (sellPrice_max_dpp / litre_price1);

			  change_v1 = 0;      //reset tbe flag.
			  index_ = strlen(keyboard_entry);

			  if (index_ >= 1)
			  {
					key_value = strtof(keyboard_entry, &endPtr);
//					key_value = strtod(keyboard_entry, NULL);

				  	if (sellmode == P)
					{
				  		key_value_sellmodeP1 = 1;
				  		key_value_original1 = key_value;

				  		//======== Convert to Volume Equivalent =======//
						key_value = (key_value / litre_price1);
					}

					if(key_value > auth_v1)
						key_value = auth_v1;

					if( (key_value > pump_max_litres1) || (key_value > sellPrice_max_dpp) )
					{
						if(pump_max_litres1 < sellPrice_max_dpp)
						{
							key_value = pump_max_litres1;
							pump_LitreOverflow = 1;
						}
						else if (pump_max_litres1 > sellPrice_max_dpp)
						{
							 key_value = sellPrice_max_dpp;
							 display_overflow1 = 1;
						}
					}

					target_pulser1 = amt2pulser(key_value);   //calculate pulse frm amt.

				}  //End of keyboard Input Check

			    else
			    {
			    	if( (auth_v1 > pump_max_litres1) || (auth_v1 > sellPrice_max_dpp) )
			    	{
						  if(pump_max_litres1 < sellPrice_max_dpp)
						  {
							  auth_v1 = pump_max_litres1;
							  pump_LitreOverflow = 1;
						  }

						  else if (pump_max_litres1 > sellPrice_max_dpp)
						  {
							  auth_v1 = sellPrice_max_dpp;
							  display_overflow1 = 1;
						  }
			    	}

				  sprintf(keyboard_entry,"%2f", auth_v1);

				  key_value = strtof(keyboard_entry, &endPtr);

				  target_pulser1 = amt2pulser(key_value);   //calculate pulse frm amt.
			   }
		 }
//		 else
//		 {
//			  key_value = sellPrice_max_dpp;
//		      display_overflow1 = 1;
//			  target_pulser1 = price2pulser(key_value);  //calculate pulse frm price.
//		 }
	   }
//============================================================================
//============================================================================
//	  if (authorise_flag == 1)
//	   {
//		  target_pulser1 = price2pulser(5000);
//		  key_value  = 5000;
//		  authorise_flag == 0;
//	   }
//============================================================================
//============================================================================

	  	  price  = 0.0;
		  amt  = 0.0;

		  if (key_value == 0)
		  {
			   target_pulser1 = 0;
		  }
		  else
			  pulser_rem1 = target_pulser1 - current_pulser1;

		  if( (target_pulser1 == 0) || (key_value == 0) )
		  {
			  hardwareError_flag1 = 1;

//			  make_string(P, dp(0.00, dp_amount1));
//			  make_string(L, dp(0.00, dp_vol1));
		  }

		  if(firstTime_1 == 1)
		  {
		 	motor_tmr1 = 0;
		 	firstTime_1 = 0;
		  }

		  if (motor_tmr1 >= 2000)
		  {
				drive_motor1(ACTIVATE);

				if(settings_stream1[0].pump_type_ != LAFENG)
				{
					 drive_slow_sole1(ACTIVATE);
					 drive_fast_sole1(DEACTIVATE);
				}
				else
				{
					drive_slow_sole1(DEACTIVATE);     // DEACTIVATE here actually means ACTIVATE
					drive_fast_sole1(ACTIVATE);		  // ACTIVATE here actually means DEACTIVATE
				}


			  if( (lock_clr == 0) && (firstTime_1 == 0) ) ///   to activate this section once.
			  {
					 current_pulser1 = 0;
					 overall_currentPulser1 = 0;
					 clr_pulser1();    //clear hardware pulser

					 //current_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);
					 lock_clr = 1;
					 filling1 = 1;

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
//			  if( (lock_clr == 0) && (firstTime_1 == 0) && (pkey == 'D') ) //  to activate this section once.
//				  	  	  	  	  	  	  	  	  	  	  	  	  	  	   // 'D' is an Enter Key
//			  {
//					 drive_slow_sole1(ACTIVATE);
//
//					 current_pulser1 = 0;
//					 clr_pulser();    //clear hardware pulser
//
//					 //current_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);
//					 lock_clr = 1;
//					 filling1 = 1;
//
//					 firstTime = 1;
//					 firstTime_1 = 1;
//	//				 motor_tmr1 = 0;
//			  }
//		  }


//		  slow_flow1();



		 // int cnv = 0;
	     //char temp[10] = {0};
		 //snprintf(temp , sizeof(temp), "%.2f", price);  //lcd_print_line1(upper1);
         //snprintf(middle1, sizeof(middle1), "%.2f", amt);   //lcd_print_line2(lower);

	  	  make_string(P, price);
		  make_string(L, amt);

//		 if (t > 300)
		 if (t > 100)
		 {
//				lcd_print_line1(upper1);
//				lcd_print_line2(middle1);

				if(settings_stream1[0].display_format == PL)
				{
					 lcd_print_line1(upper1);
					 lcd_print_line2(middle1);
				 }
				 else if(settings_stream1[0].display_format == LP)
				 {
					  lcd_print_line1(middle1);
					  lcd_print_line2(upper1);
				 }

				char str__[8]= {0};
				snprintf(str__, sizeof(str_), "%.2f", litre_price);
				lcd_print_line3(str__);
				t = 0;

							//trigger first pulser...
				if(firstTime == 1)
				{
					firstTime = 2;
					#if (_USE_SOFT_PULSER == 1)
						 //  pulser_new += 1;
						   current_pulser1++;
					#endif

				}
				else if (firstTime == 2)
				{
					firstTime = 3;

					#if (_USE_SOFT_PULSER == 1)
							 //  pulser_new += 1;
							current_pulser1++;
					#else
							current_pulser1 = 0;
							overall_currentPulser1 = 0;
							clr_pulser1();    //clear hardware pulser
					#endif
				}
				else if (firstTime == 3)
				{
					#if (_USE_SOFT_PULSER == 1)
						 //  pulser_new += 1;
						   current_pulser1++;
					#else
						 // pulser_new = __HAL_TIM_GET_COUNTER(&htim5);
						   current_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);
						   overall_currentPulser1 = current_pulser1;
					#endif
				}


//			#if (_USE_SOFT_PULSER == 1)
//					 //  pulser_new += 1;
//					   current_pulser1++;
//			#else
//					 // pulser_new = __HAL_TIM_GET_COUNTER(&htim5);
//					   current_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);
//			#endif

	 	   }

		   #if (_USE_SOFT_PULSER == 1)
		 	 pulser_new = current_pulser1;
		   #endif

		   if (firstTime == 3)
		       pulser_new = current_pulser1;
//=====================================================================
 		// filling1 = 1;
		// send_pump(ACTIVATE);  //pump on...

//  if(operating_side == side_a)
//	{
		working_volTotaliser1 = totaliser_vol1;
		working_volTotaliser1c = totaliser_vol1c;
		working_amtTotaliser1 = totaliser_amt1;
		working_amtTotaliser1c = totaliser_amt1c;
//	}
//  else
//   {
//  		working_volTotaliser1 = totaliser_vol2;
//  		working_volTotaliser1c = totaliser_vol2c;
//  		working_amtTotaliser1 = totaliser_amt2;
//  		working_amtTotaliser1c = totaliser_amt2c;
//  	}

  r_volTotaliser1 = floor( working_volTotaliser1c );
  old_r_volTotaliser1 = r_volTotaliser1;

//  r_amtTotaliser = floor(working_amtTotaliser1c);
//  old_r_amtTotaliser = r_amtTotaliser;

 // slow_flow1();

	return authorised_nozzleup_State;
}



//------------------
uint32_t price2pulser(float price)
{
//  float temp = (price / litre_price) *  pulser_index_c;
//  display_minimumPulser = (0.09 * pulser_index_c);   //9 centilitres
//  original_pulse = temp;
  //temp = floor(temp);
  float temp = (price / litre_price);
//  temp -= 0.01;

  temp -= 0.004;

  temp *= pulser_index_c;
  original_pulse = temp;
//  temp -= 4;
  return  floor(temp);
}

uint32_t amt2pulser(float amt)
{
	 float temp;

//	 amt -= 0.01;

	 amt -= 0.004;

	 temp = amt * pulser_index_c;
//	 display_minimumPulser = (0.09 * pulser_index_c);   //9 centilitres
	 original_pulse = temp;
	 return  floor(temp);
}

float pulser2price(uint32_t pulse_)
{
	 float temp = litre_price * (1.0/pulser_index_c);
	 temp  = temp * (float) pulse_;
	 return (temp );
}

float amt2price(float amt_)
{
	 float temp = litre_price * amt_;
	 return (temp );
}

float pulser2amt(uint32_t pulse_)
{
	 float temp = (float)pulse_ * (1.0/pulser_index_c);
	 return (temp );
}

float pulser2amt_R(uint32_t pulse_)
{
	 float temp = pulse_ * (1.0/pulser_index);
	 //temp  = temp * (float) pulse_;
	 return (temp );
}
//---------------------------------------------------
void make_string(sellmode_ sll, float pr)
{
    int8_t tmp = 1,
    	   ind = 0;

    char *endPtr;

	char temp[10] = {0};
	char ttmp[10] = {0};

	 if (sll == L)
	 {
		 if (dp_vol1 == 1)
		     snprintf(temp , sizeof(temp), "%.1f", pr);
		 else if (dp_vol1 == 2)
			 snprintf(temp , sizeof(temp), "%.2f", pr);
		 else if (dp_vol1 == 3)
		 	 snprintf(temp , sizeof(temp), "%.3f", pr);

//		 amt_middle1 = atof(temp);
//		 amt_middle1 += 0.00011;
		 amt_middle1 = strtof(temp, &endPtr);

		 if(pr < display_minimumCentilitre1)   // 9 centilitres
		 {
			 pr = 0.00;

			 if (dp_vol1 == 1)
				 snprintf(temp , sizeof(temp), "%.1f", pr);
			 else if (dp_vol1 == 2)
				 snprintf(temp , sizeof(temp), "%.2f", pr);
			 else if (dp_vol1 == 3)
				 snprintf(temp , sizeof(temp), "%.3f", pr);
		 }
	 }

	 if (sll == P)
	 {
		 if (dp_amount1 == 1)
			 snprintf(temp , sizeof(temp), "%.1f", pr);
		 else if (dp_amount1 == 2)
			 snprintf(temp , sizeof(temp), "%.2f", pr);
		 else if (dp_amount1 == 3)
			 snprintf(temp , sizeof(temp), "%.3f", pr);

//		price_upper1 = atof(temp);
//		price_upper1 += 0.00011;

		price_upper1 = strtof(temp, &endPtr);

		if(pr < display_minimumCentilitrePrice1)   // 9 centilitres
		{
			 pr = 0.00;

			if (dp_amount1 == 1)
				 snprintf(temp , sizeof(temp), "%.1f", pr);
			else if (dp_amount1 == 2)
				 snprintf(temp , sizeof(temp), "%.2f", pr);
			else if (dp_amount1 == 3)
				 snprintf(temp , sizeof(temp), "%.3f", pr);
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

	if(eNextState1 == filling_State)
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
		 for (uint8_t i = 0 ; i < 10 ; i++)
		   {
				if (sll == P)
				{
					upper1[i] = ttmp[i];

//					if(settings[0].display_mode == PL)
//						upper1[i] = ttmp[i];
//					else if(settings[0].display_mode == LP)
//						middle1[i] = ttmp[i];
				}
				else
				{
					middle1[i] = ttmp[i];

//					if(settings[0].display_mode == PL)
//						middle1[i] = ttmp[i];
//					else if(settings[0].display_mode == LP)
//						upper1[i] = ttmp[i];
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
eSystemState filling_pulse_Handler(void)
{
    //initialise the solenoid and motor...

	pump_status_1 = STATUS_FILLING;

	status_change_pump1 = 1;

//	lcd_print_line1(upper1);
//	lcd_print_line2(middle1);

	if(settings_stream1[0].display_format == PL)
	  {
		 lcd_print_line1(upper1);
		 lcd_print_line2(middle1);
	  }
	  else if(settings_stream1[0].display_format == LP)
	  {
		  lcd_print_line1(middle1);
		  lcd_print_line2(upper1);
	  }

	char str__[10]= {0};
		snprintf(str__, sizeof(str__), "%.2f", litre_price);
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
eSystemState filling_resumed_Handler(void)
{

	return filling_State;
}



eSystemState keypad_entry_State_Handler(void)
{

    return  ePrevState; //
	//return keypad_entry_State;
}

//----------------------------------------
eSystemState filling_State_Handler(void)
{
	//extern uint32_t pulser_new;

	if(filling_mamo_flag1 == 1)
	{
		return filling_State;
	}

	float temp = 0.0;

	static int8_t flow_flag = 1;

	static int32_t current_pulser_ = 0,
					   old_pulser_ = 0;

	pump1_status_4G = STATUS_FILLING_;

    filling1  = 1;

    firstTime_nozz1 = 1;

    lock_clr = 0;

	  if(target_pulser1 > 0)
	  {
		 //	pulser_rem1 = target_pulser1 - current_pulser1; //now handled in
														 //interrupt
	  }

//================================================================

//	#if !defined (DEV_MODE)
	#if sense_battery == 1
	  	if(batteryStatus == NOBATTERY)
		{
			  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
			  HAL_Delay(200);
			  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
			  filling1 = 0;
			  stop_flow1();
			  get_time();
			  do_calcs();
			  update_info();
			  save_volumeTotaliser(operating_side);
			  save_amountTotaliser(operating_side);
//			  save_lastSale(operating_side);
			  save_lastSale_fram(operating_side);

			  if(settings_stream1[0].mode == AUTO_MODE)
			  {
				 //////////////////////////////////////////////////////////////

				 hardwareErrorFlag_source1 = 1;

				 //////////////////////////////////////////////////////////////
			  }

			  return write_flash_State;
		}
	#endif     //#if sense_battery == 1

//    power outage during filling1  end transaction...
	#if sense_power == 1
	  if( (readpwr() == 0)||(read_p_pwr() == 0) )
	  {
		  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
		  HAL_Delay(200);
		  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
		  filling1 = 0;
		  stop_flow1();
		  get_time();
		  do_calcs();
		  update_info();
//		  save_volumeTotaliser(operating_side);
//		  save_amountTotaliser(operating_side);
//		  save_lastSale(operating_side);

		  save_totaliser_fram(side_a);
//		  save_amountTotaliser_fram(side_a);
		  save_lastSale_fram(side_a);

		  if(settings_stream1[0].mode == AUTO_MODE)
		  {
			 //////////////////////////////////////////////////////////////

			 hardwareErrorFlag_source1 = 1;

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
			  filling1 = 0;
			  stop_flow1();
			  get_time();
			  do_calcs();
			  update_info();
			  save_volumeTotaliser(operating_side);
			  save_amountTotaliser(operating_side);
//			  save_lastSale(operating_side);
			  save_lastSale_fram(operating_side);

			  if(settings_stream1[0].mode == AUTO_MODE)
			  {
				 //////////////////////////////////////////////////////////////

				 hardwareErrorFlag_source1 = 1;

				 //////////////////////////////////////////////////////////////
			  }

			  return write_flash_State;
		}
	#endif   //#if !defined (DEV_MODE)

	if(settings_stream1[0].mode == AUTO_MODE)
	{
		if(timer_go >= TIMEOUT_GO)   //if go's timeout is 5sec threshold
		{
			go_timeOut1 = 1;

			HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
			HAL_Delay(200);
			HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
			filling1 = 0;
			stop_flow1();
			get_time();
			do_calcs();
			update_info();

			save_totaliser_fram(side_a);
//			save_amountTotaliser_fram(side_a);
			save_lastSale_fram(side_a);

			return write_flash_State;
		}
	}


	if (stop_flag == 1)   //if stop key pressed
	{
		filling1 = 0,
		nozzle_bit = 0;

		stop_flag = 0;
		stop_flow1(); //send_solenoid(1);  //stop solenoid.
		get_time();
        do_calcs();
        update_info();
//        save_volumeTotaliser(operating_side);
//        save_amountTotaliser(operating_side);
//        save_lastSale(operating_side);

        save_totaliser_fram(side_a);
//        save_amountTotaliser_fram(side_a);
        save_lastSale_fram(side_a);

        //--------------------------------------------------------
//        	dpFlag = 0;
//        	error_clr_flag = 1;
//        	 index_ = 0;
//        	 _index = 0;
//        	 for(int i = 0; i < 9; i++)
//        	 {
//        	   keypad_pw_xter1[i] = 0;
//        	   keyboard_entry[i] = 0;   //clear the buffer
//        	 }
//
//        	 for(int i = 0; i <= 8; i++)
//        	 {
//        		 keyboard[i] = 0;
//        	 }
//        	 //--------------------------------------------------------------------
//        	 if (sellmode == P)
//        	 {
//        		  write_v(3, "P     0");  //send_keypad("p    ");  //5 xters  lafeng..
//        	 }
//        	 else if(sellmode == L)
//        	 {
//        		  write_v(3, "L     0");  //send_keypad("l    ");  //5 xters lafeng
//        	 }
//        //	 else if(sellmode == V)
//        //	 {
//        //		  write_v(3, "v    0");  //send_keypad("p    ");  //5 xters  lafeng..
//        //	 }
//        	 send_keypad(keyboard);

        	 keypad_zerorize();

//        	 pump_status_ = STATUS_FILLING_COMP;

        	 if(settings_stream1[0].mode == AUTO_MODE)
        	 {
				 //////////////////////////////////////////////////////////////
				 ///////// SIGNALS GO-CONTROLLER ABOUT NOZZLE STATUS //////////

				 status_change_noz1 = 1;
				 nozzle_out1 = false;

				 stopFlag_source1 = 1;

				 //////////////////////////////////////////////////////////////
    		 }

       return write_flash_State;
	}

	if(litre_price1 == 0)
	{
		stop_flow1(); //send_solenoid(1);  //stop solenoid.
		filling1 = 0;
		do_calcs();
		get_time();
		update_info();
//		save_volumeTotaliser(operating_side);
//		save_amountTotaliser(operating_side);
//		save_lastSale(operating_side);

		save_totaliser_fram(side_a);
//		save_amountTotaliser_fram(side_a);
		save_lastSale_fram(side_a);
		_litre_price1 = 1;

		  if(settings_stream1[0].mode == AUTO_MODE)
		  {
			 //////////////////////////////////////////////////////////////

			 hardwareErrorFlag_source1 = 1;

			 //////////////////////////////////////////////////////////////
		  }

		return write_flash_State;
	}
	else if(pump_max_litres1 == 0)
	{
		stop_flow1(); //send_solenoid(1);  //stop solenoid.
		filling1 = 0;
		do_calcs();
		get_time();
		update_info();
//		save_volumeTotaliser(operating_side);
//		save_amountTotaliser(operating_side);
//		save_lastSale(operating_side);

		save_totaliser_fram(side_a);
//		save_amountTotaliser_fram(side_a);
		save_lastSale_fram(side_a);
		_pump_max_litres1 = 1;

		  if(settings_stream1[0].mode == AUTO_MODE)
		  {
			 //////////////////////////////////////////////////////////////

			 hardwareErrorFlag_source1 = 1;

			 //////////////////////////////////////////////////////////////
		  }

		return write_flash_State;
	}
//======================== @ filling1 =============================
	  // get_time2();
			   temp = pulser2amt_R(current_pulser1);
	   amt_real1 = dp(temp, dp_vol1);
	   	   temp = pulser2amt(current_pulser1);
	   amt = dp(temp, dp_vol1);
	   	   	   temp = amt2price(amt);
	   price = dp(temp, dp_amount1);

	    make_string(P, dp(price, dp_amount1));
	    make_string(L, dp(amt, dp_vol1));
	    //lcd_print_line1(upper1);
	    //lcd_print_line2(middle1);

//	    running_volTotaliser1  = working_volTotaliser1  + amt_real1;
//	   	running_volTotaliser1c = working_volTotaliser1c + amt;
//
//	    running_amtTotaliser1  = working_amtTotaliser1  + price_real1;
//		running_amtTotaliser1c = working_amtTotaliser1c + price;

//		amt_middle1 = atof(middle1);
//		amt_middle1 += 0.00011;
//
//		price_upper1 = atof(upper1);
//		price_upper1 += 0.00011;

		running_volTotaliser1 = working_volTotaliser1 + amt_real1;
	//	   	  running_volTotaliser1c = working_volTotaliser1c + amt;

		running_volTotaliser1c = working_volTotaliser1c + amt_middle1;

		running_amtTotaliser1 = working_amtTotaliser1 + price_real1;
	//	   	  running_amtTotaliser1 = working_amtTotaliser1 + price;

	   	running_amtTotaliser1c = working_amtTotaliser1c + price_upper1;

	   	float pricecheck = running_amtTotaliser1c - priceOld1;

	    if (pricecheck >= 1000.00)
	    {
		   priceOld1 = running_amtTotaliser1c;
		   save_amountSend(side_a);

		   char str[65];
		   sprintf(str, "[Side-A]... #%0.2f intermittent worth of sales made now!", pricecheck);
		   server_write(str);
	    }
////============================================================
//         for totaliser toggle.
	  r_volTotaliser1 	  = floor( running_volTotaliser1c );
//	  r_amtTotaliser 	  = floor(running_amtTotaliser1c);
//
//	if(r_volTotaliser1 != old_r_volTotaliser1)
//	{
//		totalizer1Timer = 0;
////			then toggle the totaliser harware I/O.
//		drive_totaliser1(ACTIVATE);
//		countar++;
//	}
//	else
//	{
//		//deactivate totaliser output...
//		if(totalizer1Timer > 200)
//		{
//			drive_totaliser1(DEACTIVATE);
//			countar2++;
//		}
//
//	}
//	  old_r_volTotaliser1 = r_volTotaliser1;   //update...
//	  old_r_amtTotaliser = r_amtTotaliser;
//============================================================
	  if (t > LCD_UPDATE_RATE)
	  {
		    if(settings_stream1[0].display_format == PL)
			{
				lcd_print_line1(upper1);
				lcd_print_line2(middle1);
			}
			else if(settings_stream1[0].display_format == LP)
			{
				lcd_print_line1(middle1);
				lcd_print_line2(upper1);
			}

			char str__[9]= {0};
			snprintf(str__, sizeof(str__), "%.2f", litre_price);
			lcd_print_line3(str__);

			keypad_fillingUpdate1();

		    t = 0;
	  }

	  reset_timer(timeout_dispense); //don't time out.
//==========================pulser capture ============================
#if (_USE_SOFT_PULSER == 1)
	 // if ( (t > 50)&&(t < 200) )
	 //  			{
	 //  	    	   current_pulser1++;   //transfer this to the interrupt routine..
	 //  			}
#else
//	       current_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);
#endif
//======================================================================
//	   if(target_pulser1 > 0)
//	   {
//			//pulser_rem1 = target_pulser1 - current_pulser1;
//	   }

	 if(target_pulser1 > 0)
     {
		  if (pulser_rem1 <= 0 || pulser_complete1 == 1 )  //sales complete...
		  {
				stop_flow1(); //send_solenoid(1);  //stop solenoid.
				filling1 = 0;
				do_calcs();
				get_time();
				update_info();
	//		        save_volumeTotaliser(operating_side);
	//		        save_amountTotaliser(operating_side);
	//		        save_lastSale(operating_side);

				save_totaliser_fram(side_a);
	//				save_amountTotaliser_fram(side_a);
				save_lastSale_fram(side_a);

				pump_status_1 = STATUS_MAMO_REACHED;

//				target_pulser1 = 0;

				if(settings_stream1[0].mode == AUTO_MODE)
				{
					mamo_reached_flag1_1 = 1;
				}

				return write_flash_State;
		  }
		  else
		  {
			//  slow_flow1();
//			  if (pulser_rem1 >= fast_flow_threshold)
//				  {
//				    fast_flow1();
//				  }
			  if(pulser_rem1 >= slowFlow_endThreshold1)
			  {
					if(current_pulser1 >= slowFlow_startThreshold1)
					{
						fast_flow1();

						fastFlow1 = 1;
					}
					else
					{
						slow_flow1();

						fastFlow1 = 0;

					}
			  }
			  else
			  {
				  slow_flow1();

				  fastFlow1 = 0;
			  }
		  }
       }

	   if(flow_flag == 1)
	   {
	 		_tt1 = 0;
	   }
	   else if( (_tt1 > 60000) && (flow_flag == 2) )
	   {
	 		old_pulser_ = current_pulser1;
	 		flow_flag = 3;
	   }
	   else if( (_tt1 > 65000) && (flow_flag == 3) )
	 	{
	 		if(old_pulser_ > current_pulser1)
	 		{
	 			current_pulser1 = old_pulser_ ;
	 			flow_loss = 1;
	 			filling1 = 0,  nozzle_bit = 0;
				stop_flag = 0;
				stop_flow1(); //send_solenoid(1);  //stop solenoid.
				get_time();
				do_calcs();
				update_info();
//				save_volumeTotaliser(operating_side);
//				save_amountTotaliser(operating_side);
//				save_lastSale(operating_side);
//				save_lastSale_fram(operating_side);

				save_totaliser_fram(side_a);
//				save_amountTotaliser_fram(side_a);
				save_lastSale_fram(side_a);

				keypad_zerorize();

				  if(settings_stream1[0].mode == AUTO_MODE)
				  {
					 //////////////////////////////////////////////////////////////

					 hardwareErrorFlag_source1 = 1;

					 //////////////////////////////////////////////////////////////
				  }

			    return write_flash_State;
	 		}
	 		flow_flag = 1;
	 	}

	   return filling_State;
}



//--------------------------------------------------------------
void do_calcs ()
{

	float temp;
	 if(target_pulser1 > 0)  // if price or volume is programmed
	 {
	   if (pulser_rem1 <= 0 || pulser_complete1 == 1 )  //sales complete...
	   {
		   //get_time2();

			if (sellmode == P)
			{

			   //---------------------------------------------------------
				//price = dp(key_value,dp_price);   // temp = amt2price(amt);
				//amt   = dp( price_/litre_price ,dp_amount);

				 temp = pulser2amt(current_pulser1);
							 amt = dp(temp, dp_vol1);

				 temp = amt2price(amt);
							 price = dp(temp, dp_amount1);

				if(key_value_sellmodeP1 == 1)
				{
					key_value_sellmodeP1 = 0;
					key_value = key_value_original1;
				}

				/********************************************
				 *
				 * SHOW WHAT THE USER NEEDS...
				 *
				 * ******************************************/
				 price_ = dp(key_value, dp_amount1);
					 temp = price_/litre_price;
				 amt_   = dp(temp, dp_vol1); //calculate vol frm price.

				//=========================================================
				//    Also calculate the values based on the real P.Indx
						  temp  = pulser2amt_R(current_pulser1);
					  amt_real1 = dp(temp, dp_vol1);
						  temp = amt2price(amt_real1);
					  price_real1 = dp(temp, dp_amount1);
				//=========================================================
			   }

			  if (sellmode == L)
			  {
					temp = pulser2amt(target_pulser1);
					amt  = dp(temp, dp_vol1);

					price  = amt2price(amt);

					if(key_value_sellmodeL1 == 1)
					{
						key_value_sellmodeL1 = 0;
						key_value = key_value_original1;
					}

					/********************************************
					 *
					 * SHOW WHAT THE USER NEEDS TO SEE...
					 *
					 * ******************************************/
					amt_   = key_value;
					price_ = amt_ * litre_price; //calculate price from price.amt
					//=========================================================
					//    Also calculate the values based on the real P.Indx
							  temp  = pulser2amt_R(current_pulser1);
						  amt_real1 = dp(temp, dp_vol1);
							  temp = amt2price(amt_real1);
						  price_real1 = dp(temp, dp_amount1);
					//=========================================================
			   }
			 //------------------------------------------------------------------

			  make_string(P, dp(price_, dp_amount1));
			  make_string(L, dp(amt_, dp_vol1));

			  if(settings_stream1[0].display_format == PL)
			  {
				 lcd_print_line1(upper1);
				 lcd_print_line2(middle1);
			  }
			  else if(settings_stream1[0].display_format == LP)
			  {
				  lcd_print_line1(middle1);
				  lcd_print_line2(upper1);
			  }

			  running_volTotaliser1 = working_volTotaliser1 + amt_real1;
	//	   	  running_volTotaliser1c = working_volTotaliser1c + amt;

			  running_volTotaliser1c = working_volTotaliser1c + amt_middle1;

			  running_amtTotaliser1 = working_amtTotaliser1 + price_real1;
	//	   	  running_amtTotaliser1 = working_amtTotaliser1 + price;

			  running_amtTotaliser1c = working_amtTotaliser1c + price_upper1;

			  float pricecheck = running_amtTotaliser1c - priceOld1;

				if (pricecheck >= 1000.00)
				{
				   priceOld1 = running_amtTotaliser1c;
				   save_amountSend(side_a);

				   char str[65];
				   sprintf(str, "[Side-A]... #%0.2f intermittent worth of sales made now!", pricecheck);
				   server_write(str);
				}

			   totaliser_vol1 = running_volTotaliser1;    // update totaliser
			   totaliser_vol1c = running_volTotaliser1c;  // update totaliser
			   totaliser_amt1 = running_amtTotaliser1;    // update totaliser
			   totaliser_amt1c = running_amtTotaliser1c;  // update totaliser

			//============================================================

			   //         for totaliser toggle.

				r_volTotaliser1 	  = floor( running_volTotaliser1c );

				if(r_volTotaliser1 != old_r_volTotaliser1)
				{
					totalizer1Timer = 0;

			  //			then toggle the totaliser harware I/O.

					drive_totaliser1(ACTIVATE);

					countar++;
				}
				else
				{
					//deactivate totaliser output...

					if(totalizer1Timer > 200)
					{
						drive_totaliser1(DEACTIVATE);
					}

				}
				  old_r_volTotaliser1 = r_volTotaliser1;   //update...

			//-------------------------------------------------------------------

			   return;
		 }
	   else
	   {
		  //programmed but still dispensing @ stop pt.
			 temp = pulser2amt(current_pulser1);  amt   = dp(temp,dp_vol1);
			 temp = amt2price(amt);   			price = dp(temp,dp_amount1);

			  make_string(P, dp(price, dp_amount1));
			  make_string(L, dp(amt, dp_vol1));

			//=========================================================
			//    Also calculate the values based on the real P.Indx
						  temp  = pulser2amt_R(current_pulser1);
					  amt_real1 = dp(temp,dp_vol1);
						  temp = amt2price(amt_real1);
					  price_real1 = dp(temp, dp_amount1);
			//=========================================================

	//		  lcd_print_line1(upper1);
	//		  lcd_print_line2(middle1);
			  if(settings_stream1[0].display_format == PL)
			  {
				 lcd_print_line1(upper1);
				 lcd_print_line2(middle1);
			  }
			  else if(settings_stream1[0].display_format == LP)
			  {
				  lcd_print_line1(middle1);
				  lcd_print_line2(upper1);
			  }


			  running_volTotaliser1 = working_volTotaliser1 + amt_real1;
	//	   	  running_volTotaliser1c = working_volTotaliser1c + amt;

			  running_volTotaliser1c = working_volTotaliser1c + amt_middle1;

			  running_amtTotaliser1 = working_amtTotaliser1 + price_real1;
	//	   	  running_amtTotaliser1 = working_amtTotaliser1 + price;

			  running_amtTotaliser1c = working_amtTotaliser1c + price_upper1;

			  float pricecheck = running_amtTotaliser1c - priceOld1;

				if (pricecheck >= 1000.00)
				{
				   priceOld1 = running_amtTotaliser1c;
				   save_amountSend(side_a);

				   char str[65];
				   sprintf(str, "[Side-A]... #%0.2f intermittent worth of sales made now!", pricecheck);
				   server_write(str);
				}

			   totaliser_vol1 = running_volTotaliser1;    // update totaliser
			   totaliser_vol1c = running_volTotaliser1c;  // update totaliser
			   totaliser_amt1 = running_amtTotaliser1;    // update totaliser
			   totaliser_amt1c = running_amtTotaliser1c;  // update totaliser

			 //-------------------------------------------------------------------
			   return;
	   }
	 }
	 else   // if target_pulser1 == 0
	 {
		 //if un programmed...
		 //still dispensing @ stop pt.

			 temp = pulser2amt(current_pulser1);
				  amt = dp(temp,dp_vol1);

			 temp = amt2price(amt);
				 price = dp(temp,dp_amount1);

			//compare the final and initial prices...
				 float ddp = 0;
				 if      (dp_amount1 == 1) ddp =  0.1;
				 else if (dp_amount1 == 2) ddp =  0.01;
				 else if (dp_amount1 == 3) ddp =  0.001;

				 if( (temp - price) > ( (ddp/10)*2) )  //result was rounded down.
				 {

					 //need to do a little fixing here...
					 price = price + ddp;
				 }

			  make_string(P, price); //,dp_price)
			  make_string(L, amt);  //,dp_amount)

			//=========================================================
			//    Also calculate the values based on the real P.Indx
					  temp  = pulser2amt_R(current_pulser1);
				  amt_real1 = dp(temp,dp_vol1);
					  temp = amt2price(amt_real1);
				  price_real1 = dp(temp, dp_amount1);
			//=========================================================

	//		  lcd_print_line1(upper1);
	//		  lcd_print_line2(middle1);

			  if(settings_stream1[0].display_format == PL)
			  {
				 lcd_print_line1(upper1);
				 lcd_print_line2(middle1);
			  }
			  else if(settings_stream1[0].display_format == LP)
			  {
				  lcd_print_line1(middle1);
				  lcd_print_line2(upper1);
			  }

			  running_volTotaliser1 = working_volTotaliser1 + amt_real1;
	//	   	  running_volTotaliser1c = working_volTotaliser1c + amt;

			  running_volTotaliser1c = working_volTotaliser1c + amt_middle1;

			  running_amtTotaliser1 = working_amtTotaliser1 + price_real1;
	//	   	  running_amtTotaliser1 = working_amtTotaliser1 + price;

			  running_amtTotaliser1c = working_amtTotaliser1c + price_upper1;

			  float pricecheck = running_amtTotaliser1c - priceOld1;

				if (pricecheck >= 1000.00)
				{
				   priceOld1 = running_amtTotaliser1c;
				   save_amountSend(side_a);

				   char str[65];
				   sprintf(str, "[Side-A]... #%0.2f intermittent worth of sales made now!", pricecheck);
				   server_write(str);
				}

			   totaliser_vol1 = running_volTotaliser1;    // update totaliser
			   totaliser_vol1c = running_volTotaliser1c;  // update totaliser
			   totaliser_amt1 = running_amtTotaliser1;    // update totaliser
			   totaliser_amt1c = running_amtTotaliser1c;  // update totaliser

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
void state_ini(void)
{
	float temp = 0.0;

	irrecov_flag = 0;
	progg = 0;

	pump_status_1 = STATUS_PNP;
//    pump_status_2 = STATUS_PNP;

    if(settings_stream1[0].mode != AUTO_MODE)
	{
		eNextState1 =  idle_State; //   prog_State; //
		eLastState1 =  idle_State;
	}
    else
    {
    	eNextState1 =  pnp_State;
		eLastState1 =  pnp_State;
    }

	index_ = 0;

	filling1 = 0;
	stop_flow1();


//	lastVolumeSale1 = lastSale_storeA.lastVolumeSale_real;
//	lastVolumeSale1c =  lastSale_storeA.lastVolumeSale_cal;

//	temp = pulser2amt(current_pulser1);
//	amt = dp(temp,dp_amount);
//    temp = amt2price(lastVolumeSale1);
//	price = dp(temp, dp_price);

//	char str_p[10] = {0};
//	char str_l[10] = {0};
//
//	snprintf(str_p, sizeof(str_p), "%.2f", price);
//	snprintf(str_l, sizeof(str_l), "%.2f", lastVolumeSale1);

//	write_v(1, "p        ");
	if(settings_stream1[0].display_format == PL)
	{
		lcd_print_line1("P        ");
		write_v(1, "p        ");
	}
	else if(settings_stream1[0].display_format == LP)
	{
		lcd_print_line1("L        ");
		write_v(1, "l        ");
	}

//	write_v(2, "l        ");
	if(settings_stream1[0].display_format == PL)
	{
		lcd_print_line2("L        ");
		write_v(2, "l        ");

		make_string(P, dp(lastAmountSale1c, dp_amount1) );
		    make_string(L, dp(lastVolumeSale1c, dp_vol1) );
	}
	else if(settings_stream1[0].display_format == LP)
	{
		lcd_print_line2("P        ");
		write_v(2, "p        ");
	}

//    temp = amt2price(lastVolumeSale1c);
//	price = dp(temp, dp_amount1);
//	make_string(P, dp(price, dp_amount1) );

	make_string(P, dp(lastAmountSale1c, dp_amount1) );
    make_string(L, dp(lastVolumeSale1c, dp_vol1) );

//	lcd_print_line1(str_p);
//	lcd_print_line2(str_l);

//	printDisp_c("p",1,0,8,LT,CLEAR); printDisp_f(str_p,1,0,8,LT,NOCLEAR);  //display price
//
//	printDisp_c("l",2,0,8,LT,CLEAR); printDisp_f(str_l,2,0,8,LT,NOCLEAR);  //display litre

	char str__[10] = {0};
	snprintf(str__, sizeof(str__), "%.2f", litre_price);
	lcd_print_line3(str__);

	 if(sellmode == L)
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
	 keypad_print(keyboard);
}

//void states(void)
//{
//	eNewEvent1 = read_event1();
//	if (eNewEvent1 != _keypress_Event)
//	{
//        //ePrevState = eNextState1;
//	}
//
//	eSystemEvent ev;
//	  if( (eNewEvent1 < _no_Event)  )  //if event occured, check if the current state is sensitive to it..
//	  {
//		  eLastState1 = eNextState1;   // store state...
//           if(eNewEvent1   == _auth_command_Event) //_nozzleup_Event) //_filling_pulse_Event  _nozzleup_Event)
//            {
//           	  //int yiuyu = 0;
//            }
//          //...   scan through the allowed  events of the state if its among them..
//	    for (int i = 0; i < max_events_per_state; i++)
//		 {
//		   ev = (asStateEventMachine[eNextState1].states[i]);  //
//		   if (ev == 0)
//			{
//			   // eNextState1 = eLastState1;   //stay in the same event..
//				break;  // we reached end of event in allowed events array, exit scan..
//			}
//		   if (eNewEvent1 == ev) // if the just triggered event is among allowed events..
//		   {
//                //then check if it has an handler and switch to the event Handler...
//			   if (asEventMachine[ev-1].pfEventMachineHandler == NULL) //no handler supplied
//			   {
//				   break; //eNextState1 = (*asStateEventMachine[eNextState1].pfStateMachineHandler)(); //switch to state handler.
//			   }
//			   else
//			   {
//				   eNextState1 = (*asEventMachine[eNewEvent1-1].pfEventMachineHandler)();  //switch to event handler.
//				   eNewEvent1 = _no_Event;
//				   // int ty = 0;
//
//			   }
//		   } // if (eNewEvent1 == ev)
//		 }  ///for
//
//	     eNextState1 = (*asStateEventMachine[eNextState1].pfStateMachineHandler)(); //switch to state handler.
//	    // int yy = 56;
//	  }
//	 else
//	  {
//		//int s0 = (asStateEventMachine[eNextState1].states[0]);
//		//int s1 = (asStateEventMachine[eNextState1].states[1]);
//		//int s2 = (asStateEventMachine[eNextState1].states[2]);
//
//		  // no event, switch to same state...
//		  eNextState1 = (*asStateEventMachine[eNextState1].pfStateMachineHandler)();  //
//					  //  (*asStateEventMachine[eNextState1].pfStateMachineHandler)();
//		 // int yui = 0;
//		  return;
//	  }
//
//
//	  /*
//	if((eNextState1 < last_State) && (eNewEvent1 < last_Event)&& (asStateMachine[eNextState1].eStateMachineEvent == eNewEvent1) && (asStateMachine[eNextState1].pfStateMachineEvnentHandler != NULL))
//		{
//			// function call as per the state and event and return the next state of the finite state machine
//			eNextState1 = (*asStateMachine[eNextState1].pfStateMachineEvnentHandler)();
//		}
//		else
//		{
//
//		}  */
//	  //eNextState1 = (*asStateEventMachine[eNextState1].pfStateMachineHandler)();
//	 // return;
//}


//void states2(void)
//{
//	eNewEvent2 = read_event2();
//	if (eNewEvent2 != _keypress_Event)
//	{
//        //ePrevState = eNextState1;
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
//			       // eNextState1 = eLastState1;   //stay in the same event..
//			        break;  // we reached end of event in allowed events array, exit scan..
//			    }
//		   if (eNewEvent2 == ev) // if the just triggered event is among allowed events..
//		   {
//                //then check if it has an handler and switch to the event Handler...
//			   if (asEventMachine[ev-1].pfEventMachineHandler == NULL) //no handler supplied
//			   {
//				   break; //eNextState1 = (*asStateEventMachine[eNextState1].pfStateMachineHandler)(); //switch to state handler.
//			   }
//			      else
//			   {
//				   eNextState2 = (*asEventMachine[eNewEvent2-1].pfEventMachineHandler)();  //switch to event handler.
//				   eNewEvent2 = _no_Event;
//				   // int ty = 0;
//
//			   }
//		   } // if (eNewEvent1 == ev)
//		 }  ///for
//
//	     eNextState2 = (*asStateEventMachine[eNextState2].pfStateMachineHandler)(); //switch to state handler.
//	    // int yy = 56;
//	  }
//	 else
//	  {
//		//int s0 = (asStateEventMachine[eNextState1].states[0]);
//		//int s1 = (asStateEventMachine[eNextState1].states[1]);
//		//int s2 = (asStateEventMachine[eNextState1].states[2]);
//
//		  // no event, switch to same state...
//		  eNextState2 = (*asStateEventMachine[eNextState2].pfStateMachineHandler)();  //
//					  //  (*asStateEventMachine[eNextState1].pfStateMachineHandler)();
//		 // int yui = 0;
//		  return;
//	  }
//
//
//	  /*
//	if((eNextState1 < last_State) && (eNewEvent1 < last_Event)&& (asStateMachine[eNextState1].eStateMachineEvent == eNewEvent1) && (asStateMachine[eNextState1].pfStateMachineEvnentHandler != NULL))
//		{
//			// function call as per the state and event and return the next state of the finite state machine
//			eNextState1 = (*asStateMachine[eNextState1].pfStateMachineEvnentHandler)();
//		}
//		else
//		{
//
//		}  */
//	  //eNextState1 = (*asStateEventMachine[eNextState1].pfStateMachineHandler)();
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
	eNewEvent1 = read_event1_1();
	if (eNewEvent1 != _keypress_Event)
	{
        //ePrevState = eNextState1;
	}

	eSystemEvent ev;
	  if( (eNewEvent1 < _no_Event)  )  //if event occured, check if the current state is sensitive to it..
	  {
		  eLastState1 = eNextState1;   // store state...
           if(eNewEvent1   == _auth_command_Event) //_nozzleup_Event) //_filling_pulse_Event  _nozzleup_Event)
            {
           	  uint8_t yiuyu = 0;
            }
          //...   scan through the allowed  events of the state if its among them..
//	    for (uint8_t i = 0; i < max_events_per_state; i++)
	    for (uint8_t i = 0; i < 12; i++)
		 {
		   ev = (asStateEventMachine_1[eNextState1].states[i]);  //
		   if (ev == 0)
			{
			   // eNextState1 = eLastState1;   //stay in the same event..
				break;  // we reached end of event in allowed events array, exit scan..
			}
		   if (eNewEvent1 == ev) // if the just triggered event is among allowed events..
		   {
                //then check if it has a handler and switch to the event Handler...
			   if (asEventMachine_1[ev-1].pfEventMachineHandler == NULL) //no handler supplied
			   {
				   break; //eNextState1 = (*asStateEventMachine[eNextState1].pfStateMachineHandler)(); //switch to state handler.
			   }
			   else
			   {

				   eNextState1_1 = eNextState1;
				   eNewEvent1_1 = eNewEvent1;

				   eNextState1 = (*asEventMachine_1[eNewEvent1-1].pfEventMachineHandler)();  //switch to event handler.
				   eNewEvent1 = _no_Event;
				   // int ty = 0;

				   if( (pump_status_1 == STATUS_RESET) && (eNextState1 == idle_State) )
				   {
					   dummyData = 0;
				   }

			   }
		   } // if (eNewEvent1 == ev)
		 }  ///for

	     eNextState1_1 = eNextState1;

	     eNextState1 = (*asStateEventMachine_1[eNextState1].pfStateMachineHandler)(); //switch to state handler.
	    // int yy = 56;

	     if( (pump_status_1 == STATUS_RESET) && (eNextState1 == idle_State) )
		   {
			   dummyData = 0;
		   }

	  }
	 else
	  {
		//int s0 = (asStateEventMachine[eNextState1].states[0]);
		//int s1 = (asStateEventMachine[eNextState1].states[1]);
		//int s2 = (asStateEventMachine[eNextState1].states[2]);

		  eNextState1_1 = eNextState1;

		  // no event, switch to same state...
		  eNextState1 = (*asStateEventMachine_1[eNextState1].pfStateMachineHandler)();  //
					  //  (*asStateEventMachine[eNextState1].pfStateMachineHandler)();
		 // int yui = 0;

		  if( (pump_status_1 == STATUS_RESET) && (eNextState1 == idle_State) )
		   {
			   dummyData = 0;
		   }

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


//----------------------------------------
eSystemState filledmamo_State_Handler(void)
{
//	filling1 = 0,
//	nozzle_bit = 0;

	filling_mamo_flag1 = 0;


	  if (t > LCD_UPDATE_RATE)
	  {

		  if(settings_stream1[0].display_format == PL)
		  {
			 lcd_print_line1(upper1);
			 lcd_print_line2(middle1);
		  }
		  else if(settings_stream1[0].display_format == LP)
		  {
			  lcd_print_line1(middle1);
			  lcd_print_line2(upper1);
		  }
		  char str__[8]= {0};
		  snprintf(str__, sizeof(str_), "%.2f", litre_price);
		  lcd_print_line3(str__);
		  t = 0;
	  }

//	  reset_timer(timeout_dispense); //don't time out.

	   return filledmamo_State;
}

//----------------------------------------
eSystemState pnpState_Handler(void)
{
	static int  printer_status;

	static int8_t idleState_flag = 1;

	static int32_t current_pulser_ = 0,
				   old_pulser_ = 0;

	uint16_t gerCtTime;

	int pulser_diff = 0;

	stop_fueling_bit = 1;

	lock_clr = 0;

	progg = 0;

	firstTime_nozz1 = 1;


	#if sense_power == 1
	  if(readpwr() == 0)
	  {
		   modem_power(DEACTIVATE);

		   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
		   HAL_Delay(200);
		   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);

		   //count time elapsed
		   if (shutdown_timer1 > 120)
		   {
			   displayandkeypad_power(DEACTIVATE);   //shutdown... after  2 minutes
		   }
	  }
	  else
	  {
		  //clear the timer//
		  shutdown_timer1 = 0;

		  modem_power(ACTIVATE);
		  displayandkeypad_power(ACTIVATE);
	  }
	#endif

	#if !defined (DEV_MODE)
		if(batteryStatus == LOWBATTERY)
		{
			lcd_print_line1("  Louu   ");
			lcd_print_line2("Battery  ");
			lcd_print_line3(" Err70 ");

			return inactive_State;
		}
		else if(batteryStatus == NOBATTERY)
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

	//	if(calib_pulser1 < 15800)  //15987, 15967 .... 1106247681
		calibration_flag1 = CALIBRATED;
		if(calibration_flag1 != CALIBRATED) //15800)  //15987, 15967 .... 1106247681
		{
			retrieve_calibrationFlag(side_a);

			if(calibration_flag1 != CALIBRATED) //takes care of accidental clearing of calibration_flag1 by F-keys
			{
				calibration1_error = 1;

				lcd_print_line1("Calibrat");
				lcd_print_line2("  Error ");
				lcd_print_line3("Err 23 ");

				return inactive_State;
			}
		}
		else
		{
			calibration1_error = 0;
		}
	#endif    //#if !defined (DEV_MODE) ,,



	 if  (t > 500)
	 {
		 lcd_print_line1("  Auto  ");
		 lcd_print_line2(" NNode  ");

//		 lcd_print_line3("      ");

		 char str__[8] = {0};
		 snprintf(str__, sizeof(str__), "%.2f", litre_price1);
		 lcd_print_line3(str__);

		 t = 0;
	 }


	 if( (keypress_ == 21) && (nozzleup_awaitingauth_state_not_timedOut == 0) )
	 {
		   compose_printer();
		   HAL_Delay(1000);
		   printer_status = 1;
	 }

	     print__1();   //print the transaction.

	  	return pnp_State;
}



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

	for(uint8_t i = 0; i < 9; i++)
	{
		keypad_pw_xter1[i] = 0;
		keyboard_entry[i] = 0;   //clear the buffer
	}

	for(uint8_t i = 0; i <= 8; i++)
	{
		keyboard[i] = 0;
	}

	//--------------------------------------------------------------------
	if (sellmode == P)
	{
		write_v(3, "P     0");  //send_keypad("p    ");  //5 xters  lafeng..
	}
	else if(sellmode == L)
	{
		write_v(3, "L     0");  //send_keypad("l    ");  //5 xters lafeng
	}

	//	 else if(sellmode == V)
	//	 {
	//		  write_v(3, "v    0");  //send_keypad("p    ");  //5 xters  lafeng..
	//	 }

	keypad_print(keyboard);
}


void keypad_fillingUpdate1(void)
{
	static uint8_t lcd_size = 5;

	if(settings_stream1[0].keypad__  == LAFNG17_K)
	{
		lcd_size = 5; //change this latter to accomodate other lcds.
	}
	else if( (settings_stream1[0].keypad__  == LAFNG18_K) || (settings_stream1[0].keypad__ == LAFNG18_K_V2) )
	{
		lcd_size = 7; //change this latter to accomodate other lcds.
	}
	else if( (settings_stream1[0].keypad__  == BLSKY18_K) || (settings_stream1[0].keypad__  == BLSKY22) )
	{
		lcd_size = 7;   //6;
	}

  //================================================================


	int8_t size = lcd_size;   // No xter to display 'P/L'

	int8_t indexx_ = 0,
		   space = 0;

	uint8_t ind = 0;

	if(sellmode == P)
	{
	indexx_ = strlen(upper1);

	if( (indexx_ <= size) || (indexx_ <= 6) )
	{
		space = size - indexx_;
	}
	else
	   space = 0;  //full size of xters.

	while (space > 0) //write spaces first..
	{
	 keyboard[ind++] = ' ';
	   space--;
	}
	//---------------------------------
	if( (indexx_ <= size) || (indexx_ <= 6) )
		  space = indexx_;
	else
	{
		space = size;
	}

	if( (strchr(upper1, '.')) && (indexx_ > 6) ) space = 8;
	while(space > 0)
	{
	   keyboard[ind++] = upper1[indexx_ - space];
	   space--;
	}

	if(keyboard[ind - 1] == '.') keyboard[ind] = '0';

	}
	else if(sellmode == L)
	{
	indexx_ = strlen(middle1);

	if( (indexx_ <= size) || (indexx_ <= 6) )
	{
		space = size - indexx_;
	}
	else
	   space = 0;  //full size of xters.

	while (space > 0) //write spaces first..
	{
	 keyboard[ind++] = ' ';
	   space--;
	}
	//---------------------------------
	if( (indexx_ <= size) || (indexx_ <= 6) )
		  space = indexx_;
	else
	{
		space = size;
	}

	if( (strchr(middle1, '.')) && (indexx_ > 6) ) space = 8;
	while(space > 0)
	{
	   keyboard[ind++] = middle1[indexx_ - space];
	   space--;
	}

	if(keyboard[ind - 1] == '.') keyboard[ind] = '0';

	}

	keypad_print(keyboard);
}
