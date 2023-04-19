#include "screen/tsms_ili9341.h"

// 0x9341
uint16_t TSMS_ILI9341_readId(TSMS_SCHP screen) {
	__internal_tsms_lcd_write_command(screen, 0XD3);
	uint16_t id;
	id = __internal_tsms_lcd_read_data(screen);
	id = __internal_tsms_lcd_read_data(screen);
	id = __internal_tsms_lcd_read_data(screen);
	id <<= 8;
	id |= __internal_tsms_lcd_read_data(screen);
	return id;
}

TSMS_RESULT TSMS_ILI9341_init(TSMS_SCHP screen, void * op) {
	__internal_tsms_lcd_write_command(screen, 0xCF);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0xC1);
	__internal_tsms_lcd_write_data(screen, 0X30);
	__internal_tsms_lcd_write_command(screen, 0xED);
	__internal_tsms_lcd_write_data(screen, 0x64);
	__internal_tsms_lcd_write_data(screen, 0x03);
	__internal_tsms_lcd_write_data(screen, 0X12);
	__internal_tsms_lcd_write_data(screen, 0X81);
	__internal_tsms_lcd_write_command(screen, 0xE8);
	__internal_tsms_lcd_write_data(screen, 0x85);
	__internal_tsms_lcd_write_data(screen, 0x10);
	__internal_tsms_lcd_write_data(screen, 0x7A);
	__internal_tsms_lcd_write_command(screen, 0xCB);
	__internal_tsms_lcd_write_data(screen, 0x39);
	__internal_tsms_lcd_write_data(screen, 0x2C);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x34);
	__internal_tsms_lcd_write_data(screen, 0x02);
	__internal_tsms_lcd_write_command(screen, 0xF7);
	__internal_tsms_lcd_write_data(screen, 0x20);
	__internal_tsms_lcd_write_command(screen, 0xEA);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_command(screen, 0xC0);    //Power control
	__internal_tsms_lcd_write_data(screen, 0x1B);           //VRH[5:0]
	__internal_tsms_lcd_write_command(screen, 0xC1);    //Power control
	__internal_tsms_lcd_write_data(screen, 0x01);           //SAP[2:0];BT[3:0]
	__internal_tsms_lcd_write_command(screen, 0xC5);    //VCM control
	__internal_tsms_lcd_write_data(screen, 0x30);           //3F
	__internal_tsms_lcd_write_data(screen, 0x30);           //3C
	__internal_tsms_lcd_write_command(screen, 0xC7);    //VCM control2
	__internal_tsms_lcd_write_data(screen, 0XB7);
	__internal_tsms_lcd_write_command(screen, 0x36);    // Memory Access Control
	__internal_tsms_lcd_write_data(screen, 0x48);
	__internal_tsms_lcd_write_command(screen, 0x3A);
	__internal_tsms_lcd_write_data(screen, 0x55);
	__internal_tsms_lcd_write_command(screen, 0xB1);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x1A);
	__internal_tsms_lcd_write_command(screen, 0xB6);    // Display Function Control
	__internal_tsms_lcd_write_data(screen, 0x0A);
	__internal_tsms_lcd_write_data(screen, 0xA2);
	__internal_tsms_lcd_write_command(screen, 0xF2);    // 3Gamma Function Disable
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_command(screen, 0x26);    //Gamma curve selected
	__internal_tsms_lcd_write_data(screen, 0x01);
	__internal_tsms_lcd_write_command(screen, 0xE0);    //Set Gamma
	__internal_tsms_lcd_write_data(screen, 0x0F);
	__internal_tsms_lcd_write_data(screen, 0x2A);
	__internal_tsms_lcd_write_data(screen, 0x28);
	__internal_tsms_lcd_write_data(screen, 0x08);
	__internal_tsms_lcd_write_data(screen, 0x0E);
	__internal_tsms_lcd_write_data(screen, 0x08);
	__internal_tsms_lcd_write_data(screen, 0x54);
	__internal_tsms_lcd_write_data(screen, 0XA9);
	__internal_tsms_lcd_write_data(screen, 0x43);
	__internal_tsms_lcd_write_data(screen, 0x0A);
	__internal_tsms_lcd_write_data(screen, 0x0F);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_command(screen, 0XE1);    //Set Gamma
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x15);
	__internal_tsms_lcd_write_data(screen, 0x17);
	__internal_tsms_lcd_write_data(screen, 0x07);
	__internal_tsms_lcd_write_data(screen, 0x11);
	__internal_tsms_lcd_write_data(screen, 0x06);
	__internal_tsms_lcd_write_data(screen, 0x2B);
	__internal_tsms_lcd_write_data(screen, 0x56);
	__internal_tsms_lcd_write_data(screen, 0x3C);
	__internal_tsms_lcd_write_data(screen, 0x05);
	__internal_tsms_lcd_write_data(screen, 0x10);
	__internal_tsms_lcd_write_data(screen, 0x0F);
	__internal_tsms_lcd_write_data(screen, 0x3F);
	__internal_tsms_lcd_write_data(screen, 0x3F);
	__internal_tsms_lcd_write_data(screen, 0x0F);
	__internal_tsms_lcd_write_command(screen, 0x2B);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x01);
	__internal_tsms_lcd_write_data(screen, 0x3f);
	__internal_tsms_lcd_write_command(screen, 0x2A);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0xef);
	__internal_tsms_lcd_write_command(screen, 0x11);    //Exit Sleep
	TSMS_delay(120);
	__internal_tsms_lcd_write_command(screen, 0x29);    //display on
	return TSMS_SUCCESS;
}