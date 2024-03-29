#ifndef TSMS_ERR_H
#define TSMS_ERR_H

typedef enum {
	TSMS_ERROR_TYPE_MALLOC_FAILED = 0,
	TSMS_ERROR_TYPE_INIT_FAILED,
	TSMS_ERROR_TYPE_REALLOC_FAILED,
	TSMS_ERROR_TYPE_SCREEN_TYPE_NOT_SUPPORTED,
	TSMS_ERROR_TYPE_TOUCH_TYPE_NOT_SUPPORTED,
	TSMS_ERROR_TYPE_DISPLAY_REFRESH_TOO_SLOW
} TSMS_ERROR_TYPE;

#include "tsms_string.h"

void TSMS_ERR_report(TSMS_ERROR_TYPE type, pString message);

#endif //TSMS_ERR_H
