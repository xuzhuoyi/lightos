/*
 * task.c
 *
 *  Created on: 2017��1��25��
 *      Author: xzy47
 */


#include <stdlib.h>
#include "lightos.h"
#include "task.h"

#define HW32_REG(ADDRESS)  (*((volatile unsigned long  *)(ADDRESS)))

l_base_t l_curTaskID = 0;
l_base_t l_nextTaskID = 0;
l_uint8_t l_taskNumber = 0;

l_sp_t l_PSPArray[LCONFIG_TASK_MAX_NUMBER] = {0};
l_tcb_t *l_TCBArray[LCONFIG_TASK_MAX_PRIORITY] = {0};
l_tcb_t *l_curTCB;

extern l_uint8_t l_taskPriorityTable;

extern const l_uint8_t l_priorityBitmap[];


l_err_t LTaskIncrementTick(void)
{
	static l_uint32_t osTick;

	l_curTCB = l_TCBArray[l_priorityBitmap[l_taskPriorityTable]];

	if(l_curTCB->ulTimeSlice == 0)
	{
		return L_EOK;
	}

	if(++osTick == l_curTCB->ulTimeSlice)
	{
		 osTick = 0;
		 l_curTCB = l_curTCB->pxNextTCB;
		 l_nextTaskID = l_curTCB->ucTID;
	}
	return L_EOK;
}

l_err_t LTaskCreate(l_uint8_t           ucTID,
                    LTaskFunction_t     pxEntry,
                    const char * const  pcName,
                    const l_uint16_t    usStackDepth,
                    const l_uint32_t    ulTimeSlice,
                    l_uint8_t           ucPriority,
                    l_handle_t * const  pxHandle)
{
    l_stack_t *pxTopOfStack;
    l_tcb_t *pxNewTCB;
    if(ucTID >= LCONFIG_TASK_MAX_NUMBER)
        return L_ETASK_NUM_OVERFLOW;
    l_taskNumber++;
    pxNewTCB = malloc(sizeof (l_tcb_t));
    if(!l_TCBArray[ucPriority])
    {
        l_TCBArray[ucPriority] = pxNewTCB;
        pxNewTCB->pxNextTCB = pxNewTCB;
    }
    else
    {
        pxNewTCB->pxNextTCB = l_TCBArray[ucPriority]->pxNextTCB;
        l_TCBArray[ucPriority]->pxNextTCB = pxNewTCB;
    }
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
	//PSP_array�д洢��Ϊtask0_stack�����β��ַ-16*4����task0_stack[1023-16]��ַ
	//HW32_REG((l_PSPArray[ucTID] + (14<<2))) = (l_stack_t) pxEntry; /* PC */
	//task0��PC�洢��task0_stack[1023-16]��ַ  +14<<2�У���task0_stack[1022]��
	//HW32_REG((l_PSPArray[ucTID] + (15<<2))) = 0x01000000;            /* xPSR */
    pxNewTCB->usStackDepth = usStackDepth;
    pxNewTCB->ucTID = ucTID;
    pxNewTCB->ulTimeSlice = ulTimeSlice;
    pxNewTCB->xTaskStatus = L_SREADY;


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
