#ifndef TSMS_MAP_H
#define TSMS_MAP_H

typedef struct TSMS_MAP_NODE * TSMS_MAP_NODE_POINTER;
typedef TSMS_MAP_NODE_POINTER TSMS_MNP;

typedef struct TSMS_MAP * TSMS_MAP_POINTER;
typedef TSMS_MAP_POINTER TSMS_MP;

typedef struct TSMS_MAP_ITERATOR TSMS_MI;
typedef TSMS_MI * TSMS_MIP;

typedef struct TSMS_MAP_ENTRY TSMS_ME;

#include "tsms_def.h"
#include "tsms_function_def.h"

struct TSMS_MAP_ITERATOR {
	TSMS_MP map;
	TSMS_POS current;
	TSMS_MNP next;
};

struct TSMS_MAP_NODE {
	void* key;
	void* value;
	TSMS_MNP next;
};

struct TSMS_MAP {
	TSMS_MNP* base;
	TSMS_SIZE diffusion;
	TSMS_SIZE size;
	TSMS_HASH_FUNCTION hash;
	TSMS_COMPARE_FUNCTION compare;
};

struct TSMS_MAP_ENTRY {
	void* key;
	void* value;
};

extern const TSMS_ME TSMS_EMPTY_MAP_ENTRY;
extern const TSMS_MI TSMS_EMPTY_MAP_ITERATOR;

TSMS_MP TSMS_MAP_create(TSMS_SIZE diffusion, TSMS_HASH_FUNCTION hash, TSMS_COMPARE_FUNCTION compare);

TSMS_RESULT TSMS_MAP_put(TSMS_MP map, void* key, void* value);

void* TSMS_MAP_get(TSMS_MP map, void* key);

TSMS_RESULT TSMS_MAP_remove(TSMS_MP map, void* key);

TSMS_RESULT TSMS_MAP_release(TSMS_MP map);

TSMS_MI TSMS_MAP_iterator(TSMS_MP map);

bool TSMS_MAP_hasNext(TSMS_MIP iter);

TSMS_ME TSMS_MAP_next(TSMS_MIP iter);

TSMS_RESULT TSMS_MAP_clear(TSMS_MP map);

#endif //TSMS_MAP_H
