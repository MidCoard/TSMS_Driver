#include "tsms_link_list.h"
#include "tsms_int_link_list.h"
#include "tsms_float_link_list.h"
#include "tsms.h"

TSMS_LKLP TSMS_LINK_LIST_create() {
	TSMS_LKLP list = (TSMS_LKLP) TSMS_malloc(sizeof(struct TSMS_LINK_LIST));
	if (list == TSMS_NULL)
		return TSMS_NULL;
	list->head = TSMS_NULL;
	list->tail = TSMS_NULL;
	list->length = 0;
	return list;
}

TSMS_RESULT TSMS_LINK_LIST_add(TSMS_LKLP list, void *element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_LINK_LIST_insert(list, element, list->length);
}

TSMS_RESULT TSMS_LINK_LIST_remove(TSMS_LKLP list, TSMS_POS index) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (index < 0 || index >= list->length)
		return TSMS_ERROR;
	TSMS_LKNP node;
	if (index < list->length / 2) {
		node = list->head;
		for (TSMS_POS i = 0; i < index; i++)
			node = node->next;
	} else {
		node = list->tail;
		for (TSMS_POS i = list->length - 1; i > index; i--)
			node = node->prev;
	}
	return TSMS_LINK_LIST_deleteNode(list, node);
}

TSMS_POS TSMS_LINK_LIST_removeElement(TSMS_LKLP list, void *element) {
	if (list == TSMS_NULL)
		return -1;
	TSMS_LKNP node = list->head;
	TSMS_POS index = 0;
	while (node != TSMS_NULL) {
		if (node->element == element) {
			TSMS_LINK_LIST_deleteNode(list, node);
			return index;
		}
		node = node->next;
		index++;
	}
	return -1;
}

TSMS_RESULT TSMS_LINK_LIST_release(TSMS_LKLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = TSMS_LINK_LIST_clear(list);
	free(list);
	return result;
}

TSMS_RESULT TSMS_LINK_LIST_clear(TSMS_LKLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LKNP node = list->head;
	while (node != TSMS_NULL) {
		TSMS_LKNP temp = node;
		node = node->next;
		free(temp);
	}
	list->head = TSMS_NULL;
	list->tail = TSMS_NULL;
	list->length = 0;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LINK_LIST_deleteNode(TSMS_LKLP list, TSMS_LKNP node) {
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
	list->length--;
	free(node);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_LINK_LIST_insert(TSMS_LKLP list, void *element, TSMS_POS index) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (index < 0 || index > list->length)
		return TSMS_ERROR;
	TSMS_LKNP node = (TSMS_LKNP) TSMS_malloc(sizeof(struct TSMS_LINK_NODE));
	if (node == TSMS_NULL)
		return TSMS_ERROR;
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
		TSMS_LKNP temp;
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

TSMS_ILKLP TSMS_INT_LINK_LIST_create() {
	TSMS_ILKLP list = (TSMS_ILKLP) TSMS_malloc(sizeof(struct TSMS_INT_LINK_LIST));
	if (list == TSMS_NULL)
		return TSMS_NULL;
	list->head = TSMS_NULL;
	list->tail = TSMS_NULL;
	list->length = 0;
	return list;
}

TSMS_RESULT TSMS_INT_LINK_LIST_add(TSMS_ILKLP list, int element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_INT_LINK_LIST_insert(list, element, list->length);
}

TSMS_RESULT TSMS_INT_LINK_LIST_remove(TSMS_ILKLP list, TSMS_POS index) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (index < 0 || index >= list->length)
		return TSMS_ERROR;
	TSMS_ILKNP node;
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

TSMS_POS TSMS_INT_LINK_LIST_removeElement(TSMS_ILKLP list, int element) {
	if (list == TSMS_NULL)
		return -1;
	TSMS_ILKNP node = list->head;
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

TSMS_RESULT TSMS_INT_LINK_LIST_release(TSMS_ILKLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = TSMS_INT_LINK_LIST_clear(list);
	free(list);
	return result;
}

TSMS_RESULT TSMS_INT_LINK_LIST_clear(TSMS_ILKLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_ILKNP node = list->head;
	while (node != TSMS_NULL) {
		TSMS_ILKNP temp = node;
		node = node->next;
		free(temp);
	}
	list->head = TSMS_NULL;
	list->tail = TSMS_NULL;
	list->length = 0;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_INT_LINK_LIST_insert(TSMS_ILKLP list, int element, TSMS_POS index) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (index < 0 || index > list->length)
		return TSMS_ERROR;
	TSMS_ILKNP node = (TSMS_ILKNP) TSMS_malloc(sizeof(struct TSMS_INT_LINK_NODE));
	if (node == TSMS_NULL)
		return TSMS_ERROR;
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
		TSMS_ILKNP temp;
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

TSMS_RESULT TSMS_INT_LINK_LIST_deleteNode(TSMS_ILKLP list, TSMS_ILKNP node) {
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


TSMS_FLKLP TSMS_FLOAT_LINK_LIST_create() {
	TSMS_FLKLP list = (TSMS_FLKLP) TSMS_malloc(sizeof(struct TSMS_FLOAT_LINK_LIST));
	if (list == TSMS_NULL)
		return TSMS_NULL;
	list->head = TSMS_NULL;
	list->tail = TSMS_NULL;
	list->length = 0;
	return list;
}

TSMS_RESULT TSMS_FLOAT_LINK_LIST_add(TSMS_FLKLP list, float element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_FLOAT_LINK_LIST_insert(list, element, list->length);
}

TSMS_RESULT TSMS_FLOAT_LINK_LIST_remove(TSMS_FLKLP list, TSMS_POS index) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (index < 0 || index >= list->length)
		return TSMS_ERROR;
	TSMS_FLKNP node;
	if (index < list->length / 2) {
		node = list->head;
		for (TSMS_POS i = 0; i < index; i++)
			node = node->next;
	} else {
		node = list->tail;
		for (TSMS_POS i = list->length - 1; i > index; i--)
			node = node->prev;
	}
	return TSMS_FLOAT_LINK_LIST_deleteNode(list, node);
}

TSMS_POS TSMS_FLOAT_LINK_LIST_removeElement(TSMS_FLKLP list, float element) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_FLKNP node = list->head;
	TSMS_POS index = 0;
	while (node != TSMS_NULL) {
		if (node->element == element) {
			TSMS_FLOAT_LINK_LIST_deleteNode(list, node);
			return index;
		}
		node = node->next;
		index++;
	}
	return TSMS_ERROR;
}

TSMS_RESULT TSMS_FLOAT_LINK_LIST_release(TSMS_FLKLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_FLOAT_LINK_LIST_clear(list);
	free(list);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_FLOAT_LINK_LIST_clear(TSMS_FLKLP list) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_FLKNP node = list->head;
	while (node != TSMS_NULL) {
		TSMS_FLKNP temp = node;
		node = node->next;
		free(temp);
	}
	list->head = TSMS_NULL;
	list->tail = TSMS_NULL;
	list->length = 0;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_FLOAT_LINK_LIST_insert(TSMS_FLKLP list, float element, TSMS_POS index) {
	if (list == TSMS_NULL)
		return TSMS_ERROR;
	if (index < 0 || index > list->length)
		return TSMS_ERROR;
	TSMS_FLKNP node = (TSMS_FLKNP) TSMS_malloc(sizeof(struct TSMS_FLOAT_LINK_NODE));
	if (node == TSMS_NULL)
		return TSMS_ERROR;
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
		node->prev = list->tail;
		node->next = TSMS_NULL;
		if (list->tail != TSMS_NULL)
			list->tail->next = node;
		list->tail = node;
		if (list->head == TSMS_NULL)
			list->head = node;
	} else {
		TSMS_FLKNP temp;
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

TSMS_RESULT TSMS_FLOAT_LINK_LIST_deleteNode(TSMS_FLKLP list, TSMS_FLKNP node) {
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
