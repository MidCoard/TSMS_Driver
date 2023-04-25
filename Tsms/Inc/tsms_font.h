#ifndef TSMS_FONT_H
#define TSMS_FONT_H

typedef enum {
	TSMS_FONT_TYPE_INVALID = 0, TSMS_FONT_TYPE_ARRAY, TSMS_FONT_TYPE_FILE
} TSMS_FONT_TYPE;

typedef struct TSMS_FONT_DATA TSMS_FONT_DATA;

#include "tsms_def.h"

typedef uint8_t TSMS_FONT_SIZE;

struct TSMS_FONT_DATA {
	TSMS_FONT_TYPE type;
	void * data;
	uint8_t width;
	uint8_t height;
	TSMS_BITS bits;
};

extern pString TSMS_FONT_ARRAY_DEFAULT_FONT;
extern pString TSMS_FONT_ARRAY_THICK_FONT;
extern pString TSMS_FONT_ARRAY_HOME_SPUN_FONT;
extern pString TSMS_FONT_ARRAY_SEVEN_SEG_FONT;
extern pString TSMS_FONT_ARRAY_WIDE_FONT;
extern pString TSMS_FONT_ARRAY_TINY_FONT;
extern pString TSMS_FONT_ARRAY_LARGE_FONT;
extern pString TSMS_FONT_ARRAY_HUGE_FONT;
extern pString TSMS_FONT_ARRAY_MEGA_FONT;

TSMS_FONT_DATA TSMS_FONT_resolve(TSMS_FONT_TYPE type, void* font, uint8_t c);

bool TSMS_FONT_pixel(TSMS_FONT_DATA font, uint8_t x, uint8_t y);

#endif //TSMS_FONT_H