/*
 * portmacro.h
 *
 *  Created on: 2017.1.25
 *      Author: xzy47
 */

#ifndef PORTABLE_PORTMACRO_H_
#define PORTABLE_PORTMACRO_H_

#include <stdint.h>
#include "stm32f4xx.h"
#include "core_cm4.h"

#define LPORT_STACK_GROWTH_DIR L_STACK_HIGH_TO_LOW

typedef uint8_t l_uint8_t;
typedef uint16_t l_uint16_t;
typedef uint32_t l_uint32_t;
typedef uint64_t l_uint64_t;

typedef int8_t l_int8_t;
typedef int16_t l_int16_t;
typedef int32_t l_int32_t;
typedef int64_t l_int64_t;

typedef uint32_t l_stack_t;
typedef l_uint32_t l_handle_t;
typedef l_uint32_t l_sp_t;
typedef l_uint32_t l_base_t;

#define LPORT_NVIC_INT_CTRL_REG		( * ( ( volatile uint32_t * ) 0xe000ed04 ) )
#define LPORT_NVIC_PENDSVSET_BIT	( 1UL << 28UL )

#define LPORT_SYSTICK_ENABLE        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk
#define LPORT_SYSTICK_DISABLE       SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk

#endif /* PORTABLE_PORTMACRO_H_ */
