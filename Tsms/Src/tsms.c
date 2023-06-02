#include "tsms.h"
#include "tsms_printer.h"

TSMS_RESULT TSMS_init(TSMS_CLOCK_FREQUENCY frequency, TSMS_CLOCK_FREQUENCY timerFrequency) {
	TSMS_RESULT result = TSMS_SUCCESS;
	result |= TSMS_IT_init(frequency);
	result |= TSMS_SPI_init(frequency);
	result |= TSMS_IIC_init(frequency);
	result |= TSMS_TIMER_init(timerFrequency);
	result |= TSMS_STRING_init();
	result |= TSMS_FONT_init();
	result |= TSMS_LIST_init();
#ifdef TSMS_GUI_STYLE
	result |= TSMS_GUI_STYLE_init();
	result |= TSMS_NATIVE_MUTABLE_init();
#endif
	return result;
}

void TSMS_delay(uint32_t ms) {
#ifdef TSMS_STM32
	HAL_Delay(ms);
#endif
}

void * TSMS_malloc(size_t size) {
	void * result = malloc(size);
	if (result == TSMS_NULL) {
		TSMS_ERR_report(TSMS_ERROR_TYPE_MALLOC_FAILED, TSMS_NULL);
		print("Malloc failed at %p\n", __builtin_return_address(0));
		return TSMS_NULL;
	}
	return result;
}

void * TSMS_realloc(void * ptr, size_t size) {
	void * result = realloc(ptr, size);
	if (result == TSMS_NULL) {
		TSMS_ERR_report(TSMS_ERROR_TYPE_REALLOC_FAILED, TSMS_NULL);
		print("Realloc failed at %p\n", __builtin_return_address(0));
		return TSMS_NULL;
	}
	return result;
}