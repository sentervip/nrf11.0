#include "flash.h"
#include "app_util_platform.h"
#include "oled.h"
#include "nrf_delay.h"
#include "string.h"

nrf_drv_spi_t gtFlashSpi = NRF_DRV_SPI_INSTANCE(2);

void initSpiToFlash(void)
{
	nrf_drv_spi_config_t ltSpiCfg = NRF_DRV_SPI_DEFAULT_CONFIG(2);
	ltSpiCfg.sck_pin = SPIM2_SCK_PIN;
	ltSpiCfg.mosi_pin = SPIM2_MOSI_PIN;
	ltSpiCfg.miso_pin = SPIM2_MISO_PIN;
	ltSpiCfg.frequency = NRF_DRV_SPI_FREQ_8M;
	ltSpiCfg.bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST;
	nrf_drv_spi_init(&gtFlashSpi, &ltSpiCfg, NULL);
}

void initFlash(void)
{
	initSpiToFlash();
	nrf_gpio_cfg_output(SPIM2_CS_PIN);
	FLASH_CS_H;
#if 0//clear all flash
	nrf_delay_ms(1);
	writeFlashCmd(FLASH_WRITE_ENABLE);
	writeFlashCmd(FLASH_BULK_ERASE);
#endif
#if 0
	uint8_t lau8Data[256] = {0};
	nrf_delay_ms(1);
	readFlashBytes(lau8Data, 0, 128);
#endif
}

void unInitFlash(void)
{
	nrf_drv_spi_uninit(&gtFlashSpi);
}

uint32_t readFlashId(void)
{
	uint32_t lu32FlashId = 0;
	uint8_t lu8Cmd = FLASH_READ_ID;
    FLASH_CS_L;
	nrf_delay_ms(10);
	nrf_drv_spi_transfer(&gtFlashSpi, &lu8Cmd, 1, (uint8_t *)&lu32FlashId, 4);
	FLASH_CS_H;
	return lu32FlashId;
}

void writeFlashCmd(uint8_t fu8Cmd)
{
	FLASH_CS_L;
	nrf_drv_spi_transfer(&gtFlashSpi, &fu8Cmd, 1, NULL, 0);
	FLASH_CS_H;
}

void writeFlashPage(uint8_t *fpu8DataBuff, uint32_t fu32Add)
{
	uint8_t lau8CmdAndData[4] = {0};
	lau8CmdAndData[0] = FLASH_PAGE_WRITE;
	lau8CmdAndData[1] = fu32Add >> 16;
	lau8CmdAndData[2] = fu32Add >> 8;
	lau8CmdAndData[3] = fu32Add;
	
	writeFlashCmd(FLASH_WRITE_ENABLE);
	FLASH_CS_L;
	nrf_drv_spi_transfer(&gtFlashSpi, lau8CmdAndData, 4, NULL, 0);
	nrf_drv_spi_transfer(&gtFlashSpi, fpu8DataBuff, 128, NULL, 0);
	nrf_drv_spi_transfer(&gtFlashSpi, &fpu8DataBuff[128], 128, NULL, 0);
	FLASH_CS_H;	
	nrf_delay_ms(50);
	//writeFlashCmd(FLASH_WRITE_DISABLE);
}

void readFlashPage(uint8_t *fpu8DataBuff, uint32_t fu32Add)
{
	uint8_t lau8CmdAndData[4] = {0};
	lau8CmdAndData[0] = FLASH_READ_BYTES;
	lau8CmdAndData[1] = fu32Add >> 16;
	lau8CmdAndData[2] = fu32Add >> 8;
	lau8CmdAndData[3] = fu32Add;
	
	FLASH_CS_L;
	nrf_drv_spi_transfer(&gtFlashSpi, lau8CmdAndData, 4, NULL, 0);
	nrf_drv_spi_transfer(&gtFlashSpi, NULL, 0, fpu8DataBuff, 128);
	nrf_drv_spi_transfer(&gtFlashSpi, NULL, 0, &fpu8DataBuff[128], 128);
	FLASH_CS_H;
}

void readFlashBytes(uint8_t *fpu8DataBuff,  uint32_t fu32Add, uint8_t fu8Len)
{
	uint8_t lau8CmdAndData[4] = {0};
	lau8CmdAndData[0] = FLASH_READ_BYTES;
	lau8CmdAndData[1] = fu32Add >> 16;
	lau8CmdAndData[2] = fu32Add >> 8;
	lau8CmdAndData[3] = fu32Add;
	
	FLASH_CS_L;
	nrf_drv_spi_transfer(&gtFlashSpi, lau8CmdAndData, 4, NULL, 0);
	nrf_drv_spi_transfer(&gtFlashSpi, NULL, 0, fpu8DataBuff, fu8Len);
	FLASH_CS_H;
}
uint32_t lu32FlashId = 0;
uint8_t lau8data[256] = {0};
void testPageOp(void)
{	
	lu32FlashId = readFlashId();

	nrf_delay_ms(1);
	for(uint16_t i = 0; i < 256; i ++)
	{
		lau8data[i] = i;
	}
	writeFlashPage(lau8data, 0);
	nrf_delay_ms(50);
	for(uint16_t i = 0; i < 256; i ++)
	{
		lau8data[i] = 0;
	}
	readFlashPage(lau8data, 0);
	
	lu32FlashId = readFlashId();
	//writeFlashCmd(FLASH_SLEEP);
	UNUSED_PARAMETER(lu32FlashId);
}

