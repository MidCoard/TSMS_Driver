#ifndef TSMS_IT_H
#define TSMS_IT_H

typedef struct TSMS_IT_GPIO *TSMS_IT_GPIO_POINTER;
typedef TSMS_IT_GPIO_POINTER TSMS_IGP;

typedef struct TSMS_IT_PRINTER *TSMS_IT_PRINTER_POINTER;
typedef TSMS_IT_PRINTER_POINTER TSMS_IPP;

typedef struct TSMS_IT_TIMER *TSMS_IT_TIMER_POINTER;
typedef TSMS_IT_TIMER_POINTER TSMS_ITP;

#include "tsms_timer.h"
#include "tsms_printer.h"
#include "tsms_gpio.h"

struct TSMS_IT_GPIO {
	TSMS_GHP gpio;
	void *handler;
	TSMS_IT_GPIO_CALLBACK callback;
};

struct TSMS_IT_PRINTER {
	TSMS_PHP printer;
	void *handler;
	TSMS_IT_PRINTER_CALLBACK callback;
};

struct TSMS_IT_TIMER {
	pTimer timer;
	void *handler;
	TSMS_IT_TIMER_TYPE type;
	TSMS_IT_TIMER_CALLBACK callback;
};

#endif //TSMS_IT_H
