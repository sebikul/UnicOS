#include "semaphores.h"
#include "task.h"
#include "kernel.h"
#include "mem.h"
#include "task.h"
#include "types.h"
#include "msgqueue.h"

uint32_t index=0;
semaphore_t* sem_array[256]; 

void semadd(semaphore_t *sem){
	if( index > 255 )
		return;

	sem_array[index] = malloc(sizeof(semaphore_t));
	memcpy(sem_array[index],sem,sizeof(semaphore_t));
	index++;
}

semaphore_t *semget(uint32_t semid){
	for ( int i = 0 ; i < index ; i++ ) {
		if( sem_array[i] != NULL && sem_array[i]->id == semid )
			return sem_array[i];
	}
	return NULL;
}

void create_sem(msgqueue_t *queue, uint32_t value, uint32_t id){

	semaphore_t *sem = malloc(sizeof(semaphore_t));
	sem->id = id;
	sem->value = value;
	sem->queue = queue;
	semadd(sem);
}

void delete_sem(semaphore_t *sem){

	signal_all(sem);
	msgqueue_delete(sem->queue);
	free(sem);
}

bool wait_sem(pid_t pid, semaphore_t *sem){

	task_t *tsk = task_find_by_pid(pid);
	if( tsk == NULL )
		return FALSE;

	if( sem->value == 0 ){
		task_pause(tsk);
		msgqueue_add(sem->queue,tsk, sizeof(task_t));
	} else {
		sem->value--;
	}
	return TRUE;

}
bool wait_cond(semaphore_t *sem){

	if( sem->value != 0 ) {
		sem->value--;
		return TRUE;
	}	
	return FALSE;
}
bool wait_time(pid_t pid, semaphore_t *sem, uint64_t msec){

	task_t *tsk = task_find_by_pid(pid);
	if( tsk == NULL )
		return FALSE;

	if( sem->value != 0 ) {
		sem->value--;
		return TRUE;
	}

	task_sleep(tsk, msec);
	return msec != 0; 
}

void signal_sem(semaphore_t *sem){

	if( msgqueue_isempty(sem->queue) ) {
		task_t *t = msgqueue_deq(sem->queue); 
		task_ready(t);
	} else {
		sem->value++;
	}
}

//ATOMIC
void signal_all(semaphore_t *sem){

	bool ints = kset_ints(FALSE);
	
	while ( !msgqueue_isempty(sem->queue) ) {
		task_t *t = msgqueue_deq(sem->queue); 
		task_ready(t);
	}

	sem->value=0;
	kset_ints(ints);
}


