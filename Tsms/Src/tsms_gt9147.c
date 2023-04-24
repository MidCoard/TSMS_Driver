#include "touch/tsms_gt9147.h"

static uint8_t _gt9147CommandBuffer[4];

static uint8_t _gt9147Buffer[200];

// details show in GT9147_Programmer_Guide
uint8_t TSMS_GT9147_CUSTOM_CFG_TBL[] = {
		TSMS_GT9147_REG_CONFIG_ADDRESS >> 8, TSMS_GT9147_REG_CONFIG_ADDRESS & 0xFF, // config address

		0x92, // version
		0xe0, 0x01, // x: 480
		0x20, 0x03, // y: 800
		0x05, // touch number: 5
		0x36, // low 2 bits, 0: INT rising edge, 1: INT falling edge, 2: INT low level, 3: INT high level
		// 4 bit: high: change x 2 y, low: no change

		0x00,
		0x01, // shake count : high 4 bits press up, low 4 bits press down
		0x08,
		0x28,
		0x05,
		0x50, 0x32,
		0x03, // low power mode countdown time (s)
		0x05, // refresh rate (ms) (5 + n)
		0x00, 0x00,
		0xff, 0xff, // gesture switch
		0x00, 0x00, // space
		0x00,
		0x00, 0x00, 0x00, 0x00,
		0x89, 0x28,
		0x0a,
		0x17, 0x15, // frequency factor
		0x31, 0x0d, // frequency
		0x00,
		0x00,
		0x02, 0x9b, 0x03, 0x25,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00,
		0x00, 0x0f, 0x94, 0x94, 0xc5, 0x02, 0x07, 0x00, 0x00, 0x04,
		0x8d, 0x13, 0x00, 0x5c, 0x1e, 0x00, 0x3c, 0x30, 0x00, 0x29,
		0x4c, 0x00, 0x1e, 0x78, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x12, 0x14, 0x16,
		0x18, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0x00, 0x02, 0x04, 0x05, 0x06, 0x08, 0x0a, 0x0c,
		0x0e, 0x1d, 0x1e, 0x1f, 0x20, 0x22, 0x24, 0x28, 0x29, 0xff,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff,
};

uint8_t TSMS_GT9147_CUSTOM_CFG_TBL_0x62[] = {TSMS_GT9147_REG_CONFIG_ADDRESS >> 8,
                                          TSMS_GT9147_REG_CONFIG_ADDRESS & 0xFF, // config address
                                          0x62,
                                             0xe0, 0x1, 0x20, 0x3,
                                             0x5, 0x34, 0xc0, 0x1, 0x8, 0x28, 0xf, 0x50, 0x32,
                                             0x3, 0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0x16, 0x16, 0x1f, 0x14, 0x89,
                                             0x28, 0xa, 0x17, 0x15, 0x31, 0xd, 0x0, 0x0, 0x8, 0x22, 0x4, 0x11, 0x0, 0x0,
                                             0x0, 0x0, 0x0, 0x3, 0x82, 0x8, 0x8, 0x0, 0x0, 0xf, 0x2c, 0x94, 0xc5, 0x2, 0x7,
                                             0x0, 0x0, 0x4, 0x9d, 0x10, 0x0, 0x84, 0x14, 0x0, 0x70, 0x19, 0x0, 0x5f, 0x20,
                                             0x0, 0x55, 0x27, 0x0, 0x54, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                             0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                             0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x1a, 0x18, 0x16, 0x14,
                                             0x12, 0x10, 0xe, 0xc, 0xa, 0x8, 0x0, 0x0, 0x0, 0x0, 0x1f, 0xff, 0xff, 0xff,
                                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0,
                                             0x2, 0x4, 0x5, 0x6, 0x8, 0xa, 0xc, 0xe, 0x1d, 0x1e, 0x1f, 0x20, 0x22, 0x24,
                                             0x28, 0x29, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff,
                                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf6, 0xff, 0xff, 0xff, 0xff
};

uint8_t TSMS_GT9147_CUSTOM_CFG_TBL_0x60[] = {
		TSMS_GT9147_REG_CONFIG_ADDRESS >> 8,
		TSMS_GT9147_REG_CONFIG_ADDRESS & 0xFF, // config address
		0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
		0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
		0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
		0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
		0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
		0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
		0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
		0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
		0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
		0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
		0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF
};

TSMS_INLINE void __tsms_internal_write_register(TSMS_GT9147 gt9147, uint16_t reg, const uint8_t *buffer, uint16_t length) {
	_gt9147CommandBuffer[0] = reg >> 8;
	_gt9147CommandBuffer[1] = reg & 0xFF;
	for (uint16_t i = 0; i < length; i++)
		_gt9147CommandBuffer[i + 2] = buffer[i];
	TSMS_IIC_writeBytes(gt9147->iic, _gt9147CommandBuffer, length + 2);
}

TSMS_INLINE void __tsms_internal_read_register(TSMS_GT9147 gt9147, uint16_t reg, uint8_t *buffer, uint16_t length) {
	_gt9147CommandBuffer[0] = reg >> 8;
	_gt9147CommandBuffer[1] = reg & 0xFF;
	TSMS_IIC_writeBytes(gt9147->iic, _gt9147CommandBuffer, 2);
	TSMS_IIC_readBytes(gt9147->iic, buffer, length);
}

TSMS_INLINE uint32_t __tsms_internal_read_id(TSMS_GT9147 gt9147) {
	__tsms_internal_read_register(gt9147, TSMS_GT9147_REG_ID_ADDRESS, _gt9147Buffer, 4);
	return _gt9147Buffer[0] << 24 | _gt9147Buffer[1] << 16 | _gt9147Buffer[2] << 8 | _gt9147Buffer[3];
}

TSMS_INLINE void __tsms_internal_touch_request(TSMS_THP touch, pLock preLock) {
	TSMS_GT9147 gt9147 = (TSMS_GT9147) touch->custom;
	pLock lock;
	if ((lock = TSMS_SEQUENCE_PRIORITY_LOCK_tryLock(touch->lock, preLock, 0)) != TSMS_NULL) {
		__tsms_internal_read_register(gt9147, TSMS_GT9147_REG_STATUS_ADDRESS, _gt9147Buffer, 1);
		for (TSMS_POS i = 0; i < touch->list->length; i++)
			free(touch->list->list[i]);
		TSMS_LIST_clear(touch->list);
		TSMS_SIZE touchCount = _gt9147Buffer[0] & 0x0F;
		if ((_gt9147Buffer[0] & 0x80) != 0 && touchCount > 0 ) {
			__tsms_internal_read_register(gt9147, 0x814f, _gt9147Buffer,
			                              touchCount * 8);
			for (TSMS_SIZE i = 0; i < touchCount; i++) {
				uint16_t id = _gt9147Buffer[i * 8];
				uint16_t x = _gt9147Buffer[i * 8 + 1] | (_gt9147Buffer[i * 8 + 2] << 8);
				uint16_t y = _gt9147Buffer[i * 8 + 3] | (_gt9147Buffer[i * 8 + 4] << 8);
				uint16_t size = _gt9147Buffer[i * 8 + 5] | (_gt9147Buffer[i * 8 + 6] << 8);
				if (touch->display->screen->displayDirection == TSMS_DISPLAY_DIRECTION_HORIZONTAL) {
					TSMS_UTIL_swapUnsignedShort(&x, &y);
					x = touch->display->screen->width - x;
				}
				bool flag = false;
				if (touch->callback != TSMS_NULL)
					flag = touch->callback(touch, id, x, y, size, touch->handler);
				if (!flag) {
					TSMS_TDP data = malloc(sizeof(struct TSMS_TOUCH_DATA));
					data->id = id;
					data->x = x;
					data->y = y;
					data->size = size;
					TSMS_LIST_add(touch->list, data);
				}
			}
		}
		TSMS_SEQUENCE_PRIORITY_LOCK_unlock(touch->lock, lock);
	}
	_gt9147Buffer[0] = 0;
	__tsms_internal_write_register(gt9147, TSMS_GT9147_REG_STATUS_ADDRESS, _gt9147Buffer, 1);
}

TSMS_INLINE void __tsms_internal_auto_request_both(void* t, TSMS_GHP gpio) {
	TSMS_THP touch = t;
	if (touch->requestFlag) {
		pLock lock;
		if ((lock = TSMS_SEQUENCE_PRIORITY_LOCK_tryLock(touch->lock, TSMS_NULL, 0)) != TSMS_NULL) {
			if (touch->requestFlag) {
				__tsms_internal_touch_request(touch, lock);
				touch->requestFlag = false;
			}
			TSMS_SEQUENCE_PRIORITY_LOCK_unlock(touch->lock, lock);
		}
	}
}

TSMS_INLINE void __tsms_internal_auto_request(void* t, TSMS_GHP gpio) {
	TSMS_THP touch = t;
	TSMS_GT9147 gt9147 = touch->custom;
	__tsms_internal_touch_request(touch, TSMS_NULL);
	_gt9147Buffer[0] = 0;
	__tsms_internal_write_register(gt9147, TSMS_GT9147_REG_STATUS_ADDRESS, _gt9147Buffer, 1);
}

TSMS_RESULT TSMS_GT9147_setRequestMode(TSMS_THP touch, TSMS_TOUCH_REQUEST_MODE mode) {
	TSMS_GT9147 gt9147 = touch->custom;
	if (mode == TSMS_TOUCH_REQUEST_MODE_SYNC_AND_IT)
		return TSMS_IT_addGPIO(gt9147->interrupt, __tsms_internal_auto_request_both, touch);
	else if (mode == TSMS_TOUCH_REQUEST_MODE_IT)
		return TSMS_IT_addGPIO(gt9147->interrupt, __tsms_internal_auto_request, touch);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_GT9147_request(TSMS_THP touch, TSMS_TOUCH_REQUEST_MODE mode) {
	if (mode == TSMS_TOUCH_REQUEST_MODE_SYNC_AND_IT) {
		if (touch->requestFlag)
			return TSMS_FAIL;
		pLock lock;
		if ((lock = TSMS_SEQUENCE_PRIORITY_LOCK_tryLock(touch->lock, TSMS_NULL, 0)) != TSMS_NULL) {
			touch->requestFlag = true;
			TSMS_SEQUENCE_PRIORITY_LOCK_unlock(touch->lock, lock);
		} else return TSMS_FAIL;
	} else if (mode == TSMS_TOUCH_REQUEST_MODE_SYNC)
		__tsms_internal_touch_request(touch, TSMS_NULL);
	return TSMS_SUCCESS;
}

uint32_t TSMS_GT9147_readId(TSMS_THP touch) {
	TSMS_GT9147 gt9147 = touch->custom;
	return __tsms_internal_read_id(gt9147);
}

TSMS_GT9147 TSMS_GT9147_createHandler(TSMS_IHP iic, TSMS_GHP interrupt) {
	TSMS_GT9147 gt9147 = malloc(sizeof(struct TSMS_GT9147_HANDLER));
	if (gt9147 == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_GT9147");
		TSMS_ERR_report(TSMS_ERROR_TYPE_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	gt9147->iic = iic;
	TSMS_GPIO_setMode(interrupt, TSMS_GPIO_MODE_IT_RISING, TSMS_GPIO_PULL_UP);
	gt9147->interrupt = interrupt;
	return gt9147;
}

TSMS_RESULT TSMS_GT9147_init(TSMS_THP touch, void * op) {
	if (op == TSMS_NULL)
		return TSMS_FAIL;
	TSMS_GT9147 gt9147 = touch->custom;
	TSMS_GPIO_setMode(gt9147->interrupt, TSMS_GPIO_MODE_IT_RISING, TSMS_GPIO_PULL_NONE);
	TSMS_delay(50);
	__tsms_internal_read_register(gt9147, TSMS_GT9147_REG_CONFIG_ADDRESS, _gt9147Buffer, 184);
	uint8_t * option = op;
	int cmp = memcmp(_gt9147Buffer + 1, option + 3, 183);
	if (_gt9147Buffer[0] != option[2] && cmp != 0) {
		if (option[2] < _gt9147Buffer[0])
			option[2] = _gt9147Buffer[0] + 1;
		_gt9147Buffer[0] = 0;
		_gt9147Buffer[1] = 1;
		for (TSMS_POS i = 2; i < 186; i++)
			_gt9147Buffer[0] += option[i];
		_gt9147Buffer[0] = (~_gt9147Buffer[0]) + 1;
		TSMS_IIC_writeBytes(gt9147->iic, option, 186);
		__tsms_internal_write_register(gt9147, TSMS_GT9147_REG_CONFIG_CHECKSUM_ADDRESS, _gt9147Buffer, 2);
		TSMS_delay(50);
		__tsms_internal_read_register(gt9147, TSMS_GT9147_REG_CONFIG_ADDRESS, _gt9147Buffer, 1);
		if (_gt9147Buffer[0] != option[2]) {
			tString temp = TSMS_STRING_temp("GT9147 init failed");
			TSMS_ERR_report(TSMS_ERROR_TYPE_INIT_FAILED, &temp);
			return TSMS_FAIL;
		}
	}
	return TSMS_SUCCESS;
}