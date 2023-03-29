#ifndef TSMS_LONG_LIST_H
#define TSMS_LONG_LIST_H

struct TSMS_LONG_LIST;

typedef struct TSMS_LONG_LIST *TSMS_LONG_LIST_POINTER;
typedef TSMS_LONG_LIST_POINTER TSMS_LLP;

#include "tsms.h"

struct TSMS_LONG_LIST {
	long *list;
	TSMS_SIZE length;
	TSMS_SIZE actualLength;
	TSMS_SIZE initLength;
};

TSMS_LLP TSMS_LONG_LIST_create(TSMS_SIZE initSize);

TSMS_RESULT TSMS_LONG_LIST_add(TSMS_LLP list, long element);

TSMS_RESULT TSMS_LONG_LIST_clear(TSMS_LLP list);

TSMS_RESULT TSMS_LONG_LIST_release(TSMS_LLP list);

TSMS_RESULT TSMS_LONG_LIST_insert(TSMS_LLP list, long element, TSMS_POS index);

TSMS_RESULT TSMS_LONG_LIST_truncate(TSMS_LLP list, TSMS_POS index);

#endif //TSMS_LONG_LIST_H
