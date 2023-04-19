#ifndef TSMS_H
#define TSMS_H

#include "memory.h"
#include "stdarg.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "tsms_def.h"
#include "tsms_delay.h"
#include "tsms_err.h"

#ifdef TSMS_OPTIMIZATION
#define TSMS_INLINE inline static
#else
#define TSMS_INLINE static
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

#if defined(USE_HAL_DRIVER) && defined(STM32F103xB)
#define TSMS_STM32
#define TSMS_STM32_GPIO
#endif

#if defined(USE_HAL_DRIVER) && defined(STM32F411xE)
#define TSMS_STM32
#define TSMS_STM32_GPIO
#endif

#if defined(USE_HAL_DRIVER) && defined(STM32G070xx)
#define TSMS_STM32
#define TSMS_STM32_GPIO
#endif

#if defined(USE_HAL_DRIVER) && defined(STM32F722xx)
#define TSMS_STM32
#define TSMS_STM32_GPIO
#endif


#ifdef TSMS_STM32

#include "main.h" // for stm32 hal driver, this can directly access the whole stm32 hardware interface
#ifdef HAL_SPI_MODULE_ENABLED
#define TSMS_STM32_SPI
#endif
#ifdef HAL_I2C_MODULE_ENABLED
#define TSMS_STM32_IIC
#endif

#ifdef HAL_SDRAM_MODULE_ENABLED
#define TSMS_STM32_SDRAM
#endif

#endif

TSMS_RESULT TSMS_IT_init(TSMS_CLOCK_FREQUENCY frequency);

TSMS_RESULT TSMS_SPI_init(TSMS_CLOCK_FREQUENCY frequency);

TSMS_RESULT TSMS_IIC_init(TSMS_CLOCK_FREQUENCY frequency);

TSMS_RESULT TSMS_STRING_init();

TSMS_RESULT TSMS_LIST_init();

TSMS_RESULT TSMS_init(TSMS_CLOCK_FREQUENCY frequency);

void TSMS_delay(uint32_t ms);

TSMS_BITS TSMS_bits(uint8_t bits);

#endif //TSMS_H