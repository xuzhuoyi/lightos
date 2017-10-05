/*
 * scheduler.c
 *
 *  Created on: 2017.9.10
 *      Author: xuzhuoyi
 */

#include "task.h"

//l_list_t l_TCBArray[LCONFIG_TASK_MAX_PRIORITY] = {0};

const l_uint8_t l_priorityBitmap[] =
{
    /* 00 */ 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 10 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 20 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 30 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 40 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 50 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 60 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 70 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 80 */ 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 90 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* A0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* B0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* C0 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* D0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* E0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* F0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

l_uint8_t l_curTaskPriority = 0;

l_uint8_t l_taskPriorityTable = 0;
l_uint8_t curPriority = 0;

void LSchedulerRun(l_schmsg_t eSchMsg)
{
    //static l_uint32_t osTick;
    curPriority = l_priorityBitmap[l_taskPriorityTable];
    l_tcb_t *curTCB = l_TCBArray[curPriority].pxItem->pvItem;
    if(eSchMsg == L_SCHEDULER_NORMAL)
    {

        if(curTCB->ulTimeSlice == 0)
        {
            return;
        }

        if(++curTCB->ulSliceTick == curTCB->ulTimeSlice)
        {
            curTCB->ulSliceTick = 0;
            l_TCBArray[curPriority].pxItem = l_TCBArray[curPriority].pxItem->pxNext;
            l_nextTaskID = ((l_tcb_t *)l_TCBArray[curPriority].pxItem->pvItem)->ucTID;
        }
    }
    else
        l_nextTaskID = curTCB->ucTID;

    if(l_nextTaskID != l_curTaskID)
        LPORT_NVIC_INT_CTRL_REG = LPORT_NVIC_PENDSVSET_BIT;
}

