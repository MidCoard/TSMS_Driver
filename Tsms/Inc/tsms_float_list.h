#ifndef TSMS_FLOAT_LIST_H
#define TSMS_FLOAT_LIST_H

typedef struct TSMS_FLOAT_LIST *TSMS_FLOAT_LIST_POINTER;
typedef TSMS_FLOAT_LIST_POINTER TSMS_FLP;

#include "tsms_def.h"

struct TSMS_FLOAT_LIST {
	float *list;
	TSMS_SIZE length;
	TSMS_SIZE actualLength;
	TSMS_SIZE initLength;
};

TSMS_FLP TSMS_FLOAT_LIST_create(TSMS_SIZE initSize);

TSMS_RESULT TSMS_FLOAT_LIST_add(TSMS_FLP list, float element);

TSMS_RESULT TSMS_FLOAT_LIST_clear(TSMS_FLP list);

TSMS_RESULT TSMS_FLOAT_LIST_release(TSMS_FLP list);

bool TSMS_FLOAT_LIST_contains(TSMS_FLP list, float element);

TSMS_RESULT TSMS_FLOAT_LIST_remove(TSMS_FLP list, TSMS_POS index);

TSMS_POS TSMS_FLOAT_LIST_removeElement(TSMS_FLP list, float element);

#endif //TSMS_FLOAT_LIST_H
