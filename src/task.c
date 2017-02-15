/*
 * task.c
 *
 *  Created on: 2017年1月25日
 *      Author: xzy47
 */


#include <stdlib.h>
#include "lightos.h"
#include "task.h"

#define HW32_REG(ADDRESS)  (*((volatile unsigned long  *)(ADDRESS)))

l_uint32_t l_curTaskID = 0;
l_uint32_t l_nextTaskID = 0;
l_uint8_t l_taskNumber = 0;

l_sp_t l_PSPArray[LCONFIG_TASK_MAX_NUMBER] = {0};
l_tcb_t *l_TCBArray[LCONFIG_TASK_MAX_NUMBER] = {0};


l_err_t LTaskIncrementTick(void)
{
	static l_uint32_t osTick;

	if(l_TCBArray[l_curTaskID]->ulTimeSlice == 0)
	{
		return L_EOK;
	}

	if(++osTick == l_TCBArray[l_curTaskID]->ulTimeSlice)
	{
		 osTick = 0;
		 if(++l_nextTaskID >= LCONFIG_TASK_MAX_NUMBER)
		     l_nextTaskID = 0;
		 while(l_PSPArray[l_nextTaskID] == 0)
		     if(++l_nextTaskID >= LCONFIG_TASK_MAX_NUMBER)
		         l_nextTaskID = 0;
	}
	return L_EOK;
}

l_err_t LTaskCreate(l_uint8_t           ucTID,
                    LTaskFunction_t     pxEntry,
                    const char * const  pcName,
                    const l_uint16_t    usStackDepth,
                    const l_uint32_t    ulTimeSlice,
                    l_handle_t * const  pxHandle)
{
    l_stack_t *pxTopOfStack;
	  l_tcb_t *pxNewTCB;
    if(ucTID >= LCONFIG_TASK_MAX_NUMBER)
        return L_ETASK_NUM_OVERFLOW;
    l_taskNumber++;
    pxNewTCB = malloc(sizeof (l_tcb_t));
    l_TCBArray[ucTID] = pxNewTCB;
    pxNewTCB->pxStack = malloc(usStackDepth * sizeof(l_stack_t));

#if LPORT_STACK_GROWTH_DIR < 0
    pxTopOfStack = pxNewTCB->pxStack + ( usStackDepth - ( uint16_t ) 1 );
    /*lint !e923 MISRA exception.  Avoiding casts between pointers and integers is not practical.  Size differences accounted for using portPOINTER_SIZE_TYPE type. */
    pxTopOfStack = ( l_stack_t * ) ( ( ( l_uint32_t ) pxTopOfStack ) & ( ~( ( l_uint32_t ) LPORT_BYTE_ALIGNMENT_MASK ) ) );
#else /* portSTACK_GROWTH */
    pxTopOfStack = pxNewTCB->pxStack;
#endif /* portSTACK_GROWTH */

    l_PSPArray[ucTID] = (l_sp_t)LPortInitStack(pxTopOfStack, pxEntry);
    //l_PSPArray[ucTID] = ((l_uint32_t) pxNewTCB->pxStack) + usStackDepth * sizeof(l_stack_t) - 16*4;
	//PSP_array中存储的为task0_stack数组的尾地址-16*4，即task0_stack[1023-16]地址
	//HW32_REG((l_PSPArray[ucTID] + (14<<2))) = (l_stack_t) pxEntry; /* PC */
	//task0的PC存储在task0_stack[1023-16]地址  +14<<2中，即task0_stack[1022]中
	//HW32_REG((l_PSPArray[ucTID] + (15<<2))) = 0x01000000;            /* xPSR */
    pxNewTCB->usStackDepth = usStackDepth;
	  pxNewTCB->ucTID = ucTID;
	  pxNewTCB->ulTimeSlice = ulTimeSlice;

	*pxHandle = (l_int32_t)pxNewTCB;
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
    l_PSPArray[pxTCB->ucTID] = 0;
    free(pxTCB->pxStack);
    free(pxTCB);
    return L_EOK;
}
