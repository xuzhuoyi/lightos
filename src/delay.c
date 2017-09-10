/*
 * delay.c
 *
 *  Created on: 2017.9.10日
 *      Author: xuzhuoyi
 */

#include "task.h"

extern l_tcb_t *l_curTCB[];
extern l_uint8_t curPriority;

void LDelayTick(l_base_t xDelayTick)
{
    l_curTCB[curPriority]->xTaskStatus = L_SPENDING;
}
