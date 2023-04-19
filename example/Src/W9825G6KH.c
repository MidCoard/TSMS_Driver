#include "W9825G6KH.h"
#include "fmc.h"

static TSMS_RESULT W9825G6KH_writeCommand(struct W9825G6KH_Handler *handler, uint8_t command, uint8_t refresh, uint16_t value) {
	if (handler == TSMS_NULL)
		return TSMS_ERROR;
	FMC_SDRAM_CommandTypeDef commandTypeDef;
	commandTypeDef.CommandMode = command;
	commandTypeDef.CommandTarget = handler->sdram->Init.SDBank == FMC_SDRAM_BANK1 ? FMC_SDRAM_CMD_TARGET_BANK1 : FMC_SDRAM_CMD_TARGET_BANK2;
	commandTypeDef.AutoRefreshNumber = refresh;
	commandTypeDef.ModeRegisterDefinition = value;
	return HAL_SDRAM_SendCommand(handler->sdram, &commandTypeDef, 0xFFFF) == HAL_OK ? TSMS_SUCCESS : TSMS_FAIL;
}

struct W9825G6KH_Handler * W9825G6KH_initHardware(SDRAM_HandleTypeDef * sdram, uint32_t* address) {
	struct W9825G6KH_Handler * handler = malloc(sizeof(struct W9825G6KH_Handler));
	handler->sdram = sdram;
	handler->address = address;
	if (W9825G6KH_writeCommand(handler, FMC_SDRAM_CMD_CLK_ENABLE, 1, 0) != TSMS_SUCCESS) {
		free(handler);
		return TSMS_NULL;
	}
	TSMS_delay(1);
	if (W9825G6KH_writeCommand(handler, FMC_SDRAM_CMD_PALL, 1, 0) != TSMS_SUCCESS) {
		free(handler);
		return TSMS_NULL;
	}
	if (W9825G6KH_writeCommand(handler, FMC_SDRAM_CMD_AUTOREFRESH_MODE, 8, 0) != TSMS_SUCCESS) {
		free(handler);
		return TSMS_NULL;
	}
	uint16_t modeConfig = SDRAM_BURST_LENGTH_1 | SDRAM_BURST_TYPE_SEQUENTIAL | SDRAM_WRITE_BURST_MODE_SINGLE;
	if (sdram->Init.CASLatency == FMC_SDRAM_CAS_LATENCY_2)
		modeConfig |= SDRAM_CAS_LATENCY_2;
	else if (sdram->Init.CASLatency == FMC_SDRAM_CAS_LATENCY_3)
		modeConfig |= SDRAM_CAS_LATENCY_3;
	else {
		free(handler);
		return TSMS_NULL;
	}
	if (W9825G6KH_writeCommand(handler, FMC_SDRAM_CMD_LOAD_MODE, 1,
	                       modeConfig
						   ) != TSMS_SUCCESS) {
		free(handler);
		return TSMS_NULL;
	}
	if (HAL_SDRAM_ProgramRefreshRate(handler->sdram, 761) != HAL_OK) {
		free(handler);
		return TSMS_NULL;
	}
	return handler;
}

TSMS_RESULT W9825G6KH_writeBuffer(struct W9825G6KH_Handler *handler, uint32_t offset, const uint16_t * data, uint32_t size) {
	if (handler == TSMS_NULL)
		return TSMS_ERROR;
	uint16_t * address = ((uint16_t*)handler->address) + offset;
	for (uint32_t i = 0; i < size; i++) {
		*address = data[i];
		address++;
	}
	return TSMS_SUCCESS;
}

TSMS_RESULT W9825G6KH_readBuffer(struct W9825G6KH_Handler *handler, uint32_t offset, uint16_t * data, uint32_t size) {
	if (handler == TSMS_NULL)
		return TSMS_ERROR;
	uint16_t * address = ((uint16_t*)handler->address) + offset;
	for (uint32_t i = 0; i < size; i++) {
		data[i] = *address;
		address++;
	}
	return TSMS_SUCCESS;
}