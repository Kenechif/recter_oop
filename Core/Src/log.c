/*
 * log.c
 *
 *  Created on: 27 Apr 2022
 *      Author: tunjo
 */
#include "settings.h"
#include "states.h"
#include "../flash_mem/w25qxxConf.h"
#include "../flash_mem/w25qxx.h"

#include "config.h"
#include "pump_comm.h"

#include "log.h"

#define W25QXX_DUMMY_BYTE 0xA5

extern SPI_HandleTypeDef _W25QXX_SPI;

#define W25qxx_Delay(delay) HAL_Delay(delay)


extern UART_HandleTypeDef huart2;

extern const char device_id [];

extern char session_id[9];

extern float litre_price1;
extern int8_t self->opmode,
			  opmode2;
//			  connected = 0;

extern uint8_t connected = 0;

extern uint32_t self->transaction_period,
				transaction_period2;

extern uint8_t firstTime_filling1,
			   firstTime_filling2;

extern uint8_t mamo_reached_flag1,
			   self->mamo_reached_flag1_1,
			   self->filling_mamo_flag = 0,
			   self->stopFlag_source = 0,
			   self->nozzleDown_source = 0,
			   reset_flag1 = 0,
			   self->hardwareError_flag = 0,
			   self->hardwareErrorFlag_source = 0,
			   self->go_timeOut = 0,
			   self->mamo_fillingInfo_send = 0;

extern uint8_t mamo_reached_flag2,
			   mamo_reached_flag2_1,
			   filling_mamo_flag2 = 0,
			   stopFlag_source2 = 0,
			   nozzleDown_source2 = 0,
			   reset_flag2 = 0,
			   hardwareError_flag2 = 0,
			   hardwareErrorFlag_source2 = 0,
			   go_timeOut2 = 0,
			   mamo_fillingInfo_send2 = 0;




uint32_t flash_write_id;    //read and write.
uint32_t flash_read_id;     //address of the flash.

  wr_buffer;
  rd_buffer;

//int log_wrt_ptrA,log_wrt_ptrB = 0;

int log_memb = 0;
const int max = 50-1;

//extern RTC_DateTypeDef gDate;
//extern RTC_TimeTypeDef gTime;

//
//extern log_new self->log_new;
//extern log_new log_b_new;

extern pump_status_enum self->pump_status,
						pump_status_2;

extern log_new self->log_new,
			   log_b_new,
			   synchedLog_a_new,
			   synchedLog_b_new;

//extern log_new1 log_a_new1,
//				log_b_new1;

//=== screen Arrays ====
extern char upper1[10],
			middle1[10],
			upper2[10],
			middle2[10];

extern const uint32_t self->flash_begin;
extern const uint32_t self->flash_end;

extern const uint32_t flash_beginB;
extern const uint32_t flash_endB;

extern flash_store_info self->flash_info,flash_infoB;

extern const int flash_info_sto;
extern const int flash_stoA;
extern const int flash_stoB;

extern const uint16_t self->flash_sto_fram,
					  flash_stoB_fram;

uint32_t self->flash_read_id = 0;
uint32_t flash_read_idB = 0;

extern int operating_side;
extern float price_real1, price_real2,
			 amt_real1, amt_real2,
			 price, price2,
			 amt, amt2,
//			 self->totaliser_vol, totaliser_vol1c,
//			 totaliser_vol2, totaliser_vol2c,
			 totaliser_amt1, totaliser_amt1c,
			 totaliser_amt2, totaliser_amt2c,
			 priceOld1,
			 priceOld2;

extern float self->totaliser_vol,
			 totaliser_vol1c,
			 totaliser_vol2,
			 totaliser_vol2c;

extern float firstTotaliser_vol1,
			 firstTotaliser_vol1c,
			 firstTotaliser_vol2,
			 firstTotaliser_vol2c;

extern totaliser_store totaliser_vol_storeA,
						totaliser_vol_storeB,
						otaliser_amt_storeA,
						totaliser_amt_storeB;

extern uint8_t hour,
			   minute,
			   second,
			   day,
			   month,
			   year,
			   dayofweek;

extern time_ timeA, timeB;
extern date_ dateA, dateB;

extern uint8_t _litre_price1,
			  _litre_price2,
			  _pump_max_litres1,
			  _pump_max_litres2;

//extern ep_ epp;

//==================================================
void flash_info_read()
{
	EEPROM_Read(flash_info_sto, flash_stoA, &self->flash_info, sizeof(self->flash_info));
	  // log_wrt_ptrA = self->flash_info.current_loc;
	EEPROM_Read(flash_info_sto, flash_stoB, &flash_infoB, sizeof(flash_infoB));
  // log_wrt_ptrB = flash_infoB.current_loc;
}

void flash_info_read_fram()
{
	FRAM_Read(self->flash_sto_fram, &self->flash_info, sizeof(self->flash_info));
	FRAM_Read(flash_stoB_fram, &flash_infoB, sizeof(flash_infoB));
}

//==================================================
void update_info()       //save_log( )
{
   static int in_use = 1;
   if(operating_side == side_a)
    {
      //  if(log_wrt_ptrA > max)
      //  {
      //  	log_wrt_ptrA = 0;   //wrap write pointer.
      //  }
        //====================================
        //        price and volume

//	    memset(self->log_new.pr__ , '0', sizeof(self->log_new.pr__) ); //calibrated
//	    memset(self->log_new.vol__ , '0', sizeof(self->log_new.pr__) );
//	    strncpy(self->log_new.pr__ , upper1, sizeof(self->log_new.pr__) );
//	    strncpy(self->log_new.vol__ , middle1, sizeof(self->log_new.vol__) );

	   char *endPtr;

	    self->log_new.pr__ = strtof(upper1, &endPtr);  //calibrated
//	    self->log_new.pr__  += 0.00011;  //make small correction for the inherent rounddown.
	    self->log_new.vol__ = strtof(middle1, &endPtr);
//	    self->log_new.vol__ += 0.00011;  //make small correction for the inherent rounddown.

	    self->log_new.pr_ = price_real1;  //real
        self->log_new.vol_ = amt_real1;
//        self->log_new.pr__ = price;  //calibrated
//        self->log_new.vol__ = amt;
       // self->log_new.pr_d = 0; //price_;
       // self->log_new.pr_d = 0; //amt_;

        //=====================================
        //=========    totaliser   ==========
		 self->log_new.totaliserVol_real =  self->totaliser_vol;  //totaliser_vol_storeA.totaliserVol_real; //self->totaliser_vol;
		 self->log_new.totaliserVol_cal  =  totaliser_vol1c; //totaliser_vol_storeA.totaliserVol_cal;  //totaliser_vol2;

		 self->log_new.totaliserAmount_real =  totaliser_amt1;  //totaliser_vol_storeA.totaliserVol_real; //self->totaliser_vol;
		 self->log_new.totaliserAmount_cal  =  totaliser_amt1c; //totaliser_vol_storeA.totaliserVol_cal;  //totaliser_vol2;

        //=====================================
        //==========   date ==========
         self->log_new.time_e._hh = hour;
         self->log_new.time_e._mn = minute;

         //=========  time  ==========
         self->log_new.date._yy = year;
         self->log_new.date._mm = month;
         self->log_new.date._dd = day;

         self->log_new.litre_price_ = litre_price1;

       // log_wrt_ptrA +=1 ;
    }

   if(operating_side == side_b)
       {
           //if(log_wrt_ptrB > max)
          // {
          // 	log_wrt_ptrB = 0;   //wrap write pointer.
          // }
           //====================================
           //        price and volume
           log_b_new.pr_ = price_real2;  //real
           log_b_new.vol_ = amt_real2;
//           log_b_new.pr__ = price2;  //calibrated
//           log_b_new.vol__ = amt2;

           char *endPtr;

		   log_b_new.pr__ = strtof(upper2, &endPtr);  //calibrated
//		   log_b_new.pr__  += 0.00011;  //make small correction for the inherent rounddown.
		   log_b_new.vol__ = strtof(middle2, &endPtr);
//		   log_b_new.vol__ += 0.00011;  //make small correction for the inherent rounddown.


          // self->log_new.pr_d = 0; //price_;
          // self->log_new.pr_d = 0; //amt_;

           //=====================================
           //=========    totaliser   ==========
           log_b_new.totaliserVol_real = totaliser_vol2;  //totaliser_vol_storeB.totaliserVol_real; //self->totaliser_vol;
           log_b_new.totaliserVol_cal  = totaliser_vol2c; //totaliser_vol_storeB.totaliserVol_cal;  //totaliser_vol2;

           log_b_new.totaliserAmount_real = totaliser_amt2;  //totaliser_vol_storeB.totaliserVol_real; //self->totaliser_vol;
		   log_b_new.totaliserAmount_cal  = totaliser_amt2c; //totaliser_vol_storeB.totaliserVol_cal;  //totaliser_vol2;

           //=====================================
           //==========   date ==========
            log_b_new.time_e._hh = hour;
            log_b_new.time_e._mn = minute;

            //=========  time  ==========
            log_b_new.date._yy = year;
            log_b_new.date._mm = month;
            log_b_new.date._dd = day;

            log_b_new.litre_price_ = litre_price2;

          // log_wrt_ptrB +=1 ;
       }
}

//---------------------------------------------------
//===================================================
//             flash memory states.
//ooooooooooooooooooooooooooooooooooooooooooooooooooo
eSystemState read_flash_State_Handler(void)
{
	static int self->flshr = 0;
	uint32_t flash_read_id_ = 0;
	static int self->prev_state_sto = 0;

	uint32_t pg = 0;

 if (operating_side == side_a)
 {
	 pg = self->flash_read_id/w25qxx.PageSize;
     W25qxx_ReadPage(&self->log_new,  pg, 0, sizeof(self->log_new) );
 }
 else
 {
	 pg = flash_read_idB/w25qxx.PageSize;
     W25qxx_ReadPage(&log_b_new,  pg, 0, sizeof(log_b_new) );
 }

 	 return operator_State;

//-----------------------------------------------------------

		if(self->flshr == 0)
		{
		   while (w25qxx.Lock == 1) return read_flash_State;   //wait for pending job
		   w25qxx.Lock = 1;  // lock access to flash mem. operations.
		   self->prev_state_sto = ePrevState;
		   //------------------------------------------------------
		   //   restrict the reading addresses to within each zones.
		  		  if (operating_side == side_a)
		  			   {
		  			     flash_read_id_ = self->flash_read_id;   //assign the read address.
		  			     if(flash_read_id_ > self->flash_end)   flash_read_id_ = self->flash_end;
		  			     if(flash_read_id_ < self->flash_begin) flash_read_id_ = self->flash_begin;
		  			   }
		  		  if (operating_side == side_b)
		  			   {
		  			     flash_read_id_ = flash_read_idB;   //assign the read address.
		  			     if(flash_read_id_ > flash_endB)   flash_read_id_ = flash_endB;
		  			     if(flash_read_id_ < flash_beginB) flash_read_id_ = flash_beginB;
		  			   }
		   //-------------------------------------------------------
		   self->flshr = 1;
		      return read_flash_State;
		}

		if(self->flshr == 1)
		{
			flash_read_id_ = flash_read_id;
			HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_RESET);
			if (w25qxx.ID >= W25Q256)
			{
				W25qxx_Spi(0x0C);
				W25qxx_Spi((flash_read_id & 0xFF000000) >> 24);
			}
			else
			{
				W25qxx_Spi(0x0B);
			}
			W25qxx_Spi((flash_read_id & 0xFF0000) >> 16);
			W25qxx_Spi((flash_read_id & 0xFF00) >> 8);
			W25qxx_Spi(flash_read_id_ & 0xFF);
			W25qxx_Spi(0);
		  //-------------------------read the data--------------------------
			if (operating_side == side_a)
				HAL_SPI_Receive(&_W25QXX_SPI, &self->log_new, sizeof(self->log_new), 1000);
			else
				HAL_SPI_Receive(&_W25QXX_SPI, &log_b_new, sizeof(log_b_new), 1000);
			HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_SET);

			w25qxx.Lock = 0;   //  unlock access to the flash mem.
			self->flshr = 0;         //reset sub state variable
			return operator_State;

			return self->prev_state_sto; //prog_State; //return to the calling state.
		}
    return read_flash_State;
}

//----------------------------------------
eSystemState write_flash_State_Handler(void)
{
	static int self->flshw = 0;
	uint32_t flash_write_id_ = 0;
	//wrt_Buffer   contains the data to be written int the flash mem.

	static int8_t aflag2 = 0;
	int pg = 0;

	uint32_t next_loc = 0;

	if(operating_sideA)
	{
		self->log_new.self->transaction_period = self->transaction_period;
	    firstTime_filling1 = 1;

	    keypad_fillingUpdate1();

		if(self->opmode == AUTO_MODE)
		{
			self->log_new.autoTranxFlag = 1;
		}
		else if(self->opmode == MANUAL_MODE)
		{
			self->log_new.autoTranxFlag = 0;
		}


		pg = self->flash_info.current_loc/w25qxx.PageSize;

		generateTransc_ID(self->log_new.transaction_id);

		self->log_new.timestamp = RtcToInt_synchedTranx(2019, side_a);
		strcpy(self->log_new.nozzle_name, pumpName[0].pump_name);
		strcpy(self->log_new.nozzle_product, self->settings_stream1[0].product_);

		memset(self->log_new.device_id, '\0', sizeof(self->log_new.device_id));
		strncpy(self->log_new.device_id, device_id, 15);

		memset(self->log_new.session_id, '\0', sizeof(self->log_new.session_id));
//		strncpy(self->log_new.session_id, session_id1, strlen(session_id1));

		W25qxx_WritePage(&self->log_new, pg, 0, sizeof(self->log_new) );
	}
	else if(operating_sideB)
	{
		log_b_new.transaction_period = transaction_period2;
		firstTime_filling2 = 1;

		keypad_fillingUpdate2();

		if(opmode2 == AUTO_MODE)
		{
			log_b_new.autoTranxFlag = 1;
		}
		else if(opmode2 == MANUAL_MODE)
		{
			log_b_new.autoTranxFlag = 0;
		}


		pg = flash_infoB.current_loc/w25qxx.PageSize;

		generateTransc_ID(log_b_new.transaction_id);

		log_b_new.timestamp = RtcToInt_synchedTranx(2019, side_b);
		strcpy(log_b_new.nozzle_name, pumpName[1].pump_name);
		strcpy(log_b_new.nozzle_product, self->settings_stream1[0].product_);

		memset(log_b_new.device_id, '\0', sizeof(log_b_new.device_id));
		strncpy(log_b_new.device_id, device_id, 15);

		memset(log_b_new.session_id, '\0', sizeof(log_b_new.session_id));
//	    strncpy(log_b_new.session_id, session_id2, strlen(session_id2));


		W25qxx_WritePage(&log_b_new, pg, 0, sizeof(log_b_new));
	}

		//-----------------------------------------------------------------------------------
		// update the address and the number of
		// logs written.
		// next saving address is
		  if (operating_side == side_a)
		  {
			 next_loc = self->flash_info.current_loc + 256;  //sizeof(self->log_new);

			 if (next_loc > self->flash_end)
			 {
				 next_loc = self->flash_begin;   //self->flash_end => 0x3FFFFF --> 4,194,303 pg16,383.996
			 }
			 self->flash_info.current_loc  =  next_loc;
			 self->flash_info.number_logs  =  self->flash_info.number_logs + 1;
//			 EEPROM_Write(flash_info_sto, flash_stoA, &self->flash_info, sizeof(self->flash_info));
			 FRAM_Write(self->flash_sto_fram, &self->flash_info, sizeof(self->flash_info));

//			 ep1b_save.total_tranxA++;
			 save_totalTransaction_sides(side_a);

			 if(self->opmode == AUTO_MODE)
			 {
				save_totalAutoTransaction_sides(side_a);
			 }

//
////			//============================================//
////			// 				EP2 ROUTINE SENDING			  //
////			//============================================//
//			if (HAL_GPIO_ReadPin(network_connected_GPIO_Port, network_connected_Pin) == 1 )
//			{
//				connected = 1;
//				ep2_send(side_a);
//			}
//			else
//			{
//				connected = 0;
//			}

//			 if(self->pump_status != STATUS_MAMO_REACHED)
			 if(self->settings_stream1[0].noz_override == override)
				 keypad_zerorize();

			//============================================//

		  }
		  else if (operating_side == side_b)
		  {
			 next_loc =  flash_infoB.current_loc + 256; //sizeof(log_b_new);   //flash_beginB => 0x400000 --> 4,194,304 pg16,384

			 if (next_loc > flash_endB)
			 {
				 next_loc = flash_beginB;  //flash_endB => 0x7FFFFF --> 8,388,607 pg32767.996
			 }

			 flash_infoB.current_loc  =  next_loc;
			 flash_infoB.number_logs  =  flash_infoB.number_logs + 1;

//			 EEPROM_Write(flash_info_sto, flash_stoB, &flash_infoB, sizeof(flash_infoB));
			 FRAM_Write(flash_stoB_fram, &flash_infoB, sizeof(flash_infoB));

//			 ep1b_save.total_tranxB++;
			 save_totalTransaction_sides(side_b);

			 if(opmode2 == AUTO_MODE)
			 {
				save_totalAutoTransaction_sides(side_b);
			 }

////			//============================================//
////			// 				EP2 ROUTINE SENDING			  //
////			//============================================//
//			if (HAL_GPIO_ReadPin(network_connected_GPIO_Port, network_connected_Pin) == 1 )
//			{
//				connected = 1;
//				ep2_send(side_b);
//			}
//			else
//			{
//				connected = 0;
//			}

//			 if(pump_status_2 != STATUS_MAMO_REACHED)
			 if(self->settings_stream1[1].noz_override == override)
				 keypad_zerorize2();

			//============================================//

		  }
		//------------------------------------------------------------------------------------


		  #if defined(AUTO_SALE_TEST)
		  //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA//
		  //======================= AUTOMATED SALES TEST ==========================//

		  	  #if defined(DEBUG_AUTO_SALE_TEST)
				char str_autoSale [50] = {0};
				static uint8_t transact1 = 0,
						       transact2 = 0;
				if (operating_side == side_a)
				{
					memset(str_autoSale, '/0', sizeof(str_autoSale));

					sprintf(str_autoSale,
								"\n\nTransaction [Side-A] : #%d, ",
								transact1++);

					HAL_UART_Transmit(&huart3, str_autoSale, strlen((char*)str_autoSale), HAL_MAX_DELAY);

					HAL_Delay(1);
					memset(str_autoSale, '/0', sizeof(str_autoSale));

					if(sellmode == P)
					{
						sprintf(str_autoSale,
									"Programmed Sale : #%0.2f, ",
									key_value);
					}
					else if(sellmode == L)
					{
						sprintf(str_autoSale,
									"Programmed Sale : %0.2f L, ",
									key_value);
					}

					HAL_UART_Transmit(&huart3, str_autoSale, strlen((char*)str_autoSale), HAL_MAX_DELAY);

					HAL_Delay(1);

					memset(str_autoSale, '/0', sizeof(str_autoSale));
					sprintf(str_autoSale,
								"Transaction Vol. : %0.2f, ",
								amt_middle1);

					HAL_UART_Transmit(&huart3, str_autoSale, strlen((char*)str_autoSale), HAL_MAX_DELAY);

					HAL_Delay(1);

					memset(str_autoSale, '/0', sizeof(str_autoSale));
					sprintf(str_autoSale,
								"Totalizer : %0.2f, ",
								totaliser_vol1c);

					HAL_UART_Transmit(&huart3, str_autoSale, strlen((char*)str_autoSale), HAL_MAX_DELAY);

					HAL_Delay(1);

					memset(str_autoSale, '/0', sizeof(str_autoSale));
					sprintf(str_autoSale,
								"Log No. : #%d\n\n ",
								 self->flash_info.number_logs);

					HAL_UART_Transmit(&huart3, str_autoSale, strlen((char*)str_autoSale), HAL_MAX_DELAY);

				}

				else if (operating_side == side_b)
				{
					memset(str_autoSale, '/0', sizeof(str_autoSale));

					sprintf(str_autoSale,
								"\n\nTransaction [Side-B] : #%d, ",
								transact2++);

					HAL_UART_Transmit(&huart3, str_autoSale, strlen((char*)str_autoSale), HAL_MAX_DELAY);

					HAL_Delay(1);
					memset(str_autoSale, '/0', sizeof(str_autoSale));

					if(sellmode2 == P)
					{
						sprintf(str_autoSale,
									"Programmed Sale : #%0.2f, ",
									key_value2);
					}
					else if(sellmode2 == L)
					{
						sprintf(str_autoSale,
									"Programmed Sale : %0.2f L, ",
									key_value2);
					}

					HAL_UART_Transmit(&huart3, str_autoSale, strlen((char*)str_autoSale), HAL_MAX_DELAY);

					HAL_Delay(1);

					memset(str_autoSale, '/0', sizeof(str_autoSale));
					sprintf(str_autoSale,
								"Transaction Vol. : %0.2f, ",
								amt_middle2);

					HAL_UART_Transmit(&huart3, str_autoSale, strlen((char*)str_autoSale), HAL_MAX_DELAY);

					HAL_Delay(1);

					memset(str_autoSale, '/0', sizeof(str_autoSale));
					sprintf(str_autoSale,
								"Totalizer : %0.2f, ",
								totaliser_vol2c);

					HAL_UART_Transmit(&huart3, str_autoSale, strlen((char*)str_autoSale), HAL_MAX_DELAY);

					HAL_Delay(1);

					memset(str_autoSale, '/0', sizeof(str_autoSale));
					sprintf(str_autoSale,
								"Log No. : #%d\n\n ",
								 flash_infoB.number_logs);

					HAL_UART_Transmit(&huart3, str_autoSale, strlen((char*)str_autoSale), HAL_MAX_DELAY);

				}

			 #endif    //#if DEBUG_AUTO_SALE_TEST

		//UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU//

		#endif //#ifdef AUTO_SALE_TEST



		w25qxx.Lock = 0;       // unlock the flash memory.
		self->flshw = 0;             // reset the sub state.

		if( (self->settings_stream1[0].mode == AUTO_MODE) || (self->settings_stream1[1].mode == AUTO_MODE) )
		{
			if(operating_side == side_a)
			{
				if(self->stopFlag_source == 1)
				{
					self->stopFlag_source = 0;

					self->pump_status = STATUS_FILLING_COMP;

					self->status_change_noz = 1;

					return idle_State;
				}
				else if(self->nozzleDown_source == 1)
				{
					self->nozzleDown_source = 0;

					self->pump_status = STATUS_FILLING_COMP;

					self->status_change_noz = 1;
					self->status_change_pump = 1;

					return idle_State;
				}
				else if (self->mamo_reached_flag1_1 == 1)
				{
					self->mamo_reached_flag1_1 = 0;

					self->mamo_fillingInfo_send = 1;

					self->filling_mamo_flag = 1;  //Ensures Routine in the filling state is not on repeat

					return filling_State;
				}
				else if (self->hardwareErrorFlag_source == 1)
				{
					self->hardwareErrorFlag_source = 0;
					self->hardwareError_flag = 1;

					return filling_State;
				}
				else if(self->go_timeOut == 1)
				{
					self->go_timeOut = 0;

					self->pump_status = STATUS_PNP;
					return pnp_State;
				}
			}

			else if(operating_side == side_b)
			{
				if(stopFlag_source2 == 1)
				{
					stopFlag_source2 = 0;

					pump_status_2 = STATUS_FILLING_COMP;

					status_change_noz2 = 1;

					return idle_State;
				}
				else if(nozzleDown_source2 == 1)
				{
					nozzleDown_source2 = 0;

					pump_status_2 = STATUS_FILLING_COMP;

					status_change_noz2 = 1;
					status_change_pump2 = 1;

					return idle_State;
				}
				else if (mamo_reached_flag2_1 == 1)
				{
					mamo_reached_flag2_1 = 0;
					mamo_fillingInfo_send2 = 1;
					filling_mamo_flag2 = 1;  //Ensures Routine in the filling state is not on repeat

					return filling_State;
				}
				else if (hardwareErrorFlag_source2 == 1)
				{
					hardwareErrorFlag_source2 = 0;
					hardwareError_flag2 = 1;

					return filling_State;
				}
				else if(go_timeOut2 == 1)
				{
					go_timeOut2 = 0;

					pump_status_2 = STATUS_PNP;
					return pnp_State;
				}
			}
		}

		else if( (self->settings_stream1[0].mode == MANUAL_MODE) || (self->settings_stream1[1].mode == MANUAL_MODE) )
		{
			if (operating_side == side_a)
			{
				self->pump_status = STATUS_FILLING_COMP;
			}
			else if (operating_side == side_b)
			{
				pump_status_2 = STATUS_FILLING_COMP;
			}
		}

		return idle_State;     //write complete go back to idle state.
//=========================================================================================

	//	 W25qxx_WritePage(pBuffer, Page_Address, OffsetInByte, NumByteToWrite_up_to_PageSize);

	if(self->flshw == 0)
	{
		  while (w25qxx.Lock == 1)
			  return read_flash_State; //wait for pending job

		  w25qxx.Lock = 1;  // lock access to flash mem. operations.
		  //----------------------------------------------------
		  //   assign the writing address.
		  if (operating_side == side_a)
		   {
			 flash_write_id_ = self->flash_info.current_loc;
		   }
		  if (operating_side == side_b)
		   {
			 flash_write_id_ = flash_infoB.current_loc;
		   }
		  //-----------------------------------------------------
		  self->flshw = 1;
		  return write_flash_State;
	}
//--------------------------wait for write end -----------------------------
	if(self->flshw == 1)
	{
		 HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_RESET);
		 W25qxx_Spi(0x05);
		 self->flshw = 2;
		 return write_flash_State;
	}

	if(self->flshw == 2)
	{
		w25qxx.StatusRegister1 = W25qxx_Spi(W25QXX_DUMMY_BYTE);
		self->flshw = 3;
		return write_flash_State;
	}

	if(self->flshw == 3)
	{
		if ((w25qxx.StatusRegister1 & 0x01) == 0x01)
		 {
			self->flshw = 2;
			return write_flash_State;
		 }
		  HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_SET);
		  self->flshw = 4;
		  return write_flash_State;
	}
//-------------------------  write enable  -------------------------
	if(self->flshw == 4)
		{
			HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_RESET);
			W25qxx_Spi(0x06);
			HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_SET);
			self->flshw = 5;
			return write_flash_State;
		}
//-------------------------  set the address ----------------------
		if(self->flshw == 5)
		{
			flash_write_id_ = flash_write_id;
	    	HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_RESET);
	    	if (w25qxx.ID >= W25Q256)
			{
				W25qxx_Spi(0x12);
				W25qxx_Spi((flash_write_id_ & 0xFF000000) >> 24);
			}
			else
			{
				W25qxx_Spi(0x02);
			}
	    	W25qxx_Spi((flash_write_id_ & 0xFF0000) >> 16);
			W25qxx_Spi((flash_write_id_ & 0xFF00) >> 8);
			W25qxx_Spi(flash_write_id_ & 0xFF);

			extern int data_size;
         //----------------------------write the data--------------------------
			if (operating_side == side_a)
				HAL_SPI_Transmit(&_W25QXX_SPI, &self->log_new, data_size, 1000);
			else
				HAL_SPI_Transmit(&_W25QXX_SPI, &log_b_new, data_size, 1000);

			//HAL_SPI_Transmit(&_W25QXX_SPI, &wr_buffer, sizeof(wr_buffer), 100);

	    	HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_SET);
			self->flshw = 6;
			return write_flash_State;
		}


	//--------------------------wait for write end -----------------------------
		if(self->flshw == 6)
		{
			 HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_RESET);
			 W25qxx_Spi(0x05);
			 self->flshw = 7;
			 return write_flash_State;
		}

		if(self->flshw == 7)
		{
			w25qxx.StatusRegister1 = W25qxx_Spi(W25QXX_DUMMY_BYTE);
			self->flshw = 8;
		    return write_flash_State;
		}

		if(self->flshw == 8)
		{
			if ((w25qxx.StatusRegister1 & 0x01) == 0x01)
			 {
				self->flshw = 7;   //back to re test the status register.
				return write_flash_State;
			 }
			  HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_SET);
			  self->flshw = 9;
			  return write_flash_State;
		}

		if(self->flshw == 9)
		  {
			//-----------------------------------------------------------------------------------
			// update the address and the number of
			// logs written.
			// next saving address is
			  if (operating_side == side_a)
			  {
				 next_loc =  self->flash_info.current_loc + 256;  //sizeof(self->log_new);

				 if (next_loc > self->flash_end)
				 {
					 next_loc = self->flash_begin;
				 }

				 self->flash_info.current_loc  =  next_loc;
				 self->flash_info.number_logs  =  self->flash_info.number_logs + 1;
//				     EEPROM_Write(flash_info_sto, flash_stoA, &self->flash_info, sizeof(self->flash_info));
				 FRAM_Write(self->flash_sto_fram, &self->flash_info, sizeof(self->flash_info));

			  }
			  else if (operating_side == side_b)
			  {
				 next_loc =  flash_infoB.current_loc + 256; //sizeof(log_b_new);

				 if (next_loc > flash_endB)
				 {
					 next_loc = flash_beginB;
				 }

				 flash_infoB.current_loc  =  next_loc;
				 flash_infoB.number_logs  =  flash_infoB.number_logs + 1;
//					 EEPROM_Write(flash_info_sto, flash_stoB, &flash_infoB, sizeof(flash_infoB));
				 FRAM_Write(flash_stoB_fram, &flash_infoB, sizeof(flash_infoB));
			  }
			//------------------------------------------------------------------------------------
			w25qxx.Lock = 0;       // unlock the flash memory.
			self->flshw = 0;             // reset the sub state.
			return idle_State;     //write complete go back to idle state.
		  }
    return write_flash_State;
}


void  clear_totA(void)
{
	self->totaliser_vol = 0;
	totaliser_vol1c = 0;
	self->log_new.totaliserVol_real = 0;
	self->log_new.totaliserVol_cal = 0;
//	save_volumeTotaliser(side_a);
	save_volumeTotaliser_fram(side_a);

}

void  clear_totB(void)
{
	totaliser_vol2 = 0;
	totaliser_vol2c = 0;
	log_b_new.totaliserVol_real = 0;
	log_b_new.totaliserVol_cal = 0;
//	save_volumeTotaliser(side_b);
	save_volumeTotaliser_fram(side_b);
}


void clear_logA(void)
{
	self->flash_info.current_loc  =  self->flash_begin;
	self->flash_info.number_logs  =  0;
//	EEPROM_Write(flash_info_sto, flash_stoA, &self->flash_info, sizeof(self->flash_info));
	FRAM_Write(self->flash_sto_fram, &self->flash_info, sizeof(self->flash_info));
	W25qxx_EraseBlock( 0 );
	W25qxx_EraseBlock( 1 );
	W25qxx_EraseBlock( 2 );
	W25qxx_EraseBlock( 3 );
	W25qxx_EraseBlock( 4 );
	W25qxx_EraseBlock( 5 );
}

void clear_logB(void)
{
	flash_infoB.current_loc  =  flash_beginB;
	flash_infoB.number_logs  =  0;
//	EEPROM_Write(flash_info_sto, flash_stoB, &flash_infoB, sizeof(flash_infoB));
	FRAM_Write(flash_stoB_fram, &flash_infoB, sizeof(flash_infoB));
	W25qxx_EraseBlock( 64 );
	W25qxx_EraseBlock( 65 );
	W25qxx_EraseBlock( 66 );
	W25qxx_EraseBlock( 67 );
	W25qxx_EraseBlock( 68 );
	W25qxx_EraseBlock( 69 );
}


