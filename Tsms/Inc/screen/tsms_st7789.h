#ifndef TSMS_ST7789_H
#define TSMS_ST7789_H

#define TSMS_ST7789_ID 0x8552
#include "tsms_display.h"

uint16_t TSMS_ST7789_readId(TSMS_SCHP screen);

TSMS_RESULT TSMS_ST7789_init(TSMS_SCHP screen, void* option);


#endif //TSMS_ST7789_H
