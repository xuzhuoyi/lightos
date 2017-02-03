/*
 * task.c
 *
 *  Created on: 2017年1月25日
 *      Author: xzy47
 */


#include <stdlib.h>
#include "lightos.h"
#include "stm32f4xx_hal.h"
#include "task.h"

#define HW32_REG(ADDRESS)  (*((volatile unsigned long  *)(ADDRESS)))

l_uint8_t Cur_TaskID = 0;
l_uint8_t l_nextTaskID = 0;
l_uint8_t l_taskNumber = 0;

l_uint32_t PSP_array[LCONFIG_TASK_MAX_NUMBER] = {0};


l_err_t LTaskIncrementTick(void)
{
	static l_uint32_t osTick;

	if(LCONFIG_OS_TIMESLICE == 0)
	{
		return L_EOK;
	}

	if(++osTick == LCONFIG_OS_TIMESLICE)
	{
		 osTick = 0;
		 if(++l_nextTaskID >= LCONFIG_TASK_MAX_NUMBER)
		     l_nextTaskID = 0;
		 while(PSP_array[l_nextTaskID] == 0)
		     if(++l_nextTaskID >= LCONFIG_TASK_MAX_NUMBER)
		         l_nextTaskID = 0;
	}
	return L_EOK;
}

l_err_t LTaskCreate(l_uint8_t           ucTID,
                    LTaskFunction_t     pxEntry,
                    const char * const  pcName,
                    const l_uint16_t    usStackDepth,
                    l_uint32_t * const  pxHandle)
{
    if(ucTID >= LCONFIG_TASK_MAX_NUMBER)
        return L_ETASK_NUM_OVERFLOW;
    l_taskNumber++;
    l_tcb_t *pxNewTCB = malloc(sizeof (l_tcb_t));
    pxNewTCB->pxStack = malloc(usStackDepth * sizeof(l_stack_t));
	PSP_array[ucTID] = ((l_uint32_t) pxNewTCB->pxStack) + usStackDepth * sizeof(l_stack_t) - 16*4;
	//PSP_array中存储的为task0_stack数组的尾地址-16*4，即task0_stack[1023-16]地址
	HW32_REG((PSP_array[ucTID] + (14<<2))) = (l_stack_t) pxEntry; /* PC */
	//task0的PC存储在task0_stack[1023-16]地址  +14<<2中，即task0_stack[1022]中
	HW32_REG((PSP_array[ucTID] + (15<<2))) = 0x01000000;            /* xPSR */

	pxNewTCB->ucTID = ucTID;

	*pxHandle = (u_int32_t)pxNewTCB;
	return L_EOK;
}

void LTaskStartScheduler(void)
{
	LPORT_SYSTICK_ENABLE;
}

void LTaskStopScheduler(void)
{
	LPORT_SYSTICK_DISABLE;
}

l_err_t LTaskDelete(l_uint32_t ulHandle)
{
    l_tcb_t * pxTCB = (l_tcb_t *) ulHandle;
    PSP_array[pxTCB->ucTID] = 0;
    free(pxTCB->pxStack);
    free(pxTCB);
    return L_EOK;
}
