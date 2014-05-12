#ifndef __X86_MEMORY_H__
#define __X86_MEMORY_H__

#define DPL_KERNEL              0
#define DPL_USER                3

#define NR_SEGMENTS             3
#define SEG_KERNEL_CODE         1 
#define SEG_KERNEL_DATA         2

struct GateDescriptor {
	uint32_t offset_15_0      : 16;
	uint32_t segment          : 16;
	uint32_t pad0             : 8;
	uint32_t type             : 4;
	uint32_t system           : 1;
	uint32_t privilege_level  : 2;
	uint32_t present          : 1;
	uint32_t offset_31_16     : 16;
};

//Interrupt and exception frame
// http://cslab.nju.edu.cn/opsystem/#OS2014_5
struct TrapFrame {
	uint32_t edi, esi, ebp, xxx, ebx, edx, ecx, eax;// no esp
	int32_t irq;
//	uint32_t gs, fs, es, ds;
//	uint32_t err, eip, cs, eflags;
};
typedef struct TrapFrame TrapFrame;

#endif
