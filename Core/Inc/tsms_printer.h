#ifndef TSMS_PRINTER_H
#define TSMS_PRINTER_H

#include "tsms.h"
#include "tsms_util.h"
#include "tsms_string.h"

typedef struct TSMS_PRINTER_HANDLER *TSMS_PRINTER_HANDLER_POINTER;
typedef TSMS_PRINTER_HANDLER_POINTER TSMS_PHP;

typedef void (*TSMS_PRINTER_CALLBACK)(void* handler, TSMS_PHP printer);

struct TSMS_PRINTER_HANDLER {
#if defined(TSMS_STM32) && defined(HAL_UART_MODULE_ENABLED)
	UART_HandleTypeDef *handler;
#endif
	char buffer;
	TSMS_UCLP str;
	volatile bool hasData;
	TSMS_PRINTER_CALLBACK callback;
	void *callbackData;
	pString strBuffer;
	pString customBuffer;
	char* stringBuffer;
};

#if defined(TSMS_STM32) && defined(HAL_UART_MODULE_ENABLED)

TSMS_PHP TSMS_PRINTER_createUARTPrinter(UART_HandleTypeDef *handler);

#endif

typedef void(*TSMS_IT_PRINTER_CALLBACK)(void*,TSMS_PHP);

TSMS_RESULT TSMS_IT_addPrinter(TSMS_PHP php, TSMS_IT_PRINTER_CALLBACK callback, void* handler);

pString TSMS_PRINTER_get(TSMS_PHP printer);

pString TSMS_PRINTER_getBlocking(TSMS_PHP printer);

pString TSMS_PRINTER_getBlockingCustom(TSMS_PHP printer, pString customBuffer);

TSMS_RESULT TSMS_PRINTER_print(TSMS_PHP printer, char *);

TSMS_RESULT TSMS_PRINTER_println(TSMS_PHP printer, char *);

TSMS_RESULT TSMS_PRINTER_printInt(TSMS_PHP printer, int);

TSMS_RESULT TSMS_PRINTER_printIntln(TSMS_PHP printer, int);

TSMS_RESULT TSMS_PRINTER_printFloat(TSMS_PHP printer, float);

TSMS_RESULT TSMS_PRINTER_printFloatln(TSMS_PHP printer, float);

TSMS_RESULT TSMS_PRINTER_printChar(TSMS_PHP printer, char);

TSMS_RESULT TSMS_PRINTER_printCharln(TSMS_PHP printer, char);

TSMS_RESULT TSMS_PRINTER_printf(TSMS_PHP printer, const char *, ...);

TSMS_RESULT TSMS_PRINTER_setDefaultPrinter(TSMS_PHP printer);

TSMS_RESULT print(const char *, ...);
TSMS_RESULT TSMS_PRINTER_setCallback(TSMS_PHP printer, TSMS_PRINTER_CALLBACK callback, void *data);

#endif //TSMS_PRINTER_H
