#ifndef TSMS_ILI9341_H
#define TSMS_ILI9341_H

#define TSMS_ILI9341_ID 0x9341

#include "tsms_display.h"

uint16_t TSMS_ILI9341_readId(TSMS_SCHP screen);

TSMS_RESULT TSMS_ILI9341_init(TSMS_SCHP screen, void * option);

TSMS_RESULT TSMS_ILI9341_setDisplayDirection(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction);

TSMS_RESULT TSMS_ILI9341_setScanDirection(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction);

TSMS_RESULT TSMS_ILI9341_setCursor(TSMS_SCHP screen, uint16_t x, uint16_t y);

#endif //TSMS_ILI9341_H
