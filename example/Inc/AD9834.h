#ifndef TSMS_AD9834_H
#define TSMS_AD9834_H

#define AD9834_RESERVE 0
#define AD9834_MD 1
#define AD9834_DIV2 2
#define AD9834_SIGN_PIB 3
#define AD9834_OPBITEN 4
#define AD9834_SLEEP12 5
#define AD9834_SLEEP1 6
#define AD9834_RESET 7
#define AD9834_PIN_SW 8
#define AD9834_PSEL 9
#define AD9834_FSEL 10
#define AD9834_HLB 11
#define AD9834_B28 12

#define AD9834_FREQ0 13
#define AD9834_FREQ1 14
#define AD9834_PHASE0 15
#define AD9834_PHASE1 16

#define AD9834_REG_CONTROL 0
#define AD9834_REG_FREQ_0 1
#define AD9834_REG_FREQ_1 2
#define AD9834_REG_PHASE_0 3
#define AD9834_REG_PHASE_1 3

#define AD9834_INTERNAL_FREQ (75*1000*1000)
#define AD9834_FREQ_FACTOR (1<<28)
#define AD9834_FREQ_LSB_MASK 0x3FFF
#define AD9834_PHASE_FACTOR (1<<12)


#include "tsms_driver.h"
#include "tsms_math.h"

typedef enum {
	AD9834_MODE_SINE_WAVE = 0,
	AD9834_MODE_TRI_WAVE = 1
} AD9834_MODE;

typedef enum {
	AD9834_SIGN_MODE_DISABLE = 0,
	AD9834_SIGN_MODE_DAC_MSB_2,
	AD9834_SIGN_MODE_DAC_MSB,
	AD9834_SIGN_MODE_COMPARE_OUT
} AD9834_SIGN_MODE;

typedef enum {
	AD9834_POWER_MODE_NO_POWER_DOWN = 0,
	AD9834_POWER_MODE_DAC_POWER_DOWN,
	AD9834_POWER_MODE_INTERNAL_CLOCK_POWER_DOWN,
	AD9834_POWER_MODE_BOTH_POWER_DOWN
} AD9834_POWER_MODE;

typedef enum {
	AD9834_CHANNEL_0 = 0,
	AD9834_CHANNEL_1,
} AD9834_CHANNEL;

struct AD9834_Handler {
	TSMS_DHP handler;
	bool pinSwitch;
	TSMS_GHP fsync;
	TSMS_GHP fs;
	TSMS_GHP ps;
	TSMS_GHP reset;
	TSMS_GHP sleep;
	float freq;
};

struct AD9834_Handler* AD9834_initHardware(TSMS_GHP sclk, TSMS_GHP sda, TSMS_GHP fsync, TSMS_GHP fs, TSMS_GHP ps, TSMS_GHP reset,
                         TSMS_GHP sleep, float freq, bool pinSwitch);

void AD9834_setSignOutputMode(struct AD9834_Handler* handler, AD9834_SIGN_MODE mode);

void AD9834_setOutputMode(struct AD9834_Handler* handler, AD9834_MODE mode);

void AD9834_setPhase0(struct AD9834_Handler* handler, float phase);

void AD9834_setPhase1(struct AD9834_Handler* handler, float phase);

void AD9834_setFrequencyLSB0(struct AD9834_Handler* handler, float frequency);

void AD9834_setFrequencyLSB1(struct AD9834_Handler* handler, float frequency);

void AD9834_setFrequencyMSB0(struct AD9834_Handler* handler, float frequency);

void AD9834_setFrequencyMSB1(struct AD9834_Handler* handler, float frequency);

void AD9834_setFrequency0(struct AD9834_Handler* handler, float frequency);

void AD9834_setFrequency1(struct AD9834_Handler* handler, float frequency);

void AD9834_setFrequency(struct AD9834_Handler* handler, float frequency0, float frequency1);

void AD9834_setOutputChannel(struct AD9834_Handler* handler, AD9834_CHANNEL frequencyChannel, AD9834_CHANNEL phaseChannel);

TSMS_RESULT AD9834_setPowerMode(struct AD9834_Handler *handler, AD9834_POWER_MODE mode);

#endif //TSMS_AD9834_H
