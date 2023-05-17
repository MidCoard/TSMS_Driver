#ifndef TSMS_FILTER_H
#define TSMS_FILTER_H

#define TSMS_EXTEND_WINDOW_FILTER \
    uint32_t windowSize;          \
    float * window;               \
    bool * mask;                  \
    uint32_t windowIndex;         \
    uint32_t windowCount;         \
    TSMS_FILTER_UPDATE update;

typedef struct TSMS_WINDOW_FILTER tWindowFilter;
typedef tWindowFilter * pWindowFilter;

typedef struct TSMS_MEAN_FILTER tMeanFilter;
typedef tMeanFilter * pMeanFilter;

typedef struct TSMS_MEDIAN_FILTER tMedianFilter;
typedef tMedianFilter * pMedianFilter;

typedef struct TSMS_MODE_FILTER tModeFilter;
typedef tModeFilter * pModeFilter;

#include "tsms.h"
#include "tsms_float_link_list.h"

typedef void (*TSMS_FILTER_UPDATE)(pWindowFilter, float, float, bool, bool);

struct TSMS_WINDOW_FILTER {
    TSMS_EXTEND_WINDOW_FILTER
};

struct TSMS_MEDIAN_FILTER {
    TSMS_EXTEND_WINDOW_FILTER
    float median;
	TSMS_FLKLP medianList;
};

struct TSMS_MEAN_FILTER {
    TSMS_EXTEND_WINDOW_FILTER
	float mean;
    float sum;
};

struct TSMS_MODE_FILTER {
	TSMS_EXTEND_WINDOW_FILTER
	float mode;
	TSMS_LP modeList;
};

pMedianFilter TSMS_FILTER_createMedianFilter(uint16_t windowSize);

pMeanFilter TSMS_FILTER_createMeanFilter(uint16_t windowSize);

pModeFilter TSMS_FILTER_createModeFilter(uint16_t windowSize);

TSMS_RESULT TSMS_FILTER_insertWindowFilter(pWindowFilter filter, float value);

TSMS_RESULT TSMS_FILTER_setWindowFilter(pWindowFilter filter, TSMS_POS index, float value);

TSMS_RESULT TSMS_FILTER_removeWindowFilter(pWindowFilter filter, TSMS_POS index);

TSMS_RESULT TSMS_FILTER_releaseWindowFilter(pWindowFilter filter);

TSMS_RESULT TSMS_FILTER_releaseMeanFilter(pMeanFilter filter);

TSMS_RESULT TSMS_FILTER_releaseMedianFilter(pMedianFilter filter);

TSMS_RESULT TSMS_FILTER_releaseModeFilter(pModeFilter filter);

#endif //TSMS_FILTER_H
