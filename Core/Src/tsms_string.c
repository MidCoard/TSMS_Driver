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
	str->cString = TSMS_NULL;
	str->length = 0;
	return str;
}

