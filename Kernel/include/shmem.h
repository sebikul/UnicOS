#include "types.h"

typedef struct mpoint_t{
	uint32_t memid;
	uint32_t semid;
	uint32_t user;
	uint64_t size;
	uint32_t used;
	void *shmaddr;
	uint32_t locked;
	bool r_flag;
	uint32_t atcount;
}mpoint_t;


mpoint_t *shmget(uint64_t size, uint32_t user);
bool shmctl(uint32_t cmd, uint32_t user, mpoint_t *mp);
void shmat(mpoint_t *mp);
int shmdt(mpoint_t *mp);
uint32_t shm_read(char* data, uint32_t size , uint32_t user, mpoint_t *mp);
uint32_t shm_write(char* data, uint32_t size , uint32_t user, mpoint_t *mp);
void freemem(mpoint_t *mp);
