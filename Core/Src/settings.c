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
#include "stdio.h"
#include "EEPROM.h"


//#######################################

char device_id [] =	"860537064685993";        //"860537064685357";           //"860537064685993";     //"860537064685340";

//#######################################



 const int max_events_per_state = 10;

int8_t connected;

 uint16_t fast_flow_threshold  = 0;  //=  pulser_index_c ;   //140;

extern int8_t change_p,
			  change_v;

extern float auth_v,
			 auth_p;

extern float amt_real,
		  	 amt_real2;
//			 amt,
//			 amt2;

extern int t;

extern uint16_t _tt,
				_tt2;

extern uint32_t transaction_period,
				transaction_period2;

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

	float totaliser_vol1 = 0.00;
	float totaliser_vol1c = 0.00;
	float totaliser_vol2 = 0.00;
	float totaliser_vol2c = 0.00;

	float firstTotaliser_vol1 = 0.00,
		  firstTotaliser_vol1c = 0.00,
		  firstTotaliser_vol2 = 0.00,
		  firstTotaliser_vol2c = 0.00;

	float totaliser_amt1 = 0.00,
		  totaliser_amt1c = 0.00,
		  totaliser_amt2 = 0.00,
		  totaliser_amt2c = 0.00;

	float working_volTotaliser = 0;
	float working_volTotaliserc = 0;
	float running_volTotaliser = 0;
	float running_volTotaliserc = 0;

	float working_volTotaliser2 = 0;
	float working_volTotaliser2c = 0;
	float running_volTotaliser2 = 0;
	float running_volTotaliser2c = 0;

	float working_amtTotaliser = 0,
			working_amtTotaliserc = 0,
			running_amtTotaliser = 0,
			running_amtTotaliserc = 0,
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

float lastSale1 = 0.00,
      lastSale1c = 0.00;
float lastSale2 = 0.00,
	  lastSale2c = 0.00;

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
  uint32_t password_level1 = 1234;
  uint32_t password_level2 = 12345678;

 int log_max = 50;

 const int save_settings1_loc = 0;
 const int save_settings2_loc = 5;
 const int16_t save_pumpType_loc = 400;
 const int16_t save_productType_loc = save_pumpType_loc + 1;
 const int16_t save_nozzleId_loc = save_pumpType_loc + 2;
 const int16_t totalizerDay_loc = 404;

 const int8_t lastSale_loc  =  40;
 const int lastSale1_loc =  0;
 const int lastSale2_loc =  lastSale1_loc + (2+(2*4));  //4bytes*2=8bytes+2 = 10bytes ahead.

 const int tot_loc  =  10;
 const int tot1_loc =  0;
 const int tot2_loc =  tot1_loc + (2+(2*4));  //4bytes*2=8bytes+2 = 10bytes ahead.

 const int totAmount_loc  =  230;
 const int totAmount1_loc =  0;
 const int totAmount2_loc =  totAmount1_loc + (2+(2*4));  //4bytes*2=8bytes+2 = 10bytes ahead.

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
				totaliser_amt_storeB;

lastSale_store lastSale_storeA,
			   lastSale_storeB;

time_ timeA, timeB;
date_ dateA, dateB;

const uint32_t flash_beginA = 0x000000;
const uint32_t flash_endA   = 0x3fffff;

const uint32_t flash_beginB = 0x400000;
const uint32_t flash_endB   = 0x7fffff;


//   MENU ITEMS

 char* menu1[4]  = {"  tot   ","  log   "};
 char* menu2[16] = {"  nnode ","address "," no22le ","prog.type"," price  ","no flo. t","hi. litre","ch  pass","send cfg","get. cfg ","clr log","clr tot","calibrat.","display ", "  cloc"};
 char* menu3[3]  = {"tmm cfg","flo rate"};

//-------------------------------------
void retrieve_settings()
{
	extern const int save_settings1_loc;
	extern const int save_settings2_loc;

   int sz = sizeof(copy[0]);

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
	    timeout_noflow = settings[sdd].noflow_;

	    pump_type = settings[sdd].pump_type_;

	    password_level1 = settings[sdd].passwd1;
	    password_level2 = settings[sdd].passwd2;

	    dp_price1 = settings[sdd].dp_price;
	    dp_amount1 = settings[sdd].dp_amount;
	    dp_unitprice1 = settings[sdd].dp_unitprice;
	    pump_max_litres = settings[sdd].max_amt_;
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
			    timeout_noflow = settings[sdd].noflow_;

			    pump_type = settings[sdd].pump_type_;

			    password_level1 = settings[sdd].passwd1;
			    password_level2 = settings[sdd].passwd2;

			    dp_price2 = settings[sdd].dp_price;
			    dp_amount2 = settings[sdd].dp_amount;
			    dp_unitprice2 = settings[sdd].dp_unitprice;
			    pump_max_litres2 = settings[sdd].max_amt_;
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

	    settings[sdd].pi_ = 180;
	    settings[sdd].pi_c = 180;
	    settings[sdd].mode = MANUAL;

	    settings[sdd].price_ = 120.00;

	    timeout_picknozzle = 30;
	    timeout_dispense   = 60;

	    settings[sdd].noflow_ = 30;

//	    settings[sdd].pump_type_ = bluesky;    // lafeng;
//	    settings[sdd].pump_type_ = pump_type;

	    settings[sdd].passwd1 = 1234;
	    settings[sdd].passwd2 = 12345678;

	    settings[sdd].dp_price     = 2;
	    settings[sdd].dp_amount    = 2;
	    settings[sdd].dp_unitprice = 2;

	    settings[sdd].noz = nooveride;

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
		//EEPROM_Write_NUM(tot_loc, tot1_loc, tot);
		  totaliser_vol_storeA.totaliserVol_cal = totaliser_vol1c;
	  	  totaliser_vol_storeA.totaliserVol_real = totaliser_vol1;
	  	EEPROM_Write(tot_loc, tot1_loc, &totaliser_vol_storeA, sz);
	  }
	if (side == side_b)
	  {
		//EEPROM_Write_NUM(tot_loc, tot2_loc, tot);
		  totaliser_vol_storeB.totaliserVol_cal = totaliser_vol2c;
	  	  totaliser_vol_storeB.totaliserVol_real = totaliser_vol2;
	  	EEPROM_Write(tot_loc, tot2_loc, &totaliser_vol_storeB, sz);
	  }
}

//===================================================
/*
 *  read volumeTotaliser
 */
float retrieve_volumeTotaliser(pump_sid side)
{
  int sz = sizeof( totaliser_vol_storeA);
	if (side == side_a)
	{
		//EEPROM_Read_NUM(tot_loc,tot1_loc);
		EEPROM_Read(tot_loc, tot1_loc, &totaliser_vol_storeA, sz);
		totaliser_vol1c =  totaliser_vol_storeA.totaliserVol_cal;
		totaliser_vol1 = totaliser_vol_storeA.totaliserVol_real;

	  	if(isnan(totaliser_vol1c)) totaliser_vol1c = 0.0;
	  	if(isnan(totaliser_vol1)) totaliser_vol1 = 0.0;

	}
	if (side == side_b)
	{
		//EEPROM_Read_NUM(tot_loc,tot2_loc);
		EEPROM_Read(tot_loc, tot2_loc, &totaliser_vol_storeB, sz);
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
	  	EEPROM_Write(tot_loc, tot1_loc, &totaliser_vol_storeA, sz);
	  }
	if (side == side_b)
	  {
		  totaliser_vol_storeB.totaliserVol_cal = 0.00;
	  	  totaliser_vol_storeB.totaliserVol_real = 0.00;
	  	EEPROM_Write(tot_loc, tot2_loc, &totaliser_vol_storeB, sz);
	  }
}


//void save_day1stVolTotaliser(pump_sid side)
//{
//	int sz = sizeof( totaliser_vol_storeA);
//
//	if (side == side_a)
//	  {
//		//EEPROM_Write_NUM(tot_loc, tot1_loc, tot);
//		  totaliser_vol_storeA.totaliserVol_cal = totaliser_vol1c;
//	  	  totaliser_vol_storeA.totaliserVol_real = totaliser_vol1;
//	  	EEPROM_Write(day1stTot_loc, tot1_loc, &totaliser_vol_storeA, sz);
//	  }
//	if (side == side_b)
//	  {
//		//EEPROM_Write_NUM(tot_loc, tot2_loc, tot);
//		  totaliser_vol_storeB.totaliserVol_cal = totaliser_vol2c;
//	  	  totaliser_vol_storeB.totaliserVol_real = totaliser_vol2;
//	  	EEPROM_Write(day1stTot_loc, tot2_loc, &totaliser_vol_storeB, sz);
//	  }
//}

//===================================================
/*
 *  read totaliser
 */
//float retrieve_day1stVolTotaliser(pump_sid side)
//{
//  int sz = sizeof( totaliser_vol_storeA);
//	if (side == side_a)
//	{
//		//EEPROM_Read_NUM(tot_loc,tot1_loc);
//		EEPROM_Read(day1stTot_loc, tot1_loc, &totaliser_vol_storeA, sz);
//		firstTotaliser_vol1c =  totaliser_vol_storeA.totaliserVol_cal;
//		firstTotaliser_vol1 = totaliser_vol_storeA.totaliserVol_real;
//
//	  	if(isnan(firstTotaliser_vol1c)) firstTotaliser_vol1c = 0.0;
//	  	if(isnan(firstTotaliser_vol1)) firstTotaliser_vol1 = 0.0;
//
//	}
//	if (side == side_b)
//	{
//		//EEPROM_Read_NUM(tot_loc,tot2_loc);
//		EEPROM_Read(day1stTot_loc, tot2_loc, &totaliser_vol_storeB, sz);
//		 firstTotaliser_vol2c = totaliser_vol_storeB.totaliserVol_cal;
//	  	 firstTotaliser_vol2  = totaliser_vol_storeB.totaliserVol_real;
//
//	  	if(isnan(firstTotaliser_vol2c)) firstTotaliser_vol2c = 0.0;
//	  	if(isnan(firstTotaliser_vol2)) firstTotaliser_vol2 = 0.0;
//
//	}
//}

//===================================================
/*
 * save lastSale
 */
void save_lastSale(pump_sid side)
{
	int8_t sz = sizeof(lastSale_storeA);

	if (side == side_a)
	  {
		//EEPROM_Write_NUM(tot_loc, tot1_loc, tot);
		lastSale_storeA.lastSale_real = amt_real;   //    log_a_new.vol_ = amt_real;
		lastSale_storeA.lastSale_cal = amt;   //log_a_new.vol__ = amt;   //calibrated
	  	EEPROM_Write(lastSale_loc, lastSale1_loc, &lastSale_storeA, sz);
	  }
	if (side == side_b)
	  {
		//EEPROM_Write_NUM(tot_loc, tot2_loc, tot);
		lastSale_storeB.lastSale_real = amt_real2;   //    log_b_new.vol_ = amt_real2;
		lastSale_storeB.lastSale_cal = amt2;   //log_b_new.vol__ = amt2;   //calibrated
	  	EEPROM_Write(lastSale_loc, lastSale2_loc, &lastSale_storeB, sz);
	  }
}

//===================================================
/*
 *  read lastSale
 */
float retrieve_lastSale(pump_sid side)
{
  int8_t sz = sizeof(lastSale_storeA);

	if (side == side_a)
	{
		EEPROM_Read(lastSale_loc, lastSale1_loc, &lastSale_storeA, sz);
		lastSale1 = lastSale_storeA.lastSale_real;
		lastSale1c =  lastSale_storeA.lastSale_cal;

	  	if(isnan(lastSale1)) lastSale1 = 0.0;
	  	if(isnan(lastSale1c)) lastSale1c = 0.0;

	}
	if (side == side_b)
	{
		EEPROM_Read(lastSale_loc, lastSale2_loc, &lastSale_storeB, sz);
		lastSale2  = lastSale_storeB.lastSale_real;
		lastSale2c = lastSale_storeB.lastSale_cal;

	  	if(isnan(lastSale2)) lastSale2 = 0.0;
	  	if(isnan(lastSale2c)) lastSale2c = 0.0;

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
		lastSale_storeA.lastSale_real = 0.00;   //    log_a_new.vol_ = amt_real;
		lastSale_storeA.lastSale_cal = 0.00;   //log_a_new.vol__ = amt;   //calibrated
	  	EEPROM_Write(lastSale_loc, lastSale1_loc, &lastSale_storeA, sz);
	  }
	if (side == side_b)
	  {
		lastSale_storeB.lastSale_real = 0.00;   //    log_b_new.vol_ = amt_real2;
		lastSale_storeB.lastSale_cal = 0.00;   //log_b_new.vol__ = amt2;   //calibrated
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
	if (side == side_b)
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
float retrieve_amountTotaliser(pump_sid side)
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
	if (side == side_b)
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
	if (side == side_b)
	  {
		  totaliser_amt_storeB.totaliserVol_cal = 0.00;
	  	  totaliser_amt_storeB.totaliserVol_real = 0.00;
	  	  EEPROM_Write(totAmount_loc, totAmount2_loc, &totaliser_amt_storeB, sz);
	  }
}


//===================================================

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
	'A',	//1
	'1',	//2
	'5',	//3
	'3',	//4
	'8',	//5
	'2',	//6
	'4',	//7
	'7',	//8
	'6',	//9
	'9',	//10
	'B',	//11
	'C',	//12
	'D',	//13
	'.',	//14
	'F',    //15
	'0'     //16
};

char bluesky_keypad[22] =     //bluesky 12pin
{
	'-',	//0
	'*',	//1
	'1',	//2
	'5',	//3
	'3',	//4
	'8',	//5
	'2',	//6
	'4',	//7
	'7',	//8
	'6',	//9
	'9',	//10
	'B',	//11
	'D',	//12  //start / enter
	'C',	//13  //stop
	'.',	//14
	'F',    //15   clear
	'B',    //16   f3 key  up
	'A',    //17   f1 key   mapped for back
	'C',    //18   f2 key   down
	'-',    //19   f4 key  PROG key.
	'0',    //20
	'-',    //21   //print totaliser view.
};


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
