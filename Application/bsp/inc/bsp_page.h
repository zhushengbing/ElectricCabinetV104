#ifndef __BSP_PAGE_H__
#define __BSP_PAGE_H__

#include "config.h"

typedef enum
{
	PAGE0 = 0,
	PAGE1,
	PAGE2,
	PAGE3,
	PAGE4,
	PAGE5,
	PAGE6,
	PAGE7,
	PAGE8,
	PAGE9,
	PAGE10,
	PAGE11,
	PAGE12,
	PAGE13,
	PAGE14,
	PAGE15,
	PAGEMax,
}PAGE_Index_Type;

typedef void (*pFunc_PageKey_Process)(uint16_t);

extern pFunc_PageKey_Process PageKey_Process[12];

#endif
