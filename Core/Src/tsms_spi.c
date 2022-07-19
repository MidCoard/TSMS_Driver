#include "tsms_spi.h"

TSMS_SHP TSMS_SPI_createSoftwareHandler(TSMS_GHP cs, TSMS_GHP sclk, TSMS_GHP din, TSMS_GHP dout, TSMS_SPI_MODE mode) {
	TSMS_SHP spi = malloc(sizeof (struct TSMS_SPI_HANDLER));
	spi->cs = cs;
	spi->sclk = sclk;
	spi->din = din;
	spi->dout = dout;
	spi->mode = mode;
	spi->isHardware = false;
	return spi;
}


inline static void TSMS_SPI_() {
// modify the spi value




}

#ifdef TSMS_STM32_SPI

TSMS_SHP TSMS_SPI_createSoftwareHanlder(GPIO_TypeDef * csPort, uint16_t csPin,
                                        GPIO_TypeDef * sclkPort, uint16_t sclkPin,
                                        GPIO_TypeDef * dinPort, uint16_t dinPin,
                                        GPIO_TypeDef * doutPort, uint16_t doutPin) {
	return TSMS_SPI_createSoftwareHandler(TSMS_GPIO_createHandler(csPort, csPin),
	                                      TSMS_GPIO_createHandler(sclkPort, sclkPin),
	                                      TSMS_GPIO_createHandler(dinPort, dinPin),
	                                      TSMS_GPIO_createHandler(doutPort, doutPin));
}

TSMS_SHP TSMS_SPI_createHardwareHandler(SPI_TypeDef * spi) {
	TSMS_SHP handler = malloc(sizeof (struct TSMS_SPI_HANDLER));
	handler->hardwareHandler = spi;
	handler->isHardware = true;
	return handler;
}

#endif

TSMS_RESULT TSMS_SPI_transmitBytes(TSMS_SHP spi, TSMS_SPI_DATA data, TSMS_SPI_DATA_LENGTH length) {
	if (spi->isHardware) {
#ifdef TSMS_STM32_SPI
		HAL_SPI_T
#else
		return TSMS_TIMEOUT;
#endif
	} else {
		for (TSMS_SPI_DATA_LENGTH i = 0;i < length;i++)
			TSMS_SPI_transmitByte(data[i]);
	}
}