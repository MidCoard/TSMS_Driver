#include "tsms_timer.h"
#include "tsms_it.h"

double TSMS_DELAY_UNIT_TO_SECOND[4] = {1, 0.001, 0.000001, 0.000000001};

TSMS_INLINE void __tsms_internal_period_callback(void * handler, pTimer timer) {
	if (timer->option.enableDelay)
		timer->periods++;
	if (timer->option.enableCallbackInterrupt)
		if (timer->callback != TSMS_NULL)
			timer->callback(timer->handler, timer);
}

TSMS_CLOCK_FREQUENCY defaultTimerClock;

TSMS_RESULT TSMS_TIMER_init(TSMS_CLOCK_FREQUENCY frequency) {
	defaultTimerClock = frequency;
	return TSMS_SUCCESS;
}

#ifdef TSMS_STM32_TIMER

pTimer TSMS_TIMER_create(TIM_HandleTypeDef* tim, TSMS_TIMER_OPTION option) {
	pTimer timer = (pTimer) malloc(sizeof(tTimer));
	if (timer == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for pTimer");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	timer->timer = tim;
	timer->option = option;
	timer->periods = 0;
	timer->callback = TSMS_NULL;
	if (option.enablePeriodInterrupt)
		TSMS_IT_addTimer(timer, TSMS_IT_TIMER_PERIOD_ELAPSED, __tsms_internal_period_callback, TSMS_NULL);
	if (option.enableDelay)
		if (!option.enablePeriodInterrupt)
			TSMS_IT_addTimer(timer, TSMS_IT_TIMER_PERIOD_ELAPSED, __tsms_internal_period_callback, TSMS_NULL);
	return timer;
}

TSMS_RESULT TSMS_TIMER_start(pTimer timer) {
	if (timer == TSMS_NULL)
		return TSMS_ERROR;
	if (timer->option.enablePeriodInterrupt)
		return HAL_TIM_Base_Start_IT(timer->timer) == HAL_OK ? TSMS_SUCCESS : TSMS_FAIL;
	else
		return HAL_TIM_Base_Start(timer->timer) == HAL_OK ? TSMS_SUCCESS : TSMS_FAIL;
}

TSMS_RESULT TSMS_TIMER_stop(pTimer timer) {
	if (timer == TSMS_NULL)
		return TSMS_ERROR;
	if (timer->option.enablePeriodInterrupt)
		return HAL_TIM_Base_Stop_IT(timer->timer) == HAL_OK ? TSMS_SUCCESS : TSMS_FAIL;
	else
		return HAL_TIM_Base_Stop(timer->timer) == HAL_OK ? TSMS_SUCCESS : TSMS_FAIL;
}

TSMS_RESULT TSMS_TIMER_delay(pTimer timer, TSMS_DELAY_TIME delay) {
	if (timer == TSMS_NULL)
		return TSMS_ERROR;
	if (!timer->option.enableDelay)
		return TSMS_ERROR;
	volatile uint64_t now = TSMS_TIMER_nowRaw(timer);
	volatile uint64_t target = now + delay * (defaultTimerClock * TSMS_DELAY_UNIT_TO_SECOND[timer->option.delayUnit]) / (timer->timer->Init.Prescaler + 1);
	while (now < target)
		now = TSMS_TIMER_nowRaw(timer);
	return TSMS_SUCCESS;
}

volatile double TSMS_TIMER_now(pTimer timer) {
	if (timer == TSMS_NULL)
		return -1;
	return (double) TSMS_TIMER_nowRaw(timer) / ((double) defaultTimerClock / (timer->timer->Init.Prescaler + 1));
}

volatile uint64_t TSMS_TIMER_nowRaw(pTimer timer) {
	if (timer == TSMS_NULL)
		return 0;
	return timer->periods * (timer->timer->Init.Period + 1) + __HAL_TIM_GET_COUNTER(timer->timer);
}

#endif

TSMS_RESULT TSMS_TIMER_setCallback(pTimer timer, TSMS_TIMER_CALLBACK callback, void * handler) {
	if (timer == TSMS_NULL)
		return TSMS_ERROR;
	timer->callback = callback;
	timer->handler = handler;
	return TSMS_SUCCESS;
}