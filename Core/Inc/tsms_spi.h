//
// Created by 周蜀杰 on 2022/7/19.
//

#ifndef TSMS_SPI_H
#define TSMS_SPI_H

#endif //TSMS_SPI_H

#include "tsms_gpio.h"

struct TSMS_SPI_HANDLER {

	TSMS_GHP cs;
	TSMS_GHP sclk;
	TSMS_GHP din;
	TSMS_GHP dout;
	bool isHardware;

#ifdef TSMS_STM32_SPI
	SPI_TypeDef * hardwareHandler;
#endif

};

typedef struct TSMS_SPI_HANDLER * TSMS_SPI_HANDLER_POINTER;
typedef TSMS_SPI_HANDLER_POINTER TSMS_SHP;

TSMS_SHP TSMS_SPI_createSoftwareHandler(TSMS_GHP cs, TSMS_GHP sclk, TSMS_GHP din, TSMS_GHP dout);

#ifdef TSMS_STM32_SPI
TSMS_SHP TSMS_SPI_createSoftwareHanlder(GPIO_TypeDef * csPort, uint16_t csPin,
										GPIO_TypeDef * sclkPort, uint16_t sclkPin,
										GPIO_TypeDef * dinPort, uint16_t dinPin,
										GPIO_TypeDef * doutPort, uint16_t doutPin);
TSMS_SHP TSMS_SPI_createHardwareHandler(SPI_TypeDef * spi);
#endif
