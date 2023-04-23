#ifndef TSMS_DISPLAY_H
#define TSMS_DISPLAY_H

typedef enum {
	TSMS_SCREEN_SYNC_REQUEST, TSMS_SCREEN_IT_AUTO_REQUEST, TSMS_SCREEN_SYNC_AND_IT_REQUEST
} TSMS_SCREEN_REQUEST_MODE;

typedef enum {
	TSMS_TOUCH_SYNC_REQUEST, TSMS_TOUCH_IT_AUTO_REQUEST, TSMS_TOUCH_SYNC_AND_IT_REQUEST
} TSMS_TOUCH_REQUEST_MODE;

typedef enum {
	TSMS_SCREEN_AUTO_DETECT, TSMS_SCREEN_ILI9341, TSMS_SCREEN_ST7789, // default width 240 height 320
	TSMS_SCREEN_NT35310, // default width 320 height 480
	TSMS_SCREEN_NT35510, TSMS_SCREEN_NT5510, // default width 480 height 800
	TSMS_SCREEN_SSD1963, // default width 800 height 480
} TSMS_SCREEN_TYPE;

typedef enum {
	TSMS_TOUCH_AUTO_DETECT, TSMS_TOUCH_GT9147
} TSMS_TOUCH_TYPE;

typedef enum {
	TSMS_DISPLAY_VERTICAL, TSMS_DISPLAY_HORIZONTAL
} TSMS_DISPLAY_DIRECTION;

typedef enum {
	TSMS_SCAN_L2R_U2D = 0, TSMS_SCAN_L2R_D2U, TSMS_SCAN_R2L_U2D, TSMS_SCAN_R2L_D2U,
	TSMS_SCAN_U2D_L2R, TSMS_SCAN_U2D_R2L, TSMS_SCAN_D2U_L2R, TSMS_SCAN_D2U_R2L,
}TSMS_SCAN_DIRECTION;

#include "tsms_def.h"

typedef TSMS_RESULT(*TSMS_SCREEN_FUNCTION)(TSMS_SCHP);
typedef TSMS_RESULT(*TSMS_INIT_SCREEN_FUNCTION)(TSMS_SCHP screen, void* option);
typedef TSMS_RESULT(*TSMS_DISPLAY_DIRECTION_FUNCTION)(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction);
typedef TSMS_RESULT(*TSMS_SCAN_DIRECTION_FUNCTION)(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction);
typedef TSMS_RESULT(*TSMS_CURSOR_FUNCTION)(TSMS_SCHP screen, uint16_t x, uint16_t y);


typedef TSMS_RESULT(*TSMS_RESET_TOUCH_FUNCTION)(TSMS_THP);
typedef TSMS_RESULT(*TSMS_INIT_TOUCH_FUNCTION)(TSMS_THP touch, void* option);
typedef TSMS_RESULT(*TSMS_REQUEST_TOUCH_FUNCTION)(TSMS_THP touch, TSMS_TOUCH_REQUEST_MODE mode);
typedef TSMS_RESULT(*TSMS_REQUEST_TOUCH_FUNCTION)(TSMS_THP touch, TSMS_TOUCH_REQUEST_MODE mode);
typedef bool(*TSMS_TOUCH_CALLBACK)(TSMS_THP touch, uint8_t id, uint16_t x, uint16_t y, uint16_t size, void * handler);

struct TSMS_DISPLAY_HANDLER {
	TSMS_THP touch;
	TSMS_SCHP screen;

	TSMS_SCREEN_REQUEST_MODE screenMode;
	TSMS_TOUCH_REQUEST_MODE touchMode;

	float refreshRate;
	pTimer timer;
	double lastTime;
	double restCount;
	uint64_t count;
	pLock lock;
};

struct TSMS_SCREEN_HANDLER {
	volatile uint16_t * command;
	volatile uint16_t * data;
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
	uint16_t swapY;
	uint16_t swapStep;
	bool lazySwapLabels[100];
	pSequencePriorityLock lock;

	TSMS_INIT_SCREEN_FUNCTION init;
	TSMS_DISPLAY_DIRECTION_FUNCTION setDisplayDirection;
	TSMS_SCAN_DIRECTION_FUNCTION setScanDirection;
	TSMS_CURSOR_FUNCTION setCursor;
};

struct TSMS_TOUCH_HANDLER {
	void* custom;
	TSMS_RESET_TOUCH_FUNCTION reset;
	TSMS_TOUCH_TYPE type;
	TSMS_INIT_TOUCH_FUNCTION init;
	TSMS_REQUEST_TOUCH_FUNCTION request;
	TSMS_REQUEST_TOUCH_FUNCTION setRequestMode;

	TSMS_TOUCH_CALLBACK callback;
	void *handler;

	TSMS_GHP resetPin;

	TSMS_LP list;
	pSequencePriorityLock lock;
	bool requestFlag;
};

struct TSMS_TOUCH_DATA {
	uint16_t x;
	uint16_t y;
	uint16_t id;
	uint16_t size;
};

void TSMS_SCREEN_writeRegister(TSMS_SCHP screen, uint16_t reg, uint16_t value);

void TSMS_SCREEN_writeCommand(TSMS_SCHP screen, volatile uint16_t command);

volatile uint16_t TSMS_SCREEN_readData(TSMS_SCHP screen);

void TSMS_SCREEN_writeData(TSMS_SCHP screen, volatile uint16_t data);



TSMS_THP TSMS_TOUCH_createHandler(void* handler, TSMS_TOUCH_TYPE type, TSMS_GHP rst, void * option);

TSMS_RESULT TSMS_TOUCH_reset(TSMS_THP touch);

TSMS_RESULT TSMS_TOUCH_request(TSMS_THP touch, TSMS_TOUCH_REQUEST_MODE mode);

TSMS_LP TSMS_TOUCH_getTouchPointList(TSMS_THP touch);

TSMS_RESULT TSMS_TOUCH_setCallback(TSMS_THP touch, TSMS_TOUCH_CALLBACK callback, void *handler);



TSMS_SCHP
TSMS_SCREEN_create16BitHandler(uint16_t *command, uint16_t *data, TSMS_GHP bg, TSMS_SCREEN_TYPE type, uint16_t width,
                               uint16_t height, uint16_t *swapBuffer, TSMS_SSD1963_OPTION* option);

TSMS_RESULT TSMS_SCREEN_reset(TSMS_SCHP screen);

TSMS_RESULT TSMS_SCREEN_enableBackgroudLight(TSMS_SCHP screen);

TSMS_RESULT TSMS_SCREEN_disableBackgroudLight(TSMS_SCHP screen);

TSMS_RESULT TSMS_SCREEN_setDisplayDirection(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction);

TSMS_RESULT TSMS_SCREEN_setScanDirection(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction);

TSMS_RESULT TSMS_SCREEN_swap(TSMS_SCHP screen);

TSMS_RESULT TSMS_SCREEN_setCursor(TSMS_SCHP screen, uint16_t x, uint16_t y);

TSMS_RESULT TSMS_SCREEN_drawPoint(TSMS_SCHP screen, uint16_t x, uint16_t y, TSMS_COLOR color, pLock preLock);

TSMS_RESULT TSMS_SCREEN_drawLine(TSMS_SCHP screen, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, TSMS_COLOR color, pLock preLock);

TSMS_RESULT TSMS_SCREEN_drawGradientLine(TSMS_SCHP screen, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, TSMS_COLOR from, TSMS_COLOR to, pLock preLock);

TSMS_RESULT TSMS_SCREEN_drawRect(TSMS_SCHP screen, uint16_t x, uint16_t y, uint16_t w, uint16_t h, TSMS_COLOR color, pLock preLock);

TSMS_RESULT TSMS_SCREEN_fillRect(TSMS_SCHP screen, uint16_t x, uint16_t y, uint16_t w, uint16_t h, TSMS_COLOR color, pLock preLock);

TSMS_RESULT TSMS_SCREEN_drawThickLine(TSMS_SCHP screen, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t thickness, TSMS_COLOR color, pLock preLock);

TSMS_RESULT TSMS_SCREEN_drawChar(TSMS_SCHP screen, uint16_t x, uint16_t y, TSMS_FONT_TYPE fontType, void * font, uint8_t c, TSMS_COLOR color, TSMS_FONT_SIZE size, pLock preLock);

TSMS_RESULT TSMS_SCREEN_drawString(TSMS_SCHP screen, uint16_t x, uint16_t y, TSMS_FONT_TYPE fontType, void * font, pString str, TSMS_COLOR color, TSMS_FONT_SIZE size, pLock preLock);

TSMS_RESULT TSMS_SCREEN_request(TSMS_SCHP screen, TSMS_SCREEN_REQUEST_MODE mode);



TSMS_DPHP TSMS_DISPLAY_createHandler(TSMS_SCHP screen, TSMS_THP touch, float refreshRate);

TSMS_RESULT TSMS_DISPLAY_setRequestMode(TSMS_DPHP display, pTimer timer, TSMS_SCREEN_REQUEST_MODE screenMode, TSMS_TOUCH_REQUEST_MODE touchMode);

TSMS_RESULT TSMS_DISPLAY_reset(TSMS_DPHP display);

TSMS_RESULT TSMS_DISPLAY_request(TSMS_DPHP display);


#endif //TSMS_DISPLAY_H
