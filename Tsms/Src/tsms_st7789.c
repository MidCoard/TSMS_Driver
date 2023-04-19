#include "screen/tsms_st7789.h"

// 0x8552
uint16_t TSMS_ST7789_readId(TSMS_SCHP screen) {
	TSMS_SCREEN_writeCommand(screen, 0X04);
	uint16_t id;
	id = TSMS_SCREEN_readData(screen);
	id = TSMS_SCREEN_readData(screen);
	id = TSMS_SCREEN_readData(screen);
	id <<= 8;
	id |= TSMS_SCREEN_readData(screen);
	return id;
}

TSMS_RESULT TSMS_ST7789_init(TSMS_SCHP screen, void* option) {
	TSMS_SCREEN_writeCommand(screen, 0x11);

	TSMS_delay(120);

	TSMS_SCREEN_writeCommand(screen, 0x36);
	TSMS_SCREEN_writeData(screen, 0x00);


	TSMS_SCREEN_writeCommand(screen, 0x3A);
	TSMS_SCREEN_writeData(screen, 0X05);

	TSMS_SCREEN_writeCommand(screen, 0xB2);
	TSMS_SCREEN_writeData(screen, 0x0C);
	TSMS_SCREEN_writeData(screen, 0x0C);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x33);
	TSMS_SCREEN_writeData(screen, 0x33);

	TSMS_SCREEN_writeCommand(screen, 0xB7);
	TSMS_SCREEN_writeData(screen, 0x35);

	TSMS_SCREEN_writeCommand(screen, 0xBB);       //vcom
	TSMS_SCREEN_writeData(screen, 0x32);               //30

	TSMS_SCREEN_writeCommand(screen, 0xC0);
	TSMS_SCREEN_writeData(screen, 0x0C);

	TSMS_SCREEN_writeCommand(screen, 0xC2);
	TSMS_SCREEN_writeData(screen, 0x01);

	TSMS_SCREEN_writeCommand(screen, 0xC3);       //vrh
	TSMS_SCREEN_writeData(screen, 0x10);               //17 0D

	TSMS_SCREEN_writeCommand(screen, 0xC4);       //vdv
	TSMS_SCREEN_writeData(screen, 0x20);      //20

	TSMS_SCREEN_writeCommand(screen, 0xC6);
	TSMS_SCREEN_writeData(screen, 0x0f);

	TSMS_SCREEN_writeCommand(screen, 0xD0);
	TSMS_SCREEN_writeData(screen, 0xA4);
	TSMS_SCREEN_writeData(screen, 0xA1);

	TSMS_SCREEN_writeCommand(screen, 0xE0);       //Set Gamma
	TSMS_SCREEN_writeData(screen, 0xd0);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x02);
	TSMS_SCREEN_writeData(screen, 0x07);
	TSMS_SCREEN_writeData(screen, 0x0a);
	TSMS_SCREEN_writeData(screen, 0x28);
	TSMS_SCREEN_writeData(screen, 0x32);
	TSMS_SCREEN_writeData(screen, 0X44);
	TSMS_SCREEN_writeData(screen, 0x42);
	TSMS_SCREEN_writeData(screen, 0x06);
	TSMS_SCREEN_writeData(screen, 0x0e);
	TSMS_SCREEN_writeData(screen, 0x12);
	TSMS_SCREEN_writeData(screen, 0x14);
	TSMS_SCREEN_writeData(screen, 0x17);


	TSMS_SCREEN_writeCommand(screen, 0XE1);       //Set Gamma
	TSMS_SCREEN_writeData(screen, 0xd0);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x02);
	TSMS_SCREEN_writeData(screen, 0x07);
	TSMS_SCREEN_writeData(screen, 0x0a);
	TSMS_SCREEN_writeData(screen, 0x28);
	TSMS_SCREEN_writeData(screen, 0x31);
	TSMS_SCREEN_writeData(screen, 0x54);
	TSMS_SCREEN_writeData(screen, 0x47);
	TSMS_SCREEN_writeData(screen, 0x0e);
	TSMS_SCREEN_writeData(screen, 0x1c);
	TSMS_SCREEN_writeData(screen, 0x17);
	TSMS_SCREEN_writeData(screen, 0x1b);
	TSMS_SCREEN_writeData(screen, 0x1e);

	TSMS_SCREEN_writeCommand(screen, 0x2A);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xef);

	TSMS_SCREEN_writeCommand(screen, 0x2B);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x01);
	TSMS_SCREEN_writeData(screen, 0x3f);

	TSMS_SCREEN_writeCommand(screen, 0x29);       //display on
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_ST7789_setDisplayDirection(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction) {
	return TSMS_ILI9341_setDisplayDirection(screen, direction);
}

TSMS_RESULT TSMS_ST7789_setScanDirection(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction) {
	return TSMS_ILI9341_setScanDirection(screen, direction);
}

TSMS_RESULT TSMS_ST7789_setCursor(TSMS_SCHP screen, uint16_t x, uint16_t y) {
	return TSMS_ILI9341_setCursor(screen, x, y);
}