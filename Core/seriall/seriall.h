/*
 * seriall.h
 *
 *  Created on: Apr 25, 2023
 *      Author: KC
 */

#ifndef SERIALL_SERIALL_H_
#define SERIALL_SERIALL_H_

#include "main.h"
#include "settings.h"

//char uart2_rx_buf[pump_rx_bufsize] = {0};
//
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
//
//typedef enum
// {
//	  DN_LAFNG17K = 0b00000001,		//keypad->LAFNG-12
//	  DN_BLSKY18K,       			//keypad->BLSKY-12
//	  DN_BLSKY22					//keypad->BLSKY-16
// }pump;
//
//typedef enum
//{
//	nozzle_name,
//	product_name,
//	disp_type,
//	keypad_type,
//}pumpCompPart;
//
//
//typedef struct
//  {
////     opmode_  mode;
////     nozzle_overide noz;
////     sellmode_ def_t;
////     pump pump_type_;
////     float price_;
////     int id_;
////     int noflow_;
////     int max_amt_;
////     uint32_t passwd1;
////     uint32_t passwd2;
////     float pi_c;
////     float pi_;
////     int dp_price;
////	 int8_t dp_amount;
////	 int dp_unitprice;
//	 //peripherals
//	 displaytype display_;
//	 keypadtype keypad__;
//	 productname product__;
//	 nozzleid noz_id;
//  }pump_settings;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern int8_t config_found;

extern long tranxA_token,
			tranxB_token;

extern int8_t config_mode;

//extern pump_settings settings[2];

extern pump_settings_stream1 settings_stream1[2],
					 	 	 settings_config_stream1[2];

extern pump_settings_stream2 settings_stream2[2],
							 settings_config_stream2[2];

extern pump_settings_stream3 settings_stream3[2],
				       	     settings_config_stream3[2],
							 copy_stream3[2];

void config_rx(void);
void configure_pump(pumpCompPart _case);
void config_rx_parse(void);

#endif /* SERIALL_SERIALL_H_ */
