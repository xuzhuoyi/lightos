/*
 * task.c
 *
 *  Created on: 2017年1月25日
 *      Author: xzy47
 */


#include "lightos.h"

l_uint8_t Cur_TaskID = 0;
l_uint8_t l_nextTaskID = 0;

l_uint32_t Task_StackSP[3];

extern void Task_0();	 //任务0
extern void Task_1();	 //任务1
extern void Task_2();	 //任务2

l_uint32_t LTaskIncrementTick( void )
{
	static l_uint8_t osTick;

	if(LCONFIG_OS_TIMESLICE == 0)
	{
		return 0;
	}

	if(++osTick == LCONFIG_OS_TIMESLICE)
	{
		 osTick = 0;
		 if(++l_nextTaskID >= 3)
			 l_nextTaskID = 0;
	}
}

l_uint32_t LTaskCreate( void )
{
	Task_StackSP[0] = Task_0;
	Task_StackSP[1] = Task_1;
	Task_StackSP[2] = Task_2;

	Cur_TaskID = 0;

}
