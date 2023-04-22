#ifndef TSMS_IT_H
#define TSMS_IT_H

#include "tsms_def.h"

struct TSMS_IT_GPIO {
	TSMS_GHP gpio;
	TSMS_IT_GPIO_CALLBACK callback;
	void *handler;
};

struct TSMS_IT_PRINTER {
	void *handler;
	TSMS_PHP printer;
	TSMS_IT_PRINTER_CALLBACK callback;
};

struct TSMS_IT_TIMER {
	pTimer timer;
	void *handler;
	TSMS_IT_TIMER_TYPE type;
	TSMS_IT_TIMER_CALLBACK callback;
};

#endif //TSMS_IT_H
