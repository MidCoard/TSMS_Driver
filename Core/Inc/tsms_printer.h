#ifndef TSMS_PRINTER_H
#define TSMS_PRINTER_H

#include "tsms.h"

struct TSMS_PRINTER_HANDLER {
#if defined(TSMS_STM32) && defined(HAL_UART_MODULE_ENABLED)
	UART_HandleTypeDef *handler;
#endif
};

typedef struct TSMS_PRINTER_HANDLER *TSMS_PRINTER_HANDLER_POINTER;
typedef TSMS_PRINTER_HANDLER_POINTER TSMS_PHP;

#if defined(TSMS_STM32) && defined(HAL_UART_MODULE_ENABLED)

TSMS_PHP TSMS_PRINTER_createUARTPrinter(UART_HandleTypeDef *handler);

#endif

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

#endif //TSMS_PRINTER_H
