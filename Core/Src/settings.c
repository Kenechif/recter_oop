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

const char device_id [] = "860537065691297";   //"860537064685993";        //"860537064685357";           //"860537064685993";     //"860537064685340";

const char firmware_date [] = "Aug 28 2023";
const char firmware_time [] = "17:07:30";
const char chip_type [] = "STM32F4";

const uint16_t firmware_version = 25100;

uint16_t  pump_SN = 77;
char session_id[9] = {0};

//##################################################



 const int max_events_per_state = 10;

 // The shared secret is FdelOnwuka
 uint8_t hmacKey[] = {0x46, 0x64, 0x65, 0x6C, 0x4F, 0x6E, 0x77, 0x75, 0x6B, 0x61};

uint8_t connected;

uint8_t serverTimeFlag = 0;

 uint16_t fast_flow_threshold  = 0;  //=  pulser_index_c ;   //140;

extern int8_t change_p,
			  change_v;

extern float auth_v,
			 auth_p;

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
		  totaliser_amt2c = 0.00;

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

 int8_t dp_price1 = 2,
		dp_price2 = 2,
		dp_amount1 = 2,
		dp_amount2 = 2,
		dp_unitprice1 = 2,
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


 int8_t opmode  = MANUAL;
 int8_t opmode2 = MANUAL;

 nozzle_overide overide_ = nooveride;
 nozzle_overide overide_2 = nooveride;

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
 const int save_settings1_loc = 500;
 const int save_settings2_loc = 581;   //581 --> 660
 const int sessionId_loc = 661,
		   sessionId1_loc = 0,
		   sessionId2_loc = sessionId1_loc + 9;  ////670 --> 678

 const int startShiftTotVol_loc  =  679,
 	 	   startShiftTotVol1_loc =  0,
 	 	   startShiftTotVol2_loc =  startShiftTotVol1_loc + (2+(2*4));   // 689 -> 698

 const int startShiftTotAmount_loc  =  699,
 	 	   startShiftTotAmount1_loc =  0,
 	 	   startShiftTotAmount2_loc =  startShiftTotAmount1_loc + (2+(2*4));  // 709 -> 718

 const int16_t save_pumpType_loc = 400;
 const int16_t save_productType_loc = save_pumpType_loc + 1;
 const int16_t save_nozzleId_loc = save_pumpType_loc + 2;
 const uint16_t totalizerDay_loc = 404;

// const int8_t lastSale_loc  =  40;
// const int lastSale1_loc =  0;
// const int lastSale2_loc =  lastSale1_loc + (2+(2*4));  //4bytes*2=8bytes+2 = 10bytes ahead.  // 50 -> 60

 const int8_t lastSale_loc = 282;
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

 const int8_t ctTimed_settings_loc  =  159;
 const int8_t ctTimed_settings1_loc =  0;
 const int8_t ctTimed_settings2_loc =  ctTimed_settings1_loc + (1+(3*4));   // 172 -> 185

 const int8_t ctTimed_flag_loc = 186;
 const int8_t ctTimed_flag1_loc = 0;
 const int8_t ctTimed_flag2_loc = ctTimed_flag1_loc + 2;   // 188 -> 190

 const int8_t original_pi_c_loc = 191;
 const int8_t original_pi_c1_loc = 0;
 const int8_t original_pi_c2_loc = (original_pi_c1_loc + 4);   // 195 -> 199

// const int8_t original_pi_c_loc = 190;
// const int8_t original_pi_c1_loc = 0;
// const int8_t original_pi_c2_loc = original_pi_c1_loc + (1 + (2 *4));   // 199 -> 208

 //--------------------------------------------------------------
 //  eeprom locations of the storage of  flash memory parameters
 const int flash_info_sto  =  11;    //page
 const int flash_stoA =  0;    		 //capacity for 2 * 32bit integer
 const int flash_stoB =  10;    	 //+ 2 * 4bytes  +2

 //==============================================================
 const int attendant_sto = 12;     //page 12
 const int att1_loc      = 0;      //offset  0.
 const int att2_loc      = 20;     //offset  20.

 //==============================================================
 // define the settings structure of the settings.
 //==============================================================

 pump_settings settings[2], copy[2];

log_new log_a_new,
		log_b_new,
		synchedLog_a_new,
		synchedLog_b_new;

log_new1 log_a_new1 , log_b_new1;

flash_store_info flash_infoA,flash_infoB;

totaliser_store totaliser_vol_storeA,
				totaliser_vol_storeB,
				totaliser_amt_storeA,
				totaliser_amt_storeB,
				startShiftTotaliser_vol_storeA,
				startShiftTotaliser_vol_storeB,
				startShiftTotaliser_amt_storeA,
				startShiftTotaliser_amt_storeB;

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
 char* menu2[16] = {"  nnode ","Address "," Nozzle ","Prog.type","  Price ","no flo. t","Hi. Litre","Ch  Pass","Send cfg","Get. cfg ","Clr log","Clr tot","Calibrat.","Display ", "  Cloc"};
 char* menu3[3]  = {"tmm cfg","flo rate"};

 char* menu_level1[4] = {"Shft. Tot ", "  Price ", "  Cloc"};
 char* menu_level2[18] = {"  nnode ", "Address ", " Nozzle ", "Disp.Styl", "  Price ", "no flo. t", "Hi. Litre", "Ch  Pass", "Calibrat.", "DP Count", "  Cloc", "Sides No", "Start CL", "Calib.Can", "Shift.Typ", "Shift No", "  Tone  "};
 char* menu_level3[20] = {"  nnode ", "Address ", " Nozzle ", "Disp.Styl", "  Price ", "no flo. t", "Hi. Litre", "Ch  Pass", "Calibrat.", "DP Count", "  Cloc", "Sides No", "Start CL", "Calib.Can", "Shift.Typ", "Shift No", "  Tone  ", "Calib.Typ", "Conn.Card"};

 char* pass[4] = {" pass 1 ", " pass 2 ", " pass 3 "};
 char* ch_pass[4] = {"pass.1", "pass.2", "pass.3"};

 char* login_type[4] = {"[ None ]", "[ Code ]", "[ Card ]"};

//-------------------------------------
void retrieve_settings()
{
   extern const int save_settings1_loc;
   extern const int save_settings2_loc;
   int sz;
//   sz = sizeof(settings[0].display_mode);  // PL/LP   ==> default : PL  // Level 2
//   sz = sizeof(settings[0]);
   sz = sizeof(copy[0]);

   if( EEPROM_Read(save_settings1_loc, 0, &settings[0], sz) );
   else
	   storage_fail = 1;
   if( EEPROM_Read(save_settings2_loc, 0, &settings[1], sz) );
   else
	   storage_fail = 1;


   dp_init(side_a);
   dp_init(side_b);

   //read totaliser settings..
}

/*
 *
 */
void load_settings(pump_sid side)
{
  int sdd = 0;
//   if(side == side_b)
//   {
//	   sdd = 1;
//   }

   if (side == side_a)
   {
	     sdd = 0;
//			litre_price1 = settings[0].price_;
			//totaliser_vol2 = 12345678.00;

	    sellmode = settings[sdd].def_t;
	    //totaliser_vol1 = 57638694.00;

	    overide_ = settings[sdd].noz;

	    pulser_index = settings[sdd].pi_;
	    pulser_index_c = settings[sdd].pi_c;

	    fast_flow_threshold = pulser_index_c;

	    opmode  = settings[sdd].mode;

	    litre_price  = settings[sdd].price_;
	    litre_price1 = settings[sdd].price_;

	    timeout_picknozzle = 30;
	    timeout_dispense   = 60;
	    timeout_noflow = settings[sdd].noFlow_timeOut;

	    pump_type = settings[sdd].pump_type_;

	    strncpy(password_level1, settings[sdd].passwd1, sizeof(password_level1) );
	    strncpy(password_level2, settings[sdd].passwd2, sizeof(password_level2) );
	    strncpy(password_level3, settings[sdd].passwd3, sizeof(password_level3) );
//	    password_level2 = settings[sdd].passwd2;
//	    password_level3 = settings[sdd].passwd3;

	    dp_price1 = settings[sdd].dp_price;
	    dp_amount1 = settings[sdd].dp_amount;
	    dp_unitprice1 = settings[sdd].dp_unitprice;
	    pump_max_litres1 = settings[sdd].max_amt_;

//	    settings[sdd].side_size = 2;   // 1/2    ==> default : 2   // Level 2
//		settings[sdd].display_mode = PL;  // PL/LP   ==> default : PL  // Level 2
//		settings[sdd].keypress_tone = Yes;  // Yes/No   ==> default : No   // Level 2

		display_minimumCentilitre1 = settings[sdd].startUp_suppressVol;  // (0 - 10) cL   ==> default : 4cL  // Level 2
		calibrationCan_measure1 = settings[sdd].calibration_measureCan;  // 10L/20L   ==> default : 20L  // Level 2

   }
   else
   {
	    sdd = 1;

		litre_price2 = settings[1].price_;
		sellmode2 = settings[sdd].def_t;
		//totaliser_vol1 = 57638694.00;

		overide_2 = settings[sdd].noz;

		pulser_index2 = settings[sdd].pi_;
		pulser_index_c2 = settings[sdd].pi_c;
		opmode2  = settings[sdd].mode;

	   // litre_price  = settings[sdd].price_;
		litre_price2 = settings[sdd].price_;

		timeout_picknozzle = 30;
		timeout_dispense   = 60;
		timeout_noflow = settings[sdd].noFlow_timeOut;

		pump_type = settings[sdd].pump_type_;

		strncpy(password_level1, settings[sdd].passwd1, sizeof(password_level1) );
		strncpy(password_level2, settings[sdd].passwd2, sizeof(password_level2) );
		strncpy(password_level3, settings[sdd].passwd3, sizeof(password_level3) );
//		password_level1 = settings[sdd].passwd1;
//		password_level2 = settings[sdd].passwd2;
//		password_level3 = settings[sdd].passwd3;

		dp_price2 = settings[sdd].dp_price;
		dp_amount2 = settings[sdd].dp_amount;
		dp_unitprice2 = settings[sdd].dp_unitprice;
		pump_max_litres2 = settings[sdd].max_amt_;

		display_minimumCentilitre2 = settings[sdd].startUp_suppressVol;  // (0 - 10) cL   ==> default : 4cL  // Level 2
		calibrationCan_measure2 = settings[sdd].calibration_measureCan;  // 10L/20L   ==> default : 20L  // Level 2
   }
}
//========================================
/*
 *
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
	     settings[sdd].def_t = P;

	    settings[sdd].pi_ = 798.35;
	    settings[sdd].pi_c = 760.33;
	    settings[sdd].mode = AUTO;

	    settings[sdd].price_ = 120.00;

	    timeout_picknozzle = 30;
	    timeout_dispense   = 60;

	    settings[sdd].noFlow_timeOut = 30;

//	    settings[sdd].pump_type_ = bluesky;    // lafeng;
//	    settings[sdd].pump_type_ = pump_type;

	    strncpy(settings[sdd].passwd1, "0000", 9);
	    strncpy(settings[sdd].passwd2, "0000", 9);
	    strncpy(settings[sdd].passwd3, "0000", 9);

	    settings[sdd].dp_price     = 2;
	    settings[sdd].dp_amount    = 2;
	    settings[sdd].dp_unitprice = 2;

	    settings[sdd].noz = nooveride;
	    settings[sdd].max_amt_ = 99999999;   //Maximum pump litres

	    settings[sdd].side_size = 2;   // 1/2    ==> default : 2   // Level 2
	    settings[sdd].display_mode = PL;  // PL/LP   ==> default : PL  // Level 2
	    settings[sdd].keypress_tone = No;  // Yes/No   ==> default : No   // Level 2

		settings[sdd].startUp_suppressVol = 0.04;  // (0 - 10) cL   ==> default : 4cL  // Level 2
		settings[sdd].calibration_measureCan = 20;  // 10L/20L   ==> default : 20L  // Level 2
	    settings[sdd].shift_login_type = None_; // None_/Code_/Card_   ==> default : None_  // Level 2

	    settings[sdd].commCard_enforced = true;  // Yes/No   ==> default : Yes   // Level 3
	    settings[sdd].calibration_type = Wizard; // Wizard/Manual_calib   ==> default : Wizard   // Level 3

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
 *
 */

void save_settings()
{
	extern const int save_settings1_loc;
	extern const int save_settings2_loc;
    int sz = sizeof(copy[0]);

  // for (int i = 0 ; i < sz;i++)
  // {
	   EEPROM_Write(save_settings1_loc, 0, &settings[0], sz);
	   EEPROM_Write(save_settings2_loc, 0, &settings[1], sz);
  // }
}


//==============================================
/*
 * save volumeTotaliser
 */
void save_volumeTotaliser(pump_sid side)
{
	int sz = sizeof( totaliser_vol_storeA);

	if (side == side_a)
	  {
		//EEPROM_Write_NUM(totVol_loc, totVol1_loc, tot);
		  totaliser_vol_storeA.totaliserVol_cal = totaliser_vol1c;
	  	  totaliser_vol_storeA.totaliserVol_real = totaliser_vol1;
	  	EEPROM_Write(totVol_loc, totVol1_loc, &totaliser_vol_storeA, sz);
	  }
	else if (side == side_b)
	  {
		//EEPROM_Write_NUM(totVol_loc, totVol2_loc, tot);
		  totaliser_vol_storeB.totaliserVol_cal = totaliser_vol2c;
	  	  totaliser_vol_storeB.totaliserVol_real = totaliser_vol2;
	  	EEPROM_Write(totVol_loc, totVol2_loc, &totaliser_vol_storeB, sz);
	  }
}

//===================================================
/*
 *  read volumeTotaliser
 */
void retrieve_volumeTotaliser(pump_sid side)
{
  int sz = sizeof( totaliser_vol_storeA);
	if (side == side_a)
	{
		//EEPROM_Read_NUM(totVol_loc,totVol1_loc);
		EEPROM_Read(totVol_loc, totVol1_loc, &totaliser_vol_storeA, sz);
		totaliser_vol1c =  totaliser_vol_storeA.totaliserVol_cal;
		totaliser_vol1 = totaliser_vol_storeA.totaliserVol_real;

	  	if(isnan(totaliser_vol1c)) totaliser_vol1c = 0.0;
	  	if(isnan(totaliser_vol1)) totaliser_vol1 = 0.0;

	}
	else if (side == side_b)
	{
		//EEPROM_Read_NUM(totVol_loc,totVol2_loc);
		 EEPROM_Read(totVol_loc, totVol2_loc, &totaliser_vol_storeB, sz);
		 totaliser_vol2c = totaliser_vol_storeB.totaliserVol_cal;
	  	 totaliser_vol2  = totaliser_vol_storeB.totaliserVol_real;

	  	if(isnan(totaliser_vol2c)) totaliser_vol2c = 0.0;
	  	if(isnan(totaliser_vol2)) totaliser_vol2 = 0.0;

	}
}

//==============================================
/*
 * clear volumeTotaliser
 */
void clear_volumeTotaliser(pump_sid side)
{
	int sz = sizeof( totaliser_vol_storeA);

	if (side == side_a)
	  {
		  totaliser_vol_storeA.totaliserVol_cal = 0.00;
	  	  totaliser_vol_storeA.totaliserVol_real = 0.00;
	  	EEPROM_Write(totVol_loc, totVol1_loc, &totaliser_vol_storeA, sz);
	  }
	else if (side == side_b)
	  {
		  totaliser_vol_storeB.totaliserVol_cal = 0.00;
	  	  totaliser_vol_storeB.totaliserVol_real = 0.00;
	  	EEPROM_Write(totVol_loc, totVol2_loc, &totaliser_vol_storeB, sz);
	  }
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

//==============================================
/*
 * save amountTotaliser
 */
void save_amountTotaliser(pump_sid side)
{
	int sz = sizeof( totaliser_vol_storeA);

	if (side == side_a)
	  {
		  totaliser_amt_storeA.totaliserVol_cal = totaliser_amt1c;
	  	  totaliser_amt_storeA.totaliserVol_real = totaliser_amt1;
	  	  EEPROM_Write(totAmount_loc, totAmount1_loc, &totaliser_amt_storeA, sz);
	  }
	else if (side == side_b)
	  {
		  totaliser_amt_storeB.totaliserVol_cal = totaliser_amt2c;
	  	  totaliser_amt_storeB.totaliserVol_real = totaliser_amt2;
	  	  EEPROM_Write(totAmount_loc, totAmount2_loc, &totaliser_amt_storeB, sz);
	  }
}

//===================================================
/*
 *  read amountTotaliser
 */
void retrieve_amountTotaliser(pump_sid side)
{
  int sz = sizeof( totaliser_amt_storeA);
	if (side == side_a)
	{
		EEPROM_Read(totAmount_loc, totAmount1_loc, &totaliser_amt_storeA, sz);
		totaliser_amt1c =  totaliser_amt_storeA.totaliserVol_cal;
		totaliser_amt1 = totaliser_amt_storeA.totaliserVol_real;

	  	if(isnan(totaliser_amt1c)) totaliser_amt1c = 0.0;
	  	if(isnan(totaliser_amt1)) totaliser_amt1 = 0.0;

	}
	else if (side == side_b)
	{
		 EEPROM_Read(totAmount_loc, totAmount2_loc, &totaliser_amt_storeB, sz);
		 totaliser_amt2c = totaliser_amt_storeB.totaliserVol_cal;
	  	 totaliser_amt2  = totaliser_amt_storeB.totaliserVol_real;

	  	if(isnan(totaliser_amt2c)) totaliser_amt2c = 0.0;
	  	if(isnan(totaliser_amt2)) totaliser_amt2 = 0.0;

	}
}

//==============================================
/*
 * clear amountTotaliser
 */
void clear_amountTotaliser(pump_sid side)
{
	int sz = sizeof( totaliser_vol_storeA);

	if (side == side_a)
	  {
		  totaliser_amt_storeA.totaliserVol_cal = 0.00;
	  	  totaliser_amt_storeA.totaliserVol_real = 0.00;
	  	  EEPROM_Write(totAmount_loc, totAmount1_loc, &totaliser_amt_storeA, sz);
	  }
	else if (side == side_b)
	  {
		  totaliser_amt_storeB.totaliserVol_cal = 0.00;
	  	  totaliser_amt_storeB.totaliserVol_real = 0.00;
	  	  EEPROM_Write(totAmount_loc, totAmount2_loc, &totaliser_amt_storeB, sz);
	  }
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
		  vol_effective1 = ( (atof(ep31_save.pump[0].calibrate_ct.ct_effectiveMinusBase) + vol_calibrated1) + 0.00011);

		  ct_settingsA.original = vol_real1;
		  ct_settingsA.base = vol_calibrated1;
	  	  ct_settingsA.effective = vol_effective1;
	  	  EEPROM_Write(ct_settings_loc, ct_settings1_loc, &ct_settingsA, sz);
	  }
	else if (side == side_b)
	  {
		  vol_effective2 = ( (atof(ep31_save.pump[1].calibrate_ct.ct_effectiveMinusBase) + vol_calibrated2) + 0.00011);

		  ct_settingsB.original = vol_real2;
		  ct_settingsB.base = vol_calibrated2;
		  ct_settingsB.effective = vol_effective2;
	  	  EEPROM_Write(ct_settings_loc, ct_settings2_loc, &ct_settingsB, sz);
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

//==============================================
/*
 * save ctTimed_settings
 */
void save_ctTimedSettings(pump_sid side)
{
	int8_t sz = sizeof(ctTimed_settingsA);

	if (side == side_a)
	  {
		  vol_effective1_1 = ( (atof(ep31_save.pump[0].calibrate_ct.ct_effectiveMinusBase) + vol_calibrated1) + 0.00011);

		  ctTimed_settingsA.effective = vol_effective1_1;
		  ctTimed_settingsA.startTime = atoi(ep31_save.pump[0].calibrate_ct.ct_startTime);
	  	  ctTimed_settingsA.endTime = atoi(ep31_save.pump[0].calibrate_ct.ct_endTime);
	  	  EEPROM_Write(ctTimed_settings_loc, ctTimed_settings1_loc, &ctTimed_settingsA, sz);
	  }
	else if (side == side_b)
	  {
		  vol_effective2_2 = ( (atof(ep31_save.pump[1].calibrate_ct.ct_effectiveMinusBase) + vol_calibrated2) + 0.00011);

		  ctTimed_settingsB.effective = vol_effective2_2;
		  ctTimed_settingsB.startTime = atoi(ep31_save.pump[1].calibrate_ct.ct_startTime);
	  	  ctTimed_settingsB.endTime = atoi(ep31_save.pump[1].calibrate_ct.ct_endTime);
	  	  EEPROM_Write(ctTimed_settings_loc, ctTimed_settings2_loc, &ctTimed_settingsB, sz);
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

	  	if(isnan(vol_effective1_1)) vol_effective1_1 = 0.0;

	}
	else if (side == side_b)
	{
		 EEPROM_Read(ctTimed_settings_loc, ctTimed_settings2_loc, &ctTimed_settingsB, sz);

		 vol_effective2_2 =  ctTimed_settingsB.effective;
		 startTime2 = ctTimed_settingsB.startTime;
		 endTime2 = ctTimed_settingsB.endTime;

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

//==============================================
/*
 * save originalPi_c
 */
void save_originalPi_c(pump_sid side)
{
	int8_t sz = sizeof(original_pi_c.original_pi_c1);

	if (side == side_a)
	  {
		  original_pi_c.original_pi_c1 = settings[0].pi_c;
	  	  EEPROM_Write(original_pi_c_loc, original_pi_c1_loc, &original_pi_c.original_pi_c1, sz);
	  }
	else if (side == side_b)
	  {
		  original_pi_c.original_pi_c2 = settings[1].pi_c;
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

		settings[0].pi_c = original_pi_c.original_pi_c1;
	}
	else if (side == side_b)
	{
		 EEPROM_Write(original_pi_c_loc, original_pi_c2_loc, &original_pi_c.original_pi_c2, sz);

		 settings[1].pi_c = original_pi_c.original_pi_c2;
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
		    original_pi_c.original_pi_c1 = settings[0].pi_c;
			EEPROM_Write(original_pi_c_loc, original_pi_c1_loc, &original_pi_c.original_pi_c1, sz);
	  }
	else if (side == side_b)
	  {
			original_pi_c.original_pi_c2 = settings[1].pi_c;
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
		calib_pulser =  (settings[0].pi_c * vol_calibrated1);
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
		calib_pulser =  (settings[1].pi_c * vol_calibrated2);
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
 * save Start-Shift volumeTotaliser
 */
void save_volumeTotaliser_startShift(pump_sid side)
{
	int sz = sizeof(startShiftTotaliser_vol_storeA);

	if (side == side_a)
	  {
//		EEPROM_Write_NUM(totVol_loc, totVol1_loc, tot);
		startShiftTotaliser_vol_storeA.totaliserVol_cal = totaliser_vol1c;
		startShiftTotaliser_vol_storeA.totaliserVol_real = totaliser_vol1;
	  	EEPROM_Write(startShiftTotVol_loc, startShiftTotVol1_loc, &startShiftTotaliser_vol_storeA, sz);
	  }
	else if (side == side_b)
	  {
		//EEPROM_Write_NUM(totVol_loc, totVol2_loc, tot);
		startShiftTotaliser_vol_storeB.totaliserVol_cal = totaliser_vol2c;
		startShiftTotaliser_vol_storeB.totaliserVol_real = totaliser_vol2;
	  	EEPROM_Write(startShiftTotVol_loc, startShiftTotVol2_loc, &startShiftTotaliser_vol_storeB, sz);
	  }
}

//===================================================
/*
 *  read Start-Shift volumeTotaliser
 */
void retrieve_volumeTotaliser_startShift(pump_sid side)
{
  int sz = sizeof(startShiftTotaliser_vol_storeA);
	if (side == side_a)
	{
		//EEPROM_Read_NUM(totVol_loc,totVol1_loc);
		EEPROM_Read(startShiftTotVol_loc, startShiftTotVol1_loc, &startShiftTotaliser_vol_storeA, sz);
		startShiftTotaliser_vol1c =  startShiftTotaliser_vol_storeA.totaliserVol_cal;
		startShiftTotaliser_vol1 = startShiftTotaliser_vol_storeA.totaliserVol_real;

	  	if(isnan(startShiftTotaliser_vol1c)) startShiftTotaliser_vol1c = 0.0;
	  	if(isnan(startShiftTotaliser_vol1)) startShiftTotaliser_vol1 = 0.0;

	}
	else if (side == side_b)
	{
		//EEPROM_Read_NUM(totVol_loc,totVol2_loc);
		 EEPROM_Read(startShiftTotVol_loc, startShiftTotVol2_loc, &startShiftTotaliser_vol_storeB, sz);
		 startShiftTotaliser_vol2c = startShiftTotaliser_vol_storeB.totaliserVol_cal;
		 startShiftTotaliser_vol2  = startShiftTotaliser_vol_storeB.totaliserVol_real;

	  	if(isnan(startShiftTotaliser_vol2c)) startShiftTotaliser_vol2c = 0.0;
	  	if(isnan(startShiftTotaliser_vol2)) startShiftTotaliser_vol2 = 0.0;

	}
}

//==============================================
/*
 * clear Start-Shift volumeTotaliser
 */
void clear_volumeTotaliser_startShift(pump_sid side)
{
	int sz = sizeof(startShiftTotaliser_vol_storeA);

	if (side == side_a)
	  {
		startShiftTotaliser_vol_storeA.totaliserVol_cal = 0.00;
		startShiftTotaliser_vol_storeA.totaliserVol_real = 0.00;
	  	EEPROM_Write(startShiftTotVol_loc, startShiftTotVol1_loc, &startShiftTotaliser_vol_storeA, sz);
	  }
	else if (side == side_b)
	  {
		startShiftTotaliser_vol_storeB.totaliserVol_cal = 0.00;
		startShiftTotaliser_vol_storeB.totaliserVol_real = 0.00;
	  	EEPROM_Write(startShiftTotVol_loc, startShiftTotVol2_loc, &startShiftTotaliser_vol_storeB, sz);
	  }
}


//==============================================
/*
 * save Start-Shift amountTotaliser
 */
void save_amountTotaliser_startShift(pump_sid side)
{
	int sz = sizeof(startShiftTotaliser_vol_storeA);

	if (side == side_a)
	  {
		  startShiftTotaliser_amt_storeA.totaliserVol_cal = startShiftTotaliser_amt1c;
		  startShiftTotaliser_amt_storeA.totaliserVol_real = startShiftTotaliser_amt1;
	  	  EEPROM_Write(startShiftTotAmount_loc, startShiftTotAmount1_loc, &startShiftTotaliser_amt_storeA, sz);
	  }
	else if (side == side_b)
	  {
		  startShiftTotaliser_amt_storeB.totaliserVol_cal = startShiftTotaliser_amt2c;
		  startShiftTotaliser_amt_storeB.totaliserVol_real = startShiftTotaliser_amt2;
	  	  EEPROM_Write(startShiftTotAmount_loc, startShiftTotAmount2_loc, &startShiftTotaliser_amt_storeB, sz);
	  }
}

//===================================================
/*
 *  read Start-Shift amountTotaliser
 */
void retrieve_amountTotaliser_startShift(pump_sid side)
{
  int sz = sizeof(startShiftTotaliser_amt_storeA);
	if (side == side_a)
	{
		EEPROM_Read(startShiftTotAmount_loc, startShiftTotAmount1_loc, &startShiftTotaliser_amt_storeA, sz);
		startShiftTotaliser_amt1c =  startShiftTotaliser_amt_storeA.totaliserVol_cal;
		startShiftTotaliser_amt1 = startShiftTotaliser_amt_storeA.totaliserVol_real;

	  	if(isnan(startShiftTotaliser_amt1c)) startShiftTotaliser_amt1c = 0.0;
	  	if(isnan(startShiftTotaliser_amt1)) startShiftTotaliser_amt1 = 0.0;

	}
	else if (side == side_b)
	{
		 EEPROM_Read(totAmount_loc, startShiftTotAmount2_loc, &startShiftTotaliser_amt_storeB, sz);
		 startShiftTotaliser_amt2c = startShiftTotaliser_amt_storeB.totaliserVol_cal;
		 startShiftTotaliser_amt2  = startShiftTotaliser_amt_storeB.totaliserVol_real;

	  	if(isnan(startShiftTotaliser_amt2c)) startShiftTotaliser_amt2c = 0.0;
	  	if(isnan(startShiftTotaliser_amt2)) startShiftTotaliser_amt2 = 0.0;

	}
}

//==============================================
/*
 * clear Start-Shift amountTotaliser
 */
void clear_amountTotaliser_startShift(pump_sid side)
{
	int sz = sizeof(startShiftTotaliser_vol_storeA);

	if (side == side_a)
	  {
		  startShiftTotaliser_amt_storeA.totaliserVol_cal = 0.00;
		  startShiftTotaliser_amt_storeA.totaliserVol_real = 0.00;
	  	  EEPROM_Write(startShiftTotAmount_loc, startShiftTotAmount1_loc, &startShiftTotaliser_amt_storeA, sz);
	  }
	else if (side == side_b)
	  {
		  startShiftTotaliser_amt_storeB.totaliserVol_cal = 0.00;
		  startShiftTotaliser_amt_storeB.totaliserVol_real = 0.00;
	  	  EEPROM_Write(startShiftTotAmount_loc, startShiftTotAmount2_loc, &startShiftTotaliser_amt_storeB, sz);
	  }
}


//===================================================
/*
 *  copy settings to the structure to be used for prog.
 */
void copy_settings(copy_dir dir)
{
	int sz = sizeof(copy[0]);
	uint8_t* cpy;
	uint8_t* sett;

	 for(int sd = 0;sd<2;sd++ )
	 {
		cpy = &copy[sd];
		sett = &settings[sd];

		   for (int i = 0 ; i < sz;i++)
		   {
			   if (dir == move_to_copy)
			   {
				   *cpy++ = *sett++;
			   }
				 else
			   {
				   *sett++ = *cpy++;
			   }
		   }
	 }
}
//====================================================
int get_auth()
{
	auth_flag = 0;

	if(opmode == MANUAL)
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
	if(opmode2 == MANUAL)
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


void dp_init(pump_sid sdd)
{
	if(settings[sdd-1].dp_price == 0)
	   settings[sdd-1].dp_price = 2;
    if(settings[sdd-1].dp_amount == 0)
	   settings[sdd-1].dp_amount = 2;
    if(settings[sdd-1].dp_unitprice == 0)
	   settings[sdd-1].dp_unitprice = 2;
}
