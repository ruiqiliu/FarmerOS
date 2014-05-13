#include "common.h"
#include "x86.h"
#include "device.h"
#include "thread.h"

void init_thread();

void a(void){
	while(1)
		printf("a");
}
void b(void){
	while(1)
		printf("b");
}
void
entry(void) {
	init_timer();
	init_idt();

	/* Initialize the intel 8259 PIC */
	init_intr();
	init_serial();
	init_thread();

	create_kthread(&a);
	create_kthread(&b);
	enable_interrupt();


	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}


