#ifndef TSMS_H
#define TSMS_H

#include "malloc.h"
#include "stdbool.h"
#include "memory.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "tsms_def.h"

#ifdef TSMS_OPTIMIZATION
#define TSMS_INLINE inline
#else
#define TSMS_INLINE
#endif

#ifndef TSMS_DRIVER
// for internal use
#define TSMS_DRIVER
#endif

//for stm32 series
#if defined(USE_HAL_DRIVER) && defined(STM32H743xx)
#define TSMS_STM32
#define TSMS_STM32_GPIO
#endif

#if defined(USE_HAL_DRIVER) && defined(STM32G474xx)
#define TSMS_STM32
#define TSMS_STM32_GPIO
#endif

#if defined(USE_HAL_DRIVER) && defined(STM32L431xx)
#define TSMS_STM32
#define TSMS_STM32_GPIO
#endif

#ifdef TSMS_STM32

#include "main.h" // for stm32 hal driver, this can directly access the whole stm32 hardware interface
#ifdef HAL_SPI_MODULE_ENABLED
#define TSMS_STM32_SPI // for no external hardware please use TSMS_STM32_NO_SPI
#endif
#ifdef HAL_I2C_MODULE_ENABLED
#define TSMS_STM32_IIC // for no external hardware please use TSMS_STM32_NO_IIC
#endif

#endif

#define TSMS_NULL 0

typedef enum {
	TSMS_SUCCESS, TSMS_ERROR, TSMS_TIMEOUT, TSMS_FAIL
} TSMS_RESULT;

typedef void(*TSMS_DELAY_FUNCTION)();

typedef uint32_t TSMS_DELAY_TIME;

typedef void(*TSMS_DELAY_FUNCTION)();
typedef void(*TSMS_CUSTOM_DELAY_FUNCTION)(TSMS_DELAY_TIME);

#define TSMS_MASK(x) ((1<<x)-1)

TSMS_RESULT TSMS_IT_init(TSMS_CLOCK_FREQUENCY frequency);
TSMS_RESULT TSMS_SPI_init(TSMS_CLOCK_FREQUENCY frequency);
TSMS_RESULT TSMS_IIC_init(TSMS_CLOCK_FREQUENCY frequency);

TSMS_RESULT TSMS_init(TSMS_CLOCK_FREQUENCY frequency);

#define TSMS_NO_DELAY_TIME 0

void TSMS_NO_DELAY(TSMS_DELAY_TIME time);


#endif //TSMS_H