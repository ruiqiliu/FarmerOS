#include "thread.h"
#include "common.h"
#include "device.h"
#define THREAD_MAX_NUM 128

//global structure
Thread pcbs[THREAD_MAX_NUM];
list_head *runq, *sleepq;
Thread *currentThread;

//uint32_t
/*
 * pid management functions
 */
uint32_t getPID(){
//TODO
	static uint32_t cur = 0;
	return cur++;
}

// 创建一个内核线程
Thread *create_kthread(void (*entry)(void)){
	uint32_t pid = getPID();
	Thread * tmpThread = &pcbs[pid];

	TrapFrame *tf = ((TrapFrame*)(tmpThread->kstack+STK_SZ)) -1;
	tmpThread->tf = tf;
	tf->eax = tf->ebx = tf->ecx = tf->edx =0;
//	tf->cs = GDT_ENTRY(1);

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
	printf("thread change\n");
}
