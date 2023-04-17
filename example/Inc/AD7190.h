#ifndef TSMS_AD7190_H
#define TSMS_AD7190_H

#define AD7190_RESERVE 0
#define AD7190_CREAD 1
#define AD7190_RS 2
#define AD7190_RW 3
#define AD7190_WEN 4
#define AD7190_CHD 5
#define AD7190_PARITY 6
#define AD7190_NOREF 7
#define AD7190_ERR 8
#define AD7190_RDY 9
#define AD7190_FS 10
#define AD7190_REJ60 11
#define AD7190_SINGLE 12
#define AD7190_ENPAR 13
#define AD7190_SINC3 14
#define AD7190_CLK 15
#define AD7190_DAT_STA 16
#define AD7190_MD 17
#define AD7190_G 18
#define AD7190_UB 19
#define AD7190_BUF 20
#define AD7190_REFDET 21
#define AD7190_BURN 22
#define AD7190_CH 23
#define AD7190_REFSEL 24
#define AD7190_CHOP 25
#define AD7190_DATA 26
#define AD7190_ID 27
#define AD7190_P0DAT 28
#define AD7190_P1DAT 29
#define AD7190_P2DAT 30
#define AD7190_P3DAT 31
#define AD7190_GP10EN 32
#define AD7190_GP32EN 33
#define AD7190_BPDSW 34
#define AD7190_OFFSET 35
#define AD7190_FULL_SCALE 36

#define AD7190_CMD_READ 1<<6
#define AD7190_CMD_WRITE 0

#define AD7190_REG_ADDRESS(x) ((x)<<3)

typedef enum {
	AD7190_CURRENT_CHANNEL_0,
	AD7190_CURRENT_CHANNEL_1,
	AD7190_CURRENT_CHANNEL_2,
	AD7190_CURRENT_CHANNEL_3,
	AD7190_CURRENT_CHANNEL_4,
	AD7190_CURRENT_CHANNEL_5,
	AD7190_CURRENT_CHANNEL_6,
	AD7190_CURRENT_CHANNEL_7,
	AD7190_CURRENT_CHANNEL_TEMP = AD7190_CURRENT_CHANNEL_2
} AD7190_CURRENT_CHANNEL;

typedef enum {
	AD7190_CHANNEL_0 = 1,
	AD7190_CHANNEL_1 = 2,
	AD7190_CHANNEL_2 = 4,
	AD7190_CHANNEL_3 = 8,
	AD7190_CHANNEL_4 = 16,
	AD7190_CHANNEL_5 = 32,
	AD7190_CHANNEL_6 = 64,
	AD7190_CHANNEL_7 = 128,
	AD7190_CHANNEL_TEMP = AD7190_CHANNEL_2
} AD7190_CHANNELS;

#define AD7190_UNIPOLAR_MASK 0x01000000
#define AD7190_BIPOLAR_MASK 0x00800000

#include "tsms_driver.h"
#include "tsms_it.h"

typedef enum {
	AD7190_REG_COMM,
	AD7190_REG_STATUS,
	AD7190_REG_MODE,
	AD7190_REG_CONF,
	AD7190_REG_DATA,
	AD7190_REG_ID,
	AD7190_REG_GPOCON,
	AD7190_REG_OFFSET,
	AD7190_REG_FULL_SCALE
} AD7190_REGISTER;
typedef uint8_t AD7190_CLOCK_MODE;
typedef enum {
	AD7190_MODE_CONTINUOUS_CONV,
	AD7190_MODE_SINGLE_CONV,
	AD7190_MODE_IDLE,
	AD7190_MODE_PWR_DOWN,
	AD7190_MODE_CAL_INT_ZERO,
	AD7190_MODE_CAL_INT_FULL,
	AD7190_MODE_CAL_SYS_ZERO,
	AD7190_MODE_CAL_SYS_FULL
} AD7190_MODE;
typedef enum {
	AD7190_GAIN_5V,
	AD7190_GAIN_0_625V = 3,
	AD7190_GAIN_0_3125V,
	AD7190_GAIN_0_15625V,
	AD7190_GAIN_0_078125V,
	AD7190_GAIN_0_0390625V,
} AD7190_GAIN;
typedef enum {
	AD7190_REF_IN1,
	AD7190_REF_IN2
} AD7190_REFERENCE;
typedef enum {
	AD7190_GPIO_P0,
	AD7190_GPIO_P1,
	AD7190_GPIO_P2,
	AD7190_GPIO_P3
} AD7190_GPIO;
typedef enum {
	AD7190_CLOCK_MODE_EXT_CRY,
	AD7190_CLOCK_MODE_EXT_CLK,
	AD7190_CLOCK_MODE_INT_CLK,
	AD7190_CLOCK_MODE_INT_CLK_MCLK2
} AD7190_CHANNEL;
typedef TSMS_GPIO_STATUS AD7190_GPIO_STATUS;

struct AD7190_Handler {
	TSMS_DHP handler;
	float data[8];
	float reference;
	float gain;
};

struct AD7190_Handler *
AD7190_initSoftware(GPIO_TypeDef *sclk, uint16_t sclkPin, GPIO_TypeDef *mosi, uint16_t mosiPin, GPIO_TypeDef *miso,
                    uint16_t misoPin, GPIO_TypeDef *cs, uint16_t csPin, float reference);

void AD7190_enableContinuousMode(struct AD7190_Handler *handler);

void AD7190_disableContinuousMode(struct AD7190_Handler *handler);

void AD7190_enableSingleCycleConversion(struct AD7190_Handler *handler);

void AD7190_disableSingleCycleConversion(struct AD7190_Handler *handler);

void AD7190_enableParityCheck(struct AD7190_Handler *handler);

void AD7190_disableParityCheck(struct AD7190_Handler *handler);

AD7190_CURRENT_CHANNEL AD7190_getCurrentChannel(struct AD7190_Handler *handler);

bool AD7190_isDataReady(struct AD7190_Handler *handler);

bool AD7190_isError(struct AD7190_Handler *handler);

bool AD7190_isReferenceDetected(struct AD7190_Handler *handler);

bool AD7190_isParityError(struct AD7190_Handler *handler);

void AD7190_setFilterDivider(struct AD7190_Handler *handler, uint16_t divider);

void AD7190_enable60HzRejection(struct AD7190_Handler *handler);

void AD7190_disable60HzRejection(struct AD7190_Handler *handler);

void AD7190_enableSin3Filter(struct AD7190_Handler *handler);

void AD7190_disableSin3Filter(struct AD7190_Handler *handler);

void AD7190_setClockMode(struct AD7190_Handler *handler, AD7190_CLOCK_MODE mode);

void AD7190_setMode(struct AD7190_Handler *handler, AD7190_MODE mode);

void AD7190_setGain(struct AD7190_Handler *handler, AD7190_GAIN gain);

bool AD7190_setReference(struct AD7190_Handler *handler, AD7190_REFERENCE reference);

void AD7190_setChannels(struct AD7190_Handler *handler, AD7190_CHANNELS channel);

void AD7190_enableDataStatusRead(struct AD7190_Handler *handler);

void AD7190_disableDataStatusRead(struct AD7190_Handler *handler);

void AD7190_enableBipolar(struct AD7190_Handler *handler);

void AD7190_enableUnipolar(struct AD7190_Handler *handler);

void AD7190_enableBuffer(struct AD7190_Handler *handler);

void AD7190_disableBuffer(struct AD7190_Handler *handler);

void AD7190_enableReferenceDetected(struct AD7190_Handler *handler);

void AD7190_disableReferenceDetected(struct AD7190_Handler *handler);

void AD7190_enableBurnoutCurrent(struct AD7190_Handler *handler);

void AD7190_disableBurnoutCurrent(struct AD7190_Handler *handler);

void AD7190_enableChop(struct AD7190_Handler *handler);

void AD7190_disableChop(struct AD7190_Handler *handler);

void AD7190_openBridgePowerDownSwitch(struct AD7190_Handler *handler);

void AD7190_closeBridgePowerDownSwitch(struct AD7190_Handler *handler);

void AD7190_enableGPIO01(struct AD7190_Handler *handler);

void AD7190_disableGPIO01(struct AD7190_Handler *handler);

void AD7190_enableGPIO23(struct AD7190_Handler *handler);

void AD7190_disableGPIO23(struct AD7190_Handler *handler);

void AD7190_writeGPIO(struct AD7190_Handler *handler, AD7190_GPIO gpio, AD7190_GPIO_STATUS state);

float *AD7190_readData(struct AD7190_Handler *handler);

void AD7190_reset(struct AD7190_Handler *handler);

AD7190_GPIO_STATUS AD7190_readGPIO(struct AD7190_Handler *handler, AD7190_GPIO gpio);

#endif //TSMS_AD7190_H
