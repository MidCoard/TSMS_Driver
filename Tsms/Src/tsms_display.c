#include "tsms_display.h"

TSMS_INLINE void __internal_tsms_swap(uint16_t *a, uint16_t *b) {
	uint16_t t = *a;
	*a = *b;
	*b = t;
}

TSMS_INLINE void __internal_tsms_lcd_write_register(TSMS_SCHP screen, uint16_t reg, uint16_t value) {
	*screen->command = reg;
	*screen->data = value;
}

TSMS_INLINE void __internal_tsms_lcd_write_command(TSMS_SCHP screen, volatile uint16_t command) {
	*screen->command = command;
}

TSMS_INLINE volatile uint16_t __internal_tsms_lcd_read_data(TSMS_SCHP screen) {
	return *screen->data;
}

TSMS_INLINE void __internal_tsms_lcd_write_data(TSMS_SCHP screen, volatile uint16_t data) {
	*screen->data = data;
}

// 0x9341
TSMS_INLINE uint16_t __internal_tsms_read_ili9341_id(TSMS_SCHP screen) {
	__internal_tsms_lcd_write_command(screen, 0XD3);
	uint16_t id;
	id = __internal_tsms_lcd_read_data(screen);
	id = __internal_tsms_lcd_read_data(screen);
	id = __internal_tsms_lcd_read_data(screen);
	id <<= 8;
	id |= __internal_tsms_lcd_read_data(screen);
	return id;
}

// 0x8552
TSMS_INLINE uint16_t __internal_tsms_read_st7789_id(TSMS_SCHP screen) {
	__internal_tsms_lcd_write_command(screen, 0X04);
	uint16_t id;
	id = __internal_tsms_lcd_read_data(screen);
	id = __internal_tsms_lcd_read_data(screen);
	id = __internal_tsms_lcd_read_data(screen);
	id <<= 8;
	id |= __internal_tsms_lcd_read_data(screen);
	return id;
}

// 0X5310
TSMS_INLINE uint16_t __internal_tsms_read_nt35310_id(TSMS_SCHP screen) {
	__internal_tsms_lcd_write_command(screen, 0XD4);
	uint16_t id;
	id = __internal_tsms_lcd_read_data(screen);
	id = __internal_tsms_lcd_read_data(screen);
	id = __internal_tsms_lcd_read_data(screen);
	id <<= 8;
	id |= __internal_tsms_lcd_read_data(screen);
	return id;
}

// 0x8000 or 0x5510
TSMS_INLINE uint16_t __internal_tsms_read_nt35510_nt5510_id(TSMS_SCHP screen) {
	__internal_tsms_lcd_write_register(screen, 0xF000, 0x0055);
	__internal_tsms_lcd_write_register(screen, 0xF001, 0x00AA);
	__internal_tsms_lcd_write_register(screen, 0xF002, 0x0052);
	__internal_tsms_lcd_write_register(screen, 0xF003, 0x0008);
	__internal_tsms_lcd_write_register(screen, 0xF004, 0x0001);
	uint16_t id;
	__internal_tsms_lcd_write_command(screen, 0xC500);
	id = __internal_tsms_lcd_read_data(screen);
	id <<= 8;
	__internal_tsms_lcd_write_command(screen, 0xC501);
	id |= __internal_tsms_lcd_read_data(screen);
	return id;
}

//0X5761
TSMS_INLINE uint16_t __internal_tsms_read_ssd1963_id(TSMS_SCHP screen) {
	__internal_tsms_lcd_write_command(screen, 0XA1);
	uint16_t id;
	id = __internal_tsms_lcd_read_data(screen);
	id = __internal_tsms_lcd_read_data(screen);
	id = __internal_tsms_lcd_read_data(screen);
	id <<= 8;
	id |= __internal_tsms_lcd_read_data(screen);
	return id;
}

TSMS_INLINE void __internal_tsms_set_ssd1963_light(TSMS_SCHP screen, uint8_t light) {
	__internal_tsms_lcd_write_command(screen, 0xBE);
	__internal_tsms_lcd_write_data(screen, 0x05);
	__internal_tsms_lcd_write_data(screen, light);
	__internal_tsms_lcd_write_data(screen, 0x01);
	__internal_tsms_lcd_write_data(screen, 0xFF);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x00);
}

TSMS_INLINE TSMS_RESULT __tsms_internal_reset_gt9147(TSMS_THP touch) {
	TSMS_GT9147 gt9147 = touch->custom;
	TSMS_GPIO_write(gt9147->reset, TSMS_GPIO_LOW);
	TSMS_delay(1);
	TSMS_GPIO_write(gt9147->reset, TSMS_GPIO_HIGH);
	return TSMS_SUCCESS;
}

TSMS_SCHP
TSMS_SCREEN_create16BitHandler(uint16_t *command, uint16_t *data, TSMS_GHP bg, TSMS_SCREEN_TYPE type, uint16_t width,
                               uint16_t height, TSMS_SSD1963_OP option) {
	TSMS_SCHP screen = malloc(sizeof(struct TSMS_SCREEN_HANDLER));
	if (screen == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_SCHP");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	screen->reset = TSMS_NULL;
	screen->command = command;
	screen->data = data;
	screen->bg = bg;
	screen->type = TSMS_SCREEN_AUTO_DETECT;
	screen->defaultWidth = width;
	screen->defaultHeight = height;
	TSMS_delay(50);
	uint16_t id;
	if (type == TSMS_SCREEN_AUTO_DETECT) {
		id = __internal_tsms_read_ili9341_id(screen);
		if (id == TSMS_ILI9341_ID)
			screen->type = TSMS_SCREEN_ILI9341;
		else {
			TSMS_delay(5);
			id = __internal_tsms_read_st7789_id(screen);
			if (id == TSMS_ST7789_ID)
				screen->type = TSMS_SCREEN_ST7789;
			else {
				TSMS_delay(5);
				id = __internal_tsms_read_nt35310_id(screen);
				if (id == TSMS_NT35310_ID)
					screen->type = TSMS_SCREEN_NT35310;
				else {
					TSMS_delay(5);
					id = __internal_tsms_read_nt35510_nt5510_id(screen);
					if (id == TSMS_NT35510_ID)
						screen->type = TSMS_SCREEN_NT35510;
					else if (id == TSMS_NT5510_ID)
						screen->type = TSMS_SCREEN_NT5510;
					else {
						TSMS_delay(5);
						id = __internal_tsms_read_ssd1963_id(screen);
						if (id == TSMS_SSD1963_ID)
							screen->type = TSMS_SCREEN_SSD1963;
					}
				}
			}
		}
	} else if (type == TSMS_SCREEN_ILI9341) {
		id = __internal_tsms_read_ili9341_id(screen);
		if (id == TSMS_ILI9341_ID)
			screen->type = TSMS_SCREEN_ILI9341;
	} else if (type == TSMS_SCREEN_ST7789) {
		id = __internal_tsms_read_st7789_id(screen);
		if (id == TSMS_ST7789_ID)
			screen->type = TSMS_SCREEN_ST7789;
	} else if (type == TSMS_SCREEN_NT35310) {
		id = __internal_tsms_read_nt35310_id(screen);
		if (id == TSMS_NT35310_ID)
			screen->type = TSMS_SCREEN_NT35310;
	} else if (type == TSMS_SCREEN_NT35510) {
		id = __internal_tsms_read_nt35510_nt5510_id(screen);
		if (id == TSMS_NT35510_ID)
			screen->type = TSMS_SCREEN_NT35510;
	} else if (type == TSMS_SCREEN_NT5510) {
		id = __internal_tsms_read_nt35510_nt5510_id(screen);
		if (id == TSMS_NT5510_ID)
			screen->type = TSMS_SCREEN_NT5510;
	} else if (type == TSMS_SCREEN_SSD1963) {
		id = __internal_tsms_read_ssd1963_id(screen);
		if (id == TSMS_SSD1963_ID)
			screen->type = TSMS_SCREEN_SSD1963;
	}
	if (screen->type != TSMS_SCREEN_AUTO_DETECT) {
		if (screen->type == TSMS_SCREEN_ILI9341) {
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
		} else if (screen->type == TSMS_SCREEN_ST7789) {
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
		} else if (screen->type == TSMS_SCREEN_NT35310) {
			__internal_tsms_lcd_write_command(screen, 0xED);
			__internal_tsms_lcd_write_data(screen, 0x01);
			__internal_tsms_lcd_write_data(screen, 0xFE);

			__internal_tsms_lcd_write_command(screen, 0xEE);
			__internal_tsms_lcd_write_data(screen, 0xDE);
			__internal_tsms_lcd_write_data(screen, 0x21);

			__internal_tsms_lcd_write_command(screen, 0xF1);
			__internal_tsms_lcd_write_data(screen, 0x01);
			__internal_tsms_lcd_write_command(screen, 0xDF);
			__internal_tsms_lcd_write_data(screen, 0x10);

			//VCOMvoltage//
			__internal_tsms_lcd_write_command(screen, 0xC4);
			__internal_tsms_lcd_write_data(screen, 0x8F);      //5f

			__internal_tsms_lcd_write_command(screen, 0xC6);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xE2);
			__internal_tsms_lcd_write_data(screen, 0xE2);
			__internal_tsms_lcd_write_data(screen, 0xE2);
			__internal_tsms_lcd_write_command(screen, 0xBF);
			__internal_tsms_lcd_write_data(screen, 0xAA);

			__internal_tsms_lcd_write_command(screen, 0xB0);
			__internal_tsms_lcd_write_data(screen, 0x0D);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x0D);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x11);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x19);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x21);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x2D);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x3D);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x5D);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x5D);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xB1);
			__internal_tsms_lcd_write_data(screen, 0x80);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x8B);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x96);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xB2);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x02);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x03);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xB3);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xB4);
			__internal_tsms_lcd_write_data(screen, 0x8B);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x96);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xA1);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xB5);
			__internal_tsms_lcd_write_data(screen, 0x02);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x03);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x04);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xB6);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xB7);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x3F);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x5E);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x64);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x8C);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xAC);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xDC);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x70);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x90);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xEB);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xDC);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xB8);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xBA);
			__internal_tsms_lcd_write_data(screen, 0x24);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xC1);
			__internal_tsms_lcd_write_data(screen, 0x20);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x54);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xFF);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xC2);
			__internal_tsms_lcd_write_data(screen, 0x0A);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x04);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xC3);
			__internal_tsms_lcd_write_data(screen, 0x3C);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x3A);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x39);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x37);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x3C);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x36);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x32);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x2F);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x2C);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x29);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x26);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x24);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x24);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x23);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x3C);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x36);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x32);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x2F);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x2C);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x29);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x26);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x24);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x24);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x23);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xC4);
			__internal_tsms_lcd_write_data(screen, 0x62);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x05);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x84);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xF0);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x18);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xA4);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x18);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x50);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x0C);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x17);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x95);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xF3);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xE6);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xC5);
			__internal_tsms_lcd_write_data(screen, 0x32);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x44);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x65);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x76);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x88);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xC6);
			__internal_tsms_lcd_write_data(screen, 0x20);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x17);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x01);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xC7);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xC8);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xC9);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xE0);
			__internal_tsms_lcd_write_data(screen, 0x16);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x1C);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x21);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x36);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x46);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x52);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x64);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x7A);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x8B);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x99);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xA8);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xB9);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xC4);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xCA);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xD2);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xD9);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xE0);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xF3);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xE1);
			__internal_tsms_lcd_write_data(screen, 0x16);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x1C);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x22);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x36);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x45);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x52);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x64);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x7A);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x8B);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x99);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xA8);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xB9);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xC4);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xCA);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xD2);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xD8);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xE0);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xF3);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xE2);
			__internal_tsms_lcd_write_data(screen, 0x05);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x0B);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x1B);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x34);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x44);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x4F);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x61);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x79);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x88);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x97);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xA6);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xB7);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xC2);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xC7);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xD1);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xD6);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xDD);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xF3);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_command(screen, 0xE3);
			__internal_tsms_lcd_write_data(screen, 0x05);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xA);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x1C);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x33);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x44);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x50);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x62);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x78);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x88);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x97);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xA6);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xB7);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xC2);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xC7);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xD1);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xD5);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xDD);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xF3);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xE4);
			__internal_tsms_lcd_write_data(screen, 0x01);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x01);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x02);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x2A);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x3C);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x4B);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x5D);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x74);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x84);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x93);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xA2);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xB3);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xBE);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xC4);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xCD);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xD3);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xDD);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xF3);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_command(screen, 0xE5);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x02);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x29);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x3C);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x4B);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x5D);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x74);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x84);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x93);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xA2);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xB3);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xBE);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xC4);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xCD);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xD3);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xDC);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xF3);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xE6);
			__internal_tsms_lcd_write_data(screen, 0x11);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x34);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x56);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x76);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x77);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x66);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x88);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x99);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xBB);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x99);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x66);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x55);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x55);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x45);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x43);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x44);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xE7);
			__internal_tsms_lcd_write_data(screen, 0x32);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x55);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x76);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x66);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x67);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x67);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x87);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x99);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xBB);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x99);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x77);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x44);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x56);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x23);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x33);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x45);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xE8);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x99);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x87);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x88);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x77);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x66);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x88);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xAA);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xBB);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x99);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x66);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x55);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x55);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x44);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x44);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x55);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xE9);
			__internal_tsms_lcd_write_data(screen, 0xAA);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0xAA);

			__internal_tsms_lcd_write_command(screen, 0xCF);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xF0);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x50);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xF3);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xF9);
			__internal_tsms_lcd_write_data(screen, 0x06);
			__internal_tsms_lcd_write_data(screen, 0x10);
			__internal_tsms_lcd_write_data(screen, 0x29);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0x3A);
			__internal_tsms_lcd_write_data(screen, 0x55);    //66

			__internal_tsms_lcd_write_command(screen, 0x11);
			TSMS_delay(100);
			__internal_tsms_lcd_write_command(screen, 0x29);
			__internal_tsms_lcd_write_command(screen, 0x35);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0x51);
			__internal_tsms_lcd_write_data(screen, 0xFF);
			__internal_tsms_lcd_write_command(screen, 0x53);
			__internal_tsms_lcd_write_data(screen, 0x2C);
			__internal_tsms_lcd_write_command(screen, 0x55);
			__internal_tsms_lcd_write_data(screen, 0x82);
			__internal_tsms_lcd_write_command(screen, 0x2c);
		} else if (screen->type == TSMS_SCREEN_NT35510 || screen->type == TSMS_SCREEN_NT5510) {
			__internal_tsms_lcd_write_register(screen, 0xF000, 0x55);
			__internal_tsms_lcd_write_register(screen, 0xF001, 0xAA);
			__internal_tsms_lcd_write_register(screen, 0xF002, 0x52);
			__internal_tsms_lcd_write_register(screen, 0xF003, 0x08);
			__internal_tsms_lcd_write_register(screen, 0xF004, 0x01);
			//AVDD Set AVDD 5.2V
			__internal_tsms_lcd_write_register(screen, 0xB000, 0x0D);
			__internal_tsms_lcd_write_register(screen, 0xB001, 0x0D);
			__internal_tsms_lcd_write_register(screen, 0xB002, 0x0D);
			//AVDD ratio
			__internal_tsms_lcd_write_register(screen, 0xB600, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xB601, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xB602, 0x34);
			//AVEE -5.2V
			__internal_tsms_lcd_write_register(screen, 0xB100, 0x0D);
			__internal_tsms_lcd_write_register(screen, 0xB101, 0x0D);
			__internal_tsms_lcd_write_register(screen, 0xB102, 0x0D);
			//AVEE ratio
			__internal_tsms_lcd_write_register(screen, 0xB700, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xB701, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xB702, 0x34);
			//VCL -2.5V
			__internal_tsms_lcd_write_register(screen, 0xB200, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xB201, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xB202, 0x00);
			//VCL ratio
			__internal_tsms_lcd_write_register(screen, 0xB800, 0x24);
			__internal_tsms_lcd_write_register(screen, 0xB801, 0x24);
			__internal_tsms_lcd_write_register(screen, 0xB802, 0x24);
			//VGH 15V (Free pump)
			__internal_tsms_lcd_write_register(screen, 0xBF00, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xB300, 0x0F);
			__internal_tsms_lcd_write_register(screen, 0xB301, 0x0F);
			__internal_tsms_lcd_write_register(screen, 0xB302, 0x0F);
			//VGH ratio
			__internal_tsms_lcd_write_register(screen, 0xB900, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xB901, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xB902, 0x34);
			//VGL_REG -10V
			__internal_tsms_lcd_write_register(screen, 0xB500, 0x08);
			__internal_tsms_lcd_write_register(screen, 0xB501, 0x08);
			__internal_tsms_lcd_write_register(screen, 0xB502, 0x08);
			__internal_tsms_lcd_write_register(screen, 0xC200, 0x03);
			//VGLX ratio
			__internal_tsms_lcd_write_register(screen, 0xBA00, 0x24);
			__internal_tsms_lcd_write_register(screen, 0xBA01, 0x24);
			__internal_tsms_lcd_write_register(screen, 0xBA02, 0x24);
			//VGMP/VGSP 4.5V/0V
			__internal_tsms_lcd_write_register(screen, 0xBC00, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xBC01, 0x78);
			__internal_tsms_lcd_write_register(screen, 0xBC02, 0x00);
			//VGMN/VGSN -4.5V/0V
			__internal_tsms_lcd_write_register(screen, 0xBD00, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xBD01, 0x78);
			__internal_tsms_lcd_write_register(screen, 0xBD02, 0x00);
			//VCOM
			__internal_tsms_lcd_write_register(screen, 0xBE00, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xBE01, 0x64);
			//Gamma Setting
			__internal_tsms_lcd_write_register(screen, 0xD100, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD101, 0x33);
			__internal_tsms_lcd_write_register(screen, 0xD102, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD103, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xD104, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD105, 0x3A);
			__internal_tsms_lcd_write_register(screen, 0xD106, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD107, 0x4A);
			__internal_tsms_lcd_write_register(screen, 0xD108, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD109, 0x5C);
			__internal_tsms_lcd_write_register(screen, 0xD10A, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD10B, 0x81);
			__internal_tsms_lcd_write_register(screen, 0xD10C, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD10D, 0xA6);
			__internal_tsms_lcd_write_register(screen, 0xD10E, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD10F, 0xE5);
			__internal_tsms_lcd_write_register(screen, 0xD110, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD111, 0x13);
			__internal_tsms_lcd_write_register(screen, 0xD112, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD113, 0x54);
			__internal_tsms_lcd_write_register(screen, 0xD114, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD115, 0x82);
			__internal_tsms_lcd_write_register(screen, 0xD116, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD117, 0xCA);
			__internal_tsms_lcd_write_register(screen, 0xD118, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD119, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD11A, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD11B, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD11C, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD11D, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xD11E, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD11F, 0x67);
			__internal_tsms_lcd_write_register(screen, 0xD120, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD121, 0x84);
			__internal_tsms_lcd_write_register(screen, 0xD122, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD123, 0xA4);
			__internal_tsms_lcd_write_register(screen, 0xD124, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD125, 0xB7);
			__internal_tsms_lcd_write_register(screen, 0xD126, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD127, 0xCF);
			__internal_tsms_lcd_write_register(screen, 0xD128, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD129, 0xDE);
			__internal_tsms_lcd_write_register(screen, 0xD12A, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD12B, 0xF2);
			__internal_tsms_lcd_write_register(screen, 0xD12C, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD12D, 0xFE);
			__internal_tsms_lcd_write_register(screen, 0xD12E, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD12F, 0x10);
			__internal_tsms_lcd_write_register(screen, 0xD130, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD131, 0x33);
			__internal_tsms_lcd_write_register(screen, 0xD132, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD133, 0x6D);
			__internal_tsms_lcd_write_register(screen, 0xD200, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD201, 0x33);
			__internal_tsms_lcd_write_register(screen, 0xD202, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD203, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xD204, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD205, 0x3A);
			__internal_tsms_lcd_write_register(screen, 0xD206, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD207, 0x4A);
			__internal_tsms_lcd_write_register(screen, 0xD208, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD209, 0x5C);
			__internal_tsms_lcd_write_register(screen, 0xD20A, 0x00);

			__internal_tsms_lcd_write_register(screen, 0xD20B, 0x81);
			__internal_tsms_lcd_write_register(screen, 0xD20C, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD20D, 0xA6);
			__internal_tsms_lcd_write_register(screen, 0xD20E, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD20F, 0xE5);
			__internal_tsms_lcd_write_register(screen, 0xD210, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD211, 0x13);
			__internal_tsms_lcd_write_register(screen, 0xD212, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD213, 0x54);
			__internal_tsms_lcd_write_register(screen, 0xD214, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD215, 0x82);
			__internal_tsms_lcd_write_register(screen, 0xD216, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD217, 0xCA);
			__internal_tsms_lcd_write_register(screen, 0xD218, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD219, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD21A, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD21B, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD21C, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD21D, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xD21E, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD21F, 0x67);
			__internal_tsms_lcd_write_register(screen, 0xD220, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD221, 0x84);
			__internal_tsms_lcd_write_register(screen, 0xD222, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD223, 0xA4);
			__internal_tsms_lcd_write_register(screen, 0xD224, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD225, 0xB7);
			__internal_tsms_lcd_write_register(screen, 0xD226, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD227, 0xCF);
			__internal_tsms_lcd_write_register(screen, 0xD228, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD229, 0xDE);
			__internal_tsms_lcd_write_register(screen, 0xD22A, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD22B, 0xF2);
			__internal_tsms_lcd_write_register(screen, 0xD22C, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD22D, 0xFE);
			__internal_tsms_lcd_write_register(screen, 0xD22E, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD22F, 0x10);
			__internal_tsms_lcd_write_register(screen, 0xD230, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD231, 0x33);
			__internal_tsms_lcd_write_register(screen, 0xD232, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD233, 0x6D);
			__internal_tsms_lcd_write_register(screen, 0xD300, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD301, 0x33);
			__internal_tsms_lcd_write_register(screen, 0xD302, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD303, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xD304, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD305, 0x3A);
			__internal_tsms_lcd_write_register(screen, 0xD306, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD307, 0x4A);
			__internal_tsms_lcd_write_register(screen, 0xD308, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD309, 0x5C);
			__internal_tsms_lcd_write_register(screen, 0xD30A, 0x00);

			__internal_tsms_lcd_write_register(screen, 0xD30B, 0x81);
			__internal_tsms_lcd_write_register(screen, 0xD30C, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD30D, 0xA6);
			__internal_tsms_lcd_write_register(screen, 0xD30E, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD30F, 0xE5);
			__internal_tsms_lcd_write_register(screen, 0xD310, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD311, 0x13);
			__internal_tsms_lcd_write_register(screen, 0xD312, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD313, 0x54);
			__internal_tsms_lcd_write_register(screen, 0xD314, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD315, 0x82);
			__internal_tsms_lcd_write_register(screen, 0xD316, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD317, 0xCA);
			__internal_tsms_lcd_write_register(screen, 0xD318, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD319, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD31A, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD31B, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD31C, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD31D, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xD31E, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD31F, 0x67);
			__internal_tsms_lcd_write_register(screen, 0xD320, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD321, 0x84);
			__internal_tsms_lcd_write_register(screen, 0xD322, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD323, 0xA4);
			__internal_tsms_lcd_write_register(screen, 0xD324, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD325, 0xB7);
			__internal_tsms_lcd_write_register(screen, 0xD326, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD327, 0xCF);
			__internal_tsms_lcd_write_register(screen, 0xD328, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD329, 0xDE);
			__internal_tsms_lcd_write_register(screen, 0xD32A, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD32B, 0xF2);
			__internal_tsms_lcd_write_register(screen, 0xD32C, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD32D, 0xFE);
			__internal_tsms_lcd_write_register(screen, 0xD32E, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD32F, 0x10);
			__internal_tsms_lcd_write_register(screen, 0xD330, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD331, 0x33);
			__internal_tsms_lcd_write_register(screen, 0xD332, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD333, 0x6D);
			__internal_tsms_lcd_write_register(screen, 0xD400, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD401, 0x33);
			__internal_tsms_lcd_write_register(screen, 0xD402, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD403, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xD404, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD405, 0x3A);
			__internal_tsms_lcd_write_register(screen, 0xD406, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD407, 0x4A);
			__internal_tsms_lcd_write_register(screen, 0xD408, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD409, 0x5C);
			__internal_tsms_lcd_write_register(screen, 0xD40A, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD40B, 0x81);

			__internal_tsms_lcd_write_register(screen, 0xD40C, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD40D, 0xA6);
			__internal_tsms_lcd_write_register(screen, 0xD40E, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD40F, 0xE5);
			__internal_tsms_lcd_write_register(screen, 0xD410, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD411, 0x13);
			__internal_tsms_lcd_write_register(screen, 0xD412, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD413, 0x54);
			__internal_tsms_lcd_write_register(screen, 0xD414, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD415, 0x82);
			__internal_tsms_lcd_write_register(screen, 0xD416, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD417, 0xCA);
			__internal_tsms_lcd_write_register(screen, 0xD418, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD419, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD41A, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD41B, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD41C, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD41D, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xD41E, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD41F, 0x67);
			__internal_tsms_lcd_write_register(screen, 0xD420, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD421, 0x84);
			__internal_tsms_lcd_write_register(screen, 0xD422, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD423, 0xA4);
			__internal_tsms_lcd_write_register(screen, 0xD424, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD425, 0xB7);
			__internal_tsms_lcd_write_register(screen, 0xD426, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD427, 0xCF);
			__internal_tsms_lcd_write_register(screen, 0xD428, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD429, 0xDE);
			__internal_tsms_lcd_write_register(screen, 0xD42A, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD42B, 0xF2);
			__internal_tsms_lcd_write_register(screen, 0xD42C, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD42D, 0xFE);
			__internal_tsms_lcd_write_register(screen, 0xD42E, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD42F, 0x10);
			__internal_tsms_lcd_write_register(screen, 0xD430, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD431, 0x33);
			__internal_tsms_lcd_write_register(screen, 0xD432, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD433, 0x6D);
			__internal_tsms_lcd_write_register(screen, 0xD500, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD501, 0x33);
			__internal_tsms_lcd_write_register(screen, 0xD502, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD503, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xD504, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD505, 0x3A);
			__internal_tsms_lcd_write_register(screen, 0xD506, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD507, 0x4A);
			__internal_tsms_lcd_write_register(screen, 0xD508, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD509, 0x5C);
			__internal_tsms_lcd_write_register(screen, 0xD50A, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD50B, 0x81);

			__internal_tsms_lcd_write_register(screen, 0xD50C, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD50D, 0xA6);
			__internal_tsms_lcd_write_register(screen, 0xD50E, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD50F, 0xE5);
			__internal_tsms_lcd_write_register(screen, 0xD510, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD511, 0x13);
			__internal_tsms_lcd_write_register(screen, 0xD512, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD513, 0x54);
			__internal_tsms_lcd_write_register(screen, 0xD514, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD515, 0x82);
			__internal_tsms_lcd_write_register(screen, 0xD516, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD517, 0xCA);
			__internal_tsms_lcd_write_register(screen, 0xD518, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD519, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD51A, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD51B, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD51C, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD51D, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xD51E, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD51F, 0x67);
			__internal_tsms_lcd_write_register(screen, 0xD520, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD521, 0x84);
			__internal_tsms_lcd_write_register(screen, 0xD522, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD523, 0xA4);
			__internal_tsms_lcd_write_register(screen, 0xD524, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD525, 0xB7);
			__internal_tsms_lcd_write_register(screen, 0xD526, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD527, 0xCF);
			__internal_tsms_lcd_write_register(screen, 0xD528, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD529, 0xDE);
			__internal_tsms_lcd_write_register(screen, 0xD52A, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD52B, 0xF2);
			__internal_tsms_lcd_write_register(screen, 0xD52C, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD52D, 0xFE);
			__internal_tsms_lcd_write_register(screen, 0xD52E, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD52F, 0x10);
			__internal_tsms_lcd_write_register(screen, 0xD530, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD531, 0x33);
			__internal_tsms_lcd_write_register(screen, 0xD532, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD533, 0x6D);
			__internal_tsms_lcd_write_register(screen, 0xD600, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD601, 0x33);
			__internal_tsms_lcd_write_register(screen, 0xD602, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD603, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xD604, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD605, 0x3A);
			__internal_tsms_lcd_write_register(screen, 0xD606, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD607, 0x4A);
			__internal_tsms_lcd_write_register(screen, 0xD608, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD609, 0x5C);
			__internal_tsms_lcd_write_register(screen, 0xD60A, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD60B, 0x81);

			__internal_tsms_lcd_write_register(screen, 0xD60C, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD60D, 0xA6);
			__internal_tsms_lcd_write_register(screen, 0xD60E, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD60F, 0xE5);
			__internal_tsms_lcd_write_register(screen, 0xD610, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD611, 0x13);
			__internal_tsms_lcd_write_register(screen, 0xD612, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD613, 0x54);
			__internal_tsms_lcd_write_register(screen, 0xD614, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD615, 0x82);
			__internal_tsms_lcd_write_register(screen, 0xD616, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD617, 0xCA);
			__internal_tsms_lcd_write_register(screen, 0xD618, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD619, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xD61A, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD61B, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xD61C, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD61D, 0x34);
			__internal_tsms_lcd_write_register(screen, 0xD61E, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD61F, 0x67);
			__internal_tsms_lcd_write_register(screen, 0xD620, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD621, 0x84);
			__internal_tsms_lcd_write_register(screen, 0xD622, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD623, 0xA4);
			__internal_tsms_lcd_write_register(screen, 0xD624, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD625, 0xB7);
			__internal_tsms_lcd_write_register(screen, 0xD626, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD627, 0xCF);
			__internal_tsms_lcd_write_register(screen, 0xD628, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD629, 0xDE);
			__internal_tsms_lcd_write_register(screen, 0xD62A, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD62B, 0xF2);
			__internal_tsms_lcd_write_register(screen, 0xD62C, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xD62D, 0xFE);
			__internal_tsms_lcd_write_register(screen, 0xD62E, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD62F, 0x10);
			__internal_tsms_lcd_write_register(screen, 0xD630, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD631, 0x33);
			__internal_tsms_lcd_write_register(screen, 0xD632, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xD633, 0x6D);
			//LV2 Page 0 enable
			__internal_tsms_lcd_write_register(screen, 0xF000, 0x55);
			__internal_tsms_lcd_write_register(screen, 0xF001, 0xAA);
			__internal_tsms_lcd_write_register(screen, 0xF002, 0x52);
			__internal_tsms_lcd_write_register(screen, 0xF003, 0x08);
			__internal_tsms_lcd_write_register(screen, 0xF004, 0x00);
			//Display control
			__internal_tsms_lcd_write_register(screen, 0xB100, 0xCC);
			__internal_tsms_lcd_write_register(screen, 0xB101, 0x00);
			//Source hold time
			__internal_tsms_lcd_write_register(screen, 0xB600, 0x05);
			//Gate EQ control
			__internal_tsms_lcd_write_register(screen, 0xB700, 0x70);
			__internal_tsms_lcd_write_register(screen, 0xB701, 0x70);
			//Source EQ control (Mode 2)
			__internal_tsms_lcd_write_register(screen, 0xB800, 0x01);
			__internal_tsms_lcd_write_register(screen, 0xB801, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xB802, 0x03);
			__internal_tsms_lcd_write_register(screen, 0xB803, 0x03);
			//Inversion mode (2-dot)
			__internal_tsms_lcd_write_register(screen, 0xBC00, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xBC01, 0x00);
			__internal_tsms_lcd_write_register(screen, 0xBC02, 0x00);
			//Timing control 4H w/ 4-delay
			__internal_tsms_lcd_write_register(screen, 0xC900, 0xD0);
			__internal_tsms_lcd_write_register(screen, 0xC901, 0x02);
			__internal_tsms_lcd_write_register(screen, 0xC902, 0x50);
			__internal_tsms_lcd_write_register(screen, 0xC903, 0x50);
			__internal_tsms_lcd_write_register(screen, 0xC904, 0x50);
			__internal_tsms_lcd_write_register(screen, 0x3500, 0x00);
			__internal_tsms_lcd_write_register(screen, 0x3A00, 0x55);  //16-bit/pixel
			__internal_tsms_lcd_write_command(screen, 0x1100);
			TSMS_delay(1);
			__internal_tsms_lcd_write_command(screen, 0x2900);
		} else if (screen->type == TSMS_SCREEN_SSD1963) {
			__internal_tsms_lcd_write_command(screen,
			                                  0xE2);        //Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz
			__internal_tsms_lcd_write_data(screen, 0x1D);        //参数1
			__internal_tsms_lcd_write_data(screen, 0x02);        //参数2 Divider M = 2, PLL = 300/(M+1) = 100MHz
			__internal_tsms_lcd_write_data(screen, 0x04);        //参数3 Validate M and N values
			TSMS_delay(1);
			__internal_tsms_lcd_write_command(screen, 0xE0);        // Start PLL command
			__internal_tsms_lcd_write_data(screen, 0x01);        // enable PLL
			TSMS_delay(10);
			__internal_tsms_lcd_write_command(screen, 0xE0);        // Start PLL command again
			__internal_tsms_lcd_write_data(screen, 0x03);        // now, use PLL output as system clock
			TSMS_delay(12);
			__internal_tsms_lcd_write_command(screen, 0x01);        //软复位
			TSMS_delay(10);

			__internal_tsms_lcd_write_command(screen, 0xE6);        //设置像素频率,33Mhz
			__internal_tsms_lcd_write_data(screen, 0x2F);
			__internal_tsms_lcd_write_data(screen, 0xFF);
			__internal_tsms_lcd_write_data(screen, 0xFF);

			__internal_tsms_lcd_write_command(screen, 0xB0);        //设置LCD模式
			__internal_tsms_lcd_write_data(screen, 0x20);        //24位模式
			__internal_tsms_lcd_write_data(screen, 0x00);        //TFT 模式

			__internal_tsms_lcd_write_data(screen, (defaultSSD1963Option.horizontalResolution - 1) >> 8);//设置LCD水平像素
			__internal_tsms_lcd_write_data(screen, defaultSSD1963Option.horizontalResolution - 1);
			__internal_tsms_lcd_write_data(screen, (defaultSSD1963Option.verticalResolution - 1) >> 8);//设置LCD垂直像素
			__internal_tsms_lcd_write_data(screen, defaultSSD1963Option.verticalResolution - 1);
			__internal_tsms_lcd_write_data(screen, 0x00);        //RGB序列

			__internal_tsms_lcd_write_command(screen, 0xB4);        //Set horizontal period
			__internal_tsms_lcd_write_data(screen, (TSMS_SSD1963_horizontalTotal(option) - 1) >> 8);
			__internal_tsms_lcd_write_data(screen, TSMS_SSD1963_horizontalTotal(option) - 1);
			__internal_tsms_lcd_write_data(screen, TSMS_SSD1963_horizontalPulseStart(option) >> 8);
			__internal_tsms_lcd_write_data(screen, TSMS_SSD1963_horizontalPulseStart(option));
			__internal_tsms_lcd_write_data(screen, option->horizontalPulseWidth - 1);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_command(screen, 0xB6);        //Set vertical period
			__internal_tsms_lcd_write_data(screen, (TSMS_SSD1963_verticalTotal(option) - 1) >> 8);
			__internal_tsms_lcd_write_data(screen, TSMS_SSD1963_verticalTotal(option) - 1);
			__internal_tsms_lcd_write_data(screen, TSMS_SSD1963_verticalPulseStart(option) >> 8);
			__internal_tsms_lcd_write_data(screen, TSMS_SSD1963_verticalPulseStart(option));
			__internal_tsms_lcd_write_data(screen, option->verticalPulseWidth - 1);
			__internal_tsms_lcd_write_data(screen, 0x00);
			__internal_tsms_lcd_write_data(screen, 0x00);

			__internal_tsms_lcd_write_command(screen, 0xF0);    //设置SSD1963与CPU接口为16bit  
			__internal_tsms_lcd_write_data(screen, 0x03);    //16-bit(565 format) data for 16bpp

			__internal_tsms_lcd_write_command(screen, 0x29);    //开启显示
			//设置PWM输出  背光通过占空比可调 
			__internal_tsms_lcd_write_command(screen, 0xD0);    //设置自动白平衡DBC
			__internal_tsms_lcd_write_data(screen, 0x00);    //disable

			__internal_tsms_lcd_write_command(screen, 0xBE);    //配置PWM输出
			__internal_tsms_lcd_write_data(screen, 0x05);    //1设置PWM频率
			__internal_tsms_lcd_write_data(screen, 0xFE);    //2设置PWM占空比
			__internal_tsms_lcd_write_data(screen, 0x01);    //3设置C
			__internal_tsms_lcd_write_data(screen, 0x00);    //4设置D
			__internal_tsms_lcd_write_data(screen, 0x00);    //5设置E
			__internal_tsms_lcd_write_data(screen, 0x00);    //6设置F

			__internal_tsms_lcd_write_command(screen, 0xB8);    //设置GPIO配置
			__internal_tsms_lcd_write_data(screen, 0x03);    //2个IO口设置成输出
			__internal_tsms_lcd_write_data(screen, 0x01);    //GPIO使用正常的IO功能
			__internal_tsms_lcd_write_command(screen, 0xBA);
			__internal_tsms_lcd_write_data(screen, 0X01);    //GPIO[1:0]=01,控制LCD方向0X1963

			__internal_tsms_set_ssd1963_light(screen, 255);//背光设置为最亮
		}
		TSMS_SCREEN_setDisplayDirection(screen, TSMS_DISPLAY_VERTICAL);
		TSMS_SCREEN_enableBackgroudLight(screen);
		return screen;
	}
	tString temp = TSMS_STRING_temp("screen type not supported");
	TSMS_ERR_report(TSMS_ERR_SCREEN_TYPE_NOT_SUPPORTED, &temp);
	free(screen);
	return TSMS_NULL;
}

TSMS_DPHP TSMS_DISPLAY_createHandler(TSMS_SCHP screen, TSMS_THP touch) {
	// touch could be null
	if (screen == TSMS_NULL)
		return TSMS_NULL;
	TSMS_DPHP display = malloc(sizeof(struct TSMS_DISPLAY_HANDLER));
	if (display == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_DPHP");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	display->touch = touch;
	display->screen = screen;
	return display;
}

TSMS_THP TSMS_TOUCH_createGT9147Handler(TSMS_IHP iic, TSMS_GHP reset, TSMS_GHP interrupt) {
	TSMS_THP touch = malloc(sizeof(struct TSMS_TOUCH_HANDLER));
	if (touch == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_THP");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		return TSMS_NULL;
	}
	touch->reset = __tsms_internal_reset_gt9147;
	TSMS_GT9147 gt9147 = malloc(sizeof(struct TSMS_GT9147_HANDLER));
	if (gt9147 == TSMS_NULL) {
		tString temp = TSMS_STRING_temp("malloc failed for TSMS_GT9147");
		TSMS_ERR_report(TSMS_ERR_MALLOC_FAILED, &temp);
		free(touch);
		return TSMS_NULL;
	}
	gt9147->iic = iic;
	gt9147->reset = reset;
	gt9147->interrupt = interrupt;
	return touch;
}

TSMS_RESULT TSMS_DISPLAY_reset(TSMS_DPHP display) {
	if (display == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_RESULT result = TSMS_SUCCESS;
	if (display->screen->reset != TSMS_NULL)
		result |= display->screen->reset(display->screen);
	if (display->touch != TSMS_NULL && display->touch->reset != TSMS_NULL)
		return result | display->touch->reset(display->touch);
	return result;
}

TSMS_RESULT TSMS_SCREEN_enableBackgroudLight(TSMS_SCHP screen) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_GPIO_write(screen->bg, TSMS_GPIO_HIGH);
}

TSMS_RESULT TSMS_SCREEN_disableBackgroudLight(TSMS_SCHP screen) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	return TSMS_GPIO_write(screen->bg, TSMS_GPIO_LOW);
}

TSMS_RESULT TSMS_SCREEN_setDisplayDirection(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	if (direction == TSMS_DISPLAY_VERTICAL) {
		screen->width = screen->defaultWidth;
		screen->height = screen->defaultHeight;
		if (screen->type == TSMS_SCREEN_ILI9341 || screen->type == TSMS_SCREEN_NT35310 ||
		    screen->type == TSMS_SCREEN_ST7789) {
			screen->writeCommand = 0x2C;
			screen->setXCommand = 0x2A;
			screen->setYCommand = 0x2B;
		} else if (screen->type == TSMS_SCREEN_NT5510 || screen->type == TSMS_SCREEN_NT35510) {
			screen->writeCommand = 0x2C00;
			screen->setXCommand = 0x2A00;
			screen->setYCommand = 0x2B00;
		} else if (screen->type == TSMS_SCREEN_SSD1963) {
			screen->writeCommand = 0X2C;
			screen->setXCommand = 0X2B;
			screen->setYCommand = 0X2A;
		}
	} else {
		screen->width = screen->defaultHeight;
		screen->height = screen->defaultWidth;
		if (screen->type == TSMS_SCREEN_ILI9341 || screen->type == TSMS_SCREEN_NT35310 ||
		    screen->type == TSMS_SCREEN_ST7789) {
			screen->writeCommand = 0x2C;
			screen->setXCommand = 0x2A;
			screen->setYCommand = 0x2B;
		} else if (screen->type == TSMS_SCREEN_NT5510 || screen->type == TSMS_SCREEN_NT35510) {
			screen->writeCommand = 0X2C00;
			screen->setXCommand = 0X2A00;
			screen->setYCommand = 0X2B00;
		} else if (screen->type == TSMS_SCREEN_SSD1963) {
			screen->writeCommand = 0X2C;
			screen->setXCommand = 0X2A;
			screen->setYCommand = 0X2B;
		}
	}
	screen->direction = direction;
	return TSMS_SCREEN_setScanDirection(screen, TSMS_SCAN_L2R_U2D);
}

TSMS_RESULT TSMS_SCREEN_setScanDirection(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction) {
	if (screen == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_SCAN_DIRECTION dir = 0;
	if ((screen->direction == TSMS_DISPLAY_HORIZONTAL && screen->type != TSMS_SCREEN_SSD1963) ||
	    (screen->direction == TSMS_DISPLAY_VERTICAL && screen->type == TSMS_SCREEN_SSD1963)) {
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
	}
	uint16_t reg;
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
	if (screen->type == TSMS_SCREEN_NT5510 || screen->type == TSMS_SCREEN_NT35510)
		reg = 0X3600;
	else reg = 0X36;
	if (screen->type != TSMS_SCREEN_NT35310 && screen->type != TSMS_SCREEN_NT35510 &&
	    screen->type != TSMS_SCREEN_NT5510 && screen->type != TSMS_SCREEN_SSD1963)
		value |= 0X08;
	__internal_tsms_lcd_write_register(screen, reg, value);
	if (screen->type != TSMS_SCREEN_SSD1963)
		if (value & 0X20) {
			if (screen->width < screen->height)
				__internal_tsms_swap(&screen->width, &screen->height);
		} else if (screen->width > screen->height)
			__internal_tsms_swap(&screen->width, &screen->height);

	if (screen->type == TSMS_SCREEN_NT5510 || screen->type == TSMS_SCREEN_NT35510) {
		__internal_tsms_lcd_write_command(screen, screen->setXCommand);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_command(screen, screen->setXCommand + 1);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_command(screen, screen->setXCommand + 2);
		__internal_tsms_lcd_write_data(screen, (screen->width - 1) >> 8);
		__internal_tsms_lcd_write_command(screen, screen->setXCommand + 3);
		__internal_tsms_lcd_write_data(screen, (screen->width - 1) & 0XFF);
		__internal_tsms_lcd_write_command(screen, screen->setYCommand);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_command(screen, screen->setYCommand + 1);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_command(screen, screen->setYCommand + 2);
		__internal_tsms_lcd_write_data(screen, (screen->height - 1) >> 8);
		__internal_tsms_lcd_write_command(screen, screen->setYCommand + 3);
		__internal_tsms_lcd_write_data(screen, (screen->height - 1) & 0XFF);
	} else {
		__internal_tsms_lcd_write_command(screen, screen->setXCommand);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_data(screen, (screen->width - 1) >> 8);
		__internal_tsms_lcd_write_data(screen, (screen->width - 1) & 0XFF);
		__internal_tsms_lcd_write_command(screen, screen->setYCommand);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_data(screen, 0);
		__internal_tsms_lcd_write_data(screen, (screen->height - 1) >> 8);
		__internal_tsms_lcd_write_data(screen, (screen->height - 1) & 0XFF);
	}
	screen->scan = direction;
	return TSMS_SUCCESS;
}
