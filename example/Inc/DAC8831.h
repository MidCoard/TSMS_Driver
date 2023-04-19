#ifndef TSMS_DAC8831_H
#define TSMS_DAC8831_H

#include "tsms_driver.h"

struct DAC8831_Handler {
	TSMS_DHP handler;
	TSMS_GHP ldac;
	float reference;
};

void DAC8831_update(struct DAC8831_Handler *handler);

void DAC8831_setAnalogOutput(struct DAC8831_Handler *handler, float out);

void DAC8831_setAnalogOutputLater(struct DAC8831_Handler *handler, float out);

#ifdef TSMS_STM32_GPIO
struct DAC8831_Handler *
DAC8831_initHardware(GPIO_TypeDef *sdi, uint16_t sdiPin, GPIO_TypeDef *sclk, uint16_t sclkPin, GPIO_TypeDef *cs,
                     uint16_t csPin, GPIO_TypeDef *ldac, uint16_t ldacPin, float reference);
#endif

#endif //TSMS_DAC8831_H
