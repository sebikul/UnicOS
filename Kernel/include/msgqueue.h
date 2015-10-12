#include "types.h"

#ifndef MSGQUEUE_H
#define MSGQUEUE_H

typedef struct msgqueue msgqueue_t;

msgqueue_t* msgqueue_create(uint32_t maxsize);
void msgqueue_add(msgqueue_t *msgqueue, void* msg, int size);
void msgqueue_undo(msgqueue_t *msgqueue);
void* msgqueue_deq(msgqueue_t *msgqueue_t);
void* msgqueue_peek(msgqueue_t *msgqueue);
void msgqueue_clear(msgqueue_t *msgqueue);
bool msgqueue_isempty(msgqueue_t *msgqueue);
int msgqueue_size(msgqueue_t *msgqueue);
void msgqueue_delete(msgqueue_t *msgqueue);


#endif