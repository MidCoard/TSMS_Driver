#include "tsms_filter.h"
#include "tsms_float_list.h"
#include "tsms_algorithm.h"

struct _modeInfo {
	float value;
	TSMS_SIZE count;
};

TSMS_INLINE void __tsms_internal_update_median_filter(pWindowFilter filter, float oldValue, float newValue, bool oldMask, bool newMask) {
	pMedianFilter medianFilter = filter;
	if (oldMask)
		TSMS_FLOAT_LINK_LIST_removeElement(medianFilter->medianList, oldValue);
	TSMS_POS index = TSMS_ALGORITHM_lowerBoundFloatLinkList(medianFilter->medianList, newValue, TSMS_NULL);
	if (index == -1)
		index = medianFilter->medianList->length;
	TSMS_FLOAT_LINK_LIST_insert(medianFilter->medianList, newValue, index);
	if (medianFilter->windowCount % 2 == 0) {
		TSMS_FLKNP node = medianFilter->medianList->head;
		for (TSMS_POS i = 0; i < medianFilter->windowCount / 2 - 1; i++)
			node = node->next;
		medianFilter->median = (node->element + node->next->element) / 2;
	} else {
		TSMS_FLKNP node = medianFilter->medianList->head;
		for (TSMS_POS i = 0; i < medianFilter->windowCount / 2; i++)
			node = node->next;
		medianFilter->median = node->element;
	}
}

TSMS_INLINE void __tsms_internal_update_mean_filter(pWindowFilter filter, float oldValue, float newValue, bool oldMask, bool newMask) {
    pMeanFilter meanFilter = filter;
    meanFilter->sum += newValue - oldValue;
	meanFilter->mean = meanFilter->sum / meanFilter->windowCount;
}

TSMS_INLINE void __tsms_internal_update_mode_filter(pWindowFilter filter, float oldValue, float newValue, bool oldMask, bool newMask) {
	pModeFilter modeFilter = filter;
	bool flag = false;
	for (TSMS_SIZE i = 0; i < modeFilter->modeList->length; i++) {
		struct _modeInfo * modeInfo = modeFilter->modeList->list[i];
		if (oldMask && modeInfo->value == oldValue)
			modeInfo->count--;
		if (newMask && modeInfo->value == newValue) {
			modeInfo->count++;
			flag = true;
		}
	}
	if (!flag) {
		struct _modeInfo * modeInfo = (struct _modeInfo *) TSMS_malloc(sizeof(struct _modeInfo));
		if (modeInfo == TSMS_NULL)
			return;
		modeInfo->value = newValue;
		modeInfo->count = 1;
		TSMS_LIST_add(modeFilter->modeList, modeInfo);
	}
	TSMS_SIZE maxCount = 0;
	TSMS_FLP maxCountValueList = TSMS_FLOAT_LIST_create(filter->windowCount);
	for (TSMS_SIZE i = 0; i < modeFilter->modeList->length;) {
		struct _modeInfo * modeInfo = modeFilter->modeList->list[i];
		if (modeInfo->count > maxCount) {
			maxCount = modeInfo->count;
			TSMS_FLOAT_LIST_clear(maxCountValueList);
			TSMS_FLOAT_LIST_add(maxCountValueList, modeInfo->value);
		} else if (modeInfo->count == maxCount)
			TSMS_FLOAT_LIST_add(maxCountValueList, modeInfo->value);
		if (modeInfo->count == 0)
			TSMS_LIST_remove(modeFilter->modeList, i);
		else i++;
	}
	float sum = 0;
	for (TSMS_SIZE i = 0; i < maxCountValueList->length; i++)
		sum += maxCountValueList->list[i];
	modeFilter->mode = sum / maxCountValueList->length;
	TSMS_FLOAT_LIST_release(maxCountValueList);
}

pMedianFilter TSMS_FILTER_createMedianFilter(uint16_t windowSize) {
    pMedianFilter filter = (pMedianFilter) TSMS_malloc(sizeof(tMedianFilter));
    if (filter == TSMS_NULL)
        return TSMS_NULL;
    filter->window = (float *)TSMS_malloc(sizeof(float) * windowSize);
    filter->mask = (bool *)TSMS_malloc(sizeof(bool) * windowSize);
	filter->medianList = TSMS_FLOAT_LINK_LIST_create();
	if (filter->medianList == TSMS_NULL || filter->window == TSMS_NULL || filter->mask == TSMS_NULL) {
		TSMS_FILTER_releaseMedianFilter(filter);
		return TSMS_NULL;
	}
    for (uint16_t i = 0; i < windowSize; i++) {
        filter->window[i] = 0.0f;
        filter->mask[i] = false;
    }
    filter->windowSize = windowSize;
    filter->windowIndex = 0;
	filter->windowCount = 0;
    filter->update = __tsms_internal_update_median_filter;
	filter->median = 0.0f;
    return filter;
}

pMeanFilter TSMS_FILTER_createMeanFilter(uint16_t windowSize) {
    pMeanFilter filter = (pMeanFilter)TSMS_malloc(sizeof(tMeanFilter));
    if (filter == TSMS_NULL)
        return TSMS_NULL;
    filter->window = (float *)TSMS_malloc(sizeof(float) * windowSize);
    filter->mask = (bool *)TSMS_malloc(sizeof(bool) * windowSize);
    if (filter->mask == TSMS_NULL || filter->window == TSMS_NULL) {
	    TSMS_FILTER_releaseMeanFilter(filter);
        return TSMS_NULL;
    }
    for (uint16_t i = 0; i < windowSize; i++) {
        filter->window[i] = 0.0f;
        filter->mask[i] = false;
    }
    filter->windowSize = windowSize;
    filter->windowIndex = 0;
	filter->windowCount = 0;
    filter->update = __tsms_internal_update_mean_filter;
	filter->mean = 0.0f;
	filter->sum = 0;
    return filter;
}

pModeFilter TSMS_FILTER_createModeFilter(uint16_t windowSize) {
	pModeFilter filter = (pModeFilter)TSMS_malloc(sizeof(tModeFilter));
	if (filter == TSMS_NULL)
		return TSMS_NULL;
	filter->window = (float *)TSMS_malloc(sizeof(float) * windowSize);
	filter->mask = (bool *)TSMS_malloc(sizeof(bool) * windowSize);
	filter->modeList = TSMS_LIST_create(windowSize);
	if (filter->modeList == TSMS_NULL || filter->window == TSMS_NULL || filter->mask == TSMS_NULL) {
		TSMS_FILTER_releaseModeFilter(filter);
		return TSMS_NULL;
	}
	for (uint16_t i = 0; i < windowSize; i++) {
		filter->window[i] = 0.0f;
		filter->mask[i] = false;
	}
	filter->windowSize = windowSize;
	filter->windowIndex = 0;
	filter->windowCount = 0;
	filter->update = __tsms_internal_update_mode_filter;
	filter->mode = 0.0f;
	return filter;
}

TSMS_RESULT TSMS_FILTER_insertWindowFilter(pWindowFilter filter, float value) {
    if (filter == TSMS_NULL)
        return TSMS_ERROR;
	if (!filter->mask[filter->windowIndex])
		filter->windowCount++;
	filter->update(filter, filter->window[filter->windowIndex], value, filter->mask[filter->windowIndex], true);
    filter->mask[filter->windowIndex] = true;
    filter->window[filter->windowIndex] = value;
    filter->windowIndex++;
    if (filter->windowIndex >= filter->windowSize)
        filter->windowIndex = 0;
    return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_FILTER_setWindowFilter(pWindowFilter filter, TSMS_POS index, float value) {
    if (filter == TSMS_NULL)
        return TSMS_ERROR;
    if (index >= filter->windowSize)
        return TSMS_ERROR;
	if (!filter->mask[index])
		filter->windowCount++;
	filter->update(filter, filter->window[index], value, filter->mask[index], true);
    filter->mask[index] = true;
    filter->window[index] = value;
    return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_FILTER_removeWindowFilter(pWindowFilter filter, TSMS_POS index) {
    if (filter == TSMS_NULL)
        return TSMS_ERROR;
    if (index >= filter->windowSize)
        return TSMS_ERROR;
	if (filter->mask[index])
		filter->windowCount--;
    filter->update(filter, filter->window[index], 0.0f, filter->mask[index], false);
	filter->mask[index] = false;
    filter->window[index] = 0.0f;
    return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_FILTER_releaseWindowFilter(pWindowFilter filter) {
    if (filter == TSMS_NULL)
        return TSMS_ERROR;
    free(filter->window);
    free(filter->mask);
    free(filter);
    return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_FILTER_releaseMeanFilter(pMeanFilter filter) {
	if (filter == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_FILTER_releaseWindowFilter((pWindowFilter)filter);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_FILTER_releaseMedianFilter(pMedianFilter filter) {
	if (filter == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_FLOAT_LINK_LIST_release(filter->medianList);
	TSMS_FILTER_releaseWindowFilter((pWindowFilter)filter);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_FILTER_releaseModeFilter(pModeFilter filter) {
	if (filter == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LIST_release(filter->modeList);
	TSMS_FILTER_releaseWindowFilter((pWindowFilter)filter);
	return TSMS_SUCCESS;
}