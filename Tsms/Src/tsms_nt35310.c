#include "screen/tsms_nt35310.h"
#include "screen/tsms_ili9341.h"

// 0X5310
uint16_t TSMS_NT35310_readId(TSMS_SCHP screen) {
	TSMS_SCREEN_writeCommand(screen, 0XD4);
	uint16_t id;
	id = TSMS_SCREEN_readData(screen);
	id = TSMS_SCREEN_readData(screen);
	id = TSMS_SCREEN_readData(screen);
	id <<= 8;
	id |= TSMS_SCREEN_readData(screen);
	return id;
}

TSMS_RESULT TSMS_NT35310_init(TSMS_SCHP screen, void* option) {
	TSMS_SCREEN_writeCommand(screen, 0xED);
	TSMS_SCREEN_writeData(screen, 0x01);
	TSMS_SCREEN_writeData(screen, 0xFE);

	TSMS_SCREEN_writeCommand(screen, 0xEE);
	TSMS_SCREEN_writeData(screen, 0xDE);
	TSMS_SCREEN_writeData(screen, 0x21);

	TSMS_SCREEN_writeCommand(screen, 0xF1);
	TSMS_SCREEN_writeData(screen, 0x01);
	TSMS_SCREEN_writeCommand(screen, 0xDF);
	TSMS_SCREEN_writeData(screen, 0x10);

	//VCOMvoltage//
	TSMS_SCREEN_writeCommand(screen, 0xC4);
	TSMS_SCREEN_writeData(screen, 0x8F);      //5f

	TSMS_SCREEN_writeCommand(screen, 0xC6);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xE2);
	TSMS_SCREEN_writeData(screen, 0xE2);
	TSMS_SCREEN_writeData(screen, 0xE2);
	TSMS_SCREEN_writeCommand(screen, 0xBF);
	TSMS_SCREEN_writeData(screen, 0xAA);

	TSMS_SCREEN_writeCommand(screen, 0xB0);
	TSMS_SCREEN_writeData(screen, 0x0D);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x0D);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x11);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x19);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x21);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x2D);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x3D);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x5D);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x5D);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xB1);
	TSMS_SCREEN_writeData(screen, 0x80);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x8B);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x96);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xB2);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x02);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x03);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xB3);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xB4);
	TSMS_SCREEN_writeData(screen, 0x8B);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x96);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xA1);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xB5);
	TSMS_SCREEN_writeData(screen, 0x02);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x03);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x04);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xB6);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xB7);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x3F);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x5E);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x64);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x8C);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xAC);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xDC);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x70);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x90);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xEB);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xDC);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xB8);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xBA);
	TSMS_SCREEN_writeData(screen, 0x24);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xC1);
	TSMS_SCREEN_writeData(screen, 0x20);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x54);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xFF);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xC2);
	TSMS_SCREEN_writeData(screen, 0x0A);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x04);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xC3);
	TSMS_SCREEN_writeData(screen, 0x3C);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x3A);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x39);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x37);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x3C);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x36);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x32);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x2F);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x2C);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x29);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x26);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x24);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x24);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x23);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x3C);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x36);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x32);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x2F);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x2C);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x29);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x26);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x24);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x24);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x23);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xC4);
	TSMS_SCREEN_writeData(screen, 0x62);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x05);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x84);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xF0);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x18);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xA4);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x18);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x50);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x0C);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x17);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x95);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xF3);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xE6);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xC5);
	TSMS_SCREEN_writeData(screen, 0x32);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x44);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x65);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x76);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x88);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xC6);
	TSMS_SCREEN_writeData(screen, 0x20);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x17);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x01);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xC7);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xC8);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xC9);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xE0);
	TSMS_SCREEN_writeData(screen, 0x16);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x1C);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x21);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x36);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x46);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x52);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x64);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x7A);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x8B);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x99);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xA8);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xB9);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xC4);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xCA);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xD2);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xD9);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xE0);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xF3);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xE1);
	TSMS_SCREEN_writeData(screen, 0x16);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x1C);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x22);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x36);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x45);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x52);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x64);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x7A);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x8B);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x99);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xA8);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xB9);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xC4);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xCA);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xD2);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xD8);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xE0);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xF3);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xE2);
	TSMS_SCREEN_writeData(screen, 0x05);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x0B);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x1B);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x34);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x44);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x4F);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x61);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x79);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x88);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x97);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xA6);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xB7);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xC2);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xC7);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xD1);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xD6);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xDD);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xF3);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeCommand(screen, 0xE3);
	TSMS_SCREEN_writeData(screen, 0x05);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xA);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x1C);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x33);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x44);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x50);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x62);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x78);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x88);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x97);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xA6);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xB7);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xC2);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xC7);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xD1);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xD5);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xDD);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xF3);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xE4);
	TSMS_SCREEN_writeData(screen, 0x01);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x01);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x02);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x2A);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x3C);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x4B);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x5D);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x74);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x84);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x93);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xA2);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xB3);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xBE);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xC4);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xCD);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xD3);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xDD);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xF3);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeCommand(screen, 0xE5);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x02);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x29);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x3C);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x4B);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x5D);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x74);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x84);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x93);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xA2);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xB3);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xBE);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xC4);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xCD);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xD3);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xDC);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xF3);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xE6);
	TSMS_SCREEN_writeData(screen, 0x11);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x34);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x56);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x76);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x77);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x66);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x88);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x99);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xBB);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x99);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x66);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x55);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x55);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x45);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x43);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x44);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xE7);
	TSMS_SCREEN_writeData(screen, 0x32);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x55);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x76);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x66);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x67);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x67);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x87);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x99);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xBB);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x99);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x77);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x44);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x56);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x23);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x33);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x45);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xE8);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x99);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x87);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x88);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x77);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x66);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x88);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xAA);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xBB);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x99);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x66);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x55);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x55);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x44);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x44);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x55);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xE9);
	TSMS_SCREEN_writeData(screen, 0xAA);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0xAA);

	TSMS_SCREEN_writeCommand(screen, 0xCF);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xF0);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x50);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xF3);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xF9);
	TSMS_SCREEN_writeData(screen, 0x06);
	TSMS_SCREEN_writeData(screen, 0x10);
	TSMS_SCREEN_writeData(screen, 0x29);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0x3A);
	TSMS_SCREEN_writeData(screen, 0x55);    //66

	TSMS_SCREEN_writeCommand(screen, 0x11);
	TSMS_delay(100);
	TSMS_SCREEN_writeCommand(screen, 0x29);
	TSMS_SCREEN_writeCommand(screen, 0x35);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0x51);
	TSMS_SCREEN_writeData(screen, 0xFF);
	TSMS_SCREEN_writeCommand(screen, 0x53);
	TSMS_SCREEN_writeData(screen, 0x2C);
	TSMS_SCREEN_writeCommand(screen, 0x55);
	TSMS_SCREEN_writeData(screen, 0x82);
	TSMS_SCREEN_writeCommand(screen, 0x2c);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_NT35310_setDisplayDirection(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction) {
	return TSMS_ILI9341_setDisplayDirection(screen, direction);
}

TSMS_RESULT TSMS_NT35310_setScanDirection(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction) {
	TSMS_SCAN_DIRECTION dir = direction;
	if (screen->displayDirection == TSMS_DISPLAY_DIRECTION_HORIZONTAL)
		switch (direction) {
			case TSMS_SCAN_DIRECTION_L2R_U2D:
				dir = 6;
				break;
			case TSMS_SCAN_DIRECTION_L2R_D2U:
				dir = 7;
				break;
			case TSMS_SCAN_DIRECTION_R2L_U2D:
				dir = 4;
				break;
			case TSMS_SCAN_DIRECTION_R2L_D2U:
				dir = 5;
				break;
			case TSMS_SCAN_DIRECTION_U2D_L2R:
				dir = 1;
				break;
			case TSMS_SCAN_DIRECTION_U2D_R2L:
				dir = 0;
				break;
			case TSMS_SCAN_DIRECTION_D2U_L2R:
				dir = 3;
				break;
			case TSMS_SCAN_DIRECTION_D2U_R2L:
				dir = 2;
				break;
		}
	uint16_t reg = 0x36;
	uint16_t value = 0;
	switch (dir) {
		case TSMS_SCAN_DIRECTION_L2R_U2D:
			value |= (0 << 7) | (0 << 6) | (0 << 5);
			break;
		case TSMS_SCAN_DIRECTION_L2R_D2U:
			value |= (1 << 7) | (0 << 6) | (0 << 5);
			break;
		case TSMS_SCAN_DIRECTION_R2L_U2D:
			value |= (0 << 7) | (1 << 6) | (0 << 5);
			break;
		case TSMS_SCAN_DIRECTION_R2L_D2U:
			value |= (1 << 7) | (1 << 6) | (0 << 5);
			break;
		case TSMS_SCAN_DIRECTION_U2D_L2R:
			value |= (0 << 7) | (0 << 6) | (1 << 5);
			break;
		case TSMS_SCAN_DIRECTION_U2D_R2L:
			value |= (0 << 7) | (1 << 6) | (1 << 5);
			break;
		case TSMS_SCAN_DIRECTION_D2U_L2R:
			value |= (1 << 7) | (0 << 6) | (1 << 5);
			break;
		case TSMS_SCAN_DIRECTION_D2U_R2L:
			value |= (1 << 7) | (1 << 6) | (1 << 5);
			break;
	}
	TSMS_SCREEN_writeRegister(screen, reg, value);
	if (value & 0X20) {
		if (screen->width < screen->height)
			TSMS_UTIL_swapUnsignedShort(&screen->width, &screen->height);
	} else if (screen->width > screen->height)
		TSMS_UTIL_swapUnsignedShort(&screen->width, &screen->height);

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

TSMS_RESULT TSMS_NT35310_setCursor(TSMS_SCHP screen, uint16_t x, uint16_t y) {
	return TSMS_ILI9341_setCursor(screen, x, y);
}