/*
 * pump_comm.h
 *
 *  Created on: Oct 16, 2022
 *      Author: tunjo
 */

#ifndef PUMP_COMM_PUMP_COMM_H_
#define PUMP_COMM_PUMP_COMM_H_

#include "stdbool.h"
#include "interface1.h"
#include "stdint.h"
#include "settings.h"

//#define pump_rx_bufsize  150
#define STX		0xA5
#define rx_STX	0x5A



//states, used for decision and sending to server
typedef enum{
	PUMP_NOT_ACTIVE,		//a.k.a PNP (Pump Not Programmed)
	PUMP_NOT_LOCKED,
	NOZZLE_HANG_DOWN,
	NOZZLE_HANG_UP,

	PUMP_AUTH_NOZZLE_HANG_DOWN,
	PUMP_AUTH_NOZZLE_HANG_UP,

	PUMP_FILLING,
	PUMP_FILLED_LIMIT,

	PUMP_FILL_COMP_NOZZLE_HANG_DOWN,
	PUMP_FILL_COMP_NOZZLE_HANG_UP,

	//------------------------------------
	// Added to reflect the state of the server 17/8/2022 by tunjow
	//only used by interface device...
	server_busy,
	server_error,
	server_timeout,
	server_success,
	//------------------------------------

	PUMP_SWITCHED_OFF = 252,
	PUMP_STATUS_OTHERS = 253,
	PUMP_ERROR,
	PUMP_STATUS_UNKNOWN,
}pump_state_enum;        //simply returned pump status + nozzle state


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
	STATUS_SWITCHED_OFF
}pump_status_enum;


///* enum for pump status */
//typedef enum
//{
//	STATUS_PUMP_ON = 255,
//
//	STATUS_IDLE = 1,
//	STATUS_NOZZLE_DOWN,
//	STATUS_NOZZLE_UP,
//	STATUS_AUTHORIZED_NOZZLE_DOWN,
//	STATUS_AUTHORIZED_NOZZLE_UP,
//	STATUS_FILLING_
//}pump_status_enum_4G;

//typedef enum {
//	AUTH_PUMP_ON_NOZZLE_UP,
//	AUTH_PUMP_ON_NOZZLE_DOWN,
//}auth_type_enum;

//for keeping track of the transaction data
typedef enum{
	D_NILL,
	D_FILLING,
	D_FILL_COMPLETE,
	D_LOGGED
}data_state_enum;

//for switch between auto mode and transaction mode
typedef enum{
//	MANUAL, //ATTENDATE_MODE,
//	AUTO, //VOUCHER_MODE,
	AUTO_PERMANENT
}mode_enum;

/** @enum 	Transaction type
 * 			For switching	*/
typedef enum
{
	TT_ATTENDANT,
	TT_EFUELING,

}trans_type_enum;

/** @struct 	For sending data to the main thread to log */
typedef struct{
	int PID; 		//pump index
	char tid[18];	//transaction id
	float amo;		//amount
	float vol;		//volume
	float tz;		//totalizer
	int tp;			//transaction duration/period
	float unp;		//transaction price
	int tm;			//transaction time
	char tty;		//transaction type (wyfy channel in this case)
	char tg[20];		//transaction type (wyfy channel in this case)
}log_data_struct;

//==============================================================================
typedef struct
	{
    	pump_state_enum state;				//this is used in a finite state machine, it is also what GO broadcast to the server
		pump_state_enum prevState;			//previous machine state
	//	auth_type_enum auth_type;

		data_state_enum status_;       //state for the data enum

	//	bool nozzle_state; 	//NEW for multi nozzle pump, TRUE : UP, FALSE : DOWN

		mode_enum mode;
		bool permanent_auto;
		bool log_next_trans_as_efueling;
		unsigned char pumpno;

		char name[10];
		float unitprice;
		double vol_totalizer;
		double amo_totalizer;
		float filled_vol;
		float filled_amo;
		int filling_start_time;		//time at the start of filling
		int filling_duration;		//time it takes to complete a filling process
		int nozzle_num;

		bool  setPriceFlag;			//used to alert the protocol interface to set the price to a new price
		float setPriceFloat;
		float temp_tz;

		bool newDay;				//used to know when the main should capture the first toltalizer of the day
		bool first_operation;
	}pump_;
//=========================================================================================
	typedef enum
		{
			GET_STATE = 0xB8,
			CONTROL_DISPENSER = 0xA0,
			UNCTRL_DISP = 0xB0,
			FUELING = 0xA1,
			STOP = 0xB1,
			F_WX_FX_CASH = 0xA2,    	/**< Filling with fixed cash (Preset amount: 9999.99) */
			F_WX_FX_LITER = 0xA3,   	/**< Filling with fixed liter (Preset volume: 9999.99) */
			SETTING_PRICE = 0xA4,	    /**< max: 99.99 */
			SETTING_PRE_VALVE = 0xA6,   /**< MAX: 0.99 */
			CLR_SHIFT_SUM = 0xA8,
			QRY_SHIFT_SUM = 0xA9,
			QRY_TOTAL_CASH = 0xAC,
			QRY_TOTAL_LITER = 0xAD,
		//----------------------------
			GET_PUMP_DATA = 0xAE,
			PUT_PUMP_DATA = 0xBA,
			GET_STATION   = 0xBB
		}command_enum;

		/* enum for nozzle state */
		typedef enum{
			NOZZLE_DOWN,
			NOZZLE_UP,
		}nozzle_state_enum;

		typedef enum
		{
			NOZZLE_BIT = 0x40,	// == 1? nozzle up, 0? nozzle down
			FILLING_BIT = 0x01, // == 1 ? filling state
			STATE_BIT = 0x02, // == 1? idle state
		}state_bit_mask;
//============================================================
		typedef struct
		{
			char name1[4];
			char name2[4];
			char product1[4];
			char product2[4];
		   //---------------------
			char station_name[20];
			char address[70];
		}Name_address;
//============================================================
      typedef struct
      {
    	 char address;
    	 uint8_t nozzle;
    	 float price;
    	 char mode[7];
    	 uint8_t dp_vol;
    	 uint8_t dp_amo;
    	 uint8_t dp_unp;
      }pump_data;
//============================================================


#ifdef __cplusplus
extern "C" {
#endif

void pump_ini();
void pump_rx();

//===================================
extern int8_t  change_p, change_v;

pump_status_enum pump_status_;
extern pump_status_enum_4G pump1_status_4G,
					pump2_status_4G;

uint8_t sendState(void);
void sendvol_totalizer(void);
void getFilledAmo(void);
void getfilled_vol(void);
void getUnitPrice(void);
void setUnitPrice(void);
void getName(void);

//int getState(int ind);
//float getUnitPrice();
//float getvol_totalizer();
//float getfilled_vol();
//float getFilledAmo();
//char* getName();

void setMode(int ind,mode_enum md);
mode_enum getMode(int ind);

setName(int ind,char* name);
//char* getName(int ind);

setProduct(int ind ,char* pd);
char* getProduct(int ind);

 int auth_by_vol(int ind,float vol);	// authorize the pump's volume
 int auth_by_amo(int ind,float amnt);	// authorize the pump's amount
 int set_amo(int ind,float amnt);	    //
// void authorise(int ind);	            // authorize pump with maximum vol MAX_PRESET_VOL

 int8_t controller_authorise(void);

 void stop(int ind);
 setIndex(int ind,	int i );
 int getIndex(int ind);


 uint8_t msg_parse_pump(char* str);



#ifdef __cplusplus
}
#endif

#endif /* PUMP_COMM_PUMP_COMM_H_ */
