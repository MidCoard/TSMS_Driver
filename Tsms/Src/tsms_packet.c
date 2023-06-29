#include "comm/tsms_packet.h"
#include "tsms_util.h"
#include "tsms_int_list.h"

static TSMS_SIZE _packetTypeSize[] = {4, 4, 0, 1, 1, 8, 8, 2, 1, 0, 0, 0};

TSMS_INLINE long __tsms_internal_checksum(const uint8_t* data, TSMS_SIZE size) {
	long sum = 0;
	for (TSMS_SIZE i = 0; i < size; i++)
		sum += data[i];
	return sum;
}

TSMS_INLINE TSMS_RESULT __tsms_internal_packet_write(pPacket packet, void* data, TSMS_SIZE size) {
	if (packet->capacity < packet->size + size) {
		void* temp = TSMS_realloc(packet->data, packet->size + size * 2);
		if (temp == TSMS_NULL) {
			temp = TSMS_realloc(packet->data, packet->size + size);
			if (temp == TSMS_NULL)
				return TSMS_ERROR;
			packet->capacity = packet->size + size;
		} else
			packet->capacity = packet->size + size * 2;
		packet->data = temp;
	}
	memcpy(packet->data + packet->size, data, size);
	packet->size += size;
	return TSMS_SUCCESS;
}

TSMS_INLINE TSMS_RESULT __tsms_internal_packet_write_byte(pPacket packet, uint8_t data) {
	return __tsms_internal_packet_write(packet, &data, sizeof (uint8_t));
}

TSMS_INLINE TSMS_RESULT __tsms_internal_packet_write_int(pPacket packet, uint32_t data) {
	return __tsms_internal_packet_write(packet, &data, sizeof (uint32_t));
}

TSMS_INLINE TSMS_RESULT __tsms_internal_packet_write_long(pPacket packet, uint64_t data) {
	return __tsms_internal_packet_write(packet, &data, sizeof (uint64_t));
}

TSMS_INLINE TSMS_RESULT __tsms_internal_packet_write_bytes(pPacket packet, void* data, TSMS_SIZE size) {
	return __tsms_internal_packet_write(packet, data, size);
}

TSMS_INLINE TSMS_RESULT __tsms_internal_packet_write_string(pPacket packet, pString str) {
	TSMS_RESULT result = __tsms_internal_packet_write_int(packet, str->length);
	if (result != TSMS_SUCCESS)
		return TSMS_ERROR;
	return __tsms_internal_packet_write_bytes(packet, str->cStr, str->length);
}

TSMS_INLINE void* __tsms_internal_packet_read(uint8_t* data, TSMS_POS position, TSMS_SIZE size, TSMS_SIZE typeSize) {
	if (position + typeSize > size)
		return TSMS_NULL;
	return data + position;
}

TSMS_INLINE uint8_t __tsms_internal_packet_read_byte(uint8_t* data, TSMS_POS position, TSMS_SIZE size) {
	return *((uint8_t *) __tsms_internal_packet_read(data, position, size, sizeof (uint8_t)));
}

TSMS_INLINE uint8_t * __tsms_internal_packet_read_bytes(uint8_t* data, TSMS_POS position, TSMS_SIZE size, TSMS_SIZE length) {
	return (uint8_t *) __tsms_internal_packet_read(data, position, size, length);
}

TSMS_INLINE uint32_t __tsms_internal_packet_read_int(uint8_t* data, TSMS_POS position, TSMS_SIZE size) {
	return *((uint32_t *) __tsms_internal_packet_read(data, position, size, sizeof(uint32_t)));
}

TSMS_INLINE uint64_t __tsms_internal_packet_read_long(uint8_t* data, TSMS_POS position, TSMS_SIZE size) {
	return *((uint64_t *) __tsms_internal_packet_read(data, position, size, sizeof (uint64_t)));
}

TSMS_INLINE pString __tsms_internal_packet_read_string(uint8_t* data, TSMS_POS position, TSMS_SIZE size) {
	uint32_t length = __tsms_internal_packet_read_int(data, position, size);
	if (length == 0)
		return TSMS_STRING_create();
	return TSMS_STRING_createWithFixSize(__tsms_internal_packet_read_bytes(data, position + sizeof (uint32_t), size, length), length);
}

TSMS_INLINE TSMS_RESULT __tsms_internal_packet_resize(pPacket packet) {
	void* temp = TSMS_realloc(packet->data, packet->size);
	if (temp == TSMS_NULL)
		return TSMS_ERROR;
	packet->data = temp;
	packet->capacity = packet->size;
	return TSMS_SUCCESS;
}

pPacketBuilder TSMS_PACKET_BUILDER_create(pString protocol, TSMS_CHECKSUM_FUNCTION checksum) {
	pPacketBuilder builder = (pPacketBuilder) TSMS_malloc(sizeof(tPacketBuilder));
	if (builder == TSMS_NULL)
		return TSMS_NULL;
	builder->data = TSMS_LONG_MAP_create(16);
	TSMS_LP list = TSMS_STRING_split(protocol, '|');
	builder->typesLength = list->length;
	builder->types = (TSMS_TYPE *) TSMS_malloc(sizeof(TSMS_TYPE) * builder->typesLength);
	builder->statics = TSMS_LIST_create(0);
	for (TSMS_POS i = 0; i < list->length; i++) {
		pString str = (pString) list->list[i];
		if (TSMS_STRING_equals(str, TSMS_STRING_static("I")) || TSMS_STRING_equals(str, TSMS_STRING_static("i")) ||
		    TSMS_STRING_equals(str, TSMS_STRING_static("int")) || TSMS_STRING_equals(str, TSMS_STRING_static("INT")))
			builder->types[i] = TSMS_TYPE_INT;
		else if (TSMS_STRING_equals(str, TSMS_STRING_static("F")) || TSMS_STRING_equals(str, TSMS_STRING_static("f")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("float")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("FLOAT")))
			builder->types[i] = TSMS_TYPE_FLOAT;
		else if (TSMS_STRING_equals(str, TSMS_STRING_static("S")) || TSMS_STRING_equals(str, TSMS_STRING_static("s")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("string")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("STRING")))
			builder->types[i] = TSMS_TYPE_STRING;
		else if (TSMS_STRING_equals(str, TSMS_STRING_static("C")) || TSMS_STRING_equals(str, TSMS_STRING_static("c")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("char")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("CHAR")))
			builder->types[i] = TSMS_TYPE_CHAR;
		else if (TSMS_STRING_equals(str, TSMS_STRING_static("B")) || TSMS_STRING_equals(str, TSMS_STRING_static("b")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("bool")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("BOOL")))
			builder->types[i] = TSMS_TYPE_BOOL;
		else if (TSMS_STRING_equals(str, TSMS_STRING_static("D")) || TSMS_STRING_equals(str, TSMS_STRING_static("d")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("double")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("DOUBLE")))
			builder->types[i] = TSMS_TYPE_DOUBLE;
		else if (TSMS_STRING_equals(str, TSMS_STRING_static("L")) || TSMS_STRING_equals(str, TSMS_STRING_static("l")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("long")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("LONG")))
			builder->types[i] = TSMS_TYPE_LONG;
		else if (TSMS_STRING_equals(str, TSMS_STRING_static("H")) || TSMS_STRING_equals(str, TSMS_STRING_static("h")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("short")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("SHORT")))
			builder->types[i] = TSMS_TYPE_SHORT;
		else if (TSMS_STRING_equals(str, TSMS_STRING_static("Y")) || TSMS_STRING_equals(str, TSMS_STRING_static("y")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("byte")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("BYTE")))
			builder->types[i] = TSMS_TYPE_BYTE;
		else if (TSMS_STRING_equals(str, TSMS_STRING_static("R")) || TSMS_STRING_equals(str, TSMS_STRING_static("r")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("reserved")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("RESERVED")))
			builder->types[i] = TSMS_TYPE_RESERVED;
		else if (TSMS_STRING_equals(str, TSMS_STRING_static("CS")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("cs")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("checksum")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("CHECKSUM")))
			builder->types[i] = TSMS_TYPE_CHECKSUM;
		else {
			builder->types[i] = TSMS_TYPE_STATIC;
			TSMS_LIST_add(builder->statics, TSMS_STRING_unescape(str));
		}
		TSMS_STRING_release(str);
	}
	TSMS_LIST_release(list);
	builder->checksum = checksum == TSMS_NULL ? __tsms_internal_checksum : checksum;
	return builder;
}

pPacketBuilder TSMS_PACKET_BUILDER_createEmpty() {
	pPacketBuilder builder = TSMS_malloc(sizeof(tPacketBuilder));
	if (builder == TSMS_NULL)
		return TSMS_NULL;
	builder->data = TSMS_LONG_MAP_create(16);
	builder->types = TSMS_NULL;
	builder->statics = TSMS_NULL;
	builder->checksum = __tsms_internal_checksum;
	return builder;
}

TSMS_RESULT TSMS_PACKET_BUILDER_clear(pPacketBuilder builder) {
	if (builder == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LMI it = TSMS_LONG_MAP_iterator(builder->data);
	while (TSMS_LONG_MAP_hasNext(&it)) {
		TSMS_LME next = TSMS_LONG_MAP_next(&it);
		TSMS_LP list = next.value;
		if (next.key == TSMS_TYPE_STRING)
			for (TSMS_SIZE i = 0; i < list->length; i++) {
				TSMS_STRING_release(*(pString *) list->list[i]);
				free(list->list[i]);
			}
		else if (list->length != 0)
			for (TSMS_SIZE i = 0; i < list->length; i++)
				free(list->list[i]);
		TSMS_LIST_release(list);
	}
	TSMS_LONG_MAP_clear(builder->data);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_PACKET_BUILDER_release(pPacketBuilder builder) {
	if (builder == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_PACKET_BUILDER_clear(builder);
	TSMS_LONG_MAP_release(builder->data);
	if (builder->statics != TSMS_NULL)
		for (TSMS_SIZE i = 0; i < builder->statics->length; i++)
			TSMS_STRING_release(builder->statics->list[i]);
	TSMS_LIST_release(builder->statics);
	free(builder->types);
	free(builder);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_PACKET_BUILDER_write(pPacketBuilder builder, TSMS_TYPE type, void* data, TSMS_SIZE size) {
	if (builder == TSMS_NULL)
		return TSMS_ERROR;
	void* clone = TSMS_clone(data, size);
	TSMS_LP d = TSMS_LONG_MAP_get(builder->data, type);
	if (d == TSMS_NULL)
		TSMS_LONG_MAP_put(builder->data, type, d = TSMS_LIST_create(0));
	TSMS_PAIR * pair = TSMS_malloc(sizeof(TSMS_PAIR));
	if (pair == TSMS_NULL) {
		free(clone);
		return TSMS_ERROR;
	}
	TSMS_LIST_add(d, clone);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_PACKET_BUILDER_writeInt(pPacketBuilder builder, uint32_t data) {
	return TSMS_PACKET_BUILDER_write(builder, TSMS_TYPE_INT, &data, sizeof(uint32_t));
}

TSMS_RESULT TSMS_PACKET_BUILDER_writeFloat(pPacketBuilder builder, float data) {
	return TSMS_PACKET_BUILDER_write(builder, TSMS_TYPE_FLOAT, &data, sizeof(float));
}

TSMS_RESULT TSMS_PACKET_BUILDER_writePointer(pPacketBuilder builder, TSMS_TYPE type, void* data) {
	return TSMS_PACKET_BUILDER_write(builder, type, &data, sizeof(void*));
}

TSMS_RESULT TSMS_PACKET_BUILDER_writeString(pPacketBuilder builder, pString data) {
	return TSMS_PACKET_BUILDER_writePointer(builder, TSMS_TYPE_STRING, TSMS_STRING_clone(data));
}

TSMS_RESULT TSMS_PACKET_BUILDER_writeChar(pPacketBuilder builder, char data) {
	return TSMS_PACKET_BUILDER_write(builder, TSMS_TYPE_CHAR, &data, sizeof(char));
}

pPacket TSMS_PACKET_BUILDER_resolve(pPacketBuilder builder, uint8_t* data, TSMS_SIZE size) {
	if (builder == TSMS_NULL)
		return TSMS_NULL;
	pPacket packet = TSMS_malloc(sizeof(tPacket));
	if (packet == TSMS_NULL)
		return TSMS_NULL;
	packet->size = 0;
	packet->capacity = 0;
	packet->offset = 0;
	if (builder->types == TSMS_NULL) {
		packet->data = TSMS_malloc(size);
		if (packet->data == TSMS_NULL) {
			TSMS_PACKET_release(packet);
			return TSMS_NULL;
		}
		memcpy(packet->data, data, size);
		packet->size = size;
		packet->capacity = size;
	} else {
		TSMS_POS staticPos = 0;
		TSMS_POS position = 0;
		bool first = true;
		long checksum = 0;
		for (TSMS_POS i = 0; i < builder->typesLength; i++) {
			TSMS_TYPE type = builder->types[i];
			if (type == TSMS_TYPE_CHECKSUM) {
				if (first) {
					first = false;
					checksum = __tsms_internal_packet_read_long(data, position, size);
				} else if (checksum != __tsms_internal_packet_read_long(data, position, size)) {
					TSMS_PACKET_release(packet);
						return TSMS_NULL;
				}
				long temp = 0;
				memcpy(data + position, &temp, 8);
				position += 8;
			} else if (type == TSMS_TYPE_RESERVED) {
				if (0 != __tsms_internal_packet_read_byte(data, position, size)) {
					TSMS_PACKET_release(packet);
					return TSMS_NULL;
				}
				position += 1;
			} else if (type == TSMS_TYPE_STATIC) {
				pString str = __tsms_internal_packet_read_string(data, position, size);
				if (!TSMS_STRING_equals(str, builder->statics->list[staticPos++])) {
					TSMS_PACKET_release(packet);
					TSMS_STRING_release(str);
					return TSMS_NULL;
				}
				position += (4 + str->length);
				TSMS_STRING_release(str);
			} else if (type == TSMS_TYPE_STRING) {
				pString str = __tsms_internal_packet_read_string(data, position, size);
				if (str == TSMS_NULL) {
					TSMS_PACKET_release(packet);
					TSMS_STRING_release(str);
					return TSMS_NULL;
				}
				position += (4 + str->length);
				TSMS_STRING_release(str);
			} else {
				__tsms_internal_packet_read(data, position, size, _packetTypeSize[type]);
				position += _packetTypeSize[type];
			}
		}
		if (!first) {
			long temp = builder->checksum(data, size);
			if (temp != checksum) {
				TSMS_PACKET_release(packet);
				return TSMS_NULL;
			}
		}
		packet->data = TSMS_malloc(size);
		if (packet->data == TSMS_NULL) {
			TSMS_PACKET_release(packet);
			return TSMS_NULL;
		}
		memcpy(packet->data, data, size);
		packet->size = size;
		packet->capacity = size;
	}
	return packet;
}

pPacket TSMS_PACKET_BUILDER_build(pPacketBuilder builder) {
	if (builder == TSMS_NULL)
		return TSMS_NULL;
	pPacket packet = TSMS_malloc(sizeof(tPacket));
	if (packet == TSMS_NULL)
		return TSMS_NULL;
	packet->data = TSMS_malloc(0);
	if (packet->data == TSMS_NULL) {
		TSMS_PACKET_release(packet);
		return TSMS_NULL;
	}
	packet->size = 0;
	packet->capacity = 0;
	packet->offset = 0;
	TSMS_POS positions[12];
	memset(positions, 0, sizeof(positions));
	TSMS_ILP checksumMark = TSMS_INT_LIST_create(1);
	for (TSMS_POS i = 0; i < builder->typesLength; i++) {
		TSMS_TYPE type = builder->types[i];
		TSMS_LP list = TSMS_LONG_MAP_get(builder->data, type);
		TSMS_POS position = positions[type];
		if (type == TSMS_TYPE_STRING) {
			if (list == TSMS_NULL || position >= list->length) {
				TSMS_PACKET_release(packet);
				TSMS_INT_LIST_release(checksumMark);
				return TSMS_NULL;
			}
			TSMS_RESULT result = __tsms_internal_packet_write_string(packet, *(pString*)(list->list[position]));
			if (result != TSMS_SUCCESS) {
				TSMS_PACKET_release(packet);
				TSMS_INT_LIST_release(checksumMark);
				return TSMS_NULL;
			}
		} else if (type == TSMS_TYPE_STATIC) {
			if (position >= builder->statics->length) {
				TSMS_PACKET_release(packet);
				TSMS_INT_LIST_release(checksumMark);
				return TSMS_NULL;
			}
			TSMS_RESULT result = __tsms_internal_packet_write_string(packet, builder->statics->list[position]);
			if (result != TSMS_SUCCESS) {
				TSMS_PACKET_release(packet);
				TSMS_INT_LIST_release(checksumMark);
				return TSMS_NULL;
			}
		} else if (type == TSMS_TYPE_RESERVED) {
			TSMS_RESULT result = __tsms_internal_packet_write_byte(packet, 0);
			if (result != TSMS_SUCCESS) {
				TSMS_PACKET_release(packet);
				TSMS_INT_LIST_release(checksumMark);
				return TSMS_NULL;
			}
		} else if (type == TSMS_TYPE_CHECKSUM) {
			TSMS_INT_LIST_add(checksumMark, packet->size);
			TSMS_RESULT result = __tsms_internal_packet_write_long(packet, 0);
			if (result != TSMS_SUCCESS) {
				TSMS_PACKET_release(packet);
				TSMS_INT_LIST_release(checksumMark);
				return TSMS_NULL;
			}
		} else {
			if (list == TSMS_NULL || position >= list->length) {
				TSMS_PACKET_release(packet);
				TSMS_INT_LIST_release(checksumMark);
				return TSMS_NULL;
			}
			TSMS_RESULT result = __tsms_internal_packet_write(packet, list->list[position], _packetTypeSize[type]);
			if (result != TSMS_SUCCESS) {
				TSMS_PACKET_release(packet);
				TSMS_INT_LIST_release(checksumMark);
				return TSMS_NULL;
			}
		}
		positions[type]++;
	}
	if (checksumMark->length != 0) {
		long checksum = builder->checksum(packet->data, packet->size);
		for (TSMS_POS i = 0; i < checksumMark->length; i++)
			memcpy(packet->data + checksumMark->list[i], &checksum, sizeof(long));
	}
	TSMS_RESULT result = __tsms_internal_packet_resize(packet);
	if (result != TSMS_SUCCESS) {
		TSMS_PACKET_release(packet);
		TSMS_INT_LIST_release(checksumMark);
		return TSMS_NULL;
	}
	TSMS_PACKET_BUILDER_clear(builder);
	TSMS_INT_LIST_release(checksumMark);
	return packet;
}

TSMS_RESULT TSMS_PACKET_release(pPacket packet) {
	if (packet == TSMS_NULL)
		return TSMS_ERROR;
	free(packet->data);
	free(packet);
	return TSMS_SUCCESS;
}

void * TSMS_PACKET_read(pPacket packet, TSMS_SIZE size) {
	if (packet == TSMS_NULL || packet->offset + size > packet->size)
		return TSMS_NULL;
	void * data = packet->data + packet->offset;
	packet->offset += size;
	return data;
}

uint32_t TSMS_PACKET_readInt(pPacket packet) {
	void * data = TSMS_PACKET_read(packet, sizeof(uint32_t));
	if (data == TSMS_NULL)
		return 0;
	return *((uint32_t*)data);
}

uint64_t TSMS_PACKET_readLong(pPacket packet) {
	void * data = TSMS_PACKET_read(packet, sizeof(uint64_t));
	if (data == TSMS_NULL)
		return 0;
	return *((uint64_t*)data);
}

pString TSMS_PACKET_readString(pPacket packet) {
	uint32_t size = TSMS_PACKET_readInt(packet);
	if (size == 0)
		return TSMS_NULL;
	void * data = TSMS_PACKET_read(packet, size);
	if (data == TSMS_NULL)
		return TSMS_NULL;
	return TSMS_STRING_createWithFixSize(data, size);
}

char TSMS_PACKET_readChar(pPacket packet) {
	void * data = TSMS_PACKET_read(packet, sizeof(char));
	if (data == TSMS_NULL)
		return 0;
	return *((char*)data);
}