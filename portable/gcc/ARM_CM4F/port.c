/*
 * port.c
 *
 *  Created on: 2017.1.25
 *      Author: xzy47
 */


#include "lightos.h"
#include "task.h"


void LPortSysTickHandler(void)
{
    LTickIncrement();
//	if(l_nextTaskID != l_curTaskID)
//	    LPORT_NVIC_INT_CTRL_REG = LPORT_NVIC_PENDSVSET_BIT;
}

void LPortInitScheduler(void)
{
	void (*p_funcFirst)();

	__asm volatile ("MSR psp, %0\n" : : "r" ((l_PSPArray[l_curTaskID] + 16*4)) :"sp" );
	LTaskStartScheduler();

#ifdef LCONFIG_TASK_PRIVILEGE_LEVELS
	__asm volatile ("MSR control, %0" : : "r" (0x2) : "memory");
#else
	__asm volatile ("MSR control, %0" : : "r" (0x3) : "memory");
#endif

	__asm volatile ("isb 0xF":::"memory");

	curPriority = l_priorityBitmap[l_taskPriorityTable];

	p_funcFirst = (void (*)())*(uint32_t *)(l_PSPArray[l_nextTaskID] + 56);
	p_funcFirst();

}

l_stack_t *LPortInitStack(l_stack_t *pxTopOfStack, LTaskFunction_t pxEntry)
{
    l_stack_t  *p_stk;
    l_uint32_t  *p_stk32;
                                                                    /* Load and pre-align stack pointer.                    */
    p_stk = pxTopOfStack;
    p_stk--;
    p_stk32 = (l_uint32_t *)p_stk;
                                                                    /* Save registers as if auto-saved.                     */
                                                                    /* Follow stacking method in "9) Perform automatic ...  */
                                                                    /* ..context save" of section 3.4 in "SPRU430E"         */
    *p_stk32-- = 0x01000000;                                    /*   T:ST0                                              */
    *p_stk32-- = (l_uint32_t)pxEntry;                           /*   AH:AL                                              */
    *p_stk32-- = 0x55554444;                                    /*   PH:PL                                              */
    *p_stk32-- = 0x77776666;                                    /*   AR1:AR0                                            */
    *p_stk32-- = 0x99998888;                                    /*   DP:ST1                                             */
    *p_stk32-- = 0xBBBBAAAA;                                    /*   DBGSTAT:IER                                        */
    *p_stk32-- = 0xDDDDCCCC;                                    /*   Save Return Address [PC+1].                        */
                                                                    /* Save remaining registers.                            */
    *p_stk32-- = 0x77776666;                                    /*   AR1H:AR0H                                          */
    *p_stk32-- = 0x99999999;                                    /*   XAR2                                               */
    *p_stk32-- = 0xAAAAAAAA;                                    /*   XAR3                                               */
    *p_stk32-- = 0xBBBBBBBB;                                    /*   XAR4: void * parameter.                            */
    *p_stk32-- = 0xCCCCCCCC;                                    /*   XAR5                                               */
    *p_stk32-- = 0xDDDDDDDD;                                    /*   XAR6                                               */
    *p_stk32-- = 0xEEEEEEEE;                                    /*   XAR7                                               */
    *p_stk32-- = 0xFFFFFFFF;                                    /*   XT                                                 */
    *p_stk32 = 0x00000000;                                    /*   RPC                                                */
                                                                /* Return pointer to next free location.                */
    return ((l_stack_t *)p_stk32);

}

void SVC_Handler(void)
{
    LPORT_NVIC_INT_CTRL_REG = LPORT_NVIC_PENDSVSET_BIT;
}
