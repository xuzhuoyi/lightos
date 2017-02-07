/*
 * port.c
 *
 *  Created on: 2017年1月25日
 *      Author: xzy47
 */


#include "lightos.h"
#include "task.h"


void LPortSysTickHandler(void)
{
	LTaskIncrementTick();
	if(l_nextTaskID != l_curTaskID)
	    LPORT_NVIC_INT_CTRL_REG = LPORT_NVIC_PENDSVSET_BIT;
}

void LPortInitScheduler(void)
{
	/* 设置PSP指向任务0堆栈的栈顶 */
	__asm volatile ("MSR psp, %0\n" : : "r" ((PSP_array[l_curTaskID] + 16*4)) :"sp" );
	LTaskStartScheduler();
	__asm volatile ("MSR control, %0" : : "r" (0x3) : "memory");

	__asm volatile ("isb 0xF":::"memory");

		    //Task_0();
	__asm volatile ("b Task_0");

}
