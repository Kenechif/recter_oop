/*
 * dart_.c
 *
 *  Created on: 28 Jun 2024
 *      Author: Fidel
 */




#include "dart_.h"
#include "../crc/checksum.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <main.h>
#include <stdbool.h>

int head_posn = 0;

command_enum1 command_,
			  command_2;

uint8_t status1_;

uint8_t configChangeInfo_next1 = 0,
		configChangeInfo_next2 = 0;

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

extern uint8_t status_change_pump2 = 0,
			   status_change_noz2 = 0;

int16_t header = 0,
		footer = 0;

extern uint8_t track_num1 = 0,
			   valid_pair11 = 0,
			   track_num2 = 0,
			   valid_pair12 = 0,
			   track_num1_0 = 0,
			   track_num2_0 = 0;

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

void dart_init(void)
{
    addr = 0x4F + settings_stream1[0].noz_addr;
    addr2 = 0x4F + settings_stream1[1].noz_addr;

    command_ = NO_COMMAND;
    command_2 = NO_COMMAND;
    command_response = false;
    command_response2 = false;

	ctrl = 0;
	ctrl2 = 0;
	TX = 0;
	TX2 = 0;
	crc1 = 0;
	crc2 = 0;

	preset_vol = 0;
	preset_amo = 0;
	price_update = 0;

	dpamo = 0;	//0-8
	dpvol = 0;
	dpunp = 0;

	resp = NOREPLY;
	resp2 = NOREPLY;

	r_addr = 0; //response addr
	r_ctrl = 0;	//response ctrl character
	r_trans = 0;
	r_pumpno = 0;		//pump id i.e. which pump
	r_TX = 0;

	r_addr2 = 0; //response addr
	r_ctrl2 = 0;	//response ctrl character
	r_trans2 = 0;
	r_pumpno2 = 0;		//pump id i.e. which pump
	r_TX2 = 0;
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

	r_addr = MainBuf[header];
	r_pumpno = r_addr - 0x4F;			//pumpno i.e either pump 1 or 2 on
	r_ctrl = MainBuf[header + 1];

	//===  GO's TX#  ===//
	r_TX = r_ctrl & 0x0F;		//the TX of the received message (from slave) attached to the ctrl character

	if (r_pumpno == settings_stream1[0].noz_addr)
	{
		memset(r_raw_data1, 0, sizeof(r_raw_data1));

		for (i = 2, j = 0; i < 125; i++, j++)
		{
			r_raw_data1[j] = MainBuf[header + i];	//shift the data in the array into the r_raw_data vector
			if(r_raw_data1[j] == SF)
			{
				break;
			}
		}

		parse_decode();
	}
	else if (r_pumpno == settings_stream1[1].noz_addr)
	{
		r_addr2 = r_addr;
		r_pumpno2 = r_pumpno;
		r_ctrl2 = r_ctrl;
		r_TX2 = r_TX;

		memset(r_raw_data2, 0, sizeof(r_raw_data2));

		for (i = 2, j = 0; i < 125; i++, j++)
		{
			r_raw_data2[j] = MainBuf[header + i];	//shift the data in the array into the r_raw_data vector
			if(r_raw_data2[j] == SF)
			{
				break;
			}
		}

		parse_decode2();
	}
	else
	{
		resp = NOREPLY;
		resp2 = NOREPLY;
	}
}


void parse_decode(void)
{
	u8* r_raw_data_arr;

	uint8_t data_[50] = {0},
			data1[50] = {0},
			data2[50] = {0};

	uint16_t crc_original,
			 crc_check;

	uint8_t MSN = (r_ctrl & 0xF0);

	switch (MSN)
	{

			case 0x30:	//if control character == 0x30
						// TRACE_DART("<%s>-- [reply_transaction]: %x\n",__FUNCTION__ ,  r_raw_data[0]);
						//loop through all characters in the message, find the characters of interest then move to the next set of characters

							for (uint8_t i = 0; i < 150; i++)
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
									}
									else
									{
										resp = CRC_ERROR;
									}
									i += 3;

									dummyValue = 0;

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
										}

										preset_voll = bcdArray_to_int(preset_vol, 4);

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
									i += 6;

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

										preset_amtt = bcdArray_to_int(preset_amt, 4);

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

									i += 6;

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

										price_updatee = bcdArray_to_int(price_update, 3);

//										price_update1 = (price_updatee * 0.1);

										switch(dp_unitprice1)
										{
											case 0	: { price_update1 = (price_updatee * 1); break; }
											case 1	: { price_update1 = (price_updatee * 0.1); break; }
											case 2	: { price_update1 = (price_updatee * 0.01); break; }
											case 3	: { price_update1 = (price_updatee * 0.001); break; }
											default	: { break; }
										}

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

								//###########################################################################//
								//'50 37 05 03 00 58 10 a2 0a 03 fa
								//===========================================================================//
								//==========================   SET PUMP PARAMETERS   ========================//
								//===========================================================================//
								else if( (r_raw_data1[i] == 0x09) && (r_raw_data1[i+1] == 0x33) ) //51 Data Bytes
								{
									crc_original = r_raw_data1[i+56];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data1[i+55]);


									//==============================================================//
									//==================== VALIDATING THE CRC ======================//

									data_[0] = r_addr;
									data_[1] = r_ctrl;

									////////////////////////////////////////
									//'50 37 05 03 00 58 - - - 10 a2 0a 03 fa
									////////////////////////////////////////
									//=== the read buffer contains only data from the transaction byte to the SF byte ===//
									//================== Data contains a cumulative of 51 raw Bytes ================//
									for(uint8_t ii = 0, j = 2; ii < 53; ii++, j++)  //51 + Trans No. + Data Len
									{
										data_[j] = r_raw_data1[ii];
									}


									crc_check = crc_16(data_, 55);


									if(crc_check == crc_original)
									{
										for (uint8_t j = 0; j < 51; j++)
										{
											set_param1[j] = r_raw_data1[i+2+j];
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

								//###########################################################################//
								//'50 37  66 01  01  ee 37 03 fa
								//===========================================================================//
								//============================ OTP SESSION CLEAR ============================//
								//===========================================================================//
								else if(r_raw_data1[i] == 0x66 && r_raw_data1[i+1] == 0x01)
								{
									crc_original = r_raw_data1[i+4];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data1[i+3]);


									//==============================================================//
									//==================== VALIDATING THE CRC ======================//
									//'50 37 66 01 01 d2 83 03 fa'
									data_[0] = r_addr;
									data_[1] = r_ctrl;

									for(uint8_t ii = 0, j = 2; ii < 3; ii++, j++)   //1 + Trans No. + Data Len
									{
										data_[j] = r_raw_data1[ii];
									}

									crc_check = crc_16(data_, 5);

									if(crc_check == crc_original)
									{
										command_ = CLEAR_OTP_SESSION;

										resp = DATA_CLEAR_OTP_SESSION;

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

								//###########################################################################//
								//'50 37  67 01  01  bf f7 03 fa
								//===========================================================================//
								//============================ CONFIG CHANGE QUERY ==========================//
								//===========================================================================//
								else if(r_raw_data1[i] == 0x67 && r_raw_data1[i+1] == 0x01)
								{
									crc_original = r_raw_data1[i+4];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data1[i+3]);


									//==============================================================//
									//==================== VALIDATING THE CRC ======================//
									//50 37 67 01 01 bf f7 03 fa'
									data_[0] = r_addr;
									data_[1] = r_ctrl;

									for(uint8_t ii = 0, j = 2; ii < 3; ii++, j++)   //1 + Trans No. + Data Len
									{
										data_[j] = r_raw_data1[ii];
									}

									crc_check = crc_16(data_, 5);

									if(crc_check == crc_original)
									{
										command_ = REQUEST_CONFIG_CHANGE_INFO;

										resp = DATA_REQUEST_CONFIG_CHANGE_INFO;

										command_response = true;

										if(data_[4] == 0x00)                       // Controller Requests Current Config-change Item       //Controller fails in sending preceding data to Server
										{
											configChangeInfo_next1 = 0;
										}
										else if(data_[4] == 0x01)  				   // Controller Requests Next Config-change Item         //Controller successfully sends preceding data to server
										{
											configChangeInfo_next1 = 1;
										}

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

								//###########################################################################//
								//
								//res => ep:1a. {"st":0,"tk":31290,"ud":0,"tm":37427375,"am":0.0,
								//"mt":{"ty":3,"pn":"all","pr":590.0,"sh":null,"fg":0,"tg":"p|all"},
								//"pv":0.0,"wv":0.0,"sa":0.0,"bal":0.0,"dc":null,"wb":null,"ft":null}
								//
								///////////////////////////////////////////////////////////////////////////////
								///////////////////////////////////////////////////////////////////////////////
								//'50 37  68 05  12 34 56 78 90  a2 0a 03 fa
								//'50 38  68 05  00 37 42 48 57  CE BC 03 fa
								//===========================================================================//
								//============================   UPDATE DATE/TIME   =========================//
								//===========================================================================//
								else if( (r_raw_data1[i] == 0x68) && (r_raw_data1[i+1] == 0x05) )//5 Data Bytes
								{
									uint8_t update_datetime[5];
									char update_datetimeee[20];

									crc_original = r_raw_data1[i+8];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data1[i+7]);


									//==============================================================//
									//==================== VALIDATING THE CRC ======================//

									data_[0] = r_addr;
									data_[1] = r_ctrl;

									////////////////////////////////////////
									//'50 37  68 05  12 34 56 78 90  a2 0a 03 fa
									////////////////////////////////////////
									//===the read buffer contains only from the transaction byte to the SF byte ===//
									//================== Data contains a cumulative of 5 raw Bytes ===============//
									for(uint8_t ii = 0, j = 2; ii < 7; ii++, j++)  //5 + Trans No. + Data Len
									{
										data_[j] = r_raw_data1[ii];
									}


									crc_check = crc_16(data_, 9);

									if(crc_check == crc_original)
									{
										for (uint8_t j = 0; j < 5; j++)
										{
											update_datetime[j] = r_raw_data1[i+2+j];
										}

										update_date_time = bcdArray_to_int(update_datetime, 5);

										///////////////////////////////////////////////////////////////////
										///////////////////  ACTUATE THE CHANGE... ////////////////////////


										//			if( (priceChange_check > 0.1) || (priceChange_check < -0.1) )
										//			{
														   ttostr(update_date_time, 1);
														   ttostr(update_date_time, 2);
										//			}

										///////////////////////////////////////////////////////////////////


										command_ = DATE_TIME_UPDATE;

										resp = DATA_DATE_TIME_UPDATE;

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

								//##############################################################################//
								//'50 38  69 04  01  20 04 00 46  f0 03 fa '   //Calibration Data Request
								//'50 38  69 04  00  20 04 05 87  0f 03 fa '   //In-bound Calibration-Set Command
								//==============================================================================//
								//=====================   SET/REQUEST CALIBRATION DETAILS   ====================//
								//==============================================================================//
								else if( (r_raw_data1[i] == 0x69) && (r_raw_data1[i+1] == 0x05) ) //5 Data Bytes
								{
									crc_original = r_raw_data1[i+7];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data1[i+6]);

									//==============================================================//
									//==================== VALIDATING THE CRC ======================//

									data_[0] = r_addr;
									data_[1] = r_ctrl;

									////////////////////////////////////////////
									//'50 38  69 04  01  20 04 00 46 f0 03 fa '   //Calibration Data Request
									//'50 38  69 04  00  20 04 05 87 0f 03 fa '   //In-bound Calibration-Set Command
									////////////////////////////////////////////
									//=== the read buffer contains only data from the transaction byte to the SF byte ===//
									//================== Data contains a cumulative of 4 raw Bytes ================//
									for(uint8_t ii = 0, j = 2; ii < 6; ii++, j++)  //4 + Trans No. + Data Len
									{
										data_[j] = r_raw_data1[ii];
									}


									crc_check = crc_16(data_, 8);

									if(crc_check == crc_original)
									{
										if(data_[4] == 0x00)                       //Controller commands to set new Calibration Parameters
										{
											for (uint8_t j = 0; j < 3; j++)
											{
												set_calib1[j] = bcd_to_int(r_raw_data1[i+3+j]);
											}

											resp = DATA_SET_CALIBRATION_PARAM;
										}
										else if(data_[4] == 0x01)  				   //Controller requests Calibration Info
										{
											command_ = REQUEST_CALIBRATION_PARAM;

											resp = DATA_REQUEST_CALIBRATION_PARAM;

											command_response = true;
										}

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

							break;

			case 0xC0	: 	resp = r_ACK;

							MSN = (r_ctrl & 0x0F);
							if(MSN == TX)
							{
//								checked = 1;

								TX++;
								if(TX > 0x0F)
								{
									TX = 0x00;
								}
							}

							command_ = NO_COMMAND;

							command_response = false;

							resp = NOREPLY;

							break;  //increase the tx on receiving an ACK

			case 0x50	:	resp = r_NACK;

							MSN = (r_ctrl & 0x0F);
							if(MSN == TX)
							{

							}

							break;

			case 0x70	:	resp = r_EOT;

							break;
			default		:
							resp = JUNK;
							break;
	}//switch ctrl
}

void parse_decode2(void)
{
	u8* r_raw_data_arr;

	uint8_t data_[50] = {0},
			data1[50] = {0},
			data2[50] = {0};

	uint16_t crc_original,
			 crc_check;

	uint8_t MSN = (r_ctrl2 & 0xF0);

	switch (MSN)
	{

			case 0x30:	//if control character == 0x30

							for (uint8_t i = 0; i < 150; i++)
							{
								//check the r_trans and r_lng byte ... NB: r_trans = r_raw_data[i] and r_lng = r_raw_data[i+1]
								//returns status at the request of status or change of status


								//'50 36 01 01 00 9f d4 03 fa '
								//===========================================================================//
								//============================= COMMAND TO PUMP =============================//
								//===========================================================================//
								if( (r_raw_data2[i] == 0x01) && (r_raw_data2[i+1] == 0x01) )  //Trans-No  & Length
								{
									command_response2 = true;

									//this is a Master's Command
									switch(r_raw_data2[i+2])
									{
											//for the commands, ctrl = 0x30; RETURN_PUMP_PARAM RETURN_PUMP_IDENTITY  SWITCH_OFF
										case 0x00	:	{command_2 = GETSTATUS; break;}
										case 0x02  	:	{command_2 = RETURN_PUMP_PARAM; break;}
										case 0x03  	:	{command_2 = RETURN_PUMP_IDENTITY; break;}
										case 0x04  	:   {command_2 = REQUEST_FILLING_INFO; break;}
										case 0x05 	:	{command_2 = RESET1; break;}   //50 36 01 01 05 9f d4 03 fa
										case 0x06 	:	{command_2 = AUTHORISE; break;}
										case 0x08 	:	{command_2 = STOP; break;}
										case 0x0A 	:	{command_2 = SWITCH_OFF; break;}
										default: break;
									}

									crc_original = r_raw_data2[i+4];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data2[i+3]);

									data_[0] = r_addr2;
									data_[1] = r_ctrl2;

									for(uint8_t i = 0, j = 2; i < 5; i++, j++)
									{
										data_[j] = r_raw_data2[i];
									}


									crc_check = crc_16(data_, 5);

									//////////////////////////
//									crc_check = crc_original;
									//////////////////////////

									if(crc_check == crc_original)
									{
										resp2 = DATA_COMMAND;
										ack_send2 = true;

									}
									else
									{
										resp2 = CRC_ERROR;
									}
									i += 3; //no. of data(1) plus 2 for the r_trans and r_lng

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
								else if( (r_raw_data2[i] == 0x03) && (r_raw_data2[i+1] == 0x04) ) //Trans-No  & Length
								{
									uint32_t preset_voll = 0;
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

										preset_voll = bcdArray_to_int(preset_vol, 4);

										switch(dp_vol2)
										{
											case 0	: { auth_v2 = (preset_voll * 1); break; }
											case 1	: { auth_v2 = (preset_voll * 0.1); break; }
											case 2	: { auth_v2 = (preset_voll * 0.01); break; }
											case 3	: { auth_v2 = (preset_voll * 0.001); break; }
											default	: { break; }
										}

										resp2 = DATA_PRESET_VOL;
										ack_send2 = true;
									}
									else
									{
										resp2 = CRC_ERROR;
									}

									//==============================================================//

									i += 6; //no. of data(8) plus 2 for the r_trans and r_lng

									break;
								}

								//filling price and nozzle status is returned when you request status info or filling info
								//===========================================================================//
								//=============================== PRESET AMOUNT =============================//
								//===========================================================================//
								else if(r_raw_data2[i] == 0x04 && r_raw_data2[i+1] == 0x04)
								{
									uint32_t preset_amtt = 0;
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

										preset_amtt = bcdArray_to_int(preset_amt, 4);

										switch(dp_amount2)
										{
											case 0	: { auth_p2 = (preset_amtt * 1); break; }
											case 1	: { auth_p2 = (preset_amtt * 0.1); break; }
											case 2	: { auth_p2 = (preset_amtt * 0.01); break; }
											case 3	: { auth_p2 = (preset_amtt * 0.001); break; }
											default	: { break; }
										}

										resp2 = DATA_PRESET_AMO;
										ack_send2 = true;
									}
									else
									{
										resp2 = CRC_ERROR;
									}

									i += 6;

									break;
								}

								//###########################################################################//
								//'50 37 05 03 00 58 10 a2 0a 03 fa
								//===========================================================================//
								//========================== PRICE-UPDATE (DP => 1)==========================//
								//===========================================================================//
								else if(r_raw_data2[i] == 0x05 && r_raw_data2[i+1] == 0x03)
								{
									uint32_t price_updatee = 0;
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

										price_updatee = bcdArray_to_int(price_update, 3);

//										price_update2 = (price_updatee * 0.1);

										switch(dp_unitprice2)
										{
											case 0	: { price_update2 = (price_updatee * 1); break; }
											case 1	: { price_update2 = (price_updatee * 0.1); break; }
											case 2	: { price_update2 = (price_updatee * 0.01); break; }
											case 3	: { price_update2 = (price_updatee * 0.001); break; }
											default	: { break; }
										}

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

										command_response2 = true;

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
								//============================== SUSPEND REQUEST ==============================//
								//===========================================================================//
								else if( (r_raw_data2[i] == 0x0E) && (r_raw_data2[i+1] == 0x01) )
								{

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
										command_2 = SUSPEND_REQUEST;

										resp2 = DATA_SUSPEND;

										command_response2 = true;

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
								//'50 37 05 03 00 58 10 a2 0a 03 fa
								//===========================================================================//
								//==========================   SET PUMP PARAMETERS   ========================//
								//===========================================================================//
								else if( (r_raw_data2[i] == 0x09) && (r_raw_data1[i+1] == 0x33) ) //51 Data Bytes
								{
									crc_original = r_raw_data2[i+56];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data2[i+55]);


									//==============================================================//
									//==================== VALIDATING THE CRC ======================//

									data_[0] = r_addr2;
									data_[1] = r_ctrl2;

									////////////////////////////////////////
									//'50 37 05 03 00 58 - - - 10 a2 0a 03 fa
									////////////////////////////////////////
									//===the read buffer contains only data from the transaction byte to the SF byte ===//
									//================== Data contains a cumulative of 51 raw Bytes ===============//
									for(uint8_t ii = 0, j = 2; ii < 53; ii++, j++)  //51 + Trans No. + Data Len
									{
										data_[j] = r_raw_data2[ii];
									}


									crc_check = crc_16(data_, 55);


									if(crc_check == crc_original)
									{
										for (uint8_t j = 0; j < 51; j++)
										{
											set_param2[j] = r_raw_data2[i+2+j];
										}

										resp2 = DATA_SET_PUMP_PARAM;
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

								//###########################################################################//
								//'50 37 66 01 01 ee 37 03 fa
								//===========================================================================//
								//============================ OTP SESSION CLEAR ============================//
								//===========================================================================//
								else if(r_raw_data2[i] == 0x66 && r_raw_data2[i+1] == 0x01)
								{
									crc_original = r_raw_data2[i+4];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data2[i+3]);


									//==============================================================//
									//==================== VALIDATING THE CRC ======================//
									//'50 37 66 01 01 d2 83 03 fa'
									data_[0] = r_addr2;
									data_[1] = r_ctrl2;

									for(uint8_t ii = 0, j = 2; ii < 3; ii++, j++)   //1 + Trans No. + Data Len
									{
										data_[j] = r_raw_data2[ii];
									}

									crc_check = crc_16(data_, 5);

									if(crc_check == crc_original)
									{
										command_2 = CLEAR_OTP_SESSION;

										resp2 = DATA_CLEAR_OTP_SESSION;

										command_response2 = true;

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

								//###########################################################################//
								//'51 30 67 01 00 42 83 03 fa  <=== Send Current Iteration of Info
								//'51 30 67 01 01 83 43 03 fa  <=== Send Next Iteration of Info
								//===========================================================================//
								//============================ CONFIG CHANGE QUERY ==========================//
								//===========================================================================//
								else if(r_raw_data2[i] == 0x67 && r_raw_data2[i+1] == 0x01)
								{
									crc_original = r_raw_data2[i+4];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data2[i+3]);


									//==============================================================//
									//==================== VALIDATING THE CRC ======================//
									data_[0] = r_addr2;
									data_[1] = r_ctrl2;

									for(uint8_t ii = 0, j = 2; ii < 3; ii++, j++)   //1 + Trans No. + Data Len
									{
										data_[j] = r_raw_data2[ii];
									}

									crc_check = crc_16(data_, 5);

									if(crc_check == crc_original)
									{
										command_2 = REQUEST_CONFIG_CHANGE_INFO;

										resp2 = DATA_REQUEST_CONFIG_CHANGE_INFO;

										command_response2 = true;

										if(data_[4] == 0x00)                       // Controller Requests Current Config-change Item
										{
											configChangeInfo_next2 = 0;
										}
										else if(data_[4] == 0x01)  				   // Controller Requests Next Config-change Item       //Controller fails in sending preceding data to Server
										{
											configChangeInfo_next2 = 1;
										}

										ack_send2 = true;
									}
									else
									{
										resp = CRC_ERROR;
									}
									//=================== DONE, VALIDATING THE CRC =================//
									//==============================================================//

									break;
								}

								//###########################################################################//
								//
								//res => ep:1a. {"st":0,"tk":31290,"ud":0,"tm":37427375,"am":0.0,
								//"mt":{"ty":3,"pn":"all","pr":590.0,"sh":null,"fg":0,"tg":"p|all"},
								//"pv":0.0,"wv":0.0,"sa":0.0,"bal":0.0,"dc":null,"wb":null,"ft":null}
								//
								///////////////////////////////////////////////////////////////////////////////
								///////////////////////////////////////////////////////////////////////////////
								//'50 37  68 05  12 34 56 78 90  a2 0a 03 fa
								//===========================================================================//
								//============================   UPDATE DATE/TIME   =========================//
								//===========================================================================//
								else if( (r_raw_data2[i] == 0x68) && (r_raw_data2[i+1] == 0x05) )  //4 Data Bytes
								{
									uint8_t update_datetime[5];
									char update_datetimeee[20];

									crc_original = r_raw_data2[i+8];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data2[i+7]);


									//==============================================================//
									//==================== VALIDATING THE CRC ======================//

									data_[0] = r_addr2;
									data_[1] = r_ctrl2;

									////////////////////////////////////////
									//'50 37  68 05  12 34 56 78 90  a2 0a 03 fa
									////////////////////////////////////////
									//===the read buffer contains only from the transaction byte to the SF byte ===//
									//================== Data contains a cumulative of 5 raw Bytes ===============//
									for(uint8_t ii = 0, j = 2; ii < 7; ii++, j++)  //5 + Trans No. + Data Len
									{
										data_[j] = r_raw_data2[ii];
									}


									crc_check = crc_16(data_, 9);


									if(crc_check == crc_original)
									{
										for (uint8_t j = 0; j < 5; j++)
										{
											update_datetime[j] = r_raw_data2[i+2+j];
										}

										update_date_time = bcdArray_to_int(update_datetime, 5);

										///////////////////////////////////////////////////////////////////
										///////////////////  ACTUATE THE CHANGE... ////////////////////////


										//			if( (priceChange_check > 0.1) || (priceChange_check < -0.1) )
										//			{
														   ttostr(update_date_time, 1);
														   ttostr(update_date_time, 2);
										//			}

										///////////////////////////////////////////////////////////////////

										resp2 = DATA_DATE_TIME_UPDATE;
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

								//##############################################################################//
								//51 30 69 05 01 01 20 04 00 34 E2 03 FA   //Calibration Data Request
								//51 30 69 05 00 01 20 04 05 C9 21 03 FA   //In-bound Calibration-Set Command
								//==============================================================================//
								//=====================   SET/REQUEST CALIBRATION DETAILS   ====================//
								//==============================================================================//
								else if( (r_raw_data2[i] == 0x69) && (r_raw_data2[i+1] == 0x05) ) //5 Data Bytes
								{
									crc_original = r_raw_data2[i+8];
									crc_original = (crc_original << 8);
									crc_original = (crc_original + r_raw_data2[i+7]);

									//==============================================================//
									//==================== VALIDATING THE CRC ======================//

									data_[0] = r_addr2;
									data_[1] = r_ctrl2;

									////////////////////////////////////////////
									//51 30 69 06 00 01 00 20 04 05 21 70 03 FA   //Calibration Data Request
									//51 30 69 06 01 01 00 20 04 05 EO B0 03 FA   //In-bound Calibration-Set Command
									////////////////////////////////////////////
									//=== the read buffer contains only data from the transaction byte to the SF byte ===//
									//================== Data contains a cumulative of 5 raw Bytes ================//
									for(uint8_t ii = 0, j = 2; ii < 7; ii++, j++)  //5 + Trans No. + Data Len
									{
										data_[j] = r_raw_data2[ii];
									}


									crc_check = crc_16(data_, 9);

									if(crc_check == crc_original)
									{
										if(data_[4] == 0x00)                       //Controller commands to set new Calibration Parameters
										{
											for (uint8_t j = 0; j < 3; j++)
											{
												set_calib2[j] = bcd_to_int(r_raw_data2[i+4+j]);
											}

											resp2 = DATA_SET_CALIBRATION_PARAM;
										}
										else if(data_[4] == 0x01)  				   //Controller requests Calibration Info
										{
											command_2 = REQUEST_CALIBRATION_PARAM;

											resp2 = DATA_REQUEST_CALIBRATION_PARAM;

											command_response2 = true;
										}

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

							MSN = (r_ctrl2 & 0x0F);
							if(MSN == TX2)
							{
								TX2++;
								if(TX2 > 0x0F)
								{
									TX2 = 0x00;
								}
							}

							command_2 = NO_COMMAND;

							command_response2 = false;

							resp2 = NOREPLY;

							break;

							//increase the tx on receiving an ACK

			case 0x50	:	resp2 = r_NACK;

							MSN = (r_ctrl2 & 0x0F);
							if(MSN == TX2)
							{

							}

							break;

			case 0x70	:	resp2 = r_EOT;

							break;
			default		:
							resp2 = JUNK;
							break;
	} //switch ctrl
}


void process_response1(response_enum response)
{
	uint16_t crc;

	addr = 0x4F + settings_stream1[0].noz_addr;    //00H -> FFH
	TX = TX & 0x0F;
	ctrl = TX;

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
			case r_POLL:   // 20H

							//If having nothing to send, send EOT

							if(command_response == false)
							{
								if(status_change_noz1 == 1)   // Nozzle Status Change
								{
									if(nozzle_out1 == true)
									{
										send_nozzleStatus1(0);     //nozStatus = 0, //0 -> in, 1 -> out
										nozzlezUp1 = 1;    //retains this nozzleup memory for some later use
									}
									else if(nozzle_out1 == false){
										send_nozzleStatus1(0);  //nozStatus = 0, //0 -> in, 1 -> out
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
									send_pumpStatus1(0);


									crc = crc_16(DART_BUFF1, 5);
									DART_BUFF1[5] = crc & 0x00FF;
									DART_BUFF1[6] = crc >> 8;
									DART_BUFF1[7] = ETX;
									DART_BUFF1[8] = SF;

									array_len = 9;

									status_change_pump1 = 0;
								}
								/////////////////////////////////////////// DC2  ///////////////////////////////////////////
								//========================================================================================//
								//==========  This transaction is sent by the pump at change of filling values  ==========//
								//========================================================================================//
								if(pump_status_1 == STATUS_FILLING)
								{
									static float old_value = 0;

									float vol_ = go_fillingInfo_vol1();

									if(vol_ != old_value)
									{
										send_fillingInfo1(0);

										crc = crc_16(DART_BUFF1, 12);
										DART_BUFF1[12] = crc & 0x00FF;
										DART_BUFF1[13] = crc >> 8;
										DART_BUFF1[14] = ETX;
										DART_BUFF1[15] = SF;

										array_len = 16;
									}
									old_value = vol_;

									if(mamo_fillingInfo_send1 == 1)
									{
										mamo_fillingInfo_send1 = 0;
									}
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
							}
							else if( (command_ == GETSTATUS) || (command_ == REQUEST_FILLING_INFO)
									|| (command_ == RETURN_PUMP_PARAM) || (command_ == RETURN_PUMP_IDENTITY)  )
							{
								_process_response1(DATA_COMMAND);
							}
							else if(command_ == REQUEST_VOL_TOTAL_COUNT)
							{
								_process_response1(DATA_REQUEST_VOL_TOTAL_COUNT);
							}
							else if(command_ == REQUEST_CONFIG_CHANGE_INFO)
							{
								_process_response1(DATA_REQUEST_CONFIG_CHANGE_INFO);
							}
							else if(command_ == REQUEST_CALIBRATION_PARAM)
							{
								_process_response1(DATA_REQUEST_CALIBRATION_PARAM);
							}
							else if(command_ == DATE_TIME_UPDATE)
							{
								_process_response1(DATA_DATE_TIME_UPDATE);
							}
							else if(command_ == CLEAR_OTP_SESSION)
							{
								_process_response1(DATA_CLEAR_OTP_SESSION);
							}

							go_write1();

							break;

			case r_ACK:   //C0H -> CFH

							resp = NOREPLY;
							break;

			case r_NACK:   //50H -> 5FH

							if( (command_ == GETSTATUS) || (command_ == REQUEST_FILLING_INFO)
									|| (command_ == RETURN_PUMP_PARAM) || (command_ == RETURN_PUMP_IDENTITY)  )
							{
								_process_response1(DATA_COMMAND);
							}
							else if(command_ == REQUEST_VOL_TOTAL_COUNT)
							{
								_process_response1(DATA_REQUEST_VOL_TOTAL_COUNT);
							}
							else if(command_ == REQUEST_CONFIG_CHANGE_INFO)
							{
								_process_response1(DATA_REQUEST_CONFIG_CHANGE_INFO);
							}
							else if(command_ == REQUEST_CALIBRATION_PARAM)
							{
								_process_response1(DATA_REQUEST_CALIBRATION_PARAM);
							}
							else if(command_ == DATE_TIME_UPDATE)
							{
								_process_response1(DATA_DATE_TIME_UPDATE);
							}
							else if(command_ == CLEAR_OTP_SESSION)
							{
								_process_response1(DATA_CLEAR_OTP_SESSION);
							}

							go_write1();

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
		_process_response1(response);
		go_write1();
	}
}


void process_response2(response_enum response)
{
	uint16_t crc;

	addr2 = 0x4F + settings_stream1[1].noz_addr;    //00H -> FFH
	TX2 = TX2 & 0x0F;

	ctrl2 = TX2;

	if (response == CRC_ERROR)
	{
		ctrl2 = 0x50 | r_TX2;

		memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

		DART_BUFF2[0] = addr2;
		DART_BUFF2[1] = ctrl2;
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
			case r_POLL:   // 20H

							if(command_response2 == false)
							{
								if(status_change_noz2 == 1)
								{
									if(nozzle_out2 == true)
									{
										send_nozzleStatus2(0);  //Param : uint8_t buff_index
										nozzlezUp2 = 1;    //retains this nozzleup memory for some later use
									}
									else if(nozzle_out2 == false){
										send_nozzleStatus2(0);  //, 180.00, 0);   //nozStatus = 0, //0 -> in, 1 -> out
										nozzlezUp2 = 0;  //clears this nozzleup memory @ any slightest nozzledown
									}

									crc = crc_16(DART_BUFF2, 8);
									DART_BUFF2[8] = crc & 0x00FF;
									DART_BUFF2[9] = crc >> 8;
									DART_BUFF2[10] = ETX;
									DART_BUFF2[11] = SF;

									array_len2 = 12;

									status_change_noz2 = 0;
								}
								else if(status_change_pump2 == 1)   //Generic Pump-Status-Change
								{
									send_pumpStatus2(0);


									crc = crc_16(DART_BUFF2, 5);
									DART_BUFF2[5] = crc & 0x00FF;
									DART_BUFF2[6] = crc >> 8;
									DART_BUFF2[7] = ETX;
									DART_BUFF2[8] = SF;

									array_len2 = 9;

									status_change_pump2 = 0;
								}
								///////////////////////////////////////  DC2  ///////////////////////////////////////
								//=================================================================================//
								//==========  This transaction is sent by the pump at change of a value  ==========//
								//=================================================================================//
								if(pump_status_2 == STATUS_FILLING)
								{
									static float old_value = 0;

									float vol_ = go_fillingInfo_vol2();

									if(vol_ != old_value)
									{
										send_fillingInfo2(0);

										crc = crc_16(DART_BUFF2, 12);
										DART_BUFF2[12] = crc & 0x00FF;
										DART_BUFF2[13] = crc >> 8;
										DART_BUFF2[14] = ETX;
										DART_BUFF2[15] = SF;

										array_len2 = 16;
									}
									old_value = vol_;

									if(mamo_fillingInfo_send2 == 1)
									{
										mamo_fillingInfo_send2 = 0;
									}
								}

								//==========================================================//
								//==========  If having nothing to send, send EOT ==========//
								//==========================================================//
								else
								{
									ctrl2 = 0x70;    //EOT

									DART_BUFF2[1] = ctrl2;
									DART_BUFF2[2] = SF;

									array_len2 = 3;
								}
							}
							else if( (command_2 == GETSTATUS) || (command_2 == REQUEST_FILLING_INFO)
									|| (command_2 == RETURN_PUMP_PARAM) || (command_2 == RETURN_PUMP_IDENTITY)  )
							{
								_process_response2(DATA_COMMAND);
							}
							else if(command_2 == REQUEST_VOL_TOTAL_COUNT)
							{
								_process_response2(DATA_REQUEST_VOL_TOTAL_COUNT);
							}
							else if(command_2 == REQUEST_CONFIG_CHANGE_INFO)
							{
								_process_response2(DATA_REQUEST_CONFIG_CHANGE_INFO);
							}
							else if(command_2 == REQUEST_CALIBRATION_PARAM)
							{
								_process_response2(DATA_REQUEST_CALIBRATION_PARAM);
							}
							else if(command_2 == DATE_TIME_UPDATE)
							{
								_process_response2(DATA_DATE_TIME_UPDATE);
							}
							else if(command_2 == CLEAR_OTP_SESSION)
							{
								_process_response2(DATA_CLEAR_OTP_SESSION);
							}

							go_write2();

							break;

			case r_ACK:   //C0H -> CFH

							resp2 = NOREPLY;
							break;

			case r_NACK:   //50H -> 5FH

							if( (command_2 == GETSTATUS) || (command_2 == REQUEST_FILLING_INFO)
									|| (command_2 == RETURN_PUMP_PARAM) || (command_2 == RETURN_PUMP_IDENTITY)  )
							{
								_process_response2(DATA_COMMAND);
							}
							else if(command_2 == REQUEST_VOL_TOTAL_COUNT)
							{
								_process_response2(DATA_REQUEST_VOL_TOTAL_COUNT);
							}
							else if(command_2 == REQUEST_CONFIG_CHANGE_INFO)
							{
								_process_response2(DATA_REQUEST_CONFIG_CHANGE_INFO);
							}
							else if(command_2 == REQUEST_CALIBRATION_PARAM)
							{
								_process_response2(DATA_REQUEST_CALIBRATION_PARAM);
							}
							else if(command_2 == DATE_TIME_UPDATE)
							{
								_process_response2(DATA_DATE_TIME_UPDATE);
							}
							else if(command_2 == CLEAR_OTP_SESSION)
							{
								_process_response2(DATA_CLEAR_OTP_SESSION);
							}

							go_write2();

							break;

			case r_ACKPOLL:    //E0H -> EFH
							ctrl2 = 0xE0 | r_TX2;
							break;

			case r_EOT:   // 70H -> 7FH
							ctrl2 = 0x70 | r_TX2;
							break;

			default:
							break;
		}
	}
	else
	{
		_process_response2(response);
		go_write2();
	}
}


void _process_response1(response_enum response)
{

	unsigned char status_;
	uint16_t crc;

	static uint8_t check = 0;

	uint8_t decimalPlaces = 2;
	double roundedNum;
	int num_;

	ctrl = TX;

	if(response == DATA_COMMAND)   //This transaction is sent by the pump if the status is changed or if the pump receives the command 'RETURN STATUS’
	{

		if(ack_send == true)
		{
			//==> send ack
			send_acknowledgement1(ACK);
			ack_send = false;


			switch (command_)
			{
				//for the Pump-Status Commands
				case RESET1		      :
										{
											command_response = false;
											if(settings_stream1[0].mode == AUTO_MODE)
											{
												if( (pump_status_1 == STATUS_FILLING_COMP) || (pump_status_1 == STATUS_MAMO_REACHED) || (pump_status_1 == STATUS_SWITCHED_OFF) )
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
												if(pump_status_1 == STATUS_RESET)
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
												if((pump_status_1 == STATUS_RESET) || (pump_status_1 == STATUS_SUSPENDED) ||
												   (pump_status_1 == STATUS_MAMO_REACHED) || (pump_status_1 == STATUS_AUTH) ||
												   (pump_status_1 == STATUS_FILLING) || (pump_status_1 == STATUS_SWITCHED_OFF) )
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
												if(pump_status_1 == STATUS_AUTH)
												{
//													pump_status_ = STATUS_SUSPENDED;

													authsuspend_flag1 = 1;
													//Motor turned off
												}
												else if(pump_status_1 == STATUS_FILLING)
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

				default									: 	break;
			}

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

				if(ack_send == true)
				{
					//==> send ack
					send_acknowledgement1(ACK);
					ack_send = false;
				}
				else
				{

					ctrl |= 0x30;

					memset(DART_BUFF1, 0, sizeof(DART_BUFF1));

					DART_BUFF1[0] = addr;
					DART_BUFF1[1] = ctrl;


					//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
					////////////////////////////////////////// DC1 ////////////////////////////////////////
					/////////////////////////////////// GET PUMP-STATUS ///////////////////////////////////


					trans = 0x01;
					lng = 0x01;

					DART_BUFF1[2] = trans;
					DART_BUFF1[3] = lng;

					//==============================//
	//				pump_status_ = STATUS_RESET;
					//=============================//


					switch (pump_status_1)
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

//					decimalPlaces = 1;
					decimalPlaces = dp_unitprice1;

					roundedNum = round_off(fillingPrice, decimalPlaces);
//					roundedNum = roundedNum * 10;
					roundedNum = roundedNum * powerOfTen(dp_unitprice1);;
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
			}

			//=============================================================================//
			//        This transaction is sent by the pump at change of a value            //
			//        or if the pump receives the command RETURN FILLING INFORMATION       //
			//=============================================================================//
			else if (command_ == REQUEST_FILLING_INFO)        //FILLED_VOLUME_AND_AMOUNT
			{

				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
				//////////////////////////////////  GET FILLED_VOLUME_AND_AMOUNT //////////////////////////////////


				if(ack_send == true)
				{
					//==> send ack
					send_acknowledgement1(ACK);
					ack_send = false;
				}
				else
				{
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

					ctrl |= 0x30;
					trans = 0x02;
					lng = 0x08;

					memset(DART_BUFF1, 0, sizeof(DART_BUFF1));

					DART_BUFF1[0] = addr;
					DART_BUFF1[1] = ctrl;
					DART_BUFF1[2] = trans;
					DART_BUFF1[3] = lng;

//					decimalPlaces = 2;
					decimalPlaces = dp_vol1;

					roundedNum = round_off(vol_, decimalPlaces);
//					roundedNum = roundedNum * 100;
					roundedNum = roundedNum * powerOfTen(dp_vol1);
					num_ = (int)(roundedNum);

					int_to_bcd(num_, bcd_);

					for (uint8_t i = 0, j = 3; i < 4; i++, j--)
					{
						DART_BUFF1[i + 4] = bcd_[j];
					}

//					decimalPlaces = 2;
					decimalPlaces = dp_amount1;

					roundedNum = round_off(amo_, decimalPlaces);
//					roundedNum = roundedNum * 100;
					roundedNum = roundedNum * powerOfTen(dp_amount1);
					num_ = (int)(roundedNum);

					memset(bcd_, 0, sizeof(bcd_));

					int_to_bcd(num_, bcd_);

					for (uint8_t i = 0, j = 3; i < 4; i++, j--)
					{
						DART_BUFF1[i + 8] = bcd_[j];
					}


					//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
					/////////////////////////////////// GET NOZSTATUS_AND_FILLINGPRICE //////////////////////////////////

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

//					decimalPlaces = 2;
					decimalPlaces = dp_unitprice1;

					roundedNum = round_off(fillingPrice, decimalPlaces);
//					roundedNum = roundedNum * 100;
					roundedNum = roundedNum * powerOfTen(dp_unitprice1);
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
			}


			else if (command_ == RETURN_PUMP_IDENTITY)       //PUMP_IDENTITY)
			{
//				float fillingPrice = 1550.1234;
//				int decimalPlaces = 2;
//				double roundedNum;
				int num_, pump_id;
				unsigned char bcd_[10] = {0};  // Array to hold the BCD result

				uint16_t crc;

				if(ack_send == true)
				{
					//==> send ack
					send_acknowledgement1(ACK);
					ack_send = false;
				}
				else
				{

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
			}

			//=============================================================================//
			//   	  This transaction is sent by the pump if SET PUMP PARAMETERS          //
			//		  is received or if the pump receives the command                      //
			//		   'RETURN PUMP PARAMETERS'.      			                           //
			//=============================================================================//
			else if (command_ == RETURN_PUMP_PARAM)       //PUMP_IDENTITY)
			{
				if(ack_send == true)
				{
					//==> send ack
					send_acknowledgement1(ACK);
					ack_send = false;
				}
				else
				{

					uint8_t dp_vol = dp_vol1;
					uint8_t dp_amo = dp_amount1;
					uint8_t dp_unp = dp_unitprice1;

//					int pump_limit = 1000;  //1000 Litres

					int num_, pump_id;
					unsigned char bcd_[10] = {0};  // Array to hold the BCD result

					uint16_t crc;

					ctrl |= 0x30;
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

					int_to_bcd(pump_max_litres2, bcd_);

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
		send_acknowledgement1(ACK);
		ack_send = false;

		///////////////////////////////////////////////////////////////////
		///////////////////  ACTUATE THE CHANGE... ////////////////////////

		float priceChange_check = price_update1 - settings_stream1[0].price_;

		if (settings_stream1[0].mode == AUTO_MODE)
		{

			if( (priceChange_check > 0.1) || (priceChange_check < -0.1) )
			{
				   changeLitrePrice1 = 1;
			}

			if(pump_status_1 == STATUS_PNP)
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
			send_acknowledgement1(ACK);
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

//			decimalPlaces = 3;
			decimalPlaces = dp_vol1;

			roundedNum = round_off(tot_vol, decimalPlaces);
//			roundedNum = roundedNum * 1000;
			roundedNum = roundedNum * powerOfTen(dp_vol1);
			num_ = (int)(roundedNum);

			int_to_bcd(num_, bcd_);

			for (uint8_t ii = 0, j = 4; ii < 5; ii++, j--)   //j=>4 : MSB in it, LSB is in j=0
			{
				DART_BUFF1[ii + 5] = bcd_[j];
			}

			roundedNum = round_off(tot_vol1, decimalPlaces);
//			roundedNum = roundedNum * 1000;
			roundedNum = roundedNum * powerOfTen(dp_vol1);
			num_ = (int)(roundedNum);

			memset(bcd_, 0, sizeof(bcd_));

			int_to_bcd(num_, bcd_);

			for (uint8_t ii = 0, j = 4; ii < 5; ii++, j--)
			{
				DART_BUFF1[ii + 10] = bcd_[j];
			}

//			roundedNum = round_off(tot_vol2, decimalPlaces);
//			roundedNum = roundedNum * 1000;
//			num_ = (int)(roundedNum);
//
//			memset(bcd_, 0, sizeof(bcd_));
//
//			int_to_bcd(num_, bcd_);
//
//			for (uint8_t ii = 0, j = 0; ii < 5; ii++, j--)
//			{
//				DART_BUFF1[ii + 15] = bcd_[j];
//			}

			for (uint8_t ii = 0; ii < 5; ii++)
			{
				DART_BUFF1[ii + 15] = 0x00;
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
		send_acknowledgement1(ACK);
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
		send_acknowledgement1(ACK);
		ack_send = false;

		command_response = false;

		if(settings_stream1[0].mode == AUTO_MODE)
		{
			if((pump_status_1 == STATUS_FILLING) || (pump_status_1 == STATUS_AUTH) )
			{
//				pump_status_ = STATUS_SUSPENDED;
				//Motor turned off

				//Effect the Change...
			}
		}
	}
	else if(response == DATA_RESUME)
	{
		send_acknowledgement1(ACK);
		ack_send = false;

		command_response = false;
		if(settings_stream1[0].mode == AUTO_MODE)
		{
			if(pump_status_1 == STATUS_SUSPENDED)
			{
				//Motor turned on

				//Effect the Change...
			}
		}
	}
	else if(response == DATA_REQUEST_CONFIG_CHANGE_INFO)
	{
		if(ack_send == true)
		{
			//==> send ack
			send_acknowledgement1(ACK);
			ack_send = false;
		}
		else
		{

			//TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
			//  _________________________________________________________________________________________________________________________________________________________________
			// | NOZ-NUM | TX-NUM | TX-CODE | TX-LEN | OTP-SEED [2 Bytes] | TIMESTAMP [5 Bytes] | CONFIG-1 PARTICULAR [1 Byte] | OLD-VALUE 1 [3 Bytes] | NEW-VALUE 1 [3 Bytes]|
			// |_________|________|_________|________|____________________|_____________________|______________________________|_______________________|______________________|__
			// ________________________________________________________________________________________________________
			//   |CONFIG-2 PARTICULAR [1 Byte] | OLD-VALUE 2 [3 Bytes] | NEW-VALUE 2 [3 Bytes] | CRC1 |CRC2 | ETX | SF |
			// __|_____________________________|_______________________|_______________________|______|_____|_____|____|
			//
			//YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY

			// 51 30 67 01 00 42 83 03 FA  <== From Controller

			// 51 30 67 01 01 83 43 03 FA  <== From Controller

			//'50 38  67 15  12 34  12 34 56 78 90   00 00 18 03 67 00 00   00 00 00 59 57 83 27   ab 03 fa '

			//####################################################################################################################################################################



			//=============================================================================//
			//       	 This transaction is sent by the pump at a Request of              //
			//        				 "REQUEST CONFIG CHANGE INFO"                          //
			//=============================================================================//

			uint16_t otp_seed;
			uint32_t time_stamp;

			unsigned char bcd_[6] = {0};  // Array to hold the BCD result

			uint16_t crc;

			uint8_t configCheck;


			retrieve_configFlag_fram(side_a);

			//=================================================================//
			// Checks for whether Config Mode has even been accessed @all
			//=================================================================//
			if(configMode1 == CONFIGUNMODIFIED)  // && (configChangeInfo_next1 == 0) )
			{
				send_acknowledgement1(EOT);
				return;
			}


			retrieve_config_otpSeed_time_fram(side_a);
			retrieve_settings_original_fram(side_a);

			otp_seed = configChange[0].otp_seed;
			time_stamp = configChange[0].time_stamp;

			ctrl |= 0x30;
			trans = 0x67;

			memset(DART_BUFF1, 0, sizeof(DART_BUFF1));

			DART_BUFF1[0] = addr;
			DART_BUFF1[1] = ctrl;
			DART_BUFF1[2] = trans;
//			DART_BUFF1[3] = lng;

			//'50 38  67 15   12 34  12 34 56 78 90   00 00 18 03 67 00 00   00 00 00 59 57 83 27   ab 03 fa '

			int_to_bcd(otp_seed, bcd_);

			for (uint8_t ii = 0, j = 1; ii < 2; ii++, j--)   //j=>1 : MSB in it, LSB is in j=>0
			{
				DART_BUFF1[ii + 4] = bcd_[j];
			}

			memset(bcd_, 0, sizeof(bcd_));

			//'50 38  67 15   12 34  12 34 56 78 90   00 00 18 03 67 00 00   00 00 00 59 57 83 27   ab 03 fa '

			int_to_bcd(time_stamp, bcd_);

			for (uint8_t ii = 0, j = 4; ii < 5; ii++, j--)   //j=>4 : MSB in it, LSB is in j=>0
			{
				DART_BUFF1[ii + 6] = bcd_[j];
			}

			retrieve_configChange_trackNum_fram(side_a);

			if(configChangeInfo_next1 == 0)
			{
				if(trackNum[0].track_num0 > 24)
					trackNum[0].track_num0 = 0;

				trackNum[0].track_num = trackNum[0].track_num0;
				configCheck = configChange_notify_build1(trackNum[0].track_num0);
			}
			else if(configChangeInfo_next1 == 1)
			{
				if(trackNum[0].track_num > 24)
					trackNum[0].track_num = 0;

				configCheck = configChange_notify_build1(trackNum[0].track_num);
			}

			save_configChange_trackNum_fram(side_a);

			if(configCheck == 2)
			{
				DART_BUFF1[3] = 21;  //Data-Length;

				crc = crc_16(DART_BUFF1, 25);

				DART_BUFF1[25] = crc & 0x00FF;
				DART_BUFF1[26] = crc >> 8;
				DART_BUFF1[27] = ETX;
				DART_BUFF1[28] = SF;

				array_len = 29;
			}
			else if (configCheck == 1)
			{
				DART_BUFF1[3] = 14;  //Data-Length;

				crc = crc_16(DART_BUFF1, (25 - 7));

				DART_BUFF1[25 - 7] = crc & 0x00FF;
				DART_BUFF1[26 - 7] = crc >> 8;
				DART_BUFF1[27 - 7] = ETX;
				DART_BUFF1[28 - 7] = SF;

				array_len = (29 - 7);
			}

			//================================================================================//
			// If Config Mode is been accessed, but :
			// (i)  No changes effected,
			// (ii) Or all the possible changes info have been effectively passed on to GO
			//===============================================================================//
			else if (configCheck == 0)
			{
				send_acknowledgement1(EOT);

				clear_configFlag_fram(side_a);
				clear_configChange_trackNum_fram(side_a);
			}



			//'50 38 67 0E   12 34 12 34 56 78    00 00 18 03 67 00 00   00 00 00 59 57 83 27   ab 03 fa '
			command_ = NO_COMMAND;
			resp = NOREPLY;

		}
	}
	else if(response == DATA_DATE_TIME_UPDATE)
	{

		if(ack_send == true)
		{
			//==> send ack
			send_acknowledgement1(ACK);
			ack_send = false;
		}
		else
		{
			/***********************************************
			*
			*'50 30 68 01 01 8e 80 03 fa'
			*
			************************************************/

			uint16_t crc;

			ctrl |= 0x30;
			trans = 0x68;
			lng = 0x01;

			memset(DART_BUFF1, 0, sizeof(DART_BUFF1));

			DART_BUFF1[0] = addr;
			DART_BUFF1[1] = ctrl;
			DART_BUFF1[2] = trans;
			DART_BUFF1[3] = lng;
			DART_BUFF1[4] = 0x01;

			crc = crc_16(DART_BUFF1, 5);
			DART_BUFF1[5] = crc & 0x00FF;
			DART_BUFF1[6] = crc >> 8;
			DART_BUFF1[7] = ETX;
			DART_BUFF1[8] = SF;

			array_len = 9;

			resp = NOREPLY;
		}
	}

	else if(response == DATA_CLEAR_OTP_SESSION)
	{
		if(ack_send == true)
		{
			send_acknowledgement1(ACK);
			ack_send = false;

			///////////////////////////////////////////////////////////////////
			///////////////////  ACTUATE THE CHANGE... ////////////////////////

			clear_otpSeed_session_fram(side_a);

			///////////////////////////////////////////////////////////////////

		}
		else
		{
			/***********************************************
			*
			*'50 30 66 01 01 ef 43 03 fa'
			*
			************************************************/

			uint16_t crc;

			ctrl |= 0x30;
			trans = 0x66;
			lng = 0x01;

			memset(DART_BUFF1, 0, sizeof(DART_BUFF1));

			DART_BUFF1[0] = addr;
			DART_BUFF1[1] = ctrl;
			DART_BUFF1[2] = trans;
			DART_BUFF1[3] = lng;
			DART_BUFF1[4] = 0x01;

			crc = crc_16(DART_BUFF1, 5);
			DART_BUFF1[5] = crc & 0x00FF;
			DART_BUFF1[6] = crc >> 8;
			DART_BUFF1[7] = ETX;
			DART_BUFF1[8] = SF;

			array_len = 9;

			resp = NOREPLY;
		}
	}

	else if(response == DATA_REQUEST_CALIBRATION_PARAM)
	{
		if(ack_send == true)
		{
			//==> send ack
			send_acknowledgement1(ACK);
			ack_send = false;
		}
		else
		{

			//TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
			//  ____________________________________________________________________________________________________________________________________________________________________________________
			// | NOZ-NUM | TX-NUM | TX-CODE | TX-LEN | DIRECTION [1 Byte] |   CTT [1 Byte]	 |	ACTUAL CALIBRATION [2 Bytes] | APPARENT CALIBRATION [1 Byte] | NEW APPARENT-CALIBRATION [1 Byte] |
			// |         |        |         |        | (ie. SET/REQUEST)  |  1 --> Non-Timed |            (1 DP)             |     		 (2 DP)              |			    (2 DP)               |
			// |         |        |         |        |                    |  2 --> Timed     |                               |                               |									 |
			// |_________|________|_________|________|____________________|__________________|_______________________________|_______________________________|___________________________________|__
			// _________________________
			//  | CRC1 |CRC2 | ETX | SF |
			//  |      |     |     |    |
			//  |      |     |     |    |
			// _|______|_____|_____|____|
			//
			//YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY

			//'50 38  69 04  01  20 04 00 46 f0 03 fa '   //Calibration Data Request

			//'50 38  69 04  00  20 04 05 47 0c 03 fa '   //In-bound Calibration-Set Command

			//####################################################################################################################################################################
			//
			// [{"nm":"P7","nz":0,"ctt":1,"ct":"20|0.5|0.4},{"nm":
			// ctt":2,"ct":"1500|20|0.5|0.4|2005}
			// ctt":2,"ct":"20|0.5|0.4-1500|20|0.5|0.4|2005}

			//=============================================================================//
			//       	 This transaction is sent by the pump at a Request of              //
			//        				 "REQUEST CALIBRATION INFO"                            //
			//=============================================================================//

			unsigned char bcd_[6] = {0};  // Array to hold the BCD result

			uint16_t crc;


			retrieve_ctSettings_fram(side_a);
//			vol_real1 =  ct_settingsA.original;
//			vol_calibrated1 = ct_settingsA.base;
//			vol_effective1 = ct_settingsA.effective;

			ctrl |= 0x30;
			trans = 0x69;
			lng	  = 0x06;

			memset(DART_BUFF1, 0, sizeof(DART_BUFF1));

			DART_BUFF1[0] = addr;
			DART_BUFF1[1] = ctrl;
			DART_BUFF1[2] = trans;
			DART_BUFF1[3] = lng;
			DART_BUFF1[4] = 0x01;       //Signals sending out Calib Details to the Controller

			int_to_bcd(vol_real1, bcd_);

			for (uint8_t ii = 0, j = 1; ii < 2; ii++, j--) // 2 Bytes
			{
				DART_BUFF1[ii + 5] = bcd_[j];
			}

			memset(bcd_, 0, sizeof(bcd_));

			float vol_calibrated1_ = (vol_calibrated1 - vol_real1);

			roundedNum = round_off(vol_calibrated1_, decimalPlaces);
			roundedNum = roundedNum * 100;
			num_ = (int)(roundedNum);

			int_to_bcd(num_, bcd_);

//			int_to_bcd(vol_calibrated1, bcd_);

			for (uint8_t ii = 0, j = 0; ii < 1; ii++)
			{
				DART_BUFF1[ii + 7] = bcd_[j];
			}

			memset(bcd_, 0, sizeof(bcd_));

			float vol_effective1_ = (vol_effective1 - vol_real1);

//			vol_effective2_ = 0.4;

			roundedNum = round_off(vol_effective1_, decimalPlaces);
			roundedNum = roundedNum * 100;
			num_ = (int)(roundedNum);

			int_to_bcd(num_, bcd_);

//			int_to_bcd(vol_effective1, bcd_);

			for (uint8_t ii = 0, j = 0; ii < 1; ii++)
			{
				DART_BUFF1[ii + 8] = bcd_[j];
			}


			crc = crc_16(DART_BUFF1, 9);

			DART_BUFF1[9] = crc & 0x00FF;
			DART_BUFF1[10] = crc >> 8;
			DART_BUFF1[11] = ETX;
			DART_BUFF1[12] = SF;

			array_len = 13;

			command_ = NO_COMMAND;
			resp = NOREPLY;
		}
	}
	else if(response == DATA_SET_CALIBRATION_PARAM)
	{
		send_acknowledgement1(ACK);
		ack_send = false;

		///////////////////////////////////////////////////////////////////
		///////////////////  ACTUATE THE CHANGE... ////////////////////////


		if (settings_stream1[0].mode == AUTO_MODE)
		{
			vol_real1 = set_calib1[0];
			vol_calibrated1 = ((set_calib1[1] * 0.1) + set_calib1[0]);
			vol_effective1 = ((set_calib1[2] * 0.1) + set_calib1[0]);

			save_ctSettings_fram(side_a);

			online_calibFlag1 = 1;
			save_online_calibFlag_fram(side_a);
		}

		///////////////////////////////////////////////////////////////////
	}
}


void _process_response2(response_enum response)
{

	unsigned char status_2;
	uint16_t crc;

	static uint8_t check = 0;

	uint8_t decimalPlaces = 2;
	double roundedNum;
	int num_;

	ctrl2 = TX2;

	if(response == DATA_COMMAND)   //This transaction is sent by the pump if the status is changed or if the pump receives the command 'RETURN STATUS’.
	{
		if(ack_send2 == true)
		{
			//==> send ack
			send_acknowledgement2(ACK);
			ack_send2 = false;


			switch (command_2)
			{
				//for the Pump-Status Commands
				case RESET1		      :
										{
											command_response2 = false;
											if(settings_stream1[1].mode == AUTO_MODE)
											{
												if( (pump_status_2 == STATUS_FILLING_COMP) || (pump_status_2 == STATUS_MAMO_REACHED) || (pump_status_2 == STATUS_SWITCHED_OFF) )
												{
													reset_flag2 = 1;

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
											command_response2 = false;
											if(settings_stream1[1].mode == AUTO_MODE)
											{
												if(pump_status_2 == STATUS_RESET)
												{
													auth_cmd_flag2 = 1;  //activate auth cmd.
												}
											}
											break;
										}
				case STOP1		 	:
										{
											command_response2 = false;
											if(settings_stream1[1].mode == AUTO_MODE)
											{
												if((pump_status_2 == STATUS_RESET) || (pump_status_2 == STATUS_SUSPENDED) ||
												   (pump_status_2 == STATUS_MAMO_REACHED) || (pump_status_2 == STATUS_AUTH) ||
												   (pump_status_2 == STATUS_FILLING) || (pump_status_2 == STATUS_SWITCHED_OFF) )
												{
													stop_flag2 = 1;  //deactivate auth cmd.
												}
											}
											break;
										}
				case SWITCH_OFF		:
										{
											command_response2 = false;
											if(settings_stream1[1].mode == AUTO_MODE)
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
											command_response2 = false;
											if(settings_stream1[1].mode == AUTO_MODE)
											{
												if(pump_status_2 == STATUS_AUTH)
												{
													authsuspend_flag2 = 1;
													//Motor turned off
												}
												else if(pump_status_2 == STATUS_FILLING)
												{
													//Motor turned off
													fillingsuspend_flag2 = 1;
												}
											}
											break;
										}
				case RESUME_REQUEST	  :
										{
											command_response2 = false;
											if(settings_stream1[1].mode == AUTO_MODE)
											{
												if(authsuspend_flag2 == 1)
												{
													authresume_flag2 = 1;
													//Motor turned on

													authsuspend_flag2 = 0;
												}
												else if(fillingsuspend_flag2 == 1)
												{
													fillingresume_flag2 = 1;
													//Motor turned on

													fillingsuspend_flag2 = 0;
												}
											}
											break;
										}

				default									: 	break;
			}

		}
		else
		{

			//===================================================================//
			//   This transaction is sent by the pump if the status is changed   //
			//   or if the pump receives the command 'RETURN STATUS’.            //
			//===================================================================//

			if (command_2 == GETSTATUS)
			{

				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
				//////////////////////////////////////////  GET PUMP-STATUS ///////////////////////////////////////
				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//

				uint8_t decimalPlaces;
				double roundedNum;
				int num_;

				unsigned char bcd_[10] = {0};  // Array to hold the BCD result

				uint16_t crc;

				ctrl2 = TX2;
				ctrl2 |= 0x30;

				memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

				DART_BUFF2[0] = addr2;
				DART_BUFF2[1] = ctrl2;


				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
				////////////////////////////////////////  DC1 ////////////////////////////////////////
				/////////////////////////////////// GET PUMP-STATUS //////////////////////////////////


				trans = 0x01;
				lng = 0x01;

				DART_BUFF2[2] = trans;
				DART_BUFF2[3] = lng;

				//==============================//
//				pump_status_ = STATUS_RESET;
				//=============================//


				switch (pump_status_2)
				{
					//for the Pump-Status Commands
					case STATUS_PNP		      				:	{status_2 = 0x00; break;}
					case STATUS_RESET 						:	{status_2 = 0x01; break;}
					case STATUS_AUTH 						:	{status_2 = 0x02; break;}
					case STATUS_FILLING						:	{status_2 = 0x04; break;}
					case STATUS_FILLING_COMP				:	{status_2 = 0x05; break;}
					case STATUS_MAMO_REACHED				:	{status_2 = 0x06; break;}   //MAX_AMOUNTVOLUME_REACHED
					case STATUS_SWITCHED_OFF				:	{status_2 = 0x07; break;}
					default									: 	break;
				}

				DART_BUFF2[4] = status_2;


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
				float fillingPrice = go_fillingPrice2();

				trans = 0x03;
				lng = 0x04;

				DART_BUFF2[5] = trans;
				DART_BUFF2[6] = lng;

//				decimalPlaces = 1;
				decimalPlaces = dp_unitprice2;

				roundedNum = round_off2(fillingPrice, decimalPlaces);
//				roundedNum = roundedNum * 10;
				roundedNum = roundedNum * powerOfTen(dp_unitprice2);
				num_ = (int)(roundedNum);

				memset(bcd_, 0, sizeof(bcd_));

				int_to_bcd(num_, bcd_);

				for (uint8_t i = 0, j = 2;  i < 3; i++, j--)
				{
					DART_BUFF2[i + 7] = bcd_[j];
				}

				uint8_t nozNum = 1,
						nozStatus = 0, //0 -> in, 1 -> out
						nozIO;

				if(nozzle_out2 == true) nozStatus = 1;
				else nozStatus = 0;

				// nozIO = nozNum;
				nozStatus = (nozStatus << 4);
				nozIO = nozNum | nozStatus;

				DART_BUFF2[10] = nozIO;    //Nozzle 1


				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

				crc = crc_16(DART_BUFF2, 11);
				DART_BUFF2[11] = crc & 0x00FF;
				DART_BUFF2[12] = crc >> 8;
				DART_BUFF2[13] = ETX;
				DART_BUFF2[14] = SF;

				array_len2 = 15;

				resp2 = NOREPLY;

			}

			//=============================================================================//
			//        This transaction is sent by the pump at change of a value            //
			//        or if the pump receives the command RETURN FILLING INFORMATION       //
			//=============================================================================//
			else if (command_2 == REQUEST_FILLING_INFO)        //FILLED_VOLUME_AND_AMOUNT
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
				float vol_ = go_fillingInfo_vol2();
//				float amo_ = 1500.1234;
				float amo_ = go_fillingInfo_amt2();

				uint8_t decimalPlaces;
				double roundedNum;
				int num_;

				unsigned char bcd_[10] = {0};  // Array to hold the BCD result

				uint16_t crc;

				ctrl2 = TX2;
				ctrl2 |= 0x30;
				trans = 0x02;
				lng = 0x08;

				memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

				DART_BUFF2[0] = addr2;
				DART_BUFF2[1] = ctrl2;
				DART_BUFF2[2] = trans;
				DART_BUFF2[3] = lng;

//				decimalPlaces = 2;
				decimalPlaces = dp_vol2;

				roundedNum = round_off2(vol_, decimalPlaces);
//				roundedNum = roundedNum * 100;
				roundedNum = roundedNum * powerOfTen(dp_vol2);
				num_ = (int)(roundedNum);

				int_to_bcd(num_, bcd_);

				for (uint8_t i = 0, j = 3; i < 4; i++, j--)
				{
					DART_BUFF2[i + 4] = bcd_[j];
				}

//				decimalPlaces = 2;
				decimalPlaces = dp_amount2;

				roundedNum = round_off2(amo_, decimalPlaces);
//				roundedNum = roundedNum * 100;
				roundedNum = roundedNum * powerOfTen(dp_amount2);
				num_ = (int)(roundedNum);

				memset(bcd_, 0, sizeof(bcd_));

				int_to_bcd(num_, bcd_);

				for (uint8_t i = 0, j = 3; i < 4; i++, j--)
				{
					DART_BUFF2[i + 8] = bcd_[j];
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
				float fillingPrice = go_fillingPrice2();

				trans = 0x03;
				lng = 0x04;

				DART_BUFF2[12] = trans;
				DART_BUFF2[13] = lng;

//				decimalPlaces = 1;
				decimalPlaces = dp_unitprice2;

				roundedNum = round_off2(fillingPrice, decimalPlaces);
//				roundedNum = roundedNum * 10;
				roundedNum = roundedNum * powerOfTen(dp_unitprice2);
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

				if(nozzle_out2 == true) nozStatus = 1;
				else nozStatus = 0;

				// nozIO = nozNum;
				nozStatus = (nozStatus << 4);
				nozIO = nozNum | nozStatus;

				DART_BUFF2[17] = nozIO;    //Nozzle 1


				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


				crc = crc_16(DART_BUFF2, 18);
				DART_BUFF2[18] = crc & 0x00FF;
				DART_BUFF2[19] = crc >> 8;
				DART_BUFF2[20] = ETX;
				DART_BUFF2[21] = SF;

				array_len2 = 22;
			}


			else if (command_2 == RETURN_PUMP_IDENTITY)       //PUMP_IDENTITY)
			{
				int num_, pump_id;
				unsigned char bcd_[10] = {0};  // Array to hold the BCD result

				uint16_t crc;

				ctrl2 |= 0x30 ;
				trans = 0x09;
				lng = 0x05;

				memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

				DART_BUFF2[0] = addr2;
				DART_BUFF2[1] = ctrl2;
				DART_BUFF2[2] = trans;
				DART_BUFF2[3] = lng;

//				roundedNum = roundUp(fillingPrice, decimalPlaces);
//				roundedNum = roundedNum * 100;
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

			//=============================================================================//
			//   	  This transaction is sent by the pump if SET PUMP PARAMETERS          //
			//		  is received or if the pump receives the command                      //
			//		   'RETURN PUMP PARAMETERS'.      			                           //
			//=============================================================================//
			else if (command_2 == RETURN_PUMP_PARAM)       //PUMP_IDENTITY)
			{
				if(ack_send2 == true)
				{
					//==> send ack
					send_acknowledgement2(ACK);
					ack_send2 = false;
				}
				else
				{

					uint8_t dp_vol = dp_vol2;
					uint8_t dp_amo = dp_amount2;
					uint8_t dp_unp = dp_unitprice2;

//					int pump_limit = 1000;  //1000 Litres

					int num_, pump_id;
					unsigned char bcd_[10] = {0};  // Array to hold the BCD result

					uint16_t crc;

					ctrl2 |= 0x30 ;
					trans = 0x07;
					lng = 0x33;

					memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

					DART_BUFF2[0] = addr2;
					DART_BUFF2[1] = ctrl2;
					DART_BUFF2[2] = trans;
					DART_BUFF2[3] = lng;


					for (uint8_t i = 0;  i < 22; i++)
					{
						DART_BUFF2[i + 4] = 0;
					}

					DART_BUFF2[26] = dp_vol;
					DART_BUFF2[27] = dp_amo;
					DART_BUFF2[28] = dp_unp;

					for (uint8_t i = 0;  i < 5; i++)
					{
						DART_BUFF2[i + 29] = 0;
					}

					int_to_bcd(pump_max_litres2, bcd_);

					for (uint8_t i = 0, j = 4;  i < 5; i++, j--)
					{
						DART_BUFF2[i + 34] = bcd_[j];
					}

					for (uint8_t i = 0;  i < 2; i++)
					{
						DART_BUFF2[i + 38] = 0;
					}

					for (uint8_t i = 0;  i < 2; i++)
					{
						DART_BUFF2[i + 38] = 0;
					}

					for (uint8_t i = 0;  i < 15; i++)
					{
						DART_BUFF2[i + 40] = 0;
					}

					crc = crc_16(DART_BUFF2, 55);
					DART_BUFF2[55] = crc & 0x00FF;
					DART_BUFF2[56] = crc >> 8;
					DART_BUFF2[57] = ETX;
					DART_BUFF2[58] = SF;

					array_len2 = 59;

					resp2 = NOREPLY;
				}
			}
		}
	}
	else if(response == DATA_PRICE_UPDATE)   //This transaction is sent by the pump if the status is changed or if the pump receives the command 'RETURN STATUS’.
	{
		send_acknowledgement2(ACK);
		ack_send2 = false;

		///////////////////////////////////////////////////////////////////
		///////////////////  ACTUATE THE CHANGE... ////////////////////////

		float priceChange_check = price_update2 - settings_stream1[1].price_;

		if (settings_stream1[1].mode == AUTO_MODE)
		{

			if( (priceChange_check > 0.1) || (priceChange_check < -0.1) )
			{
				   changeLitrePrice2 = 1;
			}

			if(pump_status_2 == STATUS_PNP)
			{
				fillingComplete_flag2 = 1;
			}
		}
		///////////////////////////////////////////////////////////////////
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
			//       	 This transaction is sent by the pump at a Request of              //
			//        				 "REQUEST VOLUME TOTAL COUNTERS"                       //
			//=============================================================================//
			//command_ ==> REQUEST_VOL_TOTAL_COUNT;      //REQUEST TOTALIZER   DP ==> 3

			float tot_vol = totaliser_vol2c;
			float tot_vol1 = tot_vol;
			float tot_vol2 = 0.0000;
			uint8_t decimalPlaces;
			uint8_t noz_id = 1;
			double roundedNum;
			int num_;

			unsigned char bcd_[6] = {0};  // Array to hold the BCD result

			ctrl2 = 0x03;
			ctrl2 |= 0x30;
			trans = 0x65;
			lng = 0x10;

			memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

			DART_BUFF2[0] = addr2;
			DART_BUFF2[1] = ctrl2;
			DART_BUFF2[2] = trans;
			DART_BUFF2[3] = lng;
			DART_BUFF2[4] = noz_id;

			//'50 38 65 10 01 00 00 18 03 67 00 00 18 03 67 00 00 00 00 00 59 ab 03 fa '

//			decimalPlaces = 3;
			decimalPlaces = dp_vol2;

			roundedNum = round_off2(tot_vol, decimalPlaces);
//			roundedNum = roundedNum * 1000;
			roundedNum = roundedNum * powerOfTen(dp_vol2);
			num_ = (int)(roundedNum);

			int_to_bcd(num_, bcd_);

			for (uint8_t ii = 0, j = 4; ii < 5; ii++, j--)   //j=>4 : MSB in it, LSB is in j=0
			{
				DART_BUFF2[ii + 5] = bcd_[j];
			}

			roundedNum = round_off2(tot_vol1, decimalPlaces);
//			roundedNum = roundedNum * 1000;
			roundedNum = roundedNum * powerOfTen(dp_vol2);
			num_ = (int)(roundedNum);

			memset(bcd_, 0, sizeof(bcd_));

			int_to_bcd(num_, bcd_);

			for (uint8_t ii = 0, j = 4; ii < 5; ii++, j--)
			{
				DART_BUFF2[ii + 10] = bcd_[j];
			}

//			roundedNum = round_off(tot_vol2, decimalPlaces);
//			roundedNum = roundedNum * 1000;
//			num_ = (int)(roundedNum);
//
//			memset(bcd_, 0, sizeof(bcd_));
//
//			int_to_bcd(num_, bcd_);
//
//			for (uint8_t ii = 0, j = 0; ii < 5; ii++, j--)
//			{
//				DART_BUFF2[ii + 15] = bcd_[j];
//			}

			for (uint8_t ii = 0; ii < 5; ii++)
			{
				DART_BUFF2[ii + 15] = 0x00;
			}

			//'50 38 65 10 01 00 00 18 03 67 00 00 18 03 67 00 00 00 00 00 59 ab 03 fa '

			crc = crc_16(DART_BUFF2, 20);
			DART_BUFF2[20] = crc & 0x00FF;
			DART_BUFF2[21] = crc >> 8;
			DART_BUFF2[22] = ETX;
			DART_BUFF2[23] = SF;

			array_len2 = 24;

			resp2 = NOREPLY;

		}
	}
	else if( (response == DATA_PRESET_VOL) || (response == DATA_PRESET_AMO)  )
	{
		send_acknowledgement2(ACK);
		ack_send2 = false;

		//Effect the Change...
		 if(response == DATA_PRESET_VOL)
		 {
			 change_v2 = 1;
		 }
		 else if(response == DATA_PRESET_AMO)
		 {
			 change_p2 = 1;
		 }
	}
	else if(response == DATA_SUSPEND)
	{
		send_acknowledgement2(ACK);
		ack_send2 = false;

		command_response2 = false;

		if(settings_stream1[1].mode == AUTO_MODE)
		{
			if((pump_status_2 == STATUS_FILLING) || (pump_status_2 == STATUS_AUTH) )
			{
//				pump_status_ = STATUS_SUSPENDED;
				//Motor turned off

				//Effect the Change...
			}
		}
	}
	else if(response == DATA_RESUME)
	{
		send_acknowledgement2(ACK);
		ack_send2 = false;

		command_response2 = false;
		if(settings_stream1[1].mode == AUTO_MODE)
		{
			if(pump_status_2 == STATUS_SUSPENDED)
			{
				//Motor turned on

				//Effect the Change...
			}
		}
	}

	else if(response == DATA_REQUEST_CONFIG_CHANGE_INFO)
	{
		if(ack_send2 == true)
		{
			//==> send ack
			send_acknowledgement2(ACK);
			ack_send2 = false;
		}
		else
		{

			//TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
			//  _________________________________________________________________________________________________________________________________________________________________
			// | NOZ-NUM | TX-NUM | TX-CODE | TX-LEN | OTP-SEED [2 Bytes] | TIMESTAMP [5 Bytes] | CONFIG-1 PARTICULAR [1 Byte] | OLD-VALUE 1 [3 Bytes] | NEW-VALUE 1 [3 Bytes]|
			// |_________|________|_________|________|____________________|_____________________|______________________________|_______________________|______________________|__
			// ________________________________________________________________________________________________________
			//   |CONFIG-2 PARTICULAR [1 Byte] | OLD-VALUE 2 [3 Bytes] | NEW-VALUE 2 [3 Bytes] | CRC1 |CRC2 | ETX | SF |
			// __|_____________________________|_______________________|_______________________|______|_____|_____|____|
			//
			//YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY

			// 51 30 67 01 00 42 83 03 FA  <== From Controller

			// 51 30 67 01 01 83 43 03 FA  <== From Controller

			//'50 38  67 15  12 34  12 34 56 78 90   00 00 18 03 67 00 00   00 00 00 59 57 83 27   ab 03 fa '

			//####################################################################################################################################################################



			//=============================================================================//
			//       	 This transaction is sent by the pump at a Request of              //
			//        				 "REQUEST CONFIG CHANGE INFO"                          //
			//=============================================================================//

			uint16_t otp_seed;
			uint32_t time_stamp;

			unsigned char bcd_[6] = {0};  // Array to hold the BCD result

			uint16_t crc;

			uint8_t configCheck;


			retrieve_configFlag_fram(side_b);

			//=================================================================//
			// Checks for whether Config Mode has even been accessed @all
			//=================================================================//
			if(configMode2 == CONFIGUNMODIFIED)   // && (configChangeInfo_next2 == 0) )
			{
				send_acknowledgement2(EOT);
				return;
			}


			retrieve_config_otpSeed_time_fram(side_b);
			retrieve_settings_original_fram(side_b);

			otp_seed = configChange[1].otp_seed;
			time_stamp = configChange[1].time_stamp;

			ctrl2 |= 0x30;
			trans = 0x67;

			memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

			DART_BUFF2[0] = addr2;
			DART_BUFF2[1] = ctrl2;
			DART_BUFF2[2] = trans;

			//'50 38  67 15   12 34  12 34 56 78 90   00 00 18 03 67 00 00   00 00 00 59 57 83 27   ab 03 fa '

			int_to_bcd(otp_seed, bcd_);

			for (uint8_t ii = 0, j = 1; ii < 2; ii++, j--)   //j=>1 : MSB in it, LSB is in j=>0
			{
				DART_BUFF2[ii + 4] = bcd_[j];
			}

			memset(bcd_, 0, sizeof(bcd_));

			//'50 38  67 15   12 34  12 34 56 78 90   00 00 18 03 67 00 00   00 00 00 59 57 83 27   ab 03 fa '

			int_to_bcd(time_stamp, bcd_);

			for (uint8_t ii = 0, j = 4; ii < 5; ii++, j--)   //j=>4 : MSB in it, LSB is in j=>0
			{
				DART_BUFF2[ii + 6] = bcd_[j];
			}

			retrieve_configChange_trackNum_fram(side_b);

			if(configChangeInfo_next2 == 0)
			{
				if(trackNum[1].track_num0 > 24)
					trackNum[1].track_num0 = 0;

				trackNum[1].track_num = trackNum[1].track_num0;
				configCheck = configChange_notify_build2(trackNum[1].track_num0);
			}
			else if(configChangeInfo_next2 == 1)
			{
				if(trackNum[1].track_num > 24)
					trackNum[1].track_num = 0;

				configCheck = configChange_notify_build2(trackNum[1].track_num);
			}

			save_configChange_trackNum_fram(side_b);

			if(configCheck == 2)
			{
				DART_BUFF2[3] = 21;  //Data-Length;

				crc = crc_16(DART_BUFF2, 25);

				DART_BUFF2[25] = crc & 0x00FF;
				DART_BUFF2[26] = crc >> 8;
				DART_BUFF2[27] = ETX;
				DART_BUFF2[28] = SF;

				array_len2 = 29;
			}
			else if (configCheck == 1)
			{
				DART_BUFF2[3] = 14;  //Data-Length;

				crc = crc_16(DART_BUFF2, (25 - 7));

				DART_BUFF2[25 - 7] = crc & 0x00FF;
				DART_BUFF2[26 - 7] = crc >> 8;
				DART_BUFF2[27 - 7] = ETX;
				DART_BUFF2[28 - 7] = SF;

				array_len2 = (29 - 7);
			}

			//================================================================================//
			// If Config Mode is been accessed, but :
			// (i)  No changes effected,
			// (ii) Or all the possible changes info have been effectively passed on to GO
			//===============================================================================//
			else if (configCheck == 0)
			{
				send_acknowledgement2(EOT);

				clear_configFlag_fram(side_b);
				clear_configChange_trackNum_fram(side_b);
			}



			//'50 38 67 0E   12 34 12 34 56 78    00 00 18 03 67 00 00   00 00 00 59 57 83 27   ab 03 fa '

			command_2 = NO_COMMAND;
			resp2 = NOREPLY;

		}
	}
	else if(response == DATA_DATE_TIME_UPDATE)
	{

		if(ack_send2 == true)
		{
			//==> send ack
			send_acknowledgement2(ACK);
			ack_send2 = false;
		}
		else
		{
			/***********************************************
			*
			*'51 30 68 01 01 b3 40 03 fa'
			*
			************************************************/

			uint16_t crc;

			ctrl2 |= 0x30;
			trans = 0x68;
			lng = 0x01;

			memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

			DART_BUFF2[0] = addr;
			DART_BUFF2[1] = ctrl;
			DART_BUFF2[2] = trans;
			DART_BUFF2[3] = lng;
			DART_BUFF2[4] = 0x01;

			crc = crc_16(DART_BUFF2, 5);
			DART_BUFF2[5] = crc & 0x00FF;
			DART_BUFF2[6] = crc >> 8;
			DART_BUFF2[7] = ETX;
			DART_BUFF2[8] = SF;

			array_len2 = 9;

			resp2 = NOREPLY;
		}
	}
	else if(response == DATA_CLEAR_OTP_SESSION)
	{

		if(ack_send2 == true)
		{
			send_acknowledgement2(ACK);
			ack_send2 = false;

			///////////////////////////////////////////////////////////////////
			///////////////////  ACTUATE THE CHANGE... ////////////////////////

			clear_otpSeed_session_fram(side_b);

			///////////////////////////////////////////////////////////////////

		}
		else
		{
			/***********************************************
			*
			*'51 30 66 01 01 d2 83 03 fa'
			*
			************************************************/

			uint16_t crc;

			ctrl2 |= 0x30;
			trans = 0x66;
			lng = 0x01;

			memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

			DART_BUFF2[0] = addr;
			DART_BUFF2[1] = ctrl;
			DART_BUFF2[2] = trans;
			DART_BUFF2[3] = lng;
			DART_BUFF2[4] = 0x01;

			crc = crc_16(DART_BUFF2, 5);
			DART_BUFF2[5] = crc & 0x00FF;
			DART_BUFF2[6] = crc >> 8;
			DART_BUFF2[7] = ETX;
			DART_BUFF2[8] = SF;

			array_len2 = 9;

			resp2 = NOREPLY;
		}
	}

	else if(response == DATA_REQUEST_CALIBRATION_PARAM)
	{
		if(ack_send2 == true)
		{
			//==> send ack
			send_acknowledgement2(ACK);
			ack_send2 = false;
		}
		else
		{

			//TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
			//  ____________________________________________________________________________________________________________________________________________________________________________________
			// | NOZ-NUM | TX-NUM | TX-CODE | TX-LEN | DIRECTION [1 Byte] |   CTT [1 Byte]	 |	ACTUAL CALIBRATION [2 Bytes] | APPARENT CALIBRATION [1 Byte] | NEW APPARENT-CALIBRATION [1 Byte] |
			// |         |        |         |        | (ie. SET/REQUEST)  |  1 --> Non-Timed |            (1 DP)             |     		 (2 DP)              |			    (2 DP)               |
			// |         |        |         |        |                    |  2 --> Timed     |                               |                               |									 |
			// |_________|________|_________|________|____________________|__________________|_______________________________|_______________________________|___________________________________|__
			// _________________________
			//  | CRC1 |CRC2 | ETX | SF |
			//  |      |     |     |    |
			//  |      |     |     |    |
			// _|______|_____|_____|____|
			//
			//YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY

			//'50 38  69 04  01  20 04 00 46 f0 03 fa '   //Calibration Data Request

			//'50 38  69 04  00  20 04 05 47 0c 03 fa '   //In-bound Calibration-Set Command

			//####################################################################################################################################################################
			//
			// [{"nm":"P7","nz":0,"ctt":1,"ct":"20|0.5|0.4},{"nm":
			// ctt":2,"ct":"1500|20|0.5|0.4|2005}
			// ctt":2,"ct":"20|0.5|0.4-1500|20|0.5|0.4|2005}

			//=============================================================================//
			//       	 This transaction is sent by the pump at a Request of              //
			//        				 "REQUEST CALIBRATION INFO"                            //
			//=============================================================================//

			unsigned char bcd_[6] = {0};  // Array to hold the BCD result

			uint16_t crc;


			retrieve_ctSettings_fram(side_b);

			ctrl2 |= 0x30;
			trans = 0x69;
			lng	  = 0x06;

			memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

			DART_BUFF2[0] = addr2;
			DART_BUFF2[1] = ctrl2;
			DART_BUFF2[2] = trans;
			DART_BUFF2[3] = lng;
			DART_BUFF2[4] = 0x01;       //Signals sending out Calib Details to the Controller

//			vol_real2 = 345;

			int_to_bcd(vol_real2, bcd_);

			for (uint8_t ii = 0, j = 1; ii < 2; ii++, j--)  //2 Bytes
			{
				DART_BUFF2[ii + 5] = bcd_[j];
			}

			memset(bcd_, 0, sizeof(bcd_));

			float vol_calibrated2_ = (vol_calibrated2 - vol_real2);

//			vol_calibrated2_ = 0.82;

			roundedNum = round_off2(vol_calibrated2_, decimalPlaces);
			roundedNum = roundedNum * 100;
			num_ = (int)(roundedNum);

			int_to_bcd(num_, bcd_);

//			int_to_bcd(vol_calibrated2, bcd_);

			for (uint8_t ii = 0, j = 0; ii < 1; ii++)
			{
				DART_BUFF2[ii + 7] = bcd_[j];
			}

			memset(bcd_, 0, sizeof(bcd_));

			float vol_effective2_ = (vol_effective2 - vol_real2);

//			vol_effective2_ = 0.4;

			roundedNum = round_off2(vol_effective2_, decimalPlaces);
			roundedNum = roundedNum * 100;
			num_ = (int)(roundedNum);

			int_to_bcd(num_, bcd_);

//			int_to_bcd(vol_effective2, bcd_);

			for (uint8_t ii = 0, j = 0; ii < 1; ii++)
			{
				DART_BUFF2[ii + 8] = bcd_[j];
			}


			crc = crc_16(DART_BUFF2, 9);

			DART_BUFF2[9] = crc & 0x00FF;
			DART_BUFF2[10] = crc >> 8;
			DART_BUFF2[11] = ETX;
			DART_BUFF2[12] = SF;

			array_len2 = 13;

			command_2 = NO_COMMAND;
			resp2 = NOREPLY;
		}
	}
	else if(response == DATA_SET_CALIBRATION_PARAM)
	{
		send_acknowledgement2(ACK);
		ack_send2 = false;

		///////////////////////////////////////////////////////////////////
		///////////////////  ACTUATE THE CHANGE... ////////////////////////


		if (settings_stream1[1].mode == AUTO_MODE)
		{
			vol_real2 = set_calib2[0];
			vol_calibrated2 = ((set_calib2[1] * 0.1) + set_calib2[0]);
			vol_effective2 = ((set_calib2[2] * 0.1) + set_calib2[0]);

			save_ctSettings_fram(side_b);

			online_calibFlag2 = 1;
			save_online_calibFlag_fram(side_b);
		}

		///////////////////////////////////////////////////////////////////
	}
}


void send_acknowledgement1(response_enum response)
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
						ctrl2 = 0xC0 | r_TX2;	//reply with the previous msg tx
						break;

		case NACK:   //50H -> 5FH
						ctrl2 = 0x50 | r_TX2;
						break;

		case ACKPOLL:    //E0H -> EFH
						ctrl2 = 0xE0 | r_TX2;
						break;

		case EOT:   // 70H -> 7FH
						ctrl2 = 0x70 | r_TX2;
						break;
		default:
						break;
	}

	DART_BUFF2[1] = ctrl2;
	DART_BUFF2[2] = SF;

	array_len2 = 3;
}


void go_write1(void)
{
//	uint8_t id = 0;

    HAL_UART_Transmit_DMA(&huart2, DART_BUFF1, array_len);

//		  id++;

	//=======================================================================
}


void go_write2(void)
{

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

	save_settings_fram();   //save to eeprom
	load_settings(side_a); //load the settings into the internal variables
}

void go_setUnitPrice2(float price_update)
{
	settings_stream1[1].price_ = price_update2;

	save_settings_fram();   //save to eeprom
	load_settings(side_b); //load the settings into the internal variables
}

float go_fillingInfo_vol1(void)
{
	return amt_middle1;
}

float go_fillingInfo_vol2(void)
{
	return amt_middle2;
}

float go_fillingInfo_amt1(void)
{
	return price_upper1;
}

float go_fillingInfo_amt2(void)
{
	return price_upper2;
}

float go_fillingPrice1(void)
{
	return litre_price;
}

float go_fillingPrice2(void)
{
	return litre_price2;
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


//double round_off(float value, int decimalPlaces)
//{
//    double factor = pow(10, decimalPlaces);
//    return ceil(value * factor) / factor;
//}

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

void int_to_bcd_(int num, unsigned char *bcd, uint8_t bcd_size)
{
    for (int8_t i = bcd_size - 1; i >= 0; i--)
    {
        bcd[i] = (num % 10) | ((num / 10 % 10) << 4);
        num /= 100;
    }
}


int8_t countDigits(int number)
{
    int count = 0;

    // Handle negative numbers by making the number positive
    if (number < 0) {
        number = -number;
    }

    do {
        count++;
        number /= 10;
    } while (number != 0);

    return count;
}

int powerOfTen(uint8_t exponent)
{
    static const int powers[] =
    {
        1,
		10,
		100,
		1000,
		10000,
		100000,
		1000000,
		10000000,
		100000000
    };

    return powers[exponent];
}

// Function to convert BCD to integer
uint8_t bcd_to_int(unsigned char bcd)
{
    // Extract the tens and ones place digits
	uint8_t tens = (bcd >> 4) & 0xF;  // Get the upper 4 bits (tens place)
	uint8_t ones = bcd & 0xF;         // Get the lower 4 bits (ones place)

    // Combine the digits into an integer
    return (tens * 10 + ones);
}

// Function to convert multiple BCD bytes to a larger integer
uint32_t bcdArray_to_int(unsigned char bcd[], uint8_t length)
{
	uint32_t result = 0;
    for (uint8_t i = 0; i < length; i++) {
        result = result * 100 + bcd_to_int(bcd[i]);  // Shift and add the next BCD byte
    }
    return result;
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

    for (uint8_t i = 0; i < 8; ++i)
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

void send_nozzleStatus1(uint8_t buff_index)  //, float filling_price, uint8_t nozzle_status)
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

//				decimalPlaces = 1;
				decimalPlaces = dp_unitprice1;

				roundedNum = round_off(fillingPrice, decimalPlaces);
//				roundedNum = roundedNum * 10;
				roundedNum = roundedNum * powerOfTen(dp_unitprice1);
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

void send_nozzleStatus2(uint8_t buff_index)
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

				float fillingPrice = go_fillingPrice2();

				ctrl2 = TX2;
				ctrl2 |= 0x30;
				trans = 0x03;
				lng = 0x04;

//				memset(DART_BUFF1, 0, sizeof(DART_BUFF1));

				DART_BUFF2[0] = addr2;
				DART_BUFF2[1] = ctrl2;

				DART_BUFF2[2 + buff_index] = trans;
				DART_BUFF2[3 + buff_index] = lng;

//				decimalPlaces = 1;
				decimalPlaces = dp_unitprice2;

				roundedNum = round_off2(fillingPrice, decimalPlaces);
//				roundedNum = roundedNum * 10;
				roundedNum = roundedNum * powerOfTen(dp_unitprice2);
				num_ = (int)(roundedNum);

				memset(bcd_, 0, sizeof(bcd_));

				int_to_bcd(num_, bcd_);

				for (uint8_t i = 0, j = 2;  i < 3; i++, j--)
				{
					DART_BUFF2[i + 4 + buff_index] = bcd_[j];
				}

				if(nozzle_out2 == true) nozStatus = 1;
				else nozStatus = 0;

				nozStatus = (nozStatus << 4);
				nozIO = nozNum | nozStatus;

				DART_BUFF2[7 + buff_index] = nozIO;    //Nozzle 1
}


void send_pumpStatus1(uint8_t buff_index)
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


	switch (pump_status_1)
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

void send_pumpStatus2(uint8_t buff_index)
{
	uint8_t status_;

	ctrl2 = TX2;
	ctrl2 |= 0x30;
	trans = 0x01;
	lng = 0x01;

	DART_BUFF2[0] = addr2;
	DART_BUFF2[1] = ctrl2;

	DART_BUFF2[2 + buff_index] = trans;
	DART_BUFF2[3 + buff_index] = lng;


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

	DART_BUFF2[4 + buff_index] = status_;
}


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
//		DART_BUFF1[ii + 19] = trans;
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

void send_fillingInfo1(uint8_t buff_index)
{


	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
	//////////////////////////////////  GET FILLED_VOLUME_AND_AMOUNT //////////////////////////////////



	//=============================================================================//
	////////////////////////////////////  DC2  //////////////////////////////////////
	//        This transaction is sent by the pump at change of a value            //
	//        or if the pump receives the command RETURN FILLING INFORMATION       //
	//=============================================================================//
	//command_ ==> REQUEST_FILLING_INFO        //FILLED_VOLUME_AND_AMOUNT

	float vol_ = go_fillingInfo_vol1();

	float amo_ = go_fillingInfo_amt1();

	uint8_t decimalPlaces;
	double roundedNum;
	int num_;
	// unsigned int bcd;
	unsigned char bcd_[4] = {0};  // Array to hold the BCD result

	uint16_t crc;

	ctrl = TX;
	ctrl |= 0x30;
	trans = 0x02;
	lng = 0x08;

	memset(DART_BUFF1, 0, sizeof(DART_BUFF1));

	DART_BUFF1[0] = addr;
	DART_BUFF1[1] = ctrl;
	DART_BUFF1[2 + buff_index] = trans;
	DART_BUFF1[3 + buff_index] = lng;

//	decimalPlaces = 2;
	decimalPlaces = dp_vol1;

	roundedNum = round_off(vol_, decimalPlaces);
//	roundedNum = roundedNum * 100;
	roundedNum = roundedNum * powerOfTen(dp_vol1);
	num_ = (int)(roundedNum);

//	num_ = (num_ * powerOfTen(8 - countDigits(num_)));

//	int_to_bcd(num_, bcd_);

	int_to_bcd_(num_, bcd_, sizeof(bcd_));

//	for (uint8_t i = 0, j = 3; i < 4; i++, j--)
//	{
//		DART_BUFF1[i + 4 + buff_index] = bcd_[j];
//	}

	for (uint8_t i = 0; i < 4; i++)
	{
		DART_BUFF1[i + 4 + buff_index] = bcd_[i];
	}

//	decimalPlaces = 2;
	decimalPlaces = dp_amount1;

	roundedNum = round_off(amo_, decimalPlaces);
//	roundedNum = roundedNum * 100;
	roundedNum = roundedNum * powerOfTen(dp_amount1);
	num_ = (int)(roundedNum);

	//To ensure MSB remains in the 1st BCD Byte
//	num_ = (num_ * powerOfTen(8 - countDigits(num_)));



	memset(bcd_, 0, sizeof(bcd_));

//	int_to_bcd(num_, bcd_);

	int_to_bcd_(num_, bcd_, sizeof(bcd_));

	// for(uint8_t i = 0; i < 4; i++)
//	for (uint8_t i = 0, j = 3; i < 4; i++, j--)
//	{
//		DART_BUFF1[i + 8 + buff_index] = bcd_[j];
//	}

	for (uint8_t i = 0; i < 4; i++)
	{
		DART_BUFF1[i + 8 + buff_index] = bcd_[i];
	}



	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

}



void send_fillingInfo2(uint8_t buff_index)
{


	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
	//////////////////////////////////  GET FILLED_VOLUME_AND_AMOUNT //////////////////////////////////



	//=============================================================================//
	////////////////////////////////////  DC2  //////////////////////////////////////
	//        This transaction is sent by the pump at change of a value            //
	//        or if the pump receives the command RETURN FILLING INFORMATION       //
	//=============================================================================//
	//command_ ==> REQUEST_FILLING_INFO        //FILLED_VOLUME_AND_AMOUNT

	float vol_ = go_fillingInfo_vol2();

	float amo_ = go_fillingInfo_amt2();

	uint8_t decimalPlaces;
	double roundedNum;
	int num_;
	// unsigned int bcd;
	unsigned char bcd_[10] = {0};  // Array to hold the BCD result

	uint16_t crc;

	ctrl2 = TX2;
	ctrl2 |= 0x30;
	trans = 0x02;
	lng = 0x08;

	memset(DART_BUFF2, 0, sizeof(DART_BUFF2));

	DART_BUFF2[0] = addr2;
	DART_BUFF2[1] = ctrl2;
	DART_BUFF2[2 + buff_index] = trans;
	DART_BUFF2[3 + buff_index] = lng;

//	decimalPlaces = 2;
	decimalPlaces = dp_vol2;

	roundedNum = round_off2(vol_, decimalPlaces);
//	roundedNum = roundedNum * 100;
	roundedNum = roundedNum * powerOfTen(dp_vol2);
	num_ = (int)(roundedNum);

//	int_to_bcd(num_, bcd_);
//
//	for (uint8_t i = 0, j = 3; i < 4; i++, j--)
//	{
//		DART_BUFF2[i + 4 + buff_index] = bcd_[j];
//	}
//
//	decimalPlaces = 1;
//
//	roundedNum = round_off(amo_, decimalPlaces);
//	roundedNum = roundedNum * 10;
//	num_ = (int)(roundedNum);
//
//	memset(bcd_, 0, sizeof(bcd_));
//
//	int_to_bcd(num_, bcd_);
//
//	// for(uint8_t i = 0; i < 4; i++)
//	for (uint8_t i = 0, j = 3; i < 4; i++, j--)
//	{
//		DART_BUFF2[i + 8 + buff_index] = bcd_[j];
//	}

	int_to_bcd_(num_, bcd_, sizeof(bcd_));

	for (uint8_t i = 0; i < 4; i++)
	{
		DART_BUFF2[i + 4 + buff_index] = bcd_[i];
	}

//	decimalPlaces = 2;
	decimalPlaces = dp_amount2;

	roundedNum = round_off2(amo_, decimalPlaces);
//	roundedNum = roundedNum * 100;
	roundedNum = roundedNum * powerOfTen(dp_amount2);
	num_ = (int)(roundedNum);

	memset(bcd_, 0, sizeof(bcd_));

	int_to_bcd_(num_, bcd_, sizeof(bcd_));

	for (uint8_t i = 0; i < 4; i++)
	{
		DART_BUFF2[i + 8 + buff_index] = bcd_[i];
	}

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

}


uint8_t configChange_notify_build1(uint8_t track_num)
{
		//TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
		//  _________________________________________________________________________________________________________________________________________________________________
		// | NOZ-NUM | TX-NUM | TX-CODE | TX-LEN | OTP-SEED [2 Bytes] | TIMESTAMP [5 Bytes] | CONFIG-1 PARTICULAR [1 Byte] | OLD-VALUE 1 [3 Bytes] | NEW-VALUE 1 [3 Bytes]|
		// |_________|________|_________|________|____________________|_____________________|______________________________|_______________________|______________________|__
		// ________________________________________________________________________________________________________
		//   |CONFIG-2 PARTICULAR [1 Byte] | OLD-VALUE 2 [3 Bytes] | NEW-VALUE 2 [3 Bytes] | CRC1 |CRC2 | ETX | SF |
		// __|_____________________________|_______________________|_______________________|______|_____|_____|____|
		//
		//YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY

		//'50 38   67 15   12 34  12 34 56 78 90   00  00 18 03  67 00 00   00  00 00 59  57 83 27   ab 03 fa '

		//####################################################################################################################################################################

	unsigned char bcd_[6] = {0};  // Array to hold the BCD result

	uint8_t decimalPlaces = 2;
	double roundedNum;
	int num_;

	switch(track_num)
	{
			case MO1 :  // Mode => Manual/Auto                      					// index ==> 0x00
						trackNum[0].track_num++;
						if(settings_original_stream1[0].mode != settings_stream1[0].mode)
						{
							DART_BUFF1[11] = MO1;

							memset(bcd_, 0, sizeof(bcd_));

							int_to_bcd(settings_original_stream1[0].mode, bcd_);

							for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
							{
								DART_BUFF1[ii + 12] = bcd_[j];
							}

							memset(bcd_, 0, sizeof(bcd_));

							int_to_bcd(settings_stream1[0].mode, bcd_);

							for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
							{
								DART_BUFF1[ii + 15] = bcd_[j];
							}

							trackNum[0].track_num0 = (trackNum[0].track_num - 1);
							valid_pair1++;
						}

			case NA1 :  // Nozzle Address												// index ==> 0x01
						trackNum[0].track_num++;
						if(settings_original_stream1[0].noz_addr != settings_stream1[0].noz_addr)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = NA1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].noz_addr, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].noz_addr, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = NA1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].noz_addr, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].noz_addr, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case NO1 :	// Nozzle Override												// index ==> 0x02
						trackNum[0].track_num++;
						if(settings_original_stream1[0].noz_override != settings_stream1[0].noz_override)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = NO1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = NO1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case NC1 :  // Nozzle Count													// index ==> 0x03
						trackNum[0].track_num++;
						if(settings_original_stream2[0].noz_count != settings_stream2[0].noz_count)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = NC1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[0].noz_count, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[0].noz_count, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = NC1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[0].noz_count, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[0].noz_count, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case UN1 :  // Unit Price													// index ==> 0x04
						trackNum[0].track_num++;
						if(settings_original_stream1[0].price_ != settings_stream1[0].price_)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = UN1;

								memset(bcd_, 0, sizeof(bcd_));roundedNum = round_off(settings_original_stream1[0].price_, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream1[0].price_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_stream1[0].price_, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream1[0].price_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = UN1;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_original_stream1[0].price_, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream1[0].price_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_stream1[0].price_, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream1[0].price_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case TO1 :  // Timeout => No-Flow											// index ==> 0x05
						trackNum[0].track_num++;
						if(settings_original_stream2[0].timeOut_noFlow != settings_stream2[0].timeOut_noFlow)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = TO1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[0].timeOut_noFlow, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[0].timeOut_noFlow, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = TO1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[0].timeOut_noFlow, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[0].timeOut_noFlow, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case ML1 :  // Maximum Litre												// index ==> 0x06
						trackNum[0].track_num++;
						if(settings_original_stream1[0].max_amt_ != settings_stream1[0].max_amt_)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = ML1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].max_amt_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].max_amt_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = ML1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].max_amt_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].max_amt_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case PW1 :  // Password-1 Change											// index ==> 0x07
						trackNum[0].track_num++;
						if(strcmp(settings_original_stream3[0].passwd1, settings_stream3[0].passwd1) != 0)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = PW1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream3[0].passwd1, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream3[0].passwd1, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = PW1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream3[0].passwd1, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream3[0].passwd1, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case PW2 :  // Password-2 Change											// index ==> 0x08
						trackNum[0].track_num++;
						if(strcmp(settings_original_stream3[0].passwd2, settings_stream3[0].passwd2) != 0)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = PW2;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream3[0].passwd2, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream3[0].passwd2, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = PW2;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream3[0].passwd2, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream3[0].passwd2, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case PW3 :  // Password-3 Change											// index ==> 0x09
						trackNum[0].track_num++;
						if(strcmp(settings_original_stream3[0].passwd3, settings_stream3[0].passwd3) != 0)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = PW3;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream3[0].passwd3, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream3[0].passwd3, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = PW3;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream3[0].passwd3, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream3[0].passwd3, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case CV1 :  // Calibration Value-1 => Apparent Pulser-index					// index ==> 0x0A
						trackNum[0].track_num++;
						if(settings_original_stream1[0].pi_real != settings_stream1[0].pi_real)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = NO1;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_original_stream1[0].pi_real, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream1[0].pi_real, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_stream1[0].pi_real, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream1[0].pi_real, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = NO1;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_original_stream1[0].pi_real, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream1[0].pi_real, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_stream1[0].pi_real, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream1[0].pi_real, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case CV2 :  // Calibration Value-2 => Apparent Pulser-index					// index ==> 0x0B
						trackNum[0].track_num++;
						if(settings_original_stream1[0].pi_cal != settings_stream1[0].pi_cal)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = CV2;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_original_stream1[0].pi_cal, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream1[0].pi_cal, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_stream1[0].pi_cal, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream1[0].pi_cal, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = CV2;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_original_stream1[0].pi_cal, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream1[0].pi_cal, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_stream1[0].pi_cal, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream1[0].pi_cal, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case CS1 :  // Calibration Can-size											// index ==> 0x0C
						trackNum[0].track_num++;
						if(settings_original_stream2[0].calibration_measureCan != settings_stream2[0].calibration_measureCan)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = CS1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[0].calibration_measureCan, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[0].calibration_measureCan, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = CS1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[0].calibration_measureCan, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[0].calibration_measureCan, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case DP1 :  // Display Decimal Point-1	=> 	Amount						// index ==> 0x0D
						trackNum[0].track_num++;
						if(settings_original_stream1[0].dp_amount != settings_stream1[0].dp_amount)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = DP1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].dp_amount, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].dp_amount, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = DP1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].dp_amount, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].dp_amount, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case DP2 :  // Display Decimal Point-2	=> 	Volume						// index ==> 0x0E
						trackNum[0].track_num++;
						if(settings_original_stream1[0].dp_vol != settings_stream1[0].dp_vol)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = DP2;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].dp_vol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].dp_vol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = DP2;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].dp_vol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].dp_vol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case DP3 :  // Display Decimal Point-3	=> 	Unit Price						// index ==> 0x0F
						trackNum[0].track_num++;
						if(settings_original_stream1[0].dp_unitprice != settings_stream1[0].dp_unitprice)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = DP3;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].dp_unitprice, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].dp_unitprice, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = DP3;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].dp_unitprice, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].dp_unitprice, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case DS1 :   // Display Format => L/P or P/L									// index ==> 0x10
						trackNum[0].track_num++;
						if(settings_original_stream1[0].display_format != settings_stream1[0].display_format)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = DS1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].display_format, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].display_format, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = DS1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].display_format, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].display_format, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case DT1 :    // Volume Display Threshold										// index ==> 0x11
						trackNum[0].track_num++;
						if(settings_original_stream2[0].startUp_suppressVol != settings_stream2[0].startUp_suppressVol)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = DT1;

								memset(bcd_, 0, sizeof(bcd_));

								decimalPlaces = 4;
								roundedNum = round_off(settings_original_stream2[0].startUp_suppressVol, decimalPlaces);
								roundedNum = roundedNum * 10000;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream2[0].startUp_suppressVol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								decimalPlaces = 4;
								roundedNum = round_off(settings_stream2[0].startUp_suppressVol, decimalPlaces);
								roundedNum = roundedNum * 10000;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream2[0].startUp_suppressVol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = DT1;

								memset(bcd_, 0, sizeof(bcd_));

								decimalPlaces = 4;
								roundedNum = round_off(settings_original_stream2[0].startUp_suppressVol, decimalPlaces);
								roundedNum = roundedNum * 10000;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream2[0].startUp_suppressVol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								decimalPlaces = 4;
								roundedNum = round_off(settings_stream2[0].startUp_suppressVol, decimalPlaces);
								roundedNum = roundedNum * 10000;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream2[0].startUp_suppressVol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case SL1 :	// Shift Login Type												// index ==> 0x12
						trackNum[0].track_num++;
						if(settings_original_stream2[0].shift_login_type != settings_stream2[0].shift_login_type)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = SL1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[0].shift_login_type, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[0].shift_login_type, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = SL1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[0].shift_login_type, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[0].shift_login_type, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case SC1 :	// Shift-Count per day											// index ==> 0x13
						trackNum[0].track_num++;
						if(settings_original_stream2[0].number_of_shifts != settings_stream2[0].number_of_shifts)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = SC1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[0].number_of_shifts, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[0].number_of_shifts, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = SC1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[0].number_of_shifts, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[0].number_of_shifts, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case TN1 :	// Tone															// index ==> 0x14
						trackNum[0].track_num++;
						if(settings_original_stream2[0].keypress_tone != settings_stream2[0].keypress_tone)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = TN1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[0].keypress_tone, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[0].keypress_tone, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = TN1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[0].keypress_tone, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[0].keypress_tone, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case PT1 :	// Pulser Type => Quadrature/Non-Quadrature						// index ==> 0x15
						trackNum[0].track_num++;
						if(settings_original_stream2[0].pulser_type_ != settings_stream2[0].pulser_type_)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = PT1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[0].pulser_type_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[0].pulser_type_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = PT1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[0].pulser_type_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[0].pulser_type_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case PO1 :	// Pulser Offset Value											// index ==> 0x16
						trackNum[0].track_num++;
						if(settings_original_stream2[0].pulser_offset != settings_stream2[0].pulser_offset)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = PO1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = PO1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[0].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[0].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case SF1 :	// Start Slow-Flow Width										// index ==> 0x17
						trackNum[0].track_num++;
						if(settings_original_stream2[0].valve_salesStart != settings_stream2[0].valve_salesStart)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = SF1;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_original_stream2[0].valve_salesStart, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream2[0].valve_salesStart, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_stream2[0].valve_salesStart, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream2[0].valve_salesStart, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = SF1;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_original_stream2[0].valve_salesStart, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream2[0].valve_salesStart, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_stream2[0].valve_salesStart, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream2[0].valve_salesStart, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

			case SF2 :  		// Closing Slow-Flow Width                       				// index ==> 0x18  ==>0d24
						trackNum[0].track_num++;
						if(settings_original_stream2[0].valve_salesEnd != settings_stream2[0].valve_salesEnd)
						{
							if(valid_pair1 == 0)
							{
								DART_BUFF1[11] = SF2;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_original_stream2[0].valve_salesEnd, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream2[0].valve_salesEnd, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_stream2[0].valve_salesEnd, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream2[0].valve_salesEnd, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 15] = bcd_[j];
								}

								trackNum[0].track_num0 = (trackNum[0].track_num - 1);
								valid_pair1++;
							}
							else
							{
								DART_BUFF1[18] = SF2;

								memset(bcd_, 0, sizeof(bcd_));

								decimalPlaces = 2;

								roundedNum = round_off(settings_original_stream2[0].valve_salesEnd, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_stream2[0].valve_salesEnd, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream2[0].valve_salesEnd, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF1[ii + 22] = bcd_[j];
								}

								valid_pair1 = 0;

								return 2;
							}
						}

						//=====================================================================//
						// If only one modification is made on the config
						// valid_pair1 => 1  : @ this very stage
						//=====================================================================//
						if(valid_pair1 == 1)
						{
//							DART_BUFF1[3] = 13;  //Data-Length
							return 1;
						}
						//=====================================================================//
						// If no modification is made on the config
						// valid_pair1 => 0  : @ this very stage
						//=====================================================================//
						else if(valid_pair1 == 0)
						{
//							configMode1 = CONFIGUNMODIFIED;
//							save_configFlag_fram(side_a);
							return 0;
						}

		}
}


//TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
//  _________________________________________________________________________________________________________________________________________________________________
// | NOZ-NUM | TX-NUM | TX-CODE | TX-LEN | OTP-SEED [2 Bytes] | TIMESTAMP [4 Bytes] | CONFIG-1 PARTICULAR [1 Byte] | OLD-VALUE 1 [3 Bytes] | NEW-VALUE 1 [3 Bytes]|
// |_________|________|_________|________|____________________|_____________________|______________________________|_______________________|______________________|__
// ________________________________________________________________________________________________________
//   |CONFIG-2 PARTICULAR [1 Byte] | OLD-VALUE 2 [3 Bytes] | NEW-VALUE 2 [3 Bytes] | CRC1 |CRC2 | ETX | SF |
// __|_____________________________|_______________________|_______________________|______|_____|_____|____|
//
//YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY

//'50 38 67 14 12 34 12 34 56 78    00 00 18 03 67 00 00   00 00 00 59 57 83 27   ab 03 fa '

//####################################################################################################################################################################


uint8_t configChange_notify_build2(uint8_t track_num)
{
		//TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
		//  _________________________________________________________________________________________________________________________________________________________________
		// | NOZ-NUM | TX-NUM | TX-CODE | TX-LEN | OTP-SEED [2 Bytes] | TIMESTAMP [5 Bytes] | CONFIG-1 PARTICULAR [1 Byte] | OLD-VALUE 1 [3 Bytes] | NEW-VALUE 1 [3 Bytes]|
		// |_________|________|_________|________|____________________|_____________________|______________________________|_______________________|______________________|__
		// ________________________________________________________________________________________________________
		//   |CONFIG-2 PARTICULAR [1 Byte] | OLD-VALUE 2 [3 Bytes] | NEW-VALUE 2 [3 Bytes] | CRC1 |CRC2 | ETX | SF |
		// __|_____________________________|_______________________|_______________________|______|_____|_____|____|
		//
		//YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY

		//'50 38   67 15   12 34  12 34 56 78 90   00  00 18 03  67 00 00   00  00 00 59  57 83 27   ab 03 fa '

		//####################################################################################################################################################################

	unsigned char bcd_[6] = {0};  // Array to hold the BCD result

	uint8_t decimalPlaces = 2;
	double roundedNum;
	int num_;

	switch(track_num)
	{
			case MO1 :  // Mode => Manual/Auto                      					// index ==> 0x00
						trackNum[1].track_num++;
						if(settings_original_stream1[1].mode != settings_stream1[1].mode)
						{
							DART_BUFF2[11] = MO1;

							memset(bcd_, 0, sizeof(bcd_));

							int_to_bcd(settings_original_stream1[1].mode, bcd_);

							for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
							{
								DART_BUFF2[ii + 12] = bcd_[j];
							}

							memset(bcd_, 0, sizeof(bcd_));

							int_to_bcd(settings_stream1[1].mode, bcd_);

							for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
							{
								DART_BUFF2[ii + 15] = bcd_[j];
							}

							trackNum[1].track_num0 = (trackNum[1].track_num - 1);
							valid_pair2++;
						}

			case NA1 :  // Nozzle Address												// index ==> 0x01
						trackNum[1].track_num++;
						if(settings_original_stream1[1].noz_addr != settings_stream1[1].noz_addr)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = NA1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].noz_addr, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].noz_addr, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = NA1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].noz_addr, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].noz_addr, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case NO1 :	// Nozzle Override												// index ==> 0x02
						trackNum[1].track_num++;
						if(settings_original_stream1[1].noz_override != settings_stream1[1].noz_override)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = NO1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = NO1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case NC1 :  // Nozzle Count													// index ==> 0x03
						trackNum[1].track_num++;
						if(settings_original_stream2[1].noz_count != settings_stream2[1].noz_count)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = NC1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[1].noz_count, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[1].noz_count, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = NC1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[1].noz_count, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[1].noz_count, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case UN1 :  // Unit Price													// index ==> 0x04
						trackNum[1].track_num++;
						if(settings_original_stream1[1].price_ != settings_stream1[1].price_)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = UN1;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off2(settings_original_stream1[1].price_, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream1[1].price_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off2(settings_stream1[1].price_, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream1[1].price_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = UN1;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off2(settings_original_stream1[1].price_, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream1[1].price_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off2(settings_stream1[1].price_, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream1[1].price_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case TO1 :  // Timeout => No-Flow											// index ==> 0x05
						trackNum[1].track_num++;
						if(settings_original_stream2[1].timeOut_noFlow != settings_stream2[1].timeOut_noFlow)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = TO1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[1].timeOut_noFlow, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[1].timeOut_noFlow, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = TO1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[1].timeOut_noFlow, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[1].timeOut_noFlow, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case ML1 :  // Maximum Litre												// index ==> 0x06
						trackNum[1].track_num++;
						if(settings_original_stream1[1].max_amt_ != settings_stream1[1].max_amt_)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = ML1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].max_amt_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].max_amt_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = ML1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].max_amt_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].max_amt_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case PW1 :  // Password-1 Change											// index ==> 0x07
						trackNum[1].track_num++;
						if(strcmp(settings_original_stream3[1].passwd1, settings_stream3[1].passwd1) != 0)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = PW1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream3[1].passwd1, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream3[1].passwd1, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = PW1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream3[1].passwd1, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream3[1].passwd1, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case PW2 :  // Password-2 Change											// index ==> 0x08
						trackNum[1].track_num++;
						if(strcmp(settings_original_stream3[1].passwd2, settings_stream3[1].passwd2) != 0)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = PW2;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream3[1].passwd2, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream3[1].passwd2, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = PW2;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream3[1].passwd2, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream3[1].passwd2, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case PW3 :  // Password-3 Change											// index ==> 0x09
						trackNum[1].track_num++;
						if(strcmp(settings_original_stream3[1].passwd3, settings_stream3[1].passwd3) != 0)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = PW3;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream3[1].passwd3, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream3[1].passwd3, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = PW3;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream3[1].passwd3, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream3[1].passwd3, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case CV1 :  // Calibration Value-1 => Apparent Pulser-index					// index ==> 0x0A
						trackNum[1].track_num++;
						if(settings_original_stream1[1].pi_real != settings_stream1[1].pi_real)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = NO1;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off2(settings_original_stream1[1].pi_real, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream1[1].pi_real, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off2(settings_stream1[1].pi_real, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream1[1].pi_real, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = NO1;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off2(settings_original_stream1[1].pi_real, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream1[1].pi_real, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off2(settings_stream1[1].pi_real, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream1[1].pi_real, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case CV2 :  // Calibration Value-2 => Apparent Pulser-index					// index ==> 0x0B
						trackNum[1].track_num++;
						if(settings_original_stream1[1].pi_cal != settings_stream1[1].pi_cal)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = CV2;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off2(settings_original_stream1[1].pi_cal, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream1[1].pi_cal, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off2(settings_stream1[1].pi_cal, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream1[1].pi_cal, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = CV2;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off2(settings_original_stream1[1].pi_cal, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream1[1].pi_cal, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off2(settings_stream1[1].pi_cal, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream1[1].pi_cal, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case CS1 :  // Calibration Can-size											// index ==> 0x0C
						trackNum[1].track_num++;
						if(settings_original_stream2[1].calibration_measureCan != settings_stream2[1].calibration_measureCan)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = CS1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[1].calibration_measureCan, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[1].calibration_measureCan, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = CS1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[1].calibration_measureCan, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[1].calibration_measureCan, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case DP1 :  // Display Decimal Point-1	=> 	Amount						// index ==> 0x0D
						trackNum[1].track_num++;
						if(settings_original_stream1[1].dp_amount != settings_stream1[1].dp_amount)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = DP1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].dp_amount, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].dp_amount, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = DP1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].dp_amount, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].dp_amount, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case DP2 :  // Display Decimal Point-2	=> 	Volume						// index ==> 0x0E
						trackNum[1].track_num++;
						if(settings_original_stream1[1].dp_vol != settings_stream1[1].dp_vol)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = DP2;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].dp_vol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].dp_vol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = DP2;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].dp_vol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].dp_vol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case DP3 :  // Display Decimal Point-3	=> 	Unit Price						// index ==> 0x0F
						trackNum[1].track_num++;
						if(settings_original_stream1[1].dp_unitprice != settings_stream1[1].dp_unitprice)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = DP3;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].dp_unitprice, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].dp_unitprice, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = DP3;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].dp_unitprice, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].dp_unitprice, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case DS1 :   // Display Format => L/P or P/L									// index ==> 0x10
						trackNum[1].track_num++;
						if(settings_original_stream1[1].display_format != settings_stream1[1].display_format)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = DS1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].display_format, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].display_format, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = DS1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].display_format, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].display_format, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case DT1 :    // Volume Display Threshold										// index ==> 0x11
						trackNum[1].track_num++;
						if(settings_original_stream2[1].startUp_suppressVol != settings_stream2[1].startUp_suppressVol)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = DT1;

								memset(bcd_, 0, sizeof(bcd_));

								decimalPlaces = 4;
								roundedNum = round_off2(settings_original_stream2[1].startUp_suppressVol, decimalPlaces);
								roundedNum = roundedNum * 10000;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);


//								int_to_bcd(settings_original_stream2[1].startUp_suppressVol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								decimalPlaces = 4;
								roundedNum = round_off2(settings_stream2[1].startUp_suppressVol, decimalPlaces);
								roundedNum = roundedNum * 10000;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream2[1].startUp_suppressVol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = DT1;

								memset(bcd_, 0, sizeof(bcd_));

								decimalPlaces = 4;
								roundedNum = round_off(settings_original_stream2[1].startUp_suppressVol, decimalPlaces);
								roundedNum = roundedNum * 10000;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream2[1].startUp_suppressVol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								decimalPlaces = 4;
								roundedNum = round_off(settings_stream2[1].startUp_suppressVol, decimalPlaces);
								roundedNum = roundedNum * 10000;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream2[1].startUp_suppressVol, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case SL1 :	// Shift Login Type												// index ==> 0x12
						trackNum[1].track_num++;
						if(settings_original_stream2[1].shift_login_type != settings_stream2[1].shift_login_type)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = SL1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[1].shift_login_type, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[1].shift_login_type, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = SL1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[1].shift_login_type, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[1].shift_login_type, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case SC1 :	// Shift-Count per day											// index ==> 0x13
						trackNum[1].track_num++;
						if(settings_original_stream2[1].number_of_shifts != settings_stream2[1].number_of_shifts)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = SC1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[1].number_of_shifts, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[1].number_of_shifts, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = SC1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[1].number_of_shifts, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[1].number_of_shifts, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case TN1 :	// Tone															// index ==> 0x14
						trackNum[1].track_num++;
						if(settings_original_stream2[1].keypress_tone != settings_stream2[1].keypress_tone)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = TN1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[1].keypress_tone, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[1].keypress_tone, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = TN1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[1].keypress_tone, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[1].keypress_tone, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case PT1 :	// Pulser Type => Quadrature/Non-Quadrature						// index ==> 0x15
						trackNum[1].track_num++;
						if(settings_original_stream2[1].pulser_type_ != settings_stream2[1].pulser_type_)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = PT1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[1].pulser_type_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[1].pulser_type_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = PT1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream2[1].pulser_type_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream2[1].pulser_type_, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case PO1 :	// Pulser Offset Value											// index ==> 0x16
						trackNum[1].track_num++;
						if(settings_original_stream2[1].pulser_offset != settings_stream2[1].pulser_offset)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = PO1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = PO1;

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_original_stream1[1].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								int_to_bcd(settings_stream1[1].noz_override, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case SF1 :	// Start Slow-Flow Width										// index ==> 0x17
						trackNum[1].track_num++;
						if(settings_original_stream2[1].valve_salesStart != settings_stream2[1].valve_salesStart)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = SF1;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_original_stream2[1].valve_salesStart, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream2[1].valve_salesStart, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_stream2[1].valve_salesStart, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream2[1].valve_salesStart, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = SF1;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_original_stream2[1].valve_salesStart, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream2[1].valve_salesStart, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_stream2[1].valve_salesStart, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream2[1].valve_salesStart, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

			case SF2 :  		// Closing Slow-Flow Width                       				// index ==> 0x18  ==>0d24
						trackNum[1].track_num++;
						if(settings_original_stream2[1].valve_salesEnd != settings_stream2[1].valve_salesEnd)
						{
							if(valid_pair2 == 0)
							{
								DART_BUFF2[11] = SF2;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_original_stream2[1].valve_salesEnd, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream2[1].valve_salesEnd, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 12] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_stream2[1].valve_salesEnd, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream2[1].valve_salesEnd, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 15] = bcd_[j];
								}

								trackNum[1].track_num0 = (trackNum[1].track_num - 1);
								valid_pair2++;
							}
							else
							{
								DART_BUFF2[18] = SF2;

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_original_stream2[1].valve_salesEnd, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_original_stream2[1].valve_salesEnd, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 19] = bcd_[j];
								}

								memset(bcd_, 0, sizeof(bcd_));

								roundedNum = round_off(settings_stream2[1].valve_salesEnd, decimalPlaces);
								roundedNum = roundedNum * 100;
								num_ = (int)(roundedNum);

								int_to_bcd(num_, bcd_);

//								int_to_bcd(settings_stream2[1].valve_salesEnd, bcd_);

								for (uint8_t ii = 0, j = 2; ii < 3; ii++, j--)   //j=>2 : MSB in it, LSB is in j=>0
								{
									DART_BUFF2[ii + 22] = bcd_[j];
								}

								valid_pair2 = 0;

								return 2;
							}
						}

						//=====================================================================//
						// If only one modification is made on the config
						// valid_pair2 => 1  : @ this very stage
						//=====================================================================//
						if(valid_pair2 == 1)
						{
							return 1;
						}
						//=====================================================================//
						// If no modification is made on the config
						// valid_pair2 => 0  : @ this very stage
						//=====================================================================//
						else if(valid_pair2 == 0)
						{
//							configMode2 = CONFIGUNMODIFIED;
//							save_configFlag_fram(side_b);
							return 0;
						}

		}
}


//void parse_decode2(void)
//{
//
////	int size = r_raw_data.size();
//	// TRACE_DART("<%s>  r_ctrl: [%x]\n", __FUNCTION__,  r_ctrl);
//	int a = 0;
//	u8* r_raw_data_arr;
//
//	uint8_t data_[50] = {0},
//			data1[50] = {0},
//			data2[50] = {0};
//
//	uint16_t crc_original,
//			 crc_check;
//
//	uint8_t MSN = (r_ctrl2 & 0xF0);
//
////	switch (r_ctrl & 0xF0)
//	switch (MSN)
//	{
//
//			case 0x30:	//if control character == 0x30
//
//							for (int16_t i = 0; i < 150; i++)
//							{
//								//check the r_trans and r_lng byte ... NB: r_trans = r_raw_data[i] and r_lng = r_raw_data[i+1]
//								//returns status at the request of status or change of status
//
//								//Master's Commands to the Pump
//
//
//								//'50 36 01 01 00 9f d4 03 fa '
//								//===========================================================================//
//								//============================= COMMAND TO PUMP =============================//
//								//===========================================================================//
//								if( (r_raw_data2[i] == 0x01) && (r_raw_data2[i+1] == 0x01) )  //Trans-No  & Length
//								{
//									//this is a Master's Command
//									switch(r_raw_data2[i+2])
//									{
//											//for the commands, ctrl = 0x30; RETURN_PUMP_PARAM RETURN_PUMP_IDENTITY  SWITCH_OFF
//										case 0x00	:	{command_2 = GETSTATUS; break;}
//										case 0x02  	:	{command_2 = RETURN_PUMP_PARAM; break;}
//										case 0x03  	:	{command_2 = RETURN_PUMP_IDENTITY; break;}
//										case 0x04  	:   {command_2 = REQUEST_FILLING_INFO; break;}
//										case 0x05 	:	{command_2 = RESET1; break;}
//										case 0x06 	:	{command_2 = AUTHORISE; break;}
//										case 0x08 	:	{command_2 = STOP; break;}
//										case 0x0A 	:	{command_2 = SWITCH_OFF; break;}
//										default: break;
//									}
//
//									crc_original = r_raw_data2[i+4];
//									crc_original = (crc_original << 8);
//									crc_original = (crc_original + r_raw_data2[i+3]);
//
//									// Example GO Packet Frame
//									// 	-> UART1 [9]b '51 30 01 01 04 a3 5f 03 fa '
//
//									data_[0] = r_addr2;
//									data_[1] = r_ctrl2;
//
//									for(uint8_t i = 0, j = 2; i < 5; i++, j++)
//									{
//										data_[j] = r_raw_data2[i];
//									}
//
//
//									crc_check = crc_16(data_, 5);
//
//
//									if(crc_check == crc_original)
//									{
//										resp2 = DATA_COMMAND;
//										ack_send2 = true;
//									}
//									else
//									{
//										resp2 = CRC_ERROR;
//									}
//
//									dummyValue = 0;
//
//									t_exec3 = t_exec2 - t_exec1;
//									break;
//								}
//
//								//0x005138 02 08  00 00 00 33  00 00 04 78   03 04 00 14 50 11   1E 0C 03 FA
//
//								//Returns filled data and volume after requesting filling information
//								//===========================================================================//
//								//=============================== PRESET VOLUME =============================//
//								//===========================================================================//
//								else if( (r_raw_data2[i] == 0x03) && (r_raw_data2[i+1] == 0x04) ) //Trans-No  & Length
//								{
//									//
//									uint16_t preset_voll = 0;
//									uint8_t preset_vol[4];
//									char preset_volll[20];
//
//									crc_original = r_raw_data2[i+7];
//									crc_original = (crc_original << 8);
//									crc_original = (crc_original + r_raw_data2[i+6]);
//
//									//==============================================================//
//									//==================== VALIDATING THE CRC ======================//
//									data_[0] = r_addr2;
//									data_[1] = r_ctrl2;
//
//									for(uint8_t ii = 0, j = 2; ii < 8; ii++, j++)
//									{
//										data_[j] = r_raw_data2[ii];
//									}
//
//
//									crc_check = crc_16(data_, 8);
//
//
//									if(crc_check == crc_original)
//									{
//
//										for (uint8_t j = 0; j < 4; j++)
//										{
//											preset_vol[j] = r_raw_data2[i+2+j];
//										}
//
//										preset_voll = (preset_vol[0] * 1000000) + (preset_vol[1] * 10000) + (preset_vol[2] * 100) + (preset_vol[3] * 1);
//										sprintf(preset_volll, "0x%ld", preset_voll);
//
//										long n = strtol(preset_volll, NULL, 16);
//										uint32_t preset_vol0 = packed_bcd_to_decimal(n);
//
//
//										resp2 = DATA_PRESET_VOL;
//										ack_send2 = true;
//									}
//									else
//									{
//										resp2 = CRC_ERROR;
//									}
//
//									//==============================================================//
//
//									break;
//								}
//
//								//filling price and nozzle status is returned when you request status info or filling info
//								//===========================================================================//
//								//=============================== PRESET AMOUNT =============================//
//								//===========================================================================//
//								else if(r_raw_data2[i] == 0x04 && r_raw_data2[i+1] == 0x04)
//								{
//									uint16_t preset_amtt = 0;
//									uint8_t preset_amt[4];
//									char preset_amttt[20];
//
//									crc_original = r_raw_data2[i+7];
//									crc_original = (crc_original << 8);
//									crc_original = (crc_original + r_raw_data2[i+6]);
//
//									data_[0] = r_addr2;
//									data_[1] = r_ctrl2;
//
//									for(uint8_t ii = 0, j = 2; ii < 8; ii++, j++)
//									{
//										data_[j] = r_raw_data2[ii];
//									}
//
//
//									crc_check = crc_16(data_, 8);
//
//
//									if(crc_check == crc_original)
//									{
//										for (uint8_t j = 0; j < 4; j++)
//										{
//											preset_amt[j] = r_raw_data2[i+2+j];
//										}
//
//										preset_amtt = (preset_amt[0] * 1000000) + (preset_amt[1] * 10000) + (preset_amt[2] * 100) + (preset_amt[3] * 1);
//										sprintf(preset_amttt, "0x%ld", preset_amtt);
//
//										long n = strtol(preset_amttt, NULL, 16);
//
//										uint32_t preset_amt0 = packed_bcd_to_decimal(n);
//
//
//										resp2 = DATA_PRESET_AMO;
//										ack_send2 = true;
//									}
//									else
//									{
//										resp2 = CRC_ERROR;
//									}
//									break;
//								}
//								//'50 37 05 03 00 58 10 a2 0a 03 fa
//								//===========================================================================//
//								//========================== PRICE-UPDATE (DP => 1)==========================//
//								//===========================================================================//
//								else if(r_raw_data2[i] == 0x05 && r_raw_data2[i+1] == 0x03)
//								{
//									uint16_t price_updatee = 0;
//									uint8_t price_update[3];
//									char price_updateee[20];
//
//									crc_original = r_raw_data2[i+6];
//									crc_original = (crc_original << 8);
//									crc_original = (crc_original + r_raw_data2[i+5]);
//
//
//									//==============================================================//
//									//==================== VALIDATING THE CRC ======================//
//
//									data_[0] = r_addr2;
//									data_[1] = r_ctrl2;
//
//									//'50 37 05 03 00 58 10 a2 0a 03 fa
//
//									for(uint8_t ii = 0, j = 2; ii < 7; ii++, j++)
//									{
//										data_[j] = r_raw_data2[ii];
//									}
//
//
//									crc_check = crc_16(data_, 7);
//
//
//									if(crc_check == crc_original)
//									{
//										//'50 37 05 03 00 58 10 a2 0a 03 fa
//										for (uint8_t j = 0; j < 3; j++)
//										{
//											price_update[j] = r_raw_data2[i+2+j];
//										}
//
//										price_updatee = ( (price_update[0] * 10000) + (price_update[1] * 100) + (price_update[2] * 1) );
//										sprintf(price_updateee, "0x%ld", price_updatee);
//
//										// char num[]="0x3076";
//										long n = strtol(price_updateee, NULL, 16);
//										uint32_t price_update0 = packed_bcd_to_decimal(n);
//
//
//										resp2 = DATA_PRICE_UPDATE;
//										ack_send2 = true;
//									}
//									else
//									{
//										resp2 = CRC_ERROR;
//									}
//									//=================== DONE, VALIDATING THE CRC =================//
//									//==============================================================//
//
//									break;
//								}
//
//								//'50 35 65 01 01 1f 8f 03 fa '
//								//===========================================================================//
//								//============================== GET-TOTALIZER ==============================//
//								//===========================================================================//
//								else if(r_raw_data2[i] == 0x65 && r_raw_data2[i+1] == 0x01)
//								{
//									uint16_t price_updatee = 0;
//									uint8_t price_update[3];
//									char price_updateee[20];
//
//									crc_original = r_raw_data2[i+4];
//									crc_original = (crc_original << 8);
//									crc_original = (crc_original + r_raw_data2[i+3]);
//
//
//									//==============================================================//
//									//==================== VALIDATING THE CRC ======================//
//									//'50 35 65 01 01 1f 8f 03 fa '
//									data_[0] = r_addr2;
//									data_[1] = r_ctrl2;
//
//									for(uint8_t ii = 0, j = 2; ii < 5; ii++, j++)
//									{
//										data_[j] = r_raw_data2[ii];
//									}
//
//
//									crc_check = crc_16(data_, 5);
//
//
//									if(crc_check == crc_original)
//									{
//										command_2 = REQUEST_VOL_TOTAL_COUNT;
//
//										resp2 = DATA_REQUEST_VOL_TOTAL_COUNT;
//										ack_send2 = true;
//									}
//									else
//									{
//										resp2 = CRC_ERROR;
//									}
//									//=================== DONE, VALIDATING THE CRC =================//
//									//==============================================================//
//
//									break;
//								}
//
//
//							}
//
//							break;
//
//			case 0x20	:	resp2 = r_POLL;
//
//							break;
//
//			case 0xC0	: 	resp2 = r_ACK;
//							// TRACE_DART("<>--- msg - ack\n");
//							MSN = (r_ctrl & 0x0F);
//							if(MSN == TX2)
//							{
//								checked = 1;
//								TX2++;
//							}
//
//							command_2 = NO_COMMAND;
//
//							command_response2 = false;
//
//							resp2 = NOREPLY;
//
//							break; //increase the tx on receiving an ACK
//
//			case 0x50	:	resp2 = r_NACK;
//							MSN = (r_ctrl & 0x0F);
//							if(MSN == TX2)
//							{
//
//							}
//
////							TX = 0;
//							break;
//
//			case 0x70	:	resp2 = r_EOT;
//							// TRACE_DART("<>--- msg - eot\n");
//							break;
//			default		:
//							resp2 = JUNK;
//							break;
//	}//switch ctrl
//}
//
//
