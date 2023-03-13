#ifndef TSMS_MAP_H
#define TSMS_MAP_H

#include "tsms.h"

struct TSMS_MAP_NODE;

typedef struct TSMS_MAP_NODE * TSMS_MAP_NODE_POINTER;

typedef TSMS_MAP_NODE_POINTER TSMS_MNP;

struct TSMS_MAP_NODE {
	void* key;
	void* value;
	TSMS_MNP next;
};

typedef long (*TSMS_MAP_HASH_FUNCTION)(void* key);

struct TSMS_MAP {
	TSMS_MNP* base;
	TSMS_MAP_HASH_FUNCTION hash;
	TSMS_SIZE diffusion;
	TSMS_SIZE size;
};

typedef struct TSMS_MAP * TSMS_MAP_POINTER;

typedef TSMS_MAP_POINTER TSMS_MP;

struct TSMS_MAP_ITERATOR {
	TSMS_MP map;
	TSMS_POS current;
	TSMS_MNP next;
};

typedef struct TSMS_MAP_ITERATOR TSMS_MI;

typedef TSMS_MI * TSMS_MIP;

struct TSMS_MAP_ENTRY {
	void* key;
	void* value;
};

typedef struct TSMS_MAP_ENTRY TSMS_ME;

extern TSMS_ME TSMS_EMPTY_MAP_ENTRY;

extern TSMS_MI TSMS_EMPTY_MAP_ITERATOR;

TSMS_MP TSMS_MAP_create(TSMS_SIZE diffusion, TSMS_MAP_HASH_FUNCTION hash);

TSMS_RESULT TSMS_MAP_put(TSMS_MP map, void* key, void* value);

void* TSMS_MAP_get(TSMS_MP map, void* key);

TSMS_RESULT TSMS_MAP_remove(TSMS_MP map, void* key);

TSMS_RESULT TSMS_MAP_release(TSMS_MP map);

TSMS_MI TSMS_MAP_iterator(TSMS_MP map);

bool TSMS_MAP_hasNext(TSMS_MIP iter);

TSMS_ME TSMS_MAP_next(TSMS_MIP iter);

TSMS_RESULT TSMS_MAP_clear(TSMS_MP map);

#endif //TSMS_MAP_H
