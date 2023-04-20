#include "tsms_int_stack.h"

TSMS_INT_STP TSMS_INT_STACK_create() {
	TSMS_INT_STP stack = (TSMS_INT_STP) malloc(sizeof(struct TSMS_INT_STACK));
	if (stack == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for int stack");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	stack->list = TSMS_INT_LINK_LIST_create();
	if (stack->list == NULL) {
		TSMS_INT_STACK_release(stack);
		return NULL;
	}
	return stack;
}

TSMS_RESULT TSMS_INT_STACK_push(TSMS_INT_STP stack, int element) {
	if (stack == NULL)
		return TSMS_ERROR;
	return TSMS_INT_LINK_LIST_add(stack->list, element);
}

int TSMS_INT_STACK_pop(TSMS_INT_STP stack) {
	int element = TSMS_INT_STACK_peek(stack);
	if (element == -1)
		return -1;
	TSMS_INT_LINK_LIST_deleteNode(stack->list, stack->list->tail);
	return element;
}

int TSMS_INT_STACK_peek(TSMS_INT_STP stack) {
	if (stack == NULL)
		return -1;
	if (stack->list->length == 0)
		return -1;
	return stack->list->tail->element;
}

TSMS_RESULT TSMS_INT_STACK_clear(TSMS_INT_STP stack) {
	if (stack == NULL)
		return TSMS_ERROR;
	return TSMS_INT_LINK_LIST_clear(stack->list);
}

TSMS_RESULT TSMS_INT_STACK_release(TSMS_INT_STP stack) {
	if (stack == NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = TSMS_INT_LINK_LIST_release(stack->list);
	free(stack);
	return result;
}

TSMS_SIZE TSMS_INT_STACK_size(TSMS_INT_STP stack) {
	return stack->list->length;
}

bool TSMS_INT_STACK_empty(TSMS_INT_STP stack) {
	return TSMS_INT_STACK_size(stack) == 0;
}