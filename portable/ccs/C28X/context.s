
.ref _l_curTaskID
.ref _l_PSPArray
.ref _l_nextTaskID

    .def   _OS_CPU_RTOSINT_Handler
    .def   _OS_CPU_GetST0
    .def   _OS_CPU_GetST1

OS_CTX_SAVE  .macro
                                                                ; Save remaining registers.
    PUSH    AR1H:AR0H                                           ;   AR1H:AR0H
    PUSH    XAR2                                                ;   XAR2
    PUSH    XAR3                                                ;   XAR3
    PUSH    XAR4                                                ;   XAR4
    PUSH    XAR5                                                ;   XAR5
    PUSH    XAR6                                                ;   XAR6
    PUSH    XAR7                                                ;   XAR7
    PUSH    XT                                                  ;   XT
    PUSH    RPC                                                 ;   RPC

 .if .TMS320C2800_FPU32 = 1                                     ; Save FPU registers, if enabled.
    MOVZ    AR1   , @SP                                         ;   Can't use SP for indirect addressing.
    MOV32  *XAR1++, R0H                                         ;   R0H
    MOV32  *XAR1++, R1H                                         ;   R1H
    MOV32  *XAR1++, R2H                                         ;   R2H
    MOV32  *XAR1++, R3H                                         ;   R3H
    MOV32  *XAR1++, R4H                                         ;   R4H
    MOV32  *XAR1++, R5H                                         ;   R5H
    MOV32  *XAR1++, R6H                                         ;   R6H
    MOV32  *XAR1++, R7H                                         ;   R7H
    MOV32  *XAR1++, STF                                         ;   STF
    MOV     AL    , AR1                                         ;   Can't use indirect addressing with RB.
    MOV     @SP   , AL                                          ;   Restore SP.
    PUSH    RB                                                  ;   RB
 .endif
    .endm


;********************************************************************************************************
;                                           OS_CTX_RESTORE
;
; Description : This MACRO restores the CPU registers (i.e. context) from the new task's stack in the
;               reverse order of OS_CTX_SAVE (see above)
;********************************************************************************************************

OS_CTX_RESTORE  .macro
 .if .TMS320C2800_FPU32 = 1                                     ; Restore FPU registers, if enabled.
    POP     RB                                                  ;   RB
    MOVZ    AR1, @SP                                            ;   Can't use SP for indirect addressing.
    MOV32   STF, *--XAR1                                        ;   STF
    MOV32   R7H, *--XAR1                                        ;   R7H
    MOV32   R6H, *--XAR1                                        ;   R6H
    MOV32   R5H, *--XAR1                                        ;   R5H
    MOV32   R4H, *--XAR1                                        ;   R4H
    MOV32   R3H, *--XAR1                                        ;   R3H
    MOV32   R2H, *--XAR1                                        ;   R2H
    MOV32   R1H, *--XAR1                                        ;   R1H
    MOV32   R0H, *--XAR1                                        ;   R0H
    MOV     AL    , AR1                                         ;   Restore SP.
    MOV     @SP   , AL
 .endif
                                                                ; Restore CPU registers part 1.
    POP     RPC                                                 ;   RPC
    POP     XT                                                  ;   XT
    POP     XAR7                                                ;   XAR7
    POP     XAR6                                                ;   XAR6
    POP     XAR5                                                ;   XAR5
    POP     XAR4                                                ;   XAR4
    POP     XAR3                                                ;   XAR3
    POP     XAR2                                                ;   XAR2

                                                                ; Overwrite saved IER in stack frame.
    MOVZ    AR0 , @SP                                           ;   Get SP.
    SUBB    XAR0, #6                                            ;   Point to old IER.
    MOVL    ACC , *XAR0                                         ;   Get old DBGSTAT:IER.
    AND     ACC, #0xFFFF << 16
    MOV     AL, IER                                             ;   Get current IER.
    MOVL   *XAR0, ACC                                           ;   Overwrite IER.

                                                                ; Restore registers part 2.
    POP     AR1H:AR0H                                           ;   AR1H:AR0H
    .endm


.text
    .newblock

.asmfunc
_OS_CPU_RTOSINT_Handler:
    OS_CTX_SAVE

    MOVL    R1, _l_curTaskID
    MOVL    R3, _l_PSPArray
    MOVL    R4, _l_nextTaskID

    MOVB    R2, *R1
    MOVL    ACC, R2 << #2
    ADDL    ACC, R3
    MOVL    *ACC, SP
    MOVB    R4, *R4
    MOVB    *R1, R4
    MOVL    ACC, R4 << #2
    ADDL    ACC, R3
    MOVL    *ACC, R0
    MOVL     SP, R0
    OS_CTX_RESTORE

    LCR      LR
     .endasmfunc

    .asmfunc
_OS_CPU_GetST0:
    PUSH    ST0
    POP     AL
    LRETR
    .endasmfunc

    .asmfunc
_OS_CPU_GetST1:
    PUSH    ST1
    POP     AL
    LRETR
    .endasmfunc
.end
