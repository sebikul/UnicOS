//TOASK: con que profundidad hay que validar las entradas?

#include "syscalls.h"
#include "include/libc.h"

void printf(char* arg){
	//TODO: VALIDAR ENTRADA
	//TOASK: printf deberia manejar %d,%f,etc... ?

	sys_write(FD_WRITE,&c,strlen(arg));

}

void putChar(char c){
	//TODO: VALIDAR ENTRADA

	sys_write(FD_WRITE,&c,1);
}

int strlen(char* str){
	int size;
	for (size = 0; *str !='\0' ;size++, str++)

	return size;
}

int getChar(char* c){
	//TOASK: validar si hay caracteres? o lo valida kernel?
	char test[1]=c;
	sys_read(FD_READ,&c,1);
	if (test==c)
		return -1; //no pudo leer el caracter -diferenciar motivos?-

	return 1;
}

int scanf(char* c){
	//TOASK: validar?
	char str[80]={0};
	int err=getc(&str);
	int size=1;
	
	while(str != EOF || err==-1 || size<80){
		err=getc(&str+size);
		size++;
	}

	if(size==1 && err==-1)
		return -1;

	c=&str;
	return size;
}

void printTime(){

	int timer[3]={0};
	char timerc[6]={0};

	sys_rtc_time(&timer[0], &timer[1], &timer[2]);	

	intToChar(timer[0],timerc);//*timerc & *timerc+1 -> hours
	intToChar(timer[1],timerc+2);//*timerc+2 & *timerc+3 -> minutes
	intToChar(timer[2],timerc+4);//*timerc+4 & *timerc+5 -> seconds

	for (int i = 0; i < 6; i++)
	{
		putChar(timerc[i]);
		if(i%2!=0 && i!=5)
			putChar(":");
	}

}

void intToChar(int number,char* c){

	int i=0;

	if(number<0){
		number=-number;
		c[i++]='-';
	}
		

	while (number<10 && i<10){
		int dig=number%10;
		number/=10;
		c[i++]=dig+'0';
	}

}
