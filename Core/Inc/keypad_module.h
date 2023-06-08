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

int checkkey(void);
int scankey(void);
void keypad_ini(void);
int readkeyboard(int type);

int key_int(void);

//void write_keypad_lcd(int num);
int write_keypad_lcd(int fxn,char* num);

int keypad_lcd(int fxn,char* num);

extern int keynew;
extern int keyold;


//================================================
//  			    KEYPAD 2
//================================================

int checkkey2(void);
int scankey2(void);
void keypad_ini2(void);
int readkeyboard2(int type);

int key_int2(void);

//void write_keypad_lcd(int num);
int write_keypad_lcd2(int fxn,char* num);

int keypad_lcd2(int fxn,char* num);

extern int keynew2;
extern int keyold2;

#ifdef __cplusplus
}
#endif

#endif /*keypad_module */
