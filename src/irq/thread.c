#include "thread.h"
#include "common.h"
#include "device.h"
#include "x86.h"
#define THREAD_MAX_NUM 128

//global structure
Thread pcbs[THREAD_MAX_NUM];
list_head runq, sleepq, freeq;
Thread *currentThread;
Thread *idle;

//file scope
static int lock_count = 0;

/*
 * pid management functions
 */
uint32_t getPID(){
//TODO
	static uint32_t cur = 0;
	return cur++;
}
/* 1. Initialize runq and sleepq
 * 2. Initialize the 0 thread
 *
*/
void init_thread(){
	//global thread queue initialized here
	int i;
	INIT_LIST_HEAD(&runq); INIT_LIST_HEAD(&sleepq); INIT_LIST_HEAD(&freeq);

	//pcbs[0] is used by idle
	for(i=1; i < THREAD_MAX_NUM; i++){
		pcbs[i].used = 0;
		pcbs[i].pid = -1;
		list_add_tail(&(pcbs[i].list),&freeq);
	}
assert(THREAD_MAX_NUM - 1==list_num(&freeq));

	/* idle thread:
	 * 1. getPID and get the pcb
	 * 2. initialize pcb
	 * 3. add to the runq
	 */

	uint32_t pid = getPID();
	assert(pid == 0);

	idle = &pcbs[pid];

	//initialize pcb
	idle->used = 1;
	idle->pid = 0;
	idle->state = RUNNING;

	currentThread = idle;
}

// 创建一个内核线程
Thread *create_kthread(void (*entry)(void)){
	/*
	 * 1. getPID and get the pcb
	 * 2. delete from freeq add to the runq
	 * 3. initialize pcb
	 */
	uint32_t pid = getPID();
	//TODO check pid

	Thread * tmpThread = &pcbs[pid];
	tmpThread->pid = pid;
	list_del(&tmpThread->list);
	list_add_tail(&tmpThread->list, &runq);

	TrapFrame *tf = ((TrapFrame*)(tmpThread->kstack+STK_SZ)) -1;
	tmpThread->tf = tf;
	tmpThread->state = READY;
	tmpThread->used = 1;

	//initialize thread stack
	tf->eax = tf->ebx = tf->ecx = tf->edx =0;
	tf->xxx = tf->ebp = tf->edi = tf->esi =0;
	tf->cs = (SEG_KERNEL_CODE)<<3;
	tf->eip = (uint32_t)entry;
	tf->eflags = 1 << 9; // set IF
	return tmpThread;
}


// 使当前进程/线程立即阻塞，并可以在未来被唤醒
void sleep(void){
	lock();
assert(currentThread != idle);
	list_del_init(&currentThread->list);// remove from the runq
	currentThread->state = BLOCKED;
	list_add_tail(&currentThread->list,&sleepq);
	unlock();
	wait_for_interrupt();
}


// 唤醒一个进程/线程
void wakeup(Thread *t){
	//delete it from sleepq and add it to the runq
	lock();
	if(t->state == BLOCKED){
			list_del(&t->list);
			currentThread->state = READY;
			list_add_tail(&t->list,&runq);
	}


	unlock();
}


// 短临界区保护，实现关中断保护的原子操作
void lock(void){
	disable_interrupt();
	lock_count++;
}
void unlock(void){
	lock_count--;
	if(lock_count == 0)
		enable_interrupt();
}

void schedule(void){

	Thread * prev = currentThread;
	Thread * next = NULL;

	if(list_empty(&runq)){

		next = idle;

	} else {

		 if(prev == idle){
			 //running thread is idle, choose a thread from runq;
			 next = list_entry(runq.next, struct Thread, list);
			 prev->state = READY;
		 } else {
			 // choose another thread from runq
			 list_head * head = NULL;
			 if(prev->state != RUNNING){
				 //previous thread end or blocked, and it is not in the runq
				 next = list_entry(runq.next, struct Thread, list);

			 } else {
				 head = prev->list.next;

				 if(head == &runq) head = head->next;//discard runq head

				 next = list_entry(head, struct Thread, list);
				 prev->state = READY;
			 }
		 }
	}

	next->state = RUNNING;
	currentThread = next;


}
