#include <console.h>

static int var1 = 0;
static int var2 = 0;

int main() {

	console();

	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}
