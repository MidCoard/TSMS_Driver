#include "screen/tsms_ssd1963.h"

TSMS_INLINE void __internal_tsms_set_ssd1963_light(TSMS_SCHP screen, uint8_t light) {
	__internal_tsms_lcd_write_command(screen, 0xBE);
	__internal_tsms_lcd_write_data(screen, 0x05);
	__internal_tsms_lcd_write_data(screen, light);
	__internal_tsms_lcd_write_data(screen, 0x01);
	__internal_tsms_lcd_write_data(screen, 0xFF);
	__internal_tsms_lcd_write_data(screen, 0x00);
	__internal_tsms_lcd_write_data(screen, 0x00);
}

struct TSMS_SSD1963_OPTION defaultSSD1963Option = {
		800, 400, 1, 46, 210, 1, 23, 22
};

uint16_t TSMS_SSD1963_horizontalTotal(TSMS_SSD1963_OP option) {
	return option->horizontalResolution + option->horizontalBackPorch + option->horizontalFrontPorch;
}

uint16_t TSMS_SSD1963_horizontalPulseStart(TSMS_SSD1963_OP option) {
	return option->horizontalBackPorch;
}

uint16_t TSMS_SSD1963_verticalTotal(TSMS_SSD1963_OP option) {
	return option->verticalResolution + option->verticalBackPorch + option->verticalFrontPorch;
}

uint16_t TSMS_SSD1963_verticalPulseStart(TSMS_SSD1963_OP option) {
	return option->verticalBackPorch;
}

//0X5761
uint16_t TSMS_SSD1963_readId(TSMS_SCHP screen) {
	__internal_tsms_lcd_write_command(screen, 0XA1);
	uint16_t id;
	id = __internal_tsms_lcd_read_data(screen);
	id = __internal_tsms_lcd_read_data(screen);
	id = __internal_tsms_lcd_read_data(screen);
	id <<= 8;
	id |= __internal_tsms_lcd_read_data(screen);
	return id;
}

TSMS_RESULT TSMS_SSD1963_init(TSMS_SCHP screen, void * op) {
	TSMS_SSD1963_OP option = op;
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
	return TSMS_SUCCESS;
}

