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

typedef struct TSMS_CUSTOM_HANDLER *TSMS_CUSTOM_HANDLER_POINTER;
typedef TSMS_CUSTOM_HANDLER_POINTER TSMS_CHP;

typedef struct TSMS_IIC_HANDLER *TSMS_IIC_HANDLER_POINTER;
typedef TSMS_IIC_HANDLER_POINTER TSMS_IHP;

typedef struct TSMS_SPI_HANDLER *TSMS_SPI_HANDLER_POINTER;
typedef TSMS_SPI_HANDLER_POINTER TSMS_SHP;

typedef struct TSMS_GPIO_HANDLER *TSMS_GPIO_HANDLER_POINTER;
typedef TSMS_GPIO_HANDLER_POINTER TSMS_GHP;

typedef struct TSMS_TOUCH_HANDLER *TSMS_TOUCH_HANDLER_POINTER;
typedef TSMS_TOUCH_HANDLER_POINTER TSMS_THP;

typedef struct TSMS_SCREEN_HANDLER *TSMS_SCREEN_HANDLER_POINTER;
typedef TSMS_SCREEN_HANDLER_POINTER TSMS_SCHP;

typedef struct TSMS_DISPLAY_HANDLER * TSMS_DISPLAY_HANDLER_POINTER;
typedef TSMS_DISPLAY_HANDLER_POINTER TSMS_DPHP;

typedef struct TSMS_DRIVER_HANDLER *TSMS_DRIVER_HANDLER_POINTER;
typedef TSMS_DRIVER_HANDLER_POINTER TSMS_DHP;

typedef struct TSMS_REGISTER_HANDLER *TSMS_REGISTER_HANDLER_POINTER;
typedef TSMS_REGISTER_HANDLER_POINTER TSMS_RHP;

typedef struct TSMS_REGISTER_HANDLER_LIST *TSMS_REGISTER_HANDLER_LIST_POINTER;
typedef TSMS_REGISTER_HANDLER_LIST_POINTER TSMS_RHLP;

typedef struct TSMS_PRINTER_HANDLER *TSMS_PRINTER_HANDLER_POINTER;
typedef TSMS_PRINTER_HANDLER_POINTER TSMS_PHP;

typedef struct TSMS_SYNC_STACK *TSMS_SYNC_STACK_POINTER;
typedef TSMS_SYNC_STACK_POINTER TSMS_SYNC_STP;



typedef struct TSMS_IT_GPIO *TSMS_IT_GPIO_POINTER;
typedef TSMS_IT_GPIO_POINTER TSMS_IGP;

typedef struct TSMS_IT_PRINTER *TSMS_IT_PRINTER_POINTER;
typedef TSMS_IT_PRINTER_POINTER TSMS_IPP;

typedef struct TSMS_IT_TIMER *TSMS_IT_TIMER_POINTER;
typedef TSMS_IT_TIMER_POINTER TSMS_ITP;



typedef struct TSMS_LOCK_HANDLER tLock;
typedef tLock *pLock;

typedef struct TSMS_SEQUENCE_LOCK_HANDLER tSequenceLock;
typedef tSequenceLock *pSequenceLock;

typedef struct TSMS_SEQUENCE_PRIORITY_LOCK_HANDLER tSequencePriorityLock;
typedef tSequencePriorityLock *pSequencePriorityLock;



typedef struct TSMS_STACK *TSMS_STACK_POINTER;
typedef TSMS_STACK_POINTER TSMS_STP;

typedef struct TSMS_INT_STACK *TSMS_INT_STACK_POINTER;
typedef TSMS_INT_STACK_POINTER TSMS_INT_STP;



typedef struct TSMS_LINK_LIST *TSMS_LINK_LIST_POINTER;
typedef TSMS_LINK_LIST_POINTER TSMS_LKLP;

typedef struct TSMS_LINK_NODE TSMS_LKN;
typedef TSMS_LKN *TSMS_LINK_NODE_POINTER;
typedef TSMS_LINK_NODE_POINTER TSMS_LKNP;

typedef struct TSMS_INT_LINK_LIST *TSMS_INT_LINK_LIST_POINTER;
typedef TSMS_INT_LINK_LIST_POINTER TSMS_INT_LKLP;

typedef struct TSMS_INT_LINK_NODE TSMS_INT_LKN;
typedef TSMS_INT_LKN *TSMS_INT_LINK_NODE_POINTER;
typedef TSMS_INT_LINK_NODE_POINTER TSMS_INT_LKNP;



typedef struct TSMS_LIST *TSMS_LIST_POINTER;
typedef TSMS_LIST_POINTER TSMS_LP;

typedef struct TSMS_CHAR_LIST *TSMS_CHAR_LIST_POINTER;
typedef TSMS_CHAR_LIST_POINTER TSMS_CLP;

typedef struct TSMS_INT_LIST *TSMS_INT_LIST_POINTER;
typedef TSMS_INT_LIST_POINTER TSMS_ILP;

typedef struct TSMS_LONG_LIST *TSMS_LONG_LIST_POINTER;
typedef TSMS_LONG_LIST_POINTER TSMS_LLP;



typedef struct TSMS_MAP_NODE * TSMS_MAP_NODE_POINTER;
typedef TSMS_MAP_NODE_POINTER TSMS_MNP;

typedef struct TSMS_MAP * TSMS_MAP_POINTER;
typedef TSMS_MAP_POINTER TSMS_MP;

typedef struct TSMS_MAP_ITERATOR TSMS_MI;
typedef TSMS_MI * TSMS_MIP;

typedef struct TSMS_MAP_ENTRY TSMS_ME;

typedef struct TSMS_LONG_MAP_NODE * TSMS_LONG_MAP_NODE_POINTER;
typedef TSMS_LONG_MAP_NODE_POINTER TSMS_LMNP;

typedef struct TSMS_LONG_MAP * TSMS_LONG_MAP_POINTER;
typedef TSMS_LONG_MAP_POINTER TSMS_LMP;

typedef struct TSMS_LONG_MAP_ITERATOR TSMS_LMI;
typedef TSMS_LMI * TSMS_LMIP;

typedef struct TSMS_LONG_MAP_ENTRY TSMS_LME;



typedef struct TSMS_SET * TSMS_SET_POINTER;
typedef TSMS_SET_POINTER TSMS_SP;

typedef struct TSMS_SET_ITERATOR TSMS_SI;
typedef TSMS_SI * TSMS_SIP;

typedef struct TSMS_LONG_SET *TSMS_LONG_SET_POINTER;
typedef TSMS_LONG_SET_POINTER TSMS_LSP;

typedef struct TSMS_LONG_SET_ITERATOR TSMS_LSI;
typedef TSMS_LSI *TSMS_LSIP;



typedef struct TSMS_DEQUE *TSMS_DEQUE_POINTER;
typedef TSMS_DEQUE_POINTER TSMS_DP;



typedef struct TSMS_COLOR TSMS_CR;
typedef struct TSMS_SSD1963_OPTION TSMS_SSD1963_OPTION;


typedef struct TSMS_TIMER_HANDLER tTimer;
typedef tTimer *pTimer;

typedef struct TSMS_STRING_HANDLER tString;
typedef tString *pString;

typedef void(*TSMS_IT_PRINTER_CALLBACK)(void *, TSMS_PHP);

typedef enum {
	TSMS_IT_GPIO_FALLING, TSMS_IT_GPIO_RISING
} TSMS_IT_GPIO_TYPE;

typedef void(*TSMS_IT_GPIO_CALLBACK)(void *, TSMS_GHP);

typedef enum {
	TSMS_IT_TIMER_PERIOD_ELAPSED, TSMS_IT_TIMER_CAPTURE
} TSMS_IT_TIMER_TYPE;

typedef void(*TSMS_IT_TIMER_CALLBACK)(void *, pTimer);

#include "memory.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "stdbool.h"
#include "tsms_err.h"
#include "tsms_delay.h"

#endif //TSMS_DEF_H
