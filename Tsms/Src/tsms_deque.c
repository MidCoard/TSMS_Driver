#include "tsms_deque.h"

TSMS_DP TSMS_DEQUE_create() {
	TSMS_DP deque = (TSMS_DP) malloc(sizeof(struct TSMS_DEQUE));
	if (deque == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for deque");
		TSMS_ERR_report(TSMS_ERROR_TYPE_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	deque->list = TSMS_LINK_LIST_create();
	if (deque->list == TSMS_NULL) {
		TSMS_DEQUE_release(deque);
		return TSMS_NULL;
	}
	return deque;
}

TSMS_RESULT TSMS_DEQUE_addFirst(TSMS_DP deque, void* element) {
	if (deque == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_LINK_LIST_insert(deque->list, element, 0);
}

TSMS_RESULT TSMS_DEQUE_addLast(TSMS_DP deque, void* element) {
	if (deque == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_LINK_LIST_add(deque->list, element);
}

void* TSMS_DEQUE_removeFirst(TSMS_DP deque) {
	void * element = TSMS_DEQUE_peekFirst(deque);
	if (element == TSMS_NULL)
		return TSMS_NULL;
	TSMS_LINK_LIST_deleteNode(deque->list, deque->list->head);
	return element;
}

void* TSMS_DEQUE_removeLast(TSMS_DP deque) {
	void * element = TSMS_DEQUE_peekLast(deque);
	if (element == TSMS_NULL)
		return TSMS_NULL;
	TSMS_LINK_LIST_deleteNode(deque->list, deque->list->tail);
	return element;
}

void* TSMS_DEQUE_peekFirst(TSMS_DP deque) {
	if (deque == TSMS_NULL)
		return TSMS_NULL;
	if (deque->list->head == TSMS_NULL)
		return TSMS_NULL;
	return deque->list->head->element;
}

void* TSMS_DEQUE_peekLast(TSMS_DP deque) {
	if (deque == TSMS_NULL)
		return TSMS_NULL;
	if (deque->list->tail == TSMS_NULL)
		return TSMS_NULL;
	return deque->list->tail->element;
}

TSMS_RESULT TSMS_DEQUE_clear(TSMS_DP deque) {
	if (deque == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_LINK_LIST_clear(deque->list);
}

TSMS_RESULT TSMS_DEQUE_release(TSMS_DP deque) {
	if (deque == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = TSMS_DEQUE_clear(deque);
	free(deque->list);
	free(deque);
	return result;
}

TSMS_SIZE TSMS_DEQUE_size(TSMS_DP deque) {
	return deque->list->length;
}

bool TSMS_DEQUE_empty(TSMS_DP deque) {
	return TSMS_DEQUE_size(deque) == 0;
}