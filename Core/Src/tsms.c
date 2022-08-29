#include "tsms.h"

TSMS_RESULT TSMS_init() {
	TSMS_RESULT result = TSMS_SUCCESS;
#ifdef TSMS_IT_H
	result |= TSMS_IT_init();
#endif
	return result;
}