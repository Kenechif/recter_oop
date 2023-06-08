///*
// * config.h
// *
// *  Created on: Oct 14, 2022
// *      Author: tunjo
// */
//
#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

#include "string.h"
#include "stdint.h"
#include "stdbool.h"
#include "stm32f4xx_hal.h"
#include "settings.h"
#include "states.h"
#include "states2.h"
#include "EEPROM.h"
#include "../flash_mem/w25qxxConf.h"
#include "../flash_mem/w25qxx.h"

#define TM_BITS_SEC		6
#define TM_BITS_MIN		(6 + TM_BITS_SEC)
#define TM_BITS_HOUR	(5 + TM_BITS_MIN)
#define TM_BITS_DAY		(5 + TM_BITS_HOUR)

//#define u8  uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define MAX_NOP 2

typedef enum
{
	ep0,
	ep1a,
	ep1b,
	ep2,
	ep5 = 6,
}ep_;

typedef struct{
	long timestamp;
	long token;
	char transaction_id[20];
	char nozzle_name[5];
	char nozzle_product[5];
	int8_t nozzle_address;
	float litre_price;
	float transaction_vol;
	float transaction_price;
	float totalizer;
	int transaction_period;
	char transaction_type;
	char device_id[16];
	char tag[12];
	char storage_loc;
}transaction;

typedef struct
{
	int8_t status;
	float totalizer;
	float tolalizer_first;
	long totalizerFirst_timestamp;
	char nozzle_name[5];
}pumps_ep;

typedef struct{
	long timestamp;
	long token;
	char device_id[16];
	pumps_ep pump[2];
	long boot_time;
	int8_t reboot_cause;
}ep0_;

ep0_ ep0_save;

extern float price,
			 amt,
			 price2,
			 amt2,
			 litre_price,
			 litre_price2;

extern pump_names pumpName[2];

extern float totaliser_vol1c,
			 totaliser_vol2c;

extern log_new log_a_new,
			   log_b_new,
			   synchedLog_a_new,
			   synchedLog_b_new;

long tranxA_token,
	 tranxB_token;

extern int8_t config_mode;
//ep0_save.device_id, ep0_save.token, ep0_save.pump[0].status, ep0_save.pump[0].totalizer,
//ep0_save.pump[0].tolalizer_first, ep0_save.pump[0].totalizerFirst_timestamp,
//ep0_save.pump[0].nozzle_name, ep0_save.pump[1].status, ep0_save.pump[1].totalizer,
//ep0_save.pump[1].totalizerFirst_timestamp, ep0_save.pump[1].nozzle_name,
//ep0_save.boot_time, ep0_save.timestamp, ep0_save.reboot_cause}

//ep0_save.timestamp = RtcToInt(2019);
//ep0_save.token = generate_tk();
//generateTransc_ID(tranx_save.transaction_id);

//ep0_save.pump[0].status
//ep0_save.pump[1].status
//ep0_save.pump[0].totalizer
//ep0_save.pump[1].totalizer
//ep0_save.pump[0].tolalizer_first
//ep0_save.pump[1].tolalizer_first
//ep0_save.pump[0].totalizerFirst_timestamp
//ep0_save.pump[1].totalizerFirst_timestamp
//ep0_save.pump[0].nozzle_name
//ep0_save.pump[1].nozzle_name

//"{\"ep\":0,\"di\":\"%s\",\"tk\":%ld,\"pumps\":
//[{\"st\":%d,\"tz\":%0.3f,\"ft\":%0.3f,\"fttm\":%ld,\"nm\":\"%s\"},
//{\"st\":%d,\"tz\":%0.3f,\"ft\":0.3f,\"fttm\":%ld,\"nm\":\"%s\"}],
//\"bt\":%ld,\"tm\":%ld,\"rs\":%d}"
//
//
//EP0
//{"ep":0,"di":"862273049192628","tk":752774887,"pumps":
//[{"st":2,"tz":502614.400,"ft":502614.400,"fttm":50874640,"nm":"P12"},
//{"st":2,"tz":404306.600,"ft":404306.600,"fttm":50874638,"nm":"P11"}],
//"bt":50874635,"tm":50874673,"rs":0}

//{
//  "ep":0,
//  "di":"862273049192628",
//  "tk":752774887,
//  "pumps":[
//    {
//      "st":2,
//      "tz":502614.400,
//      "ft":502614.400,
//      "fttm":50874640,
//      "nm":"P12"
//    },
//    {
//      "st":2,
//      "tz":404306.600,
//      "ft":404306.600,
//      "fttm":50874638,
//      "nm":"P11"
//    }
//  ],
//  "bt":50874635,
//  "tm":50874673,
//  "rs":0
//}

//long boot-time;
//long timestamp;
//int8_t reboot_cause;

//typedef struct
//{
//	int8_t status;
//	float totalizer;
//	float tolalizer_first;
//	long totalizerFirst_timestamp;
//	char nozzle_name;
//}pumps_ep;



//{
//    "ep":1,
//    "di":"862273049192628",
//    "tk":1064198019,
//    "pumps":[
//        {
//            "st":2,
//            "tz":502614.406,
//            "ft":502614.406,
//            "fttm":50874640,
//            "nm":"P12"
//        },
//        {
//            "st":2,
//            "tz":404306.594,
//            "ft":404306.594,
//            "fttm":50874638,
//            "nm":"P11"
//        }
//    ],
//    "tm":50874763,
//    "fv":20419,
//    "sl":'i'
//}

//typedef struct
//{
//	int8_t status;
//	float totalizer;
//	float tolalizer_first;
//	long totalizerFirst_timestamp;
//	char nozzle_name;
//}pumps_ep;

typedef struct{
	long timestamp;
	long token;
	char device_id[16];
	pumps_ep pump[2];
	int firmware_version;
	char storage_loc;
}ep1a_;

ep1a_ ep1a_save;

//"{\"ep\":1,\"di\":\"%s\",\"tk\":%ld,\"pumps\":
//[{\"st\":%d,\"tz\":%0.3f,\"ft\":%0.3f,\"fttm\":%ld,\"nm\":\"%s\"},
//{\"st\":%d,\"tz\":%0.3f,\"ft\":0.3f,\"fttm\":%ld,\"nm\":\"%s\"}],
//\"tm\":%ld,\"fv\":%d,\"sl\":\'%c\'}"

//ep1a_save.device_id, ep1a_save.token, ep1a_save.pump[0].status,
//ep1a_save.pump[0].totalizer, ep1a_save.pump[0].tolalizer_first,
//ep1a_save.pump[0].totalizerFirst_timestamp, ep1a_save.pump[0].nozzle_name,
//ep1a_save.pump[1].status, ep1a_save.pump[1].totalizer, ep1a_save.pump[1].tolalizer_first,
//ep1a_save.pump[1].nozzle_name, ep1a_save.firmware_version, ep1a_save.storage_loc

//ep1a_save.timestamp = RtcToInt(2019);
//ep1a_save.token = generate_tk();
//generateTransc_ID(tranx_save.transaction_id);

//ep1a_save.pump[0].status
//ep1a_save.pump[1].status
//ep1a_save.pump[0].totalizer
//ep1a_save.pump[1].totalizer
//ep1a_save.pump[0].tolalizer_first
//ep1a_save.pump[1].tolalizer_first
//ep1a_save.pump[0].totalizerFirst_timestamp
//ep1a_save.pump[1].totalizerFirst_timestamp
//ep1a_save.pump[0].nozzle_name
//ep1a_save.pump[1].nozzle_name



//{
//    "ep":1,
//    "di":"862273049192628",
//    "tk":175709118,
//    "pumps":[
//        {
//            "st":2,
//            "tz":502991.094,
//            "ft":502614.406,
//            "fttm":50874640,
//            "nm":"P12"
//        },
//        {
//            "st":2,
//            "tz":405111.625,
//            "ft":404306.594,
//            "fttm":50874638,
//            "nm":"P11"
//        }
//    ],
//    "tt":3684,
//    "st":3684,
//    "tta":0,
//    "sta":0,
//    "se":0,
//    "te":0,
//    "fwd":"Nov 28 2020",
//    "fwt":"17:07:30",
//    "mu":0.000,
//    "ch":"800C",
//    "mua":0.000,
//    "bt":50874635,
//    "tm":50894451,
//    "fv":20419,
//    "sl":'i'
//}


typedef struct{
	long timestamp;
	long token;
	char device_id[16];
	pumps_ep pump[2];
	int total_tranx;
	int total_tranxA;
	int total_tranxB;
	int synched_tranx;
	int synched_tranxA;
	int synched_tranxB;
	int total_autoTranx;
	int synched_autoTranx;
	int total_autoTranxA;
	int synched_autoTranxA;
	int total_autoTranxB;
	int synched_autoTranxB;
	int total_events;
	int synched_events;
	char firmware_date[15];
	char firmware_time[12];
	float mem_usage;
	char chip_type[15];
	float mem_usage_auto;
	long boot_time;
	int firmware_version;
	char storage_loc;
}ep1b_;

ep1b_ ep1b_save;

//int total_tranx;
//int synched_tranx;
//int total_autoTranx;
//int synched_autoTranx;
//int total_events;
//int synched_events;
//
//ep1b_save.total_tranx = 60;
//ep1b_save.synched_tranx = 55;
//ep1b_save.total_autoTranx = 33;
//ep1b_save.synched_autoTranx = 30;
//ep1b_save.total_events = 60;
//ep1b_save.synched_events = 55;



//"{\"ep\":1,\"di\":\"%s\",\"tk\":%ld,\"pumps\":
//[{\"st\":%d,\"tz\":%0.3f,\"ft\":%0.3f,\"fttm\":%ld,\"nm\":\"%s\"},
//{\"st\":%d,\"tz\":%0.3f,\"ft\":0.3f,\"fttm\":%ld,\"nm\":\"%s\"}],
//\"tt\":%d,\"st\":%d,\"tta\":%d,\"sta\":%d,\"se\":%d,\"te\":%d,\"fwd\":\"%s\",
//\"fwt\":\"%s\",\"mu\":%0.3f,\"ch\":\"%s\",\"mua\":0.3f,\"bt\":%ld,
//\"tm\":%ld,\"fv\":%d,\"sl\":\'%c\'}"
//
//ep1b_save.device_id, ep1b_save.token, ep1b_save.pump[0].status, ep1b_save.pump[0].totalizer,
//ep1b_save.pump[0].tolalizer_first, ep1b_save.pump[0].totalizerFirst_timestamp, ep1b_save.pump[0].nozzle_name,
//ep1b_save.pump[1].status, ep1b_save.pump[1].totalizer, ep1b_save.pump[1].tolalizer_first,
//ep1b_save.pump[1].totalizerFirst_timestamp, ep1b_save.pump[1].nozzle_name, ep1b_save.total_tranx,
//ep1b_save.synched_tranx, ep1b_save.total_autoTranx, ep1b_save.synched_events, ep1b_save.total_events,
//ep1b_save.firmware_date, ep1b_save.firmware_time, ep1b_save.mem_usage, ep1b_save.chip_type,
//ep1b_save.mem_usage_auto, ep1b_save.boot_time, ep1b_save.timestamp, ep1b_save.firmware_version,
//ep1b_save.storage_loc


//ep1b_save.timestamp = RtcToInt(2019);
//ep1b_save.token = generate_tk();
//generateTransc_ID(tranx_save.transaction_id);

//ep1b_save.pump[0].status
//ep1b_save.pump[1].status
//ep1b_save.pump[0].totalizer
//ep1b_save.pump[1].totalizer
//ep1b_save.pump[0].tolalizer_first
//ep1b_save.pump[1].tolalizer_first
//ep1b_save.pump[0].totalizerFirst_timestamp
//ep1b_save.pump[1].totalizerFirst_timestamp
//ep1b_save.pump[0].nozzle_name
//ep1b_save.pump[1].nozzle_name



typedef struct
{
	char nozzle_name[6];
	long timestamp;
	float totalizer;
}firstTotalizer_ep;

typedef struct{
	long timestamp;
	long token;
//	char device_id[16];
	firstTotalizer_ep firstTotalizer[2];
	char sentEntry_count[8];
}ep5_;

ep5_ ep5_save;

//"{\"ep\":5,\"di\":\"%s\",\"tk\":%ld,\"cn\":\"%s\",\"FT\":
//[{\"pn\":\"%s\",\"tm\":%ld,\"tz\":%0.3f}],\"tm\":%ld}"

//ep5_save.device_id, ep5_save.token, ep5_save.sentEntry_count,
//ep5_save.firstTotalizer.nozzle_name, ep5_save.firstTotalizer.timestamp,
//ep5_save.firstTotalizer.totalizer, ep5_save.sentEntry_count);

//ep5_save.timestamp = RtcToInt(2019);
//ep5_save.token = generate_tk();
//generateTransc_ID(tranx_save.transaction_id);

//ep5_save.firstTotalizer.nozzle_name
//ep5_save.firstTotalizer.timestamp





//{
//    "ep":5,
//    "di":"862273049192628",
//    "tk":1761869001,
//    "cn":"0|0",
//    "FT":[
//        {
//            "pn":"P11",
//            "tm":50743085,
//            "tz":400631.719
//        }
//    ],
//    "tm":50874675
//}


//transaction tranx_save, tranx_serverResponse;

typedef void (*ptrCallBack)(ep_ ep);

typedef struct
{
//	char token[12];
	long token;
	ep_ ep;
	ptrCallBack ptrMessgResp_callBack;
}LIST;

LIST list[15];

//LIST list[] = {
//	{},
//	{},
//	{},
//	{}
//}

char token_str[12],
     statuss[10];

extern char config_buf[pump_rx_bufsize];

//tranx_save.timestamp = RtcToInt(2019);
//tranx_save.token = generate_tk();
//generateTransc_ID(tranx_save.transaction_id);


//generateTransc_ID(transaction_id);

//{
//    "ep":2,
//    "di":"862273049192628",
//    "tk":123007901,
//    "tm":50894353,
//    "ti":"jKLOXZtn",
//    "ta":2007.500,
//    "tv":12.130,
//    "pl":165.500,
//    "tz":502991.094,
//    "pm":"P12",
//    "pa":1,
//    "pr":"PMS",
//    "tt":'a',
//    "tp":30325,
//    "sl":'i',
//    ”tg”:”12345678”
//}


typedef struct{
	char name[5];	    //string
	char product[5];	//string
	int address;		//read as is
	int nozzle;
	float price;		//read ad is
	char mode[7];	//mode: auto or manual
	int num;
	int tok_prod;
}config_atg;


typedef struct
{
	char u2t[15];
	int32_t ldm;  		/**< low data mode */
	int16_t sst;        /**< station shift time from config*/
	int8_t sst_duration;   //duration to hold the pump down....
	int16_t msg_type;
	uint32_t gib_mode;
	char gid[17];             // Interface id goes here.
	char atag[20];			 // Attendant 1 tag is supplied here.
	char atag2[20];			 // Attendant 2 tag is supplied here.
	int8_t inh;
}config_others;

typedef struct{
	uint8_t tz; 	    //totalizer decimal point
	uint8_t vol;	    //volume dp
	uint8_t amo;	    //amount dp
	uint8_t unp; 	    //unit price dp
	uint8_t pvl;     	//preset vol dp
	bool unp_avail; //unit price decimal point available
}config_dp;

typedef struct{
	char domain[20];	//stores domain name data from config
	u16 port1;		    //stores port info
	u16 port2;		    //stores port info
	char apn[20];		//access point name of network
}config_net;

typedef struct{
	char mode[20];
	char ssid[20];
	char password[20];
	char ip[20];
	char gwip[20];
}config_wifi;
//=========================================


typedef struct
{
	config_net    net;
	config_atg    pump[MAX_NOP];
	config_wifi   wifi;
	config_others others;
	config_dp     dp;
}config_struct;

//extern config_struct config_data;

//
#ifdef __cplusplus
extern "C" {
#endif


//const int totalTranx_loc,
//			totalTranx1_loc,
//			synchedTranx_loc,
//			synchedTranx1_loc,
//			totalAutoTranx_loc,
//			totalAutoTranx1_loc,
//			synchedAutoTranx_loc,
//			synchedAutoTranx1_loc,
//			totalEvents_loc,
//			totalEvents1_loc,
//			synchedEvents_loc,
//			synchedEvents1_loc;
//
//const long token1_loc,
//			token2_loc,
//			token3_loc,
//			token4_loc,
//			token5_loc,
//			token6_loc,
//			token7_loc,
//			token8_loc,
//			token9_loc,
//			token10_loc,
//			token11_loc,
//			token12_loc,
//			token13_loc,
//			token14_loc,
//			token15_loc;
//
//const int8_t array1_loc,
//			 array2_loc,
//			 array3_loc,
//			 array4_loc,
//			 array5_loc,
//			 array6_loc,
//			 array7_loc,
//			 array8_loc,
//			 array9_loc,
//			 array10_loc,
//			 array11_loc,
//			 array12_loc,
//			 array13_loc,
//			 array14_loc,
//			 array15_loc;


extern SPI_HandleTypeDef hspi2;

extern pump_settings settings[2];

extern uint8_t day;

extern float litre_price,
			 price_real,
			 amt_real;

extern const int8_t totalizerDay_loc;

extern flash_store_info flash_infoA, flash_infoB;


//long token_track[15];

extern uint16_t timer_ep,
				timer_spi;
//				ep2_timer;

void read_config();

void ep_send(ep_ designation);

void epSend_interval(void);

void serverResponse_parse(ep_ ep);

int8_t list_push(long token, ep_ ep);

void list_pop(void);

void server_write(char* write_string);

void spi_rx(void);

uint16_t generate_tk(void);

void generateTransc_ID(char* ti);

uint8_t tokenTrack_push(long token);

uint8_t tokenTrack_array(void);

void tokenArray_parse(void);

void array_bubbleSort(void);

void missingTokenArray_get(void);

uint8_t ep2Token_push(long token);

uint8_t ep2TokenTrack_array(void);

void ep2TokenArray_parse(void);

void ep2Array_bubbleSort(void);

void ep2MissingTokenArray_get(void);

void save_totalTransaction(void);

void retrieve_totalTransaction(void);

void save_synchedTransaction(void);

void retrieve_synchedTransaction(void);

void save_totalAutoTranx(void);

void retrieve_totalAutoTranx(void);

void save_synchedAutoTranx(void);

void retrieve_synchedAutoTranx(void);

void save_totalEvents(void);

void retrieve_totalEvents(void);

void save_synchedEvents(void);

void retrieve_synchedEvents(void);


void synchedLog_get(pump_sid ab);
uint32_t RtcToInt_synchedTranx(uint32_t deviceYear, pump_sid ab);


#ifdef __cplusplus
}
#endif

#endif /* CONFIG_CONFIG_H_ */
