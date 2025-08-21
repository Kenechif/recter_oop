#include "stm32f4xx_hal.h"
#include "keypad_module.h"
#include "myLibraries.h"     // for the delay function
#include "settings.h"
#include "main.h"

int disp_len_pad = 6;
int lat_cnt = 0;
static int key = 0;
//*****************************************************************
static char buf[9] = {0};  //keypad LCD display graphics array.
//*****************************************************************
// extern int8_t pump_type;
 extern pump pump_type;

// extern pump_settings settings[2];

 extern pump_settings_stream1 self->settings_stream1[2],
 					 	 	 settings_original_stream1[2];

 extern pump_settings_stream2 self->settings_stream2[2],
 							 settings_original_stream2[2];

 extern pump_settings_stream3 settings_stream3[2],
 				       	     settings_original_stream3[2],
 							 copy_stream3[2];

 extern char keyboard_entry[10];

///////////////////////////////////////////////////////////////////

 int disp_len_pad2 = 6;
 int lat_cnt2 = 0;
 static int key2 = 0;
 //*****************************************************************
 static char buf2[9] = {0};  //keypad LCD display graphics array.
 //*****************************************************************
  extern int pump_type2;

  extern int8_t self->dpFlag,
  	  	  	  	self->dpCount;


//=================================================================
void shift_(uint8_t data_byte,uint8_t lat);
void shift(uint8_t data_byte,uint8_t lat);

void shift_2(uint8_t data_byte,uint8_t lat);
void shift2(uint8_t data_byte,uint8_t lat);

void keypad_ini(Nozzle *self)
{
	GPIO_TypeDef *clock_port, *data_port;
	uint16_t clock_pin, data_pin;

	clock_port = (self->nozzle_id == 0)? clock_port : clkk2_GPIO_Port;
	data_port = (self->nozzle_id == 0)? data_port : datak2_GPIO_Port;

	clock_pin = (self->nozzle_id == 0)? clkk1_Pin : clkk2_Pin;
	data_pin = (self->nozzle_id == 0)? data_pin : datak2_Pin;

	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);

	for(uint8_t i = 0; i < 21; i++)
	{
		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
		_Delay(1);
		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
		_Delay(1);
	}
}

void keypad_ini2(void)
{
	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
	for(int i = 0; i < 21; i++)
	{
			HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
			_Delay(1);
			HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
			_Delay(1);
	}
}


/*
 *
 */
uint8_t checkkey(Nozzle *self)
{
	GPIO_TypeDef *port;
	uint16_t pin;

	port = (self->nozzle_id == 0)? com12_port : com12k2_GPIO_Port;
	pin = (self->nozzle_id == 0)? com12_pin : com12k2_Pin;

	return(  (HAL_GPIO_ReadPin(port, pin ) == 1)? 0 : 1  );   // return 1 if keypressed
}

uint8_t checkkey2(void)
{
	 return(  (HAL_GPIO_ReadPin(com12k2_GPIO_Port, com12k2_Pin ) == 1)? 0:1  );   // return 1 if keypressed
}

/**
 * this routine scans the entire keys while monitoring the common PIN for a LOGIC LOW (denoting key press)
 * it then returns the SCAN number.
 *
 * if no key pressed 0 is returned.
 *
 *  * @param[out]  int returns the keyscan number or zero if nokey pressed
 */
uint8_t scankey(FuelDispenser *disp, Nozzle *self)
{
	uint8_t count = 0,
			keynumber = 0;

	GPIO_TypeDef *clock_port, *data_port;
	uint16_t clock_pin, data_pin;

	clock_port = (self->nozzle_id == 0)? clock_port : clkk2_GPIO_Port;
	data_port = (self->nozzle_id == 0)? data_port : datak2_GPIO_Port;

	clock_pin = (self->nozzle_id == 0)? clkk1_Pin : clkk2_Pin;
	data_pin = (self->nozzle_id == 0)? data_pin : datak2_Pin;

	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_RESET);
	_Delay(1);
	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);        //shift in first bit.
	_Delay(1);
	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
	count++;

	if( checkkey(&self) == 1) {
	  keynumber = 1;
	  goto keyfound;    //return i;
	}
 //-----------------------------------------------------------------------
   HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);

	for(uint8_t i = 2; i < 22; i++)
	{
		_Delay(1);
		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);             //shift the rest 20 bits.
		_Delay(1);
		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
		count++;
		if( checkkey() == 1)
		{
		  keynumber = i;

		  if(self->settings_stream2[0].keypress_tone == Yes)
		  {
			  HAL_GPIO_WritePin(disp->buzzer_GPIO_Port, disp->buzzer_Pin, GPIO_PIN_SET);
			  _Delay(100);
		  }

		  goto keyfound;    //return i;
	    }
	}
      goto endd; //return 0;     // return with 0 if no key pressed.

 keyfound:   _Delay(0);
//	        int temp = keynumber;
              while(count < 22)
              {
            	 // temp++;
            	   HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);  //shift out the rest of the bits.
            		_Delay(1);
            	   HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
            	   count++;
              }

	  endd:    // shift out the last bit...
	            _Delay(1);
				HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);  //shift the rest 20 bits.
				_Delay(1);
				HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);

	  HAL_GPIO_WritePin(disp->buzzer_GPIO_Port, disp->buzzer_Pin, GPIO_PIN_RESET);
	  return keynumber;
}

//============================================================================================

int scankey2(void)
{
	int count2 = 0;
	int keynumber2 = 0;
	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_RESET);
	_Delay(1);
	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);        //shift in first bit.
	_Delay(1);
	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
	count2++;
	  if( checkkey2() == 1)
	  {
		  keynumber2 = 1;
		  goto keyfound;    //return i;
	  }
 //-----------------------------------------------------------------------
   HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);

	for(int i = 2; i < 22; i++)
	{
		    _Delay2(1);
			HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);   //shift the rest 20 bits.
			_Delay2(1);
			HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
			count2++;
			  if( checkkey2() == 1)
			  {
					  keynumber2 = i;

					  if(self->settings_stream2[1].keypress_tone == Yes)
					  {
						  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
						  _Delay(100);
					  }

					  goto keyfound;    //return i;
			   }
	}
      goto endd; //return 0;     // return with 0 if no key pressed.

 keyfound:   _Delay2(0);
//	        int temp = keynumber;
              while(count2 < 22)
              {
            	 // temp++;
            	   HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);  //shift out the rest of the bits.
            		_Delay2(1);
            	   HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
            	   count2++;
              }

	  endd:    // shift out the last bit...
	            _Delay2(1);
				HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);  //shift the rest 20 bits.
				_Delay2(1);
				HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
	  return keynumber2;
}
//===============================================================================================

int readkeyboard(FuelDispenser *disp, Nozzle *self)
{
	uint8_t ky = scankey(&disp, &self);
	self->key  = ky;
	if (ky > 0)
	{

	}

	return ky;  //_Delay(1);
}

//===============================================================================================

int readkeyboard2(int type)
{
  //while(1)
  //{
	int ky2 = scankey2();
        key2  = ky2;
  	   if (ky2 > 0)
       {
          //  return ky;
          //  _Delay(1);
  	   }
 // }
	   return ky2;  //_Delay(1);
}


uint8_t To_pattern(int Input)
{
	switch (Input)
	{
		case 32: return 0;
		case 48: return 0b01111110;
		case 49: return 0b00110000;
		case 50: return 0b01101101;
		case 51: return 0b01111001;
		case 52: return 0b00110011;
		case 53: return 0b01011011;
		case 54: return 0b01011111;
		case 55: return 0b01110000;
		case 56: return 0b01111111;
		case 57: return 0b01111011;
		//case 'c': return 0b00000010;
		//case 's': return 0b00000000;
		 case  'a': return(0b01110111);
	   	 case  'b': return(0b00011111);
	 	 case  'c': return(0b01001110);
	 	 case  'd': return(0b00111101);
		 case  'e': return(0b01001111);
		 case  'f': return(0b01000111);
		 case  'g': return 0b01111011;
		 case  'h': return 0b00010111;
		 case  'i': return(0b00000110);
		 case  'j': return(0b00111000);
		 case  'l': return(0b00001110);
		 case  'L': return(0b00001110);
         case  'n': return(0b01110110);
         case  'o': return 0b01111110;
         case  'p': return(0b01100111);
         case  'P': return(0b01100111);
         case  'r': return(0b01000110);
         case  's': return 0b01011011;
         case  't': return(0b00001111);
	     case  'u': return(0b00111110);
	     case  'v': return(0b00111110);
	     case  'y': return(0b00110011);
	     case  '.': return(0b10000000);
	     case  '-': return(0b00000001);
	     default: return 0;   //0b11111100;
	}
}

/**
 *  This routine writes to the KEYPAD LCD.
 *  * @param[in]  num  int contains the numeric value to be displayed on the KEYPAD LCD
 */
uint8_t write_keypad_lcd(Nozzle *self, uint8_t fxn, char* num)
{
	extern uint8_t self->keyEntry_len;

	GPIO_TypeDef *latch_port, *clock_port, *data_port, *com12_port;
	uint16_t latch_pin, clock_pin, data_pin, com12_pin;

	latch_port = (self->nozzle_id == 0)? lthk1_GPIO_Port : lthk2_GPIO_Port;
	clock_port = (self->nozzle_id == 0)? clkk1_GPIO_Port : clkk2_GPIO_Port;
	data_port = (self->nozzle_id == 0)? datak1_GPIO_Port : datak2_GPIO_Port;
	com12_port = (self->nozzle_id == 0)? com12k1_GPIO_Port : com12k2_GPIO_Port;

	latch_pin = (self->nozzle_id == 0)? lthk1_Pin : lthk2_Pin;
	clock_pin = (self->nozzle_id == 0)? clkk1_Pin : clkk2_Pin;
	data_pin = (self->nozzle_id == 0)? datak1_Pin : datak2_Pin;
	com12_pin = (self->nozzle_id == 0)? com12k1_Pin : com12k2_Pin;

	int8_t count = 0,
		   decimalPoint_flag = 0,
		   postn,
		   scan_code = 0;

	self->dpFlag = 0;

	int8_t cnv = strlen(num);      //snprintf(buf2, sz, "%ld", num);

	count = cnv;

	keypad_ini(&self);

	// prefresh the lcd first.
	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);

	if(fxn == 1)
		goto refresh;

	if(self->dpFlag == 0)
	{
		postn = 6;
		if(strchr(num, '.') ) --count;
		while(count<7)
		{
			self->buf[postn] = 0;           //prefill leading spaces with spaces (zeros)
			count++;
			postn--;
		}
		count = postn;

		while (postn > -1) //for(int i = cnv; i<8; i++)   //fill the rest of the buffer with the code pattern.
		{
			if(num[count-postn] == '.' )
			{
				scan_code = To_pattern( ToInt( num[(count-postn) + 1] ) );
				self->buf[postn] = scan_code + 0X80;
				count++;
			}
			else
			{
					scan_code = To_pattern( ToInt( num[count-postn] ) );
					self->buf[postn] = scan_code;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];
			}
			postn--;
		}
	}
	else if (self->dpFlag == 1)
	{
		postn = 7;
		while(count <= 8)
		{
			self->buf[postn] = 0;           //prefill leading spaces with spaces (zeros)
			count++;
			postn--;
		}

		count = 6;

		while (postn >= -1) //for(int i = cnv; i<8; i++)   //fill the rest of the buffer with the code pattern.
		{

			if(postn == (self->dpCount - 1) )
			{

			}
			else if(postn == ((self->dpCount - 1) - 1) )
			{
				scan_code = To_pattern( ToInt( num[count - (postn + 1)] ) );
				self->buf[(postn + 1)] = scan_code + 0X80;
//				postn++;
			}

			else
			{
				scan_code = To_pattern( ToInt( num[count - (postn + 1)] ) );

				if (  ((self->keyEntry_len == 8) && (self->dpCount != 1) && ((postn == 1 ) || (postn == 0 ) || (postn == -1))) ||
					((self->keyEntry_len == 7) && ((postn == 0 ) || (postn == -1))) ||
					((self->keyEntry_len == 6) && (postn == -1))  || ((self->dpCount == 2) && (postn == -1)) ||
					((self->dpCount == 3) && ((postn == 0 ) || (postn == -1)))  ||
					((self->dpCount == 4) && ((postn == 1 ) || (postn == 0 ) || (postn == -1))) )
				{
					self->buf[postn + 1] = scan_code;
				}
				else
					self->buf[postn] = scan_code;
			}
			postn--;
		}
	}

	refresh:
	// move the buf[] array into the shift register.
	for(uint8_t i = 0; i < 7 ; i++)
	{
		shift(&self, self->buf[i], 0);
	}

	// push "0" into the keyboards memory first.
	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
	_Delay(1);
	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
	_Delay(1);
	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);


	//push in "1"s past the keyboard registers memory of 24-1 places
	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);

	for(uint8_t i = 0; i < 22; i++)
	{
		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
		_Delay(1);
		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
		_Delay(1);
	}

	//then latch the shifted data...

	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_SET);  //latch pin high

	if(lat_cnt == 0)
	{
	  HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET); //Latch pin low
	}
	lat_cnt++;
	if(lat_cnt == 2) lat_cnt = 0;

	//_Delay(10);

//	pushout the remaining zero from the keyboard registers.
	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
	_Delay(1);
		  HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
	_Delay(1);

	//==========================================================================
	   if(fxn == 2) return;     // return if fxn 2 selected (screen only..)
	//==========================================================================
	   //flush registers with 1
	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);

	 for(uint8_t pmp = 0; pmp < 25; pmp++)
	 {
			HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
		 _Delay(1);
			HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
		 _Delay(1);
	 }
	   //=======================================================================

	 uint8_t keynumber = 0;
	 uint8_t fill = 22;
	   // push "0" into the keyboards memory first.

	   	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_RESET);
	   	_Delay(1);
	   	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
	   	_Delay(1);
	   	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);

	   	//check keypress..
	      if (HAL_GPIO_ReadPin(com12_port, com12_pin ) == 0)
	   	  {
	   	        keynumber = 1;
	   	        return keynumber;
	   	  }

	    HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);

	    for(uint8_t i = 2; i < fill; i++)
	   	{
	   		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
	   		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
\
	   		if( checkkey(&self) == 1)
			{
				keynumber = i;
				return keynumber;
			}
	   	}

	   	return keynumber;

}


void shift_(Nozzle *self, uint8_t data_byte, uint8_t lat)
{
	GPIO_TypeDef *latch_port, *clock_port, *data_port;
	uint16_t latch_pin, clock_pin, data_pin;

	latch_port = (self->nozzle_id == 0)? lthk1_GPIO_Port : lthk2_GPIO_Port;
	clock_port = (self->nozzle_id == 0)? clkk1_GPIO_Port : clkk2_GPIO_Port;
	data_port = (self->nozzle_id == 0)? datak1_GPIO_Port : datak2_GPIO_Port;

	latch_pin = (self->nozzle_id == 0)? lthk1_Pin : lthk2_Pin;
	clock_pin = (self->nozzle_id == 0)? clkk1_Pin : clkk2_Pin;
	data_pin = (self->nozzle_id == 0)? datak1_Pin : datak2_Pin;

	//Latch pin low
	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET);

	for (uint8_t i = 0; i < 8; i++)
	{
		//clock pin low
		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
		// delay 1 ms
		_Delay(1);
		// check  bit, second bit ...
		//if high set out high
		if ( byteRead(data_byte, i) == 0 )
			HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);
		else	//if low set dp low
			HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_RESET);

		//set clock pin high
		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);
		//delay 1 ms
		_Delay(1);
	}



  if(lat == 1)
  {
	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_SET);      //latch pin high
   	_Delay(1);
   	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET);    //Latch pin low
  }
}

void shift(Nozzle *self, uint8_t data_byte, uint8_t lat)
{
	GPIO_TypeDef *latch_port, *clock_port, *data_port;
	uint16_t latch_pin, clock_pin, data_pin;

	latch_port = (self->nozzle_id == 0)? lthk1_GPIO_Port : lthk2_GPIO_Port;
	clock_port = (self->nozzle_id == 0)? clkk1_GPIO_Port : clkk2_GPIO_Port;
	data_port = (self->nozzle_id == 0)? datak1_GPIO_Port : datak2_GPIO_Port;

	latch_pin = (self->nozzle_id == 0)? lthk1_Pin : lthk2_Pin;
	clock_pin = (self->nozzle_id == 0)? clkk1_Pin : clkk2_Pin;
	data_pin = (self->nozzle_id == 0)? datak1_Pin : datak2_Pin;

	//Latch pin low
	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET);

	for (uint8_t i = 0; i < 8; i++)
	{
		//clock pin low
		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
		// delay 1 ms
		_Delay(1);
		// check  bit, second bit ...
		//if high set out high
		uint8_t bitt ;
		if(lat_cnt == 0)
			    bitt = 1;
		else
			    bitt = 0;

		if ( byteRead(data_byte, i) == bitt )
			HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);
		else	//if low set dp low
			HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_RESET);

		//set clock pin high
		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);
		//delay 1 ms
		_Delay(1);
	}

	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);

  if(lat == 1)
  {
	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_SET);      //latch pin high
   	_Delay(1);
   	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET);    //Latch pin low
  }
}

/**
 *  This routine writes to the KEYPAD LCD and also read the key press
 *  This routine writes to the KEYPAD LCD.
 *  * @param[in]  num  int contains the numeric value to be displayed on the KEYPAD LCD
 *  * @param[out] int  outputs 0 for no keypressed or greater than 0 for the keyposition.
 */
uint8_t keypad_lcd(FuelDispenser *disp, Nozzle *self, uint8_t fxn, char* num)
{
	GPIO_TypeDef *latch_port, *clock_port, *data_port;
	uint16_t latch_pin, clock_pin, data_pin;

	latch_port = (self->nozzle_id == 0)? lthk1_GPIO_Port : lthk2_GPIO_Port;
	clock_port = (self->nozzle_id == 0)? clkk1_GPIO_Port : clkk2_GPIO_Port;
	data_port = (self->nozzle_id == 0)? datak1_GPIO_Port : datak2_GPIO_Port;

	latch_pin = (self->nozzle_id == 0)? lthk1_Pin : lthk2_Pin;
	clock_pin = (self->nozzle_id == 0)? clkk1_Pin : clkk2_Pin;
	data_pin = (self->nozzle_id == 0)? datak1_Pin : datak2_Pin;

	if( (self->settings_stream1[0].keypad__ != LAFNG17_K) && (self->settings_stream1[0].keypad__ != LAFNG18_K) && (self->settings_stream1[0].keypad__ != LAFNG18_K_V2))
	{
		uint8_t rr =	write_keypad_lcd(&self, fxn, num);
	    return rr;
	}


	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET); //Latch pin low --------------------------------------------

	if(fxn == 1) goto refresh;

	uint8_t keynumber = 0;

	int8_t count = 0,
		   scan_code = 0,
		   inv = 0,
		   postn = 0;

	count = strlen(num);      //snprintf(buf2, sz, "%ld", num);

	postn = 6;

	if(strchr(num, '.') ) --count;

	inv = count;

	while(count < 7)
	{
		self->buf[postn] = 0;           //prefill leading spaces with spaces (zeros)
		count++;
		postn--;
	}
	count = postn;

	while (postn > -1) //for(int i = cnv; i<8; i++)   //fill the rest of the buffer with the code pattern.
	{
		if(num[count - postn] == '.')  //&& (num[cnv2 - 1] == '.') )
		{
//				buf2[postn2] = 0x7E + 0X80;
			scan_code = To_pattern( ToInt( num[(count - postn) + 1] ) );

			if( (self->settings_stream1[0].keypad__  == LAFNG18_K) || (self->settings_stream1[0].keypad__ == LAFNG18_K_V2))
			{
				scan_code += 0x80;
//				buf[postn] = (scan_code ^ 0xFF);
				self->buf[postn] = scan_code;
			}
			else
			{
				scan_code += 0x80;
				self->buf[postn] = scan_code;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];
			}

//			buf2[postn] = scan_code + 0X80;
			count++;
		}
		else
		{
				scan_code = To_pattern( ToInt( num[count - postn] ) );

				if( (self->settings_stream1[0].keypad__  == LAFNG18_K) || (self->settings_stream1[0].keypad__ == LAFNG18_K_V2) )
				{
//					self->buf[postn] = (scan_code ^ 0xFF);
					self->buf[postn] = scan_code;
				}
				else
				{
					self->buf[postn] = scan_code;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];
				}
		}
		postn--;
	}

	refresh:
 //===========================================================================
 // HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET); //Latch pin low
 // move the buf[] array into the shift register.

	if (self->settings_stream1[0].keypad__ == LAFNG17_K)
	{
	    	disp_len_pad = 5;
	}
    else if(self->settings_stream1[0].keypad__  == LAFNG18_K)
    {
    	disp_len_pad = 7;
    }
    else if(self->settings_stream1[0].keypad__ == LAFNG18_K_V2)
    {
    	disp_len_pad = 8;
    }

	for(uint8_t i = 0; i < disp_len_pad; i++)  //7 bytes for bluesky 5 bytes for lafeng
	{
		shift_(self->buf[i], 0);
	}

	//push in "1"s past the keyboard registers memory of 24-1 places
	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);

	uint8_t fill = 24;

	if ( (self->settings_stream1[0].keypad__ == LAFNG17_K) || (self->settings_stream1[0].keypad__ == LAFNG18_K) || (self->settings_stream1[0].keypad__ == LAFNG18_K_V2))
		fill = 16;

	for(uint8_t i = 1; i < fill; i++)
	{
		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
		//	_Delay(1);
		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
		//	_Delay(1);
	}
	//then latch the shifted data...
	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_SET);  //latch pin high
    //	_Delay(1);
	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET); //Latch pin low
    //	_Delay(1);
    //--------------------	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_SET);  //latch pin high

    HAL_GPIO_TogglePin(disp->refresh_GPIO_Port, disp->refresh_Pin);

//==========================================================================
        if(fxn == 2) return;     // return if fxn 2 selected (screen only..)
//==========================================================================

    HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);
    for(uint8_t i = 0; i < 5; i++)
   	{
   		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
   		//_Delay(1);
   		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
   		//_Delay(1);
   	}

//====================================z===================================
	keynumber = 0;
// push "0" into the keyboards memory first.

	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_RESET);
	//_Delay(1);
	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
	//_Delay(1);
	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);


	//check keypress..
   if( checkkey(&self) == 1)
   {
		keynumber = 1;

//		if(self->settings_stream2[0].keypress_tone == Yes)
//		{
//		  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
//		}
//
//		HAL_Delay(1);
//
//		HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);

		return keynumber;
   }

   fill = 22;
//   if(pump_type == lafeng) fill = 17;
//   if(pump_type == DN_LAFNG17K)
   if ( (self->settings_stream1[0].keypad__ == LAFNG17_K) || (self->settings_stream1[0].keypad__ == LAFNG18_K) || (self->settings_stream1[0].keypad__ == LAFNG18_K_V2))
	   fill = 17;

   HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);

   for(uint8_t i = 2; i < fill; i++)
	{
		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
		//_Delay(1);
		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
		//_Delay(1);
		if( checkkey(&self) == 1)
		{
			keynumber = i;

//			if(self->settings_stream2[0].keypress_tone == Yes)
//		    {
//			  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
//		    }
//
//			HAL_Delay(1);
//
//			HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
			return keynumber;
		}
	}

	//pushout the remaining zero from the keyboard registers.
//	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
//	_Delay(1);
//		  HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
//	_Delay(1);
	return keynumber;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//                        					  SIDE2
/////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  This routine writes to the KEYPAD LCD.
 *  * @param[in]  num  int contains the numeric value to be displayed on the KEYPAD LCD
 */
uint8_t write_keypad_lcd2(uint8_t fxn, char* num)
{
	extern uint8_t keyEntry2_len;
	int8_t count2 = 0,
		   scan_code2 = 0,
		   postn2;

	dpFlag2 = 0;

	int8_t cnv2 =   strlen(num);      //snprintf(buf2, sz, "%ld", num);

	count2 = cnv2;

	keypad_ini2();

	//char buf2[9]  = {0};
	//int sz = sizeof(buf2);

	// prefresh the lcd first.

	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);

	if(fxn == 1) goto refresh;

//	for(int i = 0 ; i < (7+22);i++)
//	{
//		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
//		_Delay(1);
//		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
//	}
//
//	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET);  // _Delay(1);
//	_Delay(1);
//	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_SET);
//	_Delay(1);
//	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET);  // _Delay(1);
//	_Delay(1);
//
//	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_SET);
//
//	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET);


	if(dpFlag2 == 0)
	{
		postn2 = 6;
		if(strchr(num, '.') ) --count2;
		while(count2 < 7)
		{
			buf2[postn2] = 0;           //prefill leading spaces with spaces (zeros)
			count2++;
			postn2--;
		}
		count2 = postn2;

		while (postn2 > -1) //for(int i = cnv; i<8; i++)   //fill the rest of the buffer with the code pattern.
		{
			if(num[count2 - postn2] == '.')  //&& (num[cnv2 - 1] == '.') )
			{
//				buf2[postn2] = 0x7E + 0X80;
				scan_code2 = To_pattern( ToInt( num[(count2 - postn2) + 1] ) );
				buf2[postn2] = scan_code2 + 0X80;
				count2++;
			}
			else
			{
					scan_code2 = To_pattern( ToInt( num[count2 - postn2] ) );
					buf2[postn2] = scan_code2;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];
			}
			postn2--;
		}
	}
	else if (dpFlag2 == 1)
	{
		postn2 = 7;
		while(count2 <= 8)
		{
			buf2[postn2] = 0;           //prefill leading spaces with spaces (zeros)
			count2++;
			postn2--;
		}

		count2 = 6;

		while (postn2 >= -1) //for(int i = cnv; i<8; i++)   //fill the rest of the buffer with the code pattern.
		{

			if(postn2 == (dpCount2 - 1) )
			{

			}
			else if(postn2 == ((dpCount2 - 1) - 1) )
			{
				scan_code2 = To_pattern( ToInt( num[count2 - (postn2 + 1)] ) );
				buf2[(postn2 + 1)] = scan_code2 + 0X80;
			}

			else
			{
				scan_code2 = To_pattern( ToInt( num[count2 - (postn2 + 1)] ) );
				if (  ((keyEntry2_len == 8) && (dpCount2 != 1) && ((postn2 == 1 ) || (postn2 == 0 ) || (postn2 == -1))) ||
					((keyEntry2_len == 7) && ((postn2 == 0 ) || (postn2 == -1))) ||
					((keyEntry2_len == 6) && (postn2 == -1))  || ((dpCount2 == 2) && (postn2 == -1)) ||
					((dpCount2 == 3) && ((postn2 == 0 ) || (postn2 == -1)))  ||
					((dpCount2 == 4) && ((postn2 == 1 ) || (postn2 == 0 ) || (postn2 == -1))) )
				{
					buf2[postn2 + 1] = scan_code2;
				}
				else
					buf2[postn2] = scan_code2;
			}
			postn2--;
		}
	}




//	int scan_code2 = 0;
//	count2 = postn2;
//	while (postn2 > -1) //for(int i = cnv; i<8; i++)   //fill the rest of the buffer with the code pattern.
//	{
//		scan_code2 =  To_pattern( ToInt( num[count2-postn2] ) );
//		buf2[postn2] = scan_code2;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];
//		postn2--;
//	}



	refresh:
	// move the buf[] array into the shift register.
	for(uint8_t i = 0; i < 7 ; i++)
	{
		shift2(buf2[i], 0);
	}

	// push "0" into the keyboards memory first.
	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
	_Delay2(1);
	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
	_Delay2(1);
	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);


	//push in "1"s past the keyboard registers memory of 24-1 places
	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);

	for(uint8_t i = 0; i < 22; i++)
	{
		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
		_Delay2(1);
		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
		_Delay2(1);
	}

	//then latch the shifted data...

	HAL_GPIO_WritePin(lthk2_GPIO_Port, lthk2_Pin, GPIO_PIN_SET);  //latch pin high

	if(lat_cnt2 == 0)
	{
	  HAL_GPIO_WritePin(lthk2_GPIO_Port, lthk2_Pin, GPIO_PIN_RESET); //Latch pin low
	}
	lat_cnt2++;
	if(lat_cnt2 == 2) lat_cnt2 = 0;

	//_Delay(10);

//	pushout the remaining zero from the keyboard registers.
	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
	_Delay2(1);
		  HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
	_Delay2(1);

	//==========================================================================
	   if(fxn == 2) return;     // return if fxn 2 selected (screen only..)
	//==========================================================================
	   //flush registers with 1
	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);

	 for(uint8_t pmp = 0; pmp < 25; pmp++)
	 {
			HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
		 _Delay2(1);
			HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
		 _Delay2(1);
	 }
	   //=======================================================================

	 uint8_t keynumber = 0;
	 uint8_t fill = 22;
	   // push "0" into the keyboards memory first.

	   	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_RESET);
	   	_Delay2(1);
	   	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
	   	_Delay2(1);
	   	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);

	   	//check keypress..
	      if (HAL_GPIO_ReadPin(com12k2_GPIO_Port, com12k2_Pin ) == 0)
	   	   {
	   	        keynumber = 1;
//         #if delay_keypad == 1
//
//	   	       if(self->settings_stream2[1].keypress_tone == Yes)
//			   {
//				  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
//			   }
//
//	   	       HAL_Delay(keypad_delay);
//         #endif
//
//	   	       HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
	   	       return keynumber;
	   	   }

	    //  fill = 22;
	    //  if(pump_type == lafeng) fill = 17;

	    HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);

	  for(uint8_t i = 2; i < fill; i++)
	  {
	   		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
	   		//_Delay(1);
	   		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
	   		//_Delay(1);
	   		if( checkkey2() == 1)
	   		{
	   				keynumber = i;
//             #if delay_keypad == 1
//
//	   			 if(self->settings_stream2[1].keypress_tone == Yes)
//				 {
//					  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
//				 }
//
//	   			 HAL_Delay(keypad_delay);
//             #endif
//
//	   			 HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
	   			 return keynumber;
	   		}
	 }

	   //pushout the remaining zero from the keyboard registers.
	   //	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_RESET);
	   //	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
	   //	_Delay(1);
	   //		  HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
	   //	_Delay(1);
	   	return keynumber;
}


//int write_keypad_lcd2(int fxn,char* num)
//{
//	extern int8_t keyEntry2_len;
//
//	keypad_ini2();
//	int count2 = 0;
//
//	//char buf2[9]  = {0};
//	//int sz = sizeof(buf2);
//
//	// prefresh the lcd first.
//
//	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
//
//	if(fxn == 1) goto refresh;
//
////	for(int i = 0 ; i < (7+22);i++)
////	{
////		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
////		_Delay(1);
////		HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
////	}
////
////	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET);  // _Delay(1);
////	_Delay(1);
////	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_SET);
////	_Delay(1);
////	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET);  // _Delay(1);
////	_Delay(1);
////
////	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_SET);
////
////	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_RESET);
//
//
//	int cnv2 =   strlen(num);      //snprintf(buf2, sz, "%ld", num);
//
//	count2 = cnv2;
//	int postn2 = 6;
//
//	while(count2<7)
//	{
//		buf2[postn2] = 0;           //prefill leading spaces with spaces (zeros)
//		count2++;
//		postn2--;
//	}
//	int scan_code2 = 0;
//	count2 = postn2;
//	while (postn2 > -1) //for(int i = cnv; i<8; i++)   //fill the rest of the buffer with the code pattern.
//	{
//		scan_code2 =  To_pattern( ToInt( num[count2-postn2] ) );
//		buf2[postn2] = scan_code2;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];
//		postn2--;
//	}
//
//	refresh:
//	// move the buf[] array into the shift register.
//	for(int i = 0; i < 7 ; i++)
//	{
//		shift2(buf2[i],0);
//	}
//
//	// push "0" into the keyboards memory first.
//	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_RESET);
//
//	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
//	_Delay2(1);
//	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
//	_Delay2(1);
//	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);
//
//
//	//push in "1"s past the keyboard registers memory of 24-1 places
//	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);
//
//	for(int i=0;i<22;i++)
//	{
//		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
//		_Delay2(1);
//		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
//		_Delay2(1);
//	}
//
//	//then latch the shifted data...
//
//	HAL_GPIO_WritePin(lthk2_GPIO_Port, lthk2_Pin, GPIO_PIN_SET);  //latch pin high
//
//	if(lat_cnt2 == 0)
//	{
//	  HAL_GPIO_WritePin(lthk2_GPIO_Port, lthk2_Pin, GPIO_PIN_RESET); //Latch pin low
//	}
//	lat_cnt2++;
//	if(lat_cnt2 == 2) lat_cnt2 = 0;
//
//	//_Delay(10);
//
////	pushout the remaining zero from the keyboard registers.
//	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
//	_Delay2(1);
//		  HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
//	_Delay2(1);
//
//	//==========================================================================
//	   if(fxn == 2) return;     // return if fxn 2 selected (screen only..)
//	//==========================================================================
//	   //flush registers with 1
//	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);
//
//	 for(int pmp = 0;pmp<25;pmp++)
//	 {
//			HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
//		 _Delay2(1);
//			HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
//		 _Delay2(1);
//	 }
//	   //=======================================================================
//
//	 int keynumber = 0;
//	 int fill = 22;
//	   // push "0" into the keyboards memory first.
//
//	   	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_RESET);
//	   	_Delay2(1);
//	   	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
//	   	_Delay2(1);
//	   	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
//
//	   	//check keypress..
//	      if (HAL_GPIO_ReadPin(com12k2_GPIO_Port, com12k2_Pin ) == 0)
//	   	   {
//	   	        keynumber = 1;
//         #if delay_keypad == 1
//	   	       HAL_Delay(keypad_delay);
//         #endif
//	   	        return keynumber;
//	   	   }
//
//	    //  fill = 22;
//	    //  if(pump_type == lafeng) fill = 17;
//
//	    HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);
//
//	  for(int i=2;i<fill;i++)
//	   	{
//	   		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
//	   		//_Delay(1);
//	   		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
//	   		//_Delay(1);
//	   		if( checkkey2() == 1)
//	   			{
//	   				keynumber = i;
//             #if delay_keypad == 1
//	   			 HAL_Delay(keypad_delay);
//             #endif
//	   				return keynumber;
//	   			}
//	   	}
//
//	   //pushout the remaining zero from the keyboard registers.
//	   //	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_RESET);
//	   //	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
//	   //	_Delay(1);
//	   //		  HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
//	   //	_Delay(1);
//	   	return keynumber;
//}


void shift_2(uint8_t data_byte, uint8_t lat)
{
	//Latch pin low
	HAL_GPIO_WritePin(lthk2_GPIO_Port, lthk2_Pin, GPIO_PIN_RESET);

	for (uint8_t i = 0; i < 8; i++)
	{
		//clock pin low
		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
		// delay 1 ms
		_Delay2(1);
		// check  bit, second bit ...
		//if high set out high
		if ( byteRead(data_byte, i) == 0 )
			HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);
		else	//if low set dp low
			HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_RESET);

		//set clock pin high
		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);
		//delay 1 ms
		_Delay2(1);
	}



  if(lat == 1)
  {
	HAL_GPIO_WritePin(lthk2_GPIO_Port, lthk2_Pin, GPIO_PIN_SET);      //latch pin high
   	_Delay(1);
   	HAL_GPIO_WritePin(lthk2_GPIO_Port, lthk2_Pin, GPIO_PIN_RESET);    //Latch pin low
  }
}

void shift2(uint8_t data_byte,uint8_t lat)
{
	//Latch pin low
	HAL_GPIO_WritePin(lthk2_GPIO_Port, lthk2_Pin, GPIO_PIN_RESET);

	for (uint8_t i = 0; i < 8; i++)
	{
		//clock pin low
		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
		// delay 1 ms
		_Delay2(1);
		// check  bit, second bit ...
		//if high set out high
		int bitt ;
		if(lat_cnt2 == 0)
			    bitt = 1;
		else
			    bitt = 0;

		if ( byteRead(data_byte, i) == bitt )
			HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);
		else	//if low set dp low
			HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_RESET);

		//set clock pin high
		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_SET);
		//delay 1 ms
		_Delay2(1);
	}

	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);

  if(lat == 1)
  {
	HAL_GPIO_WritePin(lthk2_GPIO_Port, lthk2_Pin, GPIO_PIN_SET);      //latch pin high
   	_Delay2(1);
   	HAL_GPIO_WritePin(lthk2_GPIO_Port, lthk2_Pin, GPIO_PIN_RESET);    //Latch pin low
  }
}

/**
 *  This routine writes to the KEYPAD LCD and also read the key press
 *  This routine writes to the KEYPAD LCD.
 *  * @param[in]  num  int contains the numeric value to be displayed on the KEYPAD LCD
 *  * @param[out] int  outputs 0 for no keypressed or greater than 0 for the keyposition.
 */
uint8_t keypad_lcd2(int fxn, char* num)
{

//	if(pump_type2 != lafeng)
//	if(pump_type != DN_LAFNG17K)
	if( (self->settings_stream1[1].keypad__ != LAFNG17_K) && (self->settings_stream1[1].keypad__ != LAFNG18_K) && (self->settings_stream1[1].keypad__ != LAFNG18_K_V2) )
	{
		uint8_t rr =	write_keypad_lcd2(fxn, num);
	    return rr;
	}


	HAL_GPIO_WritePin(lthk2_GPIO_Port, lthk2_Pin, GPIO_PIN_RESET); //Latch pin low --------------------------------------------

	if(fxn == 1)
		goto refresh;

	uint8_t keynumber = 0;

	int8_t count = 0,
		   scan_code = 0,
		   inv = 0,
		   postn = 0;

	count = strlen(num);

	postn = 6;

	if(strchr(num, '.') ) --count;

	inv = count;

	while(count < 7)
	{
		buf2[postn] = 0;           //prefill leading spaces with spaces (zeros)
		count++;
		postn--;
	}
	count = postn;

	while (postn > -1)   //fill the rest of the buffer with the code pattern.
	{
		if(num[count - postn] == '.')
		{
			scan_code = To_pattern( ToInt( num[(count - postn) + 1] ) );

			scan_code += 0x80;
			buf2[postn] = scan_code;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];

			count++;
		}
		else
		{
				scan_code = To_pattern( ToInt( num[count - postn] ) );

				buf2[postn] = scan_code;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];
		}
		postn--;
	}

	refresh:
 //===========================================================================

	if (self->settings_stream1[1].keypad__ == LAFNG17_K)
	{
	    	disp_len_pad2 = 5;
	}
    else if(self->settings_stream1[1].keypad__  == LAFNG18_K)
    {
    	disp_len_pad2 = 7;
    }
    else if(self->settings_stream1[1].keypad__ == LAFNG18_K_V2)
    {
    	disp_len_pad2 = 8;
    }

	for(uint8_t i = 0; i < disp_len_pad2; i++)  //7 bytes for bluesky 5 bytes for lafeng
	{
		shift_2(buf2[i], 0);
	}

	//push in "1"s past the keyboard registers memory of 24-1 places
	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);

	uint8_t fill = 24;

	if ( (self->settings_stream1[1].keypad__ == LAFNG17_K) || (self->settings_stream1[1].keypad__ == LAFNG18_K) || (self->settings_stream1[1].keypad__ == LAFNG18_K_V2) )
		fill = 16;

	for(uint8_t i = 1; i < fill; i++)
	{
		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
		//	_Delay(1);
		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
		//	_Delay(1);
	}
	//then latch the shifted data...
	HAL_GPIO_WritePin(lthk2_GPIO_Port, lthk2_Pin, GPIO_PIN_SET);  //latch pin high
    //	_Delay(1);
	HAL_GPIO_WritePin(lthk2_GPIO_Port, lthk2_Pin, GPIO_PIN_RESET); //Latch pin low
    //	_Delay(1);
    //--------------------	HAL_GPIO_WritePin(latch_port, latch_pin, GPIO_PIN_SET);  //latch pin high

    HAL_GPIO_TogglePin(refresh2_GPIO_Port, refresh2_Pin);

//==========================================================================
        if(fxn == 2) return;     // return if fxn 2 selected (screen only..)
//==========================================================================

    HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);
    for(uint8_t i = 0; i < 5; i++)
   	{
   		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
   		//_Delay(1);
   		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
   		//_Delay(1);
   	}

//=======================================================================
	keynumber = 0;
// push "0" into the keyboards memory first.

	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_RESET);
	//_Delay(1);
	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
	//_Delay(1);
	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);


	//check keypress..
   if( checkkey2() == 1)
	   {
	        keynumber = 1;

//	        if(self->settings_stream2[1].keypress_tone == Yes)
//			{
//			  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
//			}
//
//			HAL_Delay(1);
//
//			HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);

	        return keynumber;
	   }

   fill = 22;

   if ( (self->settings_stream1[1].keypad__ == LAFNG17_K) || (self->settings_stream1[1].keypad__ == LAFNG18_K) || (self->settings_stream1[1].keypad__ == LAFNG18_K_V2))
	   fill = 17;

   HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);

   for(uint8_t i = 2; i < fill; i++)
	{
		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
		//_Delay(1);
		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
		//_Delay(1);
		if( checkkey2() == 1)
			{
				keynumber = i;

//				if(self->settings_stream2[1].keypress_tone == Yes)
//			    {
//				  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
//			    }
//
//				HAL_Delay(1);
//
//				HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);

				return keynumber;
			}
	}

	//pushout the remaining zero from the keyboard registers.
//	HAL_GPIO_WritePin(data_port, data_pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_SET);
//	_Delay(1);
//		  HAL_GPIO_WritePin(clock_port, clock_pin, GPIO_PIN_RESET);
//	_Delay(1);
	return keynumber;
}

