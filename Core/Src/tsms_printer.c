#include "tsms_printer.h"

char stringBuffer[1024];
char charBuffer[2] = {0, 0};
TSMS_PHP defaultPrinter = TSMS_NULL;

#if defined(TSMS_STM32) && defined(HAL_UART_MODULE_ENABLED)

TSMS_PHP TSMS_PRINTER_createUARTPrinter(UART_HandleTypeDef *handler) {
	TSMS_PHP printer = malloc(sizeof(struct TSMS_PRINTER_HANDLER));
	printer->handler = handler;
	return printer;
}

#endif

TSMS_RESULT TSMS_PRINTER_print(TSMS_PHP printer, char *str) {
#if defined(TSMS_STM32) && defined(HAL_UART_MODULE_ENABLED)
	HAL_StatusTypeDef status = HAL_UART_Transmit(printer->handler, str, strlen(str), 10000);
	return status == HAL_OK ? TSMS_SUCCESS : TSMS_ERROR;
#else
	return TSMS_TIMEOUT;
#endif

}

TSMS_RESULT TSMS_PRINTER_println(TSMS_PHP printer, char *str) {
	TSMS_RESULT result = TSMS_PRINTER_print(printer, str);
	return result | TSMS_PRINTER_print(printer, "\n");
}

TSMS_RESULT TSMS_PRINTER_printInt(TSMS_PHP printer, int v) {
	itoa(v, stringBuffer, 10);
	return TSMS_PRINTER_print(printer, stringBuffer);
}

TSMS_RESULT TSMS_PRINTER_printIntln(TSMS_PHP printer, int v) {
	TSMS_RESULT result = TSMS_PRINTER_printInt(printer, v);
	return result | TSMS_PRINTER_print(printer, "\n");
}

TSMS_RESULT TSMS_PRINTER_printChar(TSMS_PHP printer, char c) {
	charBuffer[0] = c;
	return TSMS_PRINTER_print(printer, charBuffer);
}

TSMS_RESULT TSMS_PRINTER_printCharln(TSMS_PHP printer, char c) {
	TSMS_RESULT result = TSMS_PRINTER_printChar(printer, c);
	return result | TSMS_PRINTER_print(printer, "\n");
}

TSMS_RESULT TSMS_PRINTER_printFloat(TSMS_PHP printer, float f) {
	sprintf(stringBuffer, "%f", f);
	return TSMS_PRINTER_print(printer, stringBuffer);
}

TSMS_RESULT TSMS_PRINTER_printFloatln(TSMS_PHP printer, float f) {
	TSMS_RESULT result = TSMS_PRINTER_printFloat(printer, f);
	return result | TSMS_PRINTER_print(printer, "\n");
}

TSMS_RESULT TSMS_PRINTER_printf(TSMS_PHP printer, const char *str, ...) {
	va_list p;
	va_start(p, str);
	vsprintf(stringBuffer, str, p);
	va_end(p);
	return TSMS_PRINTER_print(printer, stringBuffer);
}

TSMS_RESULT TSMS_PRINTER_setDefaultPrinter(TSMS_PHP printer) {
	defaultPrinter = printer;
	return TSMS_SUCCESS;
}

int __io_putchar(int ch) {
	if (defaultPrinter == TSMS_NULL)
		return -1;
	TSMS_PRINTER_printChar(defaultPrinter, ch);
	return 1;
}