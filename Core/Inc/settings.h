/*
 * settings.h
 *
 *  Created on: 25 Feb 2022
 *      Author: tunjo
 */

#ifndef INC_SETTINGS_H_
#define INC_SETTINGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "string.h"
#include "stddef.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

#include "stm32f4xx_hal.h"

#include <time.h>

#include "checksum.h"
//#include "ds1307_for_stm32_hal.h"


#define randnum(min, max) \ ((rand() % (int)(((max) + 1) - (min))) + (min))

#define CCRAM __attribute__((section(".ccmram")))


#define _PMS      					1
#define _AGO      					2
#define _DPK      					3

#define TIMEOUT_GO					8000       //8 sec


//:::::::::::::::::::::::::::::::::::::::::::://

#define DEBUG 0
//	  #define DEBUG 1

#define DEBUG1 0
//	  #define DEBUG1 1

#define DEBUG2 0
//	  #define DEBUG2 1

#define PRODUCT_TYPE _DPK

#define DEV_MODE

#define OTP_ENABLE

//	  #define PULSER_BASED_TV

#define AUTO_SALE_TEST
#define DEBUG_AUTO_SALE_TEST

//    #define INCIDENT_RECORD

//:::::::::::::::::::::::::::::::::::::::::::://


//============================================

#ifdef DEV_MODE
//	#define _USE_SOFT_PULSER            1
	#define _USE_SOFT_PULSER            0
	#define sense_battery 				0
//	#define sense_battery 				1
//	#define sense_power  				0
	#define sense_power  				1
#else
	#define _USE_SOFT_PULSER            0
//	#define sense_battery 				1
	#define sense_battery 				0
	#define sense_power  				1
#endif

#define use_internal_rtc			0
#define delay_keypad                1

#define test_battery        		0
#define test_power 					0
#define test_rtc					0
#define test_motor					0
#define test_solenoid				0
#define test_totaliser				0

#define LCD_UPDATE_RATE             100
#define keypad_delay				70

#define MAX_EVENTS_PER_STATE 		13

#define pump_rx_bufsize  			1000

#define RxBuf_SIZE   				512
#define MainBuf_SIZE 				2048

#define CALIBRATED 					0b10011001
#define UNCALIBRATED 				0b00000000

#define POWERINTERRUPTION 			0b10011001
#define NOPOWERINTERRUPTION 		0b00000000

#define FRESHDAY					0b00000001
#define VOUCHERNUMBERGOTTEN			0b00000011
#define VOUCHER_DOWNLOADED          0b00000111


#define BATTERY_OK					0b00000000
#define LOW_BATTERY					0b00000001
#define NO_BATTERY					0b00000010

#define CONFIGMODIFIED				0b10011001
#define CONFIGUNMODIFIED			0b00000000

#define OTPSESSION_ON				0b10011001
#define OTPSESSION_OFF				0b00000000

#define VOL_SUPPRESSED 				12
#define VALIDKEYPRESS_THRESHOLD		1

#define OK                          0
#define FAIL                        1

#define MIN_VAL						0.0f
#define MAX_VAL						99999999.0f

#define RECOVERED                   1
#define NO_RECOVERY                 0

#define TONE_DURATION 				1

#define THRESHOLD_TV 				0.1    //0.1L

#define SAVED_TO_MAIN_TOTALIZER     0
#define UNSAVED_TO_MAIN_TOTALIZER   1


#define BATTERY_DELAY 				1500

#define UNPROGRAMMED_SALE           0
#define LITRE_PROGRAMMED            1
#define PRICE_PROGRAMMED            2


/***********************************************************************/
#define DEBOUNCE_TIME_MS 20  //5  //10  //20 // Debounce period in milliseconds
#define DEBOUNCE_DELAY DEBOUNCE_TIME_MS
#define LONG_PRESS_DELAY 500  // Long press delay

#define NOZZLESWITCH_DEBOUNCE_DELAY 600

#define RX_BUFFER_SIZE 100

//typedef enum {
//    IDLE,
//    DEBOUNCING
//} debounce_state_t;

// Key states
typedef enum {
    KEY_IDLE,
    KEY_DEBOUNCE,
    KEY_SHORT_PRESS,
    KEY_LONG_PRESS
} KeyState;


// Switch states
typedef enum {
    SWITCH_IDLE,
    SWITCH_DEBOUNCE,
    SWITCH_SHORT_PRESS,
    SWITCH_LONG_PRESS
} SwitchState;

//// Define timing thresholds (in milliseconds)
//#define DEBOUNCE_DELAY 50    // Debounce delay
//#define LONG_PRESS_DELAY 1000  // Long press delay
//
//// Key states
//typedef enum {
//    KEY_IDLE,
//    KEY_DEBOUNCE,
//    KEY_SHORT_PRESS,
//    KEY_LONG_PRESS
//} KeyState;

//// Keypad state variables
//uint32_t lastDebounceTime = 0;
//uint32_t keyPressStartTime = 0;
//int lastKeyState = 0;
//KeyState keyState = KEY_IDLE;


/**********************************************************************/

//#define  fast_flow_threshold       ( 140 )
//============================================

typedef enum
 {
     LAFENG,
     BLUESKY
 }pumptype;

 typedef enum
  {
      AUTO_MODE,
      MANUAL_MODE
  }opmode_;


 typedef enum
  {
		P,
		L,
		V
  }sellmode_;


typedef enum
{
	MO1,    // Mode => Manual/Auto                      					// index ==> 0x00
	NA1,    // Nozzle Address												// index ==> 0x01
	NO1,    // Nozzle Override												// index ==> 0x02
	NC1,	// Nozzle Count													// index ==> 0x03
	UN1,	// Unit Price													// index ==> 0x04
//	TM1,	// Clock Setting => Date/Time									// index ==> 0x05
	TO1,	// Timeout => No-Flow											// index ==> 0x05
	ML1,	// Maximum Litre												// index ==> 0x06
	PW1,	// Password-1 Change											// index ==> 0x07
	PW2,	// Password-2 Change											// index ==> 0x08
	PW3,	// Password-3 Change											// index ==> 0x09
	CV1,	// Calibration Value-1 => Real Pulser-index						// index ==> 0x0A
	CV2,	// Calibration Value-2 => Apparent Pulser-index					// index ==> 0x0B
	CS1,	// Calibration Can-size											// index ==> 0x0C
	DP1,	// Display Decimal Point-1  => 	Amount							// index ==> 0x0D
	DP2,	// Display Decimal Point-2	=> 	Volume							// index ==> 0x0E
	DP3,	// Display Decimal Point-3	=> 	Unit Price						// index ==> 0x0F
	DS1,	// Display Format => L/P or P/L									// index ==> 0x10
	DT1,	// Volume Display Threshold										// index ==> 0x11
	SL1,	// Shift Login Type												// index ==> 0x12
	SC1,	// Shift-Count per day											// index ==> 0x13
	TN1,	// Tone															// index ==> 0x14
	PT1,	// Pulser Type => Quadrature/Non-Quadrature						// index ==> 0x15
	PO1,	// Pulser Offset Value											// index ==> 0x16
	SF1,	// Start Slow-Flow Width										// index ==> 0x17
	SF2		// Closing Slow-Flow Width                       				// index ==> 0x18  ==>0d24
}config_change;


typedef enum
{
	NOZZLE_HANGUP,
	NOZZLE_PICKUP
}nozzleState_change;

  enum
  {
	January = 1,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December
  };

  enum
  {
	  _success = 1,
	  _failed = 0
  };

  typedef struct     //structure for log.
    {
      char tm_[10];
      float pr_;
      float amt_;
      float totaliser_;
    }log_;

typedef struct
{
	  uint8_t _mm; // 1
	  uint8_t _dd; // 2
	  uint8_t _yy; // 3
	  uint8_t _dow; //    +1
}date_;

typedef struct
{
	 uint8_t _hh; // 4
	 uint8_t _mn; // 5
}time_;

typedef struct   //structure for log.
{
						//real...
	 float pr_;    //4
	 float vol_;   //8
						//calibrated...
	 float pr__;   //12
	 float vol__;  //16 -----> 16

	// float pr_d;
	// float vol_d;
	//   uint64_t totaliserVol_real;
	//   uint64_t totaliserVol_cal;

	  float totaliserVol_real; //4
	  float totaliserVol_cal;	//4  -->8 ---> 24

	  date_ date;
		//  	uint8_t mm; // 1
		//      uint8_t dd; // 2
		//      uint8_t yy; // 3
		//      uint8_t dow;// 4
	  time_ time_e;
		  //uint8_t hh; // 4
		  //uint8_t mn; // 5  //5 --> 29 + 1

	 char tt_;          		   // ---> 31
	 char tag_[18];     		   // ---> 31 + 18
	 char tm_[1];       		   // ---> 31 + 19

	 float litre_price_;   		   // --> 50 + 4

	 float totaliserAmount_real;   //4
	 float totaliserAmount_cal;	   //4  -->8 ---> 54 + 8

	 uint32_t transaction_period;  //--> 62 + 4 => 66

	 long timestamp;               //--> 67 + 4 => 71

	 char transaction_id[20];      //--> 71 + 20 => 91

	 char nozzle_product[4];       //--> 91 + 4 => 95

	 char device_id[16];           //--> 95 + 16 => 111

	 char nozzle_name[5];          //--> 111 + 5 => 116

	 uint8_t autoTranxFlag;	       //--> 116 + 1

	 int8_t nozzle_address;        //--> 117 + 1 => 118

	 char storage_loc;             //--> 118 + 1 => 119

	 char session_id[9];           //--> 119 + 9 => 128

	 char voucher_[];

}log_new;

typedef struct{
	unsigned long timestamp;
	unsigned long token;
	char transaction_id[20];
	char nozzle_name[5];
	char nozzle_product[6];
	uint8_t nozzle_address;
	float litre_price;
	float transaction_vol;
	float transaction_price;
	float totalizer;
	uint32_t transaction_period;
	char transaction_type;
	char device_id[16];
	char tag[12];
	char storage_loc;
	char session_id[9];
}transaction_1;

//typedef struct   //structure for log.
//{
//						//real...
//	 float pr_;    //4
//	 float vol_;   //8
//						//calibrated...
//	 char pr__ [10];   //18
//	 char vol__ [10];  //28 -----> 28
//
//	// float pr_d;
//	// float vol_d;
//	//   uint64_t totaliserVol_real;
//	//   uint64_t totaliserVol_cal;
//
//	  float totaliserVol_real; //4
//	  float totaliserVol_cal;	//4  -->8 ---> 36
//
//	  date_ date;
//		//  	uint8_t mm; // 1
//		//      uint8_t dd; // 2
//		//      uint8_t yy; // 3
//		//      uint8_t dow;// 4
//	  time_ time;
//		  //uint8_t hh; // 5
//		  //uint8_t mn; // 6  //6 --> 42
//
//	 char tt_;          // ---> 43
//	 char tag_[18];     // ---> 43 + 18
//	 char tm_[1];       // ---> 61 + 1
//
//	 float litre_price_;   // --> 62 + 4
//
//	 float totaliserAmount_real;  //4
//	 float totaliserAmount_cal;	  //4  -->8 ---> 66 + 8
//
//	 uint8_t autoTranxFlag;	  //--> 74 + 1
//
//	 uint32_t transaction_period;  //--> 75 + 4 => 79
//
//	 char transaction_id[20];     //--> 79 + 20 => 99 ...
//
//	 long timestamp;              //--> 99 + 4 => 103
//
//	 char nozzle_product[6];      //--> 103 + 6 => 109
//
//	 char nozzle_name[5];         //--> 109 + 6 => 115
//
//	 int8_t nozzle_address;       //--> 115 + 1 => 116
//
//	 char storage_loc;            //--> 116 + 1 => 117
//
//	 char device_id[16];          //--> 117 + 16 => 133
//
//	 char voucher_[];
//
//}log_new;


//typedef struct   //structure for log.
//{
//	float totaliserAmount_real; //4
//	float totaliserAmount_cal;	//4  -->8
//}totaliserAmount_store;

 typedef struct
 {
	  float totaliserVol_real;      //4
	  float totaliserVol_cal;	    //4
	  float totaliserAmount_real;   //4
	  float totaliserAmount_cal;	//4  -->16
 }totaliser_store;


typedef struct
{
  float amountOld; //4
}_amountSend;

 typedef struct
  {
 	  float totaliserVol_real; //4
 	  float totaliserVol_cal;	//4  -->8
 	  unsigned long timestamp;  // 8 + 4 => 12
  }firstTotaliser_store;

 typedef struct
 {
	  float lastVolumeSale_real; //4
	  float lastVolumeSale_cal;	//4  -->8
	  float lastAmountSale_real; //4
	  float lastAmountSale_cal;	//4  -->16
 }lastSale_store;

 typedef struct
 {
	  int16_t pulser_benchMark;
	  int16_t pulser_value;
	  uint8_t power_interruption;
 }_calibrationData;

 typedef struct
 {
	 uint16_t original;   //2
	 float base;          //2 + 4 => 6
	 float effective;     //6 + 4 => 10
 }ct_settings;

 typedef struct
 {
	 float effective;
	 unsigned int startTime;
	 unsigned int endTime;
	 uint8_t day;
 }ctTimed_settings;


typedef struct
{
	 uint8_t ctTimed_flag1;
	 uint8_t ctTimed_flag2;
}ctTimed_flag_;

typedef struct
{
	 float original_pi_c1;
	 float original_pi_c2;
}_original_pi_c;

typedef struct
{
	uint32_t current_loc;
	uint32_t number_logs;   // 8 Bytes
}flash_store_info;

typedef struct
{
	  uint32_t recovered_count;         //4
	  uint8_t recovered_countFlag;	    //4
}recovered_pulser_count;

recovered_pulser_count recovered_pulserCount[2];


typedef struct
{
	char session_id[9]; //9
}_sessionId;

typedef enum
{
	totaliser_view,
	log_view
}operatorfxn_;

typedef enum
{
	override,
	nooverride
}nozzle_override;

typedef enum
{
	quadrature,
	non_quadrature
}pulser_type;

typedef enum
{
  pr,
  ltr
}default_type;

typedef enum
{
   side_a = 1,
   side_b
}pump_sid;

typedef enum
{
   tot_a = 1,
   tot_b,
   amt_a,
   amt_b
}corrected_sid;

typedef enum
{
   move_to_settings,
   move_to_copy,
   move_to_settings0
}copy_dir;



typedef enum
{
  P1 = 1,
  P2,
  P3,
  P4,
  P5
}nozzleid;

typedef enum
{
	PMS = 1,
	DPK,
	AGO,
}productname;

typedef struct
{
	char pump_name[5];
}pump_names;

pump_names pumpName[2];

//=================================
// peripherals

typedef enum
{
  LAFNG885 = 1,
  BLSKY886_N,           //N -> Normal
  BLSKY886_IN			//IN -> Inverted
}displaytype;

typedef enum
{
  LAFNG17_K = 1,    	//ie. 17 punch-keys + a turn-key
  BLSKY18_K,			//ie. 18 punch-keys + a turn-key
  BLSKY22,				//ie. 22 punch-keys + no turn-key
  LAFNG18_K,    		//ie. 18 punch-keys + a turn-key
  LAFNG18_K_V2    		//ie. 18 punch-keys + a turn-key + Variant II
}keypadtype;

typedef enum
{
  DN_LAFNG17K = 0b00000001,		//Display->Normal, keypad->LAFNG-17K
  DN_BLSKY18K,       			//Display->Normal, keypad->BLSKY-18K
  DN_BLSKY22,					//Display->Normal, keypad->BLSKY-22
  DIN_BLSKY18K,       			//Display->Inverted, keypad->BLSKY-18K
  DIN_BLSKY22,					//Display->Inverted, keypad->BLSKY-22
  DN_LAFNG18K,					//Display->Normal, keypad->LAFNG-18K
  BLSKY886_N_LAFNG18_K  		//Display->Normal/BLSKY886_N, keypad->LAFNG-18K
}pump;


//typedef enum
//{
//  DN_LAFNG17K = 0b00000001,		//keypad->LAFNG17_K
//  DN_BLSKY18K,       			//keypad->BLSKY-12
//  DN_BLSKY22					//keypad->BLSKY-16
//}pump;

 typedef enum
 {
 	nozzle_name,
 	product_name,
 	disp_type,
 	keypad_type,
 }pumpCompPart;

 typedef enum
 {
	 PL = 1,
	 LP,
 }displaymode;


 typedef enum
 {
 	Yes = 1,
	No
 }keypresstone;

 typedef enum
 {
 	None_,
	Code_,
	Card_
 }shiftlogintype;

 typedef enum
 {
	 Wizard = 1,
	 Manual_calib
 }calibrationType;


// displayMode displaymode;
// keypressTone keypresstone;


 //==================================
 //structure for settings
 typedef struct
 {
 	 opmode_  mode;
 	 nozzle_override noz_override;
 	 sellmode_ def_t;
 	 pumptype pump_type_;
 	 float price_;
 	 float pi_cal;
 	 float pi_real;
 	 uint8_t noz_addr;
 	 uint8_t dp_amount;
 	 uint8_t dp_vol;
 	 uint8_t dp_unitprice;

	 nozzleid noz_id;
	 displaymode display_format;  // PL/LP   ==> default : PL  // Level 2
 	 //peripherals
 	 displaytype display__;
 	 keypadtype keypad__;

 	 char product_[4];

 	 uint32_t max_amt_;				//<== 32 Bytes

 }pump_settings_stream1;


// typedef struct
// {
// 	 opmode_  mode;
// 	 nozzle_override noz_override;
// 	 float price_;
//
// 	 float pi_c;
// 	 float pi_;
// 	 uint8_t noz_addr;
// 	 uint8_t dp_amount;
// 	 uint8_t dp_vol;
// 	 uint8_t dp_unitprice;
//
//	 nozzleid noz_id;
//	 displaymode display_mode;  // PL/LP   ==> default : PL  // Level 2
//
// 	 uint16_t max_amt_;
//
// 	 float valve_salesStart;
//	 float valve_salesEnd;
//
//	 uint8_t calibration_measureCan;  // 10L/20L   ==> default : 20L  // Level 2
//	 keypresstone keypress_tone;  // Yes/No   ==> default : No   // Level 2
//
//
//	 uint8_t pulser_type_;
//	 uint8_t pulser_offset;
//	 uint8_t number_of_shifts;
//
//	 uint8_t noz_count;   // 1/2    ==> default : 2   // Level 2
//	 shiftlogintype shift_login_type;
//
////	 uint16_t non_calibration_seed;
//
//	 char passwd1[9];
//	 char passwd2[9];
//	 char passwd3[9];
//
// }pump_settings_stream1;



 //==================================
 //structure for settings
 typedef struct
 {
 	 float valve_salesStart;
 	 float valve_salesEnd;

 	 uint8_t calibration_measureCan;  // 10L/20L   ==> default : 20L  // Level 2
 	 keypresstone keypress_tone;  // Yes/No   ==> default : No   // Level 2
 	 uint8_t pulserError_status;  // 1/0   ==> default :   // Level 2
 	 uint8_t idleState_maxPulseValue;  //    ==> default :   // Level 2

 	 float startUp_suppressVol;  // (0 - 10) cL   ==> default : 4cL  // Level 2

 	 uint8_t pulser_type_;
 	 uint8_t pulser_offset;
 	 uint8_t number_of_shifts;
 	 calibrationType calibration_type;

 	 uint8_t totalizer_day;
 	 uint8_t noz_count;   // 1/2    ==> default : 2   // Level 2
 	 bool commCard_enforced;
 	 shiftlogintype shift_login_type;

 	 uint16_t non_calibration_seed;

 	 uint16_t timeOut_noFlow; 			// ==> 28 Bytes

 }pump_settings_stream2;


 typedef struct
 {
 	 char passwd1[9];
 	 char passwd2[9];
 	 char passwd3[9];			  //<==  (27 + 1) Bytes   ==> 28 Bytes

 }pump_settings_stream3;



// pump_settings_stream1 settings_stream1[2],
//   					   settings_original_stream1[2],
//  					   copy_stream1[2];
//
//  pump_settings_stream2 settings_stream2[2],
// 					   settings_original_stream2[2],
// 					   copy_stream2[2];
//
//  pump_settings_stream3 settings_stream3[2],
//   				       settings_original_stream3[2],
//  					   copy_stream3[2];

 typedef struct
 {
 	 uint32_t timestamp_event;    // 4 Bytes
 	 float programmed_sale;       // 4 Bytes
 	 uint8_t nozzleState_change_; // 1 Byte + 3 Byte-padding
 }incident_record;

 incident_record incidentRecord[2];

bool nozzle_out1,
	 nozzle_out2;

uint8_t status_change_pump1,
		status_change_noz1,
		mamo_reached_flag1,
		mamo_reached_flag1_1,
		filling_mamo_flag1,
		stopFlag_source1,
		hardwareError_flag1,
		hardwareErrorFlag_source1,
		mamo_fillingInfo_send1;

uint8_t status_change_pump2,
		status_change_noz2,
		mamo_reached_flag2,
		mamo_reached_flag2_1,
		filling_mamo_flag2,
		stopFlag_source2,
		hardwareError_flag2,
		hardwareErrorFlag_source2,
		mamo_fillingInfo_send2;

int tone_duration1,
	tone_duration2;

extern float key_value CCRAM,
	         key_value2 CCRAM;

extern uint8_t sales_type1 CCRAM,
			   sales_type2 CCRAM;

uint32_t target_pulser1,
		 current_pulser1,
		 target_pulser2,
		 current_pulser2;

//extern uint32_t currentPulser_recovered1,
//				currentPulser_recovered2,
//				targetPulser_recovered1,
//				targetPulser_recovered2;

extern uint8_t nozzle_flag,
	 	       nozzle_flag_old,
		       nozzle_flag2,
		       nozzle_flag_old2;

extern uint8_t fillingresume_flag1,
			   fillingresume_flag2,
			   fillingresume_flag1_1,
			   fillingresume_flag2_1;

////==================================
////structure for settings
//typedef struct
//{
//	 opmode_  mode;
//	 nozzle_overide noz;
//	 sellmode_ def_t;
//	 pump pump_type_;
//	 float price_;
//	 int id_;
////	 int noflow_;
//	 int noFlow_timeOut;
//	 unsigned int max_amt_;
//	 float pi_c;
//	 float pi_;
//	 uint8_t dp_price;
//	 uint8_t dp_amount;
//	 uint8_t dp_unitprice;
//
//	 //peripherals
//	 displaytype display__;
//	 char product_[4];
//	 keypadtype keypad__;
//	 nozzleid noz_id;
//	 uint8_t totalizer_day;
//	 uint8_t side_size;   // 1/2    ==> default : 2   // Level 2      <== 40 Bytes
//
//	 displaymode display_mode;  // PL/LP   ==> default : PL  // Level 2
//	 keypresstone keypress_tone;  // Yes/No   ==> default : No   // Level 2
//	 uint8_t pulserError_status;  // 1/0   ==> default :   // Level 2
//	 uint8_t idleState_maxPulseValue;  //    ==> default :   // Level 2
////	 float startUp_suppressVol;  // (0 - 10) cL   ==> default : 4cL  // Level 2
//	 uint8_t calibration_measureCan;  // 10L/20L   ==> default : 20L  // Level 2
//	 uint8_t number_of_shifts;
//	 calibrationType calibration_type;
//	 bool commCard_enforced;
//	 float valve_salesStart;
//	 float valve_salesEnd;                         //<== 20 Bytes
//
//	 uint8_t pulser_type_;
//	 uint8_t pulser_offset;                        //<== { ( 2 Bytes + 20 Bytes + 40 Bytes ) + 2 Bytes Padding } ==> 64 Bytes
//
//}pump_settings_stream1;
//
//
//typedef struct
//{
////	 uint16_t non_calibration_seed;
//	 float startUp_suppressVol;  // (0 - 10) cL   ==> default : 4cL  // Level 2
//	 char passwd1[9];
//	 char passwd2[9];
//	 char passwd3[9];
//
//	 shiftlogintype shift_login_type;    //<==  30 Bytes
//
//	 uint16_t non_calibration_seed;
//}pump_settings_stream2;


////==================================
////structure for settings
//typedef struct
//{
//	 opmode_  mode;
//	 nozzle_overide noz;
//	 sellmode_ def_t;
//	 pump pump_type_;
//	 float price_;
//	 int id_;
////	 int noflow_;
//	 int noFlow_timeOut;
//	 unsigned int max_amt_;
//	 float pi_c;
//	 float pi_;
//	 uint8_t dp_price;
//	 uint8_t dp_amount;
//	 uint8_t dp_unitprice;
//	 //peripherals
//	 displaytype display__;
//	 char product_[4];
//	 keypadtype keypad__;
//	 nozzleid noz_id;
//	 uint8_t totalizer_day;
//	 uint8_t side_size;   // 1/2    ==> default : 2   // Level 2
//	 displaymode display_mode;  // PL/LP   ==> default : PL  // Level 2
//	 keypresstone keypress_tone;  // Yes/No   ==> default : No   // Level 2
//	 uint8_t pulserError_status;  // 1/0   ==> default :   // Level 2
//	 uint8_t idleState_maxPulseValue;  //    ==> default :   // Level 2
//	 float startUp_suppressVol;  // (0 - 10) cL   ==> default : 4cL  // Level 2
//	 uint8_t calibration_measureCan;  // 10L/20L   ==> default : 20L  // Level 2
//	 uint8_t number_of_shifts;
//	 calibrationType calibration_type;
//	 bool commCard_enforced;
//	 char passwd1[9];
//	 char passwd2[9];
//	 char passwd3[9];
//	 shiftlogintype shift_login_type;
//	 uint8_t pulser_type_;
//	 uint8_t pulser_offset;
//	 float valve_salesStart;
//	 float valve_salesEnd;
//	 uint16_t non_calibration_seed;
//
//}pump_settings;


typedef struct
{
  char user_id[9];
  char user_pin[5];
  uint8_t type[4];
  char user_name[16];
  char session_id[9];
  char bal[16];
  char typ;
  uint8_t login_state;
}_card;

_card attendant1, attendant2, customer;

////==================================
////structure for settings
//typedef struct
//{
//	 opmode_  mode;   // Online/Offline   ==> default : Online  // Level 2
//	 nozzle_overide noz;
//	 sellmode_ def_t;
//	 pump pump_type_;
//	 float price_;    // Level 1
//	 int id_;
//	 int noFlow_timeOut;   // (10 - 120) sec   ==> default : 30  // Level 2
//	 unsigned int max_amt_;
//	 uint32_t passwd1;
//	 uint32_t passwd2;
//	 float pi_c;
//	 float pi_;
//	 int8_t dp_price;	   // 1/2/3   ==> default : 2  // Level 2
//	 int8_t dp_amount;     // 1/2/3   ==> default : 2  // Level 2
//	 int8_t dp_unitprice;  // 1/2/3   ==> default : 2  // Level 2
//	 //peripherals
//	 displaytype display__;  // Level 2
//	 keypadtype keypad__;
//	 char product_[6];
//	 nozzleid noz_id;
//	 int8_t totalizer_day;
//	 uint8_t side_size;   // 1/2    ==> default : 2   // Level 2
//	 uint8_t display_mode;  // PL/LP   ==> default : PL  // Level 2
//	 uint8_t keypress_tone;  // Yes/No   ==> default : No   // Level 2
//	 uint8_t pulserError_status;  // 1/0   ==> default :   // Level 2
//	 uint8_t idleState_maxPulseValue;  //    ==> default :   // Level 2
//	 uint8_t startUp_suppressVol;  // (0 - 10) cL   ==> default : 4cL  // Level 2
//	 uint8_t calibration_measureCan;  // 10L/20L   ==> default : 20L  // Level 2
//	 uint8_t calibration_measureCan;  // 10L/20L   ==> default : 20L  // Level 2

//}pump_settings;

//==================================


/* enum for pump status */
typedef enum
{
	STATUS_PUMP_ON = 255,

	STATUS_IDLE = 1,
	STATUS_NOZZLE_DOWN,
	STATUS_NOZZLE_UP,
	STATUS_AUTHORIZED_NOZZLE_DOWN,
	STATUS_AUTHORIZED_NOZZLE_UP,
	STATUS_FILLING_
}pump_status_enum_4G;


pump_status_enum_4G pump1_status_4G,
				    pump2_status_4G;

/* enum for pump status */
typedef enum
{
	STATUS_UNKNOWN,    //used for idle state by the Main

	STATUS_PNP,
	STATUS_RESET,
	STATUS_AUTH,
	STATUS_FILLING,
	STATUS_FILLING_COMP,
	STATUS_MAMO_REACHED,
	STATUS_SUSPENDED,
	STATUS_SWITCHED_OFF
}pump_status_enum;

pump_status_enum pump_status_1,
				 pump_status_2;


typedef struct
{
  float totaliserVol_real;      		  //4
  float totaliserVol_cal;	    		  //4
  float totaliserAmount_real;   		  //4
  float totaliserAmount_cal;			  //4
  float lastVolumeSale_cal;               //4
  float lastAmountSale_cal;				  //4
  uint32_t timestamp;					  //4
  float sales_value;	              //4
  uint32_t target_pulser;				  //4
  uint32_t current_pulser;				  //4
  uint8_t totalizer_save_status;          //1

  /*
   * SALE'S PROGRAM STATUS
   *
   |	0 ==> Not Programmed, or the Programmed Amount exceeds limits (pump's, display's or auth's)
   |	1 ==> Litre Programmed
   |	2 ==> Price Programmed
   *
   */
  uint8_t sales_type;		    	//1

  pump_status_enum pump_status;             //1 Byte + A Byte Padding       --> 44

}totaliser_store_frequent;


typedef struct
{
	 char type[2];
	 char pumpName[5];
	 char price[8];
	 char shutDownDevice[5];
	 char channelLock[5];
}ep1_mt;

ep1_mt mt_pump[2];

typedef struct
{
	char nozzle_name[6];
	unsigned long timestamp;
	float totalizer;
	float totalizer_real;
}firstTotalizer_ep;

typedef struct{
	unsigned long timestamp;
	unsigned long token;
	firstTotalizer_ep firstTotalizer[2];
	char sentEntry_count[8];
}ep5_;

ep5_ ep5_save;

typedef struct
{
	uint8_t ctt;
	char ct_original[6];
	char ct_baseMinusOriginal[6];
	char ct_effectiveMinusBase[6];
	char ct_startTime[6];
	char ct_endTime[6];
}calibrate;

//calibrate calibrate_ct;

typedef struct
{
	char nozzle_name[5];
	uint8_t nozzle_id;
	calibrate calibrate_ct;
}pumps_ep31;

typedef struct{
	unsigned long timestamp;
	unsigned long token;
	char device_id[16];
	pumps_ep31 pump[2];
	unsigned int firmware_version;
	char storage_loc;
}ep31_;

ep31_ ep31_save;

//typedef struct
//{
//  uint8_t user_id[5];
//  char user_pin[5];
//  uint8_t type[4];
//  char user_name[16];
//  char bal[16];
//  char session_id[9];
//  char typ;
//  uint8_t login_state;
//}_card;

_card attendant1, attendant2, customer;

typedef struct{
	unsigned long timestamp;
	unsigned long token;
	char device_id[16];
	_card pump[2];
}ep20_;

ep20_ ep20_save;

//{
//      "ep":20,
//      "di":"864120050755751",
//      "tk":36685,
//      "tm":194453,
//      "ci":"aac2367d",
//      "pin":"5600"
//}
typedef struct OtpSeed_Session
{
	uint32_t time_stamp;
	uint16_t otp_seed;
	uint8_t otpSeed_flag;   // 7 Bytes + 1 Byte-padding ==> 8 Bytes
}OtpSeed_Session;

OtpSeed_Session otpSeed_session[2];

//typedef struct OtpSeed
//{
//	uint16_t otp_seed;
//	uint32_t time_stamp;   // 6 Bytes + 2 Byte-padding ==> 8 Bytes
//}OtpSeed;

typedef struct ConfigChange
{
	uint32_t time_stamp;
	uint16_t otp_seed;		// 6 Bytes + 2Byte Padding ==>8 Bytes
}ConfigChange;

ConfigChange configChange[2];

typedef struct TrackNum
{
	uint8_t track_num0;
	uint8_t track_num;		    // 2 Bytes
}TrackNum;

TrackNum trackNum[2];

//=== screen Arrays ====
extern char upper1[10],
			middle1[10],
			upper2[10],
			middle2[10];

 unsigned int pump_max_litres1,
 	 	 	  pump_max_litres2;

 bool operating_sideA,
 	  operating_sideB;

 pump_names pumpName[2];

 bool operating_sideA,
 	  operating_sideB;

 int8_t storage_fail,
 	 	tokenFlag,
		changeLitrePrice;

 char keyboard_entry[10];

 uint8_t keyEntry_len,
 	 	keyEntry2_len;


uint8_t ctTimed_flag1,
		ctTimed_flag2;

uint16_t vol_real1,
	     vol_real2;

int startTime1,
	startTime2,
	endTime1,
	endTime2;

uint8_t ctTimed_day1,
		ctTimed_day2;

float vol_calibrated1,
 	  vol_effective1,
	  vol_calibrated2,
	  vol_effective2,
	  vol_effective1_1,
	  vol_effective2_2;

uint32_t calib_pulser1,
         calib_pulser2;

uint8_t overflow_count1,
		overflow_count2;

int8_t recalibration_request;

uint8_t firstTime_nozz1,
		firstTime_nozz2;

uint32_t pulser_benchMark1,
		 pulser_benchMark2;

uint8_t calibrationCan_measure1,
		calibrationCan_measure2;

uint8_t calibration_flag1,
		calibration_flag2,
		pwr1,
		pwr2,
		configMode1,
		configMode2;

uint8_t online_calibFlag1,
		online_calibFlag2;

float temppp_;


float price_update1,
	  price_update2;

uint8_t changeLitrePrice1,
		changeLitrePrice2;

uint8_t track_num1,
		valid_pair1,
		track_num2,
		valid_pair2,
		track_num1_0,
		track_num2_0;


uint16_t otp_seed1,
		 otp_seed2;

int8_t dpFlag,
	   dpCount,
	   dpFlag2,
	   dpCount2;

uint16_t nextLoc_A,
		 nextLoc_B;

char rx_buf[pump_rx_bufsize];

// The shared secret is FdelOnwuka
extern uint8_t hmacKey[]; // = {0x46, 0x64, 0x65, 0x6C, 0x4F, 0x6E, 0x77, 0x75, 0x6B, 0x61};

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;

extern uint16_t pump_SN;

char otp_code1[7],
	 otp_code2[7];

uint8_t batteryVoltage_ready;



//float running_volTotaliser1_tmin1,
//	  running_volTotaliser1_tmin2,
//	  running_volTotaliser1_tmin3,
//	  running_volTotaliser1c_tmin1,
//	  running_volTotaliser1c_tmin2,
//	  running_volTotaliser1c_tmin3;
//
//float running_volTotaliser2_tmin1,
//	  running_volTotaliser2_tmin2,
//	  running_volTotaliser2_tmin3,
//	  running_volTotaliser2c_tmin1,
//	  running_volTotaliser2c_tmin2,
//	  running_volTotaliser2c_tmin3;
//
//float running_amtTotaliser1_tmin1,
//	 running_amtTotaliser1_tmin2,
//	 running_amtTotaliser1_tmin3,
//	 running_amtTotaliser1c_tmin1,
//	 running_amtTotaliser1c_tmin2,
//	 running_amtTotaliser1c_tmin3;
//
//float running_amtTotaliser2_tmin1,
//	 running_amtTotaliser2_tmin2,
//	 running_amtTotaliser2_tmin3,
//	 running_amtTotaliser2c_tmin1,
//	 running_amtTotaliser2c_tmin2,
//	 running_amtTotaliser2c_tmin3;
//
//float running_volTotaliser1_array[4],
//	  running_volTotaliser1c_array[4],
//	  running_volTotaliser2_array[4],
//	  running_volTotaliser2c_array[4];
//
//float running_amtTotaliser1_array[4],
//	  running_amtTotaliser1c_array[4],
//	  running_amtTotaliser2_array[4],
//	  running_amtTotaliser2c_array[4];
//
//float amt_middle1_tmin1,
//	 amt_middle1_tmin2,
//	 amt_middle1_tmin3,
//	 amt_middle2_tmin1,
//	 amt_middle2_tmin2,
//	 amt_middle2_tmin3;
//
//float amt_real1_tmin1,
//	 amt_real1_tmin2,
//	 amt_real1_tmin3,
//	 amt_real2_tmin1,
//	 amt_real2_tmin2,
//	 amt_real2_tmin3;
//
//float amt_real1_array[4],
//	 amt_middle1_array[4],
//	 amt_real2_array[4],
//	 amt_middle2_array[4];

//void get_settings();

void clr_pulser1();
void clr_pulser2();

void dp_init(pump_sid sdd);

double round_off(float value, int decimalPlaces);
double round_off2(float value, int decimalPlaces);

void save_settings(void);
void save_settings_original(void);
void retrieve_settings(void);
void retrieve_settings_original(void);

void save_settings_fram(void);
void retrieve_settings_fram(void);

void save_settings_original_fram(pump_sid side);
void retrieve_settings_original_fram(pump_sid side);

uint8_t save_totaliser_eeprom(pump_sid side);
uint8_t retrieve_totaliser_eeprom(pump_sid side);
void clear_totaliser_eeprom(pump_sid side);

uint8_t save_totaliser_eeprom_check(pump_sid side);
uint8_t save_totaliser_fram_check(pump_sid side);

uint8_t retrieve_totaliser_fram_check(pump_sid side);
uint8_t retrieve_totaliser_eeprom_check(pump_sid side);

uint8_t save_totaliser_fram(pump_sid side);
uint8_t retrieve_totaliser_fram(pump_sid side);
void clear_totaliser_fram(pump_sid side);

uint8_t save_totaliserFrequent_fram(pump_sid side);
uint8_t retrieve_totaliserFrequent_fram(pump_sid side);
uint8_t clear_totaliserFrequent_fram(pump_sid side);

uint8_t save_totaliserFrequent_fram_check(pump_sid side);
uint8_t save_totaliserFrequent_eeprom_check(pump_sid side);

uint8_t save_totaliserFrequent_eeprom(pump_sid side);
uint8_t retrieve_totaliserFrequent_eeprom(pump_sid side);
uint8_t clear_totaliserFrequent_eeprom(pump_sid side);

void saver_Totaliser_startShift_fram(pump_sid side);
void retrieve_Totaliser_startShift_fram(pump_sid side);
void clear_Totaliser_startShift_fram(pump_sid side);

void save_amountTotaliser(pump_sid side);
void retrieve_amountTotaliser(pump_sid side);
void clear_amountTotaliser(pump_sid side);

void save_amountTotaliser_fram(pump_sid side);
void retrieve_amountTotaliser_fram(pump_sid side);
void clear_amountTotaliser_fram(pump_sid side);

void save_lastSale_eeprom(pump_sid side);
void retrieve_lastSale_eeprom(pump_sid side);
void clear_lastSale_eeprom(pump_sid side);

void save_lastSale_fram(pump_sid side);
uint8_t retrieve_lastSale_fram(pump_sid side);
void clear_lastSale_fram(pump_sid side);

void save_ctSettings(pump_sid side);
void retrieve_ctSettings(pump_sid side);
void clear_ctSettings(pump_sid side);

void save_ctSettings_fram(pump_sid side);
void retrieve_ctSettings_fram(pump_sid side);
void clear_ctSettings_fram(pump_sid side);

void save_online_calibFlag_fram(pump_sid side);
void retrieve_online_calibFlag_fram(pump_sid side);
void clear_online_calibFlag_fram(pump_sid side);

void save_ctTimedSettings(pump_sid side);
void retrieve_ctTimedSettings(pump_sid side);
void clear_ctTimedSettings(pump_sid side);

void save_ctTimedSettings_fram(pump_sid side);
void retrieve_ctTimedSettings_fram(pump_sid side);
void clear_ctTimedSettings_fram(pump_sid side);

void save_ctTimedFlag(pump_sid side);
void retrieve_ctTimedFlag(pump_sid side);
void clear_ctTimedFlag(pump_sid side);

void save_ctTimedFlag_fram(pump_sid side);
void retrieve_ctTimedFlag_fram(pump_sid side);
void clear_ctTimedFlag_fram(pump_sid side);

void save_1stVolTotaliser_day(pump_sid side);
void retrieve_1stVolTotaliser_day(pump_sid side);
void clear_1stVolTotaliser_day(pump_sid side);

void save_1stVolTotaliser_day_fram(pump_sid side);
void retrieve_1stVolTotaliser_day_fram(pump_sid side);
void clear_1stVolTotaliser_day_fram(pump_sid side);

void save_calibrationPulser(pump_sid side);
void retrieve_calibrationPulser(pump_sid side);
void clear_calibrationPulser(pump_sid side);

void save_calibrationPulser_fram(pump_sid side);
void retrieve_calibrationPulser_fram(pump_sid side);
void clear_calibrationPulser_fram(pump_sid side);

void save_sessionId(pump_sid side);
void retrieve_sessionId(pump_sid side);
void clear_sessionId(pump_sid side);

void save_calibrationFlag(pump_sid side);
void retrieve_calibrationFlag(pump_sid side);
void clear_calibrationFlag(pump_sid side);

void save_calibrationFlag_fram(pump_sid side);
void retrieve_calibrationFlag_fram(pump_sid side);
void clear_calibrationFlag_fram(pump_sid side);

void save_calibrationData(pump_sid side);
void retrieve_calibrationData(pump_sid side);
void clear_calibrationData(pump_sid side);

void save_calibrationData_fram(pump_sid side);
void retrieve_calibrationData_fram(pump_sid side);
void clear_calibrationData_fram(pump_sid side);

void save_amountSend(pump_sid side);
void retrieve_amountSend(pump_sid side);
void clear_amountSend(pump_sid side);

void save_configFlag(pump_sid side);
void retrieve_configFlag(pump_sid side);
void clear_configFlag(pump_sid side);

void save_configFlag_fram(pump_sid side);
void retrieve_configFlag_fram(pump_sid side);
void clear_configFlag_fram(pump_sid side);


void save_otp(pump_sid side);
void retrieve_otp(pump_sid side);
void clear_otp(pump_sid side);

void save_config_otpSeed_time_fram(pump_sid side);
void retrieve_config_otpSeed_time_fram(pump_sid side);
void clear_config_otpSeed_time_fram(pump_sid side);

void save_otpSeed_session_fram(pump_sid side);
void retrieve_otpSeed_session_fram(pump_sid side);
void clear_otpSeed_session_fram(pump_sid side);

void save_configChange_trackNum_fram(pump_sid side);
void retrieve_configChange_trackNum_fram(pump_sid side);
void clear_configChange_trackNum_fram(pump_sid side);

void save_incidentNextLoc_fram(pump_sid side);
void retrieve_incidentNextLoc_fram(pump_sid side);
void clear_incidentNextLoc_fram(pump_sid side);

void save_programmedSaleEvent_fram(pump_sid side);

uint8_t save_recoveredPulserCount_fram_check(pump_sid side);
uint8_t save_recoveredPulserCount_fram(pump_sid side);

void copy_settings(copy_dir dir);
void load_settings(pump_sid side);

int get_auth(void);

int get_auth_cmd();

void make_settings(pump_sid side);

float sellPrice_max_dp(int8_t amount_dp);
float sellPrice_max_dp2(int8_t amount_dp);

void int_to_bcd(int num, unsigned char *bcd);
void int_to_bcd_(int num, unsigned char *bcd, uint8_t bcd_size);

void sellmode_write1(uint8_t sellmodee);
void sellmode_write2(uint8_t sellmodee);

//typedef enum
//{
//  P1 = 1,
//  P2,
//  P3,
//  P4
//}nozzleid;
//
//typedef enum
//{
//	PMS,
//	DPK,
//	AGO,
//}productname;
//
//typedef enum
//{
//  LAFNG17_K = 1,
//  BLSKY18_K,
//  BLSKY22
//}keypadtype;
//
//
//typedef enum
//{
//  LAFNG885 = 1,
//  BLSKY886_N
//}displaytype;
//
//typedef enum
//{
//	nozzle_name,
//	product_name,
//	disp_type,
//	keypad_type,
//}pumpCompPart;
//
//typedef enum
//{
//  DN_LAFNG17K = 0b00000001,		//keypad->LAFNG-12
//  DN_BLSKY18K,       			//keypad->BLSKY-12
//  DN_BLSKY22					//keypad->BLSKY-16
//}pump;
//
//typedef enum
//{
//	nozzle_name,
//	product_name,
//	disp_type,
//	keypad_type,
//}pumpCompPart;


#ifdef __cplusplus
}
#endif

#endif /* INC_SETTINGS_H_ */                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
