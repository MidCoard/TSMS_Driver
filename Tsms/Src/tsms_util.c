#include "tsms_util.h"

const TSMS_COLOR TSMS_COLOR_BLACK = {0, 0, 0};
const TSMS_COLOR TSMS_COLOR_WHITE = {255, 255, 255};
const TSMS_COLOR TSMS_COLOR_RED = {255, 0, 0};
const TSMS_COLOR TSMS_COLOR_GREEN = {0, 255, 0};
const TSMS_COLOR TSMS_COLOR_BLUE = {0, 0, 255};
const TSMS_COLOR TSMS_COLOR_YELLOW = {255, 255, 0};
const TSMS_COLOR TSMS_COLOR_CYAN = {0, 255, 255};
const TSMS_COLOR TSMS_COLOR_MAGENTA = {255, 0, 255};

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

TSMS_BITS TSMS_UTIL_bits(uint8_t bits) {
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

TSMS_COLOR TSMS_UTIL_color(uint8_t r, uint8_t g, uint8_t b) {
	return (TSMS_COLOR ) {r, g, b };
}

uint16_t TSMS_UTIL_color565(TSMS_COLOR color) {
	return (color.red & 0xF8) << 8 | (color.green & 0xFC) << 3 | (color.blue & 0xF8) >> 3;
}

uint32_t TSMS_UTIL_color888(TSMS_COLOR color) {
	return (color.red & 0xFF) << 16 | (color.green & 0xFF) << 8 | (color.blue & 0xFF);
}

void TSMS_UTIL_swap(uint16_t* a, uint16_t* b) {
	uint16_t t = *a;
	*a = *b;
	*b = t;
}

TSMS_COLOR TSMS_UTIL_gradientColor(TSMS_COLOR color1, TSMS_COLOR color2, float ratio) {
	TSMS_COLOR color;
	color.red = (uint8_t) (color1.red * (1 - ratio) + color2.red * ratio);
	color.green = (uint8_t) (color1.green * (1 - ratio) + color2.green * ratio);
	color.blue = (uint8_t) (color1.blue * (1 - ratio) + color2.blue * ratio);
	return color;
}