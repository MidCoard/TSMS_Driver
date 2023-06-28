#ifndef TSMS_INT_LIST_H
#define TSMS_INT_LIST_H

typedef struct TSMS_INT_LIST *TSMS_INT_LIST_POINTER;
typedef TSMS_INT_LIST_POINTER TSMS_ILP;

#include "tsms_def.h"

struct TSMS_INT_LIST {
	int *list;
	TSMS_SIZE length;
	TSMS_SIZE capacity;
	TSMS_SIZE initLength;
};

TSMS_ILP TSMS_INT_LIST_create(TSMS_SIZE initSize);

TSMS_RESULT TSMS_INT_LIST_add(TSMS_ILP list, int element);

TSMS_RESULT TSMS_INT_LIST_clear(TSMS_ILP list);

TSMS_RESULT TSMS_INT_LIST_release(TSMS_ILP list);

bool TSMS_INT_LIST_contains(TSMS_ILP list, int element);

TSMS_RESULT TSMS_INT_LIST_remove(TSMS_ILP list, TSMS_POS index);

TSMS_POS TSMS_INT_LIST_removeElement(TSMS_ILP list, int element);

#endif //TSMS_INT_LIST_H
