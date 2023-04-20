#ifndef TSMS_INT_LIST_H
#define TSMS_INT_LIST_H

#include "tsms_def.h"

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
