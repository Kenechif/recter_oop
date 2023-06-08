
#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stm32f4xx_hal.h"


void EEPROM_Write (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
int8_t EEPROM_Read (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
void EEPROM_PageErase (uint16_t page);

void EEPROM_Write_NUM (uint16_t page, uint16_t offset, float  fdata);
float EEPROM_Read_NUM (uint16_t page, uint16_t offset);

#ifdef __cplusplus
}
#endif

#endif /* INC_EEPROM_H_ */
