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

 extern pump_settings_stream1 settings_stream1[2],
 					 	 	 settings0_stream1[2];

 extern pump_settings_stream2 settings_stream2[2],
 							 settings0_stream2[2];

 extern pump_settings_stream3 settings_stream3[2],
 				       	     settings0_stream3[2],
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

  extern int8_t dpFlag,
  	  	  	  	dpCount;


//=================================================================
void shift_(uint8_t data_byte,uint8_t lat);
void shift(uint8_t data_byte,uint8_t lat);

void shift_2(uint8_t data_byte,uint8_t lat);
void shift2(uint8_t data_byte,uint8_t lat);

void keypad_ini(void)
{
	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
	for(int i = 0; i < 21; i++)
	{
			HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
			_Delay(1);
			HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
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
int checkkey(void)
{
	 return(  (HAL_GPIO_ReadPin(com12k1_GPIO_Port, com12k1_Pin ) == 1)? 0:1  );   // return 1 if keypressed
}

int checkkey2(void)
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
int scankey(void)
{
	int count = 0;
	int keynumber = 0;
	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_RESET);
	_Delay(1);
	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);        //shift in first bit.
	_Delay(1);
	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
	count++;
	  if( checkkey() == 1) {
		  keynumber = 1;
		  goto keyfound;    //return i;
	  }
 //-----------------------------------------------------------------------
   HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);

	for(int i=2;i<22;i++)
	{
		    _Delay(1);
			HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);             //shift the rest 20 bits.
			_Delay(1);
			HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
			count++;
			  if( checkkey() == 1) {
					  keynumber = i;
					  goto keyfound;    //return i;
				  }
	}
      goto endd; //return 0;     // return with 0 if no key pressed.

 keyfound:   _Delay(0);
//	        int temp = keynumber;
              while(count < 22)
              {
            	 // temp++;
            	   HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);  //shift out the rest of the bits.
            		_Delay(1);
            	   HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
            	   count++;
              }

	  endd:    // shift out the last bit...
	            _Delay(1);
				HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);  //shift the rest 20 bits.
				_Delay(1);
				HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
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

int readkeyboard(int type)
{
  //while(1)
  //{
	int ky = scankey();
        key  = ky;
  	   if (ky > 0)
       {
          //  return ky;
          //  _Delay(1);
  	   }
 // }
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
int write_keypad_lcd(int fxn,char* num)
{
	extern int8_t keyEntry_len;

	int8_t count = 0,
		   decimalPoint_flag = 0,
		   postn,
		   scan_code = 0;

	dpFlag = 0;

	int8_t cnv = strlen(num);      //snprintf(buf2, sz, "%ld", num);

	count = cnv;

	keypad_ini();


	//char buf2[9]  = {0};
	//int sz = sizeof(buf2);

	// prefresh the lcd first.

	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);

	if(fxn == 1) goto refresh;

//	for(int i = 0 ; i < (7+22);i++)
//	{
//		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
//		_Delay(1);
//		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
//	}
//
//	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET);  // _Delay(1);
//	_Delay(1);
//	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_SET);
//	_Delay(1);
//	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET);  // _Delay(1);
//	_Delay(1);
//
//	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_SET);
//
//	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET);

	/*
	*/



//	 int8_t space2, c;
//	 c = strlen(num);
//	 if( (strchr(num, '.')) && (num[cnv-1] != '.') )
//	 {
////		 count--;
//	 }
//	 else
//	 {
//		 space2 = 7 - index_;     //6 - index_;
//	 }

//	 int8_t space2, c;
//	 c = strlen(keyboard_entry);
//	 if( (strchr(keyboard_entry, '.')) && (keyboard_entry[c-1] != '.') )
//	 {
//		 space2 = 7 + ( -index_ + 1);
//	 }
//	 else
//	 {
//		 space2 = 7 - index_;     //6 - index_;
//	 }

//	count = cnv;


//	 int8_t c;
//	 c = strlen(num);
//	 if( (strchr(num, '.')) && (num[c-1] != '.') )
//	 {
//		 space2 = 7 + ( -index_ + 1);
//	 }
//	 else
//	 {
//		 space2 = 7 - index_;     //6 - index_;
//	 }

	if(dpFlag == 0)
	{
		postn = 6;
		if(strchr(num, '.') ) --count;
		while(count<7)
		{
			buf[postn] = 0;           //prefill leading spaces with spaces (zeros)
			count++;
			postn--;
		}
		count = postn;

		while (postn > -1) //for(int i = cnv; i<8; i++)   //fill the rest of the buffer with the code pattern.
		{
//			if( (num[count-postn] == '.' )  && (num[cnv-1] == '.') )
			if(num[count-postn] == '.' ) // && (num[cnv-1] == '.') )
			{
	//			char buff = buf[postn + 1];
	//			buf[postn--] = buff + 0x80;
	//			postn--;
	//			continue;
				scan_code = To_pattern( ToInt( num[(count-postn) + 1] ) );
//				buf[postn] = 0x7E + 0X80;    //0x7E for a '0'
				buf[postn] = scan_code + 0X80;
				count++;
//				postn--;
			}
			else
			{
					scan_code = To_pattern( ToInt( num[count-postn] ) );
					buf[postn] = scan_code;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];
			}
			postn--;
		}
	}
	else if (dpFlag == 1)
	{
		postn = 7;
		while(count <= 8)
		{
			buf[postn] = 0;           //prefill leading spaces with spaces (zeros)
			count++;
			postn--;
		}

		count = 6;

		while (postn >= -1) //for(int i = cnv; i<8; i++)   //fill the rest of the buffer with the code pattern.
		{

			if(postn == (dpCount-1) )
			{

			}
			else if(postn == ((dpCount-1)-1) )
			{
				scan_code = To_pattern( ToInt( num[count - (postn + 1)] ) );
				buf[(postn + 1)] = scan_code + 0X80;
//				postn++;
			}

			else
			{
				scan_code = To_pattern( ToInt( num[count - (postn + 1)] ) );
//				if (postn == (dpCount - 3) )
//				{
//					buf[postn + 1] = scan_code;
//				}
				if (  ((keyEntry_len == 8) && (dpCount != 1) && ((postn == 1 ) || (postn == 0 ) || (postn == -1))) ||
					((keyEntry_len == 7) && ((postn == 0 ) || (postn == -1))) ||
					((keyEntry_len == 6) && (postn == -1))  || ((dpCount == 2) && (postn == -1)) ||
					((dpCount == 3) && ((postn == 0 ) || (postn == -1)))  ||
					((dpCount == 4) && ((postn == 1 ) || (postn == 0 ) || (postn == -1))) )
				{
					buf[postn + 1] = scan_code;
				}
				else
					buf[postn] = scan_code;
			}
			postn--;
		}

//		dpFlag = 0;
	}

//	int scan_code = 0;

//	if( (strchr(num, '.')) && (num[cnv-1] != '.') )
//	 {
////		postn++;
//	 }

//	if (dpFlag == 1)
//		count = 6;
//	else
//		count = postn;
//
//	while (postn > -1) //for(int i = cnv; i<8; i++)   //fill the rest of the buffer with the code pattern.
//	{
//		if (dpFlag == 1)
//		{
//			if(postn == 0)
//			{
//
//			}
//			else if(postn == -1)
//			{
//				scan_code = To_pattern( ToInt( num[count - (postn + 1)] ) );
//				buf[(postn + 1)] = scan_code + 0X80;
//
////				decimalPoint_flag = 0;
//			}
//
//			else
//			{
//				scan_code = To_pattern( ToInt( num[count - (postn + 1)] ) );
//				buf[postn] = scan_code;
//			}
//		}
//		else if( (num[count-postn] == '.' )  && (num[cnv-1] == '.') )
//		{
////			char buff = buf[postn + 1];
////			buf[postn--] = buff + 0x80;
////			postn--;
////			continue;
////			scan_code = To_pattern( ToInt( num[count-postn] ) );
//			buf[postn] = 0x7E + 0X80;
//		}
//		else if( (num[count-postn] == '.' )  && (num[cnv-1] != '.') )
//		{
////			char buff = buf[postn + 1];
////			buf[postn--] = buff + 0x80;
////			postn--;
////			continue;
////			scan_code = To_pattern( ToInt( num[count-postn] ) );
////			buf[postn] = 0x7E + 0X80;
//
//			postn++;
//
//			decimalPoint_flag = 1;
//		}
//		else
//		{
//			if(decimalPoint_flag == 1)
//			{
//
//				scan_code = To_pattern( ToInt( num[count-postn] ) );
//				buf[postn] = scan_code + 0X80;
//
//				decimalPoint_flag = 0;
//			}
//
//			else
//			{
//				scan_code = To_pattern( ToInt( num[count-postn] ) );
//				buf[postn] = scan_code;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];
//			}
//		}
//		postn--;
//	}

	refresh:
	// move the buf[] array into the shift register.
	for(int i = 0; i < 7 ; i++)
	{
		shift(buf[i],0);
	}

	// push "0" into the keyboards memory first.
	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
	_Delay(1);
	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
	_Delay(1);
	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);


	//push in "1"s past the keyboard registers memory of 24-1 places
	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);

	for(int i=0;i<22;i++)
	{
		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
		_Delay(1);
		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
		_Delay(1);
	}

	//then latch the shifted data...

	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_SET);  //latch pin high

	if(lat_cnt == 0)
	{
	  HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET); //Latch pin low
	}
	lat_cnt++;
	if(lat_cnt == 2) lat_cnt = 0;

	//_Delay(10);

//	pushout the remaining zero from the keyboard registers.
	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
	_Delay(1);
		  HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
	_Delay(1);

	//==========================================================================
	   if(fxn == 2) return;     // return if fxn 2 selected (screen only..)
	//==========================================================================
	   //flush registers with 1
	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);

	 for(int pmp = 0;pmp<25;pmp++)
	 {
			HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
		 _Delay(1);
			HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
		 _Delay(1);
	 }
	   //=======================================================================

	 int keynumber = 0;
	 int fill = 22;
	   // push "0" into the keyboards memory first.

	   	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_RESET);
	   	_Delay(1);
	   	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
	   	_Delay(1);
	   	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);

	   	//check keypress..
	      if (HAL_GPIO_ReadPin(com12k1_GPIO_Port, com12k1_Pin ) == 0)
	   	   {
	   	        keynumber = 1;
         #if delay_keypad == 1
	   	       HAL_Delay(keypad_delay);
         #endif
	   	        return keynumber;
	   	   }

	    //  fill = 22;
	    //  if(pump_type == lafeng) fill = 17;

	    HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);

	      for(int i=2;i<fill;i++)
	   	{
	   		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
	   		//_Delay(1);
	   		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
	   		//_Delay(1);
	   		if( checkkey() == 1)
	   			{
	   				keynumber = i;
             #if delay_keypad == 1
	   			 HAL_Delay(keypad_delay);
             #endif
	   				return keynumber;
	   			}
	   	}

	   	//pushout the remaining zero from the keyboard registers.
	   //	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_RESET);
	   //	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
	   //	_Delay(1);
	   //		  HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
	   //	_Delay(1);
	   	return keynumber;

}


void shift_(uint8_t data_byte,uint8_t lat)
{
	//Latch pin low
	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET);

	for (int i = 0; i < 8; i++)
	{
		//clock pin low
		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
		// delay 1 ms
		_Delay(1);
		// check  bit, second bit ...
		//if high set out high
		if ( byteRead(data_byte, i) == 0 )
			HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);
		else	//if low set dp low
			HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_RESET);

		//set clock pin high
		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);
		//delay 1 ms
		_Delay(1);
	}



  if(lat == 1)
  {
	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_SET);      //latch pin high
   	_Delay(1);
   	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET);    //Latch pin low
  }
}

void shift(uint8_t data_byte,uint8_t lat)
{
	//Latch pin low
	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET);

	for (int i = 0; i < 8; i++)
	{
		//clock pin low
		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
		// delay 1 ms
		_Delay(1);
		// check  bit, second bit ...
		//if high set out high
		int bitt ;
		if(lat_cnt == 0)
			    bitt = 1;
		else
			    bitt = 0;

		if ( byteRead(data_byte, i) == bitt )
			HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);
		else	//if low set dp low
			HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_RESET);

		//set clock pin high
		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);
		//delay 1 ms
		_Delay(1);
	}

	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);

  if(lat == 1)
  {
	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_SET);      //latch pin high
   	_Delay(1);
   	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET);    //Latch pin low
  }
}

/**
 *  This routine writes to the KEYPAD LCD and also read the key press
 *  This routine writes to the KEYPAD LCD.
 *  * @param[in]  num  int contains the numeric value to be displayed on the KEYPAD LCD
 *  * @param[out] int  outputs 0 for no keypressed or greater than 0 for the keyposition.
 */
int keypad_lcd(int fxn,char* num)
{

//	if(pump_type != lafeng)
//	if(pump_type != DN_LAFNG17K)
	if( (settings_stream1[0].keypad__ != LAFNG17_K) && (settings_stream1[0].keypad__ != LAFNG18_K) )
	{
	  int rr =	write_keypad_lcd(fxn,num);
	  return rr;
	}


	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET); //Latch pin low --------------------------------------------

	if(fxn == 1) goto refresh;

	int8_t keynumber = 0;
	int8_t count = 0;
	int8_t scan_code = 0;
	int8_t inv = 0;
	int8_t postn = 0;

	count = strlen(num);      //snprintf(buf2, sz, "%ld", num);

//	inv = count;

	//count = cnv;
	postn = 6;

	if(strchr(num, '.') ) --count;

	inv = count;

	while(count < 7)
	{
		buf[postn] = 0;           //prefill leading spaces with spaces (zeros)
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

			if(settings_stream1[0].keypad__  == LAFNG18_K)
			{
				scan_code += 0x80;
//				buf[postn] = (scan_code ^ 0xFF);
				buf[postn] = scan_code;
			}
			else
			{
				scan_code += 0x80;
				buf[postn] = scan_code;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];
			}

//			buf2[postn] = scan_code + 0X80;
			count++;
		}
		else
		{
				scan_code = To_pattern( ToInt( num[count - postn] ) );

				if(settings_stream1[0].keypad__  == LAFNG18_K)
				{
//					buf[postn] = (scan_code ^ 0xFF);
					buf[postn] = scan_code;
				}
				else
				{
					buf[postn] = scan_code;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];
				}
		}
		postn--;
	}

//	if(settings_stream1[0].keypad__  == LAFNG18_K)
//	{
//		for (int8_t i = 8; i >= inv; i--)
//		{
//			buf[i] = (buf[i] ^ 0xFF);
//		}
//	}

//	count =   strlen(num);      //snprintf(buf2, sz, "%ld", num);
//
//	//count = cnv;
//	int postn = 6;
//
//	while(count<7)
//	{
//		buf[postn] = 0;    //prefill leading spaces with spaces (zeros)
//		count++;
//		postn--;
//	}
//	int scan_code = 0;
//	count = postn;
//	while (postn > -1)//fill the rest of the buffer with the code pattern.
//	{
//		scan_code =  To_pattern( ToInt( num[count-postn] ) );
//		buf[postn] = scan_code;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];
//		postn--;
//	}
	refresh:
 //===========================================================================
 // HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET); //Latch pin low
 // move the buf[] array into the shift register.

//	if (pump_type == lafeng) disp_len_pad = 5;
//	if (pump_type == DN_LAFNG17K)
	if (settings_stream1[0].keypad__ == LAFNG17_K)
	{
	    	disp_len_pad = 5;
	}
    else if(settings_stream1[0].keypad__  == LAFNG18_K)
    {
    	disp_len_pad = 7;
    }

	for(int i = 0; i < disp_len_pad; i++)  //7 bytes for bluesky 5 bytes for lafeng
	{
		shift_(buf[i], 0);
	}

	//push in "1"s past the keyboard registers memory of 24-1 places
	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);

    int fill = 24;
//if(pump_type == lafeng) fill = 16;
//if(pump_type == DN_LAFNG17K)
	if ( (settings_stream1[0].keypad__ == LAFNG17_K) || (settings_stream1[0].keypad__ == LAFNG18_K) )
		fill = 16;

	for(int i = 1; i < fill; i++)
		{
			HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
			//	_Delay(1);
			HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
			//	_Delay(1);
		}
	//then latch the shifted data...
	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_SET);  //latch pin high
    //	_Delay(1);
	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET); //Latch pin low
    //	_Delay(1);
    //--------------------	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_SET);  //latch pin high

    HAL_GPIO_TogglePin(refresh_GPIO_Port,refresh_Pin);

//==========================================================================
        if(fxn == 2) return;     // return if fxn 2 selected (screen only..)
//==========================================================================

    HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);
    for(int i=0;i<5;i++)
   	{
   		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
   		//_Delay(1);
   		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
   		//_Delay(1);
   	}

//=======================================================================
	keynumber = 0;
// push "0" into the keyboards memory first.

	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_RESET);
	//_Delay(1);
	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
	//_Delay(1);
	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);


	//check keypress..
   if( checkkey() == 1)
	   {
	        keynumber = 1;
	        HAL_Delay(1);
	        return keynumber;
	   }

   fill = 22;
//   if(pump_type == lafeng) fill = 17;
//   if(pump_type == DN_LAFNG17K)
   if ( (settings_stream1[0].keypad__ == LAFNG17_K) || (settings_stream1[0].keypad__ == LAFNG18_K) )
	   fill = 17;

   HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);

   for(int i = 2; i < fill; i++)
	{
		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
		//_Delay(1);
		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
		//_Delay(1);
		if( checkkey() == 1)
			{
				keynumber = i;
				HAL_Delay(1);
				return keynumber;
			}
	}

	//pushout the remaining zero from the keyboard registers.
//	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
//	_Delay(1);
//		  HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
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
int write_keypad_lcd2(int fxn,char* num)
{
	extern int8_t keyEntry2_len;
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
//		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
//		_Delay(1);
//		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
//	}
//
//	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET);  // _Delay(1);
//	_Delay(1);
//	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_SET);
//	_Delay(1);
//	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET);  // _Delay(1);
//	_Delay(1);
//
//	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_SET);
//
//	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET);


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
	for(int i = 0; i < 7 ; i++)
	{
		shift2(buf2[i],0);
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

	for(int i=0;i<22;i++)
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

	 for(int pmp = 0;pmp<25;pmp++)
	 {
			HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
		 _Delay2(1);
			HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
		 _Delay2(1);
	 }
	   //=======================================================================

	 int keynumber = 0;
	 int fill = 22;
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
         #if delay_keypad == 1
	   	       HAL_Delay(keypad_delay);
         #endif
	   	        return keynumber;
	   	   }

	    //  fill = 22;
	    //  if(pump_type == lafeng) fill = 17;

	    HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);

	  for(int i = 2; i < fill; i++)
	   	{
	   		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
	   		//_Delay(1);
	   		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
	   		//_Delay(1);
	   		if( checkkey2() == 1)
	   			{
	   				keynumber = i;
             #if delay_keypad == 1
	   			 HAL_Delay(keypad_delay);
             #endif
	   				return keynumber;
	   			}
	   	}

	   //pushout the remaining zero from the keyboard registers.
	   //	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_RESET);
	   //	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
	   //	_Delay(1);
	   //		  HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
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
////		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
////		_Delay(1);
////		HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
////	}
////
////	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET);  // _Delay(1);
////	_Delay(1);
////	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_SET);
////	_Delay(1);
////	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET);  // _Delay(1);
////	_Delay(1);
////
////	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_SET);
////
////	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET);
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
//	   //	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_RESET);
//	   //	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
//	   //	_Delay(1);
//	   //		  HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
//	   //	_Delay(1);
//	   	return keynumber;
//}


void shift_2(uint8_t data_byte, uint8_t lat)
{
	//Latch pin low
	HAL_GPIO_WritePin(lthk2_GPIO_Port, lthk2_Pin, GPIO_PIN_RESET);

	for (int i = 0; i < 8; i++)
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
		//HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);
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

	for (int i = 0; i < 8; i++)
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
		//HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_SET);
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
int keypad_lcd2(int fxn, char* num)
{

//	if(pump_type2 != lafeng)
//	if(pump_type != DN_LAFNG17K)
	if( (settings_stream1[1].keypad__ != LAFNG17_K) && (settings_stream1[1].keypad__ != LAFNG18_K) )
	{
	  int rr =	write_keypad_lcd2(fxn, num);
	  return rr;
	}


	HAL_GPIO_WritePin(lthk2_GPIO_Port, lthk2_Pin, GPIO_PIN_RESET); //Latch pin low --------------------------------------------

	if(fxn == 1)
		goto refresh;

	int8_t keynumber = 0;
	int8_t count = 0;
	int8_t scan_code = 0;
	int8_t inv = 0;
	int8_t postn = 0;

//	keypad_ini2();
//
//	//char buf2[9]  = {0};
//	//int sz = sizeof(buf2);
//
//	// prefresh the lcd first.
//
//	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);

	count = strlen(num);      //snprintf(buf2, sz, "%ld", num);

//	inv = count;

	//count = cnv;
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

	while (postn > -1) //for(int i = cnv; i<8; i++)   //fill the rest of the buffer with the code pattern.
	{
		if(num[count - postn] == '.')  //&& (num[cnv2 - 1] == '.') )
		{
//				buf2[postn2] = 0x7E + 0X80;
			scan_code = To_pattern( ToInt( num[(count - postn) + 1] ) );

//			if(settings_stream1[1].keypad__  == LAFNG18_K)
//			{
//				scan_code += 0x80;
//				buf2[postn] = (scan_code ^ 0xFF);
//			}
//			else
//			{
				scan_code += 0x80;
				buf2[postn] = scan_code;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];
//			}

//			buf2[postn] = scan_code + 0X80;
			count++;
		}
		else
		{
				scan_code = To_pattern( ToInt( num[count - postn] ) );

//				if(settings_stream1[1].keypad__  == LAFNG18_K)
//				{
////					if(scan_code == 0)
////						buf2[postn] = scan_code;
////					else
//						buf2[postn] = (scan_code ^ 0xFF);
//				}
//				else
//				{
					buf2[postn] = scan_code;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];
//				}
		}
		postn--;
	}


//	if(settings_stream1[1].keypad__  == LAFNG18_K)
//	{
//		for (int8_t i = 8; i >= inv; i--)
//		{
//			buf2[i] = (buf2[i] ^ 0xFF);
//		}
//	}

//	while(count<7)
//	{
//		buf[postn] = 0;    //prefill leading spaces with spaces (zeros)
//		count++;
//		postn--;
//	}
//	int scan_code = 0;
//	count = postn;
//	while (postn > -1)//fill the rest of the buffer with the code pattern.
//	{
//		scan_code =  To_pattern( ToInt( num[count-postn] ) );
//		if(settings_stream1[1].keypad__  == LAFNG18_K)
//		{
//			buf2[postn] = (scan_code ^ 0xFF);
//		}
//		else
//		{
//			buf2[postn] = scan_code;     //128+64+32+16+8+4+2+1; //buf2[cnv-i];
//		}
//		postn--;
//	}
	refresh:
 //===========================================================================
 // HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_RESET); //Latch pin low
 // move the buf[] array into the shift register.

//	if (pump_type == lafeng) disp_len_pad2 = 5;
//	if (pump_type == DN_LAFNG17K) disp_len_pad2 = 5;
	if (settings_stream1[1].keypad__ == LAFNG17_K)
	{
	    	disp_len_pad2 = 5;
	}
    else if(settings_stream1[1].keypad__  == LAFNG18_K)
    {
    	disp_len_pad2 = 7;
    }

	for(int i = 0; i < disp_len_pad2; i++)  //7 bytes for bluesky 5 bytes for lafeng
	{
		shift_2(buf2[i], 0);
	}

	//push in "1"s past the keyboard registers memory of 24-1 places
	HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);

	int8_t fill = 24;
//if(pump_type == lafeng) fill = 16;
//if(pump_type == DN_LAFNG17K)
	if ( (settings_stream1[1].keypad__ == LAFNG17_K) || (settings_stream1[1].keypad__ == LAFNG18_K) )
		fill = 16;

	for(int i = 1; i < fill; i++)
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
    //--------------------	HAL_GPIO_WritePin(lthk1_GPIO_Port, lthk1_Pin, GPIO_PIN_SET);  //latch pin high

    HAL_GPIO_TogglePin(refresh2_GPIO_Port, refresh2_Pin);

//==========================================================================
        if(fxn == 2) return;     // return if fxn 2 selected (screen only..)
//==========================================================================

    HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);
    for(int i = 0; i < 5; i++)
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
	        HAL_Delay(1);
	        return keynumber;
	   }

   fill = 22;
//   if(pump_type == lafeng) fill = 17;
//   if(pump_type == DN_LAFNG17K)
   if ( (settings_stream1[1].keypad__ == LAFNG17_K) || (settings_stream1[1].keypad__ == LAFNG18_K) )
	   fill = 17;

   HAL_GPIO_WritePin(datak2_GPIO_Port, datak2_Pin, GPIO_PIN_SET);

   for(int i = 2; i < fill; i++)
	{
		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_SET);
		//_Delay(1);
		HAL_GPIO_WritePin(clkk2_GPIO_Port, clkk2_Pin, GPIO_PIN_RESET);
		//_Delay(1);
		if( checkkey2() == 1)
			{
				keynumber = i;
				HAL_Delay(1);
				return keynumber;
			}
	}

	//pushout the remaining zero from the keyboard registers.
//	HAL_GPIO_WritePin(datak1_GPIO_Port, datak1_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_SET);
//	_Delay(1);
//		  HAL_GPIO_WritePin(clkk1_GPIO_Port, clkk1_Pin, GPIO_PIN_RESET);
//	_Delay(1);
	return keynumber;
}

