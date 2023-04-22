#include "tsms_font.h"

pString TSMS_FONT_defaultFont;
pString TSMS_FONT_thickFont;
pString TSMS_FONT_homeSpunFont;
pString TSMS_FONT_sevenSegFont;
pString TSMS_FONT_wideFont;
pString TSMS_FONT_tinyFont;
pString TSMS_FONT_largeFont;
pString TSMS_FONT_hugeFont;
pString TSMS_FONT_megaFont;

void TSMS_FONT_init() {
	TSMS_FONT_defaultFont = TSMS_STRING_createAndInit("TSMS_FONT_defaultFont");
	TSMS_FONT_thickFont = TSMS_STRING_createAndInit("TSMS_FONT_thickFont");
	TSMS_FONT_homeSpunFont = TSMS_STRING_createAndInit("TSMS_FONT_homeSpunFont");
	TSMS_FONT_sevenSegFont = TSMS_STRING_createAndInit("TSMS_FONT_sevenSegFont");
	TSMS_FONT_wideFont = TSMS_STRING_createAndInit("TSMS_FONT_wideFont");
	TSMS_FONT_tinyFont = TSMS_STRING_createAndInit("TSMS_FONT_tinyFont");
	TSMS_FONT_largeFont = TSMS_STRING_createAndInit("TSMS_FONT_largeFont");
	TSMS_FONT_hugeFont = TSMS_STRING_createAndInit("TSMS_FONT_hugeFont");
	TSMS_FONT_megaFont = TSMS_STRING_createAndInit("TSMS_FONT_megaFont");
}

TSMS_FONT_DATA TSMS_FONT_resolve(TSMS_FONT_TYPE type, void* font, uint8_t c) {
	void * data = TSMS_NULL;
	uint8_t width = 0;
	uint8_t height = 0;
	TSMS_SIZE bits = 0;
	if (type == TSMS_ARRAY_FONT) {
		pString name = font;
		if (TSMS_STRING_equals(name, TSMS_FONT_defaultFont)) {
			if (c - 32 >= 94)
				return (TSMS_FONT_DATA){TSMS_INVALID_FONT, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_DEFAULT_FONT[c - 32];
			width = TSMS_FONT_DEFAULT_FONT_WIDTH;
			height = TSMS_FONT_DEFAULT_FONT_HEIGHT;
			bits = TSMS_8BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_thickFont)) {
			if (c - 32 >= 59)
				return (TSMS_FONT_DATA){TSMS_INVALID_FONT, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_THICK_FONT[c - 32];
			width = TSMS_FONT_THICK_FONT_WIDTH;
			height = TSMS_FONT_THICK_FONT_HEIGHT;
			bits = TSMS_8BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_homeSpunFont)) {
			if (c - 32 >= 95)
				return (TSMS_FONT_DATA){TSMS_INVALID_FONT, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_HOME_SPUN_FONT[c - 32];
			width = TSMS_FONT_HOME_SPUN_FONT_WIDTH;
			height = TSMS_FONT_HOME_SPUN_FONT_HEIGHT;
			bits = TSMS_8BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_sevenSegFont)) {
			if (c - 32 >= 92)
				return (TSMS_FONT_DATA){TSMS_INVALID_FONT, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_SEVEN_SEG_FONT[c - 32];
			width = TSMS_FONT_SEVEN_SEG_FONT_WIDTH;
			height = TSMS_FONT_SEVEN_SEG_FONT_HEIGHT;
			bits = TSMS_8BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_wideFont)) {
			if (c - 32 >= 59)
				return (TSMS_FONT_DATA){TSMS_INVALID_FONT, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_WIDE_FONT[c - 32];
			width = TSMS_FONT_WIDE_FONT_WIDTH;
			height = TSMS_FONT_WIDE_FONT_HEIGHT;
			bits = TSMS_8BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_tinyFont)) {
			if (c - 32 >= 95)
				return (TSMS_FONT_DATA){TSMS_INVALID_FONT, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_TINY_FONT[c - 32];
			width = TSMS_FONT_TINY_FONT_WIDTH;
			height = TSMS_FONT_TINY_FONT_HEIGHT;
			bits = TSMS_8BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_largeFont)) {
			if (c - 32 >= 59)
				return (TSMS_FONT_DATA){TSMS_INVALID_FONT, TSMS_NULL, 0,0,0};
			data = TSMS_FONT_LARGE_FONT[c - 32];
			width = TSMS_FONT_LARGE_FONT_WIDTH;
			height = TSMS_FONT_LARGE_FONT_HEIGHT;
			bits = TSMS_16BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_hugeFont)) {
			if (c - '.' >= 13)
				return (TSMS_FONT_DATA){TSMS_INVALID_FONT, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_HUGE_FONT[c - '.'];
			width = TSMS_FONT_HUGE_FONT_WIDTH;
			height = TSMS_FONT_HUGE_FONT_HEIGHT;
			bits = TSMS_24BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_megaFont)) {
			if (c - '.' >= 13)
				return (TSMS_FONT_DATA) {TSMS_INVALID_FONT, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_MEGA_FONT[c - '.'];
			width = TSMS_FONT_MEGA_FONT_WIDTH;
			height = TSMS_FONT_MEGA_FONT_HEIGHT;
			bits = TSMS_32BIT;
		} else return (TSMS_FONT_DATA){TSMS_INVALID_FONT, TSMS_NULL, 0, 0, 0};
	} else if (type == TSMS_FILE_FONT) {

	}
	return (TSMS_FONT_DATA){type, data, width, height, bits};
}

bool TSMS_FONT_pixel(TSMS_FONT_DATA font, uint8_t x, uint8_t y) {
	if (font.type == TSMS_ARRAY_FONT) {
		void * data = font.data;
		switch (font.bits) {
			case TSMS_8BIT:
				return ((uint8_t*)data)[x] & (1 << y);
			case TSMS_16BIT:
				return ((uint16_t*)data)[x] & (1 << y);
			case TSMS_24BIT:
			case TSMS_32BIT:
				return ((uint32_t*)data)[x] & (1 << y);
		}
	} else if (font.type == TSMS_FILE_FONT) {

	}
	return false;
}