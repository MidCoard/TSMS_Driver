#ifndef TSMS_LONG_MAP_H
#define TSMS_LONG_MAP_H

typedef struct TSMS_LONG_MAP_NODE * TSMS_LONG_MAP_NODE_POINTER;
typedef TSMS_LONG_MAP_NODE_POINTER TSMS_LMNP;

typedef struct TSMS_LONG_MAP * TSMS_LONG_MAP_POINTER;
typedef TSMS_LONG_MAP_POINTER TSMS_LMP;

typedef struct TSMS_LONG_MAP_ITERATOR TSMS_LMI;
typedef TSMS_LMI * TSMS_LMIP;

typedef struct TSMS_LONG_MAP_ENTRY TSMS_LME;

extern TSMS_LMI TSMS_EMPTY_LONG_MAP_ITERATOR;
extern TSMS_LME TSMS_EMPTY_LONG_MAP_ENTRY;

#include "tsms.h"

struct TSMS_LONG_MAP_NODE {
	long key;
	void* value;
	TSMS_LMNP next;
};

struct TSMS_LONG_MAP {
	TSMS_LMNP* base;
	TSMS_SIZE diffusion;
	TSMS_SIZE size;
};

struct TSMS_LONG_MAP_ITERATOR {
	TSMS_LMP map;
	TSMS_POS current;
	TSMS_LMNP next;
};

struct TSMS_LONG_MAP_ENTRY {
	long key;
	void* value;
};

TSMS_LMP TSMS_LONG_MAP_create(TSMS_SIZE diffusion);

TSMS_RESULT TSMS_LONG_MAP_put(TSMS_LMP map, long key, void* value);

void* TSMS_LONG_MAP_get(TSMS_LMP map, long key);

TSMS_RESULT TSMS_LONG_MAP_remove(TSMS_LMP map, long key);

TSMS_RESULT TSMS_LONG_MAP_release(TSMS_LMP map);

TSMS_LMI TSMS_LONG_MAP_iterator(TSMS_LMP map);

bool TSMS_LONG_MAP_hasNext(TSMS_LMIP iter);

TSMS_LME TSMS_LONG_MAP_next(TSMS_LMIP iter);

TSMS_RESULT TSMS_LONG_MAP_clear(TSMS_LMP map);

#endif //TSMS_LONG_MAP_H
