#ifndef TSMS_PRINTER_H
#define TSMS_PRINTER_H

typedef struct TSMS_PRINTER_HANDLER *TSMS_PRINTER_HANDLER_POINTER;
typedef TSMS_PRINTER_HANDLER_POINTER TSMS_PHP;

#include "tsms.h"
#include "tsms_util.h"
#include "tsms_string.h"

typedef void (*TSMS_PRINTER_CALLBACK)(void *handler, TSMS_PHP printer);

struct TSMS_PRINTER_HANDLER {
#if defined(TSMS_STM32) && defined(HAL_UART_MODULE_ENABLED)
	UART_HandleTypeDef *handler;
#endif
	char buffer;
	TSMS_CLP str;
	volatile bool hasData;
	TSMS_PRINTER_CALLBACK callback;
	void *callbackData;
	pString strBuffer;
	pString customBuffer;
	char *stringBuffer;
};

#if defined(TSMS_STM32) && defined(HAL_UART_MODULE_ENABLED)

TSMS_PHP TSMS_PRINTER_createUARTPrinter(UART_HandleTypeDef *handler);

#endif

typedef void(*TSMS_IT_PRINTER_CALLBACK)(void *, TSMS_PHP);

TSMS_RESULT TSMS_IT_addPrinter(TSMS_PHP php, TSMS_IT_PRINTER_CALLBACK callback, void *handler);

pString TSMS_PRINTER_get(TSMS_PHP printer);

pString TSMS_PRINTER_getBlocking(TSMS_PHP printer);

pString TSMS_PRINTER_getBlockingCustom(TSMS_PHP printer, pString customBuffer);

TSMS_RESULT TSMS_PRINTER_setCallback(TSMS_PHP printer, TSMS_PRINTER_CALLBACK callback, void *data);

TSMS_RESULT TSMS_print(TSMS_PHP printer, char * str);

TSMS_RESULT TSMS_println(TSMS_PHP printer, char * str);

TSMS_RESULT TSMS_printInt(TSMS_PHP printer, int v);

TSMS_RESULT TSMS_printIntln(TSMS_PHP printer, int v);

TSMS_RESULT TSMS_printFloat(TSMS_PHP printer, float v);

TSMS_RESULT TSMS_printFloatln(TSMS_PHP printer, float v);

TSMS_RESULT TSMS_printChar(TSMS_PHP printer, char c);

TSMS_RESULT TSMS_printCharln(TSMS_PHP printer, char c);

TSMS_RESULT TSMS_printf(TSMS_PHP printer, const char * c, ...);

TSMS_RESULT TSMS_setDefaultPrinter(TSMS_PHP printer);

TSMS_RESULT print(const char * str, ...);

#endif //TSMS_PRINTER_H
