#include "tsms_err.h"
#include "tsms_printer.h"

void TSMS_ERR_report(TSMS_ERROR_TYPE type, pString message) {
#ifdef TSMS_ERROR_REPORT
	print("TSMS ERROR: %d, %s\n", type, message != TSMS_NULL ? message->cStr : "");
#endif
}
