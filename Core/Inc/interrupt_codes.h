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
