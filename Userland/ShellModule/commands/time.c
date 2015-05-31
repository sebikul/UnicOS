#include <libc.h>

void command_time(int argc, char** argv) {

	time_t* t=time();

	printf("El tiempo actual es: %02i:%02i:%02i del %02i/%02i/%02i\n", t->hour, t->minute, t->second, t->day, t->month, t->year);

}
void command_settime(int hours, int minutes, int seconds) {

	printf("\n");
	if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59) {
		printf("Horario invalido.");
	}
	printf("Seteando la hora");
}