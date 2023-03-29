#include "tsms_string.h"

pString TSMS_EMPTY_STRING;

TSMS_MP staticMap;

TSMS_INLINE long __internal_tsms_hash(void* p) {
	return TSMS_STRING_hash(p);
}

TSMS_INLINE bool __internal_tsms_compare(void * key1, void * key2) {
	return TSMS_STRING_equals(key1, key2);
}

bool TSMS_STRING_equals(pString str1, pString str2) {
	if (str1 == TSMS_NULL && str2 == TSMS_NULL)
		return true;
	if (str1 == TSMS_NULL || str2 == TSMS_NULL)
		return false;
	if (str1->length != str2->length)
		return false;
	for (TSMS_POS i = 0; i < str1->length; i++)
		if (str1->cStr[i] != str2->cStr[i])
			return false;
	return true;
}

bool TSMS_STRING_startsWith(pString str, pString prefix) {
	if (str == TSMS_NULL || prefix == TSMS_NULL)
		return false;
	if (str->length < prefix->length)
		return false;
	for (TSMS_POS i = 0; i < prefix->length; i++)
		if (str->cStr[i] != prefix->cStr[i])
			return false;
	return true;
}

pString TSMS_STRING_create() {
	pString str = malloc(sizeof(tString));
	if (str == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for tString");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	str->staticString = false;
	str->cStr = malloc(1);
	if (str->cStr == TSMS_NULL) {
		TSMS_STRING_release(str);
		tString temp = TSMS_STRING_temp("malloc failed for cStr");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	str->cStr[0] = '\0';
	str->length = 0;
	return str;
}

pString TSMS_STRING_createAndInit(const char *cStr) {
	pString str = TSMS_STRING_create();
	if (str == TSMS_NULL)
		return TSMS_NULL;
	if (cStr == TSMS_NULL)
		return str;
	str->cStr = realloc(str->cStr, strlen(cStr) + 1);
	if (str->cStr == TSMS_NULL) {
		TSMS_STRING_release(str);
		tString temp = TSMS_STRING_temp("realloc failed for cStr");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	strcpy(str->cStr, cStr);
	str->length = strlen(cStr);
	str->staticString = false;
	return str;
}

pString TSMS_STRING_static(const char *cStr) {
	pString tmp = TSMS_MAP_get(staticMap, cStr);
	if (tmp != TSMS_NULL)
		return tmp;
	pString str = TSMS_STRING_create();
	if (str == TSMS_NULL)
		return TSMS_NULL;
	free(str->cStr);
	str->staticString = true;
	str->cStr = cStr;
	str->length = strlen(cStr);
	TSMS_MAP_put(staticMap, cStr, str);
	return str;
}

TSMS_RESULT TSMS_STRING_release(pString str) {
	if (str == TSMS_NULL)
		return TSMS_ERROR;
	if (str->staticString)
		return TSMS_ERROR;
	free(str->cStr);
	free(str);
	return TSMS_SUCCESS;
}

pString TSMS_STRING_subString(pString str, TSMS_POS start, TSMS_POS end) {
	if (str == TSMS_NULL)
		return TSMS_NULL;
	if (start < 0 || end < 0 || start > end || end > str->length)
		return TSMS_NULL;
	pString sub = TSMS_STRING_create();
	if (sub == TSMS_NULL)
		return TSMS_NULL;
	sub->cStr = realloc(sub->cStr, end - start + 1);
	if (sub->cStr == TSMS_NULL) {
		TSMS_STRING_release(sub);
		return TSMS_NULL;
	}
	for (TSMS_POS i = start; i < end; i++)
		sub->cStr[i - start] = str->cStr[i];
	sub->cStr[end - start] = '\0';
	sub->length = end - start;
	return sub;
}

TSMS_LP TSMS_STRING_split(pString str, char spilt) {
	if (str == TSMS_NULL)
		return TSMS_EMPTY_LIST;
	TSMS_LP ulp = TSMS_LIST_create(10);
	if (ulp == TSMS_NULL)
		return TSMS_EMPTY_LIST;
	int pos = 0;
	for (int i = 0; i < str->length; i++)
		if (str->cStr[i] == spilt) {
			TSMS_LIST_add(ulp, TSMS_STRING_subString(str, pos, i));
			pos = i + 1;
		}
	if (pos < str->length)
		TSMS_LIST_add(ulp, TSMS_STRING_subString(str, pos, str->length));
	return ulp;
}

float TSMS_STRING_toFloat(pString str) {
	if (str == TSMS_NULL)
		return NAN;
	return atof(str->cStr);
}

int TSMS_STRING_toInt(pString str) {
	if (str == TSMS_NULL)
		return 0;
	return atoi(str->cStr);
}


TSMS_RESULT TSMS_STRING_getString(TSMS_CLP list, pString str) {
	if (str->staticString)
		return TSMS_ERROR;
	free(str->cStr);
	str->cStr = malloc(list->length + 1);
	if (str->cStr == TSMS_NULL)
		return TSMS_ERROR;
	for (TSMS_POS i = 0; i < list->length; i++)
		str->cStr[i] = list->list[i];
	str->cStr[list->length] = '\0';
	str->length = list->length;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_STRING_copy(pString str1, pString str2) {
	if (str2->staticString)
		return TSMS_ERROR;
	char* tmp = realloc(str2->cStr, str1->length + 1);
	if (tmp == TSMS_NULL)
		return TSMS_ERROR;
	str2->cStr = tmp;
	for (TSMS_POS i = 0; i < str1->length; i++)
		str2->cStr[i] = str1->cStr[i];
	str2->cStr[str1->length] = '\0';
	str2->length = str1->length;
	return TSMS_SUCCESS;
}

long TSMS_STRING_indexOf(pString str, char c) {
	if (str == TSMS_NULL)
		return -1;
	for (TSMS_POS i = 0; i < str->length; i++)
		if (str->cStr[i] == c)
			return i;
	return -1;
}

TSMS_RESULT TSMS_STRING_init() {
	staticMap = TSMS_MAP_create(256, __internal_tsms_hash, __internal_tsms_compare);
	TSMS_EMPTY_STRING = TSMS_STRING_static("");
	if (staticMap == TSMS_NULL || TSMS_EMPTY_STRING == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_SUCCESS;
}

pString TSMS_STRING_createAndInitChar(char c) {
	pString str = TSMS_STRING_create();
	if (str == TSMS_NULL)
		return TSMS_NULL;
	str->cStr = realloc(str->cStr, sizeof (char) * 2);
	if (str->cStr == TSMS_NULL) {
		TSMS_STRING_release(str);
		tString temp = TSMS_STRING_temp("malloc failed for cStr");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	str->cStr[0] = c;
	str->cStr[1] = '\0';
	str->length = 1;
	str->staticString = false;
	return str;
}

pString TSMS_STRING_createAndInitBytes(const uint8_t * bytes) {
	pString str = TSMS_STRING_create();
	if (str == TSMS_NULL)
		return TSMS_NULL;
	if (bytes == TSMS_NULL)
		return str;
	for (int i = 0; bytes[i] != 0; i++)
		str->length++;
	str->cStr = realloc(str->cStr, sizeof (char) * (str->length + 1));
	if (str->cStr == TSMS_NULL) {
		TSMS_STRING_release(str);
		tString temp = TSMS_STRING_temp("malloc failed for cStr");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	for (int i = 0; i < str->length; i++)
		str->cStr[i] = bytes[i];
	str->cStr[str->length] = '\0';
	str->staticString = false;
	return str;
}

TSMS_RESULT TSMS_STRING_append(pString str1, pString str2) {
	if (str1 == TSMS_NULL || str2 == TSMS_NULL)
		return TSMS_ERROR;
	if (str1->staticString)
		return TSMS_ERROR;
	char* tmp = realloc(str1->cStr, str1->length + str2->length + 1);
	if (tmp == TSMS_NULL)
		return TSMS_ERROR;
	str1->cStr = tmp;
	for (TSMS_POS i = 0; i < str2->length; i++)
		str1->cStr[str1->length + i] = str2->cStr[i];
	str1->cStr[str1->length + str2->length] = '\0';
	str1->length += str2->length;
	return TSMS_SUCCESS;
}

tString TSMS_STRING_temp(const char* str) {
	tString tStr = {str, strlen(str), false};
	return tStr;
}

pString TSMS_STRING_empty() {
	return TSMS_STRING_createAndInit("");
}

long TSMS_STRING_hash(pString str) {
	if (str == TSMS_NULL)
		return 0;
	char* cStr = str->cStr;
	size_t len = strlen(cStr);
	long hash = 0;
	for (size_t i = 0; i < len; i++)
		hash = hash * 31 + cStr[i];
	return hash;
}
