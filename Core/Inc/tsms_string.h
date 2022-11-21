#ifndef TSMS_STRING_H
#define TSMS_STRING_H

#include "tsms.h"

struct TSMS_STRING_HANDLER {
	char* cString;
	uint32_t length;
};

typedef struct TSMS_STRING_HANDLER tString;
typedef tString* pString;

bool TSMS_STRING_equals(pString str1, pString str2);

bool TSMS_STRING_startsWith(pString str, pString prefix);

pString TSMS_STRING_create();

#endif //TSMS_STRING_H
