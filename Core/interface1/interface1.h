/*
 * nextion1.h
 *
 *  Created on: Oct 14, 2022
 *      Author: tunjo
 */

#ifndef INTERFACE1_INTERFACE1_H_
#define INTERFACE1_INTERFACE1_H_




#define nextion_rx_bufsize  150
#define TOTAL_NO_OF_PUMPS 2



#include "stdio.h"
#include "string.h"
#include "stdbool.h"

typedef struct
{     //  char di[17];
		char cmd[3];
		char msg[200];
		char id[20];
	//	bool isDirty;
}msgg;

//----------------------------------------------------
typedef struct
{
	char head;
	char id[20];
	char cmd[3];
	char msg[200];
	char tail;
	bool isDirty;
//	char msg_[200];
	uint8_t ln_;
} rcv_struct;


/************  enum for tag file *********************/
  typedef enum
  {
	  clear,
	  no_clear
  }tag_action;


/****************ack status for stop,pause,resume,mode,login logout*******/
typedef enum
{
	success_,
	failed,
	pump_not_found
}auth_cmd;

/**************************Ack status for authorize ***************************/
typedef enum
{
	success,
	failed_pump_offline,
	failed_nozzle_down,
	failed_dispensing
}auth_ack;
/******************************COMMANDS FOR GO*********************************/
typedef enum
{
	_not_defined,
	_get_status = 1,
	_auth_amt,
	_auth_vol,
	_stop_cmd,
	_pause_cmd,
	_resume_cmd,
	_switch_mode_cmd,
	_login_cmd,
	_logout_cmd,
	_server_resp = 11,        //Added for returning the server message sent..
	_who
} GO_CMD;
/************************************* mode ***********************************/
typedef enum
{
	_manual,
	_auto
}MODE;

/*****************************************************************/
typedef enum
{
   not_active = 0,
   nozzle_down = 2,
   nozzle_up = 3,
   nozzle_up_auth = 5,
   filling_ = 6,
   filling_complete_nozzle_down = 8,
   switched_off = 252,
   not_configured = 254,
   offline_ = 255
}pump_status;

/*************************************/
typedef enum
{
	auth_price = 0,
	auth_vol
}auth_type_;

/*************************************/
typedef enum
{
	fail = 0,
	ok
}parse_result;

/*************************   structure for holding ***************/
/***********************     the val from callback  **************/
typedef struct
{
	//char sent_token[20];
	uint32_t id;
	uint32_t tk;
	float am;
	char pump_name[3];
	char token_[12];
	char transaction_type[2];
	bool wait_for_server;
	uint8_t auth_type;
	char str_[200];
}ep4_callback;


#ifdef __cplusplus
extern "C" {
#endif

//void nextion1_ini();
//void nextion1_rx();
//
//bool msg_parse(char* str, int n_);
//void BuildString();
//void uartSendData(char* output);
//
//int get_pump_index_from_name_string(char* pn);
//
////--------------------------------------
//void write_att_tag(int fxn);
//void read_att_tag();
////--------------------------------------
//void write_att_tag2(int fxn);
//void read_att_tag2();
//
//void GO_fsm1();


#ifdef __cplusplus
}
#endif

#endif /* INTERFACE1_INTERFACE1_H_ */
