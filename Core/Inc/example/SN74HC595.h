#ifndef TSMS_SN74HC595_H
#define TSMS_SN74HC595_H

#include "gpio.h"
#include "tsms_driver.h"
#include "tsms_gpio.h"

#define SN74HC595_Q0 0
#define SN74HC595_Q1 1
#define SN74HC595_Q2 2
#define SN74HC595_Q3 3
#define SN74HC595_Q4 4
#define SN74HC595_Q5 5
#define SN74HC595_Q6 6
#define SN74HC595_Q7 7

#define SN74HC595_SQ0 0
#define SN74HC595_SQ1 1
#define SN74HC595_SQ2 2
#define SN74HC595_SQ3 3
#define SN74HC595_SQ4 4
#define SN74HC595_SQ5 5
#define SN74HC595_SQ6 6
#define SN74HC595_SQ7 7

#define SN74HC595_REG_STORAGE 0
#define SN74HC595_REG_SHIFT 1

#define SN74HC595_SRCLR 0
#define SN74HC595_SRCLK 1
#define SN74HC595_RCLK 2
#define SN74HC595_DATA 3

struct SN74HC595_Handler {
	TSMS_DHP handler;
};

struct SN74HC595_Handler * SN74HC595_initHardware(GPIO_TypeDef* srclr, uint16_t srclrPin,
                            GPIO_TypeDef* srclk, uint16_t srclkPin,
                            GPIO_TypeDef* rclk, uint16_t rclkPin,
                            GPIO_TypeDef* data, uint16_t dataPin);

void SN74HC595_clearStorageRegister(struct SN74HC595_Handler* handler);
void SN74HC595_writeByteToShiftRegister(struct SN74HC595_Handler* handler, uint8_t data);
void SN74HC595_latchToStorageRegister(struct SN74HC595_Handler* handler);
void SN74HC595_writeByte(struct SN74HC595_Handler* handler, uint8_t data);

#endif //TSMS_SN74HC595_H
