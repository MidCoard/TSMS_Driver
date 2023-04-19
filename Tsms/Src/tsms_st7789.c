#include "screen/tsms_st7789.h"

// 0x8552
uint16_t TSMS_ST7789_readId(TSMS_SCHP screen) {
	__internal_tsms_lcd_write_command(screen, 0X04);
	uint16_t id;
	id = __internal_tsms_lcd_read_data(screen);
	id = __internal_tsms_lcd_read_data(screen);
	id = __internal_tsms_lcd_read_data(screen);
	id <<= 8;
	id |= __internal_tsms_lcd_read_data(screen);
	return id;
}

TSMS_RESULT TSMS_ST7789_init(TSMS_SCHP screen, void* option) {
	__internal_tsms_lcd_write_command(screen, 0x11);

	TSMS_delay(120);

	__internal_tsms_lcd_write_command(screen, 0x36);
	__internal_tsms_lcd_write_data(screen, 0x00);


	__internal_tsms_lcd_write_command(screen, 0x3A);
	__internal_tsms_lcd_write_data(screen, 0X05);

	__internal_tsms_lcd_write_command(screen, 0xB2);
	__internal_tsms_lcd_write_data(screen, 0x0C);
	__internal_tsms_lcd_write_data(screen, 0x0C);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x33);
	__internal_tsms_lcd_write_data(screen, 0x33);

	__internal_tsms_lcd_write_command(screen, 0xB7);
	__internal_tsms_lcd_write_data(screen, 0x35);

	__internal_tsms_lcd_write_command(screen, 0xBB);       //vcom
	__internal_tsms_lcd_write_data(screen, 0x32);               //30

	__internal_tsms_lcd_write_command(screen, 0xC0);
	__internal_tsms_lcd_write_data(screen, 0x0C);

	__internal_tsms_lcd_write_command(screen, 0xC2);
	__internal_tsms_lcd_write_data(screen, 0x01);

	__internal_tsms_lcd_write_command(screen, 0xC3);       //vrh
	__internal_tsms_lcd_write_data(screen, 0x10);               //17 0D

	__internal_tsms_lcd_write_command(screen, 0xC4);       //vdv
	__internal_tsms_lcd_write_data(screen, 0x20);      //20

	__internal_tsms_lcd_write_command(screen, 0xC6);
	__internal_tsms_lcd_write_data(screen, 0x0f);

	__internal_tsms_lcd_write_command(screen, 0xD0);
	__internal_tsms_lcd_write_data(screen, 0xA4);
	__internal_tsms_lcd_write_data(screen, 0xA1);

	__internal_tsms_lcd_write_command(screen, 0xE0);       //Set Gamma
	__internal_tsms_lcd_write_data(screen, 0xd0);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x02);
	__internal_tsms_lcd_write_data(screen, 0x07);
	__internal_tsms_lcd_write_data(screen, 0x0a);
	__internal_tsms_lcd_write_data(screen, 0x28);
	__internal_tsms_lcd_write_data(screen, 0x32);
	__internal_tsms_lcd_write_data(screen, 0X44);
	__internal_tsms_lcd_write_data(screen, 0x42);
	__internal_tsms_lcd_write_data(screen, 0x06);
	__internal_tsms_lcd_write_data(screen, 0x0e);
	__internal_tsms_lcd_write_data(screen, 0x12);
	__internal_tsms_lcd_write_data(screen, 0x14);
	__internal_tsms_lcd_write_data(screen, 0x17);


	__internal_tsms_lcd_write_command(screen, 0XE1);       //Set Gamma
	__internal_tsms_lcd_write_data(screen, 0xd0);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x02);
	__internal_tsms_lcd_write_data(screen, 0x07);
	__internal_tsms_lcd_write_data(screen, 0x0a);
	__internal_tsms_lcd_write_data(screen, 0x28);
	__internal_tsms_lcd_write_data(screen, 0x31);
	__internal_tsms_lcd_write_data(screen, 0x54);
	__internal_tsms_lcd_write_data(screen, 0x47);
	__internal_tsms_lcd_write_data(screen, 0x0e);
	__internal_tsms_lcd_write_data(screen, 0x1c);
	__internal_tsms_lcd_write_data(screen, 0x17);
	__internal_tsms_lcd_write_data(screen, 0x1b);
	__internal_tsms_lcd_write_data(screen, 0x1e);

	__internal_tsms_lcd_write_command(screen, 0x2A);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0xef);

	__internal_tsms_lcd_write_command(screen, 0x2B);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x01);
	__internal_tsms_lcd_write_data(screen, 0x3f);

	__internal_tsms_lcd_write_command(screen, 0x29);       //display on
	return TSMS_SUCCESS;
}