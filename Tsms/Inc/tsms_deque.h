#ifndef TSMS_DEQUE_H
#define TSMS_DEQUE_H

#include "tsms_link_list.h"

typedef struct TSMS_DEQUE *TSMS_DEQUE_POINTER;
typedef TSMS_DEQUE_POINTER TSMS_DP;

struct TSMS_DEQUE {
	TSMS_LKLP list;
};

#include "tsms.h"

TSMS_DP TSMS_DEQUE_create();

TSMS_RESULT TSMS_DEQUE_addFirst(TSMS_DP deque, void* element);

TSMS_RESULT TSMS_DEQUE_addLast(TSMS_DP deque, void* element);

void* TSMS_DEQUE_removeFirst(TSMS_DP deque);

void* TSMS_DEQUE_removeLast(TSMS_DP deque);

void* TSMS_DEQUE_peekFirst(TSMS_DP deque);

void* TSMS_DEQUE_peekLast(TSMS_DP deque);

TSMS_RESULT TSMS_DEQUE_clear(TSMS_DP deque);

TSMS_RESULT TSMS_DEQUE_release(TSMS_DP deque);

TSMS_SIZE TSMS_DEQUE_size(TSMS_DP deque);

bool TSMS_DEQUE_empty(TSMS_DP deque);

#endif //TSMS_DEQUE_H
