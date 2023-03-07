#include "tsms_map.h"

TSMS_INLINE TSMS_MNHP __internal_tsms_create_node(void * key, void * value) {
	TSMS_MNHP node = malloc(sizeof (struct TSMS_MAP_NODE_HANDLER));
	node->key = key;
	node->value = value;
	return node;
}

TSMS_MHP TSMS_MAP_create(TSMS_SIZE diffusion, TSMS_MAP_HASH_FUNCTION hash) {
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

TSMS_LLP TSMS_MAP_keys(TSMS_MHP map) {
	if (map == TSMS_NULL)
		return TSMS_NULL;
	TSMS_LLP list = TSMS_LIST_createList(map->size);
	TSMS_MNHP cur;
	for (TSMS_POS i = 0; i < map->diffusion; i++) {
		cur = map->base[i];
		while (cur != TSMS_NULL) {
			TSMS_LIST_addList(list, cur->key);
			cur = cur->next;
		}
	}
	return list;
}

TSMS_LLP TSMS_MAP_values(TSMS_MHP map) {
	if (map == TSMS_NULL)
		return TSMS_NULL;
	TSMS_LLP list = TSMS_LIST_createList(map->size);
	TSMS_MNHP cur;
	for (TSMS_POS i = 0; i < map->diffusion; i++) {
		cur = map->base[i];
		while (cur != TSMS_NULL) {
			TSMS_LIST_addList(list, cur->value);
			cur = cur->next;
		}
	}
	return list;
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