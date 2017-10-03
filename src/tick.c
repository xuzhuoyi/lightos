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
    LSchedulerRun();
    return L_EOK;
}

/*l_tick_t LTickFromMillisecond(l_uint32_t ms)*/

