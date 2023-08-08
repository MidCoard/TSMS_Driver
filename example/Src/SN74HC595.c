#include "SN74HC595.h"
#include "tsms_custom.h"
#include "tsms_gpio.h"
#include "tsms_driver.h"

void SN74HC595_clearStorageRegister(struct SN74HC595_Handler *handler) {
	TSMS_GPIO_write(TSMS_CUSTOM_getGPIO(handler->handler->custom, SN74HC595_SRCLR), TSMS_GPIO_STATUS_LOW);
	TSMS_CUSTOM_delay(handler->handler->custom, TSMS_NO_DELAY_TIME);
	TSMS_GPIO_write(TSMS_CUSTOM_getGPIO(handler->handler->custom, SN74HC595_SRCLR), TSMS_GPIO_STATUS_HIGH);
	TSMS_REG_setRegisterByList(handler->handler->regs, SN74HC595_REG_STORAGE, 0);
}

void SN74HC595_writeByteToShiftRegister(struct SN74HC595_Handler *handler, uint8_t data) {
	for (int i = 0; i < 8; i++) {
		TSMS_GPIO_write(TSMS_CUSTOM_getGPIO(handler->handler->custom, SN74HC595_DATA),
		                (data & 0x80) ? TSMS_GPIO_STATUS_HIGH : TSMS_GPIO_STATUS_LOW);
		TSMS_CUSTOM_delay(handler->handler->custom, TSMS_NO_DELAY_TIME);
		TSMS_GPIO_write(TSMS_CUSTOM_getGPIO(handler->handler->custom, SN74HC595_SRCLK), TSMS_GPIO_STATUS_HIGH);
		TSMS_CUSTOM_delay(handler->handler->custom, TSMS_NO_DELAY_TIME);
		TSMS_GPIO_write(TSMS_CUSTOM_getGPIO(handler->handler->custom, SN74HC595_SRCLK), TSMS_GPIO_STATUS_LOW);
		data <<= 1;
	}
	TSMS_REG_setRegisterByList(handler->handler->regs, SN74HC595_REG_SHIFT, data);
}

void SN74HC595_latchToStorageRegister(struct SN74HC595_Handler *handler) {
	TSMS_GPIO_write(TSMS_CUSTOM_getGPIO(handler->handler->custom, SN74HC595_RCLK), TSMS_GPIO_STATUS_HIGH);
	TSMS_CUSTOM_delay(handler->handler->custom, TSMS_NO_DELAY_TIME);
	TSMS_GPIO_write(TSMS_CUSTOM_getGPIO(handler->handler->custom, SN74HC595_RCLK), TSMS_GPIO_STATUS_LOW);
	TSMS_REG_setRegisterByList(handler->handler->regs, SN74HC595_REG_STORAGE,
	                           TSMS_REG_getRegisterByList(handler->handler->regs, SN74HC595_REG_SHIFT));
}

void SN74HC595_writeByte(struct SN74HC595_Handler *handler, uint8_t data) {
	SN74HC595_writeByteToShiftRegister(handler, data);
	SN74HC595_latchToStorageRegister(handler);
}

static void __internal_delay(TSMS_DELAY_TIME time) {
	volatile uint32_t i = 10;
	while (i--) {
	}
}

struct SN74HC595_Handler *SN74HC595_initHardware(TSMS_GHP srclr,
                                                 TSMS_GHP srclk,
                                                 TSMS_GHP rclk,
                                                 TSMS_GHP data) {
	struct SN74HC595_Handler *handler = TSMS_malloc(sizeof(struct SN74HC595_Handler));
	handler->handler = TSMS_DRIVER_createCustomHandler(
			TSMS_CUSTOM_createSpecialHandler(__internal_delay, TSMS_NULL_GHP, TSMS_NULL_GHP, 4,
			                                 srclr, srclk, rclk, data),
			TSMS_REG_createList(2, TSMS_REG_8BitRegister(SN74HC595_REG_STORAGE, SN74HC595_Q0, SN74HC595_Q1, SN74HC595_Q2, SN74HC595_Q3,
			                                             SN74HC595_Q4, SN74HC595_Q5, SN74HC595_Q6, SN74HC595_Q7),
			                    TSMS_REG_8BitRegister(SN74HC595_REG_SHIFT, SN74HC595_SQ0, SN74HC595_SQ1, SN74HC595_SQ2, SN74HC595_SQ3,
			                                          SN74HC595_SQ4, SN74HC595_SQ5, SN74HC595_SQ6, SN74HC595_SQ7)));
	TSMS_GPIO_write(TSMS_CUSTOM_getGPIO(handler->handler->custom, SN74HC595_SRCLR), TSMS_GPIO_STATUS_HIGH);
	TSMS_CUSTOM_delay(handler->handler->custom, TSMS_NO_DELAY_TIME);
	SN74HC595_clearStorageRegister(handler);
	TSMS_GPIO_write(TSMS_CUSTOM_getGPIO(handler->handler->custom, SN74HC595_SRCLK), TSMS_GPIO_STATUS_LOW);
	TSMS_GPIO_write(TSMS_CUSTOM_getGPIO(handler->handler->custom, SN74HC595_RCLK), TSMS_GPIO_STATUS_LOW);
	TSMS_GPIO_write(TSMS_CUSTOM_getGPIO(handler->handler->custom, SN74HC595_DATA), TSMS_GPIO_STATUS_LOW);
	return handler;
}