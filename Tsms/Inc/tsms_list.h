#ifndef TSMS_LIST_H
#define TSMS_LIST_H

#include "tsms_def.h"

struct TSMS_LIST {
	void **list;
	TSMS_SIZE length;
	TSMS_SIZE actualLength;
	TSMS_SIZE initLength;
};

extern TSMS_LP TSMS_EMPTY_LIST;

TSMS_LP TSMS_LIST_create(TSMS_SIZE initSize);

TSMS_RESULT TSMS_LIST_add(TSMS_LP list, void *element);

TSMS_RESULT TSMS_LIST_remove(TSMS_LP list, TSMS_POS index);

TSMS_POS TSMS_LIST_removeElement(TSMS_LP list, void *element);

TSMS_RESULT TSMS_LIST_release(TSMS_LP list);

TSMS_RESULT TSMS_LIST_clear(TSMS_LP list);

#endif //TSMS_LIST_H
