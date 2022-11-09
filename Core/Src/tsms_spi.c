#include "tsms_spi.h"
#include "tsms_driver.h"

TSMS_INLINE static void __tsms_internal_spi_delay() {
	volatile uint8_t c = 1;
	while(c--);
}

TSMS_INLINE static void TSMS_SPI_SCLK_HIGH(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->sclk, TSMS_GPIO_HIGH);
}

TSMS_INLINE static void TSMS_SPI_SCLK_LOW(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->sclk, TSMS_GPIO_LOW);
}

TSMS_INLINE static void TSMS_SPI_CS_HIGH(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->cs, TSMS_GPIO_HIGH);
}

TSMS_INLINE static void TSMS_SPI_CS_LOW(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->cs, TSMS_GPIO_LOW);
}

TSMS_INLINE static void TSMS_SPI_DOUT_HIGH(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->dout, TSMS_GPIO_HIGH);
}

TSMS_INLINE static void TSMS_SPI_DOUT_LOW(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->dout, TSMS_GPIO_LOW);
}

TSMS_INLINE static TSMS_GPIO_STATUS TSMS_SPI_DIN(TSMS_SHP spi) {
	return TSMS_GPIO_read(spi->din);
}

TSMS_INLINE static void TSMS_SPI_receiveCustomBit(TSMS_SHP spi,uint8_t bits,uint32_t * data) {
    *data = 0;
	if (!TSMS_SPI_MODE_CPHA(spi->mode)) {
		for (uint8_t i = 0; i < bits; i++) {
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			if (!TSMS_SPI_MODE_CPHA(spi->mode))
				*data |= TSMS_SPI_DIN(spi) << (spi->type == TSMS_TRANSFER_MSB ? bits - i - 1 : i);
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
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_GPIO_write(spi->sclk, TSMS_SPI_MODE_CPOL(spi->mode));
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			if (TSMS_SPI_MODE_CPHA(spi->mode))
				*data |= TSMS_SPI_DIN(spi) << (spi->type == TSMS_TRANSFER_MSB ? bits - i - 1 : i);
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		}
	}

}

TSMS_INLINE static void TSMS_SPI_transmitCustomBit(TSMS_SHP spi, uint8_t bits, uint32_t data) {
	if (!TSMS_SPI_MODE_CPHA(spi->mode)) {
		for (uint8_t i = 0;i<bits;i++) {
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			if (!TSMS_SPI_MODE_CPHA(spi->mode))
				TSMS_GPIO_write(spi->dout, (data >> (spi->type == TSMS_TRANSFER_MSB ? bits - i - 1 : i)) & 0x01);
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_GPIO_write(spi->sclk, !TSMS_SPI_MODE_CPOL(spi->mode));
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_GPIO_write(spi->sclk, TSMS_SPI_MODE_CPOL(spi->mode));
		}
	} else {
		for (uint8_t i = 0;i<bits;i++) {
			TSMS_GPIO_write(spi->sclk, !TSMS_SPI_MODE_CPOL(spi->mode));
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			TSMS_GPIO_write(spi->sclk, TSMS_SPI_MODE_CPOL(spi->mode));
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
			if (TSMS_SPI_MODE_CPHA(spi->mode))
				TSMS_GPIO_write(spi->dout, (data >> (spi->type == TSMS_TRANSFER_MSB ? bits - i - 1 : i)) & 0x01);
			TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		}
	}

}

TSMS_INLINE static void __tsms_internal_spi_release0(TSMS_SHP spi) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
	HAL_SPI_DeInit(spi->hardwareHandler);
#endif
	free(spi);
}

TSMS_INLINE static void __tsms_internal_spi_release1(TSMS_SHP spi) {
	TSMS_GPIO_release(spi->cs);
	TSMS_GPIO_release(spi->sclk);
	TSMS_GPIO_release(spi->dout);
	TSMS_GPIO_release(spi->din);
	__tsms_internal_spi_release0(spi);
}

#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)

#if defined(TSMS_STM32_SPI_USE_HAL_GPIO)

TSMS_SHP TSMS_SPI_createSoftwareHandler(GPIO_TypeDef * csPort, uint16_t csPin,
                                        GPIO_TypeDef * sclkPort, uint16_t sclkPin,
                                        GPIO_TypeDef * dinPort, uint16_t dinPin,
                                        GPIO_TypeDef * doutPort, uint16_t doutPin,
										TSMS_SPI_MODE mode, TSMS_GPIO_STATUS csValid, TSMS_TRANSFER_TYPE type) {
	TSMS_SHP spi = malloc(sizeof (struct TSMS_SPI_HANDLER));
	if (spi == TSMS_NULL)
		return TSMS_NULL;
	spi->cs = TSMS_GPIO_createHandler(csPort, csPin);
	spi->sclk = TSMS_GPIO_createHandler(sclkPort, sclkPin);
	spi->dout = TSMS_GPIO_createHandler(dinPort, dinPin);
	spi->din = TSMS_GPIO_createHandler(doutPort, doutPin);
	spi->mode = mode;
	spi->csValid = csValid;
	spi->delay = __tsms_internal_spi_delay;
	spi->type = type;
	spi->isHardware = false;
	spi->release = __tsms_internal_spi_release1;
	if (spi->mode == TSMS_SPI_MODE_0 || spi->mode == TSMS_SPI_MODE_1)
		TSMS_SPI_SCLK_LOW(spi);
	else TSMS_SPI_SCLK_HIGH(spi);
	return spi;
}
#else

TSMS_SHP TSMS_SPI_createSoftwareHandler(TSMS_GHP cs, TSMS_GHP sclk, TSMS_GHP dout, TSMS_GHP din, TSMS_SPI_MODE mode, TSMS_GPIO_STATUS csValid, TSMS_TRANSFER_TYPE type) {
	TSMS_SHP spi = malloc(sizeof (struct TSMS_SPI_HANDLER));
	if (spi == TSMS_NULL)
		return TSMS_NULL;
	spi->cs = cs;
	spi->sclk = sclk;
	spi->dout = dout;
	spi->din = din;
	spi->mode = mode;
	spi->csValid = csValid;
	spi->delay = __tsms_internal_spi_delay;
	spi->type = type;
	spi->isHardware = false;
	spi->release = __tsms_internal_spi_release0;
	if (spi->mode == TSMS_SPI_MODE_0 || spi->mode == TSMS_SPI_MODE_1)
		TSMS_SPI_SCLK_LOW(spi);
	else TSMS_SPI_SCLK_HIGH(spi);
	return spi;
}

#endif

TSMS_SHP TSMS_SPI_createHardwareHandler(SPI_HandleTypeDef * spi) {
	TSMS_SHP handler = malloc(sizeof (struct TSMS_SPI_HANDLER));
	handler->hardwareHandler = spi;
	handler->isHardware = true;
	handler->release = __tsms_internal_spi_release0;
	return handler;
}

#else

TSMS_SHP TSMS_SPI_createSoftwareHandler(TSMS_GHP cs, TSMS_GHP sclk, TSMS_GHP din, TSMS_GHP dout, TSMS_SPI_MODE mode, TSMS_GPIO_STATUS csValid, TSMS_TRANSFER_TYPE type) {
	TSMS_SHP spi = malloc(sizeof (struct TSMS_SPI_HANDLER));
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
		TSMS_SPI_SCLK_LOW(spi);
	else TSMS_SPI_SCLK_HIGH(spi);
	return spi;
}


#endif


TSMS_RESULT TSMS_SPI_transmitCustomBits(TSMS_SHP spi, uint32_t *data, uint8_t bits, uint32_t length) {
	if (spi->isHardware) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)

		// todo ????????
		return TSMS_ERROR;
#else
		return TSMS_TIMEOUT;
#endif
	} else {
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_HIGH(spi);
		else TSMS_SPI_CS_LOW(spi);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		for (uint32_t i = 0;i < length;i++)
			TSMS_SPI_transmitCustomBit(spi, bits, data[i]);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_LOW(spi);
		else TSMS_SPI_CS_HIGH(spi);
		return TSMS_SUCCESS;
	}
}

TSMS_RESULT TSMS_SPI_receiveCustomBits(TSMS_SHP spi, uint32_t *data, uint8_t bits, uint32_t length) {
    if (spi->isHardware) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
	    // todo ????????
		return TSMS_ERROR;
#else
        return TSMS_TIMEOUT;
#endif
    } else {
        if (spi->csValid == TSMS_GPIO_HIGH)
            TSMS_SPI_CS_HIGH(spi);
        else TSMS_SPI_CS_LOW(spi);
	    TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
        for (uint32_t i = 0;i < length;i++)
            TSMS_SPI_receiveCustomBit(spi,bits,data + i);
	    TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
        if (spi->csValid == TSMS_GPIO_HIGH)
            TSMS_SPI_CS_LOW(spi);
        else TSMS_SPI_CS_HIGH(spi);
        return TSMS_SUCCESS;
    }
}

TSMS_RESULT TSMS_SPI_transform(TSMS_SHP spi, uint32_t *data,uint8_t writeBits,uint8_t  readBits, uint32_t writeLength, uint32_t readLength) {
	if (spi->isHardware) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
		// todo ????????
		return TSMS_ERROR;
#else
		return TSMS_TIMEOUT;
#endif
	} else {
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_HIGH(spi);
		else TSMS_SPI_CS_LOW(spi);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		for (uint32_t i = 0;i < writeLength;i++)
			TSMS_SPI_transmitCustomBit(spi, writeBits,data[i]);
		for (uint32_t i = writeLength;i < writeLength + readLength;i++)
			TSMS_SPI_receiveCustomBit(spi, readBits, data + i);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_LOW(spi);
		else TSMS_SPI_CS_HIGH(spi);
		return TSMS_SUCCESS;
	}
}

TSMS_RESULT TSMS_SPI_sequenceTransform(TSMS_SHP spi, uint32_t n, ...) {
	if (spi->isHardware) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
		// todo ????????
		return TSMS_ERROR;
#else
		return TSMS_TIMEOUT;
#endif
	} else {
		va_list args;
		va_start(args, n);
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_HIGH(spi);
		else TSMS_SPI_CS_LOW(spi);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		for (uint32_t i = 0;i < n;i++) {
			struct TSMS_SPI_OPERATION operation = va_arg(args, struct TSMS_SPI_OPERATION);
			if (operation.type == TSMS_SPI_READ)
				TSMS_SPI_receiveCustomBits(spi, operation.data, operation.bits, operation.length);
			else if (operation.type == TSMS_SPI_WRITE)
				TSMS_SPI_transmitCustomBits(spi, operation.data, operation.bits, operation.length);
		}
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		TSMS_SPI_delay(spi, TSMS_NO_DELAY_TIME);
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_LOW(spi);
		else TSMS_SPI_CS_HIGH(spi);
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

void TSMS_SPI_delay(TSMS_SHP spi, TSMS_DELAY_TIME time) {
	if (spi->customDelay != TSMS_NULL)
		spi->customDelay(time);
	else spi->delay();
}