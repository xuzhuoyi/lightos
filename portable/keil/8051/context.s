
	
?PR?OS_CPU_RTOSINT_Handler?CONTEXT    SEGMENT CODE
?PR?OSStartHighRdy?CONTEXT            SEGMENT CODE
?PR?OS_CPU_GetST0?CONTEXT             SEGMENT CODE

	
    extrn data(l_port_8051_stk, l_port_8051_stkdep, l_port_8051_curstk)
	extrn xdata(l_curTaskID, l_PSPArray, l_nextTaskID)

    public   OS_CPU_RTOSINT_Handler
    public   OS_CPU_GetST0
    public   OSStartHighRdy
    public   OSStkStart
		
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
    MOV     R1, #l_port_8051_stk
	INC     R1
	MOV     DPH, @R1
	INC		R1
	MOV     DPL, @R1

    MOV     R5, l_port_8051_stkdep
	MOV     R0, #OSStkStart

    INC     R0
    MOVX    A, @DPTR
    MOV     @R0, A
	INC     DPTR
    DJNZ    R5, $ - 4
	
	ENDM
	
OS_STACK_SAVE  MACRO

    using   0
    MOV     R1, #l_port_8051_curstk
	INC     R1
	MOV     DPH, @R1
	INC		R1
	MOV     DPL, @R1
	MOV     A, SP
	SUBB    A, #OSStkStart
	MOV     R5, A
	MOV     R0, #OSStkStart
save_stack:	
	INC     R0
	MOV     A, @R0
    MOVX    @DPTR, A
    
	INC     DPTR
    DJNZ    R5, save_stack
	
	ENDM


    RSEG ?PR?OSStartHighRdy?CONTEXT
OSStartHighRdy:
	
	OS_STACK_RESTORE
	
	MOV     SP, R0
	
    OS_CTX_RESTORE
                                                                ; IRET into task.
    RET


    RSEG    ?PR?OS_CPU_RTOSINT_Handler?CONTEXT
OS_CPU_RTOSINT_Handler:
    OS_CTX_SAVE
	OS_STACK_SAVE

    MOV     DPTR, #l_curTaskID
	MOVX    A, @DPTR
	MOV     R2, A
	MOV     DPTR, #l_PSPArray
	JZ      found_cur_psp
find_cur_psp:
    INC     DPTR
	DJNZ    R2, find_cur_psp
found_cur_psp:	
	MOV     A, SP
	MOVX    @DPTR, A
	
	MOV     DPTR, #l_nextTaskID
	MOVX    A, @DPTR
	MOV     R3, A
	MOV     R2, A
	MOV     DPTR, #l_PSPArray
	JZ      found_next_psp
find_next_psp:
    INC     DPTR
	DJNZ    R2, find_next_psp
found_next_psp:	
	MOVX    A, @DPTR
	MOV     SP, A
	
	MOV     A, R3
	MOV     DPTR, #l_curTaskID
	MOVX    @DPTR, A
	
	OS_STACK_RESTORE
    OS_CTX_RESTORE

    RETI

    RSEG    ?PR?OS_CPU_GetST0?CONTEXT
OS_CPU_GetST0:
    using   0
    PUSH    PSW
    POP     AR7
    RET
	
	END
