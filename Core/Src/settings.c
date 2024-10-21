/*
 * settings.c
 *
 *  Created on: Mar 8, 2022
 *      Author: tunji
 *
 */
#include "settings.h"
#include "states.h"
#include "states2.h"

#include "stdbool.h"
#include "string.h"
#include "stddef.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "EEPROM.h"


//##################################################

const char device_id [] = "860537065062176";   //"860537065691297";   //"860537065062176";   //"860537065695256";   //"860537064685993"; //"860537065691297";   //"860537064685993";        //"860537064685357";           //"860537064685993";     //"860537064685340";

const char firmware_date [] = "Aug 28 2023";
const char firmware_time [] = "17:07:30";
const char chip_type [] = "STM32F4";

const uint16_t firmware_version = 25100;

uint16_t  pump_SN = 77;
char session_id[9] = {0};

//unsigned char pumpno = 0x01,
//		      pumpno2 = 0x02;

//##################################################



 const int max_events_per_state = 10;

 // The shared secret is FdelOnwuka
// uint8_t hmacKey[] = {0x46, 0x64, 0x65, 0x6C, 0x4F, 0x6E, 0x77, 0x75, 0x6B, 0x61};


 // The shared secret is FuelMetric
uint8_t hmacKey[] = {0x46, 0x75, 0x65, 0x6C, 0x4D, 0x65, 0x74, 0x72, 0x69, 0x63};

uint8_t connected;

uint8_t serverTimeFlag = 0;

 uint16_t fast_flow_threshold1  = 0,  //=  pulser_index_c ;   //140;
		 fast_flow_threshold2  = 0,
		 slowFlow_startThreshold1 = 0,
		 slowFlow_startThreshold2 = 0,
		 slowFlow_endThreshold1 = 0,
		 slowFlow_endThreshold2 = 0;

extern uint8_t change_p1,
			   change_v1,
			   change_p2,
			   change_v2;

extern float auth_v1,
			 auth_p1,
			 auth_v2,
			 auth_p2;

extern float amt_real1,
		  	 amt_real2,
			 price_real1,
			 price_real2;
extern int t;

extern uint16_t _tt1,
				_tt2,
				totalizer1Timer,
				totalizer2Timer,
				priceChange_timer1,
				priceChange_timer2;

extern uint32_t transaction_period,
				transaction_period2;

extern float display_minimumCentilitre1,
			 display_minimumCentilitre2;

extern uint8_t calibrationCan_measure1,
			   calibrationCan_measure2;

 float price = 0.0;
 float amt = 0.0;
 int auth_flag = 0;

 float price2 = 0.0;
 float amt2 = 0.0;
 int auth_flag2 = 0;

 int operating_side = 0;

// uint8_t dummyValue = 0;
 //-------------------------------------
//.....        Access level       .....
int access_level = non;    //default

//---- To be retrieved from storage..
//--------------------------------------
    sellmode_ sellmode = P;
    sellmode_ sellmode2 = P;

	float totaliser_vol1 = 0.00,
	      totaliser_vol1c = 0.00,
	      totaliser_vol2 = 0.00,
		  totaliser_vol2c = 0.00;

	float firstTotaliser_vol1 = 0.00,
		  firstTotaliser_vol1c = 0.00,
		  firstTotaliser_vol2 = 0.00,
		  firstTotaliser_vol2c = 0.00;

	float totaliser_amt1 = 0.00,
		  totaliser_amt1c = 0.00,
		  totaliser_amt2 = 0.00,
		  totaliser_amt2c = 0.00,
		  priceOld1 = 0.00,
		  priceOld2 = 0.00;

	float startShiftTotaliser_vol1 = 0.00,
		  startShiftTotaliser_vol1c = 0.00,
		  startShiftTotaliser_vol2 = 0.00,
		  startShiftTotaliser_vol2c = 0.00;

	float startShiftTotaliser_amt1 = 0.00,
		  startShiftTotaliser_amt1c = 0.00,
		  startShiftTotaliser_amt2 = 0.00,
		  startShiftTotaliser_amt2c = 0.00;

	float working_volTotaliser1 = 0;
	float working_volTotaliser1c = 0;
	float running_volTotaliser1 = 0;
	float running_volTotaliser1c = 0;

	float working_volTotaliser2 = 0;
	float working_volTotaliser2c = 0;
	float running_volTotaliser2 = 0;
	float running_volTotaliser2c = 0;

	float working_amtTotaliser1 = 0,
			working_amtTotaliser1c = 0,
			running_amtTotaliser1 = 0,
			running_amtTotaliser1c = 0,
			working_amtTotaliser2 = 0,
			working_amtTotaliser2c = 0,
			running_amtTotaliser2 = 0,
			running_amtTotaliser2c = 0;

 float pulser_index = 500;
 float pulser_index_c = 500;

 float pulser_index2 = 500;
 float pulser_index_c2 = 500;

 uint8_t dp_amount1 = 2,
		 dp_vol1 = 2,
		 dp_unitprice1 = 2;

 uint8_t dp_amount2 = 2,
		 dp_vol2 = 2,
		 dp_unitprice2 = 2;


float lastVolumeSale1 = 0.00,
      lastVolumeSale1c = 0.00;
float lastVolumeSale2 = 0.00,
	  lastVolumeSale2c = 0.00;

float lastAmountSale1 = 0.00,
      lastAmountSale1c = 0.00;
float lastAmountSale2 = 0.00,
	  lastAmountSale2c = 0.00;

float price_upper1,
	  amt_middle1,
	  price_upper2,
	  amt_middle2;


 int8_t opmode  = MANUAL_MODE;
 int8_t opmode2 = MANUAL_MODE;

 nozzle_override override_ = nooverride;
 nozzle_override override_2 = nooverride;

 float litre_price  = 165.00;
 float litre_price1 = 162.00;
 float litre_price2 = 180.00;

 int timeout_picknozzle = 30;
 int timeout_dispense   = 60;
 int timeout_noflow = 60;


//================================
// int pump_type = bluesky;   //lafeng;
// int pump_type2 = bluesky;	//LAFNG885

 pump pump_type,
      disp_type1,
      disp_type2;

// displaytype  disp_type1;
// displaytype  disp_type2;

// int disp_type1 = BLSKY886_N;
// int disp_type2 = BLSKY886_N;




 int max_dp = 3;
//================================
  char password_level1[9] = {'0', '0', '0', '0', '\0'};
  char password_level2[9] = {'0', '0', '0', '0', '\0'};
  char password_level3[9] = {'0', '0', '0', '0', '\0'};

//  uint32_t password_level1 = 1234;
//  uint32_t password_level2 = 12; // 12345678;
//  uint32_t password_level3 = 13;  //12345678;

 int log_max = 50;

// const int save_settings1_loc = 0;
// const int save_settings2_loc = 5;
// const int save_settings1_loc = 500;
// const int save_settings2_loc = 581;   //581 --> 660

 const uint16_t save_settings1_loc = 863,    //size => 32 Bytes
 	 	   	    save_settings2_loc = 895,    //895 --> 927
 	 	   	    save_settings3_loc = 927,    //size => 28 Bytes
 	 	   	    save_settings4_loc = 955,    //955 --> 983
				save_settings5_loc = 983,    //size => 28 Bytes
				save_settings6_loc = 1011;   //1011 --> 1039

 const uint16_t save_settings_original1_loc = 1040,   //size => 32 Bytes
		 	    save_settings_original2_loc = 1072,   //1072 --> 1104
				save_settings_original3_loc = 1104,   //size => 28 Bytes
				save_settings_original4_loc = 1132,   //1132 --> 1243
				save_settings_original5_loc = 1160,   //size => 28 Bytes
				save_settings_original6_loc = 1188;   //1188 --> 1216


 //TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT//

 //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW//
 ////////////////////////////////  FRAM MEMORY LOCATIONS ///////////////////////////////////
 //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//


 const uint16_t save_settings1_loc_fram = 0,    //size => 32 Bytes
  	 	   	    save_settings2_loc_fram = 32,    //32 --> 63
  	 	   	    save_settings3_loc_fram = 64,    //size => 28 Bytes
  	 	   	    save_settings4_loc_fram = 92,    //92 --> 119
 				save_settings5_loc_fram = 120,    //size => 28 Bytes
 				save_settings6_loc_fram = 148;   //148 --> 175


 const uint16_t lastSale1_loc_fram = 176;
 const uint16_t lastSale2_loc_fram = ( lastSale1_loc_fram + (4*4) );  //4bytes*4=16bytes = 16bytes ahead.  // 192 -> 207

// const uint16_t totVol1_loc_fram  =  208;
// const uint16_t totVol2_loc_fram =  totVol1_loc_fram + (2+(2*4));   // 218 -> 227
//
// const uint16_t totAmount1_loc_fram  =  228;
// const uint16_t totAmount2_loc_fram =  totAmount1_loc_fram + (2+(2*4));  // 238 -> 247

 const uint16_t tot1_loc_fram = 208;   //16 Bytes
 const uint16_t tot2_loc_fram = 224;   // 224 -> 239    //239 - 247 = -8

 const uint16_t save_pumpType_loc_fram = 248,
 	 	 	 	save_productType_loc_fram = save_pumpType_loc_fram + 1,
				save_nozzleId_loc_fram = save_pumpType_loc_fram + 2,
				totalizerDay_loc_fram = 251;


 const uint16_t firstTotVol1_loc_fram  =  252,
 	 	 	 	firstTotVol2_loc_fram =  firstTotVol1_loc_fram + (1+(3*4));   // 265 -> 277

 const uint16_t ct_settings1_loc_fram  =  278,
 	 	 	 	ct_settings2_loc_fram =  ct_settings1_loc_fram + (1+(3*4));   // 291 -> 303

 const uint16_t calib_pulser1_loc_fram  =  304,
 	 	 	 	calib_pulser2_loc_fram =  calib_pulser1_loc_fram + (1 + 4);   // 309 -> 313

 const uint16_t ctTimed_settings1_loc_fram  =  314,
		 	 	ctTimed_settings2_loc_fram =  ctTimed_settings1_loc_fram + (1+(4*4));   // 331 -> 347

 const uint16_t ctTimed_flag1_loc_fram = 348,
		 	 	ctTimed_flag2_loc_fram = ctTimed_flag1_loc_fram + 2;   // 350 -> 351

 const uint16_t calibrationDetails1_loc_fram  =  352,   //Size => 6 Bytes
		 	 	calibrationDetails2_loc_fram =  calibrationDetails1_loc_fram + (1+(3*2));   // 359 -> 365

 const uint16_t configFlag1_loc_fram = 366,
		   	    configFlag2_loc_fram = 367;

 const uint16_t calibrationFlag1_loc_fram = 368,
		   	    calibrationFlag2_loc_fram = 369;

 const uint16_t flash_stoA_fram = 370,    // 8 bytes
		 	 	flash_stoB_fram = 378;    //378 --> 385     //386 - 500 = -114

 const uint16_t startShiftTot1_loc_fram = 386,   // 16Bytes
		 	 	startShiftTot2_loc_fram = 401;   // 401 -> 417     //418 - 500 = -88

 const uint16_t config_otpSeed_time1_fram = 500,
		 	 	config_otpSeed_time2_fram = 508;  //508 --> 515

 const uint16_t save_settings_original1_loc_fram = 516,   //size => 32 Bytes
 		 	    save_settings_original2_loc_fram = 548,   //548 --> 580
 				save_settings_original3_loc_fram = 580,   //size => 28 Bytes
 				save_settings_original4_loc_fram = 608,   //608 --> 636
 				save_settings_original5_loc_fram = 636,   //size => 28 Bytes
 				save_settings_original6_loc_fram = 664;   //664 --> 691

 const uint16_t configChange_timeStamp1_loc_fram = 692,   //size => 4 Bytes
		 	 	configChange_timeStamp2_loc_fram = 696;   //696 --> 699

 const uint16_t track_num1_loc_fram = 700,
		 	 	track_num2_loc_fram = 701;

 const uint16_t otpSeed_session1_loc_fram = 702,          //size => 12 Bytes
 		   	    otpSeed_session2_loc_fram = 714;          //714 --> 725

 const uint16_t online_calibFlag1_loc_fram = 726,
		 	 	online_calibFlag2_loc_fram = 727;


 //YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY//


  //--------------------------------------------------------------
//  eeprom locations of the storage of  flash memory parameters
// const int flash_info_sto  =  1217;    //page
// const int flash_stoA =  0;    		 //capacity for 2 * 32bit integer
// const int flash_stoB =  10;    	 //+ 2 * 4bytes  +2     //1237
//--------------------------------------------------------------

//GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG//
//======  FRAM locations of the storage of  flash memory parameters  =====//

const int flash_info_sto  =  1217;    //8 bytes
const int flash_stoA =  0;
const int flash_stoB =  1225; //+ ( 1 + (32 * 2));    	 //1225 --> 1232   //1233 - 1347 = -114

//GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG//

 //==============================================================
 const int attendant_sto = 1347;     //page 12
 const int att1_loc      = 0;      //offset  0.
 const int att2_loc      = 20;     //offset  20.     //1367 --> 1386

 //==============================================================


// //--------------------------------------------------------------
// //  eeprom locations of the storage of  flash memory parameters
// const int flash_info_sto  =  11;    //page
// const int flash_stoA =  0;    		 //capacity for 2 * 32bit integer
// const int flash_stoB =  10;    	 //+ 2 * 4bytes  +2
//
// //==============================================================
// const int attendant_sto = 12;     //page 12
// const int att1_loc      = 0;      //offset  0.
// const int att2_loc      = 20;     //offset  20.
//
// //==============================================================

 const int configFlag1_loc = 1387,
		   configFlag2_loc = 1388;


 const int otp_loc = 1389,
		   otp1_loc = 0,
		   otp2_loc = otp1_loc + 7;  //1396 --> 1402


 const int sessionId_loc = 661,
		   sessionId1_loc = 0,
		   sessionId2_loc = sessionId1_loc + 9;  ////670 --> 678

 const int startShiftTotVol_loc  =  679,
 	 	   startShiftTotVol1_loc =  0,
 	 	   startShiftTotVol2_loc =  startShiftTotVol1_loc + (2+(2*4));   // 689 -> 698

 const int startShiftTotAmount_loc  =  699,
 	 	   startShiftTotAmount1_loc =  0,
 	 	   startShiftTotAmount2_loc =  startShiftTotAmount1_loc + (2+(2*4));  // 709 -> 718

 const int calibrationFlag1_loc = 800,
		   calibrationFlag2_loc = 801;


 const int amountSend_loc  =  803;       //For the intermittent transmission of regular-sized sale
 const int amountSend1_loc =  0;
 const int amountSend2_loc =  amountSend1_loc + (4 + 1);  // 808 -> 812

 const int16_t ctTimed_settings_loc  =  813;
 const int16_t ctTimed_settings1_loc =  0;
 const int16_t ctTimed_settings2_loc =  ctTimed_settings1_loc + (1+(4*4));   // 830 -> 847

 const int16_t calibrationDetails_loc  =  848;   //Size => 6 Bytes
 const int16_t calibrationDetails1_loc =  0;
 const int16_t calibrationDetails2_loc =  calibrationDetails1_loc + (1+(3*2));   // 855 -> 862

 const int16_t save_pumpType_loc = 400;
 const int16_t save_productType_loc = save_pumpType_loc + 1;
 const int16_t save_nozzleId_loc = save_pumpType_loc + 2;
 const uint16_t totalizerDay_loc = 404;

// const int8_t lastSale_loc  =  40;
// const int lastSale1_loc =  0;
// const int lastSale2_loc =  lastSale1_loc + (2+(2*4));  //4bytes*2=8bytes+2 = 10bytes ahead.  // 50 -> 60

 const int16_t lastSale_loc = 282;
 const int lastSale1_loc = 0;
 const int lastSale2_loc = ( lastSale1_loc + (4*4) );  //4bytes*4=16bytes = 16bytes ahead.  // 298 -> 314

 const int totVol_loc  =  10;
 const int totVol1_loc =  0;
 const int totVol2_loc =  totVol1_loc + (2+(2*4));   // 20 -> 30

 const int8_t firstTotVol_loc  =  90;
 const int8_t firstTotVol1_loc =  0;
 const int8_t firstTotVol2_loc =  firstTotVol1_loc + (3+(3*4));   // 105 -> 120

 const int totAmount_loc  =  230;
 const int totAmount1_loc =  0;
 const int totAmount2_loc =  totAmount1_loc + (2+(2*4));  // 240 -> 250

 const int8_t ct_settings_loc  =  121;
 const int ct_settings1_loc =  0;
 const int ct_settings2_loc =  ct_settings1_loc + (1+(3*4));   // 134 -> 147

 const int8_t calib_pulser_loc  =  148;
 const int calib_pulser1_loc =  0;
 const int calib_pulser2_loc =  calib_pulser1_loc + (1 + 4);   // 153 -> 158

// const int8_t ctTimed_settings_loc  =  159;
// const int8_t ctTimed_settings1_loc =  0;
// const int8_t ctTimed_settings2_loc =  ctTimed_settings1_loc + (1+(3*4));   // 172 -> 185

 const int8_t ctTimed_flag_loc = 186;
 const int8_t ctTimed_flag1_loc = 0;
 const int8_t ctTimed_flag2_loc = ctTimed_flag1_loc + 2;   // 188 -> 190

 const int8_t original_pi_c_loc = 191;
 const int8_t original_pi_c1_loc = 0;
 const int8_t original_pi_c2_loc = (original_pi_c1_loc + 4);   // 195 -> 199

// const int8_t original_pi_c_loc = 190;
// const int8_t original_pi_c1_loc = 0;
// const int8_t original_pi_c2_loc = original_pi_c1_loc + (1 + (2 *4));   // 199 -> 208

// //--------------------------------------------------------------
// //  eeprom locations of the storage of  flash memory parameters
// const int flash_info_sto  =  11;    //page
// const int flash_stoA =  0;    		 //capacity for 2 * 32bit integer
// const int flash_stoB =  10;    	 //+ 2 * 4bytes  +2
//
// //==============================================================
// const int attendant_sto = 12;     //page 12
// const int att1_loc      = 0;      //offset  0.
// const int att2_loc      = 20;     //offset  20.
//
// //==============================================================
 // define the settings structure of the settings.
 //==============================================================

// pump_settings settings[2], copy[2], settings0[2];

 pump_settings_stream1 settings_stream1[2],
 					   settings_original_stream1[2],
					   copy_stream1[2];

 pump_settings_stream2 settings_stream2[2],
 				       settings_original_stream2[2],
					   copy_stream2[2];

 pump_settings_stream3 settings_stream3[2],
 				       settings_original_stream3[2],
					   copy_stream3[2];

log_new log_a_new,
		log_b_new,
		synchedLog_a_new,
		synchedLog_b_new;

//log_new1 log_a_new1, log_b_new1;

_amountSend amountSend[2];

_calibrationData calibrationData[2];

flash_store_info flash_infoA,
				 flash_infoB;

totaliser_store totaliser_storeA,
				totaliser_storeB,
				startShiftTotaliser_storeA,
				startShiftTotaliser_storeB;

firstTotaliser_store firstTotaliser_vol_storeA,
					 firstTotaliser_vol_storeB,
					 firstTtotaliser_amt_storeA,
					 firstTtotaliser_amt_storeB;

lastSale_store lastSale_storeA,
			   lastSale_storeB;

_sessionId sessionId[2];

ct_settings ct_settingsA,
			ct_settingsB;

ctTimed_settings ctTimed_settingsA,
			     ctTimed_settingsB;

_original_pi_c original_pi_c;

ctTimed_flag_ ctTimed_flag;

time_ timeA, timeB;
date_ dateA, dateB;

const uint32_t flash_beginA = 0x000000;
const uint32_t flash_endA   = 0x3fffff;

const uint32_t flash_beginB = 0x400000;
const uint32_t flash_endB   = 0x7fffff;


//   MENU ITEMS

 char* menu1[4]  = {"  tot   ","  log   "};
 char* menu2[16] = {"  NNode ","Address "," Nozzle ","Prog.type","  Price ","no flo. t","Hi. Litre","Ch  Pass","Send cfg","Get. cfg ","Clr log","Clr tot","Calibrat.","Display ", " Clock"};
 char* menu3[3]  = {"tmm cfg","flo rate"};

 char* menu_level1[4] = {"Shft. Tot ", "  Price ", " Clock"};
 char* menu_level2[21] = {"  NNode ", "Address ", " Nozzle ", "Disp.Styl", "  Price ", "No flo. t", "Hi. Litre", "Ch  Pass", "Calibrat.", "DP Count", " Clock", "Sides No", "Start CL", "Calib.Can", "Shift.Typ", "Shift No", "  Tone  ", " Pulser", "  Valve ", "No-Calib"};
 char* menu_level3[23] = {"  NNode ", "Address ", " Nozzle ", "Disp.Styl", "  Price ", "No flo. t", "Hi. Litre", "Ch  Pass", "Calibrat.", "DP Count", " Clock", "Sides No", "Start CL", "Calib.Can", "Shift.Typ", "Shift No", "  Tone  ", " Pulser", "  Valve ", "No-Calib", "Calib.Typ", "Conn.Card"};


 char* pass[4] = {" pass 1 ", " pass 2 ", " pass 3 "};
 char* ch_pass[4] = {"pass.1", "pass.2", "pass.3"};

 char* login_type[4] = {"[ None ]", "[ Code ]", "[ Card ]"};


 //========================================
 /*
  *   Load Settings
 */
void load_settings(pump_sid side)
{
     uint8_t sdd = 0;
//   if(side == side_b)
//   {
//	   sdd = 1;
//   }

   if (side == side_a)
   {
	     sdd = 0;
//			litre_price1 = settings[0].price_;
			//totaliser_vol2 = 12345678.00;

	    sellmode = settings_stream1[sdd].def_t;
	    //totaliser_vol1 = 57638694.00;

	    override_ = settings_stream1[sdd].noz_override;

	    pulser_index = settings_stream1[sdd].pi_real;
	    pulser_index_c = settings_stream1[sdd].pi_cal;

	    fast_flow_threshold1 = pulser_index_c;

	    opmode  = settings_stream1[sdd].mode;

	    litre_price  = settings_stream1[sdd].price_;
	    litre_price1 = settings_stream1[sdd].price_;

	    timeout_picknozzle = 60;     //30;
	    timeout_dispense   = 250;    //60;
	    timeout_noflow = settings_stream2[sdd].timeOut_noFlow;

	    pump_type = settings_stream1[sdd].pump_type_;

	    strncpy(password_level1, settings_stream3[sdd].passwd1, sizeof(password_level1) );
	    strncpy(password_level2, settings_stream3[sdd].passwd2, sizeof(password_level2) );
	    strncpy(password_level3, settings_stream3[sdd].passwd3, sizeof(password_level3) );
//	    password_level2 = settings[sdd].passwd2;
//	    password_level3 = settings[sdd].passwd3;

	    dp_amount1 = settings_stream1[sdd].dp_amount;
	    dp_vol1 = settings_stream1[sdd].dp_vol;
	    dp_unitprice1 = settings_stream1[sdd].dp_unitprice;
	    pump_max_litres1 = settings_stream1[sdd].max_amt_;

//	    settings[sdd].side_size = 2;   // 1/2    ==> default : 2   // Level 2
//		settings[sdd].display_mode = PL;  // PL/LP   ==> default : PL  // Level 2
//		settings[sdd].keypress_tone = Yes;  // Yes/No   ==> default : No   // Level 2

		display_minimumCentilitre1 = settings_stream2[sdd].startUp_suppressVol;  // (0 - 10) cL   ==> default : 4cL  // Level 2
		calibrationCan_measure1 = settings_stream2[sdd].calibration_measureCan;  // 10L/20L   ==> default : 20L  // Level 2

	    //============== FOR PETROL STATION PUMP =================//
//	    settings[sdd].startUp_suppressVol = 0.5;  // (0 - 10) dL   ==> default : 0.5L  // Level 2
//		settings[sdd].calibration_measureCan = 100;  // 10L/20L   ==> default : 20L  // Level 2
//	    settings[sdd].shift_login_type = None_; // None_/Code_/Card_   ==> default : None_  // Level 2

//	    settings[sdd].commCard_enforced = true;  // Yes/No   ==> default : Yes   // Level 3
//	    settings[sdd].calibration_type = Wizard; // Wizard/Manual_calib   ==> default : Wizard   // Level 3
//
//	    settings[sdd].pulser_type_ = non_quadrature;   //quadrature;
//	    settings[sdd].pulser_offset = 2;
//	    settings[sdd].fastValve = 20;
//	    settings[sdd].non_calibration_seed = 0;

   }
   else
   {
	    sdd = 1;

		litre_price2 = settings_stream1[sdd].price_;
		sellmode2 = settings_stream1[sdd].def_t;
		//totaliser_vol1 = 57638694.00;

		override_2 = settings_stream1[sdd].noz_override;

		pulser_index2 = settings_stream1[sdd].pi_real;
		pulser_index_c2 = settings_stream1[sdd].pi_cal;
		opmode2  = settings_stream1[sdd].mode;

		fast_flow_threshold2 = pulser_index_c2;

	   // litre_price  = settings[sdd].price_;
		litre_price2 = settings_stream1[sdd].price_;

		timeout_picknozzle = 30;
		timeout_dispense   = 60;
		timeout_noflow = settings_stream2[sdd].timeOut_noFlow;

		pump_type = settings_stream1[sdd].pump_type_;

		strncpy(password_level1, settings_stream3[sdd].passwd1, sizeof(password_level1) );
		strncpy(password_level2, settings_stream3[sdd].passwd2, sizeof(password_level2) );
		strncpy(password_level3, settings_stream3[sdd].passwd3, sizeof(password_level3) );
//		password_level1 = settings[sdd].passwd1;
//		password_level2 = settings[sdd].passwd2;
//		password_level3 = settings[sdd].passwd3;

		dp_amount2 = settings_stream1[sdd].dp_amount;
		dp_vol2 = settings_stream1[sdd].dp_vol;
		dp_unitprice2 = settings_stream1[sdd].dp_unitprice;
		pump_max_litres2 = settings_stream1[sdd].max_amt_;

		display_minimumCentilitre2 = settings_stream2[sdd].startUp_suppressVol;  // (0 - 10) cL   ==> default : 4cL  // Level 2
		calibrationCan_measure2 = settings_stream2[sdd].calibration_measureCan;  // 10L/20L   ==> default : 20L  // Level 2
   }
}

//========================================
/*
 *   Make Settings
 */
void make_settings(pump_sid side)
{
	int sdd = 0;

	 totaliser_vol1 = 0.00;
	 totaliser_vol2 = 0.00;
	 totaliser_amt1 = 0.00;
	 totaliser_amt2 = 0.00;

   if(side == side_b)
   {
	   sdd = 1;
   }
   	settings_stream1[sdd].def_t = P;    //P/L  ==> default : P
   	settings_stream1[sdd].display_format = PL;  // PL/LP   ==> default : PL  // Level 2

   	settings_stream1[sdd].pi_real = 407.3;   //399.25;   //798.35;
   	settings_stream1[sdd].pi_cal = 391.64;   //383.89;  //760.33;
//	    settings[0].pi_ = 407.3;   //399.25;   //798.35;
//	  	settings[0].pi_c = 391.64;   //383.89;  //760.33;
//	    settings[1].pi_ = 399.25;   //798.35;
//	  	settings[1].pi_c = 383.89;  //760.33;
   	settings_stream1[sdd].mode = MANUAL_MODE;   //AUTO;

   	settings_stream1[sdd].price_ = 648.00;

	    timeout_picknozzle = 60;
	    timeout_dispense   = 250;

	    settings_stream2[sdd].timeOut_noFlow = 300;

	    settings_stream1[sdd].pump_type_ = BLUESKY;    // LAFENG;
//	    settings[sdd].pump_type_ = pump_type;

	    strncpy(settings_stream3[sdd].passwd1, "0000", 9);
	    strncpy(settings_stream3[sdd].passwd2, "0000", 9);
	    strncpy(settings_stream3[sdd].passwd3, "0000", 9);

	    settings_stream1[sdd].dp_amount = 2;
	    settings_stream1[sdd].dp_vol    = 2;
	    settings_stream1[sdd].dp_unitprice = 2;

	    settings_stream1[sdd].noz_override = override;  //nooveride;
	    settings_stream1[sdd].max_amt_ = 999999999;   //Maximum pump litres

	    settings_stream2[sdd].noz_count = 2;   // 1/2    ==> default : 2   // Level 2
	    settings_stream2[sdd].keypress_tone = Yes;  // Yes/No   ==> default : No   // Level 2

	    settings_stream2[sdd].startUp_suppressVol = 0.04;  // (0 - 10) cL   ==> default : 4cL  // Level 2
	    settings_stream2[sdd].calibration_measureCan = 20;  // 10L/20L   ==> default : 20L  // Level 2
	    settings_stream2[sdd].shift_login_type = None_; // None_/Code_/Card_   ==> default : None_  // Level 2
	    settings_stream2[sdd].number_of_shifts = 1;

	    settings_stream2[sdd].commCard_enforced = true;  // Yes/No   ==> default : Yes   // Level 3
	    settings_stream2[sdd].calibration_type = Wizard; // Wizard/Manual_calib   ==> default : Wizard   // Level 3
	    settings_stream2[sdd].pulser_type_ = quadrature;   //non_quadrature;   //quadrature;
	    settings_stream2[sdd].pulser_offset = 0;
	    settings_stream2[sdd].valve_salesStart = 0.15;
	    settings_stream2[sdd].valve_salesEnd = 1.0;
	    settings_stream2[sdd].non_calibration_seed = 0;

	     /*
		   if (sdd == side_a)
		   {
				settings[1].price_ = 120;
		   }
		   else
		   {
			   settings[0].price_ = 180;
		   }   */
}
//==============================================
/*
 *  Save Settings
 */

//void save_settings(void)
//{
//	extern const int save_settings1_loc;
//	extern const int save_settings2_loc;
//    int sz = sizeof(copy[0]);
//
//  // for (int i = 0 ; i < sz;i++)
//  // {
//	   EEPROM_Write(save_settings1_loc, 0, &settings[0], sz);
//	   EEPROM_Write(save_settings2_loc, 0, &settings[1], sz);
//  // }
//}


//==============================================
/*
 *  Save Settings
 */
void save_settings(void)
{
	uint8_t sz;

	sz = sizeof(copy_stream1[0]);

	EEPROM_Write(save_settings1_loc, 0, &settings_stream1[0], sz);
	HAL_Delay(2);
	EEPROM_Write(save_settings2_loc, 0, &settings_stream1[1], sz);
	HAL_Delay(2);

	sz = sizeof(copy_stream2[0]);

	EEPROM_Write(save_settings3_loc, 0, &settings_stream2[0], sz);
	HAL_Delay(2);
	EEPROM_Write(save_settings4_loc, 0, &settings_stream2[1], sz);
	HAL_Delay(2);

	sz = sizeof(copy_stream3[0]);

	EEPROM_Write(save_settings5_loc, 0, &settings_stream3[0], sz);
	HAL_Delay(2);
	EEPROM_Write(save_settings6_loc, 0, &settings_stream3[1], sz);
}

void save_settings_fram(void)
{
	uint8_t sz;

	sz = sizeof(copy_stream1[0]);

//	void FRAM_Write(uint16_t memAddress, uint8_t *data, uint16_t size);

	FRAM_Write(save_settings1_loc_fram, &settings_stream1[0], sz);
	HAL_Delay(2);
	FRAM_Write(save_settings2_loc_fram, &settings_stream1[1], sz);
	HAL_Delay(2);

	sz = sizeof(copy_stream2[0]);

	FRAM_Write(save_settings3_loc_fram, &settings_stream2[0], sz);
	HAL_Delay(2);
	FRAM_Write(save_settings4_loc_fram, &settings_stream2[1], sz);
	HAL_Delay(2);

	sz = sizeof(copy_stream3[0]);

	FRAM_Write(save_settings5_loc_fram, &settings_stream3[0], sz);
	HAL_Delay(2);
	FRAM_Write(save_settings6_loc_fram, &settings_stream3[1], sz);
}


//void save_settings(void)
//{
//	uint8_t sz;
//
//	sz = sizeof(copy_stream1[0]);
//
//	EEPROM_Write(0, 0, &settings_stream1[0], sz);
//	HAL_Delay(10);
//	EEPROM_Write(1, 0, &settings_stream1[1], sz);
//	HAL_Delay(10);
//
//	sz = sizeof(copy_stream2[0]);
//
//	EEPROM_Write(2, 0, &settings_stream2[0], sz);
//	HAL_Delay(10);
//	EEPROM_Write(3, 0, &settings_stream2[1], sz);
//	HAL_Delay(10);
//
//	sz = sizeof(copy_stream3[0]);
//
//	EEPROM_Write(4, 0, &settings_stream3[0], sz);
//	HAL_Delay(10);
//	EEPROM_Write(5, 0, &settings_stream3[1], sz);
//}

////==============================================
///*
// * save_settings0
// */
//
//void save_settings0(void)
//{
//	extern const int save_settings01_loc;
//	extern const int save_settings02_loc;
//    int8_t sz = sizeof(copy[0]);
//
//  // for (int i = 0 ; i < sz;i++)
//  // {
//	   EEPROM_Write(save_settings01_loc, 0, &settings0[0], sz);
//	   EEPROM_Write(save_settings02_loc, 0, &settings0[1], sz);
//  // }
//}


//==============================================
/*
 * save_settings0
 */

void save_settings_original(void)
{
	uint8_t sz;

	sz = sizeof(settings_original_stream1[0]);

	EEPROM_Write(save_settings_original1_loc, 0, &settings_original_stream1[0], sz);
	EEPROM_Write(save_settings_original2_loc, 0, &settings_original_stream1[1], sz);

	sz = sizeof(settings_original_stream2[0]);

	EEPROM_Write(save_settings_original3_loc, 0, &settings_original_stream2[0], sz);
	EEPROM_Write(save_settings_original4_loc, 0, &settings_original_stream2[1], sz);

	sz = sizeof(settings_original_stream3[0]);

	EEPROM_Write(save_settings_original5_loc, 0, &settings_original_stream3[0], sz);
	EEPROM_Write(save_settings_original6_loc, 0, &settings_original_stream3[1], sz);
}

void save_settings_original_fram(pump_sid side)
{
	uint8_t sz;

	if(side == side_a)
	{
		sz = sizeof(settings_original_stream1[0]);
		FRAM_Write(save_settings_original1_loc_fram, &settings_original_stream1[0], sz);

		sz = sizeof(settings_original_stream2[0]);
		FRAM_Write(save_settings_original3_loc_fram, &settings_original_stream2[0], sz);

		sz = sizeof(settings_original_stream3[0]);
		FRAM_Write(save_settings_original5_loc_fram, &settings_original_stream3[0], sz);

//		sz = sizeof(configChange[0].time_stamp);
//		configChange[0].time_stamp = RtcToInt(2019);
//		FRAM_Write(configChange_timeStamp1_loc_fram, &configChange[0].time_stamp, sz);
	}
	else if(side == side_b)
	{
		sz = sizeof(settings_original_stream1[0]);
		FRAM_Write(save_settings_original2_loc_fram, &settings_original_stream1[1], sz);

		sz = sizeof(settings_original_stream2[0]);
		FRAM_Write(save_settings_original4_loc_fram, &settings_original_stream2[1], sz);

		sz = sizeof(settings_original_stream3[0]);
		FRAM_Write(save_settings_original6_loc_fram, &settings_original_stream3[1], sz);

//		sz = sizeof(configChange[0].time_stamp);
//		configChange[1].time_stamp = RtcToInt(2019);
//		FRAM_Write(configChange_timeStamp2_loc_fram, &configChange[1].time_stamp, sz);
	}
}

//-------------------------------------





//==============================================
/*
 *  Retrieve Settings
 */

void retrieve_settings(void)
{
	uint8_t sz;

	sz = sizeof(copy_stream1[0]);

//   if( EEPROM_Read(save_settings1_loc, 0, &settings_original_stream1[0], sz) );
//   else
//	   storage_fail = 1;
   EEPROM_Read(save_settings1_loc, 0, &settings_stream1[0], sz);
   HAL_Delay(2);
   EEPROM_Read(save_settings2_loc, 0, &settings_stream1[1], sz);

//   if( EEPROM_Read(save_settings2_loc, 0, &settings_original_stream1[1], sz) );
//   else
//	   storage_fail = 1;

	sz = sizeof(copy_stream2[0]);

   HAL_Delay(2);
   EEPROM_Read(save_settings3_loc, 0, &settings_stream2[0], sz);
   HAL_Delay(2);
   EEPROM_Read(save_settings4_loc, 0, &settings_stream2[1], sz);

	sz = sizeof(copy_stream3[0]);

   HAL_Delay(2);
   EEPROM_Read(save_settings5_loc, 0, &settings_stream3[0], sz);
   HAL_Delay(2);
   EEPROM_Read(save_settings6_loc, 0, &settings_stream3[1], sz);

   dp_init(side_a);
   dp_init(side_b);
}

void retrieve_settings_fram(void)
{
	uint8_t sz;

	sz = sizeof(copy_stream1[0]);

   FRAM_Read(save_settings1_loc_fram, &settings_stream1[0], sz);
   HAL_Delay(2);
   FRAM_Read(save_settings2_loc_fram, &settings_stream1[1], sz);

	sz = sizeof(copy_stream2[0]);

   HAL_Delay(2);
   FRAM_Read(save_settings3_loc_fram, &settings_stream2[0], sz);
   HAL_Delay(2);
   FRAM_Read(save_settings4_loc_fram, &settings_stream2[1], sz);

	sz = sizeof(copy_stream3[0]);

   HAL_Delay(2);
   FRAM_Read(save_settings5_loc_fram, &settings_stream3[0], sz);
   HAL_Delay(2);
   FRAM_Read(save_settings6_loc_fram, &settings_stream3[1], sz);

   dp_init(side_a);
   dp_init(side_b);
}

//=============================================
/*
 *  Retrieve Settings
 */
void retrieve_settings_original(void)
{
	uint8_t sz;

	sz = sizeof(settings_original_stream1[0]);

	EEPROM_Read(save_settings_original1_loc, 0, &settings_original_stream1[0], sz);
	HAL_Delay(2);
	EEPROM_Read(save_settings_original2_loc, 0, &settings_original_stream1[1], sz);
	HAL_Delay(2);

	sz = sizeof(settings_original_stream2[0]);

	EEPROM_Read(save_settings_original3_loc, 0, &settings_original_stream2[0], sz);
	HAL_Delay(2);
	EEPROM_Read(save_settings_original4_loc, 0, &settings_original_stream2[1], sz);
	HAL_Delay(2);

	sz = sizeof(settings_original_stream3[0]);

	EEPROM_Read(save_settings_original5_loc, 0, &settings_original_stream3[0], sz);
	HAL_Delay(2);
	EEPROM_Read(save_settings_original6_loc, 0, &settings_original_stream3[1], sz);
}


void retrieve_settings_original_fram(pump_sid side)
{
	uint8_t sz;

	if(side == side_a)
	{
		sz = sizeof(settings_original_stream1[0]);
		FRAM_Read(save_settings_original1_loc_fram, &settings_original_stream1[0], sz);
		HAL_Delay(2);

		sz = sizeof(settings_original_stream2[0]);
		FRAM_Read(save_settings_original3_loc_fram, &settings_original_stream2[0], sz);
		HAL_Delay(2);

		sz = sizeof(settings_original_stream3[0]);
		FRAM_Read(save_settings_original5_loc_fram, &settings_original_stream3[0], sz);
		HAL_Delay(2);

//		sz = sizeof(configChange[0].time_stamp);
//		FRAM_Read(configChange_timeStamp1_loc_fram, &configChange[0].time_stamp, sz);
	}
	else if(side == side_b)
	{
		sz = sizeof(settings_original_stream1[0]);
		FRAM_Read(save_settings_original2_loc_fram, &settings_original_stream1[1], sz);
		HAL_Delay(2);

		sz = sizeof(settings_original_stream2[0]);
		FRAM_Read(save_settings_original4_loc_fram, &settings_original_stream2[1], sz);
		HAL_Delay(2);

		sz = sizeof(settings_original_stream3[0]);
		FRAM_Read(save_settings_original6_loc_fram, &settings_original_stream3[1], sz);

//		sz = sizeof(configChange[0].time_stamp);
//		FRAM_Read(configChange_timeStamp2_loc_fram, &configChange[1].time_stamp, sz);
	}
}

//==============================================
/*
 * save volumeTotaliser
 */
void save_volumeTotaliser(pump_sid side)
{
//	int sz = sizeof( totaliser_vol_storeA);
//
//	if (side == side_a)
//	  {
//		//EEPROM_Write_NUM(totVol_loc, totVol1_loc, tot);
//		  totaliser_vol_storeA.totaliserVol_cal = totaliser_vol1c;
//	  	  totaliser_vol_storeA.totaliserVol_real = totaliser_vol1;
//	  	  EEPROM_Write(totVol_loc, totVol1_loc, &totaliser_vol_storeA, sz);
//	  }
//	else if (side == side_b)
//	  {
//		//EEPROM_Write_NUM(totVol_loc, totVol2_loc, tot);
//		  totaliser_vol_storeB.totaliserVol_cal = totaliser_vol2c;
//	  	  totaliser_vol_storeB.totaliserVol_real = totaliser_vol2;
//	  	  EEPROM_Write(totVol_loc, totVol2_loc, &totaliser_vol_storeB, sz);
//	  }
}

void save_totaliser_fram(pump_sid side)
{
	uint8_t sz = sizeof(totaliser_storeA);

	if (side == side_a)
	{
		totaliser_storeA.totaliserVol_cal = totaliser_vol1c;
		totaliser_storeA.totaliserVol_real = totaliser_vol1;
		totaliser_storeA.totaliserAmount_cal = totaliser_amt1c;
		totaliser_storeA.totaliserAmount_real = totaliser_amt1;
		FRAM_Write(tot1_loc_fram, &totaliser_storeA, sz);
	}
	else if (side == side_b)
	{
		totaliser_storeB.totaliserVol_cal = totaliser_vol2c;
		totaliser_storeB.totaliserVol_real = totaliser_vol2;
		totaliser_storeB.totaliserAmount_cal = totaliser_amt2c;
		totaliser_storeB.totaliserAmount_real = totaliser_amt2;
		FRAM_Write(tot2_loc_fram, &totaliser_storeB, sz);
	}
}

//===================================================
/*
 *  read volumeTotaliser
 */
void retrieve_volumeTotaliser(pump_sid side)
{
//  int sz = sizeof( totaliser_vol_storeA);
//	if (side == side_a)
//	{
//		//EEPROM_Read_NUM(totVol_loc,totVol1_loc);
//		EEPROM_Read(totVol_loc, totVol1_loc, &totaliser_vol_storeA, sz);
//		totaliser_vol1c =  totaliser_vol_storeA.totaliserVol_cal;
//		totaliser_vol1 = totaliser_vol_storeA.totaliserVol_real;
//
//	  	if(isnan(totaliser_vol1c)) totaliser_vol1c = 0.0;
//	  	if(isnan(totaliser_vol1)) totaliser_vol1 = 0.0;
//
//	}
//	else if (side == side_b)
//	{
//		//EEPROM_Read_NUM(totVol_loc,totVol2_loc);
//		 EEPROM_Read(totVol_loc, totVol2_loc, &totaliser_vol_storeB, sz);
//		 totaliser_vol2c = totaliser_vol_storeB.totaliserVol_cal;
//	  	 totaliser_vol2  = totaliser_vol_storeB.totaliserVol_real;
//
//	  	if(isnan(totaliser_vol2c)) totaliser_vol2c = 0.0;
//	  	if(isnan(totaliser_vol2)) totaliser_vol2 = 0.0;
//
//	}
}

void retrieve_totaliser_fram(pump_sid side)
{
	uint8_t sz = sizeof(totaliser_storeA);

	if (side == side_a)
	{
		FRAM_Read(tot1_loc_fram, &totaliser_storeA, sz);

		totaliser_vol1c = totaliser_storeA.totaliserVol_cal;
		totaliser_vol1 = totaliser_storeA.totaliserVol_real;

		totaliser_amt1c = totaliser_storeA.totaliserAmount_cal;
		totaliser_amt1 = totaliser_storeA.totaliserAmount_real;

	  	if(isnan(totaliser_vol1c)) totaliser_vol1c = 0.0;
	  	if(isnan(totaliser_vol1)) totaliser_vol1 = 0.0;
	  	if(isnan(totaliser_amt1c)) totaliser_amt1c = 0.0;
	  	if(isnan(totaliser_amt1)) totaliser_amt1 = 0.0;

	}
	else if (side == side_b)
	{
	  	FRAM_Read(tot2_loc_fram, &totaliser_storeB, sz);

		totaliser_vol2c = totaliser_storeB.totaliserVol_cal;
		totaliser_vol2 = totaliser_storeB.totaliserVol_real;

		totaliser_amt2c = totaliser_storeB.totaliserAmount_cal;
		totaliser_amt2 = totaliser_storeB.totaliserAmount_real;

	  	if(isnan(totaliser_vol2c)) totaliser_vol2c = 0.0;
	  	if(isnan(totaliser_vol2)) totaliser_vol2 = 0.0;
	  	if(isnan(totaliser_amt2c)) totaliser_amt2c = 0.0;
	  	if(isnan(totaliser_amt2)) totaliser_amt2 = 0.0;
	}
}

//==============================================
/*
 * clear volumeTotaliser
 */
void clear_volumeTotaliser(pump_sid side)
{
//	int sz = sizeof( totaliser_vol_storeA);
//
//	if (side == side_a)
//	  {
//		  totaliser_vol_storeA.totaliserVol_cal = 0.00;
//	  	  totaliser_vol_storeA.totaliserVol_real = 0.00;
//	  	  EEPROM_Write(totVol_loc, totVol1_loc, &totaliser_vol_storeA, sz);
//	  }
//	else if (side == side_b)
//	  {
//		  totaliser_vol_storeB.totaliserVol_cal = 0.00;
//	  	  totaliser_vol_storeB.totaliserVol_real = 0.00;
//	  	  EEPROM_Write(totVol_loc, totVol2_loc, &totaliser_vol_storeB, sz);
//	  }
}

void clear_totaliser_fram(pump_sid side)
{
	uint8_t sz = sizeof(totaliser_storeA);

	if (side == side_a)
	{
		totaliser_storeA.totaliserVol_cal = 0.00;
		totaliser_storeA.totaliserVol_real = 0.00;
		totaliser_storeA.totaliserAmount_cal = 0.00;
		totaliser_storeA.totaliserAmount_real = 0.00;
		FRAM_Write(tot1_loc_fram, &totaliser_storeA, sz);
	}
	else if (side == side_b)
	{
		totaliser_storeB.totaliserVol_cal = 0.00;
		totaliser_storeB.totaliserVol_real = 0.00;
		totaliser_storeB.totaliserAmount_cal = 0.00;
		totaliser_storeB.totaliserAmount_real = 0.00;
		FRAM_Write(tot2_loc_fram, &totaliser_storeB, sz);
	}
}



//==============================================
/*
 * save Start-Shift volumeTotaliser
 */
void save_Totaliser_startShift_fram(pump_sid side)
{
	uint8_t sz = sizeof(startShiftTotaliser_storeA);

	if (side == side_a)
	{
		startShiftTotaliser_storeA.totaliserVol_cal = totaliser_vol1c;
		startShiftTotaliser_storeA.totaliserVol_real = totaliser_vol1;
		startShiftTotaliser_storeA.totaliserAmount_cal = totaliser_amt1c;
		startShiftTotaliser_storeA.totaliserAmount_real = totaliser_amt1;
		FRAM_Write(startShiftTot1_loc_fram, &startShiftTotaliser_storeA, sz);
	}
	else if (side == side_b)
	{
		startShiftTotaliser_storeB.totaliserVol_cal = totaliser_vol2c;
		startShiftTotaliser_storeB.totaliserVol_real = totaliser_vol2;
		startShiftTotaliser_storeB.totaliserAmount_cal = totaliser_amt2c;
		startShiftTotaliser_storeB.totaliserAmount_real = totaliser_amt2;
		FRAM_Write(startShiftTot2_loc_fram, &startShiftTotaliser_storeB, sz);
	}
}


//===================================================
/*
 *  read Start-Shift volumeTotaliser
 */
void retrieve_Totaliser_startShift_fram(pump_sid side)
{
	uint8_t sz = sizeof(startShiftTotaliser_storeA);

	if (side == side_a)
	{
		FRAM_Read(startShiftTot1_loc_fram, &startShiftTotaliser_storeA, sz);

		startShiftTotaliser_vol1c = startShiftTotaliser_storeA.totaliserVol_cal;
		startShiftTotaliser_vol1 = startShiftTotaliser_storeA.totaliserVol_real;

		startShiftTotaliser_amt1c = startShiftTotaliser_storeA.totaliserAmount_cal;
		startShiftTotaliser_amt1 = startShiftTotaliser_storeA.totaliserAmount_real;

		if(isnan(startShiftTotaliser_vol1c)) startShiftTotaliser_vol1c = 0.0;
		if(isnan(startShiftTotaliser_vol1)) startShiftTotaliser_vol1 = 0.0;
		if(isnan(startShiftTotaliser_amt1c)) startShiftTotaliser_amt1c = 0.0;
		if(isnan(startShiftTotaliser_amt1)) startShiftTotaliser_amt1 = 0.0;

	}
	else if (side == side_b)
	{
		FRAM_Read(startShiftTot2_loc_fram, &startShiftTotaliser_storeB, sz);

		startShiftTotaliser_vol2c = startShiftTotaliser_storeB.totaliserVol_cal;
		startShiftTotaliser_vol2 = startShiftTotaliser_storeB.totaliserVol_real;

		startShiftTotaliser_amt2c = startShiftTotaliser_storeB.totaliserAmount_cal;
		startShiftTotaliser_amt2 = startShiftTotaliser_storeB.totaliserAmount_real;

		if(isnan(startShiftTotaliser_vol2c)) startShiftTotaliser_vol2c = 0.0;
		if(isnan(startShiftTotaliser_vol2)) startShiftTotaliser_vol2 = 0.0;
		if(isnan(startShiftTotaliser_amt2c)) startShiftTotaliser_amt2c = 0.0;
		if(isnan(startShiftTotaliser_amt2)) startShiftTotaliser_amt2 = 0.0;
	}
}



//==============================================
/*
 * clear Start-Shift volumeTotaliser
 */
void clear_Totaliser_startShift_fram(pump_sid side)
{
	uint8_t sz = sizeof(startShiftTotaliser_storeA);

	if (side == side_a)
	{
		startShiftTotaliser_storeA.totaliserVol_cal = 0.00;
		startShiftTotaliser_storeA.totaliserVol_real = 0.00;
		startShiftTotaliser_storeA.totaliserAmount_cal = 0.00;
		startShiftTotaliser_storeA.totaliserAmount_real = 0.00;
		FRAM_Write(startShiftTot1_loc_fram, &startShiftTotaliser_storeA, sz);
	}
	else if (side == side_b)
	{
		startShiftTotaliser_storeB.totaliserVol_cal = 0.00;
		startShiftTotaliser_storeB.totaliserVol_real = 0.00;
		startShiftTotaliser_storeB.totaliserAmount_cal = 0.00;
		startShiftTotaliser_storeB.totaliserAmount_real = 0.00;
		FRAM_Write(startShiftTot2_loc_fram, &startShiftTotaliser_storeB, sz);
	}
}



void save_volumeTotaliser_startShift(pump_sid side)
{
//	int sz = sizeof(startShiftTotaliser_vol_storeA);
//
//	if (side == side_a)
//	  {
////		EEPROM_Write_NUM(totVol_loc, totVol1_loc, tot);
//		startShiftTotaliser_vol_storeA.totaliserVol_cal = totaliser_vol1c;
//		startShiftTotaliser_vol_storeA.totaliserVol_real = totaliser_vol1;
//	  	EEPROM_Write(startShiftTotVol_loc, startShiftTotVol1_loc, &startShiftTotaliser_vol_storeA, sz);
//	  }
//	else if (side == side_b)
//	  {
//		//EEPROM_Write_NUM(totVol_loc, totVol2_loc, tot);
//		startShiftTotaliser_vol_storeB.totaliserVol_cal = totaliser_vol2c;
//		startShiftTotaliser_vol_storeB.totaliserVol_real = totaliser_vol2;
//	  	EEPROM_Write(startShiftTotVol_loc, startShiftTotVol2_loc, &startShiftTotaliser_vol_storeB, sz);
//	  }
}


//===================================================
/*
 *  read Start-Shift volumeTotaliser
 */
void retrieve_volumeTotaliser_startShift(pump_sid side)
{
//  int sz = sizeof(startShiftTotaliser_vol_storeA);
//	if (side == side_a)
//	{
//		//EEPROM_Read_NUM(totVol_loc,totVol1_loc);
//		EEPROM_Read(startShiftTotVol_loc, startShiftTotVol1_loc, &startShiftTotaliser_vol_storeA, sz);
//		startShiftTotaliser_vol1c =  startShiftTotaliser_vol_storeA.totaliserVol_cal;
//		startShiftTotaliser_vol1 = startShiftTotaliser_vol_storeA.totaliserVol_real;
//
//	  	if(isnan(startShiftTotaliser_vol1c)) startShiftTotaliser_vol1c = 0.0;
//	  	if(isnan(startShiftTotaliser_vol1)) startShiftTotaliser_vol1 = 0.0;
//
//	}
//	else if (side == side_b)
//	{
//		//EEPROM_Read_NUM(totVol_loc,totVol2_loc);
//		 EEPROM_Read(startShiftTotVol_loc, startShiftTotVol2_loc, &startShiftTotaliser_vol_storeB, sz);
//		 startShiftTotaliser_vol2c = startShiftTotaliser_vol_storeB.totaliserVol_cal;
//		 startShiftTotaliser_vol2  = startShiftTotaliser_vol_storeB.totaliserVol_real;
//
//	  	if(isnan(startShiftTotaliser_vol2c)) startShiftTotaliser_vol2c = 0.0;
//	  	if(isnan(startShiftTotaliser_vol2)) startShiftTotaliser_vol2 = 0.0;
//
//	}
}

//==============================================
/*
 * clear Start-Shift volumeTotaliser
 */
void clear_volumeTotaliser_startShift(pump_sid side)
{
//	int sz = sizeof(startShiftTotaliser_vol_storeA);
//
//	if (side == side_a)
//	  {
//		startShiftTotaliser_vol_storeA.totaliserVol_cal = 0.00;
//		startShiftTotaliser_vol_storeA.totaliserVol_real = 0.00;
//	  	EEPROM_Write(startShiftTotVol_loc, startShiftTotVol1_loc, &startShiftTotaliser_vol_storeA, sz);
//	  }
//	else if (side == side_b)
//	  {
//		startShiftTotaliser_vol_storeB.totaliserVol_cal = 0.00;
//		startShiftTotaliser_vol_storeB.totaliserVol_real = 0.00;
//	  	EEPROM_Write(startShiftTotVol_loc, startShiftTotVol2_loc, &startShiftTotaliser_vol_storeB, sz);
//	  }
}


//==============================================
/*
 * save volumeTotaliser, first for the day
 */
void save_1stVolTotaliser_day(pump_sid side)
{
	int sz = sizeof(firstTotaliser_vol_storeA);

	if (side == side_a)
	  {
		  firstTotaliser_vol_storeA.totaliserVol_cal = totaliser_vol1c;
	  	  firstTotaliser_vol_storeA.totaliserVol_real = totaliser_vol1;
	  	  firstTotaliser_vol_storeA.timestamp = RtcToInt(2019);
	  	  EEPROM_Write(firstTotVol_loc, firstTotVol1_loc, &firstTotaliser_vol_storeA, sz);
	  }
	else if (side == side_b)
	  {
		  firstTotaliser_vol_storeB.totaliserVol_cal = totaliser_vol2c;
	  	  firstTotaliser_vol_storeB.totaliserVol_real = totaliser_vol2;
	  	  firstTotaliser_vol_storeB.timestamp = firstTotaliser_vol_storeA.timestamp;
	  	  EEPROM_Write(firstTotVol_loc, firstTotVol2_loc, &firstTotaliser_vol_storeB, sz);
	  }
}

void save_1stVolTotaliser_day_fram(pump_sid side)
{
	int sz = sizeof(firstTotaliser_vol_storeA);

	if (side == side_a)
	  {
		  firstTotaliser_vol_storeA.totaliserVol_cal = totaliser_vol1c;
	  	  firstTotaliser_vol_storeA.totaliserVol_real = totaliser_vol1;
	  	  firstTotaliser_vol_storeA.timestamp = RtcToInt(2019);
	  	  FRAM_Write(firstTotVol1_loc_fram, &firstTotaliser_vol_storeA, sz);
	  }
	else if (side == side_b)
	  {
		  firstTotaliser_vol_storeB.totaliserVol_cal = totaliser_vol2c;
	  	  firstTotaliser_vol_storeB.totaliserVol_real = totaliser_vol2;
	  	  firstTotaliser_vol_storeB.timestamp = firstTotaliser_vol_storeA.timestamp;
	  	  FRAM_Write(firstTotVol2_loc_fram, &firstTotaliser_vol_storeB, sz);
	  }
}
//===================================================
/*
 *  read volumetotaliser, firstfor the day
 */
void retrieve_1stVolTotaliser_day(pump_sid side)
{
  int sz = sizeof(firstTotaliser_vol_storeA);
	if (side == side_a)
	{
		EEPROM_Read(firstTotVol_loc, firstTotVol1_loc, &firstTotaliser_vol_storeA, sz);
		ep5_save.firstTotalizer[0].totalizer =  firstTotaliser_vol_storeA.totaliserVol_cal;
		ep5_save.firstTotalizer[0].totalizer_real = firstTotaliser_vol_storeA.totaliserVol_real;
		ep5_save.firstTotalizer[0].timestamp = firstTotaliser_vol_storeA.timestamp;

		if(isnan(ep5_save.firstTotalizer[0].totalizer)) ep5_save.firstTotalizer[0].totalizer = 0.0;
		if(isnan(ep5_save.firstTotalizer[0].totalizer_real)) ep5_save.firstTotalizer[0].totalizer_real = 0.0;
//		if(isnan(ep5_save.firstTotalizer[0].timestamp)) ep5_save.firstTotalizer[0].timestamp = 0;

	}
	else if (side == side_b)
	{
		EEPROM_Read(firstTotVol_loc, firstTotVol2_loc, &firstTotaliser_vol_storeB, sz);
		ep5_save.firstTotalizer[1].totalizer = firstTotaliser_vol_storeB.totaliserVol_cal;
		ep5_save.firstTotalizer[1].totalizer_real  = firstTotaliser_vol_storeB.totaliserVol_real;
	  	ep5_save.firstTotalizer[1].timestamp = firstTotaliser_vol_storeB.timestamp;

	  	if(isnan(ep5_save.firstTotalizer[1].totalizer)) ep5_save.firstTotalizer[1].totalizer = 0.0;
	  	if(isnan(ep5_save.firstTotalizer[1].totalizer_real)) ep5_save.firstTotalizer[1].totalizer_real = 0.0;
//	  	if(isnan(ep5_save.firstTotalizer[1].timestamp)) ep5_save.firstTotalizer[1].timestamp = 0;

	}
}

void retrieve_1stVolTotaliser_day_fram(pump_sid side)
{
  int sz = sizeof(firstTotaliser_vol_storeA);
	if (side == side_a)
	{
		FRAM_Read(firstTotVol1_loc_fram, &firstTotaliser_vol_storeA, sz);
		ep5_save.firstTotalizer[0].totalizer =  firstTotaliser_vol_storeA.totaliserVol_cal;
		ep5_save.firstTotalizer[0].totalizer_real = firstTotaliser_vol_storeA.totaliserVol_real;
		ep5_save.firstTotalizer[0].timestamp = firstTotaliser_vol_storeA.timestamp;

		if(isnan(ep5_save.firstTotalizer[0].totalizer)) ep5_save.firstTotalizer[0].totalizer = 0.0;
		if(isnan(ep5_save.firstTotalizer[0].totalizer_real)) ep5_save.firstTotalizer[0].totalizer_real = 0.0;
	}
	else if (side == side_b)
	{
		FRAM_Read(firstTotVol2_loc_fram, &firstTotaliser_vol_storeB, sz);
		ep5_save.firstTotalizer[1].totalizer = firstTotaliser_vol_storeB.totaliserVol_cal;
		ep5_save.firstTotalizer[1].totalizer_real  = firstTotaliser_vol_storeB.totaliserVol_real;
	  	ep5_save.firstTotalizer[1].timestamp = firstTotaliser_vol_storeB.timestamp;

	  	if(isnan(ep5_save.firstTotalizer[1].totalizer)) ep5_save.firstTotalizer[1].totalizer = 0.0;
	  	if(isnan(ep5_save.firstTotalizer[1].totalizer_real)) ep5_save.firstTotalizer[1].totalizer_real = 0.0;
	}
}

//==============================================
/*
 * clear volumeTotaliser, first of the day
 */
void clear_1stvolTotaliser_day(pump_sid side)
{
	int sz = sizeof(firstTotaliser_vol_storeA);

	if (side == side_a)
	  {
		firstTotaliser_vol_storeA.totaliserVol_cal = 0.00;
		firstTotaliser_vol_storeA.totaliserVol_real = 0.00;
		firstTotaliser_vol_storeA.timestamp = 4325376;
		EEPROM_Write(firstTotVol_loc, firstTotVol1_loc, &firstTotaliser_vol_storeA, sz);
	  }
	else if (side == side_b)
	  {
		firstTotaliser_vol_storeB.totaliserVol_cal = 0.00;
		firstTotaliser_vol_storeB.totaliserVol_real = 0.00;
		firstTotaliser_vol_storeB.timestamp = 4325376;
		EEPROM_Write(firstTotVol_loc, firstTotVol2_loc, &firstTotaliser_vol_storeB, sz);
	  }
}

void clear_1stvolTotaliser_day_fram(pump_sid side)
{
	int sz = sizeof(firstTotaliser_vol_storeA);

	if (side == side_a)
	  {
		firstTotaliser_vol_storeA.totaliserVol_cal = 0.00;
		firstTotaliser_vol_storeA.totaliserVol_real = 0.00;
		firstTotaliser_vol_storeA.timestamp = 4325376;
		FRAM_Write(firstTotVol1_loc_fram, &firstTotaliser_vol_storeA, sz);
	  }
	else if (side == side_b)
	  {
		firstTotaliser_vol_storeB.totaliserVol_cal = 0.00;
		firstTotaliser_vol_storeB.totaliserVol_real = 0.00;
		firstTotaliser_vol_storeB.timestamp = 4325376;
		FRAM_Write(firstTotVol2_loc_fram, &firstTotaliser_vol_storeB, sz);
	  }
}
//===================================================
/*
 * save lastSale
 */
void save_lastSale(pump_sid side)
{
	int8_t sz = sizeof(lastSale_storeA);

	if (side == side_a)
	  {
		//EEPROM_Write_NUM(totVol_loc, totVol1_loc, tot);
		lastSale_storeA.lastVolumeSale_real = amt_real1;   //    log_a_new.vol_ = amt_real1;

//		lastSale_storeA.lastVolumeSale_cal = atoff(amt_middle1);   //log_a_new.vol__ = amt;   //calibrated
		lastSale_storeA.lastVolumeSale_cal = amt_middle1;   //log_a_new.vol__ = amt;   //calibrated
//		lastSale_storeA.lastVolumeSale_cal += 0.00011;  //make small correction for the inherent rounddown.

		lastSale_storeA.lastAmountSale_real = price_real1;  //log_a_new.pr_ = price_real1;  //real

//		lastSale_storeA.lastAmountSale_cal = atoff(price_upper1);  //log_a_new.pr__ = price;  //calibrated
		lastSale_storeA.lastAmountSale_cal = price_upper1;  //log_a_new.pr__ = price;  //calibrated
//		lastSale_storeA.lastAmountSale_cal += 0.00011;  //make small correction for the inherent rounddown.

	  	EEPROM_Write(lastSale_loc, lastSale1_loc, &lastSale_storeA, sz);
	  }
	else if (side == side_b)
	  {
		//EEPROM_Write_NUM(totVol_loc, totVol2_loc, tot);
		lastSale_storeB.lastVolumeSale_real = amt_real2;   //    log_b_new.vol_ = amt_real2;

//		lastSale_storeB.lastVolumeSale_cal = atof(middle2);   //log_b_new.vol__ = amt2;   //calibrated
		lastSale_storeB.lastVolumeSale_cal = amt_middle2;   //log_b_new.vol__ = amt2;   //calibrated
//		lastSale_storeB.lastVolumeSale_cal += 0.00011;  //make small correction for the inherent rounddown.

		lastSale_storeB.lastAmountSale_real = price_real2;   // log_b_new.pr_ = price_real2;  //real

//		lastSale_storeB.lastAmountSale_cal = atof(upper2);  // log_b_new.pr__ = price2;  //calibrated
		lastSale_storeB.lastAmountSale_cal = price_upper2;  // log_b_new.pr__ = price2;  //calibrated
//		lastSale_storeB.lastAmountSale_cal += 0.00011;  //make small correction for the inherent rounddown.

	  	EEPROM_Write(lastSale_loc, lastSale2_loc, &lastSale_storeB, sz);
	  }
}

void save_lastSale_fram(pump_sid side)
{
	int8_t sz = sizeof(lastSale_storeA);

	if (side == side_a)
	  {

		lastSale_storeA.lastVolumeSale_real = amt_real1;   //    log_a_new.vol_ = amt_real1;

		lastSale_storeA.lastVolumeSale_cal = amt_middle1;   //log_a_new.vol__ = amt;   //calibrated

		lastSale_storeA.lastAmountSale_real = price_real1;  //log_a_new.pr_ = price_real1;  //real

		lastSale_storeA.lastAmountSale_cal = price_upper1;  //log_a_new.pr__ = price;  //calibrated

		FRAM_Write(lastSale1_loc_fram, &lastSale_storeA, sz);
	  }
	else if (side == side_b)
	  {
		lastSale_storeB.lastVolumeSale_real = amt_real2;   //    log_b_new.vol_ = amt_real2;

		lastSale_storeB.lastVolumeSale_cal = amt_middle2;   //log_b_new.vol__ = amt2;   //calibrated

		lastSale_storeB.lastAmountSale_real = price_real2;   // log_b_new.pr_ = price_real2;  //real

		lastSale_storeB.lastAmountSale_cal = price_upper2;  // log_b_new.pr__ = price2;  //calibrated

		FRAM_Write(lastSale2_loc_fram, &lastSale_storeB, sz);
	  }
}


//===================================================
/*
 *  read lastSale
 */
void retrieve_lastSale(pump_sid side)
{
  int8_t sz = sizeof(lastSale_storeA);

	if (side == side_a)
	{
		EEPROM_Read(lastSale_loc, lastSale1_loc, &lastSale_storeA, sz);
		lastVolumeSale1 = lastSale_storeA.lastVolumeSale_real;
		lastVolumeSale1c =  lastSale_storeA.lastVolumeSale_cal;
		lastAmountSale1 = lastSale_storeA.lastAmountSale_real;
		lastAmountSale1c =  lastSale_storeA.lastAmountSale_cal;

	  	if(isnan(lastVolumeSale1)) lastVolumeSale1 = 0.0;
	  	if(isnan(lastVolumeSale1c)) lastVolumeSale1c = 0.0;
	  	if(isnan(lastAmountSale1)) lastAmountSale1 = 0.0;
	  	if(isnan(lastAmountSale1c)) lastAmountSale1c = 0.0;

	}
	else if (side == side_b)
	{
		EEPROM_Read(lastSale_loc, lastSale2_loc, &lastSale_storeB, sz);
		lastVolumeSale2  = lastSale_storeB.lastVolumeSale_real;
		lastVolumeSale2c = lastSale_storeB.lastVolumeSale_cal;
		lastAmountSale2  = lastSale_storeB.lastAmountSale_real;
		lastAmountSale2c = lastSale_storeB.lastAmountSale_cal;

	  	if(isnan(lastVolumeSale2)) lastVolumeSale2 = 0.0;
	  	if(isnan(lastVolumeSale2c)) lastVolumeSale2c = 0.0;
	  	if(isnan(lastAmountSale2)) lastAmountSale2 = 0.0;
	  	if(isnan(lastAmountSale2c)) lastAmountSale2c = 0.0;

	}
}

void retrieve_lastSale_fram(pump_sid side)
{
  int8_t sz = sizeof(lastSale_storeA);

	if (side == side_a)
	{
		FRAM_Read(lastSale1_loc_fram, &lastSale_storeA, sz);
		lastVolumeSale1 = lastSale_storeA.lastVolumeSale_real;
		lastVolumeSale1c =  lastSale_storeA.lastVolumeSale_cal;
		lastAmountSale1 = lastSale_storeA.lastAmountSale_real;
		lastAmountSale1c =  lastSale_storeA.lastAmountSale_cal;

	  	if(isnan(lastVolumeSale1)) lastVolumeSale1 = 0.0;
	  	if(isnan(lastVolumeSale1c)) lastVolumeSale1c = 0.0;
	  	if(isnan(lastAmountSale1)) lastAmountSale1 = 0.0;
	  	if(isnan(lastAmountSale1c)) lastAmountSale1c = 0.0;

	}
	else if (side == side_b)
	{
		FRAM_Read(lastSale2_loc_fram, &lastSale_storeB, sz);
		lastVolumeSale2  = lastSale_storeB.lastVolumeSale_real;
		lastVolumeSale2c = lastSale_storeB.lastVolumeSale_cal;
		lastAmountSale2  = lastSale_storeB.lastAmountSale_real;
		lastAmountSale2c = lastSale_storeB.lastAmountSale_cal;

	  	if(isnan(lastVolumeSale2)) lastVolumeSale2 = 0.0;
	  	if(isnan(lastVolumeSale2c)) lastVolumeSale2c = 0.0;
	  	if(isnan(lastAmountSale2)) lastAmountSale2 = 0.0;
	  	if(isnan(lastAmountSale2c)) lastAmountSale2c = 0.0;

	}
}

//===================================================
/*
 * clear lastSale
 */
void clear_lastSale(pump_sid side)
{
	int8_t sz = sizeof(lastSale_storeA);

	if (side == side_a)
	  {
		lastSale_storeA.lastVolumeSale_real = 0.00;   //    log_a_new.vol_ = amt_real1;
		lastSale_storeA.lastVolumeSale_cal = 0.00;   //log_a_new.vol__ = amt;   //calibrated
		lastSale_storeA.lastAmountSale_real = 0.00;
		lastSale_storeA.lastAmountSale_cal = 0.00;
	  	EEPROM_Write(lastSale_loc, lastSale1_loc, &lastSale_storeA, sz);
	  }
	else if (side == side_b)
	  {
		lastSale_storeB.lastVolumeSale_real = 0.00;   //    log_b_new.vol_ = amt_real2;
		lastSale_storeB.lastVolumeSale_cal = 0.00;   //log_b_new.vol__ = amt2;   //calibrated
		lastSale_storeB.lastAmountSale_real = 0.00;
		lastSale_storeB.lastAmountSale_cal = 0.00;
	  	EEPROM_Write(lastSale_loc, lastSale2_loc, &lastSale_storeB, sz);
	  }
}

void clear_lastSale_fram(pump_sid side)
{
	int8_t sz = sizeof(lastSale_storeA);

	if (side == side_a)
	  {
		lastSale_storeA.lastVolumeSale_real = 0.00;   //    log_a_new.vol_ = amt_real1;
		lastSale_storeA.lastVolumeSale_cal = 0.00;   //log_a_new.vol__ = amt;   //calibrated
		lastSale_storeA.lastAmountSale_real = 0.00;
		lastSale_storeA.lastAmountSale_cal = 0.00;
		FRAM_Write(lastSale1_loc_fram, &lastSale_storeA, sz);
	  }
	else if (side == side_b)
	  {
		lastSale_storeB.lastVolumeSale_real = 0.00;   //    log_b_new.vol_ = amt_real2;
		lastSale_storeB.lastVolumeSale_cal = 0.00;   //log_b_new.vol__ = amt2;   //calibrated
		lastSale_storeB.lastAmountSale_real = 0.00;
		lastSale_storeB.lastAmountSale_cal = 0.00;
		FRAM_Write(lastSale2_loc_fram, &lastSale_storeB, sz);
	  }
}

//==============================================
/*
 * save amountTotaliser
 */
void save_amountTotaliser(pump_sid side)
{
//	int sz = sizeof( totaliser_vol_storeA);
//
//	if (side == side_a)
//	  {
//		  totaliser_storeA.totaliserAmount_cal = totaliser_amt1c;
//		  totaliser_storeA.totaliserAmount_real = totaliser_amt1;
//	  	  EEPROM_Write(totAmount_loc, totAmount1_loc, &totaliser_amt_storeA, sz);
//	  }
//	else if (side == side_b)
//	  {
//		  totaliser_amt_storeB.totaliserVol_cal = totaliser_amt2c;
//	  	  totaliser_amt_storeB.totaliserVol_real = totaliser_amt2;
//	  	  EEPROM_Write(totAmount_loc, totAmount2_loc, &totaliser_amt_storeB, sz);
//	  }
}

void save_amountTotaliser_fram(pump_sid side)
{
//	int sz = sizeof( totaliser_vol_storeA);
//
//	if (side == side_a)
//	  {
//		  totaliser_amt_storeA.totaliserVol_cal = totaliser_amt1c;
//	  	  totaliser_amt_storeA.totaliserVol_real = totaliser_amt1;
//	  	  FRAM_Write(totAmount1_loc_fram, &totaliser_amt_storeA, sz);
//	  }
//	else if (side == side_b)
//	  {
//		  totaliser_amt_storeB.totaliserVol_cal = totaliser_amt2c;
//	  	  totaliser_amt_storeB.totaliserVol_real = totaliser_amt2;
//	  	  FRAM_Write(totAmount2_loc_fram, &totaliser_amt_storeB, sz);
//	  }
}
//===================================================
/*
 *  read amountTotaliser
 */
void retrieve_amountTotaliser(pump_sid side)
{
//  int sz = sizeof( totaliser_amt_storeA);
//	if (side == side_a)
//	{
//		EEPROM_Read(totAmount_loc, totAmount1_loc, &totaliser_amt_storeA, sz);
//		totaliser_amt1c =  totaliser_amt_storeA.totaliserVol_cal;
//		totaliser_amt1 = totaliser_amt_storeA.totaliserVol_real;
//
//	  	if(isnan(totaliser_amt1c)) totaliser_amt1c = 0.0;
//	  	if(isnan(totaliser_amt1)) totaliser_amt1 = 0.0;
//
//	}
//	else if (side == side_b)
//	{
//		 EEPROM_Read(totAmount_loc, totAmount2_loc, &totaliser_amt_storeB, sz);
//		 totaliser_amt2c = totaliser_amt_storeB.totaliserVol_cal;
//	  	 totaliser_amt2  = totaliser_amt_storeB.totaliserVol_real;
//
//	  	if(isnan(totaliser_amt2c)) totaliser_amt2c = 0.0;
//	  	if(isnan(totaliser_amt2)) totaliser_amt2 = 0.0;
//
//	}
}

void retrieve_amountTotaliser_fram(pump_sid side)
{
//  int sz = sizeof( totaliser_amt_storeA);
//	if (side == side_a)
//	{
//		FRAM_Read(totAmount1_loc_fram, &totaliser_amt_storeA, sz);
//		totaliser_amt1c =  totaliser_amt_storeA.totaliserVol_cal;
//		totaliser_amt1 = totaliser_amt_storeA.totaliserVol_real;
//
//	  	if(isnan(totaliser_amt1c)) totaliser_amt1c = 0.0;
//	  	if(isnan(totaliser_amt1)) totaliser_amt1 = 0.0;
//
//	}
//	else if (side == side_b)
//	{
//		 FRAM_Read(totAmount1_loc_fram, &totaliser_amt_storeB, sz);
//		 totaliser_amt2c = totaliser_amt_storeB.totaliserVol_cal;
//	  	 totaliser_amt2  = totaliser_amt_storeB.totaliserVol_real;
//
//	  	if(isnan(totaliser_amt2c)) totaliser_amt2c = 0.0;
//	  	if(isnan(totaliser_amt2)) totaliser_amt2 = 0.0;
//
//	}
}
//==============================================
/*
 * clear amountTotaliser
 */
void clear_amountTotaliser(pump_sid side)
{
//	int sz = sizeof( totaliser_vol_storeA);
//
//	if (side == side_a)
//	  {
//		  totaliser_amt_storeA.totaliserVol_cal = 0.00;
//	  	  totaliser_amt_storeA.totaliserVol_real = 0.00;
//	  	  EEPROM_Write(totAmount_loc, totAmount1_loc, &totaliser_amt_storeA, sz);
//	  }
//	else if (side == side_b)
//	  {
//		  totaliser_amt_storeB.totaliserVol_cal = 0.00;
//	  	  totaliser_amt_storeB.totaliserVol_real = 0.00;
//	  	  EEPROM_Write(totAmount_loc, totAmount2_loc, &totaliser_amt_storeB, sz);
//	  }
}

void clear_amountTotaliser_fram(pump_sid side)
{
//	int sz = sizeof( totaliser_vol_storeA);
//
//	if (side == side_a)
//	  {
//		  totaliser_amt_storeA.totaliserVol_cal = 0.00;
//	  	  totaliser_amt_storeA.totaliserVol_real = 0.00;
//	  	  FRAM_Write(totAmount1_loc_fram, &totaliser_amt_storeA, sz);
//	  }
//	else if (side == side_b)
//	  {
//		  totaliser_amt_storeB.totaliserVol_cal = 0.00;
//	  	  totaliser_amt_storeB.totaliserVol_real = 0.00;
//	  	  FRAM_Write(totAmount2_loc_fram, &totaliser_amt_storeB, sz);
//	  }
}

//==============================================
/*
 * save ct_settings
 */
void save_ctSettings(pump_sid side)
{
	int8_t sz = sizeof(ct_settingsA);

	if (side == side_a)
	  {
//		  vol_effective1 = ( (atof(ep31_save.pump[0].calibrate_ct.ct_effectiveMinusBase) + vol_calibrated1) + 0.00011);
		  vol_effective1 = ( (atof(ep31_save.pump[0].calibrate_ct.ct_effectiveMinusBase) + vol_real1) + 0.00011);

		  ct_settingsA.original = vol_real1;
		  ct_settingsA.base = vol_calibrated1;
	  	  ct_settingsA.effective = vol_effective1;
	  	  EEPROM_Write(ct_settings_loc, ct_settings1_loc, &ct_settingsA, sz);
	  }
	else if (side == side_b)
	  {
//		  vol_effective2 = ( (atof(ep31_save.pump[1].calibrate_ct.ct_effectiveMinusBase) + vol_calibrated2) + 0.00011);
		  vol_effective2 = ( (atof(ep31_save.pump[1].calibrate_ct.ct_effectiveMinusBase) + vol_real2) + 0.00011);

		  ct_settingsB.original = vol_real2;
		  ct_settingsB.base = vol_calibrated2;
		  ct_settingsB.effective = vol_effective2;
	  	  EEPROM_Write(ct_settings_loc, ct_settings2_loc, &ct_settingsB, sz);
	  }
}


void save_ctSettings_fram(pump_sid side)
{
	int8_t sz = sizeof(ct_settingsA);

	if (side == side_a)
	  {
		  ct_settingsA.original = vol_real1;
		  ct_settingsA.base = vol_calibrated1;
	  	  ct_settingsA.effective = vol_effective1;
	  	  FRAM_Write(ct_settings1_loc_fram, &ct_settingsA, sz);
	  }
	else if (side == side_b)
	  {
		  ct_settingsB.original = vol_real2;
		  ct_settingsB.base = vol_calibrated2;
		  ct_settingsB.effective = vol_effective2;
	  	  FRAM_Write(ct_settings2_loc_fram, &ct_settingsB, sz);
	  }
}
//===================================================
/*
 *  read ct_settings
 */
void retrieve_ctSettings(pump_sid side)
{
  int8_t sz = sizeof(ct_settingsA);

	if (side == side_a)
	{
		EEPROM_Read(ct_settings_loc, ct_settings1_loc, &ct_settingsA, sz);

		vol_real1 =  ct_settingsA.original;
		vol_calibrated1 = ct_settingsA.base;
		vol_effective1 = ct_settingsA.effective;

	  	if(isnan(vol_calibrated1)) vol_calibrated1 = 0.0;
	  	if(isnan(vol_effective1)) vol_effective1 = 0.0;

	}
	else if (side == side_b)
	{
		 EEPROM_Read(ct_settings_loc, ct_settings2_loc, &ct_settingsB, sz);

		 vol_real2 = ct_settingsB.original;
		 vol_calibrated2  = ct_settingsB.base;
	  	 vol_effective2 = ct_settingsB.effective;

	  	if(isnan(vol_calibrated2)) vol_calibrated2 = 0.0;
	  	if(isnan(vol_effective2)) vol_effective2 = 0.0;

	}
}

void retrieve_ctSettings_fram(pump_sid side)
{
  int8_t sz = sizeof(ct_settingsA);

	if (side == side_a)
	{
		FRAM_Read(ct_settings1_loc_fram, &ct_settingsA, sz);

		vol_real1 =  ct_settingsA.original;
		vol_calibrated1 = ct_settingsA.base;
		vol_effective1 = ct_settingsA.effective;

	  	if(isnan(vol_calibrated1)) vol_calibrated1 = 0.0;
	  	if(isnan(vol_effective1)) vol_effective1 = 0.0;

	}
	else if (side == side_b)
	{
		 FRAM_Read(ct_settings2_loc_fram, &ct_settingsB, sz);

		 vol_real2 = ct_settingsB.original;
		 vol_calibrated2  = ct_settingsB.base;
	  	 vol_effective2 = ct_settingsB.effective;

	  	if(isnan(vol_calibrated2)) vol_calibrated2 = 0.0;
	  	if(isnan(vol_effective2)) vol_effective2 = 0.0;

	}
}
//==============================================
/*
 * clear ct_settings
 */
void clear_ctSettings(pump_sid side)
{
	int8_t sz = sizeof(ct_settingsA);

	if (side == side_a)
	  {
		  ct_settingsA.original = 0;
		  ct_settingsA.base = 0.00;
		  ct_settingsA.effective = 0.00;
	  	  EEPROM_Write(ct_settings_loc, ct_settings1_loc, &ct_settingsA, sz);
	  }
	else if (side == side_b)
	  {
		  ct_settingsB.original = 0;
		  ct_settingsB.base = 0.00;
		  ct_settingsB.effective = 0.00;
		  EEPROM_Write(ct_settings_loc, ct_settings2_loc, &ct_settingsB, sz);
	  }
}

void clear_ctSettings_fram(pump_sid side)
{
	int8_t sz = sizeof(ct_settingsA);

	if (side == side_a)
	  {
		  ct_settingsA.original = 0;
		  ct_settingsA.base = 0.00;
		  ct_settingsA.effective = 0.00;
	  	  FRAM_Write(ct_settings1_loc_fram, &ct_settingsA, sz);
	  }
	else if (side == side_b)
	  {
		  ct_settingsB.original = 0;
		  ct_settingsB.base = 0.00;
		  ct_settingsB.effective = 0.00;
		  FRAM_Write(ct_settings2_loc_fram, &ct_settingsB, sz);
	  }
}

//==============================================
/*
 * save ctTimed_settings
 */
void save_ctTimedSettings(pump_sid side)
{
	int8_t sz = sizeof(ctTimed_settingsA);

	if (side == side_a)
	  {
//		  vol_effective1_1 = ( (atof(ep31_save.pump[0].calibrate_ct.ct_effectiveMinusBase) + vol_calibrated1) + 0.00011);
		  vol_effective1_1 = ( (atof(ep31_save.pump[0].calibrate_ct.ct_effectiveMinusBase) + vol_real1) + 0.00011);

		  ctTimed_settingsA.effective = vol_effective1_1;
		  ctTimed_settingsA.startTime = atoi(ep31_save.pump[0].calibrate_ct.ct_startTime);
	  	  ctTimed_settingsA.endTime = atoi(ep31_save.pump[0].calibrate_ct.ct_endTime);
	  	  ctTimed_settingsA.day = settings_stream2[0].totalizer_day;
	  	  EEPROM_Write(ctTimed_settings_loc, ctTimed_settings1_loc, &ctTimed_settingsA, sz);
	  }
	else if (side == side_b)
	  {
//		  vol_effective2_2 = ( (atof(ep31_save.pump[1].calibrate_ct.ct_effectiveMinusBase) + vol_calibrated2) + 0.00011);
		  vol_effective2_2 = ( (atof(ep31_save.pump[1].calibrate_ct.ct_effectiveMinusBase) + vol_real2) + 0.00011);

		  ctTimed_settingsB.effective = vol_effective2_2;
		  ctTimed_settingsB.startTime = atoi(ep31_save.pump[1].calibrate_ct.ct_startTime);
	  	  ctTimed_settingsB.endTime = atoi(ep31_save.pump[1].calibrate_ct.ct_endTime);
	  	  ctTimed_settingsB.day = settings_stream2[0].totalizer_day;
	  	  EEPROM_Write(ctTimed_settings_loc, ctTimed_settings2_loc, &ctTimed_settingsB, sz);
	  }
}

void save_ctTimedSettings_fram(pump_sid side)
{
	int8_t sz = sizeof(ctTimed_settingsA);

	if (side == side_a)
	  {
//		  vol_effective1_1 = ( (atof(ep31_save.pump[0].calibrate_ct.ct_effectiveMinusBase) + vol_calibrated1) + 0.00011);
		  vol_effective1_1 = ( (atof(ep31_save.pump[0].calibrate_ct.ct_effectiveMinusBase) + vol_real1) + 0.00011);

		  ctTimed_settingsA.effective = vol_effective1_1;
		  ctTimed_settingsA.startTime = atoi(ep31_save.pump[0].calibrate_ct.ct_startTime);
	  	  ctTimed_settingsA.endTime = atoi(ep31_save.pump[0].calibrate_ct.ct_endTime);
	  	  ctTimed_settingsA.day = settings_stream2[0].totalizer_day;
	  	  FRAM_Write(ctTimed_settings1_loc_fram, &ctTimed_settingsA, sz);
	  }
	else if (side == side_b)
	  {
//		  vol_effective2_2 = ( (atof(ep31_save.pump[1].calibrate_ct.ct_effectiveMinusBase) + vol_calibrated2) + 0.00011);
		  vol_effective2_2 = ( (atof(ep31_save.pump[1].calibrate_ct.ct_effectiveMinusBase) + vol_real2) + 0.00011);

		  ctTimed_settingsB.effective = vol_effective2_2;
		  ctTimed_settingsB.startTime = atoi(ep31_save.pump[1].calibrate_ct.ct_startTime);
	  	  ctTimed_settingsB.endTime = atoi(ep31_save.pump[1].calibrate_ct.ct_endTime);
	  	  ctTimed_settingsB.day = settings_stream2[0].totalizer_day;
	  	  FRAM_Write(ctTimed_settings2_loc_fram, &ctTimed_settingsB, sz);
	  }
}

//===================================================
/*
 *  read ctTimed_settings
 */
void retrieve_ctTimedSettings(pump_sid side)
{
  int8_t sz = sizeof(ctTimed_settingsA);

	if (side == side_a)
	{
		EEPROM_Read(ctTimed_settings_loc, ctTimed_settings1_loc, &ctTimed_settingsA, sz);

		vol_effective1_1 =  ctTimed_settingsA.effective;
		startTime1 = ctTimed_settingsA.startTime;
		endTime1 = ctTimed_settingsA.endTime;
		ctTimed_day1 = ctTimed_settingsA.day;

	  	if(isnan(vol_effective1_1)) vol_effective1_1 = 0.0;

	}
	else if (side == side_b)
	{
		 EEPROM_Read(ctTimed_settings_loc, ctTimed_settings2_loc, &ctTimed_settingsB, sz);

		 vol_effective2_2 =  ctTimed_settingsB.effective;
		 startTime2 = ctTimed_settingsB.startTime;
		 endTime2 = ctTimed_settingsB.endTime;
		 ctTimed_day2 = ctTimed_settingsB.day;

		 if(isnan(vol_effective2_2)) vol_effective2_2 = 0.0;

	}
}


void retrieve_ctTimedSettings_fram(pump_sid side)
{
  int8_t sz = sizeof(ctTimed_settingsA);

	if (side == side_a)
	{
		FRAM_Read(ctTimed_settings1_loc_fram, &ctTimed_settingsA, sz);

		vol_effective1_1 =  ctTimed_settingsA.effective;
		startTime1 = ctTimed_settingsA.startTime;
		endTime1 = ctTimed_settingsA.endTime;
		ctTimed_day1 = ctTimed_settingsA.day;

	  	if(isnan(vol_effective1_1)) vol_effective1_1 = 0.0;

	}
	else if (side == side_b)
	{
		 FRAM_Read(ctTimed_settings2_loc_fram, &ctTimed_settingsB, sz);

		 vol_effective2_2 =  ctTimed_settingsB.effective;
		 startTime2 = ctTimed_settingsB.startTime;
		 endTime2 = ctTimed_settingsB.endTime;
		 ctTimed_day2 = ctTimed_settingsB.day;

		 if(isnan(vol_effective2_2)) vol_effective2_2 = 0.0;

	}
}


//==============================================
/*
 * clear ctTimed_settings
 */
void clear_ctTimedSettings(pump_sid side)
{
	int8_t sz = sizeof(ctTimed_settingsA);

	if (side == side_a)
	  {
		  ctTimed_settingsA.effective = 0.0;
		  ctTimed_settingsA.startTime = 0;
		  ctTimed_settingsA.endTime = 0;
	  	  EEPROM_Write(ctTimed_settings_loc, ctTimed_settings1_loc, &ctTimed_settingsA, sz);
	  }
	else if (side == side_b)
	  {
		  ctTimed_settingsB.effective = 0.0;
		  ctTimed_settingsB.startTime = 0;
		  ctTimed_settingsB.endTime = 0;
		  EEPROM_Write(ctTimed_settings_loc, ctTimed_settings2_loc, &ctTimed_settingsB, sz);
	  }
}

void clear_ctTimedSettings_fram(pump_sid side)
{
	int8_t sz = sizeof(ctTimed_settingsA);

	if (side == side_a)
	  {
		  ctTimed_settingsA.effective = 0.0;
		  ctTimed_settingsA.startTime = 0;
		  ctTimed_settingsA.endTime = 0;
	  	  FRAM_Write(ctTimed_settings1_loc_fram, &ctTimed_settingsA, sz);
	  }
	else if (side == side_b)
	  {
		  ctTimed_settingsB.effective = 0.0;
		  ctTimed_settingsB.startTime = 0;
		  ctTimed_settingsB.endTime = 0;
		  FRAM_Write(ctTimed_settings2_loc_fram, &ctTimed_settingsB, sz);
	  }
}

//==============================================
/*
 * save ctTimedFlag
 */
void save_ctTimedFlag(pump_sid side)
{
	int8_t sz = sizeof(ctTimed_flag);

	if (side == side_a)
	  {
		  ctTimed_flag.ctTimed_flag1 = ctTimed_flag1;
	  	  EEPROM_Write(ctTimed_flag_loc, ctTimed_flag1_loc, &ctTimed_flag, sz);
	  }
	else if (side == side_b)
	  {
		  ctTimed_flag.ctTimed_flag2 = ctTimed_flag2;
	  	  EEPROM_Write(ctTimed_flag_loc, ctTimed_flag2_loc, &ctTimed_flag, sz);
	  }
}

void save_ctTimedFlag_fram(pump_sid side)
{
	int8_t sz = sizeof(ctTimed_flag);

	if (side == side_a)
	  {
		  ctTimed_flag.ctTimed_flag1 = ctTimed_flag1;
	  	  FRAM_Write(ctTimed_flag1_loc_fram, &ctTimed_flag, sz);
	  }
	else if (side == side_b)
	  {
		  ctTimed_flag.ctTimed_flag2 = ctTimed_flag2;
	  	  FRAM_Write(ctTimed_flag2_loc_fram, &ctTimed_flag, sz);
	  }
}

//===================================================
/*
 *  read ctTimedFlag
 */
void retrieve_ctTimedFlag(pump_sid side)
{
  int8_t sz = sizeof(ctTimed_flag);

	if (side == side_a)
	{
		EEPROM_Read(ctTimed_flag_loc, ctTimed_flag1_loc, &ctTimed_flag, sz);

		ctTimed_flag1 = ctTimed_flag.ctTimed_flag1;
	}
	else if (side == side_b)
	{
		 EEPROM_Write(ctTimed_flag_loc, ctTimed_flag2_loc, &ctTimed_flag, sz);

		 ctTimed_flag2 = ctTimed_flag.ctTimed_flag2;
	}
}

void retrieve_ctTimedFlag_fram(pump_sid side)
{
  int8_t sz = sizeof(ctTimed_flag);

	if (side == side_a)
	{
		FRAM_Read(ctTimed_flag1_loc_fram, &ctTimed_flag, sz);

		ctTimed_flag1 = ctTimed_flag.ctTimed_flag1;
	}
	else if (side == side_b)
	{
		 FRAM_Write(ctTimed_flag2_loc_fram, &ctTimed_flag, sz);

		 ctTimed_flag2 = ctTimed_flag.ctTimed_flag2;
	}
}

//==============================================
/*
 * clear ctTimedFlag
 */
void clear_ctTimedFlag(pump_sid side)
{
	int8_t sz = sizeof(ctTimed_flag);

	if (side == side_a)
	  {
		  ctTimed_flag.ctTimed_flag1 = 0;
		  EEPROM_Write(ctTimed_flag_loc, ctTimed_flag1_loc, &ctTimed_flag, sz);
	  }
	else if (side == side_b)
	  {
		  ctTimed_flag.ctTimed_flag2 = 0;
		  EEPROM_Write(ctTimed_flag_loc, ctTimed_flag2_loc, &ctTimed_flag, sz);
	  }
}

void clear_ctTimedFlag_fram(pump_sid side)
{
	int8_t sz = sizeof(ctTimed_flag);

	if (side == side_a)
	  {
		  ctTimed_flag.ctTimed_flag1 = 0;
		  FRAM_Write(ctTimed_flag1_loc_fram, &ctTimed_flag, sz);
	  }
	else if (side == side_b)
	  {
		  ctTimed_flag.ctTimed_flag2 = 0;
		  FRAM_Write(ctTimed_flag2_loc_fram, &ctTimed_flag, sz);
	  }
}

//==============================================
/*
 * save originalPi_c
 */
void save_originalPi_c(pump_sid side)
{
	int8_t sz = sizeof(original_pi_c.original_pi_c1);

	if (side == side_a)
	  {
		  original_pi_c.original_pi_c1 = settings_stream1[0].pi_cal;
	  	  EEPROM_Write(original_pi_c_loc, original_pi_c1_loc, &original_pi_c.original_pi_c1, sz);
	  }
	else if (side == side_b)
	  {
		  original_pi_c.original_pi_c2 = settings_stream1[1].pi_cal;
	  	  EEPROM_Write(original_pi_c_loc, original_pi_c2_loc, &original_pi_c.original_pi_c1, sz);
	  }
}

//===================================================
/*
 *  read ctTimedFlag
 */
void retrieve_originalPi_c(pump_sid side)
{
  int8_t sz = sizeof(original_pi_c.original_pi_c1);

	if (side == side_a)
	{
		EEPROM_Read(original_pi_c_loc, original_pi_c1_loc, &original_pi_c.original_pi_c1, sz);

		settings_stream1[0].pi_cal = original_pi_c.original_pi_c1;
	}
	else if (side == side_b)
	{
		 EEPROM_Write(original_pi_c_loc, original_pi_c2_loc, &original_pi_c.original_pi_c2, sz);

		 settings_stream1[1].pi_cal = original_pi_c.original_pi_c2;
	}
}

//==============================================
/*
 * clear ctTimedFlag
 */
void clear_originalPi_c(pump_sid side)
{
	int8_t sz = sizeof(original_pi_c.original_pi_c1);

	if (side == side_a)
	  {
		    original_pi_c.original_pi_c1 = settings_stream1[0].pi_cal;
			EEPROM_Write(original_pi_c_loc, original_pi_c1_loc, &original_pi_c.original_pi_c1, sz);
	  }
	else if (side == side_b)
	  {
			original_pi_c.original_pi_c2 = settings_stream1[1].pi_cal;
		    EEPROM_Write(original_pi_c_loc, original_pi_c2_loc, &original_pi_c.original_pi_c2, sz);
	  }
}

//==============================================
/*
 * save calibrationPulser
 */
void save_calibrationPulser(pump_sid side)
{
	int8_t sz = sizeof(calib_pulser1);

	  if (side == side_a)
	  {
		  EEPROM_Write(calib_pulser_loc, calib_pulser1_loc, &calib_pulser1, sz);
	  }
	  else if (side == side_b)
	  {
	  	  EEPROM_Write(calib_pulser_loc, calib_pulser2_loc, &calib_pulser2, sz);
	  }
}

void save_calibrationPulser_fram(pump_sid side)
{
	int8_t sz = sizeof(calib_pulser1);

	  if (side == side_a)
	  {
		  FRAM_Write(calib_pulser1_loc_fram, &calib_pulser1, sz);
	  }
	  else if (side == side_b)
	  {
	  	  FRAM_Write(calib_pulser2_loc_fram, &calib_pulser2, sz);
	  }
}

//===================================================
/*
 *  read calibrationPulser
 */
void retrieve_calibrationPulser(pump_sid side)
{
  int8_t sz = sizeof(calib_pulser1);
  uint32_t calib_pulser;

	if (side == side_a)
	{
		EEPROM_Read(calib_pulser_loc, calib_pulser1_loc, &calib_pulser1, sz);
		calib_pulser =  (settings_stream1[0].pi_cal * vol_calibrated1);
		if (calib_pulser != calib_pulser1)
		{
//			settings[0].pi_c = (calib_pulser1 / vol_calibrated1);
//			save_settings();   //save to eeprom
//			load_settings(side_a); //load the settings into the internal variables
		}
	}
	else if (side == side_b)
	{
		EEPROM_Read(calib_pulser_loc, calib_pulser2_loc, &calib_pulser2, sz);
		calib_pulser =  (settings_stream1[1].pi_cal * vol_calibrated2);
		if (calib_pulser != calib_pulser2)
		{
//			settings[1].pi_c = (calib_pulser2 / vol_calibrated2);
//			save_settings();   //save to eeprom
//			load_settings(side_b); //load the settings into the internal variables
		}
	}
}

void retrieve_calibrationPulser_fram(pump_sid side)
{
  int8_t sz = sizeof(calib_pulser1);
  uint32_t calib_pulser;

	if (side == side_a)
	{
		FRAM_Read(calib_pulser1_loc_fram, &calib_pulser1, sz);
		calib_pulser =  (settings_stream1[0].pi_cal * vol_calibrated1);
		if (calib_pulser != calib_pulser1)
		{
//			settings[0].pi_c = (calib_pulser1 / vol_calibrated1);
//			save_settings();   //save to eeprom
//			load_settings(side_a); //load the settings into the internal variables
		}
	}
	else if (side == side_b)
	{
		FRAM_Read(calib_pulser2_loc_fram, &calib_pulser2, sz);
		calib_pulser =  (settings_stream1[1].pi_cal * vol_calibrated2);
		if (calib_pulser != calib_pulser2)
		{
//			settings[1].pi_c = (calib_pulser2 / vol_calibrated2);
//			save_settings();   //save to eeprom
//			load_settings(side_b); //load the settings into the internal variables
		}
	}
}

//==============================================
/*
 * clear calibrationPulser
 */
void clear_calibrationPulser(pump_sid side)
{
	int8_t sz = sizeof(calib_pulser1);
	calib_pulser1 = 0;
	calib_pulser2 = 0;

	  if (side == side_a)
	  {
		  EEPROM_Write(calib_pulser_loc, calib_pulser1_loc, &calib_pulser1, sz);
	  }
	  else if (side == side_b)
	  {
		  EEPROM_Write(calib_pulser_loc, calib_pulser2_loc, &calib_pulser2, sz);
	  }
}

void clear_calibrationPulser_fram(pump_sid side)
{
	int8_t sz = sizeof(calib_pulser1);
	calib_pulser1 = 0;
	calib_pulser2 = 0;

	  if (side == side_a)
	  {
		  FRAM_Write(calib_pulser1_loc_fram, &calib_pulser1, sz);
	  }
	  else if (side == side_b)
	  {
		  FRAM_Write(calib_pulser2_loc_fram, &calib_pulser2, sz);
	  }
}
//===================================================


//==============================================
/*
 * save sessionId
 */
void save_sessionId(pump_sid side)
{
	int sz = sizeof(sessionId[0]);

	if (side == side_a)
	{
		strncpy(sessionId[0].session_id, attendant1.session_id, sizeof(sessionId[0].session_id) );
	  	EEPROM_Write(sessionId_loc, sessionId1_loc, &sessionId[0], sz);
	}
	else if (side == side_b)
	{
		strncpy(sessionId[1].session_id, attendant2.session_id, sizeof(sessionId[1].session_id) );
	  	EEPROM_Write(sessionId_loc, sessionId2_loc, &sessionId[1], sz);
	}
}

//===================================================
/*
 *  read sessionId
 */
void retrieve_sessionId(pump_sid side)
{
  int sz = sizeof(sessionId[0]);
	if (side == side_a)
	{
		EEPROM_Read(sessionId_loc, sessionId1_loc, &sessionId[0], sz);
		strncpy(attendant1.session_id, sessionId[0].session_id, sizeof(attendant1.session_id) );
	}
	else if (side == side_b)
	{
	  	 EEPROM_Read(sessionId_loc, sessionId2_loc, &sessionId[1], sz);
		 strncpy(attendant2.session_id, sessionId[1].session_id, sizeof(attendant2.session_id) );
	}
}

//==============================================
/*
 * clear sessionId
 */
void clear_sessionId(pump_sid side)
{
	int sz = sizeof(sessionId[0]);

	if (side == side_a)
	  {
		  memset(sessionId[0].session_id, '\0', sizeof(sessionId[0].session_id) );
		  EEPROM_Write(sessionId_loc, sessionId1_loc, &sessionId[0], sz);
	  }
	else if (side == side_b)
	  {
		  memset(sessionId[1].session_id, '\0', sizeof(sessionId[1].session_id) );
		  EEPROM_Write(sessionId_loc, sessionId2_loc, &sessionId[1], sz);
	  }
}




//==============================================
/*
 * save Start-Shift amountTotaliser
 */
void save_amountTotaliser_startShift(pump_sid side)
{
//	int sz = sizeof(startShiftTotaliser_vol_storeA);
//
//	if (side == side_a)
//	  {
//		  startShiftTotaliser_amt_storeA.totaliserVol_cal = startShiftTotaliser_amt1c;
//		  startShiftTotaliser_amt_storeA.totaliserVol_real = startShiftTotaliser_amt1;
//	  	  EEPROM_Write(startShiftTotAmount_loc, startShiftTotAmount1_loc, &startShiftTotaliser_amt_storeA, sz);
//	  }
//	else if (side == side_b)
//	  {
//		  startShiftTotaliser_amt_storeB.totaliserVol_cal = startShiftTotaliser_amt2c;
//		  startShiftTotaliser_amt_storeB.totaliserVol_real = startShiftTotaliser_amt2;
//	  	  EEPROM_Write(startShiftTotAmount_loc, startShiftTotAmount2_loc, &startShiftTotaliser_amt_storeB, sz);
//	  }
}

//===================================================
/*
 *  read Start-Shift amountTotaliser
 */
void retrieve_amountTotaliser_startShift(pump_sid side)
{
//	int sz = sizeof(startShiftTotaliser_amt_storeA);
//	if (side == side_a)
//	{
//		EEPROM_Read(startShiftTotAmount_loc, startShiftTotAmount1_loc, &startShiftTotaliser_amt_storeA, sz);
//		startShiftTotaliser_amt1c =  startShiftTotaliser_amt_storeA.totaliserVol_cal;
//		startShiftTotaliser_amt1 = startShiftTotaliser_amt_storeA.totaliserVol_real;
//
//	  	if(isnan(startShiftTotaliser_amt1c)) startShiftTotaliser_amt1c = 0.0;
//	  	if(isnan(startShiftTotaliser_amt1)) startShiftTotaliser_amt1 = 0.0;
//
//	}
//	else if (side == side_b)
//	{
//		 EEPROM_Read(startShiftTotAmount_loc, startShiftTotAmount2_loc, &startShiftTotaliser_amt_storeB, sz);
//		 startShiftTotaliser_amt2c = startShiftTotaliser_amt_storeB.totaliserVol_cal;
//		 startShiftTotaliser_amt2  = startShiftTotaliser_amt_storeB.totaliserVol_real;
//
//	  	if(isnan(startShiftTotaliser_amt2c)) startShiftTotaliser_amt2c = 0.0;
//	  	if(isnan(startShiftTotaliser_amt2)) startShiftTotaliser_amt2 = 0.0;
//
//	}
}

//==============================================
/*
 * clear Start-Shift amountTotaliser
 */
void clear_amountTotaliser_startShift(pump_sid side)
{
//	int sz = sizeof(startShiftTotaliser_vol_storeA);
//
//	if (side == side_a)
//	  {
//		  startShiftTotaliser_amt_storeA.totaliserVol_cal = 0.00;
//		  startShiftTotaliser_amt_storeA.totaliserVol_real = 0.00;
//	  	  EEPROM_Write(startShiftTotAmount_loc, startShiftTotAmount1_loc, &startShiftTotaliser_amt_storeA, sz);
//	  }
//	else if (side == side_b)
//	  {
//		  startShiftTotaliser_amt_storeB.totaliserVol_cal = 0.00;
//		  startShiftTotaliser_amt_storeB.totaliserVol_real = 0.00;
//	  	  EEPROM_Write(startShiftTotAmount_loc, startShiftTotAmount2_loc, &startShiftTotaliser_amt_storeB, sz);
//	  }
}



//===================================================
/*
 *  save Calibration Flag
 */
void save_calibrationFlag(pump_sid side)
{
	if (side == side_a)
	{
		EEPROM_Write_NUM (calibrationFlag1_loc, 0, calibration_flag1);
	}
	else if (side == side_b)
	{
		EEPROM_Write_NUM (calibrationFlag2_loc, 0, calibration_flag2);
	}

}

void save_calibrationFlag_fram(pump_sid side)
{
	if (side == side_a)
	{
		FRAM_WriteByte (calibrationFlag1_loc_fram, calibration_flag1);
	}
	else if (side == side_b)
	{
		FRAM_WriteByte (calibrationFlag2_loc_fram, calibration_flag2);
	}

}


void save_online_calibFlag_fram(pump_sid side)
{
	if (side == side_a)
	{
		FRAM_WriteByte (online_calibFlag1_loc_fram, online_calibFlag1);
	}
	else if (side == side_b)
	{
		FRAM_WriteByte (online_calibFlag2_loc_fram, online_calibFlag2);
	}

}

//===================================================
/*
 *  read Calibration Flag
 */
void retrieve_calibrationFlag(pump_sid side)
{
	if (side == side_a)
	{
		calibration_flag1 = EEPROM_Read_NUM (calibrationFlag1_loc, 0);
	}
	else if (side == side_b)
	{
		calibration_flag2 = EEPROM_Read_NUM (calibrationFlag2_loc, 0);
	}
}

void retrieve_calibrationFlag_fram(pump_sid side)
{
	if (side == side_a)
	{
		calibration_flag1 = FRAM_ReadByte (calibrationFlag1_loc_fram);
	}
	else if (side == side_b)
	{
		calibration_flag2 = FRAM_ReadByte (calibrationFlag2_loc_fram);
	}
}

void retrieve_online_calibFlag_fram(pump_sid side)
{
	if (side == side_a)
	{
		online_calibFlag1 = FRAM_ReadByte (online_calibFlag1_loc_fram);
	}
	else if (side == side_b)
	{
		online_calibFlag2 = FRAM_ReadByte (online_calibFlag2_loc_fram);
	}
}

//==============================================
/*
 * clear Calibration Flag
 */
void clear_calibrationFlag(pump_sid side)
{
	if (side == side_a)
	{
		EEPROM_Write_NUM (calibrationFlag1_loc, 0, 0);
	}
	else if (side == side_b)
	{
		EEPROM_Write_NUM (calibrationFlag2_loc, 0, 0);
	}
}

void clear_calibrationFlag_fram(pump_sid side)
{
	if (side == side_a)
	{
		FRAM_WriteByte (calibrationFlag1_loc_fram, 0);
	}
	else if (side == side_b)
	{
		FRAM_WriteByte (calibrationFlag2_loc_fram, 0);
	}
}

void clear_online_calibFlag_fram(pump_sid side)
{
	if (side == side_a)
	{
		FRAM_WriteByte (online_calibFlag1_loc_fram, 0);
	}
	else if (side == side_b)
	{
		FRAM_WriteByte (online_calibFlag2_loc_fram, 0);
	}

}

//===================================================


//===================================================
/*
 *  save Calibration Data
 */
void save_calibrationData(pump_sid side)
{
	uint8_t sz = sizeof(calibrationData[0]);

	if (side == side_a)
	{
		calibrationData[0].pulser_benchMark = pulser_benchMark1;
		calibrationData[0].pulser_value = calib_pulser1;
		calibrationData[0].power_interruption = pwr1;
		EEPROM_Write(calibrationDetails_loc, calibrationDetails1_loc, &calibrationData[0], sz);
	}
	else if (side == side_b)
	{
		calibrationData[1].pulser_benchMark = pulser_benchMark2;
		calibrationData[1].pulser_value = calib_pulser2;
		calibrationData[1].power_interruption = pwr2;
		EEPROM_Write(calibrationDetails_loc, calibrationDetails2_loc, &calibrationData[1], sz);
	}

}

void save_calibrationData_fram(pump_sid side)
{
	uint8_t sz = sizeof(calibrationData[0]);

	if (side == side_a)
	{
		calibrationData[0].pulser_benchMark = pulser_benchMark1;
		calibrationData[0].pulser_value = calib_pulser1;
		calibrationData[0].power_interruption = pwr1;
		FRAM_Write(calibrationDetails1_loc_fram, &calibrationData[0], sz);
	}
	else if (side == side_b)
	{
		calibrationData[1].pulser_benchMark = pulser_benchMark2;
		calibrationData[1].pulser_value = calib_pulser2;
		calibrationData[1].power_interruption = pwr2;
		FRAM_Write(calibrationDetails2_loc_fram, &calibrationData[1], sz);
	}

}

//===================================================
/*
 *  read Calibration Data
 */
void retrieve_calibrationData(pump_sid side)
{
	uint8_t sz = sizeof(calibrationData[0]);
	if (side == side_a)
	{
		EEPROM_Read(calibrationDetails_loc, calibrationDetails1_loc, &calibrationData[0], sz);
		pulser_benchMark1 = calibrationData[0].pulser_benchMark;
		calib_pulser1 = calibrationData[0].pulser_value;
		pwr1 = calibrationData[0].power_interruption;
	}
	else if (side == side_b)
	{
		EEPROM_Read(calibrationDetails_loc, calibrationDetails2_loc, &calibrationData[1], sz);
		pulser_benchMark2 = calibrationData[1].pulser_benchMark;
		calib_pulser2 = calibrationData[1].pulser_value;
		pwr2 = calibrationData[1].power_interruption;
	}
}


void retrieve_calibrationData_fram(pump_sid side)
{
	uint8_t sz = sizeof(calibrationData[0]);
	if (side == side_a)
	{
		FRAM_Read(calibrationDetails1_loc_fram, &calibrationData[0], sz);
		pulser_benchMark1 = calibrationData[0].pulser_benchMark;
		calib_pulser1 = calibrationData[0].pulser_value;
		pwr1 = calibrationData[0].power_interruption;
	}
	else if (side == side_b)
	{
		FRAM_Read(calibrationDetails2_loc_fram, &calibrationData[1], sz);
		pulser_benchMark2 = calibrationData[1].pulser_benchMark;
		calib_pulser2 = calibrationData[1].pulser_value;
		pwr2 = calibrationData[1].power_interruption;
	}
}
//==============================================
/*
 * clear Calibration Data
 */
void clear_calibrationData(pump_sid side)
{
	uint8_t sz = sizeof(calibrationData[0]);

	if (side == side_a)
	{
		calibrationData[0].pulser_benchMark = 0;
		calibrationData[0].pulser_value = 0;
		calibrationData[0].power_interruption = 0;
		EEPROM_Write(calibrationDetails_loc, calibrationDetails1_loc, &calibrationData[0], sz);
	}
	else if (side == side_b)
	{
		calibrationData[1].pulser_benchMark = 0;
		calibrationData[1].pulser_value = 0;
		calibrationData[1].power_interruption = 0;
		EEPROM_Write(calibrationDetails_loc, calibrationDetails2_loc, &calibrationData[1], sz);
	}
}

void clear_calibrationData_fram(pump_sid side)
{
	uint8_t sz = sizeof(calibrationData[0]);

	if (side == side_a)
	{
		calibrationData[0].pulser_benchMark = 0;
		calibrationData[0].pulser_value = 0;
		calibrationData[0].power_interruption = 0;
		FRAM_Write(calibrationDetails1_loc_fram, &calibrationData[0], sz);
	}
	else if (side == side_b)
	{
		calibrationData[1].pulser_benchMark = 0;
		calibrationData[1].pulser_value = 0;
		calibrationData[1].power_interruption = 0;
		FRAM_Write(calibrationDetails2_loc_fram, &calibrationData[1], sz);
	}
}

//===================================================


//===================================================
/*
 *  save Configuration Flag
 */
void save_configFlag(pump_sid side)
{
	if (side == side_a)
	{
		EEPROM_Write_NUM (configFlag1_loc, 0, configMode1);
	}
	else if (side == side_b)
	{
		EEPROM_Write_NUM (configFlag2_loc, 0, configMode2);
	}

}

void save_configFlag_fram(pump_sid side)
{
	if (side == side_a)
	{
		FRAM_WriteByte (configFlag1_loc_fram, configMode1);
	}
	else if (side == side_b)
	{
		FRAM_WriteByte (configFlag2_loc_fram, configMode2);
	}

}

//===================================================
/*
 *  read Configuration Flag
 */
void retrieve_configFlag(pump_sid side)
{
	if (side == side_a)
	{
		configMode1 = EEPROM_Read_NUM (configFlag1_loc, 0);
	}
	else if (side == side_b)
	{
		configMode2 = EEPROM_Read_NUM (configFlag2_loc, 0);
	}
}

void retrieve_configFlag_fram(pump_sid side)
{
	if (side == side_a)
	{
		configMode1 = FRAM_ReadByte (configFlag1_loc_fram);
	}
	else if (side == side_b)
	{
		configMode2 = FRAM_ReadByte (configFlag2_loc_fram);
	}
}

//==============================================
/*
 * clear Configuration Flag
 */
void clear_configFlag(pump_sid side)
{
	if (side == side_a)
	{
		EEPROM_Write_NUM (configFlag1_loc, 0, 0);
	}
	else if (side == side_b)
	{
		EEPROM_Write_NUM (configFlag2_loc, 0, 0);
	}
}

void clear_configFlag_fram(pump_sid side)
{
	if (side == side_a)
	{
		FRAM_WriteByte (configFlag1_loc_fram, CONFIGUNMODIFIED);
	}
	else if (side == side_b)
	{
		FRAM_WriteByte (configFlag2_loc_fram, CONFIGUNMODIFIED);
	}
}
//==============================================

//==============================================
/*
 * save amountSend
 */
void save_amountSend(pump_sid side)
{
	int sz = sizeof(amountSend[0]);

	if (side == side_a)
	  {
		  amountSend[0].amountOld = priceOld1;
	  	  EEPROM_Write(amountSend_loc, amountSend1_loc, &amountSend[0], sz);
	  }
	else if (side == side_b)
	  {
		  amountSend[1].amountOld = priceOld2;
	  	  EEPROM_Write(amountSend_loc, amountSend2_loc, &amountSend[1], sz);
	  }
}

//===================================================
/*
 *  read amountSend
 */
void retrieve_amountSend(pump_sid side)
{
  int sz = sizeof(amountSend[0]);
	if (side == side_a)
	{
		EEPROM_Read(amountSend_loc, amountSend1_loc, &amountSend[0], sz);
		priceOld1 = amountSend[0].amountOld;

	  	if(isnan(priceOld1)) priceOld1 = 0.0;

	}
	else if (side == side_b)
	{
		 EEPROM_Read(totAmount_loc, totAmount2_loc, &amountSend[1], sz);
		 priceOld2 = amountSend[1].amountOld;

	  	if(isnan(priceOld2)) priceOld2 = 0.0;
	}
}

//==============================================
/*
 * clear amountSend
 */
void clear_amountSend(pump_sid side)
{
	int sz = sizeof(amountSend[0]);

	if (side == side_a)
	  {
		  amountSend[0].amountOld = 0.00;
	  	  EEPROM_Write(amountSend_loc, amountSend1_loc, &amountSend[0], sz);
	  }
	else if (side == side_b)
	  {
		  amountSend[1].amountOld = 0.00;
	  	  EEPROM_Write(amountSend_loc, amountSend2_loc, &amountSend[1], sz);
	  }
}




//==============================================
/*
 * save OTP
 */
void save_otp(pump_sid side)
{
	uint8_t sz = sizeof(otp_code1);

	if (side == side_a)
	{
	  	EEPROM_Write(otp_loc, otp1_loc, &otp_code1, sz);
	}
	else if (side == side_b)
	{
	  	EEPROM_Write(otp_loc, otp2_loc, &otp_code2, sz);
	}
}


void save_config_otpSeed_time_fram(pump_sid side)
{
	uint8_t sz = sizeof(configChange[0]);

	if (side == side_a)
	{
		configChange[0].otp_seed = otp_seed1;
		configChange[0].time_stamp = RtcToInt(2019);
	  	FRAM_Write(config_otpSeed_time1_fram, &configChange[0], sz);
	}
	else if (side == side_b)
	{
		configChange[1].otp_seed = otp_seed2;
		configChange[1].time_stamp = RtcToInt(2019);
		FRAM_Write(config_otpSeed_time2_fram, &configChange[1], sz);
	}
}


void save_otpSeed_session_fram(pump_sid side)
{
	uint8_t sz = sizeof(otpSeed_session[0]);

	if (side == side_a)
	{
		otpSeed_session[0].otp_seed = otp_seed1;
		otpSeed_session[0].time_stamp = RtcToInt(2019);
		otpSeed_session[0].otpSeed_flag = OTPSESSION_ON;
	  	FRAM_Write(otpSeed_session1_loc_fram, &otpSeed_session[0], sz);
	}
	else if (side == side_b)
	{
		otpSeed_session[1].otp_seed = otp_seed2;
		otpSeed_session[1].time_stamp = RtcToInt(2019);
		otpSeed_session[1].otpSeed_flag = OTPSESSION_ON;
		FRAM_Write(otpSeed_session1_loc_fram, &otpSeed_session[1], sz);
	}
}


//===================================================
/*
 *  read OTP
 */
void retrieve_otp(pump_sid side)
{
	uint8_t sz = sizeof(otp);
	if (side == side_a)
	{
		EEPROM_Read(otp_loc, otp1_loc, &otp_code1, sz);
	}
	else if (side == side_b)
	{
	  	 EEPROM_Read(otp_loc, otp2_loc, &otp_code2, sz);
	}
}

void retrieve_config_otpSeed_time_fram(pump_sid side)
{
	uint8_t sz = sizeof(configChange[0]);

	if (side == side_a)
	{
		FRAM_Read(config_otpSeed_time1_fram, &configChange[0], sz);
	}
	else if (side == side_b)
	{
	  	 FRAM_Read(config_otpSeed_time2_fram, &configChange[1], sz);
	}
}


void retrieve_otpSeed_session_fram(pump_sid side)
{
	uint8_t sz = sizeof(otpSeed_session[0]);

	if (side == side_a)
	{
		FRAM_Read(otpSeed_session1_loc_fram, &otpSeed_session[0], sz);
	}
	else if (side == side_b)
	{
		FRAM_Read(otpSeed_session1_loc_fram, &otpSeed_session[1], sz);
	}
}

//==============================================
/*
 * clear OTP
 */
void clear_otp(pump_sid side)
{
	uint8_t sz = sizeof(otp_code1);

	if (side == side_a)
	  {
		  memset(otp_code1, '\0', sizeof(otp_code1) );
		  EEPROM_Write(otp_loc, otp1_loc, &otp_code1, sz);
	  }
	else if (side == side_b)
	  {
		  memset(otp_code2, '\0', sizeof(otp_code2) );
		  EEPROM_Write(otp_loc, otp2_loc, &otp_code2, sz);
	  }
}

void clear_config_otpSeed_time_fram(pump_sid side)
{
	uint8_t sz = sizeof(configChange[0]);

	if (side == side_a)
	  {
		  configChange[0].otp_seed = 0;
		  configChange[0].time_stamp = 0;
		  FRAM_Write(config_otpSeed_time1_fram, &configChange[0], sz);
	  }
	else if (side == side_b)
	  {
		  configChange[1].otp_seed = 0;
		  configChange[1].time_stamp = 0;
		  FRAM_Write(config_otpSeed_time2_fram, &configChange[1], sz);
	  }
}

void clear_otpSeed_session_fram(pump_sid side)
{
	uint8_t sz = sizeof(otpSeed_session[0]);

	if (side == side_a)
	{
		otpSeed_session[0].otp_seed = 0;
		otpSeed_session[0].time_stamp = 0;
		otpSeed_session[0].otpSeed_flag = OTPSESSION_OFF;
	  	FRAM_Write(otpSeed_session1_loc_fram, &otpSeed_session[0], sz);
	}
	else if (side == side_b)
	{
		otpSeed_session[1].otp_seed = 0;
		otpSeed_session[1].time_stamp = 0;
		otpSeed_session[1].otpSeed_flag = OTPSESSION_OFF;
		FRAM_Write(otpSeed_session1_loc_fram, &otpSeed_session[1], sz);
	}
}

//==============================================

//==============================================
/*
 * save Config Change Track No.
 */
void save_configChange_trackNum_fram(pump_sid side)
{
	if (side == side_a)
	{
	  	FRAM_WriteByte(track_num1_loc_fram, track_num1);
	}
	else if (side == side_b)
	{
	  	FRAM_WriteByte(track_num2_loc_fram, track_num2);
	}
}

//==============================================
/*
 * retrieve Config Change Track No.
 */
void retrieve_configChange_trackNum_fram(pump_sid side)
{
	if (side == side_a)
	{
		track_num1 = FRAM_ReadByte (track_num1_loc_fram);
	}
	else if (side == side_b)
	{
		track_num2 = FRAM_ReadByte (track_num2_loc_fram);
	}
}

//==============================================
/*
 * clear Config Change Track No.
 */
void clear_configChange_trackNum_fram(pump_sid side)
{
	if (side == side_a)
	{
		FRAM_WriteByte(track_num1_loc_fram, 0);
	}
	else if (side == side_b)
	{
		FRAM_WriteByte(track_num2_loc_fram, 0);
	}
}
//==============================================
/*
 *  copy settings to the structure to be used for prog.
 */
void copy_settings(copy_dir dir)
{
	int8_t sz;
	uint8_t *cpy1, *cpy2, *cpy3;
	uint8_t *sett1, *sett2, *sett3;
	uint8_t *sett01, *sett02, *sett03;

	 for(uint8_t sd = 0; sd < 2; sd++ )
	 {
		cpy1 = &copy_stream1[sd];
		sett1 = &settings_stream1[sd];
		sett01 = &settings_original_stream1[sd];

		cpy2 = &copy_stream2[sd];
		sett2 = &settings_stream2[sd];
		sett02 = &settings_original_stream2[sd];

		cpy3 = &copy_stream3[sd];
		sett3 = &settings_stream3[sd];
		sett03 = &settings_original_stream3[sd];

		sz = sizeof(copy_stream1[0]);

		   for (uint8_t i = 0; i < sz; i++)
		   {
			   if (dir == move_to_copy)
			   {
				   *cpy1++ = *sett1++;
			   }
			   else if (dir == move_to_settings)
			   {
				   *sett1++ = *cpy1++;
			   }
			   else if (dir == move_to_settings0)
			   {
				   *sett01++ = *sett1++;
			   }
		   }

		   sz = sizeof(copy_stream2[0]);
		   for (uint8_t i = 0; i < sz; i++)
		   {
			   if (dir == move_to_copy)
			   {
				   *cpy2++ = *sett2++;
			   }
			   else if (dir == move_to_settings)
			   {
				   *sett2++ = *cpy2++;
			   }
			   else if (dir == move_to_settings0)
			   {
				   *sett02++ = *sett2++;
			   }
		   }

		   sz = sizeof(copy_stream3[0]);
		   for (uint8_t i = 0; i < sz; i++)
		   {
			   if (dir == move_to_copy)
			   {
				   *cpy3++ = *sett3++;
			   }
			   else if (dir == move_to_settings)
			   {
				   *sett3++ = *cpy3++;
			   }
			   else if (dir == move_to_settings0)
			   {
				   *sett03++ = *sett3++;
			   }
		   }
	 }
}
//====================================================
int get_auth()
{
	auth_flag = 0;

	if(opmode == MANUAL_MODE)
	{
	  //if (t > 500)
	  //{
		 auth_flag = 1;
		 //t = 0;
	  //}
	}
	else
	{
      //wait for "GO" to send authorise signal.
		return 0;
	}
	return auth_flag;
}
//===============================================
//===============================================
int get_auth2()
{
	auth_flag2 = 0;
	if(opmode2 == MANUAL_MODE)
		{
	       auth_flag2 = 1;
		}
	else
	{
      //wait for "GO" to send authorise signal.
		return 0;
	}
	return auth_flag;
}
///================ keypad decoder ==============
char lafeng_keypad[17] =
{
	'-',	//0
	'A',	//1   //Back
	'1',	//2
	'5',	//3
	'3',	//4
	'8',	//5
	'2',	//6
	'4',	//7
	'7',	//8
	'6',	//9
	'9',	//10
	'B',	//11   //Up-Key
	'C',	//12   //Down-Key
	'D',	//13	//Select-Key
	'.',	//14
	'F',    //15    //clear
	'0'     //16
};

char lafeng_keypad_18K[18] =
{
	'-',	//0
	'B',	//1   //Back
	'1',	//2
	'5',	//3
	'3',	//4
	'8',	//5
	'2',	//6
	'4',	//7
	'7',	//8
	'6',	//9
	'9',	//10
	'C',	//11   //Up-Key
	'A',	//12   //Down-Key
	'D',	//13	//Select-Key
	'.',	//14
	'F',    //15    //clear
	'0',    //16
	'J'     //17   //Totalizer
};

char lafeng_keypad_18K_V2[18] =
{
	'-',	//0
	'C',	//1   //Down-Key
	'1',	//2
	'5',	//3
	'3',	//4
	'8',	//5
	'2',	//6
	'4',	//7
	'7',	//8
	'6',	//9
	'9',	//10
	'B',	//11   //Up-Key
	'A',	//12   //Back-Key
	'D',	//13	//Select-Key
	'.',	//14
	'F',    //15    //clear
	'0',    //16
	'J'     //17   //Totalizer
};


char bluesky_keypad[22] =     //bluesky 12pin
{
	'-',	//0
	'B',	//1  // */+           Up-key
	'1',	//2
	'5',	//3
	'3',	//4
	'8',	//5
	'2',	//6
	'4',	//7
	'7',	//8
	'6',	//9
	'9',	//10
	'C',	//11  //P/L / -        Down-key
	'D',	//12  //start / enter
	'A',	//13  //stop / exit
	'.',	//14
	'F',    //15   clear
	'E',    //16   f3 key  up
	'G',    //17   f1 key   mapped for back
	'H',    //18   f2 key   down
	'I',    //19   f4 key  PROG key.
	'0',    //20
	'J',    //21   //print totaliser view.
};

//char lafeng_keypad[17] =
//{
//	'-',	//0
//	'A',	//1
//	'1',	//2
//	'5',	//3
//	'3',	//4
//	'8',	//5
//	'2',	//6
//	'4',	//7
//	'7',	//8
//	'6',	//9
//	'9',	//10
//	'B',	//11
//	'C',	//12
//	'D',	//13
//	'.',	//14
//	'F',    //15
//	'0'     //16
//};

//char bluesky_keypad[22] =     //bluesky 12pin
//{
//	'-',	//0
//	'*',	//1
//	'1',	//2
//	'5',	//3
//	'3',	//4
//	'8',	//5
//	'2',	//6
//	'4',	//7
//	'7',	//8
//	'6',	//9
//	'9',	//10
//	'B',	//11
//	'D',	//12  //start / enter
//	'C',	//13  //stop
//	'.',	//14
//	'F',    //15   clear
//	'B',    //16   f3 key  up
//	'A',    //17   f1 key   mapped for back
//	'C',    //18   f2 key   down
//	'-',    //19   f4 key  PROG key.
//	'0',    //20
//	'-',    //21   //print totaliser view.
//};

float sellPrice_max_dp(int8_t amount_dp)
{
	  switch(amount_dp)
	  {
		  case 0: return 99999999;
		  case 1: return 9999999.9;
		  case 2: return 999999.99;
		  case 3: return 99999.999;

	  }
}

float sellPrice_max_dp2(int8_t amount_dp)
{
	  switch(amount_dp)
	  {
		  case 0: return 99999999;
		  case 1: return 9999999.9;
		  case 2: return 999999.99;
		  case 3: return 99999.999;

	  }
}


void dp_init(pump_sid sdd)
{
	if(settings_stream1[sdd-1].dp_amount == 0)
		settings_stream1[sdd-1].dp_amount = 2;
    if(settings_stream1[sdd-1].dp_vol == 0)
    	settings_stream1[sdd-1].dp_vol = 2;
    if(settings_stream1[sdd-1].dp_unitprice == 0)
    	settings_stream1[sdd-1].dp_unitprice = 2;
}

void clr_pulser1()
{
   __HAL_TIM_SET_COUNTER(&htim5, settings_stream2[0].pulser_offset);
}

void clr_pulser2()
{
   __HAL_TIM_SET_COUNTER(&htim2, settings_stream2[1].pulser_offset);
}
