#include "tsms_set.h"

const bool TSMS_SET_TRUE = true;

TSMS_SP TSMS_SET_create(TSMS_SIZE diffusion, TSMS_MAP_HASH_FUNCTION hash, TSMS_MAP_COMPARE_FUNCTION compare) {
	TSMS_SP set = (TSMS_SP) malloc(sizeof(struct TSMS_SET));
	if (set == TSMS_NULL) {
		tString temp = TSMS_STRING_temp(" malloc failed for set");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	set->map = TSMS_MAP_create(diffusion, hash, compare);
	if (set->map == NULL) {
		free(set);
		return TSMS_NULL;
	}
	return set;
}

TSMS_RESULT TSMS_SET_add(TSMS_SP set, void* key) {
	if (set == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_MAP_put(set->map, key, &TSMS_SET_TRUE);
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