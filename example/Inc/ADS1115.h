#ifndef TSMS_ADS1115_H
#define TSMS_ADS1115_H


#include "tsms_driver.h"
#include "tsms_iic.h"
#include "tsms_printer.h"

#define ADS1115_RESERVE 0
#define ADS1115_P 1
#define ADS1115_D 2
#define ADS1115_COMP_QUE 3
#define ADS1115_COMP_LAT 4
#define ADS1115_COMP_POL 5
#define ADS1115_COMP_MODE 6
#define ADS1115_DR 7
#define ADS1115_M 8
#define ADS1115_PGA 9
#define ADS1115_MUX 10
#define ADS1115_OS 11
#define ADS1115_LO_THRESH 12
#define ADS1115_HI_THRESH 13

#define ADS1115_REG_ADDRESS_POINTER 0
#define ADS1115_REG_CONVERSION 1
#define ADS1115_REG_CONFIG 2
#define ADS1115_REG_LO_THRESH 3
#define ADS1115_REG_HI_THRESH 4


typedef enum {
	ADS1115_8_SPS = 0,
	ADS1115_16_SPS,
	ADS1115_32_SPS,
	ADS1115_64_SPS,
	ADS1115_128_SPS,
	ADS1115_250_SPS,
	ADS1115_475_SPS,
	ADS1115_860_SPS
} ADS1115_DATA_RATE;
typedef enum {
	ADS1115_6_144V = 0,
	ADS1115_4_096V,
	ADS1115_2_048V,
	ADS1115_1_024V,
	ADS1115_0_512V,
	ADS1115_0_256V
} ADS1115_GAIN;
typedef enum {
	ADS1115_CONTINUOUS = 0,
	ADS1115_SINGLE_SHOT
} ADS1115_MODE;
typedef enum {
	ADS1115_TRADITIONAL = 0,
	ADS1115_WINDOW
} ADS1115_COMPARATOR_MODE;
typedef enum {
	ADS1115_AFTER_1 = 0,
	ADS1115_AFTER_2,
	ADS1115_AFTER_4,
	ADS1115_DISABLE
} ADS1115_COMPARATOR_QUEUE_MODE;
typedef enum {
	ADS1115_AIN0_AIN1 = 0,
	ADS1115_AIN0_AIN3,
	ADS1115_AIN1_AIN3,
	ADS1115_AIN2_AIN3,
	ADS1115_AIN0_GND,
	ADS1115_AIN1_GND,
	ADS1115_AIN2_GND,
	ADS1115_AIN3_GND
} ADS1115_CHANNEL_MUX;

struct ADS1115_Handler {
	TSMS_DHP handler;
	float gain;
};
#ifdef TSMS_STM32_GPIO
struct ADS1115_Handler *
ADS1115_initHardware(GPIO_TypeDef *sda, uint16_t sdaPin, GPIO_TypeDef *scl, uint16_t sclPin, uint8_t address);
#endif

uint16_t ADS1115_readRegister(struct ADS1115_Handler *handler, uint8_t reg);

float ADS1115_readData(struct ADS1115_Handler *handler);

void ADS1115_setDataRate(struct ADS1115_Handler *handler, ADS1115_DATA_RATE dataRate);

void ADS1115_setGain(struct ADS1115_Handler *handler, ADS1115_GAIN gain);

void ADS1115_setMode(struct ADS1115_Handler *handler, ADS1115_MODE mode);

void ADS1115_setComparatorMode(struct ADS1115_Handler *handler, ADS1115_COMPARATOR_MODE mode);

void ADS1115_setComparatorPolarity(struct ADS1115_Handler *handler, TSMS_GPIO_STATUS polarity);

void ADS1115_enableComparatorLatch(struct ADS1115_Handler *handler);

void ADS1115_disableComparatorLatch(struct ADS1115_Handler *handler);

void ADS1115_setComparatorQueueMode(struct ADS1115_Handler *handler, ADS1115_COMPARATOR_QUEUE_MODE queue);

void ADS1115_setLowThreshold(struct ADS1115_Handler *handler, float threshold);

void ADS1115_setHighThreshold(struct ADS1115_Handler *handler, float threshold);

void ADS1115_setChannel(struct ADS1115_Handler *handler, ADS1115_CHANNEL_MUX channel);

void ADS1115_startConversion(struct ADS1115_Handler *handler);

bool ADS1115_isInConversion(struct ADS1115_Handler *handler);

#endif //TSMS_ADS1115_H
