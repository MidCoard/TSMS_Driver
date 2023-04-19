#include "tsms_display.h"

void TSMS_SCREEN_writeRegister(TSMS_SCHP screen, uint16_t reg, uint16_t value) {
	*screen->command = reg;
	*screen->data = value;
}

void TSMS_SCREEN_writeCommand(TSMS_SCHP screen, volatile uint16_t command) {
	*screen->command = command;
}

volatile uint16_t TSMS_SCREEN_readData(TSMS_SCHP screen) {
	return *screen->data;
}

void TSMS_SCREEN_writeData(TSMS_SCHP screen, volatile uint16_t data) {
	*screen->data = data;
}

TSMS_INLINE TSMS_RESULT __tsms_internal_reset_gt9147(TSMS_THP touch) {
	TSMS_GT9147 gt9147 = touch->custom;
	TSMS_GPIO_write(gt9147->reset, TSMS_GPIO_LOW);
	TSMS_delay(1);
	TSMS_GPIO_write(gt9147->reset, TSMS_GPIO_HIGH);
	return TSMS_SUCCESS;
}

TSMS_SCHP
TSMS_SCREEN_create16BitHandler(uint16_t *command, uint16_t *data, TSMS_GHP bg, TSMS_SCREEN_TYPE type, uint16_t width,
                               uint16_t height, uint16_t *swapBuffer, TSMS_SSD1963_OP option) {
	TSMS_SCHP screen = malloc(sizeof(struct TSMS_SCREEN_HANDLER));
	if (screen == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_SCHP");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	screen->reset = TSMS_NULL;
	screen->command = command;
	screen->data = data;
	screen->bg = bg;
	screen->type = type;
	screen->defaultWidth = width;
	screen->defaultHeight = height;
	screen->swapBuffer = swapBuffer;
	TSMS_delay(50);
	uint16_t id;
	if (screen->type == TSMS_SCREEN_AUTO_DETECT || screen->type == TSMS_SCREEN_ILI9341) {
		id = TSMS_ILI9341_readId(screen);
		if (id == TSMS_ILI9341_ID) {
			screen->type = TSMS_SCREEN_ILI9341;
			screen->init = TSMS_ILI9341_init;
			screen->setDisplayDirection = TSMS_ILI9341_setDisplayDirection;
			screen->setScanDirection = TSMS_ILI9341_setScanDirection;
			screen->setCursor = TSMS_ILI9341_setCursor;
		}
	}
	if (screen->type == TSMS_SCREEN_AUTO_DETECT || screen->type == TSMS_SCREEN_ST7789) {
		id = TSMS_ST7789_readId(screen);
		if (id == TSMS_ST7789_ID) {
			screen->type = TSMS_SCREEN_ST7789;
			screen->init = TSMS_ST7789_init;
			screen->setDisplayDirection = TSMS_ST7789_setDisplayDirection;
			screen->setScanDirection = TSMS_ST7789_setScanDirection;
			screen->setCursor = TSMS_ST7789_setCursor;
		}
	}
	if (screen->type == TSMS_SCREEN_AUTO_DETECT || screen->type == TSMS_SCREEN_NT35310) {
		id = TSMS_NT35310_readId(screen);
		if (id == TSMS_NT35310_ID) {
			screen->type = TSMS_SCREEN_NT35310;
			screen->init = TSMS_NT35310_init;
			screen->setDisplayDirection = TSMS_NT35310_setDisplayDirection;
			screen->setScanDirection = TSMS_NT35310_setScanDirection;
			screen->setCursor = TSMS_NT35310_setCursor;
		}
	}
	if (screen->type == TSMS_SCREEN_AUTO_DETECT || screen->type == TSMS_SCREEN_NT35510) {
		id = TSMS_NT5510_readId(screen);
		if (id == TSMS_NT35510_ID) {
			screen->type = TSMS_SCREEN_NT35510;
			screen->init = TSMS_NT5510_init;
			screen->setDisplayDirection = TSMS_NT5510_setDisplayDirection;
			screen->setScanDirection = TSMS_NT5510_setScanDirection;
			screen->setCursor = TSMS_NT5510_setCursor;
		}
	}
	if (screen->type == TSMS_SCREEN_AUTO_DETECT || screen->type == TSMS_SCREEN_NT5510) {
		id = TSMS_NT5510_readId(screen);
		if (id == TSMS_NT5510_ID) {
			screen->type = TSMS_SCREEN_NT5510;
			screen->init = TSMS_NT5510_init;
			screen->setDisplayDirection = TSMS_NT5510_setDisplayDirection;
			screen->setScanDirection = TSMS_NT5510_setScanDirection;
			screen->setCursor = TSMS_NT5510_setCursor;
		}
	}
	if (screen->type == TSMS_SCREEN_AUTO_DETECT || screen->type == TSMS_SCREEN_SSD1963) {
		id = TSMS_SSD1963_readId(screen);
		if (id == TSMS_SSD1963_ID) {
			screen->type = TSMS_SCREEN_SSD1963;
			screen->init = TSMS_SSD1963_init;
			screen->setDisplayDirection = TSMS_SSD1963_setDisplayDirection;
			screen->setScanDirection = TSMS_SSD1963_setScanDirection;
			screen->setCursor = TSMS_SSD1963_setCursor;
		}
	}
	if (screen->type != TSMS_SCREEN_AUTO_DETECT) {
		screen->init(screen, option);
		TSMS_SCREEN_setDisplayDirection(screen, TSMS_DISPLAY_VERTICAL);
		TSMS_SCREEN_enableBackgroudLight(screen);
		return screen;
	}
	tString temp = TSMS_STRING_temp("screen type not supported");
	TSMS_ERR_report(TSMS_ERR_SCREEN_TYPE_NOT_SUPPORTED, &temp);
	free(screen);
	return TSMS_NULL;
}

TSMS_DPHP TSMS_DISPLAY_createHandler(TSMS_SCHP screen, TSMS_THP touch) {
	// touch could be null
	if (screen == TSMS_NULL)
		return TSMS_NULL;
	TSMS_DPHP display = malloc(sizeof(struct TSMS_DISPLAY_HANDLER));
	if (display == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_DPHP");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	display->touch = touch;
	display->screen = screen;
	return display;
}

TSMS_THP TSMS_TOUCH_createGT9147Handler(TSMS_IHP iic, TSMS_GHP reset, TSMS_GHP interrupt) {
	TSMS_THP touch = malloc(sizeof(struct TSMS_TOUCH_HANDLER));
	if (touch == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_THP");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	touch->reset = __tsms_internal_reset_gt9147;
	TSMS_GT9147 gt9147 = malloc(sizeof(struct TSMS_GT9147_HANDLER));
	if (gt9147 == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_GT9147");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		free(touch);
		return TSMS_NULL;
	}
	gt9147->iic = iic;
	gt9147->reset = reset;
	gt9147->interrupt = interrupt;
	return touch;
}

TSMS_RESULT TSMS_DISPLAY_reset(TSMS_DPHP display) {
	if (display == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = TSMS_SUCCESS;
	if (display->screen->reset != TSMS_NULL)
		result |= display->screen->reset(display->screen);
	if (display->touch != TSMS_NULL && display->touch->reset != TSMS_NULL)
		return result | display->touch->reset(display->touch);
	return result;
}

TSMS_RESULT TSMS_SCREEN_enableBackgroudLight(TSMS_SCHP screen) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_GPIO_write(screen->bg, TSMS_GPIO_HIGH);
}

TSMS_RESULT TSMS_SCREEN_disableBackgroudLight(TSMS_SCHP screen) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_GPIO_write(screen->bg, TSMS_GPIO_LOW);
}

TSMS_RESULT TSMS_SCREEN_setDisplayDirection(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT  result = screen->setDisplayDirection(screen, direction);
	if (result != TSMS_SUCCESS)
		return result;
	return TSMS_SCREEN_setScanDirection(screen, TSMS_SCAN_L2R_U2D);
}

TSMS_RESULT TSMS_SCREEN_setScanDirection(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	return screen->setScanDirection(screen, direction);
}

TSMS_RESULT TSMS_SCREEN_setCursor(TSMS_SCHP screen, uint16_t x, uint16_t y) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	return screen->setCursor(screen, x, y);
}

TSMS_RESULT TSMS_SCREEN_swap(TSMS_SCHP screen) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_SCREEN_setCursor(screen, 0, 0);
	TSMS_SCREEN_writeCommand(screen, screen->writeCommand);
	for (uint32_t i = 0; i < screen->width * screen->height; i++)
		TSMS_SCREEN_writeData(screen, screen->swapBuffer[i]); // one row by one row
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_SCREEN_drawPoint(TSMS_SCHP screen, uint16_t x, uint16_t y, TSMS_CP color) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	if (x >= screen->width || y >= screen->height)
		return TSMS_ERROR;
	if (screen->swapBuffer == TSMS_NULL) {
		TSMS_SCREEN_setCursor(screen, x, y);
		TSMS_SCREEN_writeCommand(screen, screen->writeCommand);
		TSMS_SCREEN_writeData(screen, TSMS_UTIL_color565(color));
	} else
		screen->swapBuffer[y * screen->width + x] = TSMS_UTIL_color565(color);
	return TSMS_SUCCESS;
}

