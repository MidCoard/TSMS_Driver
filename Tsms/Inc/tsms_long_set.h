#ifndef TSMS_LONG_SET_H
#define TSMS_LONG_SET_H

#include "tsms_def.h"
#include "tsms_long_map.h"

extern TSMS_LSI TSMS_EMPTY_LONG_SET_ITERATOR;

struct TSMS_LONG_SET {
	TSMS_LMP map;
};

struct TSMS_LONG_SET_ITERATOR {
	TSMS_LMI mapIterator;
};

TSMS_LSP TSMS_LONG_SET_create(TSMS_SIZE diffusion);

TSMS_RESULT TSMS_LONG_SET_add(TSMS_LSP set, long key);

bool TSMS_LONG_SET_contains(TSMS_LSP set, long key);

TSMS_RESULT TSMS_LONG_SET_remove(TSMS_LSP set, long key);

TSMS_RESULT TSMS_LONG_SET_release(TSMS_LSP set);

TSMS_LSI TSMS_LONG_SET_iterator(TSMS_LSP set);

bool TSMS_LONG_SET_hasNext(TSMS_LSIP iter);

long TSMS_LONG_SET_next(TSMS_LSIP iter);


#endif //TSMS_LONG_SET_H
