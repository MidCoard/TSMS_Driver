#ifndef TSMS_DELAY_H
#define TSMS_DELAY_H

#define TSMS_NO_DELAY_TIME (TSMS_DELAY_TIME)(-1)

typedef enum  {
	TSMS_DELAY_UNIT_S,
	TSMS_DELAY_UNIT_MS,
	TSMS_DELAY_UNIT_US,
	TSMS_DELAY_UNIT_NS
} TSMS_DELAY_UNIT;

typedef int32_t TSMS_DELAY_TIME;

typedef void(*TSMS_DELAY_FUNCTION)();

typedef void(*TSMS_CUSTOM_DELAY_FUNCTION)(TSMS_DELAY_TIME);

void TSMS_delay(uint32_t ms);

#endif //TSMS_DELAY_H
