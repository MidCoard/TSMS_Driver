#include "tsms_iic.h"
#include "tsms_util.h"

TSMS_INLINE void __tsms_internal_iic_delay() {
	volatile int v;
	int i;

	for (i = 0; i < 3; ++i) {
		v;
	}
}

TSMS_INLINE void __tsms_internal_iic_sda_high(TSMS_IHP iic) {
	TSMS_GPIO_write(iic->sda, TSMS_GPIO_STATUS_HIGH);
}

TSMS_INLINE void __tsms_internal_iic_sda_low(TSMS_IHP iic) {
	TSMS_GPIO_write(iic->sda, TSMS_GPIO_STATUS_LOW);
}

TSMS_INLINE void __tsms_internal_iic_scl_high(TSMS_IHP iic) {
	TSMS_GPIO_write(iic->scl, TSMS_GPIO_STATUS_HIGH);
}

TSMS_INLINE void __tsms_internal_iic_scl_low(TSMS_IHP iic) {
	TSMS_GPIO_write(iic->scl, TSMS_GPIO_STATUS_LOW);
}

TSMS_INLINE void __tsms_internal_iic_release0(TSMS_IHP iic) {
#ifdef TSMS_STM32_IIC
	HAL_I2C_DeInit(iic->hardwareHandler);
#endif
	free(iic);
}

TSMS_INLINE void __tsms_internal_iic_release1(TSMS_IHP iic) {
	TSMS_GPIO_release(iic->scl);
	TSMS_GPIO_release(iic->sda);
	free(iic);
}

TSMS_GPIO_STATUS TSMS_IIC_readBit(TSMS_IHP handler) {
	if (handler == TSMS_NULL)
		return TSMS_GPIO_STATUS_ERROR;
	handler->delay();
	handler->delay();
	__tsms_internal_iic_scl_high(handler);
	handler->delay();
	TSMS_GPIO_STATUS status = TSMS_GPIO_read(handler->sda);
	handler->delay();
	__tsms_internal_iic_scl_low(handler);
	return status;
}

TSMS_RESULT TSMS_IIC_writeBit(TSMS_IHP handler, uint8_t bit) {
	if (handler == TSMS_NULL)
		return TSMS_ERROR;
	handler->delay();
	if (bit)
		__tsms_internal_iic_sda_high(handler);
	else
		__tsms_internal_iic_sda_low(handler);
	handler->delay();
	__tsms_internal_iic_scl_high(handler);
	handler->delay();
	handler->delay();
	__tsms_internal_iic_scl_low(handler);
	return TSMS_SUCCESS;
}

TSMS_IHP TSMS_IIC_createSoftwareIIC(TSMS_GHP sda, TSMS_GHP scl, uint8_t address, TSMS_TRANSFER_TYPE type) {
	TSMS_IHP iic = TSMS_malloc(sizeof(struct TSMS_IIC_HANDLER));
	if (iic == TSMS_NULL)
		return TSMS_NULL;
	iic->sda = sda;
	iic->scl = scl;
	__tsms_internal_iic_sda_high(iic);
	__tsms_internal_iic_scl_high(iic);
	iic->isHardware = false;
	iic->delay = __tsms_internal_iic_delay;
	iic->release = __tsms_internal_iic_release0;
	iic->address = address;
	iic->type = type;
	return iic;
}

#ifdef TSMS_STM32_IIC

TSMS_IHP TSMS_IIC_createHardwareIIC(I2C_HandleTypeDef *handler, uint8_t address, TSMS_TRANSFER_TYPE type) {
	TSMS_IHP iic = TSMS_malloc(sizeof(struct TSMS_IIC_HANDLER));
	if (iic == TSMS_NULL)
		return TSMS_NULL;
	iic->hardwareHandler = handler;
	iic->isHardware = true;
	iic->release = __tsms_internal_iic_release0;
	iic->address = address;
	iic->type = type;
	return iic;
}

#endif


TSMS_RESULT TSMS_IIC_release(TSMS_IHP iic) {
	if (iic == TSMS_NULL)
		return TSMS_ERROR;
	iic->release(iic);
	return TSMS_SUCCESS;
}

uint8_t TSMS_IIC_read(TSMS_IHP handler, bool nack) {
	if (handler == TSMS_NULL)
		return 0;
	uint8_t v = 0;
	for (int i = 0; i < 8; i++)
		v = (v << 1u) | TSMS_IIC_readBit(handler);
	handler->delay();
	if (nack)
		__tsms_internal_iic_sda_high(handler);
	else
		__tsms_internal_iic_sda_low(handler);
	handler->delay();
	__tsms_internal_iic_scl_high(handler);
	handler->delay();
	handler->delay();
	__tsms_internal_iic_scl_low(handler);
	__tsms_internal_iic_sda_high(handler);
	return v;
}


TSMS_RESULT TSMS_IIC_start(TSMS_IHP handler) {
	if (handler == TSMS_NULL)
		return TSMS_ERROR;
	__tsms_internal_iic_sda_high(handler);
	__tsms_internal_iic_scl_high(handler);
	handler->delay();
	__tsms_internal_iic_sda_low(handler);
	handler->delay();
	__tsms_internal_iic_scl_low(handler);
	handler->delay();
	return TSMS_SUCCESS;
}


TSMS_RESULT TSMS_IIC_stop(TSMS_IHP handler) {
	if (handler == TSMS_NULL)
		return TSMS_ERROR;
	__tsms_internal_iic_scl_low(handler);
	__tsms_internal_iic_sda_low(handler);
	handler->delay();
	__tsms_internal_iic_scl_high(handler);
	handler->delay();
	__tsms_internal_iic_sda_high(handler);
	handler->delay();
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_IIC_write(TSMS_IHP handler, uint8_t v) {
	if (handler == TSMS_NULL)
		return TSMS_ERROR;
	for (int i = 0; i < 8; i++) {
		TSMS_IIC_writeBit(handler, (v & (0x80)) ? TSMS_GPIO_STATUS_HIGH : TSMS_GPIO_STATUS_LOW);
		v <<= 1;
	}
	return TSMS_SUCCESS;
}

bool TSMS_IIC_wait(TSMS_IHP handler) {
	if (handler == TSMS_NULL)
		return false;
	__tsms_internal_iic_sda_high(handler);
	handler->delay();
	handler->delay();
	__tsms_internal_iic_scl_high(handler);
	handler->delay();
	if (TSMS_GPIO_read(handler->sda)) {
		handler->delay();
		__tsms_internal_iic_scl_low(handler);
		TSMS_IIC_stop(handler);
		return false;
	}
	handler->delay();
	__tsms_internal_iic_scl_low(handler);
	return true;
}

TSMS_RESULT TSMS_IIC_writeBytes(TSMS_IHP handler, uint8_t *data, uint16_t length) {
	if (handler == TSMS_NULL)
		return TSMS_ERROR;
	if (handler->isHardware) {
#ifdef TSMS_STM32_IIC
		if (HAL_I2C_Master_Transmit(handler->hardwareHandler, handler->address, data, length, 0xffffffff) == HAL_OK)
			return TSMS_SUCCESS;
		else return TSMS_ERROR;
#else
		return TSMS_FAIL;
#endif
	} else {
		TSMS_IIC_start(handler);
		TSMS_IIC_write(handler, (handler->address << 1u) | TSMS_IIC_WRITE);
		if (!TSMS_IIC_wait(handler))
			return TSMS_ERROR;
		for (int i = 0; i < length; i++) {
			TSMS_IIC_write(handler, data[i]);
			if (!TSMS_IIC_wait(handler))
				return TSMS_ERROR;
		}
		TSMS_IIC_stop(handler);
		return TSMS_SUCCESS;
	}
}

TSMS_RESULT TSMS_IIC_readBytes(TSMS_IHP handler, uint8_t *data, uint16_t length) {
	if (handler == TSMS_NULL)
		return TSMS_ERROR;
	if (handler->isHardware) {
#ifdef TSMS_STM32_IIC
		if (HAL_I2C_Master_Receive(handler->hardwareHandler, handler->address, data, length, 0xffffffff) == HAL_OK)
			return TSMS_SUCCESS;
		else return TSMS_ERROR;
#else
		return TSMS_FAIL;
#endif
	} else {
		TSMS_IIC_start(handler);
		TSMS_IIC_write(handler, (handler->address << 1u) | TSMS_IIC_READ);
		if (!TSMS_IIC_wait(handler))
			return TSMS_ERROR;
		for (int i = 0; i < length; i++)
			data[i] = TSMS_IIC_read(handler, i == length - 1);
		TSMS_IIC_stop(handler);
		return TSMS_SUCCESS;
	}
}

TSMS_RESULT TSMS_IIC_writeCustomRegister(TSMS_IHP handler, uint32_t reg, TSMS_BITS regBits, uint32_t data, TSMS_BITS bits) {
	if (handler == TSMS_NULL)
		return TSMS_ERROR;
	static uint8_t buffer[8];
	if (handler->type == TSMS_TRANSFER_TYPE_LSB) {
		for (int i = 0; i < regBits + 1; i++)
			buffer[i] = TSMS_UTIL_reverseByte((reg >> (i * 8)) & 0xff);
		for (int i = 0; i < bits + 1; i++)
			buffer[i + regBits + 1] = TSMS_UTIL_reverseByte((data >> (i * 8)) & 0xff);
	}
	else {
		for (int i = regBits; i >= 0; i--)
			buffer[regBits - i] = (reg >> (i * 8)) & 0xff;
		for (int i = bits; i >= 0; i--)
			buffer[bits - i + regBits + 1] = (data >> (i * 8)) & 0xff;
	}
	return TSMS_IIC_writeBytes(handler, buffer, regBits + bits + 2);
}

TSMS_RESULT TSMS_IIC_readCustomRegister(TSMS_IHP handler, uint32_t reg, TSMS_BITS regBits, uint32_t *data, TSMS_BITS bits) {
	if (handler == TSMS_NULL)
		return TSMS_ERROR;
	static uint8_t buffer[4];
	if (handler->type == TSMS_TRANSFER_TYPE_LSB)
		for (int i = 0; i < regBits + 1; i++)
			buffer[i] = TSMS_UTIL_reverseByte((reg >> (i * 8)) & 0xff);
	else
		for (int i = regBits; i >= 0; i--)
			buffer[regBits - i] = (reg >> (i * 8)) & 0xff;
	TSMS_RESULT result = TSMS_IIC_writeBytes(handler, buffer, regBits + 1);
	if (result != TSMS_SUCCESS)
		return result;
	result = TSMS_IIC_readBytes(handler, buffer, bits + 1);
	if (result != TSMS_SUCCESS)
		return result;
	*data = 0;
	if (handler->type == TSMS_TRANSFER_TYPE_LSB)
		for (int i = 0; i < bits + 1; i++)
			*data |= TSMS_UTIL_reverseByte(buffer[i]) << (i * 8);
	else
		for (int i = bits; i >= 0; i--)
			*data |= buffer[bits - i] << (i * 8);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_IIC_init(TSMS_CLOCK_FREQUENCY frequency) {
	return TSMS_SUCCESS;
}