#include "tsms_printer.h"

static TSMS_PHP _printerDefaultPrinter = TSMS_NULL;

#if defined(TSMS_STM32) && defined(HAL_UART_MODULE_ENABLED)

TSMS_INLINE void __tsms_internal_callback(void * handler, TSMS_PHP php) {
	if (php->buffer != 0) {
		if (php->buffer == '\n') {
			if (php->customBuffer != TSMS_NULL)
				TSMS_STRING_getString(php->str, php->customBuffer);
			else
				TSMS_STRING_getString(php->str, php->strBuffer);
			php->customBuffer = TSMS_NULL;
			TSMS_CHAR_LIST_clear(php->str);
			if (php->callback != TSMS_NULL)
				php->callback(php->callbackData, php);
			else
				php->hasData = true;
		} else TSMS_CHAR_LIST_add(php->str, php->buffer);
	}
	HAL_UART_Receive_IT(php->handler,  &php->buffer, 1);
}

TSMS_PHP TSMS_PRINTER_createUARTPrinter(UART_HandleTypeDef *handler) {
	TSMS_PHP printer = malloc(sizeof(struct TSMS_PRINTER_HANDLER));
	printer->handler = handler;
	printer->hasData = false;
	printer->str = TSMS_CHAR_LIST_create(1024);
	printer->callback = TSMS_NULL;
	printer->strBuffer = TSMS_STRING_create();
	printer->customBuffer = TSMS_NULL;
	printer->stringBuffer = malloc(1024 * sizeof(char));
	TSMS_IT_addPrinter(printer, __tsms_internal_callback, TSMS_NULL);
	HAL_UART_Receive_IT(printer->handler,  &printer->buffer, 1);
	return printer;
}

#endif

pString TSMS_PRINTER_get(TSMS_PHP printer) {
	if (printer == TSMS_NULL)
		return TSMS_NULL;
	if (printer->hasData) {
		printer->hasData = false;
		return printer->strBuffer;
	}
	return TSMS_NULL;
}

pString TSMS_PRINTER_getBlocking(TSMS_PHP printer) {
	if (printer == TSMS_NULL)
		return TSMS_NULL;
	while (!printer->hasData);
	return TSMS_PRINTER_get(printer);
}

pString TSMS_PRINTER_getBlockingCustom(TSMS_PHP printer, pString customBuffer) {
	if (printer == TSMS_NULL)
		return TSMS_NULL;
	printer->customBuffer = customBuffer;
	return TSMS_PRINTER_getBlocking(printer);
}

TSMS_RESULT TSMS_print(TSMS_PHP printer, char * str) {
#if defined(TSMS_STM32) && defined(HAL_UART_MODULE_ENABLED)
	HAL_StatusTypeDef status = HAL_UART_Transmit(printer->handler, str, strlen(str),1000);
	return status == HAL_OK ? TSMS_SUCCESS : TSMS_ERROR;
#else
	return TSMS_FAIL;
#endif

}

TSMS_RESULT TSMS_println(TSMS_PHP printer, char * str) {
	TSMS_RESULT result = TSMS_print(printer, str);
	return result | TSMS_print(printer, "\n");
}

TSMS_RESULT TSMS_printInt(TSMS_PHP printer, int v) {
	sprintf(printer->stringBuffer, "%d", v);
	return TSMS_print(printer, printer->stringBuffer);
}

TSMS_RESULT TSMS_printIntln(TSMS_PHP printer, int v) {
	TSMS_RESULT result = TSMS_printInt(printer, v);
	return result | TSMS_print(printer, "\n");
}

TSMS_RESULT TSMS_printChar(TSMS_PHP printer, char c) {
	printer->stringBuffer[0] = c;
	printer->stringBuffer[1] = '\0';
	return TSMS_print(printer, printer->stringBuffer);
}

TSMS_RESULT TSMS_printCharln(TSMS_PHP printer, char c) {
	TSMS_RESULT result = TSMS_printChar(printer, c);
	return result | TSMS_print(printer, "\n");
}

TSMS_RESULT TSMS_printFloat(TSMS_PHP printer, float v) {
	sprintf(printer->stringBuffer, "%f", v);
	return TSMS_print(printer, printer->stringBuffer);
}

TSMS_RESULT TSMS_printFloatln(TSMS_PHP printer, float v) {
	TSMS_RESULT result = TSMS_printFloat(printer, v);
	return result | TSMS_print(printer, "\n");
}

TSMS_RESULT TSMS_printf(TSMS_PHP printer, const char *c, ...) {
	va_list p;
	va_start(p, c);
	vsprintf(printer->stringBuffer, c, p);
	va_end(p);
	return TSMS_print(printer, printer->stringBuffer);
}

TSMS_RESULT TSMS_setDefaultPrinter(TSMS_PHP printer) {
	_printerDefaultPrinter = printer;
	return TSMS_SUCCESS;
}

TSMS_RESULT print(const char *str, ...) {
	if (_printerDefaultPrinter == TSMS_NULL)
		return TSMS_ERROR;
	va_list p;
	va_start(p, str);
	vsprintf(_printerDefaultPrinter->stringBuffer, str, p);
	va_end(p);
	TSMS_print(_printerDefaultPrinter, _printerDefaultPrinter->stringBuffer);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_PRINTER_setCallback(TSMS_PHP printer, TSMS_PRINTER_CALLBACK callback, void *data) {
	if (printer == TSMS_NULL)
		return TSMS_ERROR;
	printer->callback = callback;
	printer->callbackData = data;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_PRINTER_release(TSMS_PHP printer) {
	if (printer == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_STRING_release(printer->strBuffer);
	TSMS_STRING_release(printer->customBuffer);
	TSMS_CHAR_LIST_release(printer->str);
	free(printer->stringBuffer);
	free(printer);
	return TSMS_SUCCESS;
}