#ifndef TSMS_UTIL_H
#define TSMS_UTIL_H

#include "tsms.h"

typedef struct TSMS_COLOR TSMS_CP;

struct TSMS_COLOR {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

extern TSMS_CP TSMS_COLOR_BLACK;
extern TSMS_CP TSMS_COLOR_WHITE;
extern TSMS_CP TSMS_COLOR_RED;
extern TSMS_CP TSMS_COLOR_GREEN;
extern TSMS_CP TSMS_COLOR_BLUE;
extern TSMS_CP TSMS_COLOR_YELLOW;
extern TSMS_CP TSMS_COLOR_CYAN;
extern TSMS_CP TSMS_COLOR_MAGENTA;

uint8_t TSMS_UTIL_reverseByte(uint8_t v);

uint32_t TSMS_UTIL_reverseData(uint32_t v, uint8_t bits);

uint8_t *TSMS_UTIL_streamAppend(uint8_t * stream, TSMS_SIZE size, uint8_t * data, TSMS_SIZE dataSize);

TSMS_BITS TSMS_UTIL_bits(uint8_t bits);

TSMS_CP TSMS_UTIL_color(uint8_t r, uint8_t g, uint8_t b);

uint16_t TSMS_UTIL_color565(TSMS_CP color);

uint32_t TSMS_UTIL_color888(TSMS_CP color);

#endif //TSMS_UTIL_H
