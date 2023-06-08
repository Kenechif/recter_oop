/*
 * drt.c
 *
 *  Created on: may 15, 2022
 *      Author: tunji
 */

#include "drt.h"
#include "stdint.h"

unsigned char pumpno;			//pump no i.e. which pump nozzle

/*************************************************************
	 * Control characters in the WAYNE protocol for sending and receiving messages
	 **************************************************************/
		unsigned char addr;				//address of pump
		unsigned short trans;			//transaction no. .. CD1, CD2 e.t.c
		unsigned char lng;				//length of data
		unsigned char ctrl;				//ctrl character
			unsigned char TX; 			//the LSB (second 4 bit) of the ctrl byte
			//int TX_cnt;

		unsigned char crc1;
		unsigned char crc2;
		unsigned char full_msg[200];	//stores the full_msg to send
		char full_msg_arr[200];
			int full_msg_size;

			//set these before calling send (SET_MAMO) 99999
		int preset_vol;
		int preset_amo;
		int price_update;		//
		int mamo_update;
		unsigned char dpvol;	//0-8	no. of decimals you can have there
		unsigned char dpamo;	//0-8
		unsigned char dpunp;	//0-4

	/*********************************************************************
	 *	Control characters in the WAYNE protocol for PUMP RESPONSE MESSAGE
	 **********************************************************/
		unsigned char r_addr; 			//response addr
		unsigned char r_ctrl;			//response ctrl character
		unsigned char r_pumpno;			//pump no i.e. which pump nozzle
		unsigned char r_trans;			//transaction no. CD1, CD2 e.t.c
		unsigned char r_lng;			//length of data received in message
		unsigned char r_nozzle;			//nozzle status
		unsigned char r_TX;				//TX character attached to the ctrl byte
		unsigned char r_alarm_code;
		unsigned char r_suspend;
		unsigned char r_resume;

		/* Variables for usable raw data from the pump */
		unsigned char r_vol[4];
		unsigned char r_amo[4];
		unsigned char r_filling_price[3];		//filling price
		unsigned char r_MAMO[4];
		unsigned char r_pump_id[5];
		unsigned char r_total_volume[15];		//total volume

		static unsigned char price_update_bcd[MAX_NOP*MAX_NON][3];

		unsigned char r_raw_data[200];	//raw data received

//		/* calculated info from pump data above */
//		float filling_price;
//		double totalizer;
//		float filled_vol;


void receive_response(unsigned char* buffer, int size){
	int addr = 0x4f + pumpno;
	//test auth	: calculate the crc (crc1, crc2) and test with the received crc (r_crc1, r_crc2)
	int ADDR_index = 0;
	int SF_index = 0;
	char buf_string[60] = {0};
	unsigned char r_crc1, r_crc2;	//crc from the pump received with the message

	//look for addr index 0x4F+2
	for (int i=0; i<size; i++){
		if (buffer[i] == addr){
			ADDR_index = i;
			buffer+= ADDR_index;
			break;
		}
		else{
			ADDR_index = -i;
		}
	}

	//look for the stop flag 0xfa
	for (int i=0; i<size; i++){
		if (buffer[i] == SF){		//SF = 0xfa
			SF_index = i;
		}
	}

	//if can't find addr or sf goto end
	if (ADDR_index == -1 || SF_index == 0){
		//		resp = JUNK;
		goto end;
	}

	if (buffer[SF_index-1] == SF){
		SF_index-=1;		//this is used to fix the bug that didn't see etx on the second to the last character but on the third to the last
	}

	if (SF_index!=0){			//if SF_index is present
		if(SF_index > 5){				//if it is a data message and not ack or nack
			if (buffer[SF_index-1]	== DLE)		//check if there is a DLE character then excempt it from the calculation
				SF_index-=1;

			if (buffer[SF_index-1] == ETX){	//if ETX is before SF

				r_crc2 = buffer[SF_index-2];	//	get response crc at the 2nd place from the SF
				r_crc1 = buffer[SF_index-3];

				get_crc(buffer, (SF_index-3));	//get the buffer and the size

				if (r_crc1 == crc1 && r_crc2 == crc2){
					parse_message(buffer, (SF_index-3));
				}
				else
					resp = CRC_ERROR;
			}
			else
				resp = NO_ETX;		//NO_etx error transmission 0x03
		}
		else if (SF_index == 2)	//if it is an ack, nack e.t.c
			parse_message(buffer);//call the second parse message for ack, nack and poll
	}

	else	{//can't find SF in message //if can't find stop character and size > 1, i.e it is not a 00 and probably
		end:
		for (int i=0; i<size; i++){
			if (buffer[i] == 0){
				resp = NOREPLY;
			}
			else{	//the moment it encounters an unusual character when there is no stop character
				resp = JUNK;
				break;
			}
		}
		//TRACE_DART(" <%s>!! NO REPLY or NO REASONABLE REPLY", __FUNCTION__);
	}
}
//parse_message : separate the received message into its different constituent
parse_message(unsigned char* arr, int size){
	//r_raw_data.clear();
	r_addr = arr[0];
	r_pumpno = r_addr-0x4f;			//pumpno i.e either pump 1 or 2 on
	r_ctrl = arr[1];				//control character that specifies the type of message received
	r_TX = r_ctrl & 0x0f;		    //the TX of the received message (from slave) attached to the ctrl character
	//r_trans = arr[2];				//received transaction ID
	//r_lng = arr[3];					//length of data byte

	//TRACE_DART(" <%s>-- : r_addr: %x, r_ctrl: %x, r_lng: %x \n", __FUNCTION__, r_ctrl, arr[0], r_lng);

	if (r_pumpno == pumpno)	{
		for (int i=2; i<size; i++){
			//r_raw_data.push_back(arr[i]);	//shift the data in the array into the r_raw_data vector
		}
	//	process_data();
	}
	else{
		//TRACE_DART("<%s> Not for me Pump[%d]<>", __FUNCTION__, pumpno);
		resp = NOREPLY;
	}
	//include a condition to ensure that the message parsed is more than a particular value
}

//function overload of the parsed message to parse ack, nack, eot messages
void parse_message1(unsigned char* arr){
	//r_raw_data.clear();

	r_addr = arr[0];
	r_pumpno = r_addr-0x4f;	//pumpno i.e either pump one or two
	r_ctrl = arr[1];		//control character that specifies the type of message received
	r_TX = r_ctrl & 0x0f;	//the TX of the received message attached to the ctrl character

//	process_data();
}


