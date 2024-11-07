/*
 * myLibraries.h
 *
 *  Created on: Aug 5, 2021
 *      Author: HP
 */

#ifndef INC_MYLIBRARIES_H_
#define INC_MYLIBRARIES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

typedef enum{
	LSB_FIRST,
	MSB_FIRST
}shift_out_type;

typedef enum{
	RT,
	LT
}justify_;

typedef enum{
	NOCLEAR,
	CLEAR
}clear_;

typedef enum{
	DEACTIVATE,
	ACTIVATE
}drive;

#define MAX_STRLEN 23   //40

extern uint32_t millis;

/***
 * Enables other source files reference functions in the
 * source file with the same name as this header file
 */

void shiftOut(uint8_t data_byte, uint8_t lat);
int32_t Multiplex( int row, int col);  //char *str,

void printDisp_i(int32_t num, int row, int col, int n, justify_ jst, clear_ cl);
void printDisp_f(float num, uint8_t row, uint8_t col, int8_t n, justify_ jst, clear_ cl);
void printDisp_c(char *num, uint8_t row, uint8_t col, int8_t n, justify_ jst, clear_ cl);

void process(int isflt, char* str, int8_t cnv, int row, int col, int n, int justify, int clear );
void _Delay(int v);

void clear_screen1();
uint8_t ToBits(int Input);
uint8_t ToInt(char c);

int readsettingskey_state(void);
uint8_t readkey19_state(void);

///////////////////////////////////////////////////////////////////////////////////////////////
void shiftOut2(uint8_t data_byte, uint8_t lat);
int32_t Multiplex2( int row, int col);  //char *str,

void printDisp_i2(int32_t num, int row, int col,int n,justify_ jst,clear_ cl);
void printDisp_f2(float num, int row, int col,int n,justify_ jst,clear_ cl);
void printDisp_c2(char* num, int row, int col,int n,justify_ jst,clear_ cl);

void process2(int isflt,char* str,int8_t cnv, int row, int col,int n ,int justify ,int clear );
void _Delay2(int v);

void clear_screen2();

//uint8_t ToBits(int Input);
//uint8_t ToInt(char c);

int readsettingskey2_state(void);
uint8_t readkey192_state(void);
///////////////////////////////////////////////////////////////////////////////////////////////

//hardware  routines.
int readNozzle1(void);
int readtotaliser1_state(void);
int readsettingskey_state(void);

void drive_totaliser1(drive drv);
void drive_solenoid1(drive drv);
void drive_pump1(drive drv);

void slow_flow1(void);
void fast_flow1(void);
void stop_flow1(void);

//=================================
int readNozzle2(void);
int readtotaliser2_state(void);
int readsettingske2_state(void);

void drive_totaliser2(drive drv);
void drive_solenoid2(drive drv);
void drive_pump2(drive drv);

void slow_flow2(void);
void fast_flow2(void);
void stop_flow2(void);

//---------------------------
int read_p_pwr(void);
int readpwr(void);
void displayandkeypad_power(drive drv);
float battery_read(void);
float battery_sense(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_MYLIBRARIES_H_ */
