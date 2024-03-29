#include "tsms_timer.h"

static double _timerDelayUnitToSecond[4] = {1, 0.001, 0.000001, 0.000000001};

static pTimer _timerDefaultTimer;

TSMS_CLOCK_FREQUENCY TSMS_DEFAULT_TIMER_CLOCK_FREQUENCY;

TSMS_INLINE void __tsms_internal_period_callback(void * handler, pTimer timer) {
	if (timer->option.enablePeriodInterrupt)
		timer->periods++;
	if (timer->option.enableCallbackInterrupt)
		if (timer->callback != TSMS_NULL)
			timer->callback(timer->handler, timer);
}

TSMS_RESULT TSMS_TIMER_init(TSMS_CLOCK_FREQUENCY frequency) {
	TSMS_DEFAULT_TIMER_CLOCK_FREQUENCY = frequency;
	return TSMS_SUCCESS;
}

#ifdef TSMS_STM32_TIMER

pTimer TSMS_TIMER_create(TIM_HandleTypeDef* tim, TSMS_TIMER_OPTION option) {
	pTimer timer = (pTimer) TSMS_malloc(sizeof(tTimer));
	if (timer == TSMS_NULL)
		return TSMS_NULL;
	timer->timer = tim;
	timer->option = option;
	timer->periods = 0;
	timer->callback = TSMS_NULL;
	if (option.enablePeriodInterrupt || option.enableCallbackInterrupt)
		TSMS_IT_addTimer(timer, TSMS_IT_TIMER_TYPE_PERIOD_ELAPSED, __tsms_internal_period_callback, TSMS_NULL);
	return timer;
}
#endif

TSMS_RESULT TSMS_TIMER_start(pTimer timer) {
	if (timer == TSMS_NULL)
		return TSMS_ERROR;
#ifdef TSMS_STM32_TIMER
	if (timer->option.enablePeriodInterrupt)
		return HAL_TIM_Base_Start_IT(timer->timer) == HAL_OK ? TSMS_SUCCESS : TSMS_FAIL;
	else
		return HAL_TIM_Base_Start(timer->timer) == HAL_OK ? TSMS_SUCCESS : TSMS_FAIL;
#else
	return TSMS_SUCCESS;
#endif
}

TSMS_RESULT TSMS_TIMER_stop(pTimer timer) {
	if (timer == TSMS_NULL)
		return TSMS_ERROR;
#ifdef TSMS_STM32_TIMER
	if (timer->option.enablePeriodInterrupt)
		return HAL_TIM_Base_Stop_IT(timer->timer) == HAL_OK ? TSMS_SUCCESS : TSMS_FAIL;
	else
		return HAL_TIM_Base_Stop(timer->timer) == HAL_OK ? TSMS_SUCCESS : TSMS_FAIL;
#else
	return TSMS_SUCCESS;
#endif
}

TSMS_RESULT TSMS_TIMER_delay(pTimer timer, TSMS_DELAY_TIME delay) {
	if (timer == TSMS_NULL)
		return TSMS_ERROR;
	if (!timer->option.enablePeriodInterrupt)
		return TSMS_ERROR;
#ifdef TSMS_STM32_TIMER
	volatile uint64_t now = TSMS_TIMER_nowRaw(timer);
	volatile uint64_t target = now + delay * (TSMS_DEFAULT_TIMER_CLOCK_FREQUENCY * _timerDelayUnitToSecond[timer->option.delayUnit]) / (timer->timer->Init.Prescaler + 1);
	while (now < target)
		now = TSMS_TIMER_nowRaw(timer);
	return TSMS_SUCCESS;
#else
	return TSMS_SUCCESS;
#endif
}

volatile double TSMS_TIMER_now(pTimer timer) {
	if (timer == TSMS_NULL)
		return -1;
#ifdef TSMS_STM32_TIMER
	return (double) TSMS_TIMER_nowRaw(timer) / ((double) TSMS_DEFAULT_TIMER_CLOCK_FREQUENCY / (timer->timer->Init.Prescaler + 1));
#else
	return 0;
#endif
}

volatile uint64_t TSMS_TIMER_nowRaw(pTimer timer) {
	if (timer == TSMS_NULL)
		return 0;
#ifdef TSMS_STM32_TIMER
	return timer->periods * (timer->timer->Init.Period + 1) + __HAL_TIM_GET_COUNTER(timer->timer);
#else
	return 0;
#endif
}

TSMS_RESULT TSMS_TIMER_setCallback(pTimer timer, TSMS_TIMER_CALLBACK callback, void * handler) {
	if (timer == TSMS_NULL)
		return TSMS_ERROR;
	timer->callback = callback;
	timer->handler = handler;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_TIMER_setDefaultTimer(pTimer timer) {
	_timerDefaultTimer = timer;
	return TSMS_SUCCESS;
}

pTimer TSMS_TIMER_getDefaultTimer() {
	return _timerDefaultTimer;
}

TSMS_RESULT TSMS_TIMER_delayDefault(TSMS_DELAY_TIME delay) {
	if (_timerDefaultTimer == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_TIMER_delay(_timerDefaultTimer, delay);
}