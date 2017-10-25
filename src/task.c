/*
 * task.c
 *
 *  Created on: 2017.1.25
 *      Author: xzy47
 */


#include <stdlib.h>
#include "lightos.h"
#include "task.h"

#define HW32_REG(ADDRESS)  (*((volatile unsigned long  *)(ADDRESS)))

l_base_t l_curTaskID = 0;
l_base_t l_nextTaskID = 0;
l_uint8_t l_taskNumber = 0;
l_tick_t l_nextWeakTick = -1;

l_sp_t l_PSPArray[LCONFIG_TASK_MAX_NUMBER] = {0};
l_tcb_t *l_curTCB[LCONFIG_TASK_MAX_PRIORITY] = {0};
l_list_t l_TCBArray[LCONFIG_TASK_MAX_PRIORITY] = {0};
l_list_t l_delayTaskList;


l_err_t LTaskCreate(l_uint8_t           ucTID,
                    LTaskFunction_t     pxEntry,
                    const char * const  pcName,
                    const l_uint16_t    usStackDepth,
                    const l_uint32_t    ulTimeSlice,
                    l_uint8_t           ucPriority,
                    l_tcstatus_t        exTCStatus,
                    l_handle_t * const  pxHandle)
{
    l_item_t *xItem;
    l_stack_t *pxTopOfStack;
    l_tcb_t *pxNewTCB;

    if(ucTID >= LCONFIG_TASK_MAX_NUMBER)
        return L_ETASK_NUM_OVERFLOW;
    l_taskNumber++;
    pxNewTCB = malloc(sizeof (l_tcb_t));

    xItem = malloc(sizeof(l_item_t));
    xItem->pvItem = pxNewTCB;

    LListInsertEnd(&l_TCBArray[ucPriority], xItem);

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

	//HW32_REG((l_PSPArray[ucTID] + (14<<2))) = (l_stack_t) pxEntry; /* PC */

	//HW32_REG((l_PSPArray[ucTID] + (15<<2))) = 0x01000000;            /* xPSR */
    pxNewTCB->usStackDepth = usStackDepth;
    pxNewTCB->ucTID = ucTID;

    pxNewTCB->ulTimeSlice = ulTimeSlice;
    pxNewTCB->ulSliceTick = 0;

    pxNewTCB->xTaskStatus = exTCStatus;

    pxNewTCB->ucPriority = ucPriority;
    if(exTCStatus == L_TCSREADY)
        l_taskPriorityTable |= 1 << ucPriority;


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

void LTaskDelayTick(l_tick_t xDelayTick)
{
    l_item_t * pxDelayItem = malloc(sizeof(l_item_t));
    LPORT_SYSTICK_DISABLE;
    pxDelayItem->pvItem = l_TCBArray[curPriority].pxItem->pvItem;

    l_tick_t weakTick = LTickGet() + xDelayTick;
    ((l_tcb_t *)l_TCBArray[curPriority].pxItem->pvItem)->xTaskStatus = L_SPENDING;

    ((l_tcb_t *)l_TCBArray[curPriority].pxItem->pvItem)->xReadyTick = weakTick;
    if(l_nextWeakTick > weakTick)
        l_nextWeakTick = weakTick;

    LListInsertEnd(&l_delayTaskList, pxDelayItem);

    pxDelayItem = l_TCBArray[curPriority].pxItem;
    LListDeleteCur(&l_TCBArray[curPriority]);
    free(pxDelayItem);

    if(l_TCBArray[curPriority].ucNumberOfItems == 0)
        l_taskPriorityTable &= ~(1 << curPriority);
    LPORT_SYSTICK_ENABLE;

    LSchedulerRun(L_SCHEDULER_NEXT);
}
