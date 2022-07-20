#ifndef TSMS_GPIO_H
#define TSMS_GPIO_H

#include "tsms.h"

struct TSMS_GPIO_HANDLER {
#ifdef TSMS_STM32_GPIO
	GPIO_TypeDef * port;
	uint16_t pin;
#endif
};

typedef struct TSMS_GPIO_HANDLER * TSMS_GPIO_HANDLER_POINTER;
typedef TSMS_GPIO_HANDLER_POINTER TSMS_GHP;

typedef enum{
	TSMS_GPIO_HIGH,TSMS_GPIO_LOW,TSMS_GPIO_ERROR
} TSMS_GPIO_STATUS;

#define TSMS_NULL_GHP (TSMS_GHP)TSMS_NULL

#ifdef TSMS_STM32_GPIO

TSMS_GHP TSMS_GPIO_createHandler(GPIO_TypeDef * port, uint16_t pin);

#endif

TSMS_RESULT TSMS_GPIO_write(TSMS_GHP gpio, TSMS_GPIO_STATUS  status);
TSMS_GPIO_STATUS TSMS_GPIO_read(TSMS_GHP gpio);

TSMS_RESULT TSMS_GPIO_release(TSMS_GHP gpio);

#endif //TSMS_GPIO_H
