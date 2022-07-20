#ifndef TSMS_SPI_H
#define TSMS_SPI_H

#include "tsms_gpio.h"

typedef enum {
	TSMS_SPI_MODE_0,TSMS_SPI_MODE_1,TSMS_SPI_MODE_2,TSMS_SPI_MODE_3
} TSMS_SPI_MODE;

typedef enum {
	TSMS_SPI_MSB = 0U, TSMS_SPI_LSB
} TSMS_SPI_TRANSFER_TYPE;

struct TSMS_SPI_HANDLER {

	TSMS_GHP cs;
	TSMS_GHP sclk;
	TSMS_GHP din;
	TSMS_GHP dout;
	bool isHardware;
	TSMS_SPI_MODE mode;
	TSMS_GPIO_STATUS csValid;
	TSMS_DELAY_FUNCTION delay;
	TSMS_SPI_TRANSFER_TYPE type;

#ifdef TSMS_STM32_SPI
	SPI_HandleTypeDef * hardwareHandler;
#endif
};

typedef struct TSMS_SPI_HANDLER * TSMS_SPI_HANDLER_POINTER;
typedef TSMS_SPI_HANDLER_POINTER TSMS_SHP;

TSMS_SHP TSMS_SPI_createSoftwareHandler(TSMS_GHP cs, TSMS_GHP sclk, TSMS_GHP din, TSMS_GHP dout, TSMS_SPI_MODE mode, TSMS_GPIO_STATUS csValid, TSMS_SPI_TRANSFER_TYPE type);

#ifdef TSMS_STM32_SPI
TSMS_SHP TSMS_SPI_createSoftwareHanlder(GPIO_TypeDef * csPort, uint16_t csPin,
                                        GPIO_TypeDef * sclkPort, uint16_t sclkPin,
                                        GPIO_TypeDef * dinPort, uint16_t dinPin,
                                        GPIO_TypeDef * doutPort, uint16_t doutPin,
										TSMS_SPI_MODE mode, TSMS_GPIO_STATUS csValid, TSMS_SPI_TRANSFER_TYPE type);
TSMS_SHP TSMS_SPI_createHardwareHandler(SPI_HandleTypeDef * spi);
#endif

TSMS_RESULT TSMS_SPI_transmitBytes(TSMS_SHP spi, uint8_t *data, uint32_t length);

TSMS_RESULT TSMS_SPI_transmitHalfWords(TSMS_SHP spi, uint16_t *data, uint32_t length);

TSMS_RESULT TSMS_SPI_transmitWords(TSMS_SHP spi, uint32_t *data, uint32_t length);

TSMS_RESULT TSMS_SPI_transmit24Bits(TSMS_SHP spi, uint32_t *data, uint32_t length);

TSMS_RESULT TSMS_SPI_transmitCustomBits(TSMS_SHP spi, uint32_t *data, uint8_t bits, uint32_t length);

TSMS_RESULT TSMS_SPI_receiveBytes(TSMS_SHP spi, uint8_t *data, uint32_t length);

TSMS_RESULT TSMS_SPI_receiveHalfWords(TSMS_SHP spi, uint16_t *data, uint32_t length);

TSMS_RESULT TSMS_SPI_receiveWords(TSMS_SHP spi, uint32_t *data, uint32_t length);

TSMS_RESULT TSMS_SPI_receive24Bits(TSMS_SHP spi, uint32_t *data, uint32_t length);

TSMS_RESULT TSMS_SPI_receiveCustomBits(TSMS_SHP spi, uint32_t *data, uint8_t bits, uint32_t length);

TSMS_RESULT TSMS_SPI_release(TSMS_SHP spi);

#endif //TSMS_SPI_H