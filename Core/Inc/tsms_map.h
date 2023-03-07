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

TSMS_MHP TSMS_MAP_create(TSMS_SIZE diffusion, TSMS_MAP_HASH_FUNCTION hash);

TSMS_RESULT TSMS_MAP_put(TSMS_MHP map, void* key, void* value);

void* TSMS_MAP_get(TSMS_MHP map, void* key);

TSMS_RESULT TSMS_MAP_remove(TSMS_MHP map, void* key);

TSMS_RESULT TSMS_MAP_release(TSMS_MHP map);

TSMS_LLP TSMS_MAP_keys(TSMS_MHP map);

TSMS_LLP TSMS_MAP_values(TSMS_MHP map);

TSMS_RESULT TSMS_MAP_clear(TSMS_MHP map);

#endif //TSMS_MAP_H
