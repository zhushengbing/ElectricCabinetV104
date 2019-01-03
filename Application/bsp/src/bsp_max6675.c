#include "bsp_max6675.h"
#include "bsp_init.h"

void Bsp_MAX6675_Delay(void)
{
	uint16_t u16Num = 100;
	while(u16Num--);
}

uint16_t Bsp_GetMAX6675_Value(MAX6675_Port_TypeDef* Port, MAX6675_Pin_TypeDef* Pin)
{
	uint16_t i;
	uint16_t value = 0;
	
	Port->MAX6675_SCK->ODR &= ~Pin->MAX6675_SCK;
	Bsp_MAX6675_Delay();
	Port->MAX6675_CS->ODR &= ~Pin->MAX6675_CS;
	Bsp_MAX6675_Delay();
	for(i=0;i<16;i++)
	{
		value <<= 1;
		if(Port->MAX6675_MISO->IDR & Pin->MAX6675_MISO)
			value |= 0x01;
		Port->MAX6675_SCK->ODR |= Pin->MAX6675_SCK;
		Bsp_MAX6675_Delay();
		Port->MAX6675_SCK->ODR &= ~Pin->MAX6675_SCK;
		Bsp_MAX6675_Delay();
	}
	Port->MAX6675_CS->ODR |= Pin->MAX6675_CS;
	return value;
}

