#include "AD7606.h"
#include "tsms_driver.h"
#include "tsms_iic.h"
#include "tsms_spi.h"
#include "tsms_custom.h"

static void AD7606_busyInterrupt(void *handler, TSMS_GHP gpio) {
	struct AD7606_Handler *ad7606 = handler;
	ad7606->ready = 1;
}

#ifdef TSMS_STM32_GPIO

struct AD7606_Handler *
AD7606_initSerialHardware(GPIO_TypeDef *byteSelect, uint16_t byteSelectPin, GPIO_TypeDef *db15, uint16_t db15Pin,
                          GPIO_TypeDef *standBy, uint16_t standByPin, GPIO_TypeDef *range, uint16_t rangePin,
                          GPIO_TypeDef *convstA, uint16_t convstAPin, GPIO_TypeDef *convstB, uint16_t convstBPin,
                          GPIO_TypeDef *os0, uint16_t os0Pin, GPIO_TypeDef *os1, uint16_t os1Pin, GPIO_TypeDef *os2,
                          uint16_t os2Pin, GPIO_TypeDef *referenceSelect, uint16_t referenceSelectPin,
                          GPIO_TypeDef *reset, uint16_t resetPin, GPIO_TypeDef *cs, uint16_t csPin, GPIO_TypeDef *sclk,
                          uint16_t sclkPin, GPIO_TypeDef *busy, uint16_t busyPin, GPIO_TypeDef *firstData,
                          uint16_t firstDataPin, GPIO_TypeDef *doutA, uint16_t doutAPin, GPIO_TypeDef *doutB,
                          uint16_t doutBPin, float referenceVoltage) {
	struct AD7606_Handler *handler = TSMS_malloc(sizeof(struct AD7606_Handler));
	handler->parallel = TSMS_NULL;
	handler->handler = TSMS_DRIVER_createSPIHandler(
			TSMS_SPI_createSoftwareHandler(TSMS_GPIO_createHandler(cs, csPin), TSMS_GPIO_createHandler(sclk, sclkPin),
			                               TSMS_GPIO_createHandler(doutA, doutAPin), TSMS_NULL_GHP, TSMS_SPI_MODE_3,
			                               TSMS_GPIO_STATUS_LOW, TSMS_TRANSFER_TYPE_MSB),
			TSMS_REG_createList(0));
	handler->overSample = TSMS_CUSTOM_createParallelHandler(TSMS_NULL, TSMS_TRANSFER_TYPE_LSB, 3,
	                                                        TSMS_GPIO_createHandler(os0, os0Pin),
	                                                        TSMS_GPIO_createHandler(os1, os1Pin),
	                                                        TSMS_GPIO_createHandler(os2, os2Pin));
	handler->range = TSMS_GPIO_createHandler(range, rangePin);
	handler->reset = TSMS_GPIO_createHandler(reset, resetPin);
	handler->referenceSelect = TSMS_GPIO_createHandler(referenceSelect, referenceSelectPin);
	handler->digitalInterface = TSMS_CUSTOM_createParallelHandler(TSMS_NULL, TSMS_TRANSFER_TYPE_LSB, 2,
	                                                              TSMS_GPIO_createHandler(byteSelect, byteSelectPin),
	                                                              TSMS_GPIO_createHandler(db15, db15Pin));
	handler->busy = TSMS_GPIO_createHandler(busy, busyPin);
	handler->firstData = TSMS_GPIO_createHandler(firstData, firstDataPin);
	handler->mode = TSMS_CUSTOM_createParallelHandler(TSMS_NULL, TSMS_TRANSFER_TYPE_LSB, 2,
	                                                  TSMS_GPIO_createHandler(standBy, standByPin),
	                                                  TSMS_GPIO_createHandler(range, rangePin));
	handler->convstA = TSMS_GPIO_createHandler(convstA, convstAPin);
	handler->convstB = TSMS_GPIO_createHandler(convstB, convstBPin);
	handler->referenceVoltage = referenceVoltage;
	TSMS_GPIO_write(handler->reset, TSMS_GPIO_STATUS_LOW);
	HAL_Delay(20);
	TSMS_GPIO_write(handler->reset, TSMS_GPIO_STATUS_HIGH);
	HAL_Delay(20);
	TSMS_GPIO_write(handler->reset, TSMS_GPIO_STATUS_LOW);

	TSMS_GPIO_write(handler->convstA, TSMS_GPIO_STATUS_HIGH);
	TSMS_GPIO_write(handler->convstB, TSMS_GPIO_STATUS_HIGH);

	uint32_t value = 1;

	TSMS_CUSTOM_parallelWrite(handler->digitalInterface, &value, 1);


	TSMS_IT_addGPIO(handler->busy, AD7606_busyInterrupt, handler);

	AD7606_setRange(handler, AD7606_RANGE_5V);
	AD7606_setMode(handler, AD7606_MODE_POWER_ON);

	return handler;
}
#endif

void AD7606_setOverSampleRate(struct AD7606_Handler *handler, AD7606_OS_RATE overSampleRate) {
	uint32_t value = overSampleRate;
	TSMS_CUSTOM_parallelWrite(handler->overSample, &value, 1);
}

void AD7606_setRange(struct AD7606_Handler *handler, AD7606_RANGE range) {
	TSMS_GPIO_write(handler->range, range);
	handler->voltage = range == AD7606_RANGE_5V ? 5 : 10;
}

void AD7606_setReferenceMode(struct AD7606_Handler *handler, AD7606_REFERENCE_MODE referenceMode) {
	TSMS_GPIO_write(handler->referenceSelect, referenceMode);
}

// set range after setting the mode
void AD7606_setMode(struct AD7606_Handler *handler, AD7606_MODE mode) {
	uint32_t value = mode;
	TSMS_CUSTOM_parallelWrite(handler->mode, &value, 1);
}

static uint32_t AD7606_DATA[8];
static float AD7606_FLOAT_DATA[8];

float *AD7606_readData(struct AD7606_Handler *handler) {
	if (handler->handler->type == TSMS_DRIVER_TYPE_SPI) {
		handler->ready = 0;
		TSMS_GPIO_write(handler->convstA, TSMS_GPIO_STATUS_LOW);
		TSMS_GPIO_write(handler->convstB, TSMS_GPIO_STATUS_LOW);
		TSMS_SPI_delay(handler->handler->spi, TSMS_NO_DELAY_TIME);
		TSMS_GPIO_write(handler->convstA, TSMS_GPIO_STATUS_HIGH);
		TSMS_GPIO_write(handler->convstB, TSMS_GPIO_STATUS_HIGH);
		while (!handler->ready);
		handler->handler->spiRead(handler->handler->spi, AD7606_DATA, 16, 8);
		for (int i = 0; i < 8; i++)
			AD7606_FLOAT_DATA[i] =
					((int16_t) AD7606_DATA[i]) * handler->voltage * 2.5 / handler->referenceVoltage / 32768;
	}
	return AD7606_FLOAT_DATA;
}


