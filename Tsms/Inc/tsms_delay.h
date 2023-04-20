#ifndef TSMS_DELAY_H
#define TSMS_DELAY_H

#include "tsms_def.h"

typedef int32_t TSMS_DELAY_TIME;
#define TSMS_NO_DELAY_TIME (TSMS_DELAY_TIME)(-1)

typedef void(*TSMS_DELAY_FUNCTION)();

typedef void(*TSMS_CUSTOM_DELAY_FUNCTION)(TSMS_DELAY_TIME);

void TSMS_delay(uint32_t ms);

#endif //TSMS_DELAY_H
