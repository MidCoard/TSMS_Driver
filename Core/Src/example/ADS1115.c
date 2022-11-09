#include "ADS1115.h"

static uint8_t ADS1115_BUFFER[3];

uint16_t ADS1115_readRegister(struct ADS1115_Handler *handler, uint8_t reg) {
	uint16_t data;
	TSMS_REG_writeRegisterByList(handler->handler->regs, ADS1115_P, reg - 1);
	ADS1115_BUFFER[0] = TSMS_REG_getRegisterByList(handler->handler->regs, ADS1115_REG_ADDRESS_POINTER);
	TSMS_IIC_writeBytes(handler->handler->iic, ADS1115_BUFFER, 1);
	TSMS_IIC_readBytes(handler->handler->iic, ADS1115_BUFFER + 1, 2);
	data = ADS1115_BUFFER[1] << 8 | ADS1115_BUFFER[2];
	return data;
}

static void ADS1115_writeRegister(struct ADS1115_Handler *handler, uint8_t reg, uint16_t data) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, ADS1115_P, reg - 1);
	ADS1115_BUFFER[0] = TSMS_REG_getRegisterByList(handler->handler->regs, ADS1115_REG_ADDRESS_POINTER);
	ADS1115_BUFFER[1] = data >> 8;
	ADS1115_BUFFER[2] = data & 0xFF;
	TSMS_IIC_writeBytes(handler->handler->iic, ADS1115_BUFFER, 3);
}

struct ADS1115_Handler *
ADS1115_initHardware(GPIO_TypeDef *sda, uint16_t sdaPin, GPIO_TypeDef *scl, uint16_t sclPin, uint8_t address) {
	struct ADS1115_Handler *handler = (struct ADS1115_Handler *) malloc(sizeof(struct ADS1115_Handler));
	handler->handler = TSMS_DRIVER_createIICHandler(TSMS_IIC_createSoftwareIIC(
			TSMS_GPIO_createHandler(sda, sdaPin),
			TSMS_GPIO_createHandler(scl, sclPin),
			address,
			TSMS_TRANSFER_MSB
	), TSMS_REG_createList(5,
	                       TSMS_REG_8BitRegister(ADS1115_P, ADS1115_P, ADS1115_P, ADS1115_P, ADS1115_RESERVE,
	                                             ADS1115_RESERVE, ADS1115_RESERVE, ADS1115_RESERVE),
	                       TSMS_REG_16BitRegister(ADS1115_D, ADS1115_D, ADS1115_D, ADS1115_D, ADS1115_D, ADS1115_D,
	                                              ADS1115_D, ADS1115_D, ADS1115_D, ADS1115_D, ADS1115_D, ADS1115_D,
	                                              ADS1115_D, ADS1115_D, ADS1115_D, ADS1115_D),
	                       TSMS_REG_16BitRegister(ADS1115_COMP_QUE, ADS1115_COMP_QUE, ADS1115_COMP_LAT,
	                                              ADS1115_COMP_POL, ADS1115_COMP_MODE, ADS1115_DR, ADS1115_DR,
	                                              ADS1115_DR, ADS1115_M, ADS1115_PGA, ADS1115_PGA, ADS1115_PGA,
	                                              ADS1115_MUX, ADS1115_MUX, ADS1115_MUX, ADS1115_OS),
	                       TSMS_REG_16BitRegister(ADS1115_LO_THRESH, ADS1115_LO_THRESH, ADS1115_LO_THRESH,
	                                              ADS1115_LO_THRESH, ADS1115_LO_THRESH, ADS1115_LO_THRESH,
	                                              ADS1115_LO_THRESH, ADS1115_LO_THRESH, ADS1115_LO_THRESH,
	                                              ADS1115_LO_THRESH, ADS1115_LO_THRESH, ADS1115_LO_THRESH,
	                                              ADS1115_LO_THRESH, ADS1115_LO_THRESH, ADS1115_LO_THRESH,
	                                              ADS1115_LO_THRESH),
	                       TSMS_REG_16BitRegister(ADS1115_HI_THRESH, ADS1115_HI_THRESH, ADS1115_HI_THRESH,
	                                              ADS1115_HI_THRESH, ADS1115_HI_THRESH, ADS1115_HI_THRESH,
	                                              ADS1115_HI_THRESH, ADS1115_HI_THRESH, ADS1115_HI_THRESH,
	                                              ADS1115_HI_THRESH, ADS1115_HI_THRESH, ADS1115_HI_THRESH,
	                                              ADS1115_HI_THRESH, ADS1115_HI_THRESH, ADS1115_HI_THRESH,
	                                              ADS1115_HI_THRESH)
	));
	TSMS_REG_setRegister(handler->handler->regs->regs[ADS1115_REG_CONFIG], 0x8583);
	TSMS_REG_setRegister(handler->handler->regs->regs[ADS1115_REG_LO_THRESH], 0x8000);
	TSMS_REG_setRegister(handler->handler->regs->regs[ADS1115_REG_HI_THRESH], 0x7FFF);
	handler->gain = 2.048 / 32768;
	return handler;
}

float ADS1115_readData(struct ADS1115_Handler *handler) {
	uint16_t data = ADS1115_readRegister(handler, ADS1115_REG_CONVERSION);
	TSMS_REG_setRegisterByList(handler->handler->regs, ADS1115_REG_CONVERSION, data);
	return ((int16_t)data) * handler->gain;
}

void ADS1115_setDataRate(struct ADS1115_Handler *handler, ADS1115_DATA_RATE dataRate) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, ADS1115_DR, dataRate);
	ADS1115_writeRegister(handler, ADS1115_REG_CONFIG,
	                      TSMS_REG_getRegisterByList(handler->handler->regs, ADS1115_REG_CONFIG));
}

void ADS1115_setGain(struct ADS1115_Handler *handler, ADS1115_GAIN gain) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, ADS1115_PGA, gain);
	ADS1115_writeRegister(handler, ADS1115_REG_CONFIG,
	                      TSMS_REG_getRegisterByList(handler->handler->regs, ADS1115_REG_CONFIG));
	switch (gain) {
		case ADS1115_6_144V:
			handler->gain = 6.144 / 32768;
			break;
		case ADS1115_4_096V:
			handler->gain = 4.096 / 32768;
			break;
		case ADS1115_2_048V:
			handler->gain = 2.048 / 32768;
			break;
		case ADS1115_1_024V:
			handler->gain = 1.024 / 32768;
			break;
		case ADS1115_0_512V:
			handler->gain = 0.512 / 32768;
			break;
		case ADS1115_0_256V:
			handler->gain = 0.256 / 32768;
			break;
	}
}

void ADS1115_setMode(struct ADS1115_Handler *handler, ADS1115_MODE mode) {
	TSMS_RESULT result = TSMS_REG_writeRegisterByList(handler->handler->regs, ADS1115_M, mode);
	ADS1115_writeRegister(handler, ADS1115_REG_CONFIG,
	                      TSMS_REG_getRegisterByList(handler->handler->regs, ADS1115_REG_CONFIG));
}

void ADS1115_setComparatorMode(struct ADS1115_Handler *handler, ADS1115_COMPARATOR_MODE mode) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, ADS1115_COMP_MODE, mode);
	ADS1115_writeRegister(handler, ADS1115_REG_CONFIG,
	                      TSMS_REG_getRegisterByList(handler->handler->regs, ADS1115_REG_CONFIG));
}

void ADS1115_setComparatorPolarity(struct ADS1115_Handler *handler, TSMS_GPIO_STATUS polarity) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, ADS1115_COMP_POL, polarity);
	ADS1115_writeRegister(handler, ADS1115_REG_CONFIG,
	                      TSMS_REG_getRegisterByList(handler->handler->regs, ADS1115_REG_CONFIG));
}

void ADS1115_enableComparatorLatch(struct ADS1115_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, ADS1115_COMP_LAT, 1);
	ADS1115_writeRegister(handler, ADS1115_REG_CONFIG,
	                      TSMS_REG_getRegisterByList(handler->handler->regs, ADS1115_REG_CONFIG));
}

void ADS1115_disableComparatorLatch(struct ADS1115_Handler *handler) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, ADS1115_COMP_LAT, 0);
	ADS1115_writeRegister(handler, ADS1115_REG_CONFIG,
	                      TSMS_REG_getRegisterByList(handler->handler->regs, ADS1115_REG_CONFIG));
}

void ADS1115_setComparatorQueueMode(struct ADS1115_Handler *handler, ADS1115_COMPARATOR_QUEUE_MODE queue) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, ADS1115_COMP_QUE, queue);
	ADS1115_writeRegister(handler, ADS1115_REG_CONFIG,
	                      TSMS_REG_getRegisterByList(handler->handler->regs, ADS1115_REG_CONFIG));
}

void ADS1115_setLowThreshold(struct ADS1115_Handler *handler, float threshold) {
	TSMS_REG_setRegisterByList(handler->handler->regs, ADS1115_REG_LO_THRESH, (uint16_t)(threshold / handler->gain));
	ADS1115_writeRegister(handler, ADS1115_REG_LO_THRESH, TSMS_REG_getRegisterByList(handler->handler->regs, ADS1115_REG_LO_THRESH));
}

void ADS1115_setHighThreshold(struct ADS1115_Handler *handler, float threshold) {
	TSMS_REG_setRegisterByList(handler->handler->regs, ADS1115_REG_HI_THRESH, (uint16_t)(threshold / handler->gain));
	ADS1115_writeRegister(handler, ADS1115_REG_HI_THRESH,  TSMS_REG_getRegisterByList(handler->handler->regs, ADS1115_REG_HI_THRESH));
}

void ADS1115_setChannel(struct ADS1115_Handler *handler, ADS1115_CHANNEL_MUX channel) {
	TSMS_REG_writeRegisterByList(handler->handler->regs, ADS1115_MUX, channel);
	ADS1115_writeRegister(handler, ADS1115_REG_CONFIG,
	                      TSMS_REG_getRegisterByList(handler->handler->regs, ADS1115_REG_CONFIG));
}

void ADS1115_startConversion(struct ADS1115_Handler *handler) {
	ADS1115_writeRegister(handler, ADS1115_REG_CONFIG,
	                      TSMS_REG_tempWriteRegisterByList(handler->handler->regs, ADS1115_OS, 1));
}

bool ADS1115_isInConversion(struct ADS1115_Handler *handler) {
	return !(ADS1115_readRegister(handler, ADS1115_REG_CONFIG) & 0x8000);
}



