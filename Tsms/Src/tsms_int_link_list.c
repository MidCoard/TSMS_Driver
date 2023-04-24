#include "tsms_int_link_list.h"

TSMS_INT_LKLP TSMS_INT_LINK_LIST_create() {
	TSMS_INT_LKLP list = (TSMS_INT_LKLP) malloc(sizeof(struct TSMS_INT_LINK_LIST));
	if (list == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for int link list");
		TSMS_ERR_report(TSMS_ERROR_TYPE_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	list->head = TSMS_NULL;
	list->tail = TSMS_NULL;
	list->length = 0;
	return list;
}

TSMS_RESULT TSMS_INT_LINK_LIST_add(TSMS_INT_LKLP list, int element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_INT_LINK_LIST_insert(list, element, list->length);
}

TSMS_RESULT TSMS_INT_LINK_LIST_remove(TSMS_INT_LKLP list, TSMS_POS index) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (index < 0 || index >= list->length)
		return TSMS_ERROR;
	TSMS_INT_LKNP node;
	if (index < list->length / 2) {
		node = list->head;
		for (TSMS_POS i = 0; i < index; i++)
			node = node->next;
	} else {
		node = list->tail;
		for (TSMS_POS i = list->length - 1; i > index; i--)
			node = node->prev;
	}
	return TSMS_INT_LINK_LIST_deleteNode(list, node);
}

TSMS_POS TSMS_INT_LINK_LIST_removeElement(TSMS_INT_LKLP list, int element) {
	if (list == TSMS_NULL)
		return -1;
	TSMS_INT_LKNP node = list->head;
	TSMS_POS index = 0;
	while (node != TSMS_NULL) {
		if (node->element == element) {
			TSMS_INT_LINK_LIST_deleteNode(list, node);
			return index;
		}
		node = node->next;
		index++;
	}
	return -1;
}

TSMS_RESULT TSMS_INT_LINK_LIST_release(TSMS_INT_LKLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = TSMS_INT_LINK_LIST_clear(list);
	free(list);
	return result;
}

TSMS_RESULT TSMS_INT_LINK_LIST_clear(TSMS_INT_LKLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_INT_LKNP node = list->head;
	while (node != TSMS_NULL) {
		TSMS_INT_LKNP temp = node;
		node = node->next;
		free(temp);
	}
	list->head = TSMS_NULL;
	list->tail = TSMS_NULL;
	list->length = 0;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_INT_LINK_LIST_insert(TSMS_INT_LKLP list, int element, TSMS_POS index) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (index < 0 || index > list->length)
		return TSMS_ERROR;
	TSMS_INT_LKNP node = (TSMS_INT_LKNP) malloc(sizeof(struct TSMS_INT_LINK_NODE));
	if (node == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for int link node");
		TSMS_ERR_report(TSMS_ERROR_TYPE_MALLOC_FAILED, &temp);
		return TSMS_ERROR;
	}
	node->element = element;
	if (index == 0) {
		node->prev = TSMS_NULL;
		node->next = list->head;
		if (list->head != TSMS_NULL)
			list->head->prev = node;
		list->head = node;
		if (list->tail == TSMS_NULL)
			list->tail = node;
	} else if (index == list->length) {
		node->next = TSMS_NULL;
		node->prev = list->tail;
		if (list->tail != TSMS_NULL)
			list->tail->next = node;
		list->tail = node;
		if (list->head == TSMS_NULL)
			list->head = node;
	} else {
		TSMS_INT_LKNP temp;
		if (index < list->length / 2) {
			temp = list->head;
			for (TSMS_POS i = 0; i < index; i++)
				temp = temp->next;
		} else {
			temp = list->tail;
			for (TSMS_POS i = list->length - 1; i > index; i--)
				temp = temp->prev;
		}
		node->prev = temp->prev;
		node->next = temp;
		temp->prev->next = node;
		temp->prev = node;
	}
	list->length++;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_INT_LINK_LIST_deleteNode(TSMS_INT_LKLP list, TSMS_INT_LKNP node) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (node == TSMS_NULL)
		return TSMS_ERROR;
	if (node->prev != TSMS_NULL)
		node->prev->next = node->next;
	if (node->next != TSMS_NULL)
		node->next->prev = node->prev;
	if (node == list->head)
		list->head = node->next;
	if (node == list->tail)
		list->tail = node->prev;
	free(node);
	list->length--;
	return TSMS_SUCCESS;
}
