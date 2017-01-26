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

/*void LPortPendSVHandler( void )
{
	l_tmpNextID = l_nextTaskID;
	__asm volatile
	(
			"	.align 4							\n"
			    "Cur_TaskIDConst:   .word Cur_TaskID    \n"
				"PSP_arrayConst:    .word PSP_array     \n"
				"l_nextTaskIDConst: .word l_tmpNextID  \n"
			"	mrs r7, psp							\n"
	"	mrs r0, psp							\n"
	"	isb									\n"
	"										\n"
	"	stmdb  r0!, {r4-r11}			    \n"
	"	ldr    R1,Cur_TaskIDConst           \n"
    "   ldr    R3,PSP_arrayConst            \n"
    "   ldr    R4,l_nextTaskIDConst         \n"
	"	ldrb	r2,[r1]					    \n"
	"   str    R0,[R3, R2, LSL #2]          \n"
	"	LDRB    R4,[R4]                      \n"
	"   STRB    R4,[R1]                      \n"
	"   LDR    R0,[R3, R4, LSL #2]          \n"

	"   LDMIA  R0!,{R4-R11}                 \n"
    "                                       \n"
	"	msr psp, r0							\n"
	"	isb									\n"
	"										\n"
	"										\n"
	"	bx lr								\n"
	"										\n"

	:
	:
	:
	);
}
*/
