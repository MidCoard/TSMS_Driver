#include "tsms_driver.h"

static void addAndRemove(TSMS_RHP reg, uint32_t add,uint32_t remove) {
	reg->value |= add;
	reg->value &= remove;
}

static void add(TSMS_RHP reg, uint32_t offset) {
	addAndRemove(reg,offset,0xFFFFFFFF);
}

static void remove(TSMS_RHP reg, uint32_t mask) {
	addAndRemove(reg, 0,mask);
}

uint8_t position[32]; // notice this is globally used. If use multithread to access the register configuration, there may be something woring.

TSMS_RHP TSMS_REG_Register(uint8_t bits) {
	TSMS_RHP reg = malloc(sizeof (struct TSMS_REGISTER_HANDLER));
	if (reg == TSMS_NULL)
		return TSMS_NULL;
	reg->bits = bits;
	reg->types = malloc(sizeof (TSMS_REGISTER_DATA_TYPE) * 8);
	memset(reg->types,TSMS_REGISTER_MSB, sizeof (TSMS_REGISTER_DATA_TYPE) * 8);
	reg->sizes = malloc(sizeof (uint8_t) * 8);
	memset(reg->sizes, 0, sizeof (uint8_t) * 8);
	reg->starts = malloc(sizeof (uint8_t) * 8);
	memset(reg->starts, 0, sizeof (uint8_t) * 8);
	return reg;
}

TSMS_RHP TSMS_REG_8BitRegister(TSMS_REGISTER_8BIT) {
	TSMS_RHP reg = malloc(sizeof (struct TSMS_REGISTER_HANDLER));
	if (reg == TSMS_NULL)
		return TSMS_NULL;
	reg->bits = 8;

	position[0] = bit0;
	position[1] = bit1;
	position[2] = bit2;
	position[3] = bit3;
	position[4] = bit4;
	position[5] = bit5;
	position[6] = bit6;
	position[7] = bit7;
	reg->types = malloc(sizeof (TSMS_REGISTER_DATA_TYPE) * 8);
	memset(reg->types,TSMS_REGISTER_MSB, sizeof (TSMS_REGISTER_DATA_TYPE) * 8);

	reg->sizes = malloc(sizeof (uint8_t) * 8);
	memset(reg->sizes, 0, sizeof (uint8_t) * 8);

	reg->starts = malloc(sizeof (uint8_t) * 8);
	memset(reg->starts, 0, sizeof (uint8_t) * 8);

	uint8_t previous = -1;
	for (uint8_t i = 0;i<8;i++) {
		uint8_t now = position[i];
		reg->sizes[position[i]]++;
		if (now != previous)
			reg->starts[now] = i;
		previous = now;
	}

	return reg;
}

TSMS_RESULT TSMS_REG_write(TSMS_RHP reg, uint8_t pos, uint32_t value) {
	if (reg->types[pos] == TSMS_REGISTER_MSB)
		return TSMS_REG_writeAt(reg, reg->starts[pos], reg->sizes[pos] ,value);
	else {
		uint8_t size = reg->sizes[pos];
		uint32_t temp = 0;
		for (uint8_t i = 0;i<size;i++) {
			temp |= (value & (1<<i)) ? 1 : 0;
			temp <<= 1;
		}
		return TSMS_REG_writeAt(reg, reg->starts[pos],reg->sizes[pos], temp);
	}
}

TSMS_RESULT TSMS_REG_writeAt(TSMS_RHP reg, uint8_t start, uint8_t bits, uint32_t value) {
	uint32_t mask = TSMS_MASK(bits);
	reg->value &= ~(mask<<start);
	reg->value |= (value & mask)<<start;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_REG_release(TSMS_RHP reg) {
	free(reg->sizes);
	free(reg->starts);
	free(reg->types);
	free(reg);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_REG_configure(TSMS_RHP reg,uint8_t pos, uint8_t left, uint8_t right, TSMS_REGISTER_DATA_TYPE type) {
	reg->starts[pos] = left;
	reg->sizes[pos] = right - left + 1;
	reg->types[pos] = type;
	return TSMS_SUCCESS;
}

TSMS_DHP TSMS_DRIVER_createHandler(TSMS_SHP spi,TSMS_RHLP regs) {
	TSMS_DHP driver = malloc(sizeof(struct TSMS_DRIVER_HANDLER));
	if (driver == TSMS_NULL)
		return TSMS_NULL;
	driver->type = TSMS_DRIVER_SPI;
	driver->spi = spi;
	driver->regs = regs;
	driver->write = TSMS_SPI_transmitCustomBits;
	driver->read = TSMS_SPI_receiveBytes; //todo
	return driver;
}