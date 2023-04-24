#ifndef TSMS_FONT_H
#define TSMS_FONT_H

typedef struct TSMS_FONT_DATA TSMS_FONT_DATA;

typedef enum {
	TSMS_FONT_TYPE_INVALID = 0, TSMS_FONT_TYPE_ARRAY, TSMS_FONT_TYPE_FILE
} TSMS_FONT_TYPE;

#include "tsms_def.h"

struct TSMS_FONT_DATA {
	TSMS_FONT_TYPE type;
	void * data;
	uint8_t width;
	uint8_t height;
	TSMS_BITS bits;
};

extern pString TSMS_FONT_defaultFont;
extern pString TSMS_FONT_thickFont;
extern pString TSMS_FONT_homeSpunFont;
extern pString TSMS_FONT_sevenSegFont;
extern pString TSMS_FONT_wideFont;
extern pString TSMS_FONT_tinyFont;
extern pString TSMS_FONT_largeFont;
extern pString TSMS_FONT_hugeFont;
extern pString TSMS_FONT_megaFont;

TSMS_FONT_DATA TSMS_FONT_resolve(TSMS_FONT_TYPE type, void* font, uint8_t c);

bool TSMS_FONT_pixel(TSMS_FONT_DATA font, uint8_t x, uint8_t y);

#endif //TSMS_FONT_H