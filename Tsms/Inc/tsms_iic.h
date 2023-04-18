#ifndef TSMS_IIC_H
#define TSMS_IIC_H

#include "tsms_gpio.h"

#define TSMS_IIC_WRITE 0
#define TSMS_IIC_READ 1

typedef struct TSMS_IIC_HANDLER *TSMS_IIC_HANDLER_POINTER;
typedef TSMS_IIC_HANDLER_POINTER TSMS_IHP;

typedef void(*TSMS_IIC_RELEASE_FUNCTION)(TSMS_IHP);

struct TSMS_IIC_HANDLER {
	TSMS_GHP sda;
	TSMS_GHP scl;
	bool isHardware;
	TSMS_DELAY_FUNCTION delay;
	TSMS_IIC_RELEASE_FUNCTION release;
	TSMS_TRANSFER_TYPE type;
	uint8_t address;

#if defined(TSMS_STM32_IIC) && defined(HAL_I2C_MODULE_ENABLED)
	I2C_HandleTypeDef * hardwareHandler;
#endif
};

#if defined(TSMS_STM32_IIC) && defined(HAL_I2C_MODULE_ENABLED)
#if defined(TSMS_STM32_IIC_USE_HAL_GPIO)
TSMS_IHP TSMS_IIC_createSoftwareIIC(GPIO_TypeDef * csPort, uint16_t csPin,
									GPIO_TypeDef * sclkPort, uint16_t sclkPin,
									uint8_t address, TSMS_TRANSFER_TYPE type);
#else
TSMS_IHP TSMS_IIC_createSoftwareIIC(TSMS_GHP sda, TSMS_GHP scl, uint8_t address, TSMS_TRANSFER_TYPE type);
#endif
TSMS_IHP TSMS_IIC_createHardwareIIC(I2C_HandleTypeDef *handler, uint8_t address, TSMS_TRANSFER_TYPE type);
#else

TSMS_IHP TSMS_IIC_createSoftwareIIC(TSMS_GHP sda, TSMS_GHP scl, uint8_t address, TSMS_TRANSFER_TYPE type);

#endif

TSMS_RESULT TSMS_IIC_release(TSMS_IHP iic);

uint8_t TSMS_IIC_read(TSMS_IHP handler, bool nack);

TSMS_RESULT TSMS_IIC_write(TSMS_IHP handler, uint8_t);

bool TSMS_IIC_wait(TSMS_IHP handler);

TSMS_RESULT TSMS_IIC_start(TSMS_IHP handler);

TSMS_RESULT TSMS_IIC_stop(TSMS_IHP handler);

TSMS_RESULT TSMS_IIC_writeBytes(TSMS_IHP handler, uint8_t *data, uint16_t length);

TSMS_RESULT TSMS_IIC_readBytes(TSMS_IHP handler, uint8_t *data, uint16_t length);

TSMS_RESULT TSMS_IIC_writeCustomRegister(TSMS_IHP handler, uint8_t reg, TSMS_BITS regBits, uint32_t data, TSMS_BITS bits);

TSMS_RESULT TSMS_IIC_readCustomRegister(TSMS_IHP handler, uint8_t reg, TSMS_BITS regBits, uint32_t *data, TSMS_BITS bits);


#endif //TSMS_IIC_H