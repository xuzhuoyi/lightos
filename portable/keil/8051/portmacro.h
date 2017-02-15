/*
 * portmacro.h
 *
 *  Created on: 2017Äê1ÔÂ25ÈÕ
 *      Author: xzy47
 */

#ifndef PORTABLE_PORTMACRO_H_
#define PORTABLE_PORTMACRO_H_

#define LPORT_STACK_GROWTH_DIR L_STACK_LOW_TO_HIGH

typedef unsigned char l_uint8_t;
typedef unsigned short l_uint16_t;
typedef unsigned long int l_uint32_t;

typedef char l_int8_t;
typedef short l_int16_t;
typedef long l_int32_t;

typedef l_uint8_t l_stack_t;
typedef l_uint32_t l_handle_t;
typedef l_uint8_t l_sp_t;
typedef l_uint8_t l_base_t;

#define LPORT_SYSTICK_ENABLE        /*SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk*/
#define LPORT_SYSTICK_DISABLE       /*SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk*/

void  OS_CPU_RTOSINT_Handler(void);

#endif /* PORTABLE_PORTMACRO_H_ */
