/*
 * port.c
 *
 *  Created on: 2017Äê1ÔÂ25ÈÕ
 *      Author: xzy47
 */


#include "lightos.h"

extern l_uint8_t Task_StackSP[3];
extern l_uint8_t l_nextTaskID;
extern l_uint8_t Cur_TaskID;

void LPortSysTickHandler( void )
{
	LTaskIncrementTick();
	LPORT_NVIC_INT_CTRL_REG = LPORT_NVIC_PENDSVSET_BIT;
}

void LPortPendSVHandler( void )
{
	__asm__("mov %0, r13"
			:"=r"(Task_StackSP[Cur_TaskID])
			:
			:);

	Cur_TaskID = l_nextTaskID;

	__asm__("mov r13, %0"
				:
				:"r"(Task_StackSP[Cur_TaskID])
				:);

}
