#ifndef TSMS_GT9147_H
#define TSMS_GT9147_H


#define GT9147_REG_CTL 0
#define GT9147_REG_CTL_ADDRESS 0x8040
#define GT9147_REG_ID_1 1
#define GT9147_REG_ID_1_ADDRESS 0x8140
#define GT9147_REG_ID_2 2
#define GT9147_REG_ID_2_ADDRESS 0x8141
#define GT9147_REG_ID_3 3
#define GT9147_REG_ID_3_ADDRESS 0x8142
#define GT9147_REG_ID_4 4
#define GT9147_REG_ID_4_ADDRESS 0x8143
#define GT9147_REG_STATUS 5
#define GT9147_REG_STATUS_ADDRESS 0x814E
#define GT9147_REG_TOUCH_1_X_L 6
#define GT9147_REG_TOUCH_1_X_L_ADDRESS 0x8150
#define GT9147_REG_TOUCH_1_X_H 7
#define GT9147_REG_TOUCH_1_X_H_ADDRESS 0x8151
#define GT9147_REG_TOUCH_1_Y_L 8
#define GT9147_REG_TOUCH_1_Y_L_ADDRESS 0x8152
#define GT9147_REG_TOUCH_1_Y_H 9
#define GT9147_REG_TOUCH_1_Y_H_ADDRESS 0x8153
#define GT9147_REG_TOUCH_1_SIZE_L 10
#define GT9147_REG_TOUCH_1_SIZE_L_ADDRESS 0x8154
#define GT9147_REG_TOUCH_1_SIZE_H 11
#define GT9147_REG_TOUCH_1_SIZE_H_ADDRESS 0x8155
#define GT9147_REG_TOUCH_2_X_L 12
#define GT9147_REG_TOUCH_2_X_L_ADDRESS 0x8156
#define GT9147_REG_TOUCH_2_X_H 13
#define GT9147_REG_TOUCH_2_X_H_ADDRESS 0x8157

#include "tsms_def.h"

extern uint8_t GT9147_CFG_TBL[];

#endif //TSMS_GT9147_H
