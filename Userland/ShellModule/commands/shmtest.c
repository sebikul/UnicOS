#include <libc.h>
#include "commands.h"
#include "syscalls.h"

void show_stat(mpoint_t *mp);

COMMAND_HELP(shmget, "[shmget] <user ID> <size> Crea una memoria compartida.");

COMMAND_START(shmget) {

	if ( argc != 3 ) {
		fprintf(stderr, "Comando invalido. Ingrese el tamano de la memoria compartida y el ID de usuario.\n");
	} else {
		uint64_t user_id = (uint64_t)ctoi(argv[1]);
		uint32_t shm_size = (uint32_t)ctoi(argv[2]);
		if ( shm_size <= 0 || shm_size > 4000 ) {
			fprintf(stderr, "Tamaño invalido.\n");
			return -1;
		}
		uint32_t shmid = sys_shm_get(shm_size, user_id);
		mpoint_t *mp = sys_shm_find(shmid);
		show_stat(mp);
		return mp->memid;
	}
	return 0;

}

void show_stat(mpoint_t *mp) {

	char* status;
	printf("ShmID: %d\n", mp->memid);
	printf("User: %d\n", mp->user);
	printf("Size: %d\n", (int)mp->size);
	printf("Used: %d\n", mp->used);
	switch(mp->locked) {
		
		case UNLOCKED:
			status = "UNLOCKED";
			break;

		case LOCKED_READ:
			status = "LOCKED_READ";
			break;

		case LOCKED_WRITE:
			status = "LOCKED_WRITE";
			break;
	}
	printf("Status: %s\n", status);
	printf("R_flag: %s\n", (mp->r_flag==TRUE)?"TRUE":"FALSE");
	printf("Attaches: %d\n", mp->atcount);
	//printf("Shmaddr: %d\n", (int)mp->shmaddr);
}

COMMAND_HELP(shmread, "[shmread] <size> <user ID> <shm ID> Lee de una memoria compartida.");

COMMAND_START(shmread) {

	if ( argc != 4 ) {
		fprintf(stderr, "Comando invalido. Ingrese tamano, ID de usuario y memoria.\n");
	} else {
		uint32_t size = (uint32_t)ctoi(argv[1]);
		if ( size <= 0 ) {
			fprintf(stderr, "Tamaño invalido.\n");
			return -1;
		}
		uint32_t user = (uint32_t)ctoi(argv[2]);
		uint32_t memid = (uint32_t)ctoi(argv[3]);
		char* data = malloc( sizeof(char) * size); 
		mpoint_t *mp = sys_shm_find(memid);
		if ( mp==NULL ) {
			fprintf(stderr, "ID de memoria invalido.\n");
			return -1;
		}
		size = sys_shm_read(data, sizeof(char)*size, user, mp);
		//TODO printf("Mensaje leido: %s.\n", data); //falla..asumo por el '0'
		return size;
	}
	return 0;
}

COMMAND_HELP(shmwrite, "[shmwrite] <user ID> <shm ID> <mensaje> Escribe en una memoria compartida.");

COMMAND_START(shmwrite) {

	if ( argc != 4 ) {
		fprintf(stderr, "Comando invalido.\n");
	} else {
		uint32_t user = (uint32_t)ctoi(argv[1]);
		uint32_t memid = (uint32_t)ctoi(argv[2]);
		uint32_t size = strlen(argv[3]);
		mpoint_t *mp = sys_shm_find(memid);
		if ( mp==NULL ) {
			fprintf(stderr, "ID de memoria invalido.\n");
			return -1;
		}
		return sys_shm_write(argv[3], sizeof(char)*size, user, mp); 
	}
	return 0;
}

COMMAND_HELP(shmctl, "[shmctl] <comando> <user ID> <shm ID> Ejecuta comandos de memoria compartida.\n Comandos: READ WRITE UNLOCK REMOVE SETU");
COMMAND_START(shmctl) {

	if ( argc != 4 ) {
		fprintf(stderr, "Comando invalido. Ingrese el tipo de comando, y los ID de usuario y memoria.\n");
	} else {
		uint32_t cmd;
		if ( strcmp(argv[1], "READ") == 0 ) {
			cmd = SHM_RLOCK;
		} else if ( strcmp(argv[1], "WRITE") == 0 ) {
			cmd = SHM_WLOCK;
		} else if ( strcmp(argv[1], "UNLOCK") == 0 ) {
			cmd = SHM_UNLOCK;
		} else if ( strcmp(argv[1], "REMOVE") == 0 ) {
			cmd = SHM_RMID;
		} else if ( strcmp(argv[1], "SETU") == 0 ) {
			cmd = SHM_SETU;
		} else {
			fprintf(stderr, "Comando invalido.\n");
			return -1;
		}
		uint32_t user = (uint32_t)ctoi(argv[2]);
		uint32_t memid = (uint32_t)ctoi(argv[3]);
		mpoint_t *mp = sys_shm_find(memid);
		if ( mp==NULL ) {
			fprintf(stderr, "ID de memoria invalido.\n");
			return -1;
		}
		bool ans = sys_shm_ctl(cmd, user, mp);
		if ( ans ) {
			switch( cmd ) {
				case SHM_RLOCK:
					fprintf(stderr, "Bloqueo de lectura activado.\n");
					break;

				case SHM_WLOCK:
					fprintf(stderr, "Bloqueo de escritura activado.\n");
					break;

				case SHM_UNLOCK:
					fprintf(stderr, "Memoria compartida desbloqueada.\n");
					break;

				case SHM_RMID:
					if( mp->r_flag ){
						fprintf(stderr, "Memoria compartida removible.\n");
					} else {
						fprintf(stderr, "Memoria compartida no removible.\n");
					}
					break;

				case SHM_SETU:
					fprintf(stderr, "El usuario %d usara la memoria compartida.\n", mp->user);
					break;
			}
		} else {
			fprintf(stderr, "Comando no ejecutado.\n");
			return -1;
		}
	}
	return 0;
}

COMMAND_HELP(shmstatus, "[shmstatus] <shm ID> Muestra el estado de una memoria compartida.");

COMMAND_START(shmstatus) {

	if ( argc != 2 ) {
		fprintf(stderr, "Comando invalido. Ingrese el ID de la memoria.\n");
	} else {
		uint32_t memid = (uint32_t)ctoi(argv[1]);
		mpoint_t *mp = sys_shm_find(memid);
		if ( mp == NULL ) {
			fprintf(stderr, "ID de memoria invalido.\n");
			return -1;
		}
		char* status;
		printf("ShmID: %d\n", mp->memid);
		printf("User: %d\n", mp->user);
		printf("Size: %d\n", (int)mp->size);
		printf("Used: %d\n", mp->used);
		switch(mp->locked) {
			
			case UNLOCKED:
				status = "UNLOCKED";
				break;

			case LOCKED_READ:
				status = "LOCKED_READ";
				break;

			case LOCKED_WRITE:
				status = "LOCKED_WRITE";
				break;
		}
		printf("Status: %s\n", status);
		printf("R_flag: %s\n", (mp->r_flag==TRUE)?"TRUE":"FALSE");
		printf("Attaches: %d\n", mp->atcount);
		//printf("Shmaddr: %d\n", mp->shmaddr);
	}
	return 0;
}

COMMAND_HELP(shmfree, "[shmfree] <shm ID> Libera una memoria compartida.");

COMMAND_START(shmfree) {

	if ( argc != 2 ) {
		fprintf(stderr, "Comando invalido. Ingrese el ID de la memoria.\n");
	} else {
		uint32_t memid = (uint32_t)ctoi(argv[1]);
		mpoint_t *mp = sys_shm_find(memid);
		if ( mp == NULL ) {
			fprintf(stderr, "ID de memoria invalido.\n");
			return -1;
		}
		if ( mp->atcount !=0 || !mp->r_flag) {
			printf("No se puede eliminar la memoria, todavia esta en uso o la memoria no esta habilitada para ser removida.\n");
		} else {
			printf("Memoria liberada correctamente.\n");
			sys_shm_free(mp);	
		}
	}
	return 0;
}