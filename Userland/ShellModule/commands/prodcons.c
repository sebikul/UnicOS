#include <libc.h>
#include "commands.h"
#include "syscalls.h"

uint32_t put(const char* item, mpoint_t *mp, semaphore_t *sem);
char* get(mpoint_t *mp, semaphore_t *sem);

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
		uint32_t size = put(argv[2], mp, sem);
		printf("Escritura realizada..\n");
		return size;
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
		char* data = get(mp, sem);
		printf("Lectura realizada...%s\n", data);
	}
	return 0;
}

uint32_t put (const char* item, mpoint_t *mp, semaphore_t *sem) {
	while ( mp->used != 0 ) {
		pid_t pid = sys_task_get_pid(); 
		sys_sem_wait(sem, pid, 0);
	}
	sys_shm_ctl(SHM_WLOCK, mp->user, mp);
	uint32_t size = sys_shm_write(item, strlen(item), mp->user, mp);
	sys_sem_sig(sem);
	return size;
}

char* get (mpoint_t *mp, semaphore_t *sem) {
	while (mp->used == 0) {
		pid_t pid = sys_task_get_pid(); 
		sys_sem_wait(sem, pid, 0);
	}
	char * data = malloc( sizeof(char) * mp->size);
	sys_shm_read(data, mp->size, mp->user, mp);
	sys_sem_sig(sem);
	return data;
}