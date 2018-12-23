#include "bsp_init.h"
#include "bsp_dev.h"

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef HAL_K1_SPI;
SPI_HandleTypeDef HAL_K3_SPI;

TIM_HandleTypeDef HAL_BaseTime_TIM;

UART_HandleTypeDef HAL_DWIN_USART;
UART_HandleTypeDef HAL_WIFI_USART;
UART_HandleTypeDef HAL_RS485_USART;

struct Dev_Params_Tags Device_Params_Info;

static void Bsp_SystemClock_Init(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 3, 0);
}

static void Bsp_K1_SPI_Init(void)
{

  /* SPI1 parameter configuration*/
  HAL_K1_SPI.Instance = SPI1;
  HAL_K1_SPI.Init.Mode = SPI_MODE_MASTER;
  HAL_K1_SPI.Init.Direction = SPI_DIRECTION_2LINES_RXONLY;
  HAL_K1_SPI.Init.DataSize = SPI_DATASIZE_4BIT;
  HAL_K1_SPI.Init.CLKPolarity = SPI_POLARITY_LOW;
  HAL_K1_SPI.Init.CLKPhase = SPI_PHASE_1EDGE;
  HAL_K1_SPI.Init.NSS = SPI_NSS_SOFT;
  HAL_K1_SPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  HAL_K1_SPI.Init.FirstBit = SPI_FIRSTBIT_MSB;
  HAL_K1_SPI.Init.TIMode = SPI_TIMODE_DISABLE;
  HAL_K1_SPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  HAL_K1_SPI.Init.CRCPolynomial = 7;
  HAL_K1_SPI.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  HAL_K1_SPI.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&HAL_K1_SPI) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

static void Bsp_K3_SPI_Init(void)
{

  /* SPI2 parameter configuration*/
  HAL_K3_SPI.Instance = SPI2;
  HAL_K3_SPI.Init.Mode = SPI_MODE_MASTER;
  HAL_K3_SPI.Init.Direction = SPI_DIRECTION_2LINES_RXONLY;
  HAL_K3_SPI.Init.DataSize = SPI_DATASIZE_4BIT;
  HAL_K3_SPI.Init.CLKPolarity = SPI_POLARITY_LOW;
  HAL_K3_SPI.Init.CLKPhase = SPI_PHASE_1EDGE;
  HAL_K3_SPI.Init.NSS = SPI_NSS_SOFT;
  HAL_K3_SPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  HAL_K3_SPI.Init.FirstBit = SPI_FIRSTBIT_MSB;
  HAL_K3_SPI.Init.TIMode = SPI_TIMODE_DISABLE;
  HAL_K3_SPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  HAL_K3_SPI.Init.CRCPolynomial = 7;
  HAL_K3_SPI.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  HAL_K3_SPI.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&HAL_K3_SPI) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM6 init function */
static void Bsp_BaseTime_Init(void)
{

  HAL_BaseTime_TIM.Instance = TIM6;
  HAL_BaseTime_TIM.Init.Prescaler = 47;
  HAL_BaseTime_TIM.Init.CounterMode = TIM_COUNTERMODE_UP;
  HAL_BaseTime_TIM.Init.Period = 999;
  HAL_BaseTime_TIM.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&HAL_BaseTime_TIM) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	__HAL_TIM_CLEAR_FLAG(&HAL_BaseTime_TIM, TIM_FLAG_UPDATE);
	__HAL_TIM_ENABLE_IT(&HAL_BaseTime_TIM, TIM_IT_UPDATE);
	__HAL_TIM_ENABLE(&HAL_BaseTime_TIM);
}

static void Bsp_DWIN_USART_Init(void)
{

  HAL_DWIN_USART.Instance = USART1;
  HAL_DWIN_USART.Init.BaudRate = 115200;
  HAL_DWIN_USART.Init.WordLength = UART_WORDLENGTH_8B;
  HAL_DWIN_USART.Init.StopBits = UART_STOPBITS_1;
  HAL_DWIN_USART.Init.Parity = UART_PARITY_NONE;
  HAL_DWIN_USART.Init.Mode = UART_MODE_TX_RX;
  HAL_DWIN_USART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  HAL_DWIN_USART.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_DWIN_USART.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  HAL_DWIN_USART.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&HAL_DWIN_USART) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	__HAL_UART_CLEAR_FLAG(&HAL_DWIN_USART, UART_FLAG_RXNE);
	__HAL_UART_ENABLE_IT(&HAL_DWIN_USART, UART_IT_RXNE);
	__HAL_UART_ENABLE(&HAL_DWIN_USART);
}

static void Bsp_RS485_USART_Init(void)
{

  HAL_RS485_USART.Instance = USART2;
  HAL_RS485_USART.Init.BaudRate = 9600;
  HAL_RS485_USART.Init.WordLength = UART_WORDLENGTH_8B;
  HAL_RS485_USART.Init.StopBits = UART_STOPBITS_1;
  HAL_RS485_USART.Init.Parity = UART_PARITY_NONE;
  HAL_RS485_USART.Init.Mode = UART_MODE_TX_RX;
  HAL_RS485_USART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  HAL_RS485_USART.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_RS485_USART.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  HAL_RS485_USART.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_RS485Ex_Init(&HAL_RS485_USART, UART_DE_POLARITY_HIGH, 0, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	__HAL_UART_CLEAR_FLAG(&HAL_RS485_USART, UART_FLAG_RXNE);
	__HAL_UART_ENABLE_IT(&HAL_RS485_USART, UART_IT_RXNE);
	__HAL_UART_ENABLE(&HAL_RS485_USART);
}

static void Bsp_WIFI_USART_Init(void)
{

  HAL_WIFI_USART.Instance = USART3;
  HAL_WIFI_USART.Init.BaudRate = 115200;
  HAL_WIFI_USART.Init.WordLength = UART_WORDLENGTH_8B;
  HAL_WIFI_USART.Init.StopBits = UART_STOPBITS_1;
  HAL_WIFI_USART.Init.Parity = UART_PARITY_NONE;
  HAL_WIFI_USART.Init.Mode = UART_MODE_TX_RX;
  HAL_WIFI_USART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  HAL_WIFI_USART.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_WIFI_USART.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  HAL_WIFI_USART.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&HAL_WIFI_USART) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	__HAL_UART_CLEAR_FLAG(&HAL_WIFI_USART, UART_FLAG_RXNE);
	__HAL_UART_ENABLE_IT(&HAL_WIFI_USART, UART_IT_RXNE);
	__HAL_UART_ENABLE(&HAL_WIFI_USART);
}

static void Bsp_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, EEPROM_SCL_Pin|EEPROM_SDA_Pin|HEATERROD2_Pin|HEATERROD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, WIFI_KEY_Pin|HEATERROD1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, FAN_HIGH_Pin|FAN_MID_Pin|FAN_LOW_Pin|MOTOR_AIR_Pin 
                          |MOTOR_WATER_Pin|K3_CS_Pin|K2_CS_Pin|K2_SCK_Pin 
                          |K2_MISO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(K1_CS_GPIO_Port, K1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : EEPROM_SCL_Pin EEPROM_SDA_Pin HEATERROD2_Pin HEATERROD3_Pin */
  GPIO_InitStruct.Pin = EEPROM_SCL_Pin|EEPROM_SDA_Pin|HEATERROD2_Pin|HEATERROD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : WIFI_STATE_Pin ALARM_HEAT_Pin ALARM_WATER_Pin */
  GPIO_InitStruct.Pin = WIFI_STATE_Pin|ALARM_HEAT_Pin|ALARM_WATER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : WIFI_KEY_Pin HEATERROD1_Pin */
  GPIO_InitStruct.Pin = WIFI_KEY_Pin|HEATERROD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : FAN_HIGH_Pin FAN_MID_Pin FAN_LOW_Pin MOTOR_AIR_Pin 
                           MOTOR_WATER_Pin K3_CS_Pin K2_CS_Pin K2_SCK_Pin 
                           K2_MISO_Pin */
  GPIO_InitStruct.Pin = FAN_HIGH_Pin|FAN_MID_Pin|FAN_LOW_Pin|MOTOR_AIR_Pin 
                          |MOTOR_WATER_Pin|K3_CS_Pin|K2_CS_Pin|K2_SCK_Pin 
                          |K2_MISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : K1_CS_Pin */
  GPIO_InitStruct.Pin = K1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(K1_CS_GPIO_Port, &GPIO_InitStruct);

}

void Bsp_USART_Send(UART_HandleTypeDef *huart ,uint8_t *pdata, uint16_t len)
{
	while(len--)
	{
		while( __HAL_UART_GET_FLAG(huart, UART_FLAG_TC) == RESET);
		huart->Instance->TDR = (*pdata++ & (uint8_t)0xFF);
	}
}

void Bsp_Params_Init(void)
{
  
}

void Bsp_Peripherals_Init(void)
{
  HAL_Init();
  Bsp_SystemClock_Init();
  Bsp_GPIO_Init();
  Bsp_BaseTime_Init();
  Bsp_DWIN_USART_Init();
  Bsp_WIFI_USART_Init();
  Bsp_RS485_USART_Init();
  Bsp_K1_SPI_Init();
  Bsp_K3_SPI_Init();
  Bsp_Params_Init();
}
