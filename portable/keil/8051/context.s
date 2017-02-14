
	
?PR?OS_CPU_RTOSINT_Handler?CONTEXT    SEGMENT CODE
?PR?OSStartHighRdy?CONTEXT            SEGMENT CODE
?PR?OS_CPU_GetST0?CONTEXT             SEGMENT CODE

	
    extrn data(l_port_8051_stk, l_port_8051_stkdep)
	extrn xdata(l_curTaskID, l_PSPArray, l_nextTaskID)

    public   OS_CPU_RTOSINT_Handler
    public   OS_CPU_GetST0
    public   OSStartHighRdy
		
?STACK SEGMENT IDATA
        RSEG ?STACK

OSStack:
		;DS n：保留n个存储单元
        DS 40H		;分配硬件堆栈的大小

OSStkStart IDATA OSStack-1

OS_CTX_SAVE  MACRO
   
    using   0
    PUSH	ACC
    PUSH	PSW
    PUSH	B
	PUSH	DPH
	PUSH	DPL
	PUSH	AR0
	PUSH	AR1
	PUSH	AR2
	PUSH	AR3
	PUSH	AR4
	PUSH	AR5
	PUSH	AR6
	PUSH	AR7

    ENDM


;********************************************************************************************************
;                                           OS_CTX_RESTORE
;
; Description : This MACRO restores the CPU registers (i.e. context) from the new task's stack in the
;               reverse order of OS_CTX_SAVE (see above)
;********************************************************************************************************

OS_CTX_RESTORE  MACRO
      
    using   0	  ; Restore CPU registers part 1.
    POP	    AR7
    POP	    AR6
    POP	    AR5
	POP	    AR4
	POP 	AR3
	POP 	AR2
	POP	    AR1
	POP	    AR0
	POP	    DPL
	POP	    DPH
	POP	    B
	POP	    PSW
	POP	    ACC
    
	endm
	
OS_STACK_RESTORE  MACRO

    using   0

restore_stack:
    INC     R0
    MOVX    A, @DPTR
    MOV     @R0, A
	INC     DPTR
    DJNZ    R5, restore_stack
	
	ENDM


    RSEG ?PR?OSStartHighRdy?CONTEXT
OSStartHighRdy:

    MOV     R1, #l_port_8051_stk
	INC     R1
	MOV     DPH, @R1
	INC		R1
	MOV     DPL, @R1

    MOV     R5, l_port_8051_stkdep + 1
	MOV     R0, #OSStkStart
	
	OS_STACK_RESTORE
	
    MOV     R3, #l_PSPArray
    MOV     DPTR, #l_nextTaskID
	MOVX    A, @DPTR
    RL      A
	RL      A
    ADD     A, R3
	MOV     R0, A
	MOV     A, @R0
	MOV     SP, A
	
    OS_CTX_RESTORE
                                                                ; IRET into task.
    RET


    RSEG    ?PR?OS_CPU_RTOSINT_Handler?CONTEXT
OS_CPU_RTOSINT_Handler:
    OS_CTX_SAVE

    MOV     R1, #l_curTaskID
    MOV     R3, #l_PSPArray
    MOV     R0, #l_nextTaskID

    MOV     ACC, @R1
    RL      A
	RL      A
    ADD     A, R3
	PUSH    AR1
    MOV     R1, ACC
    MOV     A, SP
    MOV     @R1, A
	MOV     A, @R0
    MOV     R0, A
	MOV     A, R1
	MOV     R5, A
	POP     AR1
	MOV     A, R0
    MOV     @R1, A
    MOV     ACC, R0
    RL      A
	RL      A
    ADD     A, R3
    MOV     R2, ACC
	MOV     A, R5
	MOV     R1, A
    MOV     A, @R1
    MOV     SP, A
    OS_CTX_RESTORE

    RET

    RSEG    ?PR?OS_CPU_GetST0?CONTEXT
OS_CPU_GetST0:
    using   0
    PUSH    PSW
    POP     AR7
    RET
	
	END
