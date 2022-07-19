#include "tsms_gpio.h"
#ifdef TSMS_STM32_GPIO

TSMS_GHP TSMS_GPIO_createHandler(GPIO_TypeDef * port, uint16_t pin) {
	TSMS_GHP gpio = malloc(sizeof (struct TSMS_GPIO_HANDLER));
	gpio->port = port;
	gpio->pin = pin;
	return gpio;
}

#endif