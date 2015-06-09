#include <libc.h>

static int checkDate(int hour, int minute, int second, int day, int month, int year);

void command_time(int argc, char** argv) {

	time_t* t = time();
	int hour;
	int minute;
	int second;
	int day;
	int month;
	int year;
	bool flagn = TRUE;
	switch (argc) {
	case 1:

		printf("El tiempo actual es: %02i:%02i:%02i del %02i/%02i/%02i", t->hour, t->minute, t->second, t->day, t->month, t->year);
		break;

	case 8:
		if (strcmp(argv[1], "set") == 0) {
			if (flagn) {
				hour = ctoi(argv[2]);
				minute = ctoi(argv[3]);
				second = ctoi(argv[4]);
				day = ctoi(argv[5]);
				month = ctoi(argv[6]);
				year = ctoi(argv[7]);
				if (checkDate(hour, minute, second, day, month, year)) {
					t->hour = hour;
					t->minute = minute;
					t->second = second;
					t->day = day;
					t->month = month;
					t->year = year;

					set_time(t);
				} else {
					printf("Horario invalido. Verifique la correctitud de la fecha.\n");
				}
			} else {
				printf("Horario invalido. La hora se ingresa en formato numerico unicamente.\n");
			}
		} else {
			printf("Comando invalido. Comandos disponibles [time] [time set]\n");
		}
		for (int i = 2; i < 8 && flagn; i++) {
			if (!string_numeric(argv[i])) {
				flagn = FALSE;
			}
		}

		break;

	default:
		printf("Cantidad invalida de parametros.\n");
	}

}

static int checkDate(int hour, int minute, int second, int day, int month, int year) {
	if (hour > 23 || hour < 0)
		return FALSE;
	if (minute > 59 || minute < 0)
		return FALSE;
	if (second > 59 || second < 0)
		return FALSE;
	if (day > 31 || day <= 0)
		return FALSE;
	if (month > 12 || month <= 0)
		return FALSE;
	if (year < 0 || year > 99)
		return FALSE;
	if (month == 2 && day > 28)
		return FALSE;

	return TRUE;
}
