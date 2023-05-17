#ifndef TSMS_UTIL_H
#define TSMS_UTIL_H

typedef struct TSMS_COLOR TSMS_COLOR;

#include "tsms_def.h"

struct TSMS_COLOR {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

extern const TSMS_COLOR TSMS_COLOR_BLACK;
extern const TSMS_COLOR TSMS_COLOR_WHITE;
extern const TSMS_COLOR TSMS_COLOR_RED;
extern const TSMS_COLOR TSMS_COLOR_GREEN;
extern const TSMS_COLOR TSMS_COLOR_BLUE;
extern const TSMS_COLOR TSMS_COLOR_YELLOW;
extern const TSMS_COLOR TSMS_COLOR_CYAN;
extern const TSMS_COLOR TSMS_COLOR_MAGENTA;
extern const TSMS_COLOR TSMS_COLOR_GRAY;

uint8_t TSMS_UTIL_reverseByte(uint8_t v);

uint32_t TSMS_UTIL_reverseData(uint32_t v, uint8_t bits);

uint8_t *TSMS_UTIL_streamAppend(uint8_t * stream, TSMS_SIZE size, uint8_t * data, TSMS_SIZE dataSize);

TSMS_BITS TSMS_UTIL_bits(uint8_t bits);

TSMS_COLOR TSMS_UTIL_color(uint8_t r, uint8_t g, uint8_t b);

uint16_t TSMS_UTIL_color565(TSMS_COLOR color);

uint32_t TSMS_UTIL_color888(TSMS_COLOR color);

void TSMS_UTIL_swap(void** a, void** b);

void TSMS_UTIL_swapUnsignedShort(uint16_t* a, uint16_t* b);

void TSMS_UTIL_swapFloat(float * a, float * b);

TSMS_COLOR TSMS_UTIL_gradientColor(TSMS_COLOR color1, TSMS_COLOR color2, float ratio);

bool TSMS_UTIL_equalsColor(TSMS_COLOR color1, TSMS_COLOR color2);

#endif //TSMS_UTIL_H
