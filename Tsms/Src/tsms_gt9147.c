#include "touch/tsms_gt9147.h"
#include "tsms_gpio.h"
#include "tsms_display.h"
#include "tsms_iic.h"
#include "tsms_printer.h"
#include "tsms_lock.h"
#include "tsms_list.h"

static uint8_t gt9147CommandBuffer[4];

static uint8_t gt9147Buffer[64];

// details show in GT9147_Programmer_Guide
static uint8_t gt9147ConfigTable[] = {
		TSMS_GT9147_REG_CONFIG_ADDRESS >> 8, TSMS_GT9147_REG_CONFIG_ADDRESS & 0xFF, // config address

		0x91, // version
		0xe0, 0x01, // x: 480
		0x20, 0x03, // y: 800
		0x05, // touch number: 5
		0x34, // low 2 bits, 0: INT rising edge, 1: INT falling edge, 2: INT low level, 3: INT high level
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

static uint8_t gt9147ConfigTable0x62[] = {TSMS_GT9147_REG_CONFIG_ADDRESS >> 8,
                                          TSMS_GT9147_REG_CONFIG_ADDRESS & 0xFF, // config address
                                          0x62, 0xe0, 0x1, 0x20, 0x3, 0x5, 0x34, 0xc0, 0x1, 0x8, 0x28, 0xf, 0x50, 0x32,
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

TSMS_INLINE void __tsms_internal_write_register(TSMS_GT9147 gt9147, uint16_t reg, const uint8_t *buffer, uint16_t length) {
	gt9147CommandBuffer[0] = reg >> 8;
	gt9147CommandBuffer[1] = reg & 0xFF;
	for (uint16_t i = 0; i < length; i++)
		gt9147CommandBuffer[i + 2] = buffer[i];
	TSMS_IIC_writeBytes(gt9147->iic, gt9147CommandBuffer, length + 2);
}

TSMS_INLINE void __tsms_internal_read_register(TSMS_GT9147 gt9147, uint16_t reg, uint8_t *buffer, uint16_t length) {
	gt9147CommandBuffer[0] = reg >> 8;
	gt9147CommandBuffer[1] = reg & 0xFF;
	TSMS_IIC_writeBytes(gt9147->iic, gt9147CommandBuffer, 2);
	TSMS_IIC_readBytes(gt9147->iic, buffer, length);
}

TSMS_INLINE uint32_t __tsms_internal_read_id(TSMS_GT9147 gt9147) {
	__tsms_internal_read_register(gt9147, TSMS_GT9147_REG_ID_1_ADDRESS, gt9147Buffer, 4);
	return gt9147Buffer[0] << 24 | gt9147Buffer[1] << 16 | gt9147Buffer[2] << 8 | gt9147Buffer[3];
}

TSMS_INLINE void __tsms_internal_touch_interrupt(void* t, TSMS_GHP gpio) {
	TSMS_THP touch = t;
	TSMS_GT9147 gt9147 = touch->custom;
	__tsms_internal_read_register(gt9147, TSMS_GT9147_REG_STATUS_ADDRESS, gt9147Buffer, 1);
	pLock lock;
	if ((lock = TSMS_SEQUENCE_PRIORITY_LOCK_tryLock(touch->lock, TSMS_NULL, 0)) != TSMS_NULL) {
		if (touch->request) {
			touch->request = false;
			for (TSMS_POS i = 0; i < touch->list->length; i++)
				free(touch->list->list[i]);
			TSMS_LIST_clear(touch->list);
			TSMS_SIZE touchCount = gt9147Buffer[0] & 0x0F;
			if (touchCount > 0) {
				__tsms_internal_read_register(gt9147, TSMS_GT9147_REG_TOUCH_1_X_L_ADDRESS, gt9147Buffer,
				                              touchCount * 8);
				for (TSMS_SIZE i = 0; i < touchCount; i++) {
					TSMS_TDP data = malloc(sizeof(struct TSMS_TOUCH_DATA));
					data->id = gt9147Buffer[i * 8];
					data->x = gt9147Buffer[i * 8 + 1] | (gt9147Buffer[i * 8 + 2] << 8);
					data->y = gt9147Buffer[i * 8 + 3] | (gt9147Buffer[i * 8 + 4] << 8);
					data->size = gt9147Buffer[i * 8 + 5] | (gt9147Buffer[i * 8 + 6] << 8);
					TSMS_LIST_add(touch->list, data);
				}
			}
		}
		TSMS_SEQUENCE_PRIORITY_LOCK_unlock(touch->lock, lock);
	}
	gt9147Buffer[0] = 0;
	__tsms_internal_write_register(gt9147, TSMS_GT9147_REG_STATUS_ADDRESS, gt9147Buffer, 1);
}

uint32_t TSMS_GT9147_readId(TSMS_THP touch) {
	TSMS_GT9147 gt9147 = touch->custom;
	return __tsms_internal_read_id(gt9147);
}

TSMS_GT9147 TSMS_GT9147_createHandler(TSMS_IHP iic, TSMS_GHP interrupt) {
	TSMS_GT9147 gt9147 = malloc(sizeof(struct TSMS_GT9147_HANDLER));
	if (gt9147 == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_GT9147");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	gt9147->iic = iic;
	TSMS_GPIO_setMode(interrupt, TSMS_GPIO_IT_RISING, TSMS_GPIO_PULL_UP);
	gt9147->interrupt = interrupt;
	return gt9147;
}

TSMS_RESULT TSMS_GT9147_init(TSMS_THP touch, void *option) {
	TSMS_GT9147 gt9147 = touch->custom;
	TSMS_GPIO_setMode(gt9147->interrupt, TSMS_GPIO_IT_RISING, TSMS_GPIO_PULL_DOWN);
	TSMS_delay(10);
	__tsms_internal_read_register(gt9147, TSMS_GT9147_REG_CONFIG_ADDRESS, gt9147Buffer, 1);
	if (gt9147Buffer[0] < 0x90) {
		gt9147Buffer[0] = 0;
		gt9147Buffer[1] = 1;
		for (TSMS_POS i = 2; i < 186; i++)
			gt9147Buffer[0] += gt9147ConfigTable[i];
		gt9147Buffer[0] = (~gt9147Buffer[0]) + 1;
		TSMS_IIC_writeBytes(gt9147->iic, gt9147ConfigTable, 186);
		__tsms_internal_write_register(gt9147, TSMS_GT9147_REG_CONFIG_CHECKSUM_ADDRESS, gt9147Buffer, 2);
	}
	TSMS_delay(10);
	TSMS_IT_addGPIO(gt9147->interrupt, __tsms_internal_touch_interrupt, touch);
	return TSMS_SUCCESS;
}