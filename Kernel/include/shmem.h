#include "types.h"

uint32_t shm_count();
void shmadd(mpoint_t *mp);
mpoint_t* shmget(uint32_t shmid);
uint32_t shmcreate(uint64_t size, uint32_t user);
bool shmctl(uint32_t cmd, uint32_t user, mpoint_t *mp);
void shmat(mpoint_t *mp);
uint32_t shmdt(mpoint_t *mp);
uint32_t shm_read(char*data, uint32_t size , uint32_t user, mpoint_t *mp);
uint32_t shm_write(const char* data, uint32_t size , uint32_t user, mpoint_t *mp);
void freemem(mpoint_t *mp);
