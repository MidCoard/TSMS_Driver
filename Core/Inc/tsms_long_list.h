#ifndef TSMS_LONG_LIST_H
#define TSMS_LONG_LIST_H

struct TSMS_LIST_LONG_LIST;

typedef struct TSMS_LIST_LONG_LIST *TSMS_LIST_LONG_LIST_POINTER;
typedef TSMS_LIST_LONG_LIST_POINTER TSMS_LLLP;

#include "tsms.h"

struct TSMS_LIST_LONG_LIST {
	long *list;
	TSMS_SIZE length;
	TSMS_SIZE actualLength;
	TSMS_SIZE initLength;
};

TSMS_LLLP TSMS_LONG_LIST_create(TSMS_SIZE initSize);

TSMS_RESULT TSMS_LONG_LIST_add(TSMS_LLLP list, long element);

TSMS_RESULT TSMS_LONG_LIST_clear(TSMS_LLLP list);

TSMS_RESULT TSMS_LONG_LIST_release(TSMS_LLLP list);

#endif //TSMS_LONG_LIST_H
