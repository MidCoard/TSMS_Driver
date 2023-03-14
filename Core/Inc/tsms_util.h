#ifndef TSMS_UTIL_H
#define TSMS_UTIL_H


#include "tsms.h"

uint8_t TSMS_UTIL_reverseByte(uint8_t v);

uint32_t TSMS_UTIL_reverseData(uint32_t v, uint8_t bits);

uint8_t *TSMS_UTIL_streamAppend(uint8_t * stream, TSMS_SIZE size, uint8_t * data, TSMS_SIZE dataSize);

#endif //TSMS_UTIL_H
