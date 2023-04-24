#include "DAC8831.h"
#include "tsms_spi.h"
#include "tsms_driver.h"

static void DAC8831_writeData(struct DAC8831_Handler *handler, uint16_t data) {
	uint32_t value = data;
	handler->handler->spiWrite(handler->handler->spi, &value, 16, 1);
}

void DAC8831_setAnalogOutput(struct DAC8831_Handler *handler, float out) {
	uint16_t data = out / handler->reference * 65536;
	DAC8831_writeData(handler, data);
}


void DAC8831_setAnalogOutputLater(struct DAC8831_Handler *handler, float out) {
	TSMS_GPIO_write(handler->ldac, TSMS_GPIO_STATUS_HIGH);
	DAC8831_setAnalogOutput(handler, out);
}

void DAC8831_update(struct DAC8831_Handler *handler) {
	TSMS_GPIO_write(handler->ldac, TSMS_GPIO_STATUS_LOW);
}

struct DAC8831_Handler *
DAC8831_initHardware(GPIO_TypeDef *sdi, uint16_t sdiPin, GPIO_TypeDef *sclk, uint16_t sclkPin, GPIO_TypeDef *cs,
                     uint16_t csPin, GPIO_TypeDef *ldac, uint16_t ldacPin, float reference) {
	struct DAC8831_Handler *handler = malloc(sizeof(struct DAC8831_Handler));
	handler->handler = TSMS_DRIVER_createSPIHandler(TSMS_SPI_createSoftwareHandler(
			TSMS_GPIO_createHandler(cs, csPin),
			TSMS_GPIO_createHandler(sclk, sclkPin),
			TSMS_NULL_GHP,
			TSMS_GPIO_createHandler(sdi, sdiPin),
			TSMS_SPI_MODE_0,
			TSMS_GPIO_STATUS_LOW,
			TSMS_TRANSFER_TYPE_MSB
	), TSMS_REG_createList(0));
	handler->ldac = TSMS_GPIO_createHandler(ldac, ldacPin);
	handler->reference = reference;
	TSMS_GPIO_write(handler->ldac, TSMS_GPIO_STATUS_LOW);
	return handler;
}
