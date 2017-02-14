/*
 * port.c
 *
 *  Created on: 2017年1月25日
 *      Author: xzy47
 */


#include "lightos.h"
#include "task.h"
#include <reg51.h>

extern void OSStartHighRdy(void);

l_stack_t * data l_port_8051_stk = 0;
l_uint16_t data l_port_8051_stkdep = 0;

void LPort8051PreSwitch(l_uint8_t ucTID)
{
	  l_port_8051_stk = l_TCBArray[ucTID]->pxStack;
	  l_port_8051_stkdep = l_TCBArray[ucTID]->usStackDepth;
}

void Timer0Interrupt(void) interrupt 1
{
    TH0 = 0x0FC;
    TL0 = 0x66;
    LTaskIncrementTick();
    if(l_nextTaskID != l_curTaskID)
    {
        OS_CPU_RTOSINT_Handler();
    }
}

void LPortInitScheduler(void)
{
	/* 设置PSP指向任务0堆栈的栈顶 */
	LTaskStartScheduler();
	TR0 = 1;
	LPort8051PreSwitch(0);
	OSStartHighRdy();

}

l_stack_t *LPortInitStack(l_stack_t *pxTopOfStack, LTaskFunction_t pxEntry)
{
    l_stack_t  *p_stk;
    l_uint8_t  *p_stk8;

                                                                    /* Prevent compiler warnings.                           */


                                                                    /* Load and pre-align stack pointer.                    */
        p_stk = pxTopOfStack;
        p_stk++;
        p_stk8 = (l_uint8_t *)p_stk;
                                                                    /* Save registers as if auto-saved.                     */
                                                                    /* Follow stacking method in "9) Perform automatic ...  */
        *p_stk8++ = (l_uint8_t)pxEntry;                                  /*   Save Return Address [PC+1].                        */                                                            /* ..context save" of section 3.4 in "SPRU430E"         */
        *p_stk8++ = 0x11;               /*   T:ST0                                              */
        *p_stk8++ = 0x33;                                    /*   ACC                                              */
        *p_stk8++ = 0x55;                                    /*   B                                              */
        

                                                                    /* Save remaining registers.                            */
        *p_stk8++ = 0x77;                                    /*   AR1H:AR0H                                          */
        *p_stk8++ = 0x99;                                    /*   XAR2                                               */
        *p_stk8++ = 0xAA;                                    /*   XAR3                                               */
        *p_stk8++ = 0xBB;                                    /*   XAR4: void * parameter.                            */
        *p_stk8++ = 0xCC;                                    /*   XAR5                                               */
        *p_stk8++ = 0xDD;                                    /*   XAR6                                               */
        *p_stk8++ = 0xEE;                                    /*   XAR7                                               */
        *p_stk8++ = 0xFF;                                    /*   XT                                                 */
				*p_stk8++ = 0x00;                                    
				*p_stk8++ = 0x11;
                                                                    /* Return pointer to next free location.                */
        return ((l_stack_t *)p_stk8);
}
