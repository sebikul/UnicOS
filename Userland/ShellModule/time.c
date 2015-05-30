#include <libc.h>

void command_time() {
	printTime();
}
void command_settime(int hours, int minutes, int seconds) {

	printf("\n");
	if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59) {
		printf("Horario invalido.");
	}
	printf("Seteando la hora");
}