#ifndef TSMS_MATH_H
#define TSMS_MATH_H

#include "math.h"
#include "tsms_def.h"

#define TSMS_min min
#define TSMS_max max

#define PI 3.14159265358979323846f

TSMS_LSIZE min(TSMS_LSIZE a, TSMS_LSIZE b);

TSMS_LSIZE max(TSMS_LSIZE a, TSMS_LSIZE b);

bool TSMS_between(TSMS_LSIZE a, TSMS_LSIZE b, TSMS_LSIZE c);

#endif //TSMS_MATH_H
