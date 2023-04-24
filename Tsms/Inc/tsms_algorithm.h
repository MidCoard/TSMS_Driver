#ifndef TSMS_ALGORITHM_H
#define TSMS_ALGORITHM_H

#include "tsms_def.h"

typedef int (*TSMS_COMPARE_FUNCTION)(void *a, void *b);

TSMS_RESULT TSMS_ALGORITHM_sort(TSMS_LP list, TSMS_COMPARE_FUNCTION compareFunction);

#endif //TSMS_ALGORITHM_H
