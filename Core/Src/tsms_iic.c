#include "tsms_iic.h"


static void __tsms_internal_iic_release0(TSMS_IHP iic) {
#if defined(TSMS_STM32_IIC) && defined(HAL_I2C_MODULE_ENABLED)
	HAL_I2C_DeInit(iic->hardwareHandler);
#endif
	free(iic);
}

static void __tsms_internal_iic_release1(TSMS_IHP iic) {
	TSMS_GPIO_release(iic->scl);
	TSMS_GPIO_release(iic->sda);
	free(iic);
}

#if defined(TSMS_STM32_IIC) && defined(HAL_I2C_MODULE_ENABLED)
TSMS_IHP TSMS_IIC_createHardwareIIC(I2C_HandleTypeDef *handler) {
	TSMS_IHP iic = malloc(sizeof(struct TSMS_IIC_HANDLER));
	if (iic == TSMS_NULL)
		return TSMS_NULL;
	iic->hardwareHandler = handler;
	iic->isHardware = true;
	iic->release = __tsms_internal_iic_release0;
	return iic;
}
#endif


TSMS_IHP TSMS_IIC_createSoftwareIIC(TSMS_GHP sda, TSMS_GHP scl) {
	TSMS_IHP iic = malloc(sizeof(struct TSMS_IIC_HANDLER));
	if (iic == TSMS_NULL)
		return TSMS_NULL;
	iic->sda = sda;
	iic->scl = scl;
	iic->isHardware = false;
	iic->release = __tsms_internal_iic_release1;
	return iic;
}


TSMS_RESULT TSMS_IIC_release(TSMS_IHP iic) {
	if (iic == TSMS_NULL)
		return TSMS_ERROR;
	iic->release(iic);
	return TSMS_SUCCESS;
}


