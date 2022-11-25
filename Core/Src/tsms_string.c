#include "tsms_string.h"

bool TSMS_STRING_equals(pString str1, pString str2) {
	if (str1->length != str2->length)
		return false;
	for (uint32_t i = 0; i < str1->length; i++)
		if (str1->cString[i] != str2->cString[i])
			return false;
	return true;
}


bool TSMS_STRING_startsWith(pString str, pString prefix) {
	if (str->length < prefix->length)
		return false;
	for (uint32_t i = 0; i < prefix->length; i++)
		if (str->cString[i] != prefix->cString[i])
			return false;
	return true;
}

pString TSMS_STRING_create() {
	pString str = malloc(sizeof(tString));
	if (str == TSMS_NULL)
		return TSMS_NULL;
	str->staticString = false;
	str->cString = TSMS_NULL;
	str->length = 0;
	return str;
}

pString TSMS_STRING_static(const char* cString) {
	pString str = malloc(sizeof(tString));
	if (str == TSMS_NULL)
		return TSMS_NULL;
	str->staticString = true;
	str->cString = cString;
	str->length = strlen(cString);
	return str;
}

TSMS_RESULT TSMS_STRING_release(pString str) {
	if (str == TSMS_NULL)
		return TSMS_SUCCESS;
	if (!str->staticString)
		free(str->cString);
	free(str);
	return TSMS_SUCCESS;
}

pString TSMS_STRING_subString(pString str, int start, int end) {
	if (start < 0 || end < 0 || start > end || end > str->length)
		return TSMS_NULL;
	pString sub = TSMS_STRING_create();
	if (sub == TSMS_NULL)
		return TSMS_NULL;
	sub->cString = malloc(end - start + 1);
	if (sub->cString == TSMS_NULL) {
		TSMS_STRING_release(sub);
		return TSMS_NULL;
	}
	for (int i = start; i < end; i++)
		sub->cString[i - start] = str->cString[i];
	sub->cString[end - start] = '\0';
	sub->length = end - start;
	return sub;
}

TSMS_ULP TSMS_STRING_split(pString str, char spilt) {
	TSMS_ULP ulp = TSMS_UTIL_createList(10);
	if (ulp == TSMS_NULL)
		return TSMS_NULL;
	int pos = 0;
	for (int i = 0; i < str->length; i++)
		if (str->cString[i] == spilt) {
			TSMS_UTIL_addList(ulp, TSMS_STRING_subString(str, pos, i));
			pos = i + 1;
		}
	return ulp;
}

float TSMS_STRING_toFloat(pString str) {
	return atof(str->cString);
}


TSMS_RESULT TSMS_STRING_getString(TSMS_UCLP list, pString str) {
	if (str->staticString)
		return TSMS_ERROR;
	free(str->cString);
	str->cString = malloc(list->length + 1);
	if (str->cString == TSMS_NULL)
		return TSMS_FAIL;
	for (uint32_t i = 0; i < list->length; i++)
		str->cString[i] = list->list[i];
	str->cString[list->length] = '\0';
	str->length = list->length;
	return TSMS_SUCCESS;
}
