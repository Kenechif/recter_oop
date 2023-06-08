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
extern int key_buttonpress_tmr;

extern int tot_buttonpress_tmr2;
extern int log_buttonpress_tmr2;
extern int key_buttonpress_tmr2;

extern uint16_t shutdown_timer;

uint16_t printer_time = 0;
uint8_t server_time = 0;

extern int t, t2; //ttt;
extern uint16_t _tt,
				_tt2,
				timer_ep,
				timer_spi;
//				ep2_timer;

extern int ttt;


//=====================================================
void dec_var()
	{
		t++; t2++; server_time++;
		if (server_time > 7) server_time = 7;

		_tt++; _tt2++;
		if(_tt > 65534) _tt = 0;
		if(_tt2 > 65534) _tt2 = 0;
		ttt++;
//		if(ttt > 65534) ttt = 0;

		timer_ep++;
//		ep2_timer++;
		timer_spi++;
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
		   shutdown_timer++;
		   if (shutdown_timer >= 300)  // 5 minutes
			      shutdown_timer = 300;
		 //=====================
		 count = 0;
		 HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin);  // Toggle LED

		//*************************************************
		//*************************************************
		 // seperate these timer later into another routine

		 dec_timer();   //decrement the events timer.
	     tot_buttonpress_tmr++;
	     log_buttonpress_tmr++;
	     key_buttonpress_tmr++;

	     dec_timer2();   //decrement the events timer.
		 tot_buttonpress_tmr2++;
		 log_buttonpress_tmr2++;
		 key_buttonpress_tmr2++;
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


