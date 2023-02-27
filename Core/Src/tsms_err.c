#include "tsms_err.h"

#ifdef TSMS_ERROR_REPORT

void TSMS_ERR_report(TSMS_ERR_TYPE type, pString message) {
	printf("TSMS ERROR: %d, %s\n", type, message->cStr);
}

#endif