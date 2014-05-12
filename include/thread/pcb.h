#ifndef PCB_H
#define PCB_H

#define STK_SZ 1024
struct Thread{
	TrapFrame *tf;

	list_head runq, freeq;

	char kstack[STK_SZ];
};

// 创建一个内核线程
Thread *create_kthread(void (*entry)(void));
// 使当前进程/线程立即阻塞，并可以在未来被唤醒
void sleep(void);
// 唤醒一个进程/线程
void wakeup(Thread *t);
// 短临界区保护，实现关中断保护的原子操作
void lock(void);
void unlock(void);
#endif
