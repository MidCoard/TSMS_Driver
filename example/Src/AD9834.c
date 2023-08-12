#include "AD9834.h"

static void AD9834_writeRegister(struct AD9834_Handler *handler, uint8_t reg, uint16_t value) {
	uint16_t data = (reg << 14) | value;
	TSMS_GPIO_write(handler->fsync, TSMS_GPIO_STATUS_LOW);
	handler->handler->spiWrite(handler->handler->spi, &data, 16, 1);
	TSMS_GPIO_write(handler->fsync, TSMS_GPIO_STATUS_HIGH);
}

static void AD9834_writeControlRegister(struct AD9834_Handler *handler, uint16_t data) {
	AD9834_writeRegister(handler, AD9834_REG_CONTROL, data);
}

static void AD9834_frequencySelect(struct AD9834_Handler *handler, uint8_t status) {
	if (handler->pinSwitch)
		TSMS_GPIO_write(handler->fs, status);
	else {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_FSEL, status);
		AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
	}
}

static void AD9834_phaseSelect(struct AD9834_Handler *handler, uint8_t status) {
	if (handler->pinSwitch)
		TSMS_GPIO_write(handler->ps, status);
	else if (status) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_PSEL, status);
		AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
	}
}

static void AD9834_reset(struct AD9834_Handler *handler, TSMS_GPIO_STATUS status) {
	if (handler->pinSwitch)
		TSMS_GPIO_write(handler->reset, status);
	else if (status)
		AD9834_writeControlRegister(handler, TSMS_REG_tempWriteRegisterByList(handler->handler->regs, AD9834_RESET, 1));
	else
		AD9834_writeControlRegister(handler, TSMS_REG_tempWriteRegisterByList(handler->handler->regs, AD9834_RESET, 0));
}

struct AD9834_Handler *
AD9834_initHardware(TSMS_GHP sclk, TSMS_GHP sda, TSMS_GHP fsync, TSMS_GHP fs, TSMS_GHP ps, TSMS_GHP reset,
                    TSMS_GHP sleep, float freq, bool pinSwitch) {
	struct AD9834_Handler *handler = TSMS_malloc(sizeof(struct AD9834_Handler));
	handler->handler = TSMS_DRIVER_createSPIHandler(
			TSMS_SPI_createSoftwareHandler(TSMS_NULL_GHP, sclk, TSMS_NULL_GHP, sda, TSMS_SPI_MODE_2, false,
			                               TSMS_TRANSFER_TYPE_MSB),
			TSMS_REG_createList(5, TSMS_REG_16BitRegister(AD9834_REG_CONTROL, AD9834_RESERVE, AD9834_MD, AD9834_RESERVE,
			                                              AD9834_DIV2, AD9834_SIGN_PIB, AD9834_OPBITEN, AD9834_SLEEP12, AD9834_SLEEP1, AD9834_RESET, AD9834_PIN_SW, AD9834_PSEL,
			                                              AD9834_FSEL, AD9834_HLB, AD9834_B28, AD9834_RESERVE, AD9834_RESERVE
			),
			                    TSMS_REG_16BitRegister(AD9834_REG_FREQ_0, AD9834_FREQ0, AD9834_FREQ0, AD9834_FREQ0, AD9834_FREQ0, AD9834_FREQ0, AD9834_FREQ0, AD9834_FREQ0, AD9834_FREQ0, AD9834_FREQ0, AD9834_FREQ0, AD9834_FREQ0, AD9834_FREQ0, AD9834_FREQ0, AD9834_FREQ0, AD9834_RESERVE, AD9834_RESERVE),
			                    TSMS_REG_16BitRegister(AD9834_REG_FREQ_1, AD9834_FREQ1, AD9834_FREQ1, AD9834_FREQ1, AD9834_FREQ1, AD9834_FREQ1, AD9834_FREQ1, AD9834_FREQ1, AD9834_FREQ1, AD9834_FREQ1, AD9834_FREQ1, AD9834_FREQ1, AD9834_FREQ1, AD9834_FREQ1, AD9834_FREQ1, AD9834_RESERVE, AD9834_RESERVE),
			                    TSMS_REG_16BitRegister(AD9834_REG_PHASE_0, AD9834_PHASE0, AD9834_PHASE0, AD9834_PHASE0, AD9834_PHASE0, AD9834_PHASE0, AD9834_PHASE0, AD9834_PHASE0, AD9834_PHASE0, AD9834_PHASE0, AD9834_PHASE0, AD9834_PHASE0, AD9834_PHASE0, AD9834_RESERVE, AD9834_RESERVE, AD9834_RESERVE, AD9834_RESERVE),
			                    TSMS_REG_16BitRegister(AD9834_REG_PHASE_1, AD9834_PHASE1, AD9834_PHASE1, AD9834_PHASE1, AD9834_PHASE1, AD9834_PHASE1, AD9834_PHASE1, AD9834_PHASE1, AD9834_PHASE1, AD9834_PHASE1, AD9834_PHASE1, AD9834_PHASE1, AD9834_PHASE1, AD9834_RESERVE, AD9834_RESERVE, AD9834_RESERVE, AD9834_RESERVE)
			                    ));

	TSMS_REG_setRegisterByList(handler->handler->regs, AD9834_REG_PHASE_0, 0);
	TSMS_REG_setRegisterByList(handler->handler->regs, AD9834_REG_PHASE_1, 0x2000);
	handler->pinSwitch = pinSwitch;
	handler->fsync = fsync;
	handler->fs = fs;
	handler->ps = ps;
	handler->reset = reset;
	handler->sleep = sleep;
	handler->freq = freq;
	AD9834_reset(handler, TSMS_GPIO_STATUS_HIGH);
	TSMS_delay(1);
	AD9834_reset(handler, TSMS_GPIO_STATUS_LOW);
	TSMS_GPIO_write(handler->fsync, TSMS_GPIO_STATUS_HIGH);
	if (pinSwitch) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_PIN_SW, 1);
		AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
	}
	return handler;
}

void AD9834_setFrequency0(struct AD9834_Handler* handler, float frequency) {
	uint32_t value;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD9834_B28, &value);
	if (!value) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_B28, 1);
		AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
	}
	uint32_t freq = frequency / handler->freq * AD9834_FREQ_FACTOR;
	uint16_t lsb = freq & AD9834_FREQ_LSB_MASK;
	uint16_t msb = freq >> 14;
	AD9834_writeRegister(handler, AD9834_REG_FREQ_0, lsb);
	AD9834_writeRegister(handler, AD9834_REG_FREQ_0, msb);
}

void AD9834_setFrequency1(struct AD9834_Handler* handler, float frequency) {
	uint32_t value;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD9834_B28, &value);
	if (!value) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_B28, 1);
		AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
	}
	uint32_t freq = frequency / handler->freq * AD9834_FREQ_FACTOR;
	uint16_t lsb = freq & AD9834_FREQ_LSB_MASK;
	uint16_t msb = freq >> 14;
	AD9834_writeRegister(handler, AD9834_REG_FREQ_1, lsb);
	AD9834_writeRegister(handler, AD9834_REG_FREQ_1, msb);
}

void AD9834_setFrequency(struct AD9834_Handler* handler, float frequency1, float frequency2) {
	uint32_t value;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD9834_B28, &value);
	if (!value) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_B28, 1);
		AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
	}
	uint32_t freq1 = frequency1 / handler->freq * AD9834_FREQ_FACTOR;
	uint16_t lsb1 = freq1 & AD9834_FREQ_LSB_MASK;
	uint16_t msb1 = freq1 >> 14;
	uint32_t freq2 = frequency2 / handler->freq * AD9834_FREQ_FACTOR;
	uint16_t lsb2 = freq2 & AD9834_FREQ_LSB_MASK;
	uint16_t msb2 = freq2 >> 14;
	AD9834_writeRegister(handler, AD9834_REG_FREQ_0, lsb1);
	AD9834_writeRegister(handler, AD9834_REG_FREQ_0, msb1);
	AD9834_writeRegister(handler, AD9834_REG_FREQ_1, lsb2);
	AD9834_writeRegister(handler, AD9834_REG_FREQ_1, msb2);
}

void AD9834_setFrequencyLSB0(struct AD9834_Handler* handler, float frequency) {
	uint32_t value;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD9834_B28, &value);
	if (value) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_B28, 0);
		AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
	}
	TSMS_REG_readRegisterByList(handler->handler->regs, AD9834_HLB, &value);
	if (value) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_HLB, 0);
		AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
	}
	uint32_t freq = frequency / handler->freq * AD9834_FREQ_FACTOR;
	uint16_t lsb = freq & AD9834_FREQ_LSB_MASK;
	AD9834_writeRegister(handler, AD9834_REG_FREQ_0, lsb);
}

void AD9834_setFrequencyLSB1(struct AD9834_Handler* handler, float frequency) {
	uint32_t value;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD9834_B28, &value);
	if (value) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_B28, 0);
		AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
	}
	TSMS_REG_readRegisterByList(handler->handler->regs, AD9834_HLB, &value);
	if (value) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_HLB, 0);
		AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
	}
	uint32_t freq = frequency / handler->freq * AD9834_FREQ_FACTOR;
	uint16_t lsb = freq & AD9834_FREQ_LSB_MASK;
	AD9834_writeRegister(handler, AD9834_REG_FREQ_1, lsb);
}

void AD9834_setFrequencyMSB0(struct AD9834_Handler* handler, float frequency) {
	uint32_t value;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD9834_B28, &value);
	if (value) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_B28, 0);
		AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
	}
	TSMS_REG_readRegisterByList(handler->handler->regs, AD9834_HLB, &value);
	if (!value) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_HLB, 1);
		AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
	}
	uint32_t freq = frequency / handler->freq * AD9834_FREQ_FACTOR;
	uint16_t msb = freq >> 14;
	AD9834_writeRegister(handler, AD9834_REG_FREQ_0, msb);
}

void AD9834_setFrequencyMSB1(struct AD9834_Handler* handler, float frequency) {
	uint32_t value;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD9834_B28, &value);
	if (value) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_B28, 0);
		AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
	}
	TSMS_REG_readRegisterByList(handler->handler->regs, AD9834_HLB, &value);
	if (!value) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_HLB, 1);
		AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
	}
	uint32_t freq = frequency / handler->freq * AD9834_FREQ_FACTOR;
	uint16_t msb = freq >> 14;
	AD9834_writeRegister(handler, AD9834_REG_FREQ_1, msb);
}

void AD9834_setPhase0(struct AD9834_Handler* handler, float phase) {
	uint16_t phase0 = phase * AD9834_PHASE_FACTOR / 2 / PI;
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_PHASE0, phase0);
	AD9834_writeRegister(handler, AD9834_REG_PHASE_0, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_PHASE0));
}

void AD9834_setPhase1(struct AD9834_Handler* handler, float phase) {
	uint16_t phase1 = phase * AD9834_PHASE_FACTOR / 2 / PI;
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_PHASE1, phase1);
	AD9834_writeRegister(handler, AD9834_REG_PHASE_1, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_PHASE1));
}

void AD9834_setOutputMode(struct AD9834_Handler* handler, AD9834_MODE mode) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_MD, mode);
	AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
}

void AD9834_setSignOutputMode(struct AD9834_Handler* handler, AD9834_SIGN_MODE mode) {
	if (mode == AD9834_SIGN_MODE_DISABLE)
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_OPBITEN, 0);
	else if (mode == AD9834_SIGN_MODE_DAC_MSB_2) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_OPBITEN, 1);
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_MD, 0);
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_SIGN_PIB, 0);
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_DIV2, 0);
	}
	else if (mode == AD9834_SIGN_MODE_DAC_MSB) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_OPBITEN, 1);
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_MD, 0);
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_SIGN_PIB, 0);
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_DIV2, 1);
	}
	else if (mode == AD9834_SIGN_MODE_COMPARE_OUT) {
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_OPBITEN, 1);
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_MD, 0);
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_SIGN_PIB, 1);
		TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_DIV2, 1);
	}
	AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
}

void AD9834_setOutputChannel(struct AD9834_Handler *handler, AD9834_CHANNEL frequencyChannel, AD9834_CHANNEL phaseChannel) {
	if (frequencyChannel == AD9834_CHANNEL_1)
		AD9834_frequencySelect(handler, TSMS_GPIO_STATUS_HIGH);
	else AD9834_frequencySelect(handler, TSMS_GPIO_STATUS_LOW);
	if (phaseChannel == AD9834_CHANNEL_1)
		AD9834_phaseSelect(handler, TSMS_GPIO_STATUS_HIGH);
	else AD9834_phaseSelect(handler, TSMS_GPIO_STATUS_LOW);
}

TSMS_RESULT AD9834_setPowerMode(struct AD9834_Handler *handler, AD9834_POWER_MODE mode) {
	if (handler->pinSwitch) {
		if (mode == AD9834_POWER_MODE_NO_POWER_DOWN)
			TSMS_GPIO_write(handler->sleep, TSMS_GPIO_STATUS_LOW);
		else if (mode == AD9834_POWER_MODE_DAC_POWER_DOWN)
			TSMS_GPIO_write(handler->sleep, TSMS_GPIO_STATUS_HIGH);
		else return TSMS_FAIL;
	} else {
		if (mode == AD9834_POWER_MODE_NO_POWER_DOWN) {
			TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_SLEEP1, 0);
			TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_SLEEP12, 0);
		} else if (mode == AD9834_POWER_MODE_DAC_POWER_DOWN) {
			TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_SLEEP1, 0);
			TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_SLEEP12, 1);
		} else if (mode == AD9834_POWER_MODE_INTERNAL_CLOCK_POWER_DOWN) {
			TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_SLEEP1, 1);
			TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_SLEEP12, 0);
		} else if (mode == AD9834_POWER_MODE_BOTH_POWER_DOWN) {
			TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_SLEEP1, 1);
			TSMS_REG_writeRegisterByList(handler->handler->regs, AD9834_SLEEP12, 1);
		}
		AD9834_writeControlRegister(handler, TSMS_REG_getRegisterByList(handler->handler->regs, AD9834_REG_CONTROL));
	}
	return TSMS_SUCCESS;
}