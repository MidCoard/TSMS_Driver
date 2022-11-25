#ifndef TSMS_STRING_H
#define TSMS_STRING_H

#include "tsms.h"
#include "tsms_util.h"

struct TSMS_STRING_HANDLER {
	char* cString;
	uint32_t length;
	bool staticString;
};

typedef struct TSMS_STRING_HANDLER tString;
typedef tString* pString;

bool TSMS_STRING_equals(pString str1, pString str2);

bool TSMS_STRING_startsWith(pString str, pString prefix);

pString TSMS_STRING_create();

pString TSMS_STRING_static(const char* cString);

TSMS_RESULT TSMS_STRING_release(pString str);

pString TSMS_STRING_subString(pString str, int start, int end);

TSMS_ULP TSMS_STRING_split(pString str, char spilt);

float TSMS_STRING_toFloat(pString str);

#endif //TSMS_STRING_H
