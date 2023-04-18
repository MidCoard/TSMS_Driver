#ifndef TSMS_SSD1963_H
#define TSMS_SSD1963_H

#include "tsms_def.h"

typedef struct TSMS_SSD1963_OPTION * TSMS_SSD1963_OPTION_POINTER;
typedef TSMS_SSD1963_OPTION_POINTER TSMS_SSD1963_OP;

struct TSMS_SSD1963_OPTION {
	uint16_t horizontalResolution;
	uint16_t verticalResolution;
	uint16_t horizontalPulseWidth;
	uint16_t horizontalBackPorch;
	uint16_t horizontalFrontPorch;
	uint16_t verticalPulseWidth;
	uint16_t verticalBackPorch;
	uint16_t verticalFrontPorch;
};

extern struct TSMS_SSD1963_OPTION defaultSSD1963Option;

uint16_t TSMS_SSD1963_horizontalTotal(TSMS_SSD1963_OP option);

uint16_t TSMS_SSD1963_horizontalPulseStart(TSMS_SSD1963_OP option);

uint16_t TSMS_SSD1963_verticalTotal(TSMS_SSD1963_OP option);

uint16_t TSMS_SSD1963_verticalPulseStart(TSMS_SSD1963_OP option);

#endif //TSMS_SSD1963_H
