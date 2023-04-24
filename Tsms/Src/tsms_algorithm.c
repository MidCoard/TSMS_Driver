#include "tsms_algorithm.h"
#include "tsms_list.h"

TSMS_INLINE void __tsms_internal_swap(void** a, void** b) {
	void* temp = *a;
	*a = *b;
	*b = temp;
}

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
				__tsms_internal_swap(&data[i], &data[j]);
		}
		__tsms_internal_swap(&data[pivot], &data[j]);
		__tsms_internal_quick_sort(data, size, first, j - 1, compareFunction);
		__tsms_internal_quick_sort(data, size, j + 1, last, compareFunction);
	}
}

TSMS_RESULT TSMS_ALGORITHM_sort(TSMS_LP list, TSMS_COMPARE_FUNCTION compareFunction) {
	if (list == TSMS_NULL || compareFunction == TSMS_NULL) {
		return TSMS_ERROR;
	}
	TSMS_SIZE size = list->length;
	__tsms_internal_quick_sort(list->list, size, 0, size - 1, compareFunction);
	return TSMS_SUCCESS;
}