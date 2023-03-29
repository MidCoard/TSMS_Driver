#ifndef TSMS_STRING_H
#define TSMS_STRING_H

struct TSMS_STRING_HANDLER;

typedef struct TSMS_STRING_HANDLER tString;
typedef tString *pString;

#include <math.h>
#include "tsms.h"
#include "tsms_list.h"
#include "tsms_char_list.h"
#include "tsms_set.h"

struct TSMS_STRING_HANDLER {
	char *cStr;
	TSMS_SIZE length;
	bool staticString;
};

extern pString TSMS_EMPTY_STRING;

bool TSMS_STRING_equals(pString str1, pString str2);

long TSMS_STRING_compare(pString str1, pString str2);

bool TSMS_STRING_startsWith(pString str, pString prefix);

pString TSMS_STRING_create();

pString TSMS_STRING_createAndInit(const char *cStr);

pString TSMS_STRING_static(const char *cStr);

TSMS_RESULT TSMS_STRING_release(pString str);

pString TSMS_STRING_subString(pString str, TSMS_POS start, TSMS_POS end);

TSMS_LP TSMS_STRING_split(pString str, char spilt);

float TSMS_STRING_toFloat(pString str);

int TSMS_STRING_toInt(pString str);

TSMS_RESULT TSMS_STRING_getString(TSMS_CLP list, pString buffer);

TSMS_RESULT TSMS_STRING_copy(pString str1, pString str2);

long TSMS_STRING_indexOf(pString str, char c);

pString TSMS_STRING_createAndInitChar(char c);

pString TSMS_STRING_createAndInitBytes(const uint8_t * bytes);

TSMS_RESULT TSMS_STRING_append(pString str1, pString str2);

tString TSMS_STRING_temp(const char *cStr);

pString TSMS_STRING_empty();

long TSMS_STRING_hash(pString str);

#endif //TSMS_STRING_H
