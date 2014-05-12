#include "x86.h"
#include "device.h"

void
irq_handle(struct TrapFrame *tf) {
	if(tf->irq < 1000){
		if(tf->irq == -1){
			printf("%s, %d: Unhandled exception!\n",__FUNCTION__,__LINE__);
		}
		else{
			printf("%s, %d: Unexpected exception %d!\n",__FUNCTION__,__LINE__, tf->irq);
		}
		assert(0);
	}


	if (tf->irq == 1000) {
		printf(".");
	} else if (tf->irq == 1001) {
		uint32_t code = in_byte(0x60);
		uint32_t val = in_byte(0x61);
		out_byte(0x61, val | 0x80);
		out_byte(0x61, val);
		printf("%s, %d:key code = %x\n",__FUNCTION__,__LINE__, code);

		//do_keyboard(code);
	} else {
		assert(0);
	}
}

