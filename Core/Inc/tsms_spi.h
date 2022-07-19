//
// Created by 周蜀杰 on 2022/7/19.
//

#ifndef TSMS_SPI_H
#define TSMS_SPI_H

#endif //TSMS_SPI_H

//for stm32 series
#ifdef STM32H743xx
#define TSMS_STM32_SPI // for no external hardware please use TSMS_STM32_NO_SPI
#endif


struct TSMS_SPI_HANDLER {

#ifdef TSMS_DRIVER

#endif

#ifdef TSMS_STM32_SPI

#endif

};

struct TSMS_SPI_HANDLER TSMS_SPI_createSoftwareHandler(xxx);

struct TSMS_SPI_HANDLER TSMS_SPI_createHardwareHandler(xxx);
