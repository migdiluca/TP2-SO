#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <idtLoader.h>
#include <std_buffers.h>
#include <videoDriver.h>
#include <syscallDispatcher.h>
#include <interrupts.h>
#include "beep.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

extern void * instructionPointerBackup;
extern void * stackPointerBackup;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);


	clearBSS(&bss, &endOfKernel - &bss);


	setUpBuffers();
	return getStackBase();
}

int main()
{
	load_idt();
	setUpBuffers();

	instructionPointerBackup = sampleCodeModuleAddress;
	stackPointerBackup = getStackPointer() + 2*8;
	((EntryPoint)sampleCodeModuleAddress)();



	return 0;
}
