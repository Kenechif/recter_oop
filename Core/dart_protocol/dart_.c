/*
 * dart_.c
 *
 *  Created on: 28 Jun 2024
 *      Author: Jheebolar
 */




#include "dart_.h"
#include "../crc/checksum.h"

/*
 * p_wayne_485.cpp
 *
 *  Created on: Jan 23, 2020
 *      Author: J.cliff
 *      Description: Low level logic for the wayne rs-485 protocol
 */

/* stl includes */
#include <string.h>
//#include <iostream>
//#include <map>
#include <stdlib.h>
#include <stdio.h>
//#include <iomanip>
#include <inttypes.h>
#include <main.h>
//#include <net/comm.h>
#include <stdbool.h>
#include <stdint.h>
//#include <vector>
//#include <string>


int head_posn = 0;
/* eat api includes */
// these are already contained in init.h
//extern "C"{
//#include "eat_modem.h"
//#include "eat_interface.h"
//}

/* our api includes */
//#include "date_time.h"
//#include "checksum.h"
//#include "uart_listener.h"
//#include "dart.h"
//#include "protocol\BaseProtocol.h"

//using namespace std;

//char mepsan_magic_char = 0;	/* This character is used to control the mode of operation */

//extern Uart uart[3];

command_enum1 command_,
			  command_2;

uint8_t status1_;

uint8_t rx_buf1[pump_rx_bufsize] = {0},
		rx_buf2[pump_rx_bufsize] = {0};

extern nozzle_state_enum nozzle_state;		//state of nozzle returned from pump

uint8_t checked = 0,
		pump_unprogrammed = 0;

bool ack_send = false,
	 ack_send2 = false,
	 outstanding_command = false,
	 command_response = false,
	 command_response2 = false;

extern bool nozzle_out1 = false,
			nozzle_out2 = false;

extern uint8_t status_change_pump1 = 0,
			   status_change_noz1 = 0;

int16_t header = 0,
		footer = 0;

unsigned long t_exec1 = 0,
		      t_exec2 = 0,
			  t_exec3 = 0,
			  t_exec4 = 0,
			  t_exec5 = 0,
			  t_exec6 = 0,
			  t_exec7 = 0,
			  t_exec8 = 0,
			  t_exec9 = 0;

unsigned int cheq2 = 0;


extern float price_update1 = 0,
			 price_update2 = 0;

//unsigned char price_update_bcd[MAX_NON*MAX_NOP][3];

void dart_init(void)
{
//    pumpno = 0;
    addr = 0x4F + pumpno;
    addr2 = 0x4F + pumpno2;

    command_ = NO_COMMAND;
    command_response = false;
    command_response2 = false;
	trans = 0;
	lng = 0;
	ctrl = 0;
	TX = 0;
	TX2 = 0;
	crc1 = 0;
	crc2 = 0;

	preset_vol = 0;
	preset_amo = 0;
	price_update = 0;

	full_msg_size =0;

	//set these before calling send (SET_MAMO)
	mamo_update = 0;
	vol_totalizer = 0;
	dpamo = 0;	//0-8
	dpvol = 0;
	dpunp = 0;

	resp = NOREPLY;
	resp2 = NOREPLY;
	r_addr = 0; //response addr
	r_ctrl = 0;	//response ctrl character
	r_trans = 0;
	r_pumpno = 0;		//pump id i.e. which pump
	r_nozzle = 0x01;
	r_lng = 0;
	r_alarm_code = 0;

	r_TX = 0;
	r_suspend = 0;
	r_resume = 0;

	r_addr2 = 0; //response addr
	r_ctrl2 = 0;	//response ctrl character
	r_trans2 = 0;
	r_pumpno2 = 0;		//pump id i.e. which pump
	r_nozzle2 = 0x01;
	r_lng2 = 0;
	r_alarm_code2 = 0;

	r_TX2 = 0;
	r_suspend2 = 0;
	r_resume2 = 0;

	//	r_dpvol = 0;
	//	r_dpamo = 0;
	//	r_dpunp = 0;
	//
	//	r_dptol = 1;

	filling_price=0;
	vol_totalizer=0;
	filled_vol = 0;
	filled_amo = 0;

	filling_start_time = 0;
	filling_duration = 0;

//	status1_ = STATUS_UNKNOWN1;
	nozzle_state = NOZZLE_DOWN;
	state = PUMP_STATUS_UNKNOWN;


	setPriceFlag = false;
//	data_state = D_NILL;

//	setDecimalPoints(3, 2, 1, 1);
//
}

/****************************************************************************************
 * SEND_COMMAND takes in human readable commands and make an array of hex to send to the pump
 * >> com: an enum that contains the type of commands it can handle
 **************************************************************************************/
// Function to send a response


//void parse_message(unsigned char* arr, int size)
void parse_extract(void)
{
	uint16_t i,
			 j;

	header = head;
	footer = tail;

	memset(r_raw_data1, 0, sizeof(r_raw_data1));

	r_addr = MainBuf[header];
	r_pumpno = r_addr - 0x4F;			//pumpno i.e either pump 1 or 2 on
//	r_ctrl = arr[1];				//control character that specifies the type of message received
	r_ctrl = MainBuf[header + 1];

	//===  GO's TX#  ===//
	r_TX = r_ctrl & 0x0F;		//the TX of the received message (from slave) attached to the ctrl character
	// r_trans = arr[2];				//received transaction ID
	// r_lng = arr[3];					//length of data byte

	if (r_pumpno == pumpno)
	{
		for (i = 2, j = 0; i < 125; i++, j++)
		{
//			r_raw_data1[j] = arr[i];	//shift the data in the array into the r_raw_data vector
			r_raw_data1[j] = MainBuf[header + i];	//shift the data in the array into the r_raw_data vector
//			if( (arr[i] == ETX) && (arr[i+1] == SF) )
//			if( (rx_buf1[i] == ETX) && (rx_buf1[i+1] == SF) )
			if(r_raw_data1[j] == SF)
			{
				break;
			}
		}

//		r_raw_data1[j] = MainBuf[tail];

		parse_decode();
	}
//	else if (r_pumpno == pumpno2)
//	{
//		r_addr2 = r_addr;
//		r_pumpno2 = r_pumpno;
//		r_ctrl2 = r_ctrl;
//		r_TX2 = r_TX;
//
//		for (i = 2, j = 0; i < 125; i++, j++)
//		{
//			r_raw_data2[j] = MainBuf[header + i];	//shift the data in the array into the r_raw_data vector
//			if(r_raw_data2[j] == SF)
//			{
//				break;
//			}
//		}
//
//		parse_decode2();
//	}
	else
	{
//		TRACE_DART("<%s> Not for me Pump[%d]<>", __FUNCTION__, pumpno);
		resp = NOREPLY;
	}
	//include a condition to ensure that the message parsed is more than a particular value
}



////void parse_message(unsigned char* arr, int size)
//void parse_extract(void)
//{
//	uint16_t i,
//			 j;
//
////	header = head;
////	footer = tail;
//
//	memset(r_raw_data1, 0, sizeof(r_raw_data1));
//
//	r_addr = go_buff[0];   //MainBuf[header];
//	r_pumpno = r_addr - 0x4F;			//pumpno i.e either pump 1 or 2 on
////	r_ctrl = arr[1];				//control character that specifies the type of message received
//	r_ctrl = go_buff[1];   //MainBuf[header + 1];
//
//	//===  GO's TX#  ===//
//	r_TX = r_ctrl & 0x0F;		//the TX of the received message (from slave) attached to the ctrl character
//	// r_trans = arr[2];				//received transaction ID
//	// r_lng = arr[3];					//length of data byte
//
////	t_exec6 = DWT->CYCCNT;
////	t_exec7 = t_exec6 - t_exec4;
//
//	if (r_pumpno == pumpno)
//	{
//		for (i = 2, j = 0; i < 125; i++, j++)
//		{
////			r_raw_data1[j] = arr[i];	//shift the data in the array into the r_raw_data vector
//			r_raw_data1[j] = go_buff[i];   //MainBuf[header + i];	//shift the data in the array into the r_raw_data vector
////			if( (arr[i] == ETX) && (arr[i+1] == SF) )
////			if( (rx_buf1[i] == ETX) && (rx_buf1[i+1] == SF) )
//			if(r_raw_data1[j] == SF)
//			{
//				break;
//			}
//		}
//
//		t_exec8 = DWT->CYCCNT;
//		t_exec9 = t_exec8 - t_exec6;
//
////		r_raw_data1[j] = MainBuf[tail];
//
////		t_exec6 = DWT->CYCCNT;
////		t_exec7 = t_exec6 - t_exec4;
//
//		parse_decode();
//	}
//	else
//	{
////		TRACE_DART("<%s> Not for me Pump[%d]<>", __FUNCTION__, pumpno);
//		resp = NOREPLY;
//	}
//	//include a condition to ensure that the message parsed is more than a particular value
//}
//
//
//
////void parse_message(unsigned char* arr, int size)
//void parse_extract(void)
//{
//	memset(r_raw_data1, 0, sizeof(r_raw_data1));
//
////	r_addr = arr[0];
//	r_addr = rx_buf1[0];
//	r_pumpno = r_addr-0x4F;			//pumpno i.e either pump 1 or 2 on
////	r_ctrl = arr[1];				//control character that specifies the type of message received
//	r_ctrl = rx_buf1[1];
//
//	//===  GO's TX#  ===//
//	r_TX = r_ctrl & 0x0F;		//the TX of the received message (from slave) attached to the ctrl character
//	// r_trans = arr[2];				//received transaction ID
//	// r_lng = arr[3];					//length of data byte
//
//	if (r_pumpno == pumpno)
//	{
//		for (int16_t i = 2, j = 0; i < 150; i++, j++)
//		{
////			r_raw_data1[j] = arr[i];	//shift the data in the array into the r_raw_data vector
//			r_raw_data1[j] = rx_buf1[i];	//shift the data in the array into the r_raw_data vector
////			if( (arr[i] == ETX) && (arr[i+1] == SF) )
////			if( (rx_buf1[i] == ETX) && (rx_buf1[i+1] == SF) )
//			if(rx_buf1[i] == SF)
//			{
//				break;
//			}
//		}
//		parse_decode();
//	}
//	else
//	{
////		TRACE_DART("<%s> Not for me Pump[%d]<>", __FUNCTION__, pumpno);
//		resp = NOREPLY;
//	}
//	//include a condition to ensure that the message parsed is more than a particular value
//}


//function overload of the parsed message to parse ack, nack, eot messages
//void parse_message1(unsigned char* arr){
////	r_raw_data.clear();
////
////	r_addr = arr[0];
////	r_pumpno = r_addr-0x4f;	//pumpno i.e either pump one or two
////	r_ctrl = arr[1];		//control character that specifies the type of message received
////	r_TX = r_ctrl & 0x0f;	//the TX of the received message attached to the ctrl character
////
//////	TRACE_DART("<%s> %x, %x\r\n",__FUNCTION__, r_addr, r_ctrl);
////	process_data();
//}

//process_data: get the r_raw_data and process into the different flags and data
// return value(array) of data parsed, i.e. the data itself

void parse_decode(void)
{

//	int size = r_raw_data.size();
	// TRACE_DART("<%s>  r_ctrl: [%x]\n", __FUNCTION__,  r_ctrl);
	int a = 0;
	u8* r_raw_data_arr;

	uint8_t data_[50] = {0},
			data1[50] = {0},
			data2[50] = {0};

	uint16_t crc_original,
			 crc_check;

	uint8_t MSN = (r_ctrl & 0xF0);

//	switch (r_ctrl & 0xF0)
	switch (MSN)
	{

			case 0x30:	//if control character == 0x30
						// TRACE_DART("<%s>-- [reply_transaction]: %x\n",__FUNCTION__ ,  r_raw_data[0]);
						//loop through all characters in the message, find the characters of interest then move to the next set of characters

//				millis = HAL_GetTick();
//				t_exec1 = DWT->CYCCNT;
					/* do something */
//					unsigned long t2 = DWT->CYCCNT;
//					unsigned long diff = t2 - t1;

							for (int16_t i = 0; i < 150; i++)
							{
								//check the r_trans and r_lng byte ... NB: r_trans = r_raw_data[i] and r_lng = r_raw_data[i+1]
								//returns status at the request of status or change of status

								//Master's Commands to the Pump


								//'50 36 01 01 00 9f d4 03 fa '
								//===========================================================================//
								//============================= COMMAND TO PUMP =============================//
								//===========================================================================//
								if( (r_raw_data1[i] == 0x01) && (r_raw_data1[i+1] == 0x01) )  //Trans-No  & Length
								{
									command_response = true;

									//this is a Master's Command
									switch(r_raw_data1[i+2])
									{
											//for the commands, ctrl = 0x30; RETURN_PUMP_PARAM RETURN_PUMP_IDENTITY  SWITCH_OFF
										case 0x00	:	{command_ = GETSTATUS; break;}
										case 0x02  	:	{command_ = RETURN_PUMP_PARAM; break;}
										case 0x03  	:	{command_ = RETURN_PUMP_IDENTITY; break;}
										case 0x04  	:   {command_ = REQUEST_FILLING_INFO; break;}
										case 0x05 	:	{command_ = RESET1; break;}   //50 36 01 01 05 9f d4 03 fa
										case 0x06 	:	{command_ = AUTHORISE; break;}
										case 0x08 	:	{command_ = STOP; break;}
										case 0x0A 	:	{command_ = SWITCH_OFF; break;}
										default: break;
									}

									crc_original = r_raw_data1[i+4];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data1[i+3]);

										// Example GO Packet Frame
										// 	-> UART1 [9]b '51 30 01 01 04 a3 5f 03 fa '

	//								data1[0] = (r_addr / 10);
	//								data2[0] = (r_addr % 10);
	//								data1[1] = (r_ctrl / 10);
	//								data2[1] = (r_ctrl % 10);
	//
	//								for(uint8_t i = 0, j = 2; i < 5; i++, j++)
	//								{
	//									data1[j] = (r_raw_data1[i] / 10);
	//									data2[j] = (r_raw_data1[i] % 10);
	//								}
	//								for(uint8_t i = 0; i < 7; i++)
	//								{
	//									data_[i] = (data2[i] + (data1[i] * 16));
	//								}

									data_[0] = r_addr;
									data_[1] = r_ctrl;

									for(uint8_t i = 0, j = 2; i < 5; i++, j++)
									{
										data_[j] = r_raw_data1[i];
									}


									crc_check = crc_16(data_, 5);

									//////////////////////////
//									crc_check = crc_original;
									//////////////////////////

									if(crc_check == crc_original)
									{
										resp = DATA_COMMAND;
										ack_send = true;

//										millis2 = HAL_GetTick();
//										t_exec2 = DWT->CYCCNT;
									}
									else
									{
										resp = CRC_ERROR;
									}
									// TRACE_DART("<>--- msg - STATUS\n");
									i += 3; //no. of data(1) plus 2 for the r_trans and r_lng
									a++; //to count if this iteration enters any of the conditions

									dummyValue = 0;

									t_exec3 = t_exec2 - t_exec1;
									break;
								}
								//###########################################################################//
								//0x005138 02 08  00 00 00 33  00 00 04 78   03 04 00 14 50 11   1E 0C 03 FA
								//-> UART1 [12]b '50 3a 03 04 09 99 99 00 d7 2f 03 fa ' <== preset vol: 9999900
								//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||//
								//     Returns filled data and volume after requesting filling information
								//===========================================================================//
								//=============================== PRESET VOLUME =============================//
								//===========================================================================//
								else if( (r_raw_data1[i] == 0x03) && (r_raw_data1[i+1] == 0x04) ) //Trans-No  & Length
								{
									//
									uint32_t preset_voll = 0;
									uint8_t preset_vol[4];
									char preset_volll[20];

									// printf("Decimal: %llu\n", decimal);
									// return 0;
									// filled_vol = BCD2decimal(r_vol, 4, r_dpvol);
									// filled_amo = BCD2decimal(r_amo, 4, r_dpamo);

									crc_original = r_raw_data1[i+7];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data1[i+6]);

									//==============================================================//
									//==================== VALIDATING THE CRC ======================//
									data_[0] = r_addr;
									data_[1] = r_ctrl;

									for(uint8_t ii = 0, j = 2; ii < 8; ii++, j++)
									{
										data_[j] = r_raw_data1[ii];
									}


									crc_check = crc_16(data_, 8);


									if(crc_check == crc_original)
									{

										for (uint8_t j = 0; j < 4; j++)
										{
											preset_vol[j] = r_raw_data1[i+2+j];
											// r_vol[j] = r_raw_data[i+2+j];
											// r_amo[j] = r_raw_data[i+2+j+4];
											//pick the next byte as the first byte of filling price
										}

										preset_voll = (preset_vol[0] * 1000000) + (preset_vol[1] * 10000) + (preset_vol[2] * 100) + (preset_vol[3] * 1);
										sprintf(preset_volll, "0x%ld", preset_voll);

										// char num[]="0x3076";
										long n = strtol(preset_volll, NULL, 16);
										// printf("n=%ld\n", n);  // prints 12406

										   // uint32_t bcd = 0x12345678; // Example BCD value

										// uint32_t bcd = 305419896; // Example BCD value
										// uint32_t decimal = packed_bcd_to_decimal(bcd);
										uint32_t preset_vol0 = packed_bcd_to_decimal(n);

										switch(dp_vol1)
										{
											case 0	: { auth_v1 = (preset_voll * 1); break; }
											case 1	: { auth_v1 = (preset_voll * 0.1); break; }
											case 2	: { auth_v1 = (preset_voll * 0.01); break; }
											case 3	: { auth_v1 = (preset_voll * 0.001); break; }
											default	: { break; }
										}

										resp = DATA_PRESET_VOL;
										ack_send = true;
									}
									else
									{
										resp = CRC_ERROR;
									}

									//==============================================================//

									// TRACE_DART("<>--- msg - vol_amo\n");
									// TRACE_DART("raw amo:%s  raw vol:%s",r_amo,r_vol);
									//TRACE_DART("[%s] vol:%.3f dp.vol:%d amo:%.3f dp.amo:%d",__FUNCTION__,filled_vol,r_dpvol,filled_amo,r_dpvol);
									i += 6; //no. of data(8) plus 2 for the r_trans and r_lng
									a++;

									break;
								}

								//filling price and nozzle status is returned when you request status info or filling info
								//===========================================================================//
								//=============================== PRESET AMOUNT =============================//
								//===========================================================================//
								else if(r_raw_data1[i] == 0x04 && r_raw_data1[i+1] == 0x04)
								{
									uint32_t preset_amtt = 0;
									uint8_t preset_amt[4];
									char preset_amttt[20];

									crc_original = r_raw_data1[i+7];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data1[i+6]);

									data_[0] = r_addr;
									data_[1] = r_ctrl;

									for(uint8_t ii = 0, j = 2; ii < 8; ii++, j++)
									{
										data_[j] = r_raw_data1[ii];
									}


									crc_check = crc_16(data_, 8);


									if(crc_check == crc_original)
									{
										for (uint8_t j = 0; j < 4; j++)
										{
											preset_amt[j] = r_raw_data1[i+2+j];
										}

										preset_amtt = (preset_amt[0] * 1000000) + (preset_amt[1] * 10000) + (preset_amt[2] * 100) + (preset_amt[3] * 1);
										sprintf(preset_amttt, "0x%ld", preset_amtt);

										long n = strtol(preset_amttt, NULL, 16);

										uint32_t preset_amt0 = packed_bcd_to_decimal(n);

										switch(dp_amount1)
										{
											case 0	: { auth_p1 = (preset_amtt * 1); break; }
											case 1	: { auth_p1 = (preset_amtt * 0.1); break; }
											case 2	: { auth_p1 = (preset_amtt * 0.01); break; }
											case 3	: { auth_p1 = (preset_amtt * 0.001); break; }
											default	: { break; }
										}

										resp = DATA_PRESET_AMO;
										ack_send = true;
									}
									else
									{
										resp = CRC_ERROR;
									}

									i += 6; //no. of data(8) plus 2 for the r_trans and r_lng
									a++;

									millis = HAL_GetTick();
									break;
								}

								//###########################################################################//
								//'50 37 05 03 00 58 10 a2 0a 03 fa
								//===========================================================================//
								//========================== PRICE-UPDATE (DP => 1)==========================//
								//===========================================================================//
								else if(r_raw_data1[i] == 0x05 && r_raw_data1[i+1] == 0x03)
								{
									uint32_t price_updatee = 0;
									uint8_t price_update[3];
									char price_updateee[20];

									crc_original = r_raw_data1[i+6];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data1[i+5]);


									//==============================================================//
									//==================== VALIDATING THE CRC ======================//

									data_[0] = r_addr;
									data_[1] = r_ctrl;

									//'50 37 05 03 00 58 10 a2 0a 03 fa

									for(uint8_t ii = 0, j = 2; ii < 7; ii++, j++)
									{
										data_[j] = r_raw_data1[ii];
									}


									crc_check = crc_16(data_, 7);


									if(crc_check == crc_original)
									{
										//'50 37 05 03 00 58 10 a2 0a 03 fa
										for (uint8_t j = 0; j < 3; j++)
										{
											price_update[j] = r_raw_data1[i+2+j];
										}

										price_updatee = ( (price_update[0] * 10000) + (price_update[1] * 100) + (price_update[2] * 1) );
										sprintf(price_updateee, "0x%ld", price_updatee);

										// char num[]="0x3076";
										long n = strtol(price_updateee, NULL, 16);
										uint32_t price_update0 = packed_bcd_to_decimal(n);

										price_update1 = (price_updatee * 0.1);

										resp = DATA_PRICE_UPDATE;
										ack_send = true;
									}
									else
									{
										resp = CRC_ERROR;
									}
									//=================== DONE, VALIDATING THE CRC =================//
									//==============================================================//

									break;
								}

								//'50 35 65 01 01 1f 8f 03 fa '
								//===========================================================================//
								//============================== GET-TOTALIZER ==============================//
								//===========================================================================//
								else if(r_raw_data1[i] == 0x65 && r_raw_data1[i+1] == 0x01)
								{
//									uint16_t price_updatee = 0;
//									uint8_t price_update[3];
//									char price_updateee[20];

									crc_original = r_raw_data1[i+4];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data1[i+3]);


									//==============================================================//
									//==================== VALIDATING THE CRC ======================//
									//'50 35 65 01 01 1f 8f 03 fa '
									data_[0] = r_addr;
									data_[1] = r_ctrl;

									for(uint8_t ii = 0, j = 2; ii < 5; ii++, j++)
									{
										data_[j] = r_raw_data1[ii];
									}


									crc_check = crc_16(data_, 5);


									if(crc_check == crc_original)
									{
										command_ = REQUEST_VOL_TOTAL_COUNT;

										resp = DATA_REQUEST_VOL_TOTAL_COUNT;

										command_response = true;

										ack_send = true;
									}
									else
									{
										resp = CRC_ERROR;
									}
									//=================== DONE, VALIDATING THE CRC =================//
									//==============================================================//

									break;
								}

								//'50 35 65 01 01 1f 8f 03 fa '
								//===========================================================================//
								//============================== SUSPEND REQUEST ==============================//
								//===========================================================================//
								else if( (r_raw_data1[i] == 0x0E) && (r_raw_data1[i+1] == 0x01) )
								{
//									uint16_t price_updatee = 0;
//									uint8_t price_update[3];
//									char price_updateee[20];

									crc_original = r_raw_data1[i+4];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data1[i+3]);


									//==============================================================//
									//==================== VALIDATING THE CRC ======================//
									//'50 35 65 01 01 1f 8f 03 fa '
									data_[0] = r_addr;
									data_[1] = r_ctrl;

									for(uint8_t ii = 0, j = 2; ii < 5; ii++, j++)
									{
										data_[j] = r_raw_data1[ii];
									}


									crc_check = crc_16(data_, 5);


									if(crc_check == crc_original)
									{
										command_ = SUSPEND_REQUEST;

										resp = DATA_SUSPEND;

										command_response = true;

										ack_send = true;
									}
									else
									{
										resp = CRC_ERROR;
									}
									//=================== DONE, VALIDATING THE CRC =================//
									//==============================================================//

									break;
								}
								//'50 37 05 03 00 58 10 a2 0a 03 fa
								//===========================================================================//
								//==========================   SET PUMP PARAMETERS   ========================//
								//===========================================================================//
								else if(r_raw_data1[i] == 0x09)   // && r_raw_data1[i+1] == 0x33) //51 Data Bytes
								{
									uint32_t price_updatee = 0;
									uint8_t set_param[3];
									char price_updateee[20];

									crc_original = r_raw_data1[i+56];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data1[i+55]);


									//==============================================================//
									//==================== VALIDATING THE CRC ======================//

									data_[0] = r_addr;
									data_[1] = r_ctrl;

									////////////////////////////////////////
									//'50 37 05 03 00 58 10 a2 0a 03 fa
									////////////////////////////////////////
									//===the read buffer contains only from the transaction byte to the SF byte ===//
									//================== Data contains a cumulative of 51 raw Bytes ===============//
									for(uint8_t ii = 0, j = 2; ii < 53; ii++, j++)  //51 + Trans No. + Data Len
									{
										data_[j] = r_raw_data1[ii];
									}


									crc_check = crc_16(data_, 55);


									if(crc_check == crc_original)
									{
										for (uint8_t j = 0; j < 51; j++)
										{
											set_param[j] = r_raw_data1[i+2+j];
										}

										resp = DATA_SET_PUMP_PARAM;
										ack_send = true;
									}
									else
									{
										resp = CRC_ERROR;
									}
									//=================== DONE, VALIDATING THE CRC =================//
									//==============================================================//

									break;
								}



							}

							break;

			case 0x20	:	resp = r_POLL;
							// TRACE_DART("<>--- msg - poll\n");

//							t_exec6 = DWT->CYCCNT;
//							t_exec7 = t_exec6 - t_exec4;

							break;

			case 0xC0	: 	resp = r_ACK;
							// TRACE_DART("<>--- msg - ack\n");
							MSN = (r_ctrl & 0x0F);
							if(MSN == TX)
//							if(r_ctrl == 0x32)
							{
								checked = 1;
//							}
								TX++;
								if(TX > 0x0F)
								{
									TX = 0x00;
								}
							}

							command_ = NO_COMMAND;

							command_response = false;

							resp = NOREPLY;

							break;//increase the tx on receiving an ACK

			case 0x50	:	resp = r_NACK;
							// TRACE_DART("<>--- msg - nack\n");
							MSN = (r_ctrl & 0x0F);
							if(MSN == TX)
							{

							}

//							TX = 0;
							break;

			case 0x70	:	resp = r_EOT;
							// TRACE_DART("<>--- msg - eot\n");
							break;
			default		:
							resp = JUNK;
							break;
		//if (TX >= 0x0f) TX = 0;
		//r_raw_data.size();
	}//switch ctrl
}


void parse_decode2(void)
{

//	int size = r_raw_data.size();
	// TRACE_DART("<%s>  r_ctrl: [%x]\n", __FUNCTION__,  r_ctrl);
	int a = 0;
	u8* r_raw_data_arr;

	uint8_t data_[50] = {0},
			data1[50] = {0},
			data2[50] = {0};

	uint16_t crc_original,
			 crc_check;

	uint8_t MSN = (r_ctrl2 & 0xF0);

//	switch (r_ctrl & 0xF0)
	switch (MSN)
	{

			case 0x30:	//if control character == 0x30

							for (int16_t i = 0; i < 150; i++)
							{
								//check the r_trans and r_lng byte ... NB: r_trans = r_raw_data[i] and r_lng = r_raw_data[i+1]
								//returns status at the request of status or change of status

								//Master's Commands to the Pump


								//'50 36 01 01 00 9f d4 03 fa '
								//===========================================================================//
								//============================= COMMAND TO PUMP =============================//
								//===========================================================================//
								if( (r_raw_data2[i] == 0x01) && (r_raw_data2[i+1] == 0x01) )  //Trans-No  & Length
								{
									//this is a Master's Command
									switch(r_raw_data2[i+2])
									{
											//for the commands, ctrl = 0x30; RETURN_PUMP_PARAM RETURN_PUMP_IDENTITY  SWITCH_OFF
										case 0x00	:	{command_2 = GETSTATUS; break;}
										case 0x02  	:	{command_2 = RETURN_PUMP_PARAM; break;}
										case 0x03  	:	{command_2 = RETURN_PUMP_IDENTITY; break;}
										case 0x04  	:   {command_2 = REQUEST_FILLING_INFO; break;}
										case 0x05 	:	{command_2 = RESET1; break;}
										case 0x06 	:	{command_2 = AUTHORISE; break;}
										case 0x08 	:	{command_2 = STOP; break;}
										case 0x0A 	:	{command_2 = SWITCH_OFF; break;}
										default: break;
									}

									crc_original = r_raw_data2[i+4];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data2[i+3]);

									// Example GO Packet Frame
									// 	-> UART1 [9]b '51 30 01 01 04 a3 5f 03 fa '

									data_[0] = r_addr2;
									data_[1] = r_ctrl2;

									for(uint8_t i = 0, j = 2; i < 5; i++, j++)
									{
										data_[j] = r_raw_data2[i];
									}


									crc_check = crc_16(data_, 5);


									if(crc_check == crc_original)
									{
										resp2 = DATA_COMMAND;
										ack_send2 = true;
									}
									else
									{
										resp2 = CRC_ERROR;
									}

									dummyValue = 0;

									t_exec3 = t_exec2 - t_exec1;
									break;
								}

								//0x005138 02 08  00 00 00 33  00 00 04 78   03 04 00 14 50 11   1E 0C 03 FA

								//Returns filled data and volume after requesting filling information
								//===========================================================================//
								//=============================== PRESET VOLUME =============================//
								//===========================================================================//
								else if( (r_raw_data2[i] == 0x03) && (r_raw_data2[i+1] == 0x04) ) //Trans-No  & Length
								{
									//
									uint16_t preset_voll = 0;
									uint8_t preset_vol[4];
									char preset_volll[20];

									crc_original = r_raw_data2[i+7];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data2[i+6]);

									//==============================================================//
									//==================== VALIDATING THE CRC ======================//
									data_[0] = r_addr2;
									data_[1] = r_ctrl2;

									for(uint8_t ii = 0, j = 2; ii < 8; ii++, j++)
									{
										data_[j] = r_raw_data2[ii];
									}


									crc_check = crc_16(data_, 8);


									if(crc_check == crc_original)
									{

										for (uint8_t j = 0; j < 4; j++)
										{
											preset_vol[j] = r_raw_data2[i+2+j];
										}

										preset_voll = (preset_vol[0] * 1000000) + (preset_vol[1] * 10000) + (preset_vol[2] * 100) + (preset_vol[3] * 1);
										sprintf(preset_volll, "0x%ld", preset_voll);

										long n = strtol(preset_volll, NULL, 16);
										uint32_t preset_vol0 = packed_bcd_to_decimal(n);


										resp2 = DATA_PRESET_VOL;
										ack_send2 = true;
									}
									else
									{
										resp2 = CRC_ERROR;
									}

									//==============================================================//

									break;
								}

								//filling price and nozzle status is returned when you request status info or filling info
								//===========================================================================//
								//=============================== PRESET AMOUNT =============================//
								//===========================================================================//
								else if(r_raw_data2[i] == 0x04 && r_raw_data2[i+1] == 0x04)
								{
									uint16_t preset_amtt = 0;
									uint8_t preset_amt[4];
									char preset_amttt[20];

									crc_original = r_raw_data2[i+7];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data2[i+6]);

									data_[0] = r_addr2;
									data_[1] = r_ctrl2;

									for(uint8_t ii = 0, j = 2; ii < 8; ii++, j++)
									{
										data_[j] = r_raw_data2[ii];
									}


									crc_check = crc_16(data_, 8);


									if(crc_check == crc_original)
									{
										for (uint8_t j = 0; j < 4; j++)
										{
											preset_amt[j] = r_raw_data2[i+2+j];
										}

										preset_amtt = (preset_amt[0] * 1000000) + (preset_amt[1] * 10000) + (preset_amt[2] * 100) + (preset_amt[3] * 1);
										sprintf(preset_amttt, "0x%ld", preset_amtt);

										long n = strtol(preset_amttt, NULL, 16);

										uint32_t preset_amt0 = packed_bcd_to_decimal(n);


										resp2 = DATA_PRESET_AMO;
										ack_send2 = true;
									}
									else
									{
										resp2 = CRC_ERROR;
									}
									break;
								}
								//'50 37 05 03 00 58 10 a2 0a 03 fa
								//===========================================================================//
								//========================== PRICE-UPDATE (DP => 1)==========================//
								//===========================================================================//
								else if(r_raw_data2[i] == 0x05 && r_raw_data2[i+1] == 0x03)
								{
									uint16_t price_updatee = 0;
									uint8_t price_update[3];
									char price_updateee[20];

									crc_original = r_raw_data2[i+6];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data2[i+5]);


									//==============================================================//
									//==================== VALIDATING THE CRC ======================//

									data_[0] = r_addr2;
									data_[1] = r_ctrl2;

									//'50 37 05 03 00 58 10 a2 0a 03 fa

									for(uint8_t ii = 0, j = 2; ii < 7; ii++, j++)
									{
										data_[j] = r_raw_data2[ii];
									}


									crc_check = crc_16(data_, 7);


									if(crc_check == crc_original)
									{
										//'50 37 05 03 00 58 10 a2 0a 03 fa
										for (uint8_t j = 0; j < 3; j++)
										{
											price_update[j] = r_raw_data2[i+2+j];
										}

										price_updatee = ( (price_update[0] * 10000) + (price_update[1] * 100) + (price_update[2] * 1) );
										sprintf(price_updateee, "0x%ld", price_updatee);

										// char num[]="0x3076";
										long n = strtol(price_updateee, NULL, 16);
										uint32_t price_update0 = packed_bcd_to_decimal(n);


										resp2 = DATA_PRICE_UPDATE;
										ack_send2 = true;
									}
									else
									{
										resp2 = CRC_ERROR;
									}
									//=================== DONE, VALIDATING THE CRC =================//
									//==============================================================//

									break;
								}

								//'50 35 65 01 01 1f 8f 03 fa '
								//===========================================================================//
								//============================== GET-TOTALIZER ==============================//
								//===========================================================================//
								else if(r_raw_data2[i] == 0x65 && r_raw_data2[i+1] == 0x01)
								{
									uint16_t price_updatee = 0;
									uint8_t price_update[3];
									char price_updateee[20];

									crc_original = r_raw_data2[i+4];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data2[i+3]);


									//==============================================================//
									//==================== VALIDATING THE CRC ======================//
									//'50 35 65 01 01 1f 8f 03 fa '
									data_[0] = r_addr2;
									data_[1] = r_ctrl2;

									for(uint8_t ii = 0, j = 2; ii < 5; ii++, j++)
									{
										data_[j] = r_raw_data2[ii];
									}


									crc_check = crc_16(data_, 5);


									if(crc_check == crc_original)
									{
										command_2 = REQUEST_VOL_TOTAL_COUNT;

										resp2 = DATA_REQUEST_VOL_TOTAL_COUNT;
										ack_send2 = true;
									}
									else
									{
										resp2 = CRC_ERROR;
									}
									//=================== DONE, VALIDATING THE CRC =================//
									//==============================================================//

									break;
								}


							}

							break;

			case 0x20	:	resp2 = r_POLL;

							break;

			case 0xC0	: 	resp2 = r_ACK;
							// TRACE_DART("<>--- msg - ack\n");
							MSN = (r_ctrl & 0x0F);
							if(MSN == TX2)
							{
								checked = 1;
								TX2++;
							}

							command_2 = NO_COMMAND;

							command_response2 = false;

							resp2 = NOREPLY;

							break; //increase the tx on receiving an ACK

			case 0x50	:	resp2 = r_NACK;
							MSN = (r_ctrl & 0x0F);
							if(MSN == TX2)
							{

							}

//							TX = 0;
							break;

			case 0x70	:	resp2 = r_EOT;
							// TRACE_DART("<>--- msg - eot\n");
							break;
			default		:
							resp2 = JUNK;
							break;
	}//switch ctrl
}


void process_response(response_enum response)
{

//	unsigned char status_;
	uint16_t crc;

	addr = 0x4F + pumpno;    //00H -> FFH
	// TX &= 0x0F;
	TX = TX & 0x0F;
	// ctrl = (TX & 0x0F);
	ctrl = TX;

//	if(response == DATA_REQUEST_VOL_TOTAL_COUNT)
//	{
//		dummyValue = 0;
//	}
	if (response == CRC_ERROR)
	{
		ctrl = 0x50 | r_TX;

		memset(DART_BUFF1, 0, sizeof(DART_BUFF1));

		DART_BUFF1[0] = addr;
		DART_BUFF1[1] = ctrl;
		DART_BUFF1[2] = SF;

		array_len = 3;

	}

	//=== Slave Responding to Master's Poll ===//
	else if (response == r_POLL || response == r_ACK || response == r_NACK || response == r_ACKPOLL || response == r_EOT)
	{
		memset(DART_BUFF1, 0, sizeof(DART_BUFF1));
		DART_BUFF1[0] = addr;

		switch (response)
		{
			//fOR THE BASIC ctrls

			case r_POLL:   // 20H

							//If having nothing to send, send EOT

//							ctrl = 0x20;
//							if(pump_status_ == STATUS_PNP)
//							{
//								pump_unprogrammed = 1;
//							}
//							_process_response(DATA_COMMAND);


							if(command_response == false)
							{
//								uint8_t status_change = 1;  // Nozzle-change
								if(status_change_noz1 == 1)
								{
									if(nozzle_out1 == true)
									{
										//Param : uint8_t buff_index, float filling_price, uint8_t nozzle_status
										send_nozzleStatus(0);  //, 180.00, 1);   //nozStatus = 0, //0 -> in, 1 -> out
										nozzlezUp1 = 1;    //retains this nozzleup memory for some later use
									}
									else if(nozzle_out1 == false){
										send_nozzleStatus(0);  //, 180.00, 0);   //nozStatus = 0, //0 -> in, 1 -> out
										nozzlezUp1 = 0;  //clears this nozzleup memory @ any slightest nozzledown
									}

									crc = crc_16(DART_BUFF1, 8);
									DART_BUFF1[8] = crc & 0x00FF;
									DART_BUFF1[9] = crc >> 8;
									DART_BUFF1[10] = ETX;
									DART_BUFF1[11] = SF;

									array_len = 12;

									status_change_noz1 = 0;
								}
								else if(status_change_pump1 == 1)   //Generic Pump-Status-Change
								{
									send_pumpStatus(0);


									crc = crc_16(DART_BUFF1, 5);
									DART_BUFF1[5] = crc & 0x00FF;
									DART_BUFF1[6] = crc >> 8;
									DART_BUFF1[7] = ETX;
									DART_BUFF1[8] = SF;

									array_len = 9;

									status_change_pump1 = 0;
								}
								//==========================================================//
								//==========  If having nothing to send, send EOT ==========//
								//==========================================================//
								else
								{
									ctrl = 0x70;    //EOT

									DART_BUFF1[1] = ctrl;
									DART_BUFF1[2] = SF;

									array_len = 3;
								}

								go_write(DART_BUFF1);
							}
							else if( (command_ == GETSTATUS) || (command_ == REQUEST_FILLING_INFO)
									|| (command_ == RETURN_PUMP_PARAM) || (command_ == RETURN_PUMP_IDENTITY)  )
							{
//								t_exec2 = DWT->CYCCNT;
//								t_exec3 = t_exec2 - t_exec1;

								_process_response(DATA_COMMAND);

								t_exec2 = DWT->CYCCNT;
								t_exec3 = t_exec2 - t_exec1;

								go_write(DART_BUFF1);

								t_exec4 = DWT->CYCCNT;
								t_exec5 = t_exec4 - t_exec2;
							}
							else if(command_ == REQUEST_VOL_TOTAL_COUNT)
							{
								_process_response(DATA_REQUEST_VOL_TOTAL_COUNT);

								go_write(DART_BUFF1);
							}
							break;
			case r_ACK:   //C0H -> CFH

//							ctrl = 0xC0 | r_TX;	//reply with the previous msg tx
							resp = NOREPLY;
							break;

			case r_NACK:   //50H -> 5FH
							ctrl = 0x50 | TX;
							_process_response(DATA_COMMAND);
							go_write(DART_BUFF1);
							break;
			case r_ACKPOLL:    //E0H -> EFH
							ctrl = 0xE0 | r_TX;
							break;
			case r_EOT:   // 70H -> 7FH
							ctrl = 0x70 | r_TX;
							break;
			default:
							break;
		}

//		if (response != r_NACK)
//		{
//			DART_BUFF1[1] = ctrl;
//			DART_BUFF1[2] = SF;
//
//			array_len = 3;
//		}
	}
	else
	{
		_process_response(response);
		go_write(DART_BUFF1);
	}
}

//	else if(response == DATA_COMMAND)   //This transaction is sent by the pump if the status is changed or if the pump receives the command 'RETURN STATUS’.
//	{
////		if (command_ == PUMP_NOT_PROGRAMMED || command_ == RESET0 || command_ == AUTHORIZED || command_ == FILLING || command_ == FILLING_COMPLETED || command_ == MAX_AMOUNTVOLUME_REACHED || command_ == SWITCHED_OFF)
//
//
//		//===================================================================//
//		//   This transaction is sent by the pump if the status is changed   //
//		//   or if the pump receives the command 'RETURN STATUS’.            //
//		//===================================================================//
//
//		if (command_ == GETSTATUS)
//		{
//			//---- Handles command for sending of data to controller -----//
//
//			ctrl |= 0x30 ;
//			trans = 0x01;
//			lng = 0x01;
//			memset(DART_BUFF1, 0, sizeof(DART_BUFF1));
//
//			DART_BUFF1[0] = addr;
//			DART_BUFF1[1] = ctrl;
//			DART_BUFF1[2] = trans;
//			DART_BUFF1[3] = lng;
//
//			pump_status_ = STATUS_RESET;
//
//			switch (pump_status_)
//			{
//				//for the Pump-Status Commands
//				case STATUS_PNP		      				:	{status_ = 0x00; break;}
//				case STATUS_RESET 						:	{status_ = 0x01; break;}
//				case STATUS_AUTH 						:	{status_ = 0x02; break;}
//				case STATUS_FILLING						:	{status_ = 0x04; break;}
//				case STATUS_FILLING_COMP				:	{status_ = 0x05; break;}
//				case STATUS_MAMO_REACHED				:	{status_ = 0x06; break;}   //MAX_AMOUNTVOLUME_REACHED
//				case STATUS_SWITCHED_OFF				:	{status_ = 0x07; break;}
//				default									: 	break;
//			}
//
//			DART_BUFF1[4] = status_;
//	//		uint16_t crc = calculate_crc(DART_BUFF1, 5);
//			crc = crc_16(DART_BUFF1, 5);
//			DART_BUFF1[5] = crc & 0x00FF;
//			DART_BUFF1[6] = crc >> 8;
//			DART_BUFF1[7] = ETX;
//			DART_BUFF1[8] = SF;
//
//			array_len = 9;
//		}
//
//		//=============================================================================//
//		//        This transaction is sent by the pump at change of a value            //
//		//        or if the pump receives the command RETURN FILLING INFORMATION       //
//		//=============================================================================//
//		else if (command_ == REQUEST_FILLING_INFO)        //FILLED_VOLUME_AND_AMOUNT
//		{
//
//			float vol_ = 1550.1234;
//			float amo_ = 15.1234;
//			int decimalPlaces = 2;
//			double roundedNum;
//			int num_;
//			// unsigned int bcd;
//			unsigned char bcd_[10] = {0};  // Array to hold the BCD result
//
//			uint16_t crc;
//
//			ctrl |= 0x30 ;
//			trans = 0x02;
//			lng = 0x01;
//
//			memset(DART_BUFF1, 0, sizeof(DART_BUFF1));
//
//			DART_BUFF1[0] = addr;
//			DART_BUFF1[1] = ctrl;
//			DART_BUFF1[2] = trans;
//			DART_BUFF1[3] = lng;
//
//			roundedNum = roundUp(vol_, decimalPlaces);
//			roundedNum = roundedNum * 100;
//			num_ = (int)(roundedNum);
//
//			int_to_bcd(num_, bcd_);
//
//			// for(uint8_t i = 0; i < 4; i++)
//			for (uint8_t i = 0, j = 3; i < 4; i++, j--)
//			{
//				DART_BUFF1[i + 4] = bcd_[j];
//			}
//
//			roundedNum = roundUp(amo_, decimalPlaces);
//			roundedNum = roundedNum * 100;
//			num_ = (int)(roundedNum);
//
//			memset(bcd_, 0, sizeof(bcd_));
//
//			int_to_bcd(num_, bcd_);
//
//			// for(uint8_t i = 0; i < 4; i++)
//			for (uint8_t i = 0, j = 3; i < 4; i++, j--)
//			{
//				DART_BUFF1[i + 8] = bcd_[j];
//			}
//
//	//		crc = calculate_crc(DART_BUFF1, 12);
//			crc = crc_16(DART_BUFF1, 12);
//			DART_BUFF1[12] = crc & 0x00FF;
//			DART_BUFF1[13] = crc >> 8;
//			DART_BUFF1[14] = ETX;
//			DART_BUFF1[15] = SF;
//
//			array_len = 16;
//		}
//
//		//=============================================================================//
//		//   	  This transaction is sent by the pump if the status is changed        //
//		//		  or if the pump receives the command                                  //
//		//		  'RETURN STATUS' or ‘RETURN FILLING INFORMATION’.      			   //
//		//=============================================================================//
//
//		else if ( (command_ == GETSTATUS) || (command_ == REQUEST_FILLING_INFO) )        // NOZSTATUS_AND_FILLINGPRICE
//		{
//
//			float fillingPrice = 1550.1234;
//			int decimalPlaces = 2;
//			double roundedNum;
//			int num_;
//			unsigned char bcd_[10] = {0};  // Array to hold the BCD result
//
//			uint16_t crc;
//
//			ctrl |= 0x30 ;
//			trans = 0x01;
//			lng = 0x01;
//
//			memset(DART_BUFF1, 0, sizeof(DART_BUFF1));
//
//			DART_BUFF1[0] = addr;
//			DART_BUFF1[1] = ctrl;
//			DART_BUFF1[2] = trans;
//			DART_BUFF1[3] = lng;
//
//			roundedNum = roundUp(fillingPrice, decimalPlaces);
//			roundedNum = roundedNum * 100;
//			num_ = (int)(roundedNum);
//
//			int_to_bcd(num_, bcd_);
//
//			for (uint8_t i = 0, j = 2;  i < 3; i++, j--)
//			{
//				DART_BUFF1[i + 4] = bcd_[j];
//			}
//
//			uint8_t nozNum, nozStatus, nozIO;
//
//			// nozIO = nozNum;
//			nozStatus = (nozStatus << 4);
//			nozIO = nozNum | nozStatus;
//
//	//		crc = calculate_crc(DART_BUFF1, 7);
//			crc = crc_16(DART_BUFF1, 7);
//			DART_BUFF1[7] = crc & 0x00FF;
//			DART_BUFF1[8] = crc >> 8;
//			DART_BUFF1[9] = ETX;
//			DART_BUFF1[10] = SF;
//
//			array_len = 11;
//		}
//
//		else if (command_ = RETURN_PUMP_IDENTITY)       //PUMP_IDENTITY)
//		{
//			float fillingPrice = 1550.1234;
//			int decimalPlaces = 2;
//			double roundedNum;
//			int num_, pump_id;
//			unsigned char bcd_[10] = {0};  // Array to hold the BCD result
//
//			uint16_t crc;
//
//			ctrl |= 0x30 ;
//			trans = 0x01;
//			lng = 0x01;
//
//			memset(DART_BUFF1, 0, sizeof(DART_BUFF1));
//
//			DART_BUFF1[0] = addr;
//			DART_BUFF1[1] = ctrl;
//			DART_BUFF1[2] = trans;
//			DART_BUFF1[3] = lng;
//
//			roundedNum = roundUp(fillingPrice, decimalPlaces);
//			roundedNum = roundedNum * 100;
//			num_ = pump_id;
//
//			int_to_bcd(num_, bcd_);
//
//			for (uint8_t i = 0, j = 4;  i < 5; i++, j--)
//			{
//				DART_BUFF1[i + 4] = bcd_[j];
//			}
//
//	//		crc = calculate_crc(DART_BUFF1, 9);
//			crc = crc_16(DART_BUFF1, 9);
//			DART_BUFF1[9] = crc & 0x00FF;
//			DART_BUFF1[10] = crc >> 8;
//			DART_BUFF1[11] = ETX;
//			DART_BUFF1[12] = SF;
//
//			array_len = 13;
//			}
//	}

//	go_write(DART_BUFF1);
//}


void process_response2(response_enum response)
{
	addr2 = 0x4F + pumpno2;    //00H -> FFH
	TX2 = TX2 & 0x0F;
	// ctrl = (TX & 0x0F);
	ctrl = TX2;
	if (response == CRC_ERROR)
	{
		ctrl = 0x50 | r_TX;

		memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

		DART_BUFF2[0] = addr2;
		DART_BUFF2[1] = ctrl;
		DART_BUFF2[2] = SF;

		array_len2 = 3;

	}

	//=== Slave Responding to Master's Poll ===//
	else if (response == r_POLL || response == r_ACK || response == r_NACK || response == r_ACKPOLL || response == r_EOT)
	{
		memset(DART_BUFF2, 0, sizeof(DART_BUFF2));
		DART_BUFF2[0] = addr2;

		switch (response)
		{
			//fOR THE BASIC ctrls

			case r_POLL:   // 20H

				            //==========================================================//
							//==========  If having nothing to send, send EOT ==========//
							//==========================================================//
							if(command_2 == NO_COMMAND)
							{
								ctrl = 0x70;    //EOT

								DART_BUFF2[1] = ctrl;
								DART_BUFF2[2] = SF;

								array_len2 = 3;

								go_write2(DART_BUFF2);
							}
							else if(command_2 != NO_COMMAND)
							{
								_process_response2(DATA_COMMAND);

								go_write(DART_BUFF2);
							}
							break;
			case r_ACK:   //C0H -> CFH

//							ctrl = 0xC0 | r_TX;	//reply with the previous msg tx
							resp = NOREPLY;
							break;

			case r_NACK:   //50H -> 5FH
							ctrl = 0x50 | TX;
							_process_response2(DATA_COMMAND);
							go_write2(DART_BUFF2);
							break;
			case r_ACKPOLL:    //E0H -> EFH
							ctrl = 0xE0 | r_TX;
							break;
			case r_EOT:   // 70H -> 7FH
							ctrl = 0x70 | r_TX;
							break;
			default:
							break;
		}

	}
	else
	{
		_process_response2(response);
		go_write2(DART_BUFF2);
	}
}


void _process_response(response_enum response)
{

	unsigned char status_;
	uint16_t crc;

	static uint8_t check = 0;

	if(response == DATA_COMMAND)   //This transaction is sent by the pump if the status is changed or if the pump receives the command 'RETURN STATUS’.
//		|| (outstanding_command == true) )
	{

		if(ack_send == true)
		{
			//==> send ack
			send_acknowledgement(ACK);
//			outstanding_command = true;
			ack_send = false;


			switch (command_)
			{
				//for the Pump-Status Commands
				case RESET1		      :
										{
											command_response = false;
											if(settings_stream1[0].mode == AUTO_MODE)
											{
												if( (pump_status_ == STATUS_FILLING_COMP) || (pump_status_ == STATUS_MAMO_REACHED) || (pump_status_ == STATUS_SWITCHED_OFF) )
												{
//																		pump_status_ = STATUS_RESET;

													reset_flag1 = 1;

													//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[

													//Amount, Vol., and Alarm cleared
													//Light switched on
													//Preset-Vol Cleared to default value
													//Display cleared

													//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
												}
											}
											break;
										}
				case AUTHORISE	 	:
										{
											command_response = false;
											if(settings_stream1[0].mode == AUTO_MODE)
											{
												if(pump_status_ == STATUS_RESET)
												{
//																		pump_status_ = STATUS_AUTH;
													auth_cmd_flag = 1;  //activate auth cmd.
												}
											}
											break;
										}
				case STOP1		 	:
										{
											command_response = false;
											if(settings_stream1[0].mode == AUTO_MODE)
											{
												if((pump_status_ == STATUS_RESET) || (pump_status_ == STATUS_SUSPENDED) ||
												   (pump_status_ == STATUS_MAMO_REACHED) || (pump_status_ == STATUS_AUTH) ||
												   (pump_status_ == STATUS_FILLING) || (pump_status_ == STATUS_SWITCHED_OFF) )
												{
//													pump_status_ = STATUS_AUTH;
													stop_flag = 1;  //deactivate auth cmd.
												}
											}
											break;
										}
				case SWITCH_OFF		:
										{
											command_response = false;
											if(settings_stream1[0].mode == AUTO_MODE)
											{
//												pump_status_ = STATUS_SWITCHED_OFF;
												//Lights off
												//Motor off
												//Idle State
											}
											break;
										}
				case SUSPEND_REQUEST  :
										{
											command_response = false;
											if(settings_stream1[0].mode == AUTO_MODE)
											{
												if(pump_status_ == STATUS_AUTH)
												{
//													pump_status_ = STATUS_SUSPENDED;

													authsuspend_flag1 = 1;
													//Motor turned off
												}
												else if(pump_status_ == STATUS_FILLING)
												{
													//Motor turned off
													fillingsuspend_flag1 = 1;
												}
											}
											break;
										}
				case RESUME_REQUEST	  :
										{
											command_response = false;
											if(settings_stream1[0].mode == AUTO_MODE)
											{
//												if(pump_status_ == STATUS_SUSPENDED)
												if(authsuspend_flag1 == 1)
												{
													authresume_flag1 = 1;
													//Motor turned on

													authsuspend_flag1 = 0;
												}
												else if(fillingsuspend_flag1 == 1)
												{
													fillingresume_flag1 = 1;
													//Motor turned on

													fillingsuspend_flag1 = 0;
												}
											}
											break;
										}
				case PRESET_VOL							:
				case PRESET_AMO							:	{command_response = false; break;}
				default									: 	break;
			}

//			STATUS_UNKNOWN,    //used for idle state by the Main
//
//			STATUS_PNP,
//			STATUS_RESET,
//			STATUS_AUTH,
//			STATUS_FILLING,
//			STATUS_FILLING_COMP,
//			STATUS_SUSPENDED
//			STATUS_MAMO_REACHED,
//			STATUS_SWITCHED_OFF

//			if (command_ == RESET1)
//			{
//				RESET1,
//				AUTHORISE,
//				STOP1,
//				SWITCH_OFF,
//				REQUEST_FILLING_INFO,
//				RETURN_PUMP_PARAM,
//				RETURN_PUMP_IDENTITY,
//				SUSPEND_REQUEST,
//				RESUME_REQUEST,
//				NOZSTATUS_AND_FILLINGPRICE,
//
//				PUMP_IDENTITY,
//
//				PRESET_VOL,	//Set the respective volume of these before using them
//				PRESET_AMO,
//
//				FILLED_VOLUME_AND_AMOUNT,
//
//				PRICE_UPDATE,
//				SET_MAMO,
//				REQUEST_VOL_TOTAL_COUNT,
//				NO_COMMAND
//
//				command_ = NO_COMMAND;
//			}

		}
		else
		{

			//===================================================================//
			//   This transaction is sent by the pump if the status is changed   //
			//   or if the pump receives the command 'RETURN STATUS’.            //
			//===================================================================//

			if (command_ == GETSTATUS)
			{

				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
				//////////////////////////////////////////  GET PUMP-STATUS ///////////////////////////////////////
				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//

				uint8_t decimalPlaces;
				double roundedNum;
				int num_;

				unsigned char bcd_[10] = {0};  // Array to hold the BCD result

				uint16_t crc;

				ctrl = TX;
				ctrl |= 0x30;

				memset(DART_BUFF1, 0, sizeof(DART_BUFF1));

				DART_BUFF1[0] = addr;
				DART_BUFF1[1] = ctrl;


				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
				////////////////////////////////////////  DC1 ////////////////////////////////////////
				/////////////////////////////////// GET PUMP-STATUS //////////////////////////////////


				trans = 0x01;
				lng = 0x01;

				DART_BUFF1[2] = trans;
				DART_BUFF1[3] = lng;

				//==============================//
//				pump_status_ = STATUS_RESET;
				//=============================//


				switch (pump_status_)
				{
					//for the Pump-Status Commands
					case STATUS_PNP		      				:	{status_ = 0x00; break;}
					case STATUS_RESET 						:	{status_ = 0x01; break;}
					case STATUS_AUTH 						:	{status_ = 0x02; break;}
					case STATUS_FILLING						:	{status_ = 0x04; break;}
					case STATUS_FILLING_COMP				:	{status_ = 0x05; break;}
					case STATUS_MAMO_REACHED				:	{status_ = 0x06; break;}   //MAX_AMOUNTVOLUME_REACHED
					case STATUS_SWITCHED_OFF				:	{status_ = 0x07; break;}
					default									: 	break;
				}

				DART_BUFF1[4] = status_;


				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
				//////////////////////////////////  GET NOZSTATUS-AND-FILLINGPRICE //////////////////////////////////

				//=============================================================================//
				//////////////////////////////////  DC3  ////////////////////////////////////////
				//   	  This transaction is sent by the pump if the status is changed        //
				//		  or if the pump receives the command                                  //
				//		  'RETURN STATUS' or ‘RETURN FILLING INFORMATION’.      			   //
				//=============================================================================//

				//command_ ==> REQUEST_FILLING_INFO        // NOZSTATUS_AND_FILLINGPRICE

//				float fillingPrice = 100.00;
				float fillingPrice = go_fillingPrice1();

				trans = 0x03;
				lng = 0x04;

				DART_BUFF1[5] = trans;
				DART_BUFF1[6] = lng;

				decimalPlaces = 1;

				roundedNum = roundUp(fillingPrice, decimalPlaces);
				roundedNum = roundedNum * 10;
				num_ = (int)(roundedNum);

				memset(bcd_, 0, sizeof(bcd_));

				int_to_bcd(num_, bcd_);

				for (uint8_t i = 0, j = 2;  i < 3; i++, j--)
				{
					DART_BUFF1[i + 7] = bcd_[j];
				}

				uint8_t nozNum = 1,
						nozStatus = 0, //0 -> in, 1 -> out
						nozIO;

				if(nozzle_out1 == true) nozStatus = 1;
				else nozStatus = 0;

				// nozIO = nozNum;
				nozStatus = (nozStatus << 4);
				nozIO = nozNum | nozStatus;

				DART_BUFF1[10] = nozIO;    //Nozzle 1


				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

				crc = crc_16(DART_BUFF1, 11);
				DART_BUFF1[11] = crc & 0x00FF;
				DART_BUFF1[12] = crc >> 8;
				DART_BUFF1[13] = ETX;
				DART_BUFF1[14] = SF;

				array_len = 15;

				resp = NOREPLY;

			}

			//=============================================================================//
			//        This transaction is sent by the pump at change of a value            //
			//        or if the pump receives the command RETURN FILLING INFORMATION       //
			//=============================================================================//
			else if (command_ == REQUEST_FILLING_INFO)        //FILLED_VOLUME_AND_AMOUNT
			{

				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
				//////////////////////////////////  GET FILLED_VOLUME_AND_AMOUNT //////////////////////////////////



				//=============================================================================//
				////////////////////////////////////  DC2  //////////////////////////////////////
				//        This transaction is sent by the pump at change of a value            //
				//        or if the pump receives the command RETURN FILLING INFORMATION       //
				//=============================================================================//
				//command_ ==> REQUEST_FILLING_INFO        //FILLED_VOLUME_AND_AMOUNT

//				float vol_ = 15.1234;
				float vol_ = go_fillingInfo_vol1();
//				float amo_ = 1500.1234;
				float amo_ = go_fillingInfo_amt1();

				uint8_t decimalPlaces;
				double roundedNum;
				int num_;
				// unsigned int bcd;
				unsigned char bcd_[10] = {0};  // Array to hold the BCD result

				uint16_t crc;

				ctrl = TX;
				ctrl |= 0x30;
				trans = 0x02;
				lng = 0x08;

				memset(DART_BUFF1, 0, sizeof(DART_BUFF1));

				DART_BUFF1[0] = addr;
				DART_BUFF1[1] = ctrl;
				DART_BUFF1[2] = trans;
				DART_BUFF1[3] = lng;

				decimalPlaces = 2;

				roundedNum = roundUp(vol_, decimalPlaces);
				roundedNum = roundedNum * 100;
				num_ = (int)(roundedNum);

				int_to_bcd(num_, bcd_);

				for (uint8_t i = 0, j = 3; i < 4; i++, j--)
				{
					DART_BUFF1[i + 4] = bcd_[j];
				}

				decimalPlaces = 1;

				roundedNum = roundUp(amo_, decimalPlaces);
				roundedNum = roundedNum * 10;
				num_ = (int)(roundedNum);

				memset(bcd_, 0, sizeof(bcd_));

				int_to_bcd(num_, bcd_);

				// for(uint8_t i = 0; i < 4; i++)
				for (uint8_t i = 0, j = 3; i < 4; i++, j--)
				{
					DART_BUFF1[i + 8] = bcd_[j];
				}


				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
				//////////////////////////////////  GET NOZSTATUS_AND_FILLINGPRICE //////////////////////////////////

				//=============================================================================//
				//////////////////////////////////  DC3  ////////////////////////////////////////
				//   	  This transaction is sent by the pump if the status is changed        //
				//		  or if the pump receives the command                                  //
				//		  'RETURN STATUS' or ‘RETURN FILLING INFORMATION’.      			   //
				//=============================================================================//

				//command_ ==> REQUEST_FILLING_INFO        // NOZSTATUS_AND_FILLINGPRICE

//				float fillingPrice = 100.00;
				float fillingPrice = go_fillingPrice1();

				trans = 0x03;
				lng = 0x04;

				DART_BUFF1[12] = trans;
				DART_BUFF1[13] = lng;

				decimalPlaces = 1;

				roundedNum = roundUp(fillingPrice, decimalPlaces);
				roundedNum = roundedNum * 10;
				num_ = (int)(roundedNum);

				memset(bcd_, 0, sizeof(bcd_));

				int_to_bcd(num_, bcd_);

				for (uint8_t i = 0, j = 2;  i < 3; i++, j--)
				{
					DART_BUFF1[i + 14] = bcd_[j];
				}

				uint8_t nozNum = 1,
						nozStatus = 0, //0 -> in, 1 -> out
						nozIO;

				if(nozzle_out1 == true) nozStatus = 1;
				else nozStatus = 0;

				// nozIO = nozNum;
				nozStatus = (nozStatus << 4);
				nozIO = nozNum | nozStatus;

				DART_BUFF1[17] = nozIO;    //Nozzle 1


				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


				crc = crc_16(DART_BUFF1, 18);
				DART_BUFF1[18] = crc & 0x00FF;
				DART_BUFF1[19] = crc >> 8;
				DART_BUFF1[20] = ETX;
				DART_BUFF1[21] = SF;

				array_len = 22;
			}


			else if (command_ = RETURN_PUMP_IDENTITY)       //PUMP_IDENTITY)
			{
//				float fillingPrice = 1550.1234;
//				int decimalPlaces = 2;
//				double roundedNum;
				int num_, pump_id;
				unsigned char bcd_[10] = {0};  // Array to hold the BCD result

				uint16_t crc;

				ctrl |= 0x30 ;
				trans = 0x09;
				lng = 0x05;

				memset(DART_BUFF1, 0, sizeof(DART_BUFF1));

				DART_BUFF1[0] = addr;
				DART_BUFF1[1] = ctrl;
				DART_BUFF1[2] = trans;
				DART_BUFF1[3] = lng;

//				roundedNum = roundUp(fillingPrice, decimalPlaces);
//				roundedNum = roundedNum * 100;
				num_ = pump_id;

				int_to_bcd(num_, bcd_);

				for (uint8_t i = 0, j = 4;  i < 5; i++, j--)
				{
					DART_BUFF1[i + 4] = bcd_[j];
				}

			//		crc = calculate_crc(DART_BUFF1, 9);
				crc = crc_16(DART_BUFF1, 9);
				DART_BUFF1[9] = crc & 0x00FF;
				DART_BUFF1[10] = crc >> 8;
				DART_BUFF1[11] = ETX;
				DART_BUFF1[12] = SF;

				array_len = 13;
			}

			//=============================================================================//
			//   	  This transaction is sent by the pump if SET PUMP PARAMETERS          //
			//		  is received or if the pump receives the command                      //
			//		   'RETURN PUMP PARAMETERS'.      			                           //
			//=============================================================================//
			else if (command_ = RETURN_PUMP_PARAM)       //PUMP_IDENTITY)
			{
				if(ack_send == true)
				{
					//==> send ack
					send_acknowledgement(ACK);
					ack_send = false;
				}
				else
				{

					uint8_t dp_vol = dp_vol1;
					uint8_t dp_amo = dp_amount1;
					uint8_t dp_unp = dp_unitprice1;

					int pump_limit = 1000;  //1000 Litres

					int num_, pump_id;
					unsigned char bcd_[10] = {0};  // Array to hold the BCD result

					uint16_t crc;

					ctrl |= 0x30 ;
					trans = 0x07;
					lng = 0x33;

					memset(DART_BUFF1, 0, sizeof(DART_BUFF1));

					DART_BUFF1[0] = addr;
					DART_BUFF1[1] = ctrl;
					DART_BUFF1[2] = trans;
					DART_BUFF1[3] = lng;


					for (uint8_t i = 0;  i < 22; i++)
					{
						DART_BUFF1[i + 4] = 0;
					}

					DART_BUFF1[26] = dp_vol;
					DART_BUFF1[27] = dp_amo;
					DART_BUFF1[28] = dp_unp;

					for (uint8_t i = 0;  i < 5; i++)
					{
						DART_BUFF1[i + 29] = 0;
					}

					int_to_bcd(pump_limit, bcd_);

					for (uint8_t i = 0, j = 4;  i < 5; i++, j--)
					{
						DART_BUFF1[i + 34] = bcd_[j];
					}

					for (uint8_t i = 0;  i < 2; i++)
					{
						DART_BUFF1[i + 38] = 0;
					}

					for (uint8_t i = 0;  i < 2; i++)
					{
						DART_BUFF1[i + 38] = 0;
					}

					for (uint8_t i = 0;  i < 15; i++)
					{
						DART_BUFF1[i + 40] = 0;
					}

					crc = crc_16(DART_BUFF1, 55);
					DART_BUFF1[55] = crc & 0x00FF;
					DART_BUFF1[56] = crc >> 8;
					DART_BUFF1[57] = ETX;
					DART_BUFF1[58] = SF;

					array_len = 59;

					resp = NOREPLY;
				}
			}
		}
	}
	else if(response == DATA_PRICE_UPDATE)   //This transaction is sent by the pump if the status is changed or if the pump receives the command 'RETURN STATUS’.
	{
		send_acknowledgement(ACK);
		ack_send = false;

		///////////////////////////////////////////////////////////////////
		///////////////////  ACTUATE THE CHANGE... ////////////////////////

		float priceChange_check = price_update1 - settings_stream1[0].price_;

		if (settings0_stream1[0].mode == AUTO_MODE)
		{

			if( (priceChange_check > 0.1) || (priceChange_check < -0.1) )
			{
				   changeLitrePrice1 = 1;
			}

			if(pump_status_ == STATUS_PNP)
			{
//				pump_status_ = STATUS_FILLING_COMP;
				fillingComplete_flag1 = 1;
			}
		}
		///////////////////////////////////////////////////////////////////
	}
	else if(response == DATA_REQUEST_VOL_TOTAL_COUNT)   //This transaction is sent by the pump if the status is changed or if the pump receives the command 'RETURN STATUS’.
	{
		if(ack_send == true)
		{
			//==> send ack
			send_acknowledgement(ACK);
			ack_send = false;
		}
		else
		{


			//'50 38 65 10 01 00 00 18 03 67 00 00 18 03 67 00 00 00 00 00 59 ab 03 fa '

			//=============================================================================//
			//       	 This transaction is sent by the pump at a Request of              //
			//        				 "REQUEST VOLUME TOTAL COUNTERS"                       //
			//=============================================================================//
			//command_ ==> REQUEST_VOL_TOTAL_COUNT;      //REQUEST TOTALIZER   DP ==> 3

//			float tot_vol = 1500.1234;
			float tot_vol = totaliser_vol1c;
//			float tot_vol1 = 1500.1234;
			float tot_vol1 = tot_vol;
			float tot_vol2 = 0.0000;
			uint8_t decimalPlaces;
			uint8_t noz_id = 1;
			double roundedNum;
			int num_;

			unsigned char bcd_[6] = {0};  // Array to hold the BCD result

			uint16_t crc;

			ctrl = 0x03;
			ctrl |= 0x30;
			trans = 0x65;
			lng = 0x10;

			memset(DART_BUFF1, 0, sizeof(DART_BUFF1));

			DART_BUFF1[0] = addr;
			DART_BUFF1[1] = ctrl;
			DART_BUFF1[2] = trans;
			DART_BUFF1[3] = lng;
			DART_BUFF1[4] = noz_id;

			//'50 38 65 10 01 00 00 18 03 67 00 00 18 03 67 00 00 00 00 00 59 ab 03 fa '

			decimalPlaces = 3;

			roundedNum = roundUp(tot_vol, decimalPlaces);
			roundedNum = roundedNum * 1000;
			num_ = (int)(roundedNum);

			int_to_bcd(num_, bcd_);

			for (uint8_t ii = 0, j = 4; ii < 5; ii++, j--)   //j=>4 : MSB in it, LSB is in j=0
			{
				DART_BUFF1[ii + 5] = bcd_[j];
			}

			roundedNum = roundUp(tot_vol1, decimalPlaces);
			roundedNum = roundedNum * 1000;
			num_ = (int)(roundedNum);

			memset(bcd_, 0, sizeof(bcd_));

			int_to_bcd(num_, bcd_);

			for (uint8_t ii = 0, j = 4; ii < 5; ii++, j--)
			{
				DART_BUFF1[ii + 10] = bcd_[j];
			}

			roundedNum = roundUp(tot_vol2, decimalPlaces);
			roundedNum = roundedNum * 1000;
			num_ = (int)(roundedNum);

			memset(bcd_, 0, sizeof(bcd_));

			int_to_bcd(num_, bcd_);

			for (uint8_t ii = 0, j = 0; ii < 5; ii++, j--)
			{
				DART_BUFF1[ii + 15] = bcd_[j];
			}

			//'50 38 65 10 01 00 00 18 03 67 00 00 18 03 67 00 00 00 00 00 59 ab 03 fa '

			crc = crc_16(DART_BUFF1, 20);
			DART_BUFF1[20] = crc & 0x00FF;
			DART_BUFF1[21] = crc >> 8;
			DART_BUFF1[22] = ETX;
			DART_BUFF1[23] = SF;

			array_len = 24;

			resp = NOREPLY;

		}
	}
	else if( (response == DATA_PRESET_VOL) || (response == DATA_PRESET_AMO)  )
	{
		send_acknowledgement(ACK);
		ack_send = false;

		//Effect the Change...
		 if(response == DATA_PRESET_VOL)
		 {
			 change_v1 = 1;
		 }
		 else if(response == DATA_PRESET_AMO)
		 {
			 change_p1 = 1;
		 }
	}
	else if(response == DATA_SUSPEND)
	{
		send_acknowledgement(ACK);
		ack_send = false;

		command_response = false;

		if(settings_stream1[0].mode == AUTO_MODE)
		{
			if((pump_status_ == STATUS_FILLING) || (pump_status_ == STATUS_AUTH) )
			{
//				pump_status_ = STATUS_SUSPENDED;
				//Motor turned off

				//Effect the Change...
			}
		}
	}
	else if(response == DATA_RESUME)
	{
		send_acknowledgement(ACK);
		ack_send = false;

		command_response = false;
		if(settings_stream1[0].mode == AUTO_MODE)
		{
			if(pump_status_ == STATUS_SUSPENDED)
			{
				//Motor turned on

				//Effect the Change...
			}
		}
	}
}


void _process_response2(response_enum response)
{

	unsigned char status_;
	uint16_t crc;

	static uint8_t check = 0;

	if(response == DATA_COMMAND)   //This transaction is sent by the pump if the status is changed or if the pump receives the command 'RETURN STATUS’.
//		|| (outstanding_command == true) )
	{

		if(ack_send2 == true)
		{
			//==> send ack
			send_acknowledgement2(ACK);
			ack_send2 = false;
		}
		else
		{

			//===================================================================//
			//   This transaction is sent by the pump if the status is changed   //
			//   or if the pump receives the command 'RETURN STATUS’.            //
			//===================================================================//

			if (command_2 == GETSTATUS)
			{
				//=============================================================================//
				//        This transaction is sent by the pump at change of a value            //
				//        or if the pump receives the command RETURN FILLING INFORMATION       //
				//=============================================================================//
				//command_ ==> REQUEST_FILLING_INFO        //FILLED_VOLUME_AND_AMOUNT

				float vol_ = 15.1234;
				float amo_ = 1500.1234;
				int decimalPlaces;
				double roundedNum;
				int num_;
				// unsigned int bcd;
				unsigned char bcd_[10] = {0};  // Array to hold the BCD result

				uint16_t crc;

				ctrl = 0x02;
				ctrl |= 0x30;
				trans = 0x02;
				lng = 0x08;

				memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

				DART_BUFF2[0] = addr2;
				DART_BUFF2[1] = ctrl;
				DART_BUFF2[2] = trans;
				DART_BUFF2[3] = lng;

				decimalPlaces = 2;

				roundedNum = roundUp(vol_, decimalPlaces);
				roundedNum = roundedNum * 100;
				num_ = (int)(roundedNum);

				int_to_bcd(num_, bcd_);

				// for(uint8_t i = 0; i < 4; i++)
				for (uint8_t i = 0, j = 3; i < 4; i++, j--)
				{
					DART_BUFF2[i + 4] = bcd_[j];
				}

				decimalPlaces = 1;

				roundedNum = roundUp(amo_, decimalPlaces);
				roundedNum = roundedNum * 10;
				num_ = (int)(roundedNum);

				memset(bcd_, 0, sizeof(bcd_));

				int_to_bcd(num_, bcd_);

				// for(uint8_t i = 0; i < 4; i++)
				for (uint8_t i = 0, j = 3; i < 4; i++, j--)
				{
					DART_BUFF2[i + 8] = bcd_[j];
				}


				//=============================================================================//
				//   	  This transaction is sent by the pump if the status is changed        //
				//		  or if the pump receives the command                                  //
				//		  'RETURN STATUS' or ‘RETURN FILLING INFORMATION’.      			   //
				//=============================================================================//

				//command_ ==> REQUEST_FILLING_INFO        // NOZSTATUS_AND_FILLINGPRICE

				float fillingPrice = 100.00;
	//			int decimalPlaces = 2;
	//			double roundedNum;
	//			int num_;
	//			unsigned char bcd_[10] = {0};  // Array to hold the BCD result

				trans = 0x03;
				lng = 0x04;

				DART_BUFF2[12] = trans;
				DART_BUFF2[13] = lng;

				decimalPlaces = 1;

				roundedNum = roundUp(fillingPrice, decimalPlaces);
				roundedNum = roundedNum * 10;
				num_ = (int)(roundedNum);

				memset(bcd_, 0, sizeof(bcd_));

				int_to_bcd(num_, bcd_);

				for (uint8_t i = 0, j = 2;  i < 3; i++, j--)
				{
					DART_BUFF2[i + 14] = bcd_[j];
				}

				uint8_t nozNum = 1,
						nozStatus = 0, //0 -> in, 1 -> out
						nozIO;

				// nozIO = nozNum;
				nozStatus = (nozStatus << 4);
				nozIO = nozNum | nozStatus;

				DART_BUFF2[17] = nozIO;    //Nozzle 1

				trans = 0x01;
				lng = 0x01;

				DART_BUFF2[18] = trans;
				DART_BUFF2[19] = lng;

				//==============================//
//				pump_status_2 = STATUS_RESET;
				//=============================//

				switch (pump_status_2)
				{
					//for the Pump-Status Commands
					case STATUS_PNP		      				:	{status_ = 0x00; break;}
					case STATUS_RESET 						:	{status_ = 0x01; break;}
					case STATUS_AUTH 						:	{status_ = 0x02; break;}
					case STATUS_FILLING						:	{status_ = 0x04; break;}
					case STATUS_FILLING_COMP				:	{status_ = 0x05; break;}
					case STATUS_MAMO_REACHED				:	{status_ = 0x06; break;}   //MAX_AMOUNTVOLUME_REACHED
					case STATUS_SWITCHED_OFF				:	{status_ = 0x07; break;}
					default									: 	break;
				}

				DART_BUFF2[20] = status_;

				crc = crc_16(DART_BUFF2, 21);
				DART_BUFF2[21] = crc & 0x00FF;
				DART_BUFF2[22] = crc >> 8;
				DART_BUFF2[23] = ETX;
				DART_BUFF2[24] = SF;

				array_len2 = 25;

				resp = NOREPLY;

			}

			//=============================================================================//
			//        This transaction is sent by the pump at change of a value            //
			//        or if the pump receives the command RETURN FILLING INFORMATION       //
			//=============================================================================//
			else if (command_ == REQUEST_FILLING_INFO)        //FILLED_VOLUME_AND_AMOUNT
			{

				float vol_ = 1500.1234;
				float amo_ = 15.1234;
				int decimalPlaces = 2;
				double roundedNum;
				int num_;
				// unsigned int bcd;
				unsigned char bcd_[10] = {0};  // Array to hold the BCD result

				uint16_t crc;

				ctrl |= 0x30 ;
				trans = 0x02;
				lng = 0x08;

				memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

				DART_BUFF2[0] = addr2;
				DART_BUFF2[1] = ctrl;
				DART_BUFF2[2] = trans;
				DART_BUFF2[3] = lng;

				roundedNum = roundUp(vol_, decimalPlaces);
				roundedNum = roundedNum * 100;
				num_ = (int)(roundedNum);

				int_to_bcd(num_, bcd_);

				// for(uint8_t i = 0; i < 4; i++)
				for (uint8_t i = 0, j = 3; i < 4; i++, j--)
				{
					DART_BUFF2[i + 4] = bcd_[j];
				}

				roundedNum = roundUp(amo_, decimalPlaces);
				roundedNum = roundedNum * 100;
				num_ = (int)(roundedNum);

				memset(bcd_, 0, sizeof(bcd_));

				int_to_bcd(num_, bcd_);

				// for(uint8_t i = 0; i < 4; i++)
				for (uint8_t i = 0, j = 3; i < 4; i++, j--)
				{
					DART_BUFF2[i + 8] = bcd_[j];
				}

			//		crc = calculate_crc(DART_BUFF1, 12);
				crc = crc_16(DART_BUFF2, 12);
				DART_BUFF2[12] = crc & 0x00FF;
				DART_BUFF2[13] = crc >> 8;
				DART_BUFF2[14] = ETX;
				DART_BUFF2[15] = SF;

				array_len2 = 16;
			}

			//=============================================================================//
			//   	  This transaction is sent by the pump if the status is changed        //
			//		  or if the pump receives the command                                  //
			//		  'RETURN STATUS' or ‘RETURN FILLING INFORMATION’.      			   //
			//=============================================================================//

			else if ( (command_2 == GETSTATUS) || (command_2 == REQUEST_FILLING_INFO) )        // NOZSTATUS_AND_FILLINGPRICE
			{

				float fillingPrice = 100.00;
				int decimalPlaces = 2;
				double roundedNum;
				int num_;
				unsigned char bcd_[10] = {0};  // Array to hold the BCD result

				uint16_t crc;

				ctrl |= 0x30 ;
				trans = 0x01;
				lng = 0x01;

				memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

				DART_BUFF2[0] = addr2;
				DART_BUFF2[1] = ctrl;
				DART_BUFF2[2] = trans;
				DART_BUFF2[3] = lng;

				roundedNum = roundUp(fillingPrice, decimalPlaces);
				roundedNum = roundedNum * 100;
				num_ = (int)(roundedNum);

				int_to_bcd(num_, bcd_);

				for (uint8_t i = 0, j = 2;  i < 3; i++, j--)
				{
					DART_BUFF2[i + 4] = bcd_[j];
				}

				uint8_t nozNum, nozStatus, nozIO;

				// nozIO = nozNum;
				nozStatus = (nozStatus << 4);
				nozIO = nozNum | nozStatus;

			//		crc = calculate_crc(DART_BUFF1, 7);
				crc = crc_16(DART_BUFF2, 7);
				DART_BUFF2[7] = crc & 0x00FF;
				DART_BUFF2[8] = crc >> 8;
				DART_BUFF2[9] = ETX;
				DART_BUFF2[10] = SF;

				array_len2 = 11;
			}

			else if (command_2 = RETURN_PUMP_IDENTITY)       //PUMP_IDENTITY)
			{
//				float fillingPrice = 1550.1234;
//				int decimalPlaces = 2;
//				double roundedNum;
				int num_, pump_id;
				unsigned char bcd_[6] = {0};  // Array to hold the BCD result

				uint16_t crc;

				ctrl |= 0x30 ;
				trans = 0x01;
				lng = 0x05;

				memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

				DART_BUFF2[0] = addr2;
				DART_BUFF2[1] = ctrl;
				DART_BUFF2[2] = trans;
				DART_BUFF2[3] = lng;

				num_ = pump_id;

				int_to_bcd(num_, bcd_);

				for (uint8_t i = 0, j = 4;  i < 5; i++, j--)
				{
					DART_BUFF2[i + 4] = bcd_[j];
				}

			//		crc = calculate_crc(DART_BUFF1, 9);
				crc = crc_16(DART_BUFF2, 9);
				DART_BUFF2[9] = crc & 0x00FF;
				DART_BUFF2[10] = crc >> 8;
				DART_BUFF2[11] = ETX;
				DART_BUFF2[12] = SF;

				array_len2 = 13;
				}
		}
	}
	else if(response == DATA_PRICE_UPDATE)   //This transaction is sent by the pump if the status is changed or if the pump receives the command 'RETURN STATUS’.
	{
		send_acknowledgement2(ACK);
		ack_send2 = false;

		//Effect the Change...

		pump_status_2 = STATUS_RESET;   //if after Power-on
	}
	else if(response == DATA_REQUEST_VOL_TOTAL_COUNT)   //This transaction is sent by the pump if the status is changed or if the pump receives the command 'RETURN STATUS’.
	{
		if(ack_send2 == true)
		{
			//==> send ack
			send_acknowledgement2(ACK);
			ack_send2 = false;
		}
		else
		{


			//'50 38 65 10 01 00 00 18 03 67 00 00 18 03 67 00 00 00 00 00 59 ab 03 fa '

			//=============================================================================//
			//        This transaction is sent by the pump at change of a value            //
			//        or if the pump receives the command RETURN FILLING INFORMATION       //
			//=============================================================================//
			//command_ ==> REQUEST_VOL_TOTAL_COUNT;      //REQUEST TOTALIZER

			float tot_vol = 1500.1234;
			float tot_vol1 = 1500.1234;
			float tot_vol2 = 0.0000;
			uint8_t decimalPlaces;
			uint8_t noz_id = 1;
			double roundedNum;
			int num_;

			unsigned char bcd_[6] = {0};  // Array to hold the BCD result

			uint16_t crc;

			ctrl = 0x03;
			ctrl |= 0x30;
			trans = 0x65;
			lng = 0x10;

			memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

			DART_BUFF2[0] = addr2;
			DART_BUFF2[1] = ctrl;
			DART_BUFF2[2] = trans;
			DART_BUFF2[3] = lng;
			DART_BUFF2[4] = noz_id;

			//'50 38 65 10 01 00 00 18 03 67 00 00 18 03 67 00 00 00 00 00 59 ab 03 fa '

			decimalPlaces = 3;

			roundedNum = roundUp(tot_vol, decimalPlaces);
			roundedNum = roundedNum * 1000;
			num_ = (int)(roundedNum);

			int_to_bcd(num_, bcd_);

			for (uint8_t ii = 0, j = 3; ii < 5; ii++, j--)
			{
				DART_BUFF2[ii + 5] = bcd_[j];
			}

			roundedNum = roundUp(tot_vol1, decimalPlaces);
			roundedNum = roundedNum * 1000;
			num_ = (int)(roundedNum);

			memset(bcd_, 0, sizeof(bcd_));

			int_to_bcd(num_, bcd_);

			for (uint8_t ii = 0, j = 3; ii < 5; ii++, j--)
			{
				DART_BUFF2[ii + 10] = bcd_[j];
			}

			roundedNum = roundUp(tot_vol2, decimalPlaces);
			roundedNum = roundedNum * 1000;
			num_ = (int)(roundedNum);

			memset(bcd_, 0, sizeof(bcd_));

			int_to_bcd(num_, bcd_);

			for (uint8_t ii = 0, j = 3; ii < 5; ii++, j--)
			{
				DART_BUFF2[ii + 15] = bcd_[j];
			}

			//'50 38 65 10 01 00 00 18 03 67 00 00 18 03 67 00 00 00 00 00 59 ab 03 fa '

			crc = crc_16(DART_BUFF2, 20);
			DART_BUFF2[20] = crc & 0x00FF;
			DART_BUFF2[21] = crc >> 8;
			DART_BUFF2[22] = ETX;
			DART_BUFF2[23] = SF;

			array_len2 = 24;

			resp = NOREPLY;

		}
	}
}





void send_acknowledgement(response_enum response)
{

	memset(DART_BUFF1, 0, sizeof(DART_BUFF1));
	DART_BUFF1[0] = addr;

	switch (response)
	{
		//fOR THE BASIC ctrls

		case ACK:   //C0H -> CFH
						ctrl = 0xC0 | r_TX;	//reply with the previous msg tx
						break;

		case NACK:   //50H -> 5FH
						ctrl = 0x50 | r_TX;
						break;

		case ACKPOLL:    //E0H -> EFH
						ctrl = 0xE0 | r_TX;
						break;

		case EOT:   // 70H -> 7FH
						ctrl = 0x70 | r_TX;
//						ctrl = 0x70;
						break;
		default:
						break;
	}

	DART_BUFF1[1] = ctrl;
	DART_BUFF1[2] = SF;

	array_len = 3;
}


void send_acknowledgement2(response_enum response)
{

	memset(DART_BUFF2, 0, sizeof(DART_BUFF2));
	DART_BUFF2[0] = addr2;

	switch (response)
	{
		//fOR THE BASIC ctrls

		case ACK:   //C0H -> CFH
						ctrl = 0xC0 | r_TX;	//reply with the previous msg tx
						break;

		case NACK:   //50H -> 5FH
						ctrl = 0x50 | r_TX;
						break;

		case ACKPOLL:    //E0H -> EFH
						ctrl = 0xE0 | r_TX;
						break;

		case EOT:   // 70H -> 7FH
						ctrl = 0x70 | r_TX;
//						ctrl = 0x70;
						break;
		default:
						break;
	}

	DART_BUFF2[1] = ctrl;
	DART_BUFF2[2] = SF;

	array_len2 = 3;
}


void go_write(uint8_t* write_array)
{
	uint8_t id = 0;

//	HAL_UART_Abort_IT(&huart2);
//	huart2.RxXferCount = pump_rx_bufsize;
//	huart2.pRxBuffPtr = &uart2_rx_buf[0];
//	HAL_Delay(1);
//	HAL_UART_Receive_IT(&huart2, uart2_rx_buf, pump_rx_bufsize);
//	HAL_Delay(10);

	//=======================================================================
//
//		  memset(sndd, 0, sizeof(sndd));
//		  sprintf(sndd, "((((((|1|0|%d%s))\r\n", id, write_string);

//		  HAL_UART_Transmit (&huart2, DART_BUFF1, array_len, 1000);

		  HAL_UART_Transmit_DMA(&huart2, DART_BUFF1, array_len);
//		  id++;

//		  t_exec1 = DWT->CYCCNT;
//		  Uart_sendstring(DART_BUFF1, array_len);
//		  t_exec2 = DWT->CYCCNT;
//
//		  t_exec3 = t_exec2 - t_exec1;

		  cheq2++;

		  id++;

	//=======================================================================
}


void go_write2(uint8_t* write_array)
{
	uint8_t id = 0;

	//=======================================================================

		  HAL_UART_Transmit_DMA(&huart2, DART_BUFF2, array_len2);

	//=======================================================================
}



/***********************************************************************
 *
 *Functions common to all protocols
 ***********************************************************************/

void cmd_update_price(float price){
//	price_update = price * powr(10, r_dpunp);
//	send_command(PRICE_UPDATE);
////	TRACE_DART("<cmd: %s> dpp: %d, price_update = %d", __FUNCTION__, r_dpunp, price_update); //this shows the price to update to in int with decimal(float) to BCD
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
}

void cmd_switch_off(){
//	send_command(SWITCH_OFF);
////	TRACE_DART("<cmd: %s> SWITCH OFF pump %d", __FUNCTION__, pumpno);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
}

void cmd_reset(){
//	send_command(RESET);
////	TRACE_DART("<cmd: %s>  pump %d", __FUNCTION__, pumpno);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
}

void cmd_get_decimal_points(){
//	send_command(RETURN_PUMP_PARAM);
////	TRACE_DART("<cmd: %s> %d dpv, %d dpa, %d dpp", __FUNCTION__,r_dpvol, r_dpamo, r_dpunp);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
}

void cmd_get_totalizer(){
//	send_command(REQUEST_VOL_TOTAL_COUNT);
////	TRACE_DART("<----------cmd: %s> totalizer: %lf", __FUNCTION__ ,vol_totalizer);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
}

void cmd_get_filling_info(){
//	send_command(REQUEST_FILLING_INFO);
////	TRACE_DART("<cmd: %s> filling price: %f", __FUNCTION__, filling_price);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
	//returns nozzle state as well
}

void cmd_get_status(){
//	send_command(GETSTATUS);
////	TRACE_DART("<cmd: %s> GET_STATUS pump %d| tz: %f, pl: %f, %d dpv", __FUNCTION__, pumpno, vol_totalizer, filling_price, r_dpvol);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
}

void cmd_set_vol(float vol){
//	preset_vol = vol * powr(10, r_dpvol);
//	send_command(PRESET_VOL);
////	TRACE_DART("<cmd: %s> preset vol: %d", __FUNCTION__ , preset_vol);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
}

void cmd_set_amo(float amo){
//	preset_amo = amo * powr(10, r_dpamo);
//	send_command(PRESET_AMO);
////	TRACE_DART("<cmd: %s> preset amo: %d", __FUNCTION__ , preset_amo);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
}

void cmd_authorise(){
//	send_command(AUTHORISE);
////	TRACE_DART("<cmd: %s>", __FUNCTION__);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
}

void cmd_stop(){
//	send_command(STOP);
////	TRACE_DART("<cmd: %s>", __FUNCTION__);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
}

void cmd_poll(){
//	send_command(POLL);
////	TRACE_DART("<cmd: %s>", __FUNCTION__);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
}

void cmd_ack(){
//	send_command(ACK);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size, 30);
}

void cmd_nack(){
//	send_command(NACK);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
}

//combine status and nozzle state to get a single high level status
pump_state_enum getStateInput(){

	//nozzle out or nozzle up == 0x11
	//	if (nozzle_num && (nozzle_num != (r_nozzle & 0x0f)) ) return 0;
	//	if (!nozzle_num || (nozzle_num == (r_nozzle & 0x0f)) )
	//	{
	//		//evaluate nozzle state if 1. nozzle_num is not set, 2. if nozzle_num is set and equals the bit 0-3 of r_nozzle
	//		if ((r_nozzle & 0xf0) == 0x00)
	//			nozzle_state = NOZZLE_DOWN;
	//		else if((r_nozzle & 0xf0) == 0x10)
	//			nozzle_state = NOZZLE_UP;
	//	}

	//HIGH LEVEL status for interacting with other Modules
	//nozzle in or nozzle down == 0x01
//	if (status == STATUS_UNKNOWN) state = PUMP_STATUS_UNKNOWN;
//
//	else if ((status == STATUS_PNP) && nozzle_state == NOZZLE_DOWN) //status == STATUS_PNP ||
//		state = PUMP_NOT_ACTIVE;
//	else if ((status == STATUS_PNP) && nozzle_state == NOZZLE_UP)	//status == STATUS_PNP ||
//		state = PUMP_NOT_ACTIVE;
//
//	else if ((status == STATUS_RESET) && nozzle_state == NOZZLE_DOWN) //status == STATUS_PNP ||
//		state = NOZZLE_HANG_DOWN;
//	else if ((status == STATUS_RESET) && nozzle_state == NOZZLE_UP)	//status == STATUS_PNP ||
//		state = NOZZLE_HANG_UP;
//
//	else if (status == STATUS_AUTH && nozzle_state == NOZZLE_DOWN)
//		state = PUMP_AUTH_NOZZLE_HANG_DOWN;
//	else if (status == STATUS_AUTH && nozzle_state == NOZZLE_UP)
//		state = PUMP_AUTH_NOZZLE_HANG_UP;
//
//	else if (status == STATUS_FILLING && nozzle_state == NOZZLE_DOWN)
//		state = PUMP_FILLING;
//	else if (status == STATUS_FILLING && nozzle_state == NOZZLE_UP)
//		state = PUMP_FILLING;
//
//	else if (status == STATUS_MAMO_REACHED && nozzle_state == NOZZLE_DOWN)
//		state = PUMP_FILLED_LIMIT;
//	else if (status == STATUS_MAMO_REACHED && nozzle_state == NOZZLE_UP)
//		state = PUMP_FILLED_LIMIT;
//
//	else if (status == STATUS_FILLING_COMP && nozzle_state == NOZZLE_DOWN)
//		state = PUMP_FILL_COMP_NOZZLE_HANG_DOWN;
//	else if (status == STATUS_FILLING_COMP && nozzle_state == NOZZLE_UP)
//		state = PUMP_FILL_COMP_NOZZLE_HANG_UP;
//
//	else if ((status == STATUS_SWITCHED_OFF))	//status == STATUS_PNP ||
//		state = PUMP_SWITCHED_OFF;
//
//	return state;
}


/* This function handles responses from the pump that requires regular reply */
//tempted to use a recursion here, for instances in a situtaion where GO's message is nack
int handle_resp(command_enum cmd){
	//	s;
	int cnt = 4;
	int nack_cnt = 3;
	int no_etx_cnt = 3;
	int no_crc_cnt = 3;
	int junk_cnt = 3;
	int ret = -99;
	bool leave_loop = false;

//	TRACE_DART("<%s> resp: %d", __FUNCTION__, resp);
	//send these commands based on the reply from the main command
	//retry this command as long as

	while (!leave_loop){
		//RESP is updated after calling each of the cmd below
//		TRACE_DART("<%s> [while loop] resp: %d, cnt: %d, nack: %d, crc_cnt: %d, etx_cnt: %d", __FUNCTION__, resp, cnt, nack_cnt, no_crc_cnt, no_etx_cnt);
		switch(resp){
		case r_ACK:
			cmd_poll();
			ret = 0;
			break;
		case r_NACK:		//this is suppose to resend the last message up to three times
			if (--nack_cnt > 0){
				send_command(cmd);
				send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
			}
			else{
				cmd_poll();
			}
			ret = -10;
			break;
		case NO_ETX:
			if (--no_etx_cnt>0)
				cmd_nack();
			else{
				cmd_ack();
				cmd_poll();
			}
			ret = -1;
			break;

		case CRC_ERROR:
			if (--no_crc_cnt>0){
				cmd_nack();
			}
			else{
				cmd_ack();
				cmd_poll();
			}
			ret = -2;
			break;

		case JUNK:
			if (--junk_cnt > 0)
				cmd_nack();
			else{
				cmd_ack();
				cmd_poll();
			}
			ret = -3;
			break;
		case NOREPLY:
			cmd_poll();
			--cnt;
			break;
		case r_EOT:
			leave_loop = true;
			TRACE_DART("<%s> EOT !", __FUNCTION__);
			break;
		default:		// if it is any other response i.e. data response to command
			cmd_ack();
			//			cmd_poll();
			break;
		}

		if(cnt == 0){
			leave_loop = true;
			TRACE_DART("<%s> NORELY x 3!", __FUNCTION__);
		}
	}

	if (cnt == 0){	//handle any other unfortunate error that refuses to be fixed
		cmd_ack();
		cmd_poll();
		//		if (resp == CRC_ERROR || resp == NO_ETX){
		//
		//		}
	}


	if (resp != NOREPLY){	//setting time in order to get the no resp range
		setResponseTime();
	}

	return ret;
}

/***************************************************************************
 *
 * implementing the BaseProtocol virtual functions
 *
 ***************************************************************************/

/* main program API
 * These ones interact with the global variables only
 * */

u8 getAddr(){
	return pumpno;
}

void setAddr(u8 i){
	pumpno = i;
}


//void dart::wifiAuthPumpByAmo(float amo){
//	wifiAuthAmo = amo;
//	wifiAuth = true;
//}

/* protocol thread API */
//consider includng a return value here to show if the command was responded to well
void PNP_setup(){
	;
}

void get_status(){
//	cmd_get_status();
//	handle_resp(GETSTATUS); //another command is sent here in this function
}

void get_unit_price(){
//	cmd_get_filling_info();	//this command returns, volume, amount, price and nozzle state
//	handle_resp(REQUEST_FILLING_INFO);
}

void set_unit_price(float price){
//	cmd_update_price(price);
//	handle_resp(PRICE_UPDATE);
//
//	/* added it here beceause of sslan */
//	setPriceFlag = false;
}

void go_setUnitPrice1(float price_update)
{
	settings_stream1[0].price_ = price_update1;

	save_settings();   //save to eeprom
	load_settings(side_a); //load the settings into the internal variables
}

float go_fillingInfo_vol1(void)
{
	return amt_middle1;
}

float go_fillingInfo_amt1(void)
{
	return price_upper1;
}

float go_fillingPrice1(void)
{
	return litre_price;
}
void get_totalizer(){
//	cmd_get_totalizer();
//	handle_resp(REQUEST_VOL_TOTAL_COUNT);
}

float get_filled_vol(){
//	cmd_get_filling_info();
//	handle_resp(REQUEST_FILLING_INFO);
//	return filled_vol;
}

float get_filled_amo(){
//	cmd_get_filling_info();
//	handle_resp(REQUEST_FILLING_INFO);
//	return filled_amo;
}

void get_filling_info(){
//	cmd_get_filling_info();
//	handle_resp(REQUEST_FILLING_INFO);
}

int auth_by_vol1(float vol){
//	int ret = 99;
//	int cnt = 4;
//
//	while (ret != 0 && --cnt){ //retry this 3 times and hope to get a return value of 0, i.e. try to send the preset vol up to three times
//		cmd_set_vol(vol);
//		ret = handle_resp(PRESET_VOL);
//		TRACE_DART("<%s> RET of cmd: %d ", __FUNCTION__, ret);
//	}
//
//	if (ret == 0 || mepsan_magic_char){	//if it got reply well
//		cmd_authorise();
//		handle_resp(AUTHORISE);
//	}
//
//	return ret;
}

//void dart::auth_by_amo(float amnt){
//	cmd_set_amo(amnt);
//	handle_resp(PRESET_AMO);
//
//	cmd_authorise();
//	handle_resp(AUTHORISE);
//}

//to return if the process was success full
int auth_by_amo1(float amnt){
//	int ret = 99;
//	int cnt = 4;
//
//	while (ret != 0 && --cnt){ //retry this 3 times and hope to get a return value of 0, i.e. try to send the preset vol up to three times
//		cmd_set_amo(amnt);
//		ret = handle_resp(PRESET_AMO);
//		TRACE_DART("<%s> RET of cmd: %d ", __FUNCTION__, ret);
//	}
//
//	if (ret == 0 || mepsan_magic_char){	//if it got reply well
//		cmd_authorise();
//		handle_resp(AUTHORISE);
//	}
//
//	return ret;
}

int set_amo1(float amnt){
//	int ret = 99;
//	int cnt = 4;
//
//	while (ret != 0 && --cnt){ //retry this 3 times and hope to get a return value of 0, i.e. try to send the preset vol up to three times
//		cmd_set_amo(amnt);
//		ret = handle_resp(PRESET_AMO);
//		TRACE_DART("<%s> RET of cmd: %d ", __FUNCTION__, ret);
//	}
//	return ret;
}

//void dart::authorise(){
//	cmd_set_vol(MAX_PRESET_VOL);
//	handle_resp(PRESET_VOL);
//
//	cmd_authorise();
//	handle_resp(AUTHORISE);
//}

void authorise(){
//	int ret = 99;
//	int cnt = 4;
//
//	while (ret != 0 && --cnt){ //retry this 3 times and hope to get a return value of 0, i.e. try to send the preset vol up to three times
//		cmd_set_vol(MAX_PRESET_VOL);
//		ret = handle_resp(PRESET_VOL);
////		TRACE_DART("<%s> RET of cmd: %d ", __FUNCTION__, ret);
//	}
//
//	if (ret == 0 || mepsan_magic_char){	//if it got reply well
//		cmd_authorise();
//		handle_resp(AUTHORISE);
//	}
}


void stop1(void){
//	cmd_stop();
//	handle_resp(STOP);
}

void pause() {

}

void resume() {

}

void switch_off(){
//	cmd_switch_off();
//	handle_resp(SWITCH_OFF);
}
void reset1(){
//	cmd_reset();
//	handle_resp(RESET);
}

//Had to override this function in order to enable setting a pump which is not responding to unknown status
//void calLastResponseTime(){
//	respTime = eat_get_duration_ms(lastRespTime);
//	if (respTime > NO_RESP_TIMEOUT){
//		status = STATUS_UNKNOWN;
//	}
//	//	TRACE_DART("TIME SINCE LAST RESPONSE: %d", respTime);
//}


//void uart_setup(){
//
//	if (uart_use_pump_def){
//		set_uart_ptr(&uart[0]);
//		p_uart->port=  EAT_UART_1;
//		p_uart->config.baud= EAT_UART_BAUD_9600;
//		p_uart->config.parity= EAT_UART_PARITY_ODD;
//		/** other parameters has been set by default in uart constructor */
//	}
//
////	TRACE_DART("<%s> - dart", __FUNCTION__);
//	eat_pin_set_mode(RE_DE, EAT_PIN_MODE_GPIO);
//	eat_gpio_setup(RE_DE, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);
//
//	p_uart->init();
//}
//
//void uart_send(u8* write_buff, u32 size){
//	//Pull ~RE/DE high in order to write to max485
//	eat_gpio_write(RE_DE, EAT_GPIO_LEVEL_HIGH);
//	//	eat_sleep(5);
//
//	u16 len_gpt = p_uart->writed((char*)write_buff, size);
//
//	//	u16 gpt_coef = 0;	//gpt coefficient
//	//	if (len_gpt > 5 && len_gpt < 15) gpt_coef = 25*len_gpt;
//	//	else if (len_gpt >= 15) gpt_coef = 30*len_gpt;
//	//	else gpt_coef = 18*len_gpt;
//	//
//	//	eat_gpt_start(gpt_coef, EAT_FALSE, eat_gpt_cb_fun);//RE_DELAY*16384 //((18 x length_of_byte)*62) this is the time it takes for the uart data to reach the pump
//	//	TRACE_DART("GPT! for last message started - delay: %d ms", (61*gpt_coef)/1000);
//
//	eat_gpt_start(18*len_gpt, EAT_FALSE, eat_gpt_cb_fun);//RE_DELAY*16384 //((18 x length_of_byte)*62) this is the time it takes for the uart data to reach the pump
//	TRACE_DART("GPT! for last message started - delay: %d ms", (18*61*len_gpt)/1000);
//
//	//	eat_gpio_write(RE_DE, EAT_GPIO_LEVEL_LOW);
//}
//
//void uart_complete_event_proc(){
//	//	eat_gpt_start(18,EAT_FALSE, eat_gpt_cb_fun); //61 * 18 = 1000us = 1ms
//	//	TRACE_DART("<%s> - dart, bringing RE_DE pin low to enable reception of data", __FUNCTION__);
//
//	//	TRACE_DART("GPT! for last message started - delay: %d ms", len_gpt);
//	//	eat_sleep(len_gpt);
//	//	eat_gpio_write(RE_DE, EAT_GPIO_LEVEL_LOW);
//
//	//	eat_gpio_write(RE_DE, EAT_GPIO_LEVEL_LOW);
//}
//
//void send_get_uart_msg(unsigned char* write_buf, unsigned int size, s32 wait_time){
//	int ret = 0;
//	u8 rx_buf_l[400] = {0};		//local receive buffer
//	u32 rx_len_l;		//local receive buffer length
//
//	uart_send(write_buf, size);
//	ret = p_uart->waitForResp(wait_time);
//
//	if (ret > 0){
//		eat_sleep(10);
//		rx_len_l = p_uart->readd((char*)rx_buf_l, sizeof(rx_buf_l));	//this function sets
//		receive_response(rx_buf_l, (int)rx_len_l);
//	}
//	else {
//		resp = NOREPLY;
//	}
//
//	//You can put your no_response algorithm here
//}
//
//void send_get_uart_msg(unsigned char* write_buf, unsigned int size)
//{
//	send_get_uart_msg(write_buf, size, MSG_TIMEOUT);
//}
//
//void send_get_uart_msg(){
//	;
//}







// Function to calculate CRC-16
uint16_t calculate_crc(uint8_t *data, size_t length) {
    uint16_t crc = 0x0000;    //0xFFFF; // Initial value
    uint16_t polynomial = 0x1021; // CRC-16-CCITT polynomial

    for (size_t i = 0; i < length; i++) {
        crc ^= (uint16_t)(data[i] << 8); // XOR byte into the high byte of CRC

        for (int j = 0; j < 8; j++) { // Process each bit
            if (crc & 0x8000) { // If the high bit is set
                crc = (crc << 1) ^ polynomial;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}


double roundUp(float value, int decimalPlaces)
{
    double factor = pow(10, decimalPlaces);
    return ceil(value * factor) / factor;
}

// unsigned int decimalToPackedBCD(unsigned int decimal)
// {
//     unsigned int bcd = 0;
//     int shift = 0;

//     while (decimal > 0)
// 	{
//         // Get the least significant digit
//         int digit = decimal % 10;

//         // Shift the digit into the correct place in the BCD result
//         // bcd |= (digit << (shift * 4));
// 		bcd |= (digit << (shift * 4));

//         // Move to the next digit
//         decimal /= 10;
//         shift++;
//     }

//     return bcd;
// }

void int_to_bcd(int num, unsigned char *bcd)
{
    int index = 0;
    while (num > 0) {
        unsigned char digit = num % 10;
        if (index % 2 == 0)
        {
            bcd[index / 2] = digit;
        }
        else
        {
            bcd[index / 2] |= (digit << 4);
        }
        num /= 10;
        index++;
    }
}


//parse_message : separate the received message into its different constituent
//void parse_message0(unsigned char* arr, int size)
//{
//	//r_raw_data.clear();
//	r_addr = arr[0];
//	r_pumpno = r_addr-0x4f;			//pumpno i.e either pump 1 or 2 on
//	r_ctrl = arr[1];				//control character that specifies the type of message received
//	r_TX = r_ctrl & 0x0f;		    //the TX of the received message (from slave) attached to the ctrl character
//	//r_trans = arr[2];				//received transaction ID
//	//r_lng = arr[3];					//length of data byte
//
//	//TRACE_DART(" <%s>-- : r_addr: %x, r_ctrl: %x, r_lng: %x \n", __FUNCTION__, r_ctrl, arr[0], r_lng);
//
//	if (r_pumpno == pumpno)	{
//		for (int i=2; i<size; i++){
//			//r_raw_data.push_back(arr[i]);	//shift the data in the array into the r_raw_data vector
//		}
//	//	process_data();
//	}
//	else{
//		//TRACE_DART("<%s> Not for me Pump[%d]<>", __FUNCTION__, pumpno);
//		resp = NOREPLY;
//	}
//	//include a condition to ensure that the message parsed is more than a particular value
//}

//function overload of the parsed message to parse ack, nack, eot messages
void parse_message1(unsigned char* arr)
{
	//r_raw_data.clear();

	r_addr = arr[0];
	r_pumpno = r_addr-0x4f;	//pumpno i.e either pump one or two
	r_ctrl = arr[1];		//control character that specifies the type of message received
	r_TX = r_ctrl & 0x0f;	//the TX of the received message attached to the ctrl character

//	process_data();
}

uint32_t packed_bcd_to_decimal(uint32_t bcd)
{
     uint32_t decimal = 0;
     uint32_t multiplier = 1;

    for (int i = 0; i < 8; ++i)
    {
        // Extract the rightmost BCD digit (4 bits)
        uint8_t digit = bcd & 0xF;

        // Convert to decimal and add to the result
        decimal += digit * multiplier;

        // Move to the next BCD digit
        bcd >>= 4;
        multiplier *= 10;
    }

    return decimal;
}


void go_read(void)
 {
 	  	static int16_t head_pos = 0;

 	  	static int8_t curly_brace = 0,
 	  				  header_found = 0;

 	  	static uint8_t go_rx_buf[pump_rx_bufsize] = {0};

 	  	uint8_t rx;

 	   int16_t rxcnt = pump_rx_bufsize - huart2.RxXferCount;
 	   rx = (uint8_t)uart2_rx_buf[rxcnt-1];

 	   if( (rx > 0x4F) && (header_found == 0) )    // header left square bracket 0x5B, 0d91   STX->0xA5
 	   {
 		  header_found = 1;
 		  head_pos = 0;
 		  memset(rx_buf1, 0, pump_rx_bufsize);
 		  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
 			  rx_buf1[head_pos] = rx;

 		  head_pos++;
 	   }

 	   //-------------------------------------------------------
 	   else if (header_found == 1)
 	   {
 		 //header found look for the footer...
 		   if(rx == 0xFA)
 		   {
				 if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )

				 rx_buf1[head_pos] = rx;

				 //  memcpy( pump_buf , pump_rx_buf[head_pos] ,rxcnt ); //transfer to the definitive buffer.
				 huart2.RxXferCount = pump_rx_bufsize;
				 huart2.pRxBuffPtr = &uart2_rx_buf[0]; //reset //uart2_rx_buf; //
				 header_found = 0;
				 head_pos = 0;
				 curly_brace = 0;
				 server_message_found = 1;
 		   }
 		   else
 		   {
// 			   if( (rx == '\0') && (head_pos > 10) )
// 			   {
// 					 huart2.RxXferCount = pump_rx_bufsize;
// 					 huart2.pRxBuffPtr = &uart2_rx_buf[0]; //reset //uart2_rx_buf; //
// 					 header_found = 0;
// 					 head_pos = 0;
// 					 curly_brace = 0;
// 			   }

 			   if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
 				   rx_buf1[head_pos] = rx;

 			   head_pos++;
 			   head_posn++;
 		   }
 	   }
 }






//		//===================================================================//
//		//   This transaction is sent by the pump if the status is changed   //
//		//   or if the pump receives the command 'RETURN STATUS’.            //
//		//===================================================================//
//
//		if (command_ == GETSTATUS)
//		{
//			//---- Handles command for sending of data to controller -----//
//
//			ctrl |= 0x30;
//			trans = 0x01;
//			lng = 0x01;
//			memset(DART_BUFF1, 0, sizeof(DART_BUFF1));
//
//			DART_BUFF1[0] = addr;
//			DART_BUFF1[1] = ctrl;
//			DART_BUFF1[2] = trans;
//			DART_BUFF1[3] = lng;
//
//			pump_status_ = STATUS_RESET;
//
//			if(check == 0)
//			{
//				ctrl = 0x00;
//				ctrl |= 0x00;
//				ctrl |= 0x30;
//				DART_BUFF1[1] = ctrl;
//				check = 1;
//			}
//
//			switch (pump_status_)
//			{
//				//for the Pump-Status Commands
//				case STATUS_PNP		      				:	{status_ = 0x00; break;}
//				case STATUS_RESET 						:	{status_ = 0x01; break;}
//				case STATUS_AUTH 						:	{status_ = 0x02; break;}
//				case STATUS_FILLING						:	{status_ = 0x04; break;}
//				case STATUS_FILLING_COMP				:	{status_ = 0x05; break;}
//				case STATUS_MAMO_REACHED				:	{status_ = 0x06; break;}   //MAX_AMOUNTVOLUME_REACHED
//				case STATUS_SWITCHED_OFF				:	{status_ = 0x07; break;}
//				default									: 	break;
//			}
//
//			DART_BUFF1[4] = status_;
//		//		uint16_t crc = calculate_crc(DART_BUFF1, 5);
//			crc = crc_16(DART_BUFF1, 5);
//			DART_BUFF1[5] = crc & 0x00FF;
//			DART_BUFF1[6] = crc >> 8;
//			DART_BUFF1[7] = ETX;
//			DART_BUFF1[8] = SF;
//
//			array_len = 9;
//		}
//

void send_nozzleStatus(uint8_t buff_index)  //, float filling_price, uint8_t nozzle_status)
{
				//=============================================================================//
				//   	  This transaction is sent by the pump if the status is changed        //
				//		  or if the pump receives the command                                  //
				//		  'RETURN STATUS' or ‘RETURN FILLING INFORMATION’.      			   //
				//=============================================================================//

				uint8_t nozNum = 1,
						nozStatus = 0, //0 -> in, 1 -> out
						nozIO;

				uint8_t decimalPlaces = 0;
				double roundedNum;
				int num_;
				unsigned char bcd_[10] = {0};  // Array to hold the BCD result

				float fillingPrice = go_fillingPrice1();

				ctrl = TX;
				ctrl |= 0x30;
				trans = 0x03;
				lng = 0x04;

//				memset(DART_BUFF1, 0, sizeof(DART_BUFF1));

				DART_BUFF1[0] = addr;
				DART_BUFF1[1] = ctrl;

				DART_BUFF1[2 + buff_index] = trans;
				DART_BUFF1[3 + buff_index] = lng;

				decimalPlaces = 1;

				roundedNum = roundUp(fillingPrice, decimalPlaces);
				roundedNum = roundedNum * 10;
				num_ = (int)(roundedNum);

				memset(bcd_, 0, sizeof(bcd_));

				int_to_bcd(num_, bcd_);

				for (uint8_t i = 0, j = 2;  i < 3; i++, j--)
				{
					DART_BUFF1[i + 4 + buff_index] = bcd_[j];
				}

				if(nozzle_out1 == true) nozStatus = 1;
				else nozStatus = 0;

				nozStatus = (nozStatus << 4);
				nozIO = nozNum | nozStatus;

				DART_BUFF1[7 + buff_index] = nozIO;    //Nozzle 1
}


void send_pumpStatus(uint8_t buff_index)
{
	uint8_t status_;

	ctrl = TX;
	ctrl |= 0x30;
	trans = 0x01;
	lng = 0x01;

	DART_BUFF1[0] = addr;
	DART_BUFF1[1] = ctrl;

	DART_BUFF1[2 + buff_index] = trans;
	DART_BUFF1[3 + buff_index] = lng;


	switch (pump_status_)
	{
		//for the Pump-Status Commands
		case STATUS_PNP		      				:	{status_ = 0x00; break;}
		case STATUS_RESET 						:	{status_ = 0x01; break;}
		case STATUS_AUTH 						:	{status_ = 0x02; break;}
		case STATUS_FILLING						:	{status_ = 0x04; break;}
		case STATUS_FILLING_COMP				:	{status_ = 0x05; break;}
		case STATUS_MAMO_REACHED				:	{status_ = 0x06; break;}   //MAX_AMOUNTVOLUME_REACHED
		case STATUS_SWITCHED_OFF				:	{status_ = 0x07; break;}
		default									: 	break;
	}

	DART_BUFF1[4 + buff_index] = status_;
}


////initialise the array of structure of State and state handlers and their
//// allowed  events.
//// { <state>,<handler>,{<allowed event1>,<allowed event2>,..,<allowed eventn>}}
//sStateEventMachine asStateEventMachine_1 [] =
//{
//	{prog_State, progstate_Handler,{_keydown_Event,_keypress_Event}},
////    {idle_State,idlestate_Handler,{_operator_Event,_keyup_Event,_tot_error_Event,_keypress_Event,_nozzleup_Event,_auth_command_Event, _nozzledown_Event}},
//	//{fillingcomplete_State, fillingcompletestate_Handler, {_nozzleup_Event, _resetcommand_Event, _switchoffcommand_Event}},
//	{idle_State, idlestate_Handler, {_operator_Event,_keyup_Event,_tot_error_Event,_keypress_Event,_nozzleup_Event,_auth_command_Event, _nozzledown_Event, _resetcommand_Event, _switchoffcommand_Event}},
//	{inactive_State,inactivestate_Handler,{_error_clear_Event, _keyup_Event, _keypress_Event}},
////    {nozzleup_waitingforauth_State,nozzleup_waitingforauthState_Handler,{_authorise_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
////	{reset_State, resetState_Handler, {_authorisecommand_Event, _stopcommand_Event, _switchoffcommand_Event, hardwarereset_Event, hardwareerror_Event}},
//	{nozzleup_waitingforauth_State, nozzleup_waitingforauthState_Handler, {_authorise_Event,_timeout_Event,_nozzledown_Event,_keypress_Event, _authorisecommand_Event, _stopcommand_Event, _switchoffcommand_Event, hardwarereset_Event, hardwareerror_Event}},
//
//	{authorised_nozzledown_State,authorised_nozzledown_State_Handler,{_nozzleup_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
//    {authorised_nozzleup_State,authorised_nozzleup_State_Handler,{_filling_pulse_Event,_pause_Event,_timeout_Event,_nozzledown_Event,_keypress_Event,_function_key_Event}},
////	{authorised_nozzleup_State,authorised_nozzleup_State_Handler,{_filling_pulse_Event,_pause_Event,_timeout_Event,_nozzledown_Event,_keypress_Event,_function_key_Event, _stopcommand_Event, _suspendcommand_Event, hardwarereset_Event, hardwareerror_Event}},
//	{authorisation_paused_State,authorisation_paused_State_Handler,{_resume_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
//	{filling_State,filling_state_Handler,{_filling_paused_Event,_keypress_Event,_timeout_Event,_nozzledown_Event,_keypress_Event}},
//	{filling_paused_State,filling_paused_state_Handler,{_filling_resumed_Event,_keypress_Event,_timeout_Event,_nozzledown_Event}},
//	{keypad_entry_State,keypad_entry_State_Handler,{}},
//	{operator_State,operator_State_Handler,{_keypress_Event}},
//	{savesettings_State,savesettings_State_Handler,{_keypress_Event}},
//	{read_flash_state,read_flash_state_Handler,{}},
//	{write_flash_state,write_flash_state_Handler,{}},
//
//	{switchedoff_State, switchedoffstate_Handler, {_stopcommand_Event, _resetcommand_Event}},
//    {pnp_State, pnpstate_Handler, {_priceupdate_Event}},
////    {fillingcomplete_State, fillingcompletestate_Handler, {_nozzleup_Event, _resetcommand_Event, _switchoffcommand_Event}},
////	{idle_State,idlestate_Handler,{_operator_Event,_keyup_Event,_tot_error_Event,_keypress_Event,_nozzleup_Event,_auth_command_Event, _nozzledown_Event}},
//	//{_nozzleup_Event, _resetcommand_Event, _switchoffcommand_Event}},
////	{fillingcomplete_State, fillingcompletestate_Handler, {_operator_Event,_keyup_Event,_tot_error_Event,_keypress_Event,_nozzleup_Event,_auth_command_Event, _nozzledown_Event, _resetcommand_Event, _switchoffcommand_Event}},
//
////	{reset_State, resetState_Handler, {_authorisecommand_Event, _stopcommand_Event, _switchoffcommand_Event, hardwarereset_Event, hardwareerror_Event}},
////	{authorised_State, authorisedState_Handler,{_nozzleup_Event,_timeout_Event,_nozzledown_Event,_keypress_Event, _stopcommand_Event, hardwarereset_Event, hardwareerror_Event, _suspendcommand_Event}},
////    {authorised_nozzleup_State,authorised_nozzleup_State_Handler,{_filling_pulse_Event,_pause_Event,_timeout_Event,_nozzledown_Event,_keypress_Event,_function_key_Event, _stopcommand_Event, _suspendcommand_Event, hardwarereset_Event, hardwareerror_Event}},
//	{authorisation_paused_State,authorisation_paused_State_Handler,{_resume_Event,_timeout_Event,_nozzledown_Event,_keypress_Event, _auth_resumecommand_Event,  _stopcommand_Event, hardwarereset_Event, hardwareerror_Event, _switchoffcommand_Event}},
//	{filling_State, filling_state_Handler,{_filling_paused_Event,_keypress_Event,_timeout_Event,_nozzledown_Event,_keypress_Event, _stopcommand_Event, _suspendcommand_Event, hardwarereset_Event, hardwareerror_Event, mamo_Event, _switchoffcommand_Event}},
//	{filling_paused_State, filling_paused_state_Handler,{_filling_resumed_Event,_keypress_Event,_timeout_Event,_nozzledown_Event, _filling_resumecommand_Event, _stopcommand_Event, hardwarereset_Event, hardwareerror_Event, _switchoffcommand_Event}},
//	{filledmamo_State, filledmamo_State_Handler, {_nozzledown_Event, _resetcommand_Event, _stopcommand_Event, _switchoffcommand_Event}},
//
//    {last_State, 0, {}}
//};
//

////----------------------------------------
//eSystemState filledmamo_State_Handler(void)
//{
//	return filledmamo_State;
//}
//
////----------------------------------------
//eSystemState pnpstate_Handler(void)
//{
//	return pnp_State;
//}
//
////----------------------------------------
//eSystemState switchedoffstate_Handler(void)
//{
//	return switchedoff_State;
//}

//----------------------------------------


//if (command_ == GETSTATUS)
//	{
//
//		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
//		//////////////////////////////////  GET FILLED_VOLUME_AND_AMOUNT //////////////////////////////////
//
//
//
//		//=============================================================================//
//		////////////////////////////////////  DC2  //////////////////////////////////////
//		//        This transaction is sent by the pump at change of a value            //
//		//        or if the pump receives the command RETURN FILLING INFORMATION       //
//		//=============================================================================//
//		//command_ ==> REQUEST_FILLING_INFO        //FILLED_VOLUME_AND_AMOUNT
//
////				float vol_ = 15.1234;
//		float vol_ = go_fillingInfo_vol1();
////				float amo_ = 1500.1234;
//		float amo_ = go_fillingInfo_amt1();
//
//		uint8_t decimalPlaces;
//		double roundedNum;
//		int num_;
//		// unsigned int bcd;
//		unsigned char bcd_[10] = {0};  // Array to hold the BCD result
//
//		uint16_t crc;
//
//		ctrl = TX;
//		ctrl |= 0x30;
//		trans = 0x02;
//		lng = 0x08;
//
//		memset(DART_BUFF1, 0, sizeof(DART_BUFF1));
//
//		DART_BUFF1[0] = addr;
//		DART_BUFF1[1] = ctrl;
//		DART_BUFF1[2] = trans;
//		DART_BUFF1[3] = lng;
//
//		decimalPlaces = 2;
//
//		roundedNum = roundUp(vol_, decimalPlaces);
//		roundedNum = roundedNum * 100;
//		num_ = (int)(roundedNum);
//
//		int_to_bcd(num_, bcd_);
//
//		for (uint8_t i = 0, j = 3; i < 4; i++, j--)
//		{
//			DART_BUFF1[i + 4] = bcd_[j];
//		}
//
//		decimalPlaces = 1;
//
//		roundedNum = roundUp(amo_, decimalPlaces);
//		roundedNum = roundedNum * 10;
//		num_ = (int)(roundedNum);
//
//		memset(bcd_, 0, sizeof(bcd_));
//
//		int_to_bcd(num_, bcd_);
//
//		// for(uint8_t i = 0; i < 4; i++)
//		for (uint8_t i = 0, j = 3; i < 4; i++, j--)
//		{
//			DART_BUFF1[i + 8] = bcd_[j];
//		}
//
//
//		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
//		//////////////////////////////////  GET NOZSTATUS_AND_FILLINGPRICE //////////////////////////////////
//
//		//=============================================================================//
//		//////////////////////////////////  DC3  ////////////////////////////////////////
//		//   	  This transaction is sent by the pump if the status is changed        //
//		//		  or if the pump receives the command                                  //
//		//		  'RETURN STATUS' or ‘RETURN FILLING INFORMATION’.      			   //
//		//=============================================================================//
//
//		//command_ ==> REQUEST_FILLING_INFO        // NOZSTATUS_AND_FILLINGPRICE
//
////				float fillingPrice = 100.00;
//		float fillingPrice = go_fillingPrice1();
//
////			int decimalPlaces = 2;
////			double roundedNum;
////			int num_;
////			unsigned char bcd_[10] = {0};  // Array to hold the BCD result
//
//		trans = 0x03;
//		lng = 0x04;
//
//		DART_BUFF1[12] = trans;
//		DART_BUFF1[13] = lng;
//
//		decimalPlaces = 1;
//
//		roundedNum = roundUp(fillingPrice, decimalPlaces);
//		roundedNum = roundedNum * 10;
//		num_ = (int)(roundedNum);
//
//		memset(bcd_, 0, sizeof(bcd_));
//
//		int_to_bcd(num_, bcd_);
//
//		for (uint8_t i = 0, j = 2;  i < 3; i++, j--)
//		{
//			DART_BUFF1[i + 14] = bcd_[j];
//		}
//
//		uint8_t nozNum = 1,
//				nozStatus = 0, //0 -> in, 1 -> out
//				nozIO;
//
//		if(nozzle_out1 == true) nozStatus = 1;
//		else nozStatus = 0;
//
//		// nozIO = nozNum;
//		nozStatus = (nozStatus << 4);
//		nozIO = nozNum | nozStatus;
//
//		DART_BUFF1[17] = nozIO;    //Nozzle 1
//
//
//
//		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
//		//////////////////////////////////  GET PUMP-STATUS //////////////////////////////////
//
//		trans = 0x01;
//		lng = 0x01;
//
//		DART_BUFF1[18] = trans;
//		DART_BUFF1[19] = lng;
//
//		//==============================//
////				pump_status_ = STATUS_RESET;
//		//=============================//
//
////			if(check == 0)
////			{
////				ctrl = 0x00;
////				ctrl |= 0x00;
////				ctrl |= 0x30;
////				DART_BUFF1[1] = ctrl;
////				check = 1;
////			}
//
//		switch (pump_status_)
//		{
//			//for the Pump-Status Commands
//			case STATUS_PNP		      				:	{status_ = 0x00; break;}
//			case STATUS_RESET 						:	{status_ = 0x01; break;}
//			case STATUS_AUTH 						:	{status_ = 0x02; break;}
//			case STATUS_FILLING						:	{status_ = 0x04; break;}
//			case STATUS_FILLING_COMP				:	{status_ = 0x05; break;}
//			case STATUS_MAMO_REACHED				:	{status_ = 0x06; break;}   //MAX_AMOUNTVOLUME_REACHED
//			case STATUS_SWITCHED_OFF				:	{status_ = 0x07; break;}
//			default									: 	break;
//		}
//
//		DART_BUFF1[20] = status_;
//
//
//		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
//		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//
//		crc = crc_16(DART_BUFF1, 21);
//		DART_BUFF1[21] = crc & 0x00FF;
//		DART_BUFF1[22] = crc >> 8;
//		DART_BUFF1[23] = ETX;
//		DART_BUFF1[24] = SF;
//
//		array_len = 25;
//
//		resp = NOREPLY;
//
//	}
//
//	//=============================================================================//
//	//        This transaction is sent by the pump at change of a value            //
//	//        or if the pump receives the command RETURN FILLING INFORMATION       //
//	//=============================================================================//
//

//
////=============================================================================//
//			//   	  This transaction is sent by the pump if the status is changed        //
//			//		  or if the pump receives the command                                  //
//			//		  'RETURN STATUS' or ‘RETURN FILLING INFORMATION’.      			   //
//			//=============================================================================//
//
//			else if ( (command_ == GETSTATUS) || (command_ == REQUEST_FILLING_INFO) )        // NOZSTATUS_AND_FILLINGPRICE
//			{
//
//				float fillingPrice = 100.00;
//				int decimalPlaces = 2;
//				double roundedNum;
//				int num_;
//				unsigned char bcd_[10] = {0};  // Array to hold the BCD result
//
//				uint16_t crc;
//
//				ctrl |= 0x30 ;
//				trans = 0x01;
//				lng = 0x01;
//
//				memset(DART_BUFF1, 0, sizeof(DART_BUFF1));
//
//				DART_BUFF1[0] = addr;
//				DART_BUFF1[1] = ctrl;
//				DART_BUFF1[2] = trans;
//				DART_BUFF1[3] = lng;
//
//				roundedNum = roundUp(fillingPrice, decimalPlaces);
//				roundedNum = roundedNum * 100;
//				num_ = (int)(roundedNum);
//
//				int_to_bcd(num_, bcd_);
//
//				for (uint8_t i = 0, j = 2;  i < 3; i++, j--)
//				{
//					DART_BUFF1[i + 4] = bcd_[j];
//				}
//
//				uint8_t nozNum, nozStatus, nozIO;
//
//				// nozIO = nozNum;
//				nozStatus = (nozStatus << 4);
//				nozIO = nozNum | nozStatus;
//
//			//		crc = calculate_crc(DART_BUFF1, 7);
//				crc = crc_16(DART_BUFF1, 7);
//				DART_BUFF1[7] = crc & 0x00FF;
//				DART_BUFF1[8] = crc >> 8;
//				DART_BUFF1[9] = ETX;
//				DART_BUFF1[10] = SF;
//
//				array_len = 11;
//			}


//if (command_ == GETSTATUS)
//			{
//
//				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
//				//////////////////////////////////  GET FILLED_VOLUME_AND_AMOUNT //////////////////////////////////
//
//
//
//				//=============================================================================//
//				////////////////////////////////////  DC2  //////////////////////////////////////
//				//        This transaction is sent by the pump at change of a value            //
//				//        or if the pump receives the command RETURN FILLING INFORMATION       //
//				//=============================================================================//
//				//command_ ==> REQUEST_FILLING_INFO        //FILLED_VOLUME_AND_AMOUNT
//
////				float vol_ = 15.1234;
//				float vol_ = go_fillingInfo_vol1();
////				float amo_ = 1500.1234;
//				float amo_ = go_fillingInfo_amt1();
//
//				uint8_t decimalPlaces;
//				double roundedNum;
//				int num_;
//				// unsigned int bcd;
//				unsigned char bcd_[10] = {0};  // Array to hold the BCD result
//
//				uint16_t crc;
//
//				ctrl = TX;
//				ctrl |= 0x30;
//				trans = 0x02;
//				lng = 0x08;
//
//				memset(DART_BUFF1, 0, sizeof(DART_BUFF1));
//
//				DART_BUFF1[0] = addr;
//				DART_BUFF1[1] = ctrl;
//				DART_BUFF1[2] = trans;
//				DART_BUFF1[3] = lng;
//
//				decimalPlaces = 2;
//
//				roundedNum = roundUp(vol_, decimalPlaces);
//				roundedNum = roundedNum * 100;
//				num_ = (int)(roundedNum);
//
//				int_to_bcd(num_, bcd_);
//
//				for (uint8_t i = 0, j = 3; i < 4; i++, j--)
//				{
//					DART_BUFF1[i + 4] = bcd_[j];
//				}
//
//				decimalPlaces = 1;
//
//				roundedNum = roundUp(amo_, decimalPlaces);
//				roundedNum = roundedNum * 10;
//				num_ = (int)(roundedNum);
//
//				memset(bcd_, 0, sizeof(bcd_));
//
//				int_to_bcd(num_, bcd_);
//
//				// for(uint8_t i = 0; i < 4; i++)
//				for (uint8_t i = 0, j = 3; i < 4; i++, j--)
//				{
//					DART_BUFF1[i + 8] = bcd_[j];
//				}
//
//
//				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
//				//////////////////////////////////  GET NOZSTATUS-AND-FILLINGPRICE //////////////////////////////////
//
//				//=============================================================================//
//				//////////////////////////////////  DC3  ////////////////////////////////////////
//				//   	  This transaction is sent by the pump if the status is changed        //
//				//		  or if the pump receives the command                                  //
//				//		  'RETURN STATUS' or ‘RETURN FILLING INFORMATION’.      			   //
//				//=============================================================================//
//
//				//command_ ==> REQUEST_FILLING_INFO        // NOZSTATUS_AND_FILLINGPRICE
//
////				float fillingPrice = 100.00;
//				float fillingPrice = go_fillingPrice1();
//
//	//			int decimalPlaces = 2;
//	//			double roundedNum;
//	//			int num_;
//	//			unsigned char bcd_[10] = {0};  // Array to hold the BCD result
//
//				trans = 0x03;
//				lng = 0x04;
//
//				DART_BUFF1[12] = trans;
//				DART_BUFF1[13] = lng;
//
//				decimalPlaces = 1;
//
//				roundedNum = roundUp(fillingPrice, decimalPlaces);
//				roundedNum = roundedNum * 10;
//				num_ = (int)(roundedNum);
//
//				memset(bcd_, 0, sizeof(bcd_));
//
//				int_to_bcd(num_, bcd_);
//
//				for (uint8_t i = 0, j = 2;  i < 3; i++, j--)
//				{
//					DART_BUFF1[i + 14] = bcd_[j];
//				}
//
//				uint8_t nozNum = 1,
//						nozStatus = 0, //0 -> in, 1 -> out
//						nozIO;
//
//				if(nozzle_out1 == true) nozStatus = 1;
//				else nozStatus = 0;
//
//				// nozIO = nozNum;
//				nozStatus = (nozStatus << 4);
//				nozIO = nozNum | nozStatus;
//
//				DART_BUFF1[17] = nozIO;    //Nozzle 1
//
//
//
//				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
//				//////////////////////////////////  GET PUMP-STATUS //////////////////////////////////
//
//				trans = 0x01;
//				lng = 0x01;
//
//				DART_BUFF1[18] = trans;
//				DART_BUFF1[19] = lng;
//
//				//==============================//
////				pump_status_ = STATUS_RESET;
//				//=============================//
//
//	//			if(check == 0)
//	//			{
//	//				ctrl = 0x00;
//	//				ctrl |= 0x00;
//	//				ctrl |= 0x30;
//	//				DART_BUFF1[1] = ctrl;
//	//				check = 1;
//	//			}
//
//				switch (pump_status_)
//				{
//					//for the Pump-Status Commands
//					case STATUS_PNP		      				:	{status_ = 0x00; break;}
//					case STATUS_RESET 						:	{status_ = 0x01; break;}
//					case STATUS_AUTH 						:	{status_ = 0x02; break;}
//					case STATUS_FILLING						:	{status_ = 0x04; break;}
//					case STATUS_FILLING_COMP				:	{status_ = 0x05; break;}
//					case STATUS_MAMO_REACHED				:	{status_ = 0x06; break;}   //MAX_AMOUNTVOLUME_REACHED
//					case STATUS_SWITCHED_OFF				:	{status_ = 0x07; break;}
//					default									: 	break;
//				}
//
//				DART_BUFF1[20] = status_;
//
//
//				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
//				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//
//				crc = crc_16(DART_BUFF1, 21);
//				DART_BUFF1[21] = crc & 0x00FF;
//				DART_BUFF1[22] = crc >> 8;
//				DART_BUFF1[23] = ETX;
//				DART_BUFF1[24] = SF;
//
//				array_len = 25;
//
//				resp = NOREPLY;
//
//			}
