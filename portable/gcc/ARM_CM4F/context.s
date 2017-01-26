.cpu cortex-m4
.syntax unified
.thumb

.align 4
Cur_TaskIDConst:   .word Cur_TaskID
PSP_arrayConst:    .word PSP_array
l_nextTaskIDConst: .word l_nextTaskID

.global PendSV_Handler
.type PendSV_Handler, %function
PendSV_Handler:
	mrs r0, psp
	isb

	stmdb  	r0!, {r4-r11}
	ldr    	R1,Cur_TaskIDConst
    ldr    	R3,PSP_arrayConst
    ldr    	R4,l_nextTaskIDConst

	ldrb	r2,[r1]
	str    	R0,[R3, R2, LSL #2]
	LDRB    R4,[R4]
	STRB    R4,[R1]
	LDR    	R0,[R3, R4, LSL #2]

	LDMIA  	R0!,{R4-R11}

	msr psp, r0
	isb

	bx lr


