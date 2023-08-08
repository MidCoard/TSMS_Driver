#include "ad7190.h"
#include "tsms_driver.h"
#include "tsms_spi.h"

static uint32_t AD7190_TRANSFORM_DATA[2];

static uint32_t AD7190_readRegister(struct AD7190_Handler *handler, AD7190_REGISTER reg) {
	// todo reg could be TSMS_REG
	uint32_t command = TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_COMM) | AD7190_CMD_READ |
	                   AD7190_REG_ADDRESS(reg - 1);
	AD7190_TRANSFORM_DATA[0] = command;
	uint8_t bits = 24;
	if (reg == AD7190_REG_STATUS || reg == AD7190_REG_ID || reg == AD7190_REG_GPOCON)
		// todo can be auto detect
		bits = 8;
	uint32_t value = 0;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD7190_DAT_STA, &value);
	if (value && reg == AD7190_REG_DATA)
		bits = 32;
	handler->handler->spiTransform(handler->handler->spi, AD7190_TRANSFORM_DATA, 8, bits, 1, 1);
	uint32_t data = AD7190_TRANSFORM_DATA[1];
	if (value && reg == AD7190_REG_DATA) {
		TSMS_REG_setRegisterByList(handler->handler->regs, AD7190_REG_STATUS, data & 0xFF);
		TSMS_REG_setRegisterByList(handler->handler->regs, reg, data >> 8);
	} else TSMS_REG_setRegisterByList(handler->handler->regs, reg, data);
	return data;
}

static void AD7190_writeRegister(struct AD7190_Handler *handler, AD7190_REGISTER reg, uint32_t value) {
	uint32_t command = TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_COMM) | AD7190_CMD_WRITE |
	                   AD7190_REG_ADDRESS(reg == AD7190_REG_COMM ? AD7190_REG_COMM : reg - 1);
	handler->handler->spiWrite(handler->handler->spi, &command, 8, 1);
	uint8_t bits = 24;
	if (reg == AD7190_REG_COMM || reg == AD7190_REG_GPOCON)
		bits = 8;
	handler->handler->spiWrite(handler->handler->spi, &value, bits, 1);
}

struct AD7190_Handler *
AD7190_initSoftware(TSMS_GHP sclk, TSMS_GHP mosi, TSMS_GHP miso,
                   TSMS_GHP cs, float reference) {
	struct AD7190_Handler *handler = TSMS_malloc(sizeof(struct AD7190_Handler));
	handler->handler = TSMS_DRIVER_createSPIHandler(TSMS_SPI_createSoftwareHandler(cs,
	                                                                               sclk,
	                                                                               miso,
	                                                                               mosi,
	                                                                               TSMS_SPI_MODE_3, false,
	                                                                               TSMS_TRANSFER_TYPE_MSB),
	                                                TSMS_REG_createList(9,
	                                                                    TSMS_REG_8BitRegister(AD7190_REG_COMM, AD7190_RESERVE,
	                                                                                          AD7190_RESERVE,
	                                                                                          AD7190_CREAD, AD7190_RS,
	                                                                                          AD7190_RS, AD7190_RS,
	                                                                                          AD7190_RW, AD7190_WEN),
	                                                                    TSMS_REG_8BitRegister(AD7190_REG_STATUS,AD7190_CHD, AD7190_CHD,
	                                                                                          AD7190_CHD,
	                                                                                          AD7190_RESERVE,
	                                                                                          AD7190_PARITY,
	                                                                                          AD7190_NOREF, AD7190_ERR,
	                                                                                          AD7190_RDY),
	                                                                    TSMS_REG_24BitRegister(AD7190_REG_MODE,AD7190_FS, AD7190_FS,
	                                                                                           AD7190_FS, AD7190_FS,
	                                                                                           AD7190_FS, AD7190_FS,
	                                                                                           AD7190_FS, AD7190_FS,
	                                                                                           AD7190_FS, AD7190_FS,
	                                                                                           AD7190_REJ60,
	                                                                                           AD7190_SINGLE,
	                                                                                           AD7190_RESERVE,
	                                                                                           AD7190_ENPAR,
	                                                                                           AD7190_RESERVE,
	                                                                                           AD7190_SINC3,
	                                                                                           AD7190_RESERVE,
	                                                                                           AD7190_RESERVE,
	                                                                                           AD7190_CLK, AD7190_CLK,
	                                                                                           AD7190_DAT_STA,
	                                                                                           AD7190_MD, AD7190_MD,
	                                                                                           AD7190_MD),
	                                                                    TSMS_REG_24BitRegister(AD7190_REG_CONF,AD7190_G, AD7190_G,
	                                                                                           AD7190_G, AD7190_UB,
	                                                                                           AD7190_BUF,
	                                                                                           AD7190_RESERVE,
	                                                                                           AD7190_REFDET,
	                                                                                           AD7190_BURN, AD7190_CH,
	                                                                                           AD7190_CH, AD7190_CH,
	                                                                                           AD7190_CH, AD7190_CH,
	                                                                                           AD7190_CH, AD7190_CH,
	                                                                                           AD7190_CH,
	                                                                                           AD7190_RESERVE,
	                                                                                           AD7190_RESERVE,
	                                                                                           AD7190_RESERVE,
	                                                                                           AD7190_RESERVE,
	                                                                                           AD7190_REFSEL,
	                                                                                           AD7190_RESERVE,
	                                                                                           AD7190_RESERVE,
	                                                                                           AD7190_CHOP),
	                                                                    TSMS_REG_24BitRegister(AD7190_REG_DATA,AD7190_DATA, AD7190_DATA,
	                                                                                           AD7190_DATA, AD7190_DATA,
	                                                                                           AD7190_DATA, AD7190_DATA,
	                                                                                           AD7190_DATA, AD7190_DATA,
	                                                                                           AD7190_DATA, AD7190_DATA,
	                                                                                           AD7190_DATA, AD7190_DATA,
	                                                                                           AD7190_DATA, AD7190_DATA,
	                                                                                           AD7190_DATA, AD7190_DATA,
	                                                                                           AD7190_DATA, AD7190_DATA,
	                                                                                           AD7190_DATA, AD7190_DATA,
	                                                                                           AD7190_DATA, AD7190_DATA,
	                                                                                           AD7190_DATA,
	                                                                                           AD7190_DATA),
	                                                                    TSMS_REG_8BitRegister(AD7190_REG_ID, AD7190_ID, AD7190_ID,
	                                                                                          AD7190_ID, AD7190_ID,
	                                                                                          AD7190_ID, AD7190_ID,
	                                                                                          AD7190_ID, AD7190_ID),
	                                                                    TSMS_REG_8BitRegister(AD7190_REG_GPOCON, AD7190_P0DAT,
	                                                                                          AD7190_P1DAT,
	                                                                                          AD7190_P2DAT,
	                                                                                          AD7190_P3DAT,
	                                                                                          AD7190_GP10EN,
	                                                                                          AD7190_GP32EN,
	                                                                                          AD7190_BPDSW,
	                                                                                          AD7190_RESERVE),
	                                                                    TSMS_REG_24BitRegister(AD7190_REG_OFFSET, AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET,
	                                                                                           AD7190_OFFSET),
	                                                                    TSMS_REG_24BitRegister(AD7190_REG_FULL_SCALE, AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE,
	                                                                                           AD7190_FULL_SCALE)
	                                                ));
	handler->reference = reference;
	TSMS_REG_setRegisterByList(handler->handler->regs, AD7190_REG_COMM, 0);
	TSMS_REG_setRegisterByList(handler->handler->regs, AD7190_REG_STATUS, 0x80);
	TSMS_REG_setRegisterByList(handler->handler->regs, AD7190_REG_MODE, 0x080060);
	TSMS_REG_setRegisterByList(handler->handler->regs, AD7190_REG_CONF, 0x000117);
	TSMS_REG_setRegisterByList(handler->handler->regs, AD7190_REG_DATA, 0);
	TSMS_REG_setRegisterByList(handler->handler->regs, AD7190_REG_GPOCON, 0x0);
	TSMS_REG_setRegisterByList(handler->handler->regs, AD7190_REG_OFFSET, 0x800000);
	AD7190_reset(handler);
	uint32_t reg;
	AD7190_readRegister(handler, AD7190_REG_ID);
	reg = TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_ID);
	if (reg != 0x84)
		return TSMS_NULL;
	AD7190_readRegister(handler, AD7190_REG_FULL_SCALE);
	for (int i = 0; i < 8; i++)
		handler->data[i] = 0;
	return handler;
}

void AD7190_enableContinuousMode(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_CREAD, 1);
	AD7190_writeRegister(handler, AD7190_REG_COMM, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_COMM) |
	                                               AD7190_REG_ADDRESS(AD7190_REG_DATA));

}

void AD7190_disableContinuousMode(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_CREAD, 0);
	AD7190_writeRegister(handler, AD7190_REG_COMM, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_COMM) |
	                                               AD7190_REG_ADDRESS(AD7190_REG_DATA));
}

AD7190_CURRENT_CHANNEL AD7190_getCurrentChannel(struct AD7190_Handler *handler) {
	AD7190_readRegister(handler, AD7190_REG_STATUS);
	uint32_t channel = 0;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD7190_CH, &channel);
	return channel;
}

bool AD7190_isDataReady(struct AD7190_Handler *handler) {
	AD7190_readRegister(handler, AD7190_REG_STATUS);
	uint32_t ready = 0;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD7190_RDY, &ready);
	return ready;
}

bool AD7190_isError(struct AD7190_Handler *handler) {
	AD7190_readRegister(handler, AD7190_REG_STATUS);
	uint32_t error = 0;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD7190_ERR, &error);
	return error;
}

bool AD7190_isReferenceDetected(struct AD7190_Handler *handler) {
	AD7190_readRegister(handler, AD7190_REG_STATUS);
	uint32_t refDet = 0;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD7190_NOREF, &refDet);
	return !refDet;
}

bool AD7190_isParityError(struct AD7190_Handler *handler) {
	AD7190_readRegister(handler, AD7190_REG_STATUS);
	uint32_t parity = 0;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD7190_PARITY, &parity);
	return parity;
}

void AD7190_setFilterDivider(struct AD7190_Handler *handler, uint16_t divider) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_FS, divider);
	AD7190_writeRegister(handler, AD7190_REG_MODE, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_MODE));
}

void AD7190_enable60HzRejection(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_REJ60, 1);
	AD7190_writeRegister(handler, AD7190_REG_MODE, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_MODE));
}

void AD7190_disable60HzRejection(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_REJ60, 0);
	AD7190_writeRegister(handler, AD7190_REG_MODE, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_MODE));
}

void AD7190_enableSingleCycleConversion(struct AD7190_Handler *handler) {
	AD7190_writeRegister(handler, AD7190_REG_MODE, TSMS_REG_tempWriteRegisterByList(handler->handler->regs, AD7190_SINGLE, 1));
}

void AD7190_disableSingleCycleConversion(struct AD7190_Handler *handler) {
	AD7190_writeRegister(handler, AD7190_REG_MODE, TSMS_REG_tempWriteRegisterByList(handler->handler->regs, AD7190_SINGLE, 0));
}

void AD7190_enableParityCheck(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_ENPAR, 1);
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_DAT_STA, 1);
	AD7190_writeRegister(handler, AD7190_REG_MODE, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_MODE));
}

void AD7190_disableParityCheck(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_ENPAR, 0);
	AD7190_writeRegister(handler, AD7190_REG_MODE, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_MODE));
}

// also disable sin4 filter
void AD7190_enableSin3Filter(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_SINC3, 1);
	AD7190_writeRegister(handler, AD7190_REG_MODE, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_MODE));
}

// also enable sin4 filter
void AD7190_disableSin3Filter(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_SINC3, 0);
	AD7190_writeRegister(handler, AD7190_REG_MODE, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_MODE));
}

void AD7190_setClockMode(struct AD7190_Handler *handler, AD7190_CLOCK_MODE mode) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_CLK, mode);
	AD7190_writeRegister(handler, AD7190_REG_MODE, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_MODE));
}

// data status read means that the status register content is after the data register content
void AD7190_enableDataStatusRead(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_DAT_STA, 1);
	AD7190_writeRegister(handler, AD7190_REG_MODE, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_MODE));
}

void AD7190_disableDataStatusRead(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_DAT_STA, 0);
	AD7190_writeRegister(handler, AD7190_REG_MODE, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_MODE));
}

void AD7190_setMode(struct AD7190_Handler *handler, AD7190_MODE mode) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_MD, mode);
	AD7190_writeRegister(handler, AD7190_REG_MODE, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_MODE));
}

void AD7190_setGain(struct AD7190_Handler *handler, AD7190_GAIN gain) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_G, gain);
	handler->gain = 1 << gain;
	AD7190_writeRegister(handler, AD7190_REG_CONF, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_CONF));
}

void AD7190_enableBipolar(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_UB, 0);
	AD7190_writeRegister(handler, AD7190_REG_CONF, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_CONF));
}

void AD7190_enableUnipolar(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_UB, 1);
	AD7190_writeRegister(handler, AD7190_REG_CONF, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_CONF));
}

void AD7190_enableBuffer(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_BUF, 1);
	AD7190_writeRegister(handler, AD7190_REG_CONF, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_CONF));
}

void AD7190_disableBuffer(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_BUF, 0);
	AD7190_writeRegister(handler, AD7190_REG_CONF, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_CONF));
}

void AD7190_enableReferenceDetected(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_REFDET, 1);
	AD7190_writeRegister(handler, AD7190_REG_CONF, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_CONF));
}

void AD7190_disableReferenceDetected(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_REFDET, 0);
	AD7190_writeRegister(handler, AD7190_REG_CONF, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_CONF));
}

void AD7190_enableBurnoutCurrent(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_BURN, 1);
	AD7190_writeRegister(handler, AD7190_REG_CONF, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_CONF));
}

void AD7190_disableBurnoutCurrent(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_BURN, 0);
	AD7190_writeRegister(handler, AD7190_REG_CONF, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_CONF));
}

void AD7190_setChannels(struct AD7190_Handler *handler, AD7190_CHANNELS channel) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_CH, channel);
	AD7190_writeRegister(handler, AD7190_REG_CONF, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_CONF));
}

bool AD7190_setReference(struct AD7190_Handler *handler, AD7190_REFERENCE reference) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_REFSEL, reference);
	AD7190_writeRegister(handler, AD7190_REG_CONF, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_CONF));

	AD7190_enableReferenceDetected(handler);
	TSMS_delay(100);
	AD7190_readRegister(handler, AD7190_REG_STATUS);
	bool ret = false;
	if (AD7190_isReferenceDetected(handler))
		ret = true;
	AD7190_disableReferenceDetected(handler);
	return ret;
}

void AD7190_enableChop(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_CHOP, 1);
	AD7190_writeRegister(handler, AD7190_REG_CONF, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_CONF));
}

void AD7190_disableChop(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_CHOP, 0);
	AD7190_writeRegister(handler, AD7190_REG_CONF, TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_CONF));
}

void AD7190_openBridgePowerDownSwitch(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_BPDSW, 0);
	AD7190_writeRegister(handler, AD7190_REG_CONF,
	                     TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_GPOCON));
}

void AD7190_closeBridgePowerDownSwitch(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_BPDSW, 1);
	AD7190_writeRegister(handler, AD7190_REG_CONF,
	                     TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_GPOCON));
}

void AD7190_enableGPIO01(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_GP10EN, 1);
	AD7190_writeRegister(handler, AD7190_REG_GPOCON,
	                     TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_GPOCON));
}

void AD7190_disableGPIO01(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_GP10EN, 0);
	AD7190_writeRegister(handler, AD7190_REG_GPOCON,
	                     TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_GPOCON));
}

void AD7190_enableGPIO23(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_GP32EN, 1);
	AD7190_writeRegister(handler, AD7190_REG_GPOCON,
	                     TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_GPOCON));
}

void AD7190_disableGPIO23(struct AD7190_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_GP32EN, 0);
	AD7190_writeRegister(handler, AD7190_REG_GPOCON,
	                     TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_GPOCON));
}

void AD7190_writeGPIO(struct AD7190_Handler *handler, AD7190_GPIO gpio, AD7190_GPIO_STATUS state) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, AD7190_P0DAT + gpio, state);
	AD7190_writeRegister(handler, AD7190_REG_GPOCON,
	                     TSMS_REG_getRegisterByList(handler->handler->regs, AD7190_REG_GPOCON));
}

float AD7190_convertData(struct AD7190_Handler *handler, uint32_t value) {
	uint32_t data;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD7190_UB, &data);
	if (data) {
		float ref = handler->reference / handler->gain;
		return value * ref / AD7190_UNIPOLAR_MASK;
		//unsigned
	} else {
		float ref = handler->reference / handler->gain;
		return (value / (float) AD7190_BIPOLAR_MASK - 1) * ref;
		//signed
	}
}

float AD7190_convertTemperature(struct AD7190_Handler *handler, uint32_t value) {
	uint32_t data;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD7190_UB, &data);
	if (data)
		return value / 2815.0 / 2 - 273;
	else
		return (value - AD7190_BIPOLAR_MASK) / 2815.0 - 273;
}

float *AD7190_readData(struct AD7190_Handler *handler) {
	uint32_t channels;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD7190_CH, &channels);
	while (channels) {
		AD7190_readRegister(handler, AD7190_REG_DATA);
		uint32_t value;
		TSMS_REG_readRegisterByList(handler->handler->regs, AD7190_DAT_STA, &value);
		if (value) {
			TSMS_REG_readRegisterByList(handler->handler->regs, AD7190_CHD, &value);
			channels &= ~(1 << value);
			if (value != AD7190_CURRENT_CHANNEL_TEMP)
				handler->data[value] = AD7190_convertData(handler,
				                                          TSMS_REG_getRegisterByList(handler->handler->regs,
				                                                                     AD7190_REG_DATA));
			else
				handler->data[AD7190_CURRENT_CHANNEL_TEMP] = AD7190_convertTemperature(handler,
				                                                                       TSMS_REG_getRegisterByList(
						                                                                       handler->handler->regs,
						                                                                       AD7190_REG_DATA));
		} else {
			// not recommended
			AD7190_readRegister(handler, AD7190_REG_STATUS);
			TSMS_REG_readRegisterByList(handler->handler->regs, AD7190_CHD, &value);
			channels &= ~(1 << value);
			if (value != AD7190_CURRENT_CHANNEL_TEMP)
				handler->data[value] = AD7190_convertData(handler,
				                                          TSMS_REG_getRegisterByList(handler->handler->regs,
				                                                                     AD7190_REG_DATA));
			else
				handler->data[AD7190_CURRENT_CHANNEL_TEMP] = AD7190_convertTemperature(handler,
				                                                                       TSMS_REG_getRegisterByList(
						                                                                       handler->handler->regs,
						                                                                       AD7190_REG_DATA));
		}
	}
	return handler->data;
}

static uint32_t AD7190_RESET_DATA[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void AD7190_reset(struct AD7190_Handler *handler) {
	handler->handler->spiWrite(handler->handler->spi, AD7190_RESET_DATA, 8, 5);
	TSMS_delay(1);
}

AD7190_GPIO_STATUS AD7190_readGPIO(struct AD7190_Handler *handler, AD7190_GPIO gpio) {
	AD7190_readRegister(handler, AD7190_REG_GPOCON);
	uint32_t value;
	TSMS_REG_readRegisterByList(handler->handler->regs, AD7190_P0DAT + gpio, &value);
	return value;
}


