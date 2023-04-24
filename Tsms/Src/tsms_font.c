#include "tsms_font.h"
#include "font/tsms_array_font.h"

pString TSMS_FONT_ARRAY_DEFAULT_FONT;
pString TSMS_FONT_ARRAY_THICK_FONT;
pString TSMS_FONT_ARRAY_HOME_SPUN_FONT;
pString TSMS_FONT_ARRAY_SEVEN_SEG_FONT;
pString TSMS_FONT_ARRAY_WIDE_FONT;
pString TSMS_FONT_ARRAY_TINY_FONT;
pString TSMS_FONT_ARRAY_LARGE_FONT;
pString TSMS_FONT_ARRAY_HUGE_FONT;
pString TSMS_FONT_ARRAY_MEGA_FONT;

void TSMS_FONT_init() {
	TSMS_FONT_ARRAY_DEFAULT_FONT = TSMS_STRING_static("TSMS_FONT_ARRAY_DEFAULT_FONT");
	TSMS_FONT_ARRAY_THICK_FONT = TSMS_STRING_static("TSMS_FONT_ARRAY_THICK_FONT");
	TSMS_FONT_ARRAY_HOME_SPUN_FONT = TSMS_STRING_static("TSMS_FONT_ARRAY_HOME_SPUN_FONT");
	TSMS_FONT_ARRAY_SEVEN_SEG_FONT = TSMS_STRING_static("TSMS_FONT_ARRAY_SEVEN_SEG_FONT");
	TSMS_FONT_ARRAY_WIDE_FONT = TSMS_STRING_static("TSMS_FONT_ARRAY_WIDE_FONT");
	TSMS_FONT_ARRAY_TINY_FONT = TSMS_STRING_static("TSMS_FONT_ARRAY_TINY_FONT");
	TSMS_FONT_ARRAY_LARGE_FONT = TSMS_STRING_static("TSMS_FONT_ARRAY_LARGE_FONT");
	TSMS_FONT_ARRAY_HUGE_FONT = TSMS_STRING_static("TSMS_FONT_ARRAY_HUGE_FONT");
	TSMS_FONT_ARRAY_MEGA_FONT = TSMS_STRING_static("TSMS_FONT_ARRAY_MEGA_FONT");
}

TSMS_FONT_DATA TSMS_FONT_resolve(TSMS_FONT_TYPE type, void* font, uint8_t c) {
	void * data = TSMS_NULL;
	uint8_t width = 0;
	uint8_t height = 0;
	TSMS_SIZE bits = 0;
	if (type == TSMS_FONT_TYPE_ARRAY) {
		pString name = font;
		if (TSMS_STRING_equals(name, TSMS_FONT_ARRAY_DEFAULT_FONT)) {
			if (c - 32 >= 94)
				return (TSMS_FONT_DATA){TSMS_FONT_TYPE_INVALID, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_DEFAULT_FONT[c - 32];
			width = TSMS_FONT_DEFAULT_FONT_WIDTH;
			height = TSMS_FONT_DEFAULT_FONT_HEIGHT;
			bits = TSMS_BITS_8BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_ARRAY_THICK_FONT)) {
			if (c - 32 >= 59)
				return (TSMS_FONT_DATA){TSMS_FONT_TYPE_INVALID, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_THICK_FONT[c - 32];
			width = TSMS_FONT_THICK_FONT_WIDTH;
			height = TSMS_FONT_THICK_FONT_HEIGHT;
			bits = TSMS_BITS_8BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_ARRAY_HOME_SPUN_FONT)) {
			if (c - 32 >= 95)
				return (TSMS_FONT_DATA){TSMS_FONT_TYPE_INVALID, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_HOME_SPUN_FONT[c - 32];
			width = TSMS_FONT_HOME_SPUN_FONT_WIDTH;
			height = TSMS_FONT_HOME_SPUN_FONT_HEIGHT;
			bits = TSMS_BITS_8BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_ARRAY_SEVEN_SEG_FONT)) {
			if (c - 32 >= 92)
				return (TSMS_FONT_DATA){TSMS_FONT_TYPE_INVALID, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_SEVEN_SEG_FONT[c - 32];
			width = TSMS_FONT_SEVEN_SEG_FONT_WIDTH;
			height = TSMS_FONT_SEVEN_SEG_FONT_HEIGHT;
			bits = TSMS_BITS_8BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_ARRAY_WIDE_FONT)) {
			if (c - 32 >= 59)
				return (TSMS_FONT_DATA){TSMS_FONT_TYPE_INVALID, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_WIDE_FONT[c - 32];
			width = TSMS_FONT_WIDE_FONT_WIDTH;
			height = TSMS_FONT_WIDE_FONT_HEIGHT;
			bits = TSMS_BITS_8BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_ARRAY_TINY_FONT)) {
			if (c - 32 >= 95)
				return (TSMS_FONT_DATA){TSMS_FONT_TYPE_INVALID, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_TINY_FONT[c - 32];
			width = TSMS_FONT_TINY_FONT_WIDTH;
			height = TSMS_FONT_TINY_FONT_HEIGHT;
			bits = TSMS_BITS_8BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_ARRAY_LARGE_FONT)) {
			if (c - 32 >= 59)
				return (TSMS_FONT_DATA){TSMS_FONT_TYPE_INVALID, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_LARGE_FONT[c - 32];
			width = TSMS_FONT_LARGE_FONT_WIDTH;
			height = TSMS_FONT_LARGE_FONT_HEIGHT;
			bits = TSMS_BITS_16BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_ARRAY_HUGE_FONT)) {
			if (c - '.' >= 13)
				return (TSMS_FONT_DATA){TSMS_FONT_TYPE_INVALID, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_HUGE_FONT[c - '.'];
			width = TSMS_FONT_HUGE_FONT_WIDTH;
			height = TSMS_FONT_HUGE_FONT_HEIGHT;
			bits = TSMS_BITS_24BIT;
		} else if (TSMS_STRING_equals(name, TSMS_FONT_ARRAY_MEGA_FONT)) {
			if (c - '.' >= 13)
				return (TSMS_FONT_DATA) {TSMS_FONT_TYPE_INVALID, TSMS_NULL, 0, 0, 0};
			data = TSMS_FONT_MEGA_FONT[c - '.'];
			width = TSMS_FONT_MEGA_FONT_WIDTH;
			height = TSMS_FONT_MEGA_FONT_HEIGHT;
			bits = TSMS_BITS_32BIT;
		} else return (TSMS_FONT_DATA){TSMS_FONT_TYPE_INVALID, TSMS_NULL, 0, 0, 0};
	} else if (type == TSMS_FONT_TYPE_FILE) {

	}
	return (TSMS_FONT_DATA){type, data, width, height, bits};
}

bool TSMS_FONT_pixel(TSMS_FONT_DATA font, uint8_t x, uint8_t y) {
	if (font.type == TSMS_FONT_TYPE_ARRAY) {
		void * data = font.data;
		switch (font.bits) {
			case TSMS_BITS_8BIT:
				return ((uint8_t*)data)[x] & (1 << y);
			case TSMS_BITS_16BIT:
				return ((uint16_t*)data)[x] & (1 << y);
			case TSMS_BITS_24BIT:
			case TSMS_BITS_32BIT:
				return ((uint32_t*)data)[x] & (1 << y);
		}
	} else if (font.type == TSMS_FONT_TYPE_FILE) {

	}
	return false;
}