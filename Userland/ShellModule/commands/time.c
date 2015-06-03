#include <libc.h>

void command_time(int argc, char** argv) {

	time_t* t = time();
	int hour;// = ctoi(argv[2]);
	int minute;// = ctoi(argv[3]);
	int second;// = ctoi(argv[4]);
	int day;// = ctoi(argv[5]);
	int month;// = ctoi(argv[6]);
	int year;// = ctoi(argv[7]);
	switch(argc){
		case 1:

			printf("El tiempo actual es: %02i:%02i:%02i del %02i/%02i/%02i", t->hour, t->minute, t->second, t->day, t->month, t->year);
			break;

		/*case 2:

			if(strcmp(argv[1],"set")==0){
				printf("Indique la hora que desea setear.\n");
			}else{
				printf("Comando invalido. Comandos disponibles [time] [time set]\n");
			}
			break;*/
			//time set hh mm ss dd mm aa
		case 8:
			hour = ctoi(argv[2]);
			minute = ctoi(argv[3]);
			second = ctoi(argv[4]);
			day = ctoi(argv[5]);
			month = ctoi(argv[6]);
			year = ctoi(argv[7]);
			if(checkDate(hour,minute,second,day,month,year)){
				t->hour = hour;
				t->minute = minute;
				t->second = second;
				t->day = day;
				t->month = month;
				t->year = year;

				setTime(t);
			}else{
				printf("Parametros invalidos");
			}
			break;

		default:
			
			if(strcmp(argv[1],"set")==0){
				if(argc==5){
					int h=ctoi(argv[2]);
					int m=ctoi(argv[3]);
					int s=ctoi(argv[4]);
					command_settime(h,m,s);
				}else{
					printf("Faltan parametros. Debe indicar horas minutos segundos dia mes año.\n");
				}
			}else{
				printf("Comando invalido. Comandos disponibles [time] [time set]\n");
			}

	}


}
void command_settime(int hours, int minutes, int seconds) {

	printf("\n");
	if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59) {
		printf("Horario invalido.\n");
	}else{
		time_t* t=time();
		t->hour = hours;
		t->minute = minutes;
		t->second = seconds;
		//TOASK: hay que actualizar el rtc del qemu?
		//sys_call_update_rtc(); o algo asi
		printf(" %i ");
		printf("La hora ha sido actualizada.\n");
	}

}

int checkDate(int hour, int minute,int second, int day,int month, int year){
	if(hour>23 || hour<0)
		return FALSE;
	if(minute>59 || minute<0)
		return FALSE;
	if(second>59 || second<0)
		return FALSE;
	// habra que verificar los dias dependiendo del mes


		return TRUE;
}
