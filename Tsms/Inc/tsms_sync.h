#ifndef TSMS_SYNC_H
#define TSMS_SYNC_H

#include "tsms_lock.h"

struct TSMS_SYNC_STACK {
	TSMS_STP stack;
	pLock lock;
};

TSMS_SYNC_STP TSMS_SYNC_STACK_create(TSMS_STP stack);

TSMS_RESULT TSMS_SYNC_STACK_push(TSMS_SYNC_STP stack, void* element);

void* TSMS_SYNC_STACK_pop(TSMS_SYNC_STP stack);

void* TSMS_SYNC_STACK_peek(TSMS_SYNC_STP stack);

TSMS_RESULT TSMS_SYNC_STACK_clear(TSMS_SYNC_STP stack);

TSMS_RESULT TSMS_SYNC_STACK_release(TSMS_SYNC_STP stack);

TSMS_SIZE TSMS_SYNC_STACK_size(TSMS_SYNC_STP stack);

bool TSMS_SYNC_STACK_empty(TSMS_SYNC_STP stack);

#endif //TSMS_SYNC_H
