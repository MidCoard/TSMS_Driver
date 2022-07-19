//
// Created by 周蜀杰 on 2022/7/19.
//

#ifndef TSMS_H
#define TSMS_H

#endif //TSMS_H
#include "malloc.h"
#include "stdbool.h"

#ifdef TSMS_optimization
#endif

#ifndef TSMS_DRIVER
// for internal use
#define TSMS_DRIVER
#endif

//for stm32 series
#ifdef STM32H743xx
#ifdef USE_HAL_DRIVER
#define TSMS_STM32
#define TSMS_STM32_SPI // for no external hardware please use TSMS_STM32_NO_SPI
#define TSMS_STM32_GPIO
#endif
#endif

#ifdef TSMS_STM32
#include "main.h" // for stm32 hal driver, this can directly access the whole stm32 hardware interface
#endif

#define TSMS_NULL 0

typedef enum {
	TSMS_SUCCESS, TSMS_ERROR, TSMS_TIMEOUT
} TSMS_RESULT;

