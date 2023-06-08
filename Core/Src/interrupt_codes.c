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

extern pump_status_enum pump_status_;

extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;
extern uint8_t buff[30] ;


extern int tot_buttonpress_tmr;
extern int log_buttonpress_tmr;
extern int key_buttonpress_tmr;

extern uint8_t filling;
extern uint8_t pulser_complete;
extern int pulser_rem;
extern const uint16_t fast_flow_threshold;
extern int calibr;

extern uint8_t buff[30] ;

//===============================================

extern int tot_buttonpress_tmr2;
extern int log_buttonpress_tmr2;
extern int key_buttonpress_tmr2;

extern uint8_t filling2;
extern uint8_t pulser_complete2;
extern int pulser_rem2;
//extern const uint8_t fast_flow_threshold;
extern int calibr2;

uint16_t motor_tmr = 0,
		 motor_tmr2 = 0;

/*
HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_0) // If The INT Source Is B0 pin  from P2 pulser line
	{
		 HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin); // Toggle LED
		 pulser1++;   // increment the pulser counter.
	}

	if(GPIO_Pin == GPIO_PIN_1) // If The INT Source Is B1 pin from p22 pulser line
	{
		 HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin); // Toggle LED
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
	motor_tmr++;
	motor_tmr2++;

	if(motor_tmr > 3000) motor_tmr = 3000;
	if(motor_tmr2 > 3000) motor_tmr2 = 3000;

	extern float pulser_index_c;
	extern uint32_t target_pulser, current_pulser;
	extern int t, ttt;

	extern uint32_t target_pulser2, current_pulser2;
		extern int t2;

	if ( pulser_complete == 1 ) stop_flow();

	#if (_USE_SOFT_PULSER == 1)
//	  if ( (t > 50)&&(t < 90) ) // 200
		  if (ttt>1)
	  {
		 if(filling == 1)
		 {
			current_pulser++;   //transfer this to the interrupt routine..
			ttt=0;
		 }
	  }
	#else
			current_pulser = __HAL_TIM_GET_COUNTER(&htim5);
	#endif
    //------------------------------------------------------------------
	  if (filling == 1)
		{
		  //....if programmed....
		  if(target_pulser > 0)
			{
			  if(pulser_rem > 0)
				{
				    pulser_complete = 0;
					pulser_rem = target_pulser - current_pulser;
					if(pulser_rem >= fast_flow_threshold)
					{
						if(current_pulser >=  fast_flow_threshold/2 )
							{
							    fast_flow();
							}
						else
						{
							slow_flow();
						}
					}
					else
					{
						pump_status_ = STATUS_MAMO_REACHED;
						slow_flow();
					}
				}
			  else
			  {
				  //pulser complete   ---==>> //sales complete...
				  //---------------------------------------------
				 pulser_complete = 1;
				 pump_status_ = STATUS_FILLING_COMP;
				 stop_flow(); 			  //stop solenoid.
			  }
			}
				else
			{
				//not programmed high flow
				fast_flow();
			}
		}
	  else
	  {
		  //not filling
		  pulser_complete = 0;
		  drive_totaliser1(DEACTIVATE);
		  if( calibr == 0)
			  stop_flow();
		  else
			  fast_flow();

	  }

//===============================================================================

		if ( pulser_complete2 == 1 ) stop_flow2();

		#if (_USE_SOFT_PULSER == 1)
		  if ( (t2 > 50)&&(t2 < 90) ) // 200
			{
			  if(filling2 == 1)
				 {
				    current_pulser2++;   //transfer this to the interrupt routine..
			     }
			}
		#else
				current_pulser2 = __HAL_TIM_GET_COUNTER(&htim2);
		#endif
	    //------------------------------------------------------------------
		  if (filling2 == 1)
			{
			  //....if programmed....
			  if(target_pulser2 > 0)
				{
				  if(pulser_rem2 > 0)
					{
					  pulser_complete2 = 0;
						pulser_rem2 = target_pulser2 - current_pulser2;
						  if(pulser_rem2 >= fast_flow_threshold)
							{
								if(current_pulser2 >=  fast_flow_threshold/2 )
								{
									fast_flow2();
								}
								else
								{
									slow_flow2();
								}
							}
							else
							{
								pump_status_ = STATUS_MAMO_REACHED;
								slow_flow2();
							}
					}

				  else
				  {
					  //pulser complete   ---==>> //sales complete...
					  //---------------------------------------------
					 pulser_complete2 = 1;
					 pump_status_ = STATUS_FILLING_COMP;
					 stop_flow2(); 			  //stop solenoid.
				  }
				}
					else
				{
					//not programmed high flow
					fast_flow2();
				}
			}
		  else
		  {
			  //not filling
			  pulser_complete2 = 0;
			  drive_totaliser2(DEACTIVATE);
			  if( calibr2 == 0)
				  stop_flow2();
			  else
				  fast_flow2();

		  }
}
