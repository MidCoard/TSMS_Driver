#include "tsms_spi.h"

TSMS_INLINE void __tsms_internal_spi_delay() {
//	volatile uint8_t c = 1;
//	while(c--);
}

TSMS_INLINE void __tsms_internal_spi_sclk_high(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->sclk, TSMS_GPIO_STATUS_HIGH);
}

TSMS_INLINE void __tsms_internal_spi_sclk_low(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->sclk, TSMS_GPIO_STATUS_LOW);
}

TSMS_INLINE void __tsms_internal_spi_cs_high(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->cs, TSMS_GPIO_STATUS_HIGH);
}

TSMS_INLINE void __tsms_internal_spi_cs_low(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->cs, TSMS_GPIO_STATUS_LOW);
}

TSMS_INLINE void __tsms_internal_spi_dout_high(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->dout, TSMS_GPIO_STATUS_HIGH);
}

TSMS_INLINE void __tsms_internal_spi_dout_low(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->dout, TSMS_GPIO_STATUS_LOW);
}

TSMS_INLINE TSMS_GPIO_STATUS __tsms_internal_spi_din(TSMS_SHP spi) {
	return TSMS_GPIO_read(spi->din);
}

TSMS_INLINE void __tsms_internal_spi_receive_custom_bit(TSMS_SHP spi, uint8_t bits, uint32_t *data) {
	*data = 0;
	if (!TSMS_SPI_MODE_CPHA(spi->mode)) {
		for (uint8_t i = 0; i < bits; i++) {
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			if (!TSMS_SPI_MODE_CPHA(spi->mode))
				*data |= __tsms_internal_spi_din(spi) << (spi->type == TSMS_TRANSFER_TYPE_MSB ? bits - i - 1 : i);
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_GPIO_write(spi->sclk, !TSMS_SPI_MODE_CPOL(spi->mode));
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_GPIO_write(spi->sclk, TSMS_SPI_MODE_CPOL(spi->mode));
		}
	} else {
		for (uint8_t i = 0; i < bits; i++) {
			TSMS_GPIO_write(spi->sclk, !TSMS_SPI_MODE_CPOL(spi->mode));
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			if (TSMS_SPI_MODE_CPHA(spi->mode))
				*data |= __tsms_internal_spi_din(spi) << (spi->type == TSMS_TRANSFER_TYPE_MSB ? bits - i - 1 : i);
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_GPIO_write(spi->sclk, TSMS_SPI_MODE_CPOL(spi->mode));
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		}
	}
}

TSMS_INLINE void __tsms_internal_spi_transmit_custom_bit(TSMS_SHP spi, uint8_t bits, uint32_t data) {
	if (!TSMS_SPI_MODE_CPHA(spi->mode)) {
		for (uint8_t i = 0; i < bits; i++) {
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			if (!TSMS_SPI_MODE_CPHA(spi->mode))
				TSMS_GPIO_write(spi->dout, (data >> (spi->type == TSMS_TRANSFER_TYPE_MSB ? bits - i - 1 : i)) & 0x01);
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_GPIO_write(spi->sclk, !TSMS_SPI_MODE_CPOL(spi->mode));
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_GPIO_write(spi->sclk, TSMS_SPI_MODE_CPOL(spi->mode));
		}
	} else {
		for (uint8_t i = 0; i < bits; i++) {
			TSMS_GPIO_write(spi->sclk, !TSMS_SPI_MODE_CPOL(spi->mode));
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			if (TSMS_SPI_MODE_CPHA(spi->mode))
				TSMS_GPIO_write(spi->dout, (data >> (spi->type == TSMS_TRANSFER_TYPE_MSB ? bits - i - 1 : i)) & 0x01);
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_GPIO_write(spi->sclk, TSMS_SPI_MODE_CPOL(spi->mode));
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		}
	}

}

TSMS_INLINE void __tsms_internal_spi_release0(TSMS_SHP spi) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
	HAL_SPI_DeInit(spi->hardwareHandler);
#endif
	free(spi);
}

TSMS_INLINE void __tsms_internal_spi_release1(TSMS_SHP spi) {
	TSMS_GPIO_release(spi->cs);
	TSMS_GPIO_release(spi->sclk);
	TSMS_GPIO_release(spi->dout);
	TSMS_GPIO_release(spi->din);
	__tsms_internal_spi_release0(spi);
}



TSMS_SHP TSMS_SPI_createSoftwareHandler(TSMS_GHP cs, TSMS_GHP sclk, TSMS_GHP din, TSMS_GHP dout, TSMS_SPI_MODE mode,
                                        TSMS_GPIO_STATUS csValid, TSMS_TRANSFER_TYPE type) {
	TSMS_SHP spi = TSMS_malloc(sizeof(struct TSMS_SPI_HANDLER));
	if (spi == TSMS_NULL)
		return TSMS_NULL;
	spi->cs = cs;
	spi->sclk = sclk;
	spi->dout = dout;
	spi->din = din;
	spi->mode = mode;
	spi->csValid = csValid;
	spi->customDelay = TSMS_NULL;
	spi->delay = __tsms_internal_spi_delay;
	spi->type = type;
	spi->isHardware = false;
	spi->release = __tsms_internal_spi_release0;
	if (spi->mode == TSMS_SPI_MODE_0 || spi->mode == TSMS_SPI_MODE_1)
		__tsms_internal_spi_sclk_low(spi);
	else __tsms_internal_spi_sclk_high(spi);
	return spi;
}

#ifdef TSMS_STM32_SPI

TSMS_SHP TSMS_SPI_createHardwareHandler(SPI_HandleTypeDef * spi) {
	TSMS_SHP handler = TSMS_malloc(sizeof (struct TSMS_SPI_HANDLER));
	if (handler == TSMS_NULL)
		return TSMS_NULL;
	handler->hardwareHandler = spi;
	handler->isHardware = true;
	handler->release = __tsms_internal_spi_release0;
	return handler;
}

#endif


TSMS_RESULT TSMS_SPI_transmitCustomBits(TSMS_SHP spi, uint32_t *data, uint8_t bits, uint32_t length) {
	if (spi == TSMS_NULL)
		return TSMS_ERROR;
	if (spi->isHardware) {
#ifdef TSMS_STM32_SPI
		// todo ????????
		return TSMS_ERROR;
#else
		return TSMS_FAIL;
#endif
	} else {
		if (spi->csValid == TSMS_GPIO_STATUS_HIGH)
			__tsms_internal_spi_cs_high(spi);
		else __tsms_internal_spi_cs_low(spi);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		for (TSMS_POS i = 0; i < length; i++)
			__tsms_internal_spi_transmit_custom_bit(spi, bits, data[i]);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		if (spi->csValid == TSMS_GPIO_STATUS_HIGH)
			__tsms_internal_spi_cs_low(spi);
		else __tsms_internal_spi_cs_high(spi);
		return TSMS_SUCCESS;
	}
}

TSMS_RESULT TSMS_SPI_receiveCustomBits(TSMS_SHP spi, uint32_t *data, uint8_t bits, uint32_t length) {
	if (spi == TSMS_NULL)
		return TSMS_ERROR;
	if (spi->isHardware) {
#ifdef TSMS_STM32_SPI
		// todo ????????
		return TSMS_ERROR;
#else
		return TSMS_FAIL;
#endif
	} else {
		if (spi->csValid == TSMS_GPIO_STATUS_HIGH)
			__tsms_internal_spi_cs_high(spi);
		else __tsms_internal_spi_cs_low(spi);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		for (uint32_t i = 0; i < length; i++)
			__tsms_internal_spi_receive_custom_bit(spi, bits, data + i);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		if (spi->csValid == TSMS_GPIO_STATUS_HIGH)
			__tsms_internal_spi_cs_low(spi);
		else __tsms_internal_spi_cs_high(spi);
		return TSMS_SUCCESS;
	}
}

TSMS_RESULT TSMS_SPI_transform(TSMS_SHP spi, uint32_t *data, uint8_t writeBits, uint8_t readBits, uint32_t writeLength,
                               uint32_t readLength) {
	if (spi == TSMS_NULL)
		return TSMS_ERROR;
	if (spi->isHardware) {
#ifdef TSMS_STM32_SPI
		// todo ????????
		return TSMS_ERROR;
#else
		return TSMS_FAIL;
#endif
	} else {
		if (spi->csValid == TSMS_GPIO_STATUS_HIGH)
			__tsms_internal_spi_cs_high(spi);
		else __tsms_internal_spi_cs_low(spi);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		for (uint32_t i = 0; i < writeLength; i++)
			__tsms_internal_spi_transmit_custom_bit(spi, writeBits, data[i]);
		for (uint32_t i = writeLength; i < writeLength + readLength; i++)
			__tsms_internal_spi_receive_custom_bit(spi, readBits, data + i);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		if (spi->csValid == TSMS_GPIO_STATUS_HIGH)
			__tsms_internal_spi_cs_low(spi);
		else __tsms_internal_spi_cs_high(spi);
		return TSMS_SUCCESS;
	}
}

TSMS_RESULT TSMS_SPI_sequenceTransform(TSMS_SHP spi, uint32_t n, ...) {
	if (spi == TSMS_NULL)
		return TSMS_FAIL;
	if (spi->isHardware) {
#ifdef TSMS_STM32_SPI
		// todo ????????
		return TSMS_ERROR;
#else
		return TSMS_FAIL;
#endif
	} else {
		va_list args;
		va_start(args, n);
		if (spi->csValid == TSMS_GPIO_STATUS_HIGH)
			__tsms_internal_spi_cs_high(spi);
		else __tsms_internal_spi_cs_low(spi);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		for (uint32_t i = 0; i < n; i++) {
			struct TSMS_SPI_OPERATION operation = va_arg(args, struct TSMS_SPI_OPERATION);
			if (operation.type == TSMS_SPI_OPERATION_TYPE_READ)
				TSMS_SPI_receiveCustomBits(spi, operation.data, operation.bits, operation.length);
			else if (operation.type == TSMS_SPI_OPERATION_TYPE_WRITE)
				TSMS_SPI_transmitCustomBits(spi, operation.data, operation.bits, operation.length);
		}
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		if (spi->csValid == TSMS_GPIO_STATUS_HIGH)
			__tsms_internal_spi_cs_low(spi);
		else __tsms_internal_spi_cs_high(spi);
		va_end(args);
		return TSMS_SUCCESS;
	}
}

TSMS_RESULT TSMS_SPI_release(TSMS_SHP spi) {
	if (spi == TSMS_NULL)
		return TSMS_ERROR;
	spi->release(spi);
	return TSMS_SUCCESS;
}

TSMS_SPI_MODE TSMS_SPI_mode(TSMS_SPI_CPOL cpol, TSMS_SPI_CPHA cpha) {
	uint8_t value = cpol * 2 + cpha;
	return (TSMS_SPI_MODE) value;
}

TSMS_RESULT TSMS_SPI_init(TSMS_CLOCK_FREQUENCY frequency) {
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_SPI_delay(TSMS_SHP spi, TSMS_DELAY_TIME time) {
	if (spi == TSMS_NULL)
		return TSMS_ERROR;
	if (spi->customDelay != TSMS_NULL)
		spi->customDelay(time);
	else spi->delay();
	return TSMS_SUCCESS;
}