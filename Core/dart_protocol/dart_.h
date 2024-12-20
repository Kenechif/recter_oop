/*
 * dart_.h
 *
 *  Created on: 28 Jun 2024
 *      Author: Fidel
 */

#ifndef DART_PROTOCOL_DART__H_
#define DART_PROTOCOL_DART__H_

#include "settings.h"
#include "pump_comm.h"

//#include <vector>
//#include "BaseProtocol.h"

#define MAX_LENGTH_OF_COMMAND 50 		//This is the message sent to the pump
#define DLE 0x10
#define ETX 0x03
#define SF 0xFA


#define u8  uint8_t
#define u16 uint16_t
#define u32 uint32_t


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart5;

extern SPI_HandleTypeDef hspi2;

extern char uart1_rx_buf[pump_rx_bufsize];
extern char uart2_rx_buf[pump_rx_bufsize];
extern char uart3_rx_buf[pump_rx_bufsize];
extern char uart5_rx_buf[pump_rx_bufsize];

extern pump_settings_stream1 settings_stream1[2],
					 	 	 settings_original_stream1[2];

extern pump_settings_stream2 settings_stream2[2],
							 settings_original_stream2[2];

extern pump_settings_stream3 settings_stream3[2],
				       	     settings_original_stream3[2],
							 copy_stream3[2];

extern bool go_message;

extern uint8_t RxBuf[RxBuf_SIZE],
			   MainBuf[MainBuf_SIZE];

extern uint8_t go_buff[RxBuf_SIZE];

extern int16_t head,
			   tail;

extern float litre_price,
			 price_real1,
			 amt_real1;

extern float litre_price2,
			 price_real2,
			 amt_real2;

extern const uint16_t totalizerDay_loc;

extern flash_store_info flash_infoA, flash_infoB;

extern uint8_t config_found,
			   server_message_found,
			   card1_message_found,
			   card2_message_found;

extern uint8_t connected;

extern uint16_t timer_spi;

extern uint32_t timer_ep1,
				timer_ep;

extern unsigned char pumpno,
					 pumpno2;

extern float vol_calibrated1,
			 vol_effective1,
			 vol_calibrated2,
			 vol_effective2;

extern uint16_t vol_real1,
	    	    vol_real2;

extern pump_settings_stream1 settings_stream1[2];

uint8_t  DART_BUFF1[128],
		 DART_BUFF2[128],
		 array_len,
		 array_len2;

uint8_t set_param1[51],
		set_param2[51];

uint8_t set_calib1[4],
		set_calib2[4];

uint32_t update_date_time;


uint8_t	 r_raw_data1[128],
		 r_raw_data2[128];

//uint8_t rx_buf1[pump_rx_bufsize] = {0};

extern ConfigChange configChange[2];

extern int milliseconds;

int millis2;

extern uint8_t dummyValue;

extern bool nozzle_out1,
			nozzle_out2;


// class dart: public BaseProtocol{
// 	private:
	/* options for COMMANDS to send */
		typedef enum
		{
				ACK,
				NACK,
				ACKPOLL,
				EOT,

				GETSTATUS,
				RESET1,
				AUTHORISE,
				STOP1,
				SWITCH_OFF,
				REQUEST_FILLING_INFO,
				RETURN_PUMP_PARAM,
				RETURN_PUMP_IDENTITY,
				SUSPEND_REQUEST,
				RESUME_REQUEST,

				NOZSTATUS_AND_FILLINGPRICE,

				PUMP_IDENTITY,

				PRESET_VOL,	//Set the respective volume of these before using them
				PRESET_AMO,

				FILLED_VOLUME_AND_AMOUNT,

				PRICE_UPDATE,
				SET_MAMO,
				REQUEST_VOL_TOTAL_COUNT,
				REQUEST_CONFIG_CHANGE_INFO,
				CLEAR_OTP_SESSION,
				DATE_TIME_UPDATE,
				REQUEST_CALIBRATION_PARAM,
				NO_COMMAND
			}command_enum1;

	/* flags for raw RESPONSE RECEIVED */
		typedef enum{
				NO_ETX = -2,
				CRC_ERROR = -1,
				JUNK,
				NOREPLY,

				r_POLL,
				r_EOT,
				r_ACK,
				r_NACK,
				r_ACKPOLL,
				DATA_PUMPNO,

				DATA_SUSPEND,
				DATA_RESUME,

				DATA_COMMAND,
				DATA_ALLOWED_NOZ,
				DATA_PRESET_VOL,
				DATA_PRESET_AMO,
				DATA_PRICE_UPDATE,
				DATA_SET_MAMO,
				DATA_REQUEST_VOL_TOTAL_COUNT,
				DATA_SET_PUMP_PARAM,
				DATA_CLEAR_OTP_SESSION,
				DATA_REQUEST_CONFIG_CHANGE_INFO,
				DATA_DATE_TIME_UPDATE,
				DATA_REQUEST_CALIBRATION_PARAM,
				DATA_SET_CALIBRATION_PARAM,
			}response_enum;

	/* enum for pump status */
		typedef enum
		{
			//----- Pump Status -----//
			PUMP_NOT_PROGRAMMED,
			RESET0,
			AUTHORIZED,
			FILLING,
			FILLING_COMPLETED,
			MAX_AMOUNTVOLUME_REACHED,
			SWITCHED_OFF,

		}pump_status_enum_;


	/*************************************************************
	 * Control characters in the WAYNE protocol for sending and receiving messages
	 **************************************************************/
		unsigned char addr,				//address of pump
					  addr2;
		unsigned short trans;			//transaction no. .. CD1, CD2 e.t.c
		unsigned char lng;				//length of data

		unsigned char ctrl,				//ctrl character
					  ctrl2;

		unsigned char TX, 			//the LSB (second 4 bit) of the ctrl byte
					  TX2;

		unsigned char crc1;
		unsigned char crc2;
//		vector<unsigned char> full_msg;	//stores the full_msg to send

			//set these before calling send (SET_MAMO) 99999
		int preset_vol;
		int preset_amo;
		int price_update;		//
		int mamo_update;
		unsigned char dpvol;	//0-8	no. of decimals you can have there
		unsigned char dpamo;	//0-8
		unsigned char dpunp;	//0-4

		int preset_vol2;
		int preset_amo2;		//
		int mamo_update2;
		unsigned char dpvol2;	//0-8	no. of decimals you can have there
		unsigned char dpamo2;	//0-8
		unsigned char dpunp2;	//0-4


	/*********************************************************************
	 *	Control characters in the WAYNE protocol for PUMP RESPONSE MESSAGE
	 **********************************************************/
		unsigned char r_addr; 			//response addr
		unsigned char r_ctrl;			//response ctrl character
		unsigned char r_pumpno;			//pump no i.e. which pump nozzle
		unsigned char r_trans;			//transaction no. CD1, CD2 e.t.c
		unsigned char r_TX;				//TX character attached to the ctrl byte

		//**********************************************************//
		unsigned char r_addr2; 			//response addr
		unsigned char r_ctrl2;			//response ctrl character
		unsigned char r_pumpno2;			//pump no i.e. which pump nozzle
		unsigned char r_trans2;			//transaction no. CD1, CD2 e.t.c
		unsigned char r_lng2;			//length of data received in message
		unsigned char r_TX2;				//TX character attached to the ctrl byte

		void dart_init(void);

		extern pump_status_enum pump_status_1,
								pump_status_2;

		extern uint8_t changeLitrePrice1,
					   changeLitrePrice2;

		extern uint8_t auth_cmd_flag,
					   auth_cmd_flag2,
					   stop_flag,
					   stop_flag2;

		uint8_t fillingComplete_flag1,
				nozzlezUp1,
				authsuspend_flag1,
				fillingsuspend_flag1,
				authresume_flag1,
				fillingresume_flag1;

		uint8_t fillingComplete_flag2,
				nozzlezUp2,
				authsuspend_flag2,
				fillingsuspend_flag2,
				authresume_flag2,
				fillingresume_flag2;

		extern uint8_t nozzleDown_source1,
					   reset_flag1;

		extern uint8_t nozzleDown_source2,
							   reset_flag2;

		extern float price_upper1,
					 amt_middle1;

		extern float price_upper2,
					 amt_middle2;

		extern float auth_v1,
					 auth_p1;

		extern float auth_v2,
					 auth_p2;

		extern uint8_t dp_amount1,
					   dp_vol1,
					   dp_unitprice1;

		extern uint8_t dp_amount2,
					   dp_vol2,
					   dp_unitprice2;

		extern float totaliser_vol1c,
					 totaliser_vol2c,
					 totaliser_vol1,
					 totaliser_vol2;

//		vector<unsigned char> r_raw_data;	//raw data received

		response_enum resp,				//response flag for type of message received... intended to use "map<string, unsigned char> response" initially
					  resp2;

		nozzle_state_enum nozzle_state;		//state of nozzle returned from pump

	/*********************************************************************
	 * Member functions for encoding and decode messages
	 ********************************************************************/

	void parse_extract(void);

	void parse_decode(void); 						//GETs each data from the raw data
	void parse_decode2(void);
	void process_response1(response_enum response);
	void process_response2(response_enum response);
	void _process_response1(response_enum response);
	void _process_response2(response_enum response);

	void send_acknowledgement1(response_enum response);
	void send_acknowledgement2(response_enum response);

	void go_setUnitPrice1(float price_update);
	float go_fillingPrice1(void);
	float go_fillingInfo_vol1(void);
	float go_fillingInfo_amt1(void);

	void go_setUnitPrice2(float price_update);
	float go_fillingPrice2(void);
	float go_fillingInfo_vol2(void);
	float go_fillingInfo_amt2(void);

	/***************************************************************************
	 *
	 * The BaseProtocol variables and functions
	 *
	 ***************************************************************************/

uint16_t calculate_crc(uint8_t *data, size_t length);

uint32_t packed_bcd_to_decimal(uint32_t bcd);

void go_read(void);
void go_write(void);
void go_write2(void);

void int_to_bcd(int num, unsigned char *bcd);
void int_to_bcd_(int num, unsigned char *bcd, uint8_t bcd_size);
uint8_t bcd_to_int(unsigned char bcd);
uint32_t bcdArray_to_int(unsigned char bcd[], uint8_t length);
int8_t countDigits(int number);
int powerOfTen(uint8_t exponent);

//double round_off(float value, int decimalPlaces);

void dart_init(void);

void send_nozzleStatus1(uint8_t buff_index);
void send_pumpStatus1(uint8_t buff_index);
void send_fillingInfo1(uint8_t buff_index);

void send_nozzleStatus2(uint8_t buff_index);
void send_pumpStatus2(uint8_t buff_index);
void send_fillingInfo2(uint8_t buff_index);

uint8_t configChange_notify_build1(uint8_t track_num_);

uint8_t configChange_notify_build2(uint8_t track_num_);


#endif /* SOURCE_GOGO_P_WAYNE_485_H_ */

//#endif /* DART_PROTOCOL_DART__H_ */
