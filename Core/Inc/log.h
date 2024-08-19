/*
 * log.h
 *
 *  Created on: 27 Apr 2022
 *      Author: tunjo
 */

#ifndef INC_LOG_H_
#define INC_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "states.h"


void flash_info_read(void);
void update_info(void);          //save_log();

//void read_log();

eSystemState read_flash_State_Handler(void);
eSystemState write_flash_State_Handler(void);

void clear_logA(void);
void clear_logB(void);

void clear_totA(void);
void clear_totB(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_LOG_H_ */
