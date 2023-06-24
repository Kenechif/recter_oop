///*
// * config.c
// *
// *  Created on: Oct 14, 2022
// *      Author: tunjo
// */
#include "config.h"
#include "time.h"

config_struct config_data;
//
////transaction tranx_save[2],
////			transaction_temp[2];


int8_t _15SecIncrementer,
	  _30SecIncrementer,
	  _60SecIncrementer,
	  _45seconds,
	  _75seconds,
	  _615seconds,
	  ep2_justSent = 0,
	  ep1a_justSent = 0;

transaction tranx_save, tranx_serverResponse;

extern RNG_HandleTypeDef hrng;

extern uint8_t hour,
			   minute,
			   second,
			   day,
			   month,
			   year,
			   dayofweek;

//extern pump_names pumpName[2];

extern int8_t changeLitrePrice = 0;

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

int8_t ep0_sent = 0,
	   ep5a_sent = 0,
	   ep5b_sent = 0;

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
	switch(designation)
	{
		case ep0: 	ep0_save.timestamp = RtcToInt(2019);
					ep0_save.token = generate_tk();


					ep0_save.pump[0].tolalizer_first = ep5_save.firstTotalizer[0].totalizer;
					ep0_save.pump[0].totalizerFirst_timestamp = ep5_save.firstTotalizer[0].timestamp;
					ep0_save.pump[1].tolalizer_first = ep5_save.firstTotalizer[1].totalizer;
					ep0_save.pump[1].totalizerFirst_timestamp = ep0_save.pump[0].totalizerFirst_timestamp;

//						ep5_save.firstTotalizer[0].totalizer = ep0_save.pump[0].tolalizer_first;
//						ep5_save.firstTotalizer[1].totalizer = ep0_save.pump[1].tolalizer_first;
//						ep5_save.firstTotalizer[0].timestamp = ep0_save.pump[0].totalizerFirst_timestamp;

					ep0_save.pump[0].status = pump1_status_4G;
					ep0_save.pump[0].totalizer = totaliser_vol1c;
					strcpy(ep0_save.pump[0].nozzle_name, pumpName[0].pump_name);
					ep0_save.pump[1].status = pump2_status_4G;
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

					break;

		case ep1a: 	ep1a_save.timestamp = RtcToInt(2019);
					ep1a_save.token = generate_tk();

					ep1a_save.pump[0].status = pump1_status_4G;
					ep1a_save.pump[0].totalizer = totaliser_vol1c;
					ep1a_save.pump[0].tolalizer_first = ep5_save.firstTotalizer[0].totalizer;
					ep1a_save.pump[0].totalizerFirst_timestamp = ep5_save.firstTotalizer[0].timestamp;
					ep1a_save.pump[0].nozzle_id = settings[0].noz_id;
					ep1a_save.pump[0].totalizer_real = totaliser_vol1;
					ep1a_save.pump[0].tolalizerReal_first = ep5_save.firstTotalizer[0].totalizer_real;

					strcpy(ep1a_save.pump[0].nozzle_name, pumpName[0].pump_name);
					ep1a_save.pump[1].status = pump2_status_4G;
					ep1a_save.pump[1].totalizer = totaliser_vol2c;
					ep1a_save.pump[1].tolalizer_first = ep5_save.firstTotalizer[1].totalizer;
					ep1a_save.pump[1].totalizerFirst_timestamp = ep5_save.firstTotalizer[0].timestamp;
					strcpy(ep1a_save.pump[1].nozzle_name, pumpName[1].pump_name);
					ep1a_save.pump[1].nozzle_id = (settings[0].noz_id + 1);
					ep1a_save.pump[1].totalizer_real = totaliser_vol2;
					ep1a_save.pump[1].tolalizerReal_first = ep5_save.firstTotalizer[1].totalizer_real;

					ep1a_save.firmware_version = 2023;
					ep1a_save.storage_loc = 'i';

					sprintf(ep_message,
							"{\"ep\":1,\"di\":\"%s\",\"tk\":%ld,\"pumps\":[{\"st\":%d,\"tz\":%0.3f,\"ft\":%0.3f,\"fttm\":%ld,\"nm\":\"%s\",\"nz\":%d,\"toz\":%0.3f,\"fot\":%0.3f},{\"st\":%d,\"tz\":%0.3f,\"ft\":%0.3f,\"fttm\":%ld,\"nm\":\"%s\",\"nz\":%d,\"toz\":%0.3f,\"fot\":%0.3f}],\"tm\":%ld,\"fv\":%d,\"sl\":\'%c\'}",
							device_id, ep1a_save.token, ep1a_save.pump[0].status,
							ep1a_save.pump[0].totalizer, ep1a_save.pump[0].tolalizer_first,
							ep1a_save.pump[0].totalizerFirst_timestamp, ep1a_save.pump[0].nozzle_name,
							ep1a_save.pump[0].nozzle_id, ep1a_save.pump[0].totalizer_real, ep1a_save.pump[0].tolalizerReal_first,
							ep1a_save.pump[1].status, ep1a_save.pump[1].totalizer, ep1a_save.pump[1].tolalizer_first,
							ep1a_save.pump[1].totalizerFirst_timestamp, ep1a_save.pump[1].nozzle_name, ep1a_save.pump[1].nozzle_id,
							ep1a_save.pump[1].totalizer_real, ep1a_save.pump[1].tolalizerReal_first, ep1a_save.timestamp,
							ep1a_save.firmware_version, ep1a_save.storage_loc);

					list_push(ep1a_save.token, ep1a);

//					{"ep":1,"di":"864120050705038","tk":1060422946,"tm":161772383,"pumps":[{"st":2,"tz":638449.375,"ft":638449.375,"fttm":161771979,"nm":"P7","nz":0,"toz":638449.375,"fot":638449.375},{"st":2,"tz":303126.875,"ft":303126.875,"fttm":161771978,"nm":"P8","nz":0,"toz":303126.875,"fot":303126.875}],"fv":20461}

					server_write(ep_message);

					break;

		case ep1b: 	ep1b_save.timestamp = RtcToInt(2019);
					ep1b_save.token = generate_tk();

					ep1b_save.pump[0].status = pump1_status_4G;
					ep1b_save.pump[0].totalizer = totaliser_vol1c;
					ep1b_save.pump[0].tolalizer_first = ep5_save.firstTotalizer[0].totalizer;
					ep1b_save.pump[0].totalizerFirst_timestamp = ep5_save.firstTotalizer[0].timestamp;
					strcpy(ep1b_save.pump[0].nozzle_name, pumpName[0].pump_name);
					ep1b_save.pump[1].status = pump2_status_4G;
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

					break;

	case ep2:   	tranx_save.transaction_type = 'a';
					tranx_save.storage_loc = 'i';
					strcpy(tranx_save.tag, "4jrt0v");

					sprintf(ep_message,
							"{\"ep\":2,\"di\":\"%s\",\"tk\":%ld,\"tm\":%ld,\"ti\":\"%s\",\"ta\":%0.3f,\"tv\":%0.3f,\"pl\":%0.3f,\"tz\":%0.3f,\"pm\":\"%s\",\"pa\":%d,\"pr\":\"%s\",\"tt\":\'%c\',\"tp\":%d,\"sl\":\'%c\',\"tg\":\"%s\"}",
							tranx_save.device_id, tranx_save.token, tranx_save.timestamp,
							tranx_save.transaction_id, tranx_save.transaction_price,
							tranx_save.transaction_vol, tranx_save.litre_price,
							tranx_save.totalizer, tranx_save.nozzle_name,
							tranx_save.nozzle_address, tranx_save.nozzle_product,
							tranx_save.transaction_type, tranx_save.transaction_period,
							tranx_save.storage_loc, tranx_save.tag);

					list_push(tranx_save.token, ep2);
					server_write(ep_message);
					break;

		case ep5_side_a : 	ep5_save.timestamp = RtcToInt(2019);
							ep5_save.token = generate_tk();

							strcpy(ep5_save.sentEntry_count, "0|0");
							strcpy(ep5_save.firstTotalizer[0].nozzle_name, pumpName[0].pump_name);

							sprintf(ep_message,
								"{\"ep\":5,\"di\":\"%s\",\"tk\":%ld,\"cn\":\"%s\",\"FT\":[{\"pn\":\"%s\",\"tm\":%ld,\"tz\":%0.3f,\"toz\":%0.3f}],\"tm\":%ld}",
								device_id, ep5_save.token, ep5_save.sentEntry_count,
								ep5_save.firstTotalizer[0].nozzle_name, ep5_save.firstTotalizer[0].timestamp,
								ep5_save.firstTotalizer[0].totalizer, ep5_save.firstTotalizer[0].totalizer_real, ep5_save.timestamp);

//							{"ep":5,"di":"864120050705038","tk":1357409195,"tm":161772324,"cn":"0|0","FT":[{"pn":"P8","tm":161637818,"tz":299990.969,"toz":299990.969}]}
							list_push(ep5_save.token, ep5_side_a);
							server_write(ep_message);
							break;

		case ep5_side_b :   ep5_save.timestamp = RtcToInt(2019);
							ep5_save.token = generate_tk();

							strcpy(ep5_save.sentEntry_count, "0|0");
							strcpy(ep5_save.firstTotalizer[1].nozzle_name, pumpName[1].pump_name);
	//						ep5_save.firstTotalizer.totalizer[1] = totaliser_vol2c;

							sprintf(ep_message,
								"{\"ep\":5,\"di\":\"%s\",\"tk\":%ld,\"cn\":\"%s\",\"FT\":[{\"pn\":\"%s\",\"tm\":%ld,\"tz\":%0.3f}],\"tm\":%ld}",
								device_id, ep5_save.token, ep5_save.sentEntry_count,
								ep5_save.firstTotalizer[1].nozzle_name, ep5_save.firstTotalizer[1].timestamp,
								ep5_save.firstTotalizer[1].totalizer, ep5_save.firstTotalizer[1].totalizer_real, ep5_save.timestamp);

							list_push(ep5_save.token, ep5_side_b);
							server_write(ep_message);
						    break;

//		case ep31 :			 {"ep":31,"di":"864120050705038","tk":1060422946,"tm":161772383,"pumps":[{"nm":"P7","nz":0,"ct":"20|0.5|0.0"},{"nm":"P8","nz":0,}]}
//							"{\"ep\":31,\"di\":\"%s\",\"tk\":%ld,\"tm\":%ld,\"pumps\":[{\"nm\":\"%s\",\"nz\":%d,\"ct\":\"%0.1f|%0.1f|%0.1f'}",

	}
}


void epSend_interval(void)
{
	static int8_t aflag = 0,
//				  halfMinuteIncrementer = 0,
				  firstTime = 1,
				  firstTime_ep2 = 1,
				  sending_busy = 0;
//				  _15SecIncrementer,
//				  _30SecIncrementer,
//				  _60SecIncrementer,
//				  _75seconds,
//				  _615seconds,
//				  ep2_justSent = 0,
//				  ep1a_justSent = 0;

	uint8_t array_count = 0;

	static uint16_t epochTime_past = 0;

	uint16_t epochTime_present;

	uint16_t time_interval;

	int flash_locc,
		pg;


//	epochTime_present = RtcToInt(2019);
	epochTime_present = timer_ep;

	if(aflag == 0)
	{
		epochTime_past = epochTime_present;

		aflag = 1;
	}

	time_interval = epochTime_present - epochTime_past;




	//============================================//
	// 			EP0 & EP5 ROUTINES SENDING	      //
	//============================================//

	if(time_interval >= 15000)    //15 sec interval
	{
//		send_ep0_ep5();


//		settings[0].totalizer_day = 22;
//		ep0_sent = 1;

		//============================================//
		// 				EP5 ROUTINE SENDING			  //
		//============================================//

		day = DS1307_GetDate();
//		day = 22;
		if(settings[0].totalizer_day == day)
		{
			ep5a_sent = 1;
			ep5b_sent = 1;
		}

		if(settings[0].totalizer_day != day)
		{
			ep5_save.firstTotalizer[0].totalizer = totaliser_vol1c;
			ep5_save.firstTotalizer[1].totalizer = totaliser_vol2c;
			ep5_save.firstTotalizer[0].timestamp = RtcToInt(2019);
			ep5_save.firstTotalizer[1].timestamp = ep5_save.firstTotalizer[0].timestamp;

			if(ep5a_sent == 0)
			{
				ep_send(ep5_side_a);
			}
			else if(ep5b_sent == 0)
			{
				ep_send(ep5_side_b);
			}
		}

		//============================================//




		//============================================//
		// 				EP0 ROUTINE SENDING			  //
		//============================================//
		else if (ep0_sent == 0)
		{
			ep_send(ep0);
		}

		//============================================//


		if ( (ep0_sent == 0) || (ep5a_sent == 0) || (ep5b_sent == 0) )
		{
			sending_busy = 1;
		}
		else
		{
			sending_busy = 0;
		}

		aflag = 0;
		_15SecIncrementer++;

		if(ep2_justSent == 1)
		{
			_75seconds = 1;
		}
		else if(ep1a_justSent == 1)
		{
			_615seconds = 1;
		}
	}

	//============================================//






	//============================================//
	// 				EP2 ROUTINE SENDING			  //
	//============================================//
	if(_15SecIncrementer == 2) //&& (sending_busy == 0) )  //30 sec interval
	{
		_30SecIncrementer++;
		_15SecIncrementer = 0;

		if(sending_busy == 0)
		{
			if(firstTime_ep2 == 1)
			{

				if(ep1b_save.synched_tranxA != ep1b_save.total_tranxA) //&& (ep2a_justSent == 0) )
				{
					ep2_send(side_a);
				}
				else if(ep1b_save.synched_tranxB != ep1b_save.total_tranxB)
				{
					ep2_send(side_b);
				}

				firstTime_ep2 = 0;
			}

			else if(firstTime_ep2 == 0)
			{
				if(ep1b_save.synched_tranxB != ep1b_save.total_tranxB) //&& (ep2a_justSent == 0) )
				{
					ep2_send(side_b);
				}
				else if(ep1b_save.synched_tranxA != ep1b_save.total_tranxA)
				{
					ep2_send(side_a);
				}

				firstTime_ep2 = 1;
			}


			if(_30SecIncrementer == 2)
			{
				ep2_justSent = 1;
			}

		}
	}
	//============================================//





	//============================================//
	// 			  EP1A ROUTINE SENDING		      //
	//============================================//
	if( (_30SecIncrementer == 2) || (_75seconds == 1) ) //&& (sending_busy == 0) )     //1 min interval
	{
		if(_30SecIncrementer == 2)
		{
			_60SecIncrementer++;
			_30SecIncrementer = 0;

			if( (sending_busy == 0) && (ep2_justSent == 0) )
			{
				ep_send(ep1a);

				if(_60SecIncrementer == 10)
				{
					ep1a_justSent = 1;
				}
			}
		}
		else if(_75seconds == 1)
		{
			_75seconds = 0;

			if( (sending_busy == 0) && (ep2_justSent == 1) )
			{
				ep_send(ep1a);

				ep2_justSent = 0;

				if(_60SecIncrementer == 10)
				{
					ep1a_justSent = 1;
				}
			}
		}





//		//============================================//
//		// 			EP0 & EP5 ROUTINES SENDING	      //
//		//============================================//
//
//		if(firstTyme == 0)
//		{
//			send_ep0_ep5();
//		}

		//============================================//
	}


	//============================================//





	//============================================//
	// 			 EP1B ROUTINE SENDING			  //
	//============================================//
	if( (_60SecIncrementer == 10) || (_615seconds == 1) ) //&& (sending_busy == 0) )     //10 min interval
	{
			if(_60SecIncrementer == 10)
			{
				_60SecIncrementer = 0;

				if( (sending_busy == 0) && (ep1a_justSent == 0) )
				{
					ep_send(ep1b);

				}
			}
			else if(_615seconds == 1)
			{
				_615seconds = 0;

				if( (sending_busy == 0) && (ep1a_justSent == 1) )
				{
					ep_send(ep1b);

					ep1a_justSent = 0;
				}
			}
	}

	//============================================//
}


void synchedLog_get(pump_sid ab)
{
	uint16_t lastSynchedFlashA_pg,
		     lastSynchedFlashB_pg;

	if (ab == side_a)
	{
		 EEPROM_Read(lastSynchedFlashA_loc, 0, &lastSynchedFlashA_pg, sizeof(lastSynchedFlashA_pg));
//		 EEPROM_Read(synchedTranxA_loc, 0, &lastSynchedFlashA_pg, sizeof(lastSynchedFlashA_pg));
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

	int8_t header_found = 0,
		   indexx = 0;

	int head_pos = 0;

	char rx;

	switch(ep)
	{
//		case ep0:  return;
//
		case ep1a: 	   ep1_mtResponse();
					   break;
//
		case ep1b:     ep1_mtResponse();
					   break;

		case ep2:	   snprintf(strA, sizeof(strA), "%ld", tranxA_token);
					   snprintf(strB, sizeof(strB), "%ld", tranxB_token);

					   if(strcmp(token_str, strA) == 0)
					   {
						   config_mode = 0;
						   save_synchedTransaction_sides(side_a);
					   }
					   else if(strcmp(token_str, strB) == 0)
					   {
						   config_mode = 0;
						   save_synchedTransaction_sides(side_b);
					   }
					//		   memset(rx_buf, 0, sizeof(rx_buf));
					   break;

//		case ep5: return;
	}
}

int8_t list_push(long token, ep_ ep)
{
	static int8_t list_full = 0;

	if(list_full == 1)     //Clears list, whenever full
	{
		for(int8_t i = 0; i < 15; i++)
		{
			list[i].token = 0;
		}
	}

	if (ep == ep2)
	{
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
		// In the event of an already existing ep2 in the list
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
		for(int8_t i = 0; i < 15; i++)
		{
			if (list[i].ep == ep2)   //Replaces any existing ep2, cos it's definitely a resend
			{
				list[i].token = token;
				list[i].ep = ep;
				list[i].ptrMessgResp_callBack = serverResponse_parse;
				return;
			}
		}

		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
		// In the event of no existing ep2 in the list
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
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

		list_full = 1;
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
	list_full = 1;
	return 1;
}


void server_rx_parse(void)
 {
		int head_pos = 0,
			pos = 0,
			size;

	  	int8_t st = 1, val, sz;
	  	sz = sizeof(val);

	  	char rx;

        size = strlen(rx_buf);

       statuss[1] = 0;

	   while( (st != 0) && (head_pos < size) )
	   {
		   if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
			   rx = rx_buf[head_pos];

		   if(rx == '{')     // header left square bracket 0x5B, 0d91   STX->0xA5
		   {
			  head_pos++;
			  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
				  rx = rx_buf[head_pos];

			  if(rx == '"')
			  {
				  head_pos++;
				  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
					  rx = rx_buf[head_pos];

				  if(rx == 's')
				  {
					  head_pos++;
					  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
						  rx = rx_buf[head_pos];
					  if(rx == 't')
					  {
						  head_pos += 3;
						  do
						  {
							  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
								  statuss[pos++] = rx_buf[head_pos++];

							  if( (head_pos < 0) || (head_pos >= pump_rx_bufsize) )
								  break;

						  }
						  while(rx_buf[head_pos] != ',');
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
		   		if(strstr(rx_buf, token_str))
		   		{
		   			if(list[i].ep == 0)
		   			{
		   				list[i].token = 0;
		   			}
		   			else
		   			{
		   				(*list[i].ptrMessgResp_callBack)(list[i].ep);
		   				list[i].token = 0;
		   				if(list[i].ep == ep0)
		   				{
		   					ep0_sent = 1;
		   				}
		   				else if(list[i].ep == ep5_side_a)
		   				{
		   					ep5a_sent = 1;

		   					if(ep5b_sent == 1)
		   					{
		   						settings[0].totalizer_day = day;
		   						EEPROM_Write_NUM (totalizerDay_loc, 0, settings[0].totalizer_day);
		   					}
		   				}
		   				else if(list[i].ep == ep5_side_b)
						{
							ep5b_sent = 1;

							if(ep5a_sent == 1)
							{
								settings[0].totalizer_day = day;
								EEPROM_Write_NUM (totalizerDay_loc, 0, settings[0].totalizer_day);
							}
						}
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

	uint16_t totalTransaction;

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
	uint16_t totalTransaction;
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

	uint16_t flash_beginA_page = 0,            //0x0000
		flash_beginB_page = 16384;        //0x4000

	if(ab == side_a)
	{
		sz = sizeof(ep1b_save.synched_tranxA);
		ep1b_save.synched_tranxA = (ep1b_save.synched_tranxA + 1);

		ep1b_save.synched_flashA = (ep1b_save.synched_flashA + 1);

		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
		//flash_endA => 0x3FFFFF --> 4,194,303 pg16,383.996
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
		if (ep1b_save.synched_flashA > 16383)
		{
			ep1b_save.synched_flashA = (ep1b_save.synched_tranxA - 16384);
		}
		else if (ep1b_save.synched_flashA > 32767)
		{
			ep1b_save.synched_flashA = (ep1b_save.synched_tranxA - 32767);
		}
		else if (ep1b_save.synched_flashA > 49151)
		{
			ep1b_save.synched_flashA = (ep1b_save.synched_tranxA - 49151);
		}
		else if (ep1b_save.synched_flashA > 65535)
		{
			ep1b_save.synched_flashA = flash_beginA_page;
		}

		EEPROM_Write(synchedTranxA_loc, synchedTranxA1_loc, &ep1b_save.synched_tranxA, sz);
		EEPROM_Write((lastSynchedFlashA_loc), 0, &ep1b_save.synched_flashA, sizeof(ep1b_save.synched_flashA));

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

		ep1b_save.synched_flashB = (ep1b_save.synched_flashB + 1);


		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
		//flash_beginB => 0x400000 --> 4,194,304 pg16,384
		//flash_endB => 0x7FFFFF --> 8,388,607 pg32767.996
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
		if (ep1b_save.synched_flashB > 32767)
		{
			ep1b_save.synched_flashB = (ep1b_save.synched_tranxB - 32768);
		}
		else if (ep1b_save.synched_flashB > 49151)
		{
			ep1b_save.synched_flashB = (ep1b_save.synched_tranxB - 49152);
		}
		else if (ep1b_save.synched_flashB > 65535)
		{
			ep1b_save.synched_flashB = flash_beginB_page;
		}


		EEPROM_Write(synchedTranxB_loc, synchedTranxB1_loc, &ep1b_save.synched_tranxB, sz);
		EEPROM_Write((lastSynchedFlashB_loc), 0, &ep1b_save.synched_flashB, sizeof(ep1b_save.synched_flashB));

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
	uint16_t synchedTransaction;
	sz = sizeof(synchedTransaction);

	if(ab == side_a)
	{
		EEPROM_Read(synchedTranxA_loc, synchedTranxA1_loc, &synchedTransaction, sz);
		ep1b_save.synched_tranxA = synchedTransaction;
		EEPROM_Read(lastSynchedFlashA_loc, 0, &ep1b_save.synched_flashA, sizeof(ep1b_save.synched_flashA) );
	}
	else if(ab == side_b)
	{
		EEPROM_Read(synchedTranxB_loc, synchedTranxB1_loc, &synchedTransaction, sz);
		ep1b_save.synched_tranxB = synchedTransaction;
		EEPROM_Read(lastSynchedFlashB_loc, 0, &ep1b_save.synched_flashB, sizeof(ep1b_save.synched_flashB) );
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
	uint16_t autoTransaction;
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
	uint16_t synchedAutoTranx;
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
	int8_t id = 0;

	//=======================================================================

		  memset(sndd, 0, sizeof(sndd));
		  sprintf(sndd, "((((((|1|0|%d%s))\r\n", id, write_string);

		  HAL_UART_Transmit (&huart2, sndd, strlen(sndd), 1000);

	//=======================================================================
}


void server_read(void)
 {
	  	static int16_t head_pos = 0;

	  	static int8_t curly_brace = 0,
	  				  header_found = 0;
	  	char rx;

	   int16_t rxcnt = pump_rx_bufsize - huart2.RxXferCount;
	   rx = (char)(uart2_rx_buf[rxcnt-1]);

	   if( (rx == '{') && (header_found == 0) )    // header left square bracket 0x5B, 0d91   STX->0xA5
	   {
		  header_found = 1;
		  head_pos = 0;

		  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
			  rx_buf[head_pos] = rx;

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
			   		   if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
							rx_buf[head_pos] = rx;

					   curly_brace--;

					   head_pos++;
			   	 }
			   	 else
			   	 {
			   		 if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )

			   		 rx_buf[head_pos] = rx;

					 //  memcpy( pump_buf , pump_rx_buf[head_pos] ,rxcnt ); //transfer to the definitive buffer.
					 huart2.RxXferCount = pump_rx_bufsize;
					 huart2.pRxBuffPtr = &uart2_rx_buf[0]; //reset //uart2_rx_buf; //
					 header_found = 0;
					 head_pos = 0;
					 curly_brace = 0;
					 server_message_found = 1;
//					 return;
			   	 }
		   }
		   else if(rx == '{')  //ie. if another header character found
		   {
			   if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
					rx_buf[head_pos] = rx;

			   curly_brace++;

			   head_pos++;
		   }
		   else
		   {
			   if( (rx == '\0') && (head_pos > 10) )
			   {
					 huart2.RxXferCount = pump_rx_bufsize;
					 huart2.pRxBuffPtr = &uart2_rx_buf[0]; //reset //uart2_rx_buf; //
					 header_found = 0;
					 head_pos = 0;
					 curly_brace = 0;
			   }

			   if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
				   rx_buf[head_pos] = rx;

			   head_pos++;
		   }
	   }
//	   else if (rx != '\0')
//	   {
//		   if( (rx == '\0') && (head_pos > 10) )
//		   {
//				 huart2.RxXferCount = pump_rx_bufsize;
//				 huart2.pRxBuffPtr = &uart2_rx_buf[0]; //reset //uart2_rx_buf; //
//				 header_found = 0;
//				 head_pos = 0;
//				 curly_brace = 0;
//				 config_found = 1;
//		   }
//
//		  rx_buf[head_pos] = rx;
//		  head_pos++;
//	   }
}



void online_setUnitPrice(void)
{
	settings[0].price_ = atof(mt_pump[0].price);
	settings[1].price_ = atof(mt_pump[1].price);
	save_settings();   //save to eeprom
	load_settings(side_a); //load the settings into the internal variables
	load_settings(side_b);
}



void ep1_mtResponse(void)
{
	int8_t header_found = 0,
		   indexx = 0;

	int head_pos = 0;

	char rx;

	while( (indexx < 249) && (head_pos < pump_rx_bufsize) )
	{
	   if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) && (indexx < 249) )
		   rx = rx_buf[head_pos];

//     [{"ty":3,"pn":"P9","pr":185.0,"sh":null,"fg":0,"tg":"p|P9"},{"ty":3,"pn":"P10","pr":850.0,"sh":null,"fg":0,"tg":"p|P10"}]

	   if(rx == '[')     // header left square bracket 0x5B, 0d91   STX->0xA5
	   {
		  indexx = 0;
		  mt[indexx++] = rx;
		  header_found = 1;
	   }
	   else if (header_found == 1)
	   {
		   mt[indexx++] = rx;

		   //header found look for the footer...
		   if(rx == ']')
		   {
			   mt[indexx] = '\0';
			   if(indexx > 6)
			   {
				   int8_t str_len;

				   mt_pump[0].type[0] = mt[7];
				   mt_pump[0].pumpName[0] = mt[(7 + 8)];

				   head_pos = 16;
				   indexx = 1;
				   while(mt[head_pos] != '"')
				   {
					   if(indexx < 5)
						   mt_pump[0].pumpName[indexx++] = mt[head_pos++];
				   }

				   head_pos = head_pos + 7;
				   indexx = 0;
				   do
				   {
					  mt_pump[0].price[indexx++] = mt[head_pos++];
				   }
				   while(mt[head_pos] != ',');

				   while(mt[head_pos++] != '{');
				   mt_pump[1].type[0] = (mt[head_pos + 5]);
				   mt_pump[1].pumpName[0] = mt[head_pos + (5+8)];

				   head_pos = head_pos + (13 + 1);
				   indexx = 1;
				   while(mt[head_pos] != '"')
				   {
					   if(indexx < 5)
						   mt_pump[1].pumpName[indexx++] = mt[head_pos++];
				   }

				   head_pos = head_pos + 7;
				   indexx = 0;
				   do
				   {
					  mt_pump[1].price[indexx++] = mt[head_pos++];
				   }
				   while(mt[head_pos] != ',');

				   if(mt_pump[0].type[0] == '3')
				   {
					   changeLitrePrice = 1;
				   }
				   return;
			   }
		   }
	   }
	   head_pos++;
	}
}

uint8_t ct_get(void)
{
	int head_pos = 0;

	char rx;

	while(head_pos < pump_rx_bufsize)
	{
	   if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
		   rx = rx_buf[head_pos];

	   if(rx == 'c')
	   {
		  head_pos++;
		  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
			  rx = rx_buf[head_pos];

		  if(rx == 't')
		  {
			  head_pos++;
			  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
				  rx = rx_buf[head_pos];

			  if(rx == ':')
			  {
				  head_pos++;
				  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
					  rx = rx_buf[head_pos];
				  calibrate_ct.ct = (int) rx;

				  if(rx == '0')
					  return 0;
				  else if(rx == '1')
					  return 1;
			   }
		  }
	   }

	   head_pos++;
	 }
		return 0;
}

void ep2_send(pump_sid side)
{
	if(side == side_a)
	{
		synchedLog_get(side);

		tranxA_token = generate_tk();
		tranx_save.token = 	tranxA_token;

		tranx_save.timestamp = synchedLog_a_new.timeStamp;
		strcpy(tranx_save.device_id, synchedLog_a_new.device_id);
		strcpy(tranx_save.transaction_id, synchedLog_a_new.transaction_id);
		tranx_save.transaction_price = synchedLog_a_new.pr__;
		tranx_save.transaction_vol = synchedLog_a_new.vol__;
		tranx_save.litre_price = synchedLog_a_new.litre_price_;
		tranx_save.totalizer = synchedLog_a_new.totaliserVol_cal;
		strcpy(tranx_save.nozzle_name, synchedLog_a_new.nozzle_name);
		tranx_save.nozzle_address = 1;
		strcpy(tranx_save.nozzle_product, synchedLog_a_new.nozzle_product);
		tranx_save.transaction_period = synchedLog_a_new.transaction_period;
	}
	else if(side == side_b)
	{
		synchedLog_get(side);

		tranxB_token = generate_tk();

		tranx_save.token = tranxB_token;

		tranx_save.timestamp = synchedLog_b_new.timeStamp;
		strcpy(tranx_save.device_id, synchedLog_b_new.device_id);
		strcpy(tranx_save.transaction_id, synchedLog_b_new.transaction_id);
		tranx_save.transaction_price = synchedLog_b_new.pr__;
		tranx_save.transaction_vol = synchedLog_b_new.vol__;
		tranx_save.litre_price = synchedLog_b_new.litre_price_;
		tranx_save.totalizer = synchedLog_b_new.totaliserVol_cal;
		strcpy(tranx_save.nozzle_name, synchedLog_b_new.nozzle_name);
		tranx_save.nozzle_address = 2;
		strcpy(tranx_save.nozzle_product, synchedLog_b_new.nozzle_product);
		tranx_save.transaction_period = synchedLog_b_new.transaction_period;

	}
	ep_send(ep2);
}


void send_ep0_ep5(void)
{
	static int8_t aflag1 = 0,
				  ep5b = 0,
				  sending_next = 0;

	static uint16_t time_past = 0;

	uint16_t time_present;

	uint16_t time_interval1;


//	sending_next++;

	//============================================//
	// 				EP5 ROUTINE SENDING			  //
	//============================================//

	day = DS1307_GetDate();

	if(settings[0].totalizer_day != day)
	{
		ep5_save.firstTotalizer[0].totalizer = totaliser_vol1c;
		ep5_save.firstTotalizer[1].totalizer = totaliser_vol2c;
		ep5_save.firstTotalizer[0].totalizer_real = totaliser_vol1;
		ep5_save.firstTotalizer[1].totalizer_real = totaliser_vol2;
		ep5_save.firstTotalizer[0].timestamp = RtcToInt(2019);
		ep5_save.firstTotalizer[1].timestamp = ep5_save.firstTotalizer[0].timestamp;

		if(ep5a_sent == 0)
		{
			ep_send(ep5_side_a);
		}
		else if(ep5b_sent == 0)
		{
			ep_send(ep5_side_b);
		}
	}

	//============================================//





	//============================================//
	// 				EP0 ROUTINE SENDING			  //
	//============================================//
	else if (ep0_sent == 0)
	{
		ep_send(ep0);
	}

	//============================================//

}



//void server_write(char* write_string)
//{
//	//sending data
//
//	int8_t id = 0;
//
////	static int8_t sending = 0;
//	static int16_t lnn = 0,
//			       indx = 0;
//
//	//=======================================================================
//
//	if(sending == 0)
//	{
//		 // reroute this message to the SPI port...
//		  memset(sndd,0,sizeof(sndd));
//		  sprintf(sndd, "((((((|1|0|%d%s))", id, write_string);
//		  lnn = strlen(sndd)-1;
//
//		  indx = 0;
//
//		  HAL_SPI_Transmit(&hspi2, sndd[indx], 1, 100);
//
//		  indx++;
//		  sending = 1;
//	}
//
//	//=======================================================================
//	else if(sending == 1)
//	{
//		  int8_t i = 0;
//		  while( (indx <= lnn ) && (i < 10) )
//		  {
//			  HAL_SPI_Transmit(&hspi2, sndd[indx], 1, 100);
//			  indx++;
//			  i++;
//			  HAL_Delay(40);
//
//			  if(indx == lnn)
//			  {
//				  sending = 0;
//				  return;
//			  }
//		  }
//
//
////		  while( (indx <= lnn ) && (timer_spi > 40) )
////		  {
////			  HAL_SPI_Transmit(&hspi2, sndd[indx], 1, 100);
////			  indx++;
////			  timer_spi = 0;
////		  }
////		  if(indx == lnn)
////			  sending = 0;
//	}
//
//
//}
//
//
//
//void spi_rx(void)
//{
//	static int16_t header_found = 0,
//				   head_pos = 0;
//	static int8_t curly_brace = 0,
//				  firstTime = 1;
//	char rx;
//
//	if(firstTime == 1)
//	{
//		HAL_GPIO_WritePin(request_line_GPIO_Port, request_line_Pin, GPIO_PIN_SET);
//		firstTime = 2;
//	}
//	if(firstTime == 2)
//	{
//		while( (HAL_GPIO_ReadPin(data_available_GPIO_Port, data_available_Pin) ) == 1 )
//		{
//			return;
//		}
//		firstTime = 3;
//	}
//
//	else if(firstTime == 3)
//	{
//		 while( (head_pos < 500) && (timer_spi > 40) )
//		 {
//			 //loop while read size...
//		   HAL_SPI_TransmitReceive(&hspi2, 255, &rx, 1, 100);
//
//		   if(rx == '{')     // header left square bracket 0x5B, 0d91   STX->0xA5
//		   {
//			   memset(rx_buf, 0, sizeof(rx_buf));
//
//			  header_found = 1;
//			  head_pos = 0;
//
//			  rx_buf[head_pos] = rx;
//
//			  head_pos++;
//		   }
//
//		   //-------------------------------------------------------
//		   else if (header_found == 1)
//		   {
//			   //header found look for the footer...
//			   if(rx == '}')
//			   {
//					 if(curly_brace != 0 )
//					 {
//						   rx_buf[head_pos] = rx;
//
//						   curly_brace--;
//
//						   head_pos++;
//					 }
//					 else
//					 {
//						 rx_buf[head_pos] = rx;
//
//						 HAL_GPIO_WritePin(request_line_GPIO_Port, request_line_Pin, GPIO_PIN_RESET);
//
//						 header_found = 0;
//						 head_pos = 0;
//						 curly_brace = 0;
//						 firstTime = 1;
//	//					 config_found = 1;
////						 return;
//					 }
//			   }
//			   else if(rx == '{')  //ie. if another header character found
//			   {
//				   rx_buf[head_pos] = rx;
//
//				   curly_brace++;
//
//				   head_pos++;
//			   }
//			   else
//			   {
//				   if( (rx == '\0') && (head_pos > 10) )
//				   {
//					     HAL_GPIO_WritePin(request_line_GPIO_Port, request_line_Pin, GPIO_PIN_RESET);
//
//						 header_found = 0;
//						 head_pos = 0;
//						 curly_brace = 0;
//						 firstTime = 1;
//	//					 config_found = 1;
////						 return;
//				   }
//
//				   rx_buf[head_pos] = rx;
//
//				   head_pos++;
//			   }
//		   }
//		   timer_spi = 0;
//		 }
//	}
//}
//
//
