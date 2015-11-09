#include <stdint.h>
#include "shmem.h"
#include "mem.h"


uint32_t idcont=0;

mpoint_t *shmget(uint32_t semid, uint64_t size, uint32_t flags) {
	mpoint_t *mem = malloc(sizeof(mpoint_t));
	mem->memid = idcont++;
	mem->semid = semid;
	mem->size = size;
	mem->flags = flags;
	mem->atcount = 0;
	mem->shmaddr = malloc(mem->size);

	return mem;
}

int shmctl(uint32_t cmd, mpoint_t *mp) {
	
}

void shmat(mpoint_t *mp) {
	mp->atcount++;	
}

int shmdt(mpoint_t *mp) {
	if ( mp->atcount > 0 )
		mp->atcount--;

	return mp->atcount;
}

void freemem(mpoint_t *mp) {
	if ( mp->atcount == 0 ) {
		free(mp->shmaddr);
		free(mp);
	}
}

