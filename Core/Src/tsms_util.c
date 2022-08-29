#include "tsms_util.h"

uint32_t * TSMS_UTIL_gen32BitPointer(uint32_t v) {
	uint32_t t = v;
	return &t;
}

TSMS_ULP TSMS_UTIL_createList(int initSize, size_t size) {
	TSMS_ULP list = malloc(sizeof(struct TSMS_UTIL_LIST));
	if (list == TSMS_NULL)
		return TSMS_NULL;
	list->list = malloc(size * initSize);
	if (list->list == TSMS_NULL) {
		free(list);
		return TSMS_NULL;
	}
	list->size = size;
	list->actualLength = initSize;
	list->length = 0;
	list->initLength = initSize;
	return list;
}

TSMS_RESULT TSMS_UTIL_addList(TSMS_ULP list, void* element) {
	if(list->actualLength < list->length) {
		list->actualLength *= 2;
		list->list = realloc(list->list, list->size * list->actualLength);
		if (list->list == TSMS_NULL)
			return TSMS_ERROR;
	}
	void** pos = list->list + list->size * list->length;
	*pos = element;
	list->length++;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_UTIL_removeList(TSMS_ULP list, uint32_t index) {
	if (index >= list->length)
		return TSMS_ERROR;
	for (uint32_t i = index; i < list->length - 1; i++) {
		void** pos1 = list->list + list->size * i;
		void** pos2 = list->list + list->size * (i + 1);
		*pos1 = *pos2;
	}
	list->length--;
	if (list->initLength != list->actualLength && list->length < list->actualLength / 2) {
		list->actualLength /= 2;
		list->list = realloc(list->list, list->size * list->actualLength);
		if (list->list == TSMS_NULL)
			return TSMS_ERROR;
	}
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_UTIL_removeListElement(TSMS_ULP list, void* element) {
	for (uint32_t i = 0; i < list->length; i++) {
		void** pos = list->list + list->size * i;
		if (*pos == element)
			return TSMS_UTIL_removeList(list, i);
	}
	return TSMS_FAIL;
}

uint8_t TSMS_UTIL_reverseByte(uint8_t v) {
	uint8_t t = 0;
	for (uint8_t i = 0; i < 8; i++) {
		t <<= 1;
		t |= (v & 1);
		v >>= 1;
	}
	return t;
}