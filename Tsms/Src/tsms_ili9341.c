#include "screen/tsms_ili9341.h"

// 0x9341
uint16_t TSMS_ILI9341_readId(TSMS_SCHP screen) {
	TSMS_SCREEN_writeCommand(screen, 0XD3);
	uint16_t id;
	id = TSMS_SCREEN_readData(screen);
	id = TSMS_SCREEN_readData(screen);
	id = TSMS_SCREEN_readData(screen);
	id <<= 8;
	id |= TSMS_SCREEN_readData(screen);
	return id;
}

TSMS_RESULT TSMS_ILI9341_init(TSMS_SCHP screen, void * op) {
	TSMS_SCREEN_writeCommand(screen, 0xCF);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xC1);
	TSMS_SCREEN_writeData(screen, 0X30);
	TSMS_SCREEN_writeCommand(screen, 0xED);
	TSMS_SCREEN_writeData(screen, 0x64);
	TSMS_SCREEN_writeData(screen, 0x03);
	TSMS_SCREEN_writeData(screen, 0X12);
	TSMS_SCREEN_writeData(screen, 0X81);
	TSMS_SCREEN_writeCommand(screen, 0xE8);
	TSMS_SCREEN_writeData(screen, 0x85);
	TSMS_SCREEN_writeData(screen, 0x10);
	TSMS_SCREEN_writeData(screen, 0x7A);
	TSMS_SCREEN_writeCommand(screen, 0xCB);
	TSMS_SCREEN_writeData(screen, 0x39);
	TSMS_SCREEN_writeData(screen, 0x2C);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x34);
	TSMS_SCREEN_writeData(screen, 0x02);
	TSMS_SCREEN_writeCommand(screen, 0xF7);
	TSMS_SCREEN_writeData(screen, 0x20);
	TSMS_SCREEN_writeCommand(screen, 0xEA);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeCommand(screen, 0xC0);    //Power control
	TSMS_SCREEN_writeData(screen, 0x1B);           //VRH[5:0]
	TSMS_SCREEN_writeCommand(screen, 0xC1);    //Power control
	TSMS_SCREEN_writeData(screen, 0x01);           //SAP[2:0];BT[3:0]
	TSMS_SCREEN_writeCommand(screen, 0xC5);    //VCM control
	TSMS_SCREEN_writeData(screen, 0x30);           //3F
	TSMS_SCREEN_writeData(screen, 0x30);           //3C
	TSMS_SCREEN_writeCommand(screen, 0xC7);    //VCM control2
	TSMS_SCREEN_writeData(screen, 0XB7);
	TSMS_SCREEN_writeCommand(screen, 0x36);    // Memory Access Control
	TSMS_SCREEN_writeData(screen, 0x48);
	TSMS_SCREEN_writeCommand(screen, 0x3A);
	TSMS_SCREEN_writeData(screen, 0x55);
	TSMS_SCREEN_writeCommand(screen, 0xB1);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x1A);
	TSMS_SCREEN_writeCommand(screen, 0xB6);    // Display Function Control
	TSMS_SCREEN_writeData(screen, 0x0A);
	TSMS_SCREEN_writeData(screen, 0xA2);
	TSMS_SCREEN_writeCommand(screen, 0xF2);    // 3Gamma Function Disable
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeCommand(screen, 0x26);    //Gamma curve selected
	TSMS_SCREEN_writeData(screen, 0x01);
	TSMS_SCREEN_writeCommand(screen, 0xE0);    //Set Gamma
	TSMS_SCREEN_writeData(screen, 0x0F);
	TSMS_SCREEN_writeData(screen, 0x2A);
	TSMS_SCREEN_writeData(screen, 0x28);
	TSMS_SCREEN_writeData(screen, 0x08);
	TSMS_SCREEN_writeData(screen, 0x0E);
	TSMS_SCREEN_writeData(screen, 0x08);
	TSMS_SCREEN_writeData(screen, 0x54);
	TSMS_SCREEN_writeData(screen, 0XA9);
	TSMS_SCREEN_writeData(screen, 0x43);
	TSMS_SCREEN_writeData(screen, 0x0A);
	TSMS_SCREEN_writeData(screen, 0x0F);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeCommand(screen, 0XE1);    //Set Gamma
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x15);
	TSMS_SCREEN_writeData(screen, 0x17);
	TSMS_SCREEN_writeData(screen, 0x07);
	TSMS_SCREEN_writeData(screen, 0x11);
	TSMS_SCREEN_writeData(screen, 0x06);
	TSMS_SCREEN_writeData(screen, 0x2B);
	TSMS_SCREEN_writeData(screen, 0x56);
	TSMS_SCREEN_writeData(screen, 0x3C);
	TSMS_SCREEN_writeData(screen, 0x05);
	TSMS_SCREEN_writeData(screen, 0x10);
	TSMS_SCREEN_writeData(screen, 0x0F);
	TSMS_SCREEN_writeData(screen, 0x3F);
	TSMS_SCREEN_writeData(screen, 0x3F);
	TSMS_SCREEN_writeData(screen, 0x0F);
	TSMS_SCREEN_writeCommand(screen, 0x2B);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x01);
	TSMS_SCREEN_writeData(screen, 0x3f);
	TSMS_SCREEN_writeCommand(screen, 0x2A);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xef);
	TSMS_SCREEN_writeCommand(screen, 0x11);    //Exit Sleep
	TSMS_delay(120);
	TSMS_SCREEN_writeCommand(screen, 0x29);    //display on
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_ILI9341_setDisplayDirection(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction) {
	if (direction == TSMS_DISPLAY_VERTICAL) {
		screen->width = screen->defaultWidth;
		screen->height = screen->defaultHeight;
	} else {
		screen->width = screen->defaultHeight;
		screen->height = screen->defaultWidth;
	}
	screen->writeCommand = 0x2C;
	screen->setXCommand = 0x2A;
	screen->setYCommand = 0x2B;
	screen->displayDirection = direction;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_ILI9341_setScanDirection(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction) {
	TSMS_SCAN_DIRECTION dir = direction;
	if (screen->displayDirection == TSMS_DISPLAY_HORIZONTAL)
		switch (direction) {
			case TSMS_SCAN_L2R_U2D:
				dir = 6;
				break;
			case TSMS_SCAN_L2R_D2U:
				dir = 7;
				break;
			case TSMS_SCAN_R2L_U2D:
				dir = 4;
				break;
			case TSMS_SCAN_R2L_D2U:
				dir = 5;
				break;
			case TSMS_SCAN_U2D_L2R:
				dir = 1;
				break;
			case TSMS_SCAN_U2D_R2L:
				dir = 0;
				break;
			case TSMS_SCAN_D2U_L2R:
				dir = 3;
				break;
			case TSMS_SCAN_D2U_R2L:
				dir = 2;
				break;
		}
	uint16_t reg = 0x36;
	uint16_t value = 0;
	switch (dir) {
		case TSMS_SCAN_L2R_U2D:
			value |= (0 << 7) | (0 << 6) | (0 << 5);
			break;
		case TSMS_SCAN_L2R_D2U:
			value |= (1 << 7) | (0 << 6) | (0 << 5);
			break;
		case TSMS_SCAN_R2L_U2D:
			value |= (0 << 7) | (1 << 6) | (0 << 5);
			break;
		case TSMS_SCAN_R2L_D2U:
			value |= (1 << 7) | (1 << 6) | (0 << 5);
			break;
		case TSMS_SCAN_U2D_L2R:
			value |= (0 << 7) | (0 << 6) | (1 << 5);
			break;
		case TSMS_SCAN_U2D_R2L:
			value |= (0 << 7) | (1 << 6) | (1 << 5);
			break;
		case TSMS_SCAN_D2U_L2R:
			value |= (1 << 7) | (0 << 6) | (1 << 5);
			break;
		case TSMS_SCAN_D2U_R2L:
			value |= (1 << 7) | (1 << 6) | (1 << 5);
			break;
	}
	value |= 0X08;
	TSMS_SCREEN_writeRegister(screen, reg, value);
	if (value & 0X20) {
		if (screen->width < screen->height)
			TSMS_UTIL_swap(&screen->width, &screen->height);
	} else if (screen->width > screen->height)
		TSMS_UTIL_swap(&screen->width, &screen->height);

	TSMS_SCREEN_writeCommand(screen, screen->setXCommand);
	TSMS_SCREEN_writeData(screen, 0);
	TSMS_SCREEN_writeData(screen, 0);
	TSMS_SCREEN_writeData(screen, (screen->width - 1) >> 8);
	TSMS_SCREEN_writeData(screen, (screen->width - 1) & 0XFF);
	TSMS_SCREEN_writeCommand(screen, screen->setYCommand);
	TSMS_SCREEN_writeData(screen, 0);
	TSMS_SCREEN_writeData(screen, 0);
	TSMS_SCREEN_writeData(screen, (screen->height - 1) >> 8);
	TSMS_SCREEN_writeData(screen, (screen->height - 1) & 0XFF);
	screen->scanDirection = direction;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_ILI9341_setCursor(TSMS_SCHP screen, uint16_t x, uint16_t y) {
	TSMS_SCREEN_writeCommand(screen, screen->setXCommand);
	TSMS_SCREEN_writeData(screen, x >> 8);
	TSMS_SCREEN_writeData(screen, x & 0XFF);
	TSMS_SCREEN_writeCommand(screen, screen->setYCommand);
	TSMS_SCREEN_writeData(screen, y >> 8);
	TSMS_SCREEN_writeData(screen, y & 0XFF);
	return TSMS_SUCCESS;
}