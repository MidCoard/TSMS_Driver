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

struct TSMS_LONG_MAP_NODE_HANDLER;

typedef struct TSMS_LONG_MAP_NODE_HANDLER * TSMS_LONG_MAP_NODE_HANDLER_POINTER;

typedef TSMS_LONG_MAP_NODE_HANDLER_POINTER TSMS_LMNHP;

struct TSMS_LONG_MAP_NODE_HANDLER {
	long key;
	void* value;
	TSMS_LMNHP next;
};

typedef long (*TSMS_MAP_HASH_FUNCTION)(void* key);

struct TSMS_MAP_HANDLER {
	TSMS_MNHP* base;
	TSMS_MAP_HASH_FUNCTION hash;
	TSMS_SIZE diffusion;
	TSMS_SIZE size;
};

struct TSMS_LONG_MAP_HANDLER {
	TSMS_LMNHP* base;
	TSMS_SIZE diffusion;
	TSMS_SIZE size;
};

typedef struct TSMS_MAP_HANDLER * TSMS_MAP_HANDLER_POINTER;

typedef TSMS_MAP_HANDLER_POINTER TSMS_MHP;

typedef struct TSMS_LONG_MAP_HANDLER * TSMS_LONG_MAP_HANDLER_POINTER;

typedef TSMS_LONG_MAP_HANDLER_POINTER TSMS_LMHP;

struct TSMS_MAP_ITERATOR_HANDLER {
	TSMS_MHP map;
	TSMS_POS current;
	TSMS_MNHP next;
};

typedef struct TSMS_MAP_ITERATOR_HANDLER TSMS_MIH;

typedef TSMS_MIH * TSMS_MIHP;

struct TSMS_LONG_MAP_ITERATOR_HANDLER {
	TSMS_LMHP map;
	TSMS_POS current;
	TSMS_LMNHP next;
};

typedef struct TSMS_LONG_MAP_ITERATOR_HANDLER TSMS_LMIH;

typedef TSMS_LMIH * TSMS_LMIHP;

struct TSMS_MAP_ENTRY_HANDLER {
	void* key;
	void* value;
};

typedef struct TSMS_MAP_ENTRY_HANDLER TSMS_MEH;

struct TSMS_LONG_MAP_ENTRY_HANDLER {
	long key;
	void* value;
};

typedef struct TSMS_LONG_MAP_ENTRY_HANDLER TSMS_LMEH;

extern TSMS_MEH TSMS_EMPTY_MAP_ENTRY;

extern TSMS_LMEH TSMS_EMPTY_LONG_MAP_ENTRY;

extern TSMS_MIH TSMS_EMPTY_MAP_ITERATOR;

extern TSMS_LMIH TSMS_EMPTY_LONG_MAP_ITERATOR;

TSMS_MHP TSMS_MAP_createMap(TSMS_SIZE diffusion, TSMS_MAP_HASH_FUNCTION hash);

TSMS_LMHP TSMS_MAP_createLongMap(TSMS_SIZE diffusion);

TSMS_RESULT TSMS_MAP_putMap(TSMS_MHP map, void* key, void* value);

TSMS_RESULT TSMS_MAP_putLongMap(TSMS_LMHP map, long key, void* value);

void* TSMS_MAP_getMap(TSMS_MHP map, void* key);

void* TSMS_MAP_getLongMap(TSMS_LMHP map, long key);

TSMS_RESULT TSMS_MAP_removeMap(TSMS_MHP map, void* key);

TSMS_RESULT TSMS_MAP_removeLongMap(TSMS_LMHP map, long key);

TSMS_RESULT TSMS_MAP_releaseMap(TSMS_MHP map);

TSMS_RESULT TSMS_MAP_releaseLongMap(TSMS_LMHP map);

TSMS_MIH TSMS_MAP_iteratorMap(TSMS_MHP map);

bool TSMS_MAP_hasNextMap(TSMS_MIHP iter);

TSMS_MEH TSMS_MAP_nextMap(TSMS_MIHP iter);

TSMS_LMIH TSMS_MAP_iteratorLongMap(TSMS_LMHP map);

bool TSMS_MAP_hasNextLongMap(TSMS_LMIHP iter);

TSMS_LMEH TSMS_MAP_nextLongMap(TSMS_LMIHP iter);

TSMS_RESULT TSMS_MAP_clearMap(TSMS_MHP map);

TSMS_RESULT TSMS_MAP_clearLongMap(TSMS_LMHP map);



#endif //TSMS_MAP_H