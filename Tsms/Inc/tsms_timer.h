#ifndef TSMS_TIMER_H
#define TSMS_TIMER_H

typedef struct TSMS_TIMER_HANDLER tTimer;
typedef tTimer *pTimer;

typedef struct TSMS_TIMER_OPTION* TSMS_TIMER_OPTION_POINTER;
typedef TSMS_TIMER_OPTION_POINTER TSMS_TIMER_OP;

typedef enum  {
	TSMS_DELAY_UNIT_S,
	TSMS_DELAY_UNIT_MS,
	TSMS_DELAY_UNIT_US,
	TSMS_DELAY_UNIT_NS
}TSMS_DELAY_UNIT;

typedef void(*TSMS_IT_TIMER_CALLBACK)(void *, pTimer);
typedef void(*TSMS_TIMER_CALLBACK)(void *, pTimer);

#include "tsms_it.h"

TSMS_RESULT TSMS_IT_addTimer(pTimer timer,TSMS_IT_TIMER_TYPE type, TSMS_IT_TIMER_CALLBACK callback, void *handler);

#include "tsms.h"

struct TSMS_TIMER_HANDLER {
#ifdef TSMS_STM32_TIMER
	TIM_HandleTypeDef *timer;
#endif
	TSMS_TIMER_OP option;
	uint64_t periods;
	TSMS_TIMER_CALLBACK callback;
	void *handler;
};

struct TSMS_TIMER_OPTION {
	bool enablePeriodInterrupt;
	bool enableDelay;
	bool enableCallbackInterrupt;
	TSMS_DELAY_UNIT delayUnit;
};

extern TSMS_CLOCK_FREQUENCY defaultTimerClock;

#ifdef TSMS_STM32_TIMER
pTimer TSMS_TIMER_create(TIM_HandleTypeDef* tim, TSMS_TIMER_OP option);

TSMS_RESULT TSMS_TIMER_start(pTimer timer);

TSMS_RESULT TSMS_TIMER_stop(pTimer timer);

TSMS_RESULT TSMS_TIMER_delay(pTimer timer, TSMS_DELAY_TIME delay);

volatile double TSMS_TIMER_now(pTimer timer);

volatile uint64_t TSMS_TIMER_nowRaw(pTimer timer);
#endif

TSMS_RESULT TSMS_TIMER_setCallback(pTimer timer, TSMS_TIMER_CALLBACK callback, void* handler);

#endif //TSMS_TIMER_H
