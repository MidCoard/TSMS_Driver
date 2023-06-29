#ifndef TSMS_PACKET_H
#define TSMS_PACKET_H

#include "tsms_def.h"
#include "tsms_function_def.h"
#include "tsms.h"
#include "tsms_long_map.h"

typedef struct TSMS_PACKET_BUILDER tPacketBuilder;
typedef tPacketBuilder* pPacketBuilder;

typedef struct TSMS_PACKET tPacket;
typedef tPacket* pPacket;

struct TSMS_PACKET_BUILDER {

	TSMS_LMP data;
	TSMS_SIZE typesLength;
	TSMS_TYPE * types;
	TSMS_LP statics;

	TSMS_CHECKSUM_FUNCTION checksum;
};

struct TSMS_PACKET {
	TSMS_SIZE size;
	TSMS_SIZE capacity;
	uint8_t *data;
	TSMS_POS offset;
};

pPacketBuilder TSMS_PACKET_BUILDER_create(pString protocol, TSMS_CHECKSUM_FUNCTION checksum);

pPacketBuilder TSMS_PACKET_BUILDER_createEmpty();

TSMS_RESULT TSMS_PACKET_BUILDER_clear(pPacketBuilder builder);

TSMS_RESULT TSMS_PACKET_BUILDER_release(pPacketBuilder builder);

TSMS_RESULT TSMS_PACKET_BUILDER_write(pPacketBuilder builder, TSMS_TYPE type, void* data, TSMS_SIZE size);

TSMS_RESULT TSMS_PACKET_BUILDER_writeInt(pPacketBuilder builder, uint32_t data);

TSMS_RESULT TSMS_PACKET_BUILDER_writeFloat(pPacketBuilder builder, float data);

TSMS_RESULT TSMS_PACKET_BUILDER_writePointer(pPacketBuilder builder, TSMS_TYPE type, void* data);

TSMS_RESULT TSMS_PACKET_BUILDER_writeString(pPacketBuilder builder, pString data);

TSMS_RESULT TSMS_PACKET_BUILDER_writeChar(pPacketBuilder builder, char data);

pPacket TSMS_PACKET_BUILDER_resolve(pPacketBuilder builder, uint8_t* data, TSMS_SIZE size);

pPacket TSMS_PACKET_BUILDER_build(pPacketBuilder builder);

void * TSMS_PACKET_read(pPacket packet, TSMS_SIZE size);

uint32_t TSMS_PACKET_readInt(pPacket packet);

uint64_t TSMS_PACKET_readLong(pPacket packet);

pString TSMS_PACKET_readString(pPacket packet);

char TSMS_PACKET_readChar(pPacket packet);

TSMS_RESULT TSMS_PACKET_release(pPacket packet);

#endif //TSMS_PACKET_H
