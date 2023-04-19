#ifndef TSMS_W9825G6KH_H
#define TSMS_W9825G6KH_H

#define SDRAM_BURST_LENGTH_1             0x0000
#define SDRAM_BURST_LENGTH_2             0x0001
#define SDRAM_BURST_LENGTH_4             0x0002
#define SDRAM_BURST_LENGTH_8             0x0004
#define SDRAM_BURST_TYPE_SEQUENTIAL      0x0000
#define SDRAM_BURST_TYPE_INTERLEAVED     0x0008
#define SDRAM_CAS_LATENCY_2              0x0020
#define SDRAM_CAS_LATENCY_3              0x0030
#define SDRAM_OPERATING_MODE_STANDARD    0x0000
#define SDRAM_WRITE_BURST_MODE_PROGRAMMED 0x0000
#define SDRAM_WRITE_BURST_MODE_SINGLE     0x0200

#include "tsms_def.h"

#include "tsms_driver.h"

struct W9825G6KH_Handler {

#ifdef TSMS_STM32_SDRAM
	SDRAM_HandleTypeDef* sdram;
#endif
	uint32_t *address;
};

#ifdef TSMS_STM32_SDRAM
struct W9825G6KH_Handler * W9825G6KH_initHardware(SDRAM_HandleTypeDef * sdram, uint32_t * address);
#endif

TSMS_RESULT W9825G6KH_writeBuffer(struct W9825G6KH_Handler *handler, uint32_t offset, const uint16_t * data, uint32_t size);

TSMS_RESULT W9825G6KH_readBuffer(struct W9825G6KH_Handler *handler, uint32_t offset, uint16_t * data, uint32_t size);

#endif //TSMS_W9825G6KH_H
