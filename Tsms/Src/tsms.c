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

void TSMS_Delay(uint32_t ms) {
#ifdef TSMS_STM32
	HAL_Delay(ms);
#endif
}

TSMS_BITS TSMS_bits(uint8_t bits) {
	switch (bits) {
		case 8:
			return TSMS_8BIT;
		case 16:
			return TSMS_16BIT;
		case 24:
			return TSMS_24BIT;
		case 32:
			return TSMS_32BIT;
		default:
			return TSMS_8BIT;
	}
}