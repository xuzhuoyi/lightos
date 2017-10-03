/*
 * tick.c
 *
 *  Created on: 2017.10.2
 *      Author: xzy47
 */

#include "lightos.h"
#include "task.h"

extern l_base_t l_nextTaskID;
extern const l_uint8_t l_priorityBitmap[];
extern l_uint8_t l_taskPriorityTable;

static l_tick_t l_tick = 0;

l_uint8_t curPriority = 0;

l_tick_t LTickGet(void)
{
    return l_tick;
}

void LTickSet(l_tick_t tick)
{
    /*ENTER_CRITICAL*/
    l_tick = tick;
    /*EXIT_CRITICAL*/
}

l_err_t LTickIncrement(void)
{
    static l_uint32_t osTick;
    curPriority = l_priorityBitmap[l_taskPriorityTable];

    l_item_t *curItem = l_TCBArray[curPriority].pxItem;

    if(((l_tcb_t *)curItem->pvItem)->ulTimeSlice == 0)
    {
        return L_EOK;
    }

    if(++osTick == ((l_tcb_t *)curItem->pvItem)->ulTimeSlice)
    {
         osTick = 0;
         l_TCBArray[curPriority].pxItem = curItem->pxNext;
         l_nextTaskID = ((l_tcb_t *)l_TCBArray[curPriority].pxItem->pvItem)->ucTID;
    }
    return L_EOK;
}

/*l_tick_t LTickFromMillisecond(l_uint32_t ms)*/

