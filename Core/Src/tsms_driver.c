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
	reg->value = 0;
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
	reg->value = 0;

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


TSMS_RHP TSMS_REG_16BitRegister(TSMS_REGISTER_16BIT) {
	TSMS_RHP reg = malloc(sizeof (struct TSMS_REGISTER_HANDLER));
	if (reg == TSMS_NULL)
		return TSMS_NULL;
	reg->bits = 16;
	reg->value = 0;

	position[0] = bit0;
	position[1] = bit1;
	position[2] = bit2;
	position[3] = bit3;
	position[4] = bit4;
	position[5] = bit5;
	position[6] = bit6;
	position[7] = bit7;
	position[8] = bit8;
	position[9] = bit9;
	position[10] = bit10;
	position[11] = bit11;
	position[12] = bit12;
	position[13] = bit13;
	position[14] = bit14;
	position[15] = bit15;

	reg->types = malloc(sizeof (TSMS_REGISTER_DATA_TYPE) * 16);
	memset(reg->types,TSMS_REGISTER_MSB, sizeof (TSMS_REGISTER_DATA_TYPE) * 16);

	reg->sizes = malloc(sizeof (uint8_t) * 16);
	memset(reg->sizes, 0, sizeof (uint8_t) * 16);

	reg->starts = malloc(sizeof (uint8_t) * 16);
	memset(reg->starts, 0, sizeof (uint8_t) * 16);

	uint8_t previous = -1;
	for (uint8_t i = 0;i<16;i++) {
		uint8_t now = position[i];
		reg->sizes[position[i]]++;
		if (now != previous)
			reg->starts[now] = i;
		previous = now;
	}

	return reg;
}

TSMS_RHP TSMS_REG_24BitRegister(TSMS_REGISTER_24BIT) {
    TSMS_RHP reg = malloc(sizeof (struct TSMS_REGISTER_HANDLER));
    if (reg == TSMS_NULL)
        return TSMS_NULL;
    reg->bits = 24;
	reg->value = 0;

    position[0] = bit0;
    position[1] = bit1;
    position[2] = bit2;
    position[3] = bit3;
    position[4] = bit4;
    position[5] = bit5;
    position[6] = bit6;
    position[7] = bit7;
    position[8] = bit8;
    position[9] = bit9;
    position[10] = bit10;
    position[11] = bit11;
    position[12] = bit12;
    position[13] = bit13;
    position[14] = bit14;
    position[15] = bit15;
    position[16] = bit16;
    position[17] = bit17;
    position[18] = bit18;
    position[19] = bit19;
    position[20] = bit20;
    position[21] = bit21;
    position[22] = bit22;
    position[23] = bit23;

    reg->types = malloc(sizeof (TSMS_REGISTER_DATA_TYPE) * 24);
    memset(reg->types,TSMS_REGISTER_MSB, sizeof (TSMS_REGISTER_DATA_TYPE) * 24);

    reg->sizes = malloc(sizeof (uint8_t) * 24);
    memset(reg->sizes, 0, sizeof (uint8_t) * 24);

    reg->starts = malloc(sizeof (uint8_t) * 24);
    memset(reg->starts, 0, sizeof (uint8_t) * 24);

    uint8_t previous = -1;
    for (uint8_t i = 0;i<24;i++) {
        uint8_t now = position[i];
        reg->sizes[position[i]]++;
        if (now != previous)
            reg->starts[now] = i;
        previous = now;
    }

    return reg;
}

TSMS_RHP TSMS_REG_32bitRegister(TSMS_REGISTER_32BIT) {
    TSMS_RHP reg = malloc(sizeof (struct TSMS_REGISTER_HANDLER));
    if (reg == TSMS_NULL)
        return TSMS_NULL;
    reg->bits = 32;
	reg->value = 0;

    position[0] = bit0;
    position[1] = bit1;
    position[2] = bit2;
    position[3] = bit3;
    position[4] = bit4;
    position[5] = bit5;
    position[6] = bit6;
    position[7] = bit7;
    position[8] = bit8;
    position[9] = bit9;
    position[10] = bit10;
    position[11] = bit11;
    position[12] = bit12;
    position[13] = bit13;
    position[14] = bit14;
    position[15] = bit15;
    position[16] = bit16;
    position[17] = bit17;
    position[18] = bit18;
    position[19] = bit19;
    position[20] = bit20;
    position[21] = bit21;
    position[22] = bit22;
    position[23] = bit23;
    position[24] = bit24;
    position[25] = bit25;
    position[26] = bit26;
    position[27] = bit27;
    position[28] = bit28;
    position[29] = bit29;
    position[30] = bit30;
    position[31] = bit31;

    reg->types = malloc(sizeof (TSMS_REGISTER_DATA_TYPE) * 32);
    memset(reg->types,TSMS_REGISTER_MSB, sizeof (TSMS_REGISTER_DATA_TYPE) * 32);

    reg->sizes = malloc(sizeof (uint8_t) * 32);
    memset(reg->sizes, 0, sizeof (uint8_t) * 32);

    reg->starts = malloc(sizeof (uint8_t) * 32);
    memset(reg->starts, 0, sizeof (uint8_t) * 32);

    uint8_t previous = -1;
    for (uint8_t i = 0;i<32;i++) {
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
// writeAt and readAt method are all written or read by MSB
TSMS_RESULT TSMS_REG_writeAt(TSMS_RHP reg, uint8_t start, uint8_t bits, uint32_t value) {
	uint32_t mask = TSMS_MASK(bits);
	if (mask == 0)
		return TSMS_FAIL;
	reg->value &= ~(mask<<start);
	reg->value |= (value & mask)<<start;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_REG_read(TSMS_RHP reg, uint8_t pos, uint32_t * value) {
	uint32_t val = 0;
	TSMS_RESULT result = TSMS_REG_readAt(reg, reg->starts[pos], reg->sizes[pos], &val);
	if (result != TSMS_SUCCESS)
		return result;
	if (reg->types[pos] == TSMS_REGISTER_MSB)
		*value = val;
	else {
		uint8_t size = reg->sizes[pos];
		uint32_t temp = 0;
		for (uint8_t i = 0;i<size;i++) {
			temp |= (val & (1<<i)) ? 1 : 0;
			temp <<= 1;
		}
		*value = temp;
	}
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_REG_readAt(TSMS_RHP reg, uint8_t start, uint8_t bits, uint32_t* value) {
	uint32_t mask = TSMS_MASK(bits);
	if (mask == 0)
		return TSMS_FAIL;
	*value = (reg->value >> start) & mask;
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

TSMS_DHP TSMS_DRIVER_createSPIHandler(TSMS_SHP spi, TSMS_RHLP regs) {
	TSMS_DHP driver = malloc(sizeof(struct TSMS_DRIVER_HANDLER));
	if (driver == TSMS_NULL)
		return TSMS_NULL;
	driver->type = TSMS_DRIVER_SPI;
	driver->spi = spi;
	driver->regs = regs;
	driver->write = TSMS_SPI_transmitCustomBits;
	driver->read = TSMS_SPI_receiveCustomBits;
	return driver;
}

TSMS_RHLP TSMS_REG_createList(int n,...) {
	TSMS_RHLP list = malloc(sizeof(struct TSMS_REGISTER_HANDLER_LIST));
	if (list == TSMS_NULL)
		return TSMS_NULL;
	list->size = n;
	list->regs = malloc(sizeof (TSMS_RHP) * n);
	va_list l;
	va_start(l,n);
	for (int i = 0;i<n;i++)
		list->regs[i] = va_arg(l,TSMS_RHP);
	va_end(l);
	return list;
}

TSMS_RESULT TSMS_REG_releaseList(TSMS_RHLP list) {
	free(list->regs);
	free(list);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_REG_setRegister(TSMS_RHP reg, uint32_t value) {
	reg->value = value;
	return TSMS_SUCCESS;
}

uint32_t TSMS_REG_getRegister(TSMS_RHP reg) {
	return reg->value;
}

uint32_t TSMS_REG_getRegisterByList(TSMS_RHLP list, uint8_t pos) {
	return list->regs[pos]->value;
}

TSMS_RESULT TSMS_REG_setRegisterByList(TSMS_RHLP list, uint8_t pos, uint32_t value) {
	if (list->size < pos || pos < 0)
		return TSMS_ERROR;
	return TSMS_REG_setRegister(list->regs[pos], value);
}

TSMS_RESULT TSMS_REG_writeRegister(TSMS_RHP reg, uint8_t pos, uint32_t value) {
	return TSMS_REG_write(reg, pos, value);
}

TSMS_RESULT TSMS_REG_writeRegisterByList(TSMS_RHLP list, uint8_t pos, uint32_t value) {
	TSMS_RESULT result = TSMS_FAIL;
	for (int i = 0;i<list->size;i ++)
		if (TSMS_REG_writeRegister(list->regs[i], pos, value) == TSMS_SUCCESS) {
			result = TSMS_SUCCESS;
			break;
		}
	return result;
}

TSMS_RESULT TSMS_REG_readRegister(TSMS_RHP reg, uint8_t pos, uint32_t* value) {
	return TSMS_REG_read(reg, pos ,value);
}


TSMS_RESULT TSMS_REG_readRegisterByList(TSMS_RHLP list, uint8_t pos, uint32_t* value) {
	TSMS_RESULT result = TSMS_FAIL;
	for (int i = 0;i<list->size;i ++)
		if (TSMS_REG_readRegister(list->regs[i], pos, value) == TSMS_SUCCESS) {
			result = TSMS_SUCCESS;
			break;
		}
	return result;
}

uint32_t TSMS_REG_tempWriteRegisterByList(TSMS_RHLP list, uint8_t pos, uint32_t value) {
	uint32_t ret = 0;
	for (int i = 0;i<list->size;i++)
		if ((ret = TSMS_REG_tempWriteRegister(list->regs[i],pos, value)) != 0)
			return ret;
	return ret;
}


uint32_t TSMS_REG_tempWriteRegister(TSMS_RHP reg, uint8_t pos, uint32_t value) {
	return TSMS_REG_tempWrite(reg, pos, value);
}


uint32_t TSMS_REG_tempWrite(TSMS_RHP reg, uint8_t pos, uint32_t value) {
	if (reg->types[pos] == TSMS_REGISTER_MSB)
		return TSMS_REG_tempWriteAt(reg, reg->starts[pos], reg->sizes[pos] ,value);
	else {
		uint8_t size = reg->sizes[pos];
		uint32_t temp = 0;
		for (uint8_t i = 0;i<size;i++) {
			temp |= (value & (1<<i)) ? 1 : 0;
			temp <<= 1;
		}
		return TSMS_REG_tempWriteAt(reg, reg->starts[pos],reg->sizes[pos], temp);
	}
}

uint32_t TSMS_REG_tempWriteAt(TSMS_RHP reg, uint8_t start, uint8_t bits, uint32_t value) {
	uint32_t mask = TSMS_MASK(bits);
	if (mask == 0)
		return 0;
	return reg->value | (value & mask)<<start;
}