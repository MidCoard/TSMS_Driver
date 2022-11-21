#ifndef TSMS_IT_H
#define TSMS_IT_H

#include "tsms_gpio.h"
#include "tsms_util.h"
#include "tsms_printer.h"

typedef enum {TSMS_IT_GPIO_FALLING, TSMS_IT_GPIO_RISING} TSMS_IT_GPIO_TYPE;

typedef void(*TSMS_IT_GPIO_CALLBACK)(void*,TSMS_GHP);

struct TSMS_IT_GPIO {
	TSMS_GHP gpio;
	TSMS_IT_GPIO_TYPE type;
	TSMS_IT_GPIO_CALLBACK callback;
	void* handler;
};
typedef struct TSMS_IT_GPIO * TSMS_IT_GPIO_POINTER;
typedef TSMS_IT_GPIO_POINTER TSMS_IGP;

struct TSMS_IT_PRINTER {
	TSMS_PHP printer;
	TSMS_IT_PRINTER_CALLBACK callback;
	void* handler;
};

typedef struct TSMS_IT_PRINTER * TSMS_IT_PRINTER_POINTER;
typedef TSMS_IT_PRINTER_POINTER TSMS_IPP;

TSMS_RESULT TSMS_IT_addGPIO(TSMS_GHP gpio, TSMS_IT_GPIO_TYPE type,TSMS_IT_GPIO_CALLBACK callback, void *);

TSMS_RESULT TSMS_IT_addPrinter(TSMS_PHP php, TSMS_IT_PRINTER_CALLBACK callback, void*);

#endif //TSMS_IT_H
