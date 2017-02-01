/*
 * port.c
 *
 *  Created on: 2017Äê1ÔÂ25ÈÕ
 *      Author: xzy47
 */


#include "lightos.h"

extern l_uint32_t PSP_array[3];
extern l_uint8_t l_nextTaskID;
extern l_uint8_t Cur_TaskID;
l_uint8_t l_tmpNextID = 0;


void LPortSysTickHandler( void )
{
	LTaskIncrementTick();
	if(l_nextTaskID != Cur_TaskID)
	    LPORT_NVIC_INT_CTRL_REG = LPORT_NVIC_PENDSVSET_BIT;
}
