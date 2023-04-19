#ifndef TSMS_DISPLAY_H
#define TSMS_DISPLAY_H

#define TSMS_ILI9341_ID 0x9341
#define TSMS_ST7789_ID 0x8552
#define TSMS_NT35310_ID 0x5310
#define TSMS_NT35510_ID 0x8000
#define TSMS_NT5510_ID 0x5510
#define TSMS_SSD1963_ID 0x5761
#include "screen/tsms_ssd1963.h"

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

#include "touch/tsms_gt9147.h"

typedef struct TSMS_DRIVER_HANDLER *TSMS_DRIVER_HANDLER_POINTER;
typedef TSMS_DRIVER_HANDLER_POINTER TSMS_DHP;

typedef struct TSMS_REGISTER_HANDLER *TSMS_REGISTER_HANDLER_POINTER;
typedef TSMS_REGISTER_HANDLER_POINTER TSMS_RHP;

typedef struct TSMS_TOUCH_HANDLER *TSMS_TOUCH_HANDLER_POINTER;
typedef TSMS_TOUCH_HANDLER_POINTER TSMS_THP;

typedef struct TSMS_SCREEN_HANDLER *TSMS_SCREEN_HANDLER_POINTER;
typedef TSMS_SCREEN_HANDLER_POINTER TSMS_SCHP;

#include "tsms_iic.h"

typedef struct TSMS_DISPLAY_HANDLER * TSMS_DISPLAY_HANDLER_POINTER;
typedef TSMS_DISPLAY_HANDLER_POINTER TSMS_DPHP;
typedef TSMS_RESULT(*TSMS_RESET_TOUCH_FUNCTION)(TSMS_THP);
typedef TSMS_RESULT(*TSMS_RESET_SCREEN_FUNCTION)(TSMS_SCHP);

struct TSMS_DISPLAY_HANDLER {
	TSMS_THP touch;
	TSMS_SCHP screen;
};

struct TSMS_SCREEN_HANDLER {
	uint16_t * command;
	uint16_t * data;
	TSMS_RESET_SCREEN_FUNCTION reset;
	TSMS_GHP bg;
	TSMS_SCREEN_TYPE type;
	uint16_t defaultWidth;
	uint16_t defaultHeight;
	uint16_t width;
	uint16_t height;
	uint16_t writeCommand;
	uint16_t setXCommand;
	uint16_t setYCommand;
	TSMS_DISPLAY_DIRECTION direction;
	TSMS_SCAN_DIRECTION scan;
	uint16_t *swapBuffer;
};

struct TSMS_TOUCH_HANDLER {
	void* custom;
	TSMS_RESET_TOUCH_FUNCTION reset;
};

typedef struct TSMS_GT9147_HANDLER * TSMS_GT9147_HANDLER_POINTER;
typedef TSMS_GT9147_HANDLER_POINTER TSMS_GT9147;

struct TSMS_GT9147_HANDLER {
	TSMS_IHP iic;
	TSMS_GHP reset;
	TSMS_GHP interrupt;
};

#include "tsms_driver.h"
#include "tsms_gpio.h"

TSMS_SCHP
TSMS_SCREEN_create16BitHandler(uint16_t *command, uint16_t *data, TSMS_GHP bg, TSMS_SCREEN_TYPE type, uint16_t width,
                               uint16_t height, uint16_t * swapBuffer, TSMS_SSD1963_OP option);

TSMS_THP TSMS_TOUCH_createGT9147Handler(TSMS_IHP iic, TSMS_GHP reset, TSMS_GHP interrupt);

TSMS_DPHP TSMS_DISPLAY_createHandler(TSMS_SCHP screen, TSMS_THP touch);

TSMS_RESULT TSMS_DISPLAY_reset(TSMS_DPHP display);

TSMS_RESULT TSMS_SCREEN_enableBackgroudLight(TSMS_SCHP screen);

TSMS_RESULT TSMS_SCREEN_disableBackgroudLight(TSMS_SCHP screen);

TSMS_RESULT TSMS_SCREEN_setDisplayDirection(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction);

TSMS_RESULT TSMS_SCREEN_setScanDirection(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction);

TSMS_RESULT TSMS_SCREEN_swap(TSMS_SCHP screen);

TSMS_RESULT TSMS_SCREEN_setCursor(TSMS_SCHP screen, uint16_t x, uint16_t y);

#endif //TSMS_DISPLAY_H
