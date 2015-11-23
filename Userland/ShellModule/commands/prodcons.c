#include <libc.h>
#include "commands.h"
#include "syscalls.h"

void put(char* item, mpoint_t *mp);
char* get(mpoint_t *mp);

uint32_t init = 999; //no negatives :(

COMMAND_HELP(producer, "[producer] <shm ID> <mensaje> Agrega un item a la lista.");

COMMAND_START(producer) {

	if ( argc != 3 ) {
		fprintf(stderr, "Comando invalido. Cantidad de argumentos incompatible.\n");
	} else {
		uint32_t memid = (uint32_t)ctoi(argv[1]);
		mpoint_t *mp = sys_shm_find(memid);
		if ( mp==NULL ) {
			fprintf(stderr, "ID de memoria invalido.\n");
			return -1;
		}
		if( init == 999 ) 
			init = sys_sem_get(0);

		semaphore_t *sem = sys_sem_find(init);
		//sys_sem_wait(sem, pid, msec);
	}
	return 0;
}

COMMAND_HELP(consumer, "[consumer] <shm ID> Consume un item de la lista.");

COMMAND_START(consumer) {

	if ( argc != 2 ) {
		fprintf(stderr, "Comando invalido. Cantidad de argumentos incompatible.\n");
	} else {
		uint32_t memid = (uint32_t)ctoi(argv[1]);
		mpoint_t *mp = sys_shm_find(memid);
		if ( mp==NULL ) {
			fprintf(stderr, "ID de memoria invalido.\n");
			return -1;
		}
		if( init == 999 ) 
			init = sys_sem_get(0);
		
		semaphore_t *sem = sys_sem_find(init);
	}
	return 0;
}

void put (char* item, mpoint_t *mp) {
	while ( mp->used != 0 ){}
		//sys_sem_wait();

	//poner_fifo(item);
}

char* get (mpoint_t *mp) {
	while (mp->used == 0){}
		//sys_sem_wait();

	return NULL;//sacar_fifo();
}