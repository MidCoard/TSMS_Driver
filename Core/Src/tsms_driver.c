#include "tsms_driver.h"

static void __tsms_internal_addAndRemove(TSMS_RHP reg, uint32_t add,uint32_t remove) {
	reg->value |= add;
	reg->value &= remove;
}

static void __tsms_internal_add(TSMS_RHP reg, uint32_t offset) {
	__tsms_internal_addAndRemove(reg,offset,0xFFFFFFFF);
}

static void __tsms_internal_remove(TSMS_RHP reg, uint32_t mask) {
	__tsms_internal_addAndRemove(reg, 0,mask);
}

TSMS_RHP TSMS_REG_Register(uint8_t bits) {
	TSMS_RHP reg = malloc(sizeof (struct TSMS_REGISTER_HANDLER));
	if (reg == TSMS_NULL)
		return TSMS_NULL;
	reg->bits = bits;
	reg->value = 0;
	reg->positions = TSMS_NULL;
	return reg;
}

TSMS_RHP TSMS_REG_8BitRegister(TSMS_REGISTER_8BIT) {
	TSMS_RHP reg = malloc(sizeof (struct TSMS_REGISTER_HANDLER));
	if (reg == TSMS_NULL)
		return TSMS_NULL;
	reg->bits = 8;
	reg->value = 0;
	reg->positions = malloc(sizeof (uint8_t) * 8);

	reg->positions[0] = bit0;
	reg->positions[1] = bit1;
	reg->positions[2] = bit2;
	reg->positions[3] = bit3;
	reg->positions[4] = bit4;
	reg->positions[5] = bit5;
	reg->positions[6] = bit6;
	reg->positions[7] = bit7;
	return reg;
}


TSMS_RHP TSMS_REG_16BitRegister(TSMS_REGISTER_16BIT) {
	TSMS_RHP reg = malloc(sizeof (struct TSMS_REGISTER_HANDLER));
	if (reg == TSMS_NULL)
		return TSMS_NULL;
	reg->bits = 16;
	reg->value = 0;
	reg->positions = malloc(sizeof (uint8_t) * 16);

	reg->positions[0] = bit0;
	reg->positions[1] = bit1;
	reg->positions[2] = bit2;
	reg->positions[3] = bit3;
	reg->positions[4] = bit4;
	reg->positions[5] = bit5;
	reg->positions[6] = bit6;
	reg->positions[7] = bit7;
	reg->positions[8] = bit8;
	reg->positions[9] = bit9;
	reg->positions[10] = bit10;
	reg->positions[11] = bit11;
	reg->positions[12] = bit12;
	reg->positions[13] = bit13;
	reg->positions[14] = bit14;
	reg->positions[15] = bit15;

	return reg;
}

TSMS_RHP TSMS_REG_24BitRegister(TSMS_REGISTER_24BIT) {
    TSMS_RHP reg = malloc(sizeof (struct TSMS_REGISTER_HANDLER));
    if (reg == TSMS_NULL)
        return TSMS_NULL;
    reg->bits = 24;
	reg->value = 0;
	reg->positions = malloc(sizeof (uint8_t) * 24);
    reg->positions[0] = bit0;
    reg->positions[1] = bit1;
    reg->positions[2] = bit2;
    reg->positions[3] = bit3;
    reg->positions[4] = bit4;
    reg->positions[5] = bit5;
    reg->positions[6] = bit6;
    reg->positions[7] = bit7;
    reg->positions[8] = bit8;
    reg->positions[9] = bit9;
    reg->positions[10] = bit10;
    reg->positions[11] = bit11;
    reg->positions[12] = bit12;
    reg->positions[13] = bit13;
    reg->positions[14] = bit14;
    reg->positions[15] = bit15;
    reg->positions[16] = bit16;
    reg->positions[17] = bit17;
    reg->positions[18] = bit18;
    reg->positions[19] = bit19;
    reg->positions[20] = bit20;
    reg->positions[21] = bit21;
    reg->positions[22] = bit22;
    reg->positions[23] = bit23;
    return reg;
}

TSMS_RHP TSMS_REG_32bitRegister(TSMS_REGISTER_32BIT) {
    TSMS_RHP reg = malloc(sizeof (struct TSMS_REGISTER_HANDLER));
    if (reg == TSMS_NULL)
        return TSMS_NULL;
    reg->bits = 32;
	reg->value = 0;
	reg->positions = malloc(sizeof (uint8_t) * 32);

    reg->positions[0] = bit0;
    reg->positions[1] = bit1;
    reg->positions[2] = bit2;
    reg->positions[3] = bit3;
    reg->positions[4] = bit4;
    reg->positions[5] = bit5;
    reg->positions[6] = bit6;
    reg->positions[7] = bit7;
    reg->positions[8] = bit8;
    reg->positions[9] = bit9;
    reg->positions[10] = bit10;
    reg->positions[11] = bit11;
    reg->positions[12] = bit12;
    reg->positions[13] = bit13;
    reg->positions[14] = bit14;
    reg->positions[15] = bit15;
    reg->positions[16] = bit16;
    reg->positions[17] = bit17;
    reg->positions[18] = bit18;
    reg->positions[19] = bit19;
    reg->positions[20] = bit20;
    reg->positions[21] = bit21;
    reg->positions[22] = bit22;
    reg->positions[23] = bit23;
    reg->positions[24] = bit24;
    reg->positions[25] = bit25;
    reg->positions[26] = bit26;
    reg->positions[27] = bit27;
    reg->positions[28] = bit28;
    reg->positions[29] = bit29;
    reg->positions[30] = bit30;
    reg->positions[31] = bit31;

    return reg;
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

TSMS_RESULT TSMS_REG_readAt(TSMS_RHP reg, uint8_t start, uint8_t bits, uint32_t* value) {
	uint32_t mask = TSMS_MASK(bits);
	if (mask == 0)
		return TSMS_FAIL;
	*value = (reg->value >> start) & mask;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_REG_release(TSMS_RHP reg) {
	free(reg);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_REG_configure(TSMS_RHLP list, uint8_t reg, uint8_t pos, uint8_t left, uint8_t right, TSMS_REGISTER_DATA_TYPE type) {
	list->ids[pos] = reg;
	list->starts[pos] = left;
	list->sizes[pos] = right - left + 1;
	list->types[pos] = type;
	return TSMS_SUCCESS;
}

TSMS_DHP TSMS_DRIVER_createSPIHandler(TSMS_SHP spi, TSMS_RHLP regs) {
	TSMS_DHP driver = malloc(sizeof(struct TSMS_DRIVER_HANDLER));
	if (driver == TSMS_NULL)
		return TSMS_NULL;
	driver->type = TSMS_DRIVER_SPI;
	driver->spi = spi;
	driver->regs = regs;
	driver->spiWrite = TSMS_SPI_transmitCustomBits;
	driver->spiRead = TSMS_SPI_receiveCustomBits;
	driver->spiTransform = TSMS_SPI_transform;
	driver->spiSequenceTransform = TSMS_SPI_sequenceTransform;
	return driver;
}

TSMS_DHP TSMS_DRIVER_createIICHandler(TSMS_IHP iic, TSMS_RHLP regs) {
	TSMS_DHP driver = malloc(sizeof(struct TSMS_DRIVER_HANDLER));
	if (driver == TSMS_NULL)
		return TSMS_NULL;
	driver->type = TSMS_DRIVER_IIC;
	driver->iic = iic;
	driver->regs = regs;
	driver->iicWrite = TSMS_IIC_writeCustomRegister;
	driver->iicRead = TSMS_IIC_readCustomRegister;
	return driver;
}

TSMS_DHP TSMS_DRIVER_createCustomHandler(TSMS_CHP custom, TSMS_RHLP regs) {
	TSMS_DHP driver = malloc(sizeof(struct TSMS_DRIVER_HANDLER));
	if (driver == TSMS_NULL)
		return TSMS_NULL;
	driver->type = TSMS_DRIVER_CUSTOM;
	driver->custom = custom;
	driver->regs = regs;
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
	uint8_t mx = 0;
	for (int i = 0;i<n;i++) {
		list->regs[i] = va_arg(l, TSMS_RHP);
		if (list->regs[i]->positions != TSMS_NULL)
			for (int j = 0;j<list->regs[i]->bits;j++)
				if (list->regs[i]->positions[j] > mx)
					mx = list->regs[i]->positions[j];
	}
	va_end(l);
	list->maxSize = mx + 1;
	list->ids = malloc(sizeof (uint8_t) * list->maxSize);
	memset(list->ids, 0, sizeof (uint8_t) * list->maxSize);
	list->sizes = malloc(sizeof (uint8_t) * list->maxSize);
	memset(list->sizes, 0, sizeof (uint8_t) * list->maxSize);
	list->types = malloc(sizeof (TSMS_REGISTER_DATA_TYPE) * list->maxSize);
	memset(list->types, TSMS_REGISTER_MSB, sizeof (TSMS_REGISTER_DATA_TYPE) * list->maxSize);
	list->starts = malloc(sizeof (uint8_t) * list->maxSize);
	memset(list->starts, 0, sizeof (uint8_t) * list->maxSize);
	uint8_t previous = -1;
	for (int i = 0;i<n;i++) {
		if (list->regs[i]->positions != TSMS_NULL) {
			for (int j = 0; j < list->regs[i]->bits; j++) {
				uint8_t now = list->regs[i]->positions[j];
				list->sizes[now]++;
				if (now != previous) {
					list->ids[now] = i;
					list->starts[now] = j;
				}
				previous = now;
			}
			free(list->regs[i]->positions);
		}
	}
	return list;
}

TSMS_RESULT TSMS_REG_releaseList(TSMS_RHLP list) {
	free(list->regs);
	free(list->ids);
	free(list->sizes);
	free(list->starts);
	free(list->types);
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

TSMS_RESULT TSMS_REG_writeRegisterByList(TSMS_RHLP list, uint8_t pos, uint32_t value) {
	if (list->maxSize <= pos || pos < 0)
		return TSMS_FAIL;
	uint8_t temp;
	if (list->types[pos] == TSMS_REGISTER_MSB)
		temp = value;
	else {
		temp = 0;
		for (uint8_t i = 0;i<list->sizes[pos];i++) {
			temp <<= 1;
			temp |= (value & (1<<i)) ? 1 : 0;
		}
	}
	return TSMS_REG_writeAt(list->regs[list->ids[pos]], list->starts[pos], list->sizes[pos], temp);
}

TSMS_RESULT TSMS_REG_readRegisterByList(TSMS_RHLP list, uint8_t pos, uint32_t* value) {
	if (list->maxSize <= pos || pos < 0)
		return TSMS_FAIL;
	uint32_t val;
	TSMS_RESULT result = TSMS_REG_readAt(list->regs[list->ids[pos]], list->starts[pos], list->sizes[pos], &val);
	if (result != TSMS_SUCCESS)
		return result;
	if (list->types[pos] == TSMS_REGISTER_MSB) {
		*value = val;
	} else {
		uint32_t temp = 0;
		for (uint8_t i = 0;i<list->sizes[pos];i++) {
			temp <<= 1;
			temp |= (val & (1<<i)) ? 1 : 0;
		}
		*value = temp;
	}
	return TSMS_SUCCESS;
}

uint32_t TSMS_REG_tempWriteRegisterByList(TSMS_RHLP list, uint8_t pos, uint32_t value) {
	if (list->maxSize <= pos || pos < 0)
		return TSMS_FAIL;
	uint8_t temp;
	if (list->types[pos] == TSMS_REGISTER_MSB)
		temp = value;
	else {
		temp = 0;
		for (uint8_t i = 0;i<list->sizes[pos];i++) {
			temp <<= 1;
			temp |= (value & (1<<i)) ? 1 : 0;
		}
	}
	return TSMS_REG_tempWriteAt(list->regs[list->ids[pos]], list->starts[pos], list->sizes[pos], temp);
}

uint32_t TSMS_REG_tempWriteAt(TSMS_RHP reg, uint8_t start, uint8_t bits, uint32_t value) {
	uint32_t mask = TSMS_MASK(bits);
	if (mask == 0)
		return 0;
	return reg->value | (value & mask)<<start;
}