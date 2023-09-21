/*
 * interrupt_codes.h
 *
 *  Created on: 25 Jan 2022
 *      Author: Tunjow
 */

#ifndef INC_INTERRUPT_CODES_H_
#define INC_INTERRUPT_CODES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

 typedef struct rcv_structure
{
    uint8_t preamble;	  //just a unique byte data
    uint8_t crc;		  /**< message type: command, response, */
    uint8_t command;
    uint8_t payload_len; //4  //several data type can be stringed together just like in ifsf
    uint8_t data[20-10]; //24
	uint8_t footer;      //25
} ;

 extern  UART_HandleTypeDef *huart;
// extern  rcv_structure MsgStruct_;
// extern  rcv_structure packet;
/*
union strr
{
	struct rcv_structure MsgStruct;
	uint8_t rx[25];                    // receive buffer
}rxdata;  */


extern float working_volTotaliser1,
			 working_volTotaliser1c,
			 running_volTotaliser1,
			 running_volTotaliser1c,
			 working_volTotaliser2,
			 working_volTotaliser2c,
			 running_volTotaliser2,
			 running_volTotaliser2c;

extern float working_amtTotaliser1,
			 working_amtTotaliser1c,
			 running_amtTotaliser1,
			 running_amtTotaliser1c,
			 working_amtTotaliser2,
			 working_amtTotaliser2c,
			 running_amtTotaliser2,
			 running_amtTotaliser2c;

extern uint32_t  r_volTotaliser,
		         old_r_volTotaliser;

extern uint16_t countar,
				countar2;

extern float r_amtTotaliser,
	         old_r_amtTotaliser;

static int rcv_count = 0;

//uint8_t rx_[25];

 void toggle_led(void);
 void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

 void HAL_UART_TxHalfCpltCallback_(UART_HandleTypeDef *huart);
 void HAL_UART_RxCpltCallback_(UART_HandleTypeDef *huart);

 void check_flow(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_INTERRUPT_CODES_H_ */
