///*
// * dart_.c
// *
// *  Created on: 28 Jun 2024
// *      Author: Jheebolar
// */
//
//
//
//
//#include "dart_.h"
//#include "../crc/checksum.h"
//
///*
// * p_wayne_485.cpp
// *
// *  Created on: Jan 23, 2020
// *      Author: J.cliff
// *      Description: Low level logic for the wayne rs-485 protocol
// */
//
///* stl includes */
//#include <string.h>
////#include <iostream>
////#include <map>
//#include <stdlib.h>
//#include <stdio.h>
////#include <iomanip>
//#include <inttypes.h>
//#include <main.h>
////#include <net/comm.h>
//#include <stdbool.h>
//#include <stdint.h>
////#include <vector>
////#include <string>
//
///* eat api includes */
//// these are already contained in init.h
////extern "C"{
////#include "eat_modem.h"
////#include "eat_interface.h"
////}
//
///* our api includes */
////#include "date_time.h"
////#include "checksum.h"
////#include "uart_listener.h"
////#include "dart.h"
////#include "protocol\BaseProtocol.h"
//
////using namespace std;
//
////char mepsan_magic_char = 0;	/* This character is used to control the mode of operation */
//
////extern Uart uart[3];
//
//command_enum command_;
//pump_status_enum status_;
//
////unsigned char price_update_bcd[MAX_NON*MAX_NOP][3];
//
////dart(){
//    pumpno = 0;
//    addr = 0x4F + pumpno;
//
//	command = GETSTATUS;
//	trans = 0;
//	lng = 0;
//	ctrl = 0;
//	TX = 0;
//	crc1 = 0;
//	crc2 = 0;
//
//	preset_vol = 0;
//	preset_amo = 0;
//	price_update = 0;
//
//	full_msg_size =0;
//
//	//set these before calling send (SET_MAMO)
//	mamo_update = 0;
//	vol_totalizer = 0;
//	dpamo = 0;	//0-8
//	dpvol = 0;
//	dpunp = 0;
//
//	resp = NOREPLY;
//	r_addr = 0; //response addr
//	r_ctrl = 0;	//response ctrl character
//	r_trans = 0;
//	r_pumpno = 0;		//pump id i.e. which pump
//	r_nozzle = 0x01;
//	r_lng = 0;
//	r_alarm_code = 0;
//
//	r_TX = 0;
//	r_suspend = 0;
//	r_resume = 0;
//
//	//	r_dpvol = 0;
//	//	r_dpamo = 0;
//	//	r_dpunp = 0;
//	//
//	//	r_dptol = 1;
//
//	filling_price=0;
//	vol_totalizer=0;
//	filled_vol = 0;
//	filled_amo = 0;
//
//	filling_start_time = 0;
//	filling_duration = 0;
//
//	status = STATUS_UNKNOWN;
//	nozzle_state = NOZZLE_DOWN;
//	state = PUMP_STATUS_UNKNOWN;
//
//
//	setPriceFlag = false;
////	data_state = D_NILL;
//
////	setDecimalPoints(3, 2, 1, 1);
////
////}
//
///****************************************************************************************
// * SEND_COMMAND takes in human readable commands and make an array of hex to send to the pump
// * >> com: an enum that contains the type of commands it can handle
// **************************************************************************************/
//// Function to send a response
//void send_response(command_enum com)
//{
//
//	unsigned char status_;
//	uint16_t crc;
//
//	addr = 0x4F + pumpno;    //00H -> FFH
//	// TX &= 0x0F;
//	TX = TX & 0x0F;
//	// ctrl = (TX & 0x0F);
//	ctrl = TX;
//
//	if (com == ACK || com == NACK || com == ACKPOLL || com == EOT)
//	{
//		memset(DART_BUFF1, 0, sizeof(DART_BUFF1));
//	 	DART_BUFF1[0] = addr;
//
//		switch (com)
//		{
//			//fOR THE BASIC ctrls
//
//			case ACK:   //C0H -> CFH
//				ctrl = 0xC0 | r_TX;	//reply with the previous msg tx
//				break;
//			case NACK:   //50H -> 5FH
//				ctrl = 0x50 | r_TX;
//				break;
//			case ACKPOLL:    //E0H -> EFH
//				ctrl = 0xE0 | r_TX;
//				break;
//			case EOT:   // 70H -> 7FH
//				ctrl = 0xE0 | r_TX;
//				break;
//			default:
//				break;
//		}
//		DART_BUFF1[1] = ctrl;
//		DART_BUFF1[2] = SF;
//	}
//
//	else if (com == PUMP_NOT_PROGRAMMED || com == RESET || com == AUTHORIZED || com == FILLING || com == FILLING_COMPLETED || com == MAX_AMOUNTVOLUME_REACHED || com == SWITCHED_OFF)
//	{
//		//---- Handles command for sending of data to controller -----//
//
//		ctrl |= 0x30 ;
//		trans = 0x01;
//		lng = 0x01;
//		memset(DART_BUFF1, 0, sizeof(DART_BUFF1));
//
//	 	DART_BUFF1[0] = addr;
//		DART_BUFF1[1] = ctrl;
//		DART_BUFF1[2] = trans;
//		DART_BUFF1[3] = lng;
//
////		pump_status_enum status_;
//
//		switch (com)
//		{
//			//for the Pump-Status Commands
//			case PUMP_NOT_PROGRAMMED		    :	{status_ = 0x00; break;}
//			case RESET 							:	{status_ = 0x01; break;}
//			case AUTHORIZED 					:	{status_ = 0x02; break;}
//			case FILLING						:	{status_ = 0x04; break;}
//			case FILLING_COMPLETED				:	{status_ = 0x05; break;}
//			case MAX_AMOUNTVOLUME_REACHED		:	{status_ = 0x06; break;}
//			case SWITCHED_OFF					:	{status_ = 0x07; break;}
//			default								: 	break;
//		}
//
//		DART_BUFF1[4] = status_;
////		uint16_t crc = calculate_crc(DART_BUFF1, 5);
//		crc = crc_16(DART_BUFF1, 5);
//		DART_BUFF1[5] = crc & 0x00FF;
//		DART_BUFF1[6] = crc >> 8;
//		DART_BUFF1[7] = ETX;
//		DART_BUFF1[8] = SF;
//	}
//
//	else if (com == FILLED_VOLUME_AND_AMOUNT)
//	{
//
//		float vol_ = 1550.1234;
//		float amo_ = 15.1234;
//		int decimalPlaces = 2;
//		double roundedNum;
//		int num_;
//		// unsigned int bcd;
//		unsigned char bcd_[10] = {0};  // Array to hold the BCD result
//
//		uint16_t crc;
//
//		ctrl |= 0x30 ;
//		trans = 0x02;
//		lng = 0x01;
//
//		memset(DART_BUFF1, 0, sizeof(DART_BUFF1));
//
//	 	DART_BUFF1[0] = addr;
//		DART_BUFF1[1] = ctrl;
//		DART_BUFF1[2] = trans;
//		DART_BUFF1[3] = lng;
//
//		roundedNum = roundUp(vol_, decimalPlaces);
//		roundedNum = roundedNum * 100;
//		num_ = (int)(roundedNum);
//
//		int_to_bcd(num_, bcd_);
//
//		// for(uint8_t i = 0; i < 4; i++)
//		for (uint8_t i = 0, j = 3; i < 4; i++, j--)
//		{
//			DART_BUFF1[i + 4] = bcd_[j];
//		}
//
//		roundedNum = roundUp(amo_, decimalPlaces);
//		roundedNum = roundedNum * 100;
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
////		crc = calculate_crc(DART_BUFF1, 12);
//		crc = crc_16(DART_BUFF1, 12);
//		DART_BUFF1[12] = crc & 0x00FF;
//		DART_BUFF1[13] = crc >> 8;
//		DART_BUFF1[14] = ETX;
//		DART_BUFF1[15] = SF;
//	}
//
//	else if (com == NOZSTATUS_AND_FILLINGPRICE)
//	{
//
//		float fillingPrice = 1550.1234;
//		int decimalPlaces = 2;
//		double roundedNum;
//		int num_;
//		unsigned char bcd_[10] = {0};  // Array to hold the BCD result
//
//		uint16_t crc;
//
//		ctrl |= 0x30 ;
//		trans = 0x01;
//		lng = 0x01;
//
//		memset(DART_BUFF1, 0, sizeof(DART_BUFF1));
//
//	 	DART_BUFF1[0] = addr;
//		DART_BUFF1[1] = ctrl;
//		DART_BUFF1[2] = trans;
//		DART_BUFF1[3] = lng;
//
//		roundedNum = roundUp(fillingPrice, decimalPlaces);
//		roundedNum = roundedNum * 100;
//		num_ = (int)(roundedNum);
//
//		int_to_bcd(num_, bcd_);
//
//		for (uint8_t i = 0, j = 2;  i < 3; i++, j--)
//		{
//			DART_BUFF1[i + 4] = bcd_[j];
//		}
//
//		uint8_t nozNum, nozStatus, nozIO;
//
//		// nozIO = nozNum;
//		nozStatus = (nozStatus << 4);
//		nozIO = nozNum | nozStatus;
//
////		crc = calculate_crc(DART_BUFF1, 7);
//		crc = crc_16(DART_BUFF1, 7);
//		DART_BUFF1[7] = crc & 0x00FF;
//		DART_BUFF1[8] = crc >> 8;
//		DART_BUFF1[9] = ETX;
//		DART_BUFF1[10] = SF;
//	}
//
//	else if (com == PUMP_IDENTITY)
//	{
//		float fillingPrice = 1550.1234;
//		int decimalPlaces = 2;
//		double roundedNum;
//		int num_;
//		unsigned char bcd_[10] = {0};  // Array to hold the BCD result
//
//		uint16_t crc;
//
//		ctrl |= 0x30 ;
//		trans = 0x01;
//		lng = 0x01;
//
//		memset(DART_BUFF1, 0, sizeof(DART_BUFF1));
//
//	 	DART_BUFF1[0] = addr;
//		DART_BUFF1[1] = ctrl;
//		DART_BUFF1[2] = trans;
//		DART_BUFF1[3] = lng;
//
//		roundedNum = roundUp(fillingPrice, decimalPlaces);
//		roundedNum = roundedNum * 100;
//		num_ = pump_id;
//
//		int_to_bcd(num_, bcd_);
//
//		for (uint8_t i = 0, j = 4;  i < 5; i++, j--)
//		{
//			DART_BUFF1[i + 4] = bcd_[j];
//		}
//
////		crc = calculate_crc(DART_BUFF1, 9);
//		crc = crc_16(DART_BUFF1, 9);
//		DART_BUFF1[9] = crc & 0x00FF;
//		DART_BUFF1[10] = crc >> 8;
//		DART_BUFF1[11] = ETX;
//		DART_BUFF1[12] = SF;
//		}
//}
//
//void parse_message(unsigned char* arr, int size){
//	memset(r_raw_data1, 0, sizeof(r_raw_data1));
//	r_addr = arr[0];
//	r_pumpno = r_addr-0x4F;			//pumpno i.e either pump 1 or 2 on
//	r_ctrl = arr[1];				//control character that specifies the type of message received
//	r_TX = r_ctrl & 0x0F;		//the TX of the received message (from slave) attached to the ctrl character
//	// r_trans = arr[2];				//received transaction ID
//	// r_lng = arr[3];					//length of data byte
//
//	if (r_pumpno == pumpno)
//	{
//		for (int16_t i = 2, j = 0; i < 150; i++, j++)
//		{
//			r_raw_data1[j] = arr[i];	//shift the data in the array into the r_raw_data vector
//			if( (arr[i] == ETX) && (arr[i+1] == SF) )
//			{
//				break;
//			}
//		}
//		process_data();
//	}
//	else
//	{
////		TRACE_DART("<%s> Not for me Pump[%d]<>", __FUNCTION__, pumpno);
//		resp = NOREPLY;
//	}
//	//include a condition to ensure that the message parsed is more than a particular value
//}
//
////function overload of the parsed message to parse ack, nack, eot messages
//void parse_message1(unsigned char* arr){
//	r_raw_data.clear();
//
//	r_addr = arr[0];
//	r_pumpno = r_addr-0x4f;	//pumpno i.e either pump one or two
//	r_ctrl = arr[1];		//control character that specifies the type of message received
//	r_TX = r_ctrl & 0x0f;	//the TX of the received message attached to the ctrl character
//
////	TRACE_DART("<%s> %x, %x\r\n",__FUNCTION__, r_addr, r_ctrl);
//	process_data();
//}
//
////process_data: get the r_raw_data and process into the different flags and data
//// return value(array) of data parsed, i.e. the data itself
//
//void process_data(){
//
//	int size = r_raw_data.size();
//	// TRACE_DART("<%s>  r_ctrl: [%x]\n", __FUNCTION__,  r_ctrl);
//	int a = 0;
//	u8* r_raw_data_arr;
//	switch (r_ctrl & 0xF0)
//	{
//
//		case 0x30:	//if control character == 0x30
//		// TRACE_DART("<%s>-- [reply_transaction]: %x\n",__FUNCTION__ ,  r_raw_data[0]);
//		//loop through all characters in the message, find the characters of interest then move to the next set of characters
//		for (int i = 0; i < 150; i++){
//			//check the r_trans and r_lng byte ... NB: r_trans = r_raw_data[i] and r_lng = r_raw_data[i+1]
//			//returns status at the request of status or change of status
//
//			//Master's Commands to the Pump
//			if( (r_raw_data1[i] == 0x01) && (r_raw_data1[i+1] == 0x01) )  //Trans-No  & Length
//			{
//				//this is a Master's Command
//				switch(r_raw_data1[i+2])
//				{
//						//for the commands, ctrl = 0x30; RETURN_PUMP_PARAM RETURN_PUMP_IDENTITY  SWITCH_OFF
//					case 0x00	:	{command_ = GETSTATUS; break;}
//					case 0x02  	:	{command_ = RETURN_PUMP_PARAM; break;}
//					case 0x03  	:	{command_ = RETURN_PUMP_IDENTITY; break;}
//					case 0x04  	:   {command_ = REQUEST_FILLING_INFO; break;}
//					case 0x05 	:	{command_ = RESET; break;}
//					case 0x06 	:	{command_ = AUTHORISE; break;}
//					case 0x08 	:	{command_ = STOP; break;}
//					case 0x0A 	:	{command_ = SWITCH_OFF; break;}
//					default: break;
//				}
//				resp = DATA_COMMAND;
//				// TRACE_DART("<>--- msg - STATUS\n");
//				i += 3; //no. of data(1) plus 2 for the r_trans and r_lng
//				a++; //to count if this iteration enters any of the conditions
//			}
//
//			//0x005138 02 08  00 00 00 33  00 00 04 78   03 04 00 14 50 11   1E 0C 03 FA
//
//			//Returns filled data and volume after requesting filling information
//			//==== PRESET VOLUME ====//
//			if( (r_raw_data1[i] == 0x03) && (r_raw_data1[i+1] == 0x04) ) //Trans-No  & Length
//			{
//				//
//				uint16_t preset_voll = 0;
//				uint8_t preset_vol[4];
//				char preset_volll[20];
//				for (uint8_t j = 0; j < 3; j++)
//				{
//					preset_vol[j] = r_raw_data1[i+2+j];
//					// r_vol[j] = r_raw_data[i+2+j];
//					// r_amo[j] = r_raw_data[i+2+j+4];
//					//pick the next byte as the first byte of filling price
//				}
//
//				preset_voll = (preset_vol[0] * 1000000) + (preset_vol[1] * 10000) + (preset_vol[2] * 100) + (preset_vol[3] * 1);
//				sprintf(preset_volll, "0x%ld", preset_voll);
//
//				// char num[]="0x3076";
//				long n = strtol(preset_volll, NULL, 16);
//				// printf("n=%ld\n", n);  // prints 12406
//
//				   // uint32_t bcd = 0x12345678; // Example BCD value
//
//				// uint32_t bcd = 305419896; // Example BCD value
//				// uint32_t decimal = packed_bcd_to_decimal(bcd);
//				uint32_t preset_vol0 = packed_bcd_to_decimal(n);
//				// printf("Decimal: %llu\n", decimal);
//				// return 0;
//				// filled_vol = BCD2decimal(r_vol, 4, r_dpvol);
//				// filled_amo = BCD2decimal(r_amo, 4, r_dpamo);
//
//				resp = DATA_PRESET_VOL;
//				// TRACE_DART("<>--- msg - vol_amo\n");
//				// TRACE_DART("raw amo:%s  raw vol:%s",r_amo,r_vol);
//				//TRACE_DART("[%s] vol:%.3f dp.vol:%d amo:%.3f dp.amo:%d",__FUNCTION__,filled_vol,r_dpvol,filled_amo,r_dpvol);
//				i += 6; //no. of data(8) plus 2 for the r_trans and r_lng
//				a++;
//			}
//
//			//filling price and nozzle status is returned when you request status info or filling info
//			if(r_raw_data[i] == 0x04 && r_raw_data[i+1] == 0x04)
//			{
//				uint16_t preset_amtt = 0;
//				uint8_t preset_amt[4];
//				char preset_amttt[20];
//				for (uint8_t j = 0; j < 3; j++)
//				{
//					preset_amt[j] = r_raw_data1[i+2+j];
//				}
//
//				preset_amtt = (preset_amt[0] * 1000000) + (preset_amt[1] * 10000) + (preset_amt[2] * 100) + (preset_amt[3] * 1);
//				sprintf(preset_amttt, "0x%ld", preset_amtt);
//
//				long n = strtol(preset_amttt, NULL, 16);
//
//				uint32_t preset_amt0 = packed_bcd_to_decimal(n);
//
//				resp = DATA_PRESET_AMO;
//
//				i += 6; //no. of data(8) plus 2 for the r_trans and r_lng
//				a++;
//			}
//		case 0xC0	: 	resp = r_ACK;
//						// TRACE_DART("<>--- msg - ack\n");
//						TX ++;
//						break;//increase the tx on receiving an ACK
//		case 0x50	:	resp = r_NACK;
//						// TRACE_DART("<>--- msg - nack\n");
//						TX=0;
//						break;
//		case 0x70	:	resp = r_EOT;
//						// TRACE_DART("<>--- msg - eot\n");
//						break;
//		default		:
//						resp = JUNK;
//						break;
//		//if (TX >= 0x0f) TX = 0;
//		//r_raw_data.size();
//	}//switch ctrl
//}
//
//
//
//
///***********************************************************************
// *
// *Functions common to all protocols
// ***********************************************************************/
//
//void cmd_update_price(float price){
//	price_update = price * powr(10, r_dpunp);
//	send_command(PRICE_UPDATE);
////	TRACE_DART("<cmd: %s> dpp: %d, price_update = %d", __FUNCTION__, r_dpunp, price_update); //this shows the price to update to in int with decimal(float) to BCD
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
//}
//
//void cmd_switch_off(){
//	send_command(SWITCH_OFF);
////	TRACE_DART("<cmd: %s> SWITCH OFF pump %d", __FUNCTION__, pumpno);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
//}
//
//void cmd_reset(){
//	send_command(RESET);
////	TRACE_DART("<cmd: %s>  pump %d", __FUNCTION__, pumpno);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
//}
//
//void cmd_get_decimal_points(){
//	send_command(RETURN_PUMP_PARAM);
////	TRACE_DART("<cmd: %s> %d dpv, %d dpa, %d dpp", __FUNCTION__,r_dpvol, r_dpamo, r_dpunp);
//	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
//}
//
//void cmd_get_totalizer(){
////	send_command(REQUEST_VOL_TOTAL_COUNT);
//////	TRACE_DART("<----------cmd: %s> totalizer: %lf", __FUNCTION__ ,vol_totalizer);
////	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
//}
//
//void cmd_get_filling_info(){
////	send_command(REQUEST_FILLING_INFO);
//////	TRACE_DART("<cmd: %s> filling price: %f", __FUNCTION__, filling_price);
////	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
//	//returns nozzle state as well
//}
//
//void cmd_get_status(){
////	send_command(GETSTATUS);
//////	TRACE_DART("<cmd: %s> GET_STATUS pump %d| tz: %f, pl: %f, %d dpv", __FUNCTION__, pumpno, vol_totalizer, filling_price, r_dpvol);
////	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
//}
//
//void cmd_set_vol(float vol){
////	preset_vol = vol * powr(10, r_dpvol);
////	send_command(PRESET_VOL);
//////	TRACE_DART("<cmd: %s> preset vol: %d", __FUNCTION__ , preset_vol);
////	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
//}
//
//void cmd_set_amo(float amo){
////	preset_amo = amo * powr(10, r_dpamo);
////	send_command(PRESET_AMO);
//////	TRACE_DART("<cmd: %s> preset amo: %d", __FUNCTION__ , preset_amo);
////	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
//}
//
//void cmd_authorise(){
////	send_command(AUTHORISE);
//////	TRACE_DART("<cmd: %s>", __FUNCTION__);
////	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
//}
//
//void cmd_stop(){
////	send_command(STOP);
//////	TRACE_DART("<cmd: %s>", __FUNCTION__);
////	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
//}
//
//void cmd_poll(){
////	send_command(POLL);
//////	TRACE_DART("<cmd: %s>", __FUNCTION__);
////	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
//}
//
//void cmd_ack(){
////	send_command(ACK);
////	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size, 30);
//}
//
//void cmd_nack(){
////	send_command(NACK);
////	send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
//}
//
////combine status and nozzle state to get a single high level status
//pump_state_enum getStateInput(){
//
//	//nozzle out or nozzle up == 0x11
//	//	if (nozzle_num && (nozzle_num != (r_nozzle & 0x0f)) ) return 0;
//	//	if (!nozzle_num || (nozzle_num == (r_nozzle & 0x0f)) )
//	//	{
//	//		//evaluate nozzle state if 1. nozzle_num is not set, 2. if nozzle_num is set and equals the bit 0-3 of r_nozzle
//	//		if ((r_nozzle & 0xf0) == 0x00)
//	//			nozzle_state = NOZZLE_DOWN;
//	//		else if((r_nozzle & 0xf0) == 0x10)
//	//			nozzle_state = NOZZLE_UP;
//	//	}
//
//	//HIGH LEVEL status for interacting with other Modules
//	//nozzle in or nozzle down == 0x01
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
//}
//
//
///* This function handles responses from the pump that requires regular reply */
////tempted to use a recursion here, for instances in a situtaion where GO's message is nack
//int handle_resp(command_enum cmd){
//	//	s;
//	int cnt = 4;
//	int nack_cnt = 3;
//	int no_etx_cnt = 3;
//	int no_crc_cnt = 3;
//	int junk_cnt = 3;
//	int ret = -99;
//	bool leave_loop = false;
//
////	TRACE_DART("<%s> resp: %d", __FUNCTION__, resp);
//	//send these commands based on the reply from the main command
//	//retry this command as long as
//
//	while (!leave_loop){
//		//RESP is updated after calling each of the cmd below
////		TRACE_DART("<%s> [while loop] resp: %d, cnt: %d, nack: %d, crc_cnt: %d, etx_cnt: %d", __FUNCTION__, resp, cnt, nack_cnt, no_crc_cnt, no_etx_cnt);
//		switch(resp){
//		case r_ACK:
//			cmd_poll();
//			ret = 0;
//			break;
//		case r_NACK:		//this is suppose to resend the last message up to three times
//			if (--nack_cnt > 0){
//				send_command(cmd);
//				send_get_uart_msg((u8*)full_msg_arr, (u32)full_msg_size);
//			}
//			else{
//				cmd_poll();
//			}
//			ret = -10;
//			break;
//		case NO_ETX:
//			if (--no_etx_cnt>0)
//				cmd_nack();
//			else{
//				cmd_ack();
//				cmd_poll();
//			}
//			ret = -1;
//			break;
//
//		case CRC_ERROR:
//			if (--no_crc_cnt>0){
//				cmd_nack();
//			}
//			else{
//				cmd_ack();
//				cmd_poll();
//			}
//			ret = -2;
//			break;
//
//		case JUNK:
//			if (--junk_cnt > 0)
//				cmd_nack();
//			else{
//				cmd_ack();
//				cmd_poll();
//			}
//			ret = -3;
//			break;
//		case NOREPLY:
//			cmd_poll();
//			--cnt;
//			break;
//		case r_EOT:
//			leave_loop = true;
//			TRACE_DART("<%s> EOT !", __FUNCTION__);
//			break;
//		default:		// if it is any other response i.e. data response to command
//			cmd_ack();
//			//			cmd_poll();
//			break;
//		}
//
//		if(cnt == 0){
//			leave_loop = true;
//			TRACE_DART("<%s> NORELY x 3!", __FUNCTION__);
//		}
//	}
//
//	if (cnt == 0){	//handle any other unfortunate error that refuses to be fixed
//		cmd_ack();
//		cmd_poll();
//		//		if (resp == CRC_ERROR || resp == NO_ETX){
//		//
//		//		}
//	}
//
//
//	if (resp != NOREPLY){	//setting time in order to get the no resp range
//		setResponseTime();
//	}
//
//	return ret;
//}
//
///***************************************************************************
// *
// * implementing the BaseProtocol virtual functions
// *
// ***************************************************************************/
//
///* main program API
// * These ones interact with the global variables only
// * */
//
//u8 getAddr(){
//	return pumpno;
//}
//
//void setAddr(u8 i){
//	pumpno = i;
//}
//
//
////void dart::wifiAuthPumpByAmo(float amo){
////	wifiAuthAmo = amo;
////	wifiAuth = true;
////}
//
///* protocol thread API */
////consider includng a return value here to show if the command was responded to well
//void PNP_setup(){
//	;
//}
//
//void get_status(){
////	cmd_get_status();
////	handle_resp(GETSTATUS); //another command is sent here in this function
//}
//
//void get_unit_price(){
////	cmd_get_filling_info();	//this command returns, volume, amount, price and nozzle state
////	handle_resp(REQUEST_FILLING_INFO);
//}
//
//void set_unit_price(float price){
////	cmd_update_price(price);
////	handle_resp(PRICE_UPDATE);
////
////	/* added it here beceause of sslan */
////	setPriceFlag = false;
//}
//
//void get_totalizer(){
////	cmd_get_totalizer();
////	handle_resp(REQUEST_VOL_TOTAL_COUNT);
//}
//
//float get_filled_vol(){
////	cmd_get_filling_info();
////	handle_resp(REQUEST_FILLING_INFO);
////	return filled_vol;
//}
//
//float get_filled_amo(){
////	cmd_get_filling_info();
////	handle_resp(REQUEST_FILLING_INFO);
////	return filled_amo;
//}
//
//void get_filling_info(){
////	cmd_get_filling_info();
////	handle_resp(REQUEST_FILLING_INFO);
//}
//
//int auth_by_vol(float vol){
////	int ret = 99;
////	int cnt = 4;
////
////	while (ret != 0 && --cnt){ //retry this 3 times and hope to get a return value of 0, i.e. try to send the preset vol up to three times
////		cmd_set_vol(vol);
////		ret = handle_resp(PRESET_VOL);
////		TRACE_DART("<%s> RET of cmd: %d ", __FUNCTION__, ret);
////	}
////
////	if (ret == 0 || mepsan_magic_char){	//if it got reply well
////		cmd_authorise();
////		handle_resp(AUTHORISE);
////	}
////
////	return ret;
//}
//
////void dart::auth_by_amo(float amnt){
////	cmd_set_amo(amnt);
////	handle_resp(PRESET_AMO);
////
////	cmd_authorise();
////	handle_resp(AUTHORISE);
////}
//
////to return if the process was success full
//int auth_by_amo(float amnt){
////	int ret = 99;
////	int cnt = 4;
////
////	while (ret != 0 && --cnt){ //retry this 3 times and hope to get a return value of 0, i.e. try to send the preset vol up to three times
////		cmd_set_amo(amnt);
////		ret = handle_resp(PRESET_AMO);
////		TRACE_DART("<%s> RET of cmd: %d ", __FUNCTION__, ret);
////	}
////
////	if (ret == 0 || mepsan_magic_char){	//if it got reply well
////		cmd_authorise();
////		handle_resp(AUTHORISE);
////	}
////
////	return ret;
//}
//
//int set_amo(float amnt){
////	int ret = 99;
////	int cnt = 4;
////
////	while (ret != 0 && --cnt){ //retry this 3 times and hope to get a return value of 0, i.e. try to send the preset vol up to three times
////		cmd_set_amo(amnt);
////		ret = handle_resp(PRESET_AMO);
////		TRACE_DART("<%s> RET of cmd: %d ", __FUNCTION__, ret);
////	}
////	return ret;
//}
//
////void dart::authorise(){
////	cmd_set_vol(MAX_PRESET_VOL);
////	handle_resp(PRESET_VOL);
////
////	cmd_authorise();
////	handle_resp(AUTHORISE);
////}
//
//void authorise(){
////	int ret = 99;
////	int cnt = 4;
////
////	while (ret != 0 && --cnt){ //retry this 3 times and hope to get a return value of 0, i.e. try to send the preset vol up to three times
////		cmd_set_vol(MAX_PRESET_VOL);
////		ret = handle_resp(PRESET_VOL);
//////		TRACE_DART("<%s> RET of cmd: %d ", __FUNCTION__, ret);
////	}
////
////	if (ret == 0 || mepsan_magic_char){	//if it got reply well
////		cmd_authorise();
////		handle_resp(AUTHORISE);
////	}
//}
//
//
//void stop(){
////	cmd_stop();
////	handle_resp(STOP);
//}
//
//void pause() {
//
//}
//
//void resume() {
//
//}
//
//void switch_off(){
////	cmd_switch_off();
////	handle_resp(SWITCH_OFF);
//}
//void reset(){
////	cmd_reset();
////	handle_resp(RESET);
//}
//
////Had to override this function in order to enable setting a pump which is not responding to unknown status
////void calLastResponseTime(){
////	respTime = eat_get_duration_ms(lastRespTime);
////	if (respTime > NO_RESP_TIMEOUT){
////		status = STATUS_UNKNOWN;
////	}
////	//	TRACE_DART("TIME SINCE LAST RESPONSE: %d", respTime);
////}
//
//
////void uart_setup(){
////
////	if (uart_use_pump_def){
////		set_uart_ptr(&uart[0]);
////		p_uart->port=  EAT_UART_1;
////		p_uart->config.baud= EAT_UART_BAUD_9600;
////		p_uart->config.parity= EAT_UART_PARITY_ODD;
////		/** other parameters has been set by default in uart constructor */
////	}
////
//////	TRACE_DART("<%s> - dart", __FUNCTION__);
////	eat_pin_set_mode(RE_DE, EAT_PIN_MODE_GPIO);
////	eat_gpio_setup(RE_DE, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);
////
////	p_uart->init();
////}
////
////void uart_send(u8* write_buff, u32 size){
////	//Pull ~RE/DE high in order to write to max485
////	eat_gpio_write(RE_DE, EAT_GPIO_LEVEL_HIGH);
////	//	eat_sleep(5);
////
////	u16 len_gpt = p_uart->writed((char*)write_buff, size);
////
////	//	u16 gpt_coef = 0;	//gpt coefficient
////	//	if (len_gpt > 5 && len_gpt < 15) gpt_coef = 25*len_gpt;
////	//	else if (len_gpt >= 15) gpt_coef = 30*len_gpt;
////	//	else gpt_coef = 18*len_gpt;
////	//
////	//	eat_gpt_start(gpt_coef, EAT_FALSE, eat_gpt_cb_fun);//RE_DELAY*16384 //((18 x length_of_byte)*62) this is the time it takes for the uart data to reach the pump
////	//	TRACE_DART("GPT! for last message started - delay: %d ms", (61*gpt_coef)/1000);
////
////	eat_gpt_start(18*len_gpt, EAT_FALSE, eat_gpt_cb_fun);//RE_DELAY*16384 //((18 x length_of_byte)*62) this is the time it takes for the uart data to reach the pump
////	TRACE_DART("GPT! for last message started - delay: %d ms", (18*61*len_gpt)/1000);
////
////	//	eat_gpio_write(RE_DE, EAT_GPIO_LEVEL_LOW);
////}
////
////void uart_complete_event_proc(){
////	//	eat_gpt_start(18,EAT_FALSE, eat_gpt_cb_fun); //61 * 18 = 1000us = 1ms
////	//	TRACE_DART("<%s> - dart, bringing RE_DE pin low to enable reception of data", __FUNCTION__);
////
////	//	TRACE_DART("GPT! for last message started - delay: %d ms", len_gpt);
////	//	eat_sleep(len_gpt);
////	//	eat_gpio_write(RE_DE, EAT_GPIO_LEVEL_LOW);
////
////	//	eat_gpio_write(RE_DE, EAT_GPIO_LEVEL_LOW);
////}
////
////void send_get_uart_msg(unsigned char* write_buf, unsigned int size, s32 wait_time){
////	int ret = 0;
////	u8 rx_buf_l[400] = {0};		//local receive buffer
////	u32 rx_len_l;		//local receive buffer length
////
////	uart_send(write_buf, size);
////	ret = p_uart->waitForResp(wait_time);
////
////	if (ret > 0){
////		eat_sleep(10);
////		rx_len_l = p_uart->readd((char*)rx_buf_l, sizeof(rx_buf_l));	//this function sets
////		receive_response(rx_buf_l, (int)rx_len_l);
////	}
////	else {
////		resp = NOREPLY;
////	}
////
////	//You can put your no_response algorithm here
////}
////
////void send_get_uart_msg(unsigned char* write_buf, unsigned int size)
////{
////	send_get_uart_msg(write_buf, size, MSG_TIMEOUT);
////}
////
////void send_get_uart_msg(){
////	;
////}
//
//
//
//
//
//
//
//// Function to calculate CRC-16
//uint16_t calculate_crc(uint8_t *data, size_t length) {
//    uint16_t crc = 0x0000;    //0xFFFF; // Initial value
//    uint16_t polynomial = 0x1021; // CRC-16-CCITT polynomial
//
//    for (size_t i = 0; i < length; i++) {
//        crc ^= (uint16_t)(data[i] << 8); // XOR byte into the high byte of CRC
//
//        for (int j = 0; j < 8; j++) { // Process each bit
//            if (crc & 0x8000) { // If the high bit is set
//                crc = (crc << 1) ^ polynomial;
//            } else {
//                crc <<= 1;
//            }
//        }
//    }
//
//    return crc;
//}
//
//
//double roundUp(float value, int decimalPlaces)
//{
//    double factor = pow(10, decimalPlaces);
//    return ceil(value * factor) / factor;
//}
//
//// unsigned int decimalToPackedBCD(unsigned int decimal)
//// {
////     unsigned int bcd = 0;
////     int shift = 0;
//
////     while (decimal > 0)
//// 	{
////         // Get the least significant digit
////         int digit = decimal % 10;
//
////         // Shift the digit into the correct place in the BCD result
////         // bcd |= (digit << (shift * 4));
//// 		bcd |= (digit << (shift * 4));
//
////         // Move to the next digit
////         decimal /= 10;
////         shift++;
////     }
//
////     return bcd;
//// }
//
//void int_to_bcd(int num, unsigned char *bcd)
//{
//    int index = 0;
//    while (num > 0) {
//        unsigned char digit = num % 10;
//        if (index % 2 == 0) {
//            bcd[index / 2] = digit;
//        } else {
//            bcd[index / 2] |= (digit << 4);
//        }
//        num /= 10;
//        index++;
//    }
//}
//
//
////parse_message : separate the received message into its different constituent
//parse_message(unsigned char* arr, int size)
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
//
////function overload of the parsed message to parse ack, nack, eot messages
//void parse_message1(unsigned char* arr)
//{
//	//r_raw_data.clear();
//
//	r_addr = arr[0];
//	r_pumpno = r_addr-0x4f;	//pumpno i.e either pump one or two
//	r_ctrl = arr[1];		//control character that specifies the type of message received
//	r_TX = r_ctrl & 0x0f;	//the TX of the received message attached to the ctrl character
//
////	process_data();
//}
//
//uint32_t packed_bcd_to_decimal(uint32_t bcd) {
//     uint32_t decimal = 0;
//     uint32_t multiplier = 1;
//
//    for (int i = 0; i < 8; ++i) {
//        // Extract the rightmost BCD digit (4 bits)
//        uint8_t digit = bcd & 0xF;
//
//        // Convert to decimal and add to the result
//        decimal += digit * multiplier;
//
//        // Move to the next BCD digit
//        bcd >>= 4;
//        multiplier *= 10;
//    }
//
//    return decimal;
//}
