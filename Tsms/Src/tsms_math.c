#include "tsms_math.h"

TSMS_LSIZE min(TSMS_LSIZE a, TSMS_LSIZE b) {
	return a < b ? a : b;
}

TSMS_LSIZE max(TSMS_LSIZE a, TSMS_LSIZE b) {
	return a > b ? a : b;
}

bool TSMS_between(TSMS_LSIZE a, TSMS_LSIZE b, TSMS_LSIZE c) {
    return a <= b && b <= c;
}