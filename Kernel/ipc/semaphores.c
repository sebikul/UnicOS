#include "semaphores.h"
#include "task.h"
#include "kernel.h"
#include "mem.h"
#include "task.h"
#include "types.h"
#include "msgqueue.h"

uint32_t index=0;
semaphore_t* sem_array[256]; 

uint32_t sem_count() {
	return index;
}

void semadd(semaphore_t *sem){
	if( index > 255 )
		return;

	sem_array[index] = malloc(sizeof(semaphore_t));
	memcpy(sem_array[index],sem,sizeof(semaphore_t));
	index++;
}

semaphore_t *semget(uint32_t semid){
<<<<<<< HEAD
	if( semid > index ) //TODO if id es int semid < 0 
=======
	if( semid > index || semid < 0 )
>>>>>>> 27a7e55e63483c6104200bfa81d61da53207dbc5
		return NULL;
	return sem_array[semid];
}

<<<<<<< HEAD
uint32_t create_sem(uint32_t value){
=======
void create_sem(uint32_t value){
>>>>>>> 27a7e55e63483c6104200bfa81d61da53207dbc5
	semaphore_t *sem = malloc(sizeof(semaphore_t));
	sem->id = index;
	sem->value = value;
	sem->queue = msgqueue_create(256);
	semadd(sem);
<<<<<<< HEAD
	return sem->id;
=======
>>>>>>> 27a7e55e63483c6104200bfa81d61da53207dbc5
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
<<<<<<< HEAD
}

=======

}
>>>>>>> 27a7e55e63483c6104200bfa81d61da53207dbc5
bool wait_cond(semaphore_t *sem){

	if( sem->value != 0 ) {
		sem->value--;
		return TRUE;
	}	
	return FALSE;
}
<<<<<<< HEAD

=======
>>>>>>> 27a7e55e63483c6104200bfa81d61da53207dbc5
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

<<<<<<< HEAD
	if( !msgqueue_isempty(sem->queue) ) {
		task_t *t = msgqueue_deq(sem->queue);
		t = task_find_by_pid(t->pid);
=======
	if( msgqueue_isempty(sem->queue) ) {
		task_t *t = msgqueue_deq(sem->queue); 
>>>>>>> 27a7e55e63483c6104200bfa81d61da53207dbc5
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


