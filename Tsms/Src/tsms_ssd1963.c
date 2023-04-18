#include "screen/tsms_ssd1963.h"

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

