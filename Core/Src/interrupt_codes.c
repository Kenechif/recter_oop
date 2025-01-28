/*
 * interrupt_codes.c
 *
 *  Created on: 25 Jan 2022
 *      Author: Tunjow
 */

/*!
 * This code section deals with interrupt codes
 */
#include "main.h"
#include "interrupt_codes.h"
#include "buffer.h"
#include "stm32f4xx_hal.h"
#include "command.h"
#include "states.h"
#include "states2.h"
#include "myLibraries.h"
#include "pump_comm.h"



//union strr ;

extern pump_status_enum pump_status_1,
						pump_status_2;

extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;
extern uint8_t buff[30];

extern uint8_t rxBuffer[RX_BUFFER_SIZE],  // Buffer for single character reception
			   messageBuffer[RX_BUFFER_SIZE]; // Buffer to hold complete message

extern uint16_t messageIndex;


extern int tot_buttonpress_tmr;
extern int log_buttonpress_tmr;
extern int key_buttonpress_tmr,
		   progExit_buttonpress_tmr1;

extern uint8_t filling1;
extern uint8_t pulser_complete1;
extern int pulser_rem1;
//extern const uint16_t fast_flow_threshold;

extern uint16_t fast_flow_threshold1,
				 fast_flow_threshold2,
				 slowFlow_startThreshold1,
				 slowFlow_startThreshold2,
				 slowFlow_endThreshold1,
				 slowFlow_endThreshold2;

extern int calibr1;

extern uint8_t buff[30] ;

extern uint32_t transaction_period,
				transaction_period2;

extern uint8_t firstTime_filling1,
			   firstTime_filling2;

extern uint8_t fastFlow1,
			   fastFlow2;

uint16_t countar3 = 0;

extern uint16_t countar,
		 	 	countar2;

extern uint8_t mamo_reached_flag1 = 0,
			   mamo_reached_flag1_1 = 0;

extern uint32_t  r_volTotaliser2,
		  	     old_r_volTotaliser2,
				 old_r_volTotaliser2_0;

uint32_t mech_totalizer1 CCRAM = 0,
		 mech_totalizer2 CCRAM = 0,
		 mech_totalizer_old1 CCRAM = 0,
		 mech_totalizer_old2 CCRAM = 0;

float mechTotalizer1 CCRAM = 0.0,
	  mechTotalizer2 CCRAM = 0.0,
	  mechTotalizer1_ CCRAM = 0.0,
	  mechTotalizer2_ CCRAM = 0.0;
//===============================================

extern int tot_buttonpress_tmr2;
extern int log_buttonpress_tmr2;
extern int key_buttonpress_tmr2,
		   progExit_buttonpress_tmr2;

extern uint8_t filling2;
extern uint8_t pulser_complete2;
extern int pulser_rem2;
//extern const uint8_t fast_flow_threshold;
extern int calibr2;

extern  uint16_t fast_flow_threshold1,
				 slowFlow_startThreshold1,
				 slowFlow_endThreshold1,
				 fast_flow_threshold2,
				 slowFlow_startThreshold2,
				 slowFlow_endThreshold2;

extern uint16_t totalizer1Timer,
				totalizer2Timer;

uint16_t motor_tmr1 = 0,
		 motor_tmr2 = 0;


extern void Uart_isr (UART_HandleTypeDef *huart);
extern uint16_t timeout;

/*
HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_0) // If The INT Source Is B0 pin  from P2 pulser line
	{
		 HAL_GPIO_TogglePin(nLed_GPIO_Port, nLed_Pin); // Toggle LED
		 pulser1++;   // increment the pulser counter.
	}

	if(GPIO_Pin == GPIO_PIN_1) // If The INT Source Is B1 pin from p22 pulser line
	{
		 HAL_GPIO_TogglePin(nLed_GPIO_Port, nLed_Pin); // Toggle LED
		 pulser2++;   // increment the pulser counter.
	}
}     */


/**
 *    Receive the UART message through interrupt
 */
HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	// rcv_count+=15;// (15*2);

/*	 for (int i  = 0 ; i < 15 ; i++)
	 {
		// buffer_write(&dat_str, buff[i]);
	 }  */
	// buffer_write_bytes(&dat_str, buff[0] ,15);
	// int remm = __HAL_DMA_GET_COUNTER(huart1.hdmarx);
/*	if ( (dat_str.head + 15  == dat_str.tail)||( (dat_str.head == dat_str.size)&& dat_str.tail == 0 )  )
	{
		// buffer full
		HAL_UART_Receive_DMA(&huart1,  &dat_str.buf[0] , 30);
		 //wrap
		dat_str.head = 0 ;  //advance the head by 15 members.
		dat_str.element+=15;
	    return;
	}
	else
	*/
	{
	  dat_str.head += 15;   //advance the head by 15 members.
	  dat_str.element+=15;
	/*  if(dat_str.head == dat_str.size) //wrap if needed
		  {
			  dat_str.head = 0;
			  HAL_UART_Receive_DMA(&huart1,  &dat_str.buf[0] , 30);
			  return;
		  } */
	    //  HAL_UART_Receive_DMA(&huart1,  &dat_str.buf[dat_str.head] , 30);

	}

}


HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART3)
	{
//	        HAL_UART_Transmit(&huart1, rxData, sizeof(rxData), HAL_MAX_DELAY);
//	        HAL_UART_Receive_IT(&huart1, rxData, sizeof(rxData));
			if (rxBuffer[0] == '\n' || rxBuffer[0] == '\r')   // End of message detected
			{

				messageBuffer[messageIndex] = '\0';  // Null-terminate the string
				messageIndex = 0;  // Reset the index

				char str[100];
				// Process the complete message
				sprintf(str, "\r\nInjected Amount : ");  // Unsafe, may overflow buffer
				HAL_UART_Transmit(&huart3, str, strlen((char*)str), HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart3, messageBuffer, strlen((char*)messageBuffer), HAL_MAX_DELAY);

//				amt_middle1 = strtof(messageBuffer, NULL);
				running_volTotaliser1c = strtof(messageBuffer, NULL);

				memset(str, '\0', sizeof(str));

//				sprintf(str, "\r\namt_middle1 = %0.2f\n", amt_middle1);
				sprintf(str, "\r\nrunning_volTotaliser1c = %0.2f\n", running_volTotaliser1c);
				HAL_UART_Transmit(&huart3, str, strlen((char*)str), HAL_MAX_DELAY);

			}
			else if (messageIndex < RX_BUFFER_SIZE - 1)   // Accumulate characters
			{
				messageBuffer[messageIndex++] = rxBuffer[0];
			}

		        // Re-enable interrupt reception for the next character
			HAL_UART_Receive_IT(&huart3, rxBuffer, 1);
	 }

	// full data received !
  // reenergise he receive
  // HAL_UART_Receive_DMA(&huart1,  &MsgStruct_, 15);
	/* for (int i  = 0 ; i < 15 ; i++)
		 {
		//	 buffer_write(&dat_str, buff[i+14]);
		 } */
 //  rcv_count+=15;
   //if( (f->head + 1 == f->tail)||(  (f->head+1 == f->size)&&(f->tail == 0) ) )
	   //check to see if space is in the buffer...
//if ( (dat_str.head + 30  >= dat_str.tail)||( (dat_str.head == dat_str.size)&& dat_str.tail == 0 )  )
if (dat_str.element + 15 >= dat_str.size)
{
	// buffer full
	//this should not happen.... but in the event that it does,
	//set the full flag...    and manage the next DMA  receive
	// address....
	HAL_UART_Receive_DMA(&huart1,  &dat_str.buf[0],30);
	 //wrap
	dat_str.head = 0 ;  //advance the head by 15 members.
	dat_str.element+=15;
    return;
}
else
{
  dat_str.head += 15;   //advance the head by 15 members.
  dat_str.element+=15;
  if(dat_str.head >= dat_str.size) //wrap if needed
	  {
		  dat_str.head = 0;
		  HAL_UART_Receive_DMA(&huart1,  &dat_str.buf[0],30);
		  return;
	  }
      HAL_UART_Receive_DMA(&huart1,  &dat_str.buf[dat_str.head],30);
    return;
}
  // HAL_UART_Receive_DMA(&huart1,  &dat_str.buf[0] , 30);
  // buffer_write_bytes(&dat_str, buff[14] ,15);
   int remm = __HAL_DMA_GET_COUNTER(huart1.hdmarx);
//huart1.pRxBuffPtr = &packet;
}


void check_flow(void)
{
	motor_tmr1++;
	motor_tmr2++;

	if(motor_tmr1 > 3000) motor_tmr1 = 3000;
	if(motor_tmr2 > 3000) motor_tmr2 = 3000;

	extern float pulser_index_c;

	extern uint32_t target_pulser1,
					current_pulser1,
					overall_currentPulser1;
	extern int t,
//			   ttt1,
			   t2;
//			   ttt2;

	extern unsigned int ttt1,
			   	   	    ttt2;

	extern uint32_t target_pulser2,
					current_pulser2;

//		extern int t2;

	if ( pulser_complete1 == 1 ) stop_flow1();
//	if ( pulser_complete2 == 1 ) stop_flow2();

	#if (_USE_SOFT_PULSER == 1)
//	  if ( (t > 50)&&(t < 90) ) // 200

	  if(fastFlow1 == 1)
	  {
//		  if (ttt1 > 1)
		  if ( (t > 50) && (t < 99) )
//		  if ( (t > 50) && (t < 56) )
		  {
			 if(filling1 == 1)
			 {
				current_pulser1++;   //transfer this to the interrupt routine..
//				ttt1 = 0;
			 }
		  }
	  }
	  else
	  {
		  if ( (t > 50) && (t < 56) ) // 200
		  {
			 if(filling1 == 1)
			 {
				current_pulser1++;   //transfer this to the interrupt routine..
			 }
		  }
	  }
	#else
		  if(pulser_rem1 > 0)
		  {
			  current_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);
			  overall_currentPulser1 = current_pulser1;
		  }
		  else
		  {
			  overall_currentPulser1 = __HAL_TIM_GET_COUNTER(&htim5);
		  }

	#endif
    //------------------------------------------------------------------
	if (filling1 == 1)
	{
//	  static int8_t firstTime = 1;

	  if(firstTime_filling1 == 1)
	  {
		  transaction_period = 0;
		  firstTime_filling1 = 0;
	  }

	  //....if programmed....
	  if(target_pulser1 > 0)
	  {
		  if(pulser_rem1 > 0)
		  {
			pulser_complete1 = 0;
			pulser_rem1 = target_pulser1 - current_pulser1;

			if(pulser_rem1 <= 0)
			{
				stop_flow1();
			}

			else if(pulser_rem1 >= slowFlow_endThreshold1)
			{
				if(current_pulser1 >=  slowFlow_startThreshold1)
				{
					fast_flow1();

					fastFlow1 = 1;
				}
				else
				{
					slow_flow1();

					fastFlow1 = 0;
				}
			}
			else
			{
//				pump_status_ = STATUS_MAMO_REACHED;
				slow_flow1();

				fastFlow1 = 0;
			}
		  }
		  else
		  {
			  //pulser complete   ---==>> //sales complete...
			  //---------------------------------------------
			 pulser_complete1 = 1;

//			 pump_status_1 = STATUS_MAMO_REACHED;
//			 mamo_reached_flag1 = 1;

			 stop_flow1(); 			  //stop solenoid.
		  }
	   }
	   else
	   {
			//not programmed high flow
			fast_flow1();

			fastFlow1 = 1;
	   }

	  //============================================================
	  //         for totaliser toggle.
//	  	  r_volTotaliser1 	  = floor( running_volTotaliser1c );
//	  	  r_amtTotaliser 	  = floor(running_amtTotaliser1c);

//	  	if(r_volTotaliser1 != old_r_volTotaliser1)
//	  	{
//	  		totalizer1Timer = 0;
//	  //			then toggle the totaliser harware I/O.
//	  		drive_totaliser1(ACTIVATE);
//	  		countar++;
//	  	}
	  	if(mech_totalizer1 != mech_totalizer_old1)
		{
	  		totalizer1Timer = 0;

	        //	then toggle the totaliser harware I/O.
	  		drive_totaliser1(ACTIVATE);
		}
	  	else
	  	{
	  		//deactivate totaliser output...
	  		if(totalizer1Timer > 200)
	  		{
	  			drive_totaliser1(DEACTIVATE);
//	  			countar2++;
	  		}

	  	}
//	  	  old_r_volTotaliser1 = r_volTotaliser1;   //update...

	  	//===================// Update... //===================//
	  	  mech_totalizer_old1 = mech_totalizer1;
	  	//-----------------------------------------------------//

//	  	if (totaliser_vol1c - previous_totaliserVol1c >= THRESHOLD_TV)
//		{
//			previous_totaliserVol1c = totaliser_vol1c;
//
//			save_totaliserFrequent_fram(side_a);
//			save_totaliserFrequent_eeprom(side_a);
//		}
	 }
	 else
	 {
		  //not filling1
		  pulser_complete1 = 0;

		  //deactivate totaliser output...
		  if(totalizer1Timer > 200)
		  {
			drive_totaliser1(DEACTIVATE);
			countar3++;
		  }

		  if( calibr1 == 0)
			  stop_flow1();
		  else
		  {
			  fast_flow1();

			  fastFlow1 = 1;
		  }

	 }

//===============================================================================

		if ( pulser_complete2 == 1 ) stop_flow2();

		#if (_USE_SOFT_PULSER == 1)

		  if(fastFlow2 == 1)
		  {
//			  if (ttt2 > 1)
			  if ( (t2 > 50) && (t2 < 99) )
//			  if ( (t2 > 50) && (t2 < 54) )
			  {
				 if(filling2 == 1)
				 {
					current_pulser2++;   //transfer this to the interrupt routine..
//					ttt2 = 0;
				 }
			  }
		  }
		  else
		  {
			  if ( (t2 > 50) && (t2 < 56) ) // 200
			  {
				 if(filling2 == 1)
				 {
					current_pulser2++;   //transfer this to the interrupt routine..
				 }
			  }
		  }

		#else
				current_pulser2 = __HAL_TIM_GET_COUNTER(&htim2);
		#endif
	    //------------------------------------------------------------------
		 if (filling2 == 1)
		 {
			 if(firstTime_filling2 == 1)
			 {
				  transaction_period2 = 0;
				  firstTime_filling2 = 0;
			 }
			 //....if programmed....
			  if(target_pulser2 > 0)
				{
				  if(pulser_rem2 > 0)
					{
					  pulser_complete2 = 0;
						pulser_rem2 = target_pulser2 - current_pulser2;

						if(pulser_rem2 <= 0)
						{
							stop_flow2();
						}
						else if(pulser_rem2 >= slowFlow_endThreshold2)
						{
							if(current_pulser2 >=  slowFlow_startThreshold2)
							{
								fast_flow2();

								fastFlow2 = 1;
							}
							else
							{
								slow_flow2();

								fastFlow2 = 0;
							}
						}
						else
						{
//							pump_status_ = STATUS_MAMO_REACHED;
							slow_flow2();

							fastFlow2 = 0;
						}
					  }
					  else
					  {
						  //pulser complete   ---==>> //sales complete...
						  //---------------------------------------------
						 pulser_complete2 = 1;

//						 pump_status_2 = STATUS_MAMO_REACHED;
//						 mamo_reached_flag2 = 1;

						 stop_flow2(); 			  //stop solenoid.
					  }
				   }
			  else
			   {
					//not programmed high flow
					fast_flow2();

					fastFlow2 = 1;
			   }

			  //============================================================
			  //         for totaliser toggle.
		//	  	  r_volTotaliser1 	  = floor( running_volTotaliser1c );
		//	  	  r_amtTotaliser 	  = floor(running_amtTotaliser1c);

//			  mechTotalizer2 = (mechTotalizer2_ + amt_middle2);
//
//		      mech_totalizer2 = (int)mechTotalizer2;

		//		if(r_volTotaliser2 != old_r_volTotaliser2)
		//		{
		//			totalizer2Timer = 0;
		//	  //			then toggle the totaliser harware I/O.
		//			drive_totaliser2(ACTIVATE);

//				}
				if(mech_totalizer2 != mech_totalizer_old2)
				{
			  		totalizer2Timer = 0;

			        //	then toggle the totaliser harware I/O.
			  		drive_totaliser2(ACTIVATE);
				}
				else
				{
					//deactivate totaliser output...
					if(totalizer2Timer > 200)
					{
						drive_totaliser2(DEACTIVATE);
					}

				}
				old_r_volTotaliser2 = r_volTotaliser2;   //update...

				//===================// Update... //===================//
			  	  mech_totalizer_old2 = mech_totalizer2;
			  	//-----------------------------------------------------//

//			  	if (totaliser_vol2c - previous_totaliserVol2c >= THRESHOLD_TV)
//				{
//					previous_totaliserVol2c = totaliser_vol2c;
//
//					save_totaliserFrequent_fram(side_b);
//					save_totaliserFrequent_eeprom(side_b);
//				}
			 }
			 else
			 {
				  //not filling2
				  pulser_complete2 = 0;

				  //deactivate totaliser output...
				  if(totalizer2Timer > 200)
				  {
					drive_totaliser2(DEACTIVATE);
					countar3++;
				  }

				  if( calibr2 == 0)
					  stop_flow2();
				  else
				  {
					  fast_flow2();

					  fastFlow2 = 1;
				  }
			 }
}




//void check_flow(void)
//{
//	motor_tmr1++;
//	motor_tmr2++;
//
//	if(motor_tmr1 > 3000) motor_tmr1 = 3000;
//	if(motor_tmr2 > 3000) motor_tmr2 = 3000;
//
//	extern float pulser_index_c;
//
//	extern uint32_t target_pulser1,
//					current_pulser1;
//	extern int t,
//			   ttt1;
//
//	extern uint32_t target_pulser2, current_pulser2;
//		extern int t2;
//
//	if ( pulser_complete1 == 1 ) stop_flow1();
//
//	#if (_USE_SOFT_PULSER == 1)
////	  if ( (t > 50)&&(t < 90) ) // 200
//
//	  if(fastFlow1 == 1)
//	  {
//		  if (ttt1 > 1)
//		  {
//			 if(filling1 == 1)
//			 {
//				current_pulser1++;   //transfer this to the interrupt routine..
//				ttt1 = 0;
//			 }
//		  }
//	  }
//	  else
//	  {
//		  if ( (t > 50) && (t < 56) ) // 200
//		  {
//			 if(filling1 == 1)
//			 {
//				current_pulser1++;   //transfer this to the interrupt routine..
//			 }
//		  }
//	  }
//	#else
//		  if(pulser_rem1 > 0)
//		  {
//			  current_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);
//		  }
//
////		  current_pulser1 = __HAL_TIM_GET_COUNTER(&htim5);
//
//	#endif
//    //------------------------------------------------------------------
//	if (filling1 == 1)
//	{
////	  static int8_t firstTime = 1;
//
//	  if(firstTime_filling1 == 1)
//	  {
//		  transaction_period = 0;
//		  firstTime_filling1 = 0;
//	  }
//
//	  //....if programmed....
//	  if(target_pulser1 > 0)
//	  {
//		  if(pulser_rem1 > 0)
//		  {
//			pulser_complete1 = 0;
//			pulser_rem1 = target_pulser1 - current_pulser1;
//
//			if(pulser_rem1 <= 0)
//			{
//				stop_flow1();
//			}
//
//			else if(pulser_rem1 >= fast_flow_threshold)
//			{
//				if(current_pulser1 >=  fast_flow_threshold/2 )
//				{
//					fast_flow1();
//
//					fastFlow1 = 1;
//				}
//				else
//				{
//					slow_flow1();
//
//					fastFlow1 = 0;
//				}
//			}
//			else
//			{
//				pump_status_ = STATUS_MAMO_REACHED;
//				slow_flow1();
//
//				fastFlow1 = 0;
//			}
//		  }
//		  else
//		  {
//			  //pulser complete   ---==>> //sales complete...
//			  //---------------------------------------------
//			 pulser_complete1 = 1;
//			 pump_status_ = STATUS_FILLING_COMP;
//			 stop_flow1(); 			  //stop solenoid.
//		  }
//	   }
//	   else
//	   {
//			//not programmed high flow
//			fast_flow1();
//
//			fastFlow1 = 1;
//	   }
//
//	  //============================================================
//	  //         for totaliser toggle.
////	  	  r_volTotaliser1 	  = floor( running_volTotaliser1c );
////	  	  r_amtTotaliser 	  = floor(running_amtTotaliser1c);
//
//	  	if(r_volTotaliser1 != old_r_volTotaliser1)
//	  	{
//	  		totalizer1Timer = 0;
//	  //			then toggle the totaliser harware I/O.
//	  		drive_totaliser1(ACTIVATE);
////	  		countar++;
//	  	}
//	  	else
//	  	{
//	  		//deactivate totaliser output...
//	  		if(totalizer1Timer > 200)
//	  		{
//	  			drive_totaliser1(DEACTIVATE);
////	  			countar2++;
//	  		}
//
//	  	}
//	  	  old_r_volTotaliser1 = r_volTotaliser1;   //update...
////	  	  old_r_amtTotaliser = r_amtTotaliser;
//	 }
//	 else
//	 {
//		  //not filling1
//		  pulser_complete1 = 0;
//
//		  //deactivate totaliser output...
//		  if(totalizer1Timer > 200)
//		  {
//			drive_totaliser1(DEACTIVATE);
//			countar3++;
//		  }
//
//		  if( calibr1 == 0)
//			  stop_flow1();
//		  else
//		  {
//			  fast_flow1();
//
//			  fastFlow1 = 1;
//		  }
//
//	 }
//
////===============================================================================
//
//		if ( pulser_complete2 == 1 ) stop_flow2();
//
//		#if (_USE_SOFT_PULSER == 1)
//		  if ( (t2 > 50)&&(t2 < 90) ) // 200
//			{
//			  if(filling2 == 1)
//				 {
//				    current_pulser2++;   //transfer this to the interrupt routine..
//			     }
//			}
//		#else
//				current_pulser2 = __HAL_TIM_GET_COUNTER(&htim2);
//		#endif
//	    //------------------------------------------------------------------
//		 if (filling2 == 1)
//		 {
//			 if(firstTime_filling2 == 1)
//			 {
//				  transaction_period2 = 0;
//				  firstTime_filling2 = 0;
//			 }
//			 //....if programmed....
//			  if(target_pulser2 > 0)
//				{
//				  if(pulser_rem2 > 0)
//					{
//					  pulser_complete2 = 0;
//						pulser_rem2 = target_pulser2 - current_pulser2;
//						  if(pulser_rem2 >= fast_flow_threshold)
//							{
//								if(current_pulser2 >=  fast_flow_threshold/2 )
//								{
//									fast_flow2();
//								}
//								else
//								{
//									slow_flow2();
//								}
//							}
//							else
//							{
//								pump_status_ = STATUS_MAMO_REACHED;
//								slow_flow2();
//							}
//					}
//
//				  else
//				  {
//					  //pulser complete   ---==>> //sales complete...
//					  //---------------------------------------------
//					 pulser_complete2 = 1;
//					 pump_status_ = STATUS_FILLING_COMP;
//					 stop_flow2(); 			  //stop solenoid.
//				  }
//				}
//					else
//				{
//					//not programmed high flow
//					fast_flow2();
//				}
//			}
//		  else
//		  {
//			  //not filling1
//			  pulser_complete2 = 0;
//
//			  //deactivate totaliser output...
//			  if(totalizer2Timer > 300)
//			  {
//				drive_totaliser2(DEACTIVATE);
//			  }
//			  if( calibr2 == 0)
//				  stop_flow2();
//			  else
//				  fast_flow2();
//
//		  }
//}
