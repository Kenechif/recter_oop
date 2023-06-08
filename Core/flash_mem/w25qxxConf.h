/*
 * w25qxxConf.h
 *
 *  Created on: May 4, 2022
 *      Author: tunjo
 */

#ifndef FLASH_MEM_W25QXXCONF_H_
#define FLASH_MEM_W25QXXCONF_H_

#ifdef __cplusplus
extern "C" {
#endif

#define _W25QXX_SPI                   hspi1
#define _W25QXX_CS_GPIO               FLASH_CS_GPIO_Port
#define _W25QXX_CS_PIN                FLASH_CS_Pin
#define _W25QXX_USE_FREERTOS          0
#define _W25QXX_DEBUG                 0

#ifdef __cplusplus
}
#endif

#endif /* FLASH_MEM_W25QXXCONF_H_ */
