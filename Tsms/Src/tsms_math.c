#include "tsms_math.h"

TSMS_LSIZE min(TSMS_LSIZE a, TSMS_LSIZE b) {
	return a < b ? a : b;
}

TSMS_LSIZE max(TSMS_LSIZE a, TSMS_LSIZE b) {
	return a > b ? a : b;
}