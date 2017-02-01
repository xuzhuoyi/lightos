/*
 * ldef.h
 *
 *  Created on: 2017Äê2ÔÂ1ÈÕ
 *      Author: xzy47
 */

#ifndef INCLUDE_LDEF_H_
#define INCLUDE_LDEF_H_

#include "portmacro.h"

typedef void (*LTaskFunction_t)( void * );

typedef l_uint8_t l_err_t;

#define L_EOK                0
#define L_ERROR              1
#define L_ETASK_NUM_OVERFLOW 2

#endif /* INCLUDE_LDEF_H_ */
