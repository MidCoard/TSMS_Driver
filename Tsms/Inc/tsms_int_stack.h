#ifndef TSMS_INT_STACK_H
#define TSMS_INT_STACK_H

typedef struct TSMS_INT_STACK *TSMS_INT_STACK_POINTER;
typedef TSMS_INT_STACK_POINTER TSMS_ISTP;

#include "tsms_int_link_list.h"

struct TSMS_INT_STACK {
	TSMS_ILKLP list;
};

TSMS_ISTP TSMS_INT_STACK_create();

TSMS_RESULT TSMS_INT_STACK_push(TSMS_ISTP stack, int element);

int TSMS_INT_STACK_pop(TSMS_ISTP stack);

int TSMS_INT_STACK_peek(TSMS_ISTP stack);

TSMS_RESULT TSMS_INT_STACK_clear(TSMS_ISTP stack);

TSMS_RESULT TSMS_INT_STACK_release(TSMS_ISTP stack);

TSMS_SIZE TSMS_INT_STACK_size(TSMS_ISTP stack);

bool TSMS_INT_STACK_empty(TSMS_ISTP stack);


#endif //TSMS_INT_STACK_H
