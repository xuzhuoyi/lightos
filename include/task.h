/*
 * task.h
 *
 *  Created on: 2017Äê1ÔÂ25ÈÕ
 *      Author: xzy47
 */

#ifndef INCLUDE_TASK_H_
#define INCLUDE_TASK_H_

#include "lightos.h"

extern l_uint32_t PSP_array[3];

extern l_uint8_t Cur_TaskID;

l_uint32_t LTaskIncrementTick( void );
l_uint32_t LTaskCreate( void );

void LTaskStartScheduler( void );
void LTaskStopScheduler( void );

#endif /* INCLUDE_TASK_H_ */
