/*
 * portmacro.h
 *
 *  Created on: 2017��1��25��
 *      Author: xzy47
 */

#ifndef PORTABLE_PORTMACRO_H_
#define PORTABLE_PORTMACRO_H_

#include <stdint.h>

#define LPORT_STACK_GROWTH_DIR L_STACK_LOW_TO_HIGH

typedef uint_least8_t l_uint8_t;
typedef uint16_t l_uint16_t;
typedef uint32_t l_uint32_t;
typedef uint64_t l_uint64_t;

typedef int_least8_t l_int8_t;
typedef int16_t l_int16_t;
typedef int32_t l_int32_t;
typedef int64_t l_int64_t;

typedef uint32_t l_stack_t;
typedef l_uint32_t l_handle_t;
typedef l_uint32_t l_sp_t;
typedef l_uint32_t l_base_t;

#define LPORT_NVIC_INT_CTRL_REG		( * ( ( volatile uint32_t * ) 0xe000ed04 ) )
#define LPORT_NVIC_PENDSVSET_BIT	( 1UL << 28UL )

#define LPORT_SYSTICK_ENABLE        /*SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk*/
#define LPORT_SYSTICK_DISABLE       /*SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk*/

extern  cregister  volatile  unsigned  int  IFR;
extern  cregister  volatile  unsigned  int  IER;

#define  BSP_OS_TIMER2_ADDR_BASE                               (0x00000C10u)
#define  BSP_OS_TIMER2_TIM                      *((l_uint32_t *)(BSP_OS_TIMER2_ADDR_BASE + 0x0))
#define  BSP_OS_TIMER2_PRD                      *((l_uint32_t *)(BSP_OS_TIMER2_ADDR_BASE + 0x2))
#define  BSP_OS_TIMER2_TCR                      *((l_uint16_t *)(BSP_OS_TIMER2_ADDR_BASE + 0x4))
#define  BSP_OS_TIMER2_TPR                      *((l_uint16_t *)(BSP_OS_TIMER2_ADDR_BASE + 0x6))
#define  BSP_OS_TIMER2_TPRH                     *((l_uint16_t *)(BSP_OS_TIMER2_ADDR_BASE + 0x7))

                                                                /* TIMER2 Register bits.                                */
#define  BSP_OS_TIMER2_TCR_TIF                                  0x8000u
#define  BSP_OS_TIMER2_TCR_TIE                                  0x4000u
#define  BSP_OS_TIMER2_TCR_FREE                                 0x0800u
#define  BSP_OS_TIMER2_TCR_SOFT                                 0x0400u
#define  BSP_OS_TIMER2_TCR_TRB                                  0x0020u
#define  BSP_OS_TIMER2_TCR_TSS                                  0x0010u

#define  BSP_OS_TIMER2_TPR_PSC_MASK                             0xFF00u
#define  BSP_OS_TIMER2_TPR_TDDR_MASK                            0x00FFu

#define  BSP_OS_TIMER2_TPRH_PSCH_MASK                           0xFF00u
#define  BSP_OS_TIMER2_TPRH_TDDRH_MASK                          0x00FFu

#define  CPU_INT_RTOSINT                                  16u
#define  CPU_INT_DLOGINT                                  15u
#define  CPU_INT_INT14                                    14u
#define  CPU_INT_INT13                                    13u
#define  CPU_INT_INT12                                    12u
#define  CPU_INT_INT11                                    11u
#define  CPU_INT_INT10                                    10u
#define  CPU_INT_INT9                                      9u
#define  CPU_INT_INT8                                      8u
#define  CPU_INT_INT7                                      7u
#define  CPU_INT_INT6                                      65
#define  CPU_INT_INT5                                      5u
#define  CPU_INT_INT4                                      4u
#define  CPU_INT_INT3                                      3u
#define  CPU_INT_INT2                                      2u
#define  CPU_INT_INT1                                      1u

#define LPORT_CONTEXT_SWITCH    {IER = IER | (1u << (14-1));asm(" TRAP #16");}

void CPU_IntSrcEn(l_uint16_t bit);
l_uint16_t     OS_CPU_GetST0         (void);
l_uint16_t     OS_CPU_GetST1         (void);
void  BSP_OS_TIMER2_TickInit (l_uint32_t  cnts);
void  BSP_OS_TIMER2_TickHandler(void);
void  OS_CPU_RTOSINT_Handler(void);

#endif /* PORTABLE_PORTMACRO_H_ */
