#ifndef TSMS_UTIL_H
#define TSMS_UTIL_H

#include "tsms_def.h"

extern TSMS_CR TSMS_COLOR_BLACK;
extern TSMS_CR TSMS_COLOR_WHITE;
extern TSMS_CR TSMS_COLOR_RED;
extern TSMS_CR TSMS_COLOR_GREEN;
extern TSMS_CR TSMS_COLOR_BLUE;
extern TSMS_CR TSMS_COLOR_YELLOW;
extern TSMS_CR TSMS_COLOR_CYAN;
extern TSMS_CR TSMS_COLOR_MAGENTA;

struct TSMS_COLOR {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

uint8_t TSMS_UTIL_reverseByte(uint8_t v);

uint32_t TSMS_UTIL_reverseData(uint32_t v, uint8_t bits);

uint8_t *TSMS_UTIL_streamAppend(uint8_t * stream, TSMS_SIZE size, uint8_t * data, TSMS_SIZE dataSize);

TSMS_BITS TSMS_UTIL_bits(uint8_t bits);

TSMS_CR TSMS_UTIL_color(uint8_t r, uint8_t g, uint8_t b);

uint16_t TSMS_UTIL_color565(TSMS_CR color);

uint32_t TSMS_UTIL_color888(TSMS_CR color);

void TSMS_UTIL_swap(uint16_t* a, uint16_t* b);

TSMS_CR TSMS_UTIL_gradientColor(TSMS_CR color1, TSMS_CR color2, float ratio);

#endif //TSMS_UTIL_H
