
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "bsp_init.h"
#include "bsp_dev.h"
#include "bsp_func.h"
#include "cmsis_os.h"

/* Private variables ---------------------------------------------------------*/
osThreadId DWIN_ReceiveTasHandle;
osThreadId WIFI_ReceiveTasHandle;
osThreadId mainFuncTaskHandle;
osThreadId DWIN_GuiTaskHandle;

osSemaphoreId DWIN_ReciveSemHandle;
osSemaphoreId DWIN_GuiSemHandle;
osSemaphoreId WIFI_ReceiveSemHandle;

/* Private function prototypes -----------------------------------------------*/
void StartDWIN_ReceiveTask(void const * argument);
void StartWIFI_ReceiveTask(void const * argument);
void StartmainFuncTask(void const * argument);
void StartDWIN_GuiTask(void const * argument);

int main(void)
{
  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  Bsp_Peripherals_Init();
  
  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of DWIN_ReciveSem */
  osSemaphoreDef(DWIN_ReciveSem);
  DWIN_ReciveSemHandle = osSemaphoreCreate(osSemaphore(DWIN_ReciveSem), 1);

  /* definition and creation of DWIN_GuiSem */
  osSemaphoreDef(DWIN_GuiSem);
  DWIN_GuiSemHandle = osSemaphoreCreate(osSemaphore(DWIN_GuiSem), 1);

  /* definition and creation of WIFI_ReceiveSem */
  osSemaphoreDef(WIFI_ReceiveSem);
  WIFI_ReceiveSemHandle = osSemaphoreCreate(osSemaphore(WIFI_ReceiveSem), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of DWIN_ReceiveTas */
  osThreadDef(DWIN_ReceiveTas, StartDWIN_ReceiveTask, osPriorityHigh, 0, 128);
  DWIN_ReceiveTasHandle = osThreadCreate(osThread(DWIN_ReceiveTas), NULL);

  /* definition and creation of WIFI_ReceiveTas */
  osThreadDef(WIFI_ReceiveTas, StartWIFI_ReceiveTask, osPriorityLow, 0, 128);
  WIFI_ReceiveTasHandle = osThreadCreate(osThread(WIFI_ReceiveTas), NULL);

  /* definition and creation of mainFuncTask */
  osThreadDef(mainFuncTask, StartmainFuncTask, osPriorityNormal, 0, 128);
  mainFuncTaskHandle = osThreadCreate(osThread(mainFuncTask), NULL);

  /* definition and creation of DWIN_GuiTask */
  osThreadDef(DWIN_GuiTask, StartDWIN_GuiTask, osPriorityHigh, 0, 128);
  DWIN_GuiTaskHandle = osThreadCreate(osThread(DWIN_GuiTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
 

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/* StartDWIN_ReceiveTask */
void StartDWIN_ReceiveTask(void const * argument)
{
	int ret;
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
		ret = osSemaphoreWait(DWIN_ReciveSemHandle, 1000);
		if(ret > 0)
			Bsp_GUI_Update();
		else if(ret == 0)
			Bsp_DWIN_Rev_Process();
  }
  /* USER CODE END 5 */ 
}
/* StartWIFI_ReceiveTask */
void StartWIFI_ReceiveTask(void const * argument)
{
  /* USER CODE BEGIN StartWIFI_ReceiveTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartWIFI_ReceiveTask */
}

/* StartmainFuncTask */
void StartmainFuncTask(void const * argument)
{
  /* USER CODE BEGIN StartmainFuncTask */
  /* Infinite loop */
  for(;;)
  {
//    Bsp_Sensor_GetTemperature();
		Bsp_Func_GetRealTime();
		Bsp_Func_Timer();
    Bsp_Func_Special_Mode();
		Bsp_Func_Alarm_Process();
		Bsp_Func_PutHot_Process();
		Bsp_Func_GetHot_Process();
		Bsp_MainFunc_Process();
    Bsp_Func_EEPROM_Update();
		osDelay(900);
  }
  /* USER CODE END StartmainFuncTask */
}

/* StartDWIN_GuiTask */
void StartDWIN_GuiTask(void const * argument)
{
  /* USER CODE BEGIN StartDWIN_GuiTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDWIN_GuiTask */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
