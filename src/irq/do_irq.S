# 中断和异常处理函数的入口
# 中断处理函数会在IDT中为相应的中断/异常设置处理程序
# 中断/异常的行为参见i386手册
#define __ASSEMBLY__
#include "x86/memory.h"

#   function name   error code  vector number
.globl vec0; 	vec0: pushl $0; 	pushl $0; 		jmp asm_do_irq
.globl vec1; 	vec1: pushl $0; 	pushl $1; 		jmp asm_do_irq
.globl vec2; 	vec2: pushl $0; 	pushl $2;		jmp asm_do_irq
.globl vec3; 	vec3: pushl $0; 	pushl $3; 		jmp asm_do_irq
.globl vec4; 	vec4: pushl $0; 	pushl $4; 		jmp asm_do_irq
.globl vec5; 	vec5: pushl $0; 	pushl $5; 		jmp asm_do_irq
.globl vec6; 	vec6: pushl $0;		pushl $6; 		jmp asm_do_irq
.globl vec7; 	vec7: pushl $0;		pushl $7; 		jmp asm_do_irq
.globl vec8; 	vec8: 				pushl $8; 		jmp asm_do_irq
.globl vec9; 	vec9: pushl $0; 	pushl $9; 		jmp asm_do_irq
.globl vec10; 	vec10: 				pushl $10; 		jmp asm_do_irq
.globl vec11; 	vec11: 				pushl $11; 		jmp asm_do_irq
.globl vec12; 	vec12: 				pushl $12; 		jmp asm_do_irq
.globl vec13; 	vec13: 				pushl $13; 		jmp asm_do_irq

.globl syscall; syscall:pushl $0;	pushl $0x80; 	jmp asm_do_irq  #add syscall to idt

.globl irq0; irq0: 		pushl $0;	pushl $1000; 	jmp asm_do_irq
.globl irq1; irq1:		pushl $0;	pushl $1001; 	jmp asm_do_irq

.globl irq_empty; irq_empty:pushl $0;pushl $-1;		jmp asm_do_irq

# ！调用C编写的中断处理程序(见irq/irq_handle.c)！
.globl asm_do_irq
.extern irq_handle
.extern currentThread
asm_do_irq:
	pushl %ds; pushl %es; pushl %fs; pushl %gs;
	pushal

	pushl %esp  #pass esp pointer to the tf
	call irq_handle
	addl $4, %esp  #remove esp

	#stack changed
	movl (currentThread), %eax
	movl (%eax), %esp # esp = current->tf

	popal
	popl %gs; popl %fs; popl %es; popl %ds;
	addl $8, %esp  #corresponding to pushl ebp? no!! this is for popping num and error code
	iret
