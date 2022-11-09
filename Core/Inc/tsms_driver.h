#ifndef TSMS_DRIVER_H
#define TSMS_DRIVER_H


#include "tsms_spi.h"
#include "tsms_iic.h"
#include "tsms_custom.h"

struct TSMS_REGISTER_HANDLER {
	uint8_t bits;
	uint32_t value;
	uint8_t *positions;
};

struct TSMS_REGISTER_HANDLER_LIST;

typedef struct TSMS_REGISTER_HANDLER_LIST * TSMS_REGISTER_HANDLER_LIST_POINTER;
typedef TSMS_REGISTER_HANDLER_LIST_POINTER TSMS_RHLP;

typedef enum{
	TSMS_DRIVER_SPI, TSMS_DRIVER_IIC, TSMS_DRIVER_CUSTOM
} TSMS_DRIVER_TYPE;

typedef TSMS_RESULT(*TSMS_DRIVER_SPI_WRITER)(TSMS_SHP, uint32_t*, uint8_t, uint32_t);
typedef TSMS_RESULT(*TSMS_DRIVER_SPI_READER)(TSMS_SHP, uint32_t*, uint8_t, uint32_t);

typedef TSMS_RESULT(*TSMS_DRIVER_SPI_TRANSFORM)(TSMS_SHP, uint32_t*, uint8_t, uint8_t , uint32_t ,uint32_t);

typedef TSMS_RESULT(*TSMS_DRIVER_SPI_SEQUENCE_TRANSFORM)(TSMS_SHP, uint32_t,...);

typedef TSMS_RESULT(*TSMS_DRIVER_IIC_WRITER)(TSMS_IHP, uint8_t , uint32_t, TSMS_BITS);
typedef TSMS_RESULT(*TSMS_DRIVER_IIC_READER)(TSMS_IHP, uint8_t , uint32_t*, TSMS_BITS);

struct TSMS_DRIVER_HANDLER {
	TSMS_DRIVER_TYPE type;
	TSMS_SHP spi;
	TSMS_IHP iic;
	TSMS_CHP custom;
	TSMS_RHLP regs;
	TSMS_DRIVER_SPI_WRITER spiWrite;
	TSMS_DRIVER_SPI_READER spiRead;
	TSMS_DRIVER_SPI_TRANSFORM spiTransform;
	TSMS_DRIVER_SPI_SEQUENCE_TRANSFORM spiSequenceTransform;
	TSMS_DRIVER_IIC_WRITER iicWrite;
	TSMS_DRIVER_IIC_READER iicRead;
};

typedef struct TSMS_REGISTER_HANDLER * TSMS_REGISTER_HANDLER_POINTER;
typedef TSMS_REGISTER_HANDLER_POINTER  TSMS_RHP;

typedef struct TSMS_DRIVER_HANDLER * TSMS_DRIVER_HANDLER_POINTER;
typedef TSMS_DRIVER_HANDLER_POINTER TSMS_DHP;

struct TSMS_REGISTER_HANDLER_LIST {
	TSMS_RHP* regs;
	int size;
	TSMS_REGISTER_DATA_TYPE * types;
	uint8_t *sizes;
	uint8_t *starts;
	uint8_t *ids;
	uint8_t maxSize;
};

#define TSMS_REGISTER_8BIT uint8_t bit0,uint8_t bit1, uint8_t bit2,uint8_t bit3,uint8_t bit4,uint8_t bit5,uint8_t bit6,uint8_t bit7
#define TSMS_REGISTER_16BIT TSMS_REGISTER_8BIT,uint8_t bit8,uint8_t bit9,uint8_t bit10,uint8_t bit11,uint8_t bit12,uint8_t bit13,uint8_t bit14,uint8_t bit15
#define TSMS_REGISTER_24BIT TSMS_REGISTER_16BIT,uint8_t bit16,uint8_t bit17,uint8_t bit18,uint8_t bit19,uint8_t bit20,uint8_t bit21,uint8_t bit22,uint8_t bit23
#define TSMS_REGISTER_32BIT TSMS_REGISTER_24BIT,uint8_t bit24,uint8_t bit25,uint8_t bit26,uint8_t bit27,uint8_t bit28,uint8_t bit29,uint8_t bit30,uint8_t bit31

TSMS_DHP TSMS_DRIVER_createSPIHandler(TSMS_SHP spi, TSMS_RHLP regs);

TSMS_DHP TSMS_DRIVER_createCustomHandler(TSMS_CHP custom, TSMS_RHLP regs);

TSMS_DHP TSMS_DRIVER_createIICHandler(TSMS_IHP iic, TSMS_RHLP regs);

TSMS_RHP TSMS_REG_Register(uint8_t bits);

TSMS_RHP TSMS_REG_8BitRegister(TSMS_REGISTER_8BIT);

TSMS_RHP TSMS_REG_16BitRegister(TSMS_REGISTER_16BIT);

TSMS_RHP TSMS_REG_24BitRegister(TSMS_REGISTER_24BIT);

TSMS_RHP TSMS_REG_32BitRegister(TSMS_REGISTER_32BIT);

TSMS_RESULT TSMS_REG_writeAt(TSMS_RHP reg, uint8_t start, uint8_t bits, uint32_t value);
uint32_t TSMS_REG_tempWriteAt(TSMS_RHP reg, uint8_t start, uint8_t bits, uint32_t value);

TSMS_RESULT TSMS_REG_readAt(TSMS_RHP reg, uint8_t start, uint8_t bits, uint32_t* value);

TSMS_RESULT TSMS_REG_configure(TSMS_RHLP list, uint8_t reg, uint8_t pos, uint8_t left, uint8_t right, TSMS_REGISTER_DATA_TYPE type);

TSMS_RESULT TSMS_REG_release(TSMS_RHP reg);

TSMS_RHLP TSMS_REG_createList(int n,...);

TSMS_RESULT TSMS_REG_releaseList(TSMS_RHLP list);

TSMS_RESULT TSMS_REG_setRegister(TSMS_RHP reg, uint32_t value);

uint32_t TSMS_REG_getRegister(TSMS_RHP reg);

uint32_t TSMS_REG_getRegisterByList(TSMS_RHLP list, uint8_t pos);

TSMS_RESULT TSMS_REG_setRegisterByList(TSMS_RHLP list, uint8_t pos, uint32_t value);

TSMS_RESULT TSMS_REG_writeRegisterByList(TSMS_RHLP list, uint8_t pos, uint32_t value);

TSMS_RESULT TSMS_REG_readRegisterByList(TSMS_RHLP list, uint8_t pos, uint32_t* value);

uint32_t TSMS_REG_tempWriteRegisterByList(TSMS_RHLP list, uint8_t pos, uint32_t value);

#endif //TSMS_DRIVER_H

