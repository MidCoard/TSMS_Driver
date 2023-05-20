#include "tsms_display.h"
#include "touch/tsms_gt9147.h"
#include "screen/tsms_ili9341.h"
#include "screen/tsms_st7789.h"
#include "screen/tsms_nt35310.h"
#include "screen/tsms_nt5510.h"
#include "screen/tsms_ssd1963.h"

TSMS_INLINE TSMS_RESULT __tsms_internal_touch_reset(TSMS_THP touch) {
	TSMS_GPIO_write(touch->resetPin, TSMS_GPIO_STATUS_LOW);
	TSMS_delay(10);
	TSMS_GPIO_write(touch->resetPin, TSMS_GPIO_STATUS_HIGH);
	TSMS_delay(10);
	return TSMS_SUCCESS;
}

TSMS_INLINE void __tsms_internal_screen_auto_request(void *screen, pTimer pTimer) {
	TSMS_SCREEN_swap(screen);
}

TSMS_INLINE void __tsms_internal_screen_auto_request_both(void *d, pTimer pTimer) {
	TSMS_DPHP display = d;
	if (display->restCount >= 1) {
		if (TSMS_LOCK_tryLock(display->lock)) {
			if (display->restCount >= 1) {
				if (TSMS_SCREEN_swap(display->screen) == TSMS_SUCCESS) {
					display->count++;
					display->restCount--;
				}
			}
			TSMS_LOCK_unlock(display->lock);
		}
	}
	TSMS_SEQUENCE_PRIORITY_LOCK_postLock(display->screen->lock, 1);
}


TSMS_INLINE TSMS_RESULT __tsms_internal_screen_draw_point(TSMS_SCHP screen, uint16_t x, uint16_t y, TSMS_COLOR color) {
	if (x >= screen->width || y >= screen->height)
		return TSMS_ERROR;
	if (screen->swapBuffer[0] == TSMS_NULL) {
		TSMS_SCREEN_setCursor(screen, x, y);
		TSMS_SCREEN_writeCommand(screen, screen->writeCommand);
		TSMS_SCREEN_writeData(screen, TSMS_UTIL_color565(color));
	} else {
		uint16_t color565 = TSMS_UTIL_color565(color);
		screen->swapBuffer[screen->swapIndex][y * screen->width + x] = color565;
		bool flag = color565 != screen->swapBuffer[screen->swapIndex ^ 1][y * screen->width + x];
		uint8_t *p = screen->extraBuffer + y * screen->xWidth + x / 8;
		if (flag)
			*p |= 1 << (x % 8);
		else *p &= ~(1 << (x % 8));
	}
	return TSMS_SUCCESS;
}

TSMS_INLINE TSMS_RESULT
__tsms_internal_screen_draw_line(TSMS_SCHP screen, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, TSMS_COLOR color) {
	if (x0 >= screen->width || y0 >= screen->height || x1 >= screen->width || y1 >= screen->height)
		return TSMS_ERROR;
	int16_t dx = x1 - x0;
	int16_t dy = y1 - y0;
	int16_t sx = (dx > 0) - (dx < 0);
	int16_t sy = (dy > 0) - (dy < 0);
	dx = abs(dx);
	dy = abs(dy);
	int16_t err = (dx > dy ? dx : -dy) / 2;
	int16_t e2;
	while (1) {
		if (__tsms_internal_screen_draw_point(screen, x0, y0, color) != TSMS_SUCCESS)
			return TSMS_FAIL;
		if (x0 == x1 && y0 == y1)
			break;
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
	return TSMS_SUCCESS;
}

TSMS_INLINE TSMS_RESULT
__tsms_internal_screen_fill_rect(TSMS_SCHP screen, uint16_t x, uint16_t y, uint16_t w, uint16_t h, TSMS_COLOR color) {
	if (x >= screen->width || y >= screen->height || x + w > screen->width || y + h > screen->height)
		return TSMS_ERROR;
	for (uint16_t i = x; i < x + w; i++)
		for (uint16_t j = y; j < y + h; j++)
			if (__tsms_internal_screen_draw_point(screen, i, j, color) != TSMS_SUCCESS)
				return TSMS_FAIL;
	return TSMS_SUCCESS;
}

TSMS_INLINE TSMS_RESULT
__tsms_internal_screen_draw_block(TSMS_SCHP screen, uint16_t x, uint16_t y, uint16_t offsetX, uint16_t offsetY,
                                  uint16_t blockSize, TSMS_COLOR color) {
	return __tsms_internal_screen_fill_rect(screen, x + blockSize * offsetX, y + blockSize * offsetY, blockSize,
	                                        blockSize, color);
}

TSMS_INLINE TSMS_RESULT
__tsms_internal_screen_draw_char(TSMS_SCHP screen, uint16_t x, uint16_t y, TSMS_FONT_TYPE fontType, void *font,
                                 uint8_t c, TSMS_COLOR color, TSMS_FONT_SIZE size) {
	if (x >= screen->width || y >= screen->height)
		return TSMS_ERROR;
	TSMS_FONT_DATA data = TSMS_FONT_resolve(fontType, font, c);
	if (data.type == TSMS_FONT_TYPE_INVALID)
		return TSMS_ERROR;
	for (uint16_t i = 0; i < data.width; i++)
		for (uint16_t j = 0; j < data.height; j++)
			if (TSMS_FONT_pixel(data, i, j))
				if (__tsms_internal_screen_draw_block(screen, x, y, i, j, size, color) != TSMS_SUCCESS)
					return TSMS_FAIL;
	return TSMS_SUCCESS;
}

TSMS_INLINE TSMS_RESULT
__tsms_internal_screen_draw_string(TSMS_SCHP screen, uint16_t x, uint16_t y, TSMS_FONT_TYPE fontType, void *font,
                                   pString str, TSMS_COLOR color, TSMS_FONT_SIZE size) {
	if (x >= screen->width || y >= screen->height)
		return TSMS_ERROR;
	uint16_t x0 = x;
	uint16_t y0 = y;
	for (TSMS_POS i = 0; i < str->length; i++)
		if (__tsms_internal_screen_draw_char(screen, x0, y0, fontType, font, str->cStr[i], color, size) != TSMS_SUCCESS)
			return TSMS_FAIL;
		else {
			TSMS_FONT_DATA data = TSMS_FONT_resolve(fontType, font, str->cStr[i]);
			x0 += data.width * size;
		}
	return TSMS_SUCCESS;
}

void TSMS_SCREEN_writeRegister(TSMS_SCHP screen, volatile uint16_t reg, volatile uint16_t value) {
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

TSMS_SCHP
TSMS_SCREEN_create16BitHandler(uint16_t *command, uint16_t *data, TSMS_GHP bg, TSMS_SCREEN_TYPE type, uint16_t width,
                               uint16_t height, const uint16_t *swapBuffer, uint8_t * extraBuffer, uint16_t *swapBuffer2, void *option) {
	TSMS_SCHP screen = malloc(sizeof(struct TSMS_SCREEN_HANDLER));
	if (screen == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_SCHP");
		TSMS_ERR_report(TSMS_ERROR_TYPE_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	screen->reset = TSMS_NULL;
	screen->command = command;
	screen->data = data;
	screen->bg = bg;
	screen->type = type;
	screen->defaultWidth = width;
	screen->defaultHeight = height;
	screen->swapBuffer[0] = swapBuffer;
	screen->swapBuffer[1] = swapBuffer2;
	screen->swapIndex = 0;
	screen->extraBuffer = extraBuffer;
	screen->lock = TSMS_SEQUENCE_PRIORITY_LOCK_create();
	TSMS_delay(50);
	uint16_t id;
	if (screen->type == TSMS_SCREEN_TYPE_AUTO_DETECT || screen->type == TSMS_SCREEN_TYPE_ILI9341) {
		id = TSMS_ILI9341_readId(screen);
		if (id == TSMS_ILI9341_ID) {
			screen->type = TSMS_SCREEN_TYPE_ILI9341;
			screen->init = TSMS_ILI9341_init;
			screen->setDisplayDirection = TSMS_ILI9341_setDisplayDirection;
			screen->setScanDirection = TSMS_ILI9341_setScanDirection;
			screen->setCursor = TSMS_ILI9341_setCursor;
		}
	}
	if (screen->type == TSMS_SCREEN_TYPE_AUTO_DETECT || screen->type == TSMS_SCREEN_TYPE_ST7789) {
		id = TSMS_ST7789_readId(screen);
		if (id == TSMS_ST7789_ID) {
			screen->type = TSMS_SCREEN_TYPE_ST7789;
			screen->init = TSMS_ST7789_init;
			screen->setDisplayDirection = TSMS_ST7789_setDisplayDirection;
			screen->setScanDirection = TSMS_ST7789_setScanDirection;
			screen->setCursor = TSMS_ST7789_setCursor;
		}
	}
	if (screen->type == TSMS_SCREEN_TYPE_AUTO_DETECT || screen->type == TSMS_SCREEN_TYPE_NT35310) {
		id = TSMS_NT35310_readId(screen);
		if (id == TSMS_NT35310_ID) {
			screen->type = TSMS_SCREEN_TYPE_NT35310;
			screen->init = TSMS_NT35310_init;
			screen->setDisplayDirection = TSMS_NT35310_setDisplayDirection;
			screen->setScanDirection = TSMS_NT35310_setScanDirection;
			screen->setCursor = TSMS_NT35310_setCursor;
		}
	}
	if (screen->type == TSMS_SCREEN_TYPE_AUTO_DETECT || screen->type == TSMS_SCREEN_TYPE_NT35510) {
		id = TSMS_NT5510_readId(screen);
		if (id == TSMS_NT35510_ID) {
			screen->type = TSMS_SCREEN_TYPE_NT35510;
			screen->init = TSMS_NT5510_init;
			screen->setDisplayDirection = TSMS_NT5510_setDisplayDirection;
			screen->setScanDirection = TSMS_NT5510_setScanDirection;
			screen->setCursor = TSMS_NT5510_setCursor;
		}
	}
	if (screen->type == TSMS_SCREEN_TYPE_AUTO_DETECT || screen->type == TSMS_SCREEN_TYPE_NT5510) {
		id = TSMS_NT5510_readId(screen);
		if (id == TSMS_NT5510_ID) {
			screen->type = TSMS_SCREEN_TYPE_NT5510;
			screen->init = TSMS_NT5510_init;
			screen->setDisplayDirection = TSMS_NT5510_setDisplayDirection;
			screen->setScanDirection = TSMS_NT5510_setScanDirection;
			screen->setCursor = TSMS_NT5510_setCursor;
		}
	}
	if (screen->type == TSMS_SCREEN_TYPE_AUTO_DETECT || screen->type == TSMS_SCREEN_TYPE_SSD1963) {
		id = TSMS_SSD1963_readId(screen);
		if (id == TSMS_SSD1963_ID) {
			screen->type = TSMS_SCREEN_TYPE_SSD1963;
			screen->init = TSMS_SSD1963_init;
			screen->setDisplayDirection = TSMS_SSD1963_setDisplayDirection;
			screen->setScanDirection = TSMS_SSD1963_setScanDirection;
			screen->setCursor = TSMS_SSD1963_setCursor;
		}
	}
	if (screen->type != TSMS_SCREEN_TYPE_AUTO_DETECT) {
		screen->init(screen, option);
		TSMS_SCREEN_setDisplayDirection(screen, TSMS_DISPLAY_DIRECTION_VERTICAL);
		TSMS_SCREEN_enableBackgroudLight(screen);
		return screen;
	}
	tString temp = TSMS_STRING_temp("screen type not supported");
	TSMS_ERR_report(TSMS_ERROR_TYPE_SCREEN_TYPE_NOT_SUPPORTED, &temp);
	TSMS_SEQUENCE_PRIORITY_LOCK_release(screen->lock);
	free(screen);
	return TSMS_NULL;
}

TSMS_DPHP TSMS_DISPLAY_createHandler(TSMS_SCHP screen, TSMS_THP touch, float refreshRate) {
	// touch could be null
	if (screen == TSMS_NULL)
		return TSMS_NULL;
	TSMS_DPHP display = malloc(sizeof(struct TSMS_DISPLAY_HANDLER));
	if (display == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_DPHP");
		TSMS_ERR_report(TSMS_ERROR_TYPE_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	display->touch = touch;
	if (display->touch != TSMS_NULL)
		display->touch->display = display;
	display->screen = screen;
	display->refreshRate = refreshRate;
	display->lock = TSMS_LOCK_create();
	return display;
}

TSMS_THP TSMS_TOUCH_createHandler(void *handler, TSMS_TOUCH_TYPE type, TSMS_GHP rst, void *option) {
	TSMS_THP touch = malloc(sizeof(struct TSMS_TOUCH_HANDLER));
	if (touch == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_THP");
		TSMS_ERR_report(TSMS_ERROR_TYPE_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	touch->resetPin = rst;
	touch->reset = __tsms_internal_touch_reset;
	touch->type = type;
	touch->custom = handler;
	touch->lock = TSMS_SEQUENCE_PRIORITY_LOCK_create();
	touch->list = TSMS_LIST_create(10);
	touch->callback = TSMS_NULL;
	TSMS_TOUCH_reset(touch);
	volatile uint32_t id;
	if (touch->type == TSMS_TOUCH_TYPE_AUTO_DETECT || touch->type == TSMS_TOUCH_TYPE_GT9147) {
		id = TSMS_GT9147_readId(touch);
		id;
		if (id == TSMS_GT9147_ID) {
			touch->type = TSMS_TOUCH_TYPE_GT9147;
			touch->init = TSMS_GT9147_init;
			touch->request = TSMS_GT9147_request;
			touch->setRequestMode = TSMS_GT9147_setRequestMode;
		}
	}
	if (touch->type != TSMS_TOUCH_TYPE_AUTO_DETECT) {
		touch->init(touch, option);
		return touch;
	}
	tString temp = TSMS_STRING_temp("touch type not supported");
	TSMS_ERR_report(TSMS_ERROR_TYPE_TOUCH_TYPE_NOT_SUPPORTED, &temp);
	TSMS_SEQUENCE_PRIORITY_LOCK_release(touch->lock);
	TSMS_LIST_release(touch->list);
	free(touch);
	return TSMS_NULL;
}

TSMS_RESULT TSMS_TOUCH_reset(TSMS_THP touch) {
	if (touch == TSMS_NULL)
		return TSMS_ERROR;
	if (touch->reset != TSMS_NULL)
		return touch->reset(touch);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_SCREEN_reset(TSMS_SCHP screen) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	if (screen->reset != TSMS_NULL)
		return screen->reset(screen);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_DISPLAY_reset(TSMS_DPHP display) {
	if (display == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = TSMS_SUCCESS;
	result |= TSMS_SCREEN_reset(display->screen);
	result |= TSMS_TOUCH_reset(display->touch);
	return result;
}

TSMS_RESULT TSMS_SCREEN_enableBackgroudLight(TSMS_SCHP screen) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_GPIO_write(screen->bg, TSMS_GPIO_STATUS_HIGH);
}

TSMS_RESULT TSMS_SCREEN_disableBackgroudLight(TSMS_SCHP screen) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_GPIO_write(screen->bg, TSMS_GPIO_STATUS_LOW);
}

TSMS_RESULT TSMS_SCREEN_setDisplayDirection(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = screen->setDisplayDirection(screen, direction);
	if (result != TSMS_SUCCESS)
		return result;
	return TSMS_SCREEN_setScanDirection(screen, TSMS_SCAN_DIRECTION_L2R_U2D);
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
	if (screen->swapBuffer[0] == TSMS_NULL)
		return TSMS_SUCCESS;
	pLock lock = TSMS_SEQUENCE_PRIORITY_LOCK_tryLock(screen->lock, TSMS_NULL, 1);
	if (lock == TSMS_NULL) {
		TSMS_SEQUENCE_PRIORITY_LOCK_preLock(screen->lock, 1);
		return TSMS_ERROR;
	}
	for (TSMS_POS i = 0; i < screen->xWidth; i ++)
		for (TSMS_POS j = 0; j < screen->height; j++) {
			uint8_t* p = screen->extraBuffer + j * screen->xWidth + i;
			if (*p) {
				TSMS_SCREEN_setCursor(screen, i * 8, j);
				TSMS_SCREEN_writeCommand(screen, screen->writeCommand);
				for (TSMS_POS k = 0; k < 8;k++)
					TSMS_SCREEN_writeData(screen, screen->swapBuffer[screen->swapIndex][j * screen->width + i * 8 + k]);
				*p = 0;
			}
		}
	screen->swapIndex ^= 1;
	TSMS_SEQUENCE_PRIORITY_LOCK_unlock(screen->lock, lock);
	TSMS_SEQUENCE_PRIORITY_LOCK_postLock(screen->lock, 1);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_SCREEN_drawPoint(TSMS_SCHP screen, uint16_t x, uint16_t y, TSMS_COLOR color, pLock preLock) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	pLock lock = TSMS_SEQUENCE_PRIORITY_LOCK_lock(screen->lock, preLock, 0);
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = __tsms_internal_screen_draw_point(screen, x, y, color);
	TSMS_SEQUENCE_PRIORITY_LOCK_unlock(screen->lock, lock);
	return result;
}


TSMS_RESULT TSMS_DISPLAY_setRequestMode(TSMS_DPHP display, pTimer timer, TSMS_SCREEN_REQUEST_MODE screenMode,
                                        TSMS_TOUCH_REQUEST_MODE touchMode) {
	if (display == TSMS_NULL)
		return TSMS_ERROR;
	if (screenMode == TSMS_SCREEN_REQUEST_MODE_IT) {
		if (!timer->option.enableCallbackInterrupt)
			return TSMS_ERROR;
		TSMS_TIMER_setCallback(timer, __tsms_internal_screen_auto_request, display->screen);
	} else if (screenMode == TSMS_SCREEN_REQUEST_MODE_SYNC_AND_IT) {
		if (!timer->option.enableCallbackInterrupt || !timer->option.enablePeriodInterrupt)
			return TSMS_ERROR;
		TSMS_TIMER_setCallback(timer, __tsms_internal_screen_auto_request_both, display);
	}
	display->touch->setRequestMode(display->touch, touchMode);
	display->timer = timer;
	display->lastTime = TSMS_TIMER_now(timer);
	display->restCount = 0;
	display->count = 0;
	display->screenMode = screenMode;
	display->touchMode = touchMode;
	TSMS_TIMER_start(timer);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_SCREEN_drawLine(TSMS_SCHP screen, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, TSMS_COLOR color,
                                 pLock preLock) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	if (x0 >= screen->width || y0 >= screen->height || x1 >= screen->width || y1 >= screen->height)
		return TSMS_ERROR;
	pLock lock = TSMS_SEQUENCE_PRIORITY_LOCK_lock(screen->lock, preLock, 0);
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = __tsms_internal_screen_draw_line(screen, x0, y0, x1, y1, color);
	TSMS_SEQUENCE_PRIORITY_LOCK_unlock(screen->lock, lock);
	return result;
}

TSMS_RESULT
TSMS_SCREEN_drawGradientLine(TSMS_SCHP screen, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, TSMS_COLOR from,
                             TSMS_COLOR to, pLock preLock) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	if (x0 >= screen->width || y0 >= screen->height || x1 >= screen->width || y1 >= screen->height)
		return TSMS_ERROR;
	pLock lock = TSMS_SEQUENCE_PRIORITY_LOCK_lock(screen->lock, preLock, 0);
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	int16_t dx = x1 - x0;
	int16_t dy = y1 - y0;
	int16_t sx = (dx > 0) - (dx < 0);
	int16_t sy = (dy > 0) - (dy < 0);
	dx = abs(dx);
	dy = abs(dy);
	int16_t err = (dx > dy ? dx : -dy) / 2;
	int16_t e2;
	uint16_t step = 0;
	uint16_t total = dx > dy ? dx : dy;
	while (1) {
		__tsms_internal_screen_draw_point(screen, x0, y0, TSMS_UTIL_gradientColor(from, to, ((float) step) / total));
		if (x0 == x1 && y0 == y1)
			break;
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		}
		step++;
	}
	TSMS_SEQUENCE_PRIORITY_LOCK_unlock(screen->lock, lock);
	return TSMS_SUCCESS;
}

TSMS_RESULT
TSMS_SCREEN_drawRect(TSMS_SCHP screen, uint16_t x, uint16_t y, uint16_t w, uint16_t h, TSMS_COLOR color, pLock preLock) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	if (x >= screen->width || y >= screen->height || x + w >= screen->width || y + h >= screen->height)
		return TSMS_ERROR;
	pLock lock = TSMS_SEQUENCE_PRIORITY_LOCK_lock(screen->lock, preLock, 0);
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	__tsms_internal_screen_draw_line(screen, x, y, x + w, y, color);
	__tsms_internal_screen_draw_line(screen, x + w, y, x + w, y + h, color);
	__tsms_internal_screen_draw_line(screen, x + w, y + h, x, y + h, color);
	__tsms_internal_screen_draw_line(screen, x, y + h, x, y, color);
	TSMS_SEQUENCE_PRIORITY_LOCK_unlock(screen->lock, lock);
	return TSMS_SUCCESS;
}

TSMS_RESULT
TSMS_SCREEN_fillRect(TSMS_SCHP screen, uint16_t x, uint16_t y, uint16_t w, uint16_t h, TSMS_COLOR color, pLock preLock) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	pLock lock = TSMS_SEQUENCE_PRIORITY_LOCK_lock(screen->lock, preLock, 0);
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	__tsms_internal_screen_fill_rect(screen, x, y, w, h, color);
	TSMS_SEQUENCE_PRIORITY_LOCK_unlock(screen->lock, lock);
	return TSMS_SUCCESS;
}

TSMS_RESULT
TSMS_SCREEN_drawThickLine(TSMS_SCHP screen, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t thickness,
                          TSMS_COLOR color, pLock preLock) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	if (x0 >= screen->width || y0 >= screen->height || x1 >= screen->width || y1 >= screen->height)
		return TSMS_ERROR;
	pLock lock = TSMS_SEQUENCE_PRIORITY_LOCK_lock(screen->lock, preLock, 0);
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	int16_t dx = x1 - x0;
	int16_t dy = y1 - y0;
	int16_t sx = (dx > 0) - (dx < 0);
	int16_t sy = (dy > 0) - (dy < 0);
	dx = abs(dx);
	dy = abs(dy);
	int16_t err = (dx > dy ? dx : -dy) / 2;
	int16_t e2;
	while (1) {
		__tsms_internal_screen_fill_rect(screen, x0 - thickness / 2, y0 - thickness / 2, thickness, thickness, color);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
	TSMS_SEQUENCE_PRIORITY_LOCK_unlock(screen->lock, lock);
	return TSMS_SUCCESS;
}

TSMS_RESULT
TSMS_SCREEN_drawChar(TSMS_SCHP screen, uint16_t x, uint16_t y, TSMS_FONT_TYPE fontType, void *font, uint8_t c,
                     TSMS_COLOR color, TSMS_FONT_SIZE size, pLock preLock) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	if (x >= screen->width || y >= screen->height)
		return TSMS_ERROR;
	pLock lock = TSMS_SEQUENCE_PRIORITY_LOCK_lock(screen->lock, preLock, 0);
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = __tsms_internal_screen_draw_char(screen, x, y, fontType, font, c, color, size);
	TSMS_SEQUENCE_PRIORITY_LOCK_unlock(screen->lock, lock);
	return result;
}

TSMS_RESULT
TSMS_SCREEN_drawString(TSMS_SCHP screen, uint16_t x, uint16_t y, TSMS_FONT_TYPE fontType, void *font, pString str,
                       TSMS_COLOR color, TSMS_FONT_SIZE size, pLock preLock) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	if (str == TSMS_NULL)
		return TSMS_ERROR;
	if (x >= screen->width || y >= screen->height)
		return TSMS_ERROR;
	pLock lock = TSMS_SEQUENCE_PRIORITY_LOCK_lock(screen->lock, preLock, 0);
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = __tsms_internal_screen_draw_string(screen, x, y, fontType, font, str, color, size);
	TSMS_SEQUENCE_PRIORITY_LOCK_unlock(screen->lock, lock);
	return result;
}

TSMS_RESULT TSMS_TOUCH_request(TSMS_THP touch, TSMS_TOUCH_REQUEST_MODE mode) {
	if (touch == TSMS_NULL)
		return TSMS_ERROR;
	touch->request(touch, mode);
	return TSMS_SUCCESS;
}

TSMS_LP TSMS_TOUCH_getTouchPointList(TSMS_THP touch) {
	if (touch == TSMS_NULL)
		return TSMS_NULL;
	TSMS_LP list = TSMS_LIST_create(10);
	if (touch->list->length == 0)
		return list;
	pLock lock = TSMS_SEQUENCE_PRIORITY_LOCK_lock(touch->lock, TSMS_NULL, 0);
	if (lock == TSMS_NULL)
		return list;
	for (TSMS_POS i = 0; i < touch->list->length; i++) {
		TSMS_TDP item = touch->list->list[i];
		TSMS_TDP data = malloc(sizeof(struct TSMS_TOUCH_DATA));
		data->id = item->id;
		data->x = item->x;
		data->y = item->y;
		data->size = item->size;
		TSMS_LIST_add(list, data);
	}
	TSMS_SEQUENCE_PRIORITY_LOCK_unlock(touch->lock, lock);
	return list;
}

TSMS_RESULT TSMS_TOUCH_setCallback(TSMS_THP touch, TSMS_TOUCH_CALLBACK callback, void * handler) {
	if (touch == TSMS_NULL)
		return TSMS_ERROR;
	touch->handler = handler;
	touch->callback = callback;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_SCREEN_request(TSMS_SCHP screen, TSMS_SCREEN_REQUEST_MODE mode) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_SCREEN_swap(screen);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_DISPLAY_request(TSMS_DPHP display) {
	if (display == TSMS_NULL)
		return TSMS_ERROR;
	double now = TSMS_TIMER_now(display->timer);
	display->restCount += (TSMS_TIMER_now(display->timer) - display->lastTime) * display->refreshRate;
	if (display->restCount >= 1)
		if (TSMS_LOCK_tryLock(display->lock)) {
			if (display->restCount >= 1) {
				display->count++;
				display->restCount--;
				if (display->screenMode != TSMS_SCREEN_REQUEST_MODE_IT)
					TSMS_SCREEN_request(display->screen, display->screenMode);
				if (display->touchMode != TSMS_TOUCH_REQUEST_MODE_IT)
					TSMS_TOUCH_request(display->touch, display->touchMode);
			}
			TSMS_LOCK_unlock(display->lock);
		}
	if (display->restCount > display->refreshRate)
		if (TSMS_LOCK_tryLock(display->lock)) {
			if (display->restCount > display->refreshRate) {
				display->restCount = 0;
				tString temp = TSMS_STRING_temp("Display refresh is too slow.");
				TSMS_ERR_report(TSMS_ERROR_TYPE_DISPLAY_REFRESH_TOO_SLOW, &temp);
			}
			TSMS_LOCK_unlock(display->lock);
		}
	display->lastTime = now;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_SCREEN_fillRectTopLeft(TSMS_SCHP screen, uint16_t x, uint16_t y, uint16_t width, uint16_t height, TSMS_COLOR color, pLock preLock) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	pLock lock = TSMS_SEQUENCE_PRIORITY_LOCK_lock(screen->lock, preLock, 0);
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = __tsms_internal_screen_fill_rect(screen, x, screen->height - y, width, height, color);
	TSMS_SEQUENCE_PRIORITY_LOCK_unlock(screen->lock, lock);
	return result;
}

TSMS_RESULT TSMS_SCREEN_drawCharTopLeft(TSMS_SCHP screen, uint16_t x, uint16_t y, TSMS_FONT_TYPE fontType, void *font, char c, TSMS_COLOR color, TSMS_FONT_SIZE size, pLock preLock) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	if (x >= screen->width || y >= screen->height)
		return TSMS_ERROR;
	pLock lock = TSMS_SEQUENCE_PRIORITY_LOCK_lock(screen->lock, preLock, 0);
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = __tsms_internal_screen_draw_char(screen, x, screen->height - y, fontType, font, c, color, size);
	TSMS_SEQUENCE_PRIORITY_LOCK_unlock(screen->lock, lock);
	return result;
}

TSMS_RESULT  TSMS_SCREEN_drawStringTopLeft(TSMS_SCHP screen, uint16_t x, uint16_t y, TSMS_FONT_TYPE fontType, void *font, pString str, TSMS_COLOR color, TSMS_FONT_SIZE size, pLock preLock) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	if (x >= screen->width || y >= screen->height)
		return TSMS_ERROR;
	pLock lock = TSMS_SEQUENCE_PRIORITY_LOCK_lock(screen->lock, preLock, 0);
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = __tsms_internal_screen_draw_string(screen, x, screen->height - y, fontType, font, str, color, size);
	TSMS_SEQUENCE_PRIORITY_LOCK_unlock(screen->lock, lock);
	return result;
}