#include "tsms_map.h"
#include "tsms_long_map.h"

TSMS_MEH TSMS_EMPTY_MAP_ENTRY = {TSMS_NULL, TSMS_NULL};

TSMS_LMEH TSMS_EMPTY_LONG_MAP_ENTRY = {0, TSMS_NULL};

TSMS_MIH TSMS_EMPTY_MAP_ITERATOR = {TSMS_NULL, 0, TSMS_NULL};

TSMS_LMIH TSMS_EMPTY_LONG_MAP_ITERATOR = {TSMS_NULL, 0, TSMS_NULL};

TSMS_INLINE TSMS_MNHP __internal_tsms_create_node(void * key, void * value) {
	TSMS_MNHP node = malloc(sizeof (struct TSMS_MAP_NODE_HANDLER));
	node->key = key;
	node->value = value;
	return node;
}

TSMS_INLINE TSMS_LMNHP __internal_tsms_create_long_node(long key, void * value) {
	TSMS_LMNHP node = malloc(sizeof(struct TSMS_LONG_MAP_NODE_HANDLER));
	node->key = key;
	node->value = value;
	return node;
}

TSMS_MHP TSMS_MAP_create(TSMS_SIZE diffusion, TSMS_MAP_HASH_FUNCTION hash) {
	TSMS_MHP map = malloc(sizeof(struct TSMS_MAP_HANDLER));
	if (map == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for map");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	map->base = malloc(sizeof( TSMS_MNHP) * diffusion);
	map->hash = hash;
	map->diffusion = diffusion;
	map->size = 0;
	return map;
}

TSMS_RESULT TSMS_MAP_put(TSMS_MHP map, void * key, void * value) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	long hash = map->hash(key);
	TSMS_POS offset = (hash % map->diffusion + map->diffusion) % map->diffusion;
	TSMS_MNHP cur = map->base[offset];
	// first if not exist
	if (cur == TSMS_NULL)
		map->base[offset] = __internal_tsms_create_node(key, value);
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
			cur->next = __internal_tsms_create_node(key, value);
	}
	map->size++;
	return TSMS_SUCCESS;
}

void * TSMS_MAP_get(TSMS_MHP map, void * key) {
	if (map == TSMS_NULL)
		return TSMS_NULL;
	long hash = map->hash(key);
	TSMS_POS offset = (hash % map->diffusion + map->diffusion) % map->diffusion;
	TSMS_MNHP cur = map->base[offset];
	while (cur != TSMS_NULL) {
		if (cur->key == key)
			return cur->value;
		cur = cur->next;
	}
	return TSMS_NULL;
}

TSMS_RESULT TSMS_MAP_remove(TSMS_MHP map, void * key) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	long hash = map->hash(key);
	TSMS_POS offset = (hash % map->diffusion + map->diffusion) % map->diffusion;
	TSMS_MNHP cur = map->base[offset];
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
			TSMS_MNHP tmp = cur->next;
			cur->next = cur->next->next;
			free(tmp);
			map->size--;
			return TSMS_SUCCESS;
		} else cur = cur->next;
	}
	return TSMS_FAIL;
}

TSMS_RESULT TSMS_MAP_release(TSMS_MHP map) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_MNHP cur, tmp;
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

TSMS_MIH TSMS_MAP_iterator(TSMS_MHP map) {
	if (map == TSMS_NULL)
		return TSMS_EMPTY_MAP_ITERATOR;
	TSMS_MIH iter = {map, 0, map->base[0]};
	return iter;
}

bool TSMS_MAP_hasNext(TSMS_MIHP iter) {
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

TSMS_MEH TSMS_MAP_next(TSMS_MIHP iter) {
	if (iter == TSMS_NULL || iter == &TSMS_EMPTY_MAP_ITERATOR)
		return TSMS_EMPTY_MAP_ENTRY;
	if (iter->next == TSMS_NULL) {
		TSMS_POS i = iter->current + 1;
		for (; i < iter->map->diffusion; i++)
			if (iter->map->base[i] != TSMS_NULL) {
				iter->current = i;
				iter->next = iter->map->base[i];
				TSMS_MEH entry;
				entry.key = iter->next->key;
				entry.value = iter->next->value;
				iter->next = iter->next->next;
				return entry;
			}
		return TSMS_EMPTY_MAP_ENTRY;
	}
	TSMS_MEH entry;
	entry.key = iter->next->key;
	entry.value = iter->next->value;
	iter->next = iter->next->next;
	return entry;
}

TSMS_RESULT TSMS_MAP_clear(TSMS_MHP map) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_MNHP cur, tmp;
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



TSMS_LMHP TSMS_LONG_MAP_create(TSMS_SIZE diffusion) {
	TSMS_LMHP map = malloc(sizeof(struct TSMS_LONG_MAP_HANDLER));
	if (map == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for map");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	map->base = malloc(sizeof( TSMS_LMNHP) * diffusion);
	map->diffusion = diffusion;
	map->size = 0;
	return map;
}

TSMS_RESULT TSMS_LONG_MAP_put(TSMS_LMHP map, long key, void * value) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_POS offset = (key % map->diffusion + map->diffusion) % map->diffusion;
	TSMS_LMNHP cur = map->base[offset];
	// first if not exist
	if (cur == TSMS_NULL)
		map->base[offset] = __internal_tsms_create_long_node(key, value);
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
			cur->next = __internal_tsms_create_long_node(key, value);
	}
	map->size++;
	return TSMS_SUCCESS;
}

void* TSMS_LONG_MAP_get(TSMS_LMHP map, long key) {
	if (map == TSMS_NULL)
		return TSMS_NULL;
	TSMS_POS offset = (key % map->diffusion + map->diffusion) % map->diffusion;
	TSMS_LMNHP cur = map->base[offset];
	while (cur != TSMS_NULL) {
		if (cur->key == key)
			return cur->value;
		cur = cur->next;
	}
	return TSMS_NULL;
}

TSMS_RESULT TSMS_LONG_MAP_remove(TSMS_LMHP map, long key) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_POS offset = (key % map->diffusion + map->diffusion) % map->diffusion;
	TSMS_LMNHP cur = map->base[offset];
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
			TSMS_LMNHP tmp = cur->next;
			cur->next = cur->next->next;
			free(tmp);
			map->size--;
			return TSMS_SUCCESS;
		} else cur = cur->next;
	}
	return TSMS_FAIL;
}

TSMS_RESULT TSMS_LONG_MAP_release(TSMS_LMHP map) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LMNHP cur, tmp;
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

TSMS_LMIH TSMS_LONG_MAP_iterator(TSMS_LMHP map) {
	if (map == TSMS_NULL)
		return TSMS_EMPTY_LONG_MAP_ITERATOR;
	TSMS_LMIH iter = {map, 0, map->base[0]};
	return iter;
}

bool TSMS_LONG_MAP_hasNext(TSMS_LMIHP iter) {
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

TSMS_LMEH TSMS_LONG_MAP_next(TSMS_LMIHP iter) {
	if (iter == TSMS_NULL || iter == &TSMS_EMPTY_LONG_MAP_ITERATOR)
		return TSMS_EMPTY_LONG_MAP_ENTRY;
	if (iter->next == TSMS_NULL) {
		TSMS_POS i = iter->current + 1;
		for (; i < iter->map->diffusion; i++)
			if (iter->map->base[i] != TSMS_NULL) {
				iter->current = i;
				iter->next = iter->map->base[i];
				TSMS_LMEH entry;
				entry.key = iter->next->key;
				entry.value = iter->next->value;
				iter->next = iter->next->next;
				return entry;
			}
		return TSMS_EMPTY_LONG_MAP_ENTRY;
	}
	TSMS_LMEH entry;
	entry.key = iter->next->key;
	entry.value = iter->next->value;
	iter->next = iter->next->next;
	return entry;
}

TSMS_RESULT TSMS_LONG_MAP_clear(TSMS_LMHP map) {
	if (map == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LMNHP cur, tmp;
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