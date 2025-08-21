/*
 * ini.c
 *
 *  Created on: Jan 25, 2022
 *      Author: Tunjow
 */
#include "settings.h"
#include "main.h"
#include "myLibraries.h"
#include "keypad_module.h"
#include "buffer.h"
#include "interrupt_codes.h"
#include "stm32f4xx_hal.h"
#include "thread1.h"
#include "command.h"
#include "myLibraries.h"

#include "states.h"
#include "states2.h"

//#include "string.h"
#include <string.h>

#include "EEPROM.h"
#include "FRAM.h"

// include flash routines.
#include "../flash_mem/w25qxxConf.h"
#include "../flash_mem/w25qxx.h"
//=======================
#include "write__.h"
#include "stdio.h"
#include "log.h"

#include "../ds1307/ds1307_for_stm32_hal.h"
#include "../interface1/interface1.h"
#include "../printer/printer.h"
#include "../timers/timers.h"
#include "../config/config.h"
#include "../dart_protocol/dart_.h"

#include "pump_comm.h"

//#include "../otp/sha1.h"
#include "../otp/otp.h"
#include "stdlib.h"


#define PAGE_SIZE 32  // Define the page size (typically up to 256 bytes)

#define FLASH_BEGINPAGE_SIDEB 		0x400000 //--> 4,194,304 pg16,384
#define FLASH_ENDPAGE_SIDEB 		0x7FFFFF //--> 8,388,607 pg32767.996

char ep_messagee[650] = {0};

extern const uint32_t flash_beginA,
					  flash_endA,
					  flash_beginB,
					  flash_endB;

//uint8_t MSG[200] = {0};
extern uint8_t change_p1,
			   change_v1,
			   change_p2,
			   change_v2;

extern uint16_t eeprom_pageNum;

extern float auth_v1,
			 auth_p1,
			 auth_v2,
			 auth_p2;

extern w25qxx_t w25qxx;

extern UART_HandleTypeDef huart1;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;

extern I2C_HandleTypeDef hi2c1;

extern bool keypad_zerorise1,
			keypad_zerorise2;

//extern  pump_settings settings[2],
//					  copy[2],
//					  settings0[2];


extern pump_settings_stream1 self->settings_stream1[2],
						     settings_original_stream1[2],
						     copy_stream1[2];

extern pump_settings_stream2 self->settings_stream2[2],
				       	     settings_original_stream2[2],
							 copy_stream2[2];

extern pump_settings_stream3 self->settings_stream3[2],
				       	     settings_original_stream3[2],
							 copy_stream3[2];

extern pump_status_enum self->pump_status,
						pump_status_2;

extern pump pump_type,
 	 	 	disp_type1,
            disp_type2;

extern drive self->drive,
			 drive2;

extern uint8_t hmacKey[];

uint8_t batteryStatus = BATTERY_OK;
//volatile float batt_val = 0.0;
float CCRAM batt_val = 0.0;


extern uint32_t start_cycles,
				end_cycles,
				elapsed_cycles;

extern float time_us; // Time in microseconds


//===================================================
#define DEV_ADDR 0xa0
uint8_t dataw1[] = "hello world from EEPROM";
uint8_t dataw2[] = "This is the second string for flash memory";
float dataw3 = 1234.5678;

uint8_t datar1[50];
//uint8_t datar2[100];
float datar3;

uint16_t number = 0;

#define FRAM_I2C &hi2c1

// EEPROM ADDRESS (8bits)
//#define FRAM_ADDR 0xA0    //0x50     //0x54

//uint8_t write_data = 201,
//		read_data = 0;
//
//FRAM_Write(FRAM_I2C, FRAM_ADDR, 0, &write_data, 1);
//FRAM_Read(FRAM_I2C, FRAM_ADDR, 0, &read_data, 1);

//-----------------------------------
#if use_internal_rtc == 1
//RTC_TimeTypeDef sTime = {0};
//RTC_DateTypeDef sDate = {0};
extern RTC_HandleTypeDef hrtc;
RTC_DateTypeDef gDate;
RTC_TimeTypeDef gTime;
#endif
//-----------------------------------
char time_e[10];
char date[10];
char buffer[100] = { 0 };
char buffer1[300] = { 0 };

extern char otp_code[7] = { 0 };

int8_t ttime[3],
	   ddate[4],
	   ttime2[3],
	   ddate2[4];


extern unsigned long t_exec1,
					 t_exec2,
					 t_exec3,
					 t_exec4,
					 t_exec5,
					 t_exec6,
					 t_exec7,
					 t_exec8,
					 t_exec9;

int checkk = 0;

extern uint8_t batteryVoltage_ready = 0;

uint8_t self->keyPress = 0,
	    self->nonKeyPress = 0,
		keyPress2 = 0,
	    nonKeyPress2 = 0;

KeyState self->state = KEY_IDLE,
		 state2 = KEY_IDLE;

SwitchState switch_state1 = SWITCH_IDLE,
		    switch_state2 = SWITCH_IDLE;

uint32_t self->lastDebounceTime = 0,
		 lastDebounceTime2 = 0,
		 self->keyPressStartTime = 0,
		 keyPressStartTime2 = 0;

unsigned long cheq3 = 0;

extern uint8_t self->mamo_reached_flag,
			   mamo_reached_flag1_1,
			   self->hardwareError_flag,
			   hardwareErrorFlag_source1,
			   self->fillingComplete_flag = 0,
			   self->nozzlezUp = 0,
			   self->authsuspend_flag = 0,
			   self->fillingsuspend_flag = 0,
			   self->authresume_flag = 0,
			   self->fillingresume_flag = 0,
			   fillingresume_flag1_1 = 0,
			   fillingresume_flag2_1 = 0;

extern uint8_t mamo_reached_flag2,
			   mamo_reached_flag1_2,
			   hardwareError_flag2,
			   hardwareErrorFlag_source2,
			   fillingComplete_flag2 = 0,
			   nozzlezUp2 = 0,
			   authsuspend_flag2 = 0,
			   fillingsuspend_flag2 = 0,
			   authresume_flag2 = 0,
			   fillingresume_flag2 = 0;

extern uint8_t hour,minute,second,day,month,year,dayofweek;

extern const int lastSynchedFlashA_loc,
				 lastSynchedFlashB_loc,
				 totalTranxA_loc,
				 totalTranxA1_loc,
				 totalTranxB_loc,
				 totalTranxB1_loc,
				 synchedTranxA_loc,
				 synchedTranxA1_loc,
				 synchedTranxB_loc,
				 synchedTranxB1_loc;

extern const int16_t save_productType_loc,
					save_nozzleId_loc;

//==================================================

extern pump_names self->pumpName[2];


char testChar0[9] = {0};
char testChar1[9] = {0};
int sz;

extern  uint16_t fast_flow_threshold1,
				 fast_flow_threshold2,
				 slowFlow_startThreshold1,
				 slowFlow_startThreshold2,
				 slowFlow_endThreshold1,
				 slowFlow_endThreshold2;

int8_t val;

int t, t2 = 0;

uint8_t self->key__ = 0;

uint16_t _tt1 = 0,
		 _tt2 = 0,
//		 timer_ep = 0,
//		 timer_ep1 = 0,
		 timer_spi,
		 totalizer1Timer = 0,
		 totalizer2Timer = 0,
		 priceChange_timer1 = 0,
		 priceChange_timer2 = 0,
		 timer_config1 = 0,
		 timer_config2 = 0,
		 key19Timer1 = 0,
		 key19Timer2 = 0,
		 timer_noBatt = 0,
		 timer_lowBatt = 0,
		 delay_active = 0,
		 delay_active1 = 0;
		 ;
//		 ep2_timer = 0;

uint32_t timer_ep1,
		 timer_ep;

uint32_t transaction_period = 0,
		 transaction_period2 = 0;

unsigned int ttt1 = 0,
			 ttt2 = 0;

extern uint16_t self->autoSale_timer,
		 	 	autoSale_timer2;

uint8_t server_message_found = 0,
	    card1_message_found = 0,
	    card2_message_found = 0;

extern bool go_message;

uint16_t shutdown_timer1 = 0,
		 shutdown_timer2 = 0;

char key_lcd[10], key_lcd2[10]  = {0};
uint8_t buff[30], buff2[30]  = {0};

int self->keynew,keyold = 0;
int keynew2,keyold2 = 0;

int32_t  count__, count__2 = 0;

uint8_t rcv[41] = {0};

int retn;
 //=====================================================================
 /*        create instances of the input and output buffers  */
 fifo_t  dat_str;    //create an instance of the buffer - input buffer.
 fifo_t  dat_strout; //create an instance of the buffer - output buffer.

 //=====================variables from master===========================
 int self->totaliser_flag, totaliser_flag2 = 0;
 int self->key19_flag ,key19_flag2 = 0;

 uint8_t self->auth_cmd_flag = 0,
		 auth_cmd_flag2 = 0;

 nozzle_override override_,
 	 	 	 	 override_2;

 uint8_t stop_flag,
 	 	 stop_flag2 = 0;

 uint8_t self->error_clr_flag,
 	 	 error_clr_flag2 = 0;

 uint8_t self->key_flag,
 	 	 self->key_flag_old,
		 key_flag2,
		 key_flag_old2 = 0;

 uint8_t self->nozzle_flag,
 	 	 self->nozzle_flag_old,
		 nozzle_flag2,
		 nozzle_flag_old2 = 0,
		 self->nozzle_flag_key = 0,
		 nozzle_flag_key2 = 0,
		 self->nozzle_flag_key_old = 0,
		 nozzle_flag_key_old2 = 0;

 uint32_t self->pulser_count_old,
 	 	  self->pulser_new,
		  pulser_count_old2,
		  pulser_new2 = 0;

 extern int self->timer_flag_old,
 	 	 	self->timer_flag,
			timer_flag_old2,
			timer_flag2;

 int key_longpress_status,key_longpress_status2 = 0;

 extern uint8_t self->filling,
 	 	 	 	filling2;

 extern int self->auth_flag,
 	 	 	auth_flag2;

 extern int lat_cnt,
 	 	 	lat_cnt2;

 uint8_t self->keypress_flag,
 	 	 keypress_flag2 = 0;

 uint8_t self->keypress__,
 	 	 keypress__2 = 1;

 char wrt_[10],wrt_2[10]  = {0};

 uint8_t self->prog_entry, prog_entry2 = 0;

 char keyboard[9], keyboard2[9];
 //reference variables

 extern char str_[9],
 	 	 	 str_2[9];                 //used in states.c

 extern char keyboard_entry[10],
 	 	 	 keyboard_entry2[10];

 extern int self->keypress_,
 	 	 	keypress_2;

 extern int8_t index_,
 	 	 	   index_2;
 extern char upper1[10],
 	 	 	 upper2[10];

 extern uint32_t target_pulser1,
 	 	 	 	 current_pulser1,
				 target_pulser2,
				 current_pulser2,
				 overall_currentPulser1;

 extern uint32_t currentPulser_recovered1,
 				currentPulser_recovered2,
 				targetPulser_recovered1,
 				targetPulser_recovered2;

 extern float key_value CCRAM,
 	 	 	  key_value2 CCRAM;

 extern uint8_t sales_type1 CCRAM,
 	 	 	    sales_type2 CCRAM;

// extern int //index2,
// 	 	 	index2;

 extern char keypad_pw[10],
 	 	 	 keypad_pw2[10];

 extern char keypad_pw_[10],
 	 	 	 keypad_pw_2[10];

 extern int auth;      //first set this to
 extern int side;      //pump side selected.
 extern int level;
extern int _index,
		   _index2;

extern char sc1[10],
			sc12[10];

//extern float totaliser_vol1;
//extern float totaliser_vol1c;
//
//extern float totaliser_vol2;
//extern float totaliser_vol2c;

extern float totaliser_vol1,
			 totaliser_vol1c,
			 totaliser_vol2,
			 totaliser_vol2c;

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

extern float working_volTotaliser1;
extern float working_volTotaliser1c;

extern float working_volTotaliser2;
extern float working_volTotaliser2c;

extern float working_amtTotaliser1;
extern float working_amtTotaliser1c;

extern float working_amtTotaliser2;
extern float working_amtTotaliser2c;

uint8_t self->tot_longpress_flag,
		self->log_longpress_flag,
		self->key_longpress_flag = 0,
		self->progExit_longpress_flag = 0;

uint8_t tot_longpress_flag2,
		log_longpress_flag2,
		key_longpress_flag2 = 0,
		progExit_longpress_flag2 = 0;

extern uint8_t self->timerFlag_tone,
 	 	 	   self->timerFlagOld_tone,
			   timerFlag_tone2,
 	 	 	   timerFlagOld_tone2;

extern operatorfxn_  self->operatorfxn,
					 operatorfxn2;

 extern float original_pulse,
 	 	 	  original_pulse2;

 extern int operating_side;

 unsigned char bcd_[6] = {0};  // Array to hold the BCD result

 void compose_printer();

//=====================================================
 // global variable for the flash state
 //available to the two sides
 	  extern w25qxx_t w25qxx;
//-----------------------------------------------------
 extern uint32_t flash_read_idA;
 extern uint32_t flash_read_idB;

 extern const int flash_info_sto;
 extern const int flash_stoA;
 extern const int flash_stoB;

 extern const uint16_t flash_stoA_fram,
 		 	 		   flash_stoB_fram;

 extern uint32_t flash_read_idA;
 extern uint32_t flash_read_idB;
 extern flash_store_info flash_infoA,flash_infoB;

 extern int16_t save_pumpType_loc;

 /* Lookup table for the days of week. */
//const char *DAYS_OF_WEEK[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

const char *DAYS_OF_WEEK[7] = { "Sun", "Mon", "Tue", "Wed", "Thur", "Fri", "Sat" };
const char *MONTHS[12] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};


extern float litre_price , litre_price1 , litre_price2;
extern float amt , price;

extern uint8_t ep20_available1,
			   ep20_available2;

void led_pin_out(void);

//-----------------------------------------------------
print_struct print_struct_;        /// print structure

char station_name[15] = {0};
char station_address[32] = {0};
char logo[400] = {0};

int  printer_status = 0;
//-----------------------------------------------------

extern char interf_buf1[nextion_rx_bufsize];
bool message_found , message_found2;

extern config_struct config_data;

extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern char pump_rx_buf[pump_rx_bufsize];
extern char pump_buf[pump_rx_bufsize];

extern char uart2_rx_buf[pump_rx_bufsize];

extern bool pump_message_found;
extern bool pump_msg_ready1 ;

extern bool operating_sideA = true,
			operating_sideB = false;
//char uart2_rx_buf[pump_rx_bufsize] = {0};

//=====================================================

 void set_time (void)
 {
   #if  use_internal_rtc == 0
	 	    DS1307_SetTimeZone(+1, 00);
	 		DS1307_SetDate(3);
	 		DS1307_SetMonth(10);
	 		DS1307_SetYear(22);
	 		DS1307_SetDayOfWeek(3);
	 		DS1307_SetHour(10);
	 		DS1307_SetMinute(56);
	 		DS1307_SetSecond(00);
   #else
 	  RTC_TimeTypeDef sTime;
 	  RTC_DateTypeDef sDate;

	   sTime.Hours = 10;
	   sTime.Minutes = 27;
	   sTime.Seconds = 00;
	   sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	   sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	   if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	   {
		 while(1) {}
	   }

	   sDate.WeekDay = RTC_WEEKDAY_TUESDAY;
	   sDate.Month   = RTC_MONTH_MAY;
	   sDate.Date = 31;
	   sDate.Year = 22;

	   if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	   {
		   while(1) {}
	   }

	   HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);  // backup register
#endif

 }
 //=======================================================//
 void get_time(void)
 {
   //RTC_DateTypeDef gDate;
   //RTC_TimeTypeDef gTime;
#if  use_internal_rtc == 1
   /* Get the RTC current Time */
   HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
   /* Get the RTC current Date */
   HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);


   day = gDate.Date;
   month = gDate.Month;
   year = gDate.Year;
   dayofweek = gDate.WeekDay;

   hour = gTime.Hours;
   minute = gTime.Minutes;
   second = gTime.Seconds;

   /* Display time Format: hh:mm:ss */
  // sprintf((char*)time,"%02d-%02d-%02d",gTime.Hours, gTime.Minutes, gTime.Seconds);

   /* Display date Format: mm-dd-yy */
 //  sprintf((char*)date,"%02d-%02d-%2d",gDate.Date, gDate.Month,  gDate.Year);  // 2000 +
#else
	         day = DS1307_GetDate();
	 		 month = DS1307_GetMonth();
	 		 year = DS1307_GetYear();
	 		 dayofweek = DS1307_GetDayOfWeek();
	 		 hour = DS1307_GetHour();
	 		 minute = DS1307_GetMinute();
	 		 second = DS1307_GetSecond();
	 	//	int8_t zone_hr = DS1307_GetTimeZoneHour();
	 	//	uint8_t zone_min = DS1307_GetTimeZoneMin();
#endif

	 		/* May show warning below. Ignore and proceed. */
   }
 //=============================================//
#if  use_internal_rtc == 1
  void get_time_rtc(void)
  {

    //RTC_DateTypeDef gDate;
    //RTC_TimeTypeDef gTime;

    /* Get the RTC current Time */
    HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
    /* Get the RTC current Date */
    HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

    /* Display time Format: hh:mm:ss */
    sprintf((char*)time,"%02d-%02d-%02d",gTime.Hours, gTime.Minutes, gTime.Seconds);

    /* Display date Format: mm-dd-yy */
    //sprintf((char*)date,"%02d-%02d-%2d",gDate.Date, gDate.Month,  gDate.Year);  // 2000 +
		day = gDate.Date;
		month = gDate.Month;
		year = gDate.Year;
		dayofweek = gDate.WeekDay;

		hour = gTime.Hours;
		minute = gTime.Minutes;
		second = gTime.Seconds;



/*
	         day = DS1307_GetDate();
	  		 month = DS1307_GetMonth();
	  		 year = DS1307_GetYear();
	  		 dayofweek = DS1307_GetDayOfWeek();
	  		 hour = DS1307_GetHour();
	  		 minute = DS1307_GetMinute();
	  		 second = DS1307_GetSecond();   */

	  	//	int8_t zone_hr = DS1307_GetTimeZoneHour();
	  	//	uint8_t zone_min = DS1307_GetTimeZoneMin();
	  /*		char buffer[100] = { 0 };
	  		sprintf(buffer, "%04d-%02d-%02dT%02d:%02d:%02d%+03d:%02d \n",
	  				year, month, date, hour, minute, second, zone_hr, zone_min, DAYS_OF_WEEK[dow]);
	  		*/

    }
#endif

//==============================================================================
void clear_prn()
{
	uint32_t start , end;
	end = &print_struct_.tv[7];
	start = &print_struct_.transaction_id[0];

	memset( &print_struct_.transaction_id[0], 0 , ( end - start + 1 ) ); //sizeof(print_struct_)

	uint8_t* p = 0;

	 print_struct_.name = &station_name[0];
	 print_struct_.adr  = &station_address[0];
	 print_struct_.logo = &logo[0];

	//	p = &print_struct_.name[0];
	//
	//	int u = 0;
	//
	//	for(uint32_t i = start ; i < end+1 ; i++)
	//	{
	//		p = i;
	//		*p = 48;
	//		u++;
	//	}
}
//=============================================================
void print__1(void)
{
	extern UART_HandleTypeDef huart3;
	static int prn1 = 0;
	char temp[50] = {0};

	if(	printer_status == 0)
	{
		return ;
	}
//----------------------------------------------
	 if (prn1 == 0)
	   {
		    // busy
			///===============================
			int ln = strlen(print_struct_.name);
			int diff = 32 - ln;
			diff = diff/2;
	    	memset(temp,32,diff);

		strcat(temp,print_struct_.name);
		//sprintf(temp,"%s\n\r",print_struct_.name);
		int ln_ = strlen(temp);
		temp[ln_] = '\n'; temp[ln_ + 1] = '\r';
		send_printer(temp);
		//HAL_UART_Transmit(&huart3,temp, strlen(temp),1000  );
		prn1 = 1;
		// set timeout... xxx mS.
		 set_prn_tm(100);   //use time , ==> 100ms..
		return ;
	}
//--------------------------------------
 if(prn1 == 1)
 {
	if ( dec_prn_tm(0) == 1 )
	{
       //proceed..
		prn1++; // = 2;  // proceed further..
		return ;
	}
	return ;  // return here to recycle...
 }
//--------------------------------------
 if(prn1 == 2)
 {

	sprintf(temp,"%s\n\r",print_struct_.adr);
	send_printer(temp);
		prn1++; // = 3;
			// set timeout... xxx mS.
			 set_prn_tm(100);   //use time , ==> 100ms..
			return ;
 }
//--------------------------------------
 if(prn1 == 3)
 {
	if ( dec_prn_tm(0) == 1 )
	{
       //proceed..
		prn1++;  // = 4;  // proceed further..
		return ;
	}
	return ;  // return here to recycle...
 }
 //--------------------------------------
  if(prn1 == 4)
  {

 	sprintf(temp,"Date: %s\n\r",print_struct_.date_);
 	send_printer(temp);
 		prn1++;  // = 5;
 			// set timeout... xxx mS.
 			 set_prn_tm(100);   //use time , ==> 100ms..
 			return ;
  }
 //--------------------------------------
  if(prn1 == 5)
  {
 	if ( dec_prn_tm(0) == 1 )
 	{
        //proceed..
 		prn1++; // = 6;  // proceed further..
 		return ;
 	}
 	return ;  // return here to recycle...
  }
  //--------------------------------------
  if(prn1 == 6)
    {

		sprintf(temp,"Time: %s\n\r",print_struct_.time_);
		send_printer(temp);
   		prn1++;  // = 5;
   			// set timeout... xxx mS.
   			 set_prn_tm(100);   //use time , ==> 100ms..
   			return ;
    }
   //=======================================
  if(prn1 == 7)
   {
  	if ( dec_prn_tm(0) == 1 )
  	{
         //proceed..
  		prn1++; // = 6;  // proceed further..
  		return ;
  	}
  	return ;  // return here to recycle...
   }
   //--------------------------------------
  if(prn1 == 8)
     {

 		sprintf(temp,"--------------------------------\n\r");
 		send_printer(temp);
    		prn1++;  // = 5;
    			// set timeout... xxx mS.
    			 set_prn_tm(100);   //use time , ==> 100ms..
    			return ;
     }
    //=======================================
  if(prn1 == 9)
   {
  	if ( dec_prn_tm(0) == 1 )
  	{
         //proceed..
  		prn1++; // = 6;  // proceed further..
  		return ;
  	}
  	return ;  // return here to recycle...
   }
   //--------------------------------------
  if(prn1 == 10)
      {

  		sprintf(temp,"Plate Number: N/A %s\n\r");
  		send_printer(temp);
     		prn1++;  // = 5;
     			// set timeout... xxx mS.
     			 set_prn_tm(100);   //use time , ==> 100ms..
     			return ;
      }
     //=======================================
  if(prn1 == 11)
    {
   	if ( dec_prn_tm(0) == 1 )
   	{
          //proceed..
   		prn1++; // = 6;  // proceed further..
   		return ;
   	}
   	return ;  // return here to recycle...
    }
    //--------------------------------------
  if(prn1 == 12)
     {

 		sprintf(temp,"Transaction Type: %s\n\r",print_struct_.transaction_type);
 		send_printer(temp);
    		prn1++;  // = 5;
    			// set timeout... xxx mS.
    			 set_prn_tm(100);   //use time , ==> 100ms..
    			return ;
     }
    //=======================================
  if(prn1 == 13)
    {
   	if ( dec_prn_tm(0) == 1 )
   	{
          //proceed..
   		prn1++; // = 6;  // proceed further..
   		return ;
   	}
   	return ;  // return here to recycle...
    }
    //--------------------------------------
  if(prn1 == 14)
     {

 		//sprintf(temp,"Transaction Type: %s\n\r",print_struct_.transaction_type);
 		//send_printer(temp);
    		prn1++;  // = 5;
    			// set timeout... xxx mS.
    			 set_prn_tm(100);   //use time , ==> 100ms..
    			return ;
     }
  //==========================================
  if(prn1 == 15)
    {
	  if ( dec_prn_tm(0) == 1 )
		{
			  //proceed..
			prn1++; // = 6;  // proceed further..
			return ;
		}
   	return ;  // return here to recycle...
    }
    //--------------------------------------
  if(prn1 == 16)
      {

  		sprintf(temp,"Voucher: %s\n\r",print_struct_.voucher_);
  		send_printer(temp);
     		prn1++;  // = 5;
     			// set timeout... xxx mS.
     			 set_prn_tm(100);   //use time , ==> 100ms..
     			return ;
      }
   //==========================================
  if(prn1 == 17)
     {
 	  if ( dec_prn_tm(0) == 1 )
 		{
 			  //proceed..
 			prn1++; // = 6;  // proceed further..
 			return ;
 		}
    	return ;  // return here to recycle...
     }
     //--------------------------------------
  if(prn1 == 18)
       {

   		sprintf(temp,"Product: %s\n\r",print_struct_.product_);
   		send_printer(temp);
      		prn1++;  // = 5;
      			// set timeout... xxx mS.
      			 set_prn_tm(100);   //use time , ==> 100ms..
      			return ;
       }
    //==========================================
  if(prn1 == 19)
     {
 	  if ( dec_prn_tm(0) == 1 )
 		{
 			  //proceed..
 			prn1++; // = 6;  // proceed further..
 			return ;
 		}
    	return ;  // return here to recycle...
     }
     //--------------------------------------
  if(prn1 == 20)      //pump name...
       {

   		sprintf(temp,"PUMP: %s\n\r",print_struct_.pn);
   		send_printer(temp);
      		prn1++;  // = 5;
      			// set timeout... xxx mS.
      			 set_prn_tm(100);   //use time , ==> 100ms..
      			return ;
       }
    //==========================================
  if(prn1 == 21)
     {
 	   if ( dec_prn_tm(0) == 1 )
 		{
 			  //proceed..
 			prn1++; // = 6;  // proceed further..
 			return ;
 		}
    	return ;  // return here to recycle...
     }
     //--------------------------------------
  if(prn1 == 22)      //price/litre...
       {

   		sprintf(temp,"Price/Litre: %s\n\r",print_struct_.pl);
   		send_printer(temp);
      		prn1++;  // = 5;
      			// set timeout... xxx mS.
      			 set_prn_tm(100);   //use time , ==> 100ms..
      			return ;
       }
    //==========================================
  if(prn1 == 23)
      {
  	   if ( dec_prn_tm(0) == 1 )
  		{
  			  //proceed..
  			prn1++; // = 6;  // proceed further..
  			return ;
  		}
     	return ;  // return here to recycle...
      }
      //--------------------------------------
  if(prn1 == 24)      //price/litre...
        {

    		sprintf(temp,"Transaction Amount: %s\n\r",print_struct_.ta);
    		send_printer(temp);
       		prn1++;  // = 5;
       			// set timeout... xxx mS.
       			 set_prn_tm(100);   //use time , ==> 100ms..
       			return ;
        }
     //==========================================
  if(prn1 == 25)
      {
  	   if ( dec_prn_tm(0) == 1 )
  		{
  			  //proceed..
  			prn1++; // = 6;  // proceed further..
  			return ;
  		}
     	return ;  // return here to recycle...
      }
      //--------------------------------------
  if(prn1 == 26)      //price/litre...
         {

     		sprintf(temp,"Transaction Volume: %s\n\r",print_struct_.tv);
     		send_printer(temp);
        		prn1++;  // = 5;
        			// set timeout... xxx mS.
        			 set_prn_tm(100);   //use time , ==> 100ms..
        			return ;
         }
      //==========================================
  if(prn1 == 27)
      {
  	   if ( dec_prn_tm(0) == 1 )
  		{
  			  //proceed..
  			prn1++; // = 6;  // proceed further..
  			return ;
  		}
     	return ;  // return here to recycle...
      }
      //--------------------------------------
  if(prn1 == 28)
     {

 		sprintf(temp,"--------------------------------\n\r");
 		send_printer(temp);
    		prn1++;  // = 5;
    			// set timeout... xxx mS.
    			 set_prn_tm(100);   //use time , ==> 100ms..
    			return ;
     }
    //=======================================
  if(prn1 == 29)
       {
   	   if ( dec_prn_tm(0) == 1 )
   		{
   			  //proceed..
   			prn1++; // = 6;  // proceed further..
   			return ;
   		}
      	return ;  // return here to recycle...
       }
  //-------------------------------------------
  if(prn1 == 30)      //price/litre...
         {
		///===============================
	    char gr[] = "THANK YOU";
				int ln = strlen(gr);
				int diff = 32 - ln;
				diff = diff/2;
		    	memset(temp,32,diff);


			strcat(temp,gr);
			//sprintf(temp,"%s\n\r",print_struct_.name);
			int ln_ = strlen(temp);
			temp[ln_] = '\n'; temp[ln_ + 1] = '\r';
			send_printer(temp);
			//HAL_UART_Transmit(&huart3,temp, strlen(temp),1000  );
			prn1++;   // = 1;
			// set timeout... xxx mS.
			 set_prn_tm(100);   //use time , ==> 100ms..
			return ;
         }
      //==========================================
  if(prn1 == 31)
        {
    	   if ( dec_prn_tm(0) == 1 )
    		{
    			  //proceed..
    			prn1++; // = 6;  // proceed further..
    			//printer_status = 0;
    			return ;
    		}
       	return ;  // return here to recycle...
        }
   //-------------------------------------------
  if(prn1 == 32)
      {
  		sprintf(temp,"\n\n\n\n\r");
  		send_printer(temp);
     		prn1++;  // = 5;
     			// set timeout... xxx mS.
     			 set_prn_tm(100);   //use time , ==> 100ms..
     			return ;
      }
     //=======================================
  if(prn1 == 33)
         {
     	   if ( dec_prn_tm(0) == 1 )
     		{
     			  //proceed..
     			prn1++; // = 6;  // proceed further..
     			printer_status = 0;
     			return ;
     		}
        	return ;  // return here to recycle...
         }
    //-------------------------------------------
}
//======================================================================


void check_interface1()
{

   if (message_found == 1)
    {
	   message_found = 0;    //clear the flag...
	   msg_parse(interf_buf1,1);
    }

}

//void check_interface2()
//{
//
//   if (message_found2 == 1)
//    {
//	   message_found2 = 0;    //clear the flag...
//	   msg_parse(interf_buf2,1);
//    }
//
//}

void compose_printer()
{
	extern log_new self->log_new , log_b_new;
	extern uint32_t flash_read_idA;
	extern uint32_t flash_read_idB;

	extern const uint32_t flash_beginA;
	extern const uint32_t flash_endA;

	extern const uint32_t flash_beginB;
	extern const uint32_t flash_endB;

	extern uint8_t self->prog_entry;
	extern uint8_t prog_entry2;

	    sprintf(print_struct_.name,"RECTER");
		sprintf(print_struct_.adr,"Ilupeju Bypass Ilupeju, Lagos");
		sprintf(print_struct_.date_,"Thur Oct ,%d , 2022",self->log_new.date._dd);
		sprintf(print_struct_.time_," %d : %d ", self->log_new.time_e._hh, self->log_new.time_e._mn);
		sprintf(print_struct_.transaction_type,"USSD");
		sprintf(print_struct_.voucher_,"0a2345ba");
		sprintf(print_struct_.product_,"AGO");
		sprintf(print_struct_.pn,"A2");
		sprintf(print_struct_.ta,"%2f",self->log_new.pr_);  //price);
		sprintf(print_struct_.tv,"%2f",self->log_new.vol_); //amt);
		sprintf(print_struct_.pl,"180.00");

		printer_status = 1;
}

//char sendATcommand(char* cmd)
//{
//    HAL_UART_Transmit(&huart1, (uint8_t *)cmd, strlen(cmd), 1000);
//}

//-------------------------------------------------------------
  //-----------------------------------------------------------
 void setup()
{
	 self->pump_status = STATUS_UNKNOWN;
//	 pump_status_2 = STATUS_UNKNOWN;

	 self->pump_status_4G = STATUS_PUMP_ON;

	 dart_init();

//	 ep_send(ep0);

	 mcu_power(&disp, ACTIVATE);

	 batt_charge(&disp, ACTIVATE);

	 displayandkeypad_power(&disp, ACTIVATE);   //latch power...

	 modem_power(&disp, ACTIVATE);

//	 self->settings_stream1.keypad__ = LAFNG18_K;
//	 self->settings_stream1[1].keypad__ = LAFNG18_K;

//	 self->settings_stream1.mode = MANUAL_MODE;

	 //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
	 // 				LAFENG Valve's Signal is inverted for this version of PCB                      //
	 //											PCB V5.0											   //
	 //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//

	 if (self->settings_stream1.pump_type_ == LAFENG)
	 {
		 drive_slow_sole(&self, ACTIVATE);     // ACTIVATE here actually means DEACTIVATE
		 drive_fast_sole(&self, ACTIVATE);
	 }
//	 else if (self->settings_stream1.pump_type_ == BLUESKY)
//	 {
//		 drive_slow_sole1(DEACTIVATE);
//		 drive_fast_sole1(DEACTIVATE);
//	 }

//	 if (self->settings_stream1[1].pump_type_ == LAFENG)
//	 {
//		 drive_slow_sole2(ACTIVATE); // ACTIVATE here actually means DEACTIVATE
//		 drive_fast_sole2(ACTIVATE);
//	 }
//	 else if (self->settings_stream1[1].pump_type_ == BLUESKY)
//	 {
//		 drive_slow_sole2(DEACTIVATE);
//		 drive_fast_sole2(DEACTIVATE);
//	 }

	 //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//

//	 while(1)
//	 {
//		 retn = write_keypad_lcd(0,"1234");
//		 HAL_Delay(1000);
////		 write_keypad_lcd("    ");
////		 HAL_Delay(80);
//		 while(1)
//		 {
//			 retn = write_keypad_lcd(1,"5678");
//			 HAL_Delay(1000);
//		 }
//	 }

	 //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW//

	 //ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ//
	 ///////////////////////////// FRAM TEST-GROUND ///////////////////////////////
	 //ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ//

	 //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//

//	 while(true)
//	 {
//		  // Example: Write a byte to address 0x00
//		 FRAM_WriteByte(0x00, 24); // Write the value 42 to address 0x00
//
////		  while (1)
////		  {
//		    // Read the value from FRAM at address 0x00
//		    uint8_t data = FRAM_ReadByte(0x00);
//
//		    // Add your code to use 'data', e.g., send via UART, toggle LEDs, etc.
//		    HAL_Delay(1000);  // Delay to avoid rapid reading
////		  }
//
//		  // Example data to write
//		  uint8_t writeData[PAGE_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
//		                                  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
//
//		  // Write data to FRAM starting at address 0x00
//		  FRAM_Write(0x00, writeData, PAGE_SIZE);
//
//		  // Buffer to hold the read data
//		  uint8_t readBuffer[PAGE_SIZE];
//
//		  // Read a page of data from FRAM starting at address 0x00
//		  FRAM_Read(0x00, readBuffer, PAGE_SIZE);
//
////		  while (1) {
//		    // Use the read data (e.g., send via UART, toggle LEDs, etc.)
//		    HAL_Delay(1000);  // Delay to avoid rapid loop
////		  }
//	 }

// while(1)
// {
//
//	 uint8_t write_data = 201,
//	 		 read_data = 0;
//
//	 FRAM_Write(FRAM_I2C, FRAM_ADDR, 0, &write_data, 1);
//
//	 HAL_Delay(1000);
//
//	 FRAM_Read(FRAM_I2C, FRAM_ADDR, 0, &read_data, 1);
//
////	 FRAM_Write_NUM (0, 0, 234);
//
//     HAL_Delay(1000);
//
////     float fram_read;
////
////     fram_read = FRAM_Read_NUM (0, 0);
//
////     EEPROM_Write_NUM (900, 0, 234);
//
////     HAL_Delay(1000);
//
////     get_time();
//
////     fram_read = EEPROM_Read_NUM (900, 0);
//
////     HAL_Delay(1000);
//
//}
//
//     // ===========================================================================
//		 //==============================================
//		 //    This step is to compose the settings.
//		 //==============================================
//		 make_settings(side_a);
//		 make_settings(side_b);
//
//		 self->settings_stream1.pi_ = 797.15;   //798.1;  //407.3;   //399.25;   //798.35;
//		 self->settings_stream1.pi_c = 797.15;  //767.40;  //391.64;   //383.89;  //760.33;
//		 self->settings_stream1[1].pi_ = 799.8;    //799.25;  //399.25;   //798.35;
//		 self->settings_stream1[1].pi_c = 799.8;   //768.51;   //383.89;  //760.33;
//
// //		Pulser_count1 = 15962 20|0.8|0.0
// //		Pulser_count2 = 15985 20|0.8|0.0
//
// //4650
//		 vol_real1 = 20;
//		 vol_real2 = 20;
//		 vol_calibrated1 = 20;  //20.8;   //21.0;
//		 vol_calibrated2 = 20;  //20.8;
//		 vol_effective1 = 20;   //20.8;    //21.0;
//		 vol_effective2 = 20;   //20.8;
//
////     		 calib_pulser1 =  (self->settings_stream1.pi_c * vol_calibrated1);
////     		 calib_pulser2 =  (self->settings_stream1[1].pi_c * vol_calibrated2);
//
//		 save_settings();
//
////		 I2C1->CR1 |= (1<<9);  // Stop I2C
//
//		 retrieve_settings();
// }

//	srand(time(NULL));

    keypad_lcd(&disp, &self, 2, "0");                      //keypad_lcd(0,"0");
//    keypad_lcd2(2, "0");

	buffer_init(&dat_str,buffer_length);    //initialise the buffer

//	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_1 |TIM_CHANNEL_2  ); //TIM_CHANNEL_ALL); //start encoder acquinsition.
//	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);  //TIM_CHANNEL_1 |TIM_CHANNEL_2); //TIM_CHANNEL_ALL);


	//////////////////////////////////////////////////////////
	//000000000000000000000000000000000000000000000000000000//

	retrieve_settings_fram(&self);

	//======================================================//


//	self->settings_stream2[0].pulser_type_ = quadrature; //non_quadrature;   //quadrature;
//	self->settings_stream2[1].pulser_type_ = quadrature;   //non_quadrature;   //quadrature;
//	self->settings_stream1.keypad__ = LAFNG18_K;
//	self->settings_stream1[1].keypad__ = LAFNG18_K;

	if(self->settings_stream2[0].pulser_type_ == quadrature)
	{
//		HAL_StatusTypeDef HAL_TIM_Encoder_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
		HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_1 |TIM_CHANNEL_2); //TIM_CHANNEL_ALL); //start encoder acquinsition.
		HAL_TIM_Encoder_Start(self->htim, uint32_t Channel);
	}
	else if(self->settings_stream2[0].pulser_type_ == non_quadrature)
	{
//		HAL_TIM_Base_Start(&htim5);
		HAL_TIM_Base_Start(self->htim,);
	}

	if(self->settings_stream2[1].pulser_type_ == quadrature)
	{
		HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);  //TIM_CHANNEL_1 |TIM_CHANNEL_2); //TIM_CHANNEL_ALL);
	}
	else if(self->settings_stream2[1].pulser_type_ == non_quadrature)
	{
		HAL_TIM_Base_Start(&htim2);

//		HAL_TIM_Base_Start_IT(&htim2);
	}

	//keypad_ini();
	//   uint16_t CounterTicks1 = 0;
	//   uint16_t CounterTicks2 = 0;
	//------------------------------
	//initialise realtime clock
	    disp->setup_ds1307();
//	    disp->get_time();
//	    disp->set_time();
	    disp->get_time();
	//-----------------------------


//	read_config();

	clear_screen(Nozzle *self);         //	clear_screen2();  //nextion1_ini();
//	clear_screen2();

//	pump_ini();    // activate the pump communication I/O

	HAL_UART_Receive_IT(&huart1, uart1_rx_buf, pump_rx_bufsize);    //config

//	HAL_UART_Receive_IT(&huart2, uart2_rx_buf, pump_rx_bufsize);    //server-communication

//	HAL_UART_Receive_IT(&huart3, uart3_rx_buf, pump_rx_bufsize);    //printer1
	HAL_UART_Receive_IT(&huart3, rxBuffer, 1);
//	HAL_UART_Transmit(&huart3, "Hello, I'm Usart-3!\r\n", 21, HAL_MAX_DELAY);


	/*=========================================================================*/

	//-------------------------------------------------------------------------//

	char strr__[60] = {0};

	HAL_UART_Transmit(&huart3, "\n\n\n\n =====================", 26, HAL_MAX_DELAY);
	HAL_Delay(1);
	snprintf(strr__, sizeof(strr__), "\n\n   DATE : %02d-%02d-20%d\n", day, month, year);
	HAL_UART_Transmit(&huart3, strr__, strlen((char*)strr__), HAL_MAX_DELAY);
	HAL_Delay(1);
	memset(strr__, sizeof(strr__), '\0');
	snprintf(strr__, sizeof(strr__), "\n   TIME : %02d%02d Hrs\n\n", hour, minute);
	HAL_UART_Transmit(&huart3, strr__, strlen((char*)strr__), HAL_MAX_DELAY);
	HAL_Delay(1);
	HAL_UART_Transmit(&huart3, " =====================\n\n\n\n", 26, HAL_MAX_DELAY);
	HAL_Delay(1);

	//-------------------------------------------------------------------------//

	/*=========================================================================*/


	HAL_UART_Receive_IT(&huart5, uart5_rx_buf, pump_rx_bufsize);    //printer2

	otp(hmacKey, 10, pump_SN);
//	otp2(hmacKey, 10);

//		while(1)
//		{
////			//check_interface1();
//			HAL_Delay(1000);
//			char  bbf[20] = {"hello there!"};
//	        HAL_UART_Transmit(&huart2,bbf, strlen(bbf),2000);
////	      extern char pump_rx_buf[pump_rx_bufsize];
////	      HAL_UART_Receive(&huart1,&pump_rx_buf, 10,10000);
//		}

//	while(1)
//	 {
//		 otp_seed1 = 1234;
//
//		 save_config_otpSeed_time_fram(side_a);
//
//		 configChange[0].otp_seed = 0;
//		 configChange[0].time_stamp = 0;
//
//		 retrieve_config_otpSeed_time_fram(side_a);
//
//		 HAL_Delay(1000);
//
//	 }

//	while(1)
//	{
//		current_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);
//		HAL_Delay(1000);
//	}


//unsigned long otp_seed1 = 9071;  //1000;
////long otp_preseed2 = 1234567890,
////     otp_seed = 0;
//
//while(1)
//{
////	otp_preseed1 = otp_preseed1 + 1;
////
////	otp_seed = otp_preseed1 + otp_preseed2;
////	otp_codeInt1 = getCode(otp_seed1++);
//	otp_codeInt1 = getCode(otp_seed1);
////	strcpy(code, *newCode);
//
//	sprintf(otp_code1, "%06ld", otp_codeInt1);
//
//	HAL_Delay(2000);
//
//	HAL_Delay(20);
//}


//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//

//ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ//
/////////////////////// CHECKING FOR TAMPER MECHANISM ////////////////////////
//ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ//

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//

//while(1)
//{
//	 if( HAL_GPIO_ReadPin(other_GPIO_Port, other_Pin) == 0 )
//	 {
//		 HAL_Delay(200);
//	 }
//	 if( HAL_GPIO_ReadPin(case_open_GPIO_Port, case_open_Pin) == 0)
//	 {
//		 HAL_Delay(200);
//	 }
//
//	 if(HAL_GPIO_ReadPin(pulser1_detect_GPIO_Port, pulser1_detect_Pin) == 1 )
//	 {
//		 HAL_Delay(200);
//	 }
//	 if(HAL_GPIO_ReadPin(pulser2_detect_GPIO_Port, pulser2_detect_Pin) == 1 )
//	 {
//		 HAL_Delay(200);
//	 }
//
//	 if( HAL_GPIO_ReadPin(tamper_GPIO_Port, tamper_Pin) == 0 )
//	 {
//		 HAL_Delay(200);
//	 }
//	 if( HAL_GPIO_ReadPin(tamper_GPIO_Port, tamper_Pin) == 1 )
//	 {
//		 HAL_Delay(200);
//	 }
//
//}

//while(1)
//{

//	HAL_GPIO_WritePin(clockPin_GPIO_Port, clockPin_Pin, GPIO_PIN_SET);
//	_Delay(1);
//	HAL_GPIO_WritePin(clockPin_GPIO_Port, clockPin_Pin, GPIO_PIN_RESET);
//	_Delay(1);
//	HAL_GPIO_WritePin(dataPin_GPIO_Port, dataPin_Pin, GPIO_PIN_SET);
//	_Delay(1);
//	HAL_GPIO_WritePin(dataPin_GPIO_Port, dataPin_Pin, GPIO_PIN_RESET);
//	_Delay(1);
//	HAL_GPIO_WritePin(latchPin_GPIO_Port, latchPin_Pin, GPIO_PIN_SET);  //latch pin high
//	_Delay(1);
//	HAL_GPIO_WritePin(latchPin_GPIO_Port, latchPin_Pin, GPIO_PIN_RESET);  //Latch pin low
//	_Delay(1);
//	printDisp_c("fuel",1,2,4,LT,CLEAR);
//	printDisp_c2("fuel",1,2,4,LT,CLEAR);

//	HAL_Delay(2500);

//	HAL_GPIO_WritePin(T1output_GPIO_Port,T1output_Pin, GPIO_PIN_SET);
//	HAL_Delay(2500);
//	HAL_GPIO_WritePin(T1output_GPIO_Port,T1output_Pin, GPIO_PIN_RESET);
//	HAL_Delay(2500);
//	HAL_GPIO_WritePin(T2output_GPIO_Port,T2output_Pin, GPIO_PIN_SET);
//	HAL_Delay(2500);
//	HAL_GPIO_WritePin(T2output_GPIO_Port,T2output_Pin, GPIO_PIN_RESET);
//	HAL_Delay(2500);

//	if(HAL_GPIO_ReadPin(pulser1_detect_GPIO_Port, pulser1_detect_Pin) == 1 )
//	{
//		HAL_Delay(2500);
//	}
//	else
//	{
//		HAL_Delay(2500);
//	}
//	if(HAL_GPIO_ReadPin(pulser2_detect_GPIO_Port, pulser2_detect_Pin) == 1 )
//	{
//		HAL_Delay(2500);
//	}
//	else
//	{
//		HAL_Delay(2500);
//	}
//
//}

//	 float temp;
//	while(1){
////		float floatt = 70.402;
//		auth_p = 70.402;
//		litre_price2 = 120.0;
//		 price2 = dp2(auth_p, 2);
//		 float temp = amt2price2(65.12);
//		 HAL_Delay(2000);
//	}

//	while(1){
//		drive_totaliser1(ACTIVATE);
//		HAL_Delay(2000);
//	}
//===================================================================

    clear_prn();

//===================================================================
//	printDisp_c("Fuel",1,2,4,LT,CLEAR);
//	printDisp_c("NNetrics",2,0,8,LT,CLEAR);
//	//}
//	printDisp_c2("Fuel",1,2,4,LT,CLEAR);
//	printDisp_c2("NNetrics",2,0,8,LT,CLEAR);
//
//	HAL_Delay(2500);
//
//   	reset_timer(15);
//	//start_timer(15);
//
//   //======= initialize ========
//   self->eNextState = idle_State;
//   eNextState2 = idle_State;
//
////   state_ini();
////   state_ini2();
//
//   clr_screen1();
//   clr_screen2();

#if test_battery == 1
//   while(1)
//   {
////	   clr_screen1();
//
//	   char st__[10] = {0};
//	   float batt_val = battery_read();
////	   printDisp_f(batt_val,2,0,8,LT,CLEAR);
////	   printDisp_f(batt_val, 2, 0,5,RT,CLEAR ); //send_line2(scc);
////	   printDisp_f(batt_val,2,0,7,RT,CLEAR);
//	   snprintf(st__, sizeof(st__),
//	                   "%0.2f",
//					   batt_val);
//	   printDisp_c(st__,2,0,8,LT,CLEAR);
//	   HAL_Delay(1500);
//   }
#endif   //#if test_batteryPresence


   //int t =
  //	clear_screen1();
 //  HAL_UART_Receive_DMA(&huart1,  &dat_str.buf[0] , 30);   //activate the fifo dma receiver.


//=========================================================
//                test the led pin....
   //char txx[]
//   HAL_UART_Transmit(&huart1,"Entering config",15,2000);
//=========================================================

//================================================================================
#if test_power == 1
   while(1)
   {
	   int rs = 0;
	   //if( readtotaliser1_state() == 1)

		 if(readpwr() == 0)
		   {
			printDisp_c("0",1,0,4,LT,CLEAR);
			//HAL_Delay(2000);
			rs = 1;
		   }
		   else
		   {
			//drive_totaliser1(DEACTIVATE);
			printDisp_c("1",1,0,4,LT,CLEAR);
			//HAL_Delay(2000);
			rs = 0;
		   }
   }
#endif


#if test_motor == 1
      while(1)
      {
    	  drive_motor1(ACTIVATE);
    	  drive_motor2(ACTIVATE);
    	  HAL_Delay(1000);
    	  drive_motor1(DEACTIVATE);
    	  drive_motor2(DEACTIVATE);
    	  HAL_Delay(1000);
      }
#endif

#if test_solenoid == 1
      while(1)
      {
    	  drive_slow_sole1(ACTIVATE);
    	  drive_slow_sole2(ACTIVATE);
    	  drive_fast_sole1(ACTIVATE);
    	  drive_fast_sole2(ACTIVATE);
    	  HAL_Delay(1000);
    	  drive_slow_sole1(DEACTIVATE);
    	  drive_slow_sole2(DEACTIVATE);
    	  drive_fast_sole1(DEACTIVATE);
    	  drive_fast_sole2(DEACTIVATE);
    	  HAL_Delay(1000);
      }
#endif

#if test_totaliser == 1
   while(1)
   {
			drive_totaliser1(DEACTIVATE);
			drive_totaliser2(DEACTIVATE);
			printDisp_c("0",1,0,4,LT,CLEAR);
			HAL_Delay(2000);

			drive_totaliser1(DEACTIVATE);
			drive_totaliser2(DEACTIVATE);
			printDisp_c("1",1,0,4,LT,CLEAR);
			HAL_Delay(2000);
   }
#endif

#if test_rtc == 1

tmmm:
    HAL_Delay(1000);
    get_time();
    goto tmmm;

#endif


//goto skip;

//while(1)
//{
//	calib_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);  //use hardware counter
//}

//  if( HAL_GPIO_ReadPin(nLed_GPIO_Port, nLed_Pin) == 1)  // config
  if( (HAL_GPIO_ReadPin(settings1_GPIO_Port, settings1_Pin) == 1 ) || ( HAL_GPIO_ReadPin(settings2_GPIO_Port, settings2_Pin) == 1) )
  {
	  printDisp_c("config", 1, 2, 4, LT, CLEAR);
	  printDisp_c("NNode", 2, 0, 5, LT, CLEAR);

	  printDisp_c2("config", 1, 2, 4, LT, CLEAR);
	  printDisp_c2("NNode", 2, 0, 5, LT, CLEAR);

//	  settings[0].passwd1 = 0000;
//	  settings[0].passwd2 = 0000;
//	  settings[0].passwd1 = 0000;
//	  settings[0].passwd2 = 0000;

	  HAL_Delay(2500);
//
//	 //==============================================
//	 //    This step is to compose the settings.
//	 //==============================================
//	 make_settings(side_a);
//	 make_settings(side_b);
//
//	 save_settings_fram();
////	 save_volumeTotaliser_fram(side_a); //side_a
////	 save_volumeTotaliser_fram(side_b);
//	 flash_infoA.current_loc = 0;
//	 flash_infoA.number_logs = 0;
//	 FRAM_Write(flash_stoA_fram, &flash_infoA, sizeof(flash_infoA));
//	 FRAM_Write(flash_stoB_fram, &flash_infoB, sizeof(flash_infoA));
//	 // ===========================================================================


	  disp->config_mode = 1;
//	  retrieve_settings();

//	  HAL_UART_Transmit (&huart1, "Hey!", 4, 1000);

//	  {"ni":"p1","pn":"pms","dt":"bluesky886n","kt":"bluesky22"}

//	  {"ni":"p17","pn":"pms","dt":"lafeng885",kt":"lafeng18k"}

//	  {"ni":"p17","pn":"pms","dt":"lafeng885",kt":"lafeng18k_v2"}

//	  {"ni":"p1","pn":"ago","dt":"bluesky886n","kt":"bluesky22","pi":"860537065690737","tn":"0003"}

//	  {"ni":"p17","pn":"pms","dt":"bluesky886i","kt":"bluesky22"}

	  /*
	  {"ni":"p17","pn":"pms","dt":"bluesky886n","kt":"bluesky22"}
	  {"ni":"p17","pn":"pms","dt":"bluesky886n","kt":"bluesky18k"}
	  {"ni":"p17","pn":"pms","dt":"bluesky886n","kt":"lafeng17k"}
	  {"ni":"p17","pn":"pms","dt":"bluesky886n","kt":"lafeng18k"}
	  {"ni":"p17","pn":"pms","dt":"bluesky886n","kt":"lafeng18k_v2"}
	  {"ni":"p17","pn":"pms","dt":"bluesky886n","kt":"bluesky22"}
	  */

//	 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
//	 HAL_Delay(1);
//	 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
//     HAL_Delay(1);

	  while(disp->config_found == 0)   //Loops idly while configuration is yet to be inputted
	  {
		 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
		 HAL_GPIO_TogglePin(nLed_GPIO_Port, nLed_Pin);  // Toggle LED
		 HAL_Delay(200);
		 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
		 HAL_GPIO_TogglePin(nLed_GPIO_Port, nLed_Pin);  // Toggle LED
		 HAL_Delay(1000);
	  }

//	  config_rx_parse();
////	  pumpType_configure();
//	  pumpType_configure_fram();
//
//
//	  // ===========================================================================
//
//
//	  //********************  INITIALIZATIONS ***********************//
//
//	  //----------------------//
//	  firstTotalizer_day();
//	  //----------------------//

//	  uint16_t firstTime = 0;
//
//	  uint16_t flash_beginA_page = 0,            //0x0000
//			   flash_beginB_page = 16384;        //0x4000

//	  EEPROM_Write(totalTranxA_loc, totalTranxA1_loc, &firstTime, sizeof(firstTime));
//	  EEPROM_Write(totalTranxB_loc, totalTranxB1_loc, &firstTime, sizeof(firstTime));
//	  EEPROM_Write(synchedTranxA_loc, synchedTranxA1_loc, &firstTime, sizeof(firstTime));
//	  EEPROM_Write(synchedTranxB_loc, synchedTranxB1_loc, &firstTime, sizeof(firstTime));
//
//	  EEPROM_Write(lastSynchedFlashA_loc, 0, &flash_beginA_page, sizeof(flash_beginA_page));
//	  EEPROM_Write(lastSynchedFlashB_loc, 0, &flash_beginB_page, sizeof(flash_beginB_page));

//	  for (int i = 0; i < eeprom_pageNum; i++)
//	  {
//		  EEPROM_PageErase(i);
//	  }

	  HAL_GPIO_WritePin(nLed_GPIO_Port, nLed_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);

	  FRAM_ChipErase();

	  clear_logA();
	  clear_logB();

	  W25qxx_EraseChip();

//	  clear_totalTransaction_sides(side_a);
//	  clear_totalTransaction_sides(side_b);

//	  clear_synchedTransaction_sides(side_a);
//	  clear_synchedTransaction_sides(side_b);

//	  clear_totalAutoTransaction_sides(side_a);
//	  clear_totalAutoTransaction_sides(side_b);

//	  clear_volumeTotaliser(side_a);
//	  clear_volumeTotaliser(side_b);

	  clear_totaliser_fram(&self);
//	  clear_totaliser_fram(side_b);

//	  clear_amountTotaliser(side_a);
//	  clear_amountTotaliser(side_b);

//	  clear_amountTotaliser_fram(side_a);
//	  clear_amountTotaliser_fram(side_b);

//	  clear_lastSale(side_a);
//	  clear_lastSale(side_b);

	  clear_lastSale_fram(&self);
//	  clear_lastSale_fram(side_b);

//	  clear_1stvolTotaliser_day(side_a);
//	  clear_1stvolTotaliser_day(side_b);

	  clear_1stvolTotaliser_day_fram(&self);
//	  clear_1stvolTotaliser_day_fram(side_b);

	  clear_configFlag_fram(&self);
//	  clear_configFlag_fram(side_b);

	  clear_configChange_trackNum_fram(&self);
//	  clear_configChange_trackNum_fram(side_b);

	  clear_config_otpSeed_time_fram(&self);
//	  clear_config_otpSeed_time_fram(side_b);

	  clear_otpSeed_session_fram(&self);
//	  clear_otpSeed_session_fram(side_b);

	  clear_online_calibFlag_fram(&self);
//	  clear_online_calibFlag_fram(side_b);


	  //============ Incident Test ==============//
	  clear_incidentNextLoc_fram(&self);
//	  clear_incidentNextLoc_fram(side_b);

	  clear_totaliserFrequent_fram(&self);
	  clear_totaliserFrequent_eeprom(&self);
//	  clear_totaliserFrequent_fram(side_b);
//	  clear_totaliserFrequent_eeprom(side_b);


//	  clear_ctSettings(side_a);
//	  clear_ctSettings(side_b);

//	  clear_calibrationPulser(side_a);
//	  clear_calibrationPulser(side_b);

//	  clear_sessionId(side_a);
//	  clear_sessionId(side_b);

//	  clear_amountSend(side_a);
//	  clear_amountSend(side_b);

//	  clear_calibrationData(side_a);
//	  clear_calibrationData(side_b);

//	  clear_configFlag(side_a);
//	  clear_configFlag(side_b);

//	  clear_otp(side_a);
//	  clear_otp(side_b);

//	  clear_logA();
//	  clear_logB();

//	  HAL_GPIO_WritePin(nLed_GPIO_Port, nLed_Pin, GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);

//	  W25qxx_EraseChip();


	 //==============================================
	 //    This step is to compose the settings.
	 //==============================================
//	 make_settings(side_a);
//	 make_settings(side_b);
//
//	 save_settings_fram();
//	 save_volumeTotaliser_fram(side_a); //side_a
//	 save_volumeTotaliser_fram(side_b);
	 self->flash_info.current_loc = self->flash_begin;
	 self->flash_info.number_logs = 0;
//	 flash_infoB.current_loc = flash_beginB;
//	 flash_infoB.number_logs = 0;
	 FRAM_Write(self->flash_sto_fram, &self->flash_info, sizeof(self->flash_info));
//	 FRAM_Write(flash_stoB_fram, &flash_infoB, sizeof(flash_infoB));
	 // ===========================================================================


	  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);

	// ===========================================================================


	  // ===========================================================================
		 //==============================================
		 //    This step is to compose the settings.
		 //==============================================
	 make_settings(&self);
//	 make_settings(side_b);

	 self->settings_stream1.noz_addr = 0x01;
	 self->settings_stream1[1].noz_addr = 0x02;

	 self->settings_stream1.pi_real = 805.75;  //797.15;   //798.1;  //407.3;   //399.25;   //798.35;
	 self->settings_stream1.pi_cal = 786.10;   //797.15;  //767.40;  //391.64;   //383.89;  //760.33;
	 self->settings_stream1[1].pi_real = 800.00;  //799.8;    //799.25;  //399.25;   //798.35;
	 self->settings_stream1[1].pi_cal = 780.88;   //799.8;   //768.51;   //383.89;  //760.33;

//		Pulser_count1 = 15962 20|0.8|0.0
//		Pulser_count2 = 15985 20|0.8|0.0

//4650
	 self->vol_real = 20;
//	 vol_real2 = 20;
	 self->vol_calibrated = 20.5;  //20.8;   //21.0;
//	 vol_calibrated2 = 20.5;  //20.8;
	 self->vol_effective = 20.5;   //20.8;    //21.0;
//	 vol_effective2 = 20.5;   //20.8;

	 self->calib_pulser =  (self->settings_stream1.pi_cal * self->vol_calibrated);
//	 calib_pulser2 =  (self->settings_stream1[1].pi_cal * vol_calibrated2);

	 config_rx_parse(&disp);
	 pumpType_configure_fram(&disp);


		// ===========================================================================


		//********************  INITIALIZATIONS ***********************//

		//----------------------//
	 firstTotalizer_day();
	//----------------------//

//		 save_settings();
	 save_settings_fram();

//		 save_ctSettings(side_a);
//		 save_ctSettings(side_b);
//		 save_calibrationPulser(side_a);
//		 save_calibrationPulser(side_b);

	 save_ctSettings_fram(side_a);
	 save_ctSettings_fram(side_b);
	 save_calibrationPulser_fram(side_a);
	 save_calibrationPulser_fram(side_b);

//	 calibration_flag1 = UNCALIBRATED;
	 calibration_flag1 = CALIBRATED;
//		 save_calibrationFlag(side_a);
	 save_calibrationFlag_fram(side_a);

//	 calibration_flag2 = UNCALIBRATED;
	 calibration_flag2 = CALIBRATED;
//		 save_calibrationFlag(side_b);
	 save_calibrationFlag_fram(side_b);

//		 calib_pulser1 = 0;
//		 calib_pulser2 = 0;
//		 retrieve_calibrationPulser_fram(side_a);
//		 retrieve_calibrationPulser_fram(side_b);

	//	 save_volumeTotaliser(side_a); //side_a
	//	 save_volumeTotaliser(side_b);
	//	 flash_infoA.current_loc = 0;
	//	 flash_infoA.number_logs = 0;
	//	 EEPROM_Write(flash_info_sto, flash_stoA, &flash_infoA, sizeof(flash_infoA));
	//	 EEPROM_Write(flash_info_sto, flash_stoA, &flash_infoB, sizeof(flash_infoA));
	 // ===========================================================================


//	  eeprom_clearToken();

//	  HAL_Delay(1000);

	  config_mode = 0;

	  while(true)
	  {
		 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
//		 HAL_GPIO_TogglePin(nLed_GPIO_Port, nLed_Pin);  // Toggle LED
		 HAL_Delay(100);
		 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
//		 HAL_GPIO_TogglePin(nLed_GPIO_Port, nLed_Pin);  // Toggle LED
		 HAL_Delay(4000);
	  }    //loop idly

//par_config:
//    if(    HAL_UART_Receive(&huart1, &rcv,39, 5000)  == HAL_OK )
//    {
//    	// received the characters
//    	// search for the keyboard keyword
////    	 int * ptr;
////    	 ptr = (char*)strstr(&rcv,"BLSKY22");
////    	 int y = 0;
//
//
//    	// after every thing save the settings
//    	// save_settings();
//
//    }
//    else
//    	goto par_config;

//    goto par_config;

  }
//  else if( HAL_GPIO_ReadPin(settings2_GPIO_Port, settings2_Pin) == 1 )
//  {
//	EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000011);
//	settings[0].passwd1 = 0000;
//	settings[0].passwd2 = 0000;
//	settings[0].passwd1 = 0000;
//	settings[0].passwd2 = 0000;
//	HAL_Delay(1000);
//  }
  else
  {
	  // continue with boot
	  int t = 0;

  }

//  check the input_pin.

skip:

 led_pin_out();


 ///=========================================================
 //                     eeprom test code.
////=========================================================
// W25qxx_EraseBlock(1);

/* while(1)
 {
	   current_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);
 } */

goto skip_test;
for(int i = 0;i<40;i++)
{
	// W25qxx_WriteByte(i+40, i);
}

//EEPROM_Read(0, 0, datar1, 50);

W25qxx_ReadBytes(datar1,0,50);

W25qxx_ReadBytes(datar1,30,50);

while(1) {}
//skip_test:

while(1)
{
	drive_totaliser1(ACTIVATE);
	printDisp_c("0",1,0,4,LT,CLEAR);
	HAL_Delay(2000);
	drive_totaliser1(DEACTIVATE);
	printDisp_c("1",1,0,4,LT,CLEAR);
	HAL_Delay(2000);
}

while(1)
{
	rt:
		drive_totaliser1(ACTIVATE);
		HAL_Delay(2000);
		drive_totaliser1(DEACTIVATE);
		HAL_Delay(2000);

		goto rt;


//    if( drivereadtotaliser1_state() == 0 )   //;readpwr() == 0)
//    {
//    	printDisp_c("0",1,0,4,LT,CLEAR);
//
////    	printDisp_c("0",1,0,4,LT,CLEAR); //printDisp_c("nnetrics",2,0,8,LT,CLEAR);
////    	HAL_Delay(2000);
////    	printDisp_c("Bye...",1,0,8,LT,CLEAR);
////    	HAL_Delay(3000);
////    	displayandkeypad_power(DEACTIVATE);
////    	while(1){}
//    }
//    else
//    {
//    	printDisp_c("1",1,0,4,LT,CLEAR);
//    }
  //========================================
  //  displayandkeypad_power(ACTIVATE);   //latch power...

    float batt_val = battery_read();
    printDisp_i(batt_val,2,0,8,LT,CLEAR);

    HAL_Delay(200);
}
skip_test:

//extern char dma_result_buffer[100];
//
//extern volatile uint16_t adc_dma_result[2];
//// This variable calculate the array length.
//// In our case, array size in 3
//extern int adc_channel_count;
//// This flag will help to detect
//// the DMA conversion completed or not
//extern uint8_t adc_conv_complete_flag;
//while(1)
//{
//		// when adc_conv_complete_flag is set to 1,
//		// that means DMA conversion is completed
//	   if(adc_conv_complete_flag == 1)
//	   {
//			  // this snprintf function helps to convert the adc_dma_result array
//			  // into string and store in dma_result_buffer character array
//			  snprintf(dma_result_buffer, 100, "CH_1: %d, CH_2: %d\r\n", adc_dma_result[0], adc_dma_result[1]);
//			  // we just send the dma_result_buffer character array with ADC values
//			  // to our computer serial terminal software (Tera Term) using UART peripheral of STM32
////			  HAL_UART_Transmit(&huart2, (uint8_t *) dma_result_buffer, sizeof(dma_result_buffer), HAL_MAX_DELAY);
//			  // adc_conv_complete_flag variable is set to 0, because,
//		         // we alert this flag variable for new DMA conversion completion
//			 adc_conv_complete_flag = 0;
//		        // delay for 500 Milliseconds
//			 HAL_Delay(500);
//		}
//}

//while(1)
//{
////	sConfig.Rank = ADC_REGULAR_RANK_1;
////	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
////	sConfig.Channel = ADC_CHANNEL_9;
////	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
////	HAL_ADC_Start(&hadc1);
////	HAL_ADC_PollForConversion(&hadc1,1000);
////	readValue1 = HAL_ADC_GetValue(&hadc1);
////	HAL_ADC_Stop(&hadc1);
////	sConfig.Channel = ADC_CHANNEL_8;
////	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
////	HAL_ADC_Start(&hadc1);
////	HAL_ADC_PollForConversion(&hadc1,1000);
////	readValue2 = HAL_ADC_GetValue(&hadc1);
////	HAL_ADC_Stop(&hadc1);
////	sConfig.Channel = ADC_CHANNEL_7;
////	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
////	HAL_ADC_Start(&hadc1);
////	HAL_ADC_PollForConversion(&hadc1,1000);
////	readValue3 = HAL_ADC_GetValue(&hadc1);
////	HAL_ADC_Stop(&hadc1);
////
////	HAL_Delay(10);
//
//	float batt_val = battery_read();
//	printDisp_i(batt_val, 2, 0, 8, LT, CLEAR);
//
//	HAL_Delay(100);
//	HAL_GPIO_WritePin(batt_check_GPIO_Port, batt_check_Pin, GPIO_PIN_SET);;
////	batt_val = battery_sense();
//	HAL_GPIO_WritePin(batt_check_GPIO_Port, batt_check_Pin, GPIO_PIN_RESET);
//	batt_val = battery_sense();
//	printDisp_i(batt_val, 2, 0, 8, LT, CLEAR);
//
//	HAL_Delay(200);
//}


/*
  for (int i=0; i<2; i++) //512
   {
 	  EEPROM_PageErase(i);
   }

 //  EEPROM_Write(0, 0, dataw1, strlen((char *)dataw1));
  *
   while(1) {}  */
/*
   //EEPROM_Write(3, 0, dataw1, strlen((char *)dataw1));

   EEPROM_Write(0, 50, dataw2, strlen((char *)dataw2));

   EEPROM_Write_NUM (6, 0, dataw3);

   EEPROM_Read(3, 0, datar1, 50);

  // EEPROM_Read(5, 15, datar2, 50);

   datar3 = EEPROM_Read_NUM (6, 0);   */

//	while(1)
//	{
//
//		FRAM_Write_NUM (0, 0, 234);
//
//		 HAL_Delay(1000);
//
//		 float fram_read = FRAM_Read_NUM (0, 0);
//	}



  //==============================================
  //    This step is to compose the settings.
  //==============================================
//     make_settings(side_a);
//     make_settings(side_b);
//
//     save_settings();
//     save_volumeTotaliser(side_a); //side_a
//     save_volumeTotaliser(side_b);
//     flash_infoA.current_loc = 0;
//     flash_infoA.number_logs = 0;
//     EEPROM_Write(flash_info_sto, flash_stoA, &flash_infoA, sizeof(flash_infoA));
//     EEPROM_Write(flash_info_sto, flash_stoA, &flash_infoB, sizeof(flash_infoA));
// // ===========================================================================

////    char testChar0[9] = {0};
////	char testChar1[9] = {0};

//		sz = sizeof(testChar0);
//
//		 strncpy(testChar0, "Testing", sizeof(testChar0) );
//		 EEPROM_Write(700, 0, &testChar0, sz);
//
//		 memset(testChar1, '\0', sizeof(testChar1) );
//
//		 EEPROM_Read(700, 0, &testChar1, sz);
//		 strncpy(self->attendant.session_id, testChar1, sizeof(self->attendant.session_id) );

//	settings[0].pi_ = 407.3;
//	save_settings();


//    retrieve_settings();         //read pump settings from eeprom.


    // ===========================================================================


    //********************  PLACE-HOLDERS ***********************//

	  //----------------------//
      //firstTotalizer_day();
      //----------------------//

//	  uint16_t firstTime = 0;
//
//	  uint16_t flash_beginA_page = 0,            //0x0000
//			   flash_beginB_page = 16384;        //0x4000
//
//	  EEPROM_Write(totalTranxA_loc, totalTranxA1_loc, &firstTime, sizeof(firstTime));
//	  EEPROM_Write(totalTranxB_loc, totalTranxB1_loc, &firstTime, sizeof(firstTime));
//	  EEPROM_Write(synchedTranxA_loc, synchedTranxA1_loc, &firstTime, sizeof(firstTime));
//	  EEPROM_Write(synchedTranxB_loc, synchedTranxB1_loc, &firstTime, sizeof(firstTime));
//
//	  EEPROM_Write(lastSynchedFlashA_loc, 0, &flash_beginA_page, sizeof(flash_beginA_page));
//	  EEPROM_Write(lastSynchedFlashB_loc, 0, &flash_beginB_page, sizeof(flash_beginB_page));
//
//	  clear_volumeTotaliser(side_a);
//	  clear_volumeTotaliser(side_b);
//
//	  clear_amountTotaliser(side_a);
//	  clear_amountTotaliser(side_b);
//
//	  clear_lastSale(side_a);
//	  clear_lastSale(side_b);
//
//      clear_1stvolTotaliser_day(side_a);
//   	  clear_1stvolTotaliser_day(side_b);
//
//   	  clear_ctSettings(side_a);
//   	  clear_ctSettings(side_b);
//
//   	  clear_calibrationPulser(side_a);
//   	  clear_calibrationPulser(side_b);
//
//		clear_ctTimedFlag(side_b);
//		clear_ctTimedFlag(side_b);
//
//
//	  clear_logA();
//	  clear_logB();
//
//    W25qxx_EraseChip();

	// ===========================================================================


//	pumpType_write();
	pumpType_write_fram();
    pumpType_parse();

    firstTotalizerDay_write();

//	settings[0].totalizer_day = 14;
//	settings[1].totalizer_day = 14;

//    clear_logA();
//    clear_logB();

//    if( (HAL_GPIO_ReadPin(settings1_GPIO_Port, settings1_Pin) == 1 ) || ( HAL_GPIO_ReadPin(settings2_GPIO_Port, settings2_Pin) == 1) )
//    {
////    	settings[0].passwd1 = 0000;
////    	settings[0].passwd2 = 0000;
////    	settings[1].passwd1 = 0000;
////    	settings[1].passwd2 = 0000;
//    	settings[0].max_amt_ = 1000;
//    	settings[1].max_amt_ = 1000;
////    	settings[0].pi_ = 180;
////    	settings[0].pi_c = 180;
//    	make_settings(side_a);
//    	make_settings(side_b);
//    }

//	settings[0].pump_type_ = bluesky;
//    settings[1].pump_type_ = bluesky;

//    settings[0].pump_type_ = 0b00000011;
//	settings[1].pump_type_ = 0b00000011;
//	disp_type1 = settings[0].pump_type_;
//	disp_type2 = settings[1].pump_type_;

    //	 displaytype display_;
    //	 keypadtype keypad_;
    //	 productname product_;
    //	 nozzleid noz_id;
//      }pump_settings;

//    settings[0].display_
//	settings[0].keypad_
//	settings[0].product_
//	settings[0].noz_id;


    self->settings_stream1.noz_addr = 0x01;
	self->settings_stream1[1].noz_addr = 0x02;

//	self->settings_stream2[0].valve_salesStart = 0.00; // 0.15;
//	self->settings_stream2[0].valve_salesEnd = 0.00;   //0.36;

//	self->settings_stream2[0].startUp_suppressVol = 0.12;
//	self->settings_stream2[1].startUp_suppressVol = 0.12;

//    self->settings_stream1.mode = MANUAL_MODE;    //AUTO_MODE;   //MANUAL_MODE;
//    self->settings_stream1[1].mode = MANUAL_MODE;      //AUTO_MODE; //MANUAL_MODE;
//
//    self->settings_stream1.noz = nooverride;  //nooveride
//    self->settings_stream1.noz_override = override;  //nooveride
//    self->settings_stream2[0].keypress_tone = No;   //Yes;

//    self->settings_stream2[0].calibration_measureCan = 2;

//    self->settings_stream1.keypad__ = BLSKY22;   //BLSKY22
//    self->settings_stream1.keypad__ = LAFNG18_K_V2;  //LAFNG18_K;   //BLSKY22;    //LAFNG18_K;

//
//    self->settings_stream1[1].mode = MANUAL_MODE;  //AUTO;   //MANUAL;
//    self->settings_stream1[1].mode = AUTO_MODE;    //AUTO_MODE;
//
//    self->settings_stream1[1].noz = nooverride;  //nooveride
////	self->settings_stream1[1].noz = override;  //nooveride
//	self->settings_stream1[1].keypad__ = BLSKY22;   //BLSKY22
//    self->settings_stream1[1].keypad__ = LAFNG18_K;   //BLSKY22;    //LAFNG18_K;
//    self->settings_stream1[1].keypad__ = LAFNG18_K_V2;  //LAFNG18_K;   //BLSKY22;    //LAFNG18_K;

//    self->settings_stream1.pi_cal = 797.150024;

//    self->settings_stream1.noz_addr = 0x01;
//    self->settings_stream1[1].noz_addr = 0x02;

//    self->settings_stream2[1].keypress_tone = Yes;   //No;   //Yes;
//    self->settings_stream1.display_format = PL;
//    self->settings_stream1[1].display_format = PL;
//    self->settings_stream1.def_t = P;
//    self->settings_stream1[1].def_t = P;



    // ===========================================================================


    //********************  MAKE SETTINGS ***********************//
//		settings[0].max_amt_ = 1000;
//		settings[1].max_amt_ = 1000;
//		make_settings(side_a);
//		make_settings(side_b);
    // ===========================================================================

//    settings[0].price_ = 121;

    load_settings(side_a); //load the settings into the internal variables.
    load_settings(side_b);

//    int intSize;
//    intSize = sizeof(intSize);

//    pumpName_parse();
    pumpName_parse_fram();
//    strcpy(self->pumpName[0].pump_name, "P17");
//    strcpy(self->pumpName[1].pump_name, "P18");

//    retrieve_volumeTotaliser(side_a);
//    retrieve_volumeTotaliser(side_b);
//
//    retrieve_amountTotaliser(side_a);
//    retrieve_amountTotaliser(side_b);

//    retrieve_lastSale(side_a);
//    retrieve_lastSale(side_b);

//    clear_incidentNextLoc_fram(side_a);
//    clear_incidentNextLoc_fram(side_b);
//
//    clear_totaliserFrequent_fram(side_a);
//    clear_totaliserFrequent_eeprom(side_a);
//    clear_totaliserFrequent_fram(side_b);
//    clear_totaliserFrequent_eeprom(side_b);
//
//    clear_totaliser_fram(side_a);
//    clear_totaliser_eeprom(side_a);
//    clear_totaliser_fram(side_b);
//    clear_totaliser_eeprom(side_b);

//    totaliser_vol1c = 4795.750;  //161.69;
//    totaliser_vol1 = 4795.750;
//    totaliser_amt1c = 0.00;
//   	totaliser_amt1 = 0.00;
//   	save_totaliser_fram(side_a);
//   	save_totaliser_eeprom(side_a);
//
//    totaliser_vol2c = 16106.770;  //161.69;
//    totaliser_vol2 = 16106.770;
//    totaliser_amt2c = 0.00;
//	totaliser_amt2 = 0.00;
//	save_totaliser_fram(side_b);
//	save_totaliser_eeprom(side_b);



    while(retrieve_lastSale_fram(side_a) != OK)   //If it fails, retry 5X
       {
   		static uint8_t try = 0;
   		if(try++ >= 5)
   		{
   			retrieve_lastSale_eeprom(side_a);
   			try = 0;
   			break;
   		}
       }
       while(retrieve_lastSale_fram(side_b) != OK)   //If it fails, retry 5X
       {
       	static uint8_t try = 0;
       	if(try++ >= 5)
       	{
       		retrieve_lastSale_eeprom(side_b);
       		try = 0;
       		break;
       	}
       }



//    retrieve_amountTotaliser_fram(side_a);
//    retrieve_amountTotaliser_fram(side_b);



	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
    //                                                                                                 //
	// 				                        Retrieves Totalizers, 									   //
    //	 But, at the same checks to reconcile Totalizers, in case there's reset during an active Sale  //
	//																						           //
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//


    while(retrieve_totaliser_fram(side_a) != OK)   //If it fails, retry 5X
    {
    	static uint8_t try = 0;
    	if(try++ >= 5)
    	{
    		while(retrieve_totaliser_eeprom(side_a) != OK)
			{
				if(try++ >= 10)
				{
					clear_totaliser_fram(side_a);
					clear_totaliser_eeprom(side_a);
					try = 0;
					break;
				}

			}
    		try = 0;
    		break;
    	}
    }

    while(retrieve_totaliser_fram(side_b) != OK)   //If it fails, retry 5X
    {
		static uint8_t try = 0;
		if(try++ >= 5)
		{
			while(retrieve_totaliser_eeprom(side_b) != OK)
			{
				if(try++ >= 10)
				{
					clear_totaliser_fram(side_b);
					clear_totaliser_eeprom(side_b);
					try = 0;
					break;
				}

			}
			try = 0;
			break;
		}
    }

   while(retrieve_totaliserFrequent_fram(side_a) != OK)   //If it fails, retry 5X
   {
	static uint8_t try = 0;
	if(try++ >= 5)
	{
		while(retrieve_totaliserFrequent_eeprom(side_a) != OK)
		{
			if(try++ >= 10)
			{
				clear_totaliserFrequent_fram(side_a);
				clear_totaliserFrequent_eeprom(side_a);
				try = 0;
				break;
			}

		}
		try = 0;
		break;
	}
   }

   while(retrieve_totaliserFrequent_fram(side_b) != OK)   //If it fails, retry 5X
   {
	static uint8_t try = 0;
	if(try++ >= 5)
	{
		while(retrieve_totaliserFrequent_eeprom(side_b) != OK)
		{
			if(try++ >= 10)
			{
				clear_totaliserFrequent_fram(side_b);
				clear_totaliserFrequent_eeprom(side_b);
				try = 0;
				break;
			}

		}
		try = 0;
		break;
	}
   }


    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//



    /////////////////////////////////////////////////////////////////////////////////////////
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

     // End measurement
     end_cycles = DWT->CYCCNT;

     // Calculate elapsed cycles and time
     elapsed_cycles = end_cycles - start_cycles;
     time_us = (float)elapsed_cycles / (SystemCoreClock / 1000000.0f); // Convert to µs

     //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
     /////////////////////////////////////////////////////////////////////////////////////////





    /*=========================================================================*/
	//-------------------------------------------------------------------------//

	#if defined(DEBUG_AUTO_SALE_TEST)

		memset(strr__, '/0', sizeof(strr__));

		sprintf(strr__,
					"\n\nInitial Totalizer [Side-A] : %0.2f ",
					totaliser_vol1c);

		HAL_UART_Transmit(&huart3, strr__, strlen((char*)strr__), HAL_MAX_DELAY);

		HAL_Delay(1);

		memset(strr__, '/0', sizeof(strr__));

		sprintf(strr__,
					"\n\nInitial Totalizer [Side-B] : %0.2f\n\n\n\n\n\n",
					totaliser_vol2c);

		HAL_UART_Transmit(&huart3, strr__, strlen((char*)strr__), HAL_MAX_DELAY);

		HAL_Delay(1);

	 #endif    //#if DEBUG_AUTO_SALE_TEST

	//UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU//



	//--------------------------------------------------------------------------------------------//
	//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV//

//    retrieve_totaliser_fram(side_a);
//    totaliser_vol1c = 0;
//    totaliser_vol2c = 0;
//    totaliser_vol1 = 0;
//    totaliser_vol2 = 0;
//    retrieve_totaliser_eeprom(side_a);
//    retrieve_totaliser_fram(side_b);
//    totaliser_vol1c = 0;
//    totaliser_vol2c = 0;
//    totaliser_vol1 = 0;
//    totaliser_vol2 = 0;
//    retrieve_totaliser_eeprom(side_b);
//    uint8_t sz = sizeof(totaliser_storeA);
//
    //HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH//
    //==============================================//

//    /*--------------------------------------------------------*/
//		clear_totaliser_fram(side_a);
//		clear_totaliser_fram(side_b);
//    /*--------------------------------------------------------*/

//    totaliser_vol1c = 1000000.00;
//    totaliser_vol1 = 1000000.00;

//    totaliser_vol1c = 21474836.00;
//    totaliser_vol1 = 21474836.00;
//    totaliser_vol1c = 3828.17;  //161.69;
//    totaliser_vol1 = 3828.17;

//    totaliser_vol1c = 0.0;  //161.69;
//    totaliser_vol1 = 0.0;
//    totaliser_amt1c = 0.00;
//	totaliser_amt1 = 0.00;
//	save_totaliser_fram(side_a);
//	save_totaliser_eeprom(side_a);
//
////	totaliser_vol2c = 14221.48;   //8060.52;
////	totaliser_vol2 = 14221.48;
//
//	totaliser_vol2c = 18882.39;   //(totaliser_vol2c - 18.22);   //0.0;  //161.69;
//	totaliser_vol2 = 18882.39; //(totaliser_vol2 - 18.22);   //0.0;
////	totaliser_amt2c = 0.00;
////    totaliser_amt2 = 0.00;
//	save_totaliser_fram(side_b);
//	save_totaliser_eeprom(side_b);


	//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV//
	//--------------------------------------------------------------------------------------------//

    if(fillingresume_flag1_1 == 0)
    {
		printDisp_c("Fuel", 1, 2, 4, LT, CLEAR);
		printDisp_c("NNetrics", 2, 0, 8, LT, CLEAR);
	}

    if(fillingresume_flag2_1 == 0)
    {
		printDisp_c2("Fuel", 1, 2, 4, LT, CLEAR);
		printDisp_c2("NNetrics", 2, 0, 8, LT, CLEAR);
    }

	if( (fillingresume_flag1_1 == 0) && (fillingresume_flag2_1 == 0) )
    {
		HAL_Delay(2500);
    }

	reset_timer(15);
	//start_timer(15);

   //======= initialize ========
   self->eNextState = idle_State;
   eNextState2 = idle_State;


//    retrieve_totalTransaction_sides(side_a);
//    retrieve_totalTransaction_sides(side_b);
//
//    retrieve_synchedTransaction_sides(side_a);
//    retrieve_synchedTransaction_sides(side_b);
//
//    retrieve_totalAutoTransaction_sides(side_a);
//    retrieve_totalAutoTransaction_sides(side_b);
//
//    retrieve_synchedAutoTransaction_sides(side_a);
//    retrieve_synchedAutoTransaction_sides(side_b);

//    clear_1stvolTotaliser_day(side_a);
//    clear_1stvolTotaliser_day(side_b);
//    retrieve_1stVolTotaliser_day(side_a);
//    retrieve_1stVolTotaliser_day(side_b);

    retrieve_1stVolTotaliser_day_fram(side_a);
    retrieve_1stVolTotaliser_day_fram(side_b);

//    retrieve_ctSettings(side_a);
//    retrieve_ctSettings(side_b);

    retrieve_ctSettings_fram(side_a);
    retrieve_ctSettings_fram(side_b);
    retrieve_online_calibFlag_fram(side_a);
    retrieve_online_calibFlag_fram(side_b);

    //JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ//
    //============================================//
    //xxxxxxxxxx FETCHES PULSER-COUNT xxxxxxxxxxxx//
    //============================================//
    //LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL//

//    retrieve_calibrationPulser(side_a);
//    retrieve_calibrationPulser(side_b);

      retrieve_calibrationPulser_fram(side_a);
      retrieve_calibrationPulser_fram(side_b);

    //============================================//
	//JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ//


//    retrieve_ctTimedSettings(side_a);
//    retrieve_ctTimedSettings(side_b);

    retrieve_ctTimedSettings_fram(side_a);
    retrieve_ctTimedSettings_fram(side_b);

//    retrieve_ctTimedFlag(side_a);
//    retrieve_ctTimedFlag(side_b);

    retrieve_ctTimedFlag_fram(side_a);
    retrieve_ctTimedFlag_fram(side_b);

//    clear_sessionId(side_a);
//    clear_sessionId(side_b);

//    retrieve_sessionId(side_a);
//    retrieve_sessionId(side_b);

//    clear_volumeTotaliser_startShift(side_a);
//    clear_volumeTotaliser_startShift(side_b);
//    retrieve_volumeTotaliser_startShift(side_a);
//    retrieve_volumeTotaliser_startShift(side_b);

//    retrieve_amountSend(side_a);
//    retrieve_amountSend(side_b);

//    retrieve_calibrationData(side_a);
//    retrieve_calibrationData(side_b);

//    retrieve_calibrationData_fram(side_a);
//    retrieve_calibrationData_fram(side_b);

//    retrieve_configFlag(side_a);
//    retrieve_configFlag(side_b);

    retrieve_configFlag_fram(side_a);
    retrieve_configFlag_fram(side_b);

//    startShiftTotaliser_vol1c = 500;
//    startShiftTotaliser_amt1c = 500;

//	 calibration_flag1 = CALIBRATED;
//	 save_calibrationFlag(side_a);
//
//	 calibration_flag2 = CALIBRATED;
//	 save_calibrationFlag(side_b);


//    clear_configFlag(side_a);
//    clear_otp(side_a);

//	settings[0].max_amt_ = 1000;
//	settings[1].max_amt_ = 1000;
////	settings[0].pi_ = 180;
////	settings[0].pi_c = 180;
//	make_settings(side_a);
//	make_settings(side_b);

//===============================================//
//xxxxx Niyi's Calibration on Office's Pump xxxxx//
//===============================================//
//    Pulser_count1 = 15962 20|0.8|0.0
//	  Pulser_count2 = 15985 20|0.8|0.0
//===============================================//


//    strcpy(self->settings_stream1.product_, "DPK");
//    strcpy(self->settings_stream1[1].product_, "DPK");


//    pulser_benchMark1 = 15985;   //15987;
//    pulser_benchMark2 = 15962;   //15987;

    //===============================================//
    //xxxxx Tinpet, Bale, Ajegunle  xxxxx//
    //===============================================//
    pulser_benchMark1 = 7903;   //15987;
    pulser_benchMark2 = 7903;   //15987;
    //===============================================//

//    FRAM_Write_NUM (0, 0, 234);
//
//    HAL_Delay(1000);
//
//    float fram_read = FRAM_Read_NUM (0, 0);
//
//    EEPROM_Write_NUM (900, 0, 234);
//
//    HAL_Delay(1000);
//
//    fram_read = EEPROM_Read_NUM (900, 0);



//    retrieve_totalEvents();
//    retrieve_synchedEvents();

    ep0_save.boot_time = RtcToInt(2019);

    slowFlow_startThreshold1 = (fast_flow_threshold1 * self->settings_stream2[0].valve_salesStart);
    slowFlow_endThreshold1 = (fast_flow_threshold1 * self->settings_stream2[0].valve_salesEnd);
	slowFlow_startThreshold2 = (fast_flow_threshold2 * self->settings_stream2[1].valve_salesStart);
	slowFlow_endThreshold2 = (fast_flow_threshold2 * self->settings_stream2[1].valve_salesEnd);

//	settings[1].noz = overide;   //nooveride;
//	overide_2 = settings[1].noz;
//	settings[0].pulser_offset = 0;
//	settings[1].pulser_offset = 0;

//    calib_pulser1 = 15800;

//	self->settings_stream2[0].pulser_type_ = non_quadrature;   //quadrature;
//	self->settings_stream2[1].pulser_type_ = non_quadrature;   //quadrature;

//	self->settings_stream1.noz_override = nooverride;
//	self->settings_stream1[1].noz_override = nooverride;

    day = DS1307_GetDate();

	if(self->settings_stream2[0].totalizer_day != day)
	{
//		save_1stVolTotaliser_day(side_a);
//		save_1stVolTotaliser_day(side_b);

		save_1stVolTotaliser_day_fram(side_a);
		save_1stVolTotaliser_day_fram(side_b);
	}

//    calib_pulser1 = 15800;

//    vol_real1 = 20;
//    vol_real2 = 20;
//    vol_calibrated1 = 21.0;
//    vol_calibrated2 = 21.0;
//    vol_effective1 = 21.0;
//    vol_effective2 = 21.0;

//	day = DS1307_GetDate();
//	if(settings[0].totalizer_day != day)
//	{
//		ep0_save.pump[0].tolalizer_first = totaliser_vol1c;
//		ep0_save.pump[0].totalizerFirst_timestamp = RtcToInt(2019);
//		ep0_save.pump[1].tolalizer_first = totaliser_vol2c;
//		ep0_save.pump[1].totalizerFirst_timestamp = ep0_save.pump[0].totalizerFirst_timestamp;

//		ep5_save.firstTotalizer[0].totalizer = totaliser_vol1c;
//		ep5_save.firstTotalizer[1].totalizer = totaliser_vol2c;
//		ep5_save.firstTotalizer[0].timestamp = RtcToInt(2019);
//		ep5_save.firstTotalizer[1].timestamp = ep5_save.firstTotalizer[0].timestamp;
//		//============================================//
//		// 				EP0 ROUTINE SENDING			  //
//		//============================================//
//
//		if (HAL_GPIO_ReadPin(network_connected_GPIO_Port, network_connected_Pin) == 1 )
//		{
//			ep_send(ep0);
//
////			ep0_sent = 1;
//		}
//
//		//============================================//

//		settings[0].totalizer_day = day;
//		EEPROM_Write_NUM (totalizerDay_loc, 0, settings[0].totalizer_day);
//	}

//	ep_send(ep0);


    state_ini();
    state_ini2();

    //-------------------------------------------------------------------------
    //---                external flash related.
   W25qxx_Init();   //initialise external flash memory.
   extern int data_size;
   data_size = w25qxx.PageSize;

   rre:
//	   write_flash_state_Handler();
//	   EEPROM_Read(flash_info_sto, flash_stoA, &flash_infoA, sizeof(flash_infoA));
//	goto rre;


  // EEPROM_Read(flash_info_sto,flash_stoA, datar1, 50);
  // EEPROM_Read(flash_info_sto, flash_stoA, &flash_infoA, sizeof(flash_infoA));

//   flash_info_read();
   flash_info_read_fram();
   //--------------------------------------------------------------------------
    //W25qxx_EraseChip();
 //   W25qxx_WritePage(dataw1,0, 0, sizeof(dataw1));
//rre:
//    write_flash_state_Handler();
// goto rre;
/*
    W25qxx_ReadPage(datar1, 0, 0, sizeof(dataw2));
    printDisp_c("done",1,2,4,LT,CLEAR);

      printf("done !");
       while(1)
		{
			 printf("done !");
			 HAL_Delay(1000);
		}

		*/

//	    char str__[8]= {0};
//		snprintf(str__, sizeof(str_), "%.2f", litre_price);
//		send_line3(str__);

//   pump_status_ = STATUS_PNP;
//   pump_status_2 = STATUS_PNP;

//   while (1)
//   {
//
//	    uint32_t pg = 0;
//
//	    uint8_t number1 = 0;
////	    HAL_UART_Transmit (&huart2, "Hello", 5, 1000);
////   flash_beginB => 0x400000 --> 4,194,304 pg16,384
//   //
////flash_beginB;  //flash_endB => 0x7FFFFF --> 8,388,607 pg32767.996
//		for(int i = FLASH_BEGINPAGE_SIDEB; ( (i >= FLASH_BEGINPAGE_SIDEB) && (i <= FLASH_ENDPAGE_SIDEB) ); i+=256)
////		{
////		for(int i = 4194816; ( (i <= FLASH_ENDPAGE_SIDEB) ); i+=256)
//		{
//			pg = i/w25qxx.PageSize;
//			W25qxx_ReadPage(&log_b_new,  pg, 0, sizeof(log_b_new) );
//
//			if( (isnan(log_b_new.pr_)) || (log_b_new.litre_price_ < 100) )
//			{
//				;
//			}
//			else
//			{
////				if(number1++ == 21)
////				{
////					number1 = 0;
////				}
//				sendStructData(); // Send data
//				HAL_Delay(5);
//			}
//		}
//   	 }
 }

void led_pin_out(void)
 {
   GPIO_InitTypeDef GPIO_InitStruct = {0};

   /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOC_CLK_ENABLE();

   /*Configure GPIO pin Output Level */
   HAL_GPIO_WritePin(nLed_GPIO_Port, nLed_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : nLed_Pin */
   GPIO_InitStruct.Pin = nLed_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(nLed_GPIO_Port, &GPIO_InitStruct);

 }

//============================================================
 void house_keeping()
 {
 	         Multiplex(0, 0);

 			 self->keynew = debounceKey(&self);
 			 if (self->keynew != 0)
 			 {
 				 self->keypress_flag = 1;  //indicate that a new press was detected.
			 }
 }


void house_keeping2()
{
    	Multiplex2(0, 0);

//			  keynew2 = keypad_lcd2(0, key_lcd2);  //write lcd and read keypad.
//			  if ( (keyold2 == 0)&&(keyold2 != keynew2) )  //send key only if new key is pressed
//			  {
//				 //send_keyboard();
//				 keypress_flag2 = 1;  //indicate that a new press was detected.
//			  }
//
//			  keyold2 = keynew2;

			  keynew2 = debounceKey2();
			  if (keynew2 != 0)
			  {
				 keypress_flag2 = 1;  //indicate that a new press was detected.
			  }
}
//==============================================================

void run()
{
/*
 *	//fast_flow1();
	//drive_pump1(ACTIVATE);
//	drive_solenoid1(ACTIVATE);
//		int drv = 0;
//	while(1)
	{
		drive_solenoid1(drv);
        drive_pump1(drv);
        drv++;
        if(drv > 1) drv = 0;
        HAL_Delay(1000);
	}
*/
		// count__ = __HAL_TIM_GET_COUNTER(&htim5);
		/*
			   while(1)
			   {
					   HAL_Delay(100);
					   get_time();
					   send_line1(time);
					   send_line2(date);
			   }
		*/
	//	HAL_GPIO_WritePin(batt_check_GPIO_Port, batt_check_Pin, GPIO_PIN_SET);;
	////	batt_val = battery_sense();

//	t_exec5 = DWT->CYCCNT;

//	static float batt_val = 0.0;

	if(go_message == true)
	{
	//		server_rx_parse();

		timer_go = 0;

//			t_exec1 = DWT->CYCCNT;
//			t_exec7 = t_exec6 - t_exec4;

			parse_extract();

//			t_exec2 = DWT->CYCCNT;
//			t_exec3 = t_exec2 - t_exec1;

	//		server_message_found = 0;
			go_message = false;
//		}
			if (r_pumpno == self->settings_stream1.noz_addr)
			{
				if( (resp != NOREPLY) && (resp != JUNK) )
				{

//					t_exec2 = DWT->CYCCNT;
//					t_exec3 = t_exec2 - t_exec1;

					process_response1(resp);

//					checkk++;
				}
			}
			else if (r_pumpno == self->settings_stream1[1].noz_addr)
			{

				if( (resp2 != NOREPLY) && (resp2 != JUNK) )
				{
					process_response2(resp2);
				}

//				cheq3++;
			}
	}

//		t_exec6 = DWT->CYCCNT;
//		t_exec7 = t_exec6 - t_exec4;

//	HAL_GPIO_WritePin(batt_check_GPIO_Port, batt_check_Pin, GPIO_PIN_RESET);

	float batt_val_ = battery_sense();

//	if( (batteryVoltage_ready == 1) && ( (self->eLastState != pnp_State) || (eLastState2 != pnp_State) ) )
	if(batteryVoltage_ready == 1)
	{
		batt_val = batt_val_;

		#if (sense_battery == 1)

		//	if( (batt_val < 2.00) && (batt_val >= 1.95) )
		//	if( (batt_val < 1.8) && (batt_val >= 1.5) )
			if( (batt_val < 1.48) && (batt_val >= 1.40) )   //6.0V & 6.1V
			{
				// Non-blocking delay function
//				static uint32_t start_time = 0;
//				static uint8_t delay_active = 0;
//				uint32_t currentTime = HAL_GetTick(); // Get the current system tick

				if (!delay_active) {
					// Start the delay
//					start_time = currentTime;
					timer_lowBatt = 0;
					delay_active = 1;
				}
				else
				{
					// Check if the delay has expired
//					if ((currentTime - start_time) >= 2000)
					if (timer_lowBatt >= BATTERY_DELAY)
					{
						// Delay is over
						delay_active = 0;
						timer_lowBatt = 0;
						batteryStatus = LOW_BATTERY;
					}
					else
					{
						// Delay is still active
					}
				}
//				batteryStatus = LOW_BATTERY;
			}
		//	else if(batt_val < 1.5)
		//	else if(batt_val < 1.0)
			else if(batt_val < 1.40)
			{
				// Non-blocking delay function
//				static uint32_t start_time1 = 0;
//				static uint8_t delay_active1 = 0;
//				uint32_t currentTime1 = HAL_GetTick(); // Get the current system tick

				if (!delay_active1) {
					// Start the delay
//					start_time1 = currentTime1;
					timer_noBatt = 0;
					delay_active1 = 1;
				}
				else
				{
					// Check if the delay has expired
//					if ((currentTime1 - start_time1) >= 2000)
					if (timer_noBatt >= BATTERY_DELAY)
					{
						// Delay is over
						delay_active1 = 0;
						timer_noBatt = 0;
						batteryStatus = NO_BATTERY;
					}
					else
					{
						// Delay is still active
					}
				}
//				batteryStatus = NO_BATTERY;
			}
			else
			{
				batteryStatus = BATTERY_OK;
			}

		#endif     //#if (sense_battery == 1)
	}

//	if(server_message_found == 1)
//	{
////		server_rx_parse();
//		parse_extract();
//		server_message_found = 0;
//	}
//	if(card1_message_found == 1)
//	{
//		card1_rx_parse();
//		card1_message_found = 0;
//	}
//	else if(card2_message_found == 1)
//	{
//		card2_rx_parse();
//		card2_message_found = 0;
//	}

#if defined (DEV_MODE)

//	epSend_interval();   //********//

//	if ( (ep20_available1 == 1) || (ep20_available2 == 1) )
//	{
//		if (ep20_available1 == 1)
//		{
//			ep20_send(side_a);
//		}
//		else if (ep20_available2 == 1)
//		{
//			ep20_send(side_b);
//		}
//
////		ep20_available = 0;
//
//	}

#else

	//============================================//
	// 				EP's ROUTINE SENDING			  //
	//============================================//
//	if (HAL_GPIO_ReadPin(network_connected_GPIO_Port, network_connected_Pin) == 1 )
//	{
//		connected = 1;
//		epSend_interval();
//	}
//	else
//	{
//		connected = 0;
//	}
	//============================================//

#endif     //#if defined (DEV_MODE)

//	if(server_message_found == 1)
//	if(go_message == true)
//	{
////		server_rx_parse();
//
//		t_exec6 = DWT->CYCCNT;
//		t_exec7 = t_exec6 - t_exec4;
//
//		parse_extract();
////		server_message_found = 0;
//		go_message = false;
//	}
//	if( (resp != NOREPLY) && (resp != JUNK) )
//	{
//		process_response(resp);
////		resp = NOREPLY;
//		checkk++;
//	}


//	  HAL_UART_Transmit (&huart2, "Hello", 5, 1000);
//	  HAL_Delay(3000);

	if(operating_sideA)
	{
		 operating_side = side_a;
		 house_keeping();
//		 states();
		 states_1();

//		 if(self->keypress_flag == 1)
//		 {
//			 keypress_Handler();
//			 self->keypress_flag = 0;
//		 }

		 operating_sideA = false;
		 operating_sideB = true;
	}
	else if(operating_sideB)
	{
		 operating_side = side_b;
		 house_keeping2();
		 states2();

		 operating_sideA = true;
		 operating_sideB = false;
	}

////	if(server_message_found == 1)
//	if(go_message == true)
//	{
////		server_rx_parse();
//		parse_extract();
////		server_message_found = 0;
//		go_message = false;
//	}
//	else if( (resp != NOREPLY) && (resp != JUNK) )
//	{
//		process_response(resp);
////		resp = NOREPLY;
//	}
//
//
//	if(card1_message_found == 1)
//	{
//		card1_rx_parse();
//		card1_message_found = 0;
//	}
//	else if(card2_message_found == 1)
//	{
//		card2_rx_parse();
//		card2_message_found = 0;
//	}
//	else if( (pump_message_found == 1)  && (awaiting_masterResponse == 0) )
//	{
//		 pump_message_found = 0;
//		 uint8_t res = msg_parse_pump( pump_buf );
//		 awaiting_masterResponse = 1;
//		 _tt1 = 0;
//	}
//	else if( (pump_message_found == 1)  && (awaiting_masterResponse == 1) )
//	{
//		 pump_message_found = 0;
//		 awaiting_masterResponse = 0;
//
//	}
//	else if(awaiting_masterResponse == 1)
//	{
//		 if(_tt1 > 2000)
//			 send_line3("Err5 ");
//	}

//	#if sense_battery == 1
//		if( battery_read() < 1.81 )   //1.81V @ 6.4V Low_cutOff
//		{
//
//		}
//	#endif    //#if sense_battery


//	pulsercheck = oldPulser1 - newPulser;
//	pulsercheck2 = oldPulser2 - newPulser2;
//	if(pulsercheck > threshold) && (eNextState != filling_State)
//	{
//	   activatefilling_display;
//	   reading_starts;
//	}
//	if(pulsercheck2 > threshold) && (eNextState2 != filling_State)
//	{
//	   activatefilling_display2;
//	   reading_starts2;
//	}
//
//	 while(1)
//	 {
//		 char tst[14] = {"hello there"};
//		 HAL_UART_Transmit_DMA(&huart1, tst, strlen(tst) );
//		 HAL_Delay(1000);
//	 }

//  operating_side  = side_b;
//		 house_keeping2();
//         states2();
	//	 keypad_lcd(2,"P   0");
}

//==============================================================
uint8_t  read_event1()
{
	extern bool self->lock_clr;
		 	   self->key19_flag = 0;
		 	 //  self->auth_cmd_flag = 0;
	     	 //  self->totaliser_flag = 0;
		 	 //  self->keypress_flag = 0;

            //READ STATES OF INPUTS PIN AND KEYPAD...
           	  self->totaliser_flag =  readtotaliser1_state();

			  //self->key19_flag =  readkey19_state();

	          self->key_flag =  readsettingskey_state();

	          self->nozzle_flag = readNozzle1();

   			  self->keypress_ = self->keynew;  //key flag is also set...

#ifndef DEV_MODE
   			 //--------------------------------------------------
			  //  totaliser error.
				if( (self->totaliser_flag == 0) && (self->drive != ACTIVATE) )
				{
					self->totaliser_flag = 1;
					return _tot_error_Event;
				}
   			  //--------------------------------------------------
#endif     //#ifndef DEV_MODE


	 //==========check for long press events.....========
	 //==================================================
	 //      then select the  operator  view mode...
	 //--------------------------------------------------
   			self->tot_longpress_flag = long_press_tot();
   			self->log_longpress_flag = long_press_log();
   			self->key_longpress_flag = long_press_key();
   			self->progExit_longpress_flag = long_press_progExit();

   			  if(self->tot_longpress_flag == 1)
			  {
				   self->operatorfxn = totaliser_view;
				   return _operator_Event;
			  }

   			  else if(self->log_longpress_flag == 1)
			  {
				   self->operatorfxn = log_view;
				   return _operator_Event;
			  }
   			  else if(self->key_longpress_flag == 1)
			  {
				//if not previously activated,
//				 if (key_longpress_status == 0)
//				 {
//					key_longpress_status = 1;
					self->prog_entry = 1;   //variable used to clear the var. states in settings menu.
					return _keyup_Event;
//				 }
//				 else
//				 {
//					key_longpress_status = 0;
//					self->prog_entry = 0;
//					return _keydown_Event;
//				 }
			  }
   			  else if(self->progExit_longpress_flag == 1)
			  {
				//if not previously activated,
//				 if (key_longpress_status == 0)
//				 {
//					key_longpress_status = 1;
//					self->prog_entry = 1;   //variable used to clear the var. states in settings menu.
//					return _keyup_Event;
//				 }
//				 else
//				 {
//					key_longpress_status = 0;
					self->prog_entry = 0;
					return _keydown_Event;
//				 }
			  }

       //--------------------------------------------------
	   //          error clear flag...
	   if( self->error_clr_flag == 1)
		 {
			self->error_clr_flag = 0;

			self->filling = 0;

			return _error_clear_Event;
		 }
	  //--------------------------------------------------
			 // authorise  event capture.
		if ( self->auth_flag  == 1 )
		{
			self->auth_flag = 0;
			return _authorise_Event;
		}
		//--------------------------------------------------
				// authorise  command event.
		if ( self->auth_cmd_flag  == 1 )
			{
			  //if(settings[operating_side-1].mode == offline_)
			  //{
				self->auth_cmd_flag = 0;

//				self->eNextState = authorised_nozzledown_State;
					//---------------------------------------------
					//                nozzle-up overide
					if (self->eNextState == authorised_nozzledown_State)
					{
//						if (override_ == override)
//						{
							return _nozzleup_Event;
//						}
					}
					//---------------------------------------------
				  return _auth_command_Event;
			  //}
			}

	   // nozzle up  event capture...
			if( (self->nozzle_flag_old == 0) && (self->nozzle_flag == 1) )
			{
					self->nozzle_flag_old = 1;
//					if (override_ != override)
//					{
								//send nozzleup command only in MANUAL mode
								return _nozzleup_Event;
//					}
//					else 		// NozzlezUp, awaiting authorisation
//					{
//
//					}
			}
	  //-----------------------
	  // nozzle down  event capture...
			if( (self->nozzle_flag_old == 1) && (self->nozzle_flag == 0) )
			{
					self->nozzle_flag_old = 0;
//					if (override_ != override)
						return _nozzledown_Event;
			}
		//	self->nozzle_flag_old = self->nozzle_flag;
	  //--------------------------------------------------
	  //--------------------------------------------------
		// key up  event capture...
		if( (self->key_flag_old == 0)&&(self->key_flag == 1) )
			{
					self->key_flag_old = 1;
			      return _keyup_Event;
			}
	  //-----------------------
		// key down  event capture...
		if( (self->key_flag_old == 1)&&(self->key_flag == 0) )
			{
					self->key_flag_old = 0;
				  return _keydown_Event;
			}
			self->key_flag_old = self->key_flag;
	  //--------------------------------------------------
	  //--------------------------------------------------
		// key press event capture...
		if (self->keypress_flag == 1)
			{
			    self->keypress__ = 1;
				self->keypress_flag = 0;
				return _keypress_Event;
			}
	  //--------------------------------------------------
	  /*	  if (self->key19_flag == 1)
			{
				return _key19_Event;
			}                              */
	  //--------------------------------------------------
	  //  totaliser error.
//		if( (self->totaliser_flag == 0) && (self->drive != ACTIVATE) )
//		{
//			self->totaliser_flag = 1;
//			return _tot_error_Event;
//		}
	  //--------------------------------------------------
	   //self->filling pulse detection.
		if ( (self->pulser_count_old < self->pulser_new) && ( self->eNextState == authorised_nozzleup_State ) )
			{
				self->pulser_count_old = self->pulser_new;
				self->lock_clr = 0;
				return _filling_pulse_Event;
			}
		 self->pulser_count_old = self->pulser_new;

	  //--------------------------------------------------
			  // timeout   event capture...
		if( (self->timer_flag_old == 0)&&(self->timer_flag == 1) )
		{
				  self->timer_flag_old = 1;

				  self->filling = 0;

			  return _timeout_Event;
		}
			self->timer_flag_old = self->timer_flag;

	  return _no_Event;
}

//==============================================================
int  read_event2()
{
	// return _no_Event;

	  extern bool lock_clr2;
		 	   key19_flag2 = 0;
		 	 //  self->auth_cmd_flag = 0;
	     	 //  self->totaliser_flag = 0;
		 	 //  self->keypress_flag = 0;

            //READ STATES OF INPUTS PIN AND KEYPAD...
           	  totaliser_flag2 =  readtotaliser2_state();

			  //self->key19_flag =  readkey19_state();

	          key_flag2 =  readsettingskey2_state();

//	          nozzle_flag2 = readNozzle2();
	          bool redundantHolder = nozzleSwitch_read2();

			#if defined(AUTO_SALE_TEST)

	          //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA//
			  //======================= AUTOMATED SALES TEST ==========================//
	          if(self->settings_stream1[1].mode == AUTO_MODE)
	          {
	        	  if(eNextState2 != idle_State)
	        	  {
	        		  autoSale_timer2 = 0;
	        	  }
	        	  if( (autoSale_timer2 >= 8555) && (eNextState2 == idle_State) )
				  {
//					  nozzle_flag2 = 1;
//					  nozzle_flag_old2 = 0;
					  nozzle_flag_key2 = 1;
//					  nozzle_flag_key_old2 = 0;
					  autoSale_timer2 = 0;
				  }
	          }

			  //UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU//

			#endif //#ifdef AUTO_SALE_TEST

   			  keypress_2 = keynew2;  //key flag is also set...

   			//////////////////////////////////////////////////////////
			//                 tone event capture...
			//////////////////////////////////////////////////////////

			if( (timerFlagOld_tone2 == 0) && (timerFlag_tone2 == 1) )
			{
				return _tone_Event;
			}
			else if( (timerFlagOld_tone2 == 1) && (timerFlag_tone2 == 0) )
			{
				HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
				tone_duration2 = 0;
			}

			timerFlagOld_tone2 = timerFlag_tone2;

			//////////////////////////////////////////////////////////

#ifndef DEV_MODE
   			//--------------------------------------------------
			  //  totaliser error.
				if( (totaliser_flag2 == 0) && (drive2 != ACTIVATE) )
				{
					totaliser_flag2 = 1;
					return _tot_error_Event;
				}
		    //--------------------------------------------------
#endif     //#ifndef DEV_MODE


	 //==========check for long press events.....========
	 //==================================================
	 //      then select the  operator  view mode...
	 //--------------------------------------------------
			if( (self->settings_stream1[1].keypad__ == LAFNG17_K) || (self->settings_stream1[1].keypad__ == LAFNG18_K) || (self->settings_stream1[1].keypad__ == LAFNG18_K_V2) )
			{
				tot_longpress_flag2 = long_press_tot2();
			}

   			log_longpress_flag2 = long_press_log2();
//   			key_longpress_flag2 = long_press_key2();
//   			progExit_longpress_flag2 = long_press_progExit2();

   		  if(tot_longpress_flag2 == 1)
		  {
			   operatorfxn2 = totaliser_view;
			   tot_longpress_flag2 = 0;
			   return _operator_Event;
		  }

   		  else if(log_longpress_flag2 == 1)
		  {
			   operatorfxn2 = log_view;
			   return _operator_Event;
		  }
   		  else if(key_longpress_flag2 == 1)
		  {
				//if not previously activated,
//				 if (key_longpress_status2 == 0)
//				 {
//					key_longpress_status2 = 1;
					prog_entry2 = 1;   //variable used to clear the var. states in settings menu.
					key_longpress_flag2 = 0;
					return _keyup_Event;
//				 }
//				 else
//				 {
//					key_longpress_status2 = 0;
//					prog_entry2 = 0;
//					return _keydown_Event;
//				 }
		  }
   		  else if(progExit_longpress_flag2 == 1)
		  {
				//if not previously activated,
//				 if (key_longpress_status2 == 0)
//				 {
//					key_longpress_status2 = 1;
//					prog_entry2 = 1;   //variable used to clear the var. states in settings menu.
//					return _keyup_Event;
//				 }
//				 else
//				 {
//					key_longpress_status2 = 0;
					prog_entry2 = 0;

					progExit_longpress_flag2 = 0;

					return _keydown_Event;
//				 }
		  }

       //--------------------------------------------------
	   //          error clear flag...
	   if( error_clr_flag2 == 1)
		 {
			error_clr_flag2 = 0;

			filling2 = 0;

			return _error_clear_Event;
		 }
	  //--------------------------------------------------
			 // authorise  event capture.
		if ( auth_flag2 == 1 )
		{
			auth_flag2 = 0;
			return _authorise_Event;
		}
		//--------------------------------------------------
				// authorise  command event.
//		if ( auth_cmd_flag2  == 1 )
//			{
//				auth_cmd_flag2 = 0;
//
//				if(self->settings_stream1[1].mode == MANUAL_MODE)
//				{
//					eNextState2 = authorised_nozzledown_State;
//				}
//
//				//---------------------------------------------
//				//                nozzle-up overide
//				if (eNextState2 == authorised_nozzledown_State)
//				{
//					if (overide_2 == overide)
//					{
//						return _nozzleup_Event;
//					}
//				}
//
//				if(self->settings_stream1[1].mode == MANUAL_MODE)
//				{
//					eNextState2 = authorised_nozzledown_State;
//				}
//				//---------------------------------------------
//
//				return _auth_command_Event;
//			}
//
		//--------------------------------------------------
		// authorise  command event.
			else if ( auth_cmd_flag2 == 1 )
			{
			  //if(settings[operating_side-1].mode == offline_)
			  //{
				auth_cmd_flag2 = 0;


				if(self->settings_stream1[1].mode == MANUAL_MODE)
				{
					eNextState2 = authorised_nozzledown_State;
				}
					//---------------------------------------------
					//                nozzle-up overide
					if (eNextState2 == authorised_nozzledown_State)
					{
//						if (override_2 == override)
//						{
							return _nozzleup_Event;
//						}
					}
			//				//---------------------------------------------
			//
							if(self->settings_stream1[1].mode == AUTO_MODE)
							{
								if(nozzlezUp2 == 1)   //A NozzleUp that triggers a transaction
								{
									nozzlezUp2 = 0;
									return _authorisecommand_Event;
								}
							}
			//
			//			  return _auth_command_Event;
			  //}
			}


			if(self->settings_stream1[1].mode == AUTO_MODE)
		    {

			   if (hardwareError_flag2 == 1)
			   {
				   hardwareError_flag2 = 0;
				   return _hardwareerror_Event;
			   }

			   // mamo reach  event capture...
			   else if(mamo_reached_flag2 == 1)
				{
					mamo_reached_flag2 = 0;
					return _mamo_Event;
				}

			   // idle state due-return event capture...
			   else if (fillingComplete_flag2 == 1)
			   {
				   fillingComplete_flag2 = 0;
				   return _fillingcomplete_Event;
			   }
			   else if (reset_flag2 == 1)
			   {
				   reset_flag2 = 0;
				   return _resetcommand_Event;
			   }
			   else if (authsuspend_flag2 == 1)
			   {
				   authsuspend_flag2 = 0;
				   return  _auth_suspendcommand_Event;
			   }
			   else if (fillingsuspend_flag2 == 1)
			   {
				   fillingsuspend_flag2 = 0;
				   return _filling_suspendcommand_Event;
			   }
			   else if (authresume_flag2 == 1)
			   {
				   authresume_flag2 = 0;
				   return _auth_resumecommand_Event;
			   }
			   else if (fillingresume_flag2 == 1)
			   {
				   fillingresume_flag2 = 0;
				   return _filling_resumecommand_Event;
			   }
		   }

			else if(self->settings_stream1[1].mode == MANUAL_MODE)
		    {
				if (fillingresume_flag2 == 1)
			   {
				   fillingresume_flag2 = 0;
				   return _filling_resumed_Event;
			   }
		    }

		//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH//
		//======================== NOZZLE-UP EVENT ============================//

	    // nozzle up  event capture...

   //		   if(
   //				 ( ((nozzle_flag_old2 == 0) && (nozzle_flag2 == 1)) ||
   //				 ((nozzle_flag_key_old2 == 0) && (nozzle_flag_key2 == 1)) ) &&
   //				 ((eNextState2 != operator_State) && (eLastState2 != operator_State)) &&
   //				 (self->eNextState != prog_State)
   //			 )
		   if(
				 (
					( (nozzle_flag_old2 == 0) && (nozzle_flag2 == 1) ) ||
					( (nozzle_flag_key_old2 == 0) && (nozzle_flag_key2 == 1) )
				 ) &&
				 (self->eNextState != prog_State)  //Forbids Sales-State in a Config-Mode of either Sides
			 )
		   {
//			   keypad_zerorise2 = true;

			   if((nozzle_flag_old2 == 0) && (nozzle_flag2 == 1))
			   {
				   if((nozzle_flag_key_old2 == 0) && (nozzle_flag_key2 == 0))
				   {
					   nozzle_flag_old2 = 1;
				   }
				   else if((nozzle_flag_key_old2 == 1) && (nozzle_flag_key2 == 1))
				   {
					   nozzle_flag_old2 = 1;

					   if(eNextState2 == filling_State)
						   return _no_Event; //
				   }
			   }
			   else if((nozzle_flag_key_old2 == 0) && (nozzle_flag_key2 == 1))
			   {
				   if((nozzle_flag_old2 == 1) && (nozzle_flag2 == 1))
				   {
					   nozzle_flag_key_old2 = 1;
					   return _no_Event; //
				   }
				   else if((nozzle_flag_old2 == 0) && (nozzle_flag2 == 0))
				   {
					   nozzle_flag_key_old2 = 1;
				   }
			   }
//					if (override_2 != override)
//					{
						//send nozzleup command only in MANUAL mode

				if(self->settings_stream1[1].mode == AUTO_MODE)
				{
					nozzlezUp2 = 1;
				}

				nozzle_out2 = true;

				return _nozzleup_Event;

//				}
//				else 		// NozzlezUp, awaiting authorisation
//				{
//
//				}
		   }

	   //HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH//


	   //HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH//
	   //=========================== NOZZLE-DOWN EVENT ============================//
	   //--------------------------------------------------------------------------//

	   // nozzle down  event capture...

  //		  else if(
  //					 ( ((nozzle_flag_old2 == 1) && (nozzle_flag2 == 0)) ||
  //					 ((nozzle_flag_key_old2 == 1) && (nozzle_flag_key2 == 0)) ) &&
  //					 ((eNextState2 != operator_State) && (eLastState2 != operator_State)) &&
  //					 (self->eNextState != prog_State)
  //				 )
		  else if(
				  ( (nozzle_flag_old2 == 1) && (nozzle_flag2 == 0) ) ||
				  ( (nozzle_flag_key_old2 == 1) && (nozzle_flag_key2 == 0) )
				 )
		   {
			   if((nozzle_flag_old2 == 1) && (nozzle_flag2 == 0))
			   {
				   if((nozzle_flag_key_old2 == 0) && (nozzle_flag_key2 == 0))
				   {
					   nozzle_flag_old2 = 0;  //
					   nozzle_flag_key_old2 = 0;
					   nozzle_flag_key2 = 0;

					   status_change_noz2 = 1;
					   nozzle_out2 = false;
				   }
				   else if((nozzle_flag_key_old2 == 1) && (nozzle_flag_key2 == 1))
				   {
					   nozzle_flag_old2 = 0; //
					   nozzle_flag_key_old2 = 0;
					   nozzle_flag_key2 = 0;

					   status_change_noz2 = 1;
					   nozzle_out2 = false;
				   }
			   }
			   else if((nozzle_flag_key_old2 == 1) && (nozzle_flag_key2 == 0))
			   {
				   if((nozzle_flag_old2 == 1) && (nozzle_flag2 == 1))
				   {
					   nozzle_flag_key_old2 = 0;

					   status_change_noz2 = 1;
					   nozzle_out2 = false;
				   }
				   else if((nozzle_flag_old2 == 0) && (nozzle_flag2 == 0))
				   {
					   nozzle_flag_key_old2 = 0;  //

					   status_change_noz2 = 1;
					   nozzle_out2 = false;
				   }
			   }

			   keypad_zerorize2();
   //		 if (override_ != override)
			  return _nozzledown_Event;
		   }

	   //HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH//

		//	self->nozzle_flag_old = self->nozzle_flag;
	  //--------------------------------------------------

	  //--------------------------------------------------
		// key up  event capture...
		if( (key_flag_old2 == 0)&&(key_flag2 == 1) )
			{
					key_flag_old2 = 1;
			      return _keyup_Event;
			}
	  //-----------------------
		// key down  event capture...
		if( (key_flag_old2 == 1)&&(key_flag2 == 0) )
			{
					key_flag_old2 = 0;
				  return _keydown_Event;
			}
			key_flag_old2 = key_flag2;
	  //--------------------------------------------------
	  //--------------------------------------------------
		// key press event capture...
		if (keypress_flag2 == 1)
			{
			    keypress__2 = 1;
				keypress_flag2 = 0;
				return _keypress_Event;
			}
	  //--------------------------------------------------
	  /*	  if (self->key19_flag == 1)
			{
				return _key19_Event;
			}                              */
	  //--------------------------------------------------
//	  //  totaliser error.
//		if( (totaliser_flag2 == 0) && (drive2 != ACTIVATE) )
//		{
//			totaliser_flag2 = 1;
//			return _tot_error_Event;
//		}
	  //--------------------------------------------------
	   //self->filling pulse detection.
		if ( (pulser_count_old2 < pulser_new2) && ( eNextState2 == authorised_nozzleup_State ) )
			{
				pulser_count_old2 = pulser_new2;
				lock_clr2 = 0;
				return _filling_pulse_Event;
			}
		 pulser_count_old2 = pulser_new2;

	  //--------------------------------------------------
			  // timeout   event capture...
		if( (timer_flag_old2 == 0) && (timer_flag2 == 1) )
		{
				  timer_flag_old2 = 1;

				  filling2 = 0;

			  return _timeout_Event;
		}
			timer_flag_old2 = timer_flag2;

	  return _no_Event;
}


void pumpType_configure(void)
{
	  if((self->settings_stream1.display__ == LAFNG885) && (self->settings_stream1.keypad__ == LAFNG17_K))  //LAFENG885-NormalScreen | LAFENG16-Keypad
	  {
		  EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000001);
	  }
	  else if((self->settings_stream1.display__ == BLSKY886_N) && (self->settings_stream1.keypad__ == BLSKY18_K))  //BLUESKY886-NormalScreen | BLUESKY18K-Keypad
	  {
		  EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000010);
	  }
	  else if((self->settings_stream1.display__ == BLSKY886_N) && (self->settings_stream1.keypad__ == BLSKY22))  //BLUESKY886-NormalScreen | BLUESKY22-Keypad
	  {
		  EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000011);
	  }
	  else if((self->settings_stream1.display__ == BLSKY886_IN) && (self->settings_stream1.keypad__ == BLSKY18_K))  //BLUESKY886-InvertedScreen | BLUESKY18K-Keypad
	  {
		  EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000100);
	  }
	  else if((self->settings_stream1.display__ == BLSKY886_IN) && (self->settings_stream1.keypad__ == BLSKY22))  //BLUESKY886-InvertedScreen | BLUESKY22-Keypad
	  {
		  EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000101);
	  }
	  else if((self->settings_stream1.display__ == LAFNG885) && (self->settings_stream1.keypad__ == LAFNG18_K))  //LAFENG885-NormalScreen | LAFENG18-Keypad
	  {
		  EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000110);
	  }
	  else if((self->settings_stream1.display__ == BLSKY886_N) && (self->settings_stream1.keypad__ == LAFNG18_K))  //BLSKY886_N-NormalScreen | LAFENG18-Keypad
	  {
		  EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000111);
	  }



		if(strcmp(self->settings_stream1.product_, "PMS") == 0)
		{
			EEPROM_Write_NUM (save_productType_loc, 0, PMS);
		}
		else if(strcmp(self->settings_stream1.product_, "AGO") == 0)
		{
			EEPROM_Write_NUM (save_productType_loc, 0, AGO);
		}
		else if(strcmp(self->settings_stream1.product_, "DPK") == 0)
		{
			EEPROM_Write_NUM (save_productType_loc, 0, DPK);
		}

		EEPROM_Write_NUM (save_nozzleId_loc, 0, self->settings_stream1.noz_id);
}


void pumpType_configure_fram(FuelDispenser *disp)
{
	for(uint8_t i = 0; i < NUM_NOZZLES; i++)
	{
		Nozzle *self = &disp->nozzles[0];

		if((self->settings_stream1.display__ == LAFNG885) && (self->settings_stream1.keypad__ == LAFNG17_K))  //LAFENG885-NormalScreen | LAFENG16-Keypad
		{
		  FRAM_WriteByte (disp->save_pumpType_loc_fram, 0b00000001);
		}
		else if((self->settings_stream1.display__ == BLSKY886_N) && (self->settings_stream1.keypad__ == BLSKY18_K))  //BLUESKY886-NormalScreen | BLUESKY18K-Keypad
		{
		  FRAM_WriteByte (disp->save_pumpType_loc_fram, 0b00000010);
		}
		else if((self->settings_stream1.display__ == BLSKY886_N) && (self->settings_stream1.keypad__ == BLSKY22))  //BLUESKY886-NormalScreen | BLUESKY22-Keypad
		{
		  FRAM_WriteByte (disp->save_pumpType_loc_fram, 0b00000011);
		}
		else if((self->settings_stream1.display__ == BLSKY886_IN) && (self->settings_stream1.keypad__ == BLSKY18_K))  //BLUESKY886-InvertedScreen | BLUESKY18K-Keypad
		{
		  FRAM_WriteByte (disp->save_pumpType_loc_fram, 0b00000100);
		}
		else if((self->settings_stream1.display__ == BLSKY886_IN) && (self->settings_stream1.keypad__ == BLSKY22))  //BLUESKY886-InvertedScreen | BLUESKY22-Keypad
		{
		  FRAM_WriteByte (disp->save_pumpType_loc_fram, 0b00000101);
		}
		else if((self->settings_stream1.display__ == LAFNG885) && (self->settings_stream1.keypad__ == LAFNG18_K))  //LAFENG885-NormalScreen | LAFENG18-Keypad
		{
		  FRAM_WriteByte (disp->save_pumpType_loc_fram, 0b00000110);
		}
		else if((self->settings_stream1.display__ == BLSKY886_N) && (self->settings_stream1.keypad__ == LAFNG18_K))  //BLSKY886_N-NormalScreen | LAFENG18-Keypad
		{
		  FRAM_WriteByte (disp->save_pumpType_loc_fram, 0b00000111);
		}


		if(strcmp(self->settings_stream1.product_, "PMS") == 0)
		{
			FRAM_WriteByte (disp->save_productType_loc_fram, PMS);
		}
		else if(strcmp(self->settings_stream1.product_, "AGO") == 0)
		{
			FRAM_WriteByte (disp->save_productType_loc_fram, AGO);
		}
		else if(strcmp(self->settings_stream1.product_, "DPK") == 0)
		{
			FRAM_WriteByte (disp->save_productType_loc_fram, DPK);
		}

		FRAM_WriteByte (disp->save_nozzleId_loc_fram, self->settings_stream1.noz_id);
	}
}


//		if(strcmp(settings[0].product_, "PMS") == 0)
//		{
//			val = PMS;
//			EEPROM_Write (save_productType_loc, 0, val, sz);
//			EEPROM_Read (save_productType_loc, 0, &val, sz);
//		}
//		else if(strcmp(settings[0].product_, "AGO") == 0)
//		{
//			val = AGO;
//			EEPROM_Write (save_productType_loc, 0, val, 1);
//			EEPROM_Read (save_productType_loc, 0, &val, sz);
//		}
//		else if(strcmp(settings[0].product_, "DPK") == 0)
//		{
//			vall = DPK;
//			EEPROM_Write (save_productType_loc, 0, vall, sz);
//			HAL_Delay(200);
//			EEPROM_Read (save_productType_loc, 0, &val, sz);
//		}
//
//		id = settings[0].noz_id;
//		EEPROM_Write (save_nozzleId_loc, 0, id, sz);
//		EEPROM_Read (save_nozzleId_loc, 0, &val, sz);
//}


void pumpType_parse(void)
{
//	  if(self->settings_stream1.pump_type_ == DN_LAFNG17K)
//	  {
//		  self->settings_stream1.display__ = LAFNG885;
//		  self->settings_stream1.keypad__ = LAFNG17_K;
//	  }
//	  else if(self->settings_stream1.pump_type_ == DN_LAFNG18K)
//	  {
//		  self->settings_stream1.display__ = LAFNG885;
//		  self->settings_stream1.keypad__ = LAFNG18_K;
//	  }
//	  else if(self->settings_stream1.pump_type_ == DN_BLSKY18K)
//	  {
//		  self->settings_stream1.display__ = BLSKY886_N;
//		  self->settings_stream1.keypad__ = BLSKY18_K;
//	  }
//	  else if(self->settings_stream1.pump_type_ == BLSKY886_N_LAFNG18_K)
//	  {
//		  self->settings_stream1.display__ = BLSKY886_N;
//		  self->settings_stream1.keypad__ = LAFNG18_K;
//	  }
//	  else if(self->settings_stream1.pump_type_ == DN_BLSKY22)
//	  {
//		  self->settings_stream1.display__ = BLSKY886_N;
//		  self->settings_stream1.keypad__ = BLSKY22;
//	  }
//	  else if(self->settings_stream1.pump_type_ == DIN_BLSKY18K)
//	  {
//		  self->settings_stream1.display__ = BLSKY886_IN;
//		  self->settings_stream1.keypad__ = BLSKY18_K;
//	  }
//	  else if(self->settings_stream1.pump_type_ == DIN_BLSKY22)
//	  {
//		  self->settings_stream1.display__ = BLSKY886_IN;
//		  self->settings_stream1.keypad__ = BLSKY22;
//	  }

}

//    settings[0].display_
//	settings[0].keypad_
//	settings[0].product_
//	settings[0].noz_id;

//typedef enum
//{
//  LAFNG17_K = 1,    //ie. 17 punch-keys + a turn-key
//  BLSKY18_K,		//ie. 18 punch-keys + a turn-key
//  BLSKY22			//ie. 22 punch-keys + no turn-key
//}keypadtype;
//
//
//typedef enum
//{
//  LAFNG885 = 1,
//  BLSKY886_N,           //N -> Normal
//  BLSKY886_IN			//IN -> Inverted
//}displaytype;
//
//
//typedef enum
//{
//  DN_LAFNG17K = 0b00000001,		//Display->Normal, keypad->LAFNG-17K
//  DN_BLSKY18K,       			//Display->Normal, keypad->BLSKY-18K
//  DN_BLSKY22,					//Display->Normal, keypad->BLSKY-22
//  DIN_BLSKY18K,       			//Display->Inverted, keypad->BLSKY-18K
//  DIN_BLSKY22					//Display->Inverted, keypad->BLSKY-22
//}pump;

void pumpType_write(void)
{
	self->settings_stream1.pump_type_ = EEPROM_Read_NUM (save_pumpType_loc, 0);
	self->settings_stream1[1].pump_type_ = EEPROM_Read_NUM (save_pumpType_loc, 0);
	disp_type1 = self->settings_stream1.pump_type_;
	disp_type2 = self->settings_stream1[1].pump_type_;
}

void pumpType_write_fram(FuelDispenser *disp)
{
//	uint8_t FRAM_ReadByte(uint16_t memAddress);
	for(uint8_t i = 0; i < NUM_NOZZLES; i++)
	{
		Nozzle *self = &disp->nozzles[i];

		self->settings_stream1.pump_type_ = FRAM_ReadByte(disp->save_pumpType_loc_fram);
//		self->settings_stream1[1].pump_type_ = FRAM_ReadByte(disp->save_pumpType_loc_fram);
		disp_type1 = self->settings_stream1.pump_type_;
//		disp_type2 = self->settings_stream1[1].pump_type_;
	}
}



void firstTotalizer_day(void)
{
	 EEPROM_Write_NUM (totalizerDay_loc, 0, (DS1307_GetDate() - 1));
}

void firstTotalizerDay_write(void)
{
	self->settings_stream2[0].totalizer_day = EEPROM_Read_NUM (totalizerDay_loc, 0);
	self->settings_stream2[1].totalizer_day = EEPROM_Read_NUM (totalizerDay_loc, 0);
}



void pumpName_parse(FuelDispenser *disp)
{
	 	char str[5];

		int8_t productType;

		for(uint8_t i = 0; i < NUM_NOZZLES; i++)
		{
			Nozzle *self = &disp->nozzles[i];

			self->settings_stream1.noz_id = EEPROM_Read_NUM (save_nozzleId_loc, 0);

			productType = EEPROM_Read_NUM (save_productType_loc, 0);

			if( productType == PMS)
			{
				strcpy(self->settings_stream1.product_, "PMS");

				snprintf(str, 5,"P%d", self->settings_stream1.noz_id);
				strcpy(self->pumpName[0].pump_name, str);
//				snprintf(str, 5, "P%d", (self->settings_stream1.noz_id + 1));
//				strcpy(self->pumpName[1].pump_name, str);
			}

			else if( productType == AGO)
			{
				strcpy(self->settings_stream1.product_, "AGO");

				snprintf(str, 5,"A%d", self->settings_stream1.noz_id);
				strcpy(self->pumpName[0].pump_name, str);
//				snprintf(str, 5, "A%d", (self->settings_stream1.noz_id + 1));
//				strcpy(self->pumpName[1].pump_name, str);
			}
			else if( productType == DPK)
			{
				strcpy(self->settings_stream1.product_, "DPK");

				snprintf(str, 5,"D%d", self->settings_stream1.noz_id);
				strcpy(self->pumpName[0].pump_name, str);
//				snprintf(str, 5, "D%d", (self->settings_stream1.noz_id + 1));
//				strcpy(self->pumpName[1].pump_name, str);
			}
		}
}

void pumpName_parse_fram(FuelDispenser *disp)
{
		char str[5];

		uint8_t productType;

		for(uint8_t i = 0; i < NUM_NOZZLES; i++)
		{
			Nozzle *self = &disp->nozzles[i];

			self->settings_stream1.noz_id = FRAM_ReadByte(disp->save_nozzleId_loc_fram);

			productType = FRAM_ReadByte(disp->save_productType_loc_fram);

			if( productType == PMS)
			{
				strcpy(self->settings_stream1.product_, "PMS");

				snprintf(str, 5,"P%d", self->settings_stream1.noz_id);
				strcpy(self->pumpName[0].pump_name, str);
//				snprintf(str, 5, "P%d", (self->settings_stream1.noz_id + 1));
//				strcpy(self->pumpName[1].pump_name, str);
			}

			else if( productType == AGO)
			{
				strcpy(self->settings_stream1.product_, "AGO");

				snprintf(str, 5,"A%d", self->settings_stream1.noz_id);
				strcpy(self->pumpName[0].pump_name, str);
//				snprintf(str, 5, "A%d", (self->settings_stream1.noz_id + 1));
//				strcpy(self->pumpName[1].pump_name, str);
			}
			else if( productType == DPK)
			{
				strcpy(self->settings_stream1.product_, "DPK");

				snprintf(str, 5,"D%d", self->settings_stream1.noz_id);
				strcpy(self->pumpName[0].pump_name, str);
//				snprintf(str, 5, "D%d", (self->settings_stream1.noz_id + 1));
//				strcpy(self->pumpName[1].pump_name, str);
			}
		}
}

//void eeprom_clearToken(void)
//{
//	for(int8_t i = 0; i < 15; i++)
//	{
//		EEPROM_Write_NUM ((token_loc + i), 0, 0);
//	}
//}

//void pumpType_configure(void)
//{
//	int8_t setPin1_state = 0,
//		   setPin2_state = 0;
//
//	  setPin1_state = HAL_GPIO_ReadPin(settings1_GPIO_Port, settings1_Pin);
//	  setPin2_state = HAL_GPIO_ReadPin(settings2_GPIO_Port, settings2_Pin);
//
//	  if((setPin2_state == 0) && (setPin1_state == 1))  //LAFENG885-Screen | LAFENG16-Keypad
//	  {
//		  EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000001);
//	  }
//	  else if((setPin2_state == 1) && (setPin1_state == 0))  //BLUESKY886-Screen | BLUESKY12-Keypad
//	  {
//		  EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000010);
//	  }
//	  else if((setPin2_state == 1) && (setPin1_state == 1))  //BLUESKY886-Screen | BLUESKY16-Keypad
//	  {
//		  EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000011);
//	  }
//	  else  // Default
//	  {
//
//	  }
//}



uint8_t read_event(Nozzle *self)
{
	extern bool self->lock_clr;
	   self->key19_flag = 0;
	 //  self->auth_cmd_flag = 0;
	 //  self->totaliser_flag = 0;
	 //  self->keypress_flag = 0;

	//READ STATES OF INPUTS PIN AND KEYPAD...
	  self->totaliser_flag = readtotaliser_state(&self);

	  //self->key19_flag =  readkey19_state();

	  self->key_flag = readsettingskey_state(&self);

//	  self->nozzle_flag = readNozzle1();
	  bool redundantHolder = nozzleSwitch_read(&self);

    #if defined(AUTO_SALE_TEST)
	  //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA//
	  //======================= AUTOMATED SALES TEST ==========================//
	  if(self->settings_stream1.mode == AUTO_MODE)
	  {
		  if(self->eNextState != idle_State)
		  {
			  self->autoSale_timer = 0;
		  }
		  if( (self->autoSale_timer >= 5000) && (self->eNextState == idle_State) )
		  {
			  self->nozzle_flag_key = 1;
			  self->autoSale_timer = 0;
		  }
	  }

	  //UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU//

	#endif //#ifdef AUTO_SALE_TEST

	  self->keypress_ = self->keynew;  //key flag is also set...

	//--------------------------------------------------

	//////////////////////////////////////////////////////////
	//                 tone event capture...
	//////////////////////////////////////////////////////////

	if( (self->timerFlagOld_tone == 0) && (self->timerFlag_tone == 1) )
	{
		return _tone_Event;
	}
	else if( (self->timerFlagOld_tone == 1) && (self->timerFlag_tone == 0) )
	{
		HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
		self->tone_duration = 0;
	}

	self->timerFlagOld_tone = self->timerFlag_tone;

	//////////////////////////////////////////////////////////

#ifndef DEV_MODE
   			 //--------------------------------------------------
			  //  totaliser error.
				if( (self->totaliser_flag == 0) && (self->drive != ACTIVATE) )
				{
					self->totaliser_flag = 1;
					return _tot_error_Event;
				}
   			  //--------------------------------------------------
#endif     //#ifndef DEV_MODE


	 //==========check for long press events.....========
	 //==================================================
	 //      then select the  operator  view mode...
	 //--------------------------------------------------
		if( (self->settings_stream1.keypad__ == LAFNG17_K) || (self->settings_stream1.keypad__ == LAFNG18_K) || (self->settings_stream1.keypad__ == LAFNG18_K_V2) )
		{
			self->tot_longpress_flag = long_press_tot(&self);
		}
		self->log_longpress_flag = long_press_log(&self);
//		self->key_longpress_flag = long_press_key();
//		self->progExit_longpress_flag = long_press_progExit();

		  if(self->tot_longpress_flag == 1)
		  {
			   self->operatorfxn = totaliser_view;
			   self->tot_longpress_flag = 0;
			   return _operator_Event;
		  }

		  else if(self->log_longpress_flag == 1)
		  {
			   self->operatorfxn = log_view;
			   return _operator_Event;
		  }
		  else if(self->key_longpress_flag == 1)
		  {
			//if not previously activated,
//				 if (key_longpress_status == 0)
//				 {
//					key_longpress_status = 1;
				self->prog_entry = 1;   //variable used to clear the var. states in settings menu.
				self->key_longpress_flag = 0;
				return _keyup_Event;
//				 }
//				 else
//				 {
//					key_longpress_status = 0;
//					self->prog_entry = 0;
//					return _keydown_Event;
//				 }
		  }
		  else if(self->progExit_longpress_flag == 1)
		  {
			//if not previously activated,
//				 if (key_longpress_status == 0)
//				 {
//					key_longpress_status = 1;
//					self->prog_entry = 1;   //variable used to clear the var. states in settings menu.
//					return _keyup_Event;
//				 }
//				 else
//				 {
//					key_longpress_status = 0;
					self->prog_entry = 0;
					self->progExit_longpress_flag = 0;
					return _keydown_Event;
//				 }
			  }

       //--------------------------------------------------
	   //          error clear flag...
	   if(self->error_clr_flag == 1)
		 {
			self->error_clr_flag = 0;

			self->filling = 0;

			return _error_clear_Event;
		 }
	  //--------------------------------------------------
			 // authorise  event capture.
	   else if (self->auth_flag == 1)
		{
			self->auth_flag = 0;
			return _authorise_Event;
		}

		//--------------------------------------------------
				// authorise  command event.
	   else if (self->auth_cmd_flag == 1)
		{
		  //if(settings[operating_side-1].mode == offline_)
		  //{
			self->auth_cmd_flag = 0;


			if(self->settings_stream1.mode == MANUAL_MODE)
			{
				self->eNextState = authorised_nozzledown_State;
			}
				//---------------------------------------------
				//                nozzle-up overide
				if (self->eNextState == authorised_nozzledown_State)
				{
//					if (override_ == override)
//					{
						return _nozzleup_Event;
//					}
				}
//				//---------------------------------------------
//
				if(self->settings_stream1.mode == AUTO_MODE)
				{
					if(self->nozzlezUp == 1)   //A NozzleUp that triggers a transaction
					{
//						self->nozzlezUp = 0;
						return _authorisecommand_Event;
					}
				}
//
//			  return _auth_command_Event;
		  //}
		}

	   if(self->settings_stream1.mode == AUTO_MODE)
	   {

		   if (self->hardwareError_flag == 1)
		   {
			   self->hardwareError_flag = 0;
			   return _hardwareerror_Event;
		   }

		   // mamo reach  event capture...
		   else if(self->mamo_reached_flag == 1)
			{
				self->mamo_reached_flag = 0;
				return _mamo_Event;
			}

		   // idle state due-return event capture...
		   else if (self->fillingComplete_flag == 1)
		   {
			   self->fillingComplete_flag = 0;
			   return _fillingcomplete_Event;
		   }
		   else if (self->reset_flag == 1)
		   {
			   self->reset_flag = 0;
			   return _resetcommand_Event;
		   }
		   else if (self->authsuspend_flag == 1)
		   {
			   self->authsuspend_flag = 0;
			   return  _auth_suspendcommand_Event;
		   }
		   else if (self->fillingsuspend_flag == 1)
		   {
			   self->fillingsuspend_flag = 0;
			   return _filling_suspendcommand_Event;
		   }
		   else if (self->authresume_flag == 1)
		   {
			   self->authresume_flag = 0;
			   return _auth_resumecommand_Event;
		   }
		   else if (self->fillingresume_flag == 1)
		   {
			   self->fillingresume_flag = 0;
			   return _filling_resumecommand_Event;
		   }
	   }

	   else if(self->settings_stream1.mode == MANUAL_MODE)
	   {
		   if (self->fillingresume_flag == 1)
		   {
			   self->fillingresume_flag = 0;
			   return _filling_resumed_Event;
		   }
	   }



	   // nozzle up  event capture...
////	    if( ((self->nozzle_flag_old == 0) && (self->nozzle_flag == 1)) ||
////			((self->nozzle_flag_key_old == 0) && (self->nozzle_flag_key == 1)) )
//		if((self->nozzle_flag_old == 0) && (self->nozzle_flag == 1))
//		{
//			if(self->nozzle_flag == 1)
//			{
//				self->nozzle_flag_old = 1;
//			}
//			else if (self->nozzle_flag_key == 1)
//			{
//				self->nozzle_flag_key_old = 1;
//			}
//
////			if (override_ != override)
////			{
//				//send nozzleup command only in MANUAL mode
//
//				if(self->settings_stream1.mode == AUTO_MODE)
//				{
//					self->nozzlezUp = 1;
//				}
//
//				return _nozzleup_Event;
////			}
////			else 		// NozzlezUp, awaiting authorisation
////			{
////
////			}
//		}
//	  //-----------------------
//	  // nozzle down  event capture...
////	   else if( ((self->nozzle_flag_old == 1) && (self->nozzle_flag == 0))  ||
////			   ((self->nozzle_flag_key_old == 1) && (self->nozzle_flag_key == 0)) )
//	   else if((self->nozzle_flag_old == 1) && (self->nozzle_flag == 0) )
//	   {
//			self->nozzle_flag_old = 0;
////			self->nozzle_flag_key_old = 0;
//
////			if (override_ != override)
//				return _nozzledown_Event;
//	   }

	   //HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH//
	   //======================== NOZZLE-UP EVENT ============================//
	   //--------------------------------------------------------------------------//
	   	   	  // nozzle up event capture...

//	   if(
//   			 ( ((self->nozzle_flag_old == 0) && (self->nozzle_flag == 1)) ||
//   			 ((self->nozzle_flag_key_old == 0) && (self->nozzle_flag_key == 1)) ) &&
//   			 ((self->eNextState != operator_State) && (self->eLastState != operator_State)) &&
//   			 (eNextState2 != prog_State) //&& (self->eLastState != operator_State))
//   		 )
	   if(
			 (
				( (self->nozzle_flag_old == 0) && (self->nozzle_flag == 1) ) ||
				( (self->nozzle_flag_key_old == 0) && (self->nozzle_flag_key == 1) )
			 ) &&
			 (eNextState2 != prog_State)  //Forbids Sales-State in a Config-Mode of either Sides
		 )
	   {
//		   keypad_zerorise1 = true;
//		   keypad_zerorize();

		   if((self->nozzle_flag_old == 0) && (self->nozzle_flag == 1))
		   {
			   if((self->nozzle_flag_key_old == 0) && (self->nozzle_flag_key == 0))
			   {
				   self->nozzle_flag_old = 1;
			   }
			   else if((self->nozzle_flag_key_old == 1) && (self->nozzle_flag_key == 1))
			   {
				   self->nozzle_flag_old = 1;
				   return _no_Event;
			   }
		   }
		   else if((self->nozzle_flag_key_old == 0) && (self->nozzle_flag_key == 1))
		   {
			   if((self->nozzle_flag_old == 1) && (self->nozzle_flag == 1))
			   {
				   self->nozzle_flag_key_old = 1;

				   if(self->eNextState == filling_State)
					   return _no_Event; //
			   }
			   else if((self->nozzle_flag_old == 0) && (self->nozzle_flag == 0))
			   {
				   self->nozzle_flag_key_old = 1;
			   }
		   }

	   //			if (override_ != override)
	   //			{
	   				//send nozzleup command only in MANUAL mode

	   				if(self->settings_stream1.mode == AUTO_MODE)
	   				{
	   					self->nozzlezUp = 1;
	   				}

	   				self->nozzle_out = true;

//	   				uint32_t timestamp_event = RtcToInt(2019);
////	   				int_to_bcd_(timestamp_event, bcd_, sizeof(bcd_));
//	   				int_to_bcd(timestamp_event, bcd_);
//
////	   				NOZZLE_PICKUP;

	   				return _nozzleup_Event;
	   //			}
	   //			else 		// NozzlezUp, awaiting authorisation
	   //			{
	   //
	   //			}
	   		}
	   //HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH//


	   //HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH//
	   //=========================== NOZZLE-DOWN EVENT ============================//
	   //--------------------------------------------------------------------------//
	   	  // nozzle down  event capture...

//	   	  else if(
//					 ( ((self->nozzle_flag_old == 1) && (self->nozzle_flag == 0)) ||
//					 ((self->nozzle_flag_key_old == 1) && (self->nozzle_flag_key == 0)) ) &&
//					 ((self->eNextState != operator_State) && (self->eLastState != operator_State)) &&
//					 (eNextState2 != prog_State)
//				 )


	   	  else if(
	   			  ( (self->nozzle_flag_old == 1) && (self->nozzle_flag == 0) ) ||
	   			  ( (self->nozzle_flag_key_old == 1) && (self->nozzle_flag_key == 0) )
				 )
	   	  {

	   		  if((self->nozzle_flag_old == 1) && (self->nozzle_flag == 0))
			   {
	   			   if((self->nozzle_flag_key_old == 0) && (self->nozzle_flag_key == 0))
				   {
					   self->nozzle_flag_old = 0;  //
					   self->nozzle_flag_key_old = 0;
					   self->nozzle_flag_key = 0;

					   self->status_change_noz = 1;
					   self->nozzle_out = false;
				   }
	   			   else if((self->nozzle_flag_key_old == 1) && (self->nozzle_flag_key == 1))
				   {
					   self->nozzle_flag_old = 0; //
					   self->nozzle_flag_key_old = 0;
					   self->nozzle_flag_key = 0;

					   self->status_change_noz = 1;
					   self->nozzle_out = false;
				   }
			   }
	   		   else if((self->nozzle_flag_key_old == 1) && (self->nozzle_flag_key == 0))
			   {
				   if((self->nozzle_flag_old == 1) && (self->nozzle_flag == 1))
				   {
					   self->nozzle_flag_key_old = 0;

					   self->status_change_noz = 1;
					   self->nozzle_out = false;
				   }
				   else if((self->nozzle_flag_old == 0) && (self->nozzle_flag == 0))
				   {
					   self->nozzle_flag_key_old = 0;  //

					   self->status_change_noz = 1;
					   self->nozzle_out = false;
				   }
			   }

	   		   keypad_zerorize(&self);

	   //			if (override_ != override)
	   				return _nozzledown_Event;
	   	   }
	   //HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH//
		//	self->nozzle_flag_old = self->nozzle_flag;
	  //--------------------------------------------------
	  //--------------------------------------------------
		// key up  event capture...
	   else if( (self->key_flag_old == 0) && (self->key_flag == 1) )
		{
				self->key_flag_old = 1;
			  return _keyup_Event;
		}
	  //-----------------------
		// key down  event capture...
	   else if( (self->key_flag_old == 1) && (self->key_flag == 0) )
		{
				self->key_flag_old = 0;
			  return _keydown_Event;
		}
		self->key_flag_old = self->key_flag;
	  //--------------------------------------------------
	  //--------------------------------------------------
		// key press event capture...
	   if (self->keypress_flag == 1)
		{
			self->keypress__ = 1;
			self->keypress_flag = 0;
			return _keypress_Event;
		}
	  //--------------------------------------------------
	  /*	  if (self->key19_flag == 1)
			{
				return _key19_Event;
			}                              */
	  //--------------------------------------------------
	  //  totaliser error.
//		if( (self->totaliser_flag == 0) && (self->drive != ACTIVATE) )
//		{
//			self->totaliser_flag = 1;
//			return _tot_error_Event;
//		}
	  //--------------------------------------------------
	   //self->filling pulse detection.
	   if ( (self->pulser_count_old < self->pulser_new) && ( self->eNextState == authorised_nozzleup_State ) )
			{
				self->pulser_count_old = self->pulser_new;
				self->lock_clr = 0;
				return _filling_pulse_Event;
			}
		 self->pulser_count_old = self->pulser_new;

	  //--------------------------------------------------
			  // timeout   event capture...
		if( (self->timer_flag_old == 0) && (self->timer_flag == 1) )
		{
			  self->timer_flag_old = 1;

			  self->filling = 0;

			  return _timeout_Event;
		}

		self->timer_flag_old = self->timer_flag;

	  return _no_Event;
}


bool nozzleSwitch_read(Nozzle *self)
{
	static bool self->switchState = false;
	static bool self->lastSwitchState = false;

    static uint32_t self->lastDebounceTime = 0;

    bool currentState = readNozzle(&self);

    uint32_t currentTime = HAL_GetTick(); // Get the current system tick

    if (currentState != self->lastSwitchState)
    {
    	if ((self->eNextState == operator_State) && (self->eLastState == operator_State))
		{

		}
		else
		{
			self->lastDebounceTime = currentTime;
		}
//    	self->lastDebounceTime = currentTime;
    }

    if ((currentTime - self->lastDebounceTime) > NOZZLESWITCH_DEBOUNCE_DELAY)
    { // Update the actual switch state if the reading has been stable for the debounce delay
    	if (currentState != self->switchState)
    	{
    		self->switchState = currentState;
    		if(self->switchState == true)
			{
    			self->nozzle_flag = 1;

    			if(self->eLastState != pnp_State)
    			{
    				self->timerFlag_tone = 1;
    				self->tone_duration = 300;
    			}
//    			HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
			}
			else
			{
				self->nozzle_flag = 0;
				if(self->eLastState != pnp_State)
				{
					self->timerFlag_tone = 1;
					self->tone_duration = 300;
				}
//				HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
			}
    	}
    }

    self->lastSwitchState = currentState;

    return false;
}

bool nozzleSwitch_read2(void)
{
	static bool self->switchState = false;
	static bool self->lastSwitchState = false;

    static uint32_t self->lastDebounceTime = 0;

    bool currentState = readNozzle2();

    uint32_t currentTime = HAL_GetTick(); // Get the current system tick

    if (currentState != self->lastSwitchState)
    {
    	if ((eNextState2 == operator_State) && (eLastState2 == operator_State))
    	{

    	}
    	else
    	{
    		self->lastDebounceTime = currentTime;
    	}
    }

    if ((currentTime - self->lastDebounceTime) > NOZZLESWITCH_DEBOUNCE_DELAY)
    { // Update the actual switch state if the reading has been stable for the debounce delay
    	if (currentState != self->switchState)
    	{
    		self->switchState = currentState;
    		if(self->switchState == true)
			{
    			nozzle_flag2 = 1;

    			if(eLastState2 != pnp_State)
    			{
    				timerFlag_tone2 = 1;
    				tone_duration2 = 300;
    			}
			}
			else
			{
				nozzle_flag2 = 0;

				if(eLastState2 != pnp_State)
				{
    				timerFlag_tone2 = 1;
    				tone_duration2 = 300;
				}
			}
    	}
    }

    self->lastSwitchState = currentState;

    return false;
}

//bool self->switchState = false;
//bool self->lastSwitchState = false;
//unsigned long self->lastDebounceTime = 0;
//unsigned long currentTime;
//
//if (reading != self->lastSwitchState)
//{
//	self->lastDebounceTime = currentTime; // Reset the debounce timer
//}
//if ((currentTime - self->lastDebounceTime) > DEBOUNCE_DELAY) { // Update the actual switch state if the reading has been stable for the debounce delay
//	if (reading != self->switchState)
//	{
//		self->switchState = reading; // Here you can add code to handle the switch state change
//		printf("Switch state changed to: %d\n", self->switchState);
//	}
//}
//self->lastSwitchState = reading;
//}
//	}
//}

//uint8_t debounceKey1(void)
//{
//     static uint8_t self->lastKey = 0;
//
//     uint8_t key = keypad_lcd(0, key_lcd);  //write lcd and read keypad.
//
//     uint32_t currentTime = HAL_GetTick(); // Get the current system tick
//
//     switch (self->state)
//     {
//         case KEY_IDLE:
//							 if (key != 0)
//							 {
//								 self->state = KEY_DEBOUNCE;
//								 self->lastDebounceTime = currentTime;
//							 }
//							 break;
//
//         case KEY_DEBOUNCE:
//							 if ((currentTime - self->lastDebounceTime) > DEBOUNCE_TIME_MS)
//							 {
//								 if ((key != 0) && (key != self->lastKey))
//								 {
//									 self->lastKey = key;
//									 self->state = KEY_IDLE;
//
//									#if delay_keypad == 1
//
//										   if(self->settings_stream2[0].keypress_tone == Yes)
//										   {
//											  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
//										   }
//
//										   HAL_Delay(keypad_delay);
//									 #endif
//
//										   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
//
//									 return key; // Valid keypress detected
//								 }
//
//								 self->lastKey = key;
//								 self->state = KEY_IDLE;
//							 }
//							 break;
//     }
//
//     return 0;
// }

// uint8_t debounceKey2(void)
// {
//      static uint8_t self->lastKey = 0;
//
//      uint8_t key = keypad_lcd2(0, key_lcd2);  //write lcd and read keypad.
//
//      uint32_t currentTime = HAL_GetTick(); // Get the current system tick
//
//      switch (state2)
//      {
//          case KEY_IDLE:
// 							 if (key != 0)
// 							 {
// 								 state2 = KEY_DEBOUNCE;
// 								 lastDebounceTime2 = currentTime;
// 							 }
// 							 break;
//
//          case KEY_DEBOUNCE:
// 							 if ((currentTime - lastDebounceTime2) > DEBOUNCE_TIME_MS)
// 							 {
// 								 if ((key != 0) && (key != self->lastKey))
// 								 {
// 									 self->lastKey = key;
// 									 state2 = KEY_DEBOUNCE;
//
// 									 #if delay_keypad == 1
//
//									   if(self->settings_stream2[1].keypress_tone == Yes)
//									   {
//										  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
//									   }
//
//									   HAL_Delay(keypad_delay);
//								 	 #endif
//
//									   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
//
// 									 return key; // Valid keypress detected
// 								 }
// 								 self->lastKey = key;
// 								 state2 = KEY_IDLE;
// 							 }
// 							 break;
//      }
//
//      return 0;
//  }


uint8_t debounceKey(Nozzle *self)
 {
      static uint8_t self->lastKey = 0,
    		  	  	 self->test_key = 0;
//    		  	  	  self->keyPress = 0,
//					  self->nonKeyPress = 0;

      static bool self->keyPress_debounceKey = false;

      uint32_t duration = 0;

      uint8_t key = keypad_lcd(0, key_lcd);  //write lcd and read keypad.

      self->key__ = key;

      uint32_t currentTime = HAL_GetTick(); // Get the current system tick

      switch (self->state)
      {
          case KEY_IDLE:
 							 if (key != 0)
 							 {
 								 self->state = KEY_DEBOUNCE;
 								 self->lastDebounceTime = currentTime;
								 self->keyPressStartTime = currentTime;
								 self->test_key = key;
 							 }
 							 break;

          case KEY_DEBOUNCE:

        	  	  	  	  	 duration = currentTime - self->lastDebounceTime;

        	  	  	  	  	 if (key == self->test_key)
								 self->keyPress++;
        	  	  	  	  	 else
        	  	  	  	  		 self->nonKeyPress++;

							 if(duration > DEBOUNCE_TIME_MS)
							 {
								if (key != 0)
								{
									self->state = KEY_SHORT_PRESS;
								}
								else
								{
									self->state = KEY_IDLE;
								}
							 }
							 break;

		   case KEY_SHORT_PRESS:
								if (self->keyPress_debounceKey == true)
			   	   	   	   	   	{
			   	   	   	   	   		if ( (delay_nonBlocking1(keypad_delay)) == 1)
									{
//										HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);

										// Key released before long press delay
										self->state = KEY_IDLE;

										uint8_t test_key_1 = self->test_key;
										self->test_key = 0;
										self->keyPress = 0;
										self->nonKeyPress = 0;

										self->keyPress_debounceKey = false;

										return test_key_1;
									}
			   	   	   	   	   	}

								else if (key == 0)
								{
									// Key released before long press delay
//									self->state = KEY_IDLE;

									if( (self->test_key == self->lastKey) && (self->keyPress >= VALIDKEYPRESS_THRESHOLD) )
									{

										#if delay_keypad == 1

										   if(self->settings_stream2[0].keypress_tone == Yes)
										   {
//											  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
											  self->timerFlag_tone = 1;
											  self->tone_duration = 70;
										   }

//										   HAL_Delay(keypad_delay);

										   self->keyPress_debounceKey = true;

										#endif
									}
									else
									{
										self->test_key = 0;
										self->keyPress = 0;
									    self->nonKeyPress = 0;

									    self->lastDebounceTime = currentTime;
										self->keyPressStartTime = currentTime;

										self->state = KEY_IDLE;

									    return 0;
									}
								}
								else if ((currentTime - self->keyPressStartTime) > LONG_PRESS_DELAY)
								{
									self->state = KEY_LONG_PRESS;
								}
								break;

		  case KEY_LONG_PRESS:
			  	  	  	  	  	if (key == 0)
								{
									// Key released after long press delay
									self->state = KEY_IDLE;

									if( (self->test_key == self->lastKey) && (self->keyPress >= VALIDKEYPRESS_THRESHOLD) )
									{
										if( (self->lastKey == 15) && (eNextState2 != filling_State) ) // && (self->eLastState != filling_State)) )
											self->key_longpress_flag = 1;
										else if( (self->lastKey == 14) && (eNextState2 != filling_State) )
											self->progExit_longpress_flag = 1;
										else if( (self->lastKey == 21) && (self->settings_stream1.keypad__ == BLSKY22) )
											self->tot_longpress_flag = 1;

										self->test_key = 0;
										self->keyPress = 0;
										self->nonKeyPress = 0;

										self->lastDebounceTime = currentTime;
										self->keyPressStartTime = currentTime;

										return self->lastKey;
									}
									else
									{
										self->test_key = 0;
										self->keyPress = 0;
										self->nonKeyPress = 0;

										self->lastDebounceTime = currentTime;
										self->keyPressStartTime = currentTime;

										return 0;
									}
      	  	  	  	  	  	  	}
								break;
    }

    self->lastKey = key;  // Update the last key state

    return 0;
  }

 uint8_t debounceKey2(void)
 {
      static uint8_t self->lastKey = 0,
    		  	  	 self->test_key = 0;

      static bool keyPress = false;

      uint32_t duration = 0;

      uint8_t key = keypad_lcd2(0, key_lcd2);  //write lcd and read keypad.

      uint32_t currentTime = HAL_GetTick(); // Get the current system tick

      switch (state2)
      {
          case KEY_IDLE:
 							 if (key != 0)
 							 {
 								 state2 = KEY_DEBOUNCE;
 								 lastDebounceTime2 = currentTime;
								 keyPressStartTime2 = currentTime;
								 self->test_key = key;
 							 }
 							 break;

          case KEY_DEBOUNCE:

        	  	  	  	  	  duration = currentTime - lastDebounceTime2;

							  if (key == self->test_key)
								 keyPress2++;
							  else
								 nonKeyPress2++;

							 if (duration > DEBOUNCE_TIME_MS)
							 {
								if (key != 0)
								{
									state2 = KEY_SHORT_PRESS;
								}
								else
								{
									state2 = KEY_IDLE;
								}
							 }
							 break;

		   case KEY_SHORT_PRESS:
			   	   	   	   	   	if (keyPress == true)
			   	   	   	   	   	{
			   	   	   	   	   		if ( (delay_nonBlocking2(keypad_delay)) == 1)
									{
//										HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);

										// Key released before long press delay
										state2 = KEY_IDLE;

										uint8_t test_key_1 = self->test_key;
										self->test_key = 0;
										keyPress2 = 0;
										nonKeyPress2 = 0;

										keyPress = false;

										return test_key_1;
									}
			   	   	   	   	   	}

			   	   	   	   	   	else if (key == 0)
								{
									// Key released before long press delay
//									state2 = KEY_IDLE;

									if( (self->test_key == self->lastKey) && (keyPress2 >= VALIDKEYPRESS_THRESHOLD) )   // 6) )
									{

										#if delay_keypad == 1

										   if(self->settings_stream2[1].keypress_tone == Yes)
										   {
//											  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
											  timerFlag_tone2 = 1;
											  tone_duration2 = 70;
										   }

//										   HAL_Delay(keypad_delay);

										   keyPress = true;

										#endif
									}
									else
									{
										self->test_key = 0;
										keyPress2 = 0;
										nonKeyPress2 = 0;

										lastDebounceTime2 = currentTime;
									    keyPressStartTime2 = currentTime;

									    state2 = KEY_IDLE;

										return 0;
									}
								}
								else if ((currentTime - keyPressStartTime2) > LONG_PRESS_DELAY)
								{
									state2 = KEY_LONG_PRESS;
								}
								break;

		  case KEY_LONG_PRESS:
								if (key == 0)
								{
									// Key released after long press delay
									state2 = KEY_IDLE;

									if( (self->test_key == self->lastKey) && (keyPress2 >= VALIDKEYPRESS_THRESHOLD) )  // 6) )
									{
										if( (self->lastKey == 15) && (self->eNextState != filling_State) ) // && (self->eLastState != filling_State)) )
											key_longpress_flag2 = 1;
										else if( (self->lastKey == 14) && (self->eNextState != filling_State) )
											progExit_longpress_flag2 = 1;
										else if( (self->lastKey == 21) && (self->settings_stream1[1].keypad__ == BLSKY22) )
											tot_longpress_flag2 = 1;

										self->test_key = 0;
										keyPress2 = 0;
										nonKeyPress2 = 0;

										lastDebounceTime2 = currentTime;
										keyPressStartTime2 = currentTime;

										return self->lastKey;
									}
									else
									{
										self->test_key = 0;
										keyPress2 = 0;
										nonKeyPress2 = 0;

										lastDebounceTime2 = currentTime;
										keyPressStartTime2 = currentTime;

										return 0;
									}
								}
								break;
    }

    self->lastKey = key;  // Update the last key state

    return 0;
  }

uint8_t delay_nonBlocking(Nozzle *self, uint32_t delayTime)
{
   static uint32_t self->startTime_delayNonBlocking = 0;
   static uint8_t self->firstEntry_delayNonBlocking = 0;

   uint32_t currentTime = HAL_GetTick(); // Get the current system tick

   if(self->firstEntry_delayNonBlocking == 0)
   {
	   self->startTime_delayNonBlocking = currentTime;
	   self->firstEntry_delayNonBlocking = 1;

	   return 0;
   }

   if(self->firstEntry_delayNonBlocking == 1)
   {
	   if((currentTime - self->startTime_delayNonBlocking) >= delayTime)
	   {
		   self->firstEntry_delayNonBlocking = 0;
		   self->startTime_delayNonBlocking = currentTime;
		   return 1;
	   }
	   else
	   {
		   return 0;
	   }
   }
}

uint8_t delay_nonBlocking2(uint32_t delayTime)
{
   static uint32_t startTime = 0;
   static uint8_t firstEntry = 0;

   uint32_t currentTime = HAL_GetTick(); // Get the current system tick

   if(firstEntry == 0)
   {
	   startTime = currentTime;
	   firstEntry = 1;

	   return 0;
   }

   else if(firstEntry == 1)
   {
	   if((currentTime - startTime) >= delayTime)
	   {
		   firstEntry = 0;
		   startTime = currentTime;
		   return 1;
	   }
	   else
	   {
		   return 0;
	   }
   }
}


void sendStructData(Nozzle *self)
{
	char dateTime [30],
		 strr___ [30];

	tranx_savee.transaction_type = 'a';
	tranx_savee.storage_loc = 'i';

	strcpy(tranx_savee.device_id, device_id);

	strcpy(tranx_savee.tag, "null");

	self->tranx_token = generate_tk();

	tranx_savee.token = self->tranx_token;

	//============================================================================================

	snprintf(strr___, sizeof(strr___), "%02d-%02d-20%d | %02d%02d Hrs",
			self->log_new.date._dd, self->log_new.date._mm, self->log_new.date._yy,
			self->log_new.time_e._hh, self->log_new.time_e._mn);
	strcpy(dateTime, strr___);

	//============================================================================================

	tranx_savee.timestamp = self->log_new.timestamp;
	strcpy(tranx_savee.transaction_id, self->log_new.transaction_id);
	tranx_savee.transaction_price = self->log_new.pr__;
	tranx_savee.transaction_vol = self->log_new.vol__;
	tranx_savee.litre_price = self->log_new.litre_price_;
	tranx_savee.totalizer = self->log_new.totaliserVol_cal;
	tranx_savee.nozzle_address = 1;
	strcpy(tranx_savee.nozzle_product, self->log_new.nozzle_product);
	tranx_savee.transaction_period = self->log_new.transaction_period;
	strncpy(tranx_savee.session_id, self->attendant.session_id, 9);
	strcpy(tranx_savee.nozzle_name, self->pumpName[0].pump_name);

	if(number == 1)
	{
		HAL_UART_Transmit (&huart3, "\n\n\n\n", 4, 1000);
		HAL_Delay(1);
	}

	 sprintf(ep_messagee,
					"No. %lu  ==>  {\"ep\":2,\"di\":\"%s\",\"tk\":%lu,\"tm\":%s,\"ti\":\"%s\",\"ta\":%0.3f,\"tv\":%0.3f,\"pl\":%0.3f,\"tz\":%0.3f,\"pm\":\"%s\",\"pa\":%d,\"pr\":\"%s\",\"tt\":\'%c\',\"tp\":%u,\"sl\":\'%c\',\"tg\":\"%s\",\"ci\":\"%s\"}\r\n\r\n",
					number++,
					tranx_savee.device_id, tranx_savee.token, dateTime,   //tranx_savee.timestamp,  "tm\":%lu,
					tranx_savee.transaction_id, tranx_savee.transaction_price,
					tranx_savee.transaction_vol, tranx_savee.litre_price,
					tranx_savee.totalizer, tranx_savee.nozzle_name,
					tranx_savee.nozzle_address, tranx_savee.nozzle_product,
					tranx_savee.transaction_type, tranx_savee.transaction_period,
					tranx_savee.storage_loc, tranx_savee.tag, tranx_savee.session_id);

 	HAL_UART_Transmit (&huart3, ep_messagee, strlen(ep_messagee), 1000);
}

void sendStructData2(void)
{
	char dateTime [30],
		 strr___ [30];

	tranx_savee.transaction_type = 'a';
	tranx_savee.storage_loc = 'i';
//					strcpy(tranx_save.session_id, " ");

	strcpy(tranx_savee.device_id, device_id);

	strcpy(tranx_savee.tag, "null");

	tranxB_token = generate_tk();

	tranx_savee.token = tranxB_token;

	//============================================================================================

	snprintf(strr___, sizeof(strr___), "%02d-%02d-20%d | %02d%02d Hrs",
			log_b_new.date._dd, log_b_new.date._mm, log_b_new.date._yy,
			log_b_new.time_e._hh, log_b_new.time_e._mn);
	strcpy(dateTime, strr___);

	//============================================================================================

	tranx_savee.timestamp = log_b_new.timestamp;
//		strcpy(tranx_save.device_id, log_b_new.device_id);
	strcpy(tranx_savee.transaction_id, log_b_new.transaction_id);
	tranx_savee.transaction_price = log_b_new.pr__;
	tranx_savee.transaction_vol = log_b_new.vol__;
	tranx_savee.litre_price = log_b_new.litre_price_;
	tranx_savee.totalizer = log_b_new.totaliserVol_cal;
//		strcpy(tranx_save.nozzle_name, log_b_new.nozzle_name);
	tranx_savee.nozzle_address = 2;
	strcpy(tranx_savee.nozzle_product, log_b_new.nozzle_product);
	tranx_savee.transaction_period = log_b_new.transaction_period;
	strncpy(tranx_savee.session_id, attendant2.session_id, 9);
	strcpy(tranx_savee.nozzle_name, self->pumpName[1].pump_name);

	if(number == 1)
	{
		HAL_UART_Transmit (&huart3, "\n\n\n\n", 4, 1000);
		HAL_Delay(1);
	}
	 sprintf(ep_messagee,
					"No. %lu  ==>  {\"ep\":2,\"di\":\"%s\",\"tk\":%lu,\"tm\":%s,\"ti\":\"%s\",\"ta\":%0.3f,\"tv\":%0.3f,\"pl\":%0.3f,\"tz\":%0.3f,\"pm\":\"%s\",\"pa\":%d,\"pr\":\"%s\",\"tt\":\'%c\',\"tp\":%u,\"sl\":\'%c\',\"tg\":\"%s\",\"ci\":\"%s\"}\r\n\r\n",
					number++,
					tranx_savee.device_id, tranx_savee.token, dateTime,   //tranx_savee.timestamp,  "tm\":%lu,
					tranx_savee.transaction_id, tranx_savee.transaction_price,
					tranx_savee.transaction_vol, tranx_savee.litre_price,
					tranx_savee.totalizer, tranx_savee.nozzle_name,
					tranx_savee.nozzle_address, tranx_savee.nozzle_product,
					tranx_savee.transaction_type, tranx_savee.transaction_period,
					tranx_savee.storage_loc, tranx_savee.tag, tranx_savee.session_id);

 	HAL_UART_Transmit (&huart3, ep_messagee, strlen(ep_messagee), 1000);
}

//uint8_t read_event1_1(void)
//{
//	extern bool self->lock_clr;
//	   self->key19_flag = 0;
//	 //  self->auth_cmd_flag = 0;
//	 //  self->totaliser_flag = 0;
//	 //  self->keypress_flag = 0;
//
//	//READ STATES OF INPUTS PIN AND KEYPAD...
//	  self->totaliser_flag =  readtotaliser1_state();
//
//	  //self->key19_flag =  readkey19_state();
//
//	  self->key_flag =  readsettingskey_state();
//
//	  self->nozzle_flag = readNozzle1();
//
//	  self->keypress_ = self->keynew;  //key flag is also set...
//
//#ifndef DEV_MODE
//   			 //--------------------------------------------------
//			  //  totaliser error.
//				if( (self->totaliser_flag == 0) && (self->drive != ACTIVATE) )
//				{
//					self->totaliser_flag = 1;
//					return _tot_error_Event;
//				}
//   			  //--------------------------------------------------
//#endif     //#ifndef DEV_MODE
//
//
//	 //==========check for long press events.....========
//	 //==================================================
//	 //      then select the  operator  view mode...
//	 //--------------------------------------------------
//		self->tot_longpress_flag = long_press_tot();
//		self->log_longpress_flag = long_press_log();
//		self->key_longpress_flag = long_press_key();
//		self->progExit_longpress_flag = long_press_progExit();
//
//		  if(self->tot_longpress_flag == 1)
//		  {
//			   self->operatorfxn = totaliser_view;
//			   return _operator_Event;
//		  }
//
//		  else if(self->log_longpress_flag == 1)
//		  {
//			   self->operatorfxn = log_view;
//			   return _operator_Event;
//		  }
//		  else if(self->key_longpress_flag == 1)
//		  {
//			//if not previously activated,
////				 if (key_longpress_status == 0)
////				 {
////					key_longpress_status = 1;
//				self->prog_entry = 1;   //variable used to clear the var. states in settings menu.
//				return _keyup_Event;
////				 }
////				 else
////				 {
////					key_longpress_status = 0;
////					self->prog_entry = 0;
////					return _keydown_Event;
////				 }
//		  }
//		  else if(self->progExit_longpress_flag == 1)
//		  {
//			//if not previously activated,
////				 if (key_longpress_status == 0)
////				 {
////					key_longpress_status = 1;
////					self->prog_entry = 1;   //variable used to clear the var. states in settings menu.
////					return _keyup_Event;
////				 }
////				 else
////				 {
////					key_longpress_status = 0;
//					self->prog_entry = 0;
//					return _keydown_Event;
////				 }
//			  }
//
//       //--------------------------------------------------
//	   //          error clear flag...
//	   if( self->error_clr_flag == 1)
//		 {
//			self->error_clr_flag = 0;
//
//			self->filling = 0;
//
//			return _error_clear_Event;
//		 }
//	  //--------------------------------------------------
//			 // authorise  event capture.
//	   else if ( self->auth_flag  == 1 )
//		{
//			self->auth_flag = 0;
//			return _authorise_Event;
//		}
//
//		//--------------------------------------------------
//				// authorise  command event.
//	   else if ( self->auth_cmd_flag  == 1 )
//		{
//		  //if(settings[operating_side-1].mode == offline_)
//		  //{
//			self->auth_cmd_flag = 0;
//
////				self->eNextState = authorised_nozzledown_State;
//				//---------------------------------------------
//				//                nozzle-up overide
//				if (self->eNextState == authorised_nozzledown_State)
//				{
//					if (self->overide_ == overide)
//					{
//						return _nozzleup_Event;
//					}
//				}
//				//---------------------------------------------
//
//				if(self->settings_stream1.mode == AUTO_MODE)
//				{
//					if(self->nozzlezUp == 1)   //A NozzleUp that triggers a transaction
//					{
////						self->nozzlezUp = 0;
//						return _authorisecommand_Event;
//					}
//				}
//
//			  return _auth_command_Event;
//		  //}
//		}
//
//		// mamo reach  event capture...
//	   else if(self->mamo_reached_flag == 1)
//		{
//			self->mamo_reached_flag = 0;
//			return _mamo_Event;
//		}
//
//	   // idle state due-return event capture...
//	   else if (self->fillingComplete_flag == 1)
//	   {
//		   self->fillingComplete_flag = 0;
//		   return _fillingcomplete_Event;
//	   }
//	   else if (self->reset_flag == 1)
//	   {
//		   self->reset_flag = 0;
//		   return _resetcommand_Event;
//	   }
//	   else if (self->authsuspend_flag == 1)
//	   {
//		   self->authsuspend_flag = 0;
//		   return  _auth_suspendcommand_Event;
//	   }
//	   else if (self->fillingsuspend_flag == 1)
//	   {
//		   self->fillingsuspend_flag = 0;
//		   return _filling_suspendcommand_Event;
//	   }
//	   else if (self->authresume_flag == 1)
//	   {
//		   self->authresume_flag = 0;
//		   return _auth_resumecommand_Event;
//	   }
//	   else if (self->fillingresume_flag == 1)
//	   {
//		   self->fillingresume_flag = 0;
//		   return _filling_resumecommand_Event;
//	   }
//
//	   // nozzle up  event capture...
//	   else if( (self->nozzle_flag_old == 0)&&(self->nozzle_flag == 1) )
//		{
//			self->nozzle_flag_old = 1;
//			if (self->overide_ != overide)
//			{
//				//send nozzleup command only in MANUAL mode
//				self->nozzlezUp = 1;
//				return _nozzleup_Event;
//			}
//			else 		// NozzlezUp, awaiting authorisation
//			{
//
//			}
//		}
//	  //-----------------------
//	  // nozzle down  event capture...
//	   else if( (self->nozzle_flag_old == 1)&&(self->nozzle_flag == 0) )
//		{
//			self->nozzle_flag_old = 0;
//			if (self->overide_ != overide) return _nozzledown_Event;
//		}
//		//	self->nozzle_flag_old = self->nozzle_flag;
//	  //--------------------------------------------------
//	  //--------------------------------------------------
//		// key up  event capture...
//	   else if( (self->key_flag_old == 0)&&(self->key_flag == 1) )
//			{
//					self->key_flag_old = 1;
//			      return _keyup_Event;
//			}
//	  //-----------------------
//		// key down  event capture...
//	   else if( (self->key_flag_old == 1)&&(self->key_flag == 0) )
//			{
//					self->key_flag_old = 0;
//				  return _keydown_Event;
//			}
//			self->key_flag_old = self->key_flag;
//	  //--------------------------------------------------
//	  //--------------------------------------------------
//		// key press event capture...
//	   if (self->keypress_flag == 1)
//		{
//			self->keypress__ = 1;
//			self->keypress_flag = 0;
//			return _keypress_Event;
//		}
//	  //--------------------------------------------------
//	  /*	  if (self->key19_flag == 1)
//			{
//				return _key19_Event;
//			}                              */
//	  //--------------------------------------------------
//	  //  totaliser error.
////		if( (self->totaliser_flag == 0) && (self->drive != ACTIVATE) )
////		{
////			self->totaliser_flag = 1;
////			return _tot_error_Event;
////		}
//	  //--------------------------------------------------
//	   //self->filling pulse detection.
//	   if ( (self->pulser_count_old < self->pulser_new) && ( self->eNextState == authorised_nozzleup_State ) )
//			{
//				self->pulser_count_old = self->pulser_new;
//				self->lock_clr = 0;
//				return _filling_pulse_Event;
//			}
//		 self->pulser_count_old = self->pulser_new;
//
//	  //--------------------------------------------------
//			  // timeout   event capture...
//		if( (self->timer_flag_old == 0)&&(self->timer_flag == 1) )
//		{
//				  self->timer_flag_old = 1;
//
//				  self->filling = 0;
//
//			  return _timeout_Event;
//		}
//			self->timer_flag_old = self->timer_flag;
//
//	  return _no_Event;
//}
//

