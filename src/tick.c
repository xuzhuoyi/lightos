/*
 * tick.c
 *
 *  Created on: 2017.10.2
 *      Author: xzy47
 */

#include "lightos.h"
#include "task.h"
#include <stdlib.h>

extern l_base_t l_nextTaskID;
extern const l_uint8_t l_priorityBitmap[];
extern l_uint8_t l_taskPriorityTable;
extern l_tick_t l_nextWeakTick;
extern l_list_t l_delayTaskList;

static l_tick_t l_tick = 0;

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
    l_uint32_t i;
    l_uint8_t ucNumOfItems = l_delayTaskList.ucNumberOfItems;
    l_item_t * pxDelayItem;
    l_tick++;

    if(l_nextWeakTick <= l_tick)
    {
        l_nextWeakTick = -1;
        for(i = 0; i < ucNumOfItems; i++)
        {
            if(((l_tcb_t *)l_delayTaskList.pxItem->pvItem)->xReadyTick <= l_tick)
            {
                ((l_tcb_t *)l_delayTaskList.pxItem->pvItem)->xTaskStatus = L_SREADY;
                pxDelayItem = (l_item_t *)malloc(sizeof(l_item_t));
                pxDelayItem->pvItem = l_delayTaskList.pxItem->pvItem;
                LListInsertEnd(&l_TCBArray[((l_tcb_t *)l_delayTaskList.pxItem->pvItem)->ucPriority], pxDelayItem);
                l_taskPriorityTable |= 1 << ((l_tcb_t *)l_delayTaskList.pxItem->pvItem)->ucPriority;
                pxDelayItem = l_delayTaskList.pxItem;
                LListDeleteCur(&l_delayTaskList);
                free(pxDelayItem);
            }
            else
            {
            	l_delayTaskList.pxItem = l_delayTaskList.pxItem->pxNext;
            	if(l_nextWeakTick > ((l_tcb_t *)l_delayTaskList.pxItem->pvItem)->xReadyTick)
            	    l_nextWeakTick = ((l_tcb_t *)l_delayTaskList.pxItem->pvItem)->xReadyTick;
            }


        }
    }
    LSchedulerRun(L_SCHEDULER_NORMAL);
    return L_EOK;
}

/*l_tick_t LTickFromMillisecond(l_uint32_t ms)*/
