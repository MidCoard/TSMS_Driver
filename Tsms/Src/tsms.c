#include "tsms.h"

TSMS_RESULT TSMS_init(TSMS_CLOCK_FREQUENCY frequency) {
	TSMS_RESULT result = TSMS_SUCCESS;
	result |= TSMS_IT_init(frequency);
	result |= TSMS_SPI_init(frequency);
	result |= TSMS_IIC_init(frequency);
	result |= TSMS_STRING_init();
	result |= TSMS_LIST_init();
	return result;
}