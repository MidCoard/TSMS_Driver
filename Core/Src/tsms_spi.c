#include "tsms_spi.h"

uint8_t TSMS_SPI_TRANSFER_BYTE[2][8] = {{7,6,5,4,3,2,1},{0,1,2,3,4,5,6,7}};
uint8_t TSMS_SPI_TRANSFER_HALFWORD[2][16] = {{15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}};
uint8_t TSMS_SPI_TRANSFER_WORD[2][32] = {{31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31}};
uint8_t TSMS_SPI_TRANSFER_24BIT[2][24] = {{23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23}};

TSMS_INLINE static void delay() {
	volatile uint8_t c = 1;
	while(c--);
}

TSMS_INLINE static void TSMS_SPI_SCLK_HIGH(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->sclk, TSMS_GPIO_HIGH);
}

TSMS_INLINE static void TSMS_SPI_SCLK_LOW(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->sclk, TSMS_GPIO_LOW);
}

TSMS_INLINE static void TSMS_SPI_CS_HIGH(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->cs, TSMS_GPIO_HIGH);
}

TSMS_INLINE static void TSMS_SPI_CS_LOW(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->cs, TSMS_GPIO_LOW);
}

TSMS_INLINE static void TSMS_SPI_DIN_HIGH(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->din, TSMS_GPIO_HIGH);
}

TSMS_INLINE static void TSMS_SPI_DIN_LOW(TSMS_SHP spi) {
	TSMS_GPIO_write(spi->din, TSMS_GPIO_LOW);
}

TSMS_INLINE static TSMS_GPIO_STATUS TSMS_SPI_DOUT(TSMS_SHP spi) {
	return TSMS_GPIO_read(spi->dout);
}

TSMS_INLINE static void TSMS_SPI_transmitByte(TSMS_SHP spi, uint8_t data) {
	switch (spi->mode) {
		case TSMS_SPI_MODE_0:
			for (uint8_t i = 0;i<8;i++) {
				if ((data >> TSMS_SPI_TRANSFER_BYTE[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
				TSMS_SPI_SCLK_HIGH(spi);
				TSMS_SPI_SCLK_LOW(spi);
			}
			break;
		case TSMS_SPI_MODE_1:
			for (uint8_t i = 0;i<8;i++) {
                if ((data >> TSMS_SPI_TRANSFER_BYTE[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
				TSMS_SPI_SCLK_LOW(spi);
                TSMS_SPI_SCLK_HIGH(spi);
			}
			break;
		case TSMS_SPI_MODE_2:
			for (uint8_t i = 0;i<8;i++) {
				if ((data >> TSMS_SPI_TRANSFER_BYTE[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
				TSMS_SPI_SCLK_HIGH(spi);
                TSMS_SPI_SCLK_LOW(spi);
			}
			break;
		case TSMS_SPI_MODE_3:
			for (uint8_t i = 0;i<8;i++) {
				if ((data >> TSMS_SPI_TRANSFER_BYTE[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
                TSMS_SPI_SCLK_LOW(spi);
                TSMS_SPI_SCLK_HIGH(spi);
			}
			break;
	}
}

TSMS_INLINE static void TSMS_SPI_transmitHalfWord(TSMS_SHP spi, uint16_t data) {
	switch (spi->mode) {
		case TSMS_SPI_MODE_0:
			for (uint8_t i = 0;i<16;i++) {
				if ((data >> TSMS_SPI_TRANSFER_HALFWORD[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
				TSMS_SPI_SCLK_HIGH(spi);
				TSMS_SPI_SCLK_LOW(spi);
			}
			break;
		case TSMS_SPI_MODE_1:
			for (uint8_t i = 0;i<16;i++) {
				if ((data >> TSMS_SPI_TRANSFER_HALFWORD[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
                TSMS_SPI_SCLK_LOW(spi);
                TSMS_SPI_SCLK_HIGH(spi);
			}
			break;
		case TSMS_SPI_MODE_2:
			for (uint8_t i = 0;i<16;i++) {
				if ((data >> TSMS_SPI_TRANSFER_HALFWORD[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
                TSMS_SPI_SCLK_HIGH(spi);
                TSMS_SPI_SCLK_LOW(spi);
			}
			break;
		case TSMS_SPI_MODE_3:
			for (uint8_t i = 0;i<16;i++) {
				if ((data >> TSMS_SPI_TRANSFER_HALFWORD[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
                TSMS_SPI_SCLK_LOW(spi);
                TSMS_SPI_SCLK_HIGH(spi);
			}
			break;
	}
}

TSMS_INLINE static void TSMS_SPI_transmitWord(TSMS_SHP spi, uint32_t data) {
	switch (spi->mode) {
		case TSMS_SPI_MODE_0:
			for (uint8_t i = 0;i<32;i++) {
				if ((data >> TSMS_SPI_TRANSFER_WORD[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
				TSMS_SPI_SCLK_HIGH(spi);
				TSMS_SPI_SCLK_LOW(spi);
			}
			break;
		case TSMS_SPI_MODE_1:
			for (uint8_t i = 0;i<32;i++) {
				if ((data >> TSMS_SPI_TRANSFER_WORD[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
                TSMS_SPI_SCLK_LOW(spi);
                TSMS_SPI_SCLK_HIGH(spi);
			}
			break;
		case TSMS_SPI_MODE_2:
			for (uint8_t i = 0;i<32;i++) {
				if ((data >> TSMS_SPI_TRANSFER_WORD[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
                TSMS_SPI_SCLK_HIGH(spi);
                TSMS_SPI_SCLK_LOW(spi);
			}
			break;
		case TSMS_SPI_MODE_3:
			for (uint8_t i = 0;i<32;i++) {
				if ((data >> TSMS_SPI_TRANSFER_WORD[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
                TSMS_SPI_SCLK_LOW(spi);
                TSMS_SPI_SCLK_HIGH(spi);
			}
			break;
	}
}

TSMS_INLINE static void TSMS_SPI_transmit24Bit(TSMS_SHP spi, uint32_t data) {
	switch (spi->mode) {
		case TSMS_SPI_MODE_0:
			for (uint8_t i = 0;i<24;i++) {
				if ((data >> TSMS_SPI_TRANSFER_24BIT[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
				TSMS_SPI_SCLK_HIGH(spi);
				TSMS_SPI_SCLK_LOW(spi);
			}
			break;
		case TSMS_SPI_MODE_1:
			for (uint8_t i = 0;i<24;i++) {
				if ((data >> TSMS_SPI_TRANSFER_24BIT[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
				TSMS_SPI_SCLK_LOW(spi);
                TSMS_SPI_SCLK_HIGH(spi);
			}
			break;
		case TSMS_SPI_MODE_2:
			for (uint8_t i = 0;i<24;i++) {
				if ((data >> TSMS_SPI_TRANSFER_24BIT[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
                TSMS_SPI_SCLK_HIGH(spi);
                TSMS_SPI_SCLK_LOW(spi);
			}
			break;
		case TSMS_SPI_MODE_3:
			for (uint8_t i = 0;i<24;i++) {
				if ((data >> TSMS_SPI_TRANSFER_24BIT[spi->type][i])&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
                TSMS_SPI_SCLK_LOW(spi);
                TSMS_SPI_SCLK_HIGH(spi);
			}
			break;
	}
}

TSMS_INLINE static void TSMS_SPI_transmitCustomBit(TSMS_SHP spi, uint8_t bits, uint32_t data) {
	switch (spi->mode) {
		case TSMS_SPI_MODE_0:
			for (uint8_t i = 0;i<bits;i++) {
				uint8_t pos = spi->type == TSMS_SPI_MSB ? bits - i - 1 : i;
				if ((data >> pos)&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
				TSMS_SPI_SCLK_HIGH(spi);
				TSMS_SPI_SCLK_LOW(spi);
			}
			break;
		case TSMS_SPI_MODE_1:
			for (uint8_t i = 0;i<bits;i++) {
				uint8_t pos = spi->type == TSMS_SPI_MSB ? bits - i - 1 : i;
				if ((data >> pos)&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
				TSMS_SPI_SCLK_LOW(spi);
                TSMS_SPI_SCLK_HIGH(spi);
			}
			break;
		case TSMS_SPI_MODE_2:
			for (uint8_t i = 0;i<bits;i++) {
				uint8_t pos = spi->type == TSMS_SPI_MSB ? bits - i - 1 : i;
				if ((data >> pos)&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
                TSMS_SPI_SCLK_HIGH(spi);
                TSMS_SPI_SCLK_LOW(spi);
			}
			break;
		case TSMS_SPI_MODE_3:
			for (uint8_t i = 0;i<bits;i++) {
				uint8_t pos = spi->type == TSMS_SPI_MSB ? bits - i - 1 : i;
				if ((data >> pos)&1)
					TSMS_SPI_DIN_HIGH(spi);
				else TSMS_SPI_DIN_LOW(spi);
                TSMS_SPI_SCLK_LOW(spi);
                TSMS_SPI_SCLK_HIGH(spi);
			}
			break;
	}
}

TSMS_INLINE static void TSMS_SPI_receiveByte(TSMS_SHP spi, uint8_t * data) {
	*data = 0;
	switch (spi->mode) {
		case TSMS_SPI_MODE_0:
			for (uint8_t i = 0;i<8;i++) {
				TSMS_SPI_SCLK_HIGH(spi);
				*data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_BYTE[spi->type][i]);
				TSMS_SPI_SCLK_LOW(spi);
			}
			break;
		case TSMS_SPI_MODE_1:
            TSMS_SPI_SCLK_HIGH(spi);
			for (uint8_t i = 0;i<8;i++) {
                TSMS_SPI_SCLK_LOW(spi);
				*data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_BYTE[spi->type][i]);
				TSMS_SPI_SCLK_HIGH(spi);
			}
			break;
		case TSMS_SPI_MODE_2:
            TSMS_SPI_SCLK_LOW(spi);
			for (uint8_t i = 0;i<8;i++) {
                TSMS_SPI_SCLK_HIGH(spi);
				*data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_BYTE[spi->type][i]);
				TSMS_SPI_SCLK_LOW(spi);
			}
			break;
		case TSMS_SPI_MODE_3:
			for (uint8_t i = 0;i<8;i++) {
				TSMS_SPI_SCLK_LOW(spi);
				*data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_BYTE[spi->type][i]);
				TSMS_SPI_SCLK_HIGH(spi);
			}
			break;
	}
}

TSMS_INLINE static void TSMS_SPI_receiveHalfWord(TSMS_SHP spi, uint16_t * data) {
    *data = 0;
    switch (spi->mode) {
        case TSMS_SPI_MODE_0:
            for (uint8_t i = 0;i<16;i++) {
                TSMS_SPI_SCLK_HIGH(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_HALFWORD[spi->type][i]);
                TSMS_SPI_SCLK_LOW(spi);
            }
            break;
        case TSMS_SPI_MODE_1:
            TSMS_SPI_SCLK_HIGH(spi);
            for (uint8_t i = 0;i<16;i++) {
                TSMS_SPI_SCLK_LOW(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_HALFWORD[spi->type][i]);
                TSMS_SPI_SCLK_HIGH(spi);
            }
            break;
        case TSMS_SPI_MODE_2:
            TSMS_SPI_SCLK_LOW(spi);
            for (uint8_t i = 0;i<16;i++) {
                TSMS_SPI_SCLK_HIGH(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_HALFWORD[spi->type][i]);
                TSMS_SPI_SCLK_LOW(spi);
            }
            break;
        case TSMS_SPI_MODE_3:
            for (uint8_t i = 0;i<16;i++) {
                TSMS_SPI_SCLK_LOW(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_HALFWORD[spi->type][i]);
                TSMS_SPI_SCLK_HIGH(spi);
            }
            break;
    }
}

TSMS_INLINE static void TSMS_SPI_receiveWord(TSMS_SHP spi, uint32_t * data) {
    *data = 0;
    switch (spi->mode) {
        case TSMS_SPI_MODE_0:
            for (uint8_t i = 0;i<32;i++) {
                TSMS_SPI_SCLK_HIGH(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_WORD[spi->type][i]);
                TSMS_SPI_SCLK_LOW(spi);
            }
            break;
        case TSMS_SPI_MODE_1:
            TSMS_SPI_SCLK_HIGH(spi);
            for (uint8_t i = 0;i<32;i++) {
                TSMS_SPI_SCLK_LOW(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_WORD[spi->type][i]);
                TSMS_SPI_SCLK_HIGH(spi);
            }
            break;
        case TSMS_SPI_MODE_2:
            TSMS_SPI_SCLK_LOW(spi);
            for (uint8_t i = 0;i<32;i++) {
                TSMS_SPI_SCLK_HIGH(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_WORD[spi->type][i]);
                TSMS_SPI_SCLK_LOW(spi);
            }
            break;
        case TSMS_SPI_MODE_3:
            for (uint8_t i = 0;i<32;i++) {
                TSMS_SPI_SCLK_LOW(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_WORD[spi->type][i]);
                TSMS_SPI_SCLK_HIGH(spi);
            }
            break;
    }
}

TSMS_INLINE static void TSMS_SPI_receive24Bit(TSMS_SHP spi, uint32_t * data) {
    *data = 0;
    switch (spi->mode) {
        case TSMS_SPI_MODE_0:
            for (uint8_t i = 0;i<24;i++) {
                TSMS_SPI_SCLK_HIGH(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_24BIT[spi->type][i]);
                TSMS_SPI_SCLK_LOW(spi);
            }
            break;
        case TSMS_SPI_MODE_1:
            TSMS_SPI_SCLK_HIGH(spi);
            for (uint8_t i = 0;i<24;i++) {
                TSMS_SPI_SCLK_LOW(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_24BIT[spi->type][i]);
                TSMS_SPI_SCLK_HIGH(spi);
            }
            break;
        case TSMS_SPI_MODE_2:
            TSMS_SPI_SCLK_LOW(spi);
            for (uint8_t i = 0;i<24;i++) {
                TSMS_SPI_SCLK_HIGH(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_24BIT[spi->type][i]);
                TSMS_SPI_SCLK_LOW(spi);
            }
            break;
        case TSMS_SPI_MODE_3:
            for (uint8_t i = 0;i<24;i++) {
                TSMS_SPI_SCLK_LOW(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << TSMS_SPI_TRANSFER_24BIT[spi->type][i]);
                TSMS_SPI_SCLK_HIGH(spi);
            }
            break;
    }
}

TSMS_INLINE static void TSMS_SPI_receiveCustomBit(TSMS_SHP spi,uint8_t  bits,uint32_t * data) {
    *data = 0;
    switch (spi->mode) {
        case TSMS_SPI_MODE_0:
            for (uint8_t i = 0;i<bits;i++)  {
                uint8_t pos = spi->type == TSMS_SPI_MSB ? bits - i - 1 : i;
                TSMS_SPI_SCLK_HIGH(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << pos);
                TSMS_SPI_SCLK_LOW(spi);
            }
            break;
        case TSMS_SPI_MODE_1:
            TSMS_SPI_SCLK_HIGH(spi);
            for (uint8_t i = 0;i<bits;i++)  {
                uint8_t pos = spi->type == TSMS_SPI_MSB ? bits - i - 1 : i;
                TSMS_SPI_SCLK_LOW(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << pos);
                TSMS_SPI_SCLK_HIGH(spi);
            }
            break;
        case TSMS_SPI_MODE_2:
            TSMS_SPI_SCLK_LOW(spi);
            for (uint8_t i = 0;i<bits;i++) {
                uint8_t pos = spi->type == TSMS_SPI_MSB ? bits - i - 1 : i;
                TSMS_SPI_SCLK_HIGH(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << pos);
                TSMS_SPI_SCLK_LOW(spi);
            }
            break;
        case TSMS_SPI_MODE_3:
            for (uint8_t i = 0;i<bits;i++) {
                uint8_t pos = spi->type == TSMS_SPI_MSB ? bits - i - 1 : i;
                TSMS_SPI_SCLK_LOW(spi);
                *data |= ((TSMS_SPI_DOUT(spi) == TSMS_GPIO_HIGH ? 1 : 0) << pos);
                TSMS_SPI_SCLK_HIGH(spi);
            }
            break;
    }
}

static void TSMS_SPI_internalRelease0(TSMS_SHP spi) {
	free(spi);
}

static void TSMS_SPI_internalRelease1(TSMS_SHP spi) {
	TSMS_GPIO_release(spi->cs);
	TSMS_GPIO_release(spi->sclk);
	TSMS_GPIO_release(spi->din);
	TSMS_GPIO_release(spi->dout);
	TSMS_SPI_internalRelease0(spi);
}

TSMS_SHP TSMS_SPI_createSoftwareHandler(TSMS_GHP cs, TSMS_GHP sclk, TSMS_GHP din, TSMS_GHP dout, TSMS_SPI_MODE mode, TSMS_GPIO_STATUS csValid, TSMS_SPI_TRANSFER_TYPE type) {
	TSMS_SHP spi = malloc(sizeof (struct TSMS_SPI_HANDLER));
	if (spi == TSMS_NULL)
		return TSMS_NULL;
	spi->cs = cs;
	spi->sclk = sclk;
	spi->din = din;
	spi->dout = dout;
	spi->mode = mode;
	spi->csValid = csValid;
	spi->delay = delay;
	spi->type = type;
	spi->isHardware = false;
	spi->release = TSMS_SPI_internalRelease0;
	if (spi->mode == TSMS_SPI_MODE_0 || spi->mode == TSMS_SPI_MODE_1)
		TSMS_SPI_SCLK_LOW(spi);
	else TSMS_SPI_SCLK_HIGH(spi);
	return spi;
}


#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)

TSMS_SHP TSMS_SPI_createSoftwareHanlder(GPIO_TypeDef * csPort, uint16_t csPin,
                                        GPIO_TypeDef * sclkPort, uint16_t sclkPin,
                                        GPIO_TypeDef * dinPort, uint16_t dinPin,
                                        GPIO_TypeDef * doutPort, uint16_t doutPin,
										TSMS_SPI_MODE mode, TSMS_GPIO_STATUS csValid, TSMS_SPI_TRANSFER_TYPE type) {
	TSMS_SHP spi = TSMS_SPI_createSoftwareHandler(TSMS_GPIO_createHandler(csPort, csPin),
	                                      TSMS_GPIO_createHandler(sclkPort, sclkPin),
	                                      TSMS_GPIO_createHandler(dinPort, dinPin),
	                                      TSMS_GPIO_createHandler(doutPort, doutPin),
										  mode, csValid, type);
	spi->release = TSMS_SPI_internalRelease1;
	return spi;
}

TSMS_SHP TSMS_SPI_createHardwareHandler(SPI_HandleTypeDef * spi) {
	TSMS_SHP handler = malloc(sizeof (struct TSMS_SPI_HANDLER));
	handler->hardwareHandler = spi;
	handler->isHardware = true;
	handler->release = TSMS_SPI_internalRelease0;
	return handler;
}

#endif

TSMS_RESULT TSMS_SPI_transmitBytes(TSMS_SHP spi, uint8_t * data, uint32_t length) {
	if (spi->isHardware) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
		if (HAL_SPI_Transmit(spi->hardwareHandler, data, length, 0xffffffff) == HAL_OK)
			return TSMS_SUCCESS;
		else return TSMS_ERROR;
#else
		return TSMS_TIMEOUT;
#endif
	} else {
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_HIGH(spi);
		else TSMS_SPI_CS_LOW(spi);
		spi->delay();
		for (uint32_t i = 0;i < length;i++)
			TSMS_SPI_transmitByte(spi, data[i]);
		spi->delay();
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_LOW(spi);
		else TSMS_SPI_CS_HIGH(spi);
		return TSMS_SUCCESS;
	}
}

TSMS_RESULT TSMS_SPI_transmitHalfWords(TSMS_SHP spi, uint16_t *data, uint32_t length) {
	if (spi->isHardware) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
#warning transmitting 16 bits data is not tested!
		if (HAL_SPI_Transmit(spi->hardwareHandler,data,length,0xffffffff ) == HAL_OK)
			return TSMS_SUCCESS;
		else return TSMS_ERROR;
#else
		return TSMS_TIMEOUT;
#endif
	} else {
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_HIGH(spi);
		else TSMS_SPI_CS_LOW(spi);
		spi->delay();
		for (uint32_t i = 0;i < length;i++)
			TSMS_SPI_transmitHalfWord(spi, data[i]);
		spi->delay();
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_LOW(spi);
		else TSMS_SPI_CS_HIGH(spi);
		return TSMS_SUCCESS;
	}
}

TSMS_RESULT TSMS_SPI_transmitWords(TSMS_SHP spi, uint32_t *data, uint32_t length) {
	if (spi->isHardware) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
#warning transmitting 32 bits data is not tested!
		if (HAL_SPI_Transmit(spi->hardwareHandler,data,length,0xffffffff) == HAL_OK)
			return TSMS_SUCCESS;
		else return TSMS_ERROR;
#else
		return TSMS_TIMEOUT:
#endif
	} else {
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_HIGH(spi);
		else TSMS_SPI_CS_LOW(spi);
		spi->delay();
		for (uint32_t i = 0;i < length;i++)
			TSMS_SPI_transmitWord(spi, data[i]);
		spi->delay();
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_LOW(spi);
		else TSMS_SPI_CS_HIGH(spi);
		return TSMS_SUCCESS;
	}
}

TSMS_RESULT TSMS_SPI_transmit24Bits(TSMS_SHP spi, uint32_t *data, uint32_t length) {
	if (spi->isHardware) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
#warning transmitting 24 bits data is not tested!
		if (HAL_SPI_Transmit(spi->hardwareHandler,data,length,0xffffffff) == HAL_OK)
			return TSMS_SUCCESS;
		else return TSMS_ERROR;
#else
		return TSMS_TIMEOUT:
#endif
	} else {
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_HIGH(spi);
		else TSMS_SPI_CS_LOW(spi);
		spi->delay();
		for (uint32_t i = 0;i < length;i++)
			TSMS_SPI_transmit24Bit(spi, data[i]);
		spi->delay();
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_LOW(spi);
		else TSMS_SPI_CS_HIGH(spi);
		return TSMS_SUCCESS;
	}
}

TSMS_RESULT TSMS_SPI_transmitCustomBits(TSMS_SHP spi, uint32_t *data, uint8_t bits, uint32_t length) {
	if (spi->isHardware) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
#warning transmitting 24 bits data is not tested!
		if (HAL_SPI_Transmit(spi->hardwareHandler,data,length,0xffffffff) == HAL_OK)
			return TSMS_SUCCESS;
		else return TSMS_ERROR;
#else
		return TSMS_TIMEOUT:
#endif
	} else {
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_HIGH(spi);
		else TSMS_SPI_CS_LOW(spi);
		spi->delay();
		for (uint32_t i = 0;i < length;i++)
			TSMS_SPI_transmitCustomBit(spi, bits, data[i]);
		spi->delay();
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_LOW(spi);
		else TSMS_SPI_CS_HIGH(spi);
		return TSMS_SUCCESS;
	}
}

TSMS_RESULT TSMS_SPI_receiveBytes(TSMS_SHP spi, uint8_t *data, uint32_t length) {
	if (spi->isHardware) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
		if (HAL_SPI_Receive(spi->hardwareHandler,data,length,0xffffffff) == HAL_OK)
			return TSMS_SUCCESS;
		else return TSMS_ERROR;
#else
		return TSMS_TIMEOUT;
#endif
	} else {
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_HIGH(spi);
		else TSMS_SPI_CS_LOW(spi);
		spi->delay();
		for (uint32_t i = 0;i < length;i++)
			TSMS_SPI_receiveByte(spi, data + i);
		spi->delay();
		if (spi->csValid == TSMS_GPIO_HIGH)
			TSMS_SPI_CS_LOW(spi);
		else TSMS_SPI_CS_HIGH(spi);
		return TSMS_SUCCESS;
	}
}

TSMS_RESULT TSMS_SPI_receiveHalfWords(TSMS_SHP spi, uint16_t *data, uint32_t length) {
    if (spi->isHardware) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
        if (HAL_SPI_Receive(spi->hardwareHandler,data,length,0xffffffff) == HAL_OK)
            return TSMS_SUCCESS;
        else return TSMS_ERROR;
#else
        return TSMS_TIMEOUT;
#endif
    } else {
        if (spi->csValid == TSMS_GPIO_HIGH)
            TSMS_SPI_CS_HIGH(spi);
        else TSMS_SPI_CS_LOW(spi);
        spi->delay();
        for (uint32_t i = 0;i < length;i++)
            TSMS_SPI_receiveHalfWord(spi, data + i);
        spi->delay();
        if (spi->csValid == TSMS_GPIO_HIGH)
            TSMS_SPI_CS_LOW(spi);
        else TSMS_SPI_CS_HIGH(spi);
        return TSMS_SUCCESS;
    }
}

TSMS_RESULT TSMS_SPI_receiveWords(TSMS_SHP spi, uint32_t *data, uint32_t length) {
    if (spi->isHardware) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
        if (HAL_SPI_Receive(spi->hardwareHandler,data,length,0xffffffff) == HAL_OK)
            return TSMS_SUCCESS;
        else return TSMS_ERROR;
#else
        return TSMS_TIMEOUT;
#endif
    } else {
        if (spi->csValid == TSMS_GPIO_HIGH)
            TSMS_SPI_CS_HIGH(spi);
        else TSMS_SPI_CS_LOW(spi);
        spi->delay();
        for (uint32_t i = 0;i < length;i++)
            TSMS_SPI_receiveWord(spi, data + i);
        spi->delay();
        if (spi->csValid == TSMS_GPIO_HIGH)
            TSMS_SPI_CS_LOW(spi);
        else TSMS_SPI_CS_HIGH(spi);
        return TSMS_SUCCESS;
    }
}

TSMS_RESULT TSMS_SPI_receive24Bits(TSMS_SHP spi, uint32_t *data, uint32_t length) {
    if (spi->isHardware) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
        if (HAL_SPI_Receive(spi->hardwareHandler,data,length,0xffffffff) == HAL_OK)
            return TSMS_SUCCESS;
        else return TSMS_ERROR;
#else
        return TSMS_TIMEOUT;
#endif
    } else {
        if (spi->csValid == TSMS_GPIO_HIGH)
            TSMS_SPI_CS_HIGH(spi);
        else TSMS_SPI_CS_LOW(spi);
        spi->delay();
        for (uint32_t i = 0;i < length;i++)
            TSMS_SPI_receive24Bit(spi, data + i);
        spi->delay();
        if (spi->csValid == TSMS_GPIO_HIGH)
            TSMS_SPI_CS_LOW(spi);
        else TSMS_SPI_CS_HIGH(spi);
        return TSMS_SUCCESS;
    }
}

TSMS_RESULT TSMS_SPI_receiveCustomBits(TSMS_SHP spi, uint32_t *data, uint8_t bits, uint32_t length) {
    if (spi->isHardware) {
#if defined(TSMS_STM32_SPI) && defined(HAL_SPI_MODULE_ENABLED)
        if (HAL_SPI_Receive(spi->hardwareHandler,data,length,0xffffffff) == HAL_OK)
            return TSMS_SUCCESS;
        else return TSMS_ERROR;
#else
        return TSMS_TIMEOUT;
#endif
    } else {
        if (spi->csValid == TSMS_GPIO_HIGH)
            TSMS_SPI_CS_HIGH(spi);
        else TSMS_SPI_CS_LOW(spi);
        spi->delay();
        for (uint32_t i = 0;i < length;i++)
            TSMS_SPI_receiveCustomBit(spi,bits,data + i);
        spi->delay();
        if (spi->csValid == TSMS_GPIO_HIGH)
            TSMS_SPI_CS_LOW(spi);
        else TSMS_SPI_CS_HIGH(spi);
        return TSMS_SUCCESS;
    }
}

TSMS_RESULT TSMS_SPI_release(TSMS_SHP spi) {
	spi->release(spi);
	return TSMS_SUCCESS;
}

TSMS_SPI_MODE TSMS_SPI_mode(TSMS_SPI_CPOL cpol, TSMS_SPI_CPHA cpha) {
	uint8_t value = cpol * 2 + cpha;
	return (TSMS_SPI_MODE) value;
}