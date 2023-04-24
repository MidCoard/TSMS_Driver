#ifndef TSMS_SET_H
#define TSMS_SET_H

typedef struct TSMS_SET * TSMS_SET_POINTER;
typedef TSMS_SET_POINTER TSMS_SP;

typedef struct TSMS_SET_ITERATOR TSMS_SI;
typedef TSMS_SI * TSMS_SIP;

#include "tsms_map.h"

struct TSMS_SET {
	TSMS_MP map;
};

struct TSMS_SET_ITERATOR {
	TSMS_MI mapIterator;
};

extern const TSMS_SI TSMS_EMPTY_SET_ITERATOR;

TSMS_SP TSMS_SET_create(TSMS_SIZE diffusion, TSMS_HASH_FUNCTION hash, TSMS_COMPARE_FUNCTION compare);

TSMS_RESULT TSMS_SET_add(TSMS_SP set, void* key);

bool TSMS_SET_contains(TSMS_SP set, void* key);

TSMS_RESULT TSMS_SET_remove(TSMS_SP set, void* key);

TSMS_RESULT TSMS_SET_release(TSMS_SP set);

TSMS_SI TSMS_SET_iterator(TSMS_SP set);

bool TSMS_SET_hasNext(TSMS_SIP iter);

void* TSMS_SET_next(TSMS_SIP iter);

#endif //TSMS_SET_H
