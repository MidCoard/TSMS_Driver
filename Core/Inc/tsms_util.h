#ifndef TSMS_UTIL_H
#define TSMS_UTIL_H

#include "tsms.h"

struct TSMS_UTIL_LIST {
	void** list;
	uint32_t length;
	uint32_t actualLength;
	uint32_t initLength;
};

typedef struct TSMS_UTIL_LIST* TSMS_UTIL_LIST_POINTER;
typedef TSMS_UTIL_LIST_POINTER TSMS_ULP;

uint32_t * TSMS_UTIL_gen32BitPointer(uint32_t v);

TSMS_ULP TSMS_UTIL_createList(int initSize);

TSMS_RESULT TSMS_UTIL_addList(TSMS_ULP list, void* element);

TSMS_RESULT TSMS_UTIL_removeList(TSMS_ULP list, uint32_t index);

TSMS_RESULT TSMS_UTIL_removeListElement(TSMS_ULP list, void* element);

uint8_t TSMS_UTIL_reverseByte(uint8_t v);

TSMS_RESULT TSMS_UTIL_releaseList(TSMS_ULP list);

#endif //TSMS_UTIL_H
