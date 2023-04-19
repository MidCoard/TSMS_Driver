#ifndef TSMS_ILI9341_H
#define TSMS_ILI9341_H

#define TSMS_ILI9341_ID 0x9341

#include "tsms_display.h"

uint16_t TSMS_ILI9341_readId(TSMS_SCHP screen);
TSMS_RESULT TSMS_ILI9341_init(TSMS_SCHP screen, void * op);

#endif //TSMS_ILI9341_H
