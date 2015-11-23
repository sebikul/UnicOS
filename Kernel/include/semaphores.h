#include "types.h"
#include "task.h"
#include "msgqueue.h"

uint32_t sem_count();
void semadd(semaphore_t *sem);
semaphore_t *semget(uint32_t id);
void create_sem(uint32_t value);
void delete_sem(semaphore_t *sem);

bool wait_sem(pid_t pid, semaphore_t *sem);
bool wait_cond(semaphore_t *sem);
bool wait_time(pid_t pid, semaphore_t *sem, uint64_t msec);

void signal_sem(semaphore_t *sem);
void signal_all(semaphore_t *sem);