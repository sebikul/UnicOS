#include <libc.h>
#include "commands.h"
#include "string.h"

COMMAND_HELP(time, "[time] Muestra la hora en pantalla.\n"
             "\t[time set] Permite setear fecha y hora del sistema.\n"
             "\t Recuerde que el formato del año debe ser de 1 o 2 digitos solamente y el siglo sera siempre el 21.\n"
             " Ej: time set 21 10 5 20 3 15 setea la hora a las 21:10:05 del 20/03/2015.");

static int checkDate(int hour, int minute, int second, int day, int month, int year);

COMMAND_START(time) {

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
					fprintf(FD_STDERR, "Horario invalido. Verifique la correctitud de la fecha.\n");
				}
			} else {
				fprintf(FD_STDERR, "Horario invalido. La hora se ingresa en formato numerico unicamente.\n");
			}
		} else {
			fprintf(FD_STDERR, "Comando invalido. Comandos disponibles [time] [time set]\n");
		}
		for (int i = 2; i < 8 && flagn; i++) {
			if (!string_numeric(argv[i])) {
				flagn = FALSE;
			}
		}

		break;

	default:
		fprintf(FD_STDERR, "Cantidad invalida de parametros.\n");
	}

	return 0;
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
