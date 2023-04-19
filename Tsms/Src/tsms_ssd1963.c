#include "screen/tsms_ssd1963.h"

TSMS_INLINE void __internal_tsms_set_ssd1963_light(TSMS_SCHP screen, uint8_t light) {
	TSMS_SCREEN_writeCommand(screen, 0xBE);
	TSMS_SCREEN_writeData(screen, 0x05);
	TSMS_SCREEN_writeData(screen, light);
	TSMS_SCREEN_writeData(screen, 0x01);
	TSMS_SCREEN_writeData(screen, 0xFF);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
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
	TSMS_SCREEN_writeCommand(screen, 0XA1);
	uint16_t id;
	id = TSMS_SCREEN_readData(screen);
	id = TSMS_SCREEN_readData(screen);
	id = TSMS_SCREEN_readData(screen);
	id <<= 8;
	id |= TSMS_SCREEN_readData(screen);
	return id;
}

TSMS_RESULT TSMS_SSD1963_init(TSMS_SCHP screen, void * op) {
	TSMS_SSD1963_OP option = op;
	TSMS_SCREEN_writeCommand(screen,
	                         0xE2);        //Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz
	TSMS_SCREEN_writeData(screen, 0x1D);        //参数1
	TSMS_SCREEN_writeData(screen, 0x02);        //参数2 Divider M = 2, PLL = 300/(M+1) = 100MHz
	TSMS_SCREEN_writeData(screen, 0x04);        //参数3 Validate M and N values
	TSMS_delay(1);
	TSMS_SCREEN_writeCommand(screen, 0xE0);        // Start PLL command
	TSMS_SCREEN_writeData(screen, 0x01);        // enable PLL
	TSMS_delay(10);
	TSMS_SCREEN_writeCommand(screen, 0xE0);        // Start PLL command again
	TSMS_SCREEN_writeData(screen, 0x03);        // now, use PLL output as system clock
	TSMS_delay(12);
	TSMS_SCREEN_writeCommand(screen, 0x01);        //软复位
	TSMS_delay(10);

	TSMS_SCREEN_writeCommand(screen, 0xE6);        //设置像素频率,33Mhz
	TSMS_SCREEN_writeData(screen, 0x2F);
	TSMS_SCREEN_writeData(screen, 0xFF);
	TSMS_SCREEN_writeData(screen, 0xFF);

	TSMS_SCREEN_writeCommand(screen, 0xB0);        //设置LCD模式
	TSMS_SCREEN_writeData(screen, 0x20);        //24位模式
	TSMS_SCREEN_writeData(screen, 0x00);        //TFT 模式

	TSMS_SCREEN_writeData(screen, (defaultSSD1963Option.horizontalResolution - 1) >> 8);//设置LCD水平像素
	TSMS_SCREEN_writeData(screen, defaultSSD1963Option.horizontalResolution - 1);
	TSMS_SCREEN_writeData(screen, (defaultSSD1963Option.verticalResolution - 1) >> 8);//设置LCD垂直像素
	TSMS_SCREEN_writeData(screen, defaultSSD1963Option.verticalResolution - 1);
	TSMS_SCREEN_writeData(screen, 0x00);        //RGB序列

	TSMS_SCREEN_writeCommand(screen, 0xB4);        //Set horizontal period
	TSMS_SCREEN_writeData(screen, (TSMS_SSD1963_horizontalTotal(option) - 1) >> 8);
	TSMS_SCREEN_writeData(screen, TSMS_SSD1963_horizontalTotal(option) - 1);
	TSMS_SCREEN_writeData(screen, TSMS_SSD1963_horizontalPulseStart(option) >> 8);
	TSMS_SCREEN_writeData(screen, TSMS_SSD1963_horizontalPulseStart(option));
	TSMS_SCREEN_writeData(screen, option->horizontalPulseWidth - 1);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeCommand(screen, 0xB6);        //Set vertical period
	TSMS_SCREEN_writeData(screen, (TSMS_SSD1963_verticalTotal(option) - 1) >> 8);
	TSMS_SCREEN_writeData(screen, TSMS_SSD1963_verticalTotal(option) - 1);
	TSMS_SCREEN_writeData(screen, TSMS_SSD1963_verticalPulseStart(option) >> 8);
	TSMS_SCREEN_writeData(screen, TSMS_SSD1963_verticalPulseStart(option));
	TSMS_SCREEN_writeData(screen, option->verticalPulseWidth - 1);
	TSMS_SCREEN_writeData(screen, 0x00);
	TSMS_SCREEN_writeData(screen, 0x00);

	TSMS_SCREEN_writeCommand(screen, 0xF0);    //设置SSD1963与CPU接口为16bit
	TSMS_SCREEN_writeData(screen, 0x03);    //16-bit(565 format) data for 16bpp

	TSMS_SCREEN_writeCommand(screen, 0x29);    //开启显示
	//设置PWM输出  背光通过占空比可调
	TSMS_SCREEN_writeCommand(screen, 0xD0);    //设置自动白平衡DBC
	TSMS_SCREEN_writeData(screen, 0x00);    //disable

	TSMS_SCREEN_writeCommand(screen, 0xBE);    //配置PWM输出
	TSMS_SCREEN_writeData(screen, 0x05);    //1设置PWM频率
	TSMS_SCREEN_writeData(screen, 0xFE);    //2设置PWM占空比
	TSMS_SCREEN_writeData(screen, 0x01);    //3设置C
	TSMS_SCREEN_writeData(screen, 0x00);    //4设置D
	TSMS_SCREEN_writeData(screen, 0x00);    //5设置E
	TSMS_SCREEN_writeData(screen, 0x00);    //6设置F

	TSMS_SCREEN_writeCommand(screen, 0xB8);    //设置GPIO配置
	TSMS_SCREEN_writeData(screen, 0x03);    //2个IO口设置成输出
	TSMS_SCREEN_writeData(screen, 0x01);    //GPIO使用正常的IO功能
	TSMS_SCREEN_writeCommand(screen, 0xBA);
	TSMS_SCREEN_writeData(screen, 0X01);    //GPIO[1:0]=01,控制LCD方向0X1963

	__internal_tsms_set_ssd1963_light(screen, 255);//背光设置为最亮
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_SSD1963_setDisplayDirection(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction) {
	if (direction == TSMS_DISPLAY_VERTICAL) {
		screen->width = screen->defaultWidth;
		screen->height = screen->defaultHeight;
		screen->writeCommand = 0X2C;
		screen->setXCommand = 0X2B;
		screen->setYCommand = 0X2A;
	} else {
		screen->width = screen->defaultHeight;
		screen->height = screen->defaultWidth;
		screen->writeCommand = 0X2C;
		screen->setXCommand = 0X2A;
		screen->setYCommand = 0X2B;
	}
	screen->displayDirection = direction;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_SSD1963_setScanDirection(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction) {
	TSMS_SCAN_DIRECTION dir = direction;
	if (screen->displayDirection == TSMS_DISPLAY_VERTICAL)
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
	TSMS_SCREEN_writeRegister(screen, reg, value);
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

