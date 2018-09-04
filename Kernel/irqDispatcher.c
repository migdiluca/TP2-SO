#include <time.h>
#include <keyboard_driver.h>
#include <stdint.h>


typedef void (* handler_ptr)();

static void int_20();
static void int_21();


handler_ptr irqHandlers[] = {int_20,int_21};

void irqDispatcher(uint64_t irq) {
	irqHandlers[irq]();
}

void int_20() {
	timer_handler();
}
void int_21() {
	keyboard_handler();	
}
