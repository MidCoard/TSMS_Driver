#ifndef TSMS_SSD1963_H
#define TSMS_SSD1963_H

#include "tsms_def.h"
#define TSMS_SSD1963_ID 0x5761

struct TSMS_SSD1963_OPTION {
	uint16_t horizontalResolution;
	uint16_t verticalResolution;
	uint16_t horizontalPulseWidth;
	uint16_t horizontalBackPorch;
	uint16_t horizontalFrontPorch;
	uint16_t verticalPulseWidth;
	uint16_t verticalBackPorch;
	uint16_t verticalFrontPorch;
};

extern TSMS_SSD1963_OPTION defaultSSD1963Option;

#include "tsms_display.h"

uint16_t TSMS_SSD1963_horizontalTotal(TSMS_SSD1963_OPTION option);

uint16_t TSMS_SSD1963_horizontalPulseStart(TSMS_SSD1963_OPTION option);

uint16_t TSMS_SSD1963_verticalTotal(TSMS_SSD1963_OPTION option);

uint16_t TSMS_SSD1963_verticalPulseStart(TSMS_SSD1963_OPTION option);

TSMS_RESULT TSMS_SSD1963_init(TSMS_SCHP screen, void* option);

uint16_t TSMS_SSD1963_readId(TSMS_SCHP screen);

TSMS_RESULT TSMS_SSD1963_setDisplayDirection(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction);

TSMS_RESULT TSMS_SSD1963_setScanDirection(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction);

TSMS_RESULT TSMS_SSD1963_setCursor(TSMS_SCHP screen, uint16_t x, uint16_t y);

#endif //TSMS_SSD1963_H
