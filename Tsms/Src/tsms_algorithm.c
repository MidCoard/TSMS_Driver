#include "tsms_algorithm.h"
#include "tsms_util.h"

TSMS_INLINE void __tsms_internal_quick_sort(void** data, TSMS_SIZE size, TSMS_POS first, TSMS_POS last, TSMS_COMPARE_FUNCTION compareFunction) {
	if (first < last) {
		TSMS_POS pivot = first;
		TSMS_POS i = first;
		TSMS_POS j = last;
		while (i < j) {
			while (i < j && compareFunction(data[i], data[pivot]) <= 0)
				i++;
			while (compareFunction(data[j], data[pivot]) > 0)
				j--;
			if (i < j)
				TSMS_UTIL_swap(&data[i], &data[j]);
		}
		TSMS_UTIL_swap(&data[pivot], &data[j]);
		__tsms_internal_quick_sort(data, size, first, j - 1, compareFunction);
		__tsms_internal_quick_sort(data, size, j + 1, last, compareFunction);
	}
}

TSMS_INLINE void __tsms_internal_quick_sort_float(float * data, TSMS_SIZE size, TSMS_POS first, TSMS_POS last) {
	if (first < last) {
		TSMS_POS pivot = first;
		TSMS_POS i = first;
		TSMS_POS j = last;
		while (i < j) {
			while (i < j && data[i] <= data[pivot])
				i++;
			while (data[j] > data[pivot])
				j--;
			if (i < j)
				TSMS_UTIL_swapFloat(&data[i], &data[j]);
		}
		TSMS_UTIL_swapFloat(&data[pivot], &data[j]);
		__tsms_internal_quick_sort_float(data, size, first, j - 1);
		__tsms_internal_quick_sort_float(data, size, j + 1, last);
	}
}

TSMS_RESULT TSMS_ALGORITHM_sort(TSMS_LP list, TSMS_COMPARE_FUNCTION compareFunction) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (compareFunction == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_ALGORITHM_sortArray(list->list, list->length, compareFunction);
}

TSMS_RESULT TSMS_ALGORITHM_sortArray(void** array, TSMS_SIZE size, TSMS_COMPARE_FUNCTION compareFunction) {
	if (array == TSMS_NULL)
		return TSMS_ERROR;
	if (compareFunction == TSMS_NULL)
		return TSMS_ERROR;
	__tsms_internal_quick_sort(array, size, 0, size - 1, compareFunction);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_ALGORITHM_sortFloatArray(float * array, TSMS_SIZE size) {
	if (array == TSMS_NULL)
		return TSMS_ERROR;
	__tsms_internal_quick_sort_float(array, size, 0, size - 1);
	return TSMS_SUCCESS;
}

TSMS_POS TSMS_ALGORITHM_lowerBoundFloatLinkList(TSMS_FLOAT_LINK_LIST_POINTER list, float value, TSMS_FLKNP * node) {
	if (list == TSMS_NULL)
		return -1;
	TSMS_FLKNP n = list->head;
	TSMS_POS i = 0;
	while (n != TSMS_NULL) {
		if (n->element >= value) {
			if (node != TSMS_NULL)
				*node = n;
			return i;
		}
		n = n->next;
		i++;
	}
	if (node != TSMS_NULL)
		*node = TSMS_NULL;
	return -1;
}