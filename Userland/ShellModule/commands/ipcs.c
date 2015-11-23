#include <libc.h>
#include "commands.h"
#include "syscalls.h"

void show_sem(semaphore_t *sem);
void show_shm(mpoint_t *mp);

COMMAND_HELP(ipcs, "[ipcs] Muestra la lista de los ipcs instanciados.");

COMMAND_START(ipcs) {

	if ( argc != 1 ) {
		fprintf(stderr, "Comando invalido. Cantidad de argumentos incompatible.\n");
	} else {
		uint32_t sem_size = sys_sem_count();
		uint32_t shm_size = sys_shm_count();
		uint32_t sem_cont = 0;
		uint32_t shm_cont = 0;
		uint32_t size = (sem_size > shm_size)? sem_size : shm_size;
		for ( int i = 0 ; i < size; i++ ) {
			if( i < sem_size ) {
				semaphore_t *sem = sys_sem_find(i);
				if( sem != NULL ) {
					show_sem(sem);
					sem_cont++;
				}
			}
			if( i < shm_size ) {
				mpoint_t *mp = sys_shm_find(i);
				if( mp != NULL) {
					show_shm(mp);
					shm_cont++;
				}
			}
		}
		printf("TOTAL:\t\tSemaforos: %d\t\tColas: %d\t\tMemorias: %d\n", sem_cont, sem_cont, shm_cont );
	}
	return 0;
}

void show_sem(semaphore_t *sem) {
	printf("Semaforo de ID:  %d\n", sem->id);
	printf("Cola de mensajes de tamaño:  %d\n", sem->queue->size);
}

void show_shm(mpoint_t *mp) {
	printf("Memoria compartida de ID:  %d\n", mp->memid);
}