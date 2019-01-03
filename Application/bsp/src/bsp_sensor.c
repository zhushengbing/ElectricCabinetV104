#include "bsp_sensor.h"
#include "bsp_init.h"
#include "bsp_max6675.h"
#include "bsp_dev.h"
#include "stdlib.h"

#define FILTER_BUFFER_SIZE   5
#define MAX_FILTER_ERR       3

MAX6675_Port_TypeDef MAX6675_Port[3] = 
{
	[0] = {
					.MAX6675_CS = K1_CS_GPIO_Port,
					.MAX6675_SCK = K1_SCK_GPIO_Port,
					.MAX6675_MISO = K1_MISO_GPIO_Port,
	},
	[1] = {
					.MAX6675_CS = K2_CS_GPIO_Port,
					.MAX6675_SCK = K2_SCK_GPIO_Port,
					.MAX6675_MISO = K2_MISO_GPIO_Port,
	},
	[2] = {
					.MAX6675_CS = K3_CS_GPIO_Port,
					.MAX6675_SCK = K3_SCK_GPIO_Port,
					.MAX6675_MISO = K3_MISO_GPIO_Port,
	},
};

MAX6675_Pin_TypeDef MAX6675_Pin[3] = 
{
	[0] = {
					.MAX6675_CS = K1_CS_Pin,
					.MAX6675_SCK = K1_SCK_Pin,
					.MAX6675_MISO = K1_MISO_Pin,
	},
	[1] = {
					.MAX6675_CS = K2_CS_Pin,
					.MAX6675_SCK = K2_SCK_Pin,
					.MAX6675_MISO = K2_MISO_Pin,
	},
	[2] = {
					.MAX6675_CS = K3_CS_Pin,
					.MAX6675_SCK = K3_SCK_Pin,
					.MAX6675_MISO = K3_MISO_Pin,
	},
};

int MovingFilter(uint16_t data, uint8_t dif_max, uint16_t *pFilterBuffer, uint8_t *pCount)
{
	static int ErrCount = MAX_FILTER_ERR;
	int i;
	long sum = 0;

	if(*pCount)
	{
		i = pFilterBuffer[*pCount - 1];
	}
	else
		i = pFilterBuffer[0];

	if(abs(data - i) > dif_max)
	{
		if( (ErrCount++) < MAX_FILTER_ERR )
			return -1;
		else
			ErrCount = 0;
	}

	
	if(*pCount >= FILTER_BUFFER_SIZE)
		*pCount = 0;

	pFilterBuffer[(*pCount)++] = data;

	for(i=0;i<FILTER_BUFFER_SIZE;i++)
		sum += pFilterBuffer[i];

	return sum/FILTER_BUFFER_SIZE;
}

void Bsp_Sensor_GetInWaterTemperature(void)
{
	static uint16_t FilterBuffer[FILTER_BUFFER_SIZE] = {0};
	static uint8_t FilterBufferCount = 0;
	MAX6675_Value_TypeDef u16Value;
	
	u16Value.All = Bsp_GetMAX6675_Value(&MAX6675_Port[1], &MAX6675_Pin[1]);
	
	if(u16Value.Bit.Input == 0 && u16Value.All)
	{
		int Value;
		Value = MovingFilter(u16Value.Bit.Sample >> 2, 4, FilterBuffer, &FilterBufferCount);
		if(Value > 0)
			Device_Params_Info.RT_InWater = Value;
	}
}

void Bsp_Sensor_GetOutWaterTemperature(void)
{
	static uint16_t FilterBuffer[FILTER_BUFFER_SIZE] = {0};
	static uint8_t FilterBufferCount = 0;
	MAX6675_Value_TypeDef u16Value;
	
	u16Value.All = Bsp_GetMAX6675_Value(&MAX6675_Port[2], &MAX6675_Pin[2]);
	
	if(u16Value.Bit.Input == 0 && u16Value.All)
	{
		int Value;
		Value = MovingFilter(u16Value.Bit.Sample >> 2, 4, FilterBuffer, &FilterBufferCount);
		if(Value > 0)
			Device_Params_Info.RT_OutWater = Value;
	}
}

void Bsp_Sensor_GetDeviceTemperature(void)
{
	static uint16_t FilterBuffer[FILTER_BUFFER_SIZE] = {0};
	static uint8_t FilterBufferCount = 0;
	MAX6675_Value_TypeDef u16Value;
	
	u16Value.All = Bsp_GetMAX6675_Value(&MAX6675_Port[0], &MAX6675_Pin[0]);
	
	if(u16Value.Bit.Input == 0 && u16Value.All)
	{
		int Value;
		Value = MovingFilter(u16Value.Bit.Sample >> 2, 4, FilterBuffer, &FilterBufferCount);
		if(Value > 0)
			Device_Params_Info.RT_Air = Value;
	}
}



