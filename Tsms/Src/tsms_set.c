#include "tsms_set.h"
#include "tsms_long_set.h"
#include "tsms.h"

const bool _setTrue = true;

const TSMS_SI TSMS_EMPTY_SET_ITERATOR = {{TSMS_NULL, 0, TSMS_NULL}};

long TSMS_SET_defaultHash(void* key) {
	return (long) key;
}

long TSMS_SET_defaultCompare(void* key1, void* key2) {
	return key1 - key2;
}

TSMS_SP TSMS_SET_create(TSMS_SIZE diffusion, TSMS_HASH_FUNCTION hash, TSMS_COMPARE_FUNCTION compare) {
	TSMS_SP set = (TSMS_SP) TSMS_malloc(sizeof(struct TSMS_SET));
	if (set == TSMS_NULL)
		return TSMS_NULL;
	set->map = TSMS_MAP_create(diffusion, hash, compare);
	if (set->map == NULL) {
		TSMS_SET_release(set);
		return TSMS_NULL;
	}
	return set;
}

TSMS_RESULT TSMS_SET_add(TSMS_SP set, void* key) {
	if (set == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_MAP_put(set->map, key, &_setTrue);
}

bool TSMS_SET_contains(TSMS_SP set, void* key) {
	if (set == TSMS_NULL)
		return false;
	return TSMS_MAP_get(set->map, key) != TSMS_NULL;
}

TSMS_RESULT TSMS_SET_remove(TSMS_SP set, void* key) {
	if (set == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_MAP_remove(set->map, key);
}

TSMS_RESULT TSMS_SET_release(TSMS_SP set) {
	if (set == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = TSMS_MAP_release(set->map);
	free(set);
	return result;
}

TSMS_SI TSMS_SET_iterator(TSMS_SP set) {
	TSMS_SI iter;
	iter.mapIterator = TSMS_MAP_iterator(set->map);
	return iter;
}

bool TSMS_SET_hasNext(TSMS_SIP iter) {
	return TSMS_MAP_hasNext(&iter->mapIterator);
}

void* TSMS_SET_next(TSMS_SIP iter) {
	return TSMS_MAP_next(&iter->mapIterator).key;
}


const TSMS_LSI TSMS_EMPTY_LONG_SET_ITERATOR = {{TSMS_NULL, 0, TSMS_NULL}};

TSMS_LSP TSMS_LONG_SET_create(TSMS_SIZE diffusion) {
	TSMS_LSP set = (TSMS_LSP) TSMS_malloc(sizeof(struct TSMS_LONG_SET));
	if (set == TSMS_NULL)
		return TSMS_NULL;
	set->map = TSMS_LONG_MAP_create(diffusion);
	if (set->map == NULL) {
		TSMS_LONG_SET_release(set);
		return TSMS_NULL;
	}
	return set;
}

TSMS_RESULT TSMS_LONG_SET_add(TSMS_LSP set, long key) {
	if (set == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_LONG_MAP_put(set->map, key, &_setTrue);
}

bool TSMS_LONG_SET_contains(TSMS_LSP set, long key) {
	if (set == TSMS_NULL)
		return false;
	return TSMS_LONG_MAP_get(set->map, key) != TSMS_NULL;
}

TSMS_RESULT TSMS_LONG_SET_remove(TSMS_LSP set, long key) {
	if (set == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_LONG_MAP_remove(set->map, key);
}

TSMS_RESULT TSMS_LONG_SET_release(TSMS_LSP set) {
	if (set == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = TSMS_LONG_MAP_release(set->map);
	free(set);
	return result;
}

TSMS_LSI TSMS_LONG_SET_iterator(TSMS_LSP set) {
	if (set == TSMS_NULL)
		return TSMS_EMPTY_LONG_SET_ITERATOR;
	TSMS_LSI iter;
	iter.mapIterator = TSMS_LONG_MAP_iterator(set->map);
	return iter;
}

bool TSMS_LONG_SET_hasNext(TSMS_LSIP iter) {
	return TSMS_LONG_MAP_hasNext(&iter->mapIterator);
}

long TSMS_LONG_SET_next(TSMS_LSIP iter) {
	return TSMS_LONG_MAP_next(&iter->mapIterator).key;
}