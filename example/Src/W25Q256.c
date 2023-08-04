#include "W25Q256.h"
#include "tsms_printer.h"

static uint8_t W25Q256_data[4096];

static TSMS_RESULT W25Q256_transmit(struct W25Q256_Handler * handler, uint8_t * data, uint32_t size) {
#ifdef TSMS_STM32_FLASH
	handler->qspi->Instance->DLR = size - 1;
	return HAL_QSPI_Transmit(handler->qspi, data, 5000) == HAL_OK ? TSMS_SUCCESS : TSMS_ERROR;
#else
	return TSMS_FAIL;
#endif
}

static TSMS_RESULT W25Q256_receive(struct W25Q256_Handler * handler, uint8_t * data, uint32_t size) {
#ifdef TSMS_STM32_FLASH
	handler->qspi->Instance->DLR = size - 1;
	return HAL_QSPI_Receive(handler->qspi, data, 5000) == HAL_OK ? TSMS_SUCCESS : TSMS_ERROR;
#else
	return TSMS_FAIL;
#endif
}

static TSMS_RESULT W25Q256_writeCommand(struct W25Q256_Handler * handler, uint32_t instruction, uint32_t address, uint32_t dummyCycles, uint32_t instructionMode, uint32_t addressMode, uint32_t addressSize, uint32_t dataMode) {
#ifdef TSMS_STM32_FLASH
	QSPI_CommandTypeDef command;
	command.Instruction = instruction;
	command.Address = address;
	command.DummyCycles = dummyCycles;
	command.InstructionMode = instructionMode;
	command.AddressMode = addressMode;
	command.AddressSize = addressSize;
	command.DataMode = dataMode;
	command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	command.DdrMode = QSPI_DDR_MODE_DISABLE;
	command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	return HAL_QSPI_Command(handler->qspi, &command, 5000) == HAL_OK ? TSMS_SUCCESS : TSMS_ERROR;
#else
	return TSMS_FAIL;
#endif
}

static TSMS_RESULT W25Q256_writeRegister(struct W25Q256_Handler * handler, uint8_t reg, uint8_t value) {
#ifdef TSMS_STM32_FLASH
	TSMS_RESULT status = handler->qspiMode ? W25Q256_writeCommand(handler, reg, 0, 0, QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_NONE, QSPI_ADDRESS_8_BITS, QSPI_DATA_4_LINES) :
			W25Q256_writeCommand(handler, reg, 0, 0, QSPI_INSTRUCTION_1_LINE, QSPI_ADDRESS_NONE, QSPI_ADDRESS_8_BITS, QSPI_DATA_1_LINE);
	return status | W25Q256_transmit(handler, &value, 1);
#else
	return TSMS_FAIL;
#endif
}

static uint8_t W25Q256_readRegister(struct W25Q256_Handler * handler, uint8_t reg) {
#ifdef TSMS_STM32_FLASH
	handler->qspiMode ? W25Q256_writeCommand(handler, reg, 0, 0, QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_NONE, QSPI_ADDRESS_8_BITS, QSPI_DATA_4_LINES) :
			W25Q256_writeCommand(handler, reg, 0, 0, QSPI_INSTRUCTION_1_LINE, QSPI_ADDRESS_NONE, QSPI_ADDRESS_8_BITS, QSPI_DATA_1_LINE);
	W25Q256_receive(handler, W25Q256_data, 1);
	return  W25Q256_data[0];
#else
	return TSMS_FAIL;
#endif
}

static uint16_t W25Q256_readId(struct W25Q256_Handler * handler) {
#ifdef TSMS_STM32_FLASH
	handler->qspiMode ? W25Q256_writeCommand(handler, W25Q256_REGISTER_MANUFACT_DEVICE_ID, 0, 0, QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_4_LINES, QSPI_ADDRESS_24_BITS, QSPI_DATA_4_LINES) :
			W25Q256_writeCommand(handler, W25Q256_REGISTER_MANUFACT_DEVICE_ID, 0, 0, QSPI_INSTRUCTION_1_LINE, QSPI_ADDRESS_1_LINE, QSPI_ADDRESS_24_BITS, QSPI_DATA_1_LINE);
	W25Q256_receive(handler, W25Q256_data, 2);
	uint16_t id = W25Q256_data[0] << 8 | W25Q256_data[1];
	return id;
#else
	return TSMS_FAIL;
#endif
}

static TSMS_RESULT W25Q256_enableWrite(struct W25Q256_Handler * handler) {
#ifdef TSMS_STM32_FLASH
	return handler->qspiMode ? W25Q256_writeCommand(handler, W25Q256_COMMAND_WRITE_ENABLE, 0, 0, QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_NONE, QSPI_ADDRESS_8_BITS, QSPI_DATA_NONE) :
			W25Q256_writeCommand(handler, W25Q256_COMMAND_WRITE_ENABLE, 0, 0, QSPI_INSTRUCTION_1_LINE, QSPI_ADDRESS_NONE, QSPI_ADDRESS_8_BITS, QSPI_DATA_NONE);
#else
	return TSMS_FAIL;
#endif
}

static TSMS_RESULT W25Q256_waitBusy(struct W25Q256_Handler * handler) {
#ifdef TSMS_STM32_FLASH
	uint8_t status;
	do {
		status = W25Q256_readRegister(handler, W25Q256_REGISTER_STATUS_1);
	} while (status & 0x01);
	return TSMS_SUCCESS;
#else
	return TSMS_FAIL;
#endif
}
#ifdef TSMS_STM32_FLASH
struct W25Q256_Handler * W25Q256_initHardware(QSPI_HandleTypeDef * qspi, uint32_t * address) {
	struct W25Q256_Handler * handler = TSMS_malloc(sizeof(struct W25Q256_Handler));
	handler->qspi = qspi;
	handler->address = address;
	handler->qspiMode = false;
	uint8_t status2  = W25Q256_readRegister(handler, W25Q256_REGISTER_STATUS_2);
	if (!(status2 & 0x02)) {
		W25Q256_enableWrite(handler);
		W25Q256_writeRegister(handler, W25Q256_REGISTER_STATUS_2, status2 | 0x02);
	}
	W25Q256_writeCommand(handler, W25Q256_COMMAND_ENTER_QPI_MODE, 0, 0, QSPI_INSTRUCTION_1_LINE, QSPI_ADDRESS_NONE, QSPI_ADDRESS_8_BITS, QSPI_DATA_NONE);
	handler->qspiMode = true;

	handler->id = W25Q256_readId(handler);

	volatile uint8_t status3 = W25Q256_readRegister(handler, W25Q256_REGISTER_STATUS_3);
	if (!(status3 & 0x01)) {
		W25Q256_enableWrite(handler);
		W25Q256_writeCommand(handler, W25Q256_COMMAND_4_BYTE_ADDRESSING_ENABLE, 0, 0, QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_NONE, QSPI_ADDRESS_8_BITS, QSPI_DATA_NONE);
	}
	W25Q256_enableWrite(handler);
	W25Q256_writeCommand(handler, W25Q256_COMMAND_SET_READ_PARAMETERS, 0, 0, QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_NONE, QSPI_ADDRESS_8_BITS, QSPI_DATA_4_LINES);

	W25Q256_data[0] = 0x30;
	W25Q256_transmit(handler, W25Q256_data, 1);


	return handler;
}
#endif

TSMS_RESULT W25Q256_read(struct W25Q256_Handler * handler, uint32_t address, uint8_t * data, uint32_t length) {
#ifdef TSMS_STM32_FLASH
	W25Q256_writeCommand(handler, W25Q256_COMMAND_FAST_READ_DATA, address, 8, QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_4_LINES, QSPI_ADDRESS_32_BITS, QSPI_DATA_4_LINES);
	return W25Q256_receive(handler, data, length);
#else
	return TSMS_FAIL;
#endif
}

TSMS_RESULT W25Q256_writeInPage(struct W25Q256_Handler * handler, uint32_t address, uint8_t * data, uint32_t length) {
#ifdef TSMS_STM32_FLASH
	TSMS_RESULT result = W25Q256_enableWrite(handler);
	result |= W25Q256_writeCommand(handler, W25Q256_COMMAND_PAGE_PROGRAM, address, 0, QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_4_LINES, QSPI_ADDRESS_32_BITS, QSPI_DATA_4_LINES);
	result |= W25Q256_transmit(handler, data, length);
	return result | W25Q256_waitBusy(handler);
#else
	return TSMS_FAIL;
#endif
}

TSMS_RESULT W25Q256_write0(struct W25Q256_Handler * handler, uint32_t address, uint8_t * data, uint32_t length) {
	uint32_t pageOffset = address & 0x000000FF;
	uint32_t pageLength = 0x100 - pageOffset;
	if (pageLength > length) {
		pageLength = length;
	}
	while (true) {
		TSMS_RESULT result = W25Q256_writeInPage(handler, address, data, pageLength);
		if (result != TSMS_SUCCESS) {
			return result;
		}
		if (pageLength == length) {
			return TSMS_SUCCESS;
		}
		address += pageLength;
		data += pageLength;
		length -= pageLength;
		pageLength = length > 0x100 ? 0x100 : length;
	}
}

TSMS_RESULT W25Q256_write(struct W25Q256_Handler * handler, uint32_t address, uint8_t * data, uint32_t length) {
	uint32_t pageIndex = address >> 12;
	uint32_t pageOffset = address & 0x00000FFF;
	uint32_t pageLength = 0x1000 - pageOffset;
	if (pageLength > length) {
		pageLength = length;
	}
	while (true) {
		W25Q256_read(handler, pageIndex << 12, W25Q256_data, 4096);
		uint32_t pos = 0;
		for (; pos < pageLength; pos++) {
			if (W25Q256_data[pageOffset + pos] != 0xFF) {
				break;
			}
		}
		if (pos < pageLength) {
			W25Q256_erase(handler, pageIndex);
			for (pos = 0; pos < pageLength; pos++) {
				W25Q256_data[pageOffset + pos] = data[pos];
			}
			TSMS_RESULT result = W25Q256_write0(handler, pageIndex << 12, W25Q256_data, 4096);
			if (result != TSMS_SUCCESS) {
				return result;
			}
		} else {
			TSMS_RESULT result = W25Q256_write0(handler, address, data, pageLength);
			if (result != TSMS_SUCCESS) {
				return result;
			}
		}
		if (pageLength == length) {
			return TSMS_SUCCESS;
		}
		address += pageLength;
		data += pageLength;
		length -= pageLength;
		pageIndex++;
		pageOffset = 0;
		pageLength = length > 0x1000 ? 0x1000 : length;
	}
}

TSMS_RESULT W25Q256_erase(struct W25Q256_Handler * handler, uint32_t page) {
#ifdef TSMS_STM32_FLASH
	TSMS_RESULT result = W25Q256_enableWrite(handler);
	result |= W25Q256_waitBusy(handler);
	result |= W25Q256_writeCommand(handler, W25Q256_COMMAND_SECTOR_ERASE, page << 12, 0, QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_4_LINES, QSPI_ADDRESS_32_BITS, QSPI_DATA_NONE);
	return result | W25Q256_waitBusy(handler);
#else
	return TSMS_FAIL;
#endif
}

TSMS_RESULT W25Q256_eraseAll(struct W25Q256_Handler * handler) {
#ifdef TSMS_STM32_FLASH
	TSMS_RESULT result = W25Q256_enableWrite(handler);
	result |= W25Q256_waitBusy(handler);
	result |= W25Q256_writeCommand(handler, W25Q256_COMMAND_CHIP_ERASE, 0, 0, QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_NONE, QSPI_ADDRESS_8_BITS, QSPI_DATA_NONE);
	return result | W25Q256_waitBusy(handler);
#else
	return TSMS_FAIL;
#endif
}
