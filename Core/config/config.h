///*
// * config.h
// *
// *  Created on: Oct 14, 2022
// *      Author: tunjo
// */
//
#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

#include "stdbool.h"
#include "string.h"
#include "stddef.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
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
	ep0 = 1,
	ep1a,
	ep1b,
	ep2,
	ep5_side_a, //= 6,
	ep5_side_b,
	ep31
}ep_;

typedef struct{
	long timestamp;
	long token;
	char transaction_id[20];
	char nozzle_name[5];
	char nozzle_product[6];
	int8_t nozzle_address;
	float litre_price;
	float transaction_vol;
	float transaction_price;
	float totalizer;
	uint32_t transaction_period;
	char transaction_type;
	char device_id[16];
	char tag[12];
	char storage_loc;
}transaction;

typedef struct
{
	int8_t status;
	float totalizer;
	float totalizer_real;
	float tolalizer_first;
	float tolalizerReal_first;
	long totalizerFirst_timestamp;
	char nozzle_name[5];
	uint8_t nozzle_id;
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


char mt[250];

extern char device_id [];

extern float price,
			 amt,
			 price2,
			 amt2,
			 litre_price,
			 litre_price2;

extern pump_names pumpName[2];

extern calibrate calibrate_ct;

extern float totaliser_vol1c,
			 totaliser_vol2c,
			 totaliser_vol1,
			 totaliser_vol2;

extern log_new log_a_new,
			   log_b_new,
			   synchedLog_a_new,
			   synchedLog_b_new;

long tranxA_token,
	 tranxB_token;

extern int8_t config_mode;

typedef struct{
	long timestamp;
	long token;
	char device_id[16];
	pumps_ep pump[2];
	int firmware_version;
	char storage_loc;
}ep1a_;

ep1a_ ep1a_save;

typedef struct{
	long timestamp;
	long token;
	char device_id[16];
	pumps_ep pump[2];
	uint16_t total_tranx;
	uint16_t total_tranxA;
	uint16_t total_tranxB;
	uint16_t synched_tranx;
	uint16_t synched_tranxA;
	uint16_t synched_tranxB;
	uint16_t synched_flashA;
	uint16_t synched_flashB;
	uint16_t total_autoTranx;
	uint16_t synched_autoTranx;
	uint16_t total_autoTranxA;
	uint16_t synched_autoTranxA;
	uint16_t total_autoTranxB;
	uint16_t synched_autoTranxB;
	uint16_t total_events;
	uint16_t synched_events;
	char firmware_date[15];
	char firmware_time[12];
	float mem_usage;
	char chip_type[15];
	float mem_usage_auto;
	long boot_time;
	uint16_t firmware_version;
	char storage_loc;
}ep1b_;

ep1b_ ep1b_save;

typedef struct
{
	char nozzle_name[6];
	long timestamp;
	float totalizer;
	float totalizer_real;
}firstTotalizer_ep;

typedef struct{
	long timestamp;
	long token;
//	char device_id[16];
	firstTotalizer_ep firstTotalizer[2];
	char sentEntry_count[8];
}ep5_;

ep5_ ep5_save;


typedef void (*ptrCallBack)(ep_ ep);

typedef struct
{
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

extern char rx_buf[pump_rx_bufsize];

extern pump_status_enum_4G pump1_status_4G,
						   pump2_status_4G;

typedef struct{
	char name[5];	    //string
	char product[5];	//string
	int address;		//read as is
	int nozzle;
	float price;		//read ad is
	char mode[7];	//mode: auto or MANUAL
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


extern UART_HandleTypeDef huart2;

extern SPI_HandleTypeDef hspi2;

extern char uart2_rx_buf[pump_rx_bufsize];

extern pump_settings settings[2];

extern uint8_t day;

extern float litre_price,
			 price_real,
			 amt_real;

extern const int16_t totalizerDay_loc;

extern flash_store_info flash_infoA, flash_infoB;

extern int8_t config_found,
			  server_message_found,
			  connected;

extern uint16_t timer_ep,
				timer_spi;
//				ep2_timer;

void read_config();

void ep_send(ep_ designation);

void epSend_interval(void);

void ep2_send(pump_sid side);

void send_ep0_ep5(void);

void serverResponse_parse(ep_ ep);

int8_t list_push(long token, ep_ ep);

void list_pop(void);

void server_write(char* write_string);

void server_read(void);

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

void online_setUnitPrice(void);

void ep1_mtResponse(void);




void synchedLog_get(pump_sid ab);
uint32_t RtcToInt_synchedTranx(uint32_t deviceYear, pump_sid ab);


#ifdef __cplusplus
}
#endif

#endif /* CONFIG_CONFIG_H_ */
