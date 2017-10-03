/*
 * list.c
 *
 *  Created on: 2017.10.3
 *      Author: xzy47
 */

#include "ldef.h"

void LListInsertEnd(l_list_t *pxList, l_item_t *pxInsItem)
{
    if(pxList->ucNumberOfItems == 0)
    {
        pxList->pxItem = pxInsItem;
        pxInsItem->pxNext = pxInsItem;
        pxInsItem->pxPrevious = pxInsItem;
    }
    else
    {
        pxInsItem->pxNext = pxList->pxItem;
        pxInsItem->pxPrevious = pxList->pxItem->pxPrevious;
        pxList->pxItem->pxPrevious->pxNext = pxInsItem;
        pxList->pxItem->pxPrevious = pxInsItem;
    }
    pxList->ucNumberOfItems++;
}

void LListDeleteCur(l_list_t *pxList)
{
    pxList->pxItem->pxPrevious->pxNext = pxList->pxItem->pxNext;
    pxList->pxItem->pxNext->pxPrevious = pxList->pxItem->pxPrevious;
    pxList->pxItem = pxList->pxItem->pxNext;
    pxList->ucNumberOfItems--;
}

