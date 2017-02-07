/*
 * port.c
 *
 *  Created on: 2017年1月25日
 *      Author: xzy47
 */


#include "lightos.h"
#include "task.h"

interrupt void LPortOSTimer2TickHandler (void)
{
    LPORT_OS_TIMER2_TCR |= LPORT_OS_TIMER2_TCR_TIF;
    LTaskIncrementTick();
    CPU_IntSrcEn(BSP_INT_ID_INT14);
}

void LPortInitScheduler(void)
{
	/* 设置PSP指向任务0堆栈的栈顶 */
	__asm volatile ("mov sp, %0\n" : : "r" ((l_PSPArray[l_curTaskID] + 16*4)) :"sp" );
	LTaskStartScheduler();

}

void CPU_IntSrcEn (l_uint16_t bit)
{
    if (bit <= CPU_INT_RTOSINT) {
        //CPU_CRITICAL_ENTER();
        IER = IER | (1u << (bit-1));
        //CPU_CRITICAL_EXIT();
    }
}

l_stack_t *LPortInitStack(l_stack_t *pxTopOfStack, LTaskFunction_t pxEntry)
{
    l_stack_t  *p_stk;
    l_uint32_t  *p_stk32;

                                                                    /* Prevent compiler warnings.                           */
        (void)&opt;

                                                                    /* Load and pre-align stack pointer.                    */
        p_stk = pxTopOfStack;
        p_stk++;
        p_stk32 = (l_uint32_t *)p_stk;
                                                                    /* Save registers as if auto-saved.                     */
                                                                    /* Follow stacking method in "9) Perform automatic ...  */
                                                                    /* ..context save" of section 3.4 in "SPRU430E"         */
        *p_stk32++ = (0x11110000) | OS_CPU_GetST0();                /*   T:ST0                                              */
        *p_stk32++ = 0x33332222;                                    /*   AH:AL                                              */
        *p_stk32++ = 0x55554444;                                    /*   PH:PL                                              */
        *p_stk32++ = 0x77776666;                                    /*   AR1:AR0                                            */
        *p_stk32++ = (0x00000000) | OS_CPU_GetST1();                /*   DP:ST1                                             */
        *p_stk32++ = 0x00000000;                                    /*   DBGSTAT:IER                                        */
        *p_stk32++ = (l_uint32_t)pxEntry;                                  /*   Save Return Address [PC+1].                        */

                                                                    /* Save remaining registers.                            */
        *p_stk32++ = 0x77776666;                                    /*   AR1H:AR0H                                          */
        *p_stk32++ = 0x99999999;                                    /*   XAR2                                               */
        *p_stk32++ = 0xAAAAAAAA;                                    /*   XAR3                                               */
        *p_stk32++ = 0xBBBBBBBB;                                    /*   XAR4: void * parameter.                            */
        *p_stk32++ = 0xCCCCCCCC;                                    /*   XAR5                                               */
        *p_stk32++ = 0xDDDDDDDD;                                    /*   XAR6                                               */
        *p_stk32++ = 0xEEEEEEEE;                                    /*   XAR7                                               */
        *p_stk32++ = 0xFFFFFFFF;                                    /*   XT                                                 */
        *p_stk32++ = 0x00000000;                                    /*   RPC                                                */

    #if __TMS320C28XX_FPU32__ == 1                                  /* Save FPU registers, if enabled.                      */
        *p_stk32++ = 0x00000000;                                    /*   R0H                                                */
        *p_stk32++ = 0x11111111;                                    /*   R1H                                                */
        *p_stk32++ = 0x22222222;                                    /*   R2H                                                */
        *p_stk32++ = 0x33333333;                                    /*   R3H                                                */
        *p_stk32++ = 0x44444444;                                    /*   R4H                                                */
        *p_stk32++ = 0x55555555;                                    /*   R5H                                                */
        *p_stk32++ = 0x66666666;                                    /*   R6H                                                */
        *p_stk32++ = 0x77777777;                                    /*   R7H                                                */
        *p_stk32++ = 0x00000000;                                    /*   STF                                                */
        *p_stk32++ = 0x00000000;                                    /*   RB                                                 */
    #endif
                                                                    /* Return pointer to next free location.                */
        return ((l_stack_t *)p_stk32);
}
