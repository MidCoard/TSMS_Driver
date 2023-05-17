#ifndef TSMS_LINK_LIST_H
#define TSMS_LINK_LIST_H

typedef struct TSMS_LINK_LIST *TSMS_LINK_LIST_POINTER;
typedef TSMS_LINK_LIST_POINTER TSMS_LKLP;

typedef struct TSMS_LINK_NODE *TSMS_LINK_NODE_POINTER;
typedef TSMS_LINK_NODE_POINTER TSMS_LKNP;

#include "tsms_def.h"

struct TSMS_LINK_NODE {
	void *element;
	TSMS_LKNP next;
	TSMS_LKNP prev;
};

struct TSMS_LINK_LIST {
	TSMS_LKNP head;
	TSMS_LKNP tail;
	TSMS_SIZE length;
};

TSMS_LKLP TSMS_LINK_LIST_create();

TSMS_RESULT TSMS_LINK_LIST_add(TSMS_LKLP list, void *element);

TSMS_RESULT TSMS_LINK_LIST_remove(TSMS_LKLP list, TSMS_POS index);

TSMS_POS TSMS_LINK_LIST_removeElement(TSMS_LKLP list, void *element);

TSMS_RESULT TSMS_LINK_LIST_release(TSMS_LKLP list);

TSMS_RESULT TSMS_LINK_LIST_clear(TSMS_LKLP list);

TSMS_RESULT TSMS_LINK_LIST_insert(TSMS_LKLP list, void *element, TSMS_POS index);

TSMS_RESULT TSMS_LINK_LIST_deleteNode(TSMS_LKLP list, TSMS_LKNP node);

#endif //TSMS_LINK_LIST_H
