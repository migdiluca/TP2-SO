#include <stdint.h>

#ifndef RTC_H
#define RTC_H

//Devuelve los segundos del Real Time Clock en hexadecimal.
uint8_t getSeconds();
//Devuelve los minutos del Real Time Clock en hexadecimal.
uint8_t getMinutes();
//Devuelve las horas del Real Time Clock en hexadecimal.
uint8_t getHour();

#endif
