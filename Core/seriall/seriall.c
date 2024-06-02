/*
 * seriall.c
 *
 *  Created on: Apr 25, 2023
 *      Author: KC
 */

#include "seriall.h"

char uart1_rx_buf[pump_rx_bufsize] = {0};
char uart2_rx_buf[pump_rx_bufsize] = {0};
char uart3_rx_buf[pump_rx_bufsize] = {0};
char uart5_rx_buf[pump_rx_bufsize] = {0};

extern char rx_buf[pump_rx_bufsize] = {0};

char nozz_id[5] = {0};
//	 statuss[10] = {0};



void config_rx(void)
 {
	  	static int16_t header_found = 0,
	  				   head_pos = 0;
	  	static int8_t curly_brace = 0;
	  	char tx;

	   int16_t rxcnt = pump_rx_bufsize - huart1.RxXferCount;
	   tx = (char)(uart1_rx_buf[rxcnt-1]);

	   if(tx == '{')     // header left square bracket 0x5B, 0d91   STX->0xA5
	   {
		  header_found = 1;
		  head_pos = 0;

		  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
			  rx_buf[head_pos] = tx;

		  head_pos++;
	   }

	   //-------------------------------------------------------
	   else if (header_found == 1)
	   {
		 //header found look for the footer...
		   if(tx == '}')
		   {
			   	 if(curly_brace != 0 )
			   	 {
			   		   if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
							rx_buf[head_pos] = tx;

					   curly_brace--;

					   head_pos++;
			   	 }
			   	 else
			   	 {
			   		 if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )

			   		 rx_buf[head_pos] = tx;

					 //  memcpy( pump_buf , pump_rx_buf[head_pos] ,rxcnt ); //transfer to the definitive buffer.
					 huart1.RxXferCount = pump_rx_bufsize;
					 huart1.pRxBuffPtr = &uart1_rx_buf[0]; //reset //uart2_rx_buf; //
					 header_found = 0;
					 head_pos = 0;
					 curly_brace = 0;
					 config_mode = 2;
					 config_found = 1;
					 return;
			   	 }
		   }
		   else if(tx == '{')  //ie. if another header character found
		   {
			   if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
					rx_buf[head_pos] = tx;

			   curly_brace++;

			   head_pos++;
		   }
		   else
		   {
			   if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
					rx_buf[head_pos] = tx;

			   head_pos++;
		   }

	   }
}



//
//		settings[0].display_ = bluesky886;
//		settings[0].keypad_ = bluesky12;
//		settings[0].product_ = pms;
//		settings[0].noz_id = p2;
//
////{"disp_type":"bluesky886",keypad_type":"bluesky12","prod_name":"pms","noz_id":"p2"}




void config_rx_parse(void)
 {
		int8_t head_pos = 0;
	  	int8_t pos = 0, id = 0, size;
	  	char rx;
        size = strlen(rx_buf);

        config_found = 2;

	   while( (id != 1) && (head_pos < size) )
	   {
		   if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
			   rx = rx_buf[head_pos];

//		   {"ni":"p17","pn":"pms","dt":"bluesky886n",kt":"bluesky22"}

		   if(rx == '{')     // header left square bracket 0x5B, 0d91   STX->0xA5
		   {
			  head_pos++;
			  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
				  rx = rx_buf[head_pos];

			  if(rx == '"')
			  {
				  head_pos++;
				  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
					  rx = rx_buf[head_pos];

				  if(rx == 'n')
				  {
					  head_pos++;
					  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
						  rx = rx_buf[head_pos];
					  if(rx == 'i')
					  {
						  head_pos += 4;
						  do
						  {
							  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
								  nozz_id[pos++] = rx_buf[head_pos];

							  head_pos++;

							  if( (head_pos < 0) || (head_pos >= pump_rx_bufsize) )
								  break;

						  }
						  while(rx_buf[head_pos+1] != ',');

						  id = 1;      //nozzle ID obtained
					  }
				  }
			  }

			}
		   head_pos++;
	   }

	   if(id == 1)
	   {
		   configure_pump(nozzle_name);
		   configure_pump(product_name);
		   configure_pump(disp_type);
		   configure_pump(keypad_type);
		   memset(rx_buf, 0, sizeof(rx_buf));
	   }
}





//void server_rx_parse(void)
// {
//		int head_pos = 0,
//			pos = 0,
//			size;
//
//	  	int8_t st = 0;
//	  	char rx;
//
//        size = strlen(rx_buf);
//
//        config_found = 2;
//
//	   while( (st != 1) && (head_pos < size) )
//	   {
//		   if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
//			   rx = rx_buf[head_pos];
//
//	//	   {"ni":"p1","pn":"pms","dt":"bluesky886",kt":"bluesky12"}
//
//		   if(rx == '{')     // header left square bracket 0x5B, 0d91   STX->0xA5
//		   {
//			  head_pos++;
//			  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
//				  rx = rx_buf[head_pos];
//
//			  if(rx == '"')
//			  {
//				  head_pos++;
//				  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
//					  rx = rx_buf[head_pos];
//
//				  if(rx == 's')
//				  {
//					  head_pos++;
//					  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
//						  rx = rx_buf[head_pos];
//					  if(rx == 't')
//					  {
//						  head_pos += 3;
//						  do
//						  {
//							  if( (head_pos >= 0) && (head_pos < pump_rx_bufsize) )
//								  statuss[pos++] = rx_buf[head_pos++];
//
////							  head_pos++;
//
//							  if( (head_pos < 0) || (head_pos >= pump_rx_bufsize) )
//								  break;
//
//						  }
//						  while(rx_buf[head_pos] != ',');
//
//						  st = 1;      //nozzle ID obtained
//					  }
//				  }
//			  }
//
//			}
//		   head_pos++;
//	   }
//
//	   if(st == 1)
//	   {
//		   char strA[12],
//		   	   	strB[12];
//
//		   snprintf(strA, sizeof(strA), "%ld", tranxA_token);
//		   snprintf(strB, sizeof(strB), "%ld", tranxB_token);
//
//		   if(strstr(rx_buf, strA))
//		   {
//			   config_mode = 0;
//			   save_synchedTransaction_sides(side_a);
//		   }
//		   else if(strstr(rx_buf, strB))
//		   {
//			   config_mode = 0;
//			   save_synchedTransaction_sides(side_b);
//		   }
////		   memset(rx_buf, 0, sizeof(rx_buf));
//	   }
//}




//void config_rx_parse2(void)
//{
//	   for (int8_t i = 0; i < 15; i++)
//	   {
////		   if(strstr(rx_buf,"p1"))
//		   ;
//	   }
//}



void configure_pump(pumpCompPart _case)
{
	switch(_case)
	{
		case nozzle_name:  if(strstr(nozz_id,"p1"))
	   	   	   	   	   	   {
								settings[0].noz_id = P1;
								settings[1].noz_id = P2;
	   	   	   	   	   	   }
						   else if(strstr(nozz_id,"p2"))
		   	   	   	   	   {
								settings[0].noz_id = P2;
								settings[1].noz_id = P3;
		   	   	   	   	   }
						   else if(strstr(nozz_id,"p3"))
		   	   	   	   	   {
								settings[0].noz_id = P3;
								settings[1].noz_id = P4;
		   	   	   	   	   }
						   else if(strstr(nozz_id,"p4"))
		   	   	   	   	   {
								settings[0].noz_id = P4;
								settings[1].noz_id = P5;
		   	   	   	   	   }
						   break;


		case product_name: if(strstr(rx_buf,"pms"))
		   	   	   	   	   {
								strcpy(settings[0].product_, "PMS");
								strcpy(settings[1].product_, "PMS");
		   	   	   	   	   }
						   else if(strstr(rx_buf,"dpk"))
		   	   	   	   	   {
								strcpy(settings[0].product_, "DPK");
								strcpy(settings[1].product_, "DPK");
		   	   	   	   	   }
						   else if(strstr(rx_buf,"ago"))
		   	   	   	   	   {
								strcpy(settings[0].product_, "AGO");
								strcpy(settings[1].product_, "AGO");
		   	   	   	   	   }
						   break;

		case disp_type:   if(strstr(rx_buf,"lafeng885"))
						   {
							settings[0].display__ = LAFNG885;
							settings[1].display__ = LAFNG885;
						   }
					   	   else if(strstr(rx_buf,"bluesky886n"))
						   {
							settings[0].display__ = BLSKY886_N;
							settings[1].display__ = BLSKY886_N;
						   }
					   	   else if(strstr(rx_buf,"bluesky886i"))
						   {
							settings[0].display__ = BLSKY886_IN;
							settings[1].display__ = BLSKY886_IN;
						   }
					   	   break;

		case keypad_type:  if(strstr(rx_buf,"lafeng17k"))
						   {
							settings[0].keypad__ = LAFNG17_K;
							settings[1].keypad__ = LAFNG17_K;
						   }
						   else if(strstr(rx_buf,"lafeng18k"))
						   {
							settings[0].keypad__ = LAFNG18_K;
							settings[1].keypad__ = LAFNG18_K;
						   }

						#ifdef DEV_MODE
//						   else if(strstr(rx_buf,"bluesky22"))
//						   {
//							settings[0].keypad__ = BLSKY22;
//							settings[1].keypad__ = LAFNG18_K;
//						   }
						#endif

					   	   else if(strstr(rx_buf,"bluesky18k"))
						   {
							settings[0].keypad__ = BLSKY18_K;
							settings[1].keypad__ = BLSKY18_K;
						   }
					   	   else if(strstr(rx_buf,"bluesky22"))
						   {
							settings[0].keypad__ = BLSKY22;
							settings[1].keypad__ = BLSKY22;
						   }
					   	   break;
	}
}

//{"ni":"p17","pn":"pms","dt":"bluesky886n",kt":"bluesky22"}

//Display Type
//"lafeng885" -> LAFNG885
//"bluesky886n" -> BLSKY886_N
//"bluesky886i" -> BLSKY886_IN;
//
//Keypad Type
//"lafeng17k" -> LAFNG17_K
//"bluesky18k" -> BLSKY18_K
//"bluesky22" -> BLSKY22
//
//{"ni":"p2","pn":"pms","dt":"bluesky886n",kt":"bluesky22"}


//typedef enum
//{
//  LAFNG17_K = 1,    //ie. 17 punch-keys + a turn-key
//  BLSKY18_K,		//ie. 18 punch-keys + a turn-key
//  BLSKY22			//ie. 22 punch-keys + no turn-key
//}keypadtype;
//
//
//typedef enum
//{
//  LAFNG885 = 1,
//  BLSKY886_N,           //N -> Normal
//  BLSKY886_IN			//IN -> Inverted
//}displaytype;
//
//
//typedef enum
//{
//  DN_LAFNG17K = 0b00000001,		//Display->Normal, keypad->LAFNG-17K
//  DN_BLSKY18K,       			//Display->Normal, keypad->BLSKY-18K
//  DN_BLSKY22,					//Display->Normal, keypad->BLSKY-22
//  DIN_BLSKY18K,       			//Display->Inverted, keypad->BLSKY-18K
//  DIN_BLSKY22					//Display->Inverted, keypad->BLSKY-22
//}pump;
//		settings[0].display_ = bluesky886;
//		settings[0].keypad_ = bluesky12;
//		settings[0].product_ = pms;
//		settings[0].noz_id = p2;

//{"disp_type":"bluesky886",keypad_type":"bluesky12","prod_name":"pms","noz_id":"p2"}
