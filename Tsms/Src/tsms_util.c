#include "tsms_util.h"

uint8_t TSMS_UTIL_reverseByte(uint8_t v) {
	return TSMS_UTIL_reverseData(v, 8);
}

uint32_t TSMS_UTIL_reverseData(uint32_t v, uint8_t bits) {
	uint32_t t = 0;
	for (uint8_t i = 0; i < bits; i++) {
		t <<= 1;
		t |= (v & 1);
		v >>= 1;
	}
	return t;
}

uint8_t *TSMS_UTIL_streamAppend(uint8_t * stream, TSMS_SIZE size, uint8_t * data, TSMS_SIZE dataSize) {
	if (data == TSMS_NULL)
		return TSMS_NULL;
	uint8_t * tmp = realloc(stream, size + dataSize);
	if (tmp == TSMS_NULL)
		return TSMS_NULL;
	memcpy(tmp + size, data, dataSize);
	return tmp;
}
