#include <libc.h>
#include "commands.h"
#include "syscalls.h"

COMMAND_HELP(shmget, "[shmget] <size> <user ID> Crea una memoria compartida.");

COMMAND_START(shmget) {

	if (argc != 3) {
		fprintf(stderr, "Comando invalido. Ingrese el tamaño de la memoria compartida y el ID de usuario.\n");
	} else {
		uint32_t user_id = (uint32_t)ctoi(argv[1]);
		uint64_t shm_size = (uint64_t)ctoi(argv[2]);
		//char* data_owner = malloc(sizeof(char)*40);
		//char* data_partner = malloc(sizeof(char)*50);
		//memcpy(data_partner, "TESTEANDO MEMORIA", sizeof(char)*17);
		//uint32_t amount = 20;
		//uint32_t attempt;

		uint32_t shmid= sys_shm_get(shm_size, user_id);
		mpoint_t *mp = sys_shm_find(shmid);
		show_stat(mp);
		return mp->memid;
		/*
		attempt = sys_shm_ctl(SHM_SETU, partner_id, super_shm);
		printf("PARTNER SETTING... %d\n", attempt);
		attempt = sys_shm_ctl(SHM_WLOCK, partner_id, super_shm);
		printf("PARTNER LOCKING TO WRITE... %d\n", attempt);

		attempt = sys_shm_read(data_owner, 40, user_id, super_shm);
		printf("OWNER FAILED ATTEMPT TO READ... %d\n", attempt);
		show_stat(super_shm);

		attempt = sys_shm_write(data_partner, sizeof(char)*17, partner_id, super_shm);
		printf("PARTNER WRITING... %d\n", attempt);
		show_stat(super_shm);

		attempt = sys_shm_read(data_owner, sizeof(char)*17, user_id, super_shm);
		printf("OWNER READING... %d\n", attempt);
		printf("OWNER READ: %s\n", data_owner);

		sys_shm_dt(super_shm);
		sys_shm_free(super_shm);
		*/
	}

	return 0;

}

void show_stat(mpoint_t *mp) {

	char* status;
	printf("ShmID: %d\n", mp->memid);
	printf("User: %d\n", mp->user);
	printf("Size: %d\n", mp->size);
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
	printf("Shmaddr: %d\n", mp->shmaddr);
}

COMMAND_HELP(shmread, "[shmread] <size> <user ID> <shm ID> Lee de una memoria compartida.");

COMMAND_START(shmread) {

	if (argc != 4) {
		fprintf(stderr, "Comando invalido.\n");
	} else {
		uint32_t size = (uint32_t)ctoi(argv[1]);
		uint32_t user = (uint32_t)ctoi(argv[2]);
		uint32_t memid = (uint32_t)ctoi(argv[3]);
		char* data = malloc(sizeof(char)*size);
		mpoint_t *mp = sys_shm_find(memid);
		
		sys_shm_read(data, sizeof(char)*size, user, mp);
		printf("Mensaje leido: %d.\n", data);
	}

	return 0;
}

COMMAND_HELP(shmwrite, "[shmwrite] <size> <user ID> <shm ID> <mensaje> Escribe en una memoria compartida.");

COMMAND_START(shmwrite) {

	if (argc != 5) {
		fprintf(stderr, "Comando invalido.\n");
	} else {
		uint32_t size = (uint32_t)ctoi(argv[1]);
		uint32_t user = (uint32_t)ctoi(argv[2]);
		uint32_t memid = (uint32_t)ctoi(argv[3]);
		char* data = argv[3]; 
		mpoint_t *mp = sys_shm_find(memid);
		
		sys_shm_write(data, sizeof(char)*size, user, mp);
		printf("Mensaje escrito: %d.\n", data);
	}

	return 0;
}

COMMAND_HELP(shmctl, "[shmctl] <comando> <user ID> <shm ID> Ejecuta comandos en una memoria compartida.");

COMMAND_START(shmctl) {

	if (argc != 4) {
		fprintf(stderr, "Comando invalido. Ingrese el tipo de comando, y los ID de usuario y memoria.\n");
	} else {
		uint32_t cmd = (uint32_t)ctoi(argv[1]);
		uint32_t user = (uint32_t)ctoi(argv[2]);
		uint32_t memid = (uint32_t)ctoi(argv[3]);
		mpoint_t *mp = sys_shm_find(memid);
		//manejo de respuesta.. TODO
		return sys_shm_ctl(cmd, user, mp);

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

		char* status;
		printf("ShmID: %d\n", mp->memid);
		printf("User: %d\n", mp->user);
		//printf("Size: %d\n", mp->size);
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
		//printf("Status: %s\n", status);
		//printf("R_flag: %s\n", (mp->r_flag==TRUE)?"TRUE":"FALSE");
		//printf("Attaches: %d\n", mp->atcount);
		//printf("Shmaddr: %d\n", mp->shmaddr);
	}

	return 0;
}

COMMAND_HELP(shmfree, "[shmfree] <shm ID> Libera una memoria compartida.");

COMMAND_START(shmfree) {

	if (argc != 2) {
		fprintf(stderr, "Comando invalido. Ingrese el ID de la memoria.\n");
	} else {
		uint32_t memid = (uint32_t)ctoi(argv[1]);
		mpoint_t *mp = sys_shm_find(memid);

		//manejo de attach.. TODO
		sys_shm_free(mp);
	}

	return 0;
}