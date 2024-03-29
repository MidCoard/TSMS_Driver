#include "tsms_map.h"
#include "tsms_long_map.h"
#include "tsms.h"

const TSMS_ME TSMS_EMPTY_MAP_ENTRY = {TSMS_NULL, TSMS_NULL};

const TSMS_LME TSMS_EMPTY_LONG_MAP_ENTRY = {0, TSMS_NULL};

const TSMS_MI TSMS_EMPTY_MAP_ITERATOR = {TSMS_NULL, 0, TSMS_NULL};

const TSMS_LMI TSMS_EMPTY_LONG_MAP_ITERATOR = {TSMS_NULL, 0, TSMS_NULL};

TSMS_INLINE TSMS_MNP __tsms_internal_create_node(void * key, void * value) {
	TSMS_MNP node = TSMS_malloc(sizeof (struct TSMS_MAP_NODE));
	if (node == TSMS_NULL)
		return TSMS_NULL;
	node->key = key;
	node->value = value;
	node->next = TSMS_NULL;
	return node;
}

TSMS_INLINE TSMS_LMNP __tsms_internal_create_long_node(long key, void * value) {
	TSMS_LMNP node = TSMS_malloc(sizeof(struct TSMS_LONG_MAP_NODE));
	if (node == TSMS_NULL)
		return TSMS_NULL;
	node->key = key;
	node->value = value;
	node->next = TSMS_NULL;
	return node;
}

TSMS_INLINE bool __tsms_internal_compare(TSMS_MP map, void * key1, void * key2) {
	if (map->compare == TSMS_NULL)
		return key1 == key2;
	return map->compare(key1, key2) == 0;
}

TSMS_MP TSMS_MAP_create(TSMS_SIZE diffusion, TSMS_HASH_FUNCTION hash, TSMS_COMPARE_FUNCTION compare) {
	TSMS_MP map = TSMS_malloc(sizeof(struct TSMS_MAP));
	if (map == TSMS_NULL)
		return TSMS_NULL;
	map->base = TSMS_malloc(sizeof( TSMS_MNP) * diffusion);
	if (map->base == TSMS_NULL) {
		TSMS_MAP_release(map);
		return TSMS_NULL;
	}
	for (TSMS_SIZE i = 0; i < diffusion; i++)
		map->base[i] = TSMS_NULL;
	map->hash = hash;
	map->compare = compare;
	map->diffusion = diffusion;
	map->size = 0;
	return map;
}

TSMS_RESULT TSMS_MAP_put(TSMS_MP map, void * key, void * value) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	if (value == TSMS_NULL)
		return TSMS_MAP_remove(map, key);
	long hash = map->hash(key);
	TSMS_POS offset = (hash % map->diffusion + map->diffusion) % map->diffusion;
	TSMS_MNP cur = map->base[offset];
	// first if not exist
	if (cur == TSMS_NULL)
		map->base[offset] = __tsms_internal_create_node(key, value);
	else {
		// if existed, judge it!
		if (__tsms_internal_compare(map, cur->key, key)) {
			if (cur->value == value)
				return TSMS_FAIL;
			cur->value = value;
			return TSMS_SUCCESS;
		}

		while (cur->next != TSMS_NULL) {
			if (__tsms_internal_compare(map, cur->next->key, key)) {
				if (cur->next->value == value)
					return TSMS_FAIL;
				cur->next->value = value;
				return TSMS_SUCCESS;
			} else cur = cur->next;
		}

		if (cur->next == TSMS_NULL)
			cur->next = __tsms_internal_create_node(key, value);
	}
	map->size++;
	return TSMS_SUCCESS;
}

void * TSMS_MAP_get(TSMS_MP map, void * key) {
	if (map == TSMS_NULL)
		return TSMS_NULL;
	long hash = map->hash(key);
	TSMS_POS offset = (hash % map->diffusion + map->diffusion) % map->diffusion;
	TSMS_MNP cur = map->base[offset];
	while (cur != TSMS_NULL) {
		if (__tsms_internal_compare(map, cur->key, key))
			return cur->value;
		cur = cur->next;
	}
	return TSMS_NULL;
}

TSMS_RESULT TSMS_MAP_remove(TSMS_MP map, void * key) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	long hash = map->hash(key);
	TSMS_POS offset = (hash % map->diffusion + map->diffusion) % map->diffusion;
	TSMS_MNP cur = map->base[offset];
	if (cur == TSMS_NULL)
		return TSMS_FAIL;
	if (__tsms_internal_compare(map, cur->key, key)) {
		map->base[offset] = cur->next;
		free(cur);
		map->size--;
		return TSMS_SUCCESS;
	}
	while (cur->next != TSMS_NULL) {
		if (__tsms_internal_compare(map, cur->next->key, key)) {
			TSMS_MNP tmp = cur->next;
			cur->next = cur->next->next;
			free(tmp);
			map->size--;
			return TSMS_SUCCESS;
		} else cur = cur->next;
	}
	return TSMS_FAIL;
}

TSMS_RESULT TSMS_MAP_release(TSMS_MP map) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_MNP cur, tmp;
	for (TSMS_POS i = 0; i < map->diffusion; i++) {
		cur = map->base[i];
		while (cur != TSMS_NULL) {
			tmp = cur;
			cur = cur->next;
			free(tmp);
		}
	}
	free(map->base);
	free(map);
	return TSMS_SUCCESS;
}

TSMS_MI TSMS_MAP_iterator(TSMS_MP map) {
	if (map == TSMS_NULL)
		return TSMS_EMPTY_MAP_ITERATOR;
	TSMS_MI iter = {map, 0, map->base[0]};
	return iter;
}

bool TSMS_MAP_hasNext(TSMS_MIP iter) {
	if (iter == TSMS_NULL || iter == &TSMS_EMPTY_MAP_ITERATOR)
		return false;
	if (iter->next == TSMS_NULL) {
		TSMS_POS i = iter->current + 1;
		for (; i < iter->map->diffusion; i++)
			if (iter->map->base[i] != TSMS_NULL)
				return true;
		return false;
	}
	return true;
}

TSMS_ME TSMS_MAP_next(TSMS_MIP iter) {
	if (iter == TSMS_NULL || iter == &TSMS_EMPTY_MAP_ITERATOR)
		return TSMS_EMPTY_MAP_ENTRY;
	if (iter->next == TSMS_NULL) {
		TSMS_POS i = iter->current + 1;
		for (; i < iter->map->diffusion; i++)
			if (iter->map->base[i] != TSMS_NULL) {
				iter->current = i;
				iter->next = iter->map->base[i];
				TSMS_ME entry;
				entry.key = iter->next->key;
				entry.value = iter->next->value;
				iter->next = iter->next->next;
				return entry;
			}
		return TSMS_EMPTY_MAP_ENTRY;
	}
	TSMS_ME entry;
	entry.key = iter->next->key;
	entry.value = iter->next->value;
	iter->next = iter->next->next;
	return entry;
}

TSMS_RESULT TSMS_MAP_clear(TSMS_MP map) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_MNP cur, tmp;
	for (TSMS_POS i = 0; i < map->diffusion; i++) {
		cur = map->base[i];
		while (cur != TSMS_NULL) {
			tmp = cur;
			cur = cur->next;
			free(tmp);
		}
		map->base[i] = TSMS_NULL;
	}
	map->size = 0;
	return TSMS_SUCCESS;
}



TSMS_LMP TSMS_LONG_MAP_create(TSMS_SIZE diffusion) {
	TSMS_LMP map = TSMS_malloc(sizeof(struct TSMS_LONG_MAP));
	if (map == TSMS_NULL)
		return TSMS_NULL;
	map->base = TSMS_malloc(sizeof( TSMS_LMNP) * diffusion);
	if (map->base == TSMS_NULL) {
		TSMS_LONG_MAP_release(map);
		return TSMS_NULL;
	}
	map->diffusion = diffusion;
	map->size = 0;
	return map;
}

TSMS_RESULT TSMS_LONG_MAP_put(TSMS_LMP map, long key, void * value) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_POS offset = (key % map->diffusion + map->diffusion) % map->diffusion;
	TSMS_LMNP cur = map->base[offset];
	// first if not exist
	if (cur == TSMS_NULL)
		map->base[offset] = __tsms_internal_create_long_node(key, value);
	else {
		// if existed, judge it!
		if (cur->key == key) {
			if (cur->value == value)
				return TSMS_FAIL;
			cur->value = value;
			return TSMS_SUCCESS;
		}

		while (cur->next != TSMS_NULL) {
			if (cur->next->key == key) {
				if (cur->next->value == value)
					return TSMS_FAIL;
				cur->next->value = value;
				return TSMS_SUCCESS;
			} else cur = cur->next;
		}

		if (cur->next == TSMS_NULL)
			cur->next = __tsms_internal_create_long_node(key, value);
	}
	map->size++;
	return TSMS_SUCCESS;
}

void* TSMS_LONG_MAP_get(TSMS_LMP map, long key) {
	if (map == TSMS_NULL)
		return TSMS_NULL;
	TSMS_POS offset = (key % map->diffusion + map->diffusion) % map->diffusion;
	TSMS_LMNP cur = map->base[offset];
	while (cur != TSMS_NULL) {
		if (cur->key == key)
			return cur->value;
		cur = cur->next;
	}
	return TSMS_NULL;
}

TSMS_RESULT TSMS_LONG_MAP_remove(TSMS_LMP map, long key) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_POS offset = (key % map->diffusion + map->diffusion) % map->diffusion;
	TSMS_LMNP cur = map->base[offset];
	if (cur == TSMS_NULL)
		return TSMS_FAIL;
	if (cur->key == key) {
		map->base[offset] = cur->next;
		free(cur);
		map->size--;
		return TSMS_SUCCESS;
	}
	while (cur->next != TSMS_NULL) {
		if (cur->next->key == key) {
			TSMS_LMNP tmp = cur->next;
			cur->next = cur->next->next;
			free(tmp);
			map->size--;
			return TSMS_SUCCESS;
		} else cur = cur->next;
	}
	return TSMS_FAIL;
}

TSMS_RESULT TSMS_LONG_MAP_release(TSMS_LMP map) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LMNP cur, tmp;
	for (TSMS_POS i = 0; i < map->diffusion; i++) {
		cur = map->base[i];
		while (cur != TSMS_NULL) {
			tmp = cur;
			cur = cur->next;
			free(tmp);
		}
	}
	free(map->base);
	free(map);
	return TSMS_SUCCESS;
}

TSMS_LMI TSMS_LONG_MAP_iterator(TSMS_LMP map) {
	if (map == TSMS_NULL)
		return TSMS_EMPTY_LONG_MAP_ITERATOR;
	TSMS_LMI iter = {map, 0, map->base[0]};
	return iter;
}

bool TSMS_LONG_MAP_hasNext(TSMS_LMIP iter) {
	if (iter == TSMS_NULL || iter == &TSMS_EMPTY_LONG_MAP_ITERATOR)
		return false;
	if (iter->next == TSMS_NULL) {
		TSMS_POS i = iter->current + 1;
		for (; i < iter->map->diffusion; i++)
			if (iter->map->base[i] != TSMS_NULL)
				return true;
		return false;
	}
	return true;
}

TSMS_LME TSMS_LONG_MAP_next(TSMS_LMIP iter) {
	if (iter == TSMS_NULL || iter == &TSMS_EMPTY_LONG_MAP_ITERATOR)
		return TSMS_EMPTY_LONG_MAP_ENTRY;
	if (iter->next == TSMS_NULL) {
		TSMS_POS i = iter->current + 1;
		for (; i < iter->map->diffusion; i++)
			if (iter->map->base[i] != TSMS_NULL) {
				iter->current = i;
				iter->next = iter->map->base[i];
				TSMS_LME entry;
				entry.key = iter->next->key;
				entry.value = iter->next->value;
				iter->next = iter->next->next;
				return entry;
			}
		return TSMS_EMPTY_LONG_MAP_ENTRY;
	}
	TSMS_LME entry;
	entry.key = iter->next->key;
	entry.value = iter->next->value;
	iter->next = iter->next->next;
	return entry;
}

TSMS_RESULT TSMS_LONG_MAP_clear(TSMS_LMP map) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LMNP cur, tmp;
	for (TSMS_POS i = 0; i < map->diffusion; i++) {
		cur = map->base[i];
		while (cur != TSMS_NULL) {
			tmp = cur;
			cur = cur->next;
			free(tmp);
		}
		map->base[i] = TSMS_NULL;
	}
	map->size = 0;
	return TSMS_SUCCESS;
}
