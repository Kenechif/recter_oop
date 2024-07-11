///*
// * drt.h
// *
// *  Created on: may 15, 2022
// *      Author: tunji
// */
//
//#ifndef DART_PROTOCOL_DRT_H_
//#define DART_PROTOCOL_DRT_H_
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//
//#define MAX_LENGTH_OF_COMMAND 50 		//This is the message sent to the pump
//#define DLE 0x10
//#define ETX 0x03
//#define SF 0xfa
//
//
////protocol macros
//#define MAX_PRESET_VOL 99999	//in liters
//#define MAX_NOP 1
//#define MAX_NON 2	//MAX number of nozzles
//#define MAX_NOS 2	//MAX number of Sides (pumps side, not nozzle)
//
///* options for COMMANDS to send */
//		typedef enum{
//				POLL,
//				ACK,
//				NACK,
//				ACKPOLL,
//
//				GETSTATUS,
//				RESET,
//				AUTHORISE,
//				STOP,
//				SWITCH_OFF,
//				REQUEST_FILLING_INFO,
//				RETURN_PUMP_PARAM,
//				RETURN_PUMP_IDENTITY,
//				SUSPEND_REQUEST,
//				RESUME_REQUEST,
//
//				PRESET_VOL,	//Set the respective volume of these before using them
//				PRESET_AMO,
//
//				PRICE_UPDATE,
//				SET_MAMO,
//				REQUEST_VOL_TOTAL_COUNT
//			}command_enum;
//
//	/* flags for raw RESPONSE RECEIVED */
//		typedef enum{
//				NO_ETX = -2,
//				CRC_ERROR = -1,
//				JUNK,
//				NOREPLY,
//
//				r_EOT,
//				r_ACK,
//				r_NACK,
//				DATA_PUMPNO,
//				DATA_STATUS,
//				DATA_VOL_AMO,
//				DATA_NOZZLE_STAT_PRICE,
//				DATA_ALARM_CODE,
//				DATA_MAX_AMO, //mamo: maximum amount, also get DPVOL, DPAMO, DPPRI and GRADE
//				DATA_PUMP_ID,	//
//				DATA_TOTALVOL,	//Total vol from id
//				DATA_SUSPEND,
//				DATA_RESUME,
//				//...;
//			}response_enum;
//
//	/* enum for nozzle state */
//		typedef enum{
//			NOZZLE_DOWN,
//			NOZZLE_UP,
//		}nozzle_state_enum;
//
//	/* enum for pump status */
//		typedef enum{
//			STATUS_UNKNOWN, //used for idle state BY the main program to set the state of the pump
//			STATUS_PNP,
//			STATUS_RESET,
//			STATUS_AUTH,
//			STATUS_FILLING,
//			STATUS_FILLING_COMP,
//			STATUS_MAMO_REACHED,
//			STATUS_SWITCHED_OFF,
//		}pump_status_enum;
//
//		/**************************************************************
//		 * ENUM variables for: Command, Response, state_input, High level state, data state and mode
//		 *
//		 ************************************************************/
//			command_enum command;			//the commands the user passes into the send_command method
//			response_enum resp;				//response flag for type of message received... intended to use "map<string, unsigned char> response" initially
//
//	//		pump_state_input_enum status_input;	//this is a function of the two below, used in the mearly FSM
//				pump_status_enum status;			//status returned from pump
//				nozzle_state_enum nozzle_state;		//state of nozzle returned from pump
//
//#ifdef __cplusplus
//}
//#endif
//
//
//
//#endif /* DART_PROTOCOL_DRT_H_ */
