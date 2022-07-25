
#ifndef TSMS_IT_H
#define TSMS_IT_H

#include "tsms_gpio.h"

typedef enum {TSMS_IT_GPIO_FALLING, TSMS_IT_GPIO_RISING} TSMS_IT_GPIO_TYPE;

typedef void(*TSMS_IT_GPIO_CALLBACK)(void*,TSMS_GHP);

void TSMS_IT_addGPIO(TSMS_GHP gpio, TSMS_IT_GPIO_TYPE type,TSMS_IT_GPIO_CALLBACK callback, void *);

#endif //TSMS_IT_H
