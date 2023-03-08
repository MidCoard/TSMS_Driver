#include "tsms_map.h"

TSMS_INLINE TSMS_MNHP __internal_tsms_create_node(void * key, void * value) {
	TSMS_MNHP node = malloc(sizeof (struct TSMS_MAP_NODE_HANDLER));
	node->key = key;
	node->value = value;
	return node;
}

TSMS_INLINE TSMS_LMNHP __internal_tsms_create_long_node(long key, void * value) {
	TSMS_LMNHP node = malloc(sizeof (struct TSMS_LONG_MAP_NODE_HANDLER));
	node->key = key;
	node->value = value;
	return node;
}

TSMS_LONG TSMS_long(long value) {
	TSMS_LONG result = malloc(sizeof (struct TSMS_PACKED_LONG));
	result->value = value;
	return result;
}

TSMS_MHP TSMS_MAP_createMap(TSMS_SIZE diffusion, TSMS_MAP_HASH_FUNCTION hash) {
	TSMS_MHP map = malloc(sizeof(struct TSMS_MAP_HANDLER));
	if (map == TSMS_NULL) {
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, TSMS_STRING_static("malloc failed for map"));
		return TSMS_NULL;
	}
	map->base = malloc(sizeof( TSMS_MNHP) * diffusion);
	map->hash = hash;
	map->diffusion = diffusion;
	map->size = 0;
	return map;
}

TSMS_LMHP TSMS_MAP_createLongMap(TSMS_SIZE diffusion) {
	TSMS_LMHP map = malloc(sizeof(struct TSMS_LONG_MAP_HANDLER));
	if (map == TSMS_NULL) {
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, TSMS_STRING_static("malloc failed for map"));
		return TSMS_NULL;
	}
	map->base = malloc(sizeof( TSMS_LMNHP) * diffusion);
	map->diffusion = diffusion;
	map->size = 0;
	return map;
}

TSMS_RESULT TSMS_MAP_putMap(TSMS_MHP map, void * key, void * value) {
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

TSMS_RESULT TSMS_MAP_putLongMap(TSMS_LMHP map, long key, void * value) {
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

void * TSMS_MAP_getMap(TSMS_MHP map, void * key) {
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

void* TSMS_MAP_getLongMap(TSMS_LMHP map, long key) {
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

TSMS_RESULT TSMS_MAP_removeMap(TSMS_MHP map, void * key) {
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

TSMS_RESULT TSMS_MAP_removeLongMap(TSMS_LMHP map, long key) {
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

TSMS_RESULT TSMS_MAP_releaseMap(TSMS_MHP map) {
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

TSMS_RESULT TSMS_MAP_releaseLongMap(TSMS_LMHP map) {
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

TSMS_MIHP TSMS_MAP_iteratorMap(TSMS_MHP map) {
	if (map == TSMS_NULL)
		return TSMS_NULL;
	TSMS_MIHP iter = malloc(sizeof(struct TSMS_MAP_ITERATOR_HANDLER));
	iter->map = map;
	iter->current = 0;
	iter->next = map->base[0];
	return iter;
}

bool TSMS_MAP_hasNextMap(TSMS_MIHP iter) {
	if (iter == TSMS_NULL)
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

void * TSMS_MAP_nextMapKey(TSMS_MIHP iter) {
	if (iter == TSMS_NULL)
		return TSMS_NULL;
	if (iter->next == TSMS_NULL) {
		TSMS_POS i = iter->current + 1;
		for (; i < iter->map->diffusion; i++)
			if (iter->map->base[i] != TSMS_NULL) {
				iter->current = i;
				iter->next = iter->map->base[i];
				void * key = iter->next->key;
				iter->next = iter->next->next;
				return key;
			}
		return TSMS_NULL;
	}
	void * key = iter->next->key;
	iter->next = iter->next->next;
	return key;
}

void* TSMS_MAP_nextMapValue(TSMS_MIHP iter) {
	if (iter == TSMS_NULL)
		return TSMS_NULL;
	if (iter->next == TSMS_NULL) {
		TSMS_POS i = iter->current + 1;
		for (; i < iter->map->diffusion; i++)
			if (iter->map->base[i] != TSMS_NULL) {
				iter->current = i;
				iter->next = iter->map->base[i];
				void * value = iter->next->value;
				iter->next = iter->next->next;
				return value;
			}
		return TSMS_NULL;
	}
	void * value = iter->next->value;
	iter->next = iter->next->next;
	return value;
}

TSMS_LMIHP TSMS_MAP_iteratorLongMap(TSMS_LMHP map) {
	if (map == TSMS_NULL)
		return TSMS_NULL;
	TSMS_LMIHP iter = malloc(sizeof(struct TSMS_LONG_MAP_ITERATOR_HANDLER));
	iter->map = map;
	iter->current = 0;
	iter->next = map->base[0];
	return iter;
}

bool TSMS_MAP_hasNextLongMap(TSMS_LMIHP iter) {
	if (iter == TSMS_NULL)
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

TSMS_LONG TSMS_MAP_nextLongMapKey(TSMS_LMIHP iter) {
	if (iter == TSMS_NULL)
		return TSMS_NULL;
	if (iter->next == TSMS_NULL) {
		TSMS_POS i = iter->current + 1;
		for (; i < iter->map->diffusion; i++)
			if (iter->map->base[i] != TSMS_NULL) {
				iter->current = i;
				iter->next = iter->map->base[i];
				long key = iter->next->key;
				iter->next = iter->next->next;
				return TSMS_long(key);
			}
		return TSMS_NULL;
	}
	long key = iter->next->key;
	iter->next = iter->next->next;
	return TSMS_long(key);
}

TSMS_RESULT TSMS_MAP_clearMap(TSMS_MHP map) {
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

TSMS_RESULT TSMS_MAP_clearLongMap(TSMS_LMHP map) {
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