#ifndef TSMS_CUSTOM_H
#define TSMS_CUSTOM_H

#include "tsms_gpio.h"
#include "tsms_util.h"

struct TSMS_CUSTOM_HANDLER;

typedef struct TSMS_CUSTOM_HANDLER * TSMS_CUSTOM_HANDLER_POINTER;
typedef TSMS_CUSTOM_HANDLER_POINTER TSMS_CHP;

struct TSMS_CUSTOM_HANDLER {
	TSMS_GHP sda;
	TSMS_GHP scl;
	TSMS_DELAY_FUNCTION delay;
	TSMS_CUSTOM_DELAY_FUNCTION customDelay;
	TSMS_ULP list;
	TSMS_TRANSFER_TYPE transferType;
};

TSMS_CHP TSMS_CUSTOM_createHandler(uint32_t n,...);

TSMS_CHP TSMS_CUSTOM_createSpecialHandler(TSMS_CUSTOM_DELAY_FUNCTION delay,TSMS_GHP scl, TSMS_GHP sda, uint32_t n, ...);

TSMS_CHP TSMS_CUSTOM_createParallelHandler(TSMS_CUSTOM_DELAY_FUNCTION delay, TSMS_TRANSFER_TYPE type, uint32_t n, ...);

TSMS_RESULT TSMS_CUSTOM_parallelWrite(TSMS_CHP handler, const uint32_t *data, uint32_t length);

TSMS_RESULT TSMS_CUSTOM_parallelRead(TSMS_CHP handler, uint32_t *data, uint32_t length);

TSMS_GHP TSMS_CUSTOM_getGPIO(TSMS_CHP handler, uint32_t index);

TSMS_RESULT TSMS_CUSTOM_delay(TSMS_CHP handler, TSMS_DELAY_TIME time);

TSMS_RESULT TSMS_CUSTOM_release(TSMS_CHP handler);


#endif //TSMS_CUSTOM_H
