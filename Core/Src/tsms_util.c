#include "tsms_util.h"

TSMS_ULP TSMS_UTIL_createList(int initSize) {
	TSMS_ULP list = malloc(sizeof(struct TSMS_UTIL_LIST));
	if (list == TSMS_NULL) {
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, TSMS_STRING_static("malloc failed for TSMS_ULP"));
		return TSMS_NULL;
	}
	list->list = malloc(initSize * sizeof(void *));
	if (list->list == TSMS_NULL) {
		free(list);
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, TSMS_STRING_static("malloc failed for list"));
		return TSMS_NULL;
	}
	list->actualLength = initSize;
	list->length = 0;
	list->initLength = initSize;
	return list;
}

TSMS_UCLP TSMS_UTIL_createCharList(int initSize) {
	TSMS_UCLP list = malloc(sizeof(struct TSMS_UTIL_CHAR_LIST));
	if (list == TSMS_NULL) {
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, TSMS_STRING_static("malloc failed for TSMS_UCLP"));
		return TSMS_NULL;
	}
	list->list = malloc(initSize * sizeof(char));
	if (list->list == TSMS_NULL) {
		free(list);
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, TSMS_STRING_static("malloc failed for list"));
		return TSMS_NULL;
	}
	list->actualLength = initSize;
	list->length = 0;
	list->initLength = initSize;
	return list;
}

TSMS_UILP TSMS_UTIL_createIntList(int initSize) {
	TSMS_UILP list = malloc(sizeof(struct TSMS_UTIL_INT_LIST));
	if (list == TSMS_NULL) {
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, TSMS_STRING_static("malloc failed for TSMS_UILP"));
		return TSMS_NULL;
	}
	list->list = malloc(initSize * sizeof(int));
	if (list->list == TSMS_NULL) {
		free(list);
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, TSMS_STRING_static("malloc failed for list"));
		return TSMS_NULL;
	}
	list->actualLength = initSize;
	list->length = 0;
	list->initLength = initSize;
	return list;
}

TSMS_RESULT TSMS_UTIL_addList(TSMS_ULP list, void *element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (list->actualLength <= list->length) {
		list->actualLength *= 2;
		list->list = realloc(list->list, list->actualLength * sizeof(void *));
		if (list->list == TSMS_NULL) {
			TSMS_ERR_report(TSMS_ERR_REALLOC_FAILED, TSMS_STRING_static("realloc failed for list"));
			return TSMS_ERROR;
		}
	}
	list->list[list->length++] = element;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_UTIL_addCharList(TSMS_UCLP list, char element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (list->actualLength <= list->length) {
		list->actualLength *= 2;
		list->list = realloc(list->list, list->actualLength * sizeof(char));
		if (list->list == TSMS_NULL) {
			TSMS_ERR_report(TSMS_ERR_REALLOC_FAILED, TSMS_STRING_static("realloc failed for list"));
			return TSMS_ERROR;
		}
	}
	list->list[list->length++] = element;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_UTIL_addIntList(TSMS_UILP list, int element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (list->actualLength <= list->length) {
		list->actualLength *= 2;
		list->list = realloc(list->list, list->actualLength * sizeof(int));
		if (list->list == TSMS_NULL) {
			TSMS_ERR_report(TSMS_ERR_REALLOC_FAILED, TSMS_STRING_static("realloc failed for list"));
			return TSMS_ERROR;
		}
	}
	list->list[list->length++] = element;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_UTIL_removeList(TSMS_ULP list, uint32_t index) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (index >= list->length)
		return TSMS_FAIL;
	for (uint32_t i = index; i < list->length - 1; i++)
		list->list[i] = list->list[i + 1];
	list->length--;
	if (list->initLength != list->actualLength && list->length < list->actualLength / 2) {
		list->actualLength /= 2;
		list->list = realloc(list->list, list->actualLength * sizeof(void *));
		if (list->list == TSMS_NULL)
			return TSMS_ERROR;
	}
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_UTIL_removeListElement(TSMS_ULP list, void *element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	for (uint32_t i = 0; i < list->length; i++)
		if (list->list[i] == element)
			return TSMS_UTIL_removeList(list, i);
	return TSMS_FAIL;
}

TSMS_RESULT TSMS_UTIL_clearList(TSMS_ULP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	list->length = 0;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_UTIL_releaseList(TSMS_ULP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	free(list->list);
	free(list);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_UTIL_releaseCharList(TSMS_UCLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	free(list->list);
	free(list);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_UTIL_clearIntList(TSMS_UILP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	list->length = 0;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_UTIL_releaseIntList(TSMS_UILP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	free(list->list);
	free(list);
	return TSMS_SUCCESS;
}

uint8_t TSMS_UTIL_reverseByte(uint8_t v) {
	return TSMS_UTIL_reverseData(v, 8);
}

uint32_t TSMS_UTIL_reverseData(uint32_t v, uint8_t bits) {
	uint32_t t = 0;
	for (uint8_t i = 0; i < bits; i++) {
		t <<= 1;
		t |= (v & 1);
		v >>= 1;
	}
	return t;
}

TSMS_RESULT TSMS_UTIL_clearCharList(TSMS_UCLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	list->length = 0;
	return TSMS_SUCCESS;
}