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

char vbuf[MAX_STRLEN]  = {0};
char vbuf2[MAX_STRLEN]  = {0};

char SerialData[MAX_STRLEN] = {0};

uint32_t pulser1 = 0;
uint32_t pulser2 = 0;

drive drive1,
	  drive2;

//extern  displaytype  disp_type1;
//extern  displaytype  disp_type2;
extern pump disp_type1,
            disp_type2;

extern ADC_HandleTypeDef hadc1;
extern uint16_t motor_tmr,
				motor_tmr2;

void drive_motor1(drive drv);
void drive_slow_sole1(drive drv);
void drive_fast_sole1(drive drv);

void drive_motor2(drive drv);
void drive_slow_sole2(drive drv);
void drive_fast_sole2(drive drv);


//==========================   POWER  =============================================
int readpwr(void)
{
	 return(  (HAL_GPIO_ReadPin(sense_pwr_GPIO_Port, sense_pwr_Pin ) == 1)? 0:1 );
}

int read_p_pwr(void)
{
	 return(  (HAL_GPIO_ReadPin(sense_p_pwr_GPIO_Port, sense_p_pwr_Pin ) == 1)? 0:1 );
}

void drive_power(drive drv)
{
//	if(drv == ACTIVATE)
//	 HAL_GPIO_WritePin(pwr_actv_GPIO_Port,pwr_actv_Pin, 0);
//	else
	 HAL_GPIO_WritePin(pwr_actv_GPIO_Port,pwr_actv_Pin, drv);
}

float battery_read(void)
{
	uint16_t digital_reading;
	float batt_v;

	HAL_ADC_Start(&hadc1); // start A/D conversion
	if(HAL_ADC_PollForConversion(&hadc1, 500) == HAL_OK) //check if conversion is completed & 500ms Timeout
	{
		digital_reading  = HAL_ADC_GetValue(&hadc1); // read digital value and save it inside uint32_t variable
	}
	HAL_ADC_Stop(&hadc1); // stop conversion
	HAL_Delay(100);

	batt_v = ( (digital_reading * 3.2 ) / 4095 );   //no_battery => 2.23V (@ 7.9V) low_cutoff => 1.81 (@ 6.4V)
    return batt_v;
}
//=================================================================================
///                           HARDWARE  PERIPHERALS.
//=================================================================================
int readNozzle1(void)
{
    return(  (HAL_GPIO_ReadPin(nozzleSW_GPIO_Port, nozzleSW_Pin ) == 1)? 0:1  );
}

int readNozzle2(void)
{
    return(  (HAL_GPIO_ReadPin(nozzleSW2_GPIO_Port, nozzleSW2_Pin ) == 1)? 0:1  );
}
//------------------------------------
int readtotaliser1_state(void)
{
    return(  (HAL_GPIO_ReadPin(T1sense_GPIO_Port, T1sense_Pin ) == 1)? 1:0  );
}

void drive_totaliser1(drive drv)
{
	 drive1 = drv;
	 HAL_GPIO_WritePin(T1output_GPIO_Port,T1output_Pin, drv);
}
//------------------------------------
int readtotaliser2_state(void)
{
    return(  (HAL_GPIO_ReadPin(T2sense_GPIO_Port, T2sense_Pin ) == 1)? 1:0  );
}

void drive_totaliser2(drive drv)
{
	 HAL_GPIO_WritePin(T2output_GPIO_Port,T2output_Pin, drv);
}
//------------------------------------
int readsettingskey_state(void)
{
    return(  (HAL_GPIO_ReadPin(keyk1_GPIO_Port, keyk1_Pin ) == 0)? 1:0  );
}

int readkey19_state(void)
{
    return(  (HAL_GPIO_ReadPin(pin19K1_GPIO_Port, pin19K1_Pin ) == 0)? 1:0  );
}
//--------------------------------------
int readsettingskey2_state(void)
{
    return(  (HAL_GPIO_ReadPin(keyk2_GPIO_Port, keyk2_Pin ) == 0)? 1:0  );
}

int readkey192_state(void)
{
    return(  (HAL_GPIO_ReadPin(pin19K2_GPIO_Port, pin19K2_Pin ) == 0)? 1:0  );
}
//-------------------------------------------------------------------------------
void slow_flow(void)
{
	if (motor_tmr >= 2000)
		drive_motor1(ACTIVATE);

	drive_slow_sole1(ACTIVATE);
	drive_fast_sole1(DEACTIVATE);
}

void fast_flow(void)
{
	if (motor_tmr >= 2000)
		drive_motor1(ACTIVATE);

	drive_slow_sole1(ACTIVATE);
	drive_fast_sole1(ACTIVATE);
}

void stop_flow(void)
{
	drive_motor1(DEACTIVATE);
	drive_slow_sole1(DEACTIVATE);
	drive_fast_sole1(DEACTIVATE);
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
void drive_motor1(drive drv)
{
	 HAL_GPIO_WritePin(motor_drv1_GPIO_Port,motor_drv1_Pin, drv);
}
//---------------------------------------------------------------
void drive_motor2(drive drv)
{
	 HAL_GPIO_WritePin(motor_drv2_GPIO_Port,motor_drv2_Pin, drv);
}
//---------------------------------------------------------------
void drive_slow_sole1(drive drv)
{
	 HAL_GPIO_WritePin(slow_sole1_GPIO_Port,slow_sole1_Pin, drv);
}
//---------------------------------------------------------------
void drive_slow_sole2(drive drv)
{
	 HAL_GPIO_WritePin(slow_sole2_GPIO_Port,slow_sole2_Pin, drv);
}
//---------------------------------------------------------------
void drive_fast_sole1(drive drv)
{
	 HAL_GPIO_WritePin(fast_sole1_GPIO_Port,fast_sole1_Pin, drv);
}
//---------------------------------------------------------------
void drive_fast_sole2(drive drv)
{
	 HAL_GPIO_WritePin(fast_sole2_GPIO_Port,fast_sole2_Pin, drv);
}
//---------------------------------------------------------------

 void clear_screen()
  {
     for(int i = 0; i < 22;i++)
     {
       vbuf[i] = 0;     //initialise buffer with spaces
  	   shiftOut(0,1);
     }
  }
//---------------------------------------------
 void clear_screen2()
  {
     for(int i = 0; i < 22;i++)
     {
       vbuf2[i] = 0;     			//initialise buffer with spaces
  	   shiftOut2(0,1);
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
void shiftOut(uint8_t data_byte,uint8_t lat)
{
	//Latch pin low
	HAL_GPIO_WritePin(latchPin_GPIO_Port, latchPin_Pin, GPIO_PIN_RESET);

	for (int i = 0; i < 8; i++)
	{
		//clock pin low
		HAL_GPIO_WritePin(clockPin_GPIO_Port, clockPin_Pin, GPIO_PIN_RESET);
		// delay 1 ms
		_Delay(1);
		// check first bit, second bit ...
		//if high set dp high
//      if(disp_type1 == BLSKY886_N )
      if( (disp_type1 == DN_BLSKY18K ) || (disp_type1 == DN_BLSKY22 ) )
      {
			if ( byteRead(data_byte, i) == 0 )
				HAL_GPIO_WritePin(dataPin_GPIO_Port, dataPin_Pin, GPIO_PIN_SET);
			else	//if low set dp low
				HAL_GPIO_WritePin(dataPin_GPIO_Port, dataPin_Pin, GPIO_PIN_RESET);

			//set clock pin high
			HAL_GPIO_WritePin(clockPin_GPIO_Port, clockPin_Pin, GPIO_PIN_SET);
      }
//      else if(disp_type1 == LAFNG885 )
      else if(disp_type1 == DN_LAFNG17K)
         {
   			if ( byteRead(data_byte, i) == 1 )
   				HAL_GPIO_WritePin(dataPin_GPIO_Port, dataPin_Pin, GPIO_PIN_SET);
   			else	//if low set dp low
   				HAL_GPIO_WritePin(dataPin_GPIO_Port, dataPin_Pin, GPIO_PIN_RESET);

   			//set clock pin high
   			HAL_GPIO_WritePin(clockPin_GPIO_Port, clockPin_Pin, GPIO_PIN_SET);
         }
		//delay 1 ms
		_Delay(1);
	}

  if(lat == 1)
  {
   	HAL_GPIO_WritePin(latchPin_GPIO_Port, latchPin_Pin, GPIO_PIN_SET);  //latch pin high
   	_Delay(1);
    HAL_GPIO_WritePin(latchPin_GPIO_Port, latchPin_Pin, GPIO_PIN_RESET);  //Latch pin low
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
      if( (disp_type2 == DN_BLSKY18K) || (disp_type2 == DN_BLSKY22) )
      {
			if ( byteRead2(data_byte, i) == 0 )
				HAL_GPIO_WritePin(dataPin2_GPIO_Port, dataPin2_Pin, GPIO_PIN_SET);
			else	//if low set dp low
				HAL_GPIO_WritePin(dataPin2_GPIO_Port, dataPin2_Pin, GPIO_PIN_RESET);

			//set clock pin high
			HAL_GPIO_WritePin(clockPin2_GPIO_Port, clockPin2_Pin, GPIO_PIN_SET);
      }
//      else if(disp_type2 == LAFNG885 )
    	 else if(disp_type2 == DN_LAFNG17K)
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
		 case  'a': return(0b11101110);
	   	 case  'b': return(0b00111110);
	 	 case  'c': return(0b10011100);
	 	 case  'd': return(0b01111010);
		 case  'e': return(0b10011110);
		 case  'f': return(0b10001110);
		 case  'g': return 0b11110110;
		 case  'h': return 0b00101110;
		 case  'i': return(0b00001100);
		 case  'I': return(0b00001000);
		 case  'j': return(0b01110000);
		 case  'l': return(0b00011100);
         case  'n': return(0b11101100);
         case  'o': return(0b11111100);
         case  'O': return(0b00111010);
         case  'p': return(0b11001110);
         case  'r': return(0b10001100);
         case  's': return 0b10110110;
         case  't': return(0b00011110);
	     case  'u': return(0b01111100);
	     case  'v': return(0b01111100);
	     case  'y': return 0b01100110;
	     case  'Y': return(0b01110110);
	     case  '.': return(0b00000001);
	     case  '-': return(0b00000010);
	     case  '_': return(0b00010000);
	     default: return 0;   //0b11111100;
	}

}

uint8_t ToInt(char c)
{
	return (0xff & c);
}

//===========================================================================================
int32_t Multiplex( int row, int col) //char *Data_in,
{
     uint8_t intToByte = 0;
	 uint8_t   data_l = 22;

    //----------------------------------------
	for (int i = 0; i < data_l; i++)
	{
    	intToByte =  vbuf[i];   //ToBits( ToInt(Data_in[i]) );
    	shiftOut(intToByte,0);
	}

  HAL_GPIO_WritePin(latchPin_GPIO_Port, latchPin_Pin, GPIO_PIN_SET);  //latch pin high
   _Delay(1);
  HAL_GPIO_WritePin(latchPin_GPIO_Port, latchPin_Pin, GPIO_PIN_RESET);  //Latch pin low

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
void process(int isflt,char* str,int8_t cnv, int row, int col,int n ,int justify,int clear)
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

		if(strchr(str, '.') == 1)
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
		if(clear == 1) {  vbuf[temp2] = 0; }
		  temp1--;
		  temp2--;
		  count++; if (count > xtercount_max+1) { goto end; } // keep track of the xter.
		 if (isflt == 1) vbuf[temp2-1] = 0;
	  }

          if(justify == 1) {
          // ----------calculate X coordinate here--------------
          int x_ =  col;
          if (x_ > xtercount_max) x_ = xtercount_max;  //limit..
          //-----------   write initial spaces -----------------
		   while(x_ > 0)
			   {
				 if (clear == 1 ) vbuf[temp2] = 0;
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
            	   temp2++;                 // reverse the vbuf index.
              	   int buf_ =  vbuf[temp2]; // retrieve the pattern on the previous vbuf index and add 1 (dp)
            	   vbuf[temp2] = buf_+ 1;   // add decimal point and rewrite prevous digit scan code.;
            	   temp2--;                 // reference next index.
            	   continue;
              }
			   vbuf[temp2] = ToBits( ToInt( tmp ) );

			   temp2--;
			   temp1--;
			   count++;  if(count > xtercount_max-1) { goto end; }
		   }
         //---------------------------------------------------------------------------
         //clear the extra character   ---- when in left justify and clear = 0
          if ( (justify == 1)&&(clear == 0)&&(n>0)  )
          {
        	 int n_count = n - cnv;
             while(n_count > 0){
            	 n_count--;
            	 vbuf[temp2] = 0; temp2--;
            	 count++;  if(count > xtercount_max-1) { goto end; }
             }
          }
          ///////////////////////////////////////////

          if (  (justify == 1)&&(clear == 1)  )
			  {
        	     temp1 = diff;
					 while(temp1 > 0)
					   {
				         vbuf[temp2] = 0;
				    	   temp1--;
						   temp2--;
						   count++; if (count > xtercount_max-1) { goto end; }
					   }
			  }

end:
     Multiplex( row, col);   return;   //vbuf,

	//Multiplex("101213", row, col); return;

						HAL_GPIO_WritePin(latchPin_GPIO_Port, latchPin_Pin, GPIO_PIN_SET);  //latch pin high
						_Delay(1);
						HAL_GPIO_WritePin(latchPin_GPIO_Port, latchPin_Pin, GPIO_PIN_RESET);  //Latch pin low

}
//============================================================================================================

void process2(int isflt,char* str,int8_t cnv, int row, int col,int n ,int justify,int clear)
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

		if(strchr(str, '.') == 1)
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
				   temp2++;                 // reverse the vbuf index.
				   int buf_ =  vbuf2[temp2]; // retrieve the pattern on the previous vbuf index and add 1 (dp)
				   vbuf2[temp2] = buf_+ 1;   // add decimal point and rewrite prevous digit scan code.;
				   temp2--;                 // reference next index.
				   continue;

//				   while(temp1 > 0)
//		   {
//        	 int tmp = str[cnv - temp1];     // ToBits( ToInt(Data_in[i]) );
//             if(tmp == '.')                  // search  for decimal point.
//               {                             //
//            	   temp1--;                  // skip to the next xter.
//            	   temp2++;                  // reverse the vbuf index.
//              	   int buf_ =  vbuf2[temp2]; // retrieve the pattern on the previous vbuf index and add 1 (dp)
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
          if ( (justify == 1)&&(clear == 0)&&(n>0)  )
          {
        	 int n_count = n - cnv;
             while(n_count > 0){
            	 n_count--;
            	 vbuf2[temp2] = 0; temp2--;
            	 count++;  if(count > xtercount_max-1) { goto end; }
             }
          }
          ///////////////////////////////////////////

          if (  (justify == 1)&&(clear == 1)  )
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
     Multiplex2( row, col);   return;   //vbuf,

	//Multiplex("101213", row, col); return;

	HAL_GPIO_WritePin(latchPin2_GPIO_Port, latchPin2_Pin, GPIO_PIN_SET);  //latch pin high
	_Delay(1);
	HAL_GPIO_WritePin(latchPin2_GPIO_Port, latchPin2_Pin, GPIO_PIN_RESET);  //Latch pin low

}
//==============================================================================================
/* routines to write integer , float and string to the lcd  */
void printDisp_i(int32_t num, int row, int col,int n,justify_ jst,clear_ cl)
{
	  	char str_[10]= {0};
	   	int cnv = snprintf(str_, sizeof(str_), "%ld", num);
	   	process(0,str_,cnv,row,col,n,jst,cl);
}

void printDisp_c(char *num, int row, int col,int n,justify_ jst,clear_ cl )
{
	//char str_[10]= {0};
	//sprintf(str_, "%sc", num);
	//int cnv = snprintf(str_, sizeof(str), "%ld", num);
	int max_ = 8;
	if(row < 3) max_ = 10;
	int cnv = strlen(num);
	if(cnv > max_) cnv = max_;
 	process(0,num,cnv,row,col,n,1,cl);
}

void printDisp_f(float num, int row, int col,int n ,justify_ jst , clear_ cl)
{
	char str_[10]= {0};
	int cnv = snprintf(str_, sizeof(str_), "%.2f", num);

     //	int clear = 1;

        //justify for float is always left because of the decimals
	    //int justify = 0;
	    //int n = 4;
	process(1,str_,cnv,row,col,n,0,cl);
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
