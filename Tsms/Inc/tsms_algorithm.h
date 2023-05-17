#ifndef TSMS_ALGORITHM_H
#define TSMS_ALGORITHM_H

#include "tsms_def.h"
#include "tsms_function_def.h"

#include "tsms_list.h"
#include "tsms_link_list.h"
#include "tsms_float_link_list.h"

TSMS_RESULT TSMS_ALGORITHM_sort(TSMS_LP list, TSMS_COMPARE_FUNCTION compareFunction);

TSMS_RESULT TSMS_ALGORITHM_sortArray(void** array, TSMS_SIZE size, TSMS_COMPARE_FUNCTION compareFunction);

TSMS_RESULT TSMS_ALGORITHM_sortFloatArray(float * array, TSMS_SIZE size);

TSMS_POS TSMS_ALGORITHM_lowerBoundFloatLinkList(TSMS_FLOAT_LINK_LIST_POINTER list, float value, TSMS_FLKNP * node);

#endif //TSMS_ALGORITHM_H
