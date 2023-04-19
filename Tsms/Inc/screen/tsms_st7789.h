#ifndef TSMS_ST7789_H
#define TSMS_ST7789_H

#define TSMS_ST7789_ID 0x8552
#include "tsms_display.h"

uint16_t TSMS_ST7789_readId(TSMS_SCHP screen);

TSMS_RESULT TSMS_ST7789_init(TSMS_SCHP screen, void* option);

TSMS_RESULT TSMS_ST7789_setDisplayDirection(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction);

TSMS_RESULT TSMS_ST7789_setScanDirection(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction);

TSMS_RESULT TSMS_ST7789_setCursor(TSMS_SCHP screen, uint16_t x, uint16_t y);

#endif //TSMS_ST7789_H
