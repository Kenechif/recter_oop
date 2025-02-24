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
#include "settings.h"

extern UART_HandleTypeDef huart3;
extern float price_upper1,
			 amt_middle1;

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

extern float running_volTotaliser1_tmin1,
		     running_volTotaliser1_tmin2,
		     running_volTotaliser1_tmin3,
		     running_volTotaliser1c_tmin1,
		     running_volTotaliser1c_tmin2,
		     running_volTotaliser1c_tmin3;

extern float running_volTotaliser2_tmin1,
		     running_volTotaliser2_tmin2,
		     running_volTotaliser2_tmin3,
		     running_volTotaliser2c_tmin1,
		     running_volTotaliser2c_tmin2,
		     running_volTotaliser2c_tmin3;

extern float running_amtTotaliser1_tmin1,
		     running_amtTotaliser1_tmin2,
		     running_amtTotaliser1_tmin3,
		     running_amtTotaliser1c_tmin1,
		     running_amtTotaliser1c_tmin2,
		     running_amtTotaliser1c_tmin3;

extern float running_amtTotaliser2_tmin1,
		     running_amtTotaliser2_tmin2,
		     running_amtTotaliser2_tmin3,
		     running_amtTotaliser2c_tmin1,
		     running_amtTotaliser2c_tmin2,
		     running_amtTotaliser2c_tmin3;

extern float working_amtTotaliser1,
			 working_amtTotaliser1c,
			 running_amtTotaliser1,
			 running_amtTotaliser1c,
			 working_amtTotaliser2,
			 working_amtTotaliser2c,
			 running_amtTotaliser2,
			 running_amtTotaliser2c;

extern uint32_t  r_volTotaliser1,
		         old_r_volTotaliser1,
				 r_volTotaliser2,
		         old_r_volTotaliser2;

extern float running_volTotaliser1_array[4],
			 running_volTotaliser1c_array[4],
			 running_volTotaliser2_array[4],
			 running_volTotaliser2c_array[4];

extern float running_amtTotaliser1_array[4],
			 running_amtTotaliser1c_array[4],
			 running_amtTotaliser2_array[4],
			 running_amtTotaliser2c_array[4];

extern float amt_middle1_tmin1,
			 amt_middle1_tmin2,
			 amt_middle1_tmin3,
			 amt_middle2_tmin1,
			 amt_middle2_tmin2,
			 amt_middle2_tmin3;

extern float amt_real1_tmin1,
			 amt_real1_tmin2,
			 amt_real1_tmin3,
			 amt_real2_tmin1,
			 amt_real2_tmin2,
			 amt_real2_tmin3;

extern float amt_real1_array[4],
			 amt_middle1_array[4],
			 amt_real2_array[4],
			 amt_middle2_array[4];


extern uint16_t countar,
				countar2;

extern float r_amtTotaliser,
	         old_r_amtTotaliser;

extern float amt_middle1,
			 amt_middle2;

extern float previousValue_tv1,
      	  	 currentValue_tv1;

extern float previousValue_tv2,
      	  	 currentValue_tv2;

extern float previous_totaliserVol1c,
			 previous_totaliserVol2c,
			 totaliser_vol1c,
			 totaliser_vol2c;

static int rcv_count = 0;

//extern uint32_t currentPulser_recovered1 CCRAM,
//				currentPulser_recovered2 CCRAM,
//				targetPulser_recovered1 CCRAM,
//				targetPulser_recovered2 CCRAM;

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
