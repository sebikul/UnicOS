#include "types.h"
#include "shmem.h"
#include "mem.h"

uint32_t idcont=1;
mpoint_t* shmem_array[256]; 
 

void shmadd(mpoint_t *mp){
	shmem_array[idcont] = malloc(sizeof(mpoint_t));
	memcpy(shmem_array[idcont],mp,sizeof(mpoint_t));
	idcont++;
}

mpoint_t *shmget(uint32_t shmid){
	//manejo correcto... TODO
	if(shmid > idcont)
		return NULL;
	return shmem_array[shmid];
}

uint32_t shmcreate(uint64_t size, uint32_t user) {
	mpoint_t *mem = malloc(sizeof(mpoint_t));
	mem->memid = idcont;
	mem->user = user;
	mem->size = size;
	mem->used = 0;
	mem->locked = UNLOCKED;
	mem->r_flag = FALSE;
	mem->atcount = 0;
	mem->shmaddr = malloc(mem->size);
	shmadd(mem);
	return mem->memid;
}

bool shmctl(uint32_t cmd, uint32_t user, mpoint_t *mp) {

	switch(cmd) {
		case SHM_RLOCK: 

			if ( mp->locked != UNLOCKED || mp->user != user ) 
				return FALSE;

			mp->locked = LOCKED_READ;
			break;

		case SHM_WLOCK:

			if ( mp->locked != UNLOCKED || mp->user != user ) 
				return FALSE;

			mp->locked = LOCKED_WRITE;
			break;

		case SHM_UNLOCK:

			if ( mp->user != user ) 
				return FALSE;

			mp->locked = UNLOCKED;
			break;

		case SHM_RMID:

			if ( mp->r_flag ) {
				mp->r_flag = FALSE;
			} else {
				mp->r_flag = TRUE;
			}
			break;

		case SHM_SETU:

			mp->user = user;
			break;

		default:

			return FALSE;			
	}

	return TRUE;
}

uint32_t shm_read(char* data, uint32_t size, uint32_t user, mpoint_t *mp) {

	if ( mp->locked == LOCKED_WRITE ) 
		return -1;

	shmctl(SHM_RLOCK, user, mp);

	uint32_t data_size = (size > mp->used)? mp->used : size;
	memcpy(data, mp->shmaddr, data_size);

	shmctl(SHM_UNLOCK, user, mp);
	return data_size;
}

uint32_t shm_write(char* data, uint32_t size , uint32_t user, mpoint_t *mp) {

	if ( mp->user != user || mp->locked != LOCKED_WRITE )
		return -1;

	shmctl(SHM_WLOCK, user, mp);

	uint32_t data_size = (size > mp->size)? mp->size : size; 
	memcpy(mp->shmaddr, data, data_size);
	mp->used = data_size;

	shmctl(SHM_UNLOCK, user, mp);
	return data_size;
}

void shmat(mpoint_t *mp) {
	mp->atcount++;	
}

uint32_t shmdt(mpoint_t *mp) {
	if ( mp->atcount > 0 )
		mp->atcount--;

	if ( mp->atcount == 0 && mp->r_flag ) {
		freemem(mp);
		return NULL;
	}

	return mp->atcount;
}

void freemem(mpoint_t *mp) {
	if ( mp->atcount == 0 ) {
		free(mp->shmaddr);
		free(mp);
	}

}


