/*
 * timers.c
 *
 *  Created on: May 4, 2022
 *      Author: tunjo
 */
#include "main.h"
#include "timers.h"
#include "states.h"
#include "states2.h"

extern int tot_buttonpress_tmr;
extern int log_buttonpress_tmr;
extern int key_buttonpress_tmr,
		   progExit_buttonpress_tmr1;

extern int tot_buttonpress_tmr2;
extern int log_buttonpress_tmr2;
extern int key_buttonpress_tmr2,
		   progExit_buttonpress_tmr2;

extern uint16_t shutdown_timer1,
				shutdown_timer2;

uint16_t printer_time = 0;
uint8_t server_time = 0;

extern int t, t2; //ttt1;
extern uint16_t _tt1,
				_tt2,
//				timer_ep,
//				timer_ep1,
				timer_spi,
				totalizer1Timer,
				totalizer2Timer,
				priceChange_timer1,
				priceChange_timer2,
				timer_config1,
				timer_config2;
//				ep2_timer;

extern uint32_t timer_ep,
				timer_ep1;

extern unsigned int ttt1,
		   	   	    ttt2;

extern uint32_t transaction_period,
				transaction_period2;


//=====================================================
void dec_var()
	{
		t++; t2++; server_time++;
		if (server_time > 7) server_time = 7;

		_tt1++; _tt2++;
		if(_tt1 > 65534) _tt1 = 0;
		if(_tt2 > 65534) _tt2 = 0;
		ttt1++; ttt2++;

		priceChange_timer1++;
		priceChange_timer2++;
		if(priceChange_timer1 > 65534) priceChange_timer1 = 0;
		if(priceChange_timer2 > 65534) priceChange_timer2 = 0;

		timer_config1++;
		timer_config2++;
		if(timer_config1 > 65534) timer_config1 = 0;
		if(timer_config2 > 65534) timer_config2 = 0;


		totalizer1Timer++;
		totalizer2Timer++;

		if(totalizer1Timer > 65534) totalizer1Timer = 0;
		if(totalizer2Timer > 65534) totalizer2Timer = 0;

		if(ttt1 > 65534) ttt1 = 0;
		if(ttt2 > 65534) ttt2 = 0;

		timer_ep++;
		if(timer_ep > 85534) timer_ep = 0;
		timer_ep1++;
		if(timer_ep1 > 85534) timer_ep1 = 0;
//		ep2_timer++;
//		timer_spi++;
		transaction_period++;
		if(transaction_period > 4294967294) transaction_period = 0;
		transaction_period2++;
		if(transaction_period2 > 4294967294) transaction_period2 = 0;

	}
//------------------------------------------------------


//================================================================
/**
 *  routine is temporary its just to show that the system is alive
 */
void toggle_led(void)
{

	if (printer_time > 0)
	{
		printer_time--;
	}


	 static int count;
	 dec_var();
	 count++;
	 if(count >= 1000)
	 {
		 //=====================//
		 // shutdown timer
		   shutdown_timer1++;
		   if (shutdown_timer1 >= 300)  // 5 minutes
			      shutdown_timer1 = 300;
		   shutdown_timer2++;
		   if (shutdown_timer2 >= 300)  // 5 minutes
		 			      shutdown_timer2 = 300;
		 //=====================
		 count = 0;
		 HAL_GPIO_TogglePin(nLed_GPIO_Port, nLed_Pin);  // Toggle LED

		//*************************************************
		//*************************************************
		 // seperate these timer later into another routine

		 dec_timer();   //decrement the events timer.
	     tot_buttonpress_tmr++;
	     log_buttonpress_tmr++;
	     key_buttonpress_tmr++;
	     progExit_buttonpress_tmr1++;

	     dec_timer2();   //decrement the events timer.
		 tot_buttonpress_tmr2++;
		 log_buttonpress_tmr2++;
		 key_buttonpress_tmr2++;
		 progExit_buttonpress_tmr2++;
	 }
}

//========================================================

bool dec_prn_tm(int d)
{
	if (d == 0)
	{
	   if (printer_time > 0)
	   {
	    	return 0;
	   }
	  return 1;
	}
	else
	{
		if (HAL_GPIO_ReadPin(dtr_p1_GPIO_Port, dtr_p1_Pin) )
		{
			return 1;
		}
		return 0;
	}
}
//========================================================


void set_prn_tm(int tmm__)
{
	printer_time = tmm__;
}


