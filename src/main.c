#include "common.h"
#include "x86.h"
#include "device.h"
#include "thread.h"

void init_thread();

//test threading
Thread * aptr = NULL;
Thread * bptr = NULL;
Thread * cptr = NULL;
Thread * dptr = NULL;
void a(void){
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
            printf("a");
            wakeup(bptr);
            sleep();
        }
        x ++;
    }
}
void b(void){
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
            printf("b");
            wakeup(cptr);
            sleep();
        }
        x ++;
    }
}
void c(void){
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
            printf("c");
            wakeup(dptr);
            sleep();
        }
        x ++;
    }
}
void d(void){
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
            printf("d");
            wakeup(aptr);
            sleep();
        }
        x ++;
    }
}
void grading(){
	aptr = create_kthread(&a);
	bptr = create_kthread(&b);
	cptr = create_kthread(&c);
	dptr = create_kthread(&d);

	printf("a b c d %d %d %d %d\n",aptr->pid,bptr->pid,cptr->pid,dptr->pid);
}
void
entry(void) {

	init_timer();
	init_idt();

	/* Initialize the intel 8259 PIC */
	init_intr();
	init_serial();
	init_thread();

	grading();

	enable_interrupt();
	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}


