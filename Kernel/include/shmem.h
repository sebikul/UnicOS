#include <stdint.h>

typedef struct mpoint_t{
	uint32_t memid;
	uint32_t semid;
	uint64_t size;
	void *shmaddr;
	uint32_t flags;
	uint32_t atcount;
}mpoint_t;


mpoint_t *shmget(uint32_t semid, uint64_t size, uint32_t flags);
int shmctl(uint32_t cmd, mpoint_t *mp);
void shmat(mpoint_t *mp);
int shmdt(mpoint_t *mp);
void freemem(mpoint_t *mp);
