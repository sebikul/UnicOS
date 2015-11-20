#include <libc.h>
#include "commands.h"
#include "syscalls.h"

COMMAND_HELP(semget, "[semget] <queue ID?> <value> <sem ID> Crea un semaforo con un valor inicial y un ID.");

COMMAND_START(semget) {

	if ( argc != 4 ) {
		fprintf(stderr, "Comando invalido. Cantidad de argumentos incompatible.\n");
	} else {
		//tomo una msgqueue? crea una vacia y le asigna tareas?
		msgqueue_t *queue;

		uint32_t value = ctoi(argv[2]);
		if( value < 0 ) {
			fprintf(stderr, "Valor negativo invalido.\n");
			return -1;
		}
		uint32_t semid = ctoi(argv[3]);
		sys_sem_get(queue,value,semid);
	}
	return 0;
}

COMMAND_HELP(semwait, "[semwait] <sem ID> De ser posible decrementa el valor del semaforo.\n" 
							"\t[semwait] <sem ID> <pid> De ser posible, bloquea una tarea.\n"
							"\t[semwait] <sem ID> <pid> <time> Bloquea una tarea por un tiempo(msec) determinado.");

COMMAND_START(semwait) {

	if ( argc == 2 ) {

		uint32_t semid = ctoi(argv[1]);
		semaphore_t *sem = sys_sem_find(semid);
		if ( sem == NULL) {
			fprintf(stderr, "ID invalido.\n");
			return -1;
		}
		if (sys_sem_wait(sem, NULL, 0)) {
			printf("Operacion ejecutada.\n");
		}else{
			printf("Operacion fallida.\n");
		}

	} else if ( argc == 3 ) {

		uint32_t semid = ctoi(argv[1]);
		semaphore_t *sem = sys_sem_find(semid);
		if ( sem == NULL) {
			fprintf(stderr, "ID invalido.\n");
			return -1;
		}
		pid_t pid = ctoi(argv[2]);
		if (sys_sem_wait(sem, pid, -1)) {
			printf("Operacion ejecutada.\n");
		} else {
			printf("Pid no encontrado.\n");
		}
	} else if ( argc == 4 ) {

		uint32_t semid = ctoi(argv[1]);
		semaphore_t *sem = sys_sem_find(semid);
		if ( sem == NULL) {
			fprintf(stderr, "ID invalido.\n");
			return -1;
		}
		pid_t pid = ctoi(argv[2]);
		uint32_t msec = ctoi(argv[3]);
		if( msec <= 0 ) {
			fprintf(stderr, "Tiempo negativo invalido.\n");
			return -1;
		}
		if (sys_sem_wait(sem, pid, msec)) {
			printf("Operacion ejecutada.\n");
		} else {
			printf("Pid no encontrado.\n");
		}
	} else {
		fprintf(stderr, "Cantidad invalida de parametros.\n");
		return -1;
	}
	return 0;
}

COMMAND_HELP(semsig, "[semsig] <sem ID> Desbloquea a la proxima tarea.");

COMMAND_START(semsig) {

	if ( argc != 2 ) {
		fprintf(stderr, "Comando invalido. Cantidad incorrecta de parametros.\n");
	} else {
		int32_t semid = ctoi(argv[1]);
		semaphore_t *sem = sys_sem_find(semid);
		if ( sem == NULL) {
			fprintf(stderr, "ID invalido.\n");
			return -1;
		}
		sys_sem_sig(sem);
		fprintf(stderr, "Accion realizada.\n");
	}
	return 0;
}