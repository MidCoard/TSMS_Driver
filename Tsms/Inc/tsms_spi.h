#ifndef TSMS_SPI_H
#define TSMS_SPI_H

#define TSMS_SPI_MODE_CPOL(x) (TSMS_GPIO_STATUS)((x>>1)&1)
#define TSMS_SPI_MODE_CPHA(x) (TSMS_GPIO_STATUS)(x&1)

typedef enum {
	TSMS_SPI_CPOL_LOW = 0, TSMS_SPI_CPOL_HIGH
} TSMS_SPI_CPOL;
typedef enum {
	TSMS_SPI_CPHA_LOW = 0, TSMS_SPI_CPHA_HIGH
} TSMS_SPI_CPHA;

typedef enum {
	TSMS_SPI_OPERATION_TYPE_READ = 0, TSMS_SPI_OPERATION_TYPE_WRITE
} TSMS_SPI_OPERATION_TYPE;

typedef enum {
	TSMS_SPI_MODE_0 = 0, TSMS_SPI_MODE_1, TSMS_SPI_MODE_2, TSMS_SPI_MODE_3
} TSMS_SPI_MODE;

typedef struct TSMS_SPI_HANDLER *TSMS_SPI_HANDLER_POINTER;
typedef TSMS_SPI_HANDLER_POINTER TSMS_SHP;

typedef void(*TSMS_SPI_RELEASE_FUNCTION)(TSMS_SHP);

#include "tsms_gpio.h"

struct TSMS_SPI_HANDLER {

	TSMS_GHP cs;
	TSMS_GHP sclk;
	TSMS_GHP dout;
	TSMS_GHP din;
	bool isHardware;
	TSMS_SPI_MODE mode;
	TSMS_GPIO_STATUS csValid;
	TSMS_DELAY_FUNCTION delay;
	TSMS_CUSTOM_DELAY_FUNCTION customDelay;    // todo not implemented
	TSMS_TRANSFER_TYPE type;
	TSMS_SPI_RELEASE_FUNCTION release;

#ifdef TSMS_STM32_SPI
	SPI_HandleTypeDef * hardwareHandler;
#endif
};

struct TSMS_SPI_OPERATION {
	TSMS_SPI_OPERATION_TYPE type;
	uint8_t bits;
	uint32_t *data;
	uint32_t length;
};

#ifdef TSMS_STM32_SPI

TSMS_SHP TSMS_SPI_createSoftwareHandler(TSMS_GHP cs, TSMS_GHP sclk, TSMS_GHP dout, TSMS_GHP din, TSMS_SPI_MODE mode, TSMS_GPIO_STATUS csValid, TSMS_TRANSFER_TYPE type);

#endif

TSMS_SHP TSMS_SPI_createSoftwareHandler(TSMS_GHP cs, TSMS_GHP sclk, TSMS_GHP din, TSMS_GHP dout, TSMS_SPI_MODE mode,
                                        TSMS_GPIO_STATUS csValid, TSMS_TRANSFER_TYPE type);

TSMS_RESULT TSMS_SPI_transmitCustomBits(TSMS_SHP spi, uint32_t *data, uint8_t bits, uint32_t length);

TSMS_RESULT TSMS_SPI_receiveCustomBits(TSMS_SHP spi, uint32_t *data, uint8_t bits, uint32_t length);

TSMS_RESULT TSMS_SPI_transform(TSMS_SHP spi, uint32_t *data, uint8_t writeBits, uint8_t readBits, uint32_t writeLength,
                               uint32_t readLength);

TSMS_RESULT TSMS_SPI_sequenceTransform(TSMS_SHP spi, uint32_t n, ...);

TSMS_RESULT TSMS_SPI_release(TSMS_SHP spi);

TSMS_SPI_MODE TSMS_SPI_mode(TSMS_SPI_CPOL cpol, TSMS_SPI_CPHA cpha);

TSMS_RESULT TSMS_SPI_delay(TSMS_SHP spi, TSMS_DELAY_TIME time);

#endif //TSMS_SPI_H