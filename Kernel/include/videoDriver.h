#include <stdint.h>
#include <lib.h>

#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

// Devuelve el ancho de la pantalla.
uint64_t getWidth();
// Devuelve el alto de la pantalla.
uint64_t getHeight();
// Escribe el pixel en la posicion (width, height) de un cierto color.
void writePixel(uint64_t width, uint64_t height, struct RGB color);
// Devuelve una estrucutra con el color del pixel (width, height).
struct RGB readPixel(uint64_t width, uint64_t height);
// Mueve la pantalla una cantidad de pixeles hacia arriba e imprime la parte inferior con el color background.
void movePixelsUp(uint64_t ammount,struct RGB background);
// Remplaza todos los pixeles que tengan el colorOld por el colorNew.
void replaceColor(struct RGB colorOld, struct RGB colorNew);
// Realiza un backup de todos los pixeles de la pantalla.
void backupScreen();
// Imprime todos los pixeles con el backup.
void restoreScreen();
// Escribe un rectangulo en la posicion (width,height) del color recibido, con el tamaño pasado como parametro.
void writeBlock(uint64_t width, uint64_t height, struct RGB color, uint64_t sizeX,uint64_t sizeY);
// Escribe todos los pixeles de la pantalla del color decibido.
void fillScreen(struct RGB color);

#endif
