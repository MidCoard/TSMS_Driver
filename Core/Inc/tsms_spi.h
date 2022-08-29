#ifndef TSMS_SPI_H
#define TSMS_SPI_H

#include "tsms_gpio.h"
#include "tsms_def.h"

typedef enum {
	TSMS_SPI_MODE_0 = 0,TSMS_SPI_MODE_1,TSMS_SPI_MODE_2,TSMS_SPI_MODE_3
} TSMS_SPI_MODE;

struct TSMS_SPI_HANDLER;
typedef struct TSMS_SPI_HANDLER * TSMS_SPI_HANDLER_POINTER;
typedef TSMS_SPI_HANDLER_POINTER TSMS_SHP;

typedef void(*TSMS_SPI_RELEASE_FUNCTION)(TSMS_SHP);

struct TSMS_SPI_HANDLER {

	TSMS_GHP cs;
	TSMS_GHP sclk;
	TSMS_GHP din;
	TSMS_GHP dout;
	bool isHardware;
	TSMS_SPI_MODE mode;
	TSMS_GPIO_STATUS csValid;
	TSMS_DELAY_FUNCTION delay;
	TSMS_TRANSFER_TYPE type;
	TSMS_SPI_RELEASE_FUNCTION release;

#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
	SPI_HandleTypeDef * hardwareHandler;
#endif
};
typedef enum {TSMS_SPI_CPOL_LOW = 0,TSMS_SPI_CPOL_HIGH} TSMS_SPI_CPOL;
typedef enum {TSMS_SPI_CPHA_LOW = 0,TSMS_SPI_CPHA_HIGH} TSMS_SPI_CPHA;


#define TSMS_SPI_MODE_CPOL(x) (TSMS_SPI_CPOL)((x>>1)&1)
#define TSMS_SPI_MODE_CPHA(x) (TSMS_SPI_CPHA)(x&1)

#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)

#if defined(TSMS_STM32_SPI_USE_HAL_GPIO)
TSMS_SHP TSMS_SPI_createSoftwareHandlerInternal(GPIO_TypeDef * csPort, uint16_t csPin,
                                        GPIO_TypeDef * sclkPort, uint16_t sclkPin,
                                        GPIO_TypeDef * dinPort, uint16_t dinPin,
                                        GPIO_TypeDef * doutPort, uint16_t doutPin,
										TSMS_SPI_MODE mode, TSMS_GPIO_STATUS csValid, TSMS_TRANSFER_TYPE type);
#else
TSMS_SHP TSMS_SPI_createSoftwareHandler(TSMS_GHP cs, TSMS_GHP sclk, TSMS_GHP din, TSMS_GHP dout, TSMS_SPI_MODE mode, TSMS_GPIO_STATUS csValid, TSMS_TRANSFER_TYPE type);
#endif

TSMS_SHP TSMS_SPI_createHardwareHandler(SPI_HandleTypeDef * spi);
#else

TSMS_SHP TSMS_SPI_createSoftwareHandler(TSMS_GHP cs, TSMS_GHP sclk, TSMS_GHP din, TSMS_GHP dout, TSMS_SPI_MODE mode, TSMS_GPIO_STATUS csValid, TSMS_TRANSFER_TYPE type);

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

TSMS_SPI_MODE TSMS_SPI_mode(TSMS_SPI_CPOL cpol, TSMS_SPI_CPHA cpha);

#endif //TSMS_SPI_H