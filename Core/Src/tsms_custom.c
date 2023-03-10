#include "tsms_custom.h"

TSMS_INLINE void __tsms_internal_custom_delay() {
	volatile uint8_t c = 1;
	while (c--);
}

TSMS_CHP TSMS_CUSTOM_createHandler(uint32_t n, ...) {
	va_list args;
	va_start(args, n);
	TSMS_CHP handler = (TSMS_CHP) malloc(sizeof(struct TSMS_CUSTOM_HANDLER));
	if (handler == TSMS_NULL) {
		va_end(args);
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_CHP");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	handler->sda = TSMS_NULL_GHP;
	handler->scl = TSMS_NULL_GHP;
	handler->delay = __tsms_internal_custom_delay;
	handler->customDelay = TSMS_NULL;
	handler->list = TSMS_LIST_createList(10);
	if (handler->list == TSMS_NULL) {
		free(handler);
		va_end(args);
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_CHP list");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	for (int i = 0; i < n; i++) {
		TSMS_RESULT result = TSMS_LIST_addList(handler->list, va_arg(args, TSMS_GHP));
		if (result != TSMS_SUCCESS) {
			TSMS_CUSTOM_release(handler);
			va_end(args);
			tString temp = TSMS_STRING_temp("failed to add TSMS_GHP to TSMS_CHP list");
			TSMS_ERR_report(TSMS_ERR_INIT_FAILED, &temp);
			return TSMS_NULL;
		}
	}
	va_end(args);
	return handler;
}

TSMS_CHP
TSMS_CUSTOM_createSpecialHandler(TSMS_CUSTOM_DELAY_FUNCTION delay, TSMS_GHP scl, TSMS_GHP sda, uint32_t n, ...) {
	va_list args;
	va_start(args, n);
	TSMS_CHP handler = TSMS_CUSTOM_createHandler(n, args);
	if (handler == TSMS_NULL) {
		va_end(args);
		return TSMS_NULL;
	}
	handler->sda = sda;
	handler->scl = scl;
	handler->customDelay = delay;
	va_end(args);
	return handler;
}

TSMS_CHP TSMS_CUSTOM_createParallelHandler(TSMS_CUSTOM_DELAY_FUNCTION delay, TSMS_TRANSFER_TYPE type, uint32_t n, ...) {
	va_list args;
	va_start(args, n);
	TSMS_CHP handler = TSMS_CUSTOM_createHandler(n, args);
	if (handler == TSMS_NULL) {
		va_end(args);
		return TSMS_NULL;
	}
	handler->customDelay = delay;
	handler->transferType = type;
	va_end(args);
	return handler;
}

TSMS_RESULT TSMS_CUSTOM_parallelWrite(TSMS_CHP handler, const uint32_t *data, uint32_t length) {
	if (handler == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result;
	if (handler->transferType == TSMS_TRANSFER_MSB)
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < handler->list->length; j++) {
				TSMS_GHP ghp = TSMS_CUSTOM_getGPIO(handler, j);
				if ((result = TSMS_GPIO_write(ghp, (data[i] << j) & 0x80)) != TSMS_SUCCESS)
					return result;
			}
			TSMS_CUSTOM_delay(handler, TSMS_NO_DELAY_TIME);
		}
	else if (handler->transferType == TSMS_TRANSFER_LSB)
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < handler->list->length; j++) {
				TSMS_GHP ghp = TSMS_CUSTOM_getGPIO(handler, j);
				if ((result = TSMS_GPIO_write(ghp, (data[i] >> j) & 0x01)) != TSMS_SUCCESS)
					return result;
			}
			TSMS_CUSTOM_delay(handler, TSMS_NO_DELAY_TIME);
		}
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_CUSTOM_parallelRead(TSMS_CHP handler, uint32_t *data, uint32_t length) {
	if (handler == TSMS_NULL)
		return TSMS_ERROR;
	if (handler->transferType == TSMS_TRANSFER_MSB)
		for (int i = 0; i < length; i++) {
			data[i] = 0;
			for (int j = 0; j < handler->list->length; j++) {
				TSMS_GHP ghp = TSMS_CUSTOM_getGPIO(handler, j);
				uint32_t value = TSMS_GPIO_read(ghp);
				if (value == TSMS_GPIO_ERROR)
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
				if (value == TSMS_GPIO_ERROR)
					return TSMS_ERROR;
				data[i] |= value << j;
			}
			TSMS_CUSTOM_delay(handler, TSMS_NO_DELAY_TIME);
		}
	return TSMS_SUCCESS;
}

TSMS_GHP TSMS_CUSTOM_getGPIO(TSMS_CHP handler, uint32_t index) {
	if (handler == TSMS_NULL)
		return TSMS_NULL;
	if (index >= handler->list->length)
		return TSMS_NULL;
	return handler->list->list[index];
}

TSMS_RESULT TSMS_CUSTOM_delay(TSMS_CHP handler, TSMS_DELAY_TIME time) {
	if (handler == TSMS_NULL)
		return TSMS_ERROR;
	if (handler->customDelay != TSMS_NULL)
		handler->customDelay(time);
	else
		handler->delay();
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_CUSTOM_release(TSMS_CHP handler) {
	if (handler == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LIST_releaseList(handler->list);
	free(handler->sda);
	free(handler->scl);
	free(handler);
	return TSMS_SUCCESS;
}


