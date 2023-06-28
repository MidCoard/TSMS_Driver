#ifndef TSMS_FUNCTION_DEF_H
#define TSMS_FUNCTION_DEF_H

typedef long (*TSMS_COMPARE_FUNCTION)(void *a, void *b);

typedef long (*TSMS_HASH_FUNCTION)(void* key);

typedef TSMS_RESULT (*TSMS_FREE_FUNCTION)(void *data);

typedef long (*TSMS_CHECKSUM_FUNCTION)(const uint8_t* data, TSMS_SIZE size);

#endif //TSMS_FUNCTION_DEF_H
