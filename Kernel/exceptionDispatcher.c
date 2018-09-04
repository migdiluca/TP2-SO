#include <stdint.h>
#include <std_buffers.h>
#include <lib.h>
#include <exceptionDispatcher.h>

#define REGISTER_NUM 15

typedef void (* exception_ptr)(uint64_t * instructionPointer, uint64_t * stackPointer);

void zero_division(uint64_t * instructionPointer, uint64_t * stackPointer);
void invalid_opcode(uint64_t * instructionPointer, uint64_t * stackPointer);

exception_ptr exceptions[] = {zero_division,0,0,0,0,0,invalid_opcode};

void exceptionDispatcher(int exceptionNum, uint64_t * instructionPointer, uint64_t * stackPointer) {
	exceptions[exceptionNum](instructionPointer,stackPointer);
}

void zero_division(uint64_t * instructionPointer, uint64_t * stackPointer){
	writeStrBuffer(STD_ERR,"ERROR: DIVISION BY ZERO\n");
	writeRegisters(instructionPointer, stackPointer);
}

void invalid_opcode(uint64_t * instructionPointer, uint64_t * stackPointer){
	writeStrBuffer(STD_ERR,"ERROR: INVALID OPCODE\n");
	writeRegisters(instructionPointer, stackPointer);
}

void writeRegisters(uint64_t * instructionPointer, uint64_t * stackPointer){
	writeStrBuffer(STD_ERR,"RIP: ");
	writeIntBuffer(STD_ERR,(uint64_t)instructionPointer,16);
	writeCharBuffer(STD_ERR,'\n');
	char * registers[] = {"R15","R14","R13","R12","R11","R10","R9","R8","RSI","RDI","RBP","RDX","RCX","RBX","RAX"};
	int i;
	for(i = 0; i < REGISTER_NUM; i++){
		writeStrBuffer(STD_ERR,registers[i]);
		writeStrBuffer(STD_ERR,": ");
		writeIntBuffer(STD_ERR,*(stackPointer + i),16);
		writeCharBuffer(STD_ERR,'\n');
	}
}
