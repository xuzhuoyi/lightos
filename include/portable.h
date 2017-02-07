/*
 * portable.h
 *
 *  Created on: 2017Äê1ÔÂ25ÈÕ
 *      Author: xzy47
 */

#ifndef INCLUDE_PORTABLE_H_
#define INCLUDE_PORTABLE_H_

#include "portmacro.h"
#include "lightos_config.h"

#if LCONFIG_BYTE_ALIGNMENT == 32
    #define LPORT_BYTE_ALIGNMENT_MASK ( 0x001f )
#endif

#if LCONFIG_BYTE_ALIGNMENT == 16
    #define LPORT_BYTE_ALIGNMENT_MASK ( 0x000f )
#endif

#if LCONFIG_BYTE_ALIGNMENT == 8
    #define LPORT_BYTE_ALIGNMENT_MASK ( 0x0007 )
#endif

#if LCONFIG_BYTE_ALIGNMENT == 4
    #define LPORT_BYTE_ALIGNMENT_MASK ( 0x0003 )
#endif

#if LCONFIG_BYTE_ALIGNMENT == 2
    #define LPORT_BYTE_ALIGNMENT_MASK ( 0x0001 )
#endif

#if LCONFIG_BYTE_ALIGNMENT == 1
    #define LPORT_BYTE_ALIGNMENT_MASK ( 0x0000 )
#endif

void LPortInitScheduler(void);
l_stack_t *LPortInitStack(l_stack_t *pxTopOfStack, LTaskFunction_t pxEntry);

#endif /* INCLUDE_PORTABLE_H_ */
