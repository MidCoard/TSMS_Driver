#ifndef TSMS_NT35310_H
#define TSMS_NT35310_H


#define TSMS_NT35310_ID 0x5310
#include "tsms_display.h"

uint16_t TSMS_NT35310_readId(TSMS_SCHP screen);

TSMS_RESULT TSMS_NT35310_init(TSMS_SCHP screen, void* option);

#endif //TSMS_NT35310_H
