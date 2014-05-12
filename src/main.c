#include "common.h"
#include "x86.h"
#include "device.h"

void
entry(void) {
	init_timer();
	init_idt();

	/* Initialize the intel 8259 PIC */
	init_intr();
	init_serial();


	enable_interrupt();
	asm volatile("int $0x80");
	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}


