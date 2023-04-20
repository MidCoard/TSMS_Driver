#ifndef TSMS_DISPLAY_H
#define TSMS_DISPLAY_H

typedef enum {
	TSMS_SCREEN_AUTO_DETECT, TSMS_SCREEN_ILI9341, TSMS_SCREEN_ST7789, // default width 240 height 320
	TSMS_SCREEN_NT35310, // default width 320 height 480
	TSMS_SCREEN_NT35510, TSMS_SCREEN_NT5510, // default width 480 height 800
	TSMS_SCREEN_SSD1963, // default width 800 height 480
} TSMS_SCREEN_TYPE;

typedef enum {
	TSMS_DISPLAY_VERTICAL, TSMS_DISPLAY_HORIZONTAL
} TSMS_DISPLAY_DIRECTION;

typedef enum {
	TSMS_SCAN_L2R_U2D = 0, TSMS_SCAN_L2R_D2U, TSMS_SCAN_R2L_U2D, TSMS_SCAN_R2L_D2U,
	TSMS_SCAN_U2D_L2R, TSMS_SCAN_U2D_R2L, TSMS_SCAN_D2U_L2R, TSMS_SCAN_D2U_R2L,
}TSMS_SCAN_DIRECTION;

#include "tsms_def.h"

typedef TSMS_RESULT(*TSMS_RESET_TOUCH_FUNCTION)(TSMS_THP);
typedef TSMS_RESULT(*TSMS_SCREEN_FUNCTION)(TSMS_SCHP);
typedef TSMS_RESULT(*TSMS_INIT_SCREEN_FUNCTION)(TSMS_SCHP screen, void* option);
typedef TSMS_RESULT(*TSMS_DISPLAY_DIRECTION_FUNCTION)(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction);
typedef TSMS_RESULT(*TSMS_SCAN_DIRECTION_FUNCTION)(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction);
typedef TSMS_RESULT(*TSMS_CURSOR_FUNCTION)(TSMS_SCHP screen, uint16_t x, uint16_t y);

struct TSMS_DISPLAY_HANDLER {
	TSMS_THP touch;
	TSMS_SCHP screen;
};

struct TSMS_SCREEN_HANDLER {
	uint16_t * command;
	uint16_t * data;
	TSMS_SCREEN_FUNCTION reset;
	TSMS_GHP bg;
	TSMS_SCREEN_TYPE type;
	uint16_t defaultWidth;
	uint16_t defaultHeight;
	uint16_t width;
	uint16_t height;
	uint16_t writeCommand;
	uint16_t setXCommand;
	uint16_t setYCommand;
	TSMS_DISPLAY_DIRECTION displayDirection;
	TSMS_SCAN_DIRECTION scanDirection;
	uint16_t *swapBuffer;
	TSMS_INIT_SCREEN_FUNCTION init;
	TSMS_DISPLAY_DIRECTION_FUNCTION setDisplayDirection;
	TSMS_SCAN_DIRECTION_FUNCTION setScanDirection;
	TSMS_CURSOR_FUNCTION setCursor;

	uint16_t swapY;
	uint16_t swapStep;

	bool lazySwapLabels[100];
	pSequencePriorityLock lock;
};

struct TSMS_TOUCH_HANDLER {
	void* custom;
	TSMS_RESET_TOUCH_FUNCTION reset;
};

void TSMS_SCREEN_writeRegister(TSMS_SCHP screen, uint16_t reg, uint16_t value);

void TSMS_SCREEN_writeCommand(TSMS_SCHP screen, volatile uint16_t command);

volatile uint16_t TSMS_SCREEN_readData(TSMS_SCHP screen);

void TSMS_SCREEN_writeData(TSMS_SCHP screen, volatile uint16_t data);

TSMS_SCHP
TSMS_SCREEN_create16BitHandler(uint16_t *command, uint16_t *data, TSMS_GHP bg, TSMS_SCREEN_TYPE type, uint16_t width,
                               uint16_t height, uint16_t * swapBuffer, TSMS_SSD1963_OPTION* option);

TSMS_THP TSMS_TOUCH_createGT9147Handler(TSMS_IHP iic, TSMS_GHP reset, TSMS_GHP interrupt);

TSMS_DPHP TSMS_DISPLAY_createHandler(TSMS_SCHP screen, TSMS_THP touch);

TSMS_RESULT TSMS_DISPLAY_reset(TSMS_DPHP display);

TSMS_RESULT TSMS_SCREEN_enableBackgroudLight(TSMS_SCHP screen);

TSMS_RESULT TSMS_SCREEN_disableBackgroudLight(TSMS_SCHP screen);

TSMS_RESULT TSMS_SCREEN_setDisplayDirection(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction);

TSMS_RESULT TSMS_SCREEN_setScanDirection(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction);

TSMS_RESULT TSMS_SCREEN_swap(TSMS_SCHP screen);

TSMS_RESULT TSMS_SCREEN_setCursor(TSMS_SCHP screen, uint16_t x, uint16_t y);

TSMS_RESULT TSMS_SCREEN_drawPoint(TSMS_SCHP screen, uint16_t x, uint16_t y, TSMS_CR color, pLock preLock);

TSMS_RESULT TSMS_SCREEN_enableAutoSwap(TSMS_SCHP screen, pTimer timer);

TSMS_RESULT TSMS_SCREEN_drawLine(TSMS_SCHP screen, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, TSMS_CR color, pLock preLock);

TSMS_RESULT TSMS_SCREEN_drawGradientLine(TSMS_SCHP screen, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, TSMS_CR from, TSMS_CR to, pLock preLock);

TSMS_RESULT TSMS_SCREEN_drawRect(TSMS_SCHP screen, uint16_t x, uint16_t y, uint16_t w, uint16_t h, TSMS_CR color, pLock preLock);

TSMS_RESULT TSMS_SCREEN_fillRect(TSMS_SCHP screen, uint16_t x, uint16_t y, uint16_t w, uint16_t h, TSMS_CR color, pLock preLock);

TSMS_RESULT TSMS_SCREEN_drawThickLine(TSMS_SCHP screen, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t thickness, TSMS_CR color, pLock preLock);

#endif //TSMS_DISPLAY_H
