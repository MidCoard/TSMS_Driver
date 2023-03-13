#ifndef TSMS_INT_LIST_H
#define TSMS_INT_LIST_H

struct TSMS_INT_LIST;

typedef struct TSMS_INT_LIST *TSMS_INT_LIST_POINTER;
typedef TSMS_INT_LIST_POINTER TSMS_ILP;

#include "tsms.h"

struct TSMS_INT_LIST {
	int *list;
	TSMS_SIZE length;
	TSMS_SIZE actualLength;
	TSMS_SIZE initLength;
};

TSMS_ILP TSMS_INT_LIST_create(TSMS_SIZE initSize);

TSMS_RESULT TSMS_INT_LIST_add(TSMS_ILP list, int element);

TSMS_RESULT TSMS_INT_LIST_clear(TSMS_ILP list);

TSMS_RESULT TSMS_INT_LIST_release(TSMS_ILP list);

#endif //TSMS_INT_LIST_H
