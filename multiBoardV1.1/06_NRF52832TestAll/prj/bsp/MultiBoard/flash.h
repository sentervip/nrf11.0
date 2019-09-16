#ifndef __FLASH_H__
#define __FLASH_H__

#include "nrf_drv_spi.h"
#include "boards.h"
#include "nrf_gpio.h"

#define FLASH_CS_H									nrf_gpio_pin_set(SPIM2_CS_PIN)
#define FLASH_CS_L									nrf_gpio_pin_clear(SPIM2_CS_PIN)

//M25P16 commander define
#define FLASH_WRITE_ENABLE							0X06
#define FLASH_WRITE_DISABLE							0X04
#define FLASH_READ_ID								0X9F
#define FLASH_READ_STATUS_REG						0X05
#define FLASH_WRITE_STATUS_REG						0X01
#define FLASH_WRITE_LOCK_REG						0XE5
#define FLASH_READ_LOCK_REG							0XE8
#define FLASH_READ_BYTES							0X03
#define FLASH_READ_BYTES_HSPEED						0X0B
#define FLASH_PAGE_WRITE							0X0A
#define FLASH_PAGE_PROGRAM							0X02
#define FLASH_PAGE_ERASE							0XDB
#define FLASH_SUBSECTOR_ERASE						0X20
#define FLASH_SECTOR_ERASE							0XD8
#define FLASH_BULK_ERASE							0XC7
#define FLASH_SLEEP									0XB9
#define FLASH_WAKEUP								0XAB


//define flash state register bit
#define FLASH_STATE_BIT_SRWD						7
#define FLASH_STATE_BIT_BP2							4
#define FLASH_STATE_BIT_BP1							3
#define FLASH_STATE_BIT_BP0							2
#define FLASH_STATE_BIT_WEL							1
#define FLASH_STATE_BIT_WIP							0

void initSpiToFlash(void);
void initFlash(void);
uint32_t readFlashId(void);
void writeFlashCmd(uint8_t fu8Cmd);
void writeFlashPage(uint8_t *fpu8DataBuff, uint32_t fu32Add);
void readFlashPage(uint8_t *fpu8DataBuff, uint32_t fu32Add);
void readFlashBytes(uint8_t *fpu8DataBuff,  uint32_t fu32Add, uint8_t fu8Len);
void testPageOp(void);
void unInitFlash(void);
#endif
