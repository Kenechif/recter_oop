/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "thread1.h"
#include "interrupt_codes.h"
#include "write__.h"
#include "stdio.h"
#include "myLibraries.h"
#include "string.h"
#include "stdbool.h"

#include "UartRingbuffer.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;

RNG_HandleTypeDef hrng;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
extern uint32_t pulser1;
extern uint32_t pulser2;
extern char prn[40];

extern pump_settings_stream2 settings_stream2[2];

//uint8_t dummyValue = 0;

ADC_ChannelConfTypeDef sConfig = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM5_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_UART5_Init(void);
static void MX_RNG_Init(void);
/* USER CODE BEGIN PFP */

// This array hold the the channels ADC value
volatile uint16_t adc_dma_result[2];
// This variable calculate the array length.
// In our case, array size in 3
int adc_channel_count = sizeof(adc_dma_result)/sizeof(adc_dma_result[0]);
// This flag will help to detect
// the DMA conversion completed or not
uint8_t adc_conv_complete_flag = 0;

// This character buffer array will
// store the result after conversion complete
char dma_result_buffer[100];

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//uint8_t data[100];
//
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//   HAL_UART_Transmit_IT(&huart1, data, sizeof (data));
//}


//#define RxBuf_SIZE   512
//#define MainBuf_SIZE 2048

uint8_t RxBuf[RxBuf_SIZE],
		MainBuf[MainBuf_SIZE],
		SOP;  //Start-Of-Packet

uint8_t go_buff[RxBuf_SIZE];

uint16_t oldPos = 0,
		 newPos = 0;

bool go_message = false;

int16_t head = 0,
		tail = 0;


int millis = 0;

extern unsigned long t_exec1,
					 t_exec2,
					 t_exec3,
					 t_exec4,
					 t_exec5,
					 t_exec6,
					 t_exec7,
					 t_exec8,
					 t_exec9;


//int isOK = 0;

extern unsigned char pumpno;




//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
//{
//	SOP = pumpno + 0x4F;
//
//	if (huart->Instance == USART2)
//	{
//		oldPos = newPos;  // Update the last position before copying new data
//
//		/* If the data in large and it is about to exceed the buffer size, we have to route it to the start of the buffer
//		 * This is to maintain the circular buffer
//		 * The old data in the main buffer will be overlapped
//		 */
//		if (oldPos+Size > MainBuf_SIZE)  // If the current position + new data size is greater than the main buffer
//		{
//			uint16_t datatocopy = MainBuf_SIZE-oldPos;  // find out how much space is left in the main buffer
//			memcpy ((uint8_t *)MainBuf+oldPos, RxBuf, datatocopy);  // copy data in that remaining space
//
//			oldPos = 0;  // point to the start of the buffer
//			memcpy ((uint8_t *)MainBuf, (uint8_t *)RxBuf+datatocopy, (Size-datatocopy));  // copy the remaining data
//			newPos = (Size-datatocopy);  // update the position
//		}
//
//		/* if the current position + new data size is less than the main buffer
//		 * we will simply copy the data into the buffer and update the position
//		 */
//		else
//		{
//			memcpy ((uint8_t *)MainBuf+oldPos, RxBuf, Size);
//			newPos = Size+oldPos;
//		}
//
//		head = newPos - Size;
//		tail = newPos - 1;
//
//		if (head < 0)  //checks for a wrap-around / overflow
//		{
//			head = MainBuf_SIZE - Size;
//		}
//
//		if( (MainBuf[tail] == 0xFA) && (MainBuf[head] == SOP) )
//		{
//			go_message = true;
//		}
//
//		/* start the DMA again */
//		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *) RxBuf, RxBuf_SIZE);
//		__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
//
//	}


//	/****************** PROCESS (Little) THE DATA HERE *********************/
//
////	/* Let's say we want to check for the keyword "OK" within our incoming DATA */
////	for (int i=0; i<Size; i++)
////	{
////		if ((RxBuf[i] == 'O') && (RxBuf[i+1] == 'K'))
////		{
////			isOK = 1;
////		}
////	}


//}

uint8_t END_MSG[35] = "Overflow Reached! Counter Reset!\n\r";

char buffer[5];

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	// stop_flow1();


	config_mode = 0;

	uint8_t MSG[35] = {'\0'};
	uint16_t CounterTicks = 0;


	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;


//	unsigned long t1 = DWT->CYCCNT;
//	/* do something */
//	unsigned long t2 = DWT->CYCCNT;
//	unsigned long diff = t2 - t1;

//	int millis = HAL_GetTick();

	//===========================================================================

	SCnSCB->ACTLR |= SCnSCB_ACTLR_DISDEFWBUF_Msk; // disable the write buffer
	SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk	| SCB_SHCSR_BUSFAULTENA_Msk
    | SCB_SHCSR_MEMFAULTENA_Msk; 				// Enable Usage-/Bus-/Mem Faults
	//===========================================================================
//  read signature from the backup ram...

//	int demo[12] = {0};
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

//  	  retrieve_settings();

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM5_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  MX_UART5_Init();
  MX_RNG_Init();
  /* USER CODE BEGIN 2 */

  retrieve_settings();    //Retrieves settings prior to Timers Initialisation

  MX_TIM2_Init();
  MX_TIM5_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  MX_UART5_Init();
  MX_RNG_Init();

  //--------------------------------------

//  	  HAL_GPIO_WritePin(buzzer_GPIO_Port, GPIO_PIN_12, GPIO_PIN_SET);
//	  HAL_Delay(200);
//	  HAL_GPIO_WritePin(buzzer_GPIO_Port, GPIO_PIN_12, GPIO_PIN_RESET);
//	  HAL_Delay(80);
//	  HAL_GPIO_WritePin(buzzer_GPIO_Port, GPIO_PIN_12, GPIO_PIN_SET);
//	  HAL_Delay(500);
//	  HAL_GPIO_WritePin(buzzer_GPIO_Port, GPIO_PIN_12, GPIO_PIN_RESET);


  // Initialize the DMA conversion
//   HAL_ADC_Start_DMA(&hadc1, (uint32_t *) adc_dma_result , adc_channel_count);

//   while(1)
//   {
//   		// when adc_conv_complete_flag is set to 1,
//   		// that means DMA conversion is completed
//   	   if(adc_conv_complete_flag == 1)
//   	   {
//   			  // this snprintf function helps to convert the adc_dma_result array
//   			  // into string and store in dma_result_buffer character array
//   			  snprintf(dma_result_buffer, 100, "CH_1: %d, CH_2: %d\r\n", adc_dma_result[0], adc_dma_result[1]);
//   			  // we just send the dma_result_buffer character array with ADC values
//   			  // to our computer serial terminal software (Tera Term) using UART peripheral of STM32
//   //			  HAL_UART_Transmit(&huart2, (uint8_t *) dma_result_buffer, sizeof(dma_result_buffer), HAL_MAX_DELAY);
//   			  // adc_conv_complete_flag variable is set to 0, because,
//   		         // we alert this flag variable for new DMA conversion completion
//   			 adc_conv_complete_flag = 0;
//   		        // delay for 500 Milliseconds
//   			 HAL_Delay(500);
//   		}
//   	    HAL_Delay(10);
//   }



  	Ringbuf_init ();

  	SOP = pumpno + 0x4F;

//  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RxBuf, RxBuf_SIZE);
//   __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

	setup();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	while (1)
	{
	//	 __HAL_TIM_SET_COUNTER(&htim5,4017);
	//	 demo[1000000] = 0;


//		Uart_write(3);

		int i = 0;

		memset(go_buff, 0, RxBuf_SIZE);

		t_exec1 = DWT->CYCCNT;

		while (IsDataAvailable())
		{
//		  int data = Uart_read();  // read the data in the rx_buffer
//		  Uart_write(data);  // send the data to the uart

		  go_buff[i] = Uart_read();  // read the data in the rx_buffer
		  i++;
		}

		t_exec4 = DWT->CYCCNT;
		t_exec5 = t_exec4 - t_exec1;

//		if( (MainBuf[tail] == 0xFA) && (MainBuf[head] == SOP) )
		if( (go_buff[i - 1] == 0xFA) && (go_buff[0] == SOP) )
		{
			go_message = true;
		}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		   // Read The Counter Ticks Register
//		CounterTicks = TIM2->CNT;
//		// Print The Ticks Count Via UART1
//		sprintf(MSG, "Ticks = %d\n\r", CounterTicks);
//		HAL_UART_Transmit(&huart2, MSG, sizeof(MSG), 100);
//		HAL_Delay(100);

		run();
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */
//
  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */
//
  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */
//
  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
             return;
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x13;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_SEPTEMBER;
  sDate.Date = 0x26;
  sDate.Year = 0x22;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */
//
//	TIM_Encoder_InitTypeDef sConfig = {0};
//	TIM_SlaveConfigTypeDef sSlaveConfig = {0};
//	TIM_MasterConfigTypeDef sMasterConfig = {0};
//
  /* USER CODE END TIM2_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */
//
  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 7;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 7;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
//
//
//	if(settings_stream2[1].pulser_type_ == quadrature)
//	{
//		sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
//		  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
//		  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
//		  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
//		  sConfig.IC1Filter = 7;
//		  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
//		  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
//		  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
//		  sConfig.IC2Filter = 7;
//		  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
//		  {
//		    Error_Handler();
//		  }
//		  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//		  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//		  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
//		  {
//		    Error_Handler();
//		  }
//	}
//	else if(settings_stream2[1].pulser_type_ == non_quadrature)
//	{
//		  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
//		  {
//			Error_Handler();
//		  }
//		  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
//		  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
//		  sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;
//		  sSlaveConfig.TriggerFilter = 0;
//		  if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
//		  {
//			Error_Handler();
//		  }
//		  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//		  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//		  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
//		  {
//			Error_Handler();
//		  }
//	}
//
  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

//	TIM_Encoder_InitTypeDef sConfig = {0};
//	TIM_SlaveConfigTypeDef sSlaveConfig = {0};
//	TIM_MasterConfigTypeDef sMasterConfig = {0};

	settings_stream2[0].pulser_type_ = non_quadrature;

  /* USER CODE END TIM5_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 0;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 4294967295;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 7;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 7;
  if (HAL_TIM_Encoder_Init(&htim5, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

//	if(settings_stream2[0].pulser_type_ == quadrature)
//	{
//		  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
//		  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
//		  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
//		  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
//		  sConfig.IC1Filter = 7;
//		  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
//		  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
//		  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
//		  sConfig.IC2Filter = 7;
//		  if (HAL_TIM_Encoder_Init(&htim5, &sConfig) != HAL_OK)
//		  {
//		    Error_Handler();
//		  }
//		  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//		  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//		  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
//		  {
//		    Error_Handler();
//		  }
//	}
//	else if(settings_stream2[0].pulser_type_ == non_quadrature)
//	{
//		  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
//		  {
//		    Error_Handler();
//		  }
//		  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
//		  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
//		  sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;
//		  sSlaveConfig.TriggerFilter = 0;
//		  if (HAL_TIM_SlaveConfigSynchro(&htim5, &sSlaveConfig) != HAL_OK)
//		  {
//		    Error_Handler();
//		  }
//		  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//		  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//		  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
//		  {
//		    Error_Handler();
//		  }
//	}

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_9B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_ODD;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, lthk2_Pin|refresh2_Pin|RE_Pin|slow_sole1_Pin
                          |slow_sole2_Pin|fast_sole1_Pin|fast_sole2_Pin|motor_drv1_Pin
                          |motor_drv2_Pin|clkk2_Pin|datak2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, T1output_Pin|T2output_Pin|dataPin_Pin|clockPin_Pin
                          |latchPin_Pin|dataPin2_Pin|clockPin2_Pin|latchPin2_Pin
                          |batt_check_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, pwr_actv_Pin|pump_Pin|modem_power_Pin|UCD_power_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, clkk1_Pin|datak1_Pin|lthk1_Pin|refresh_Pin
                          |nLed_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : lthk2_Pin refresh2_Pin RE_Pin slow_sole1_Pin
                           slow_sole2_Pin fast_sole1_Pin fast_sole2_Pin motor_drv1_Pin
                           motor_drv2_Pin clkk2_Pin datak2_Pin */
  GPIO_InitStruct.Pin = lthk2_Pin|refresh2_Pin|RE_Pin|slow_sole1_Pin
                          |slow_sole2_Pin|fast_sole1_Pin|fast_sole2_Pin|motor_drv1_Pin
                          |motor_drv2_Pin|clkk2_Pin|datak2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : pulser1_detect_Pin sense_p_pwr_Pin pulser2_detect_Pin */
  GPIO_InitStruct.Pin = pulser1_detect_Pin|sense_p_pwr_Pin|pulser2_detect_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : sense_pwr_Pin */
  GPIO_InitStruct.Pin = sense_pwr_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(sense_pwr_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : tamper_Pin T1sense_Pin T2sense_Pin */
  GPIO_InitStruct.Pin = tamper_Pin|T1sense_Pin|T2sense_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : T1output_Pin T2output_Pin dataPin_Pin clockPin_Pin
                           latchPin_Pin dataPin2_Pin clockPin2_Pin latchPin2_Pin
                           batt_check_Pin */
  GPIO_InitStruct.Pin = T1output_Pin|T2output_Pin|dataPin_Pin|clockPin_Pin
                          |latchPin_Pin|dataPin2_Pin|clockPin2_Pin|latchPin2_Pin
                          |batt_check_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : FLASH_CS_Pin buzzer_Pin */
  GPIO_InitStruct.Pin = FLASH_CS_Pin|buzzer_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : nozzleSW_Pin dtr_p2_Pin dtr_p1_Pin */
  GPIO_InitStruct.Pin = nozzleSW_Pin|dtr_p2_Pin|dtr_p1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : pwr_actv_Pin pump_Pin modem_power_Pin UCD_power_Pin */
  GPIO_InitStruct.Pin = pwr_actv_Pin|pump_Pin|modem_power_Pin|UCD_power_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : pin19K1_Pin keyk2_Pin com12k2_Pin pin19K2_Pin
                           settings1_Pin settings2_Pin keyk1_Pin com12k1_Pin */
  GPIO_InitStruct.Pin = pin19K1_Pin|keyk2_Pin|com12k2_Pin|pin19K2_Pin
                          |settings1_Pin|settings2_Pin|keyk1_Pin|com12k1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : clkk1_Pin datak1_Pin lthk1_Pin refresh_Pin
                           nLed_Pin */
  GPIO_InitStruct.Pin = clkk1_Pin|datak1_Pin|lthk1_Pin|refresh_Pin
                          |nLed_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : case_open_Pin */
  GPIO_InitStruct.Pin = case_open_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(case_open_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : nozzleSW2_Pin */
  GPIO_InitStruct.Pin = nozzleSW2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(nozzleSW2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : network_connected_Pin */
  GPIO_InitStruct.Pin = network_connected_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(network_connected_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

