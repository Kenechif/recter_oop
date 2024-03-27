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
	ep31,
	ep1a_priceChangeResponse_sideA,
	ep1a_priceChangeResponse_sideB,
	ep1a_priceChangeResponse_bothSides,
	ep20_side_a,
	ep20_side_b
}ep_;


extern _card attendant1, attendant2, customer;

extern ep20_ ep20_save;

typedef struct{
	unsigned long timestamp;
	unsigned long token;
	char transaction_id[20];
	char nozzle_name[5];
	char nozzle_product[6];
	uint8_t nozzle_address;
	float litre_price;
	float transaction_vol;
	float transaction_price;
	float totalizer;
	uint32_t transaction_period;
	char transaction_type;
	char device_id[16];
	char tag[12];
	char storage_loc;
	char session_id[9];
}transaction;

//typedef struct
//{
//	char card_pin[5];
//	char card_id[9];
//}_attendant1;
//
//_attendant1 attendant1;

//typedef struct
//{
//  uint8_t user_id[5];
//  char user_pin[5];
//  uint8_t type[4];
//  char user_name[16];
//  char bal[16];
//  char typ;
//  uint8_t login_state;
//}_card;
//
//_card attendant1, attendant2, customer;

typedef struct
{
	float totalizer;
	float totalizer_real;
	float tolalizer_first;
	float tolalizerReal_first;
	unsigned long totalizerFirst_timestamp;
	char nozzle_name[5];
	uint8_t nozzle_id;
	uint8_t status;
}pumps_ep;

typedef struct{
	unsigned long timestamp;
	unsigned long token;
	char device_id[16];
	pumps_ep pump[2];
	unsigned long boot_time;
	uint8_t reboot_cause;
}ep0_;

ep0_ ep0_save;


char mt[250];

uint8_t  UART_BUFF1[128],
		 UART_BUFF2[128];

char card1_rx_buf[pump_rx_bufsize],
	 card2_rx_buf[pump_rx_bufsize];

extern const char device_id [],
				  firmware_date [],
				  firmware_time [],
				  chip_type [];

extern char session_id[9];

extern const uint16_t firmware_version;


extern float price,
			 amt,
			 price2,
			 amt2,
			 litre_price,
			 litre_price2;

extern pump_names pumpName[2];

//extern calibrate calibrate_ct;

extern float totaliser_vol1c,
			 totaliser_vol2c,
			 totaliser_vol1,
			 totaliser_vol2;

extern float startShiftTotaliser_vol1,
		  	 startShiftTotaliser_vol1c,
		  	 startShiftTotaliser_vol2,
			 startShiftTotaliser_vol2c;

extern float startShiftTotaliser_amt1,
			 startShiftTotaliser_amt1c,
			 startShiftTotaliser_amt2,
			 startShiftTotaliser_amt2c;

extern log_new log_a_new,
			   log_b_new,
			   synchedLog_a_new,
			   synchedLog_b_new;

long tranxA_token,
	 tranxB_token;

extern uint8_t config_mode;

typedef struct{
	uint8_t ct;
	unsigned long timestamp;
	unsigned long token;
	char device_id[16];
	pumps_ep pump[2];
	unsigned int firmware_version;
	char storage_loc;
}ep1a_;

ep1a_ ep1a_save;

typedef struct{
	unsigned long timestamp;
	unsigned long token;
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
	unsigned long boot_time;
	uint16_t firmware_version;
	char storage_loc;
}ep1b_;

ep1b_ ep1b_save;

extern ep5_ ep5_save;


//typedef struct{
//	long timestamp;
//	long token;
//	char device_id[16];
//	pumps_ep pump[2];
//	int firmware_version;
//	char storage_loc;
//}ep31_;

//{"ep":31,"di":"864120050705038","tk":1060422946,"tm":161772383,"pumps":[{"nm":"P7","nz":0,"ct":"20|0.5|0.0"},{"nm":"P8","nz":0,"ct":"20|0.5|0.0"}]}

//typedef struct
//{
//	char nozzle_name[5];
//	uint8_t nozzle_id;
//	calibrate calibrate_ct;
//}pumps_ep31;

//typedef struct
//{
//	int8_t ct;
//	float ctt_original;
//	float ctt_baseMinusOriginal;
//	float ctt_effectiveMinusBase;
//}calibrate;

//calibrate calibrate_ct;

extern ep31_ ep31_save;

typedef void (*ptrCallBack)(ep_ ep);

typedef struct
{
	unsigned long token;
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

char serverTimeStr[16],
	 sessionIdStr[9];
char *remaining;
unsigned long serverTime;

char token_str[12],
     statuss[10];

uint8_t data_length1,
		data_length2;

uint16_t check_sum1,
		 check_sum2;

char card1_buf[100],
	 card2_buf[100];

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
	char atag[20];			 // attendant1 1 tag is supplied here.
	char atag2[20];			 // attendant1 2 tag is supplied here.
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

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart5;

extern SPI_HandleTypeDef hspi2;

extern char uart1_rx_buf[pump_rx_bufsize];
extern char uart2_rx_buf[pump_rx_bufsize];
extern char uart3_rx_buf[pump_rx_bufsize];
extern char uart5_rx_buf[pump_rx_bufsize];

extern pump_settings settings[2];

extern uint8_t day,
			   serverTimeFlag;

extern float litre_price,
			 price_real1,
			 amt_real1;

extern const uint16_t totalizerDay_loc;

extern flash_store_info flash_infoA, flash_infoB;

extern uint8_t config_found,
			   server_message_found,
			   card1_message_found,
			   card2_message_found;
//			  connected;

extern uint8_t connected;

extern uint16_t //timer_ep,
//				timer_ep1,
				timer_spi;
//				ep2_timer;

extern uint32_t timer_ep1,
				timer_ep;


uint8_t ep1a_priceChangeFlag1,
		ep1a_priceChangeFlag2,
		ep1a_priceChangeFlag_bothSides;

uint8_t TSTA1,
		WSTA1,
		LSTA1,
		card_writeOp1,
		TSTA2,
		WSTA2,
		LSTA2,
		card_writeOp2;
//		verifyResponse;

void read_config();

void ep_send(ep_ designation);

void epSend_interval(void);

void ep2_send(pump_sid side);

void ep20_send(pump_sid side);

void send_ep0_ep5(void);

void serverResponse_parse(ep_ ep);

uint8_t list_push(unsigned long token, ep_ ep);

void list_pop(void);

void server_write(char* write_string);

void server_read(void);

void spi_rx(void);

uint16_t generate_tk(void);

uint16_t generate_otpVariable1(void);
uint16_t generate_otpVariable2(void);

void generateTransc_ID(char* ti);

uint8_t tokenTrack_push(long token);

uint8_t tokenTrack_array(void);

void tokenArray_parse(void);

void array_bubbleSort(void);

void missingTokenArray_get(void);

uint8_t ep2Token_push(long token);

uint8_t ep2TokenTrack_array(void);

void ep2MissingTokenArray_get(void);

void ep2TokenArray_parse(void);

void ep2Array_bubbleSort(void);

void save_totalTransaction(void);

void retrieve_totalTransaction(void);

void clear_totalTransaction_sides(pump_sid ab);

void save_synchedTransaction(void);

void retrieve_synchedTransaction(void);

void clear_synchedTransaction_sides(pump_sid ab);

void save_totalAutoTranx(void);

void retrieve_totalAutoTranx(void);

void clear_totalAutoTransaction_sides(pump_sid ab);

//void save_synchedAutoTranx(void);
//
//void retrieve_synchedAutoTranx(void);

void save_synchedAutoTransaction_sides(pump_sid ab);

void retrieve_synchedAutoTransaction_sides(pump_sid ab);

void clear_synchedAutoTransaction_sides(pump_sid ab);

void save_totalEvents(void);

void retrieve_totalEvents(void);

void save_synchedEvents(void);

void retrieve_synchedEvents(void);

void online_setUnitPrice1(void);
void online_setUnitPrice2(void);

void ep1_mtResponse(void);

uint8_t ct_parse(pump_sid side);

uint8_t ep1_ctCheck(void);

uint16_t checksum1(uint8_t* data, uint8_t len);
uint16_t checksum2(uint8_t* data, uint8_t len);

void sendReply1(void);
void sendReply2(void);

void card1_read(void);
void card2_read(void);

void card1_rx_parse(void);
void card2_rx_parse(void);

void sessionId_parse(pump_sid side);



void synchedLog_get(pump_sid ab);
uint32_t RtcToInt_synchedTranx(uint32_t deviceYear, pump_sid ab);

void ttostr(uint32_t time_integer, uint8_t typ); // typ: 1=> time 2=>date

void serverTime_parse(void);


#ifdef __cplusplus
}
#endif

#endif /* CONFIG_CONFIG_H_ */
