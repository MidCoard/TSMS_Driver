#include "tsms_lock.h"

pLock TSMS_LOCK_create() {
	pLock lock = (pLock)malloc(sizeof(tLock));
	lock->locked = false;
	return lock;
}

bool TSMS_LOCK_tryLock(pLock lock) {
	if (lock == TSMS_NULL)
		return false;
	return __sync_bool_compare_and_swap(&lock->locked, false, true);
}

// avoid to use in interrupt, it will block the interrupt if main thread gets the lock
bool TSMS_LOCK_lock(pLock lock) {
	if (lock == TSMS_NULL)
		return false;
	while (!TSMS_LOCK_tryLock(lock)) {
		//todo give up cpu
	}
	return true;
}

bool TSMS_LOCK_unlock(pLock lock) {
	if (lock == TSMS_NULL)
		return false;
	if (!lock->locked)
		return false;
	while(!__sync_bool_compare_and_swap(&lock->locked, true, false));
	return true;
}

TSMS_RESULT TSMS_LOCK_release(pLock lock) {
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	free(lock);
	return TSMS_SUCCESS;
}


pSequenceLock TSMS_SEQUENCE_LOCK_create() {
	pSequenceLock lock = (pSequenceLock)malloc(sizeof(tSequenceLock));
	lock->lock = TSMS_LOCK_create();
	lock->stack = TSMS_STACK_create();
	return lock;
}

pLock TSMS_SEQUENCE_LOCK_currentLock(pSequenceLock lock) {
	if (lock == TSMS_NULL)
		return TSMS_NULL;
	return TSMS_STACK_peek(lock->stack);
}

pLock TSMS_SEQUENCE_LOCK_tryLock(pSequenceLock lock, pLock currentLock) {
	if (lock == TSMS_NULL)
		return TSMS_NULL;
	if (!TSMS_LOCK_tryLock(lock->lock))
		return TSMS_NULL;
	if (TSMS_STACK_peek(lock->stack) == currentLock) {
		pLock newLock = TSMS_LOCK_create();
		if (!TSMS_LOCK_tryLock(newLock)) {
			TSMS_LOCK_release(newLock);
			TSMS_LOCK_unlock(lock->lock);
			return TSMS_NULL;
		}
		TSMS_STACK_push(lock->stack, newLock);
		TSMS_LOCK_unlock(lock->lock);
		return newLock;
	}
	TSMS_LOCK_unlock(lock->lock);
	return TSMS_NULL;
}

pLock TSMS_SEQUENCE_LOCK_lock(pSequenceLock lock, pLock currentLock) {
	if (lock == TSMS_NULL)
		return TSMS_NULL;
	pLock newLock;
	while ((newLock = TSMS_SEQUENCE_LOCK_tryLock(lock, currentLock)) == TSMS_NULL) {
		// todo give up cpu
	}
	return newLock;
}

TSMS_RESULT TSMS_SEQUENCE_LOCK_unlock(pSequenceLock lock, pLock currentLock) {
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LOCK_lock(lock->lock);
	if (TSMS_STACK_peek(lock->stack) == currentLock) {
		if (TSMS_STACK_empty(lock->stack)) {
			TSMS_LOCK_unlock(lock->lock);
			return TSMS_FAIL;
		}
		pLock newLock = (pLock)TSMS_STACK_pop(lock->stack);
		TSMS_LOCK_unlock(newLock);
		TSMS_LOCK_release(newLock);
		TSMS_LOCK_unlock(lock->lock);
		return TSMS_SUCCESS;
	}
	TSMS_LOCK_unlock(lock->lock);
	return TSMS_FAIL;
}

TSMS_RESULT TSMS_SEQUENCE_LOCK_release(pSequenceLock lock) {
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LOCK_release(lock->lock);
	TSMS_STACK_release(lock->stack);
	free(lock);
	return TSMS_SUCCESS;
}


pSequencePriorityLock TSMS_SEQUENCE_PRIORITY_LOCK_create() {
	pSequencePriorityLock lock = (pSequencePriorityLock)malloc(sizeof(tSequencePriorityLock));
	lock->lock = TSMS_LOCK_create();
	lock->priorityLock = TSMS_LOCK_create();
	lock->sequenceLock = TSMS_SEQUENCE_LOCK_create();
	lock->stack = TSMS_INT_STACK_create();
	return lock;
}

TSMS_RESULT TSMS_SEQUENCE_PRIORITY_LOCK_preLock(pSequencePriorityLock lock, int priority) {
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	if (TSMS_INT_STACK_peek(lock->stack) >= priority)
		return TSMS_FAIL;
	if (!TSMS_LOCK_tryLock(lock->priorityLock))
		return TSMS_FAIL;
	if (TSMS_INT_STACK_peek(lock->stack) >= priority) {
		TSMS_LOCK_unlock(lock->priorityLock);
		return TSMS_FAIL;
	}
	TSMS_INT_STACK_push(lock->stack, priority);
	TSMS_LOCK_unlock(lock->priorityLock);
	return TSMS_SUCCESS;
}

TSMS_RESULT TSMS_SEQUENCE_PRIORITY_LOCK_postLock(pSequencePriorityLock lock, int priority) {
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	if (TSMS_INT_STACK_peek(lock->stack) != priority)
		return TSMS_FAIL;
	if (!TSMS_LOCK_tryLock(lock->priorityLock))
		return TSMS_FAIL;
	if (TSMS_INT_STACK_peek(lock->stack) != priority) {
		TSMS_LOCK_unlock(lock->priorityLock);
		return TSMS_FAIL;
	}
	TSMS_INT_STACK_pop(lock->stack);
	TSMS_LOCK_unlock(lock->priorityLock);
	return TSMS_SUCCESS;
}

pLock TSMS_SEQUENCE_PRIORITY_LOCK_tryLock(pSequencePriorityLock lock, pLock currentLock, int priority) {
	if (lock == TSMS_NULL)
		return TSMS_NULL;
	int p = TSMS_INT_STACK_peek(lock->stack);
	if (p > priority)
		return TSMS_NULL;
	if (!TSMS_LOCK_tryLock(lock->lock))
		return TSMS_NULL;
	pLock cur = TSMS_SEQUENCE_LOCK_currentLock(lock->sequenceLock);
	p = TSMS_INT_STACK_peek(lock->stack);
	if ((cur != currentLock || currentLock == TSMS_NULL ) && p > priority) {
		TSMS_LOCK_unlock(lock->lock);
		return TSMS_NULL;
	}
	pLock newLock = TSMS_SEQUENCE_LOCK_tryLock(lock->sequenceLock, currentLock);
	TSMS_LOCK_unlock(lock->lock);
	return newLock;
}

pLock TSMS_SEQUENCE_PRIORITY_LOCK_lock(pSequencePriorityLock lock, pLock currentLock, int priority) {
	if (lock == TSMS_NULL)
		return TSMS_NULL;
	pLock newLock;
	while ((newLock = TSMS_SEQUENCE_PRIORITY_LOCK_tryLock(lock, currentLock, priority)) == TSMS_NULL) {
		// todo give up cpu
	}
	return newLock;
}

TSMS_RESULT TSMS_SEQUENCE_PRIORITY_LOCK_unlock(pSequencePriorityLock lock, pLock currentLock) {
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LOCK_lock(lock->lock);
	TSMS_RESULT result = TSMS_SEQUENCE_LOCK_unlock(lock->sequenceLock, currentLock);
	TSMS_LOCK_unlock(lock->lock);
	return result;
}

TSMS_RESULT TSMS_SEQUENCE_PRIORITY_LOCK_release(pSequencePriorityLock lock) {
	if (lock == TSMS_NULL)
		return TSMS_ERROR;
	TSMS_LOCK_release(lock->lock);
	TSMS_LOCK_release(lock->priorityLock);
	TSMS_SEQUENCE_LOCK_release(lock->sequenceLock);
	TSMS_INT_STACK_release(lock->stack);
	free(lock);
	return TSMS_SUCCESS;
}