#include "tsms_it.h"

TSMS_LLP gpioList = TSMS_NULL;
TSMS_LLP printerList = TSMS_NULL;

TSMS_RESULT TSMS_IT_init(TSMS_CLOCK_FREQUENCY frequency) {
	gpioList = TSMS_LIST_create(10);
	printerList = TSMS_LIST_create(10);
	return TSMS_SUCCESS;
}

#ifdef TSMS_STM32

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	for (int i = 0; i < gpioList->length; i++) {
		TSMS_IGP gpio = gpioList->list[i];
		if (gpio->gpio->pin == GPIO_Pin)
			gpio->callback(gpio->handler, gpio->gpio);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	for (int i = 0; i < printerList->length; i++) {
		TSMS_IPP printer = printerList->list[i];
		if (printer->printer->handler == huart)
			printer->callback(printer->handler, printer->printer);
	}
}

#endif

TSMS_RESULT TSMS_IT_addGPIO(TSMS_GHP gpio, TSMS_IT_GPIO_TYPE type, TSMS_IT_GPIO_CALLBACK callback, void *handler) {
	if (gpio == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_IGP igp = malloc(sizeof(struct TSMS_IT_GPIO));
	if (igp == TSMS_NULL)
		return TSMS_ERROR;
	igp->gpio = gpio;
	igp->type = type;
	igp->callback = callback;
	igp->handler = handler;
	TSMS_LIST_add(gpioList, igp);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_IT_addPrinter(TSMS_PHP php, TSMS_IT_PRINTER_CALLBACK callback, void *handler) {
	if (php == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_IPP ipp = malloc(sizeof(struct TSMS_IT_PRINTER));
	if (ipp == TSMS_NULL)
		return TSMS_ERROR;
	ipp->printer = php;
	ipp->callback = callback;
	ipp->handler = handler;
	TSMS_LIST_add(printerList, ipp);
	return TSMS_SUCCESS;
}