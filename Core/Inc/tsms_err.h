#ifndef TSMS_ERR_H
#define TSMS_ERR_H
#include "tsms_string.h"

typedef enum {
	TSMS_ERR_MALLOC_FAILED = 0,
	TSMS_ERR_TYPE_ERROR,
	TSMS_ERR_TYPE_FATAL
} TSMS_ERR_TYPE;


void TSMS_ERR_report(TSMS_ERR_TYPE type, pString message);

#endif //TSMS_ERR_H
