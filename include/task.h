/*
 * task.h
 *
 *  Created on: 2017Äê1ÔÂ25ÈÕ
 *      Author: xzy47
 */

#ifndef INCLUDE_TASK_H_
#define INCLUDE_TASK_H_

#include "lightos.h"

extern l_uint32_t PSP_array[];

extern l_uint8_t Cur_TaskID;

l_uint32_t LTaskIncrementTick(void);
l_uint32_t LTaskCreate(l_uint8_t          cTID,
                       LTaskFunction_t    pxEntry,
                       const char * const pcName,
                       const l_uint16_t   usStackDepth);

void LTaskStartScheduler(void);
void LTaskStopScheduler(void);

#endif /* INCLUDE_TASK_H_ */
