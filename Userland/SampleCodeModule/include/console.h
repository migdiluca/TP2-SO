#ifndef CONSOLE_H
#define CONSOLE_H

void console();

//Lee de la entrada estandar y muestra la linea de comando a ejecutar
void stdin();

//Escribe una nueva linea
void newLine();

//Imprime la salida de error
void stderr();

//Imprime la salida estandar
void stdout();

//Lee de stdin hasta que se presione enter y luego ejecuta stdout seguido por stderr.
void consoleLoop();

//Inicializa los valores de la consola
void init();

//Comando que cambia la apariencia de la consola
void changeTheme();

//Comando que reinicia la consola
void resetConsole();

#endif
