#ifndef TSMS_AD7606_H
#define TSMS_AD7606_H

#include "tsms_gpio.h"

struct AD7606_Handler {
	TSMS_DHP handler;
	TSMS_CHP parallel;
	TSMS_CHP overSample;
	TSMS_GHP range;
	TSMS_GHP reset;
	TSMS_GHP referenceSelect;
	TSMS_CHP digitalInterface;
	TSMS_GHP busy;
	TSMS_GHP firstData;
	float referenceVoltage;
	TSMS_GHP convstA;
	TSMS_GHP convstB;
	TSMS_CHP mode;
	uint8_t voltage;
	volatile uint8_t ready;
};

#define AD7606_OS_NONE 0
#define AD7606_OS_2 1
#define AD7606_OS_4 2
#define AD7606_OS_8 3
#define AD7606_OS_16 4
#define AD7606_OS_32 5
#define AD7606_OS_64 6

#define AD7606_RANGE_5V 0
#define AD7606_RANGE_10V 1

#define AD7606_REFERENCE_MODE_EXTERNAL 0
#define AD7606_REFERENCE_MODE_INTERNAL 1

#define AD7606_MODE_SHUTDOWN 0
#define AD7606_MODE_POWER_ON 1
#define AD7606_MODE_STANDBY 2

typedef uint8_t AD7606_OS_RATE;
typedef uint8_t AD7606_RANGE;
typedef uint8_t AD7606_REFERENCE_MODE;
typedef uint8_t AD7606_MODE;

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
                          uint16_t doutBPin, float referenceVoltage);
#endif


void AD7606_setOverSampleRate(struct AD7606_Handler *handler, AD7606_OS_RATE overSampleRate);

void AD7606_setRange(struct AD7606_Handler *handler, AD7606_RANGE range);

void AD7606_setReferenceMode(struct AD7606_Handler *handler, AD7606_REFERENCE_MODE referenceMode);

void AD7606_setMode(struct AD7606_Handler *handler, AD7606_MODE mode);

float *AD7606_readData(struct AD7606_Handler *handler);

#endif //TSMS_AD7606_H
