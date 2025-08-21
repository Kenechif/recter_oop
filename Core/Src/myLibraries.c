/*
 * myLibraries.c
 *
 *  Created on: jan 5, 2022
 *      Author: tunjow
 */

/** Contains:
 * 1. const definitions
 * 2. Functino body
 * */
#include "myLibraries.h"
#include "main.h"
#include "stdbool.h"
#include "settings.h"

char self->vbuf[MAX_STRLEN]  = {0};
char vbuf2[MAX_STRLEN]  = {0};

char SerialData[MAX_STRLEN] = {0};

uint32_t pulser1 = 0;
uint32_t pulser2 = 0;

drive drive1,
	  drive2;

extern ADC_ChannelConfTypeDef sConfig;

extern uint8_t disp->batteryVoltage_ready;

//extern  displaytype  disp_type1;
//extern  displaytype  disp_type2;
extern pump disp_type1,
            disp_type2;

//extern pump_settings settings[2];

extern pump_settings_stream1 self->settings_stream1[2],
					 	 	 settings_original_stream1[2];

extern pump_settings_stream2 self->settings_stream2[2],
							 settings_original_stream2[2];

extern pump_settings_stream3 self->settings_stream3[2],
				       	     settings_original_stream3[2],
							 copy_stream3[2];

extern ADC_HandleTypeDef hadc1;
extern uint16_t self->motor_tmr,
				motor_tmr2;

void drive_motor(Nozzle *self, drive drv);
void drive_slow_sole(Nozzle *self, drive drv);
void drive_fast_sole(Nozzle *self, drive drv);

void drive_motor2(drive drv);
void drive_slow_sole2(drive drv);
void drive_fast_sole2(drive drv);


//==========================   POWER  =============================================
int readpwr(FuelDispenser *disp)
{
	 return(  (HAL_GPIO_ReadPin(disp->sense_pwr_GPIO_Port, disp->sense_pwr_Pin ) == 1)? 0:1 );
}

int read_p_pwr(FuelDispenser *disp)
{
	 return(  (HAL_GPIO_ReadPin(disp->sense_p_pwr_GPIO_Port, disp->sense_p_pwr_Pin ) == 1)? 0:1 );
}

void mcu_power(FuelDispenser *disp, drive drv)
{
	HAL_GPIO_WritePin(disp->UCD_power_GPIO_Port, disp->UCD_power_Pin, drv);
}

void batt_charge(FuelDispenser *disp, drive drv)
{
	HAL_GPIO_WritePin(disp->batt_check_GPIO_Port, disp->batt_check_Pin, drv);
}

void modem_power(FuelDispenser *disp, drive drv)
{
	HAL_GPIO_WritePin(disp->modem_power_GPIO_Port, disp->modem_power_Pin, drv);
}

//void displayandkeypad_power(drive drv)
//{
////	if(drv == ACTIVATE)
////	 HAL_GPIO_WritePin(pwr_actv_GPIO_Port,pwr_actv_Pin, 0);
////	else
//	 HAL_GPIO_WritePin(pwr_actv_GPIO_Port,pwr_actv_Pin, drv);
//}

void displayandkeypad_power(FuelDispenser *disp, drive drv)
{
	 HAL_GPIO_WritePin(disp->pwr_actv_GPIO_Port, disp->pwr_actv_Pin, drv);
}

float battery_read(void)
{
	uint16_t digital_reading;
	float batt_v;

	ADC_ChannelConfTypeDef sConfig = {0};

	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_2;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

//	sConfig.Channel = ADC_CHANNEL_2;
//	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	HAL_ADC_Start(&hadc1); // start A/D conversion
	if(HAL_ADC_PollForConversion(&hadc1, 500) == HAL_OK) //check if conversion is completed & 500ms Timeout
	{
		digital_reading  = HAL_ADC_GetValue(&hadc1); // read digital value and save it inside uint32_t variable
	}
	HAL_ADC_Stop(&hadc1); // stop conversion
	HAL_Delay(100);

	batt_v = ( (digital_reading * 3.3 ) / 4095 );   //no_battery => 2.23V (@ 7.9V) low_cutoff => 1.81 (@ 6.4V)
	return batt_v;
}


float battery_sense(FuelDispenser *disp)
{
	static uint16_t disp->digital_reading;
	float batt_v;

	static uint8_t disp->firstTime_battSense = 1;
	uint8_t interval = 1300;   //1300 milliseconds

	static uint32_t disp->previousMillis_battSense = 0;

	if(disp->firstTime_battSense == 1)
	{
		HAL_GPIO_WritePin(batt_check_GPIO_Port, batt_check_Pin, GPIO_PIN_RESET);  //Temporarily switch off battery-charge

		HAL_Delay(1);

		ADC_ChannelConfTypeDef sConfig = {0};

		/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
		*/
		sConfig.Channel = ADC_CHANNEL_3;
		sConfig.Rank = 1;
		sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}

		HAL_ADC_Start(&hadc1); // start A/D conversion
		if(HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) //check if conversion is completed & 10ms Timeout
		{
			disp->digital_reading = HAL_ADC_GetValue(&hadc1); // read digital value and save it inside uint32_t variable

			disp->firstTime_battSense = 0;
		}
		else
		{
			disp->firstTime_battSense = 1;
		}

		HAL_ADC_Stop(&hadc1); // stop conversion

		disp->previousMillis_battSense = millis;

//		firstTime_battSense = 0;

		disp->batteryVoltage_ready = 0;

		HAL_GPIO_WritePin(batt_check_GPIO_Port, batt_check_Pin, GPIO_PIN_SET);  //Switches battery-charge ON
	}

	else if(disp->firstTime_battSense == 0)
	{
		if (millis - disp->previousMillis_battSense >= interval)       //		HAL_Delay(2000);
		{
			disp->firstTime_battSense = 1;

			batt_v = ( (disp->digital_reading * 3.3 ) / 4095 );   //BATTERY : Fully-charged => 2.22V (@ 8.4V) low_cutoff => 1.98 (@ 6.4V); 1.91 (@ 6.0V)
															//	1.88V (@ 5.90V)  1.8V (@5.5V)

//			HAL_GPIO_WritePin(batt_check_GPIO_Port, batt_check_Pin, GPIO_PIN_SET);  //Switches battery charge ON
//			timer_batteryRead = 0;
			disp->batteryVoltage_ready = 1;

			return batt_v;
		}
	}
	return 0;
}

//float battery_sense(void)
//{
//	uint16_t disp->firstTime_battSense;
//	float batt_v;
//
//	ADC_ChannelConfTypeDef sConfig = {0};
//
//	 /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
//	  */
//	  sConfig.Channel = ADC_CHANNEL_3;
//	  sConfig.Rank = 1;
//	  sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
//	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//	  {
//	    Error_Handler();
//	  }
//
////	sConfig.Channel = ADC_CHANNEL_2;
////	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
//
//	HAL_ADC_Start(&hadc1); // start A/D conversion
//	if(HAL_ADC_PollForConversion(&hadc1, 500) == HAL_OK) //check if conversion is completed & 500ms Timeout
//	{
//		digital_reading  = HAL_ADC_GetValue(&hadc1); // read digital value and save it inside uint32_t variable
//	}
//	HAL_ADC_Stop(&hadc1); // stop conversion
//	HAL_Delay(100);
//
//	batt_v = ( (digital_reading * 3.3 ) / 4095 );   //BATTERY : Fully-charged => 2.22V (@ 8.4V) low_cutoff => 1.98 (@ 6.4V); 1.91 (@ 6.0V)
//													//	1.88V (@ 5.90V)  1.8V (@5.5V)
//	return batt_v;
//}

//float battery_sense(void)
//{
//	uint16_t digital_reading = 0;
//	uint16_t digital_reading_array[10];
//	uint8_t i;
//	float batt_v;
//
//	sConfig.Channel = ADC_CHANNEL_3;
//	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
//
//	HAL_ADC_Start(&hadc1); // start A/D conversion
//
//	for(i = 0; i < 10; i++)
//	{
//		if(HAL_ADC_PollForConversion(&hadc1, 500) == HAL_OK) //check if conversion is completed & 500ms Timeout
//		{
//			digital_reading_array[i] = HAL_ADC_GetValue(&hadc1); // read digital value and save it inside uint32_t variable
//		}
//		HAL_Delay(20);
//	}
//
//	HAL_ADC_Stop(&hadc1); // stop conversion
//	HAL_Delay(100);
//
//	for(i = 0; i < 10; i++)
//	{
//		digital_reading += digital_reading_array[i];
//	}
//
//	digital_reading = (digital_reading / 10);
//
////	batt_v = ( (digital_reading * 3.3 ) / 4095 );   //no_battery => 2.23V (@ 7.9V) low_cutoff => 1.81 (@ 6.4V)
//	batt_v = ( (digital_reading * 3.3 ) / 4095 );   //no_battery => 2.22V (@ 8.4V) low_cutoff => 1.98 (@ 6.4V)
//
//	return batt_v;
//}

//=================================================================================
///                           HARDWARE  PERIPHERALS.
//=================================================================================
//uint8_t readNozzle1(void)
//{
//    return(  (HAL_GPIO_ReadPin(nozzleSW_GPIO_Port, nozzleSW_Pin ) == 1)? 0:1  );
//}
bool readNozzle(Nozzle *self)
{
	GPIO_TypeDef *port;
	uint16_t pin;

	port = (self->nozzle_id == 0)? nozzleSW_GPIO_Port : nozzleSW2_GPIO_Port;
	pin = (self->nozzle_id == 0)? nozzleSW_Pin : nozzleSW2_Pin;

	return(  (HAL_GPIO_ReadPin(port, pin ) == 1)? false : true  );
}

uint8_t readNozzle2(void)
{
    return(  (HAL_GPIO_ReadPin(nozzleSW2_GPIO_Port, nozzleSW2_Pin ) == 1)? 0:1  );
}
//------------------------------------
//int readtotaliser1_state(void)
//{
//    return(  (HAL_GPIO_ReadPin(T1sense_GPIO_Port, T1sense_Pin ) == 1)? 1:0  );
//}

uint8_t readtotaliser_state(Nozzle *self)
{
	GPIO_TypeDef *port;
	uint16_t pin;

	port = (self->nozzle_id == 0)? T1sense_GPIO_Port : T2sense_GPIO_Port;
	pin = (self->nozzle_id == 0)? T1sense_Pin : T2sense_Pin;

	return(  (HAL_GPIO_ReadPin(port, pin ) == 1)? 1 : 0  );
}

void drive_totaliser(Nozzle *self, drive drv)
{
	GPIO_TypeDef *port;
	uint16_t pin;

	port = (self->nozzle_id == 0)? T1output_GPIO_Port : T2output_GPIO_Port;
	pin = (self->nozzle_id == 0)? T1output_Pin : T2output_Pin;

	self->drive = drv;
	HAL_GPIO_WritePin(port, pin, drv);
}

//------------------------------------
//int readtotaliser2_state(void)
//{
//    return(  (HAL_GPIO_ReadPin(T2sense_GPIO_Port, T2sense_Pin ) == 1)? 1:0  );
//}
//
//void drive_totaliser2(drive drv)
//{
//	 HAL_GPIO_WritePin(T2output_GPIO_Port,T2output_Pin, drv);
//}
//------------------------------------


uint8_t readsettingskey_state(Nozzle *self)
{
	GPIO_TypeDef *port;
	uint16_t pin;

	port = (self->nozzle_id == 0)? keyk1_GPIO_Port : keyk2_GPIO_Port;
	pin = (self->nozzle_id == 0)? keyk1_Pin : keyk2_Pin;

	return(  (HAL_GPIO_ReadPin(port, pin ) == 0)? 1 : 0  );
}

uint8_t readkey19_state(Nozzle *self)
{
	GPIO_TypeDef *port;
	uint16_t pin;

	port = (self->nozzle_id == 0)? pin19K1_GPIO_Port : pin19K2_GPIO_Port;
	pin = (self->nozzle_id == 0)? pin19K1_Pin : pin19K2_Pin;

	return(  (HAL_GPIO_ReadPin(port, pin ) == 0)? 1 : 0  );
}
//--------------------------------------
int readsettingskey2_state(void)
{
    return(  (HAL_GPIO_ReadPin(keyk2_GPIO_Port, keyk2_Pin ) == 0)? 1:0  );
}

uint8_t readkey192_state(void)
{
    return(  (HAL_GPIO_ReadPin(pin19K2_GPIO_Port, pin19K2_Pin ) == 0)? 1:0  );
}
//-------------------------------------------------------------------------------
void slow_flow(Nozzle *self)
{
	if (self->motor_tmr >= 1000)
		drive_motor(&self, ACTIVATE);

	drive_slow_sole(&self, ACTIVATE);
	drive_fast_sole(&self, DEACTIVATE);
}

void fast_flow(Nozzle *self)
{
	if (self->motor_tmr >= 2000)
		drive_motor(&self, ACTIVATE);

	drive_slow_sole(&self, ACTIVATE);
	drive_fast_sole(&self, ACTIVATE);
}

void stop_flow(Nozzle *self)
{
	drive_slow_sole(&self, DEACTIVATE);
	drive_fast_sole(&self, DEACTIVATE);
	drive_motor(&self, DEACTIVATE);
}


//---------------------------------------------------------------
void slow_flow2(void)
{
	if (motor_tmr2 >= 2000)
		drive_motor2(ACTIVATE);

	drive_slow_sole2(ACTIVATE);
	drive_fast_sole2(DEACTIVATE);
}

void fast_flow2(void)
{
	if (motor_tmr2 >= 2000)
		drive_motor2(ACTIVATE);

	drive_slow_sole2(ACTIVATE);
	drive_fast_sole2(ACTIVATE);
}

void stop_flow2(void)
{
	drive_motor2(DEACTIVATE);
	drive_slow_sole2(DEACTIVATE);
	drive_fast_sole2(DEACTIVATE);
}

//void drive_solenoid1(drive drv)
//{
//	 HAL_GPIO_WritePin(solenoid_GPIO_Port,solenoid_Pin, drv);
//}

//void drive_pump1(drive drv)
//{
//	 HAL_GPIO_WritePin(pump_GPIO_Port,pump_Pin, drv);
//}
//---------------------------------------------------------------
//void drive_motor1(drive drv)
//{
//	 HAL_GPIO_WritePin(motor_drv1_GPIO_Port,motor_drv1_Pin, drv);
//}

void drive_motor(Nozzle *self, drive drv)
{
	GPIO_TypeDef* motor_drv_port = (self->nozzle_id == 0)? motor_drv1_GPIO_Port : motor_drv2_GPIO_Port;
	uint16_t motor_drv_Pin = (self->nozzle_id == 0)? motor_drv1_Pin : motor_drv2_Pin;
	HAL_GPIO_WritePin(motor_drv_port, motor_drv_Pin, drv);
}

//---------------------------------------------------------------
void drive_motor2(drive drv)
{
	 HAL_GPIO_WritePin(motor_drv2_GPIO_Port,motor_drv2_Pin, drv);
}
//---------------------------------------------------------------
//void drive_slow_sole1(drive drv)
//{
//	if (self->settings_stream1[0].pump_type_ == LAFENG)
//	{
//		HAL_GPIO_WritePin(slow_sole1_GPIO_Port,slow_sole1_Pin, !drv);
//	}
//	else
//	{
//		HAL_GPIO_WritePin(slow_sole1_GPIO_Port,slow_sole1_Pin, drv);
//	}
//}

//void drive_slow_sole(GPIO_TypeDef *valve_port, drive drv)
//{
//	if (self->settings_stream1[0].pump_type_ == LAFENG)
//	{
////		HAL_GPIO_WritePin(slow_sole1_GPIO_Port,slow_sole1_Pin, !drv);
//		HAL_GPIO_WritePin(valve_port, slow_sole1_Pin, !drv);
//	}
//	else
//	{
////		HAL_GPIO_WritePin(slow_sole1_GPIO_Port,slow_sole1_Pin, drv);
//		HAL_GPIO_WritePin(slow_sole1_GPIO_Port,slow_sole1_Pin, drv);
//	}
//}


void drive_slow_sole(Nozzle *self, drive drv)
{
	if (self->settings_stream1[0].pump_type_ == LAFENG)
	{
		GPIO_TypeDef* port = (self->nozzle_id == 0)? slow_sole1_GPIO_Port : slow_sole2_GPIO_Port;
		uint16_t pin = (self->nozzle_id == 0)? slow_sole1_Pin : slow_sole2_Pin;
		HAL_GPIO_WritePin(port, pin, !drv);
	}
	else
	{
		GPIO_TypeDef* port = (self->nozzle_id == 0)? slow_sole1_GPIO_Port : slow_sole2_GPIO_Port;
		uint16_t pin = (self->nozzle_id == 0)? slow_sole1_Pin : slow_sole2_Pin;
		HAL_GPIO_WritePin(port, pin, drv);
	}
}

void drive_fast_sole(Nozzle *self, drive drv)
{
	if (self->settings_stream1[0].pump_type_ == LAFENG)
	{
		GPIO_TypeDef* valve_port = (self->nozzle_id == 0)? fast_sole1_GPIO_Port : fast_sole2_GPIO_Port;
		uint16_t valve_pin = (self->nozzle_id == 0)? fast_sole1_Pin : fast_sole2_Pin;
		HAL_GPIO_WritePin(valve_port,valve_pin, !drv);
	}
	else
	{
		GPIO_TypeDef* valve_port = (self->nozzle_id == 0)? fast_sole1_GPIO_Port : fast_sole2_GPIO_Port;
		uint16_t valve_pin = (self->nozzle_id == 0)? fast_sole1_Pin : fast_sole2_Pin;
		HAL_GPIO_WritePin(valve_port, valve_pin, drv);
	}
}

//void drive_fast_sole1(drive drv)
//{
//	if (self->settings_stream1[0].pump_type_ == LAFENG)
//	{
//		HAL_GPIO_WritePin(fast_sole1_GPIO_Port,fast_sole1_Pin, !drv);
//	}
//	else
//	{
//		HAL_GPIO_WritePin(fast_sole1_GPIO_Port,fast_sole1_Pin, drv);
//	}
//}
//void set_valve(uint8_t nozzle_id, GPIO_PinState state) {
//    GPIO_TypeDef* port = (nozzle_id == 0) ? VALVE0_GPIO_Port : VALVE1_GPIO_Port;
//    uint16_t pin = (nozzle_id == 0) ? VALVE0_Pin : VALVE1_Pin;
//    HAL_GPIO_WritePin(port, pin, state);
//}
//
//void emergency_stop(uint8_t nozzle_id) {
//    Nozzle *n = &disp->digital_reading.nozzles[nozzle_id];
//    set_valve(nozzle_id, GPIO_PIN_RESET);
//    n->state = STATE_IDLE;
//    HAL_TIM_Base_Stop_IT(disp.flow_tim);
//    LCD_ShowAlert("EMERGENCY STOP");
//}

//---------------------------------------------------------------
void drive_slow_sole2(drive drv)
{
	if (self->settings_stream1[1].pump_type_ == LAFENG)
	{
		HAL_GPIO_WritePin(slow_sole2_GPIO_Port,slow_sole2_Pin, !drv);
	}
	else
	{
		HAL_GPIO_WritePin(slow_sole2_GPIO_Port,slow_sole2_Pin, drv);
	}
}
//---------------------------------------------------------------
//void drive_fast_sole1(drive drv)
//{
//	if (self->settings_stream1[0].pump_type_ == LAFENG)
//	{
//		HAL_GPIO_WritePin(fast_sole1_GPIO_Port,fast_sole1_Pin, !drv);
//	}
//	else
//	{
//		HAL_GPIO_WritePin(fast_sole1_GPIO_Port,fast_sole1_Pin, drv);
//	}
//}
//---------------------------------------------------------------
void drive_fast_sole2(drive drv)
{
	if (self->settings_stream1[1].pump_type_ == LAFENG)
	{
		HAL_GPIO_WritePin(fast_sole2_GPIO_Port,fast_sole2_Pin, !drv);
	}
	else
	{
		HAL_GPIO_WritePin(fast_sole2_GPIO_Port, fast_sole2_Pin, drv);
	}
}
//---------------------------------------------------------------

 void clear_screen(Nozzle *self)
  {
     for(uint8_t i = 0; i < 22; i++)
     {
       self->vbuf[i] = 0;     //initialise buffer with spaces
  	   shiftOut(&self, 0, 1);
     }
  }
//---------------------------------------------
 void clear_screen2()
  {
     for(uint8_t i = 0; i < 22; i++)
     {
       vbuf2[i] = 0;     			//initialise buffer with spaces
  	   shiftOut2(0, 1);
     }
  }
//---------------------------------------------
    void _Delay(int v)
    {
  	  long tmp = 0;
  	  while(tmp++ < (v*2) ){}
    }
//---------------------------------------------
	void _Delay2(int v)
	{
	  long tmp = 0;
	  while(tmp++ < (v*2) ){}
	}
//-------------------------------------------------------
/**
 * @param[in]  b   byte value to get the bit of its index
 * @param[in]  i   bit index to check for
 */
bool byteRead(uint8_t b, uint8_t i)
{
	return (b & (0x01<<i));
}

//-------------------------------------------------------
/**
 * @param[in]  b   byte value to get the bit of its index
 * @param[in]  i   bit index to check for
 */
bool byteRead2(uint8_t b, uint8_t i)
{
	return (b & (0x01<<i));
}


//========================================================================================

void shiftOut(Nozzle *self, uint8_t data_byte, uint8_t lat)
{
	GPIO_TypeDef *latch_port, *clock_port, *data_port;
	uint16_t latch_pin, clock_pin, data_pin;

	latch_port = (self->nozzle_id == 0)? latchPin_GPIO_Port : latchPin2_GPIO_Port;
	clock_port = (self->nozzle_id == 0)? clockPin_GPIO_Port : clockPin2_GPIO_Port;
	data_port = (self->nozzle_id == 0)? dataPin_GPIO_Port : dataPin2_GPIO_Port;

	latch_pin = (self->nozzle_id == 0)? latchPin_Pin : latchPin2_Pin;
	clock_pin = (self->nozzle_id == 0)? clockPin_Pin : clockPin2_Pin;
	data_pin = (self->nozzle_id == 0)? dataPin_Pin : dataPin2_Pin;

	//Latch pin low
	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET);

	for (uint8_t i = 0; i < 8; i++)
	{
		//clock pin low

		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
		// delay 1 ms
		_Delay(1);
		// check first bit, second bit ...
		//if high set dp high
//      if(disp_type1 == BLSKY886_N )
//      if( (disp_type1 == DN_BLSKY18K ) || (disp_type1 == DN_BLSKY22 ) )
		if(self->settings_stream1[0].display__ == BLSKY886_N)
		{
			if ( byteRead(data_byte, i) == 0 )
				HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);
			else	//if low set dp low
				HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_RESET);

			//set clock pin high
			HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
		}
//      else if( (disp_type1 == DIN_BLSKY18K ) || (disp_type1 == DIN_BLSKY22 ) )
      else if(self->settings_stream1[0].display__ == BLSKY886_IN)
	  {
			if ( byteRead(data_byte, i) == 0 )
				HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_RESET);
			else	//if low set dp low
				HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);

			//set clock pin high
			HAL_GPIO_WritePin(clock_Port, clock_Pin, GPIO_PIN_SET);
	  }

//      else if(disp_type1 == LAFNG885 )
//      else if(disp_type1 == DN_LAFNG17K)
         else if(self->settings_stream1[0].display__ == LAFNG885)
         {
   			if ( byteRead(data_byte, i) == 1 )
   				HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);
   			else	//if low set dp low
   				HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_RESET);

   			//set clock pin high
   			HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
         }
		//delay 1 ms
		_Delay(1);
	}

  if(lat == 1)
  {
   	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_SET);  //latch pin high
   	_Delay(1);
    HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET);  //Latch pin low
  }
}
//=========================================================================================

//=========================================================================================
void shiftOut2(uint8_t data_byte,uint8_t lat)
{
	//Latch pin low
	HAL_GPIO_WritePin(latchPin2_GPIO_Port, latchPin2_Pin, GPIO_PIN_RESET);

	for (int i = 0; i < 8; i++)
	{
		//clock pin low
		HAL_GPIO_WritePin(clockPin2_GPIO_Port, clockPin2_Pin, GPIO_PIN_RESET);
		// delay 1 ms
		_Delay2(1);
		// check first bit, second bit ...
		//if high set dp high
//      if(disp_type2 == BLSKY886_N)
//      if( (disp_type2 == DN_BLSKY18K) || (disp_type2 == DN_BLSKY22) )
	  if(self->settings_stream1[1].display__ == BLSKY886_N)
      {
			if ( byteRead2(data_byte, i) == 0 )
				HAL_GPIO_WritePin(dataPin2_GPIO_Port, dataPin2_Pin, GPIO_PIN_SET);
			else	//if low set dp low
				HAL_GPIO_WritePin(dataPin2_GPIO_Port, dataPin2_Pin, GPIO_PIN_RESET);

			//set clock pin high
			HAL_GPIO_WritePin(clockPin2_GPIO_Port, clockPin2_Pin, GPIO_PIN_SET);
      }
//      else if( (disp_type2 == DIN_BLSKY18K ) || (disp_type2 == DIN_BLSKY22 ) )
	  else if(self->settings_stream1[1].display__ == BLSKY886_IN)
	  {
			if ( byteRead(data_byte, i) == 0 )

				HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_RESET);
			else	//if low set dp low
				HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);

			//set clock pin high
			HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
	  }

//      else if(disp_type2 == LAFNG885 )
//    	 else if(disp_type2 == DN_LAFNG17K)
	  	 else if(self->settings_stream1[1].display__ == LAFNG885)
         {
   			if ( byteRead2(data_byte, i) == 1 )
   				HAL_GPIO_WritePin(dataPin2_GPIO_Port, dataPin2_Pin, GPIO_PIN_SET);
   			else	//if low set dp low
   				HAL_GPIO_WritePin(dataPin2_GPIO_Port, dataPin2_Pin, GPIO_PIN_RESET);

   			//set clock pin high
   			HAL_GPIO_WritePin(clockPin2_GPIO_Port, clockPin2_Pin, GPIO_PIN_SET);
         }
		//delay 1 ms
		_Delay2(1);
	}

  if(lat == 1)
  {
   	HAL_GPIO_WritePin(latchPin2_GPIO_Port, latchPin2_Pin, GPIO_PIN_SET);  //latch pin high
   	_Delay(1);
    HAL_GPIO_WritePin(latchPin2_GPIO_Port, latchPin2_Pin, GPIO_PIN_RESET);  //Latch pin low
  }
}

//=======================================================================================
//=======================================================================================

uint8_t ToBits(int Input)
{
	switch (Input)
	{
		case 32: return 0;
		case 48: return 0b11111100;
		case 49: return 0b01100000;
		case 50: return 0b11011010;
		case 51: return 0b11110010;
		case 52: return 0b01100110;
		case 53: return 0b10110110;
		case 54: return 0b10111110;
		case 55: return 0b11100000;
		case 56: return 0b11111110;
		case 57: return 0b11110110;
		//case 'c': return 0b00000010;
		//case 's': return 0b00000000;
		 case  'A': return(0b11101110);
		 case  'a': return(0b11111010);
	   	 case  'B': return(0b00111110);
//	   	 case  'B': return 0b11111110;
	   	 case  'b': return(0b00111110);
	 	 case  'C': return(0b10011100);
	 	 case  'c': return(0b00011010);
	 	 case  'D': return(0b01111010);
	 	 case  'd': return(0b01111010);
		 case  'E': return(0b10011110);
		 case  'e': return(0b11011110);
		 case  'F': return(0b10001110);
		 case  'f': return(0b10001110);
		 case  'G': return 0b11110110;
		 case  'g': return 0b11110110;
		 case  'H': return 0b01101110;
		 case  'h': return 0b00101110;
		 case  'I': return(0b00001100);
		 case  'i': return(0b10011000);
		 case  'J': return(0b01110000);
		 case  'j': return(0b01110000);
		 case  'K': return(0b01001110);
		 case  'k': return(0b01001110);
		 case  'L': return(0b00011100);
		 case  'l': return(0b00011100);
		 case  'm': return(0b10101010);
         case  'N': return(0b11101100);
         case  'n': return(0b00101010);
         case  'O': return(0b11111100);
         case  'o': return(0b00111010);
//         case  'o': return(0b11111100);
         case  'P': return(0b11001110);
         case  'p': return(0b11001110);
         case  'R': return(0b10001100);
         case  'r': return(0b00001010);
         case  'S': return 0b10110110;
         case  's': return 0b10110110;
         case  'T': return(0b00011110);
//         case  'T': return(0b10001100);
         case  't': return(0b00011110);
	     case  'U': return(0b01111100);
	     case  'u': return(0b00111000);
	     case  'V': return(0b01111100);
	     case  'v': return(0b00111000);
	     case  'y': return 0b01100110;
	     case  'Y': return(0b01110110);
	     case  'Z': return(0b11011010);
	     case  'z': return(0b11011010);
	     case  '=': return(0b00010010);
	     case  ':': return(0b00010010);
//	     case  ':': return(0b10010010);
//	     case  ':': return(0b10010000);
	     case  '[': return(0b10011100);
	     case  ']': return(0b11110000);
	     case  '.': return(0b00000001);
	     case  '-': return(0b00000010);
	     case  '_': return(0b00010000);
	     case  '?': return(0b11001010);
	     default: return 0;   //0b11111100;
	}

}

uint8_t ToInt(char c)
{
	return (0xff & c);
}

//===========================================================================================
int32_t Multiplex(Nozzle *self, int8_t row, int8_t col) //char *Data_in,
{
     uint8_t intToByte = 0,
    		 data_l = 22;

    //----------------------------------------
	for (uint8_t i = 0; i < data_l; i++)
	{
    	intToByte =  self->vbuf[i];   //ToBits( ToInt(Data_in[i]) );
    	shiftOut(&self, intToByte, 0);
	}

  HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_SET);  //latch pin high
   _Delay(1);
  HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET);  //Latch pin low

  return 0;
}
//============================================================================================
int32_t Multiplex2( int row, int col)
{
     uint8_t intToByte = 0;
	 uint8_t   data_l = 22;

    //----------------------------------------
	for (int i = 0; i < data_l; i++)
	{
    	intToByte =  vbuf2[i];   //ToBits( ToInt(Data_in[i]) );
    	shiftOut2(intToByte,0);
	}

  HAL_GPIO_WritePin(latchPin2_GPIO_Port, latchPin2_Pin, GPIO_PIN_SET);  //latch pin high
   _Delay2(1);
  HAL_GPIO_WritePin(latchPin2_GPIO_Port, latchPin2_Pin, GPIO_PIN_RESET);  //Latch pin low

  return 0;
}
//=============================================================================================
void process(Nozzle *self, int8_t isflt, char* str, int8_t cnv, int8_t row, int8_t col, int8_t n, int8_t justify, int8_t clear)
{
   //	char str_[10]= {0};
   //	int cnv = snprintf(str_, sizeof(str), "%ld", num);

	int8_t temp1, temp2 = 0;
	int8_t count = 0;

	int8_t start;
	int8_t xtercount_max;
//-----------------------------
//	int clear = 0;
//	int justify = 1;
//	int n = 4;

	GPIO_TypeDef *latch_port;
	uint16_t latch_pin;

	latch_port = (self->nozzle_id == 0)? latchPin_GPIO_Port : latchPin2_GPIO_Port;
	latch_pin = (self->nozzle_id == 0)? latchPin_Pin : latchPin2_Pin;

//-----------------------------
 if (row > 3) return;
	if(row < 3)
	{
		xtercount_max = 8;
		start = 21 - ( (row-1)*8);
	}
	else
	{
		xtercount_max = 6;
		start = 5;

		if(strchr(str, '.'))
		{
			start = 4;
		}
	}
	count = 0; //xtercount_max;

    int diff = xtercount_max - cnv;

    //--------------------------------------------
            temp2 = start;
            temp1 = diff;
    //--------------------------------------------

	  while( (temp1 > 0) && (justify == 0) )
	  {
		if(clear == 1) {  self->vbuf[temp2] = 0; }
		  temp1--;
		  temp2--;
		  count++; if (count > xtercount_max+1) { goto end; } // keep track of the xter.
		 if (isflt == 1) self->vbuf[temp2-1] = 0;
	  }

          if(justify == 1)
          {
        	  // ----------calculate X coordinate here--------------
        	  int8_t x_ =  col;
        	  if (x_ > xtercount_max) x_ = xtercount_max;  //limit..
			  //-----------   write initial spaces -----------------
			   while(x_ > 0)
			   {
				 if (clear == 1 ) self->vbuf[temp2] = 0;
				   x_--;
				   temp2--;
				   count++; if (count > xtercount_max-1) { goto end; }
			   }
          }
    //========================================================
          temp1 = cnv;
          if(isflt == 1) temp2--;   // fixes the starting point of floats

          int8_t dp_comingLast = 0;

          while(temp1 > 0)
		  {
        	  int8_t tmp;

        	 if(dp_comingLast == 1)
        	 {
        		 tmp = '0';
        		 dp_comingLast = 0;
        	 }
        	 else
        	 {
        		 tmp = str[cnv - temp1];    // ToBits( ToInt(Data_in[i]) );
        	 }
             if(tmp == '.')                 // search  for decimal point.
             {                            //
            	   temp1--;                 // skip to the next xter.
            	   if(temp1 <= 0)
            	   {
            		   temp1++;
            		   dp_comingLast = 1;

            	   }
            	   temp2++;                 // reverse the self->vbuf index.
            	   int8_t buf_ =  self->vbuf[temp2]; // retrieve the pattern on the previous self->vbuf index and add 1 (dp)
            	   self->vbuf[temp2] = buf_+ 1;   // add decimal point and rewrite prevous digit scan code.;
            	   temp2--;                 // reference next index.
            	   continue;
              }
			   self->vbuf[temp2] = ToBits( ToInt( tmp ) );

			   temp2--;
			   temp1--;
			   count++;
			   if(count > xtercount_max-1)
			   {
				   goto end;
			   }
		   }
         //---------------------------------------------------------------------------
         //clear the extra character   ---- when in left justify and clear = 0
          if ( (justify == 1) && (clear == 0) && (n>0)  )
          {
        	  int8_t n_count = n - cnv;
        	  while(n_count > 0){
            	 n_count--;
            	 self->vbuf[temp2] = 0; temp2--;
            	 count++;  if(count > xtercount_max-1) { goto end; }
             }
          }
          ///////////////////////////////////////////

          if (  (justify == 1) && (clear == 1)  )
			  {
        	     temp1 = diff;
					 while(temp1 > 0)
					   {
				         self->vbuf[temp2] = 0;
				    	   temp1--;
						   temp2--;
						   count++; if (count > xtercount_max-1) { goto end; }
					   }
			  }

end:
     Multiplex(&self, row, col);   return;   //self->vbuf,

	//Multiplex("101213", row, col); return;

						HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_SET);  //latch pin high
						_Delay(1);
						HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET);  //Latch pin low

}
//============================================================================================================

void process2(int isflt, char* str, int8_t cnv, int row, int col, int n, int justify, int clear)
{
   //	char str_[10]= {0};
   //	int cnv = snprintf(str_, sizeof(str), "%ld", num);

	int temp1,temp2 = 0;
	int count = 0;

	int start;
	int xtercount_max;
//-----------------------------
//	int clear = 0;
//	int justify = 1;
//	int n = 4;
//-----------------------------
 if (row > 3) return;
	if(row < 3)
	{
		xtercount_max = 8;
		start = 21 - ( (row-1)*8);
	}
	else
	{
		xtercount_max = 6;
		start = 5;

		if(strchr(str, '.'))
		{
			start = 4;
		}
	}
	count = 0; //xtercount_max;

    int diff = xtercount_max - cnv;

    //--------------------------------------------
            temp2 = start;
            temp1 = diff;
    //--------------------------------------------

	  while( (temp1 > 0) && (justify == RT) )
	  {
		if(clear == 1) {  vbuf2[temp2] = 0; }
		  temp1--;
		  temp2--;
		  count++; if (count > xtercount_max+1) { goto end; } // keep track of the xter.
		 if (isflt == 1) vbuf2[temp2-1] = 0;
	  }

          if(justify == 1) {
          // ----------calculate X coordinate here--------------
          int x_ =  col;
          if (x_ > xtercount_max) x_ = xtercount_max;  //limit..
          //-----------   write initial spaces -----------------
		   while(x_ > 0)
			   {
				 if (clear == 1 ) vbuf2[temp2] = 0;
				   x_--;
				   temp2--;
				   count++; if (count > xtercount_max-1) { goto end; }
			   }
          }
    //==========================================================================
          temp1 = cnv;
          if(isflt == 1) temp2--;   // fixes the starting point of floats

          int8_t dp_comingLast = 0;

		  while(temp1 > 0)
		  {
			 int tmp;

			 if(dp_comingLast == 1)
			 {
				 tmp = '0';
				 dp_comingLast = 0;
			 }
			 else
			 {
				 tmp = str[cnv - temp1];    // ToBits( ToInt(Data_in[i]) );
			 }
			  if(tmp == '.')                 // search  for decimal point.
			  {                            //
				   temp1--;                 // skip to the next xter.
				   if(temp1 <= 0)
				   {
					   temp1++;
					   dp_comingLast = 1;

				   }
				   temp2++;                 // reverse the self->vbuf index.
				   int buf_ =  vbuf2[temp2]; // retrieve the pattern on the previous self->vbuf index and add 1 (dp)
				   vbuf2[temp2] = buf_+ 1;   // add decimal point and rewrite prevous digit scan code.;
				   temp2--;                 // reference next index.
				   continue;

//				   while(temp1 > 0)
//		   {
//        	 int tmp = str[cnv - temp1];     // ToBits( ToInt(Data_in[i]) );
//             if(tmp == '.')                  // search  for decimal point.
//               {                             //
//            	   temp1--;                  // skip to the next xter.
//            	   temp2++;                  // reverse the self->vbuf index.
//              	   int buf_ =  vbuf2[temp2]; // retrieve the pattern on the previous self->vbuf index and add 1 (dp)
//            	   vbuf2[temp2] = buf_+ 1;    // add decimal point and rewrite prevous digit scan code.;
//            	   temp2--;                  // reference next index.
//            	   continue;
               }
			   vbuf2[temp2] =  ToBits( ToInt( tmp ) );

			   temp2--;
			   temp1--;
			   count++;  if(count > xtercount_max-1) { goto end; }
		   }
         //---------------------------------------------------------------------------
         //clear the extra character   ---- when in left justify and clear = 0
          if ( (justify == 1) && (clear == 0)&&(n>0)  )
          {
        	 int n_count = n - cnv;
             while(n_count > 0){
            	 n_count--;
            	 vbuf2[temp2] = 0; temp2--;
            	 count++;  if(count > xtercount_max-1) { goto end; }
             }
          }
          ///////////////////////////////////////////

          if (  (justify == 1) && (clear == 1)  )
			  {
        	     temp1 = diff;
					 while(temp1 > 0)
					   {
				         vbuf2[temp2] = 0;
				    	   temp1--;
						   temp2--;
						   count++; if (count > xtercount_max-1) { goto end; }
					   }
			  }

end:
     Multiplex2(row, col);   return;   //self->vbuf,

	//Multiplex("101213", row, col); return;

	HAL_GPIO_WritePin(latchPin2_GPIO_Port, latchPin2_Pin, GPIO_PIN_SET);  //latch pin high
	_Delay(1);
	HAL_GPIO_WritePin(latchPin2_GPIO_Port, latchPin2_Pin, GPIO_PIN_RESET);  //Latch pin low

}
//==============================================================================================
/* routines to write integer , float and string to the lcd  */
void printDisp_i(Nozzle *self, int32_t num, int row, int col, int n, justify_ jst, clear_ cl)
{
	  	char str_[10]= {0};
	   	int cnv = snprintf(str_, sizeof(str_), "%ld", num);
	   	process(&self, 0, str_, cnv, row, col, n, jst, cl);
}

void printDisp_c(Nozzle *self, char *num, uint8_t row, uint8_t col, int8_t n, justify_ jst, clear_ cl)
{
	//char str_[10]= {0};
	//sprintf(str_, "%sc", num);
	//int cnv = snprintf(str_, sizeof(str), "%ld", num);
	int8_t max_ = 8;
	if(row < 3) max_ = 10;
	int8_t cnv = strlen(num);
	if(cnv > max_) cnv = max_;
 	process(&self, 0, num, cnv, row, col, n, 1, cl);
}

void printDisp_f(Nozzle *self, float num, uint8_t row, uint8_t col, int8_t n, justify_ jst, clear_ cl)
{
	char str_[10]= {0};
	int8_t cnv = snprintf(str_, sizeof(str_), "%.2f", num);

     //	int clear = 1;

        //justify for float is always left because of the decimals
	    //int justify = 0;
	    //int n = 4;
	process(&self, 1, str_, cnv, row, col, n, 0, cl);
}
//================================================================================================
/* routines to write integer , float and string to the lcd  */
void printDisp_i2(int32_t num, int row, int col,int n,justify_ jst,clear_ cl)
{
	  	char str_[10]= {0};
	   	int cnv = snprintf(str_, sizeof(str_), "%ld", num);
	   	process2(0,str_,cnv,row,col,n,jst,cl);
}

void printDisp_c2(char *num, int row, int col,int n,justify_ jst,clear_ cl )
{
	//char str_[10]= {0};
	//sprintf(str_, "%sc", num);
	//int cnv = snprintf(str_, sizeof(str), "%ld", num);
	int max_ = 8;
	if(row < 3) max_ = 10;
	int cnv = strlen(num);
	if(cnv > max_) cnv = max_;
 	process2(0,num,cnv,row,col,n,1,cl);
}

void printDisp_f2(float num, int row, int col,int n ,justify_ jst , clear_ cl)
{
	char str_[10]= {0};
	int cnv = snprintf(str_, sizeof(str_), "%.2f", num);

     //	int clear = 1;

        //justify for float is always left because of the decimals
	    //int justify = 0;
	    //int n = 4;
	process2(1,str_,cnv,row,col,n,0,cl);
}
//===============================================================================================
