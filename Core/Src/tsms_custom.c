//
// Created by 周蜀杰 on 2022/10/29.
//
#include "tsms_custom.h"

TSMS_INLINE void __tsms_internal_custom_delay() {
	volatile uint8_t c = 1;
	while(c--);
}

TSMS_CHP TSMS_CUSTOM_createHandler(uint32_t n,...) {
	va_list args;
	va_start(args, n);
	TSMS_CHP handler = (TSMS_CHP) malloc(sizeof(struct TSMS_CUSTOM_HANDLER));
	handler->sda = TSMS_NULL_GHP;
	handler->scl = TSMS_NULL_GHP;
	handler->delay = __tsms_internal_custom_delay;
	handler->customDelay = TSMS_NULL;
	handler->list = TSMS_UTIL_createList(10);
	for (int i = 0; i < n; i++)
		TSMS_UTIL_addList(handler->list, va_arg(args, TSMS_GHP));
	va_end(args);
	return handler;
}

TSMS_CHP TSMS_CUSTOM_createSpecialHandler(TSMS_CUSTOM_DELAY_FUNCTION delay,TSMS_GHP scl, TSMS_GHP sda, uint32_t n, ...) {
	va_list args;
	va_start(args, n);
	TSMS_CHP handler = (TSMS_CHP) malloc(sizeof(struct TSMS_CUSTOM_HANDLER));
	handler->sda = sda;
	handler->scl = scl;
	handler->delay = __tsms_internal_custom_delay;
	handler->customDelay = delay;
	handler->list = TSMS_UTIL_createList(10);
	for (int i = 0; i < n; i++)
		TSMS_UTIL_addList(handler->list, va_arg(args, TSMS_GHP));
	va_end(args);
	return handler;
}

TSMS_CHP TSMS_CUSTOM_createParallelHandler(TSMS_CUSTOM_DELAY_FUNCTION delay, TSMS_TRANSFER_TYPE type, uint32_t n, ...) {
	va_list args;
	va_start(args, n);
	TSMS_CHP handler = (TSMS_CHP) malloc(sizeof(struct TSMS_CUSTOM_HANDLER));
	handler->sda = TSMS_NULL_GHP;
	handler->scl = TSMS_NULL_GHP;
	handler->delay = __tsms_internal_custom_delay;
	handler->customDelay = delay;
	handler->list = TSMS_UTIL_createList(10);
	handler->transferType = type;
	for (int i = 0; i < n; i++)
		TSMS_UTIL_addList(handler->list, va_arg(args, TSMS_GHP));
	va_end(args);
	return handler;
}

TSMS_RESULT TSMS_CUSTOM_parallelWrite(TSMS_CHP handler, uint32_t *data, uint32_t length) {
	TSMS_RESULT result = TSMS_SUCCESS;
	if (handler->transferType == TSMS_TRANSFER_MSB)
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < handler->list->length; j++) {
				TSMS_GHP ghp = TSMS_CUSTOM_getGPIO(handler, j);
				if (TSMS_GPIO_write(ghp, (data[i] << j) & 0x80) != TSMS_SUCCESS)
					return TSMS_ERROR;
			}
			TSMS_CUSTOM_delay(handler, TSMS_NO_DELAY_TIME);
		}
	else if (handler->transferType == TSMS_TRANSFER_LSB)
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < handler->list->length; j++) {
				TSMS_GHP ghp = TSMS_CUSTOM_getGPIO(handler, j);
				if (TSMS_GPIO_write(ghp, (data[i] >> j) & 0x01) != TSMS_SUCCESS)
					return TSMS_ERROR;
			}
			TSMS_CUSTOM_delay(handler, TSMS_NO_DELAY_TIME);
		}
	return result;
}

TSMS_RESULT TSMS_CUSTOM_parallelRead(TSMS_CHP handler, uint32_t *data, uint32_t length) {
	TSMS_RESULT result = TSMS_SUCCESS;
	if (handler->transferType == TSMS_TRANSFER_MSB)
		for (int i = 0; i < length; i++) {
			data[i] = 0;
			for (int j = 0; j < handler->list->length; j++) {
				TSMS_GHP ghp = TSMS_CUSTOM_getGPIO(handler, j);
				uint32_t value = TSMS_GPIO_read(ghp);
				if (value == TSMS_ERROR)
					return TSMS_ERROR;
				data[i] |= value << (handler->list->length - j - 1);
			}
			TSMS_CUSTOM_delay(handler, TSMS_NO_DELAY_TIME);
		}
	else if (handler->transferType == TSMS_TRANSFER_LSB)
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < handler->list->length; j++) {
				TSMS_GHP ghp = TSMS_CUSTOM_getGPIO(handler, j);
				uint32_t value = TSMS_GPIO_read(ghp);
				if (value == TSMS_ERROR)
					return TSMS_ERROR;
				data[i] |= value << j;
			}
			TSMS_CUSTOM_delay(handler, TSMS_NO_DELAY_TIME);
		}
	return result;
}

TSMS_GHP TSMS_CUSTOM_getGPIO(TSMS_CHP handler, uint32_t index) {
	return handler->list->list[index];
}

void TSMS_CUSTOM_delay(TSMS_CHP handler, TSMS_DELAY_TIME time) {
	if (handler->customDelay != TSMS_NULL)
		handler->customDelay(time);
	else
		handler->delay();
}

TSMS_RESULT TSMS_CUSTOM_release(TSMS_CHP handler) {
	TSMS_UTIL_releaseList(handler->list);
	free(handler->sda);
	free(handler->scl);
	free(handler);
	return TSMS_SUCCESS;
}


