#ifndef TSMS_NT5510_H
#define TSMS_NT5510_H

#define TSMS_NT35510_ID 0x8000
#define TSMS_NT5510_ID 0x5510

#include "tsms_display.h"

uint16_t TSMS_NT5510_readId(TSMS_SCHP screen);

TSMS_RESULT TSMS_NT5510_init(TSMS_SCHP screen, void* option);

TSMS_RESULT TSMS_NT5510_setDisplayDirection(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction);

TSMS_RESULT TSMS_NT5510_setScanDirection(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction);

TSMS_RESULT TSMS_NT5510_setCursor(TSMS_SCHP screen, uint16_t x, uint16_t y);

#endif //TSMS_NT5510_H
