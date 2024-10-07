/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __keypad_module
#define __keypad_module

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "string.h"
#include "stddef.h"
#include "stdint.h"

uint8_t checkkey(void);
int scankey(void);
void keypad_ini(void);
int readkeyboard(int type);

uint8_t key_int(void);

//void write_keypad_lcd(int num);
uint8_t write_keypad_lcd(uint8_t fxn, char* num);

uint8_t keypad_lcd(uint8_t fxn, char* num);

extern int keynew;
extern int keyold;


//================================================
//  			    KEYPAD 2
//================================================

uint8_t checkkey2(void);
int scankey2(void);
void keypad_ini2(void);
int readkeyboard2(int type);

uint8_t key_int2(void);

//void write_keypad_lcd(int num);
uint8_t write_keypad_lcd2(uint8_t fxn, char* num);

uint8_t keypad_lcd2(int fxn,char* num);

extern int keynew2;
extern int keyold2;

#ifdef __cplusplus
}
#endif

#endif /*keypad_module */
