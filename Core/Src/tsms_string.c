#include "tsms_string.h"

pString TSMS_EMPTY_STRING;

pString TSMS_FAIL_STRING;

bool TSMS_STRING_equals(pString str1, pString str2) {
	if (str1 == TSMS_NULL && str2 == TSMS_NULL)
		return true;
	if (str1 == TSMS_NULL || str2 == TSMS_NULL)
		return false;
	if (str1->length != str2->length)
		return false;
	for (uint32_t i = 0; i < str1->length; i++)
		if (str1->cStr[i] != str2->cStr[i])
			return false;
	return true;
}

bool TSMS_STRING_startsWith(pString str, pString prefix) {
	if (str == TSMS_NULL || prefix == TSMS_NULL)
		return false;
	if (str->length < prefix->length)
		return false;
	for (uint32_t i = 0; i < prefix->length; i++)
		if (str->cStr[i] != prefix->cStr[i])
			return false;
	return true;
}

pString TSMS_STRING_create() {
	pString str = malloc(sizeof(tString));
	if (str == TSMS_NULL) {
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, TSMS_FAIL_STRING);
		return TSMS_NULL;
	}
	str->staticString = false;
	str->cStr = TSMS_NULL;
	str->length = 0;
	return str;
}

pString TSMS_STRING_createAndInit(const char* cStr) {
	pString str = TSMS_STRING_create();
	if (str == TSMS_NULL)
		return TSMS_NULL;
	str->cStr = malloc(strlen(cStr) + 1);
	if (str->cStr == TSMS_NULL) {
		TSMS_STRING_release(str);
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, TSMS_STRING_static("malloc failed for cStr"));
		return TSMS_NULL;
	}
	strcpy(str->cStr, cStr);
	str->length = strlen(cStr);
	str->staticString = false;
	return str;
}

pString TSMS_STRING_static(const char* cStr) {
	pString str = TSMS_STRING_create();
	if (str == TSMS_NULL)
		return TSMS_NULL;
	str->staticString = true;
	str->cStr = cStr;
	str->length = strlen(cStr);
	return str;
}

TSMS_RESULT TSMS_STRING_release(pString str) {
	if (str == TSMS_NULL)
		return TSMS_ERROR;
	if (!str->staticString)
		free(str->cStr);
	free(str);
	return TSMS_SUCCESS;
}

pString TSMS_STRING_subString(pString str, int start, int end) {
	if (str == TSMS_NULL)
		return TSMS_NULL;
	if (start < 0 || end < 0 || start > end || end > str->length)
		return TSMS_NULL;
	pString sub = TSMS_STRING_create();
	if (sub == TSMS_NULL)
		return TSMS_NULL;
	sub->cStr = malloc(end - start + 1);
	if (sub->cStr == TSMS_NULL) {
		TSMS_STRING_release(sub);
		return TSMS_NULL;
	}
	for (int i = start; i < end; i++)
		sub->cStr[i - start] = str->cStr[i];
	sub->cStr[end - start] = '\0';
	sub->length = end - start;
	return sub;
}

TSMS_ULP TSMS_STRING_split(pString str, char spilt) {
	if (str == TSMS_NULL)
		return TSMS_NULL;
	TSMS_ULP ulp = TSMS_UTIL_createList(10);
	if (ulp == TSMS_NULL)
		return TSMS_NULL;
	int pos = 0;
	for (int i = 0; i < str->length; i++)
		if (str->cStr[i] == spilt) {
			TSMS_UTIL_addList(ulp, TSMS_STRING_subString(str, pos, i));
			pos = i + 1;
		}
	if (pos < str->length)
		TSMS_UTIL_addList(ulp, TSMS_STRING_subString(str, pos, str->length));
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


TSMS_RESULT TSMS_STRING_getString(TSMS_UCLP list, pString str) {
	if (str->staticString)
		return TSMS_ERROR;
	free(str->cStr);
	str->cStr = malloc(list->length + 1);
	if (str->cStr == TSMS_NULL)
		return TSMS_ERROR;
	for (uint32_t i = 0; i < list->length; i++)
		str->cStr[i] = list->list[i];
	str->cStr[list->length] = '\0';
	str->length = list->length;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_STRING_copy(pString str1, pString str2) {
	if (str2->staticString)
		return TSMS_ERROR;
	str2->cStr = realloc(str2->cStr, str1->length + 1);
	if (str2->cStr == TSMS_NULL)
		return TSMS_ERROR;
	for (uint32_t i = 0; i < str1->length; i++)
		str2->cStr[i] = str1->cStr[i];
	str2->cStr[str1->length] = '\0';
	str2->length = str1->length;
	return TSMS_SUCCESS;
}

uint32_t TSMS_STRING_indexOf(pString str, char c) {
	if (str == TSMS_NULL)
		return -1;
	for (uint32_t i = 0; i < str->length; i++)
		if (str->cStr[i] == c)
			return i;
	return -1;
}

TSMS_RESULT TSMS_STRING_init() {
	TSMS_EMPTY_STRING = TSMS_STRING_static("");
	TSMS_FAIL_STRING = TSMS_STRING_static("malloc failed for string");
	return TSMS_SUCCESS;
}
