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

extern uint8_t prog_entry;
extern uint8_t prog_entry2;


int data_size = 0; //w25qxx.PageSize;    //0;
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

extern pump_status_enum pump_status_;
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

//extern log_ log_a[50],log_b[50];

extern pump_settings settings[2] , copy[2];

extern int8_t ttime[3],
			  ddate[4];

uint32_t target_pulser , current_pulser = 0;

extern int timeout_picknozzle;
extern int timeout_dispense;

static int time ;
static int st = 0;
int timer_flag = 0;
int timer_flag_old = 0;

int tot_buttonpress_tmr = 0;
int log_buttonpress_tmr = 0;
int key_buttonpress_tmr = 0;

extern  int t;
extern uint16_t _tt;
extern uint32_t num ;

//extern float target_pulser , current_pulser ;
extern int sellmode ;

char str_[10]= {0};

//=== screen Arrays ====
char upper[10]  = {0};
char middle[10] = {0};

//=============== keyboard entry ======================
extern char keyboard_entry[8] = {0};  //buffers the key press
char keyboard[7] = {0};

//char temp[9] = {0};

char* xter_buffer1[7] = {0};
char* xter_buffer2[7] = {0};

int kkey = 0;
//=====================================================
//char keyboard_screen[6] = {0};
int  keypress_ = 0;
int index_ = 0;
//uint32_t key_value = 0;
float key_value = 0.0;
//=====================================================
int flow_coeff = 0;
int pulser_rem = 0;
int tflow = 0;
//=====================================================
//  prog variables.
//int progg = 0;
 int prog_index = 0;
 int index2 = 0;
 int index3 = 0;
 int progg  = 0;

 uint32_t calib_pulser = 0;
  int calibr = 0;     //used for signaling motor and solenoid activatn in prog mode.

 auth_state auth = not_auth;   //first set this to
// int side = side1;           //pump side selected.
 int level = level0;
 int _index = 0;

 operatorfxn_  operatorfxn = totaliser_view;

extern char* menu1[4];  //= {"  tot   "," price  ","  log   "};
extern char* menu2[16]; //= {"f.nnode","ch pl","pr. set","f.id","nf time","hi arnt.","com prot","ch. pass","load cfg","dwn. cfg","set log","clr log"};
extern char* menu3[3]; // = {"tmm cfg","flo rate"};
//=====================================================

extern char lafeng_keypad[16];
extern char bluesky_keypad[21];
//extern int pump_type;
extern pump pump_type;
extern int auth_cmd_flag;
extern int8_t opmode;
extern int error_clr_flag;
extern int keypress__ ;

extern long int password_level1;
extern long int password_level2;

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
			 totaliser_amt2c;

extern float working_volTotaliser;
extern float running_volTotaliser;

extern float working_volTotaliserc;
extern float running_volTotaliserc;

extern float working_volTotaliser2,
			running_volTotaliser2,
			working_volTotaliser2c,
			running_volTotaliser2c;

extern float working_amtTotaliser,
			running_amtTotaliser,
			working_amtTotaliserc,
			running_amtTotaliserc;

extern float working_amtTotaliser2,
			running_amtTotaliser2,
			working_amtTotaliser2c,
			running_amtTotaliser2c;

extern int8_t dp_price1,
			  dp_amount1,
			  dp_unitprice1;

extern int stop_flag;

int irrecov_flag = 0;
//extern int error_clr_flag;
//=================================================
char keypad_buf[10] = {0};
char keypad_pw_xter[10] = {0};

char keypad_pw[10] = {0};
char keypad_pw_[10] = {0};
//=================================================
uint32_t price2pulser(float price);
uint32_t amt2pulser(float amt);

uint8_t pulser_complete = 0; //used from the interrupt to signify the completion of pump sales.

//=================================================
char sc1[10] = {32};
int volume_flag = 0;

float original_pulse = 0;

//float price_real,amt_real = 0.0;
//float amt_,price_;

extern uint32_t pulser_new;
uint8_t filling = 0;
bool lock_clr = 0;

extern  uint16_t fast_flow_threshold;
//================================================
//================================================
//extern uint8_t filling_ = 0;
extern uint8_t nozzle_bit ;
extern uint8_t stop_fueling_bit ;

uint8_t change_price = 0;
uint8_t change_volume = 0;
float auth_v, auth_p;
int8_t change_p, change_v;
extern int8_t auth_from_ctrl;
extern int8_t authorise_flag;
extern int8_t change_price_flag;
extern float set_p;

//-------------------------------------------------
//
extern flash_store_info flash_infoA,flash_infoB;
extern uint32_t flash_read_idA;
extern uint32_t flash_read_idB;

uint32_t flash_locc = 0;

extern pump disp_type1,
            disp_type2;

extern uint16_t shutdown_timer;
extern uint16_t motor_tmr;

int8_t pump_LitreOverflow = 0,
	   display_overflow = 0,
	   _litre_price = 0,
	   _auth_p = 0,
	   _auth_v = 0,
	   idle_backwardPulse = 0,
	   idle_forwardPulse = 0,
	   flow_loss = 0;

//-------------   fxn prototypes ------------------
void do_calcs ();
//int long_press_tot();
//int long_press_log();
//------------------------------------------------

uint32_t  r_volTotaliser = 0;
uint32_t  old_r_volTotaliser = 0;

float r_amtTotaliser = 0.00,
	  old_r_amtTotaliser = 0.00;

/*
 * sets the precision of the supplied float
 */
float dp(float flt, int n)
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

 	float temp_ = atof(chrrr);
 	//double temp1_ = atoff(chrrr);
 	return temp_;
}


// initialises the line 1 and 2 display variable.
void write_v(int i, char lcd_[9])
{
	if (i == 1)      //first line
	{
		  for(int ii = 0 ; ii<8 ; ii++)
			{
               upper[ii] = lcd_[ii];
               upper[ii+1] = 0;
			}
	}

	if (i == 2)   //second line
	 {
		  for(int ii = 0 ; ii<8 ; ii++)
			{
				middle[ii] = lcd_[ii];
				middle[ii+1] = 0;
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
   time = tm;
   timer_flag = 0;
   timer_flag_old = 1;
   st = 0;
}

void start_timer(int tm)
{
	time = tm;
	st = 1;
}
//=======================================
void dec_timer(void)
{
  if(st == 1)
	{
	  if (time > 0)time--;
	   if(time == 0)
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
	time = 0;
	st = 0;
}
//=======================================
int8_t read_keypad();
//int long_press_log();
//int long_press_tot();
//=======================================

// state Handlers definition...
eSystemState progstate_Handler(void);
eSystemState idlestate_Handler(void);
eSystemState inactivestate_Handler(void);
eSystemState nozzleup_waitingforauthState_Handler(void);
eSystemState authorised_nozzleup_State_Handler(void);
eSystemState authorised_nozzledown_State_Handler(void);
eSystemState authorisation_paused_State_Handler(void);
eSystemState filling_state_Handler(void);
eSystemState filling_paused_state_Handler(void);
eSystemState keypad_entry_State_Handler(void);
eSystemState operator_State_Handler(void);
eSystemState savesettings_State_Handler(void);

//eSystemState read_flash_state_Handler(void);  //moved to log.h
//eSystemState write_flash_state_Handler(void);

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


//Initialize array of structure of event and event handler
sEventMachine asEventMachine [] =
{
    {_timeout_Event,timeout_Handler},
    {_authorise_Event,authorise_Handler},
   	{_auth_command_Event,auth_command_Handler},
    {_nozzleup_Event,nozzleup_Handler},
   	{_nozzledown_Event,nozzledown_Handler},
    {_pause_Event,pause_Handler},
    {_resume_Event,resume_Handler},
   	{_keyup_Event,keyup_Handler},
   	{_keydown_Event,keydown_Handler},
   	{_keypress_Event,keypress_Handler},
   	{_tot_error_Event,tot_error_Handler},
   	{_key19_Event,key19_Handler},
   	{_filling_paused_Event,filling_paused_Handler},
   	{_filling_resumed_Event,filling_resumed_Handler},
   	{_filling_pulse_Event,filling_pulse_Handler},
	{_error_clear_Event,error_clear_Handler},
	{_operator_Event,operator_Handler},
	{_no_Event,0}
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
	{prog_State, progstate_Handler,{_keydown_Event,_keypress_Event}},
    {idle_State,idlestate_Handler,{_operator_Event,_keyup_Event,_tot_error_Event,_keypress_Event,_nozzleup_Event,_auth_command_Event}},
    {inactive_State,inactivestate_Handler,{_error_clear_Event,_keypress_Event}},
    {nozzleup_waitingforauth_State,nozzleup_waitingforauthState_Handler,{_authorise_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
	{authorised_nozzledown_State,authorised_nozzledown_State_Handler,{_nozzleup_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
    {authorised_nozzleup_State,authorised_nozzleup_State_Handler,{_filling_pulse_Event,_pause_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
	{authorisation_paused_State,authorisation_paused_State_Handler,{_resume_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
	{filling_State,filling_state_Handler,{_filling_paused_Event,_keypress_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
	{filling_paused_State,filling_paused_state_Handler,{_filling_resumed_Event,_keypress_Event,_timeout_Event,_nozzledown_Event}},
	{keypad_entry_State,keypad_entry_State_Handler,{}},
	{operator_State,operator_State_Handler,{_keypress_Event}},
	{savesettings_State,savesettings_State_Handler,{_keypress_Event}},
	{read_flash_state,read_flash_state_Handler,{}},
	{write_flash_state,write_flash_state_Handler,{}},
    {last_State,0,{}}
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
void clear_buffer(void)
{
	int i;
	for( i = 0;i<sizeof(keyboard_entry);i++)

	 {
		keyboard_entry[i] = 0;
		keypad_pw_xter[i] = 0;
	 }
	keypad_pw_xter[i] = 0;
	keypad_pw_xter[i++] = 0;
	keypad_pw_xter[i++] = 0;
	keypad_pw_xter[i++] = 0;
	index_ = 0;
	_index = 0;
}

/*
 *
 */
eSystemState operator_Handler(void)
{

	return operator_State;
}

/////////////////////////////////////////////////////////////////
uint8_t long_press_key()
{
	static int pressed_ = 0;
		//static int pressed_old = 0;
		int ky;

//		if(pump_type == bluesky)
		if( (pump_type == DN_BLSKY18K) || (pump_type == DN_BLSKY22) )
		{
		   ky = 19;  //F4 key
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
uint8_t long_press_log()
{
	static int pressed_ = 0;
	//static int pressed_old = 0;
	int ky = 0;
ky = readkey19_state();
//	if (readkey19_state() != 1)
if (ky != 1)
	{
        pressed_ = 0;
        log_buttonpress_tmr = 0;   //clr timer.
	}
	else if (ky == 1)
		ky = 2;

	 if ( (log_buttonpress_tmr >= 3)&&(pressed_ == 0 ) )
		 {
		   log_buttonpress_tmr = 3;
		    pressed_ = 1;
		    return 1;
		 }
	 return 0;
}
/////////////////////////////////////////////////////////////////
uint8_t long_press_tot()
{
	static int pressed_ = 0;
	//static int pressed_old = 0;
	int ky;

//	if(pump_type == lafeng) ky = 11;  //keypad type mapping...
	if(pump_type == DN_LAFNG17K) ky = 11;  //keypad type mapping...
	else
	  ky = 21;                      //mapped to print key...


	if(keypress_ != ky)
	{
        pressed_ = 0;
        tot_buttonpress_tmr = 0;  //clr timer.
	}
	 if((tot_buttonpress_tmr >= 3)&&(pressed_ == 0) )
		 {
		   tot_buttonpress_tmr = 3;
		    pressed_ = 1;
		    return 1;
		 }
	 return 0;
}
/////////////////////////////////////////////////////////////////
eSystemState operator_State_Handler(void)
{
	    static int fxn = 0;
        static int indx1 = 0,
        		   indxx1 = 0;
	    static int pump_indx = 1;
	    static int8_t pump_indxx = 0;

	    extern  log_max;
	    static int8_t key19_sto_ = 0, keyy19_sto_ = 0;

	    int pkey = 0;
	    int8_t keyy19 = 0;
	    extern int  printer_status ;

    //===========================================================
  	    int rd19 = readkey19_state();

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
	  if ( (operatorfxn == totaliser_view) )
		{
		  // level 0. totaliser
		   if (indx1 == 0 ) indx1 = 1;
		   if (indxx1 == 0 ) indxx1 = 1;
		  // -------------- test keys....----------------
		  	     pkey = read_keypad();

		  	         if( (pkey > 0) )  //key detected
		  	         {
		  	        	 if (pkey == 'C')  // up key  --> KeyF2
		  					{
		  	        		  //indx1++;
		  					    if (indxx1 == 1)
		  					         	indxx1 = 2;
		  					    else
		  					    		indxx1 = 1;
		  					}

		  	        	 if (pkey == 'B')  // down key
		  					{
		  	        		   //indx1--;
		  						if (indxx1 == 1)
		  							indxx1 = 2;
		  						else
		  							indxx1 = 1;
		  					}

		  	        	 if (pkey == 'A')  // back key
							{
		  	        		   send_line1(upper);
		  	        		   send_line2(middle);

							   char str__[10]= {0};
						  	   snprintf(str__, sizeof(str_), "%.2f", litre_price);
							   send_line3(str__);
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
								   clear_screen();
								   printDisp_c("l",1,0,8,LT,CLEAR);
								   printDisp_c(line1, 1, (8-jj), 5, LT, NOCLEAR);  //display price
//								   printDisp_c(line1, 1, 0,5,RT,CLEAR );
								   send_line2(line2);
								 }
								 else
								 {
									 send_line1("l        ");
									 send_line2("        ");
									 printDisp_f(totaliser_vol1c, 2, 0,5,RT,CLEAR ); //send_line2(scc);
								 }
							  send_line3("tot a");
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
								   clear_screen();
								   printDisp_c("p",1,0,8,LT,CLEAR);
								   printDisp_c(line1,1,(8-jj),5,LT,NOCLEAR);
//								   printDisp_c(line1, 1, 0,5,RT,CLEAR );
								   send_line2(line2);
								 }
								 else
								 {
									 send_line1("p        ");
									 send_line2("        ");
									 printDisp_f(totaliser_amt1c, 2, 0,5,RT,CLEAR ); //send_line2(scc);
								 }
							  send_line3("tot a");
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
							   clear_screen();
							   printDisp_c("l",1,0,8,LT,CLEAR);
							   printDisp_c(line1,1,(8-jj),5,LT,NOCLEAR);
//							   printDisp_c(line1, 1, 0,5,RT,CLEAR );
							   send_line2(line2);
							 }
							 else
							 {
								 send_line1("l        ");
								 send_line2("        ");
								 printDisp_f(totaliser_vol2c, 2, 0,5,RT,CLEAR ); //send_line2(scc);
							 }
						  send_line3("tot b");
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
							   clear_screen();
							   printDisp_c("p",1,0,8,LT,CLEAR);
							   printDisp_c(line1,1,(8-jj),5,LT,NOCLEAR);
//							   printDisp_c(line1, 1, 0,5,RT,CLEAR );
							   send_line2(line2);
							 }
							 else
							 {
								 send_line1("p        ");
								 send_line2("        ");
								 printDisp_f(totaliser_amt2c, 2, 0,5,RT,CLEAR ); //send_line2(scc);
							 }
						  send_line3("tot b");
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

	            	float pr_  =  price_real;
					float vl_  =  amt_real;
					char *tm_  =  "---------";

	            	if(loop_ == 0)    //do this only the first time
						{
	            	    	// data_size = sizeof(log_a_new);
						  if (operating_side == side_a)
						  {
								 flash_loc = flash_infoA.current_loc;
								 log_no = flash_infoA.number_logs;
								 if(log_no == 0)
									 {
										  pr_  =  0.00; //price_real;
										  vl_  =  0.00; //amt_real;
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
								    	   if (flash_loc == 0 )  // flash_beginA) //at begining of storage
											{
								    			// this shouldnt occur ,  just in case !
								    			flash_loc = flash_endA - data_size; //wrap
											}
								    	}
								    }
								   flash_locc = flash_loc;
						  	}
						  if (operating_side == side_b)
							   {
								 flash_loc = flash_infoB.current_loc;
								 log_no = flash_infoB.number_logs;
								 if(log_no == 0)
									 {
										  pr_  =  0.00; //price_real;
										  vl_  =  0.00; //amt_real;
										  tm_  =  "--------";
									 }
								    else
									{
								    	//calculate the previous save id from the current
										if (flash_loc <= flash_beginB) //at begining of storage
										{
											flash_loc = flash_endB - data_size; //wrap
										}
										else
										{
											flash_loc = flash_loc - data_size;
											if (flash_loc < flash_beginB) //at begining of storage
											{
												// this shouldnt occur ,  just in case !
												flash_loc = flash_endB - data_size; //wrap
											}
										}
										    flash_locc = flash_loc;
									}
							   }
	            		  loop_ = 1;   //disable first time assignment ^^^^
	            		  log_indx_indx = log_no;
	            		  pump_indx  = 1;  // initialise the side...

	            		  flash_read_idA = flash_loc;

	            		  return read_flash_state;  //goto read the flash location...
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
	          	  	        	   if (pkey == 'C')  // up key
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
	          	  					    	}
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
	          	  					    return read_flash_state;
	          	  					}

	          	  	        	 if (pkey == 'B')  // down key
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
	          	  	              	    return read_flash_state;
	          	  					}

	    		  	        	 if (pkey == 'A')  // back key
	    							{
	    		  	        		  loop_ = 0;  // enable the first time assignment
	    		  	        		   send_line1(upper);
	    		  	        		   send_line2(middle);

	    							   char str__[8]= {0};
	    						  	   snprintf(str__, sizeof(str__), "%.2f", litre_price);
	    							   send_line3(str__);
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
	          	  	    	  if (pump_indxx == 1)
	          	  	    	   {
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
									  snprintf(st__, sizeof(st__), "  %d-%02d",log_a_new.time._hh,log_a_new.time._mn);
									 printDisp_c(st__,2,0,8,LT,CLEAR);
	                              }
	                              	  snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx);
	                                  send_line3("      ");
	                              	  send_line3(line3);
	          	  	    	   }  //  if (indx1 == 1)

	          	  	    	 //-----------------------------------------------------

	          				   if (pump_indxx == 2)
	          				      {
									  pr_  =  log_b_new.pr__;   //price
									  vl_  =  log_b_new.vol__;  //volume
									  //  tm_  =  log_b_new.tm_;   //time


									  //dd-hh:mm,    P  , L  ,    T
									 printDisp_c("p",1,0,8,LT,CLEAR); printDisp_f(pr_,1,0,8,LT,NOCLEAR);  //display price

									 if (scroll_ == 1 )
									 {
										 printDisp_c("l",2,0,8,LT,CLEAR); printDisp_f(vl_,2,0,8,LT,NOCLEAR);  //display litre
									 }
									 else if (scroll_ == 2)
									 {
										  char st__[10]= {0};
										  snprintf(st__, sizeof(st__), "%d-%02d-%d",log_b_new.date._dd,log_b_new.date._mm,log_b_new.date._yy);
										  printDisp_c(st__,2,0,8,LT,CLEAR);
									 }
									 else if (scroll_ == 3)
									  {
										  char st__[10]= {0};
										  snprintf(st__, sizeof(st__), "  %d-%02d",log_b_new.time._hh,log_b_new.time._mn);
										  printDisp_c(st__,2,0,8,LT,CLEAR);
									  }

	          					    snprintf(line3, sizeof(line3), "bn1.%d",log_indx_indx );
	          					    send_line3("      ");
	          					    send_line3(line3);
	          				      }  //  if (indx1 == 2)
	           	             }
							 else
							 {
							   send_line1(" no log ");
							   send_line2("        ");
							   send_line3("      ");
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
			send_line1(upper);
			send_line2(middle);

			char str__[8]= {0};
			snprintf(str__, sizeof(str_), "%.2f", litre_price);
			send_line3(str__);
			error_clr_flag = 0;  //clear flag..
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

        		  keypad_pw_xter[temp++] = '-';
        		  keypad_pw_xter[temp  ] = 0;
        	  }
                return 0;
          }

          if (kkey == 'F' )  //if clear key,
			 {
               _index = 0; temp = 0;
               for(int i = 0;i<10;i++)
                {
                	keypad_pw_xter[i] = 0;
                	keypad_buf[i] = 0;
                }
               //send_line2("        ");  //clear second line.
               return 'F';
			 }

          if (kkey == 'D' )  //if ENTER key.
			 {
        	    return 'D';
			 }

          if (kkey == 'B' )  //if UP key.
			 {
				return 'B';
			 }

          if (kkey == 'C' )  //if DOWN key.
			 {
				return 'C';
			 }

          if (kkey == 'F' )  //if clear key.
			 {
				return 'F';
			 }

          if (kkey == 'A' )  //if back key.
			 {
				return 'A';
			 }

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
      send_line1("        ");
      send_line2("        ");
      send_line3("        ");
}
//=============================================================================
eSystemState progstate_Handler(void)
{
	static int indx = 0;
	static int indx_m = 0;
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

   if(prog_entry == 0 )   // first entry...
   {
	   //clear the state variables.
	    indx = 0;
	   	indx_m = 0;
	    indx2 = 0;
	    indx3 = 0;
	    attempts = 0;
	    fxn = 0;
	    pump_indx = 1;
	      //progg = 1;
	     temp = 0;
	     pkey = 0;
	     access = non;

	     prog_entry = 1;  //To prevent clearing variables on re-entry.
   }

   if (auth == not_auth)
    {
      //clear keypad....
	  //acquire the password goto level0 access level if non supplied.

	   pkey = read_keypad();

	  if(pkey > 0)  //key detected
	   {
    	   if (pkey == 'D') // enter key...
    	   {
              //password provided...  settings[0].passwd1
			   long int pass_ = atol(keypad_buf);

				   if(pass_ == password_level1)  //level 2 access ?
					 {
						auth = authed;
						access = level1;
						fxn = nothing;
						clear_buffer();
						//keypad_buf[0] = 0;
						//keyboard_entry[0] = 0;
						return prog_State;
					 }

				   if(pass_ == password_level1 )  //level 3 access ?
					   {
						  auth = authed;
						  access = level2;
						  fxn = nothing;
						  clear_buffer();
						 // keypad_buf[0] = 0;
						 // keyboard_entry[0] = 0;
						  return prog_State;
					   }
				   if(access == non)             //no match...
					   {
						  send_line2("---no---");
						  HAL_Delay(1500);
						  send_line2("        ");
						  clear_buffer();   	//clear the buffer
						  send_line2(keypad_pw_xter);
						  send_line3("        ");
					   }
    	  }// ================ if ENTER KEY PRESSED =================
    	   if (pkey == 'F') // cancel key...
    	       	   {
    		          clear_buffer();
    		          /* keypad_buf[0] = 0;
    		           keyboard_entry[0] = 0;
    		           keypad_pw_xter[0] = 0;
    		           _index = 0; */
    	       	   }
       }  //if(pkey > 0)  //key detected

	    if (t >= 500)  //time to display?
		 {
	    	send_line1("  pass  ");
			send_line2(keypad_pw_xter);
			send_line3("        ");
			t = 0;
		 }
	    return prog_State;  //exit...
   }   //if (not_auth)

        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
          //====================================================================
          //             show the  MENU and scroll thru with UP and DOWN.
          //====================================================================
             if ( (auth == authed) && (access == level1)&&(fxn == nothing)  )
               {
				   if (t >= 500)
					 {
						if(indx_m == 15) indx_m = 14;
						send_line1(menu2[indx_m]);   //show menu item.
						send_line2("        ");
						t = 0;
					 }
                 // -------------- test keys....----------------
          	     pkey = read_keypad();
          	       if(pump_indx == 0) pump_indx = 1;

          	         if( (pkey > 0) )  //key detected
          	         {
          	        	 if (pkey == 'C')  // up key
          					{
//          					 if (indx_m < 13)  indx_m++;
          					 if (indx_m <= 13)  indx_m++;
          					 else if(indx_m == 14) indx_m = 0;
          					}

          	        	 if (pkey == 'B')  // down key
          					{
          						if (indx_m > 0) indx_m--;
          						else if(indx_m == 0) indx_m = 14;
          					}

          	        	 if (pkey == 'A')  // BACK key...
							   {
								   auth = authed;
								   access = level1;
								   fxn = nothing;
								   clear_buffer();
								  // keypad_buf[0] = 0;
							   }

          	        	 if (pkey == 'D')  // enter key
          					{

          	        		 if(indx_m == 14) indx_m += 1;

                                   switch(indx_m)
          							{
                                          case setmode:
												fxn = setmode;
												pump_indx = 1;
												return prog_State;
												clear_buffer();
												clr_screen();
												break;

                                          case setnozzle:
                                        	    pump_indx = 1;
												fxn = setnozzle;
												clear_buffer();
												clr_screen();
												break;

                                          case default_sell_type:
                                        	   pump_indx = 1;
                                          	   fxn = default_sell_type;
                                          	   clear_buffer();
                                          	   clr_screen();
          									   break;

                                          case setprice:
												fxn = setprice;
												pump_indx = 1;
												//clear the keyboard buffer
												clear_buffer();
												clr_screen();
												//index_ = 0;
												//keyboard_entry[0] = 0;
												//keyboard_entry[1] = 0;
												//copy[0].price_ = litre_price1;
												//copy[1].price_ = litre_price2;
												break;

										  case setaddress:
											    pump_indx = 1;
												fxn = setaddress;
												//clear the keyboard buffer
												//index_ = 0;
												clear_buffer();
												clr_screen();
											  break;

										  case set_no_flow_time:
												fxn = set_no_flow_time;
												clear_buffer();
												clr_screen();
												break;

                                          case setmax_amt:
                                          	   fxn = setmax_amt;
                                          	   clear_buffer();
                                          	   clr_screen();
          									   break;

										  case set_password:
											    attempts = 0;
												fxn = set_password;
												clear_buffer();
												clr_screen();
												break;

										  case send_config:
												   fxn = send_config;
												   clear_buffer();
												   clr_screen();
												   break;
                                          case get_config:
												fxn = get_config;
												clear_buffer();
												clr_screen();
												break;

										  case clr_log_:
											   fxn = clr_log_;
											   //clr_screen();
											   clear_buffer();
											   break;

										  case clr_tot_:
											   fxn = clr_tot_;
											   //clr_screen();
											   clear_buffer();
											   break;

										  case calibrate_:
											   fxn = calibrate_;
											   clr_screen();
											   clear_buffer();
											   break;
										  case display_:
											   fxn = display_;
											   clear_buffer();
											   clr_screen();
											   dp_ = 1;
											   ln_ = 1;
											   break;

                                          case setclock:
												fxn = setclock;
												clear_buffer();
												clr_screen();
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
  			send_line1(menu1[index2]);
  			t = 0;
  		 }                                     */
			  //=======================================================
			  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
			  //=======================================================
			  //                set mode   ONLINE / OFFLINE.
			  //=======================================================
             if ( (auth == authed) && (access == level1)&&(fxn == setmode)  )
  	               {
  					   if (t >= 300)
  						 {
  							//send_line1();
                            if(copy[pump_indx-1].mode == online)
								{
									send_line2(" online ");
								}
                               else
								{
									send_line2("offline ");
								}

  							 if (pump_indx == 1)
								 {
									//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
									send_line3("  a  ");
								 }
  							 if (pump_indx == 2)
								 {
									//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
									send_line3("  b  ");
								 }
  							t = 0;
  						 }
  	           // -------------- test keys....----------------
  	    		int rd19 = readkey19_state();

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
  	          	        	 if (pkey == 'C')  // up key
  	          					{
  	          	        	      if (copy[pump_indx-1].mode == online) //;
  	          	        	     	copy[pump_indx-1].mode = offline;
  	          	        	      else
  	          	        	    	copy[pump_indx-1].mode = online;
  	          					}

  	          	        	 if (pkey == 'B')  // down key
  	          					{
									if (copy[pump_indx-1].mode == online) //;
										copy[pump_indx-1].mode = offline;
									  else
										copy[pump_indx-1].mode = online;
  	          					}

  	          	             if (pkey == 'F')  //change pump index.
								{
								  // pump_indx++;
									//if (pump_indx > 2) pump_indx = 1; //wrap around
								}

  	          	        	 if (pkey == 'A')  // back key
  	          					{
  	          	        		 	fxn = nothing;
  	          	        		 	send_line3("      ");
  	          					}
  	          	         }
  	          	     return prog_State;
  	               }
      //========================= MODE ==========================
      //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	  //==========================================================================
	  //                set mode   OVERIDE.
	  //==========================================================================
		 if ( (auth == authed) && (access == level1)&&(fxn == setnozzle)  )
		   {
			   if (t >= 300)
				 {
					//send_line1();
				  if(copy[pump_indx-1].noz == overide)
				  {
					send_line2("active");
				  }
				  else
				  {
					send_line2("inactive");
				  }

				 if (pump_indx == 1)
				 {
					//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
					send_line3("  a  ");
				 }
				 if (pump_indx == 2)
				 {
					//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
					send_line3("  b  ");
				 }
				t = 0;
		 }
			 // -------------- test keys....----------------
				int rd19 = readkey19_state();

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
					 if (pkey == 'C')  // up key
						{
						 if (copy[pump_indx-1].noz == overide)
							 copy[pump_indx-1].noz = nooveride;
						 else
							 copy[pump_indx-1].noz = overide;
						}

					 if (pkey == 'B')  // down key
						{
						 if (copy[pump_indx-1].noz == overide)
							 copy[pump_indx-1].noz = nooveride;
						 else
							 copy[pump_indx-1].noz = overide;
						}

					 if (pkey == 'F')  //change pump index.
					{
					  // pump_indx++;
					  // if (pump_indx > 2) pump_indx = 1; //wrap around
					}

					 if (pkey == 'A')  // back key
						{
							 fxn = nothing;
							 clr_screen();
						}
				 }
			 return prog_State;
		   }
	      //========================= MODE =======================================
		  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
		  //======================================================================
		  //                    default sell/prog type .
		  //======================================================================
			 if ( (auth == authed) && (access == level1)&&(fxn == default_sell_type)  )
				   {
					   if (t >= 300)
						 {
							//send_line1();
							  if(copy[pump_indx-1].def_t ==  P)
							  {
								send_line2("price   ");
							  }
							  else
							  {
								send_line2("litre   ");
							  }

							 if (pump_indx == 1)
							 {
								//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
								send_line3("  a  ");
							 }
							 if (pump_indx == 2)
							 {
								//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
								send_line3("  b  ");
							 }
							t = 0;
			        	 }
					 // -------------- test keys....----------------
						int rd19 = readkey19_state();

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
							 if (pkey == 'C')  // up key
								{
								 if(copy[pump_indx-1].def_t == P)
								   copy[pump_indx-1].def_t = L;
								 else
								   copy[pump_indx-1].def_t = P;
								}

							 if (pkey == 'B')  // down key
								{
								 if(copy[pump_indx-1].def_t == P)
								   copy[pump_indx-1].def_t = L;
								 else
								   copy[pump_indx-1].def_t = P;
								}

							 if (pkey == 'F')  //change pump index.
								{
								  // pump_indx++;
								  //if (pump_indx > 2) pump_indx = 1; //wrap around
								}

							 if (pkey == 'A')  // back key
								{
									 fxn = nothing;
									 clr_screen();
								}
						 }
					 return prog_State;
				   }

		  //=========================== MODE ====================================
		  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
		  //=====================================================================
		  //               set the  litre price
		  //=====================================================================
			 if ( (auth == authed) && (access == level1)&&(fxn == setprice) )
			   {
				 if (t >= 300)
				  {
					 //copy[pump_indx-1].price_ =  atoi(keyboard_entry);
					 printDisp_f(copy[pump_indx-1].price_, 1, 0, 6, RT, CLEAR);
                     float pr = atoff(keyboard_entry);
					 printDisp_f(pr,2,0,7,RT,CLEAR);

					 if (pump_indx == 1)
					 {
					   send_line3("  a  ");
					 }

					 if (pump_indx == 2)
					 {
						 send_line3("  b  ");
					 }
					t = 0;
			 }
				 // -------------- test keys....----------------
				 int rd19 = readkey19_state();

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
						 if (pkey == 'C')  // up key
							{

							}

						 if (pkey == 'B')  // down key
							{

							}

						 if (pkey == 'D')  //store entered value.
							{
							 if (pump_indx == 1)   // if side A
								{
								   copy[0].price_ =  atof(keyboard_entry);
								}
							 if (pump_indx == 2)   // if side B
								{
								   copy[1].price_ =  atof(keyboard_entry);
								}
							}

						 if (pkey == 'A')  // back key
							{
								 fxn = nothing;
								 clr_screen();
							}
					 }
				 return prog_State;
			   }
		         //=========================== MODE =====================================
			     //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
				  //=====================================================================
				  //                          ID SETTINGS.
				  //=====================================================================
					 if ( (auth == authed) && (access == level1)&&(fxn == setaddress)  )
					   {
						   if (t >= 500)
							 {
								printDisp_i(copy[pump_indx-1].id_, 1, 0, 4, RT, CLEAR);
								send_line2(keyboard_entry);

								if (pump_indx == 1)
								 {
									send_line3("  a  ");
								 }

								if (pump_indx == 2)
								 {
									send_line3("  b  ");
								 }
								t = 0;
					        }
						 // -------------- test keys....----------------
							 int rd19 = readkey19_state();

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
								 if (pkey == 'C')  // up key
									{

									}

								 if (pkey == 'B')  // down key
									{

									}

								 if (pkey == 'D')  //enter key
									{
										 if (pump_indx == 1)   // if side A
											{
											   copy[0].id_ =  atoi(keyboard_entry);
											}
										 if (pump_indx == 2)   // if side b
											{
											   copy[1].id_ =  atoi(keyboard_entry);
											}
									}

								 if (pkey == 'A')  // back key
									{
										 fxn = nothing;
										 clr_screen();
									}
							 }
						 return prog_State;
					   }
	  //=============================== MODE =====================================
	  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	  //==========================================================================
	  //                         SET NO FLOW TIME.
	  //==========================================================================
		 if ( (auth == authed) && (access == level1)&&(fxn == set_no_flow_time)  )
		   {
			   if (t >= 500)
				 {
				   printDisp_i(copy[pump_indx-1].noflow_, 1, 0, 4, RT, CLEAR);
				   send_line2(keyboard_entry);

				 if (pump_indx == 1)
				 {
					//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
					send_line3("  a  ");
				 }

				 if (pump_indx == 2)
				 {
					//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
					send_line3("  b  ");
				 }
				t = 0;
		 }
			 // -------------- test keys....----------------
				 int rd19 = readkey19_state();

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
					 if (pkey == 'C')  // up key
						{

						}

					 if (pkey == 'B')  // down key
						{

						}

					 if (pkey == 'D')  //enter key
						{
							 if (pump_indx == 1)   // if side A
								{
								   copy[0].noflow_ =  atoi(keyboard_entry);
								}
							 if (pump_indx == 2)   // if side b
								{
								   copy[1].noflow_ =  atoi(keyboard_entry);
								}
						}

					 if (pkey == 'A')  // back key
						{
							 fxn = nothing;
							 clr_screen();
						}
				 }
			 return prog_State;
		   }
  //============================ MODE ===================================
  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  //=====================================================================
  //                         SET MAX AMOUNT.
  //=====================================================================
	 if ( (auth == authed) && (access == level1)&&(fxn == setmax_amt)  )
	   {
		   if (t >= 500)
			 {
			   printDisp_i(copy[pump_indx-1].max_amt_, 1, 0, 4, RT, CLEAR);
			   send_line2(keyboard_entry);

			 if (pump_indx == 1)
			 {
				send_line3("  a  ");
			 }

			 if (pump_indx == 2)
			 {
				send_line3("  b  ");
			 }
			t = 0;
	 }
		 // -------------- test keys....----------------
			 int rd19 = readkey19_state();

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
				 if (pkey == 'C')  // up key
					{

					}

				 if (pkey == 'B')  // down key
					{

					}

				 if (pkey == 'D')  //enter key
					{
						 if (pump_indx == 1)   // if side A
							{
							   copy[0].max_amt_ =  atoi(keyboard_entry);
							}
						 if (pump_indx == 2)   // if side b
							{
							   copy[1].max_amt_ =  atoi(keyboard_entry);
							}
					}

				 if (pkey == 'A')  // back key
					{
						 fxn = nothing;
						 clr_screen();
					}
			 }
		 return prog_State;
	   }

//============================== MODE ==================================
 //=====================================================================
 //                         CLEAR LOG.
 //=====================================================================
 if ( (auth == authed) && (access == level1)&&(fxn == clr_log_)  )
   {
	   if (t >= 500)
		 {
		   //printDisp_i(copy[pump_indx-1].max_amt_, 1, 0, 4, RT, CLEAR);
		   send_line2(keyboard_entry);

		 if (pump_indx == 1)
		 {
			send_line3("  a  ");
		 }

		 if (pump_indx == 2)
		 {
			send_line3("  b  ");
		 }
		t = 0;
 }
	 // -------------- test keys....----------------
		 int rd19 = readkey19_state();

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
			 if (pkey == 'C')  // up key
				{

				}

			 if (pkey == 'B')  // down key
				{

				}

			 if (pkey == 'D')  //enter key
				{
					 if (pump_indx == 1)   // if side A
						{
						  //clear side A log
						  clear_logA();
						  send_line1("        ");
						  send_line2("cleared ");
						  HAL_Delay(2000);
						  clr_screen(); //send_line2("        ");
						  fxn = nothing;
						}

					 if (pump_indx == 2)   // if side b
						{
						 //clear side B log
						 clear_logB();
						 send_line1("        ");
						 send_line2("cleared ");
						 HAL_Delay(2000);
						 clr_screen(); //send_line2("        ");
						 fxn = nothing;
						}
				}

			 if (pkey == 'A')  // back key
				{
					 fxn = nothing;
					 clr_screen();
				}
		 }
	 return prog_State;
   }

 //=====================================================================
  //                         CLEAR totaliser.
  //=====================================================================
  if ( (auth == authed) && (access == level1)&&(fxn == clr_tot_)  )
    {
 	   if (t >= 500)
 		 {
 		   //printDisp_i(copy[pump_indx-1].max_amt_, 1, 0, 4, RT, CLEAR);
 		   send_line2(keyboard_entry);

 		 if (pump_indx == 1)
 		 {
 			send_line3("  a  ");
 		 }

 		 if (pump_indx == 2)
 		 {
 			send_line3("  b  ");
 		 }
 		t = 0;
  }
 	 // -------------- test keys....----------------
 		 int rd19 = readkey19_state();

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
 			 if (pkey == 'C')  // up key
 				{

 				}

 			 if (pkey == 'B')  // down key
 				{

 				}

 			 if (pkey == 'D')  //enter key
 				{
 					 if (pump_indx == 1)   // if side A
 						{
 						  //clear side A log
 						  clear_totA();
 						  send_line1("        ");
 						  send_line2("cleared ");
 						 HAL_Delay(2000);
 						  fxn = nothing;
 						 clr_screen();
 						}

 					 if (pump_indx == 2)   // if side b
 						{
 						 //clear side B log
 						 clear_totB();
 						 send_line1("        ");
 						 send_line2("cleared ");
 						 HAL_Delay(2000);
 						 fxn = nothing;
 						 clr_screen();
 						}
 				}

 			 if (pkey == 'A')  // back key
 				{
 					 fxn = nothing;
 					 clr_screen();
 				}
 		 }
 	 return prog_State;
    }
 //============================ MODE ===================================
 //=====================================================================
 //                         SEND CONFIG.
 //=====================================================================
 if ( (auth == authed) && (access == level1)&&(fxn == send_config)  )
   {
	   if (t >= 500)
		 {
		   //printDisp_i(copy[pump_indx-1].max_amt_, 1, 0, 4, RT, CLEAR);
		  // send_line2(keyboard_entry);
/*
		 if (pump_indx == 1)
		 {
			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
			send_line3("  a  ");
		 }

		 if (pump_indx == 2)
		 {
			//snprintf(line3, sizeof(line3), "an1.%d",log_indx_indx + 1);
			send_line3("  b  ");
		 }  */

		t = 0;
 }
	 // -------------- test keys....----------------
		 int rd19 = readkey19_state();

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
			 if (pkey == 'C')  // up key
				{

				}

			 if (pkey == 'B')  // down key
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

					  send_line2("sent    ");
					  fxn = nothing;
				}

			 if (pkey == 'A')  // back key
				{
					 fxn = nothing;
					 clr_screen();
				}
		 }
	 return prog_State;
   }


  //============================ MODE ===================================
  //=====================================================================
  //                         get config
  //=====================================================================
  if ( (auth == authed) && (access == level1)&&(fxn == get_config)  )
    {
 	   if (t >= 3000)
 		 {

 		  send_line3("      ");
 		  send_line2(" done ");
 		  fxn = nothing;
 		t = 0;
    }

  //-------------------------------------------------
  // Receive the bytes sent from the external system
  //       and store in the settings array.





  //-------------------------------------------------
  // -------------- test keys....--------------------
 		 int rd19 = readkey19_state();

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
							 clr_screen();
						}
				 }
 	 return prog_State;
    }
  //============================ MODE ==================================
  //====================================================================
  //                         SET PASSWORD
  //====================================================================
  if ( (auth == authed) && (access == level1)&&(fxn == set_password)  )
    {
	   static int chg_pw = 0;
	   static int pw1 = 0;
	   static int pw2 = 0;

 	   if (t >= 500)
 		 {
 		    send_line1(keypad_pw_xter); //keyboard_entry);
 		   if (pump_indx == 1)
 		  		 {
 		  	//		send_line3("  a  ");
 		  		 }

 		  		 if (pump_indx == 2)
 		  		 {
 		  	//		send_line3("  b  ");
 		  		 }
 	    	t = 0;
         }
 	 // -------------- test keys....----------------
 		 int rd19 = readkey19_state();

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
 			 if (pkey == 'C')  // up key
 				{

 				}

 			 if (pkey == 'B')  // down key
 				{

 				}

 			 if (pkey == 'D')  //enter key
 				{
 				 if (chg_pw == 0)
					  {
					     pw1 = atoi(keyboard_entry);
						 chg_pw = 1;
						 send_line2("re-enter");
						 HAL_Delay(2500);
						 send_line1("        ");
						 clear_buffer();
						 //index_ = 0;
						 //keyboard_entry[0] = 0;
						 //keyboard_entry[1] = 0;
						 //keypad_pw_xter[0] = 0;
						 //_index = 0;
						 return prog_State;
					  }
 				 if (chg_pw == 1)
					  {
						 if (pw1 == atoi(keyboard_entry))
							 {
								 send_line1("correct ");
								 send_line2("        ");
								 HAL_Delay(2500);
								 send_line1("        ");

                                 copy[0].passwd1 = pw1;
                                 copy[1].passwd2 = pw1;

                                 HAL_Delay(2500);
                                 send_line1("pass    ");
                                 send_line2("changed ");
                                 HAL_Delay(2500);
                                 fxn = nothing;
								 chg_pw = 1;
								 clear_buffer();
								// index_ = 0;
								//keyboard_entry[0] = 0;
								//keypad_pw_xter[0] = 0;
								//_index = 0;
							    return prog_State;

							 }
						 else
						 {

							    send_line2("---no---");
							    HAL_Delay(1500);
							    send_line2("        ");
							    attempts++;
							    if (attempts >= 3)
							    {
							    	send_line2("        ");
							    	send_line2(" denied ");
							    	HAL_Delay(1500);
							    	send_line2("        ");
							    	 fxn = nothing;
							    	 chg_pw = 1;
									clear_buffer();
							    	// index_ = 0;
									//keyboard_entry[0] = 0;
									//keypad_pw_xter[0] = 0;
									//_index = 0;
							    	 return prog_State;
							    }
							    else
							    {
							    	send_line2("        ");
									send_line2(" retry  ");
									HAL_Delay(1500);
									send_line2("        ");
									clear_buffer();
									// index_ = 0;
									//keyboard_entry[0] = 0;
									//keypad_pw_xter[0] = 0;
									//_index = 0;
									return prog_State;
							    }
						 }
						 chg_pw = 1;
					  }
 				}

 			 if (pkey == 'A')  // back key
 				{
 					 fxn = nothing;
 				   	clr_screen();
 				}
 		 }
 	 return prog_State;
    }


    //============================ MODE ==================================
    //====================================================================
    //                        CALIBRATE..
    //====================================================================
    if ( (auth == authed) && (access == level1)&&(fxn == calibrate_)  )
      {
  	   static float calib_price = 0.0;
  	   static float inv_price   =  0.0;
  	   static int vol_sold = 1;

	   if (volume_flag == 0)
	    {
		   //select sales volume first...

			   if (t >= 500)
				 {
					if (vol_sold == l1)
					{
						send_line1("10 litre");
					}
					else
						send_line1("20 litre");

				   if (pump_indx == 1)
						 {
							send_line3("  a  ");
						 }

				   if (pump_indx == 2)
						 {
							send_line3("  b  ");
						 }
					   t = 0;
				   }
   	 // -------------- test keys....----------------
   		 int rd19 = readkey19_state();

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
   			 if (pkey == 'C')  // up key
   				{
                   if (vol_sold == l1)
                   {
                	   vol_sold = l2;
                   }
                   else
                	   vol_sold = l1;
   				}

   			 if (pkey == 'B')  // down key
   				{
   				 if (vol_sold == l1)
					   {
						   vol_sold = l2;
					   }
					   else
						   vol_sold = l1;
   				}

   			 if (pkey == 'D')  //enter key
   				{
   				 	 //enter pressed user has made a choice.
   				     // then dispense the selcted volume
					volume_flag  = 1;   //set flag and goto dispense
					calib_pulser = 0;  //clear pulser
		           __HAL_TIM_SET_COUNTER(&htim5,0);   //clear harware pulser here...
					return prog_State;
   				}

   			 if (pkey == 'A')  // back key
   				{
   					 fxn = nothing;
   				     calibr = 0;
   				     clr_screen();
   				}
   		 }
      	 return prog_State;
	   }// if selected volume = 0.....
	//========= volume has been selected =============
    // dispense
      if(volume_flag == 1)
      {
         send_line1("dispense");
         HAL_Delay(2000);
         volume_flag = 2;
         calibr = 1;        /// notify the interrupt routine to on the pump
         return prog_State;
      }
    //=============================
	  if(volume_flag == 2)  //dispense.
	   {
		 if(t > 60)
		  {

			#if _USE_SOFT_PULSER == 0
			  calib_pulser = __HAL_TIM_GET_COUNTER(&htim5);  //use hardware counter
			#else
			  calib_pulser++; 							// use software counter.
			#endif

			  send_line1(" pulser ");
			  printDisp_i(calib_pulser, 2, 0, 7, RT, CLEAR);
			  t = 0;
		  }

			  pkey = read_keypad();
			  if (pkey == 'C')  //off key to stop fueling...   KeyF2
				{
                    //capture the pulser.
				  calibr = 0;  /// notify the interrupt routine to on the pump
				  calib_pulser = __HAL_TIM_GET_COUNTER(&htim5);  //use hardware counter
				  send_line1("set your");
				  send_line2("  annt  ");
				  HAL_Delay(2000);
				  volume_flag = 3;   //enter the price.
				  return prog_State;
				}

			  if (pkey == 'A')  // back key
				{
                 	volume_flag = 0;
                    calibr = 0;
                   // clr_screen();
				}

		     return prog_State;
	   }
  //==========================================================

  //==========================================================
    if(volume_flag == 3)  //Enter the price.
	   {
			  if(t > 400)
				  {
					send_line1("  annt  ");
					printDisp_f(atoff(keyboard_entry), 2, 0, 7, RT, CLEAR);
					t = 0;
				  }

			  pkey = read_keypad();
			  if (pkey == 'D')  //ENTER key to adjusted volume.
				{
				  float cal_vol = atoff(keyboard_entry);
				  send_line1("        ");   //clear screen.
				  send_line2("        ");
				  clear_buffer();
					//index_ = 0;
					//keyboard_entry[0] = 0;  //clear buffer...
					//keypad_pw_xter[0] = 0;
					//_index = 0;
					//==================================
					// calculate the price of the pulses
					// float pop = calib_pulser  / cal_vol;
					int sold_v = 0;
					float pi_c;
					float pi;

					if (vol_sold == l1)
					{
						sold_v = 10;
						pi_c = calib_pulser / cal_vol; //calibrated pi.
						pi   = calib_pulser / sold_v;
					}
					else
					{
						sold_v = 20;
						pi_c = calib_pulser / cal_vol; //calibrated pi.
						pi   = calib_pulser / sold_v;
					}

                    HAL_Delay(1700);

                    send_line1("  done  ");
                    printDisp_f(pi_c, 2,0, 8, RT, CLEAR);
                    HAL_Delay(2500);
                    //store in the settings structure.
                    if(pump_indx == 1)
                    {
                    	// data for side a.
                    	copy[0].pi_c = pi_c;
                    	copy[0].pi_  = pi;
                    }

                    if(pump_indx == 2)
					{
						// data for side b.
						copy[1].pi_c = pi_c;
						copy[1].pi_	 = pi;
					}
					 fxn = nothing;
					 calibr = 0;
					 clr_screen();
				}

			  if (pkey == 'A')  // back key
				{
					volume_flag = 0;
					clr_screen();
				}
			 return prog_State;
	   }
    return prog_State;
     }

 //====================================================================

//============================ MODE ===================================
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//=====================================================================
//                      display precision
//=====================================================================
 if ( (auth == authed) && (access == level1)&&(fxn == display_)  )
   {
	 if (ln_ == 0) ln_ = 1;
	 if (dp_ == 0) dp_ = 1;

	   if (t >= 500)
		 {
		    if (ln_ == 1)
			 {
				send_line1("   p    ");
			 }

			if (ln_ == 2)
			 {
				send_line1("   l    ");
			 }

			if (ln_ == 3)
			 {
				send_line1(" unit p ");
			 }
       //---------------------------------
		    if (dp_ == 1)
			 {
				send_line2(" 999999.9");
			 }

		    if (dp_ == 2)
			 {
				send_line2(" 99999.99");
			 }

		    if (dp_ == 3)
			 {
		    	send_line2(" 9999.999");
			 }
		    if (dp_ == 4)
			 {
				send_line2(" 999.9999");
			 }
        //--------------------------------
			 if (pump_indx == 1)
			 {
				send_line3("  a  ");
			 }

			 if (pump_indx == 2)
			 {
				send_line3("  b  ");
			 }
			t = 0;
        }
	 // -------------- test keys....----------------
		 int rd19 = readkey19_state();

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
			 if (pkey == 'C')  // up key
				{
				 	 dp_++;
				 	 if(dp_ > max_dp) dp_ = 1;
				}

			 if (pkey == 'B')  // down key
				{
				 	 ln_++;
				 	 if(ln_ > 3)ln_ = 1;
				}

			 if (pkey == 'D')  //enter key
				{
					 if (pump_indx == 1)   // if side A
						{
						   if(ln_ == 1) copy[0].dp_price  = dp_;
						   if(ln_ == 2) copy[0].dp_amount = dp_;
						   if(ln_ == 3) copy[0].dp_unitprice = dp_;
						}
					 if (pump_indx == 2)   // if side b
						{
						   if(ln_ == 1) copy[1].dp_price  = dp_;
						   if(ln_ == 2) copy[1].dp_amount = dp_;
						   if(ln_ == 3) copy[1].dp_unitprice = dp_;
						}
					 return prog_State;
				}

			 if (pkey == 'A')  // back key
				{
					 fxn = nothing;
					 clr_screen();
					 return prog_State;
				}
		 }
	 return prog_State;
   }


 //=========================== MODE ====================================
 //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 //=====================================================================
 //               set the  CLOCK
 //=====================================================================
	 if ( (auth == authed) && (access == level1) && (fxn == setclock) )
	 {
		    static int8_t respons_dir = 0,
				   entry_clear = 1,
				   montth = 0,
				   failFlag_tm = 0,
				   failFlag_dt = 0;

		    int8_t respons = 0,
		    	   tm = 0,
		    	   dte = 0;

		    static bool hh_success = false,
		    			yy_success = false;

		    char st__[10] = {0};

		  //====================================================
		  if( (t>200) && (respons_dir == 0) )
		  {
			  send_line1("1- tinne");
			  send_line2("2-  date");
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
				  clear_buffer();
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
				  printDisp_c("nnI",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
			  }

			  if( (atoi(keyboard_entry)) <= 59 )
				  tm = atoi(keyboard_entry);
			  else clear_buffer();

			  printDisp_i(tm,2,4,8,RT,CLEAR);

			  //=======================================
			  if (clock_save)
			  {
				clock_save = false;

				DS1307_SetHour(ttime[0]);
				DS1307_SetMinute(ttime[1]);
				DS1307_SetSecond(00);
				HAL_Delay(15);

				clr_screen();
				send_line2("  saved");
				HAL_Delay(1500);

				hour = DS1307_GetHour();
				minute = DS1307_GetMinute();
				second = DS1307_GetSecond();

 				clear_buffer();
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
				  clear_buffer();
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
				  printDisp_c("nnO",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
			  }
			  else if(mth_success)
			  {
				  snprintf(st__, sizeof(st__), "  %02d", day);
				  printDisp_c("dd",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
			  }

			  if( (atoi(keyboard_entry)) <= 99 )
				  dte = atoi(keyboard_entry);
			  else clear_buffer();

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

				clr_screen();
				send_line2("  saved");
				HAL_Delay(500);

				day = DS1307_GetDate();
				month = DS1307_GetMonth();
				year = DS1307_GetYear();
				dayofweek = DS1307_GetDayOfWeek();

 				clear_buffer();
 				entry_clear = 1;
 				respons_dir = 0;
			  }
   		 }

		 pkey = read_keypad();

		 if( (pkey > 0) || (failFlag_tm) || (failFlag_dt) )         //key detected
		 {
			if (pkey == 'C')  // up key
			{

			}

			if (pkey == 'B')  // down key
			{

			}

			if ( ((pkey == 'D') && ((respons == 1) || (respons_dir == 1))) || (failFlag_tm) )  //store entered value.
	        {
				 if( ((tm <= 23) && (!hh_success)) && (!failFlag_tm) )
				 {
					 ttime[0] = tm;
					 clear_buffer();
					 hh_success = true;
				 }
				 else if ( ((tm <=59) && (hh_success)) && (!failFlag_tm) )
				 {
					 ttime[1] = tm;
					 clear_buffer();
					 hh_success = false;
					 clock_save = true;
				 }
				 else
				 {
					 send_line2("        ");
					 if(!hh_success)
					 {
						 snprintf(st__, sizeof(st__), "  %02d", hour); //,log_a_new.time._mn);
						 printDisp_c("hr",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
					 }
					 else if(hh_success)
					 {
						 snprintf(st__, sizeof(st__), "  %02d", minute);
						 printDisp_c("nnI",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
					 }
					 send_line2(" failed");
					 clear_buffer();

					 if(!failFlag_tm) _tt = 0;

					 if( (failFlag_tm) && (_tt > 600) ) failFlag_tm = 0;
					 else failFlag_tm = 1;
				 }
			}

			else if( ((pkey == 'D') && ( (respons == 2) || (respons_dir == 2) )) || (failFlag_dt) )  //store entered value.
			{
				 if( (dte <= 99) && (!yy_success) && (!mth_success) && (!failFlag_dt) )
				 {
					 ddate[2] = dte;
					 clear_buffer();
					 yy_success = true;
				 }
				 else if ( (dte != 0) && (dte <= 12) && (yy_success) && (!mth_success) && (!failFlag_dt) )
				 {
					 ddate[1] = dte;
					 clear_buffer();
					 yy_success = false;
					 mth_success = true;
					 montth = dte;
				 }
				else if( (mth_success) && ((montth == January) || (montth == March) || (montth == May) || (montth == July) || (montth == August) || (montth == October) || (month == December)) && (!failFlag_dt) )
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
					__fail:   send_line2("        ");
							  if( (!yy_success) && (!mth_success) )
							  {
								  snprintf(st__, sizeof(st__), "  %02d", year);
								  printDisp_c("Yr",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
							  }
							  else if( (yy_success) && (!mth_success) )
							  {
								  snprintf(st__, sizeof(st__), "  %02d", month);
								  printDisp_c("nnO",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
							  }
							  else if(mth_success)
							  {
								  snprintf(st__, sizeof(st__), "  %02d", day);
								  printDisp_c("dd",1,0,8,LT,CLEAR); printDisp_c(st__,1,4,8,RT,NOCLEAR);
							  }

							 send_line2(" failed");
							 clear_buffer();

							 if(!failFlag_dt) _tt = 0;

							 if( (failFlag_dt) && (_tt > 600) ) failFlag_dt = 0;
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
 			 clr_screen();
 			 clear_buffer();
		}
	}
	//=======================================
	return prog_State;
  }



 //============================== MODE ==================================
  return prog_State;
} //end of prog_state.

//----------------------------------------
eSystemState idlestate_Handler(void)
{
	static int  printer_status;

	static int8_t idleState_flag = 1;

	static int32_t current_pulser_ = 0,
				   old_pulser_ = 0;

	int pulser_diff = 0;

	pump1_status_4G = STATUS_IDLE;

	stop_fueling_bit = 1;

	lock_clr = 0;

	progg = 0;

//	current_pulser_ = __HAL_TIM_GET_COUNTER(&htim5);

//	current_pulser_++;
//	current_pulser_--;

	if( (idleState_flag == 1) && (eLastState != filling_State) )
	{
		idleState_flag = 2;
		_tt = 0;
	}
	else if( (_tt > 60000) && (idleState_flag == 2) && (eLastState != filling_State) )
	{
		current_pulser_ = __HAL_TIM_GET_COUNTER(&htim5);
		old_pulser_ = current_pulser_ ;
		idleState_flag = 3;
	}
	else if( (_tt > 65000) && (idleState_flag == 3) && (eLastState != filling_State) )
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

	if ( (t > 500) && (nozzleup_awaitingauth_state_not_timedOut == 0) && (pump_LitreOverflow == 0) && (_litre_price == 0)
			&& (_auth_p == 0) && (_auth_v == 0) && (idle_backwardPulse == 0) && (idle_forwardPulse == 0)
			&& (flow_loss == 0) && (display_overflow == 0) )
	{
		 send_line1(upper);
		 send_line2(middle);
		 char str__[8]= {0};
		 snprintf(str__, sizeof(str__), "%.2f", litre_price); send_line3(str__);
		 t = 0;
	 }

	 if( (keypress_ == 21) && (nozzleup_awaitingauth_state_not_timedOut == 0) )
	 {
		   compose_printer();
		   HAL_Delay(1000);
		   printer_status = 1;
	 }

     print__1();   //print the transaction.

#if sense_power == 1
	  	  if(readpwr() == 0)
			  {
	  		  	   //count time elapsed
	  		       if (shutdown_timer > 120)
	  		    	   drive_power(DEACTIVATE);   //shutdown... after  2 minutes
			  }
	  	  else
	  	  {
	  		  //clear the timer//
	  		  shutdown_timer = 0;
	  	  }
#endif


//	  	if ( ((t > 1000) && (t <= 1000)) && (nozzleup_awaitingauth_state_not_timedOut == 1) )
//		 {
//			 // send_line1("n up u a");
//			 clr_screen();
//			 send_line1("call.  ");
//			 //t = 0;
//		 }
//		 else if ( ((t > 2000) && (t <= 3000)) && (nozzleup_awaitingauth_state_not_timedOut == 1) )
//		 {
//			send_line1("call . ");
//			//t = 0;
//		 }
//		 else if ( ((t > 3000) && (t <= 4000)) && (nozzleup_awaitingauth_state_not_timedOut == 1) )
//		 {
//			send_line1("call  .");
//			if(t > 4000) t = 0;
//		 }

	  	if ( (t > 2000) && (pump_LitreOverflow == 1) )
		{
			 // send_line1("n up u a");
//			 clr_screen();
			 send_line1("  punnp   ");
			 send_line2(" linnit  ");
			 send_line3("err7   ");
			 if(t > 6000)
			 {
				 t = 0;
				 pump_LitreOverflow = 0;
			 }
		 }
	  	else if ( (t > 2000) && (display_overflow == 1) )
		{
			 send_line1("display   ");
			 send_line2(" linnit  ");
			 send_line3("err18   ");
			 if(t > 6000)
			 {
				 t = 0;
				 display_overflow = 0;
			 }
		}
	  	else if( (t > 2000) && (_litre_price == 1) )
	  	{
	  		 send_line1("no unit   ");
			 send_line2("price  ");
			 send_line3("err8   ");
			 if(t > 6000)
			 {
				 t = 0;
				 _litre_price = 0;
			 }
	  	}
	  	else if( (t > 2000) && (_auth_p == 1) )
		{
			 send_line1("    no   ");
			 send_line2("  price  ");
			 send_line3("err9   ");
			 if(t > 6000)
			 {
				 t = 0;
				 _auth_p = 0;
			 }
		}
		else if( (t > 2000) && (_auth_v == 1) )
		{
			 send_line1("  no   ");
			 send_line2("annount  ");
			 send_line3("err10   ");
			 if(t > 6000)
			 {
				 t = 0;
				 _auth_v = 0;
			 }
		}
		else if( (t > 2000) && (idle_backwardPulse == 1) )
		{
			 send_line1("   bac   ");
			 send_line2("  flouu  ");
			 send_line3("err13   ");
			 if(t > 6000)
			 {
				 t = 0;
				 idle_backwardPulse = 0;
			 }
		}
		else if( (t > 2000) && (idle_forwardPulse == 1) )
		{
			 send_line1("   idle   ");
			 send_line2("  flouu  ");
			 send_line3("err14  ");
			 if(t > 6000)
			 {
				 t = 0;
				 idle_forwardPulse = 0;
			 }
		}
		else if( (t > 2000) && (flow_loss == 1) )
		{
			 send_line1("   flow   ");
			 send_line2("  loss  ");
			 send_line3("err16  ");
			 if(t > 6000)
			 {
				 t = 0;
				 flow_loss = 0;
			 }
		}
//		else if( (t > 2000) && (_auth_v == 1) )
//				{
//					 send_line1("  no   ");
//					 send_line2("annount  ");
//					 send_line3("err17   ");
//					 if(t > 6000)
//					 {
//						 t = 0;
//						 _auth_v = 0;
//					 }
//				}

//		 else if ( ((t > 2000) && (t <= 3000)) && (pump_LitreOverflow == 1) )
//		 {
//			send_line1(" limit ");
//			//t = 0;
//		 }
//		 else if ( ((t > 3000) && (t <= 4000)) && (pump_LitreOverflow == 1) )
//		 {
//			send_line1("");
//			if(t > 4000) t = 0;
//		 }

	  	return idle_State;
}

//-------------------------------------
eSystemState keyup_Handler(void)
{
	progg = 1;
	copy_settings(move_to_copy);	//make a copy of the settings								//to be used in the prog.
	clear_buffer();
	return prog_State;
}
//-------------------------------------
eSystemState inactivestate_Handler(void)
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
			  send_line1("1-  saue");
			  send_line2("2-delete");
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
		send_line1(" sauing ");
		send_line2("settings");

		copy_settings(move_to_settings); // copy the structure.
		save_settings();   //save to eeprom
		load_settings(side_a); //load the settings into the
		load_settings(side_b); // internal variables
		HAL_Delay(1500);

		send_line1("  done  ");
		send_line1("        ");
		sav = 0;
		progg = 0;
		auth = not_auth;
		clear_buffer();
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
		 clear_buffer();

		 send_line1(upper);
		 send_line2(middle);
		 char str__[8]= {0};
		 snprintf(str__, sizeof(str__), "%.2f", litre_price);
		 send_line3(str__);
		 return idle_State;
	}
	//========================================
	return savesettings_State;    //idle_State;  //return to idle mode
}
//--------------------------------------
eSystemState keydown_Handler(void)
{
	clear_buffer();
	prog_entry = 0;
	return savesettings_State;   //
}

//-----------------------------------
eSystemState nozzleup_Handler(void)
{
	pump1_status_4G = STATUS_NOZZLE_UP;

	reset_timer(timeout_picknozzle);
	start_timer(timeout_picknozzle);

	send_line1("88888888");
	send_line2("88888888");
	send_line3("88888");

	//set the motor on and solenoid off.
	get_auth();

//	send_solenoid(0);   //11
//	send_pump(1);      //turn on pump.

	if (eLastState == authorised_nozzledown_State)
		{
		     current_pulser = 0;
			 clr_pulser();    //clear hardware pulser
		    return authorised_nozzleup_State;
		}

	return nozzleup_waitingforauth_State;
}

//-------------
eSystemState auth_command_Handler(void)
{
	  reset_timer( timeout_picknozzle);
	  start_timer( timeout_picknozzle);
    //-----------------------------------
	  if(opmode == offline)
	  {
		  //check if any keypad entry
		 if(index_ >= 1)
		  {
              key_value = atof(keyboard_entry);
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

		  // online Mode
		  //authorise price...
		  if (change_p == 1)
			  sellmode = P;      //set sell type to price
		 //-----------------------------------------
		  //authorise volume...
		  else if (change_v == 1)
			  sellmode = L;      //set sell type to litre
		 //-----------------------------------------
	  }
    // GO can also send this command.
  return authorised_nozzledown_State;
}

//---------------
eSystemState  nozzleup_waitingforauthState_Handler(void)
{
   	if (settings[operating_side - 1].mode == offline)
		{
			//send nozzleup command only in offline mode
			return authorised_nozzleup_State;     //idle_State;
		}
   	else
   	{
   		// online mode...
   		if (controller_authorise())    // authed by controller...
   		{
   			return  authorised_nozzleup_State;
   		}
   		else
   		{
   			nozzleup_awaitingauth_state_not_timedOut = 1;
   			if(t > 2400) t = 0;

   			 if ( ((t > 400) && (t <= 900)) && (nozzleup_awaitingauth_state_not_timedOut == 1) )
			 {
				 clr_screen();
				 send_line1("auth    ");
			 }
			 else if ( ((t > 900) && (t <= 1400)) && (nozzleup_awaitingauth_state_not_timedOut == 1) )
			 {
				send_line1("auth_ ");
			 }
			 else if ( ((t > 1400) && (t <= 1900)) && (nozzleup_awaitingauth_state_not_timedOut == 1) )
			 {
				send_line1("auth__ ");
			 }
			 else if ( ((t > 1900) && (t <= 2400)) && (nozzleup_awaitingauth_state_not_timedOut == 1) )
			 {
				send_line1("auth___ ");
			 }
   		}
   	}
	return nozzleup_waitingforauth_State;
}

//----------------
eSystemState authorise_Handler(void)
{
	reset_timer(timeout_picknozzle);
	start_timer(timeout_picknozzle);

//initialise the fuel and price variables
 price  = 0.0;
 amt = 0.0;


 target_pulser = 0;  //state is coming from nozzleup ,no price/amt set

 clr_pulser();    //clear hardware pulser
 current_pulser = 0;

// int cnv = 0;
// snprintf(str_, sizeof(str_), "%.2f", price); send_line1(str_);
// snprintf(str_, sizeof(str_), "%.2f", amt);   send_line2(str_);

 send_line1("--auth--");
 send_line2("        ");
 char str__[8]= {0};
 snprintf(str__, sizeof(str__), "%.2f", litre_price); send_line3(str__);

 return authorised_nozzleup_State;
}

//---------------
eSystemState authorised_nozzledown_State_Handler(void)
{
	nozzle_bit = 0;
	pump1_status_4G = STATUS_AUTHORIZED_NOZZLE_DOWN;


	/* if (t > 1000)
		  {
				send_line1("a n dn");
			  t = 0;
		  }*/
	/* int keyboard_val = 2000;   //dummy value...

	    reset_timer(timeout_picknozzle);
    	start_timer(timeout_picknozzle); */

	 //initialise the fuel and price variables

	    if(index_ >= 1)
		  {
			  key_value = atof(keyboard_entry);
		  }
		 else
		 {
			  key_value = 0.0;
		 }

	   // key_value = 5000.000;
	    //initialise the fuel and price variables
	      if (sellmode == P)
	    	{
			  target_pulser = price2pulser(key_value);  //calculate pulse frm price.
	    	}
	      else  //amt was selected.
	      {
	    	  target_pulser = amt2pulser(key_value);   //calculate pulse frm amt.
	      }

	/*
    	if (sellmode == P)  //price was selected
    	{
    	   target_pulser = price2pulser(keyboard_val); //calculate pulse frm price.
    	}
    	  else      //amt was selected.
    	{
    	   target_pulser = amt2pulser(keyboard_val); //calculate pulse frm amt.
    	}
    	filling = 1;
   */

	    //  target_pulser = price2pulser(5000);

	  price  = 0.0;
	  amt = 0.0;

	  current_pulser = 0;

	 // int cnv = 0;
	 // snprintf(str_, sizeof(str_), "%.2f", price); send_line1(str_);
	 // snprintf(str_, sizeof(str_), "%.2f", amt);   send_line2(str_);

   if(t > 300)
   {
	  send_line1(" pic    ");
	  send_line2("no22le  ");
	  char str__[8]= {0};
	  snprintf(str__, sizeof(str__), "%.2f", litre_price); send_line3(str__);
      t = 0;
   }
	return authorised_nozzledown_State;
}

//------------------
uint32_t price2pulser(float price)
{
  float temp = (price / litre_price) *  pulser_index_c;
  original_pulse = temp;
  //temp = floor(temp);
  return  floor(temp );
}

uint32_t amt2pulser(float amt)
{
	 float temp = amt * pulser_index_c;
	 original_pulse = temp;
	    return  floor(temp );
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
void make_string(sellmode_ sll,float pr)
{
    int8_t tmp = 1,
    	   ind = 0;

	char temp[10] = {0};
	char ttmp[10] = {0};

	 if (sll == L)
	 {
		 if (dp_amount1 == 1)
		     snprintf(temp , sizeof(temp), "%.1f", pr);
		 else if (dp_amount1 == 2)
			 snprintf(temp , sizeof(temp), "%.2f", pr);
		 else if (dp_amount1 == 3)
		 	 snprintf(temp , sizeof(temp), "%.3f", pr);
	 }

	 if (sll == P)
		 {
			 if (dp_price1 == 1)
			     snprintf(temp , sizeof(temp), "%.1f", pr);
			 else if (dp_price1 == 2)
				 snprintf(temp , sizeof(temp), "%.2f", pr);
			 else if (dp_price1 == 3)
			 	 snprintf(temp , sizeof(temp), "%.3f", pr);
		 }

	int index__ = 0;
	while (tmp != 0)
	{
		tmp = temp[index__++];
	}

	int space = 9 - index__;

	if(eNextState == filling_State)
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
					upper[i] = ttmp[i];
				else
					middle[i] = ttmp[i];
		   }
     int t = 0;
}

//---------------------------------------------------
eSystemState authorised_nozzleup_State_Handler(void)
{
	float sellPrice_max_pump,
		  sellPrice_max_dpp;

	static int8_t firstTime = 1;

	nozzle_bit = 1;  stop_fueling_bit = 0;

    pump_status_ = STATUS_AUTH;
    pump1_status_4G = STATUS_AUTHORIZED_NOZZLE_UP;

 	//initialise the fuel and price variables
	//int keyboard_val = 2000;   //dummy value...
	//clear pulsercount in the prev. state.....

	 sellPrice_max_dpp = sellPrice_max_dp(dp_price1);

	 if(litre_price1 == 0)
	 {
		_litre_price = 1;
		return idle_State;
	 }
	 else if ( (change_p == 1) && (auth_p == 0) )
	 {
		_auth_p = 1;
		return idle_State;
	 }
	 else if ( (change_v == 1) && (auth_v == 0) )
	 {
		_auth_v = 1;
		return idle_State;
	 }
	 else if(opmode == offline)
	 {
		 if(index_ >= 1)
		 {
			  key_value = atof(keyboard_entry);
			  //initialise the fuel and price variables
				if (sellmode == P)
				{
					  sellPrice_max_pump = (litre_price1 * pump_max_litres);

					  if( (key_value > sellPrice_max_pump) || (key_value > sellPrice_max_dpp) )
					  {
						  if(sellPrice_max_pump < sellPrice_max_dpp)
						  {
							  key_value = sellPrice_max_pump;
							  pump_LitreOverflow = 1;
						  }
						  else if(sellPrice_max_pump > sellPrice_max_dpp)
						  {
							  key_value = sellPrice_max_dpp;
						      display_overflow = 1;
						  }
					  }

					  target_pulser = price2pulser(key_value);  //calculate pulse frm price.

				}
				else  //amt was selected.
				{
					if( (key_value > pump_max_litres) || (key_value > sellPrice_max_dpp) )
					{

						 if(pump_max_litres < sellPrice_max_dpp)
						 {
							  key_value = pump_max_litres;
							  pump_LitreOverflow = 1;
						 }
						 else if(pump_max_litres > sellPrice_max_dpp)
						 {
							  key_value = sellPrice_max_dpp;
							  display_overflow = 1;
						 }
	}

					target_pulser = amt2pulser(key_value);   //calculate pulse frm amt.
				}
		  }
		  else
		  {
//			  key_value = (litre_price1 * pump_max_litres);
//			  pump_LitreOverflow = 1;

			  key_value = sellPrice_max_dpp;
		      display_overflow = 1;

		      target_pulser = price2pulser(key_value);  //calculate pulse frm price.
//			  key_value = 0;
		  }

	 }
	 else if(opmode == online)
	 {
		  if (change_p == 1)
		  {
			  sellPrice_max_pump = (litre_price1 * pump_max_litres);

			  if( (auth_p > sellPrice_max_pump) || (auth_p > sellPrice_max_dpp) )
			  {
				  if(sellPrice_max_pump < sellPrice_max_dpp)
				  {
					  auth_p = sellPrice_max_pump;
					  pump_LitreOverflow = 1;
				  }

				  else if(sellPrice_max_pump > sellPrice_max_dpp)
				  {
					  auth_p = sellPrice_max_dpp;
				      display_overflow = 1;
				  }
			  }

			  sprintf(keyboard_entry,"%2f",auth_p);
			  key_value = atof(keyboard_entry);
			  change_p = 0;      //reset tbe flag.
			  index_ = strlen(keyboard_entry);

			  target_pulser = price2pulser(key_value);  //calculate pulse frm price.
		  }
		 //-----------------------------------------
		  //authorise volume...
		  else if (change_v == 1)
		  {
			  if( (auth_v > pump_max_litres) || (auth_v > sellPrice_max_dpp) )
			  {
				  if(pump_max_litres < sellPrice_max_dpp)
				  {
					  auth_v = pump_max_litres;
					  pump_LitreOverflow = 1;
				  }

				  else if (pump_max_litres > sellPrice_max_dpp)
				  {
					  auth_v = sellPrice_max_dpp;
				      display_overflow = 1;
				  }
			  }

			  sprintf(keyboard_entry,"%2f",auth_v);
			  key_value = atof(keyboard_entry);
			  change_v = 0;      //reset tbe flag.
			  index_ = strlen(keyboard_entry);

			  target_pulser = amt2pulser(key_value);   //calculate pulse frm amt.
		  }

		  else if (index_ >= 1)
		  {
			  	key_value = atof(keyboard_entry);

			  //initialise the fuel and price variables
				if (sellmode == P)
				{
					  sellPrice_max_pump = (litre_price1 * pump_max_litres);
					  if( (key_value > sellPrice_max_pump) || (key_value > sellPrice_max_dpp) )
					  {
						  if (sellPrice_max_pump < sellPrice_max_dpp)
						  {
							  key_value = sellPrice_max_pump;
							  pump_LitreOverflow = 1;
						  }

						  else if (sellPrice_max_pump > sellPrice_max_dpp)
						  {
							  key_value = sellPrice_max_dpp;
						      display_overflow = 1;
						  }
					  }

					  target_pulser = price2pulser(key_value);  //calculate pulse frm price.

				}
				else  //amt was selected.
				{
					if( (key_value > pump_max_litres) || (key_value > sellPrice_max_dpp) )
					{
						if(pump_max_litres < sellPrice_max_dpp)
						{
							key_value = pump_max_litres;
							pump_LitreOverflow = 1;
						}
						else if (pump_max_litres > sellPrice_max_dpp)
						{
							 key_value = sellPrice_max_dpp;
							 display_overflow = 1;
						}
					}

					target_pulser = amt2pulser(key_value);   //calculate pulse frm amt.
				}
		 }
		 else
		 {
//			  key_value = (litre_price1 * pump_max_litres);
//			  pump_LitreOverflow = 1;

			  key_value = sellPrice_max_dpp;
		      display_overflow = 1;
			  target_pulser = price2pulser(key_value);  //calculate pulse frm price.
		 }
	   }
//============================================================================
//============================================================================
//	  if (authorise_flag == 1)
//	   {
//		  target_pulser = price2pulser(5000);
//		  key_value  = 5000;
//		  authorise_flag == 0;
//	   }
//============================================================================
//============================================================================

	  price  = 0.0;
		  amt  = 0.0;

		  if (key_value == 0)
		  {
			   target_pulser = 0;
		  }
		  else
			  pulser_rem = target_pulser - current_pulser;

		  if(lock_clr == 0)  ///   to activate this section once.
		  {
				 current_pulser = 0;
				 clr_pulser();    //clear hardware pulser

				 //current_pulser = __HAL_TIM_GET_COUNTER(&htim5);
				 lock_clr = 1;
				 filling = 1;

				 firstTime = 1;
				 motor_tmr = 0;
		  }

		 // int cnv = 0;
	     //char temp[10] = {0};
		 //snprintf(temp , sizeof(temp), "%.2f", price);  //send_line1(upper);
         //snprintf(middle, sizeof(middle), "%.2f", amt);   //send_line2(lower);

	  	  make_string(P,price);
		  make_string(L,amt);

//		 if (t > 300)
		 if (t > 100)
		 {
				send_line1(upper);
				send_line2(middle);

				char str__[8]= {0};
				snprintf(str__, sizeof(str_), "%.2f", litre_price);
				send_line3(str__);
				t = 0;

							//trigger first pulser...
				if(firstTime == 1)
				{
					firstTime = 2;
					#if (_USE_SOFT_PULSER == 1)
						 //  pulser_new += 1;
						   current_pulser++;
					#endif

				}
				else if (firstTime == 2)
				{
					firstTime = 3;

					#if (_USE_SOFT_PULSER == 1)
							 //  pulser_new += 1;
							current_pulser++;
					#else
							current_pulser = 0;
							clr_pulser();    //clear hardware pulser
					#endif
				}
				else if (firstTime == 3)
				{
					#if (_USE_SOFT_PULSER == 1)
						 //  pulser_new += 1;
						   current_pulser++;
					#else
						 // pulser_new = __HAL_TIM_GET_COUNTER(&htim5);
						   current_pulser = __HAL_TIM_GET_COUNTER(&htim5);
					#endif
				}


//			#if (_USE_SOFT_PULSER == 1)
//					 //  pulser_new += 1;
//					   current_pulser++;
//			#else
//					 // pulser_new = __HAL_TIM_GET_COUNTER(&htim5);
//					   current_pulser = __HAL_TIM_GET_COUNTER(&htim5);
//			#endif

	 	   }

		   #if (_USE_SOFT_PULSER == 1)
		 	 pulser_new = current_pulser;
		   #endif

		   if (firstTime == 3)
		       pulser_new = current_pulser;
//=====================================================================
 		// filling = 1;
		// send_pump(ACTIVATE);  //pump on...

//  if(operating_side == side_a)
//	{
		working_volTotaliser = totaliser_vol1;
		working_volTotaliserc = totaliser_vol1c;
		working_amtTotaliser = totaliser_amt1;
		working_amtTotaliserc = totaliser_amt1c;
//	}
//  else
//   {
//  		working_volTotaliser = totaliser_vol2;
//  		working_volTotaliserc = totaliser_vol2c;
//  		working_amtTotaliser = totaliser_amt2;
//  		working_amtTotaliserc = totaliser_amt2c;
//  	}

  r_volTotaliser = floor( working_volTotaliserc );
  old_r_volTotaliser = r_volTotaliser;

  r_amtTotaliser = floor(working_amtTotaliserc);
  old_r_amtTotaliser = r_amtTotaliser;

 // slow_flow();

	return authorised_nozzleup_State;
}


//---------------
eSystemState pause_Handler(void)
{
   return authorisation_paused_State;
}

//---------------
eSystemState authorisation_paused_State_Handler(void)
{
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

	pump_status_ = STATUS_FILLING;

	send_line1(upper);
	send_line2(middle);
	char str__[10]= {0};
		snprintf(str__, sizeof(str__), "%.2f", litre_price);
		send_line3(str__);
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
       return filling_paused_State;
}

//---------------
eSystemState filling_resumed_Handler(void)
{

	return filling_State;
}

//----------------------------------------
eSystemState nozzledown_Handler(void)
{

	 filling = 0;
	 nozzle_bit = 0;

	 pump1_status_4G = STATUS_NOZZLE_DOWN;

	 if(nozzleup_awaitingauth_state_not_timedOut == 1)
	 {
		 nozzleup_awaitingauth_state_not_timedOut = 0;
		 clr_screen();
	 }

	 stop_flow();

	//send_line1("nos dn");
	 reset_timer(30);
	 stop_timer();

	  do_calcs();
	  get_time();
	  update_info();
	  save_volumeTotaliser(operating_side);
	  save_amountTotaliser(operating_side);
	  save_lastSale(operating_side);
	  nozzle_bit = 0;
/*
		 char str__[8]= {0};
         snprintf(str__, sizeof(str__), " %.2f", litre_price);
		 send_line3(str__);   */

		 char str__[10]= {0};
		snprintf(str__, sizeof(str_), "%.2f", litre_price);
		send_line3(str__);


	 if( ((pump_LitreOverflow == 1) && (pulser_rem > 0 )) || ((display_overflow == 1) && (pulser_rem > 0 )) )
	 {
		  if(pump_LitreOverflow == 1)
			  pump_LitreOverflow = 0;
		  if(display_overflow == 1)
			  display_overflow = 0;
	 }


  //-------------------------------------------
  //             saving to the log
	 if(eNextState == filling_State)
	 {
		  stop_fueling_bit = 0;
		  return  write_flash_state;
	 }

  //-------------------------------------------
      return idle_State;
}

//----------------------------------------
eSystemState timeout_Handler(void)
{
	 if(eLastState != authorised_nozzleup_State)
	 {
		 send_line1(" t out  ");
		 //send_line2("--------");

		 if( ((pump_LitreOverflow == 1) && (pulser_rem > 0 )) || ((display_overflow == 1) && (pulser_rem > 0 )) )
		 {
			  if(pump_LitreOverflow == 1)
				  pump_LitreOverflow = 0;
			  if(display_overflow == 1)
				  display_overflow = 0;
		 }
	 }

  //write the commodity price...
  // char str__[8]= {0};
  // snprintf(str__, sizeof(str__), " %.2f", litre_price);
  // send_line3(str__);
  // send_line3("-----");

  //compose the kind of timeout error
  if(eLastState == nozzleup_waitingforauth_State)
  {
	 send_line3("err1 ");
	 nozzleup_awaitingauth_state_not_timedOut = 0;
  }

  if(eLastState == authorised_nozzledown_State)
  {
     send_line3("err2 ");
  }

  if(eLastState == authorisation_paused_State)
  {
	 send_line3("err3 ");
  }

 if(eLastState == filling_paused_State)
  {
	 send_line3("err4 ");
  }

 if(eLastState == authorised_nozzleup_State)
 {
	 if( ((pump_LitreOverflow == 1) && (pulser_rem > 0 )) || ((display_overflow == 1) && (pulser_rem > 0 )) )
	 {
		  if(pump_LitreOverflow == 1)
			  pump_LitreOverflow = 0;
		  if(display_overflow == 1)
			  display_overflow = 0;
	 }

	 send_line1("    no ");
	 send_line2("  flouu ");
	 send_line3("err15 ");
 }
// if(eLastState == power_failure)
//  {
//	 send_line3("err5 ");
//  }
// if(eLastState == storage_error)
//  {
//	 send_line3("err6 ");
//	 store fail
//  }
// if(eLastState == pump_maxLitres) //pump_LitreOverflow
//  {
//	 send_line3("err7 ");
//	 pump limit
//  }
// if(eLastState == p/l = 0 @start)
//  {
//	 send_line3("err8 ");
//  }
// if(eLastState == p/l = 0 @filling)
//  {
//	 send_line3("err9 ");
//  }
// if(eLastState == comm error)
//  {
//	 send_line3("err10 ");
//  }
// if(eLastState == data error)
//  {
//	 send_line3("err11 ");
//  }
// if(eLastState == back pulses from idle)
//  {
//	 send_line3("err12 ");
//  }
// if(eLastState == 4ward pulses from idle)
//  {
//	 send_line3("err13 ");
//  }
// if(eLastState == no flow timeout)
//  {
//	 send_line3("err14 ");
//  }
// if(eLastState == flow lost timeout)
//  {
//	 send_line3("err15 ");
//  }
// if(eLastState == unfinished)
//  {
//	 send_line3("err16 ");
//  }


// if(eLastState == currentFlow < prevFlow)
//  {
//	 send_line3("err4 ");
//  }

	 return inactive_State;
   // return idle_State;
}

//----------------------------------------
eSystemState tot_error_Handler(void)
{
   //set irrecoverable error flag.
	irrecov_flag = 1;

	send_line1("  tot   ");
	send_line2("  error ");

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

	extern int error_clr_flag;

	extern int8_t keyEntry_len;

//	 if(pump_type == lafeng)
	 if(pump_type == DN_LAFNG17K)
	 {
	   kkey =  lafeng_keypad[keypress_];
	   allowed_xters = 6;
	 }
//	 else if(pump_type == bluesky)
	 else if( (pump_type == DN_BLSKY18K) || (pump_type == DN_BLSKY22) )
	 {
	    kkey =  bluesky_keypad[keypress_];
	    allowed_xters = 7;
	 }

   ePrevState = eLastState;

 /*  if (progg == 1)
	 {
	    return keypad_entry_State;
	 }  */

	//if (progg == 1) //if in programming mode return.

	//char prc[7] = {'p'};
	//char amt[7] = {'l'};
	//char keyboard[6] = {0};
	//char keyboard_screen[6] = {0};

   int space = allowed_xters - index_; //int space = 6 - index_;
   int ind = 1;

//----------------------------------------------------------
//If only 0 - 9 is pressed.....
if(
	(kkey != 'A')&&(kkey != 'B')&&
	(kkey != 'C')&&(kkey != 'D')&&
	(kkey != 'F')&&(kkey != '-')   //&&(index_ < 6)
  )
    {
	 if(index_ <= allowed_xters)    //only allow 6 xters...
	  {
		keyboard_entry[index_] = kkey;
		keyboard_entry[index_+1] = 0;         //NULL;
		index_++;
	  }
	}
//--------------------------------------------------------
		  if (kkey == 'F')      //'clear' key.
		  	{
			  //ePrevState = eLastState;
			  //set the error clear flag..

			    dpFlag = 0;
			    error_clr_flag = 1;
                 index_ = 0;
                 _index = 0;
                 for(int i = 0;i<9;i++)
					 {
					   keypad_pw_xter[i] = 0;
					   upper[i] = 0;
					   middle[i] = 0;
					 }

                 for(int i = 0; i <= 6; i++)
					 {
						 keyboard_entry[i] = 0;   //clear the buffer
						 keyboard[i] = 0;
					 }

                 upper[0]  = 'p';
                 middle[0] = 'l';
		  	}
//--------------------------------------------------------------------
		  if (progg == 1)
		  	 {
			    send_keypad("       ");
			    if (auth == not_auth)
			    	send_keypad(keypad_pw_xter);
			    else
			       send_keypad(keyboard_entry);
		  	    return keypad_entry_State;
		  	 }


      if ( (kkey == 'D')&&(progg == 0) ) //fueling key.
	 	{
       	   //ePrevState = eLastState;
           auth_cmd_flag = 1;  //activate auth cmd.

           return keypad_entry_State;
	 	}

      if ( (kkey == 'C')&&(progg == 0)&&(eNextState == filling_State) ) //stop sales.
		{
		   stop_flag = 1;  //activate auth cmd.
		}

//====================================================
 if ( (kkey == 'B')&&(progg == 0) )  //if change sales mode
	{
	   if(sellmode == L)
	   {
	       sellmode = P;
	   }
	   else
	   {
		   sellmode = L;
	   }
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
	 space2 = 7 - index_;     //6 - index_;
 }
 if ((sellmode == L)&&(progg == 0) )
 {
	 middle[0] = 'l';     //Append price to display.
	// if((index_-1) >= 6) space = 2;
		while (space2 > 0) //write spaces first..
		{
		   middle[ind++] = ' ';
		   space2--;
		}
	  space2 = index_;     //reload with len of actual number
	   while(space2 > 0)
		 {
			 middle[ind++] = keyboard_entry[index_ - space2];
			 space2--;
		 }
	  //  send_line1("p       ");

	  write_v(1,"p       ");

	  //  send_line2(middle);
   }
//--------------------------------------------------------------------

       if ( (sellmode == P)&&(progg == 0) )
	   {
 		 upper[0] = 'p';       // append price to display
 		// if((index_-1) >= 6) space = 2;
 		   while (space2 > 0)     //write spaces first..
 		   {
 			   upper[ind++] = ' ';
 			   space2--;
 		   }
 		 space2 = index_ ;          //reload with len of actual number

// 		 if(keyboard_entry[index_ - 1] == '.')
// 		 {
// 			ind++;
// 		 }

			while(space2 > 0)
			 {
				 upper[ind++] = keyboard_entry[index_ - space2];
				 space2--;
			 }
			//send_line1(upper);  //clear the price
			//send_line2("l       ");
		   write_v(2,"l       ");
         }
//----------------------------------------------------------------
//                            write to the keypad
	   static int lcd_size = 5;

//	   if(disp_type1 == LAFNG885 )
	   if(disp_type1 == DN_LAFNG17K)
	   {
		    lcd_size = 5; //change this latter to accomodate other lcds.
	   }
//	   else if(disp_type1 == BLSKY886_N)
	   else if( (disp_type1 == DN_BLSKY18K) || (disp_type1 == DN_BLSKY22) )
	   {
		    lcd_size = 6;
	   }
//================================================================
 int size = lcd_size - 1;   // 1 xter to display 'P/L'

 int k_index = 0;
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
		     keyboard[0] = 'p';
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
			    while(space > 0)
				 {
				   keyboard[ind++] = keyboard_entry[index_ - space];
				   space--;
				 }

	 	   }
//----------------------------------------------------------------
	  if (sellmode == L)
	  {

		 keyboard[0] = 'l';
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
			while(space > 0)
			 {
			   keyboard[ind++] = keyboard_entry[index_ - space];
			   space--;
			 }

	   }

	   if (progg == 1)
		 {
			send_keypad("      ");
			if (auth == not_auth)
				send_keypad(keypad_pw_xter);
			else
			   send_keypad(keyboard_entry);
			return keypad_entry_State;
		 }

	             send_keypad(keyboard);
//----------------------------------------------------------------
      //send_keypad(keyboard);

         return keypad_entry_State;
}
//---------------------------------------------------------------------

eSystemState keypad_entry_State_Handler(void)
{

    return  ePrevState; //
	//return keypad_entry_State;
}

//----------------------------------------
eSystemState filling_state_Handler(void)
{
	//extern uint32_t pulser_new;
	float temp = 0.0;

	static int8_t flow_flag = 1;

	static int32_t current_pulser_ = 0,
					   old_pulser_ = 0;

	pump1_status_4G = STATUS_FILLING_;

    filling  = 1;

    lock_clr = 0;

	  if(target_pulser > 0)
	  {
		 //	pulser_rem = target_pulser - current_pulser; //now handled in
														 //interrupt
	  }

//================================================================
//    power outage during filling  end transaction...
		#if sense_power == 1
	  	  if(  (readpwr() == 0)||(read_p_pwr() == 0) )
			  {
	  		      HAL_GPIO_WritePin(buzzer_GPIO_Port, GPIO_PIN_12, GPIO_PIN_SET);
	  			  HAL_Delay(200);
	  			  HAL_GPIO_WritePin(buzzer_GPIO_Port, GPIO_PIN_12, GPIO_PIN_RESET);
				  filling = 0;
				  stop_flow();
				  get_time();
				  do_calcs();
				  update_info();
				  save_volumeTotaliser(operating_side);
				  save_amountTotaliser(operating_side);
				  save_lastSale(operating_side);
				  return write_flash_state;
			  }
		#endif


	if (stop_flag == 1)   //if stop key pressed
	{
		filling = 0,  nozzle_bit = 0;
		stop_flag = 0;
		stop_flow(); //send_solenoid(1);  //stop solenoid.
		get_time();
        do_calcs();
        update_info();
        save_volumeTotaliser(operating_side);
        save_amountTotaliser(operating_side);
        save_lastSale(operating_side);
       return write_flash_state;
	}

	if(litre_price1 == 0)
	{
		stop_flow(); //send_solenoid(1);  //stop solenoid.
		filling = 0;
		do_calcs();
		get_time();
		update_info();
		save_volumeTotaliser(operating_side);
		save_amountTotaliser(operating_side);
		save_lastSale(operating_side);
		_litre_price = 1;
		return write_flash_state;
	}

//======================== @ filling =============================
	  // get_time2();
	   	   	   temp = pulser2amt(current_pulser);
	   amt = dp(temp,dp_amount1);
	   	   	   temp = amt2price(amt);
	   price = dp(temp,dp_price1);

	    make_string(P,dp(price,dp_price1));
	    make_string(L,dp(amt,dp_amount1));
	    //send_line1(upper);
	    //send_line2(middle);

	    running_volTotaliser  = working_volTotaliser  + amt_real;
	   	running_volTotaliserc = working_volTotaliserc + amt;

	    running_amtTotaliser  = working_amtTotaliser  + price_real;
		running_amtTotaliserc = working_amtTotaliserc + price;

//============================================================
//         for totaliser toggle.
	  r_volTotaliser 	  = floor( running_volTotaliserc );
	  r_amtTotaliser 	  = floor(running_amtTotaliserc);

	if(r_volTotaliser != old_r_volTotaliser)
		{
//			then toggle the totaliser harware I/O.
		   drive_totaliser1(ACTIVATE);
		}
	else
	{
		//deactivate totaliser output...
		 drive_totaliser1(DEACTIVATE);

	}
	  old_r_volTotaliser = r_volTotaliser;   //update...
	  old_r_amtTotaliser = r_amtTotaliser;
//============================================================
	  if (t > LCD_UPDATE_RATE)
		  {
			  send_line1(upper);
			  send_line2(middle);
			  char str__[8]= {0};
			  snprintf(str__, sizeof(str_), "%.2f", litre_price);
			  send_line3(str__);
			  t = 0;
		  }
	  reset_timer(timeout_dispense); //don't time out.
//==========================pulser capture ============================
#if (_USE_SOFT_PULSER == 1)
	 // if ( (t > 50)&&(t < 200) )
	 //  			{
	 //  	    	   current_pulser++;   //transfer this to the interrupt routine..
	 //  			}
#else
	       current_pulser = __HAL_TIM_GET_COUNTER(&htim5);
#endif
//======================================================================
	   if(target_pulser > 0)
	   {
			//pulser_rem = target_pulser - current_pulser;
	   }

	 if(target_pulser > 0)
       {
		  if (pulser_rem <= 0 || pulser_complete == 1 )  //sales complete...
		  	 {
				stop_flow(); //send_solenoid(1);  //stop solenoid.
				filling = 0;
		        do_calcs();
		    	get_time();
		        update_info();
		        save_volumeTotaliser(operating_side);
		        save_amountTotaliser(operating_side);
		        save_lastSale(operating_side);
		        return write_flash_state;
		  	 }
		  else
		  {
			//  slow_flow();
//			  if (pulser_rem >= fast_flow_threshold)
//				  {
//				    fast_flow();
//				  }
			  if(pulser_rem >= fast_flow_threshold)
			  {
					if(current_pulser >=  fast_flow_threshold/2 )
					{
						fast_flow();
					}
					else
					{
						slow_flow();
					}
			  }
			  else
			  {
				  slow_flow();
			  }
		  }
       }

	   if(flow_flag == 1)
	   {
	 		_tt = 0;
	   }
	   else if( (_tt > 60000) && (flow_flag == 2) )
	   {
	 		old_pulser_ = current_pulser;
	 		flow_flag = 3;
	   }
	   else if( (_tt > 65000) && (flow_flag == 3) )
	 	{
	 		if(old_pulser_ > current_pulser)
	 		{
	 			current_pulser = old_pulser_ ;
	 			flow_loss = 1;
	 			filling = 0,  nozzle_bit = 0;
				stop_flag = 0;
				stop_flow(); //send_solenoid(1);  //stop solenoid.
				get_time();
				do_calcs();
				update_info();
				save_volumeTotaliser(operating_side);
				save_amountTotaliser(operating_side);
				save_lastSale(operating_side);
			    return write_flash_state;
	 		}
	 		flow_flag = 1;
	 	}

	   return filling_State;
}



//--------------------------------------------------------------
void do_calcs ()
{

float temp;
 if(target_pulser > 0)  // if price or volume is programmed
 {
   if (pulser_rem <= 0 || pulser_complete == 1 )  //sales complete...
	 {
	   //get_time2();

	    if (sellmode == P)
	   	   {

	       //---------------------------------------------------------
	    	//price = dp(key_value,dp_price);   // temp = amt2price(amt);
	    	//amt   = dp( price_/litre_price ,dp_amount);

	    	 temp = pulser2amt(current_pulser);
	    		    	 amt = dp(temp,dp_amount1);

			 temp = amt2price(amt);
						 price = dp(temp,dp_price1);

	   			///show what the user needs...
	   		 price_ = dp(key_value,dp_price1);
	   		 	 temp = price_/litre_price;
	   		 amt_   = dp(temp,dp_amount1); //calculate vol frm price.

	 		//=========================================================
	 		//    Also calculate the values based on the real P.Indx
	 		          temp  = pulser2amt_R(current_pulser);
	 		      amt_real = dp(temp,dp_amount1);
	 		          temp = amt2price(amt_real);
	 		      price_real = dp(temp, dp_price1);
	 		//=========================================================
	   	   }

	   	  if (sellmode == L)
	   	   {
	   		    temp = pulser2amt(target_pulser);
	   		    amt  = dp(temp,dp_amount1);

	   			price  = amt2price(amt);
	   			///show what the user needs to see...
	   			amt_   = key_value;
	   			price_ = amt_ * litre_price; //calculate price from price.amt
		 		//=========================================================
		 		//    Also calculate the values based on the real P.Indx
		 		          temp  = pulser2amt_R(current_pulser);
		 		      amt_real = dp(temp,dp_amount1);
		 		          temp = amt2price(amt_real);
		 		      price_real = dp(temp, dp_price1);
		 		//=========================================================
	   	   }
	   	 //------------------------------------------------------------------
	   	  make_string(P,dp(price_,dp_price1));
	   	  make_string(L,dp(amt_,dp_amount1));

	   	  send_line1(upper);
	   	  send_line2(middle);

	   	  running_volTotaliser = working_volTotaliser + amt_real;
	   	  running_volTotaliserc = working_volTotaliserc + amt;

	   	  running_amtTotaliser = working_amtTotaliser + price_real;
	   	  running_amtTotaliserc = working_amtTotaliserc + price;

//	   	   if(operating_side == side_a)
//	   	   {
	   		   totaliser_vol1 = running_volTotaliser;    // update totaliser
	   		   totaliser_vol1c = running_volTotaliserc;  // update totaliser
	   		   totaliser_amt1 = running_amtTotaliser;    // update totaliser
	   		   totaliser_amt1c = running_amtTotaliserc;  // update totaliser
//	   	   }
//	   	   else
//	   	   {
//	   		   totaliser_vol2 = running_volTotaliser2;   // update totaliser
//	   	       totaliser_vol2c = running_volTotaliser2c; // update totaliser
//	   		   totaliser_amt2 = running_amtTotaliser2;    // update totaliser
//	   		   totaliser_amt2c = running_amtTotaliser2c;  // update totaliser
//	   	   }
	   	 //-------------------------------------------------------------------
	   	   return;
	 }
   else
   {
      //programmed but still dispensing @ stop pt.
		 temp = pulser2amt(current_pulser);  amt   = dp(temp,dp_amount1);
		 temp = amt2price(amt);   			price = dp(temp,dp_price1);

		  make_string(P,dp(price,dp_price1));
		  make_string(L,dp(amt,dp_amount1));

	    //=========================================================
		//    Also calculate the values based on the real P.Indx
			          temp  = pulser2amt_R(current_pulser);
			      amt_real = dp(temp,dp_amount1);
			          temp = amt2price(amt_real);
			      price_real = dp(temp, dp_price1);
		//=========================================================

		  send_line1(upper);
		  send_line2(middle);

		  running_volTotaliser = working_volTotaliser + amt_real;
		  running_volTotaliserc = working_volTotaliserc + amt;


	   	  running_amtTotaliser = working_amtTotaliser + price_real;
	   	  running_amtTotaliserc = working_amtTotaliserc + price;

//		  if(operating_side == side_a)
//		   {
			   totaliser_vol1 = running_volTotaliser;    // update totaliser
			   totaliser_vol1c = running_volTotaliserc;  // update totaliser
	   		   totaliser_amt1 = running_amtTotaliser;    // update totaliser
	   		   totaliser_amt1c = running_amtTotaliserc;  // update totaliser
//		   }
//		   else
//		   {
//			   totaliser_vol2 = running_volTotaliser;   // update totaliser
//			   totaliser_vol2c = running_volTotaliserc; // update totaliser
//	   		   totaliser_amt2 = running_amtTotaliser2;    // update totaliser
//		   	   totaliser_amt2c = running_amtTotaliser2c;  // update totaliser
//		   }
		 //-------------------------------------------------------------------
		   return;
   }
 }
 else   // if target_pulser == 0
 {
	 //if un programmed...
     //still dispensing @ stop pt.

		 temp = pulser2amt(current_pulser);
		      amt = dp(temp,dp_amount1);

		 temp = amt2price(amt);
		 	 price = dp(temp,dp_price1);

		//compare the final and initial prices...
		 	 float ddp = 0;
		 	 if      (dp_price1 == 1) ddp =  0.1;
			 else if (dp_price1 == 2) ddp =  0.01;
			 else if (dp_price1 == 3) ddp =  0.001;

		 	 if( (temp - price) > ( (ddp/10)*2) )  //result was rounded down.
		 	 {

		 		 //need to do a little fixing here...
		 		 price = price + ddp;
		 	 }

		  make_string(P,price); //,dp_price)
		  make_string(L,amt);  //,dp_amount)

		//=========================================================
		//    Also calculate the values based on the real P.Indx
		          temp  = pulser2amt_R(current_pulser);
		      amt_real = dp(temp,dp_amount1);
		          temp = amt2price(amt_real);
		      price_real = dp(temp, dp_price1);
		//=========================================================

		  send_line1(upper);
		  send_line2(middle);

		  running_volTotaliser = working_volTotaliser + amt_real;
		  running_volTotaliserc = working_volTotaliserc + amt;


	   	  running_amtTotaliser = working_amtTotaliser + price_real;
	   	  running_amtTotaliserc = working_amtTotaliserc + price;

//		  if(operating_side == side_a)
//		   {
			   totaliser_vol1 = running_volTotaliser;    // update totaliser
			   totaliser_vol1c = running_volTotaliserc;  // update totaliser
	   		   totaliser_amt1 = running_amtTotaliser;    // update totaliser
	   		   totaliser_amt1c = running_amtTotaliserc;  // update totaliser
//		   }
//		   else
//		   {
//			   totaliser_vol2 = running_volTotaliser;   // update totaliser
//			   totaliser_vol2c = running_volTotaliserc; // update totaliser
//	   		   totaliser_amt2 = running_amtTotaliser2;    // update totaliser
//			   totaliser_amt2c = running_amtTotaliser2c;  // update totaliser
//		   }
		 //-------------------------------------------------------------------
		   return;
     }
  return;
}

//----------------------------------------
//
//
//
eSystemState filling_paused_state_Handler(void)
{

    return filling_paused_State;
}




//-----------------------------------------
void state_ini(void)
{
	float temp = 0.0;

	irrecov_flag = 0;
	progg = 0;
	eNextState =  idle_State; //   prog_State; //
	eLastState =  idle_State;
	index_ = 0;


//	lastSale1 = lastSale_storeA.lastSale_real;
//	lastSale1c =  lastSale_storeA.lastSale_cal;

//	temp = pulser2amt(current_pulser);
//	amt = dp(temp,dp_amount);
//    temp = amt2price(lastSale1);
//	price = dp(temp, dp_price);

//	char str_p[10] = {0};
//	char str_l[10] = {0};
//
//	snprintf(str_p, sizeof(str_p), "%.2f", price);
//	snprintf(str_l, sizeof(str_l), "%.2f", lastSale1);

	write_v(1, "p        ");  send_line1("p        ");
	write_v(2, "l        ");  send_line2("l        ");

    temp = amt2price(lastSale1c);
	price = dp(temp, dp_price1);
	make_string(P, dp(price, dp_price1) );
    make_string(L, dp(lastSale1c, dp_amount1) );

//	send_line1(str_p);
//	send_line2(str_l);

//	printDisp_c("p",1,0,8,LT,CLEAR); printDisp_f(str_p,1,0,8,LT,NOCLEAR);  //display price
//
//	printDisp_c("l",2,0,8,LT,CLEAR); printDisp_f(str_l,2,0,8,LT,NOCLEAR);  //display litre

	char str__[10] = {0};
	snprintf(str__, sizeof(str__), "%.2f", litre_price);
	send_line3(str__);

	 if(sellmode == L)
	   {
		  write_v(3, "l   0");  //send_keypad("l    ");  //5 xters lafeng
	   }
	   else
	   {
		  write_v(3, "p   0");  //send_keypad("p    ");  //5 xters  lafeng..
	   }
	 send_keypad(keyboard);
}

void states(void)
{
	eNewEvent = read_event();
	if (eNewEvent != _keypress_Event)
	{
        //ePrevState = eNextState;
	}

	eSystemEvent ev;
	  if( (eNewEvent < _no_Event)  )  //if event occured, check if the current state is sensitive to it..
	  {
		  eLastState = eNextState;   // store state...
           if(eNewEvent   == _auth_command_Event) //_nozzleup_Event) //_filling_pulse_Event  _nozzleup_Event)
            {
           	  //int yiuyu = 0;
            }
          //...   scan through the allowed  events of the state if its among them..
	    for (int i = 0;i < max_events_per_state;i++)
		 {
		   ev = (asStateEventMachine[eNextState].states[i]);  //
		   if (ev == 0)
			    {
			       // eNextState = eLastState;   //stay in the same event..
			        break;  // we reached end of event in allowed events array, exit scan..
			    }
		   if (eNewEvent == ev) // if the just triggered event is among allowed events..
		   {
                //then check if it has an handler and switch to the event Handler...
			   if (asEventMachine[ev-1].pfEventMachineHandler == NULL) //no handler supplied
			   {
				   break; //eNextState = (*asStateEventMachine[eNextState].pfStateMachineHandler)(); //switch to state handler.
			   }
			      else
			   {
				   eNextState = (*asEventMachine[eNewEvent-1].pfEventMachineHandler)();  //switch to event handler.
				   eNewEvent = _no_Event;
				   // int ty = 0;

			   }
		   } // if (eNewEvent == ev)
		 }  ///for

	     eNextState = (*asStateEventMachine[eNextState].pfStateMachineHandler)(); //switch to state handler.
	    // int yy = 56;
	  }
	 else
	  {
		//int s0 = (asStateEventMachine[eNextState].states[0]);
		//int s1 = (asStateEventMachine[eNextState].states[1]);
		//int s2 = (asStateEventMachine[eNextState].states[2]);

		  // no event, switch to same state...
		  eNextState = (*asStateEventMachine[eNextState].pfStateMachineHandler)();  //
					  //  (*asStateEventMachine[eNextState].pfStateMachineHandler)();
		 // int yui = 0;
		  return;
	  }


	  /*
	if((eNextState < last_State) && (eNewEvent < last_Event)&& (asStateMachine[eNextState].eStateMachineEvent == eNewEvent) && (asStateMachine[eNextState].pfStateMachineEvnentHandler != NULL))
		{
			// function call as per the state and event and return the next state of the finite state machine
			eNextState = (*asStateMachine[eNextState].pfStateMachineEvnentHandler)();
		}
		else
		{

		}  */
	  //eNextState = (*asStateEventMachine[eNextState].pfStateMachineHandler)();
	 // return;
}


//void states2(void)
//{
//	eNewEvent2 = read_event2();
//	if (eNewEvent2 != _keypress_Event)
//	{
//        //ePrevState = eNextState;
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
//			       // eNextState = eLastState;   //stay in the same event..
//			        break;  // we reached end of event in allowed events array, exit scan..
//			    }
//		   if (eNewEvent2 == ev) // if the just triggered event is among allowed events..
//		   {
//                //then check if it has an handler and switch to the event Handler...
//			   if (asEventMachine[ev-1].pfEventMachineHandler == NULL) //no handler supplied
//			   {
//				   break; //eNextState = (*asStateEventMachine[eNextState].pfStateMachineHandler)(); //switch to state handler.
//			   }
//			      else
//			   {
//				   eNextState2 = (*asEventMachine[eNewEvent2-1].pfEventMachineHandler)();  //switch to event handler.
//				   eNewEvent2 = _no_Event;
//				   // int ty = 0;
//
//			   }
//		   } // if (eNewEvent == ev)
//		 }  ///for
//
//	     eNextState2 = (*asStateEventMachine[eNextState2].pfStateMachineHandler)(); //switch to state handler.
//	    // int yy = 56;
//	  }
//	 else
//	  {
//		//int s0 = (asStateEventMachine[eNextState].states[0]);
//		//int s1 = (asStateEventMachine[eNextState].states[1]);
//		//int s2 = (asStateEventMachine[eNextState].states[2]);
//
//		  // no event, switch to same state...
//		  eNextState2 = (*asStateEventMachine[eNextState2].pfStateMachineHandler)();  //
//					  //  (*asStateEventMachine[eNextState].pfStateMachineHandler)();
//		 // int yui = 0;
//		  return;
//	  }
//
//
//	  /*
//	if((eNextState < last_State) && (eNewEvent < last_Event)&& (asStateMachine[eNextState].eStateMachineEvent == eNewEvent) && (asStateMachine[eNextState].pfStateMachineEvnentHandler != NULL))
//		{
//			// function call as per the state and event and return the next state of the finite state machine
//			eNextState = (*asStateMachine[eNextState].pfStateMachineEvnentHandler)();
//		}
//		else
//		{
//
//		}  */
//	  //eNextState = (*asStateEventMachine[eNextState].pfStateMachineHandler)();
//	 // return;
//}


void save_date(int8_t _entri)
{
	 ddate[0] = _entri;
	 clear_buffer();
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
