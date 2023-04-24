#ifndef TSMS_TIMER_H
#define TSMS_TIMER_H

typedef enum  {
	TSMS_DELAY_UNIT_S,
	TSMS_DELAY_UNIT_MS,
	TSMS_DELAY_UNIT_US,
	TSMS_DELAY_UNIT_NS
}TSMS_DELAY_UNIT;

#include "tsms.h"

typedef void(*TSMS_TIMER_CALLBACK)(void *, pTimer);

extern TSMS_CLOCK_FREQUENCY defaultTimerClock;

TSMS_RESULT TSMS_IT_addTimer(pTimer timer,TSMS_IT_TIMER_TYPE type, TSMS_IT_TIMER_CALLBACK callback, void *handler);

typedef struct {
	bool enablePeriodInterrupt;
	bool enableCallbackInterrupt;
	TSMS_DELAY_UNIT delayUnit;
} TSMS_TIMER_OPTION;

struct TSMS_TIMER_HANDLER {
#ifdef TSMS_STM32_TIMER
	TIM_HandleTypeDef *timer;
#endif
	TSMS_TIMER_OPTION option;
	uint64_t periods;
	TSMS_TIMER_CALLBACK callback;
	void *handler;
};

#ifdef TSMS_STM32_TIMER
pTimer TSMS_TIMER_create(TIM_HandleTypeDef* tim, TSMS_TIMER_OPTION option);
#endif
TSMS_RESULT TSMS_TIMER_start(pTimer timer);

TSMS_RESULT TSMS_TIMER_stop(pTimer timer);

TSMS_RESULT TSMS_TIMER_delay(pTimer timer, TSMS_DELAY_TIME delay);

volatile double TSMS_TIMER_now(pTimer timer);

volatile uint64_t TSMS_TIMER_nowRaw(pTimer timer);

TSMS_RESULT TSMS_TIMER_setCallback(pTimer timer, TSMS_TIMER_CALLBACK callback, void* handler);

#endif //TSMS_TIMER_H
