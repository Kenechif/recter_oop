/*
 * command.c
 *
 *  Created on: Feb 3, 2022
 *      Author: Tunjow
 */

#include "myLibraries.h"
#include "keypad_module.h"
#include "buffer.h"
#include "interrupt_codes.h"           // include interrupt codes
#include "command.h"
#include "stm32f4xx_hal.h"

#include <stdio.h>


extern char key_lcd[10];
extern char key_lcd2[10];

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;

//Data received ! check integrity..and parse...
	 /*
	 *
	 * struct rcv_structure
	 {
	     uint8_t preamble;	  //just a unique byte data
	     uint8_t crc;		  //
	     uint8_t command;
	     uint8_t payload_len; //4
	     uint8_t data[20];    //24
	 	uint8_t footer;      //25
	 } ; //MsgStruct
	 *
	 */
/*

validate_packet(rcv_structure *bf)
{

}

 void docommand(rcv_structure *bf)
 {
     uint8_t cmdd = bf->command;
 }
*/

void build_packet(uint8_t cmd,char* c)
{
   packet.preamble = preamb;       //synch.preamb;
   packet.footer = footer;
   packet.crc = 0;
   packet.command = cmd;
   uint8_t ll = strlen(c);
   //if( ll  == 0) ll = 1
   if( ll  > 10) ll = 10;
   packet.payload_len = ll;
   //----------------------
   int cll = 0;
   while (cll < 10) packet.data[cll++] = 0;
   for(int i  = 0 ;i<ll;i++)
	   {
		  packet.data[i] = c[i];  //populate the data entity
	   }
}

//extern
char M[10]={0};
extern UART_HandleTypeDef huart1;
extern int32_t count__;
extern uint32_t pulser2;

char M2[10]={0};
extern UART_HandleTypeDef huart1;
extern int32_t count__2;
extern uint32_t pulser2;

//void send_()
//{
//	HAL_UART_Transmit (&huart1,&packet,25-10,3000); //(&huart1,&packet,);
//}


/////////////////////////////////////////////////
void lcd_print_line1(char lcd_[10])
{
	 printDisp_c(lcd_,1,0,8,RT,CLEAR);
}

void lcd_print_line2(char lcd_[10])
{
	 printDisp_c(lcd_,2,0,8,RT,CLEAR);
}

void lcd_print_line3(char lcd_[10])
{
	 printDisp_c(lcd_,3,0,7,RT,CLEAR);
}

void keypad_print(char lcd_[8])
{
	 for (int i = 0; i < 8; i++)
	 {
		 key_lcd[i] = lcd_[i];
	 }

	 keypad_lcd(0,key_lcd);
}

void send_solenoid(int v_)
{
   drive_solenoid1(v_);
}

void send_pump(int v_)
{
   drive_pump1(v_);
}

void send_totaliser(int v_)
{
	drive_totaliser1(1);
	//__asm_('nop');
	drive_totaliser1(0);
}

uint32_t get_pulser1()
{
	return  __HAL_TIM_GET_COUNTER(&htim5);
}

uint32_t get_pulser2()
{
	return  __HAL_TIM_GET_COUNTER(&htim2);
}

void clr_keypad()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void lcd_print_line1_2(char lcd_[10])
{
	 printDisp_c2(lcd_,1,0,8,RT,CLEAR);
}

void lcd_print_line2_2(char lcd_[10])
{
	 printDisp_c2(lcd_,2,0,8,RT,CLEAR);
}

void lcd_print_line3_2(char lcd_[10])
{
	 printDisp_c2(lcd_,3,0,7,RT,CLEAR);
}

void keypad_print2(char lcd_[8])
{
	for (uint8_t i = 0; i < 8; i++)
	 {
		 key_lcd2[i] = lcd_[i];
	 }

	  keypad_lcd2(0, key_lcd2);
}

//void send_solenoid2(int v_)
//{
//   drive_solenoid1(v_);
//}
//
//void send_pump2(int v_)
//{
//   drive_pump1(v_);
//}
//
//void send_totaliser2(int v_)
//{
//	drive_totaliser1(1);
//	//__asm_('nop');
//	drive_totaliser1(0);
//}

void clr_keypad2()
{

}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
