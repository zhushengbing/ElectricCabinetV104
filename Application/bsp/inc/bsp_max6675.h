#ifndef __BSP_MAX6675_H__
#define __BSP_MAX6675_H__

#include "config.h"

typedef struct
{
	GPIO_TypeDef* MAX6675_CS;
	GPIO_TypeDef* MAX6675_SCK;
	GPIO_TypeDef* MAX6675_MISO;
}MAX6675_Port_TypeDef;

typedef struct
{
	uint16_t MAX6675_CS;
	uint16_t MAX6675_SCK;
	uint16_t MAX6675_MISO;
}MAX6675_Pin_TypeDef;

typedef union
{
	struct
	{
		uint8_t  State    :1;
		uint8_t  DeviceID :1;
		uint8_t  Input    :1;
		uint16_t Sample   :12;
		uint8_t  Dummy    :1;
	}Bit;
	uint16_t All;
}MAX6675_Value_TypeDef;

uint16_t Bsp_GetMAX6675_Value(MAX6675_Port_TypeDef* Port, MAX6675_Pin_TypeDef* Pin);

#endif
