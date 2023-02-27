#ifndef TSMS_STRING_H
#define TSMS_STRING_H

struct TSMS_STRING_HANDLER;

typedef struct TSMS_STRING_HANDLER tString;
typedef tString* pString;

#include <math.h>
#include "tsms.h"
#include "tsms_util.h"

struct TSMS_STRING_HANDLER {
	char* cStr;
	uint32_t length;
	bool staticString;
};

extern pString TSMS_EMPTY_STRING;

bool TSMS_STRING_equals(pString str1, pString str2);

bool TSMS_STRING_startsWith(pString str, pString prefix);

pString TSMS_STRING_create();

pString TSMS_STRING_createAndInit(const char* cStr);

pString TSMS_STRING_static(const char* cStr);

TSMS_RESULT TSMS_STRING_release(pString str);

pString TSMS_STRING_subString(pString str, int start, int end);

TSMS_ULP TSMS_STRING_split(pString str, char spilt);

float TSMS_STRING_toFloat(pString str);

int TSMS_STRING_toInt(pString str);

TSMS_RESULT TSMS_STRING_getString(TSMS_UCLP list, pString buffer);

TSMS_RESULT TSMS_STRING_copy(pString str1, pString str2);

uint32_t TSMS_STRING_indexOf(pString str, char c);

#endif //TSMS_STRING_H
