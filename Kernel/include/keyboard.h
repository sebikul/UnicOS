
#ifndef HKEYBOARD_H
#define HKEYBOARD_H 

#define NULL 0

#define TRUE 1
#define FALSE 0

#define KEYBOARD_BUFFER_SIZE 128

typedef char bool;

typedef struct {
	char scancode;
	char ascii;
} scancode;

//vamos a sacrificar mucho espacio para que el codigo sea mas legible. Se podrian usar mascaras
typedef struct {
	bool caps;
} kstatus;


#endif








