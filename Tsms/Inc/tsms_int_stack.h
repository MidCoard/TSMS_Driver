#ifndef TSMS_INT_STACK_H
#define TSMS_INT_STACK_H

#include "tsms_def.h"

struct TSMS_INT_STACK {
	TSMS_INT_LKLP list;
};

TSMS_INT_STP TSMS_INT_STACK_create();

TSMS_RESULT TSMS_INT_STACK_push(TSMS_INT_STP stack, int element);

int TSMS_INT_STACK_pop(TSMS_INT_STP stack);

int TSMS_INT_STACK_peek(TSMS_INT_STP stack);

TSMS_RESULT TSMS_INT_STACK_clear(TSMS_INT_STP stack);

TSMS_RESULT TSMS_INT_STACK_release(TSMS_INT_STP stack);

TSMS_SIZE TSMS_INT_STACK_size(TSMS_INT_STP stack);

bool TSMS_INT_STACK_empty(TSMS_INT_STP stack);


#endif //TSMS_INT_STACK_H
