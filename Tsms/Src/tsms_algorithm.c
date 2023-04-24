#include "tsms_algorithm.h"
#include "tsms_list.h"

TSMS_INLINE void __tsms_internal_quick_sort(void** data, TSMS_SIZE size, TSMS_COMPARE_FUNCTION compareFunction) {
	if (size <= 1)
		return;
	TSMS_SIZE pivotIndex = size / 2;
	void* pivotNode = data[pivotIndex];
	TSMS_SIZE leftIndex = 0;
	TSMS_SIZE rightIndex = size - 1;
	while (leftIndex < rightIndex) {
		while (leftIndex < rightIndex && compareFunction(data[leftIndex], pivotNode) < 0) {
			leftIndex++;
		}
		while (leftIndex < rightIndex && compareFunction(data[rightIndex], pivotNode) > 0) {
			rightIndex--;
		}
		if (leftIndex < rightIndex) {
			void* temp = data[leftIndex];
			data[leftIndex] = data[rightIndex];
			data[rightIndex] = temp;
			leftIndex++;
			rightIndex--;
		}
	}
	__tsms_internal_quick_sort(data, leftIndex, compareFunction);
	__tsms_internal_quick_sort(data + leftIndex, size - leftIndex, compareFunction);
}

TSMS_RESULT TSMS_ALGORITHM_sort(TSMS_LP list, TSMS_COMPARE_FUNCTION compareFunction) {
	if (list == TSMS_NULL || compareFunction == TSMS_NULL) {
		return TSMS_ERROR;
	}
	TSMS_SIZE size = list->length;
	__tsms_internal_quick_sort(list->list, size, compareFunction);
	return TSMS_SUCCESS;
}