#include "types.h"
#include "shmem.h"
#include "mem.h"

#define SHM_RLOCK 			300
#define SHM_WLOCK			301
#define SHM_UNLOCK			302
#define SHM_RMID			303 
#define SHM_SETU			304

#define LOCKED_WRITE		400
#define LOCKED_READ			401
#define UNLOCKED			402

uint32_t idcont=0;


mpoint_t *shmget(uint64_t size, uint32_t user) {
	mpoint_t *mem = malloc(sizeof(mpoint_t));
	mem->memid = idcont++;
	mem->user = user;
	mem->size = size;
	mem->used = 0;
	mem->locked = UNLOCKED;
	mem->r_flag = FALSE;
	mem->atcount = 0;
	mem->shmaddr = malloc(mem->size);

	return mem;
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

	if ( mp->locked != LOCKED_WRITE ) 
		return -1;

	shmctl(SHM_RLOCK, user, mp);

	uint32_t data_size = (size > mp->used)? mp->used : size;
	memcpy(data, mp->shmaddr, data_size);

	shmctl(SHM_UNLOCK, user, mp);
	return data_size;
}

uint32_t shm_write(char* data, uint32_t size , uint32_t user, mpoint_t *mp) {

	if ( mp->user != user || mp->locked != UNLOCKED )
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

int shmdt(mpoint_t *mp) {
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

