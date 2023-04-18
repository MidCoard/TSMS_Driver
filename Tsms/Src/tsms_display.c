#include "tsms_display.h"

TSMS_INLINE TSMS_RESULT __tsms_internal_reset_gt9147(TSMS_DPHP display) {
	TSMS_GT9147 gt9147 = display->custom;
	TSMS_GPIO_write(gt9147->reset, TSMS_GPIO_LOW);
	TSMS_Delay(1);
	TSMS_GPIO_write(gt9147->reset, TSMS_GPIO_HIGH);
	return TSMS_SUCCESS;
}

TSMS_INLINE TSMS_RESULT __tsms_internal_readRegister_gt9147(TSMS_DHP driver, uint32_t reg, uint32_t * data, TSMS_BITS bits) {
	TSMS_GT9147 gt9147 = driver->display->custom;
	return TSMS_IIC_readCustomRegister(gt9147->iic, reg, TSMS_bits(driver->regs->regs[reg]->bits), data, bits);
}

TSMS_DHP TSMS_DISPLAY_initGT9147(TSMS_IHP iic, TSMS_GHP reset, TSMS_GHP interrupt) {
	TSMS_DHP driver = malloc(sizeof (struct TSMS_DRIVER_HANDLER));
	if (driver == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_DHP");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	driver->type = TSMS_DRIVER_DISPLAY;
	driver->display = malloc(sizeof (struct TSMS_DISPLAY_HANDLER));
	if (driver->display == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_DPHP");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		free(driver);
		return TSMS_NULL;
	}
	TSMS_GT9147 gt9147 = malloc(sizeof (struct TSMS_GT9147_HANDLER));
	if (gt9147 == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_GT9147");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		free(driver->display);
		free(driver);
		return TSMS_NULL;
	}
	gt9147->iic = iic;
	gt9147->reset = reset;
	gt9147->interrupt = interrupt;
	driver->display->custom = gt9147;
	driver->display->reset = __tsms_internal_reset_gt9147;
	driver->display->readRegister = __tsms_internal_readRegister_gt9147;
	TSMS_DISPLAY_reset(driver);
	uint32_t id;
	TSMS_DISPLAY_readRegister(driver, 0x8140, &id, TSMS_32BIT);
	return driver;
}

TSMS_RESULT TSMS_DISPLAY_reset(TSMS_DHP display) {
	if (display == TSMS_NULL || display->type != TSMS_DRIVER_DISPLAY)
		return TSMS_ERROR;
	return display->display->reset(display->display);
}

TSMS_RESULT TSMS_DISPLAY_readRegister(TSMS_DHP display, uint8_t reg, uint32_t * data, TSMS_BITS bits) {
	if (display == TSMS_NULL || display->type != TSMS_DRIVER_DISPLAY)
		return TSMS_ERROR;
	return display->display->readRegister(display->display, reg, data, bits);
}