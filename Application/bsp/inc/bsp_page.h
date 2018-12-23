#ifndef __BSP_PAGE_H__
#define __BSP_PAGE_H__

#include "config.h"

#define PAGE0     (uint8_t)0
#define PAGE1     (uint8_t)1
#define PAGE2     (uint8_t)2
#define PAGE3     (uint8_t)3
#define PAGE4     (uint8_t)4
#define PAGE5     (uint8_t)5
#define PAGE6     (uint8_t)6
#define PAGE7     (uint8_t)7
#define PAGE8     (uint8_t)8
#define PAGE9     (uint8_t)9
#define PAGE10    (uint8_t)10
#define PAGE11    (uint8_t)11
#define PAGE12    (uint8_t)12
#define PAGE13    (uint8_t)13
#define PAGE14    (uint8_t)14
#define PAGE15    (uint8_t)15

extern uint8_t (*PageKey_Process[12])(uint16_t);

#endif
