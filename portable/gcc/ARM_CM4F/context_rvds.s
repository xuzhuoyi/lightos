    AREA |.text|, CODE, READONLY, ALIGN=2
    THUMB


    IMPORT l_curTaskID
    IMPORT l_PSPArray
    IMPORT l_nextTaskID


PendSV_Handler PROC
	EXPORT PendSV_Handler
		
    MRS     R0, PSP
    ISB

    STMDB   R0!, {R4-R11}
    LDR     R1, =l_curTaskID
    LDR     R3, =l_PSPArray
    LDR     R4, =l_nextTaskID

    LDRB    R2, [R1]
    STR     R0, [R3, R2, LSL #2]
    LDRB    R4, [R4]
    STRB    R4, [R1]
    LDR     R0, [R3, R4, LSL #2]

    LDMIA   R0!, {R4-R11}

    MSR     PSP, R0
    ISB

    BX      LR
	ENDP

    ALIGN   4

    END
