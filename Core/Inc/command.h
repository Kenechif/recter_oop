/*
 * command.h
 *
 *  Created on: Feb 3, 2022
 *      Author: Tunjow
 */

#ifndef INC_COMMAND_H_
#define INC_COMMAND_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "buffer.h"

 typedef enum
 {
     preamb  = 200,    //preamble byte.
     footer  = 240     //footer byte.
 }synch;

//===========   commands  =========

    typedef enum
{
        clear_lcd,
        write_line1,
        write_line2,
        write_line3,
        write_keypad,

        pulser_count_ch1,
        key19,
        totaliser,
        nozzleswitch,
        reset_key,
		keypress,

        solenoid_ctrl,      //' send payload 1 for ON, 0 for OFF
        pump_ctrl,          //' send payload 1 for ON, 0 for OFF
		//---------------- added 2/3/2022
		pulser_clr,
		pulser_sell,
		lcd_clr,
		keypad_clr

} command;

extern struct rcv_structure packet;

extern fifo_t  dat_str;  //create an instance of the buffer

/*
void validate_packet(rcv_structure *bf);
void  void docommand(rcv_structure *bf);  */

void send_pulser1();
void send_key19();
void send_resetkey();
void send_key();
void send_nozzle();
void send_keyboard();
void send_totaliser();

//from master..

void send_line1(char lcd_[10]);
void send_line2(char lcd_[10]);
void send_line3(char lcd_[10]);
void send_keypad(char lcd_[7]);

void send_solenoid(int v_);
void send_pump(int v_);
void clr_pulser();
void sell_pulser(int v_);
void clr_lcd();
void clr_keypad();

void send_line12(char lcd_[10]);
void send_line22(char lcd_[10]);
void send_line32(char lcd_[10]);
void send_keypad2(char lcd_[7]);

void send_solenoid2(int v_);
void send_pump2(int v_);
void clr_pulser2();
void sell_pulser2(int v_);
void clr_lcd2();
void clr_keypad2();

#ifdef __cplusplus
}
#endif

#endif /* INC_COMMAND_H_ */
