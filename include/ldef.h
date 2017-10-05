/*
 * ldef.h
 *
 *  Created on: 2017.2.1
 *      Author: xzy47
 */

#ifndef INCLUDE_LDEF_H_
#define INCLUDE_LDEF_H_

#include "portmacro.h"

typedef void (*LTaskFunction_t)( void * );

typedef l_uint8_t   l_err_t;
typedef l_uint32_t  l_tick_t;

#define L_EOK                   0
#define L_ERROR                 1
#define L_ETASK_NUM_OVERFLOW    2

#define L_STACK_LOW_TO_HIGH     1
#define L_STACK_HIGH_TO_LOW     -1

#define L_SUSPENDED             -1
#define L_SREADY                1
#define L_SRUNNING              2
#define L_SPENDING              0

typedef enum l_scheduler_message
{
    L_SCHEDULER_NORMAL, L_SCHEDULER_NEXT
} l_schmsg_t;

typedef struct l_item
{
    void * pvItem;
    struct l_item *pxPrevious;
    struct l_item *pxNext;
} l_item_t;

typedef struct l_list
{
    uint8_t ucNumberOfItems;
    l_item_t * pxItem;
} l_list_t;

#endif /* INCLUDE_LDEF_H_ */
