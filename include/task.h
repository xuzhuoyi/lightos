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

extern l_uint8_t l_curTaskID;
extern l_uint8_t l_nextTaskID;

typedef struct l_taskControlBlock
{
    l_stack_t   *pxStack;
    l_uint8_t   ucTID;
    l_uint32_t  ulTimeSlice;
} l_tcb_t;


l_err_t LTaskIncrementTick(void);
l_err_t LTaskCreate(l_uint8_t           ucTID,
                    LTaskFunction_t     pxEntry,
                    const char * const  pcName,
                    const l_uint16_t    usStackDepth,
                    const l_uint32_t    ulTimeSlice,
                    l_uint32_t * const  pxHandle);

void LTaskStartScheduler(void);
void LTaskStopScheduler(void);
l_err_t LTaskDelete(l_uint32_t ulHandle);

#endif /* INCLUDE_TASK_H_ */
