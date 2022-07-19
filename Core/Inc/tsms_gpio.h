//
// Created by 周蜀杰 on 2022/7/19.
//

#ifndef TSMS_GPIO_H
#define TSMS_GPIO_H

#endif //TSMS_GPIO_H

#include "tsms.h"

struct TSMS_GPIO_HANDLER {
#ifdef TSMS_STM32_GPIO
	GPIO_TypeDef * port;
	uint16_t pin;
#endif
};

typedef struct TSMS_GPIO_HANDLER * TSMS_GPIO_HANDLER_POINTER;
typedef TSMS_GPIO_HANDLER_POINTER TSMS_GHP;

#define TSMS_NULL_GHP (TSMS_GHP) TSMS_NULL

#ifdef TSMS_STM32_GPIO

TSMS_GHP TSMS_GPIO_createHandler(GPIO_TypeDef * port, uint16_t pin);

#endif
