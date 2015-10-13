#include <stdint.h>
#include "msgqueue.h"
#include "string.h"
#include "mem.h"
#include "kernel.h"

typedef struct message message_t;

struct message {
	message_t *next;
	void *msg;
	int size;
};

typedef struct message message_t;

struct msgqueue {
	message_t *first;
	message_t *last;
	uint32_t size;
	uint32_t maxsize;
	volatile bool running;
};

static void message_delete(message_t * message) {
	free(message->msg);
	free(message);
}

static void message_deep_delete(message_t * message) {
	if (message->next != NULL) {
		message_deep_delete(message->next);
	}

	message_delete(message);
}

msgqueue_t* msgqueue_create(uint32_t maxsize) {
	msgqueue_t *msgqueue = malloc(sizeof(msgqueue_t));

	msgqueue->first = NULL;
	msgqueue->last = NULL;
	msgqueue->size = 0;
	msgqueue->maxsize = maxsize;
	msgqueue->running = FALSE;

	return msgqueue;
}

void msgqueue_add(msgqueue_t *msgqueue, void* msg, int size) {

	intsoff();

	while (msgqueue->running);
	msgqueue->running = TRUE;

	if (msgqueue->size == msgqueue->maxsize) {
		return;
	}

	message_t *message = malloc(sizeof(message_t));

	message->size = size;
	message->msg = malloc(size);
	memcpy(message->msg, msg, size);
	message->next = NULL;

	if (msgqueue->first == NULL) {
		msgqueue->first = msgqueue->last = message;
	} else {
		msgqueue->last->next = message;
		msgqueue->last = message;
	}

	msgqueue->size++;

	//kdebug("Encolando mensaje. Size: ");
	//kdebug_base(msgqueue->size, 10);
	//kdebug_nl();

	msgqueue->running = FALSE;

	intson();
}

void msgqueue_undo(msgqueue_t *msgqueue) {

intsoff();

	while (msgqueue->running);
	msgqueue->running = TRUE;

	message_t *message = msgqueue->first;

	if (message == NULL) {
		return;
	}

	msgqueue->first = message->next;

	if (msgqueue->first == NULL) {
		msgqueue->last == NULL;
	}

	msgqueue->size--;

	message_delete(message);

	//kdebug("Deshaciendo agragado de mensaje. Nuevo size: ");
	// kdebug_base(msgqueue->size, 10);
	// kdebug_nl();

	msgqueue->running = FALSE;

	intson();
}

void* msgqueue_deq(msgqueue_t *msgqueue) {

	void* msg;
	message_t *message;

intsoff();

	while (msgqueue->running);
	msgqueue->running = TRUE;

	while (msgqueue->first == NULL);

	message = msgqueue->first;

	msg = malloc(message->size);
	memcpy(msg, message->msg, message->size);

	msgqueue->first = message->next;

	if (msgqueue->first == NULL) {
		//kdebug("Ultimo mensaje de la cola eliminado.\n");
		msgqueue->last == NULL;
	}

	message_delete(message);

	msgqueue->size--;

	//kdebug("Desencolando mensaje. Size: ");
	// kdebug_base(msgqueue->size, 10);
	// kdebug_nl();
	msgqueue->running = FALSE;
	intson();

	return msg;
}

void* msgqueue_peeklast(msgqueue_t *msgqueue) {

	void* msg;
	message_t *message;

	message = msgqueue->last;

	if (msgqueue->size == 0) {
		//kdebug("Espiando cola vacia.\n");
		return NULL;
	}

	msg = malloc(message->size);
	memcpy(msg, message->msg, message->size);

	//kdebug("Espiando ultimo mensaje. Size: ");
	// kdebug_base(msgqueue->size, 10);
	// kdebug_nl();

	return msg;
}

void msgqueue_clear(msgqueue_t *msgqueue) {

intsoff();

	while (msgqueue->running);
	msgqueue->running = TRUE;

	if (msgqueue->first == NULL) {
		return;
	}

	message_deep_delete(msgqueue->first);

	msgqueue->first = NULL;
	msgqueue->last = NULL;

	msgqueue->size = 0;

	msgqueue->running = FALSE;
	intson();
}

bool msgqueue_isempty(msgqueue_t *msgqueue) {
	return (msgqueue->first == NULL);
}

int msgqueue_size(msgqueue_t *msgqueue) {
	return msgqueue->size;
}

void msgqueue_delete(msgqueue_t *msgqueue) {

	while (msgqueue->running);
	msgqueue->running = TRUE;

	if (msgqueue->first != NULL) {
		message_deep_delete(msgqueue->first);
	}

	msgqueue->running = FALSE;

	free(msgqueue);
}

