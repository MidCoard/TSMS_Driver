#include "tsms_spi.h"

//for stm32 serial
#ifdef STM32H743xx
#define TSMS_STM32_SPI // for no external hardware please use TSMS_STM32_NO_SPI
#endif