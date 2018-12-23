#include "bsp_init.h"
#include "bsp_dev.h"
#include "cmsis_os.h"

extern osSemaphoreId DWIN_ReciveSemHandle;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &HAL_BaseTime_TIM)
	{
		if(DWIN_Params_Info.DWIN_Rev.LifeTime)
		{
			DWIN_Params_Info.DWIN_Rev.LifeTime--;
			if(DWIN_Params_Info.DWIN_Rev.LifeTime == 0)
			{
				DWIN_Params_Info.DWIN_Rev.Count = 0;
				osSemaphoreRelease(DWIN_ReciveSemHandle);
			}
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t rev;
	if(__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE))
	{
		rev = huart->Instance->RDR;
		__HAL_UART_CLEAR_OREFLAG(huart);
	}
	else if(__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE))
	{
		rev = huart->Instance->RDR;
		
		if(huart == &HAL_DWIN_USART)
		{
			if(DWIN_Params_Info.DWIN_Rev.Count < DWIN_BUFFER_SIZE)
			{
				DWIN_Params_Info.DWIN_Rev.Buffer.Data[DWIN_Params_Info.DWIN_Rev.Count++] = rev;
				DWIN_Params_Info.DWIN_Rev.LifeTime = 3;
			}
		}
	}
}


