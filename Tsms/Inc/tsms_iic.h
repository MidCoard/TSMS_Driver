#ifndef TSMS_IIC_H
#define TSMS_IIC_H

#define TSMS_IIC_WRITE 0
#define TSMS_IIC_READ 1

typedef struct TSMS_IIC_HANDLER *TSMS_IIC_HANDLER_POINTER;
typedef TSMS_IIC_HANDLER_POINTER TSMS_IHP;

typedef void(*TSMS_IIC_RELEASE_FUNCTION)(TSMS_IHP);

#include "tsms_gpio.h"

struct TSMS_IIC_HANDLER {
	TSMS_GHP sda;
	TSMS_GHP scl;
	bool isHardware;
	TSMS_DELAY_FUNCTION delay;
	TSMS_IIC_RELEASE_FUNCTION release;
	TSMS_TRANSFER_TYPE type;
	uint8_t address;

#ifdef TSMS_STM32_IIC
	I2C_HandleTypeDef * hardwareHandler;
#endif
};

#ifdef TSMS_STM32_IIC

TSMS_IHP TSMS_IIC_createHardwareIIC(I2C_HandleTypeDef *handler, uint8_t address, TSMS_TRANSFER_TYPE type);

#endif

TSMS_IHP TSMS_IIC_createSoftwareIIC(TSMS_GHP sda, TSMS_GHP scl, uint8_t address, TSMS_TRANSFER_TYPE type);

TSMS_RESULT TSMS_IIC_release(TSMS_IHP iic);

uint8_t TSMS_IIC_read(TSMS_IHP handler, bool nack);

TSMS_RESULT TSMS_IIC_write(TSMS_IHP handler, uint8_t);

bool TSMS_IIC_wait(TSMS_IHP handler);

TSMS_RESULT TSMS_IIC_start(TSMS_IHP handler);

TSMS_RESULT TSMS_IIC_stop(TSMS_IHP handler);

TSMS_RESULT TSMS_IIC_writeBytes(TSMS_IHP handler, uint8_t *data, uint16_t length);

TSMS_RESULT TSMS_IIC_readBytes(TSMS_IHP handler, uint8_t *data, uint16_t length);

TSMS_RESULT TSMS_IIC_writeCustomRegister(TSMS_IHP handler, uint32_t reg, TSMS_BITS regBits, uint32_t data, TSMS_BITS bits);

TSMS_RESULT TSMS_IIC_readCustomRegister(TSMS_IHP handler, uint32_t reg, TSMS_BITS regBits, uint32_t *data, TSMS_BITS bits);


#endif //TSMS_IIC_H