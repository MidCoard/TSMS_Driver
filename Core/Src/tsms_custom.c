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
	handler->custom_delay = TSMS_NULL;
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
	handler->custom_delay = delay;
	handler->list = TSMS_UTIL_createList(10);
	for (int i = 0; i < n; i++)
		TSMS_UTIL_addList(handler->list, va_arg(args, TSMS_GHP));
	va_end(args);
	return handler;
}

TSMS_GHP TSMS_CUSTOM_getGPIO(TSMS_CHP handler, uint32_t index) {
	return handler->list->list[index];
}

void TSMS_CUSTOM_delay(TSMS_CHP handler, TSMS_DELAY_TIME time) {
	if (handler->custom_delay != TSMS_NULL)
		handler->custom_delay(time);
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


