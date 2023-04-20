#ifndef TSMS_DEF_H
#define TSMS_DEF_H

#include "memory.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdarg.h"
#include "stdbool.h"

typedef uint32_t TSMS_CLOCK_FREQUENCY;

typedef uint32_t TSMS_SIZE;

typedef long TSMS_POS;

typedef long TSMS_LSIZE;

typedef enum {
	TSMS_TRANSFER_MSB = 0U, TSMS_TRANSFER_LSB
} TSMS_TRANSFER_TYPE;

typedef enum {
	TSMS_REGISTER_MSB = 0, TSMS_REGISTER_LSB
} TSMS_REGISTER_DATA_TYPE;

typedef enum {
	TSMS_8BIT = 0, TSMS_16BIT, TSMS_24BIT, TSMS_32BIT
} TSMS_BITS;

typedef enum {
	TSMS_SUCCESS = 0, // a successful operation
	TSMS_ERROR = 1,  // an error occurred ( one specific case of failure )
	TSMS_TIMEOUT = 2, // a timeout occurred ( one specific case of failure )
	TSMS_FAIL = 4 // a failed operation
} TSMS_RESULT;

#define TSMS_MASK(x) ((1<<x)-1)

#define TSMS_NULL NULL

#endif //TSMS_DEF_H
