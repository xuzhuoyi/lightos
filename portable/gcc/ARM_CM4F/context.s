.cpu cortex-m4
.syntax unified
.thumb

.align 4
l_curTaskIDConst:   .word l_curTaskID
PSP_arrayConst:    .word PSP_array
l_nextTaskIDConst: .word l_nextTaskID

.global PendSV_Handler
.type PendSV_Handler, %function
PendSV_Handler:
    MRS     R0, PSP
    ISB

    STMDB   R0!, {R4-R11}
    LDR     R1, l_curTaskIDConst
    LDR     R3, PSP_arrayConst
    LDR     R4, l_nextTaskIDConst

    LDRB    R2, [R1]
    STR     R0, [R3, R2, LSL #2]
    LDRB    R4, [R4]
    STRB    R4, [R1]
    LDR     R0, [R3, R4, LSL #2]

    LDMIA   R0!, {R4-R11}

    MSR     PSP, R0
    ISB

    BX      LR


