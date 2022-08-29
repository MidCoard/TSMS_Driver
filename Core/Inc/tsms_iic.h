#ifndef TSMS_IIC_H
#define TSMS_IIC_H

#include "tsms_gpio.h"

typedef struct TSMS_IIC_HANDLER * TSMS_IIC_HANDLER_POINTER;
typedef TSMS_IIC_HANDLER_POINTER TSMS_IHP;

typedef void(*TSMS_IIC_RELEASE_FUNCTION)(TSMS_IHP);

struct TSMS_IIC_HANDLER {
	TSMS_GHP sda;
	TSMS_GHP scl;
	bool isHardware;
	TSMS_DELAY_FUNCTION delay;
	TSMS_IIC_RELEASE_FUNCTION release;

#if defined(TSMS_STM32_IIC) && defined(HAL_I2C_MODULE_ENABLED)
	I2C_HandleTypeDef * hardwareHandler;
#endif
};

TSMS_IHP TSMS_IIC_createSoftwareIIC(TSMS_GHP sda, TSMS_GHP scl);

#if defined(TSMS_STM32_IIC) && defined(HAL_I2C_MODULE_ENABLED)
TSMS_IHP TSMS_IIC_createHardwareIIC(I2C_HandleTypeDef *handler);
#endif


#endif //TSMS_IIC_H