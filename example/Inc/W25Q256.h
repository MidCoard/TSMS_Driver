#ifndef TSMS_W25Q256_H
#define TSMS_W25Q256_H

#define W25Q256_REGISTER_STATUS_1 0x05
#define W25Q256_REGISTER_STATUS_2 0x35
#define W25Q256_REGISTER_STATUS_3 0x15
#define W25Q256_REGISTER_MANUFACT_DEVICE_ID 0x90

#define W25Q256_COMMAND_WRITE_ENABLE 0x06
#define W25Q256_COMMAND_ENTER_QPI_MODE 0x38
#define W25Q256_COMMAND_4_BYTE_ADDRESSING_ENABLE 0xB7
#define W25Q256_COMMAND_SET_READ_PARAMETERS 0xC0
#define W25Q256_COMMAND_FAST_READ_DATA 0x0B
#define W25Q256_COMMAND_PAGE_PROGRAM 0x02
#define W25Q256_COMMAND_SECTOR_ERASE 0x20
#define W25Q256_COMMAND_CHIP_ERASE 0xC7

#include "tsms.h"

struct W25Q256_Handler {

#ifdef TSMS_STM32_FLASH
	QSPI_HandleTypeDef * qspi;
#endif
	uint32_t *address;
	bool qspiMode;
	uint16_t id;
};

#ifdef TSMS_STM32_FLASH
struct W25Q256_Handler * W25Q256_initHardware(QSPI_HandleTypeDef * qspi, uint32_t * address);
#endif

TSMS_RESULT W25Q256_read(struct W25Q256_Handler * handler, uint32_t address, uint8_t * data, uint32_t length);

TSMS_RESULT W25Q256_writeInPage(struct W25Q256_Handler * handler, uint32_t address, uint8_t * data, uint32_t length);

TSMS_RESULT W25Q256_write0(struct W25Q256_Handler * handler, uint32_t address, uint8_t * data, uint32_t length);

TSMS_RESULT W25Q256_write(struct W25Q256_Handler * handler, uint32_t address, uint8_t * data, uint32_t length);

TSMS_RESULT W25Q256_erase(struct W25Q256_Handler * handler, uint32_t page);

TSMS_RESULT W25Q256_eraseAll(struct W25Q256_Handler * handler);

#endif //TSMS_W25Q256_H