/*
 * thread1.h
 *
 *  Created on: Jan 25, 2022
 *      Author: Tunjow
 */

#ifndef INC_THREAD1_H_
#define INC_THREAD1_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "settings.h"

extern const int16_t totalizerDay_loc;

int8_t awaiting_masterResponse;

int8_t config_mode,
 	   config_found;

extern char rx_buf[pump_rx_bufsize];

void setup();
void run();

void dec_var(void);

int  read_event1(void);

void get_time(void);

void pumpType_configure(void);

void pumpType_parse(void);

void pumpName_parse(void);

void pumpType_write(void);

void firstTotalizer_day(void);

void firstTotalizerDay_write(void);

void eeprom_clearToken(void);

// write the


#ifdef __cplusplus
}
#endif

#endif /* INC_THREAD1_H_ */
