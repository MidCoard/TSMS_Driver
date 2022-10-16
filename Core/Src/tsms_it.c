#include "tsms_it.h"
#include "tsms_util.h"

struct TSMS_IT_GPIO {
	TSMS_GHP gpio;
	TSMS_IT_GPIO_TYPE type;
	TSMS_IT_GPIO_CALLBACK callback;
	void* handler;
};

typedef struct TSMS_IT_GPIO * TSMS_IT_GPIO_POINTER;
typedef TSMS_IT_GPIO_POINTER TSMS_IGP;

TSMS_ULP list = TSMS_NULL;

TSMS_RESULT TSMS_IT_init(TSMS_CLOCK_FREQUENCY frequency) {
	list = TSMS_UTIL_createList(10, sizeof(TSMS_IGP));
	return TSMS_SUCCESS;
}

#ifdef TSMS_STM32

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	for (int i = 0;i<list->length;i++) {
		TSMS_IGP gpio = list->list[i];
		if (gpio->gpio->pin == GPIO_Pin)
			gpio->callback(gpio->handler, gpio->gpio);
	}
}

#endif

TSMS_RESULT TSMS_IT_addGPIO(TSMS_GHP gpio, TSMS_IT_GPIO_TYPE type, TSMS_IT_GPIO_CALLBACK callback, void* handler){
	TSMS_IGP igp = malloc(sizeof(struct TSMS_IT_GPIO));
	if (igp == TSMS_NULL)
		return TSMS_ERROR;
	igp->gpio = gpio;
	igp->type = type;
	igp->callback = callback;
	igp->handler = handler;
	TSMS_UTIL_addList(list, igp);
	return TSMS_SUCCESS;
}