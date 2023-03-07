#ifndef TSMS_LIST_H
#define TSMS_LIST_H

struct TSMS_LIST_LIST;

typedef struct TSMS_LIST_LIST *TSMS_LIST_LIST_POINTER;
typedef TSMS_LIST_LIST_POINTER TSMS_LLP;

struct TSMS_LIST_CHAR_LIST;

typedef struct TSMS_LIST_CHAR_LIST *TSMS_LIST_CHAR_LIST_POINTER;
typedef TSMS_LIST_CHAR_LIST_POINTER TSMS_LCLP;

struct TSMS_LIST_INT_LIST;

typedef struct TSMS_LIST_INT_LIST *TSMS_LIST_INT_LIST_POINTER;
typedef TSMS_LIST_INT_LIST_POINTER TSMS_LILP;

struct TSMS_LIST_LONG_LIST;

typedef struct TSMS_LIST_LONG_LIST *TSMS_LIST_LONG_LIST_POINTER;
typedef TSMS_LIST_LONG_LIST_POINTER TSMS_LLLP;

#include "tsms.h"

struct TSMS_LIST_LIST {
	void **list;
	TSMS_SIZE length;
	TSMS_SIZE actualLength;
	TSMS_SIZE initLength;
};


struct TSMS_LIST_CHAR_LIST {
	char *list;
	TSMS_SIZE length;
	TSMS_SIZE actualLength;
	TSMS_SIZE initLength;
};

struct TSMS_LIST_INT_LIST {
	int *list;
	TSMS_SIZE length;
	TSMS_SIZE actualLength;
	TSMS_SIZE initLength;
};

struct TSMS_LIST_LONG_LIST {
	long *list;
	TSMS_SIZE length;
	TSMS_SIZE actualLength;
	TSMS_SIZE initLength;
};



TSMS_LLP TSMS_LIST_createList(TSMS_SIZE initSize);

TSMS_LCLP TSMS_LIST_createCharList(TSMS_SIZE initSize);

TSMS_LILP TSMS_LIST_createIntList(TSMS_SIZE initSize);

TSMS_LLLP TSMS_LIST_createLongList(TSMS_SIZE initSize);

TSMS_RESULT TSMS_LIST_addList(TSMS_LLP list, void *element);

TSMS_RESULT TSMS_LIST_addCharList(TSMS_LCLP list, char element);

TSMS_RESULT TSMS_LIST_addIntList(TSMS_LILP list, int element);

TSMS_RESULT TSMS_LIST_addLongList(TSMS_LLLP list, long element);

TSMS_RESULT TSMS_LIST_removeList(TSMS_LLP list, TSMS_POS index);

TSMS_POS TSMS_LIST_removeListElement(TSMS_LLP list, void *element);

TSMS_RESULT TSMS_LIST_releaseList(TSMS_LLP list);

TSMS_RESULT TSMS_LIST_clearCharList(TSMS_LCLP list);

TSMS_RESULT TSMS_LIST_releaseCharList(TSMS_LCLP list);

TSMS_RESULT TSMS_LIST_clearLongList(TSMS_LLLP list);

TSMS_RESULT TSMS_LIST_releaseLongList(TSMS_LLLP list);

TSMS_RESULT TSMS_LIST_clearIntList(TSMS_LILP list);

TSMS_RESULT TSMS_LIST_releaseIntList(TSMS_LILP list);

TSMS_RESULT TSMS_LIST_clearList(TSMS_LLP list);

#endif //TSMS_LIST_H
