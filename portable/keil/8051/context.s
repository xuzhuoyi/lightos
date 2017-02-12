
    extrn data(_l_curTaskID, _l_PSPArray, _l_nextTaskID)

    public   _OS_CPU_RTOSINT_Handler
    public   _OS_CPU_GetST0
    public   _OSStartHighRdy

OS_CTX_SAVE  MACRO
   
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
                                                                ; Restore CPU registers part 1.
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



_OSStartHighRdy:

    MOV     R3, _l_PSPArray
    MOV     R4, _l_nextTaskID

    MOV     R4, @XAR4
    MOV     R1, R4
    MOV     ACC, R4
    RL      ACC, #2
    ADD     ACC, R3
    MOV     R2, ACC
    MOV     A, @R2
    MOV     SP, A
    OS_CTX_RESTORE
                                                                ; IRET into task.
    RET


_OS_CPU_RTOSINT_Handler:
    OS_CTX_SAVE

    MOV     R1, _l_curTaskID
    MOV     R3, _l_PSPArray
    MOV     R4, _l_nextTaskID

    MOV     ACC, @R1
    RL      ACC, #2
    ADD     ACC, R3
    MOV     R2, ACC
    MOV     A, SP
    MOV     @R2, A
    MOV     R4, @R4
    MOV     @R1, R4
    MOV     ACC, R4
    RL      ACC, #2
    ADD     ACC, R3
    MOV     R2, ACC
    MOV     A, @R2
    MOV     SP, A
    OS_CTX_RESTORE

    RET

_OS_CPU_GetST0:
    PUSH    PSW
    POP     R7
    RET
	
	END
