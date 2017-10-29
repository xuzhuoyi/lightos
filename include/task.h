/*
 * task.h
 *
 *  Created on: 2017.1.25
 *      Author: xzy47
 */

#ifndef INCLUDE_TASK_H_
#define INCLUDE_TASK_H_

#include "lightos.h"

extern l_sp_t l_PSPArray[];

extern l_base_t l_curTaskID;
extern l_base_t l_nextTaskID;

typedef l_uint8_t l_status_t;

typedef enum l_tcstatus
{
    L_TCSUSPENDED = -1, L_TCSREADY = 1
} l_tcstatus_t;


typedef struct l_taskControlBlock
{
    l_stack_t                   *pxStack;
    l_uint8_t                   ucTID;
    l_uint16_t                  usStackDepth;
    l_uint32_t                  ulTimeSlice;
    l_uint32_t                  ulSliceTick;
    l_uint8_t                   ucPriority;
    l_status_t                  xTaskStatus;
    l_tick_t                    xReadyTick;
    struct l_taskControlBlock   *pxNextTCB;
} l_tcb_t;

extern l_list_t l_TCBArray[];

extern l_uint8_t curPriority;

extern l_uint8_t l_taskPriorityTable;

l_err_t LTaskCreate(l_uint8_t           ucTID,
                    LTaskFunction_t     pxEntry,
                    const char * const  pcName,
                    const l_uint16_t    usStackDepth,
                    const l_uint32_t    ulTimeSlice,
                    l_uint8_t           ucPriority,
                    l_tcstatus_t        exTCStatus,
                    l_handle_t * const  pxHandle);

void LTaskStartScheduler(void);
void LTaskStopScheduler(void);

l_err_t LTaskDelete(l_uint32_t ulHandle);

void LTaskDelayTick(l_tick_t xDelayTick);

void LTaskSuspendSelf(void);
void LTaskReady(l_handle_t ulHandle);

#endif /* INCLUDE_TASK_H_ */
