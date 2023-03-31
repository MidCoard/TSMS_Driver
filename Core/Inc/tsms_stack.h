#ifndef TSMS_STACK_H
#define TSMS_STACK_H


typedef struct TSMS_STACK *TSMS_STACK_POINTER;
typedef TSMS_STACK_POINTER TSMS_STP;

#include "tsms_link_list.h"

struct TSMS_STACK {
	TSMS_LKLP list;
};

TSMS_STP TSMS_STACK_create(TSMS_SIZE initSize);

TSMS_RESULT TSMS_STACK_push(TSMS_STP stack, void* element);

void* TSMS_STACK_pop(TSMS_STP stack);

void* TSMS_STACK_peek(TSMS_STP stack);

TSMS_RESULT TSMS_STACK_clear(TSMS_STP stack);

TSMS_RESULT TSMS_STACK_release(TSMS_STP stack);

TSMS_SIZE TSMS_STACK_size(TSMS_STP stack);

bool TSMS_STACK_empty(TSMS_STP stack);

#endif //TSMS_STACK_H
