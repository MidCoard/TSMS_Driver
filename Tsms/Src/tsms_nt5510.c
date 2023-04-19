#include "screen/tsms_nt5510.h"

// 0x8000 or 0x5510
uint16_t TSMS_NT5510_readId(TSMS_SCHP screen) {
	TSMS_SCREEN_writeRegister(screen, 0xF000, 0x0055);
	TSMS_SCREEN_writeRegister(screen, 0xF001, 0x00AA);
	TSMS_SCREEN_writeRegister(screen, 0xF002, 0x0052);
	TSMS_SCREEN_writeRegister(screen, 0xF003, 0x0008);
	TSMS_SCREEN_writeRegister(screen, 0xF004, 0x0001);
	uint16_t id;
	TSMS_SCREEN_writeCommand(screen, 0xC500);
	id = TSMS_SCREEN_readData(screen);
	id <<= 8;
	TSMS_SCREEN_writeCommand(screen, 0xC501);
	id |= TSMS_SCREEN_readData(screen);
	return id;
}

TSMS_RESULT TSMS_NT5510_init(TSMS_SCHP screen, void* option) {
	TSMS_SCREEN_writeRegister(screen, 0xF000, 0x55);
	TSMS_SCREEN_writeRegister(screen, 0xF001, 0xAA);
	TSMS_SCREEN_writeRegister(screen, 0xF002, 0x52);
	TSMS_SCREEN_writeRegister(screen, 0xF003, 0x08);
	TSMS_SCREEN_writeRegister(screen, 0xF004, 0x01);
	//AVDD Set AVDD 5.2V
	TSMS_SCREEN_writeRegister(screen, 0xB000, 0x0D);
	TSMS_SCREEN_writeRegister(screen, 0xB001, 0x0D);
	TSMS_SCREEN_writeRegister(screen, 0xB002, 0x0D);
	//AVDD ratio
	TSMS_SCREEN_writeRegister(screen, 0xB600, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xB601, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xB602, 0x34);
	//AVEE -5.2V
	TSMS_SCREEN_writeRegister(screen, 0xB100, 0x0D);
	TSMS_SCREEN_writeRegister(screen, 0xB101, 0x0D);
	TSMS_SCREEN_writeRegister(screen, 0xB102, 0x0D);
	//AVEE ratio
	TSMS_SCREEN_writeRegister(screen, 0xB700, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xB701, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xB702, 0x34);
	//VCL -2.5V
	TSMS_SCREEN_writeRegister(screen, 0xB200, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xB201, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xB202, 0x00);
	//VCL ratio
	TSMS_SCREEN_writeRegister(screen, 0xB800, 0x24);
	TSMS_SCREEN_writeRegister(screen, 0xB801, 0x24);
	TSMS_SCREEN_writeRegister(screen, 0xB802, 0x24);
	//VGH 15V (Free pump)
	TSMS_SCREEN_writeRegister(screen, 0xBF00, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xB300, 0x0F);
	TSMS_SCREEN_writeRegister(screen, 0xB301, 0x0F);
	TSMS_SCREEN_writeRegister(screen, 0xB302, 0x0F);
	//VGH ratio
	TSMS_SCREEN_writeRegister(screen, 0xB900, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xB901, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xB902, 0x34);
	//VGL_REG -10V
	TSMS_SCREEN_writeRegister(screen, 0xB500, 0x08);
	TSMS_SCREEN_writeRegister(screen, 0xB501, 0x08);
	TSMS_SCREEN_writeRegister(screen, 0xB502, 0x08);
	TSMS_SCREEN_writeRegister(screen, 0xC200, 0x03);
	//VGLX ratio
	TSMS_SCREEN_writeRegister(screen, 0xBA00, 0x24);
	TSMS_SCREEN_writeRegister(screen, 0xBA01, 0x24);
	TSMS_SCREEN_writeRegister(screen, 0xBA02, 0x24);
	//VGMP/VGSP 4.5V/0V
	TSMS_SCREEN_writeRegister(screen, 0xBC00, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xBC01, 0x78);
	TSMS_SCREEN_writeRegister(screen, 0xBC02, 0x00);
	//VGMN/VGSN -4.5V/0V
	TSMS_SCREEN_writeRegister(screen, 0xBD00, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xBD01, 0x78);
	TSMS_SCREEN_writeRegister(screen, 0xBD02, 0x00);
	//VCOM
	TSMS_SCREEN_writeRegister(screen, 0xBE00, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xBE01, 0x64);
	//Gamma Setting
	TSMS_SCREEN_writeRegister(screen, 0xD100, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD101, 0x33);
	TSMS_SCREEN_writeRegister(screen, 0xD102, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD103, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xD104, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD105, 0x3A);
	TSMS_SCREEN_writeRegister(screen, 0xD106, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD107, 0x4A);
	TSMS_SCREEN_writeRegister(screen, 0xD108, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD109, 0x5C);
	TSMS_SCREEN_writeRegister(screen, 0xD10A, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD10B, 0x81);
	TSMS_SCREEN_writeRegister(screen, 0xD10C, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD10D, 0xA6);
	TSMS_SCREEN_writeRegister(screen, 0xD10E, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD10F, 0xE5);
	TSMS_SCREEN_writeRegister(screen, 0xD110, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD111, 0x13);
	TSMS_SCREEN_writeRegister(screen, 0xD112, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD113, 0x54);
	TSMS_SCREEN_writeRegister(screen, 0xD114, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD115, 0x82);
	TSMS_SCREEN_writeRegister(screen, 0xD116, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD117, 0xCA);
	TSMS_SCREEN_writeRegister(screen, 0xD118, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD119, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD11A, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD11B, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD11C, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD11D, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xD11E, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD11F, 0x67);
	TSMS_SCREEN_writeRegister(screen, 0xD120, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD121, 0x84);
	TSMS_SCREEN_writeRegister(screen, 0xD122, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD123, 0xA4);
	TSMS_SCREEN_writeRegister(screen, 0xD124, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD125, 0xB7);
	TSMS_SCREEN_writeRegister(screen, 0xD126, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD127, 0xCF);
	TSMS_SCREEN_writeRegister(screen, 0xD128, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD129, 0xDE);
	TSMS_SCREEN_writeRegister(screen, 0xD12A, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD12B, 0xF2);
	TSMS_SCREEN_writeRegister(screen, 0xD12C, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD12D, 0xFE);
	TSMS_SCREEN_writeRegister(screen, 0xD12E, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD12F, 0x10);
	TSMS_SCREEN_writeRegister(screen, 0xD130, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD131, 0x33);
	TSMS_SCREEN_writeRegister(screen, 0xD132, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD133, 0x6D);
	TSMS_SCREEN_writeRegister(screen, 0xD200, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD201, 0x33);
	TSMS_SCREEN_writeRegister(screen, 0xD202, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD203, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xD204, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD205, 0x3A);
	TSMS_SCREEN_writeRegister(screen, 0xD206, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD207, 0x4A);
	TSMS_SCREEN_writeRegister(screen, 0xD208, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD209, 0x5C);
	TSMS_SCREEN_writeRegister(screen, 0xD20A, 0x00);

	TSMS_SCREEN_writeRegister(screen, 0xD20B, 0x81);
	TSMS_SCREEN_writeRegister(screen, 0xD20C, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD20D, 0xA6);
	TSMS_SCREEN_writeRegister(screen, 0xD20E, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD20F, 0xE5);
	TSMS_SCREEN_writeRegister(screen, 0xD210, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD211, 0x13);
	TSMS_SCREEN_writeRegister(screen, 0xD212, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD213, 0x54);
	TSMS_SCREEN_writeRegister(screen, 0xD214, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD215, 0x82);
	TSMS_SCREEN_writeRegister(screen, 0xD216, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD217, 0xCA);
	TSMS_SCREEN_writeRegister(screen, 0xD218, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD219, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD21A, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD21B, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD21C, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD21D, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xD21E, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD21F, 0x67);
	TSMS_SCREEN_writeRegister(screen, 0xD220, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD221, 0x84);
	TSMS_SCREEN_writeRegister(screen, 0xD222, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD223, 0xA4);
	TSMS_SCREEN_writeRegister(screen, 0xD224, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD225, 0xB7);
	TSMS_SCREEN_writeRegister(screen, 0xD226, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD227, 0xCF);
	TSMS_SCREEN_writeRegister(screen, 0xD228, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD229, 0xDE);
	TSMS_SCREEN_writeRegister(screen, 0xD22A, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD22B, 0xF2);
	TSMS_SCREEN_writeRegister(screen, 0xD22C, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD22D, 0xFE);
	TSMS_SCREEN_writeRegister(screen, 0xD22E, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD22F, 0x10);
	TSMS_SCREEN_writeRegister(screen, 0xD230, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD231, 0x33);
	TSMS_SCREEN_writeRegister(screen, 0xD232, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD233, 0x6D);
	TSMS_SCREEN_writeRegister(screen, 0xD300, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD301, 0x33);
	TSMS_SCREEN_writeRegister(screen, 0xD302, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD303, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xD304, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD305, 0x3A);
	TSMS_SCREEN_writeRegister(screen, 0xD306, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD307, 0x4A);
	TSMS_SCREEN_writeRegister(screen, 0xD308, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD309, 0x5C);
	TSMS_SCREEN_writeRegister(screen, 0xD30A, 0x00);

	TSMS_SCREEN_writeRegister(screen, 0xD30B, 0x81);
	TSMS_SCREEN_writeRegister(screen, 0xD30C, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD30D, 0xA6);
	TSMS_SCREEN_writeRegister(screen, 0xD30E, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD30F, 0xE5);
	TSMS_SCREEN_writeRegister(screen, 0xD310, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD311, 0x13);
	TSMS_SCREEN_writeRegister(screen, 0xD312, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD313, 0x54);
	TSMS_SCREEN_writeRegister(screen, 0xD314, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD315, 0x82);
	TSMS_SCREEN_writeRegister(screen, 0xD316, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD317, 0xCA);
	TSMS_SCREEN_writeRegister(screen, 0xD318, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD319, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD31A, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD31B, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD31C, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD31D, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xD31E, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD31F, 0x67);
	TSMS_SCREEN_writeRegister(screen, 0xD320, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD321, 0x84);
	TSMS_SCREEN_writeRegister(screen, 0xD322, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD323, 0xA4);
	TSMS_SCREEN_writeRegister(screen, 0xD324, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD325, 0xB7);
	TSMS_SCREEN_writeRegister(screen, 0xD326, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD327, 0xCF);
	TSMS_SCREEN_writeRegister(screen, 0xD328, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD329, 0xDE);
	TSMS_SCREEN_writeRegister(screen, 0xD32A, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD32B, 0xF2);
	TSMS_SCREEN_writeRegister(screen, 0xD32C, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD32D, 0xFE);
	TSMS_SCREEN_writeRegister(screen, 0xD32E, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD32F, 0x10);
	TSMS_SCREEN_writeRegister(screen, 0xD330, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD331, 0x33);
	TSMS_SCREEN_writeRegister(screen, 0xD332, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD333, 0x6D);
	TSMS_SCREEN_writeRegister(screen, 0xD400, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD401, 0x33);
	TSMS_SCREEN_writeRegister(screen, 0xD402, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD403, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xD404, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD405, 0x3A);
	TSMS_SCREEN_writeRegister(screen, 0xD406, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD407, 0x4A);
	TSMS_SCREEN_writeRegister(screen, 0xD408, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD409, 0x5C);
	TSMS_SCREEN_writeRegister(screen, 0xD40A, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD40B, 0x81);

	TSMS_SCREEN_writeRegister(screen, 0xD40C, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD40D, 0xA6);
	TSMS_SCREEN_writeRegister(screen, 0xD40E, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD40F, 0xE5);
	TSMS_SCREEN_writeRegister(screen, 0xD410, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD411, 0x13);
	TSMS_SCREEN_writeRegister(screen, 0xD412, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD413, 0x54);
	TSMS_SCREEN_writeRegister(screen, 0xD414, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD415, 0x82);
	TSMS_SCREEN_writeRegister(screen, 0xD416, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD417, 0xCA);
	TSMS_SCREEN_writeRegister(screen, 0xD418, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD419, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD41A, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD41B, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD41C, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD41D, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xD41E, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD41F, 0x67);
	TSMS_SCREEN_writeRegister(screen, 0xD420, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD421, 0x84);
	TSMS_SCREEN_writeRegister(screen, 0xD422, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD423, 0xA4);
	TSMS_SCREEN_writeRegister(screen, 0xD424, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD425, 0xB7);
	TSMS_SCREEN_writeRegister(screen, 0xD426, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD427, 0xCF);
	TSMS_SCREEN_writeRegister(screen, 0xD428, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD429, 0xDE);
	TSMS_SCREEN_writeRegister(screen, 0xD42A, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD42B, 0xF2);
	TSMS_SCREEN_writeRegister(screen, 0xD42C, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD42D, 0xFE);
	TSMS_SCREEN_writeRegister(screen, 0xD42E, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD42F, 0x10);
	TSMS_SCREEN_writeRegister(screen, 0xD430, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD431, 0x33);
	TSMS_SCREEN_writeRegister(screen, 0xD432, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD433, 0x6D);
	TSMS_SCREEN_writeRegister(screen, 0xD500, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD501, 0x33);
	TSMS_SCREEN_writeRegister(screen, 0xD502, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD503, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xD504, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD505, 0x3A);
	TSMS_SCREEN_writeRegister(screen, 0xD506, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD507, 0x4A);
	TSMS_SCREEN_writeRegister(screen, 0xD508, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD509, 0x5C);
	TSMS_SCREEN_writeRegister(screen, 0xD50A, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD50B, 0x81);

	TSMS_SCREEN_writeRegister(screen, 0xD50C, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD50D, 0xA6);
	TSMS_SCREEN_writeRegister(screen, 0xD50E, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD50F, 0xE5);
	TSMS_SCREEN_writeRegister(screen, 0xD510, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD511, 0x13);
	TSMS_SCREEN_writeRegister(screen, 0xD512, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD513, 0x54);
	TSMS_SCREEN_writeRegister(screen, 0xD514, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD515, 0x82);
	TSMS_SCREEN_writeRegister(screen, 0xD516, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD517, 0xCA);
	TSMS_SCREEN_writeRegister(screen, 0xD518, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD519, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD51A, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD51B, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD51C, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD51D, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xD51E, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD51F, 0x67);
	TSMS_SCREEN_writeRegister(screen, 0xD520, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD521, 0x84);
	TSMS_SCREEN_writeRegister(screen, 0xD522, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD523, 0xA4);
	TSMS_SCREEN_writeRegister(screen, 0xD524, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD525, 0xB7);
	TSMS_SCREEN_writeRegister(screen, 0xD526, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD527, 0xCF);
	TSMS_SCREEN_writeRegister(screen, 0xD528, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD529, 0xDE);
	TSMS_SCREEN_writeRegister(screen, 0xD52A, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD52B, 0xF2);
	TSMS_SCREEN_writeRegister(screen, 0xD52C, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD52D, 0xFE);
	TSMS_SCREEN_writeRegister(screen, 0xD52E, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD52F, 0x10);
	TSMS_SCREEN_writeRegister(screen, 0xD530, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD531, 0x33);
	TSMS_SCREEN_writeRegister(screen, 0xD532, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD533, 0x6D);
	TSMS_SCREEN_writeRegister(screen, 0xD600, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD601, 0x33);
	TSMS_SCREEN_writeRegister(screen, 0xD602, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD603, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xD604, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD605, 0x3A);
	TSMS_SCREEN_writeRegister(screen, 0xD606, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD607, 0x4A);
	TSMS_SCREEN_writeRegister(screen, 0xD608, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD609, 0x5C);
	TSMS_SCREEN_writeRegister(screen, 0xD60A, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD60B, 0x81);

	TSMS_SCREEN_writeRegister(screen, 0xD60C, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD60D, 0xA6);
	TSMS_SCREEN_writeRegister(screen, 0xD60E, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD60F, 0xE5);
	TSMS_SCREEN_writeRegister(screen, 0xD610, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD611, 0x13);
	TSMS_SCREEN_writeRegister(screen, 0xD612, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD613, 0x54);
	TSMS_SCREEN_writeRegister(screen, 0xD614, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD615, 0x82);
	TSMS_SCREEN_writeRegister(screen, 0xD616, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD617, 0xCA);
	TSMS_SCREEN_writeRegister(screen, 0xD618, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD619, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xD61A, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD61B, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xD61C, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD61D, 0x34);
	TSMS_SCREEN_writeRegister(screen, 0xD61E, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD61F, 0x67);
	TSMS_SCREEN_writeRegister(screen, 0xD620, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD621, 0x84);
	TSMS_SCREEN_writeRegister(screen, 0xD622, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD623, 0xA4);
	TSMS_SCREEN_writeRegister(screen, 0xD624, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD625, 0xB7);
	TSMS_SCREEN_writeRegister(screen, 0xD626, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD627, 0xCF);
	TSMS_SCREEN_writeRegister(screen, 0xD628, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD629, 0xDE);
	TSMS_SCREEN_writeRegister(screen, 0xD62A, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD62B, 0xF2);
	TSMS_SCREEN_writeRegister(screen, 0xD62C, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xD62D, 0xFE);
	TSMS_SCREEN_writeRegister(screen, 0xD62E, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD62F, 0x10);
	TSMS_SCREEN_writeRegister(screen, 0xD630, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD631, 0x33);
	TSMS_SCREEN_writeRegister(screen, 0xD632, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xD633, 0x6D);
	//LV2 Page 0 enable
	TSMS_SCREEN_writeRegister(screen, 0xF000, 0x55);
	TSMS_SCREEN_writeRegister(screen, 0xF001, 0xAA);
	TSMS_SCREEN_writeRegister(screen, 0xF002, 0x52);
	TSMS_SCREEN_writeRegister(screen, 0xF003, 0x08);
	TSMS_SCREEN_writeRegister(screen, 0xF004, 0x00);
	//Display control
	TSMS_SCREEN_writeRegister(screen, 0xB100, 0xCC);
	TSMS_SCREEN_writeRegister(screen, 0xB101, 0x00);
	//Source hold time
	TSMS_SCREEN_writeRegister(screen, 0xB600, 0x05);
	//Gate EQ control
	TSMS_SCREEN_writeRegister(screen, 0xB700, 0x70);
	TSMS_SCREEN_writeRegister(screen, 0xB701, 0x70);
	//Source EQ control (Mode 2)
	TSMS_SCREEN_writeRegister(screen, 0xB800, 0x01);
	TSMS_SCREEN_writeRegister(screen, 0xB801, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xB802, 0x03);
	TSMS_SCREEN_writeRegister(screen, 0xB803, 0x03);
	//Inversion mode (2-dot)
	TSMS_SCREEN_writeRegister(screen, 0xBC00, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xBC01, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0xBC02, 0x00);
	//Timing control 4H w/ 4-delay
	TSMS_SCREEN_writeRegister(screen, 0xC900, 0xD0);
	TSMS_SCREEN_writeRegister(screen, 0xC901, 0x02);
	TSMS_SCREEN_writeRegister(screen, 0xC902, 0x50);
	TSMS_SCREEN_writeRegister(screen, 0xC903, 0x50);
	TSMS_SCREEN_writeRegister(screen, 0xC904, 0x50);
	TSMS_SCREEN_writeRegister(screen, 0x3500, 0x00);
	TSMS_SCREEN_writeRegister(screen, 0x3A00, 0x55);  //16-bit/pixel
	TSMS_SCREEN_writeCommand(screen, 0x1100);
	TSMS_delay(1);
	TSMS_SCREEN_writeCommand(screen, 0x2900);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_NT5510_setDisplayDirection(TSMS_SCHP screen, TSMS_DISPLAY_DIRECTION direction) {
	if (direction == TSMS_DISPLAY_VERTICAL) {
		screen->width = screen->defaultWidth;
		screen->height = screen->defaultHeight;
	} else {
		screen->width = screen->defaultHeight;
		screen->height = screen->defaultWidth;
	}
	screen->writeCommand = 0x2C00;
	screen->setXCommand = 0x2A00;
	screen->setYCommand = 0x2B00;
	screen->displayDirection = direction;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_NT5510_setScanDirection(TSMS_SCHP screen, TSMS_SCAN_DIRECTION direction) {
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
	uint16_t reg = 0x3600;
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
	if (value & 0X20) {
		if (screen->width < screen->height)
			TSMS_UTIL_swap(&screen->width, &screen->height);
	} else if (screen->width > screen->height)
		TSMS_UTIL_swap(&screen->width, &screen->height);
	TSMS_SCREEN_writeCommand(screen, screen->setXCommand);
	TSMS_SCREEN_writeData(screen, 0);
	TSMS_SCREEN_writeCommand(screen, screen->setXCommand + 1);
	TSMS_SCREEN_writeData(screen, 0);
	TSMS_SCREEN_writeCommand(screen, screen->setXCommand + 2);
	TSMS_SCREEN_writeData(screen, (screen->width - 1) >> 8);
	TSMS_SCREEN_writeCommand(screen, screen->setXCommand + 3);
	TSMS_SCREEN_writeData(screen, (screen->width - 1) & 0XFF);
	TSMS_SCREEN_writeCommand(screen, screen->setYCommand);
	TSMS_SCREEN_writeData(screen, 0);
	TSMS_SCREEN_writeCommand(screen, screen->setYCommand + 1);
	TSMS_SCREEN_writeData(screen, 0);
	TSMS_SCREEN_writeCommand(screen, screen->setYCommand + 2);
	TSMS_SCREEN_writeData(screen, (screen->height - 1) >> 8);
	TSMS_SCREEN_writeCommand(screen, screen->setYCommand + 3);
	TSMS_SCREEN_writeData(screen, (screen->height - 1) & 0XFF);
	screen->scanDirection = direction;
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_NT5510_setCursor(TSMS_SCHP screen, uint16_t x, uint16_t y) {
	TSMS_SCREEN_writeCommand(screen, screen->setXCommand);
	TSMS_SCREEN_writeData(screen, x >> 8);
	TSMS_SCREEN_writeCommand(screen, screen->setXCommand + 1);
	TSMS_SCREEN_writeData(screen, x & 0XFF);
	TSMS_SCREEN_writeCommand(screen, screen->setYCommand);
	TSMS_SCREEN_writeData(screen, y >> 8);
	TSMS_SCREEN_writeCommand(screen, screen->setYCommand + 1);
	TSMS_SCREEN_writeData(screen, y & 0XFF);
	return TSMS_SUCCESS;
}

