#include "tsms_list.h"


TSMS_LLP TSMS_LIST_createList(TSMS_SIZE initSize) {
	TSMS_LLP list = malloc(sizeof(struct TSMS_LIST_LIST));
	if (list == TSMS_NULL) {
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, TSMS_STRING_static("malloc failed for TSMS_LLP"));
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

TSMS_LCLP TSMS_LIST_createCharList(TSMS_SIZE initSize) {
	TSMS_LCLP list = malloc(sizeof(struct TSMS_LIST_CHAR_LIST));
	if (list == TSMS_NULL) {
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, TSMS_STRING_static("malloc failed for TSMS_LCLP"));
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

TSMS_LILP TSMS_LIST_createIntList(TSMS_SIZE initSize) {
	TSMS_LILP list = malloc(sizeof(struct TSMS_LIST_INT_LIST));
	if (list == TSMS_NULL) {
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, TSMS_STRING_static("malloc failed for TSMS_LILP"));
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

TSMS_LLLP TSMS_LIST_createLongList(TSMS_SIZE initSize) {
	TSMS_LLLP list = malloc(sizeof(struct TSMS_LIST_LONG_LIST));
	if (list == TSMS_NULL) {
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, TSMS_STRING_static("malloc failed for TSMS_LLLP"));
		return TSMS_NULL;
	}
	list->list = malloc(initSize * sizeof(long));
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

TSMS_RESULT TSMS_LIST_addList(TSMS_LLP list, void *element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (list->actualLength <= list->length) {
		list->actualLength *= 2;
		void * tmp = realloc(list->list, list->actualLength * sizeof(void *));
		if (tmp == TSMS_NULL) {
			TSMS_ERR_report(TSMS_ERR_REALLOC_FAILED, TSMS_STRING_static("realloc failed for list"));
			return TSMS_ERROR;
		}
		list->list = tmp;
	}
	list->list[list->length++] = element;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LIST_addCharList(TSMS_LCLP list, char element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (list->actualLength <= list->length) {
		list->actualLength *= 2;
		void* tmp = realloc(list->list, list->actualLength * sizeof(char));
		if (tmp == TSMS_NULL) {
			TSMS_ERR_report(TSMS_ERR_REALLOC_FAILED, TSMS_STRING_static("realloc failed for list"));
			return TSMS_ERROR;
		}
		list->list = tmp;
	}
	list->list[list->length++] = element;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LIST_addIntList(TSMS_LILP list, int element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (list->actualLength <= list->length) {
		list->actualLength *= 2;
		void* tmp = realloc(list->list, list->actualLength * sizeof(int));
		if (tmp == TSMS_NULL) {
			TSMS_ERR_report(TSMS_ERR_REALLOC_FAILED, TSMS_STRING_static("realloc failed for list"));
			return TSMS_ERROR;
		}
		list->list = tmp;
	}
	list->list[list->length++] = element;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LIST_addLongList(TSMS_LLLP list, long element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (list->actualLength <= list->length) {
		list->actualLength *= 2;
		void * tmp = realloc(list->list, list->actualLength * sizeof(long));
		if (tmp == TSMS_NULL) {
			TSMS_ERR_report(TSMS_ERR_REALLOC_FAILED, TSMS_STRING_static("realloc failed for list"));
			return TSMS_ERROR;
		}
		list->list = tmp;
	}
	list->list[list->length++] = element;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LIST_removeList(TSMS_LLP list, TSMS_POS index) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (index >= list->length || index < 0)
		return TSMS_FAIL;
	for (TSMS_POS i = index; i < list->length - 1; i++)
		list->list[i] = list->list[i + 1];
	list->length--;
	if (list->initLength != list->actualLength && list->length < list->actualLength / 2) {
		void* tmp = realloc(list->list, list->actualLength * sizeof(void *));
		if (tmp == TSMS_NULL)
			return TSMS_ERROR;
		list->list = tmp;
		list->actualLength /= 2;
	}
	return TSMS_SUCCESS;
}

TSMS_POS TSMS_LIST_removeListElement(TSMS_LLP list, void *element) {
	if (list == TSMS_NULL)
		return -1;
	for (TSMS_POS i = 0; i < list->length; i++)
		if (list->list[i] == element) {
			if (TSMS_LIST_removeList(list, i) != TSMS_SUCCESS)
				return -1;
			return i;
		}
	return -1;
}

TSMS_RESULT TSMS_LIST_clearList(TSMS_LLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	list->length = 0;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LIST_releaseList(TSMS_LLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	free(list->list);
	free(list);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LIST_releaseCharList(TSMS_LCLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	free(list->list);
	free(list);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LIST_clearIntList(TSMS_LILP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	list->length = 0;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LIST_releaseIntList(TSMS_LILP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	free(list->list);
	free(list);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LIST_clearLongList(TSMS_LLLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	list->length = 0;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LIST_releaseLongList(TSMS_LLLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	free(list->list);
	free(list);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LIST_clearCharList(TSMS_LCLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	list->length = 0;
	return TSMS_SUCCESS;
}

