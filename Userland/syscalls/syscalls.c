#include <syscalls.h>
#include <types.h>
#include <stdint.h>

extern uint64_t syscall(uint64_t callid, ...);

mpoint_t* sys_shm_find(uint32_t shmid){
	return (mpoint_t*)syscall((uint64_t)SYSCALL_SHM_FIND, shmid);
}

uint32_t sys_shm_get(uint64_t size, uint32_t user) {
	return (uint32_t)syscall((uint64_t)SYSCALL_SHM_GET, size, user);
}

bool sys_shm_ctl(uint32_t cmd, uint32_t user, mpoint_t *mp) {
	return (bool)syscall((uint64_t)SYSCALL_SHM_CTL, cmd, user, mp); 
}

void sys_shm_at(mpoint_t *mp) {
	syscall((uint64_t)SYSCALL_SHM_AT, mp);	
}

uint32_t sys_shm_dt(mpoint_t *mp) {
	return (uint32_t)syscall((uint64_t)SYSCALL_SHM_DT, mp);
}

uint32_t sys_shm_read(char* data, uint32_t size , uint32_t user, mpoint_t *mp) {
	return (uint32_t)syscall((uint64_t)SYSCALL_SHM_READ, data, size, user, mp);
}

uint32_t sys_shm_write(const char* data, uint32_t size , uint32_t user, mpoint_t *mp) {
	return (uint32_t)syscall((uint64_t)SYSCALL_SHM_WRITE, data, size, user, mp);
}

void sys_shm_free(mpoint_t *mp) {
	syscall((uint64_t)SYSCALL_SHM_FREE, mp);
}

uint32_t sys_shm_count() {
	return syscall((uint64_t)SYSCALL_SHM_COUNT);
}

semaphore_t* sys_sem_find(uint32_t semid) {
	return (semaphore_t*)syscall((uint64_t)SYSCALL_SEM_FIND, semid);	
}

void sys_sem_get(uint32_t value) {
	syscall((uint64_t)SYSCALL_SEM_GET, value);
}

bool sys_sem_wait(semaphore_t *sem, pid_t pid, uint64_t msec) {
	return (bool)syscall((uint64_t)SYSCALL_SEM_WAIT, sem, pid, msec);
}

void sys_sem_sig(semaphore_t *sem) {
	syscall((uint64_t)SYSCALL_SEM_SIG, sem);
}

uint32_t sys_sem_count() {
	return syscall((uint64_t)SYSCALL_SEM_COUNT);
}

void sys_rtc_get(time_t* t) {
	syscall((uint64_t)SYSCALL_RTC, (uint64_t)t);
}

void sys_rtc_set(time_t* t) {
	syscall((uint64_t)SYSCALL_RTC_SET, (uint64_t)t);
}

void* sys_malloc(uint64_t len) {
	return (void*)syscall((uint64_t)SYSCALL_MALLOC, len);
}

void* sys_calloc(uint64_t len) {
	return (void*)syscall((uint64_t)SYSCALL_CALLOC, len);
}

void sys_free(void* m) {
	syscall((uint64_t)SYSCALL_FREE, (uint64_t)m);
}

int32_t sys_keyboard_catch(uint64_t scancode, dka_handler handler, uint64_t flags, char* name) {
	return syscall((uint64_t)SYSCALL_KEYBOARD_CATCH, scancode, (uint64_t)handler, flags, name);
}

void sys_clear_indexed_line(uint64_t index) {
	syscall((uint64_t)SYSCALL_VIDEO_CLR_INDEXED_LINE, index);
}

void sys_reset_cursor() {
	syscall((uint64_t) SYSCALL_VIDEO_RESET_CURSOR);
}

void sys_keyboard_replace_buffer(char* s) {
	syscall((uint64_t)SYSCALL_KEYBOARD_REPLACE_BUFFER, (uint64_t)s);
}

color_t sys_get_color() {
	return (color_t)syscall((uint64_t)SYSCALL_GET_COLOR);
}

void sys_set_color(color_t t) {
	syscall((uint64_t)SYSCALL_SET_COLOR, (uint64_t)t);
}

void sys_kbd_set_distribution(keyboard_distrib d) {
	syscall((uint64_t)SYSCALL_SET_KBD_DISTRIBUTION, (uint64_t)d);
}

void sys_set_screensaver_timer(uint64_t t) {

	syscall((uint64_t)SYSCALL_SCREENSAVER_TIMER, t);
}

void sys_clear_screen() {
	syscall((uint64_t)SYSCALL_CLEAR_SCREEN);
}

void sys_screensaver_trigger() {
	syscall((uint64_t) SYSCALL_SCREENSAVER_TRIGGER);
}

void sys_exit() {
	syscall((uint64_t)SYSCALL_EXIT);
}

void sys_keyboard_clear_handler(uint32_t index) {
	syscall((uint64_t)SYSCALL_KEYBOARD_CLEAR_HANDLER, index);
}

void sys_kdebug(char* str) {
	syscall((uint64_t)SYSCALL_KDEBUG, str);
}

pid_t sys_task_create(task_entry_point func, task_mode_t mode, const char* name, int argc, char** argv) {
	return (pid_t)syscall((uint64_t)SYSCALL_TASK_CREATE, (uint64_t)func, (task_mode_t) mode, (uint64_t)name, (uint64_t)argc, (uint64_t)argv);
}

void sys_task_ready(pid_t pid) {
	syscall((uint64_t)SYSCALL_TASK_READY, pid);
}

uint64_t sys_task_join(pid_t pid, pid_t otherpid) {
	return syscall((uint64_t) SYSCALL_TASK_JOIN, pid, otherpid);
}

pid_t sys_task_get_pid() {
	return (pid_t)syscall((uint64_t)SYSCALL_TASK_GET_PID);
}

void sys_task_yield() {
	syscall((uint64_t)SYSCALL_TASK_YIELD);
}

task_t* sys_task_getall() {
	return (task_t*)syscall((uint64_t)SYSCALL_TASK_GETALL);
}

void sys_sleep(uint64_t ms) {
	syscall((uint64_t) SYSCALL_SLEEP, ms);
}

uint64_t sys_uptime() {
	return syscall((uint64_t)SYSCALL_UPTIME);
}

void sys_atomic() {
	syscall((uint64_t)SYSCALL_ATOMIC);
}

void sys_unatomic() {
	syscall((uint64_t)SYSCALL_UNATOMIC);
}

void sys_signal_kill(pid_t pid, signal_t sig) {
	syscall((uint64_t)SYSCALL_SIGNAL_KILL, pid, (uint64_t)sig);
}
void sys_signal_set(signal_t sig, sighandler_t handler) {
	syscall((uint64_t)SYSCALL_SIGNAL_SET, (uint64_t)sig, (uint64_t)handler);
}

int32_t sys_open(const char* path, uint64_t flags) {
	return (int32_t)syscall((uint64_t)SYSCALL_FS_OPEN, (uint64_t)path, flags);
}

int32_t sys_read(int32_t fd, char* buf, uint32_t size) {
	return (int32_t)syscall((uint64_t)SYSCALL_FS_READ, (int32_t) fd, (char*) buf, (uint32_t) size);
}

int32_t sys_write(int32_t fd, const char* data, uint32_t size) {
	return (int32_t)syscall((uint64_t)SYSCALL_FS_WRITE, (int32_t) fd, (uint64_t)data, (uint32_t) size);
}

void sys_close(int32_t fd) {
	syscall((uint64_t)SYSCALL_FS_CLOSE, (int32_t)fd);
}

uint32_t sys_size(int32_t fd) {
	return (uint32_t)syscall((uint64_t)SYSCALL_FS_SIZE, (int32_t)fd);
}

void sys_ls() {
	syscall((uint64_t)SYSCALL_FS_LS);
}

int32_t sys_mkdir(const char* path) {
	return (int32_t)syscall((uint64_t)SYSCALL_FS_MKDIR, (uint64_t)path);
}
