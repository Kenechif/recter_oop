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

#include "pump_comm.h"

//#include "../otp/sha1.h"
#include "../otp/otp.h"
#include "stdlib.h"


//uint8_t MSG[200] = {0};
extern int8_t change_p, change_v;

extern float auth_v, auth_p;

extern w25qxx_t w25qxx;

extern UART_HandleTypeDef huart1;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;

extern I2C_HandleTypeDef hi2c1;

extern  pump_settings settings[2], copy[2];

extern pump pump_type,
 	 	 	disp_type1,
            disp_type2;

extern drive drive1,
			 drive2;

extern uint8_t hmacKey[];

//===================================================
#define DEV_ADDR 0xa0
uint8_t dataw1[] = "hello world from EEPROM";
uint8_t dataw2[] = "This is the second string for flash memory";
float dataw3 = 1234.5678;

uint8_t datar1[50];
//uint8_t datar2[100];
float datar3;

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

extern pump_names pumpName[2];


char testChar0[9] = {0};
char testChar1[9] = {0};
int sz;


int8_t val;

int t, t2 = 0;

uint16_t _tt1 = 0,
		 _tt2 = 0,
		 timer_ep = 0,
//		 timer_ep1 = 0,
		 timer_spi,
		 totalizer1Timer = 0,
		 totalizer2Timer = 0,
		 priceChange_timer1 = 0,
		 priceChange_timer2 = 0,
		 timer_config1 = 0,
		 timer_config2 = 0;
//		 ep2_timer = 0;

uint32_t timer_ep1;

uint32_t transaction_period = 0,
		 transaction_period2 = 0;

unsigned int ttt1 = 0,
			 ttt2 = 0;

uint8_t server_message_found = 0,
	    card1_message_found = 0,
	    card2_message_found = 0;

uint16_t shutdown_timer1 = 0,
		 shutdown_timer2 = 0;

char key_lcd[10], key_lcd2[10]  = {0};
uint8_t buff[30], buff2[30]  = {0};

int keynew,keyold = 0;
int keynew2,keyold2 = 0;

int32_t  count__, count__2 = 0;

uint8_t rcv[41] = {0};

int retn;
 //=====================================================================
 /*        create instances of the input and output buffers  */
 fifo_t  dat_str;    //create an instance of the buffer - input buffer.
 fifo_t  dat_strout; //create an instance of the buffer - output buffer.

 //=====================variables from master===========================
 int totaliser_flag, totaliser_flag2 = 0;
 int key19_flag ,key19_flag2 = 0;
 int auth_cmd_flag , auth_cmd_flag2 = 0;
 nozzle_overide overide_ , overide_2;
 int stop_flag ,stop_flag2 = 0;
 int error_clr_flag , error_clr_flag2 = 0;

 int key_flag, key_flag_old , key_flag2, key_flag_old2 = 0;
 int nozzle_flag, nozzle_flag_old , nozzle_flag2, nozzle_flag_old2 = 0;
 uint32_t pulser_count_old , pulser_new ,pulser_count_old2 , pulser_new2 = 0;
 extern int timer_flag_old , timer_flag , timer_flag_old2 , timer_flag2 ;

 int key_longpress_status,key_longpress_status2 = 0;

 extern uint8_t filling1 , filling2;

 extern int auth_flag , auth_flag2;
 extern int lat_cnt , lat_cnt2;

 int keypress_flag ,keypress_flag2 = 0;
 int keypress__ , keypress__2 = 1;
 char wrt_[10],wrt_2[10]  = {0};

 uint8_t prog_entry1, prog_entry2 = 0;

 char keyboard[7], keyboard2[7];
 //reference variables

 extern char str_[9] , str_2[9];                 //used in states.c
 extern char keyboard_entry[8] , keyboard_entry2[7];
 extern int keypress_ , keypress_2;
 extern int index_ , index_2;
 extern char upper1[10] , upper2[10];
 extern uint32_t target_pulser1 , current_pulser1 ,target_pulser2 , current_pulser2;
 extern float key_value ,key_value2;
 extern int index2 , index2;
 extern char keypad_pw[10],keypad_pw2[10];
 extern char keypad_pw_[10] , keypad_pw_2[10] ;

 extern int auth;      //first set this to
 extern int side;      //pump side selected.
 extern int level ;
extern int _index , _index2;

extern char sc1[10] , sc12[10];

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

extern float working_volTotaliser1;
extern float working_volTotaliser1c;

extern float working_volTotaliser2;
extern float working_volTotaliser2c;

extern float working_amtTotaliser1;
extern float working_amtTotaliser1c;

extern float working_amtTotaliser2;
extern float working_amtTotaliser2c;

int tot_longpress_flag,
	log_longpress_flag,
	key_longpress_flag = 0,
	progExit_longpress_flag = 0;

int tot_longpress_flag2,
	log_longpress_flag2,
	key_longpress_flag2 = 0,
	progExit_longpress_flag2 = 0;

extern operatorfxn_  operatorfxn , operatorfxn2;

 extern float original_pulse , original_pulse2;
 extern int operating_side ;

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
	extern log_new log_a_new , log_b_new;
	extern uint32_t flash_read_idA;
	extern uint32_t flash_read_idB;

	extern const uint32_t flash_beginA;
	extern const uint32_t flash_endA;

	extern const uint32_t flash_beginB;
	extern const uint32_t flash_endB;

	extern uint8_t prog_entry1;
	extern uint8_t prog_entry2;

	    sprintf(print_struct_.name,"RECTER");
		sprintf(print_struct_.adr,"Ilupeju Bypass Ilupeju, Lagos");
		sprintf(print_struct_.date_,"Thur Oct ,%d , 2022",log_a_new.date._dd);
		sprintf(print_struct_.time_," %d : %d ", log_a_new.time_e._hh, log_a_new.time_e._mn);
		sprintf(print_struct_.transaction_type,"USSD");
		sprintf(print_struct_.voucher_,"0a2345ba");
		sprintf(print_struct_.product_,"AGO");
		sprintf(print_struct_.pn,"A2");
		sprintf(print_struct_.ta,"%2f",log_a_new.pr_);  //price);
		sprintf(print_struct_.tv,"%2f",log_a_new.vol_); //amt);
		sprintf(print_struct_.pl,"180.00");

		printer_status = 1;
}

//char sendATcommand(char* cmd)
//{
//    HAL_UART_Transmit(&huart1, (uint8_t *)cmd, strlen(cmd), 1000);
//}

//-------------------------------------------------------------
 void setup()
{
	 pump_status_ = STATUS_UNKNOWN;

	 pump1_status_4G = STATUS_PUMP_ON;

//	 ep_send(ep0);

	 mcu_power(ACTIVATE);

	 batt_charge(ACTIVATE);

	 displayandkeypad_power(ACTIVATE);   //latch power...

	 modem_power(ACTIVATE);

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


//	srand(time(NULL));

    keypad_lcd(2,"0");                      //keypad_lcd(0,"0");
    keypad_lcd2(2,"0");

	buffer_init(&dat_str,buffer_length);    //initialise the buffer

	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_1 |TIM_CHANNEL_2  ); //TIM_CHANNEL_ALL); //start encoder acquinsition.
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);  //TIM_CHANNEL_1 |TIM_CHANNEL_2); //TIM_CHANNEL_ALL);

	//keypad_ini();
	//   uint16_t CounterTicks1 = 0;
	//   uint16_t CounterTicks2 = 0;
	//------------------------------
	//initialise realtime clock
	    setup_ds1307();
	    get_time();
	//  set_time();
	//-----------------------------


//	read_config();

	clear_screen1();         //	clear_screen2();  //nextion1_ini();
	clear_screen2();

//	pump_ini();    // activate the pump communication I/O

	HAL_UART_Receive_IT(&huart1, uart1_rx_buf, pump_rx_bufsize);

	HAL_UART_Receive_IT(&huart2, uart2_rx_buf, pump_rx_bufsize);

	HAL_UART_Receive_IT(&huart3, uart3_rx_buf, pump_rx_bufsize);

	HAL_UART_Receive_IT(&huart5, uart5_rx_buf, pump_rx_bufsize);

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
	printDisp_c("fuel",1,2,4,LT,CLEAR);
	printDisp_c("nnetrics",2,0,8,LT,CLEAR);
	//}
	printDisp_c2("fuel",1,2,4,LT,CLEAR);
	printDisp_c2("nnetrics",2,0,8,LT,CLEAR);

	HAL_Delay(2500);

   	reset_timer(15);
	//start_timer(15);

   //======= initialize ========
   eNextState1 = idle_State;
   eNextState2 = idle_State;

//   state_ini();
//   state_ini2();

   clr_screen1();
   clr_screen2();

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


//  if( HAL_GPIO_ReadPin(nLed_GPIO_Port, nLed_Pin) == 1)  // config
  if( (HAL_GPIO_ReadPin(settings1_GPIO_Port, settings1_Pin) == 1 ) || ( HAL_GPIO_ReadPin(settings2_GPIO_Port, settings2_Pin) == 1) )
  {
	  printDisp_c("config",1,2,4,LT,CLEAR);

//	  settings[0].passwd1 = 0000;
//	  settings[0].passwd2 = 0000;
//	  settings[0].passwd1 = 0000;
//	  settings[0].passwd2 = 0000;



	 //==============================================
	 //    This step is to compose the settings.
	 //==============================================
	 make_settings(side_a);
	 make_settings(side_b);

	 save_settings();
//	 save_volumeTotaliser(side_a); //side_a
//	 save_volumeTotaliser(side_b);
//	 flash_infoA.current_loc = 0;
//	 flash_infoA.number_logs = 0;
//	 EEPROM_Write(flash_info_sto, flash_stoA, &flash_infoA, sizeof(flash_infoA));
//	 EEPROM_Write(flash_info_sto, flash_stoA, &flash_infoB, sizeof(flash_infoA));
	 // ===========================================================================


	  config_mode = 1;
//	  retrieve_settings();

//	  HAL_UART_Transmit (&huart1, "Hey!", 4, 1000);

//	  {"ni":"p17","pn":"pms","dt":"bluesky886n","kt":"bluesky22"}

//	  {"ni":"p17","pn":"pms","dt":"bluesky886i","kt":"bluesky22"}

//	 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
//	 HAL_Delay(1);
//	 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
//     HAL_Delay(1);

	  while(config_found == 0)   //Loops idly while configuration is yet to be inputted
	  {
		 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
		 HAL_GPIO_TogglePin(nLed_GPIO_Port, nLed_Pin);  // Toggle LED
		 HAL_Delay(200);
		 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
		 HAL_GPIO_TogglePin(nLed_GPIO_Port, nLed_Pin);  // Toggle LED
		 HAL_Delay(1000);
	  }

	  config_rx_parse();
	  pumpType_configure();

	  // ===========================================================================


	  //********************  INITIALIZATIONS ***********************//

	  //----------------------//
	  firstTotalizer_day();
	  //----------------------//

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

	  clear_totalTransaction_sides(side_a);
	  clear_totalTransaction_sides(side_b);

	  clear_synchedTransaction_sides(side_a);
	  clear_synchedTransaction_sides(side_b);

	  clear_totalAutoTransaction_sides(side_a);
	  clear_totalAutoTransaction_sides(side_b);

	  clear_volumeTotaliser(side_a);
	  clear_volumeTotaliser(side_b);

	  clear_amountTotaliser(side_a);
	  clear_amountTotaliser(side_b);

	  clear_lastSale(side_a);
	  clear_lastSale(side_b);

	  clear_1stvolTotaliser_day(side_a);
	  clear_1stvolTotaliser_day(side_b);

	  clear_ctSettings(side_a);
	  clear_ctSettings(side_b);

	  clear_calibrationPulser(side_a);
	  clear_calibrationPulser(side_b);

	  clear_sessionId(side_a);
	  clear_sessionId(side_b);

	  clear_logA();
	  clear_logB();

	  HAL_GPIO_WritePin(nLed_GPIO_Port, nLed_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);

	  W25qxx_EraseChip();

	  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);

	// ===========================================================================


	  // ===========================================================================
		 //==============================================
		 //    This step is to compose the settings.
		 //==============================================
		 make_settings(side_a);
		 make_settings(side_b);


		 vol_real1 = 20;
		 vol_real2 = 20;
		 vol_calibrated1 = 21.0;
		 vol_calibrated2 = 21.0;
		 vol_effective1 = 21.0;
		 vol_effective2 = 21.0;

		 calib_pulser1 =  (settings[0].pi_c * vol_calibrated1);
		 calib_pulser2 =  (settings[1].pi_c * vol_calibrated2);

		 save_settings();

		 save_ctSettings(side_a);
		 save_ctSettings(side_b);
		 save_calibrationPulser(side_a);
		 save_calibrationPulser(side_b);

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
//		 strncpy(attendant1.session_id, testChar1, sizeof(attendant1.session_id) );


    retrieve_settings();         //read pump settings from eeprom.


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


	pumpType_write();
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


    settings[0].mode = MANUAL;  //AUTO;  //MANUAL;
    settings[1].mode = MANUAL;  //AUTO;   //MANUAL;


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

    pumpName_parse();

    strcpy(pumpName[0].pump_name, "P17");
    strcpy(pumpName[1].pump_name, "P18");

    retrieve_volumeTotaliser(side_a);
    retrieve_volumeTotaliser(side_b);

    retrieve_amountTotaliser(side_a);
    retrieve_amountTotaliser(side_b);

    retrieve_lastSale(side_a);
    retrieve_lastSale(side_b);

    retrieve_totalTransaction_sides(side_a);
    retrieve_totalTransaction_sides(side_b);

    retrieve_synchedTransaction_sides(side_a);
    retrieve_synchedTransaction_sides(side_b);

    retrieve_totalAutoTransaction_sides(side_a);
    retrieve_totalAutoTransaction_sides(side_b);

    retrieve_synchedAutoTransaction_sides(side_a);
    retrieve_synchedAutoTransaction_sides(side_b);

//    clear_1stvolTotaliser_day(side_a);
//    clear_1stvolTotaliser_day(side_b);
    retrieve_1stVolTotaliser_day(side_a);
    retrieve_1stVolTotaliser_day(side_b);

    retrieve_ctSettings(side_a);
    retrieve_ctSettings(side_b);

    retrieve_calibrationPulser(side_a);
    retrieve_calibrationPulser(side_b);

    retrieve_ctTimedSettings(side_a);
    retrieve_ctTimedSettings(side_b);

    retrieve_ctTimedFlag(side_a);
    retrieve_ctTimedFlag(side_b);

//    clear_sessionId(side_a);
//    clear_sessionId(side_b);

    retrieve_sessionId(side_a);
    retrieve_sessionId(side_b);

//    clear_volumeTotaliser_startShift(side_a);
//    clear_volumeTotaliser_startShift(side_b);
    retrieve_volumeTotaliser_startShift(side_a);
    retrieve_volumeTotaliser_startShift(side_b);
//    startShiftTotaliser_vol1c = 500;
//    startShiftTotaliser_amt1c = 500;



//	settings[0].max_amt_ = 1000;
//	settings[1].max_amt_ = 1000;
////	settings[0].pi_ = 180;
////	settings[0].pi_c = 180;
//	make_settings(side_a);
//	make_settings(side_b);



//    retrieve_totalEvents();
//    retrieve_synchedEvents();

    ep0_save.boot_time = RtcToInt(2019);

    day = DS1307_GetDate();

	if(settings[0].totalizer_day != day)
	{
		save_1stVolTotaliser_day(side_a);
		save_1stVolTotaliser_day(side_b);
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

   flash_info_read();
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
 }

 //-----------------------------------------------------------
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
 	          Multiplex(0,0);

 	    	  keynew = keypad_lcd(0,key_lcd);  //write lcd and read keypad.
 			  if ( (keyold == 0)&&(keyold != keynew) )  //send key only if new key is pressed
 				  {
 					 //send_keyboard();
 				     keypress_flag = 1;  //indicate that a new press was detected.
 				  }
 			  keyold = keynew;
 }

void house_keeping2()
{
    	Multiplex2(0,0);

			  keynew2 = keypad_lcd2(0,key_lcd2);  //write lcd and read keypad.
			  if ( (keyold2 == 0)&&(keyold2 != keynew2) )  //send key only if new key is pressed
				  {
					 //send_keyboard();
					 keypress_flag2 = 1;  //indicate that a new press was detected.
				  }
			  keyold2 = keynew2;
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


	if(server_message_found == 1)
	{
		server_rx_parse();
		server_message_found = 0;
	}
	if(card1_message_found == 1)
	{
		card1_rx_parse();
		card1_message_found = 0;
	}
	else if(card2_message_found == 1)
	{
		card2_rx_parse();
		card2_message_found = 0;
	}
//	epSend_interval();

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

	//============================================//
	// 				EP's ROUTINE SENDING			  //
	//============================================//
	if (HAL_GPIO_ReadPin(network_connected_GPIO_Port, network_connected_Pin) == 1 )
	{
		connected = 1;
		epSend_interval();
	}
	else
	{
		connected = 0;
	}
	//============================================//


	if(operating_sideA)
	{
		 operating_side = side_a;
		 house_keeping();
		 states();

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

	if(server_message_found == 1)
	{
		server_rx_parse();
		server_message_found = 0;
	}
	if(card1_message_found == 1)
	{
		card1_rx_parse();
		card1_message_found = 0;
	}
	else if(card2_message_found == 1)
	{
		card2_rx_parse();
		card2_message_found = 0;
	}
	else if( (pump_message_found == 1)  && (awaiting_masterResponse == 0) )
	{
		 pump_message_found = 0;
		 uint8_t res = msg_parse_pump( pump_buf );
		 awaiting_masterResponse = 1;
		 _tt1 = 0;
	}
	else if( (pump_message_found == 1)  && (awaiting_masterResponse == 1) )
	{
		 pump_message_found = 0;
		 awaiting_masterResponse = 0;

	}
	else if(awaiting_masterResponse == 1)
	{
		 if(_tt1 > 2000)
			 send_line3("err5 ");
	}

	#if sense_battery == 1
		if( battery_read() < 1.81 )   //1.81V @ 6.4V Low_cutOff
		{

		}
	#endif    //#if sense_battery
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
	extern bool lock_clr;
		 	   key19_flag = 0;
		 	 //  auth_cmd_flag = 0;
	     	 //  totaliser_flag = 0;
		 	 //  keypress_flag = 0;

            //READ STATES OF INPUTS PIN AND KEYPAD...
           	  totaliser_flag =  readtotaliser1_state();

			  //key19_flag =  readkey19_state();

	          key_flag =  readsettingskey_state();

	          nozzle_flag = readNozzle1();

   			  keypress_ = keynew;  //key flag is also set...


   			 //--------------------------------------------------
			  //  totaliser error.
				if( (totaliser_flag == 0) && (drive1 != ACTIVATE) )
				{
					totaliser_flag = 1;
					return _tot_error_Event;
				}
   			  //--------------------------------------------------



	 //==========check for long press events.....========
	 //==================================================
	 //      then select the  operator  view mode...
	 //--------------------------------------------------
   			tot_longpress_flag = long_press_tot();
   			log_longpress_flag = long_press_log();
   			key_longpress_flag = long_press_key();
   			progExit_longpress_flag = long_press_progExit();

   			  if(tot_longpress_flag == 1)
			  {
				   operatorfxn = totaliser_view;
				   return _operator_Event;
			  }

   			  else if(log_longpress_flag == 1)
			  {
				   operatorfxn = log_view;
				   return _operator_Event;
			  }
   			  else if(key_longpress_flag == 1)
			  {
				//if not previously activated,
//				 if (key_longpress_status == 0)
//				 {
//					key_longpress_status = 1;
					prog_entry1 = 1;   //variable used to clear the var. states in settings menu.
					return _keyup_Event;
//				 }
//				 else
//				 {
//					key_longpress_status = 0;
//					prog_entry1 = 0;
//					return _keydown_Event;
//				 }
			  }
   			  else if(progExit_longpress_flag == 1)
			  {
				//if not previously activated,
//				 if (key_longpress_status == 0)
//				 {
//					key_longpress_status = 1;
//					prog_entry1 = 1;   //variable used to clear the var. states in settings menu.
//					return _keyup_Event;
//				 }
//				 else
//				 {
//					key_longpress_status = 0;
					prog_entry1 = 0;
					return _keydown_Event;
//				 }
			  }

       //--------------------------------------------------
	   //          error clear flag...
	   if( error_clr_flag == 1)
		 {
			error_clr_flag = 0;

			filling1 = 0;

			return _error_clear_Event;
		 }
	  //--------------------------------------------------
			 // authorise  event capture.
		if ( auth_flag  == 1 )
		{
			auth_flag = 0;
			return _authorise_Event;
		}
		//--------------------------------------------------
				// authorise  command event.
		if ( auth_cmd_flag  == 1 )
			{
			  //if(settings[operating_side-1].mode == offline_)
			  //{
				auth_cmd_flag = 0;
					//---------------------------------------------
					//                nozzle-up overide
					if (eNextState1 == authorised_nozzledown_State)
					{
						if (overide_ == overide)
						{
							return _nozzleup_Event;
						}
					}
					//---------------------------------------------
				  return _auth_command_Event;
			  //}
			}

	   // nozzle up  event capture...
			if( (nozzle_flag_old == 0)&&(nozzle_flag == 1) )
			{
					nozzle_flag_old = 1;
					if (overide_ != overide)
					{
								//send nozzleup command only in MANUAL mode
								return _nozzleup_Event;
					}
					else 		// NozzlezUp, awaiting authorisation
					{

					}
			}
	  //-----------------------
	  // nozzle down  event capture...
			if( (nozzle_flag_old == 1)&&(nozzle_flag == 0) )
			{
					nozzle_flag_old = 0;
					if (overide_ != overide) return _nozzledown_Event;
			}
		//	nozzle_flag_old = nozzle_flag;
	  //--------------------------------------------------
	  //--------------------------------------------------
		// key up  event capture...
		if( (key_flag_old == 0)&&(key_flag == 1) )
			{
					key_flag_old = 1;
			      return _keyup_Event;
			}
	  //-----------------------
		// key down  event capture...
		if( (key_flag_old == 1)&&(key_flag == 0) )
			{
					key_flag_old = 0;
				  return _keydown_Event;
			}
			key_flag_old = key_flag;
	  //--------------------------------------------------
	  //--------------------------------------------------
		// key press event capture...
		if (keypress_flag == 1)
			{
			    keypress__ = 1;
				keypress_flag = 0;
				return _keypress_Event;
			}
	  //--------------------------------------------------
	  /*	  if (key19_flag == 1)
			{
				return _key19_Event;
			}                              */
	  //--------------------------------------------------
	  //  totaliser error.
//		if( (totaliser_flag == 0) && (drive1 != ACTIVATE) )
//		{
//			totaliser_flag = 1;
//			return _tot_error_Event;
//		}
	  //--------------------------------------------------
	   //filling1 pulse detection.
		if ( (pulser_count_old < pulser_new) && ( eNextState1 == authorised_nozzleup_State ) )
			{
				pulser_count_old = pulser_new;
				lock_clr = 0;
				return _filling_pulse_Event;
			}
		 pulser_count_old = pulser_new;

	  //--------------------------------------------------
			  // timeout   event capture...
		if( (timer_flag_old == 0)&&(timer_flag == 1) )
		{
				  timer_flag_old = 1;

				  filling1 = 0;

			  return _timeout_Event;
		}
			timer_flag_old = timer_flag;

	  return _no_Event;
}

//==============================================================
int  read_event2()
{
	// return _no_Event;

	  extern bool lock_clr2;
		 	   key19_flag2 = 0;
		 	 //  auth_cmd_flag = 0;
	     	 //  totaliser_flag = 0;
		 	 //  keypress_flag = 0;

            //READ STATES OF INPUTS PIN AND KEYPAD...
           	  totaliser_flag2 =  readtotaliser2_state();

			  //key19_flag =  readkey19_state();

	          key_flag2 =  readsettingskey2_state();

	          nozzle_flag2 = readNozzle2();

   			  keypress_2 = keynew2;  //key flag is also set...


   			//--------------------------------------------------
			  //  totaliser error.
				if( (totaliser_flag2 == 0) && (drive2 != ACTIVATE) )
				{
					totaliser_flag2 = 1;
					return _tot_error_Event;
				}
		    //--------------------------------------------------



	 //==========check for long press events.....========
	 //==================================================
	 //      then select the  operator  view mode...
	 //--------------------------------------------------
   			tot_longpress_flag2 = long_press_tot2();
   			log_longpress_flag2 = long_press_log2();
   			key_longpress_flag2 = long_press_key2();
   			progExit_longpress_flag2 = long_press_progExit2();

   		  if(tot_longpress_flag2 == 1)
		  {
			   operatorfxn2 = totaliser_view;
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
		if ( auth_flag2  == 1 )
		{
			auth_flag2 = 0;
			return _authorise_Event;
		}
		//--------------------------------------------------
				// authorise  command event.
		if ( auth_cmd_flag2  == 1 )
			{
				auth_cmd_flag2 = 0;
				//---------------------------------------------
				//                nozzle-up overide
				if (eNextState2 == authorised_nozzledown_State)
				{
					if (overide_2 == overide)
					{
						return _nozzleup_Event;
					}
				}
				//---------------------------------------------
				return _auth_command_Event;
			}

	   // nozzle up  event capture...
			if( (nozzle_flag_old2 == 0)&&(nozzle_flag2 == 1) )
			{
					nozzle_flag_old2 = 1;
					if (overide_2 != overide) return _nozzleup_Event;
			}
	  //-----------------------
	  // nozzle down  event capture...
			if( (nozzle_flag_old2 == 1)&&(nozzle_flag2 == 0) )
			{
					nozzle_flag_old2 = 0;
					if (overide_2 != overide) return _nozzledown_Event;
			}
		//	nozzle_flag_old = nozzle_flag;
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
	  /*	  if (key19_flag == 1)
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
	   //filling1 pulse detection.
		if ( (pulser_count_old2 < pulser_new2) && ( eNextState2 == authorised_nozzleup_State ) )
			{
				pulser_count_old2 = pulser_new2;
				lock_clr2 = 0;
				return _filling_pulse_Event;
			}
		 pulser_count_old2 = pulser_new2;

	  //--------------------------------------------------
			  // timeout   event capture...
		if( (timer_flag_old2 == 0)&&(timer_flag2 == 1) )
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
	  if((settings[0].display__ == LAFNG885) && (settings[0].keypad__ == LAFNG17_K))  //LAFENG885-NormalScreen | LAFENG16-Keypad
	  {
		  EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000001);
	  }
	  else if((settings[0].display__ == BLSKY886_N) && (settings[0].keypad__ == BLSKY18_K))  //BLUESKY886-NormalScreen | BLUESKY18K-Keypad
	  {
		  EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000010);
	  }
	  else if((settings[0].display__ == BLSKY886_N) && (settings[0].keypad__ == BLSKY22))  //BLUESKY886-NormalScreen | BLUESKY22-Keypad
	  {
		  EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000011);
	  }
	  else if((settings[0].display__ == BLSKY886_IN) && (settings[0].keypad__ == BLSKY18_K))  //BLUESKY886-InvertedScreen | BLUESKY18K-Keypad
	  {
		  EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000100);
	  }
	  else if((settings[0].display__ == BLSKY886_IN) && (settings[0].keypad__ == BLSKY22))  //BLUESKY886-InvertedScreen | BLUESKY22-Keypad
	  {
		  EEPROM_Write_NUM (save_pumpType_loc, 0, 0b00000101);
	  }



		if(strcmp(settings[0].product_, "PMS") == 0)
		{
			EEPROM_Write_NUM (save_productType_loc, 0, PMS);
		}
		else if(strcmp(settings[0].product_, "AGO") == 0)
		{
			EEPROM_Write_NUM (save_productType_loc, 0, AGO);
		}
		else if(strcmp(settings[0].product_, "DPK") == 0)
		{
			EEPROM_Write_NUM (save_productType_loc, 0, DPK);
		}

		EEPROM_Write_NUM (save_nozzleId_loc, 0, settings[0].noz_id);
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
	  if(settings[0].pump_type_ == DN_LAFNG17K)
	  {
		  settings[0].display__ = LAFNG885;
		  settings[0].keypad__ = LAFNG17_K;
	  }
	  else if(settings[0].pump_type_ == DN_BLSKY18K)
	  {
		  settings[0].display__ = BLSKY886_N;
		  settings[0].keypad__ = BLSKY18_K;
	  }
	  else if(settings[0].pump_type_ == DN_BLSKY22)
	  {
		  settings[0].display__ = BLSKY886_N;
		  settings[0].keypad__ = BLSKY22;
	  }
	  else if(settings[0].pump_type_ == DIN_BLSKY18K)
	  {
		  settings[0].display__ = BLSKY886_IN;
		  settings[0].keypad__ = BLSKY18_K;
	  }
	  else if(settings[0].pump_type_ == DIN_BLSKY22)
	  {
		  settings[0].display__ = BLSKY886_IN;
		  settings[0].keypad__ = BLSKY22;
	  }

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
	settings[0].pump_type_ = EEPROM_Read_NUM (save_pumpType_loc, 0);
	settings[1].pump_type_ = EEPROM_Read_NUM (save_pumpType_loc, 0);
	disp_type1 = settings[0].pump_type_;
	disp_type2 = settings[1].pump_type_;
}



void firstTotalizer_day(void)
{
	 EEPROM_Write_NUM (totalizerDay_loc, 0, (DS1307_GetDate() - 1));
}

void firstTotalizerDay_write(void)
{
	settings[0].totalizer_day = EEPROM_Read_NUM (totalizerDay_loc, 0);
	settings[1].totalizer_day = EEPROM_Read_NUM (totalizerDay_loc, 0);
}



void pumpName_parse(void)
{
	 	char str[5];

		int8_t productType;

		settings[0].noz_id = EEPROM_Read_NUM (save_nozzleId_loc, 0);

		productType = EEPROM_Read_NUM (save_productType_loc, 0);

	 	if( productType == PMS)
	 	{
	 		strcpy(settings[0].product_, "PMS");

	 		snprintf(str, 5,"P%d", settings[0].noz_id);
			strcpy(pumpName[0].pump_name, str);
			snprintf(str, 5, "P%d", (settings[0].noz_id + 1));
			strcpy(pumpName[1].pump_name, str);
	 	}

		else if( productType == AGO)
		{
	 		strcpy(settings[0].product_, "AGO");

	 		snprintf(str, 5,"A%d", settings[0].noz_id);
			strcpy(pumpName[0].pump_name, str);
			snprintf(str, 5, "A%d", (settings[0].noz_id + 1));
			strcpy(pumpName[1].pump_name, str);
		}
		else if( productType == DPK)
		{
	 		strcpy(settings[0].product_, "DPK");

	 		snprintf(str, 5,"D%d", settings[0].noz_id);
			strcpy(pumpName[0].pump_name, str);
			snprintf(str, 5, "D%d", (settings[0].noz_id + 1));
			strcpy(pumpName[1].pump_name, str);
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
