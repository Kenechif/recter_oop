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


//============================================
#define _USE_SOFT_PULSER            1
#define use_internal_rtc			0
#define sense_power  				1
#define sense_battery 				1
#define delay_keypad                1

#define test_battery        		0
#define test_power 					0
#define test_rtc					0
#define test_motor					0
#define test_solenoid				0
#define test_totaliser				0

#define LCD_UPDATE_RATE             100
#define keypad_delay				100

#define pump_rx_bufsize  1000

//#define  fast_flow_threshold       ( 140 )
//============================================

//typedef enum
// {
//     lafeng,
//     bluesky
// }pump;

 typedef enum
  {
      AUTO,
      MANUAL
  }opmode_;


 typedef enum
  {
		P,
		L
  }sellmode_;


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
	  time_ time;
		  //uint8_t hh; // 4
		  //uint8_t mn; // 5  //5 --> 29 + 1

	 char tt_;          // ---> 31
	 char tag_[18];     // ---> 31 + 18
	 char tm_[1];       // ---> 31 + 19

	 float litre_price_;   // --> 50 + 4

	 float totaliserAmount_real;  //4
	 float totaliserAmount_cal;	  //4  -->8 ---> 54 + 8

	 uint8_t autoTranxFlag;	  //--> 62 + 1

	 uint32_t transaction_period;  //--> 63 + 4 => 67

	 char transaction_id[20];     //--> 67 + 20 => 87

	 long timestamp;              //--> 87 + 4 => 91

	 char nozzle_product[6];      //--> 91 + 6 => 97

	 char nozzle_name[5];         //--> 97 + 6 => 103

	 int8_t nozzle_address;       //--> 103 + 1 => 104

	 char storage_loc;            //--> 104 + 1 => 105

	 char device_id[16];          //--> 105 + 16 => 121

	 char voucher_[];

}log_new;


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


typedef struct   //structure for log.
{
	float totaliserAmount_real; //4
	float totaliserAmount_cal;	//4  -->8
}log_new1;

 typedef struct
 {
	  float totaliserVol_real; //4
	  float totaliserVol_cal;	//4  -->8
 }totaliser_store;

 typedef struct
  {
 	  float totaliserVol_real; //4
 	  float totaliserVol_cal;	//4  -->8
 	  long timestamp;  // 8 + 4 => 12
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
	 uint8_t original;   //1
	 float base;         //1 + 4 => 5
	 float effective;    //5 + 4 => 9
 }ct_settings;

 typedef struct
 {
	 float effective;
	 int startTime;
	 int endTime;
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
	uint32_t number_logs;

}flash_store_info;

typedef enum
{
	totaliser_view,
	log_view
}operatorfxn_;

typedef enum
{
	overide,
	nooveride
}nozzle_overide;

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
   move_to_settings,
   move_to_copy
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
  LAFNG17_K = 1,    //ie. 17 punch-keys + a turn-key
  BLSKY18_K,		//ie. 18 punch-keys + a turn-key
  BLSKY22			//ie. 22 punch-keys + no turn-key
}keypadtype;

typedef enum
{
  DN_LAFNG17K = 0b00000001,		//Display->Normal, keypad->LAFNG-17K
  DN_BLSKY18K,       			//Display->Normal, keypad->BLSKY-18K
  DN_BLSKY22,					//Display->Normal, keypad->BLSKY-22
  DIN_BLSKY18K,       			//Display->Inverted, keypad->BLSKY-18K
  DIN_BLSKY22					//Display->Inverted, keypad->BLSKY-22
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

//==================================
//structure for settings
typedef struct
{
	 opmode_  mode;
	 nozzle_overide noz;
	 sellmode_ def_t;
	 pump pump_type_;
	 float price_;
	 int id_;
	 int noflow_;
	 unsigned int max_amt_;
	 uint32_t passwd1;
	 uint32_t passwd2;
	 float pi_c;
	 float pi_;
	 int8_t dp_price;
	 int8_t dp_amount;
	 int8_t dp_unitprice;
	 //peripherals
	 displaytype display__;
	 keypadtype keypad__;
	 char product_[6];
	 nozzleid noz_id;
	 int8_t totalizer_day;
}pump_settings;


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
	long timestamp;
	float totalizer;
	float totalizer_real;
}firstTotalizer_ep;

typedef struct{
	long timestamp;
	long token;
	firstTotalizer_ep firstTotalizer[2];
	char sentEntry_count[8];
}ep5_;

ep5_ ep5_save;

typedef struct
{
	int8_t ctt;
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
	long timestamp;
	long token;
	char device_id[16];
	pumps_ep31 pump[2];
	int firmware_version;
	char storage_loc;
}ep31_;

ep31_ ep31_save;

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

 char keyboard_entry[8];

 int8_t keyEntry_len,
 	 	keyEntry2_len;

uint8_t vol_real1,
	    vol_real2,
		ctTimed_flag1,
		ctTimed_flag2;

int startTime1,
	startTime2,
	endTime1,
	endTime2;

float vol_calibrated1,
 	  vol_effective1,
	  vol_calibrated2,
	  vol_effective2,
	  vol_effective1_1,
	  vol_effective2_2;

uint32_t calib_pulser1,
         calib_pulser2;

int8_t recalibration_request;

float temppp_;

//void get_settings();

void dp_init(pump_sid sdd);

void retrieve_settings();
void save_settings();

void save_volumeTotaliser(pump_sid side);
void retrieve_volumeTotaliser(pump_sid side);
void clear_volumeTotaliser(pump_sid side);

void save_amountTotaliser(pump_sid side);
void retrieve_amountTotaliser(pump_sid side);
void clear_amountTotaliser(pump_sid side);

void save_lastSale(pump_sid side);
void retrieve_lastSale(pump_sid side);
void clear_lastSale(pump_sid side);

void save_ctSettings(pump_sid side);
void retrieve_ctSettings(pump_sid side);
void clear_ctSettings(pump_sid side);

void save_1stVolTotaliser_day(pump_sid side);

void copy_settings(copy_dir dir);
void load_settings(pump_sid side);

int get_auth(void);

int get_auth_cmd();

void make_settings(pump_sid side);

float sellPrice_max_dp(int8_t amount_dp);

int8_t dpFlag,
	   dpCount,
	   dpFlag2,
	   dpCount2;

char rx_buf[pump_rx_bufsize];



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

#endif /* INC_SETTINGS_H_ */
