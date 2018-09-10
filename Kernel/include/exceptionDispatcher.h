#ifndef EXCEPTION_DISPATCHER_H
#define EXCEPTION_DISPATCHER_H

//Reacciona a la excepcion y deriva a la funcion correspondiente
void exceptionDispatcher(int exceptionNum, uint64_t * instructionPointer, uint64_t * stackPointer);

//Excepcion por division por cero
void zero_division(uint64_t * instructionPointer, uint64_t * stackPointer);

//Excepcion que ocurre cuando el procesador intenta ejecutar una instruccion con opcode invalida.
void invalid_opcode(uint64_t * instructionPointer, uint64_t * stackPointer);

//Escribe en STDERR el estado de los registros en el momento de una excepcion.
void writeRegisters(uint64_t * instructionPointer, uint64_t * stackPointer);


#endif
