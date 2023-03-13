#ifndef TSMS_LIST_H
#define TSMS_LIST_H

struct TSMS_LIST;

typedef struct TSMS_LIST *TSMS_LIST_POINTER;
typedef TSMS_LIST_POINTER TSMS_LP;

#include "tsms.h"

struct TSMS_LIST {
	void **list;
	TSMS_SIZE length;
	TSMS_SIZE actualLength;
	TSMS_SIZE initLength;
};

TSMS_LP TSMS_LIST_create(TSMS_SIZE initSize);

TSMS_RESULT TSMS_LIST_add(TSMS_LP list, void *element);

TSMS_RESULT TSMS_LIST_remove(TSMS_LP list, TSMS_POS index);

TSMS_POS TSMS_LIST_removeElement(TSMS_LP list, void *element);

TSMS_RESULT TSMS_LIST_release(TSMS_LP list);

TSMS_RESULT TSMS_LIST_clear(TSMS_LP list);

#endif //TSMS_LIST_H
