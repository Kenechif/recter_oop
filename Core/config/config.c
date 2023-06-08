///*
// * config.c
// *
// *  Created on: Oct 14, 2022
// *      Author: tunjo
// */
#include "config.h"
#include "time.h"

//#######################################

char device_id [] =	"860537064685340";

//#######################################

config_struct config_data;
//
////transaction tranx_save[2],
////			transaction_temp[2];

transaction tranx_save, tranx_serverResponse;

extern RNG_HandleTypeDef hrng;

extern uint8_t hour,minute,second,day,month,year,dayofweek;

//extern pump_names pumpName[2];

void read_config()
{
	sprintf((char*)config_data.others.gid,"864120054201604");
}

ep_ ep;

time_t epochTime_present;

char ep_message[400] = {0},
	 ep_message_rcvd[400] = {0},
	 sndd[500] = {0};

const int totalTranx_loc  =  60;
const int totalTranx1_loc =  0;
const int synchedTranx_loc  =  65;
const int synchedTranx1_loc =  0;
const int totalAutoTranx_loc  =  70;
const int totalAutoTranx1_loc =  0;
const int synchedAutoTranx_loc  =  75;
const int synchedAutoTranx1_loc =  0;
const int totalEvents_loc  =  80;
const int totalEvents1_loc =  0;
const int synchedEvents_loc  =  85;
const int synchedEvents1_loc =  0;

const int totalTranxA_loc  =  250,
		  totalTranxA1_loc =  0,
		  totalTranxB_loc  =  253,
		  totalTranxB1_loc =  0,
		  synchedTranxA_loc  =  256,
		  synchedTranxA1_loc =  0,
		  synchedTranxB_loc  =  259,
		  synchedTranxB1_loc =  0;

const int totalAutoTranxA_loc  =  270,
		  totalAutoTranxA1_loc =  0,
		  totalAutoTranxB_loc  =  273,
		  totalAutoTranxB1_loc =  0,
		  synchedAutoTranxA_loc  =  276,
		  synchedAutoTranxA1_loc =  0,
		  synchedAutoTranxB_loc  =  279,
		  synchedAutoTranxB1_loc =  0;

const int lastSynchedFlashA_loc = 262;
const int lastSynchedFlashB_loc = 265;

static int8_t sendEp5 = 0,
			  sending = 0,
			  firstTyme = 1;

int8_t ep0_sent = 0;

const int8_t token_loc = 80;  //token15_loc = 150; -> 154
const int8_t rootToken_loc = 155;
const long array_loc = 160;   //array15_loc = 174;
const int8_t arrayCount_loc = 176;  //up to 190
const int unsynchedFlash_loc = 191;   //up to 225

uint8_t token_array[15],
		ep2token_array[3],
		missingToken_array[15],
		countt = 0;

long token_track[15] = {0};

void ep_send(ep_ designation)
{
	static firstTime = 1;

	switch(designation)
	{
		case ep0: 	if(firstTime == 1)
					{
						sending = 0;

						ep0_save.timestamp = RtcToInt(2019);
						ep0_save.token = generate_tk();


						ep0_save.pump[0].tolalizer_first = ep5_save.firstTotalizer[0].totalizer;
						ep0_save.pump[0].totalizerFirst_timestamp = ep5_save.firstTotalizer[0].timestamp;
						ep0_save.pump[1].tolalizer_first = ep5_save.firstTotalizer[1].totalizer;
						ep0_save.pump[1].totalizerFirst_timestamp = ep0_save.pump[0].totalizerFirst_timestamp;

//						ep5_save.firstTotalizer[0].totalizer = ep0_save.pump[0].tolalizer_first;
//						ep5_save.firstTotalizer[1].totalizer = ep0_save.pump[1].tolalizer_first;
//						ep5_save.firstTotalizer[0].timestamp = ep0_save.pump[0].totalizerFirst_timestamp;

						ep0_save.pump[0].status = 1;
						ep0_save.pump[0].totalizer = totaliser_vol1c;
						strcpy(ep0_save.pump[0].nozzle_name, pumpName[0].pump_name);
						ep0_save.pump[1].status = 1;
						ep0_save.pump[1].totalizer = totaliser_vol2c;
						strcpy(ep0_save.pump[1].nozzle_name, pumpName[1].pump_name);
						ep0_save.reboot_cause = 0;

						sprintf(ep_message,
							  "{\"ep\":0,\"di\":\"%s\",\"tk\":%ld,\"pumps\":[{\"st\":%d,\"tz\":%0.3f,\"ft\":%0.3f,\"fttm\":%ld,\"nm\":\"%s\"},{\"st\":%d,\"tz\":%0.3f,\"ft\":%0.3f,\"fttm\":%ld,\"nm\":\"%s\"}],\"bt\":%ld,\"tm\":%ld,\"rs\":%d}",
							  device_id, ep0_save.token, ep0_save.pump[0].status, ep0_save.pump[0].totalizer,
							  ep0_save.pump[0].tolalizer_first, ep0_save.pump[0].totalizerFirst_timestamp,
							  ep0_save.pump[0].nozzle_name, ep0_save.pump[1].status, ep0_save.pump[1].totalizer,
							  ep0_save.pump[1].tolalizer_first,
							  ep0_save.pump[1].totalizerFirst_timestamp, ep0_save.pump[1].nozzle_name,
							  ep0_save.boot_time, ep0_save.timestamp, ep0_save.reboot_cause);

						list_push(ep0_save.token, ep0);
						server_write(ep_message);
						firstTime = 0;
					}
					else if( (firstTime == 0) && (sending == 1) )
					{
						server_write(ep_message);

						if(sending == 0)
						{
							ep0_sent = 1;
							firstTime = 1;
						}
					}
					break;

		case ep1a: 	if(firstTime == 1)
					{
						sending = 0;

						ep1a_save.timestamp = RtcToInt(2019);
						ep1a_save.token = generate_tk();

						ep1a_save.pump[0].status = 1;
						ep1a_save.pump[0].totalizer = totaliser_vol1c;
						ep1a_save.pump[0].tolalizer_first = ep5_save.firstTotalizer[0].totalizer;
						ep1a_save.pump[0].totalizerFirst_timestamp = ep5_save.firstTotalizer[0].timestamp;

						strcpy(ep1a_save.pump[0].nozzle_name, pumpName[0].pump_name);
						ep1a_save.pump[1].status = 1;
						ep1a_save.pump[1].totalizer = totaliser_vol2c;
						ep1a_save.pump[1].tolalizer_first = ep5_save.firstTotalizer[1].totalizer;
						ep1a_save.pump[1].totalizerFirst_timestamp = ep5_save.firstTotalizer[0].timestamp;
						strcpy(ep1a_save.pump[1].nozzle_name, pumpName[1].pump_name);
						ep1a_save.timestamp = ep0_save.timestamp;
						ep1a_save.firmware_version = 2023;
						ep1a_save.storage_loc = 'i';

						sprintf(ep_message,
								"{\"ep\":1,\"di\":\"%s\",\"tk\":%ld,\"pumps\":[{\"st\":%d,\"tz\":%0.3f,\"ft\":%0.3f,\"fttm\":%ld,\"nm\":\"%s\"},{\"st\":%d,\"tz\":%0.3f,\"ft\":%0.3f,\"fttm\":%ld,\"nm\":\"%s\"}],\"tm\":%ld,\"fv\":%d,\"sl\":\'%c\'}",
								device_id, ep1a_save.token, ep1a_save.pump[0].status,
								ep1a_save.pump[0].totalizer, ep1a_save.pump[0].tolalizer_first,
								ep1a_save.pump[0].totalizerFirst_timestamp, ep1a_save.pump[0].nozzle_name,
								ep1a_save.pump[1].status, ep1a_save.pump[1].totalizer, ep1a_save.pump[1].tolalizer_first,
								ep1a_save.pump[1].totalizerFirst_timestamp, ep1a_save.pump[1].nozzle_name, ep1a_save.timestamp,
								ep1a_save.firmware_version, ep1a_save.storage_loc);

						list_push(ep1a_save.token, ep1a);
						server_write(ep_message);
						firstTime = 0;
					}
					else if( (firstTime == 0) && (sending == 1) )
					{
						server_write(ep_message);

						if(sending == 0)
						{
							firstTime = 1;
							firstTyme = 1;
						}
					}

					break;

		case ep1b: 	if(firstTime == 1)
					{
						sending = 0;
						ep1b_save.timestamp = RtcToInt(2019);
						ep1b_save.token = generate_tk();

						ep1b_save.pump[0].status = 1;
						ep1b_save.pump[0].totalizer = totaliser_vol1c;
						ep1b_save.pump[0].tolalizer_first = ep5_save.firstTotalizer[0].totalizer;
						ep1b_save.pump[0].totalizerFirst_timestamp = ep5_save.firstTotalizer[0].timestamp;
						strcpy(ep1b_save.pump[0].nozzle_name, pumpName[0].pump_name);
						ep1b_save.pump[1].status = 1;
						ep1b_save.pump[1].totalizer = totaliser_vol2c;
						ep1b_save.pump[1].tolalizer_first = ep5_save.firstTotalizer[1].totalizer;
						ep1b_save.pump[1].totalizerFirst_timestamp = ep5_save.firstTotalizer[0].timestamp;
						strcpy(ep1b_save.pump[1].nozzle_name, pumpName[1].pump_name);

						ep1b_save.total_tranx = (ep1b_save.total_tranxA + ep1b_save.total_tranxB);
						ep1b_save.synched_tranx = (ep1b_save.synched_tranxA + ep1b_save.synched_tranxB);

						ep1b_save.total_events = 0;
						ep1b_save.synched_events = 0;
						strcpy(ep1b_save.firmware_date, "Nov 28 2020");
						strcpy(ep1b_save.firmware_time, "17:07:30");
						ep1b_save.mem_usage = 0.000;
						strcpy(ep1b_save.chip_type, "STM32F4");
						ep1b_save.mem_usage_auto = 0.000;
						ep1b_save.boot_time = ep0_save.timestamp;
						ep1b_save.firmware_version = 2023;
						ep1b_save.storage_loc = 'i';

						sprintf(ep_message,
								"{\"ep\":1,\"di\":\"%s\",\"tk\":%ld,\"pumps\":[{\"st\":%d,\"tz\":%0.3f,\"ft\":%0.3f,\"fttm\":%ld,\"nm\":\"%s\"},{\"st\":%d,\"tz\":%0.3f,\"ft\":%0.3f,\"fttm\":%ld,\"nm\":\"%s\"}],\"tt\":%d,\"st\":%d,\"tta\":%d,\"sta\":%d,\"se\":%d,\"te\":%d,\"fwd\":\"%s\",\"fwt\":\"%s\",\"mu\":%0.3f,\"ch\":\"%s\",\"mua\":%0.3f,\"bt\":%ld,\"tm\":%ld,\"fv\":%d,\"sl\":\'%c\'}",
								device_id, ep1b_save.token, ep1b_save.pump[0].status, ep1b_save.pump[0].totalizer,
								ep1b_save.pump[0].tolalizer_first, ep1b_save.pump[0].totalizerFirst_timestamp, ep1b_save.pump[0].nozzle_name,
								ep1b_save.pump[1].status, ep1b_save.pump[1].totalizer, ep1b_save.pump[1].tolalizer_first,
								ep1b_save.pump[1].totalizerFirst_timestamp, ep1b_save.pump[1].nozzle_name, ep1b_save.total_tranx,
								ep1b_save.synched_tranx, ep1b_save.total_autoTranx, ep1b_save.synched_autoTranx, ep1b_save.synched_events,
								ep1b_save.total_events,
								ep1b_save.firmware_date, ep1b_save.firmware_time, ep1b_save.mem_usage, ep1b_save.chip_type,
								ep1b_save.mem_usage_auto, ep1b_save.boot_time, ep1b_save.timestamp, ep1b_save.firmware_version,
								ep1b_save.storage_loc);

						list_push(ep1b_save.token, ep1b);
						server_write(ep_message);
						firstTime = 0;
					}
					else if( (firstTime == 0) && (sending == 1) )
					{
						server_write(ep_message);

						if(sending == 0)
						{
							firstTime = 1;
							firstTyme = 1;
						}
					}
					break;

		case ep2:   if(firstTyme == 1)
					{
						sending = 0;

						generateTransc_ID(tranx_save.transaction_id);

						tranx_save.transaction_type = 'a';
						tranx_save.transaction_period = 3204;
						tranx_save.storage_loc = 'i';
						strcpy(tranx_save.tag, "4jrt0v");

						sprintf(ep_message,
								"{\"ep\":2,\"di\":\"%s\",\"tk\":%ld,\"tm\":%ld,\"ti\":\"%s\",\"ta\":%0.3f,\"tv\":%0.3f,\"pl\":%0.3f,\"tz\":%0.3f,\"pm\":\"%s\",\"pa\":%d,\"pr\":\"%s\",\"tt\":\'%c\',\"tp\":%d,\"sl\":\'%c\',\”tg\”:\”%s\”}",
								device_id, tranx_save.token, tranx_save.timestamp,
								tranx_save.transaction_id, tranx_save.transaction_price,
								tranx_save.transaction_vol, tranx_save.litre_price,
								tranx_save.totalizer, tranx_save.nozzle_name,
								tranx_save.nozzle_address, tranx_save.nozzle_product,
								tranx_save.transaction_type, tranx_save.transaction_period,
								tranx_save.storage_loc, tranx_save.tag);

						list_push(tranx_save.token, ep2);
						server_write(ep_message);
						firstTyme = 0;
					}
					else if( (firstTyme == 0) && (sending == 1) )
					{
						server_write(ep_message);

						if(sending == 0)
						{
//							ep2_sent = 1;
							firstTyme = 1;
						}
					}
//					ep2Token_push(tranx_save.token);
//					if(countt++ == 5)
//					{
//						tokenTrack_array();
//						array_bubbleSort();
//						missingTokenArray_get();
//						countt = 0;
//					}

					break;

		case ep5: 	if( (sendEp5 == 0) && (firstTime == 1) )
					{
						sending = 0;
						ep5_save.timestamp = RtcToInt(2019);
						ep5_save.token = generate_tk();

						strcpy(ep5_save.sentEntry_count, "0|0");
						strcpy(ep5_save.firstTotalizer[0].nozzle_name, pumpName[0].pump_name);

						sprintf(ep_message,
							"{\"ep\":5,\"di\":\"%s\",\"tk\":%ld,\"cn\":\"%s\",\"FT\":[{\"pn\":\"%s\",\"tm\":%ld,\"tz\":%0.3f}],\"tm\":%ld}",
							device_id, ep5_save.token, ep5_save.sentEntry_count,
							ep5_save.firstTotalizer[0].nozzle_name, ep5_save.firstTotalizer[0].timestamp,
							ep5_save.firstTotalizer[0].totalizer, ep5_save.timestamp);

//						sendEp5 = 1;
						list_push(ep5_save.token, ep5);
						server_write(ep_message);
						firstTime = 0;
					}
					else if( (sendEp5 == 0) && (firstTime == 0) )
					{
						server_write(ep_message);

						if(sending == 0)
						{
							sendEp5 = 1;
							firstTime = 1;
							firstTyme = 1;
						}
					}
					else if( (sendEp5 == 1) && (firstTime == 1) )
					{
						sending = 0;
						ep5_save.timestamp = RtcToInt(2019);
						ep5_save.token = generate_tk();

						strcpy(ep5_save.sentEntry_count, "0|0");
						strcpy(ep5_save.firstTotalizer[1].nozzle_name, pumpName[1].pump_name);
//						ep5_save.firstTotalizer.totalizer[1] = totaliser_vol2c;

						sprintf(ep_message,
							"{\"ep\":5,\"di\":\"%s\",\"tk\":%ld,\"cn\":\"%s\",\"FT\":[{\"pn\":\"%s\",\"tm\":%ld,\"tz\":%0.3f}],\"tm\":%ld}",
							device_id, ep5_save.token, ep5_save.sentEntry_count,
							ep5_save.firstTotalizer[1].nozzle_name, ep5_save.firstTotalizer[1].timestamp,
							ep5_save.firstTotalizer[1].totalizer, ep5_save.timestamp);

						list_push(ep5_save.token, ep5);
						server_write(ep_message);
						firstTime = 0;
					}
					else if( (sendEp5 == 1) && (firstTime == 0) )
					{
						server_write(ep_message);

						if(sending == 0)
						{
							sendEp5 = 0;
							firstTime = 1;
							firstTyme = 1;
						}
					}

					break;
	}
}


void epSend_interval(void)
{
	static int8_t aflag = 0,
				  aflag1 = 0,
				  halfMinuteIncrementer = 0,
				  last_day,
				  ep2b = 0,
				  ep5b = 0;


	uint8_t array_count = 0;

//	static long epochTime_past = 0;

	static uint16_t epochTime_past = 0,
					time_past = 0;

//	long epochTime_present;
	uint16_t epochTime_present,
			 time_present;

	uint16_t time_interval,
			 time_interval1;

	int flash_locc,
		pg;


	//============================================//
	// 				EP0 ROUTINE SENDING			  //
	//============================================//

	if (ep0_sent == 0)
	{
		ep_send(ep0);
	}

	//============================================//


//	epochTime_present = RtcToInt(2019);
	epochTime_present = timer_ep;

	if(aflag == 0)
	{

		epochTime_past = epochTime_present;

		aflag = 1;
	}

	time_interval = epochTime_present - epochTime_past;


	//============================================//
	// 				EP2 ROUTINE SENDING			  //
	//============================================//
	if(time_interval >= 30000)   //30 sec interval
	{

		if ( (firstTyme == 1) && (ep2b == 0) )
		{
//		aflag = 0;
//			halfMinuteIncrementer++;

			if(ep1b_save.synched_tranxA != ep1b_save.total_tranxA)
			{
				synchedLog_get(side_a);
				tranx_save.timestamp = RtcToInt_synchedTranx(2019, side_a);
				tranxA_token = generate_tk();
				tranx_save.token = 	tranxA_token;

	//			EEPROM_Read( (rootToken_loc), 0, &pg, sizeof(pg));
	//			EEPROM_Read((unsynchedFlash_loc + ( i * 2) ), 0, &flash_locc, sizeof(flash_locc));

				tranx_save.transaction_price = synchedLog_a_new.pr__;
				tranx_save.transaction_vol = synchedLog_a_new.vol__;
				tranx_save.litre_price = synchedLog_a_new.litre_price_;
				tranx_save.totalizer = synchedLog_a_new.totaliserVol_cal;
				strcpy(tranx_save.nozzle_name, pumpName[0].pump_name);
				tranx_save.nozzle_address = 1;
				strcpy(tranx_save.nozzle_product, settings[0].product_);

				ep_send(ep2);
				return;
			}
		}
		else if ( (firstTyme == 0) && (ep2b == 0) )
		{
			ep_send(ep2);

			if(firstTyme == 0)
			{
				return;
			}

			ep2b = 1;
		}
		if( (firstTyme == 1) && (ep2b == 1) )
		{
			if(ep1b_save.synched_tranxB != ep1b_save.total_tranxB)
			{
				synchedLog_get(side_b);
				tranx_save.timestamp = RtcToInt_synchedTranx(2019, side_b);
				tranxB_token = generate_tk();
				tranx_save.token = 	tranxB_token;

				tranx_save.transaction_price = synchedLog_b_new.pr__;
				tranx_save.transaction_vol = synchedLog_b_new.vol__;
				tranx_save.litre_price = synchedLog_b_new.litre_price_;
				tranx_save.totalizer = synchedLog_b_new.totaliserVol_cal;
				strcpy(tranx_save.nozzle_name, pumpName[1].pump_name);
				tranx_save.nozzle_address = 2;
				strcpy(tranx_save.nozzle_product, settings[1].product_);

				ep_send(ep2);
				return;
			}
		}
		else if ( (firstTyme == 0) && (ep2b == 1) )
		{
			ep_send(ep2);

			if(firstTyme == 0)
			{
				return;
			}

			ep2b = 0;
		}

		aflag = 0;
		halfMinuteIncrementer++;
	}
	//============================================//





	//============================================//
	// 			  EP1A ROUTINE SENDING		      //
	//============================================//
	else if(halfMinuteIncrementer == 2)     //1 min interval
	{
		if (firstTyme == 1)
		{
			ep_send(ep1a);
			firstTyme = 0;
			return;
		}
		else if (firstTyme == 0)
		{
			ep_send(ep1a);

			if(firstTyme == 0)
			{
				return;
			}
		}
	}

	//============================================//





	//============================================//
	// 			 EP1B ROUTINE SENDING			  //
	//============================================//
	if(halfMinuteIncrementer == 20)   //10 min interval
	{
		if (firstTyme == 1)
		{
			ep_send(ep1b);
			firstTyme = 0;
			return;
		}
		else if (firstTyme == 0)
		{
			ep_send(ep1a);

			if(firstTyme == 0)
			{
				return;
			}

			halfMinuteIncrementer = 0;
		}
	}

	//============================================//




	//============================================//
	// 				EP5 ROUTINE SENDING			  //
	//============================================//

	day = DS1307_GetDate();

	if( (settings[0].totalizer_day != day) || (sendEp5 != 0) )
	{
		if ( (firstTyme == 1) && (ep5b == 0) )
		{
			ep5_save.firstTotalizer[0].totalizer = totaliser_vol1c;
			ep5_save.firstTotalizer[1].totalizer = totaliser_vol2c;
			ep5_save.firstTotalizer[0].timestamp = RtcToInt(2019);

//			time_present = timer_ep;

//			if(aflag1 == 0)
//			{
//				time_past = time_present;
//
				ep_send(ep5);
				firstTyme = 0;
				return;

//				aflag1 = 1;
		}
		else if ( (firstTyme == 0) && (ep5b == 0) )
		{
			ep_send(ep5);

			if(firstTyme == 0)
			{
				return;
			}

			ep5b = 1;
		}

//		time_interval1 = time_present - time_past;

		if ( (firstTyme == 1) && (ep5b == 1) )
		{
			ep_send(ep5);
			firstTyme = 0;
			return;
		}
		else if ( (firstTyme == 0) && (ep5b == 1) )
		{
			ep_send(ep5);

			if(firstTyme == 0)
			{
				return;
			}

			ep5b = 0;
		}

		settings[0].totalizer_day = day;
		EEPROM_Write_NUM (totalizerDay_loc, 0, settings[0].totalizer_day);
	}

	//============================================//
}


void synchedLog_get(pump_sid ab)
{
	int lastSynchedFlashA_pg,
		lastSynchedFlashB_pg;

	if (ab == side_a)
	{
		 EEPROM_Read(lastSynchedFlashA_loc, 0, &lastSynchedFlashA_pg, sizeof(lastSynchedFlashA_pg));
		 W25qxx_ReadPage(&synchedLog_a_new, (lastSynchedFlashA_pg), 0, sizeof(synchedLog_a_new) );
	}
	else if (ab == side_b)
	{
		 EEPROM_Read(lastSynchedFlashB_loc, 0, &lastSynchedFlashB_pg, sizeof(lastSynchedFlashB_pg));
		 W25qxx_ReadPage(&synchedLog_b_new, (lastSynchedFlashB_pg), 0, sizeof(synchedLog_b_new) );
	}
}

uint32_t RtcToInt_synchedTranx(uint32_t deviceYear, pump_sid ab)
{
	uint32_t yearShift = 27;
	uint32_t mnmask = 0x1f;

	 if(ab == side_a)
	 {
		 day = synchedLog_a_new.date._dd;
		 month = synchedLog_a_new.date._mm;
		 year = synchedLog_a_new.date._yy;
		 hour = synchedLog_a_new.time._hh;
		 minute = synchedLog_a_new.time._mn;
		 second = DS1307_GetSecond();
	 }
	 else if(ab == side_b)
	 {
		 day = synchedLog_a_new.date._dd;
		 month = synchedLog_b_new.date._mm;
		 year = synchedLog_b_new.date._yy;
		 hour = synchedLog_b_new.time._hh;
		 minute = synchedLog_b_new.time._mn;
		 second = DS1307_GetSecond();
	 }

	if (deviceYear <= 2016) deviceYear = 2000;

	uint32_t tmInt = (second & 0x3f) | ((minute & 0x3f) << TM_BITS_SEC)
					| ((hour & 0x1f) << TM_BITS_MIN)
					| ((day & 0x1f) << TM_BITS_HOUR)
					| ((month & mnmask) << 22)
					| (((year - (deviceYear-2000)) & 0x1f) << yearShift);	//five bit

	return tmInt;
}

void serverResponse_parse(ep_ ep)
{
	char strA[12], strB[12];

	switch(ep)
	{
//		case ep0:  return;
//
//		case ep1a: return;
//
//		case ep1b: return;
//
		case ep2:
					   snprintf(strA, sizeof(strA), "%ld", tranxA_token);
					   snprintf(strB, sizeof(strB), "%ld", tranxB_token);

					   if(strstr(token_str, strA))
					   {
						   config_mode = 0;
						   save_synchedTransaction_sides(side_a);
					   }
					   else if(strstr(token_str, strB))
					   {
						   config_mode = 0;
						   save_synchedTransaction_sides(side_b);
					   }
					//		   memset(config_buf, 0, sizeof(config_buf));
					   break;

//		case ep5: return;
	}
}

int8_t list_push(long token, ep_ ep)
{
	if (ep == ep2)
	{
		for(int8_t i = 0; i < 15; i++)
		{
			if (list[i].ep == ep2)
			{
				list[i].token = token;
				list[i].ep = ep;
				list[i].ptrMessgResp_callBack = serverResponse_parse;
				return;
			}
		}
		for(int8_t i = 0; i < 15; i++)
		{
			if(list[i].token == 0)
			{
				list[i].token = token;
				list[i].ep = ep;
				list[i].ptrMessgResp_callBack = serverResponse_parse;
				return;
			}
		}
		return 1;
	}
	else
	{
		for(int8_t i = 0; i < 15; i++)
		{
			if(list[i].token == 0)
			{
				list[i].token = token;
				list[i].ep = ep;
				list[i].ptrMessgResp_callBack = serverResponse_parse;
				return;
			}
		}
	}
	return 1;
}


void server_rx_parse(void)
 {
		int head_pos = 0,
			pos = 0,
			size;

	  	int8_t st = 1;
	  	char rx;

        size = strlen(config_buf);

//        config_found = 2;
       statuss[1] = 0;

	   while( (st != 0) && (head_pos < size) )
	   {
		   if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
			   rx = config_buf[head_pos];

	//	   {"ni":"p1","pn":"pms","dt":"bluesky886",kt":"bluesky12"}

		   if(rx == '{')     // header left square bracket 0x5B, 0d91   STX->0xA5
		   {
			  head_pos++;
			  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
				  rx = config_buf[head_pos];

			  if(rx == '"')
			  {
				  head_pos++;
				  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
					  rx = config_buf[head_pos];

				  if(rx == 's')
				  {
					  head_pos++;
					  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
						  rx = config_buf[head_pos];
					  if(rx == 't')
					  {
						  head_pos += 3;
						  do
						  {
							  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
								  statuss[pos++] = config_buf[head_pos++];

//							  head_pos++;

							  if( (head_pos < 0) || (head_pos >= pump_rx_bufsize) )
								  break;

						  }
						  while(config_buf[head_pos] != ',');
						  statuss[pos] = 0;

						  st = 0;      //nozzle ID obtained
					  }
				  }
			  }

			}
		   head_pos++;
	   }

	   if(st == 0)    //Successful Response --> status=0
	   {

		   for(int8_t i = 0; i < 15; i++)
		   	{
			    snprintf(token_str, sizeof(token_str), "%ld", list[i].token);
		   		if(strstr(config_buf, token_str))
		   		{
		   			if(list[i].ep == 0)
		   			{
		   				list[i].token = 0;
		   			}
		   			else
		   			{
		   				(*list[i].ptrMessgResp_callBack)(list[i].ep);
		   				list[i].token = 0;
		   			}
		   		}
		   	}
	   }
}


uint16_t generate_tk(void)
{
	uint16_t tk_int_;
	uint32_t tk_int;
	uint64_t tk_;

	HAL_RNG_GenerateRandomNumber(&hrng,  &tk_int);
	//tk_int_ = (uint32_t)tk_;
	tk_int_ = (uint16_t)tk_int;
	//tk_int &=0xf;
	//tk_int_/=1000;
	if(tk_int_ < 0) tk_int_*=-1;
//	sprintf(tk, "%d",tk_int_);

	return tk_int_;
}

//token = generate_tk();


void generateTransc_ID(char* ti)
{
	uint32_t ti_int;

//	extern RTC_TimeTypeDef sTime;
//	extern RTC_DateTypeDef sDate;

	HAL_RNG_GenerateRandomNumber(&hrng, &ti_int);
//	vTaskDelay(pdMS_TO_TICKS(45));
	ti_int/=100;
	sprintf(ti, "%d%d%d%d",year,month,ti_int,hour);
}

void save_totalTransaction_sides(pump_sid ab)
{
	int8_t sz;

	int totalTransaction;

	if(ab == side_a)
	{
		sz = sizeof(ep1b_save.total_tranxA);
		ep1b_save.total_tranxA = (ep1b_save.total_tranxA + 1);
		EEPROM_Write(totalTranxA_loc, totalTranxA1_loc, &ep1b_save.total_tranxA, sz);
	}
	else if(ab == side_b)
	{
		sz = sizeof(++ep1b_save.total_tranxB);
		ep1b_save.total_tranxB = (ep1b_save.total_tranxB + 1);
		EEPROM_Write(totalTranxB_loc, totalTranxB1_loc, &ep1b_save.total_tranxB, sz);
	}
}

void retrieve_totalTransaction_sides(pump_sid ab)
{
	int8_t sz;
	int totalTransaction;
	sz = sizeof(totalTransaction);

	if(ab == side_a)
	{
		EEPROM_Read(totalTranxA_loc, totalTranxA1_loc, &totalTransaction, sz);
		ep1b_save.total_tranxA = totalTransaction;
	}
	else if(ab == side_b)
	{
		EEPROM_Read(totalTranxB_loc, totalTranxB1_loc, &totalTransaction, sz);
		ep1b_save.total_tranxB = totalTransaction;
	}
}

void save_synchedTransaction_sides(pump_sid ab)
{
	int8_t sz;

	if(ab == side_a)
	{
		sz = sizeof(ep1b_save.synched_tranxA);
		ep1b_save.synched_tranxA = (ep1b_save.synched_tranxA + 1);
		EEPROM_Write(synchedTranxA_loc, synchedTranxA1_loc, &ep1b_save.synched_tranxA, sz);

		if(synchedLog_a_new.autoTranxFlag == 1)
		{
			ep1b_save.synched_autoTranxA = (ep1b_save.synched_autoTranxA + 1);
			EEPROM_Write(synchedAutoTranxA_loc, synchedAutoTranxA1_loc, &ep1b_save.synched_autoTranxA, sz);
		}
	}
	else if(ab == side_b)
	{
		sz = sizeof(ep1b_save.synched_tranxB);
		ep1b_save.synched_tranxB = (ep1b_save.synched_tranxB + 1);
		EEPROM_Write(synchedTranxB_loc, synchedTranxB1_loc, &ep1b_save.synched_tranxB, sz);

		if(synchedLog_b_new.autoTranxFlag == 1)
		{
			ep1b_save.synched_autoTranxB = (ep1b_save.synched_autoTranxB + 1);
			EEPROM_Write(synchedAutoTranxB_loc, synchedAutoTranxB1_loc, &ep1b_save.synched_autoTranxB, sz);
		}
	}
}

void retrieve_synchedTransaction_sides(pump_sid ab)
{
	int8_t sz;
	int synchedTransaction;
	sz = sizeof(synchedTransaction);

	if(ab == side_a)
	{
		EEPROM_Read(synchedTranxA_loc, synchedTranxA1_loc, &synchedTransaction, sz);
		ep1b_save.synched_tranxA = synchedTransaction;
	}
	else if(ab == side_b)
	{
		EEPROM_Read(synchedTranxB_loc, synchedTranxB1_loc, &synchedTransaction, sz);
		ep1b_save.synched_tranxB = synchedTransaction;
	}
}

void save_totalAutoTransaction_sides(pump_sid ab)
{
	int8_t sz;

	if(ab == side_a)
	{
		sz = sizeof(ep1b_save.total_autoTranxA);
		ep1b_save.total_autoTranxA = (ep1b_save.total_autoTranxA + 1);
		EEPROM_Write(totalAutoTranxA_loc, totalAutoTranxA1_loc, &ep1b_save.total_autoTranxA, sz);
	}
	else if(ab == side_b)
	{
		sz = sizeof(++ep1b_save.total_autoTranxB);
		ep1b_save.total_autoTranxB = (ep1b_save.total_autoTranxB + 1);
		EEPROM_Write(totalAutoTranxB_loc, totalAutoTranxB1_loc, &ep1b_save.total_autoTranxB, sz);
	}
}

void retrieve_totalAutoTransaction_sides(pump_sid ab)
{
	int8_t sz;
	int autoTransaction;
	sz = sizeof(autoTransaction);

	if(ab == side_a)
	{
		EEPROM_Read(totalAutoTranxA_loc, totalAutoTranxA1_loc, &autoTransaction, sz);
		ep1b_save.total_autoTranxA = autoTransaction;
	}
	else if(ab == side_b)
	{
		EEPROM_Read(totalAutoTranxB_loc, totalAutoTranxB1_loc, &autoTransaction, sz);
		ep1b_save.total_autoTranxB = autoTransaction;
	}
}

void save_synchedAutoTransaction_sides(pump_sid ab)
{
	int8_t sz;

	if(ab == side_a)
	{
		sz = sizeof(ep1b_save.synched_autoTranxA);
		ep1b_save.synched_autoTranxA = (ep1b_save.synched_autoTranxA + 1);
		EEPROM_Write(synchedAutoTranxA_loc, synchedAutoTranxA1_loc, &ep1b_save.synched_autoTranxA, sz);
	}
	else if(ab == side_b)
	{
		sz = sizeof(++ep1b_save.synched_autoTranxB);
		ep1b_save.synched_autoTranxA = (ep1b_save.synched_autoTranxB + 1);
		EEPROM_Write(synchedAutoTranxB_loc, synchedAutoTranxB1_loc, &ep1b_save.synched_autoTranxB, sz);
	}
}

void retrieve_synchedAutoTransaction_sides(pump_sid ab)
{
	int8_t sz;
	int synchedAutoTranx;
	sz = sizeof(synchedAutoTranx);

	if(ab == side_a)
	{
		EEPROM_Read(synchedAutoTranxA_loc, synchedAutoTranxA1_loc, &synchedAutoTranx, sz);
		ep1b_save.synched_autoTranxA = synchedAutoTranx;
	}
	else if(ab == side_b)
	{
		EEPROM_Read(synchedAutoTranxB_loc, synchedAutoTranxB1_loc, &synchedAutoTranx, sz);
		ep1b_save.synched_autoTranxB = synchedAutoTranx;
	}
}

void server_write(char* write_string)
{
	//sending data

	int8_t id = 0;

//	static int8_t sending = 0;
	static int16_t lnn = 0,
			       indx = 0;

	//=======================================================================

	if(sending == 0)
	{
		 // reroute this message to the SPI port...
		  memset(sndd,0,sizeof(sndd));
		  sprintf(sndd, "((((((|1|0|%d%s))", id, write_string);
		  lnn = strlen(sndd)-1;

		  indx = 0;

		  HAL_SPI_Transmit(&hspi2, sndd[indx], 1, 100);

		  indx++;
		  sending = 1;
	}

	//=======================================================================
	else if(sending == 1)
	{
		  int8_t i = 0;
		  while( (indx <= lnn ) && (i < 10) )
		  {
			  HAL_SPI_Transmit(&hspi2, sndd[indx], 1, 100);
			  indx++;
			  i++;
			  HAL_Delay(40);

			  if(indx == lnn)
			  {
				  sending = 0;
				  return;
			  }
		  }


//		  while( (indx <= lnn ) && (timer_spi > 40) )
//		  {
//			  HAL_SPI_Transmit(&hspi2, sndd[indx], 1, 100);
//			  indx++;
//			  timer_spi = 0;
//		  }
//		  if(indx == lnn)
//			  sending = 0;
	}


}



void spi_rx(void)
{
	static int16_t header_found = 0,
				   head_pos = 0;
	static int8_t curly_brace = 0,
				  firstTime = 1;
	char rx;

	if(firstTime == 1)
	{
		HAL_GPIO_WritePin(request_line_GPIO_Port, request_line_Pin, GPIO_PIN_SET);
		firstTime = 2;
	}
	if(firstTime == 2)
	{
		while( (HAL_GPIO_ReadPin(data_available_GPIO_Port, data_available_Pin) ) == 1 )
		{
			return;
		}
		firstTime = 3;
	}

	else if(firstTime == 3)
	{
		 while( (head_pos < 500) && (timer_spi > 40) )
		 {
			 //loop while read size...
		   HAL_SPI_TransmitReceive(&hspi2, 255, &rx, 1, 100);

		   if(rx == '{')     // header left square bracket 0x5B, 0d91   STX->0xA5
		   {
			   memset(config_buf, 0, sizeof(config_buf));

			  header_found = 1;
			  head_pos = 0;

			  config_buf[head_pos] = rx;

			  head_pos++;
		   }

		   //-------------------------------------------------------
		   else if (header_found == 1)
		   {
			   //header found look for the footer...
			   if(rx == '}')
			   {
					 if(curly_brace != 0 )
					 {
						   config_buf[head_pos] = rx;

						   curly_brace--;

						   head_pos++;
					 }
					 else
					 {
						 config_buf[head_pos] = rx;

						 HAL_GPIO_WritePin(request_line_GPIO_Port, request_line_Pin, GPIO_PIN_RESET);

						 header_found = 0;
						 head_pos = 0;
						 curly_brace = 0;
						 firstTime = 1;
	//					 config_found = 1;
//						 return;
					 }
			   }
			   else if(rx == '{')  //ie. if another header character found
			   {
				   config_buf[head_pos] = rx;

				   curly_brace++;

				   head_pos++;
			   }
			   else
			   {
				   if( (rx == '\0') && (head_pos > 10) )
				   {
					     HAL_GPIO_WritePin(request_line_GPIO_Port, request_line_Pin, GPIO_PIN_RESET);

						 header_found = 0;
						 head_pos = 0;
						 curly_brace = 0;
						 firstTime = 1;
	//					 config_found = 1;
//						 return;
				   }

				   config_buf[head_pos] = rx;

				   head_pos++;
			   }
		   }
		   timer_spi = 0;
		 }
	}
}


