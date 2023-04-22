#include "tsms.h"

TSMS_RESULT TSMS_init(TSMS_CLOCK_FREQUENCY frequency, TSMS_CLOCK_FREQUENCY timerFrequency) {
	TSMS_RESULT result = TSMS_SUCCESS;
	result |= TSMS_IT_init(frequency);
	result |= TSMS_SPI_init(frequency);
	result |= TSMS_IIC_init(frequency);
	result |= TSMS_TIMER_init(timerFrequency);
	result |= TSMS_STRING_init();
	result |= TSMS_FONT_init();
	result |= TSMS_LIST_init();
	return result;
}

void TSMS_delay(uint32_t ms) {
#ifdef TSMS_STM32
	HAL_Delay(ms);
#endif
}