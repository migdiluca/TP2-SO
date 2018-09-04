#include <stdint.h>

#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

struct RGB {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

/* Escribe un caracter con el color pasado como parametro rellenando el fondo con el color "background" de el tamaño recibido,
	desde el pixel x,y */
void writeChar(char c, uint64_t x, uint64_t y, struct RGB color, struct RGB background, uint64_t size);
// Imprime una lista de chars hasta llegar al 0
void writeString(char* string, uint64_t x, uint64_t y, struct RGB color, struct RGB background, uint64_t size);
// Imprime toda la pantalla de color negro 
void clearScreen();

#endif
