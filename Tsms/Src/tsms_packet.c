#include "comm/tsms_packet.h"
#include "tsms_util.h"

TSMS_INLINE long __tsms_internal_checksum(const uint8_t* data, TSMS_SIZE size) {
	long sum = 0;
	for (TSMS_SIZE i = 0; i < size; i++)
		sum += data[i];
	return sum;
}

pPacketBuilder TSMS_PACKET_create(pString protocol, TSMS_CHECKSUM_FUNCTION checksum) {
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
		else if (TSMS_STRING_equals(str, TSMS_STRING_static("O")) || TSMS_STRING_equals(str, TSMS_STRING_static("o")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("object")) ||
		         TSMS_STRING_equals(str, TSMS_STRING_static("OBJECT")))
			builder->types[i] = TSMS_TYPE_OBJECT;
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

pPacketBuilder TSMS_PACKET_createEmpty() {
	pPacketBuilder builder = TSMS_malloc(sizeof(tPacketBuilder));
	if (builder == TSMS_NULL)
		return TSMS_NULL;
	builder->data = TSMS_LONG_MAP_create(16);
	builder->types = TSMS_NULL;
	builder->statics = TSMS_NULL;
	builder->checksum = __tsms_internal_checksum;
	return builder;
}

TSMS_RESULT TSMS_PACKET_clear(pPacketBuilder builder) {
	if (builder == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LONG_MAP_clear(builder->data);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_PACKET_release(pPacketBuilder builder) {
	if (builder == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LONG_MAP_release(builder->data);
	TSMS_LIST_release(builder->statics);
	free(builder->types);
	free(builder);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_PACKET_write(pPacketBuilder builder, TSMS_TYPE type, void* data, TSMS_SIZE size) {
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

TSMS_RESULT TSMS_PACKET_writeInt(pPacketBuilder builder, uint32_t data) {
	return TSMS_PACKET_write(builder, TSMS_TYPE_INT, &data, sizeof(uint32_t));
}

TSMS_RESULT TSMS_PACKET_writeFloat(pPacketBuilder builder, float data) {
	return TSMS_PACKET_write(builder, TSMS_TYPE_FLOAT, &data, sizeof(float));
}

pPacket TSMS_PACKET_resolve(pPacketBuilder builder, uint8_t* data, TSMS_SIZE size) {
	if (builder == TSMS_NULL)
		return TSMS_NULL;
	pPacket packet = TSMS_malloc(sizeof(tPacket));
	if (packet == TSMS_NULL)
		return TSMS_NULL;

	return packet;
}

pPacket TSMS_PACKET_build(pPacketBuilder builder) {
	if (builder == TSMS_NULL)
		return TSMS_NULL;
	pPacket packet = TSMS_malloc(sizeof(tPacket));
	if (packet == TSMS_NULL)
		return TSMS_NULL;
	for (TSMS_SIZE i = 0; i < builder->typesLength; i++) {
		TSMS_LP list = TSMS_LONG_MAP_get(builder->data, builder->types[i]);
		if (list == TSMS_NULL)
			continue;
		for (TSMS_SIZE j = 0; j < TSMS_LIST_size(list); j++) {
			void* data = TSMS_LIST_get(list, j);
			TSMS_SIZE size = TSMS_LIST_size(list);
			TSMS_SIZE offset = 0;
			while (size > 0) {
				TSMS_SIZE write = size > 0xFF ? 0xFF : size;
				TSMS_SIZE length = write + 1;
				uint8_t* buffer = TSMS_malloc(length);
				if (buffer == TSMS_NULL)
					return TSMS_NULL;
				buffer[0] = builder->types[i];
				memcpy(buffer + 1, data + offset, write);
				TSMS_LIST_add(packet->data, buffer);
				offset += write;
				size -= write;
			}
		}
	}

	TSMS_PACKET_clear(builder);
	return packet;
}

