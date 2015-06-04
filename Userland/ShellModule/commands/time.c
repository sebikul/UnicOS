#include <libc.h>

void command_time(int argc, char** argv) {

	time_t* t = time();
	switch(argc){
		case 1:

			printf("El tiempo actual es: %02i:%02i:%02i del %02i/%02i/%02i", t->hour, t->minute, t->second, t->day, t->month, t->year);
			break;

		case 2:

			if(strcmp(argv[1],"set")==0){
				printf("Indique la hora que desea setear.\n");
			}else{
				printf("Comando invalido. Comandos disponibles [time] [time set]\n");
			}
			break;

		default:
			//TOASK: hay que ver si permitimos cambiar dia y hora por separado y eso
			//y preguntarles cuanto hace falta de funcionalidad
			//si permitir hora y fecha, todo junto, todo separado.. xq son mil formas
			//distintas de hacerlo

			//yo pensaba hacer sino time set para todos los parametros hora dia todo
			//time set hour setea hora
			//time set year setea año y asi... (lo cual es mas paja pero quizas mejor)
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
