/*
 * lightos.h
 *
 *  Created on: 2017.1.25
 *      Author: xzy47
 */

#ifndef INCLUDE_LIGHTOS_H_
#define INCLUDE_LIGHTOS_H_

#include "ldef.h"
#include "portable.h"

extern const l_uint8_t l_priorityBitmap[];

l_err_t LTickIncrement(void);
l_tick_t LTickGet(void);
void LTickSet(l_tick_t tick);

void LListInsertEnd(l_list_t *pxList, l_item_t *pxInsItem);
void LListDeleteCur(l_list_t *pxList);

#endif /* INCLUDE_LIGHTOS_H_ */
