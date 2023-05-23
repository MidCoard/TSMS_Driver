#include "tsms_gpio.h"

#ifdef TSMS_STM32_GPIO

TSMS_GHP TSMS_GPIO_createHandler(GPIO_TypeDef * port, uint16_t pin) {
	if (port == TSMS_NULL)
		return TSMS_NULL_GHP;
	TSMS_GHP gpio = TSMS_malloc(sizeof (struct TSMS_GPIO_HANDLER));
	if (gpio == TSMS_NULL)
		return TSMS_NULL_GHP;
	gpio->port = port;
	gpio->pin = pin;
	return gpio;
}

#endif

TSMS_RESULT TSMS_GPIO_write(TSMS_GHP gpio, TSMS_GPIO_STATUS status) {
	if (gpio == TSMS_NULL_GHP)
		return TSMS_ERROR;
#ifdef TSMS_STM32_GPIO
	HAL_GPIO_WritePin(gpio->port,gpio->pin,status ? GPIO_PIN_SET : GPIO_PIN_RESET);
	return TSMS_SUCCESS;
#else
	return TSMS_FAIL;
#endif
}

TSMS_GPIO_STATUS TSMS_GPIO_read(TSMS_GHP gpio) {
	if (gpio == TSMS_NULL_GHP)
		return TSMS_GPIO_STATUS_ERROR;
#ifdef TSMS_STM32_GPIO
	return HAL_GPIO_ReadPin(gpio->port, gpio->pin) ? TSMS_GPIO_STATUS_HIGH : TSMS_GPIO_STATUS_LOW;
#else
	return TSMS_GPIO_STATUS_ERROR;
#endif
}


TSMS_RESULT TSMS_GPIO_release(TSMS_GHP gpio) {
	if (gpio == TSMS_NULL_GHP)
		return TSMS_ERROR;
	free(gpio);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_GPIO_setMode(TSMS_GHP gpio, TSMS_GPIO_MODE mode, TSMS_GPIO_PULL pull) {
	if (gpio == TSMS_NULL_GHP)
		return TSMS_ERROR;
#ifdef TSMS_STM32_GPIO
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = gpio->pin;
	if (mode == TSMS_GPIO_MODE_OUTPUT_OD)
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	else if (mode == TSMS_GPIO_MODE_OUTPUT_PP)
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	else if (mode == TSMS_GPIO_MODE_INPUT)
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	else if (mode == TSMS_GPIO_MODE_IT_RISING)
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	else if (mode == TSMS_GPIO_MODE_IT_FALLING)
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	else if (mode == TSMS_GPIO_IT_MODE_RISING_FALLING)
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	else
		return TSMS_ERROR;
	if (pull == TSMS_GPIO_PULL_NONE)
		GPIO_InitStruct.Pull = GPIO_NOPULL;
	else if (pull == TSMS_GPIO_PULL_UP)
		GPIO_InitStruct.Pull = GPIO_PULLUP;
	else if (pull == TSMS_GPIO_PULL_DOWN)
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	else
		return TSMS_ERROR;
	HAL_GPIO_Init(gpio->port, &GPIO_InitStruct);
	return TSMS_SUCCESS;
#else
	return TSMS_FAIL;
#endif
}