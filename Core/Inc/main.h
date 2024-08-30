/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "string.h"
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define lthk2_Pin GPIO_PIN_2
#define lthk2_GPIO_Port GPIOE
#define refresh2_Pin GPIO_PIN_3
#define refresh2_GPIO_Port GPIOE
#define pulser1_detect_Pin GPIO_PIN_4
#define pulser1_detect_GPIO_Port GPIOE
#define RE_Pin GPIO_PIN_5
#define RE_GPIO_Port GPIOE
#define sense_pwr_Pin GPIO_PIN_6
#define sense_pwr_GPIO_Port GPIOE
#define tamper_Pin GPIO_PIN_13
#define tamper_GPIO_Port GPIOC
#define T1output_Pin GPIO_PIN_0
#define T1output_GPIO_Port GPIOC
#define T1sense_Pin GPIO_PIN_1
#define T1sense_GPIO_Port GPIOC
#define T2output_Pin GPIO_PIN_2
#define T2output_GPIO_Port GPIOC
#define T2sense_Pin GPIO_PIN_3
#define T2sense_GPIO_Port GPIOC
#define batt_voltRead_Pin GPIO_PIN_2
#define batt_voltRead_GPIO_Port GPIOA
#define batt_sense_Pin GPIO_PIN_3
#define batt_sense_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_4
#define FLASH_CS_GPIO_Port GPIOA
#define dataPin_Pin GPIO_PIN_4
#define dataPin_GPIO_Port GPIOC
#define clockPin_Pin GPIO_PIN_5
#define clockPin_GPIO_Port GPIOC
#define nozzleSW_Pin GPIO_PIN_0
#define nozzleSW_GPIO_Port GPIOB
#define pwr_actv_Pin GPIO_PIN_1
#define pwr_actv_GPIO_Port GPIOB
#define pump_Pin GPIO_PIN_2
#define pump_GPIO_Port GPIOB
#define sense_p_pwr_Pin GPIO_PIN_7
#define sense_p_pwr_GPIO_Port GPIOE
#define pulser2_detect_Pin GPIO_PIN_8
#define pulser2_detect_GPIO_Port GPIOE
#define other_Pin GPIO_PIN_9
#define other_GPIO_Port GPIOE
#define slow_sole1_Pin GPIO_PIN_10
#define slow_sole1_GPIO_Port GPIOE
#define slow_sole2_Pin GPIO_PIN_11
#define slow_sole2_GPIO_Port GPIOE
#define fast_sole1_Pin GPIO_PIN_12
#define fast_sole1_GPIO_Port GPIOE
#define fast_sole2_Pin GPIO_PIN_13
#define fast_sole2_GPIO_Port GPIOE
#define motor_drv1_Pin GPIO_PIN_14
#define motor_drv1_GPIO_Port GPIOE
#define motor_drv2_Pin GPIO_PIN_15
#define motor_drv2_GPIO_Port GPIOE
#define p1_RX_Pin GPIO_PIN_10
#define p1_RX_GPIO_Port GPIOB
#define pi_TX_Pin GPIO_PIN_11
#define pi_TX_GPIO_Port GPIOB
#define pin19K1_Pin GPIO_PIN_8
#define pin19K1_GPIO_Port GPIOD
#define clkk1_Pin GPIO_PIN_9
#define clkk1_GPIO_Port GPIOD
#define datak1_Pin GPIO_PIN_10
#define datak1_GPIO_Port GPIOD
#define lthk1_Pin GPIO_PIN_11
#define lthk1_GPIO_Port GPIOD
#define refresh_Pin GPIO_PIN_12
#define refresh_GPIO_Port GPIOD
#define keyk2_Pin GPIO_PIN_13
#define keyk2_GPIO_Port GPIOD
#define com12k2_Pin GPIO_PIN_14
#define com12k2_GPIO_Port GPIOD
#define pin19K2_Pin GPIO_PIN_15
#define pin19K2_GPIO_Port GPIOD
#define latchPin_Pin GPIO_PIN_6
#define latchPin_GPIO_Port GPIOC
#define dataPin2_Pin GPIO_PIN_7
#define dataPin2_GPIO_Port GPIOC
#define clockPin2_Pin GPIO_PIN_8
#define clockPin2_GPIO_Port GPIOC
#define latchPin2_Pin GPIO_PIN_9
#define latchPin2_GPIO_Port GPIOC
#define case_open_Pin GPIO_PIN_8
#define case_open_GPIO_Port GPIOA
#define recter_TX_Pin GPIO_PIN_9
#define recter_TX_GPIO_Port GPIOA
#define recter_Rx_Pin GPIO_PIN_10
#define recter_Rx_GPIO_Port GPIOA
#define nozzleSW2_Pin GPIO_PIN_11
#define nozzleSW2_GPIO_Port GPIOA
#define buzzer_Pin GPIO_PIN_12
#define buzzer_GPIO_Port GPIOA
#define network_connected_Pin GPIO_PIN_10
#define network_connected_GPIO_Port GPIOC
#define batt_check_Pin GPIO_PIN_11
#define batt_check_GPIO_Port GPIOC
#define p2_TX_Pin GPIO_PIN_12
#define p2_TX_GPIO_Port GPIOC
#define nLed_Pin GPIO_PIN_0
#define nLed_GPIO_Port GPIOD
#define settings1_Pin GPIO_PIN_1
#define settings1_GPIO_Port GPIOD
#define p2_RX_Pin GPIO_PIN_2
#define p2_RX_GPIO_Port GPIOD
#define settings2_Pin GPIO_PIN_3
#define settings2_GPIO_Port GPIOD
#define keyk1_Pin GPIO_PIN_4
#define keyk1_GPIO_Port GPIOD
#define com12k1_Pin GPIO_PIN_7
#define com12k1_GPIO_Port GPIOD
#define modem_power_Pin GPIO_PIN_4
#define modem_power_GPIO_Port GPIOB
#define UCD_power_Pin GPIO_PIN_5
#define UCD_power_GPIO_Port GPIOB
#define dtr_p2_Pin GPIO_PIN_8
#define dtr_p2_GPIO_Port GPIOB
#define dtr_p1_Pin GPIO_PIN_9
#define dtr_p1_GPIO_Port GPIOB
#define clkk2_Pin GPIO_PIN_0
#define clkk2_GPIO_Port GPIOE
#define datak2_Pin GPIO_PIN_1
#define datak2_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
