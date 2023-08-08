#include "AD9834.h"

uint32_t frequency0 = AD9834_FREQ_RESERVED_VALUE;
uint32_t frequency1 = AD9834_FREQ_RESERVED_VALUE;
uint32_t baseFrequency = AD9834_INTERNAL_FREQ;

uint16_t controlRegisterValue = 0;

static void addAndSet(uint16_t offset);

static void removeAndSet(uint16_t mask);

static void addAndTempAndSet(uint16_t offset, uint16_t temp);

static void addAndRemoveAndTempAndSet(uint16_t add, uint16_t remove, uint16_t temp);

static void set();

static void addAndRemoveAndSet(uint16_t offset, uint16_t mask);

static void tempAndSet(uint16_t temp);


static void AD9834_frequencySelect(struct AD9834_Handler *handler, uint8_t status) {
	if (handler->pinSwitch)
		TSMS_GPIO_write(handler->fs, status);
	else if (status)
		addAndSet(AD9834_FS_1);
	else removeAndSet(AD9834_FS_0_MASK);
}

static void AD9834_phaseSelect(struct AD9834_Handler *handler, uint8_t status) {
	if (handler->pinSwitch)
		TSMS_GPIO_write(handler->ps, status);
	else if (status)
		addAndSet(AD9834_PS_1);
	else removeAndSet(AD9834_PS_0_MASK);
}

static void AD9834_reset(struct AD9834_Handler *handler, TSMS_GPIO_STATUS status) {
	if (handler->pinSwitch)
		TSMS_GPIO_write(handler->reset, status);
	else if (status)
		addAndTempAndSet(0, AD9834_RESET_ENABLE);
}

static void AD9834_writeControlRegister(struct AD9834_Handler *handler, uint16_t data) {
	TSMS_GPIO_write(handler->fsync, TSMS_GPIO_STATUS_LOW);
	handler->handler->spiWrite(handler->handler->spi, &data, 16, 1);
	TSMS_GPIO_write(handler->fsync, TSMS_GPIO_STATUS_HIGH);
}

static void addAndRemoveAndTempAndSet(uint16_t add, uint16_t remove, uint16_t temp) {
	controlRegisterValue |= add;
	controlRegisterValue &= remove;
	AD9834_writeControlRegister(controlRegisterValue | temp);
}

static void addAndTempAndSet(uint16_t offset, uint16_t temp) {
	addAndRemoveAndTempAndSet(offset, AD9834_MASK, temp);
}

static void addAndSet(uint16_t offset) {
	addAndTempAndSet(offset, 0);
}

static void addAndRemoveAndSet(uint16_t offset, uint16_t mask) {
	addAndRemoveAndTempAndSet(offset, mask, 0);
}

static void removeAndSet(uint16_t mask) {
	addAndRemoveAndSet(0, mask);
}

static void set() {
	addAndSet(0);
}

static void tempAndSet(uint16_t temp) {
	addAndTempAndSet(0, temp);
}

//pin_sw_bit: AD9834_PIN_ENABLE or AD9834_SW_ENABLE
void AD9834_setPinFlag(uint16_t pin_sw_bit) {
	if (pin_sw_bit)
		addAndSet(pin_sw_bit);
	else removeAndSet(AD9834_SW_ENABLE_MASK);
}

//spi mode 2
//pinSwitch: AD9834_PIN_ENABLE or AD9834_SW_ENABLE
struct AD9834_Handler *
AD9834_initHardware(TSMS_GHP sclk, TSMS_GHP sda, TSMS_GHP fsync, TSMS_GHP fs, TSMS_GHP ps, TSMS_GHP reset,
                    TSMS_GHP sleep, bool pinSwitch) {
	struct AD9834_Handler *handler = TSMS_malloc(sizeof(struct AD9834_Handler));
	handler->handler = TSMS_DRIVER_createSPIHandler(
			TSMS_SPI_createSoftwareHandler(TSMS_NULL_GHP, sclk, TSMS_NULL_GHP, sda, TSMS_SPI_MODE_2, false,
			                               TSMS_TRANSFER_TYPE_MSB),
			TSMS_REG_createList(1, TSMS_REG_16BitRegister(

			)));
	handler->pinSwitch = pinSwitch;
	handler->fsync = fsync;
	handler->fs = fs;
	handler->ps = ps;
	handler->reset = reset;
	handler->sleep = sleep;
	AD9834_reset(handler, TSMS_GPIO_STATUS_HIGH);
	TSMS_delay(1);
	AD9834_reset(handler, TSMS_GPIO_STATUS_LOW);
	TSMS_GPIO_write(handler->fsync, TSMS_GPIO_STATUS_HIGH);
	if (!pinSwitch)
		tempAndSet(AD9834_RESET_ENABLE);
	else addAndSet(AD9834_PIN_ENABLE);
	TSMS_delay(1);
	if (!pinSwitch)
		set();
	return handler;
}

//internal
void AD9834_setFreq(float freq0, float freq1) {
	baseFrequency = AD9834_INTERNAL_FREQ;
	if (controlRegisterValue & AD9834_SLEEP1_ENABLE)
		removeAndSet(AD9834_SLEEP1_DISABLE_MASK);
	if (!(controlRegisterValue & AD9834_B28_ENABLE))
		addAndSet(AD9834_B28_ENABLE);
	frequency0 = freq0 / AD9834_INTERNAL_FREQ * AD9834_FREQ_FACTOR;
	uint16_t freq0_28_lsb = AD9834_FREQ_0_REGISTER_PREFIX | (frequency0 & AD9834_FREQ_LSB_MASK);
	uint16_t freq0_28_msb = AD9834_FREQ_0_REGISTER_PREFIX | (AD9834_FREQ_MSB_OFFSET(frequency0));
	frequency1 = freq1 / AD9834_INTERNAL_FREQ * AD9834_FREQ_FACTOR;
	uint16_t freq1_28_lsb = AD9834_FREQ_1_REGISTER_PREFIX | (frequency1 & AD9834_FREQ_LSB_MASK);
	uint16_t freq1_28_msb = AD9834_FREQ_1_REGISTER_PREFIX | (AD9834_FREQ_MSB_OFFSET(frequency1));
	AD9834_writeControlRegister(freq0_28_lsb);
	AD9834_writeControlRegister(freq0_28_msb);
	AD9834_writeControlRegister(freq1_28_lsb);
	AD9834_writeControlRegister(freq1_28_msb);
}

void AD9834_setFreqExternal(float freq0, float freq1, float base) {
	baseFrequency = base;
	if (!(controlRegisterValue & AD9834_SLEEP1_ENABLE))
		addAndSet(AD9834_SLEEP1_ENABLE);
	if (!(controlRegisterValue & AD9834_B28_ENABLE))
		addAndSet(AD9834_B28_ENABLE);
	frequency0 = freq0 / base * AD9834_FREQ_FACTOR;
	uint16_t freq0_28_lsb = AD9834_FREQ_0_REGISTER_PREFIX | (frequency0 & AD9834_FREQ_LSB_MASK);
	uint16_t freq0_28_msb = AD9834_FREQ_0_REGISTER_PREFIX | (AD9834_FREQ_MSB_OFFSET(frequency0));
	frequency1 = freq1 / base * AD9834_FREQ_FACTOR;
	uint16_t freq1_28_lsb = AD9834_FREQ_1_REGISTER_PREFIX | (frequency1 & AD9834_FREQ_LSB_MASK);
	uint16_t freq1_28_msb = AD9834_FREQ_1_REGISTER_PREFIX | (AD9834_FREQ_MSB_OFFSET(frequency1));
	AD9834_writeControlRegister(freq0_28_lsb);
	AD9834_writeControlRegister(freq0_28_msb);
	AD9834_writeControlRegister(freq1_28_lsb);
	AD9834_writeControlRegister(freq1_28_msb);
}

// not recommended by datasheet
// target: AD9834_FREQ_0 or AD9834_FREQ_1
void AD9834_setFreq2(uint8_t target, float freq) {
	baseFrequency = AD9834_INTERNAL_FREQ;
	if (controlRegisterValue & AD9834_SLEEP1_ENABLE)
		removeAndSet(AD9834_SLEEP1_DISABLE_MASK);
	if (!(controlRegisterValue & AD9834_B28_ENABLE))
		addAndSet(AD9834_B28_ENABLE);
	uint16_t prefix;
	if (target)
		prefix = AD9834_FREQ_1_REGISTER_PREFIX;
	else
		prefix = AD9834_FREQ_0_REGISTER_PREFIX;
	uint32_t freq_28 = freq / AD9834_INTERNAL_FREQ * AD9834_FREQ_FACTOR;
	if (target)
		frequency1 = freq_28;
	else frequency0 = freq_28;
	uint16_t freq_28_lsb = prefix | (freq_28 & AD9834_FREQ_LSB_MASK);
	uint16_t freq_28_msb = prefix | (AD9834_FREQ_MSB_OFFSET(freq_28));
	AD9834_writeControlRegister(freq_28_lsb);
	AD9834_writeControlRegister(freq_28_msb);
}

// not recommended by datasheet
// target: AD9834_FREQ_0 or AD9834_FREQ_1
void AD9834_setFreqExternal2(uint8_t target, float freq, float base) {
	baseFrequency = base;
	if (!(controlRegisterValue & AD9834_SLEEP1_ENABLE))
		addAndSet(AD9834_SLEEP1_ENABLE);
	if (!(controlRegisterValue & AD9834_B28_ENABLE))
		addAndSet(AD9834_B28_ENABLE);
	uint16_t prefix;
	if (target)
		prefix = AD9834_FREQ_1_REGISTER_PREFIX;
	else
		prefix = AD9834_FREQ_0_REGISTER_PREFIX;
	uint32_t freq_28 = freq / base * AD9834_FREQ_FACTOR;
	if (target)
		frequency1 = freq_28;
	else frequency0 = freq_28;
	uint16_t freq_28_lsb = prefix | (freq_28 & AD9834_FREQ_LSB_MASK);
	uint16_t freq_28_msb = prefix | (AD9834_FREQ_MSB_OFFSET(freq_28));
	AD9834_writeControlRegister(freq_28_lsb);
	AD9834_writeControlRegister(freq_28_msb);
}

// target: AD9834_FREQ_0 or AD9834_FREQ_1
void AD9834_setFreqLSB(uint8_t target, float freq) {
	baseFrequency = AD9834_INTERNAL_FREQ;
	if (controlRegisterValue & AD9834_SLEEP1_ENABLE)
		removeAndSet(AD9834_SLEEP1_DISABLE_MASK);
	if (controlRegisterValue & AD9834_B28_ENABLE)
		removeAndSet(AD9834_B28_MASK);
	if (controlRegisterValue & AD9834_HLB_ENABLE)
		removeAndSet(AD9834_HLB_MASK);
	uint16_t prefix;
	if (target)
		prefix = AD9834_FREQ_1_REGISTER_PREFIX;
	else
		prefix = AD9834_FREQ_0_REGISTER_PREFIX;
	uint32_t freq_28 = freq / AD9834_INTERNAL_FREQ * AD9834_FREQ_FACTOR;
	if (target)
		frequency1 = freq_28;
	else frequency0 = freq_28;
	uint16_t freq_28_lsb = prefix | (freq_28 & AD9834_FREQ_LSB_MASK);
	AD9834_writeControlRegister(freq_28_lsb);
}

// target: AD9834_FREQ_0 or AD9834_FREQ_1
void AD9834_setFreqMSB(uint8_t target, float freq) {
	baseFrequency = AD9834_INTERNAL_FREQ;
	if (controlRegisterValue & AD9834_SLEEP1_ENABLE)
		removeAndSet(AD9834_SLEEP1_DISABLE_MASK);
	if (controlRegisterValue & AD9834_B28_ENABLE)
		removeAndSet(AD9834_B28_MASK);
	if (!(controlRegisterValue & AD9834_HLB_ENABLE))
		addAndSet(AD9834_HLB_ENABLE);
	uint16_t prefix;
	if (target)
		prefix = AD9834_FREQ_1_REGISTER_PREFIX;
	else
		prefix = AD9834_FREQ_0_REGISTER_PREFIX;
	uint32_t freq_28 = freq / AD9834_INTERNAL_FREQ * AD9834_FREQ_FACTOR;
	if (target)
		frequency1 = freq_28;
	else frequency0 = freq_28;
	uint16_t freq_28_msb = prefix | (AD9834_FREQ_MSB_OFFSET(freq_28));
	AD9834_writeControlRegister(freq_28_msb);
}

// target: AD9834_FREQ_0 or AD9834_FREQ_1
void AD9834_setFreqLSBExternal(uint8_t target, float freq, float base) {
	baseFrequency = base;
	if (!(controlRegisterValue & AD9834_SLEEP1_ENABLE))
		addAndSet(AD9834_SLEEP1_ENABLE);
	if (controlRegisterValue & AD9834_B28_ENABLE)
		removeAndSet(AD9834_B28_MASK);
	if (controlRegisterValue & AD9834_HLB_ENABLE)
		removeAndSet(AD9834_HLB_MASK);
	uint16_t prefix;
	if (target)
		prefix = AD9834_FREQ_1_REGISTER_PREFIX;
	else
		prefix = AD9834_FREQ_0_REGISTER_PREFIX;
	uint32_t freq_28 = freq / base * AD9834_FREQ_FACTOR;
	if (target)
		frequency1 = freq_28;
	else frequency0 = freq_28;
	uint16_t freq_28_lsb = prefix | (freq_28 & AD9834_FREQ_LSB_MASK);
	AD9834_writeControlRegister(freq_28_lsb);
}

// target: AD9834_FREQ_0 or AD9834_FREQ_1
void AD9834_setFreqMSBExternal(uint8_t target, float freq, float base) {
	baseFrequency = base;
	if (!(controlRegisterValue & AD9834_SLEEP1_ENABLE))
		addAndSet(AD9834_SLEEP1_ENABLE);
	if (controlRegisterValue & AD9834_B28_ENABLE)
		removeAndSet(AD9834_B28_MASK);
	if (!(controlRegisterValue & AD9834_HLB_ENABLE))
		addAndSet(AD9834_HLB_ENABLE);
	uint16_t prefix;
	if (target)
		prefix = AD9834_FREQ_1_REGISTER_PREFIX;
	else
		prefix = AD9834_FREQ_0_REGISTER_PREFIX;
	uint32_t freq_28 = freq / base * AD9834_FREQ_FACTOR;
	if (target)
		frequency1 = freq_28;
	else frequency0 = freq_28;
	uint16_t freq_28_msb = prefix | (AD9834_FREQ_MSB_OFFSET(freq_28));
	AD9834_writeControlRegister(freq_28_msb);
}

// target: AD9834_PHASE_0 or AD9834_PHASE_1
void AD9834_setPhase(uint8_t target, float phase) {
	uint16_t prefix;
	if (target)
		prefix = AD9834_PHASE_1_REGISTER_PREFIX;
	else
		prefix = AD9834_PHASE_0_REGISTER_PREFIX;
	uint16_t phase_12 = phase * AD9834_PHASE_FACTOR / 2 / PI;
	AD9834_writeControlRegister(prefix | phase_12);
}

// mode: AD9834_MODE_SINE_WAVE, AD9834_MODE_TRI_WAVE, AD9834_MODE_SQUARE_WAVE
void AD9834_setOutput(uint8_t mode) {
	if (!mode)
		removeAndSet(AD9834_MODE_MASK & AD9834_OPBITEN_MASK);
	else if (mode == 1)
		addAndRemoveAndSet(AD9834_MODE_ENABLE, AD9834_OPBITEN_MASK);
	else if (mode == 2)
		addAndRemoveAndSet(AD9834_OPBITEN_ENABLE | AD9834_DIV_2_ENABLE, AD9834_MODE_MASK);
}

// sign_mode: AD9834_SIGN_MODE_DAC_MSB_2, AD9834_SIGN_MODE_DAC_MSB, AD9834_SIGN_MODE_COMPARE_OUT
void AD9834_setSignOutput(uint8_t sign_mode) {
	if (!sign_mode)
		addAndRemoveAndSet(AD9834_OPBITEN_ENABLE, AD9834_MODE_MASK & AD9834_SIGN_PIB_MASK & AD9834_DIV_2_MASK);
	else if (sign_mode == 1)
		addAndRemoveAndSet(AD9834_OPBITEN_ENABLE | AD9834_DIV_2_ENABLE, AD9834_MODE_MASK & AD9834_SIGN_PIB_MASK);
	else if (sign_mode == 2)
		addAndRemoveAndSet(AD9834_OPBITEN_ENABLE | AD9834_DIV_2_ENABLE | AD9834_SIGN_PIB_ENABLE, AD9834_MODE_MASK);
}

void AD9834_setOutputChannel(struct AD9834_Handler *handler, uint8_t frequencyChannel, uint8_t phaseChannel) {
	if (frequencyChannel)
		AD9834_frequencySelect(handler, TSMS_GPIO_STATUS_HIGH);
	else AD9834_frequencySelect(handler, TSMS_GPIO_STATUS_LOW);
	if (phaseChannel)
		AD9834_phaseSelect(handler, TSMS_GPIO_STATUS_HIGH);
	else AD9834_phaseSelect(handler, TSMS_GPIO_STATUS_LOW);
}