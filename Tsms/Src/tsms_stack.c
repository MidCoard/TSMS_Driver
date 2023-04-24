#include "tsms_stack.h"
#include "tsms_link_list.h"

TSMS_STP TSMS_STACK_create() {
	TSMS_STP stack = (TSMS_STP) malloc(sizeof(struct TSMS_STACK));
	if (stack == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for stack");
		TSMS_ERR_report(TSMS_ERROR_TYPE_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	stack->list = TSMS_LINK_LIST_create();
	if (stack->list == TSMS_NULL) {
		TSMS_STACK_release(stack);
		return TSMS_NULL;
	}
	return stack;
}

TSMS_RESULT TSMS_STACK_push(TSMS_STP stack, void* element) {
	if (stack == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_LINK_LIST_add(stack->list, element);
}

void* TSMS_STACK_pop(TSMS_STP stack) {
	void * element = TSMS_STACK_peek(stack);
	if (element == TSMS_NULL)
		return TSMS_NULL;
	TSMS_LINK_LIST_deleteNode(stack->list, stack->list->tail);
	return element;
}

void* TSMS_STACK_peek(TSMS_STP stack) {
	if (stack == TSMS_NULL)
		return TSMS_NULL;
	if (stack->list->length == 0)
		return TSMS_NULL;
	return stack->list->tail->element;
}

TSMS_RESULT TSMS_STACK_clear(TSMS_STP stack) {
	if (stack == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_LINK_LIST_clear(stack->list);
}

TSMS_RESULT TSMS_STACK_release(TSMS_STP stack) {
	if (stack == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = TSMS_LINK_LIST_release(stack->list);
	free(stack);
	return result;
}

TSMS_SIZE TSMS_STACK_size(TSMS_STP stack) {
	return stack->list->length;
}

bool TSMS_STACK_empty(TSMS_STP stack) {
	return TSMS_STACK_size(stack) == 0;
}