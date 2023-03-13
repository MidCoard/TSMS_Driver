#ifndef TSMS_MAP_H
#define TSMS_MAP_H

#include "tsms.h"

struct TSMS_MAP_NODE_HANDLER;

typedef struct TSMS_MAP_NODE_HANDLER * TSMS_MAP_NODE_HANDLER_POINTER;

typedef TSMS_MAP_NODE_HANDLER_POINTER TSMS_MNHP;

struct TSMS_MAP_NODE_HANDLER {
	void* key;
	void* value;
	TSMS_MNHP next;
};

typedef long (*TSMS_MAP_HASH_FUNCTION)(void* key);

struct TSMS_MAP_HANDLER {
	TSMS_MNHP* base;
	TSMS_MAP_HASH_FUNCTION hash;
	TSMS_SIZE diffusion;
	TSMS_SIZE size;
};

typedef struct TSMS_MAP_HANDLER * TSMS_MAP_HANDLER_POINTER;

typedef TSMS_MAP_HANDLER_POINTER TSMS_MHP;

struct TSMS_MAP_ITERATOR_HANDLER {
	TSMS_MHP map;
	TSMS_POS current;
	TSMS_MNHP next;
};

typedef struct TSMS_MAP_ITERATOR_HANDLER TSMS_MIH;

typedef TSMS_MIH * TSMS_MIHP;

struct TSMS_MAP_ENTRY_HANDLER {
	void* key;
	void* value;
};

typedef struct TSMS_MAP_ENTRY_HANDLER TSMS_MEH;

extern TSMS_MEH TSMS_EMPTY_MAP_ENTRY;

extern TSMS_MIH TSMS_EMPTY_MAP_ITERATOR;

TSMS_MHP TSMS_MAP_create(TSMS_SIZE diffusion, TSMS_MAP_HASH_FUNCTION hash);

TSMS_RESULT TSMS_MAP_put(TSMS_MHP map, void* key, void* value);

void* TSMS_MAP_get(TSMS_MHP map, void* key);

TSMS_RESULT TSMS_MAP_remove(TSMS_MHP map, void* key);

TSMS_RESULT TSMS_MAP_release(TSMS_MHP map);

TSMS_MIH TSMS_MAP_iterator(TSMS_MHP map);

bool TSMS_MAP_hasNext(TSMS_MIHP iter);

TSMS_MEH TSMS_MAP_next(TSMS_MIHP iter);

TSMS_RESULT TSMS_MAP_clear(TSMS_MHP map);

#endif //TSMS_MAP_H
