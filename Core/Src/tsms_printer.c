#include "tsms_printer.h"

char stringBuffer[1024];
char charBuffer[2] = {0, 0};
TSMS_PHP defaultPrinter = TSMS_NULL;

#if defined(TSMS_STM32) && defined(HAL_UART_MODULE_ENABLED)

static void __tsms_internal_callback(void * handler, TSMS_PHP php) {
	if (php->buffer != 0) {
		if (php->buffer == '\n') {
			if (php->customBuffer != TSMS_NULL)
				TSMS_STRING_getString(php->str, php->customBuffer);
			else
				TSMS_STRING_getString(php->str, php->strBuffer);
			php->customBuffer = TSMS_NULL;
			TSMS_UTIL_clearCharList(php->str);
			if (php->callback != TSMS_NULL)
				php->callback(php->callbackData, php);
			else
				php->hasData = true;
		} else TSMS_UTIL_addCharList(php->str, php->buffer);
	}
	HAL_UART_Receive_IT(php->handler,  &php->buffer, 1);
}

TSMS_PHP TSMS_PRINTER_createUARTPrinter(UART_HandleTypeDef *handler) {
	TSMS_PHP printer = malloc(sizeof(struct TSMS_PRINTER_HANDLER));
	printer->handler = handler;
	printer->hasData = false;
	printer->str = TSMS_UTIL_createCharList(1024);
	printer->callback = TSMS_NULL;
	printer->strBuffer = TSMS_STRING_create();
	printer->customBuffer = TSMS_NULL;
	TSMS_IT_addPrinter(printer, __tsms_internal_callback, TSMS_NULL);
	HAL_UART_Receive_IT(printer->handler,  &printer->buffer, 1);
	return printer;
}

#endif

pString TSMS_PRINTER_get(TSMS_PHP printer) {
	if (printer->hasData) {
		printer->hasData = false;
		return printer->strBuffer;
	}
	return TSMS_NULL;
}

pString TSMS_PRINTER_getBlocking(TSMS_PHP printer) {
	while (!printer->hasData);
	return TSMS_PRINTER_get(printer);
}

pString TSMS_PRINTER_getBlockingCustom(TSMS_PHP printer, pString customBuffer) {
	printer->customBuffer = customBuffer;
	return TSMS_PRINTER_getBlocking(printer);
}

TSMS_RESULT TSMS_PRINTER_print(TSMS_PHP printer, char *str) {
#if defined(TSMS_STM32) && defined(HAL_UART_MODULE_ENABLED)
	HAL_StatusTypeDef status = HAL_UART_Transmit(printer->handler, str, strlen(str),1000);
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

void print(const char * str, ...) {
	if (defaultPrinter == TSMS_NULL)
		return;
	va_list p;
	va_start(p, str);
	vsprintf(stringBuffer, str, p);
	va_end(p);
	TSMS_PRINTER_print(defaultPrinter, stringBuffer);
}

TSMS_RESULT TSMS_PRINTER_setCallback(TSMS_PHP printer, TSMS_PRINTER_CALLBACK callback, void *data) {
	if (printer->callback != TSMS_NULL)
		return TSMS_ERROR;
	printer->callback = callback;
	printer->callbackData = data;
	return TSMS_SUCCESS;
}