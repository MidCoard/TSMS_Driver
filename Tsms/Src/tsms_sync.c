#include "tsms_sync.h"
#include "tsms_stack.h"

TSMS_SYNC_STP TSMS_SYNC_STACK_create(TSMS_STP stack) {
	TSMS_SYNC_STP sync_stack = (TSMS_SYNC_STP)malloc(sizeof(struct TSMS_SYNC_STACK));
	if (sync_stack == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for sync stack");
		TSMS_ERR_report(TSMS_ERROR_TYPE_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	sync_stack->stack = stack;
	sync_stack->lock = TSMS_LOCK_create();
	return sync_stack;
}

TSMS_RESULT TSMS_SYNC_STACK_push(TSMS_SYNC_STP stack, void* element) {
	if (stack == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LOCK_lock(stack->lock);
	TSMS_RESULT result = TSMS_STACK_push(stack->stack, element);
	TSMS_LOCK_unlock(stack->lock);
	return result;
}

void* TSMS_SYNC_STACK_pop(TSMS_SYNC_STP stack) {
	if (stack == TSMS_NULL)
		return TSMS_NULL;
	TSMS_LOCK_lock(stack->lock);
	void* element = TSMS_STACK_pop(stack->stack);
	TSMS_LOCK_unlock(stack->lock);
	return element;
}

void* TSMS_SYNC_STACK_peek(TSMS_SYNC_STP stack) {
	if (stack == TSMS_NULL)
		return TSMS_NULL;
	TSMS_LOCK_lock(stack->lock);
	void* element = TSMS_STACK_peek(stack->stack);
	TSMS_LOCK_unlock(stack->lock);
	return element;
}

TSMS_RESULT TSMS_SYNC_STACK_clear(TSMS_SYNC_STP stack) {
	if (stack == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LOCK_lock(stack->lock);
	TSMS_RESULT result = TSMS_STACK_clear(stack->stack);
	TSMS_LOCK_unlock(stack->lock);
	return result;
}

TSMS_RESULT TSMS_SYNC_STACK_release(TSMS_SYNC_STP stack) {
	if (stack == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = TSMS_STACK_release(stack->stack);
	TSMS_LOCK_release(stack->lock);
	free(stack);
	return result;
}

TSMS_SIZE TSMS_SYNC_STACK_size(TSMS_SYNC_STP stack) {
	if (stack == TSMS_NULL)
		return 0;
	TSMS_LOCK_lock(stack->lock);
	TSMS_SIZE size = TSMS_STACK_size(stack->stack);
	TSMS_LOCK_unlock(stack->lock);
	return size;
}

bool TSMS_SYNC_STACK_empty(TSMS_SYNC_STP stack) {
	if (stack == TSMS_NULL)
		return true;
	TSMS_LOCK_lock(stack->lock);
	bool empty = TSMS_STACK_empty(stack->stack);
	TSMS_LOCK_unlock(stack->lock);
	return empty;
}