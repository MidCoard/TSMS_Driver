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
	uint8_t *data;
	TSMS_POS offset;
};

pPacketBuilder TSMS_PACKET_create(pString protocol, TSMS_CHECKSUM_FUNCTION checksum);

pPacketBuilder TSMS_PACKET_createEmpty();

TSMS_RESULT TSMS_PACKET_clear(pPacketBuilder builder);

TSMS_RESULT TSMS_PACKET_release(pPacketBuilder builder);

TSMS_RESULT TSMS_PACKET_write(pPacketBuilder builder, TSMS_TYPE type, void* data, TSMS_SIZE size);

TSMS_RESULT TSMS_PACKET_writeInt(pPacketBuilder builder, uint32_t data);

pPacket TSMS_PACKET_resolve(pPacketBuilder builder, uint8_t* data, TSMS_SIZE size);

pPacket TSMS_PACKET_build(pPacketBuilder builder);

#endif //TSMS_PACKET_H
