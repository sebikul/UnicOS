#include "task.h"
#include "msgqueue.h"

typedef struct smaphore_t{

	uint32_t id;
	uint32_t value; 	
	msgqueue_t *queue; 		
						
}semaphore_t;
	
semaphore_t * create_sem(msgqueue_t *queue, uint32_t value, uint32_t id);
void delete_sem(semaphore_t *sem);

bool wait_sem(task_t * tsk, semaphore_t *sem);
bool wait_cond(semaphore_t *sem);
bool wait_time(task_t * tsk, semaphore_t *sem, uint64_t msec);

void signal_sem(semaphore_t *sem);
void signal_all(semaphore_t *sem);