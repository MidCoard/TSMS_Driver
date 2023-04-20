#ifndef TSMS_CHAR_LIST_H
#define TSMS_CHAR_LIST_H

#include "tsms_def.h"

struct TSMS_CHAR_LIST {
	char *list;
	TSMS_SIZE length;
	TSMS_SIZE actualLength;
	TSMS_SIZE initLength;
};

TSMS_CLP TSMS_CHAR_LIST_create(TSMS_SIZE initSize);

TSMS_RESULT TSMS_CHAR_LIST_add(TSMS_CLP list, char element);

TSMS_RESULT TSMS_CHAR_LIST_clear(TSMS_CLP list);

TSMS_RESULT TSMS_CHAR_LIST_release(TSMS_CLP list);

#endif //TSMS_CHAR_LIST_H
