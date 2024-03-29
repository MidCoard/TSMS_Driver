#ifndef TSMS_H
#define TSMS_H

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
#ifdef HAL_TIM_MODULE_ENABLED
#define TSMS_STM32_TIMER
#endif

#ifdef HAL_UART_MODULE_ENABLED
#define TSMS_STM32_UART
#endif
#include "malloc.h"

#endif

#include "tsms_def.h"

TSMS_RESULT TSMS_IT_init(TSMS_CLOCK_FREQUENCY frequency);

TSMS_RESULT TSMS_SPI_init(TSMS_CLOCK_FREQUENCY frequency);

TSMS_RESULT TSMS_IIC_init(TSMS_CLOCK_FREQUENCY frequency);

TSMS_RESULT TSMS_STRING_init();

TSMS_RESULT TSMS_LIST_init();

TSMS_RESULT TSMS_FONT_init();

TSMS_RESULT TSMS_init(TSMS_CLOCK_FREQUENCY frequency, TSMS_CLOCK_FREQUENCY timerFrequency);

TSMS_RESULT TSMS_TIMER_init(TSMS_CLOCK_FREQUENCY frequency);

void * TSMS_malloc(size_t size);

void * TSMS_realloc(void * ptr, size_t size);

void * TSMS_clone(void * ptr, size_t size);

#ifdef TSMS_GUI_STYLE

TSMS_RESULT TSMS_GUI_STYLE_init();
TSMS_RESULT TSMS_NATIVE_MUTABLE_init();

#endif

#endif //TSMS_H