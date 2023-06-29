#ifndef TSMS_DEF_H
#define TSMS_DEF_H

#ifdef TSMS_OPTIMIZATION
#define TSMS_INLINE inline static
#else
#define TSMS_INLINE static
#endif

#include "stdint.h"

typedef uint32_t TSMS_CLOCK_FREQUENCY;

typedef uint32_t TSMS_SIZE;

typedef long TSMS_POS;

typedef long TSMS_LSIZE;

typedef enum {
	TSMS_TRANSFER_TYPE_MSB = 0, TSMS_TRANSFER_TYPE_LSB
} TSMS_TRANSFER_TYPE;

typedef enum {
	TSMS_REGISTER_DATA_TYPE_MSB = 0, TSMS_REGISTER_DATA_TYPE_LSB
} TSMS_REGISTER_DATA_TYPE;

typedef enum {
	TSMS_BITS_8BIT = 0, TSMS_BITS_16BIT, TSMS_BITS_24BIT, TSMS_BITS_32BIT
} TSMS_BITS;

typedef enum {
	TSMS_SUCCESS = 0, // a successful operation
	TSMS_ERROR = 1,  // an error occurred ( one specific case of failure )
	TSMS_TIMEOUT = 2, // a timeout occurred ( one specific case of failure )
	TSMS_FAIL = 4 // a failed operation
} TSMS_RESULT;

typedef enum {
	TSMS_TYPE_INT = 0, TSMS_TYPE_FLOAT, TSMS_TYPE_STRING, TSMS_TYPE_CHAR, TSMS_TYPE_BOOL, TSMS_TYPE_DOUBLE, TSMS_TYPE_LONG, TSMS_TYPE_SHORT, TSMS_TYPE_BYTE,

	TSMS_TYPE_STATIC, TSMS_TYPE_RESERVED, TSMS_TYPE_CHECKSUM
} TSMS_TYPE;

#define TSMS_MASK(x) ((1<<x)-1)
#define TSMS_NULL NULL

#include "memory.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "stdbool.h"
#include "tsms_err.h"
#include "tsms_delay.h"

#endif //TSMS_DEF_H
