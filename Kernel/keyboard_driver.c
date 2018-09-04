#include <stdint.h>
#include <keyboard.h>
#include <std_buffers.h>

#define BUFFER_SIZE 255
#define CAPS_LOCK 0x3A
#define R_SHIFT 0x36
#define R_SHIFT_REALEASE 0xB6
#define L_SHIFT 0x2A
#define L_SHIFT_REALEASE 0xAA

unsigned char keyboard[128] =
{
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8','9', '0', '-', '=', '\b',
	'\t','q', 'w', 'e', 'r','t', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	0,	// Control
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\'', '`',
	0,	// Left shift
	'\\', 'z', 'x', 'c', 'v', 'b', 'n','m', ',', '.', '/',
	0,	// Right shift
	'*',
	0,	// Alt
	' ',
	0,	// Caps lock
	0,	// F1 
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	// F10 
	0,	// Num lock
	0,	// Scroll Lock
	0,	// Home
	0,	// Up Arrow
	0,	// Page Up
	'-',
	0,	// Left Arrow 
	0,
	0,	// Right Arrow 
	'+',
	0,	// End
	0,	// Down Arrow 
	0,	// Page Down 
	0,	// Insert 
	0,	// Delete
	0,   0,   0,
	0,	// F11
	0,	// F12
	0,	// Undefined 
};
unsigned char shift_keyboard[128] =
{
	0,  27, '!', '@', '#', '$', '%', '^', '&', '*','(', ')', '_', '+', 
	'\b','\t','Q', 'W', 'E', 'R','T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 
	0,	// Control
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':','"', '|',
	0,	// Left
	'>', 'Z', 'X', 'C', 'V', 'B', 'N','M', '<', '>', '?',
	0,	// Right shift
	'*',
	0,	// Alt
	' ',
	0,	// Caps lock
	0,	// F1 
	0,   0,   0,   0,   0,   0,   0,   0,
	0,  //F10
	0,  //Num lock
	0,  // Scroll Lock
	0,  // Home
	0,  // Up Arrow
	0,  // Page Up 
	'-',
	0,  //Left Arrow
	0,
	0,  //Right Arrow
	'+',
	0,  // End
	0,  // Down Arrow 
	0,  // Page Down
	0,  // Inser
	0,  // Delete
	0,   0,   0,
	0,  // F11 Key
	0,  // F12 Key 
	0,  // Undefined
};	

int mayus = 0;

void keyboard_handler() {
	unsigned char key = getKey();
	if(key == R_SHIFT || key == L_SHIFT)
		mayus = 1;
	else if(key == R_SHIFT_REALEASE || key == L_SHIFT_REALEASE)
		mayus = 0;
	else if(key == CAPS_LOCK)
		mayus = !mayus;

	//Si la interrupción es por una tecla que se soltó o no es imprimible, retorna.
	if ((key & 0x80) != 0 || keyboard[key] == 0)
		return;
	if(mayus)
		writeCharBuffer(STD_IN,shift_keyboard[key]);
	else
		writeCharBuffer(STD_IN,keyboard[key]);
}
