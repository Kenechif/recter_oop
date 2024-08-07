/*
 * dart_.h
 *
 *  Created on: 28 Jun 2024
 *      Author: Jheebolar
 */

#ifndef DART_PROTOCOL_DART__H_
#define DART_PROTOCOL_DART__H_
/*
 * p_wayne_485.h
 *
 *  Created on: Jan 23, 2020
 *      Author: J.cliff
 */

#include "settings.h"
#include "pump_comm.h"

//#include <vector>
//#include "BaseProtocol.h"
//
//#ifndef SOURCE_GOGO_P_WAYNE_485_H_
//#define SOURCE_GOGO_P_WAYNE_485_H_

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

//extern pump_settings settings[2],
//					 settings0[2];

extern pump_settings_stream1 settings_stream1[2],
					 	 	 settings0_stream1[2];

extern pump_settings_stream2 settings_stream2[2],
							 settings0_stream2[2];

extern pump_settings_stream3 settings_stream3[2],
				       	     settings0_stream3[2],
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

extern unsigned char pumpno;

uint8_t  DART_BUFF1[128],
		 DART_BUFF2[128],
		 array_len;


uint8_t	 r_raw_data1[128];

//uint8_t rx_buf1[pump_rx_bufsize] = {0};

extern int millis;

int millis2;

extern uint8_t dummyValue;



// class dart: public BaseProtocol{
// 	private:
	/* options for COMMANDS to send */
		typedef enum
		{
				// POLL,
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

				//----- Pump Status -----//
				// PUMP_NOT_PROGRAMMED,
				// RESET,
				// AUTHORIZED,
				// FILLING,
				// FILLING_COMPLETED,
				// MAX_AMOUNTVOLUME_REACHED,
				// SWITCHED_OFF,

				NOZSTATUS_AND_FILLINGPRICE,

				PUMP_IDENTITY,

				PRESET_VOL,	//Set the respective volume of these before using them
				PRESET_AMO,

				FILLED_VOLUME_AND_AMOUNT,

				PRICE_UPDATE,
				SET_MAMO,
				REQUEST_VOL_TOTAL_COUNT,
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
				// DATA_STATUS,
				// DATA_VOL_AMO,
				// DATA_NOZZLE_STAT_PRICE,
				// DATA_ALARM_CODE,
				// DATA_MAX_AMO, //mamo: maximum amount, also get DPVOL, DPAMO, DPPRI and GRADE
				// DATA_PUMP_ID,	//
				// DATA_TOTALVOL,	//Total vol from id
				// DATA_SUSPEND,
				// DATA_RESUME,

				DATA_COMMAND,
				DATA_ALLOWED_NOZ,
				DATA_PRESET_VOL,
				DATA_PRESET_AMO,
				DATA_PRICE_UPDATE,
				DATA_SET_MAMO,
				DATA_REQUEST_VOL_TOTAL_COUNT,


				// PRESET_VOL,	//Set the respective volume of these before using them
				// PRESET_AMO,

				// PRICE_UPDATE,
				// SET_MAMO,
				// REQUEST_VOL_TOTAL_COUNT
				//...;
			}response_enum;

	/* enum for nozzle state */
//		typedef enum{
//			NOZZLE_DOWN,
//			NOZZLE_UP,
//		}nozzle_state_enum_;

	/* enum for pump status */
		typedef enum{
//			STATUS_UNKNOWN1, //used for idle state BY the main program to set the state of the pump

			// STATUS_PNP,
			// STATUS_RESET,
			// STATUS_AUTH,
			// STATUS_FILLING,
			// STATUS_FILLING_COMP,
			// STATUS_MAMO_REACHED,
			// STATUS_SWITCHED_OFF,

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
		unsigned char addr;				//address of pump
		unsigned short trans;			//transaction no. .. CD1, CD2 e.t.c
		unsigned char lng;				//length of data
		unsigned char ctrl;				//ctrl character
		unsigned char TX; 			//the LSB (second 4 bit) of the ctrl byte
			//int TX_cnt;

		unsigned char crc1;
		unsigned char crc2;
//		vector<unsigned char> full_msg;	//stores the full_msg to send
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

//		static unsigned char price_update_bcd[MAX_NOP*MAX_NON][3];


		extern unsigned char pumpno;
//				 addr;

		void dart_init(void);

//		/* calculated info from pump data above */
		float filling_price;
		double totalizer;
		float filled_vol;
		float filled_amo;

		int filling_start_time;		//time at the start of filling1
		int filling_duration;		//time it takes to complete a filling1 process
		int nozzle_num;

		bool  setPriceFlag;			//used to alert the protocol interface to set the price to a new price

    	pump_state_enum state;				//this is used in a finite state machine, it is also what GO broadcast to the server
		pump_state_enum prevState;			//previous machine state

//		char name[10];
//		float unitprice;
		double vol_totalizer;
		double amo_totalizer;

//		vector<unsigned char> r_raw_data;	//raw data received


	/**************************************************************
	 * ENUM variables for: Command, Response, state_input, High level state, data state and mode
	 *
	 ************************************************************/
//		command_enum command;			//the commands the user passes into the send_command method
		response_enum resp;				//response flag for type of message received... intended to use "map<string, unsigned char> response" initially

//		pump_state_input_enum status_input;	//this is a function of the two below, used in the mearly FSM
//			pump_status_enum status;			//status returned from pump
		nozzle_state_enum nozzle_state;		//state of nozzle returned from pump

//public:
	/*********************************************************************
	 * constructors
	 ********************************************************************/
//	dart();


	/*********************************************************************
	 * Algorithm for data interpretation and communication
	 ********************************************************************/
	void get_crc(unsigned char*, unsigned int); 					//Takes in an array and generate its crc then updates the public crc1 and crc2

	/*********************************************************************
	 * Member functions for encoding and decode messages
	 ********************************************************************/
	void send_command(command_enum);						//API: generate command to send to pump from user input
		void add_crc(unsigned char);					//gets the size of message from send_command() and add the crc and etx
		char* print_msg();

	void receive_response(unsigned char*, int);			//parse the response from the pump, enter: array pointer and sizeof(array)
	//void test_auth(unsigned char* , int);
//		void parse_message(unsigned char*, int);
	void parse_extract(void);
//		void parse_message(unsigned char*);
	void parse_decode(void); 						//GETs each data from the raw data
	void process_response(response_enum response);
	void process_response1(response_enum response);
	void send_acknowledgement(response_enum response);


	int handle_resp(command_enum cmd); 	//ret  = -10 : ended with a nack, -1: ended with
	/************************************************************************
	 * low level methods for building and sending messages
	 ***********************************************************************/
	void cmd_update_price(float price);
	void cmd_switch_off();
	void cmd_reset();
	void cmd_get_decimal_points();
	void cmd_get_totalizer();
	void cmd_get_filling_info();
	void cmd_get_status();
	void cmd_set_vol(float vol);
	void cmd_set_amo(float amo);
	void cmd_authorise();
	void cmd_stop();
	void cmd_poll();
	void cmd_ack();
	void cmd_nack();

//	pump_state_input_enum getStateInput();			// get the state input which is the combination of pump status and nozzle state
	pump_state_enum getStateInput();			// get the state input which is the combination of pump status and nozzle state

	/***************************************************************************
	 *
	 * The BaseProtocol variables and virtual functions
	 *
	 ***************************************************************************/


	//uart object member functions
	void uart_setup();
//	void uart_receive(); 	//for setting up the uart communication
	void uart_send(u8* write_buff, u32 size); 	//for setting up the uart communication
	void uart_complete_event_proc();

	u8 getAddr();							//get the pump address
	void setAddr(u8 i);							//set the pump address

//	void wifiAuthPumpByAmo(float amo);

//	virtual void PNP_setup();
	void get_status();				// get the pump's status
	void get_unit_price();				// get the current price from the pump
	void set_unit_price(float price);	// set the pump price
	void get_totalizer();			// get the pump's current totalizer value
	float get_filled_vol();			// get the pump's volume
	float get_filled_amo();		// get the pump's amount
	void get_filling_info();		// get the filling information from the pump
	int auth_by_vol1(float vol);	// authorize the pump's volume
	int auth_by_amo1(float amnt);	// authorize the pump's amount
	int set_amo1(float amnt);	//this method is peculiar to dart protocol only and this is used in filling state to ensure that the filled amount does not exceed its limit
	void authorise();	// authorise the pump with max vol of 10000
	void stop1();
	void pause();
	void resume();
	void switch_off();				// switch off the pump
	void reset1();					// reset the pump

	void calLastResponseTime();	//for calculating and asigning a value to the status enum

	void send_get_uart_msg(unsigned char* write_buf, unsigned int size);		//send and wait for msg from the uart

	/* Use the one below to specify a wait time for specific messages
	 * especially ACK messages */
//	void send_get_uart_msg(unsigned char* write_buf, unsigned int size, s32 wait_time );		//send and wait for msg from the uart
	void send_get_uart_msg();

//	void ~dart();

	/* polymorphism for protocol interface */
//	virtual void while_filling_auto_ch();

// };

uint16_t calculate_crc(uint8_t *data, size_t length);

uint32_t packed_bcd_to_decimal(uint32_t bcd);

void go_read(void);

//function overload of the parsed message to parse ack, nack, eot messages
void parse_message1(unsigned char* arr);

//parse_message : separate the received message into its different constituent
//void parse_message(unsigned char* arr, int size);

void int_to_bcd(int num, unsigned char *bcd);

double roundUp(float value, int decimalPlaces);

void dart_init(void);


#endif /* SOURCE_GOGO_P_WAYNE_485_H_ */

//#endif /* DART_PROTOCOL_DART__H_ */
