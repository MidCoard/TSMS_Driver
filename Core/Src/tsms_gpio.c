#include "tsms_gpio.h"

#ifdef TSMS_STM32_GPIO

TSMS_GHP TSMS_GPIO_createHandler(GPIO_TypeDef * port, uint16_t pin) {
	TSMS_GHP gpio = malloc(sizeof (struct TSMS_GPIO_HANDLER));
	gpio->port = port;
	gpio->pin = pin;
	return gpio;
}

#endif

TSMS_RESULT TSMS_GPIO_write(TSMS_GHP gpio, TSMS_GPIO_STATUS  status) {
	if (gpio == TSMS_NULL_GHP)
		return TSMS_SUCCESS;
#ifdef TSMS_STM32_GPIO
	HAL_GPIO_WritePin(gpio->port,gpio->pin,status ? GPIO_PIN_SET : GPIO_PIN_RESET);
	return TSMS_SUCCESS;
#else
	return TSMS_TIMEOUT;
#endif
}

TSMS_GPIO_STATUS TSMS_GPIO_read(TSMS_GHP gpio) {
	if (gpio == TSMS_NULL_GHP)
		return TSMS_GPIO_ERROR;
#ifdef TSMS_STM32_GPIO
	return HAL_GPIO_ReadPin(gpio->port, gpio->pin) ? TSMS_GPIO_HIGH : TSMS_GPIO_LOW;
#else
	return TSMS_GPIO_ERROR;
#endif
}