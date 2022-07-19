#include "tsms_spi.h"

TSMS_SHP TSMS_SPI_createSoftwareHandler(TSMS_GHP cs, TSMS_GHP sclk, TSMS_GHP din, TSMS_GHP dout) {
	TSMS_SHP spi = malloc(sizeof (struct TSMS_SPI_HANDLER));
	spi->cs = cs;
	spi->sclk = sclk;
	spi->din = din;
	spi->dout = dout;
	spi->isHardware = false;
	return spi;
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