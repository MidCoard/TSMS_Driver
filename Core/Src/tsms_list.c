#include "tsms_list.h"
#include "tsms_char_list.h"
#include "tsms_int_list.h"
#include "tsms_long_list.h"


TSMS_LLP TSMS_LIST_create(TSMS_SIZE initSize) {
	TSMS_LLP list = malloc(sizeof(struct TSMS_LIST_LIST));
	if (list == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_LLP");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	list->list = malloc(initSize * sizeof(void *));
	if (list->list == TSMS_NULL) {
		free(list);
		tString temp = TSMS_STRING_temp("malloc failed for list");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	list->actualLength = initSize;
	list->length = 0;
	list->initLength = initSize;
	return list;
}

TSMS_RESULT TSMS_LIST_add(TSMS_LLP list, void *element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (list->actualLength <= list->length) {
		list->actualLength *= 2;
		void * tmp = realloc(list->list, list->actualLength * sizeof(void *));
		if (tmp == TSMS_NULL) {
			tString temp = TSMS_STRING_temp("realloc failed for list");
			TSMS_ERR_report(TSMS_ERR_REALLOC_FAILED, &temp);
			return TSMS_ERROR;
		}
		list->list = tmp;
	}
	list->list[list->length++] = element;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LIST_remove(TSMS_LLP list, TSMS_POS index) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (index >= list->length || index < 0)
		return TSMS_FAIL;
	for (TSMS_POS i = index; i < list->length - 1; i++)
		list->list[i] = list->list[i + 1];
	list->length--;
	if (list->initLength != list->actualLength && list->length < list->actualLength / 2) {
		void* tmp = realloc(list->list, list->actualLength * sizeof(void *));
		if (tmp == TSMS_NULL) {
			tString temp = TSMS_STRING_temp("realloc failed for list");
			TSMS_ERR_report(TSMS_ERR_REALLOC_FAILED, &temp);
			return TSMS_ERROR;
		}
		list->list = tmp;
		list->actualLength /= 2;
	}
	return TSMS_SUCCESS;
}

TSMS_POS TSMS_LIST_removeElement(TSMS_LLP list, void *element) {
	if (list == TSMS_NULL)
		return -1;
	for (TSMS_POS i = 0; i < list->length; i++)
		if (list->list[i] == element) {
			if (TSMS_LIST_remove(list, i) != TSMS_SUCCESS)
				return -1;
			return i;
		}
	return -1;
}

TSMS_RESULT TSMS_LIST_clear(TSMS_LLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	list->length = 0;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LIST_release(TSMS_LLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	free(list->list);
	free(list);
	return TSMS_SUCCESS;
}



TSMS_CLP TSMS_CHAR_LIST_create(TSMS_SIZE initSize) {
	TSMS_CLP list = malloc(sizeof(struct TSMS_CHAR_LIST));
	if (list == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_CLP");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	list->list = malloc(initSize * sizeof(char));
	if (list->list == TSMS_NULL) {
		free(list);
		tString temp = TSMS_STRING_temp("malloc failed for list");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	list->actualLength = initSize;
	list->length = 0;
	list->initLength = initSize;
	return list;
}

TSMS_RESULT TSMS_CHAR_LIST_add(TSMS_CLP list, char element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (list->actualLength <= list->length) {
		list->actualLength *= 2;
		void* tmp = realloc(list->list, list->actualLength * sizeof(char));
		if (tmp == TSMS_NULL) {
			tString temp = TSMS_STRING_temp("realloc failed for list");
			TSMS_ERR_report(TSMS_ERR_REALLOC_FAILED, &temp);
			return TSMS_ERROR;
		}
		list->list = tmp;
	}
	list->list[list->length++] = element;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_CHAR_LIST_remove(TSMS_CLP list, TSMS_POS index) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (index >= list->length || index < 0)
		return TSMS_FAIL;
	for (TSMS_POS i = index; i < list->length - 1; i++)
		list->list[i] = list->list[i + 1];
	list->length--;
	if (list->initLength != list->actualLength && list->length < list->actualLength / 2) {
		void* tmp = realloc(list->list, list->actualLength * sizeof(char));
		if (tmp == TSMS_NULL) {
			tString temp = TSMS_STRING_temp("realloc failed for list");
			TSMS_ERR_report(TSMS_ERR_REALLOC_FAILED, &temp);
			return TSMS_ERROR;
		}
		list->list = tmp;
		list->actualLength /= 2;
	}
	return TSMS_SUCCESS;
}

TSMS_POS TSMS_CHAR_LIST_removeElement(TSMS_CLP list, char element) {
	if (list == TSMS_NULL)
		return -1;
	for (TSMS_POS i = 0; i < list->length; i++)
		if (list->list[i] == element) {
			if (TSMS_CHAR_LIST_remove(list, i) != TSMS_SUCCESS)
				return -1;
			return i;
		}
	return -1;
}

TSMS_RESULT TSMS_CHAR_LIST_clear(TSMS_CLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	list->length = 0;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_CHAR_LIST_release(TSMS_CLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	free(list->list);
	free(list);
	return TSMS_SUCCESS;
}



TSMS_ILP TSMS_INT_LIST_create(TSMS_SIZE initSize) {
	TSMS_ILP list = malloc(sizeof(struct TSMS_INT_LIST));
	if (list == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_ILP");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	list->list = malloc(initSize * sizeof(int));
	if (list->list == TSMS_NULL) {
		free(list);
		tString temp = TSMS_STRING_temp("malloc failed for list");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	list->actualLength = initSize;
	list->length = 0;
	list->initLength = initSize;
	return list;
}

TSMS_RESULT TSMS_INT_LIST_add(TSMS_ILP list, int element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (list->actualLength <= list->length) {
		list->actualLength *= 2;
		void* tmp = realloc(list->list, list->actualLength * sizeof(int));
		if (tmp == TSMS_NULL) {
			tString temp = TSMS_STRING_temp("realloc failed for list");
			TSMS_ERR_report(TSMS_ERR_REALLOC_FAILED, &temp);
			return TSMS_ERROR;
		}
		list->list = tmp;
	}
	list->list[list->length++] = element;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_INT_LIST_remove(TSMS_ILP list, TSMS_POS index) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (index >= list->length || index < 0)
		return TSMS_FAIL;
	for (TSMS_POS i = index; i < list->length - 1; i++)
		list->list[i] = list->list[i + 1];
	list->length--;
	if (list->initLength != list->actualLength && list->length < list->actualLength / 2) {
		void* tmp = realloc(list->list, list->actualLength * sizeof(int));
		if (tmp == TSMS_NULL) {
			tString temp = TSMS_STRING_temp("realloc failed for list");
			TSMS_ERR_report(TSMS_ERR_REALLOC_FAILED, &temp);
			return TSMS_ERROR;
		}
		list->list = tmp;
		list->actualLength /= 2;
	}
	return TSMS_SUCCESS;
}

TSMS_POS TSMS_INT_LIST_removeElement(TSMS_ILP list, int element) {
	if (list == TSMS_NULL)
		return -1;
	for (TSMS_POS i = 0; i < list->length; i++)
		if (list->list[i] == element) {
			if (TSMS_INT_LIST_remove(list, i) != TSMS_SUCCESS)
				return -1;
			return i;
		}
	return -1;
}

TSMS_RESULT TSMS_INT_LIST_clear(TSMS_ILP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	list->length = 0;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_INT_LIST_release(TSMS_ILP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	free(list->list);
	free(list);
	return TSMS_SUCCESS;
}



TSMS_LLLP TSMS_LONG_LIST_create(TSMS_SIZE initSize) {
	TSMS_LLLP list = malloc(sizeof(struct TSMS_LIST_LONG_LIST));
	if (list == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_LLLP");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	list->list = malloc(initSize * sizeof(long));
	if (list->list == TSMS_NULL) {
		free(list);
		tString temp = TSMS_STRING_temp("malloc failed for list");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	list->actualLength = initSize;
	list->length = 0;
	list->initLength = initSize;
	return list;
}

TSMS_RESULT TSMS_LONG_LIST_add(TSMS_LLLP list, long element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (list->actualLength <= list->length) {
		list->actualLength *= 2;
		void * tmp = realloc(list->list, list->actualLength * sizeof(long));
		if (tmp == TSMS_NULL) {
			tString temp = TSMS_STRING_temp("realloc failed for list");
			TSMS_ERR_report(TSMS_ERR_REALLOC_FAILED, &temp);
			return TSMS_ERROR;
		}
		list->list = tmp;
	}
	list->list[list->length++] = element;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LONG_LIST_remove(TSMS_LLLP list, TSMS_POS index) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (index >= list->length || index < 0)
		return TSMS_FAIL;
	for (TSMS_POS i = index; i < list->length - 1; i++)
		list->list[i] = list->list[i + 1];
	list->length--;
	if (list->initLength != list->actualLength && list->length < list->actualLength / 2) {
		void * tmp = realloc(list->list, list->actualLength * sizeof(long));
		if (tmp == TSMS_NULL) {
			tString temp = TSMS_STRING_temp("realloc failed for list");
			TSMS_ERR_report(TSMS_ERR_REALLOC_FAILED, &temp);
			return TSMS_ERROR;
		}
		list->list = tmp;
		list->actualLength /= 2;
	}
	return TSMS_SUCCESS;
}

TSMS_POS TSMS_LONG_LIST_removeElement(TSMS_LLLP list, long element) {
	if (list == TSMS_NULL)
		return -1;
	for (TSMS_POS i = 0; i < list->length; i++)
		if (list->list[i] == element) {
			if (TSMS_LONG_LIST_remove(list, i) != TSMS_SUCCESS)
				return -1;
			return i;
		}
	return -1;
}

TSMS_RESULT TSMS_LONG_LIST_clear(TSMS_LLLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	list->length = 0;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LONG_LIST_release(TSMS_LLLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	free(list->list);
	free(list);
	return TSMS_SUCCESS;
}
