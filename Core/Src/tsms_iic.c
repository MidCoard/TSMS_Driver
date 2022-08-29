#include "tsms_iic.h"

TSMS_INLINE static void __tsms_iic_delay() {
	volatile int v;
	int i;

	for (i = 0; i < 100; ++i) {
		v;
	}
}

TSMS_INLINE static void TSMS_IIC_SDA_HIGH(TSMS_IHP iic) {
	TSMS_GPIO_write(iic->sda, TSMS_GPIO_HIGH);
}

TSMS_INLINE static void TSMS_IIC_SDA_LOW(TSMS_IHP iic) {
	TSMS_GPIO_write(iic->sda, TSMS_GPIO_LOW);
}

TSMS_INLINE static void TSMS_IIC_SCL_HIGH(TSMS_IHP iic) {
	TSMS_GPIO_write(iic->scl, TSMS_GPIO_HIGH);
}

TSMS_INLINE static void TSMS_IIC_SCL_LOW(TSMS_IHP iic) {
	TSMS_GPIO_write(iic->scl, TSMS_GPIO_LOW);
}

TSMS_INLINE static void __tsms_internal_iic_release0(TSMS_IHP iic) {
#if defined(TSMS_STM32_IIC) && defined(HAL_I2C_MODULE_ENABLED)
	HAL_I2C_DeInit(iic->hardwareHandler);
#endif
	free(iic);
}

TSMS_INLINE static void __tsms_internal_iic_release1(TSMS_IHP iic) {
	TSMS_GPIO_release(iic->scl);
	TSMS_GPIO_release(iic->sda);
	free(iic);
}

TSMS_INLINE uint8_t TSMS_IIC_readBit(TSMS_IHP handler) {
	TSMS_IIC_SCL_HIGH(handler);
	TSMS_GPIO_STATUS status = TSMS_GPIO_read(handler->sda);
	handler->delay();
	TSMS_IIC_SCL_LOW(handler);
	handler->delay();
	return status;
}

TSMS_INLINE void TSMS_IIC_writeBit(TSMS_IHP handler, uint8_t bit) {
	if (bit)
		TSMS_IIC_SDA_HIGH(handler);
	else
		TSMS_IIC_SDA_LOW(handler);
	handler->delay();
	TSMS_IIC_SCL_HIGH(handler);
	handler->delay();
	TSMS_IIC_SCL_LOW(handler);
	handler->delay();
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
	iic->delay = __tsms_iic_delay;
	iic->release = __tsms_internal_iic_release1;
	return iic;
}


TSMS_RESULT TSMS_IIC_release(TSMS_IHP iic) {
	if (iic == TSMS_NULL)
		return TSMS_ERROR;
	iic->release(iic);
	return TSMS_SUCCESS;
}

uint8_t TSMS_IIC_read(TSMS_IHP handler, bool nack) {
	uint8_t v = 0;
	TSMS_GPIO_setMode(handler->sda, TSMS_GPIO_INPUT, TSMS_GPIO_PULL_UP);
	for (int i = 0; i < 8; i++)
		v = (v << 1u) | TSMS_IIC_readBit(handler);
	TSMS_GPIO_setMode(handler->sda, TSMS_GPIO_OUTPUT_PULL_PUSH, TSMS_GPIO_NO_PULL);
	TSMS_IIC_writeBit(handler,nack);
	return v;
}


TSMS_RESULT TSMS_IIC_start(TSMS_IHP handler) {
	TSMS_IIC_SDA_HIGH(handler);
	TSMS_IIC_SCL_HIGH(handler);
	handler->delay();
	TSMS_IIC_SDA_LOW(handler);
	handler->delay();
	TSMS_IIC_SCL_LOW(handler);
	handler->delay();
	return TSMS_SUCCESS;
}


TSMS_RESULT TSMS_IIC_stop(TSMS_IHP handler) {
	TSMS_IIC_SCL_LOW(handler);
	TSMS_IIC_SDA_LOW(handler);
	handler->delay();
	TSMS_IIC_SCL_HIGH(handler);
	handler->delay();
	TSMS_IIC_SDA_HIGH(handler);
	handler->delay();
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_IIC_write(TSMS_IHP handler,uint8_t v) {
	for (int i = 0; i < 8; i++) {
		TSMS_IIC_writeBit(handler,(v & (0x80)) ? TSMS_GPIO_HIGH : TSMS_GPIO_LOW);
		v <<= 1;
	}
	return TSMS_SUCCESS;
}

bool TSMS_IIC_wait(TSMS_IHP handler) {
	uint16_t time = 0;
	TSMS_IIC_SDA_HIGH(handler);
	TSMS_GPIO_setMode(handler->sda, TSMS_GPIO_INPUT, TSMS_GPIO_PULL_UP);
	handler->delay();
	TSMS_IIC_SCL_HIGH(handler);
	while (TSMS_GPIO_read(handler->sda)) {
		time++;
		if (time > 250) {
			TSMS_IIC_SCL_LOW(handler);
			TSMS_GPIO_setMode(handler->sda, TSMS_GPIO_OUTPUT_PULL_PUSH, TSMS_GPIO_NO_PULL);
			TSMS_IIC_stop(handler);
			return false;
		}
	}
	TSMS_IIC_SCL_LOW(handler);
	TSMS_GPIO_setMode(handler->sda, TSMS_GPIO_OUTPUT_PULL_PUSH, TSMS_GPIO_NO_PULL);
	return true;
}
