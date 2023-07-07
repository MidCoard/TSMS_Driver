#ifndef TSMS_STRING_H
#define TSMS_STRING_H

typedef struct TSMS_STRING tString;
typedef tString *pString;

#include "tsms_def.h"
#include "tsms_char_list.h"
#include "tsms_list.h"

struct TSMS_STRING {
	char *cStr;
	TSMS_SIZE length;
	bool staticString;
};

extern pString TSMS_EMPTY_STRING;

bool TSMS_STRING_equals(pString str1, pString str2);

bool TSMS_STRING_equalsIgnoreCase(pString str1, pString str2);

long TSMS_STRING_compare(pString str1, pString str2);

bool TSMS_STRING_startsWith(pString str, pString prefix);

pString TSMS_STRING_create();

pString TSMS_STRING_createWithString(const char *cStr);

pString TSMS_STRING_static(const char *cStr);

TSMS_RESULT TSMS_STRING_releaseStatic();

TSMS_RESULT TSMS_STRING_release(pString str);

pString TSMS_STRING_subString(pString str, TSMS_POS start, TSMS_POS end);

TSMS_LP TSMS_STRING_split(pString str, char split);

TSMS_LP TSMS_STRING_splitByString(pString str, pString split);

bool TSMS_STRING_isNumber(pString str);

double TSMS_STRING_toDouble(pString str);

int TSMS_STRING_toInt(pString str);

TSMS_RESULT TSMS_STRING_getString(TSMS_CLP list, pString buffer);

TSMS_RESULT TSMS_STRING_copy(pString str1, pString str2);

long TSMS_STRING_indexOf(pString str, char c);

pString TSMS_STRING_createWithChar(char c);

pString TSMS_STRING_createWithBytes(const uint8_t * bytes);

pString TSMS_STRING_createWithFixSize(const uint8_t * bytes, TSMS_SIZE size);

TSMS_RESULT TSMS_STRING_append(pString str1, pString str2);

TSMS_RESULT TSMS_STRING_appendString(pString str1, const char * str2);

TSMS_RESULT TSMS_STRING_appendChar(pString str, char c);

TSMS_RESULT TSMS_STRING_appendBool(pString str, bool b);

TSMS_RESULT TSMS_STRING_clear(pString str);

tString TSMS_STRING_temp(const char *cStr);

pString TSMS_STRING_empty();

long TSMS_STRING_hash(pString str);

pString TSMS_STRING_createWithInt(int i);

TSMS_RESULT TSMS_STRING_set(pString str, char * data);

pString TSMS_STRING_clone(pString str);

pString TSMS_STRING_trim(pString str);

pString TSMS_STRING_escape(pString str);

pString TSMS_STRING_unescape(pString str);

#endif //TSMS_STRING_H
