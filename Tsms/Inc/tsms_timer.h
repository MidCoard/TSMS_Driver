#ifndef TSMS_TIMER_H
#define TSMS_TIMER_H

typedef enum {
	TSMS_IT_TIMER_TYPE_PERIOD_ELAPSED, TSMS_IT_TIMER_TYPE_CAPTURE
} TSMS_IT_TIMER_TYPE;

typedef struct TSMS_TIMER_HANDLER tTimer;
typedef tTimer *pTimer;

typedef void(*TSMS_IT_TIMER_CALLBACK)(void *, pTimer);
typedef void(*TSMS_TIMER_CALLBACK)(void *, pTimer);

#include "tsms.h"
#include "tsms_def.h"

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

extern TSMS_CLOCK_FREQUENCY TSMS_DEFAULT_TIMER_CLOCK_FREQUENCY;

#ifdef TSMS_STM32_TIMER
pTimer TSMS_TIMER_create(TIM_HandleTypeDef* tim, TSMS_TIMER_OPTION option);
#endif

TSMS_RESULT TSMS_TIMER_start(pTimer timer);

TSMS_RESULT TSMS_TIMER_stop(pTimer timer);

TSMS_RESULT TSMS_TIMER_delay(pTimer timer, TSMS_DELAY_TIME delay);

volatile double TSMS_TIMER_now(pTimer timer);

volatile uint64_t TSMS_TIMER_nowRaw(pTimer timer);

TSMS_RESULT TSMS_TIMER_setCallback(pTimer timer, TSMS_TIMER_CALLBACK callback, void* handler);

TSMS_RESULT TSMS_TIMER_setDefaultTimer(pTimer timer);

pTimer TSMS_TIMER_getDefaultTimer();

TSMS_RESULT TSMS_TIMER_delayDefault(TSMS_DELAY_TIME delay);

TSMS_RESULT TSMS_IT_addTimer(pTimer timer,TSMS_IT_TIMER_TYPE type, TSMS_IT_TIMER_CALLBACK callback, void *handler);

#endif //TSMS_TIMER_H
