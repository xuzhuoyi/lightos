/*
 * task.h
 *
 *  Created on: 2017Äê1ÔÂ25ÈÕ
 *      Author: xzy47
 */

#ifndef INCLUDE_TASK_H_
#define INCLUDE_TASK_H_

#include "lightos.h"

l_uint32_t LTaskIncrementTick( void );
l_uint32_t LTaskCreate( void );
void LTaskStartScheduler( void );

#endif /* INCLUDE_TASK_H_ */
