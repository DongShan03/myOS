
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"


/*======================================================================*
                            kernel_main

typedef struct s_proc {
	STACK_FRAME regs;          // 寄存器保存在该结构体中 
	u16 ldt_sel;               // 全局描述符表选择子给出ldt的基址和限长 
	DESCRIPTOR ldts[LDT_SIZE]; // local descriptors for code and data 
	u32 pid;                   // process id passed in from MM 
	char p_name[16];           // name of the process 
}PROCESS;

typedef struct s_stackframe {
	u32	gs;			\                                 
	u32	fs;			|                                 
	u32	es;			|                                 
	u32	ds;			|                                 
	u32	edi;		|                                 
	u32	esi;		| pushed by save()                
	u32	ebp;		|                                 
	u32	kernel_esp;	<- 'popad' will ignore it         
	u32	ebx;		|                                 
	u32	edx;		|                                 
	u32	ecx;		|                                 
	u32	eax;		/                                 
	u32	retaddr;	return addr for kernel.asm::save()
	u32	eip;		\                                 
	u32	cs;			|                                 
	u32	eflags;		| pushed by CPU during interrupt  
	u32	esp;		|                                 
	u32	ss;			/                                 
}STACK_FRAME;

 *======================================================================*/
PUBLIC int kernel_main()
{
	disp_str("-----\"kernel_main\" begins-----\n");

	PROCESS* p_proc	= proc_table;

	p_proc->ldt_sel	= SELECTOR_LDT_FIRST;	// 0x28
	memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS>>3], sizeof(DESCRIPTOR));
	p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;	// change the DPL
	memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS>>3], sizeof(DESCRIPTOR));
	p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;	// change the DPL

	p_proc->regs.cs	= (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
	p_proc->regs.ds	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
	p_proc->regs.es	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
	p_proc->regs.fs	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
	p_proc->regs.ss	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
	p_proc->regs.gs	= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | RPL_TASK;
	p_proc->regs.eip= (u32)TestA;
	p_proc->regs.esp= (u32) task_stack + STACK_SIZE_TOTAL;
	p_proc->regs.eflags = 0x1202;	// IF=1, IOPL=1, bit 2 is always 1.

	p_proc_ready	= proc_table;
	restart();

	while(1){}
}

/*======================================================================*
                               TestA
 *======================================================================*/
void TestA()
{
	int i = 0;
	while(1){
		disp_str("A");
		disp_int(i++);
		disp_str(".");
		delay(1);
	}
}
