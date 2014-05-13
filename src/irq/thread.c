#include "thread.h"
#include "common.h"
#include "device.h"
#define THREAD_MAX_NUM 128

//global structure
Thread pcbs[THREAD_MAX_NUM];
list_head *runq, *sleepq;
Thread *currentThread;
Thread *idle;

//uint32_t
/*
 * pid management functions
 */
uint32_t getPID(){
//TODO
	static uint32_t cur = 0;
	return cur++;
}
//Initialize the 0 thread
void init_thread(){


	uint32_t pid = getPID();
	assert(pid == 0);
	idle = &pcbs[pid];
	idle->pid = 0;
	currentThread = idle;
}

// 创建一个内核线程
Thread *create_kthread(void (*entry)(void)){
	uint32_t pid = getPID();
	//TODO check pid

	Thread * tmpThread = &pcbs[pid];
	tmpThread->pid = pid;

	TrapFrame *tf = ((TrapFrame*)(tmpThread->kstack+STK_SZ)) -1;
	tmpThread->tf = tf;

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

}
// 唤醒一个进程/线程
void wakeup(Thread *t){

}
// 短临界区保护，实现关中断保护的原子操作
void lock(void){

}
void unlock(void){

}

void schedule(void){
	if(currentThread->pid == 0){
		currentThread = &pcbs[1];
		return ;
	}
	currentThread = (currentThread->pid == 1)? (&pcbs[2]):(&pcbs[1])  ;
	printf("thread change");
}
