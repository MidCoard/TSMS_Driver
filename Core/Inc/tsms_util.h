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


struct TSMS_UTIL_CHAR_LIST {
	char* list;
	uint32_t length;
	uint32_t actualLength;
	uint32_t initLength;
};

typedef struct TSMS_UTIL_CHAR_LIST* TSMS_UTIL_CHAR_LIST_POINTER;
typedef TSMS_UTIL_CHAR_LIST_POINTER TSMS_UCLP;

TSMS_ULP TSMS_UTIL_createList(int initSize);

TSMS_UCLP TSMS_UTIL_createCharList(int initSize);

TSMS_RESULT TSMS_UTIL_addList(TSMS_ULP list, void* element);
TSMS_RESULT TSMS_UTIL_addCharList(TSMS_UCLP list, char element);

TSMS_RESULT TSMS_UTIL_removeList(TSMS_ULP list, uint32_t index);

TSMS_RESULT TSMS_UTIL_removeListElement(TSMS_ULP list, void* element);

uint8_t TSMS_UTIL_reverseByte(uint8_t v);

uint32_t TSMS_UTIL_reverseData(uint32_t v, uint8_t bits);

TSMS_RESULT TSMS_UTIL_releaseList(TSMS_ULP list);

TSMS_RESULT TSMS_UTIL_clearCharList(TSMS_UCLP list);

bool TSMS_UTIL_startsWith(const char* str, const char* prefix);

#endif //TSMS_UTIL_H
