#include "tsms_display.h"

TSMS_INLINE void __internal_tsms_swap(uint16_t *a, uint16_t *b) {
	uint16_t t = *a;
	*a = *b;
	*b = t;
}

void __internal_tsms_lcd_write_register(TSMS_SCHP screen, uint16_t reg, uint16_t value) {
	*screen->command = reg;
	*screen->data = value;
}

void __internal_tsms_lcd_write_command(TSMS_SCHP screen, volatile uint16_t command) {
	*screen->command = command;
}

volatile uint16_t __internal_tsms_lcd_read_data(TSMS_SCHP screen) {
	return *screen->data;
}

void __internal_tsms_lcd_write_data(TSMS_SCHP screen, volatile uint16_t data) {
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
		}
	}
	if (screen->type == TSMS_SCREEN_AUTO_DETECT || screen->type == TSMS_SCREEN_ST7789) {
		id = TSMS_ST7789_readId(screen);
		if (id == TSMS_ST7789_ID) {
			screen->type = TSMS_SCREEN_ST7789;
			screen->init = TSMS_ST7789_init;
		}
	}
	if (screen->type == TSMS_SCREEN_AUTO_DETECT || screen->type == TSMS_SCREEN_NT35310) {
		id = TSMS_NT35310_readId(screen);
		if (id == TSMS_NT35310_ID) {
			screen->type = TSMS_SCREEN_NT35310;
			screen->init = TSMS_NT35310_init;
		}
	}
	if (screen->type == TSMS_SCREEN_AUTO_DETECT || screen->type == TSMS_SCREEN_NT35510) {
		id = TSMS_NT5510_readId(screen);
		if (id == TSMS_NT35510_ID) {
			screen->type = TSMS_SCREEN_NT35510;
			screen->init = TSMS_NT5510_init;
		}
	}
	if (screen->type == TSMS_SCREEN_AUTO_DETECT || screen->type == TSMS_SCREEN_NT5510) {
		id = TSMS_NT5510_readId(screen);
		if (id == TSMS_NT5510_ID) {
			screen->type = TSMS_SCREEN_NT5510;
			screen->init = TSMS_NT5510_init;
		}
	}
	if (screen->type == TSMS_SCREEN_AUTO_DETECT || screen->type == TSMS_SCREEN_SSD1963) {
		id = TSMS_SSD1963_readId(screen);
		if (id == TSMS_SSD1963_ID) {
			screen->type = TSMS_SCREEN_SSD1963;
			screen->init = TSMS_SSD1963_init;
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
	if (direction == TSMS_DISPLAY_VERTICAL) {
		screen->width = screen->defaultWidth;
		screen->height = screen->defaultHeight;
		if (screen->type == TSMS_SCREEN_ILI9341 || screen->type == TSMS_SCREEN_NT35310 ||
		    screen->type == TSMS_SCREEN_ST7789) {
			screen->writeCommand = 0x2C;
			screen->setXCommand = 0x2A;
			screen->setYCommand = 0x2B;
		} else if (screen->type == TSMS_SCREEN_NT5510 || screen->type == TSMS_SCREEN_NT35510) {
			screen->writeCommand = 0x2C00;
			screen->setXCommand = 0x2A00;
			screen->setYCommand = 0x2B00;
		} else if (screen->type == TSMS_SCREEN_SSD1963) {
			screen->writeCommand = 0X2C;
			screen->setXCommand = 0X2B;
			screen->setYCommand = 0X2A;
		}
	} else {
		screen->width = screen->defaultHeight;
		screen->height = screen->defaultWidth;
		if (screen->type == TSMS_SCREEN_ILI9341 || screen->type == TSMS_SCREEN_NT35310 ||
		    screen->type == TSMS_SCREEN_ST7789) {
			screen->writeCommand = 0x2C;
			screen->setXCommand = 0x2A;
			screen->setYCommand = 0x2B;
		} else if (screen->type == TSMS_SCREEN_NT5510 || screen->type == TSMS_SCREEN_NT35510) {
			screen->writeCommand = 0X2C00;
			screen->setXCommand = 0X2A00;
			screen->setYCommand = 0X2B00;
		} else if (screen->type == TSMS_SCREEN_SSD1963) {
			screen->writeCommand = 0X2C;
			screen->setXCommand = 0X2A;
			screen->setYCommand = 0X2B;
		}
	}
	screen->direction = direction;
	return TSMS_SCREEN_setScanDirection(screen, TSMS_SCAN_L2R_U2D);
}

TSMS_RESULT TSMS_SCREEN_setScanDirection(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_SCAN_DIRECTION dir = 0;
	if ((screen->direction == TSMS_DISPLAY_HORIZONTAL && screen->type != TSMS_SCREEN_SSD1963) ||
	    (screen->direction == TSMS_DISPLAY_VERTICAL && screen->type == TSMS_SCREEN_SSD1963)) {
		switch (direction) {
			case TSMS_SCAN_L2R_U2D:
				dir = 6;
				break;
			case TSMS_SCAN_L2R_D2U:
				dir = 7;
				break;
			case TSMS_SCAN_R2L_U2D:
				dir = 4;
				break;
			case TSMS_SCAN_R2L_D2U:
				dir = 5;
				break;
			case TSMS_SCAN_U2D_L2R:
				dir = 1;
				break;
			case TSMS_SCAN_U2D_R2L:
				dir = 0;
				break;
			case TSMS_SCAN_D2U_L2R:
				dir = 3;
				break;
			case TSMS_SCAN_D2U_R2L:
				dir = 2;
				break;
		}
	}
	uint16_t reg;
	uint16_t value = 0;
	switch (dir) {
		case TSMS_SCAN_L2R_U2D:
			value |= (0 << 7) | (0 << 6) | (0 << 5);
			break;
		case TSMS_SCAN_L2R_D2U:
			value |= (1 << 7) | (0 << 6) | (0 << 5);
			break;
		case TSMS_SCAN_R2L_U2D:
			value |= (0 << 7) | (1 << 6) | (0 << 5);
			break;
		case TSMS_SCAN_R2L_D2U:
			value |= (1 << 7) | (1 << 6) | (0 << 5);
			break;
		case TSMS_SCAN_U2D_L2R:
			value |= (0 << 7) | (0 << 6) | (1 << 5);
			break;
		case TSMS_SCAN_U2D_R2L:
			value |= (0 << 7) | (1 << 6) | (1 << 5);
			break;
		case TSMS_SCAN_D2U_L2R:
			value |= (1 << 7) | (0 << 6) | (1 << 5);
			break;
		case TSMS_SCAN_D2U_R2L:
			value |= (1 << 7) | (1 << 6) | (1 << 5);
			break;
	}
	if (screen->type == TSMS_SCREEN_NT5510 || screen->type == TSMS_SCREEN_NT35510)
		reg = 0X3600;
	else reg = 0X36;
	if (screen->type != TSMS_SCREEN_NT35310 && screen->type != TSMS_SCREEN_NT35510 &&
	    screen->type != TSMS_SCREEN_NT5510 && screen->type != TSMS_SCREEN_SSD1963)
		value |= 0X08;
	__internal_tsms_lcd_write_register(screen, reg, value);
	if (screen->type != TSMS_SCREEN_SSD1963)
		if (value & 0X20) {
			if (screen->width < screen->height)
				__internal_tsms_swap(&screen->width, &screen->height);
		} else if (screen->width > screen->height)
			__internal_tsms_swap(&screen->width, &screen->height);

	if (screen->type == TSMS_SCREEN_NT5510 || screen->type == TSMS_SCREEN_NT35510) {
		__internal_tsms_lcd_write_command(screen, screen->setXCommand);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_command(screen, screen->setXCommand + 1);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_command(screen, screen->setXCommand + 2);
		__internal_tsms_lcd_write_data(screen, (screen->width - 1) >> 8);
		__internal_tsms_lcd_write_command(screen, screen->setXCommand + 3);
		__internal_tsms_lcd_write_data(screen, (screen->width - 1) & 0XFF);
		__internal_tsms_lcd_write_command(screen, screen->setYCommand);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_command(screen, screen->setYCommand + 1);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_command(screen, screen->setYCommand + 2);
		__internal_tsms_lcd_write_data(screen, (screen->height - 1) >> 8);
		__internal_tsms_lcd_write_command(screen, screen->setYCommand + 3);
		__internal_tsms_lcd_write_data(screen, (screen->height - 1) & 0XFF);
	} else {
		__internal_tsms_lcd_write_command(screen, screen->setXCommand);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_data(screen, (screen->width - 1) >> 8);
		__internal_tsms_lcd_write_data(screen, (screen->width - 1) & 0XFF);
		__internal_tsms_lcd_write_command(screen, screen->setYCommand);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_data(screen, (screen->height - 1) >> 8);
		__internal_tsms_lcd_write_data(screen, (screen->height - 1) & 0XFF);
	}
	screen->scan = direction;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_SCREEN_setCursor(TSMS_SCHP screen, uint16_t x, uint16_t y) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	if (screen->type == TSMS_SCREEN_ILI9341 || screen->type == TSMS_SCREEN_ST7789 ||
	    screen->type == TSMS_SCREEN_NT35310) {
		__internal_tsms_lcd_write_command(screen, screen->setXCommand);
		__internal_tsms_lcd_write_data(screen, x >> 8);
		__internal_tsms_lcd_write_data(screen, x & 0XFF);
		__internal_tsms_lcd_write_command(screen, screen->setYCommand);
		__internal_tsms_lcd_write_data(screen, y >> 8);
		__internal_tsms_lcd_write_data(screen, y & 0XFF);
	} else if (screen->type == TSMS_SCREEN_SSD1963) {
		if (screen->direction == TSMS_DISPLAY_VERTICAL) {
			x = screen->width - x - 1;
			__internal_tsms_lcd_write_command(screen, screen->setXCommand);
			__internal_tsms_lcd_write_data(screen, 0);
			__internal_tsms_lcd_write_data(screen, 0);
			__internal_tsms_lcd_write_data(screen, x >> 8);
			__internal_tsms_lcd_write_data(screen, x & 0XFF);
		} else {
			__internal_tsms_lcd_write_command(screen, screen->setXCommand);
			__internal_tsms_lcd_write_data(screen, x >> 8);
			__internal_tsms_lcd_write_data(screen, x & 0XFF);
			__internal_tsms_lcd_write_data(screen, (screen->width - 1) >> 8);
			__internal_tsms_lcd_write_data(screen, (screen->width - 1) & 0XFF);
		}
		__internal_tsms_lcd_write_command(screen, screen->setYCommand);
		__internal_tsms_lcd_write_data(screen, y >> 8);
		__internal_tsms_lcd_write_data(screen, y & 0XFF);
		__internal_tsms_lcd_write_data(screen, (screen->height - 1) >> 8);
		__internal_tsms_lcd_write_data(screen, (screen->height - 1) & 0XFF);
	} else if (screen->type == TSMS_SCREEN_NT5510 || screen->type == TSMS_NT35510_ID) {
		__internal_tsms_lcd_write_command(screen, screen->setXCommand);
		__internal_tsms_lcd_write_data(screen, x >> 8);
		__internal_tsms_lcd_write_command(screen, screen->setXCommand + 1);
		__internal_tsms_lcd_write_data(screen, x & 0XFF);
		__internal_tsms_lcd_write_command(screen, screen->setYCommand);
		__internal_tsms_lcd_write_data(screen, y >> 8);
		__internal_tsms_lcd_write_command(screen, screen->setYCommand + 1);
		__internal_tsms_lcd_write_data(screen, y & 0XFF);
	}
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_SCREEN_swap(TSMS_SCHP screen) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_SCREEN_setCursor(screen, 0, 0);
	__internal_tsms_lcd_write_command(screen, screen->writeCommand);
	for (uint32_t i = 0; i < screen->width * screen->height; i++)
		__internal_tsms_lcd_write_data(screen, screen->swapBuffer[i]); // one row by one row
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_SCREEN_drawPoint(TSMS_SCHP screen, uint16_t x, uint16_t y, TSMS_CP color) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	if (x >= screen->width || y >= screen->height)
		return TSMS_ERROR;
	if (screen->swapBuffer == TSMS_NULL) {
		TSMS_SCREEN_setCursor(screen, x, y);
		__internal_tsms_lcd_write_command(screen, screen->writeCommand);
		__internal_tsms_lcd_write_data(screen, TSMS_UTIL_color565(color));
	} else
		screen->swapBuffer[y * screen->width + x] = TSMS_UTIL_color565(color);
	return TSMS_SUCCESS;
}

