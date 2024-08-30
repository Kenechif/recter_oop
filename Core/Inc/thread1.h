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

extern const uint16_t totalizerDay_loc;

extern const uint16_t save_pumpType_loc_fram,
					  save_productType_loc_fram,
					  save_nozzleId_loc_fram,
					  totalizerDay_loc_fram;

//extern nozzle_state_enum nozzle_state;		//state of nozzle returned from pump

uint8_t awaiting_masterResponse;

uint8_t config_mode,
 	    config_found;

extern char rx_buf[pump_rx_bufsize];

void setup();
void run();

void dec_var(void);

uint8_t read_event1(void);
uint8_t read_event1_1(void);

void get_time(void);

void pumpType_configure(void);

void pumpType_configure_fram(void);

void pumpType_parse(void);

void pumpName_parse(void);

void pumpName_parse_fram(void);

void pumpType_write(void);

void pumpType_write_fram(void);

void firstTotalizer_day(void);

void firstTotalizerDay_write(void);

void eeprom_clearToken(void);

// write the


#ifdef __cplusplus
}
#endif

#endif /* INC_THREAD1_H_ */
