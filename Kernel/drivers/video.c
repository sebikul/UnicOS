
#include "video.h"

static char* screen=SCREEN_START;

clear_screen(){

	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++){
	
		screen[i*2]=' ';
		screen[i*2+1]=0; //TOASK: default?

	}

}