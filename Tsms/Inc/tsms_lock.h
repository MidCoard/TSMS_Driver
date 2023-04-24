#ifndef TSMS_LOCK_H
#define TSMS_LOCK_H

typedef struct TSMS_LOCK_HANDLER tLock;
typedef tLock *pLock;

typedef struct TSMS_SEQUENCE_LOCK_HANDLER tSequenceLock;
typedef tSequenceLock *pSequenceLock;

typedef struct TSMS_SEQUENCE_PRIORITY_LOCK_HANDLER tSequencePriorityLock;
typedef tSequencePriorityLock *pSequencePriorityLock;

#include "tsms_def.h"

struct TSMS_LOCK_HANDLER {
	bool locked;
};

struct TSMS_SEQUENCE_LOCK_HANDLER {
	pLock lock;
	TSMS_STP stack;
};

struct TSMS_SEQUENCE_PRIORITY_LOCK_HANDLER {
	pSequenceLock sequenceLock;
	pLock lock;
	pLock priorityLock;
	TSMS_INT_STP stack;
};

pLock TSMS_LOCK_create();

bool TSMS_LOCK_tryLock(pLock lock);

bool TSMS_LOCK_lock(pLock lock);

bool TSMS_LOCK_unlock(pLock lock);

TSMS_RESULT TSMS_LOCK_release(pLock lock);

pSequenceLock TSMS_SEQUENCE_LOCK_create();

pLock TSMS_SEQUENCE_LOCK_currentLock(pSequenceLock lock);

pLock TSMS_SEQUENCE_LOCK_tryLock(pSequenceLock lock, pLock currentLock);

pLock TSMS_SEQUENCE_LOCK_lock(pSequenceLock lock, pLock currentLock);

TSMS_RESULT TSMS_SEQUENCE_LOCK_unlock(pSequenceLock lock, pLock currentLock);

TSMS_RESULT TSMS_SEQUENCE_LOCK_release(pSequenceLock lock);

pSequencePriorityLock TSMS_SEQUENCE_PRIORITY_LOCK_create();

TSMS_RESULT TSMS_SEQUENCE_PRIORITY_LOCK_preLock(pSequencePriorityLock lock, int priority);

TSMS_RESULT TSMS_SEQUENCE_PRIORITY_LOCK_postLock(pSequencePriorityLock lock, int priority);

pLock TSMS_SEQUENCE_PRIORITY_LOCK_tryLock(pSequencePriorityLock lock, pLock currentLock, int priority);

pLock TSMS_SEQUENCE_PRIORITY_LOCK_lock(pSequencePriorityLock lock, pLock currentLock, int priority);

TSMS_RESULT TSMS_SEQUENCE_PRIORITY_LOCK_unlock(pSequencePriorityLock lock, pLock currentLock);

TSMS_RESULT TSMS_SEQUENCE_PRIORITY_LOCK_release(pSequencePriorityLock lock);


#endif //TSMS_LOCK_H
