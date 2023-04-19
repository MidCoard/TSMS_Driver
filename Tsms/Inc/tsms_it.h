#ifndef TSMS_IT_H
#define TSMS_IT_H

typedef struct TSMS_IT_GPIO *TSMS_IT_GPIO_POINTER;
typedef TSMS_IT_GPIO_POINTER TSMS_IGP;

typedef struct TSMS_IT_PRINTER *TSMS_IT_PRINTER_POINTER;
typedef TSMS_IT_PRINTER_POINTER TSMS_IPP;

typedef struct TSMS_IT_TIMER *TSMS_IT_TIMER_POINTER;
typedef TSMS_IT_TIMER_POINTER TSMS_ITP;

typedef enum {
	TSMS_IT_GPIO_FALLING, TSMS_IT_GPIO_RISING
} TSMS_IT_GPIO_TYPE;

typedef enum {
	TSMS_IT_TIMER_PERIOD_ELAPSED, TSMS_IT_TIMER_CAPTURE
} TSMS_IT_TIMER_TYPE;

#include "tsms_gpio.h"
#include "tsms_util.h"
#include "tsms_printer.h"
#include "tsms_timer.h"

struct TSMS_IT_GPIO {
	TSMS_GHP gpio;
	TSMS_IT_GPIO_TYPE type;
	TSMS_IT_GPIO_CALLBACK callback;
	void *handler;
};

struct TSMS_IT_PRINTER {
	TSMS_PHP printer;
	TSMS_IT_PRINTER_CALLBACK callback;
	void *handler;
};

struct TSMS_IT_TIMER {
	pTimer timer;
	TSMS_IT_TIMER_CALLBACK callback;
	void *handler;
	TSMS_IT_TIMER_TYPE type;
};

#endif //TSMS_IT_H
