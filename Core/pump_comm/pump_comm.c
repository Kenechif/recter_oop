/*
 * pump_comm.c
 *
 *  Created on: Oct 16, 2022
 *      Author: tunjo
 */

#include "main.h"
#include "stdbool.h"
#include "interface1.h"
#include "stdint.h"

#include "pump_comm.h"
#include "config.h"
#include "checksum.h"
//#include "pump_comm.h"
#include "math.h"

#include "interface_fxn1.h"


//pump_   pump[2];

extern uint8_t filling1;

uint8_t nozzle_bit = 0,
		stop_fueling_bit = 0,
		nozzle_bit2 = 0,
		stop_fueling_bit2 = 0;

uint8_t cmd_frame[150];
Name_address name_address;

char temp[15] = {0};

//extern pump_settings settings[2];

extern pump_settings_stream1 settings_stream1[2],
					 	 	 settings_config_stream1[2];

extern pump_settings_stream2 settings_stream2[2],
							 settings_config_stream2[2];

extern pump_settings_stream3 settings_stream3[2],
				       	     settings_config_stream3[2],
							 copy_stream3[2];

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

char pump_rx_buf[pump_rx_bufsize] = {0};
char pump_buf[pump_rx_bufsize] = {0};

bool pump_message_found = 0;
bool pump_msg_ready1 = 0;

//bool message_found2 = 0;
//bool parse_ready2 = 0;
//bool isDirty = 0;

extern config_struct config_data;
extern float amt,price;

//msgg  retMsg[2] , mainMessage;     //], retMsg2;
//rcv_struct  msg_str;

char my_message_[400] = {0};
//char msg_[400] = {0};

  //===========================================
extern  float totaliser_vol1;    // = 0.00;
extern	float totaliser_vol1c;   // = 0.00;
extern	float totaliser_vol2;    // = 0.00;
extern	float totaliser_vol2c;   // = 0.00;

extern float firstTotaliser_vol1,
			 firstTotaliser_vol1c,
			 firstTotaliser_vol2,
			 firstTotaliser_vol2c;

extern  float totaliser_amt1,
			  totaliser_amt1c,
			  totaliser_amt2,
			  totaliser_amt2c;


extern float auth_p1, auth_v1, set_p;
extern uint8_t  change_price, change_volume;


int8_t auth_from_ctrl = 0,
	   auth_from_ctrl2 = 0;
int8_t authorise_flag = 0;
int8_t change_price_flag;

float set_p = 0.0;

//=============================================

//int getState(int ind)
uint8_t sendState(void)
{
	// decimal2BCD( price *powr(10,2),4, cmd_frame[3]); //filled_amo = BCD2decimal(&d[0], 4, r_dpvol);

	sprintf(temp,"%.2f",price);
	memcpy(&cmd_frame[3],temp,10);

	//decimal2BCD( amt  *powr(10,2),4, cmd_frame[7]);  //filled_vol = BCD2decimal(&d[4], 4, r_dpamo);

	sprintf(temp,"%.2f",amt);
	memcpy(&cmd_frame[13],temp,10);

	uint8_t pump_status__ = 0;

	if(filling1 == 1)
	{
		pump_status__ = (pump_status__ | 0b00000010);
	}

	if(nozzle_bit == 1)
	{
		pump_status__ = (pump_status__ | 0b10000000);
	}

	if(stop_fueling_bit > 0)
	{
		pump_status__ = (pump_status__ |0b00000100); // 0b00000100);
	}

	return pump_status__;
}

//float getUnitPrice(int ind)
void getUnitPrice(void)
{
	memset(temp,0,15);
	memcpy(temp,pump_buf[4],10);
//	set_p = atoi(temp);
	set_p = atoff(temp);
}

void setUnitPrice(void)
{
	settings_stream1[0].price_ = set_p;
	settings_stream1[1].price_ = set_p;
	save_settings();   //save to eeprom
	load_settings(side_a); //load the settings into the internal variables
	load_settings(side_b);
}

//float getvol_totalizer(int ind)
void sendvol_totalizer(void)
{
	//decimal2BCD( totaliser_vol1 *powr(10,2),8, cmd_frame[3]); //vol_totalizer = BCD2decimal(&d[0], 8, r_dptol);
	sprintf(temp,"%.2f",totaliser_vol1);
	memcpy(&cmd_frame[3],temp,12);
}

//float getfilled_vol(int ind)
void getfilled_vol(void)
{
	//auth_v = BCD2decimal(&cmd_frame[3], 8, 2);
	memset(temp,0,15);
	memcpy(temp,pump_buf[4],10);
//	auth_v = atof(temp);
	auth_v1 = atoff(temp);

}


//float getFilledAmo(int ind)
void getFilledAmo(void)
{
	//auth_p = BCD2decimal(&cmd_frame[3], 8, 2);
	memset(temp,0,15);
    memcpy(temp,pump_buf[4],10);
//    auth_p = atof(temp);
    auth_p1 = atoff(temp);
}


//char* getName(int ind)
void getName(void)
{
	decimal2BCD( 1 *powr(10,2), 4, cmd_frame[3]);
}

void setMode(int ind,mode_enum md)
{

}

mode_enum getMode(int ind)
{
//	if()
//	{
//	 return MANUAL;
//	}
//	else if()
//	{
//	 return AUTO;
//	}
//	else if()
//	{
//	 return AUTO_PERMANENT;
//	}
}

setName(int ind,char* name) {}

setProduct(int ind,char* pd){}
char* getProduct(int ind){}

 int auth_by_vol(int ind,float vol){ return 0;}	    // authorize the pump's volume
 int auth_by_amo(int ind,float amnt){return 0;}	// authorize the pump's amount
 int set_amo(int ind,float amnt){return 0;}	    //
// void authorise(int ind){return 0;}	 // authorize pump with maximum vol MAX_PRESET_VOL

 int8_t controller_authorise(void)
 {
	 auth_from_ctrl = 1;
	 if(auth_from_ctrl == 1)
		 return 1;
	 else
		 return 0;
 }

 void stop(int ind) {}
 setIndex(int ind,	int i ) {}
 int getIndex(int ind) {}

//=====================================================================

 //==========================================================================
 void pump_ini()
 {
	 pump_status_1 = STATUS_PNP;
	 //clear the buffers
     memset(name_address.name1[0],0,sizeof(name_address) );
 	 HAL_UART_Receive_IT(&huart2, (uint8_t*)pump_rx_buf, pump_rx_bufsize );
 }
 //==========================================================================
 void pump_rx()
 {
 	static int header_found = 0;
 	static head_pos = 0;

      int rxcnt = pump_rx_bufsize - huart2.RxXferCount ;
      int tx = pump_rx_buf[rxcnt-1];

      if(tx == STX)     // header left square bracket 0x5B, 0d91   STX->0xA5
	  {
		  header_found = 1;
		  head_pos = 0;
		  pump_buf[head_pos] = tx;
		  head_pos++;
	  }

//      if ( (header_found != 1) )
//      {
// //    	  huart3.RxXferCount = nextion_rx_bufsize;
// //    	  huart3.pRxBuffPtr = &nextion1_rx_buf[0]; //reset
// //    	  rxcnt = 0;
//      }
     //-------------------------------------------------------
      if ( (header_found == 1) )
          {
    	     pump_buf[head_pos] = tx;

     	   if(rxcnt >= pump_rx_bufsize - 10) //if over receive
     	   {
     	    	 huart2.RxXferCount = pump_rx_bufsize;
     	    	 huart2.pRxBuffPtr = pump_rx_buf[0];   //reset
     	    	 rxcnt = 0;
     	    	 header_found = 0;
                 return;
     	   }
     	 	 //header found lookmfor the footer...
     	   if(tx == rx_STX)
 			{
     		  pump_buf[head_pos] = rx_STX;
     	  	  pump_buf[head_pos++] = 0;
 			   header_found = 0;
 			 //  memcpy( pump_buf , pump_rx_buf[head_pos] ,rxcnt ); //transfer to the definitive buffer.
 			   huart2.RxXferCount = pump_rx_bufsize;
 				 huart2.pRxBuffPtr = pump_rx_buf[0]; //reset
 				 rxcnt = 0;
 				 header_found = 0;
 				 pump_message_found = 1;
 				 return;
 			}

         }
      head_pos++;
      //--------------------------------------------------------
           if ( (rxcnt >= 20)&&(header_found != 2) )
           {
          	 huart2.RxXferCount = pump_rx_bufsize;
          	 huart2.pRxBuffPtr = pump_rx_buf[0]; //reset
          	 rxcnt = 0;
          	 head_pos = 0;
           }
      //--------------------------------------------------------
 }





//=========================================================================
 uint8_t get_checksum(const uint8_t* data, int len)
 {
     uint8_t checksum = 0;
     for(int i = 0; i < len; i++)
     {
         checksum += data[i];
     }
     return ((~checksum) + 1)& 0xff ;
 }
//==========================================================================
 /*********************************************************
  *	BCD converts from binary coded decimal to decimal
  * in -> value in decimal to convert to array, MSB = index 0
  * in2 -> total size of final bcd
  * out <- pointer to array of converted int
  * return <- no. of bytes converted
  *********************************************************/
 int decimal2BCD (int Decimal, int size, unsigned char* res2)
 {
 	memset(res2,0,size);
 	//unsigned char res2[10] = {0};
 	unsigned char res1[10] ={0};
 	//unsigned char res2[10] ={0};
 	int count = 0;
 	while (Decimal!=0 && (count < sizeof(res1))){
 		res1[count] = ((((Decimal/10)%10) << 4) | (Decimal % 10));
 		Decimal/=100;
 		count++;
 	}
 	//int j = si;
 	for (int i=0; i<count; i++){
 		res2[size-1] = res1[i];
 		if (--size <= 0) break;
 	}
 	return count;
 }
 //========================================================================
 unsigned long BCD2decimal_(uint8_t* array, u32 size)
 {
 	unsigned long result = 0;
 	for (int i = ((size*2)-1); i > -1; i-=2){
 		unsigned long a = (long unsigned)powr(10,i-1);
 		unsigned long b = (long unsigned)powr(10,i);
 		result += (((*array & 0xf0) >> 4) * b);
 		result += ((*array & 0x0f) * a);
 		//		TRACE_BP("debug naani: in result: %lu [%d] array: %x a: %lu, b: %lu   cal: %lu", result, result, *array, a, b, (((*array & 0xf0) >> 4) * b));
 		array++;
 	}
 	return result;
 }
//=========================================================================
 int BCD2decimal(unsigned char* array, u32 size, u32 dp)
 {
 	//type indicates a special effect
 	double result = 0;
 	unsigned long int_result = 0;
 	int_result = BCD2decimal_(array, size);
 	//	TRACE_BP("debug naani: int_result from bsd2decimal main: %lu", int_result);
 	if(dp!= 0)
 		result = d2f(int_result, dp);
 	else
 		result = int_result;
 	//	TRACE_BP("debug after d2f: int_result from bsd2decimal main: %lf", result);
 	return result;
 }
//===========================================================================
 int powr(int a, int b){
 	long unsigned r=1;
 	for (int i=0; i<b; i++){
 		r*=a;
 	}
 	return r;
 }
//==========================================================================
 /* This converts from decimal to float as required by the pump */
 int d2f(long unsigned dec, int dp)
 {
 	double i = powr(10,dp);
 	double j = dec / i;
 	                       //printf ("new float : %.2f \n", j);
 	return j;
 }
 //===========================================================================
//void  make_message(int pmp_no,int cmdd)
//{
//	    cmd_frame[0] = rx_STX;
//		cmd_frame[1] = pmp_no;
//		cmd_frame[2] = cmdd;
//
//
//
//		//memcpy(&cmd_frame[3], &d[1],10);
//			/** NOTE: the checksum calculation includes the DATA1
//			 * However, since it involves just adding all characters
//			 * the order doesn't really matter */
//
//			cmd_frame[LANFENG_SIZE_OF_FRAME-1] = get_checksum(&cmd_frame[1], 11);
//}
//============================================================================
 uint8_t msg_parse_pump(char* str)
 {
	uint8_t head_pos = 0;

//	char di[17] = {0};
//	char msg[200]={0};              //string msg = "";
//	char
	uint8_t tail = 0;
//	char
	uint8_t headr;
	//char crc[3] = {0};
//	uint8_t 	 crc_rX_8 = 0;


    uint8_t  rx_crc = 0;
	int pump_no = 0;
	uint8_t cmd = 0;
	uint8_t indx_tail = 0;
//------------------------------------------
//	char buf_crc[150] = {0};

	//message retMsg ;
if(pump_buf[0] == 0) pump_buf[0] = 1;
	int str_len = strlen(pump_buf);


	if (pump_buf[2] != 2)   return;

//	if(str_len < 3)
//	{
//		//eat_trace("Len = %d size is below 19",str_len);
//		return fail;
//	}

	int indx = 0;
	while(indx < str_len)
	{
		if(pump_buf[indx] != STX)
		{
			indx++;
		}
		else
		{
			//eat_trace("got header @ %d ",indx);
			head_pos = indx+1;
			headr = STX;
			goto got_header;
		}
	}

	indx++;   //advance index
	//eat_trace("Header not found! ");
	//retMsg.isDirty = true;
	return fail;
got_header:


got_tail:
   // uint8_t j = 0;
   // imei
   // for(int indx_ = 0;indx_<15;indx_++)
indx++;
//=========================================================================
int indx_ = 0;

int pmp_no = 2;

    memset(cmd_frame,0,sizeof(cmd_frame));

    cmd_frame[0] = rx_STX;
  	cmd_frame[1] = pmp_no;
  	cmd_frame[2] = pump_buf[3]; //indx_tail+2];      //cmd;
  	cmd = str[3];

//  	char temp[15] = {0};

  	uint8_t ln = 0;

   switch (cmd)
    {
        case GET_STATE:
        	//--------------------------------------------------------------
			cmd_frame[23] = sendState();        //pump_status__;     //r_status = d[8];
			cmd_frame[24] = get_checksum(&cmd_frame[1], 23);
			ln = 25;
        	break;

    	case QRY_TOTAL_CASH:
    		  //amo_totalizer = BCD2decimal(&d[0], 8, r_dptol);
    		break;

    	case QRY_TOTAL_LITER:
    		sendvol_totalizer();
    		cmd_frame[15] = get_checksum(&cmd_frame[1], 14);
    		ln = 16;
    		break;
    	//=======================================================================================
    	case F_WX_FX_CASH :  	/**< Filling with fixed cash (Preset amount: 9999.99) */
    	    getFilledAmo();
    		cmd_frame[13] = get_checksum(&cmd_frame[1], 12);
//    		change_price = 1;
       		change_p1 = 1;
       		controller_authorise();
    		break;

    	case F_WX_FX_LITER:
    		getfilled_vol();
    		cmd_frame[13] = get_checksum(&cmd_frame[1], 12);
//    		change_volume = 1;
    		change_v1 = 1;
    		controller_authorise();
    		break;

    	case SETTING_PRICE:
    		getUnitPrice();
    		cmd_frame[13] = get_checksum(&cmd_frame[1], 12);
    		//set_p = BCD2decimal(&cmd_frame[3], 8, 2);
    		change_price_flag = 1;
    		//cmd_frame[13] = get_checksum(&cmd_frame[1], 11);
    		break;

    	case CONTROL_DISPENSER:
    		getName();
    		cmd_frame[13] = get_checksum(&cmd_frame[1], 11);
    		break;

		default:
			break;
    }

     HAL_UART_Transmit(&huart2, (uint8_t*)cmd_frame, ln,3000);

}
