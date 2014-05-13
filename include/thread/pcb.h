#ifndef PCB_H
#define PCB_H

#include "common.h"
#include "x86.h"
#define STK_SZ 1024*4
struct Thread{
	TrapFrame *tf;
	uint32_t pid;
	list_head runq, freeq;

	char kstack[STK_SZ];
};
typedef struct Thread Thread;

// 创建一个内核线程
Thread *create_kthread(void (*entry)(void));
// 使当前进程/线程立即阻塞，并可以在未来被唤醒
void sleep(void);
// 唤醒一个进程/线程
void wakeup(Thread *t);
// 短临界区保护，实现关中断保护的原子操作
void lock(void);
void unlock(void);
void schedule(void);
#endif
